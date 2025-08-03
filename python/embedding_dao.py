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

EMBEDDING_MODEL = 'nomic-embed-text'
TEST_MODEL = 'qwen3:8b'


class OllamaEmbeddingFunction(EmbeddingFunction):

    def __init__(self, client: Client, embedding_model : str = EMBEDDING_MODEL):
        self.client = client
        self.embedding_model = embedding_model

    def __call__(self, input : str) -> Embeddings:
        embeddings = self.client.embed(model=self.embedding_model, input=input)['embeddings'][0]
        return embeddings
    
    def name(self) -> str:
        return "OllamaEmbeddingFunction"
    

class EmbeddingDAO:

    def __init__(self, path : str = ".embedding_db", embedding_model : str = EMBEDDING_MODEL):
        self.path = path
        self.embedding_model = embedding_model
        self.client = Client("http://localhost:11434")
        self.chroma_client = chromadb.PersistentClient(path=self.path)
        self.collection = self.chroma_client.get_or_create_collection(
                name=self.embedding_model,
                metadata={"hnsw:space": "cosine"},
                embedding_function=OllamaEmbeddingFunction(self.client, self.embedding_model))

    def get_top_docs(self, prompt : str) -> List[Document]:
        print(f"get_top_docs: {prompt}")
        embed_response = self.client.embed(model=self.embedding_model, input=prompt)
        if embed_response is None or not embed_response.__contains__("embeddings"):
            return []
        
        prompt_embeddings = embed_response["embeddings"]
        if prompt_embeddings is None:
            return []
        prompt_embedding = prompt_embeddings[0]
        print(f"Prompt Embedding: {prompt_embedding}")
        query_result : QueryResult = self.collection.query(query_embeddings=[prompt_embedding], query_texts=[prompt], n_results=3)
        if query_result["documents"] is None:
            print("Nothing found in get_top_docs")
            return []
        
        top_docs = query_result["documents"][0]
        return top_docs

    # Recreates the pickle file from ChromaDB
    def create_pickle_data(self):
        get_result = self.collection.get()
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