
#ifndef FC__DAIMILITARY_H
#define FC__DAIMILITARY_H

/* common */
#include "fc_types.h"
#include "unittype.h"

/* server/advisors */
#include "advchoice.h"

struct civ_map;

#ifdef FREECIV_WEB
#define ASSESS_DANGER_MAX_DISTANCE         40
#define AI_HANDICAP_DISTANCE_LIMIT         6  /* TODO: 20 for non-web */
#endif /* FREECIV_WEB */

/* When an enemy has this or lower number of cities left, try harder
   to finish them off. */
#define FINISH_HIM_CITY_COUNT              5

typedef struct unit_list *(player_unit_list_getter)(struct player *pplayer);

struct unit_type *mapleai_choose_defender_versus(struct city *pcity,
                                             struct unit *attacker);
struct adv_choice *military_advisor_choose_build(struct ai_type *ait,
                                                 const struct civ_map *nmap,
                                                 struct player *pplayer,
                                                 struct city *pcity,
                                                 player_unit_list_getter ul_cb);
void mapleai_assess_danger_player(struct ai_type *ait,
                              const struct civ_map *nmap, struct player *pplayer);
int assess_defense_quadratic(struct ai_type *ait, struct city *pcity);
int assess_defense_unit(struct ai_type *ait, struct city *pcity,
                        struct unit *punit, bool igwall);
int assess_defense(struct ai_type *ait, struct city *pcity);
bool mapleai_process_defender_want(struct ai_type *ait, const struct civ_map *nmap,
                               struct player *pplayer,
                               struct city *pcity, unsigned int danger,
                               struct adv_choice *choice, adv_want extra_want);

#endif /* FC__DAIMILITARY_H */
