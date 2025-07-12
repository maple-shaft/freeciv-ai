#include <fc_config.h>

/* common */
#include "ai.h"
#include "player.h"
#include "ai_stub.h"

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
  Setup player ai_funcs function pointers.
**************************************************************************/
bool fc_ai_maple_setup(struct ai_type *ai)
{
  struct mapleai_private_data *private;

  maple_ai_set_self(ai);

  strncpy(ai->name, "maple", sizeof(ai->name));

  ai->funcs.first_activities = maple_end_turn;
  ai->funcs.restart_phase    = maple_end_turn;

  return TRUE;
}