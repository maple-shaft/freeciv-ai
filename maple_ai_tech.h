#ifndef FC__DAITECH_H
#define FC__DAITECH_H

/* common */
#include "fc_types.h"
#include "unittype.h"

void mapleai_manage_tech(struct ai_type *ait, struct player *pplayer);
void mapleai_clear_tech_wants(struct ai_type *ait, struct player *pplayer);
void mapleai_next_tech_goal(struct player *pplayer);
struct unit_type *mapleai_wants_role_unit(struct ai_type *ait,
                                      struct player *pplayer,
                                      struct city *pcity, int role, int want);
struct unit_type *mapleai_wants_defender_against(struct ai_type *ait,
                                             const struct civ_map *nmap,
                                             struct player *pplayer,
                                             struct city *pcity,
                                             const struct unit_type *att,
                                             int want);

#endif /* FC__DAITECH_H */