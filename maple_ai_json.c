#include "maple_ai_json.h"

#include "terrain.h"
#include "common.h"

/*
struct extra_type
{
  int id;
  struct name_translation name;
  bool ruledit_disabled;
  void *ruledit_dlg;
  enum extra_category category;
  uint16_t causes;
  uint8_t rmcauses;

  char graphic_str[MAX_LEN_NAME];
  char graphic_alt[MAX_LEN_NAME];
  char activity_gfx[MAX_LEN_NAME];
  char act_gfx_alt[MAX_LEN_NAME];
  char act_gfx_alt2[MAX_LEN_NAME];
  char rmact_gfx[MAX_LEN_NAME];
  char rmact_gfx_alt[MAX_LEN_NAME];
  char rmact_gfx_alt2[MAX_LEN_NAME];

  struct requirement_vector reqs;
  struct requirement_vector rmreqs;
  struct requirement_vector appearance_reqs;
  struct requirement_vector disappearance_reqs;

  bool buildable;
  bool generated;
  int build_time;
  int build_time_factor;
  int removal_time;
  int removal_time_factor;
  int infracost;

  int defense_bonus;
  int appearance_chance;
  int disappearance_chance;

  enum extra_unit_seen_type eus;

  bv_unit_classes native_to;

  bv_extra_flags flags;
  bv_extras conflicts;
  bv_extras hidden_by;
  bv_extras bridged_over;

  int no_aggr_near_city;
  Tech_type_id visibility_req;
  struct extra_type_list *hiders;
  struct extra_type_list *bridged;
  struct strvec *helptext;
  struct
  {
    int special_idx;
    struct base_type *base;
    struct road_type *road;
    struct resource_type *resource;
  } data;
};
*/
json_t *convert_extra_type(const struct extra_type *t) {
    json_t *obj = json_object();
    if (!obj) {
        return NULL;
    }
    json_obj_set_new(obj, "index", json_integer(t->id));
    //json_obj_set_new(obj, "continent", json_integer(t->continent));
    //json_obj_set_new(obj, "extras", json_integer(t->extras));
    return obj;
}

/*
struct unit_type {
  Unit_type_id item_number;
  struct name_translation name;
  int build_cost;
  int pop_cost;
  struct specialist *spec_type;
  int attack_strength;
  int defense_strength;
  int move_rate;
  int unknown_move_cost;
  struct requirement_vector build_reqs;
  int vision_radius_sq;
  int transport_capacity;
  int hp;
  int firepower;
  struct combat_bonus_list *bonuses;
#define U_NOT_OBSOLETED (NULL)
  const struct unit_type *obsoleted_by;
  const struct unit_type *converted_to;
  int convert_time;
  int fuel;
  bv_unit_type_flags flags;
  bv_unit_type_roles roles;
  int happy_cost;
  int upkeep[O_LAST];
  int paratroopers_range;
  struct veteran_system *veteran;
  int bombard_rate;
  int city_size;
  int city_slots;
  enum transp_def_type tp_defense;
  struct unit_class *uclass;
  bv_unit_classes cargo;
  bv_unit_classes targets;
  bv_unit_classes embarks;
  bv_unit_classes disembarks;
  enum vision_layer vlayer;
  struct strvec *helptext;
  struct {
    bool igwall;
    bool worker;
  } adv;

  struct {
    int max_defense_mp_bonus_pct;
    int defense_mp_bonuses_pct[U_LAST];
    int scramble_coeff[U_LAST];
  } cache;

  void *ais[FREECIV_AI_MOD_LAST];
};
*/
json_t *convert_unit_type(const struct unit_type *t) {

}

