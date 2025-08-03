#include "fc_config.h"

/* common */
#include "ai.h"
#include "player.h"
#include "log.h"

/* maple ai */
//#include "maple_ai_data.h"
#include "ai_stub.h"
#include "serialize.h"
#include "agent.h"

#include <jansson.h>

/* Defines */
const char *fc_ai_maple_capstr(void);
bool fc_ai_maple_setup(struct ai_type *ai);
/* End Defines */

static struct ai_type *self = NULL;

const char *fc_ai_maple_capstr(void)
{
  printf("fc_ai_maple_capstr\n");
  return FC_AI_MOD_CAPSTR;
}

static void maple_end_turn(struct player *pplayer)
{
  printf("maple_end_turn\n");
  pplayer->ai_phase_done = TRUE;
}

static void mapleai_module_close(void)
{
  printf("Maple Module Closed \n ");
}

static void mapleai_do_nothing(struct player *player) {
  // Do nothing... wait, nvm just log.
  printf("Do Nothing.");
}

static void mapleai_first_activities(struct player *pplayer)
{
    printf("mapleai_first_activities\n");
    json_t *root_json = json_object();
    printf("mapleai_first_activities\n");
    json_t *player_json = serialize_player(pplayer);
    printf("mapleai_first_activities\n");
    json_object_set(root_json, "player", player_json);
    printf("mapleai_first_activities\n");
    // Step 1: Gather state
    struct city *pcity;
    printf("mapleai_first_activities\n");
    json_t *cities_json = json_array();
    printf("mapleai_first_activities\n");
    city_list_iterate(pplayer->cities, pcity) {
      printf("city_list_iterate\n");
        // Analyze city — production, growth, defense, etc.
        json_array_append(cities_json, serialize_city(pcity));

    } city_list_iterate_end;
    printf("mapleai_first_activities\n");
    json_object_set(root_json, "cities", cities_json);
    printf("mapleai_first_activities\n");
    struct unit *punit;
    printf("mapleai_first_activities\n");
    json_t *units_json = json_array();
    printf("mapleai_first_activities\n");
    unit_list_iterate(pplayer->units, punit) {
        printf("unit_list_iterate\n");
        // Evaluate unit role, potential actions
        json_array_append(units_json, serialize_unit(punit));
    } unit_list_iterate_end;
    printf("mapleai_first_activities\n");
    json_object_set(root_json, "units", units_json);
    printf("mapleai_first_activities\n");
    char *response_str = generate(root_json);
    printf("mapleai_first_activities\n");
    printf("response_str = %s \n", response_str);
    printf("mapleai_first_activities\n");
    json_decref(root_json);
    printf("mapleai_first_activities\n");
    json_decref(player_json);
    printf("mapleai_first_activities\n");
    json_decref(cities_json);
    printf("mapleai_first_activities\n");
    json_decref(units_json);
    printf("mapleai_first_activities\n");
    // Step 2: Formulate goals
    // e.g. find enemy cities, unoccupied tiles, or resource-rich locations

    // Step 3: Schedule actions (moving units, changing production, etc.)
    // e.g. unit_go_to(punit, ptile);
}

/**********************************************************************//**
  Setup player ai_funcs function pointers.
**************************************************************************/
bool fc_ai_maple_setup(struct ai_type *ai)
{
  printf("fc_ai_maple_setup \n");
  self = ai;

  strncpy(ai->name, "maple", sizeof(ai->name));
  printf("Sanity test about to call first activities! %s \n", ai->name);
  
  ai->funcs.first_activities = mapleai_first_activities;
  ai->funcs.module_close = mapleai_module_close;
  ai->funcs.game_start = mapleai_do_nothing;
  ai->funcs.restart_phase = maple_end_turn;

  ai->funcs.player_alloc = mapleai_do_nothing;
  ai->funcs.player_free = mapleai_do_nothing;
  ai->funcs.player_save_relations = mapleai_do_nothing;
  ai->funcs.player_load_relations = mapleai_do_nothing;
  ai->funcs.gained_control = mapleai_do_nothing;
  ai->funcs.split_by_civil_war = mapleai_do_nothing;
  ai->funcs.created_by_civil_war = mapleai_do_nothing;
  ai->funcs.phase_begin = mapleai_do_nothing;
  ai->funcs.phase_finished = mapleai_do_nothing;
  ai->funcs.city_alloc = mapleai_do_nothing;
  ai->funcs.city_free = mapleai_do_nothing;
  ai->funcs.city_save = mapleai_do_nothing;
  ai->funcs.city_load = mapleai_do_nothing;
  ai->funcs.choose_building = mapleai_do_nothing;
  ai->funcs.build_adv_prepare = mapleai_do_nothing;
  ai->funcs.build_adv_init = mapleai_do_nothing;
  ai->funcs.build_adv_adjust_want = mapleai_do_nothing;
  ai->funcs.gov_value = mapleai_do_nothing;
  ai->funcs.units_ruleset_init = mapleai_do_nothing;
  ai->funcs.units_ruleset_close = mapleai_do_nothing;
  ai->funcs.unit_alloc = mapleai_do_nothing;
  ai->funcs.unit_free = mapleai_do_nothing;
  ai->funcs.unit_got = mapleai_do_nothing;
  ai->funcs.unit_lost = mapleai_do_nothing;
  ai->funcs.unit_transformed = mapleai_do_nothing;
  ai->funcs.unit_turn_end = mapleai_do_nothing;
  ai->funcs.unit_move = mapleai_do_nothing;
  ai->funcs.unit_task = mapleai_do_nothing;
  ai->funcs.unit_save = mapleai_do_nothing;
  ai->funcs.unit_load = mapleai_do_nothing;
  ai->funcs.settler_reset = mapleai_do_nothing;
  ai->funcs.settler_run = mapleai_do_nothing;
  ai->funcs.settler_cont = mapleai_do_nothing;
  ai->funcs.want_to_explore = mapleai_do_nothing;
  ai->funcs.diplomacy_actions = mapleai_do_nothing;
  ai->funcs.treaty_evaluate = mapleai_do_nothing;
  ai->funcs.treaty_accepted = mapleai_do_nothing;
  ai->funcs.first_contact = mapleai_do_nothing;
  ai->funcs.incident = mapleai_do_nothing;
  ai->funcs.log_fragment_city = mapleai_do_nothing;
  ai->funcs.log_fragment_unit = mapleai_do_nothing;
  ai->funcs.consider_plr_dangerous = mapleai_do_nothing;
  ai->funcs.consider_tile_dangerous = mapleai_do_nothing;
  ai->funcs.consider_wonder_city = mapleai_do_nothing;
  ai->funcs.revolution_start = mapleai_do_nothing;
  ai->funcs.last_activities = mapleai_do_nothing;

  return TRUE;
}