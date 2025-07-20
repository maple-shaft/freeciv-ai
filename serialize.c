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
#include <jansson.h>
#include <stdlib.h>
#include <string.h>


// Helper function to serialize a string (handles strdup)
void serialize_string(const char *str, json_t *obj, const char *key) {
    json_object_set_new(obj, key, json_string(strdup(str)));
}

// Helper function to serialize an int
void serialize_int(int int_val, json_t *obj, const char *key) {
    json_object_set_new(obj, key, json_integer(int_val));
}

// Helper function to serialize a bitvector (convert to integer)
//void serialize_bitvector(bv_unit_type_flags flags, json_t *obj, const char *key) {
//    json_object_set_new(obj, key, ((long)flags));
//}

////// Advisor Translation Stuff

json_t *serialize_adv_data(const struct adv_data *adv_data) {
    if (!adv_data) return JSON_NULL;
    json_t *ret = json_object();

    serialize_int(&adv_data->angry_priority, ret, "angry_priority");
    serialize_int(&adv_data->food_priority, ret, "food_priority");
    serialize_int(&adv_data->gold_priority, ret, "gold_priority");
    serialize_int(&adv_data->happy_priority, ret, "happy_priority");
    serialize_int(&adv_data->infra_priority, ret, "infra_priority");
    serialize_int(&adv_data->luxury_priority, ret, "luxury_priority");
    serialize_int(&adv_data->num_continents, ret, "num_continents");
    serialize_int(&adv_data->num_oceans, ret, "num_oceans");
    serialize_int(&adv_data->pollution_priority, ret, "pollution_priority");
    serialize_int(&adv_data->science_priority, ret, "science_priority");
    serialize_int(&adv_data->shield_priority, ret, "shield_priority");
    serialize_int(&adv_data->unhappy_priority, ret, "unhappy_priority");
    serialize_int(&adv_data->wonder_city, ret, "wonder_city");

    json_object_set(ret, "celebrate", json_boolean(&adv_data->celebrate));
    json_object_set(ret, "wants_science", json_boolean(&adv_data->wants_science));

    //TODO: Other fields...
    return ret;
}

////// End Advisor Translation Stuff


////// Unit Translation Stuff

// Helper function to serialize a struct specialist
json_t *serialize_specialist(const struct specialist *spec_type) {
    if (!spec_type) return JSON_NULL;

    json_t *spec_obj = json_object();
    char *name_str = name_translation_get(&spec_type->name);

    // Serialize specialist fields (assuming fields like name, cost, etc.)
    serialize_string(name_str, spec_obj, "name");
    serialize_int(spec_type->item_number, spec_obj, "item_number");
    // Other fields...
    return spec_obj;
}

json_t *serialize_unit_type(const struct unit_type *unit_type) {
    if (!unit_type) return NULL;

    json_t *ret_obj =  json_object();
    serialize_int(&unit_type->item_number, ret_obj, "item_number");
    serialize_int(&unit_type->attack_strength, ret_obj, "attack_strength");
    serialize_int(&unit_type->build_cost, ret_obj, "build_cost");
    serialize_int(&unit_type->bombard_rate, ret_obj, "bombard_rate");
    serialize_int(&unit_type->city_size, ret_obj, "city_size");
    serialize_int(&unit_type->city_slots, ret_obj, "city_slots");
    serialize_int(&unit_type->convert_time, ret_obj, "convert_time");
    serialize_int(&unit_type->defense_strength, ret_obj, "defense_strength");
    serialize_int(&unit_type->firepower, ret_obj, "firepower");
    serialize_int(&unit_type->fuel, ret_obj, "fuel");
    serialize_int(&unit_type->happy_cost, ret_obj, "happy_cost");
    serialize_int(&unit_type->hp, ret_obj, "hp");
    serialize_int(&unit_type->move_rate, ret_obj, "move_rate");
    serialize_int(&unit_type->paratroopers_range, ret_obj, "paratroopers_range");
    serialize_int(&unit_type->pop_cost, ret_obj, "pop_cost");
    serialize_int(&unit_type->transport_capacity, ret_obj, "transport_capacity");
    serialize_int(&unit_type->unknown_move_cost, ret_obj, "unknown_move_cost");
    serialize_int(&unit_type->upkeep, ret_obj, "upkeep");
    serialize_int(&unit_type->vision_radius_sq, ret_obj, "vision_radius_sq");
    
    char *name_str = name_translation_get(&unit_type->name);
    if (name_str)
    {
        serialize_string(&name_str, ret_obj, "name");
    }
    
    json_t * specialist = serialize_specialist(&unit_type->spec_type);
    if (specialist)
    {
        json_object_set(ret_obj, "spec_type", &specialist);
    }

    json_t *converted_to = serialize_unit_type(&unit_type->converted_to);
    if (converted_to)
    {
        json_object_set(ret_obj, "converted_to", &converted_to);
    }

    json_t *obsoleted_by = serialize_unit_type(&unit_type->obsoleted_by);
    if (obsoleted_by)
    {
        json_object_set(ret_obj, "obsoleted_by", &obsoleted_by);
    }

    
    //TODO other fields

    return ret_obj;
}

