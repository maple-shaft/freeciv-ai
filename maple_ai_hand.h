
#ifndef FC__DAIHAND_H
#define FC__DAIHAND_H

/* common */
#include "fc_types.h"

void mapleai_do_first_activities(struct ai_type *ait, struct player *pplayer);
void mapleai_do_last_activities(struct ai_type *ait, struct player *pplayer);

void mapleai_calc_data(const struct player *pplayer, int *trade, int *expenses,
                   int *income);

#endif /* FC__DAIHAND_H */
