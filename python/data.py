### Module containing Pydantic models for JSON serialization and deserialization

from enum import Enum
from typing import List, Optional, Dict, Any
from pydantic import BaseModel

# Enums

class UnitActivity(Enum):
    DUMMY = 1

# World and Tiles

class Tile(BaseModel):
    id: int

# Players

class PlayerDiplomacy(BaseModel):
    war_target : Optional[str] = None

class PlayerStats(BaseModel):
    available_boats: Optional[int] = None
    boats: Optional[int] = None
    ocean_workers: Optional[int] = None
    passengers: Optional[int] = None
    workers: Optional[int] = None

class Player(BaseModel):
    last_num_continents: Optional[int] = None
    last_num_oceans: Optional[int] = None
    stats: Optional[PlayerStats] = None
    diplomacy: Optional[PlayerDiplomacy] = None
    tech_want: List[float] = []


class UnitType(BaseModel):
    id: int

class UnitOrder(BaseModel):
    id: int

class GoodsType(BaseModel):
    name: str

class Unit(BaseModel):
    #action: Optional[GenActionName]
    action_decision_want: Optional[str]
    action_decision_tile_TILE_index: Optional[int]
    activity: Optional[str]
    battlegroup: Optional[int]
    carrying: Optional[GoodsType]
    current_form_turn: Optional[int]
    fuel: Optional[int]
    goto_tile_TILE_index: Optional[int]
    homecity: int
    has_orders: Optional[bool]
    hp: Optional[int]
    id: int
    moves_left: Optional[int]
    moved: Optional[bool]
    owner_PLAYER_name: Optional[str]
    paradropped: Optional[bool]
    orders: List[UnitOrder]
    orders_index: Optional[int]
    orders_repeat: Optional[bool]
    stay: Optional[bool]
    tile_TILE_index: Optional[int]
    transporter_UNIT_id: Optional[int]
    name_UNIT_TYPE_name: Optional[str]
    veteran: Optional[int]

 

