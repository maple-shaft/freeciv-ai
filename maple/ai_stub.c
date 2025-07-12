#include "fc_config.h"

/* common */
#include "ai.h"
#include "player.h"
#include "ai_stub.h"

#include "maple_ai_city.h"
#include "maple_ai_domestic.h"
#include "maple_ai_tech.h"
#include "maple_ai_unit.h"
#include "maple_ai_tools.h"
#include "maple_ai_settler.h"
#include "maple_ai_hand.h"
#include "maple_ai_diplomacy.h"

/* maple ai */
#include "maple_ai_data.h"

/* Defines */
const char *fc_ai_maple_capstr(void);
bool fc_ai_maple_setup(struct ai_type *ai);
/* End Defines */

static struct ai_type *self = NULL;

/**********************************************************************//**
  Set pointer to ai type of the classic ai.
**************************************************************************/
static void maple_ai_set_self(struct ai_type *ai)
{
  self = ai;
}

/**********************************************************************//**
  Get pointer to ai type of the classic ai.
**************************************************************************/
static struct ai_type *maple_ai_get_self(void)
{
  return self;
}

/**********************************************************************//**
  Return module capability string
**************************************************************************/
const char *fc_ai_maple_capstr(void)
{
  return FC_AI_MOD_CAPSTR;
}

/**********************************************************************//**
  Set phase done
**************************************************************************/
static void maple_end_turn(struct player *pplayer)
{
  pplayer->ai_phase_done = TRUE;
}

/**********************************************************************//**
  Free resources allocated by the Maple AI module
**************************************************************************/
static void mapleai_module_close(void)
{
  struct ai_type *deftype = maple_ai_get_self();
  FC_FREE(deftype->private);
}

/**********************************************************************//**
  Setup player ai_funcs function pointers.
**************************************************************************/
bool fc_ai_maple_setup(struct ai_type *ai)
{
  struct mapleai_private_data *private;

  maple_ai_set_self(ai);

  strncpy(ai->name, "maple", sizeof(ai->name));

  //DAB: ai->funcs.first_activities = maple_end_turn;
  //DAB: ai->funcs.restart_phase    = maple_end_turn; // Taken from ai_stub

  ai->funcs.module_close = mapleai_module_close;

  ai->funcs.player_alloc = mapleai_player_alloc;
  ai->funcs.player_free = mapleai_player_free;
  
  ai->funcs.player_save_relations = mapleai_player_save_relations;
  ai->funcs.player_load_relations = mapleai_player_load_relations;
  ai->funcs.gained_control = mapleai_gained_control;
  //DAB: ai->funcs.split_by_civil_war = mapleai_split_by_civil_war;
  //DAB: ai->funcs.created_by_civil_war = mapleai_created_by_civil_war;

  ai->funcs.phase_begin = mapleai_data_phase_begin;
  ai->funcs.phase_finished = mapleai_data_phase_finished;

  ai->funcs.city_alloc = mapleai_city_alloc;
  ai->funcs.city_free = mapleai_city_free;
  ai->funcs.city_save = mapleai_city_save;
  ai->funcs.city_load = mapleai_city_load;
  ai->funcs.choose_building = mapleai_build_adv_override;
  ai->funcs.build_adv_prepare = mapleai_wonder_city_distance;
  ai->funcs.build_adv_init = mapleai_build_adv_init;
  ai->funcs.build_adv_adjust_want = mapleai_build_adv_adjust;

  ai->funcs.gov_value = mapleai_gov_value;

  ai->funcs.units_ruleset_init = mapleai_units_ruleset_init;
  ai->funcs.units_ruleset_close = mapleai_units_ruleset_close;

  ai->funcs.unit_alloc = mapleai_unit_init;
  ai->funcs.unit_free = mapleai_unit_close;
  //DAB: ai->funcs.unit_got = mapleai_ferry_init_ferry;
  //DAB: ai->funcs.unit_lost = mapleai_ferry_lost;
  //DAB: ai->funcs.unit_transformed = mapleai_ferry_transformed;

  ai->funcs.unit_turn_end = mapleai_unit_turn_end;
  ai->funcs.unit_move = mapleai_unit_move_or_attack;
  ai->funcs.unit_task = mapleai_unit_new_adv_task;

  ai->funcs.unit_save = mapleai_unit_save;
  ai->funcs.unit_load = mapleai_unit_load;

  ai->funcs.settler_reset = mapleai_auto_settler_reset;
  ai->funcs.settler_run = mapleai_auto_settler_run;
  ai->funcs.settler_cont = mapleai_auto_settler_cont;

  ai->funcs.want_to_explore = mapleai_switch_to_explore;

  ai->funcs.first_activities = mapleai_do_first_activities;
  ai->funcs.restart_phase = maple_end_turn;
  ai->funcs.diplomacy_actions = mapleai_diplomacy_actions;
  ai->funcs.last_activities = mapleai_do_last_activities;
  ai->funcs.treaty_evaluate = mapleai_treaty_evaluate;
  ai->funcs.treaty_accepted = mapleai_treaty_accepted;
  ai->funcs.first_contact = mapleai_diplomacy_first_contact;
  ai->funcs.incident = mapleai_incident;

  //DAB: ai->funcs.log_fragment_city = cai_city_log;
  //DAB: ai->funcs.log_fragment_unit = cai_unit_log;

  ai->funcs.consider_plr_dangerous = mapleai_consider_plr_dangerous;
  ai->funcs.consider_tile_dangerous = mapleai_consider_tile_dangerous;
  ai->funcs.consider_wonder_city = mapleai_consider_wonder_city;

  /* ai->funcs.refresh = NULL; */

  /* ai->funcs.tile_info = NULL; */
  /* ai->funcs.city_info = NULL; */
  /* ai->funcs.unit_info = NULL; */

  ai->funcs.revolution_start = mapleai_revolution_start;

  return TRUE;
}