////// End Unit Translation Stuff

////// World Translation Stuff

json_t *serialize_world(const struct world *world) {
    if (!world) return NULL;
    json_t *ret_obj = json_object();
    
    //TODO: Add world serialization
    return ret_obj;
}

////// End World Translation Stuff



// Helper function to serialize a struct requirement_vector
json_t *serialize_requirement_vector(const struct requirement_vector *build_reqs, json_t *obj) {
    if (!build_reqs) return NULL;

    json_t *reqs_obj = json_object();

    // Serialize requirement_vector fields (assuming fields like requirements, etc.)
    // Example: serialize a list of requirements
    // json_t *requirements = json_array();
    // for (int i = 0; i < build_reqs->num_requirements; i++) {
    //     json_array_append_new(requirements, json_integer(build_reqs->requirements[i]));
    // }
    // json_object_set_new(reqs_obj, "requirements", requirements);

    // Add to main object
    json_object_set(obj, "build_reqs", reqs_obj);
    return reqs_obj;
}

// Helper function to serialize a struct combat_bonus_list
json_t *serialize_combat_bonus_list(const struct combat_bonus_list *bonuses, json_t *obj) {
    if (!bonuses) return NULL;

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
    if (!uclass) return NULL;

    json_t *uclass_obj = json_object();

    // Serialize unit_class fields (assuming fields like name, etc.)
    serialize_string(&uclass->name, uclass_obj, "name");

    // Add to main object
    json_object_set(obj, "uclass", uclass_obj);
    return uclass_obj;
}

// Main serialization function for unit_type
json_t *serialize_unit_type_two(const struct unit_type *unit) {
    if (!unit) return NULL;

    json_t *unit_obj = json_object();

    // Serialize unit_type fields
    json_object_set_new(unit_obj, "item_number", (long)unit->item_number);
    //serialize_string(unit->name, unit_obj, "name");
    json_object_set_new(unit_obj, "build_cost", json_integer((long)unit->build_cost));
    json_object_set_new(unit_obj, "pop_cost", json_integer((long)unit->pop_cost));
    json_object_set, "spec_type", serialize_specialist(unit->spec_type);
    json_object_set_new(unit_obj, "attack_strength", json_integer((long)unit->attack_strength));
    json_object_set_new(unit_obj, "defense_strength", json_integer((long)unit->defense_strength));
    json_object_set_new(unit_obj, "move_rate", json_integer((long)unit->move_rate));
    json_object_set_new(unit_obj, "unknown_move_cost", json_integer((long)unit->unknown_move_cost));
    //json_object_set, "build_reqs", serialize_requirement_vector(unit->build_reqs, unit_obj));
    json_object_set_new(unit_obj, "vision_radius_sq", json_integer((long)unit->vision_radius_sq));
    json_object_set_new(unit_obj, "transport_capacity", json_integer((long)unit->transport_capacity));
    json_object_set_new(unit_obj, "hp", json_integer((long)unit->hp));
    json_object_set_new(unit_obj, "firepower", json_integer((long)unit->firepower));
    json_object_set, "bonuses", serialize_combat_bonus_list(unit->bonuses, unit_obj);
    json_object_set_new(unit_obj, "obsoleted_by", JSON_NULL); // U_NOT_OBSOLETED is NULL
    json_object_set_new(unit_obj, "converted_to", JSON_NULL); // U_NOT_OBSOLETED is NULL
    json_object_set_new(unit_obj, "convert_time", json_integer((long)unit->convert_time));
    json_object_set_new(unit_obj, "fuel", json_integer((long)unit->fuel));
    serialize_bitvector(unit->flags, unit_obj, "flags");
    serialize_bitvector(unit->roles, unit_obj, "roles");
    json_object_set_new(unit_obj, "happy_cost", json_integer((long)unit->happy_cost));
    // Serialize upkeep array (assuming O_LAST is defined)
    // json_t *upkeep_array = json_array();
    // for (int i = 0; i < O_LAST; i++) {
    //     json_array_append_new(upkeep_array, json_integer(unit->upkeep[i]));
    // }
    // json_object_set_new(unit_obj, "upkeep", upkeep_array);
    json_object_set_new(unit_obj, "ais", JSON_NULL); // Void pointer array, assume NULL for simplicity
    json_object_set_new(unit_obj, "unit_class", JSON_NULL); // Assume NULL for simplicity
    json_object_set_new(unit_obj, "helptext", JSON_NULL); // Assume NULL for simplicity

    return unit_obj;
}
