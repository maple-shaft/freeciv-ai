#ifndef SERIALIZE_H
#define SERIALIZE_H

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
void serialize_float(float val, json_t *obj, const char *key);
// Helper function to serialize a struct name_translation
void serialize_name(const struct name_translation *val, json_t *obj, const char *key);

// Helper function to serialize a bool
void serialize_bool(bool val, json_t *obj, const char *key);

////// Player Translation Stuff

json_t *serialize_player_diplstate(const struct player_diplstate *player_diplstate);

json_t *serialize_player(const struct player *player);

////// End Player Translation Stuff

////// CM Translation Stuff

json_t *serialize_cm_parameter(const struct cm_parameter *cm_parameter);

////// End CM Translation Stuff

////// Advisor Translation Stuff

json_t *serialize_adv_area_info(const struct adv_area_info *aai);

json_t *serialize_adv_data(const struct adv_data *adv_data);

////// End Advisor Translation Stuff

////// City Translation Stuff

json_t *serialize_goods_type(const struct goods_type *goods_type);
json_t *serialize_trade_route(const struct trade_route *trade_route);
json_t *serialize_trade_route_list(const struct trade_route_list *trl);
json_t *serialize_tile(const struct tile *tile);
json_t *serialize_city(const struct city *city);
json_t *serialize_city_list(const struct city_list *city_list);

////// End City Translation Stuff

////// Unit Translation Stuff

// Helper function to serialize a struct specialist
json_t *serialize_specialist(const struct specialist *spec_type);
json_t *serialize_unit_type(const struct unit_type *unit_type);
// Helper function to serialize a struct unit_class
json_t *serialize_unit_class(const struct unit_class *uclass, json_t *obj);
json_t *serialize_unit_orders(const struct unit_order *unit_order_list, size_t length);
json_t *serialize_unit_order(const struct unit_order *unit_order);
json_t *serialize_unit(const struct unit *unit);
json_t *serialize_unit_list(const struct unit_list *unit_list);

////// End Unit Translation Stuff

////// World Translation Stuff

json_t *serialize_world(const struct world *world);

////// End World Translation Stuff

// Helper function to serialize a struct combat_bonus_list
json_t *serialize_combat_bonus_list(const struct combat_bonus_list *bonuses, json_t *obj);

#endif