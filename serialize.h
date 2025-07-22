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
#include <jansson.h>
#include <stdlib.h>
#include <string.h>

// Helper function to serialize a string (handles strdup)
void serialize_string(const char *str, json_t *obj, const char *key);

// Helper function to serialize an int
void serialize_int(int int_val, json_t *obj, const char *key);

// Helper function to serialize a struct name_translation
void serialize_name(const struct name_translation *val, json_t *obj, const char *key);

// Helper function to serialize a bool
void serialize_bool(bool val, json_t *obj, const char *key);

////// CM Translation Stuff

json_t *serialize_cm_parameter(const struct cm_parameter *cm_parameter);

////// End CM Translation Stuff

////// Advisor Translation Stuff

json_t *serialize_adv_area_info(const struct adv_area_info *aai);

json_t *serialize_adv_data(const struct adv_data *adv_data);

////// End Advisor Translation Stuff

////// Unit Translation Stuff

// Helper function to serialize a struct specialist
json_t *serialize_specialist(const struct specialist *spec_type);

json_t *serialize_unit_type(const struct unit_type *unit_type);

////// End Unit Translation Stuff

////// World Translation Stuff

json_t *serialize_world(const struct world *world);

////// End World Translation Stuff

// Helper function to serialize a struct combat_bonus_list
json_t *serialize_combat_bonus_list(const struct combat_bonus_list *bonuses, json_t *obj);

// Helper function to serialize a struct unit_class
json_t *serialize_unit_class(const struct unit_class *uclass, json_t *obj);
