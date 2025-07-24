#include "fc_types.h"
#include "name_translation.h"
#include "specialist.h"
#include "tile.h"
#include "traderoutes.h"
#include "workertask.h"
#include "worklist.h"
#include "base.h"
#include "extras.h"
#include "road.h"
#include "terrain.h"
#include "unitlist.h"
#include "world_object.h"
#include "advdata.h"
#include "cm.h"
#include "unittype.h"
#include "diplhand.h"
#include "maple_ai_data.h"
#include "maple_ai_settler.h"
#include <jansson.h>
#include <stdlib.h>
#include <string.h>

#include "serialize.h"

// Helper function to serialize a string (handles strdup)
void serialize_string(const char *str, json_t *obj, const char *key) {
    json_object_set_new(obj, key, json_string(strdup(str)));
}

// Helper function to serialize an int
void serialize_int(int int_val, json_t *obj, const char *key) {
    json_object_set_new(obj, key, json_integer(int_val));
}

// Helper function to serialize a float
void serialize_float(float val, json_t *obj, const char *key) {
    json_object_set_new(obj, key, json_real(val));
}

void serialize_bool(bool val, json_t *obj, const char *key) {
    json_object_set_new(obj, key, (val) ? json_true() : json_false());
}

void serialize_name(const struct name_translation *val, json_t *obj, const char *key) {
    char *name_str = name_translation_get(val);
    json_object_set_new(obj, key, json_string(strdup(name_str)));
}

////// Player Translation Stuff

json_t *serialize_player(const struct player *player) {
    if (!player) return json_null();
    json_t *ret = json_object();

    //player->ai_common // struct player_ai... what is this?
    json_object_set(ret, "cities", serialize_city_list(player->cities));

    //TODO: Other Player Fields
    return ret;
}

////// End Player Translation Stuff

////// AI Player Translation Stuff

json_t *serialize_ai_plr(const struct ai_plr *data) {
    if (!data) return json_null();
    json_t *ret = json_object();

    serialize_int(data->last_num_continents, ret, "last_num_continents");
    serialize_int(data->last_num_oceans, ret, "last_num_oceans");

    // Stats
    json_t *stats = json_object();
    json_object_set(ret, "stats", stats);
    serialize_int(data->stats.available_boats, stats, "available_boats");
    serialize_int(data->stats.boats, stats, "boats");
    serialize_int(data->stats.ocean_workers, stats, "ocean_workers");
    serialize_int(data->stats.passengers, stats, "passengers");
    serialize_int(data->stats.workers, stats, "workers");
    
    // Tech Wants
    json_t *tech_want = json_array();
    json_object_set(ret, "tech_want", tech_want);
    for (int i = 0; i < ARRAY_SIZE(data->tech_want); i++)
    {
        json_array_append(tech_want, json_real(data->tech_want[i]));
    }

    // Diplomacy
    json_t *diplomacy = json_object();
    json_object_set(ret, "diplomacy", diplomacy);
    serialize_string(data->diplomacy.war_target->name, diplomacy, "war_target");
    
    return ret;
}

////// End AI Player Translation Stuff

////// CM Translation Stuff

json_t *serialize_cm_parameter(const struct cm_parameter *cm_parameter) {
    if (!cm_parameter) return json_null();
    json_t *ret = json_object();

    serialize_int(cm_parameter->happy_factor, ret, "happy_factor");
    serialize_bool(cm_parameter->allow_disorder, ret, "allow_disorder");
    serialize_bool(cm_parameter->allow_specialists, ret, "allow_specialists");
    serialize_bool(cm_parameter->max_growth, ret, "max_growth");
    serialize_bool(cm_parameter->require_happy, ret, "require_happy");
    return ret;
}

////// End CM Translation Stuff

////// Advisor Translation Stuff

json_t *serialize_adv_area_info(const struct adv_area_info *aai) {
    if (!aai) return json_null();
    json_t *ret = json_object();

    serialize_int(aai->size, ret, "size");
    serialize_bool(aai->size, ret, "threat");
    return ret;
}

json_t *serialize_adv_want(const struct adv_want *adv_want) {
    if (!adv_want) return json_null();
    json_t *ret = json_object();

    
}

