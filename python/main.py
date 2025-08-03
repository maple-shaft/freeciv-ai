import os
import uuid
import pickle
from typing import Sequence, Dict, List
import chromadb
from chromadb.api.types import GetResult, EmbeddingFunction, Embeddings, QueryResult, Document
import requests
from numpy import dot
from numpy.linalg import norm
from bs4 import BeautifulSoup
from bs4.element import Tag
from ollama import Client, AsyncClient
from ollama._types import (ResponseError, Message)

### Freeciv Agent Initialization code

EMBEDDING_MODEL = 'nomic-embed-text'
TEST_MODEL = 'qwen3:8b'
FREECIV_WIKI_BASE = "https://freeciv.fandom.com"
FREECIV_WIKI_CONTEXTROOT = "/wiki/"
FREECIV_WIKI_ALLPAGES = "/wiki/Special:AllPages"
FREECIV_WIKI_EDIT = "?veaction=edit"
FREECIV_WIKI_CONTENT_ELEMENT = ("div", "page-content")

# Prepend all with /wiki/ to get to the meat and potatoes
MANUAL_URLS = ["Terrain","Cities","Economy","Units","Combat","Diplomacy",
               "Government","Technology","Wonders","Buildings",
               "Manual","Game_Manual","Classic","Civ2civ3","Rulesets/3.2",
               "FAQ","Introduction_to_Freeciv"
]

MANUAL_URLS2 = [
    "Happiness","Diplomatic_actions","Nations",
    "Global_warming","Nuclear_winter"
]

MANUAL_UNITS_URLS = ["Settlers","Workers","Warriors",
                     "Horsemen","Archers","Phalanx"
                     "Trireme","Chariot","Catapult",
                     "Legion","Diplomat","Explorer",
                     "Caravan","Pikemen","Knights",
                     "Caravel","Galleon","Frigate",
                     "Ironclad","Musketeers","Dragoons",
                     "Cannon","Engineers","Transport",
                     "Destroyer","Riflemen","Cavalry",
                     "Alpine_Troops","Freight","Spy",
                     "Cruiser","Battleship","Submarine",
                     "Marines","Partisan","Artillery",
                     "Fighter","AEGIS_Cruiser","Carrier",
                     "Armor","Mech._Inf.","Howitzer",
                     "Paratroopers","Helicopter","Bomber",
                     "AWACS","Nuclear","Cruise_Missle",
                     "Stealth_Bomber","Stealth_Fighter", "Barbarian",
                     "Barbarian_Leader"]

client = Client("http://localhost:11434")

class OllamaEmbeddingFunction(EmbeddingFunction):

    def __call__(self, input : str) -> Embeddings:
        embeddings = client.embed(model=EMBEDDING_MODEL, input=input)['embeddings'][0]
        return embeddings
    
    def name(self) -> str:
        return "OllamaEmbeddingFunction"



chroma_client = chromadb.PersistentClient(path=".embedding_db")
collection = chroma_client.get_or_create_collection(
        name=EMBEDDING_MODEL,
        metadata={"hnsw:space": "cosine"},
        embedding_function=OllamaEmbeddingFunction())

# Use cosine similarity to rank documents
def cosine_similarity(a, b):
    return dot(a, b) / (norm(a) * norm(b))

def get_top_docs(prompt : str):
    prompt_embedding = client.embed(model=EMBEDDING_MODEL, input=prompt)["embeddings"][0]
    query_result : QueryResult = collection.query(query_embeddings=[prompt_embedding], query_texts=[prompt], n_results=3)
    if query_result["documents"] is None:
        print("Nothing found in get_top_docs")
        return []
    
    top_docs = query_result["documents"][0]
    #for td in top_docs:
    #    print(f"Top Doc {td}")
    return query_result["documents"][0]

def pull_embedding_model():
    try:
        client.pull(model=EMBEDDING_MODEL)
    except Exception as e:
        print(e)

def get_url_text_abridged(urls : Sequence[str] = MANUAL_URLS) -> Dict[str, str]:
    ret = {}
    for man in urls:
        url = FREECIV_WIKI_BASE + FREECIV_WIKI_CONTEXTROOT + man
        response = requests.get(url)
        try:
            response.raise_for_status()
        except Exception as e:
            print(f"Url returned a problem: {url}")
            continue
        soup = BeautifulSoup(response.content, "html.parser")
        elem = FREECIV_WIKI_CONTENT_ELEMENT[0]
        att = FREECIV_WIKI_CONTENT_ELEMENT[1]
        for d in soup.find_all(name=elem, attrs={"class": att}, recursive=True):
            text = d.get_text(separator=' ', strip=True)
            ret[url] = text
    return ret

