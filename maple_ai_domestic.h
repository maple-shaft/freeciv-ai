#ifndef FC__DAIDOMESTIC_H
#define FC__DAIDOMESTIC_H

/* common */
#include "fc_types.h"

struct adv_choice *domestic_advisor_choose_build(struct ai_type *ait,
                                                 struct player *pplayer,
                                                 struct city *pcity);

void mapleai_wonder_city_distance(struct ai_type *ait, struct player *pplayer, 
                              struct adv_data *adv);

#endif /* FC__DAIDOMESTIC_H */