json_t *serialize_adv_data(const struct adv_data *adv_data) {
    if (!adv_data) return json_null();
    json_t *ret = json_object();

    serialize_int(adv_data->angry_priority, ret, "angry_priority");
    serialize_int(adv_data->food_priority, ret, "food_priority");
    serialize_int(adv_data->gold_priority, ret, "gold_priority");
    serialize_int(adv_data->happy_priority, ret, "happy_priority");
    serialize_int(adv_data->infra_priority, ret, "infra_priority");
    serialize_int(adv_data->luxury_priority, ret, "luxury_priority");
    serialize_int(adv_data->num_continents, ret, "num_continents");
    serialize_int(adv_data->num_oceans, ret, "num_oceans");
    serialize_int(adv_data->pollution_priority, ret, "pollution_priority");
    serialize_int(adv_data->science_priority, ret, "science_priority");
    serialize_int(adv_data->shield_priority, ret, "shield_priority");
    serialize_int(adv_data->unhappy_priority, ret, "unhappy_priority");
    serialize_int(adv_data->wonder_city, ret, "wonder_city");

    json_object_set(ret, "continents", serialize_adv_area_info(&adv_data->continents));

    serialize_bool(adv_data->celebrate, ret, "celebrate");
    serialize_bool(adv_data->wants_science, ret, "wants_science");

    //TODO: Other fields...
    return ret;
}

////// End Advisor Translation Stuff

////// City Translation Stuff

json_t *serialize_goods_type(const struct goods_type *goods_type) {
    if (!goods_type) return json_null();

    json_t *ret = json_object();
    serialize_name(&(goods_type->name), ret, "name");
    return ret;
}

json_t *serialize_trade_route(const struct trade_route *trade_route) {
    if (!trade_route) return json_null();
    json_t *ret = json_object();
    //trade_route_na
    serialize_int(trade_route->partner, ret, "partner");
    serialize_int(trade_route->value, ret, "value");

    serialize_string(route_direction_name(trade_route->dir), ret, "dir");
    json_object_set(ret, "goods", serialize_goods_type(trade_route->goods));
    return ret;
}

json_t *serialize_trade_route_list(const struct trade_route_list *trl) {
    if (!trl) return json_null();
    json_t *ret = json_array();
    
    struct trade_route *ptraderoute;
    trade_route_list_iterate(trl, ptraderoute) {
        json_array_append(ret, serialize_trade_route(ptraderoute));
    } trade_route_list_iterate_end;
    return ret;
}

json_t *serialize_tile(const struct tile *tile) {
    if (!tile) return json_null();
    json_t *ret = json_object();
    
    // Integers
    serialize_int(tile->altitude, ret, "altitude");
    serialize_int(tile->continent, ret, "continent");
    serialize_int(tile->index, ret, "index");
    serialize_int(tile->infra_turns, ret, "infra_turns");
    
    if (tile->owner) {
        serialize_string(tile->owner->name, ret, "owner_PLAYER_name");
    }
    if (tile->worked) {
        serialize_string(tile->worked->name, ret, "worked_CITY_name");
    }
    if (tile->terrain) {
        serialize_name(&(tile->terrain->name), ret, "terrain_TERRAIN_name");
    }
    
    //TODO: Unit list?
    return ret;
}

