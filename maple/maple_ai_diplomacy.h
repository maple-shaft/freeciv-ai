
#ifndef FC__DAIDIPLOMACY_H
#define FC__DAIDIPLOMACY_H

/* common */
#include "fc_types.h"

#include "ai.h" /* incident_type */

struct treaty;
struct Clause;
struct ai_data;

void mapleai_diplomacy_begin_new_phase(struct ai_type *ait, struct player *pplayer);
void mapleai_diplomacy_actions(struct ai_type *ait, struct player *pplayer);

void mapleai_treaty_evaluate(struct ai_type *ait, struct player *pplayer,
                         struct player *aplayer, struct treaty *ptreaty);
void mapleai_treaty_accepted(struct ai_type *ait, struct player *pplayer,
                         struct player *aplayer, struct treaty *ptreaty);

void mapleai_incident(struct ai_type *ait, enum incident_type type,
                  enum casus_belli_range scope,
                  const struct action *paction,
                  struct player *receiver,
                  struct player *violator, struct player *victim);

bool mapleai_on_war_footing(struct ai_type *ait, struct player *pplayer);

void mapleai_diplomacy_first_contact(struct ai_type *ait, struct player *pplayer,
                                 struct player *aplayer);

void mapleai_revolution_start(struct ai_type *ait, struct player *pplayer);

#endif /* FC__DAIDIPLOMACY_H */
