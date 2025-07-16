#include "fc_types.h"
#include "name_translation.h"
#include "tile.h"
#include "traderoutes.h"
#include "workertask.h"
#include "worklist.h"
#include "base.h"
#include "extras.h"
#include "road.h"
#include "terrain.h"
#include "unitlist.h"
#include <jansson.h>
#include <stdlib.h>
#include <string.h>


// Helper function to serialize a string (handles strdup)
void serialize_string(const char *str, json_t *obj, const char *key) {
    json_object_set_new(obj, key, json_string(strdup(str)));
}

// Helper function to serialize a bitvector (convert to integer)
//void serialize_bitvector(bv_unit_type_flags flags, json_t *obj, const char *key) {
//    json_object_set_new(obj, key, ((long)flags));
//}


// Helper function to serialize a struct specialist
json_t *serialize_specialist(const struct specialist *spec_type, json_t *obj) {
    if (!spec_type) return NULL;

    json_t *spec_obj = json_object();

    // Serialize specialist fields (assuming fields like name, cost, etc.)
    serialize_string(spec_type->name, spec_obj, "name");
    json_object_set_new(spec_obj, "cost", json_integer((long)spec_type->cost);

    // Add to main object
    json_object_set(obj, "spec_type", spec_obj);
    return spec_obj;
}

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
    serialize_string(uclass->name, uclass_obj, "name");

    // Add to main object
    json_object_set(obj, "uclass", uclass_obj);
    return uclass_obj;
}

// Helper function to serialize a struct strvec
// (Commented out: unknown struct type)
// json_t *serialize_strvec(const struct strvec *helptext, json_t *obj) {
//     if (!helptext) return NULL;
//
//     json_t *helptext_obj = json_object();
//
//     // Serialize strvec fields (assuming fields like strings, etc.)
//     // json_t *string_list = json_array();
//     // for (int i = 0; i < helptext->num_strings; i++) {
//     //     json_array_append_new(string_list, json_string(helptext->strings[i]));
//     // }
//     // json_object_set_new(helptext_obj, "strings", string_list);
//
//     // Add to main object
//     json_object_set(obj, "helptext", helptext_obj);
//     return helptext_obj;
// }

// Helper function to serialize a struct veteran_system
// (Commented out: unknown struct type)
// json_t *serialize_veteran_system(const struct veteran_system *veteran, json_t *obj) {
//     if (!veteran) return NULL;
//
//     json_t *veteran_obj = json_object();
//
//     // Serialize veteran_system fields (assuming fields like level, etc.)
//     // json_object_set_new(veteran_obj, "level", json_integer((long)veteran->level);
//
//     // Add to main object
//     json_object_set(obj, "veteran", veteran_obj);
//     return veteran_obj;
// }

// Main serialization function for unit_type
json_t *serialize_unit_type(const struct unit_type *unit) {
    if (!unit) return NULL;

    json_t *unit_obj = json_object();

    // Serialize unit_type fields
    json_object_set_new(unit_obj, "item_number", (long)unit->item_number);
    //serialize_string(unit->name, unit_obj, "name");
    json_object_set_new(unit_obj, "build_cost", json_integer((long)unit->build_cost));
    json_object_set_new(unit_obj, "pop_cost", json_integer((long)unit->pop_cost));
    json_object_set, "spec_type", serialize_specialist(unit->spec_type, unit_obj);
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