json_t *serialize_city(const struct city *city) {
    if (!city) return json_null();
    json_t *ret = json_object();

    //TODO add city attributes
    // Arrays
    json_t *abs_bonus = json_array();
    for (int i = 0; i < ARRAY_SIZE(city->abs_bonus); i++)
    {
        json_array_append(abs_bonus, json_integer(city->abs_bonus[i]));
    }
    json_object_set(ret, "abs_bonus", abs_bonus);


    json_t *citizen_base = json_array();
    for (int i = 0; i < ARRAY_SIZE(city->citizen_base); i++)
    {
        json_array_append(citizen_base, json_integer(city->citizen_base[i]));
    }
    json_object_set(ret, "citizen_base", citizen_base);

    // Enums
    serialize_string(city_acquire_type_name(city->acquire_t), ret, "acquire_t");
    serialize_string(capital_type_name(city->capital), ret, "capital");
    
    // Inner Structs
    json_t *client = json_object();
    json_object_set(ret, "client", client);
    serialize_int(city->client.buy_cost, client, "buy_cost");
    serialize_int(city->client.culture, client, "culture");
    serialize_int(city->client.walls, client, "walls");
    serialize_bool(city->client.happy, client, "happy");
    serialize_bool(city->client.occupied, client, "occupied");
    serialize_bool(city->client.unhappy, client, "unhappy");

    // Strings
    serialize_string(city->name, ret, "name");
    serialize_string(city->original->name, ret, "original_PLAYER_name");
    serialize_string(city->owner->name, ret, "owner_PLAYER_name");
    
    // Integers
    serialize_int(city->airlift, ret, "airlift");
    serialize_int(city->anarchy, ret, "anarchy");
    serialize_int(city->before_change_shields, ret, "before_change_shields");
    //city->bonus[6]
    serialize_int(city->caravan_shields, ret, "caravan_shields");
    serialize_int(city->city_radius_sq, ret, "city_radius_sq");
    //city->counter_values
    serialize_int(city->disbanded_shields, ret, "disbanded_shields");
    serialize_int(city->food_stock, ret, "food_stock");
    serialize_int(city->history, ret, "history");
    serialize_int(city->id, ret, "id");
    serialize_int(city->illness_trade, ret, "illness_trade");
    serialize_int(city->last_turns_shield_surplus, ret, "last_turns_shield_surplus");
    serialize_int(city->pollution, ret, "pollution");
    serialize_int(city->rapture, ret, "rapture");
    serialize_int(city->shield_stock, ret, "shield_stock");
    serialize_int(city->size, ret, "size");
    serialize_int(city->steal, ret, "steal");
    serialize_int(city->turn_founded, ret, "turn_founded");
    serialize_int(city->turn_last_built, ret, "turn_last_built");
    serialize_int(city->turn_plague, ret, "turn_plague");

    // Bools
    serialize_bool(city->did_buy, ret, "did_buy");
    serialize_bool(city->did_sell, ret, "did_sell");
    serialize_bool(city->was_happy, ret, "was_happy");
    serialize_bool(city->had_famine, ret, "had_famine");

    // Other structs
    json_object_set(ret, "cm_parameter", serialize_cm_parameter(city->cm_parameter));
    json_object_set(ret, "routes", serialize_trade_route_list(city->routes));
    // ??? city->task_reqs
    json_object_set(ret, "tile", serialize_tile(city->tile));
    //city->units_supported

    return ret;
}

json_t *serialize_city_list(const struct city_list *city_list) {
    if (!city_list) return json_null();
    json_t *ret = json_array();
    
    struct city *pcity;
    city_list_iterate(city_list, pcity) {
        json_array_append(ret, serialize_city(pcity));
    } city_list_iterate_end;
    return ret;
}

////// End City Translation Stuff

////// Unit Translation Stuff

json_t *serialize_unit(const struct unit *unit) {
    if (!unit) return json_null();

    json_t *ret = json_object();
    serialize_string(gen_action_name(unit->action), ret, "action");

    if (unit->action_decision_tile) {
        serialize_int(unit->action_decision_tile->index, ret, "action_decision_tile_TILE_index");
    }
    if (unit->carrying) {
        json_object_set(ret, "carrying", serialize_goods_type(&(unit->carrying)));
    }
    serialize_string(action_decision_name(unit->action_decision_want), ret, "action_decision_want");
    serialize_string(unit_activity_name(unit->activity), ret, "activity");
    serialize_int(unit->battlegroup, ret, "battlegroup");
    serialize_int(unit->current_form_turn, ret, "current_form_turn");
    serialize_int(unit->fuel, ret, "fuel");
    

    return ret;
}

json_t *serialize_unit_list(const struct unit_list *unit_list) {
    if (!unit_list) return json_null();
    json_t *ret = json_array();

    return ret;
}