def get_url_text(url : str) -> list[str]:
    response = requests.get(url)
    response.raise_for_status()
    bs = BeautifulSoup(response.content, "html.parser")
    links = []
    for a_tag in bs.find_all(name="a"):
        #print(f"a_tag: {a_tag}")
        href = a_tag.get("href") # type: ignore
        #print(f"{href}")
        if not href:
            continue
        if not href.startswith("/wiki"): # type: ignore
            continue
        if not href.startswith(("/wiki/Forum", "/wiki/Category", "/wiki/User", "/wiki/Screenshots")) and not href.endswith((".svg", ".png")): # type: ignore
            links.append(href)
            print(href)

    texts : list[str] = []
    for link in links:
        ur = FREECIV_WIKI_BASE + link + FREECIV_WIKI_EDIT
        res = requests.get(ur)
        try:
            res.raise_for_status()
        except Exception as e:
            continue
        soup = BeautifulSoup(res.content, "html.parser")
        att = "mw-body-content"
        for d in soup.find_all(name="div", attrs={"class": att}, recursive=True):
            text = d.get_text(separator=' ', strip=True)
            texts.append(text)
    return texts

def create_freeciv_agent():
    resp = client.create(model="freeciv_agent",
                         from_=TEST_MODEL,
                         system='You are a skilled AI player for the game Freeciv.',
                         stream=False)
    print(resp.status)
    print(f"Create Response: {resp}")

def embed_all(texts : list[str]) -> Dict[str, Sequence[float]]:
    ret = {}
    if not texts:
        raise Exception("No texts to embed")
    
    for text in texts:
        embed_response = client.embed(model=EMBEDDING_MODEL, input=text)
        embeddings = embed_response.embeddings[0]
        ret[text] = embeddings
        id = uuid.uuid4()
        collection.add(ids=[repr(id)], documents=[text], embeddings=[embeddings])
    
    return ret

def get_pickle_texts() -> list[str]:
    with open("texts.pickle", "rb") as f:
        return pickle.load(f)

# Helper function that should create a new pickle file with documents from the database
def setup_db():
    texts : List[Document] | None = collection.get()["documents"]
    if texts is None:
        raise Exception("No data")
    print(f"Len texts: {texts.__len__()}")
    with open("texts.pickle", "wb") as f:
        pickle.dump(texts, file=f)

#setup_db()  # UNCOMMENT IF YOU ACCIDENTALLY MESSED UP THE EMBEDDING FUNC AND HAVE TO RECREATE THE DB.

# Get web data
#texts = get_pickle_texts()                   # UNCOMMENT IF YOU WANT TO RECOVER ;)
#texts = get_url_text(FREECIV_WIKI_BASE, )    # UNCOMMENT IF YOU WANT TO CRAWL :)
#texts = get_url_text_abridged()               # UNCOMMENT IF YOU WANT TO RUN ;)

def add_urls_abridged(urls : Sequence[Sequence[str]]= MANUAL_URLS):
    idcounter : int = 100
    for url_group in urls:
        texts = get_url_text_abridged(urls=url_group)
        for te in texts.items():
            idcounter = idcounter + 1
            idstr : str = idcounter.__str__()
            url = te[0]
            text = te[1]
            collection.add(ids=[idstr], documents=[text], metadatas=[{"sources": url}])

def create_pickle_data():
    get_result = collection.get()
    if not get_result:
        raise Exception("No get result found")
    metadatas = get_result["metadatas"]
    if metadatas is None:
        raise Exception("No metadatas found")
    documents = get_result["documents"]
    if documents is None:
        raise Exception("No documents")
    ret = {}
    for i, document in enumerate(documents,0):
        metadata = metadatas[i]
        if metadata is None:
            raise Exception("Metadata is None")
        source = metadata["sources"]
        if not source:
            raise Exception("Source not found")
        ret[source] = document

    with open("texts.pickle", "wb") as f:
        pickle.dump(ret, f)

#add_urls_abridged([MANUAL_URLS2, MANUAL_UNITS_URLS])
#create_pickle_data() # UNCOMMENT TO RECREATE THE PICKLE FILE FROM CHROMADB
input_prompt = input(" > ")
top_docs = get_top_docs(input_prompt)

#print(top_docs)

prompt = f"""Answer the question based on the context:

Context:
{top_docs}

Question:
{input_prompt} 
"""

response = client.chat(model=TEST_MODEL, messages=[{'role': 'user', 'content': prompt}])
print(response['message']['content'])


