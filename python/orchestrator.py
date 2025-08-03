import uvicorn
from fastapi import FastAPI, Request
from typing import List, Optional, Dict, Any
from pydantic import BaseModel
from ollama import Client as OllamaClient
from embedding_dao import EmbeddingDAO

CHAT_MODEL = 'qwen3:8b'
EMBEDDING_MODEL = 'nomic-embed-text'

SYSTEM_GENERATE = """
    You are a challenging and enthusiastic AI player in the game Freeciv. Your role is to interpret the 
    current game state and provided context so that you can decide on the next actions, wants, and needs 
    for the next player turn.
"""

SYSTEM_CHAT = """
    You are a challenging and enthusiastic AI "advisor" in the game Freeciv. Your role is to interpret the
    current game state and provided context so that you can decide how to best respond to other players
    in the game. You are conversational, competitive and enthusiastic.
"""

# Set up Ollama and ChromaDB clients
ollama = OllamaClient()

# Define FastAPI app
app = FastAPI()

# Get Embedding DAO
dao = EmbeddingDAO()

# Request and response schemas
class AgentContext(BaseModel):
    state: Optional[Dict[str, Any]] = None
    choices: Optional[List[str]] = None

class AgentRequest(BaseModel):
    prompt: str
    context: Optional[AgentContext] = None

class AgentResponse(BaseModel):
    result: str

@app.post("/agent/chat", response_model=AgentResponse)
async def agent_chat(req: AgentRequest):
    print(f"Request is {req}")
    top_docs = dao.get_top_docs(req.prompt)

    prompt = f"""Answer the question based on the context:
        Context:
        {top_docs}
        
        Question:
        {req.prompt}
    """
    response = ollama.chat(model=CHAT_MODEL, messages=[{'role': 'user', 'content': prompt}])
    result = response['message']['content']
    print(result)

    return AgentResponse(
        result=result
    )

@app.post("/agent/generate", response_model=AgentResponse)
async def agent_generate(req: AgentRequest):
    top_docs = dao.get_top_docs(req.prompt)

    prompt = f"""
        You are an auto-pilot for the open-source game FreeCiv. You are a bit more sophisticated than a simple AI however and possess the 
        ability to evaluate the current Game State of a Freeciv game. The Game State below is defined in JSON format. After interpreting the 
        Game State YOU MUST GENERATE a modified version of the Game State JSON object in the same format.
        
        Game State:
        {req.prompt}

        Appendix:
        {top_docs}
    """
    response = ollama.generate(model=CHAT_MODEL, prompt=prompt, format=AgentRequest.model_json_schema())
    result = response.response
    print(result)

    return AgentResponse(
        result=result
    )

if __name__ == '__main__':
    uvicorn.run(app=app, port=8001)