// Helper function to serialize a struct specialist
json_t *serialize_specialist(const struct specialist *spec_type) {
    if (!spec_type) return json_null();

    json_t *spec_obj = json_object();
    char *name_str = name_translation_get(&spec_type->name);

    serialize_name(&(spec_type->name), spec_obj, "name");
    // Serialize specialist fields (assuming fields like name, cost, etc.)
    serialize_string(name_str, spec_obj, "name");
    serialize_int(spec_type->item_number, spec_obj, "item_number");
    // Other fields...
    return spec_obj;
}

json_t *serialize_unit_type(const struct unit_type *unit_type) {
    if (!unit_type) return json_null();

    json_t *ret_obj =  json_object();
    serialize_int(unit_type->item_number, ret_obj, "item_number");
    serialize_int(unit_type->attack_strength, ret_obj, "attack_strength");
    serialize_int(unit_type->build_cost, ret_obj, "build_cost");
    serialize_int(unit_type->bombard_rate, ret_obj, "bombard_rate");
    serialize_int(unit_type->city_size, ret_obj, "city_size");
    serialize_int(unit_type->city_slots, ret_obj, "city_slots");
    serialize_int(unit_type->convert_time, ret_obj, "convert_time");
    serialize_int(unit_type->defense_strength, ret_obj, "defense_strength");
    serialize_int(unit_type->firepower, ret_obj, "firepower");
    serialize_int(unit_type->fuel, ret_obj, "fuel");
    serialize_int(unit_type->happy_cost, ret_obj, "happy_cost");
    serialize_int(unit_type->hp, ret_obj, "hp");
    serialize_int(unit_type->move_rate, ret_obj, "move_rate");
    serialize_int(unit_type->paratroopers_range, ret_obj, "paratroopers_range");
    serialize_int(unit_type->pop_cost, ret_obj, "pop_cost");
    serialize_int(unit_type->transport_capacity, ret_obj, "transport_capacity");
    serialize_int(unit_type->unknown_move_cost, ret_obj, "unknown_move_cost");
    serialize_int(unit_type->upkeep, ret_obj, "upkeep");
    serialize_int(unit_type->vision_radius_sq, ret_obj, "vision_radius_sq");
    
    serialize_name(&(unit_type->name), ret_obj, "name");
    
    json_t *specialist = serialize_specialist(&(unit_type->spec_type));
    if (specialist != json_null())
    {
        json_object_set(ret_obj, "spec_type", specialist);
    }

    json_t *converted_to = serialize_unit_type(&(unit_type->converted_to));
    if (converted_to != json_null())
    {
        json_object_set(ret_obj, "converted_to", converted_to);
    }

    json_t *obsoleted_by = serialize_unit_type(&(unit_type->obsoleted_by));
    if (obsoleted_by != json_null())
    {
        json_object_set(ret_obj, "obsoleted_by", obsoleted_by);
    }

    
    //TODO other fields

    return ret_obj;
}

////// End Unit Translation Stuff

////// World Translation Stuff

json_t *serialize_world(const struct world *world) {
    if (!world) return json_null();
    json_t *ret_obj = json_object();
    
    //TODO: Add world serialization
    return ret_obj;
}

////// End World Translation Stuff

// Helper function to serialize a struct combat_bonus_list
json_t *serialize_combat_bonus_list(const struct combat_bonus_list *bonuses, json_t *obj) {
    if (!bonuses) return json_null();

    json_t *bonuses_obj = json_object();

    // Serialize combat_bonus_list fields (assuming fields like bonuses, etc.)
    // Example: serialize a list of bonuses
    // json_t *bonus_list = json_array();
    // for (int i = 0; i < bonuses->num_bonuses; i++) {
    //     json_array_append_new(bonus_list, json_integer(bonuses->bonuses[i]));
    // }
    // json_object_set_new(bonuses_obj, "bonuses", bonus_list);

    // Add to main object
    json_object_set(obj, "bonuses", bonuses_obj);
    return bonuses_obj;
}

// Helper function to serialize a struct unit_class
json_t *serialize_unit_class(const struct unit_class *uclass, json_t *obj) {
    if (!uclass) return json_null();

    json_t *uclass_obj = json_object();

    // Serialize unit_class fields (assuming fields like name, etc.)
    serialize_name(&(uclass->name), uclass_obj, "name");

    return uclass_obj;
}
