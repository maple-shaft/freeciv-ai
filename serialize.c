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

////// Unit Translation Stuff

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
