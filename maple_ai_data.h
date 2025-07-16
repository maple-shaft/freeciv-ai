#ifndef FC__MAIDATA_H
#define FC__MAIDATA_H

/* utility */
#include "support.h"

/* common */
#include "fc_types.h"
#include "tech.h"

/* server */
#include "srv_main.h" /* bv_id */

/* server/advisors */
#include "advtools.h"

struct player;
struct ai_plr;

void mapleai_player_alloc(struct ai_type *ait, struct player *pplayer);
void mapleai_player_free(struct ai_type *ait, struct player *pplayer);
void mapleai_player_save(struct ai_type *ait, const char *aitstr,
                     struct player *pplayer, struct section_file *file,
                     int plrno);
void mapleai_player_save_relations(struct ai_type *ait, const char *aitstr,
                               struct player *pplayer, struct player *other,
                               struct section_file *file, int plrno);
void mapleai_player_load(struct ai_type *ait, const char *aitstr,
                     struct player *pplayer, const struct section_file *file,
                     int plrno);
void mapleai_player_load_relations(struct ai_type *ait, const char *aitstr,
                               struct player *pplayer, struct player *other,
                               const struct section_file *file, int plrno);

void mapleai_player_copy(struct ai_type *ait,
                     struct player *original, struct player *created);
void mapleai_gained_control(struct ai_type *ait, struct player *pplayer);

static inline struct ai_city *def_ai_city_data(const struct city *pcity,
                                               struct ai_type *deftype)
{
  return (struct ai_city *)city_ai_data(pcity, deftype);
}

static inline struct unit_ai *def_ai_unit_data(const struct unit *punit,
                                               struct ai_type *deftype)
{
  return (struct unit_ai *)unit_ai_data(punit, deftype);
}

static inline struct ai_plr *def_ai_player_data(const struct player *pplayer,
                                                struct ai_type *deftype)
{
  return (struct ai_plr *)player_ai_data(pplayer, deftype);
}

enum winning_strategy {
  WIN_OPEN,     /* Still undetermined */
  WIN_WAR,      /* We have no other choice than to crush all opposition */
  WIN_SPACE,    /* We will race for space, peace very important */
  WIN_CAPITAL   /* We cannot win unless we take war_target's capital */
};

#define SPECENUM_NAME war_reason
#define SPECENUM_VALUE0 DAI_WR_BEHAVIOR
#define SPECENUM_VALUE0NAME "Behavior"
#define SPECENUM_VALUE1 DAI_WR_SPACE
#define SPECENUM_VALUE1NAME "Space"
#define SPECENUM_VALUE2 DAI_WR_EXCUSE
#define SPECENUM_VALUE2NAME "Excuse"
#define SPECENUM_VALUE3 DAI_WR_HATRED
#define SPECENUM_VALUE3NAME "Hatred"
#define SPECENUM_VALUE4 DAI_WR_ALLIANCE
#define SPECENUM_VALUE4NAME "Alliance"
#define SPECENUM_VALUE5 DAI_WR_NONE
#define SPECENUM_VALUE5NAME "None"
#include "specenum_gen.h"

struct ai_dip_intel {
  /* Remember one example of each for text spam purposes. */
  struct player *is_allied_with_enemy;
  struct player *at_war_with_ally;
  struct player *is_allied_with_ally;

  signed char spam;      /* timer to avoid spamming a player with chat */
  int distance;   /* average distance to that player's cities */
  int countdown;  /* we're on a countdown to war declaration */
  enum war_reason war_reason; /* why we declare war */
  signed char ally_patience; /* we EXPECT our allies to help us! */
  signed char asked_about_peace;     /* don't ask again */
  signed char asked_about_alliance;  /* don't nag! */
  signed char asked_about_ceasefire; /* don't ... you get the point */
  signed char warned_about_space;
};

struct ai_plr
{
  bool phase_initialized;

  int last_num_continents;
  int last_num_oceans;

  struct {
    int passengers;   /* Number of passengers waiting for boats */
    int boats;
    int available_boats;

    int *workers;     /* Cities to workers on continent */
    int *ocean_workers;

    bv_id diplomat_reservations;
  } stats;

    /* AI diplomacy and opinions on other players */
  struct {
    const struct ai_dip_intel **player_intel_slots;
    enum winning_strategy strategy;
    int timer; /* Pursue our goals with some stubbornness, in turns */
    char love_coeff;          /* Reduce love with this % each turn */
    char love_incr;           /* Modify love with this fixed amount */
    int req_love_for_peace;
    int req_love_for_alliance;
    struct player *war_target;
  } diplomacy;

    /* Cache map for AI settlers; defined in daisettler.c. */
  struct ai_settler *settler;

  /* The units of tech_want seem to be shields */
  adv_want tech_want[A_LAST+1];
};

struct mapleai_private_data
{
  bool contemplace_workers;
};

void mapleai_data_init(struct ai_type *ait, struct player *pplayer);
void mapleai_data_close(struct ai_type *ait, struct player *pplayer);

void mapleai_data_phase_begin(struct ai_type *ait, struct player *pplayer, bool is_new_phase);
void mapleai_data_phase_finished(struct ai_type *ait, struct player *pplayer);
bool is_ai_data_phase_open(struct ai_type *ait, struct player *pplayer);

struct ai_plr *mapleai_plr_data_get(struct ai_type *ait, struct player *pplayer, bool *caller_closes);

struct ai_dip_intel *mapleai_diplomacy_get(struct ai_type *ait, const struct player *plr1, const struct player *plr2);

void mapleai_gov_value(struct ai_type *ait, struct player *pplayer, struct government *gov, adv_want *val, bool *override);

void mapleai_adjust_policies(struct ai_type *ait, struct player *pplayer);

#endif /* FC__MAIDATA_H */
