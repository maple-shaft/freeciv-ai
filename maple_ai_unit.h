
#ifndef FC__DAIUNIT_H
#define FC__DAIUNIT_H

/* common */
#include "combat.h"
#include "fc_types.h"
#include "terrain.h"
#include "unittype.h"

struct pf_map;
struct pf_path;

struct section_file;

enum ai_unit_task { AIUNIT_NONE, AIUNIT_AUTO_WORKER, AIUNIT_BUILD_CITY,
                    AIUNIT_DEFEND_HOME, AIUNIT_ATTACK, AIUNIT_ESCORT,
                    AIUNIT_EXPLORE, AIUNIT_RECOVER, AIUNIT_HUNTER,
                    AIUNIT_TRADE, AIUNIT_WONDER };

struct unit_ai {
  /* The following are unit ids or special indicator values (<=0) */
  int ferryboat; /* The ferryboat assigned to us */
  int passenger; /* The unit assigned to this ferryboat */
  int bodyguard; /* The unit bodyguarding us */
  int charge;    /* The unit this unit is bodyguarding */

  struct tile *prev_struct, *cur_struct;
  struct tile **prev_pos, **cur_pos;

  int target; /* Target we hunt */
  bv_player hunted; /* If a player is hunting us, set by that player */
  bool done;  /* We are done controlling this unit this turn */

  enum ai_unit_task task;
};

struct unit_type_ai
{
  bool low_firepower;
  bool ferry;
  bool missile_platform;
  bool carries_occupiers;
  struct unit_type_list *potential_charges;
};

/* Simple military macros */

/* pplayers_at_war() thinks no contacts equals war, which often is
 * very annoying. */
#define WAR(plr1, plr2) \
  (player_diplstate_get(plr1, plr2)->type == DS_WAR)
#define NEVER_MET(plr1, plr2) \
  (player_diplstate_get(plr1, plr2)->type == DS_NO_CONTACT)
#define DEFENSE_POWER(ptype) \
 (ptype->defense_strength * ptype->hp * ptype->firepower)
#define ATTACK_POWER(ptype) \
 (ptype->attack_strength * ptype->hp * ptype->firepower)
#define IS_ATTACKER(ptype) \
  (ptype->attack_strength > ptype->transport_capacity)
#define POTENTIALLY_HOSTILE_PLAYER(ait, pplayer, aplayer)               \
  (WAR(pplayer, aplayer) || NEVER_MET(pplayer, aplayer)                 \
   || mapleai_diplomacy_get(ait, pplayer, aplayer)->countdown >= 0)
#define UNITTYPE_COSTS(ut)                                              \
  (ut->pop_cost * 3 + ut->happy_cost                                    \
   + ut->upkeep[O_SHIELD] + ut->upkeep[O_FOOD] + ut->upkeep[O_GOLD])

/* Invasion types */
#define INVASION_OCCUPY  0
#define INVASION_ATTACK  1

extern struct unit_type *simple_ai_types[U_LAST];

#define RAMPAGE_ANYTHING                 1
#define RAMPAGE_HUT_OR_BETTER        99998
#define RAMPAGE_FREE_CITY_OR_BETTER  99999
#define BODYGUARD_RAMPAGE_THRESHOLD (SHIELD_WEIGHTING * 4)

bool mapleai_military_rampage(struct unit *punit, int thresh_adj,
                          int thresh_move);
void mapleai_manage_units(struct ai_type *ait, struct player *pplayer);
void mapleai_manage_unit(struct ai_type *ait, struct player *pplayer,
                     struct unit *punit);
void mapleai_manage_military(struct ai_type *ait, const struct civ_map *nmap,
                         struct player *pplayer, struct unit *punit);
struct city *find_nearest_safe_city(struct unit *punit);
bool uclass_need_trans_between(struct unit_class *pclass,
                               struct tile *ctile, struct tile *ptile);
adv_want look_for_charge(struct ai_type *ait, const struct civ_map *nmap,
                         struct player *pplayer, struct unit *punit,
                         struct unit **aunit, struct city **acity);
bool mapleai_can_unit_type_follow_unit_type(const struct unit_type *follower,
                                        const struct unit_type *followee,
                                        struct ai_type *ait);

bool find_beachhead(const struct player *pplayer, struct pf_map *ferry_map,
                    struct tile *dest_tile,
                    const struct unit_type *cargo_type,
                    const struct unit_type *ferry_type,
                    struct tile **ferry_dest, struct tile **beachhead_tile);
adv_want find_something_to_kill(struct ai_type *ait, struct player *pplayer,
                                struct unit *punit,
                                struct tile **pdest_tile,
                                struct pf_path **ppath,
                                struct pf_map **pferrymap,
                                struct unit **pferryboat,
                                const struct unit_type **pboattype,
                                int *pmove_time);

int build_cost_balanced(const struct unit_type *punittype);
int unittype_def_rating_squared(const struct unit_type *att_type,
                                const struct unit_type *def_type,
                                struct player *def_player,
                                struct tile *ptile, bool fortified,
                                int veteran);
adv_want kill_desire(adv_want benefit, int attack, int loss, int vuln,
                     int attack_count);

const struct impr_type *utype_needs_improvement(const struct unit_type *putype,
                                                const struct city *pcity);

bool is_on_unit_upgrade_path(const struct unit_type *test,
                             const struct unit_type *base);

void mapleai_consider_tile_dangerous(struct ai_type *ait, struct tile *ptile,
                                 struct unit *punit,
                                 enum override_bool *result);

void mapleai_units_ruleset_init(struct ai_type *ait);
void mapleai_units_ruleset_close(struct ai_type *ait);

void mapleai_unit_init(struct ai_type *ait, struct unit *punit);
void mapleai_unit_turn_end(struct ai_type *ait, struct unit *punit);
void mapleai_unit_close(struct ai_type *ait, struct unit *punit);

#define simple_ai_unit_type_iterate(_ut)                                \
{                                                                       \
  struct unit_type *_ut;                                                \
  int _ut##_index = 0;                                                  \
  while ((_ut = simple_ai_types[_ut##_index++]) != nullptr) {

#define simple_ai_unit_type_iterate_end                                 \
  }                                                                     \
}

void mapleai_unit_save(struct ai_type *ait, const char *aitstr,
                   struct section_file *file,
                   const struct unit *punit, const char *unitstr);
void mapleai_unit_load(struct ai_type *ait, const char *aitstr,
                   const struct section_file *file,
                   struct unit *punit, const char *unitstr);

struct unit_type *mapleai_role_utype_for_terrain_class(struct city *pcity, int role,
                                                   enum terrain_class tc);

bool mapleai_unit_can_strike_my_unit(const struct unit *attacker,
                                 const struct unit *defender);

void mapleai_switch_to_explore(struct ai_type *ait, struct unit *punit,
                           struct tile *target, enum override_bool *allow);

#endif /* FC__DAIUNIT_H */