/*
struct terrain {
  int item_number;
  struct name_translation name;
  char identifier; // Single-character identifier used in games saved. 
  char identifier_load; // Single-character identifier that was used in the save game loaded. 

    #define TERRAIN_UNKNOWN_IDENTIFIER 'u'

  enum terrain_class tclass;

  int movement_cost; // Whole MP, not scaled by SINGLE_MOVE 
  int defense_bonus; // % defense bonus - defaults to zero 

  int output[O_LAST];

  struct extra_type **resources; // nullptr-terminated 
  int *resource_freq; // Same length as resources 

#define RESOURCE_FREQUENCY_MINIMUM (0)
#define RESOURCE_FREQUENCY_DEFAULT (1)
#define RESOURCE_FREQUENCY_MAXIMUM (255)

  int road_output_incr_pct[O_LAST];
  int base_time;
  int road_time;

  struct terrain *cultivate_result;
  int cultivate_time;

  struct terrain *plant_result;
  int plant_time;

  int irrigation_food_incr;
  int irrigation_time;

  int mining_shield_incr;
  int mining_time;

  int placing_time;

  struct terrain *transform_result;

  int transform_time;
  int pillage_time;

  // Currently only clean times, but named for future 
  int extra_removal_times[MAX_EXTRA_TYPES];

  const struct unit_type *animal;

  // These are special properties of the terrain used by mapgen.  If a tile
  // has a property, then the value gives the weighted amount of tiles that
  // will be assigned this terrain.
  // For instance if mountains have 70 and hills have 30 of MG_MOUNTAINOUS
  // then 70% of 'mountainous' tiles will be given mountains.
  // Ocean_depth is different.  Instead of a percentage, the depth of the
  // tile in the range 0 (never chosen) to 100 (deepest) is used.
  int property[MG_COUNT];
#define TERRAIN_OCEAN_DEPTH_MINIMUM (1)
#define TERRAIN_OCEAN_DEPTH_MAXIMUM (100)

  bv_unit_classes native_to;

  bv_terrain_flags flags;

  struct rgbcolor *rgb;

  struct strvec *helptext;
};
*/
json_t *convert_terrain(const struct terrain *t) {
    json_t *obj = json_object();
    if (!obj) {
        return NULL;
    }

    struct terrain *te = terrain_by_identifier(t->identifier)
    char *translated_name = t->name

    json_obj_set_new(obj, "item_number", json_integer(t->item_number));
    json_obj_set_new(obj, "tclass", json_string(t->tclass));
    json_obj_set_new(obj, "movement_cost", json_integer(t->movement_cost));
    json_obj_set_new(obj, "defense_bonus", json_integer(t->defense_bonus));
    json_obj_set_new(obj, "base_time", json_integer(t->base_time));
    json_obj_set_new(obj, "road_time", json_integer(t->road_time));
    json_obj_set_new(obj, "cultivate_time", json_integer(t->cultivate_time));
    json_obj_set_new(obj, "plant_time", json_integer(t->plant_time));
    json_obj_set_new(obj, "irrigation_time", json_integer(t->irrigation_time));
    json_obj_set_new(obj, "mining_time", json_integer(t->mining_time));
    json_obj_set_new(obj, "irrigation_food_incr", json_integer(t->irrigation_food_incr));
    json_obj_set_new(obj, "mining_shield_incr", json_integer(t->mining_shield_incr));
    json_obj_set_new(obj, "placing_time", json_integer(t->placing_time));
    json_obj_set_new(obj, "transform_time", json_integer(t->transform_time));
    json_obj_set_new(obj, "pillage_time", json_integer(t->pillage_time));
    //json_obj_set_new(obj, "animal", convert_unit_type(t-animal));
    return obj;
}

/*
  int index; // Index coordinate of the tile. Used to calculate (x, y) pairs
               (index_to_map_pos()) and (nat_x, nat_y) pairs
               (index_to_native_pos()).
  Continent_id continent;
  bv_extras extras;
  struct extra_type *resource;          // NULL for no resource 
  struct terrain *terrain;		// NULL for unknown tiles 
  struct unit_list *units;
  struct city *worked;			// NULL for not worked 
  struct player *owner;			// NULL for not owned 
  struct extra_type *placing;
  int infra_turns;
  struct player *extras_owner;
  struct tile *claimer;
  int altitude;
  char *label;                          // NULL for no label 
  char *spec_sprite;
*/
json_t *convert_tile(const struct tile *t) {
    json_t *obj = json_object();
    if (!obj) {
        return NULL;
    }
    json_obj_set_new(obj, "index", json_integer(t->index));
    json_obj_set_new(obj, "continent", json_integer(t->continent));
    json_obj_set_new(obj, "resource", convert_extra_type(t->resource));
    json_obj_set_new(obj, "terrain", convert_terrain(t->terrain));
    json_obj_set_new(obj, "unit_list", convert_unit_list(t->resource));
    
    return obj;
}