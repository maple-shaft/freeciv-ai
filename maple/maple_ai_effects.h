
#ifndef FC__DAI_EFFECTS_H
#define FC__DAI_EFFECTS_H

#include "fc_types.h"

adv_want mapleai_effect_value(struct player *pplayer,
                          const struct adv_data *adv, const struct city *pcity,
                          const bool capital, int turns,
                          const struct effect *peffect, const int c,
                          const int nplayers);

adv_want mapleai_content_effect_value(const struct player *pplayer,
                                  const struct city *pcity,
                                  int amount,
                                  int num_cities,
                                  int happiness_step);

bool mapleai_can_requirement_be_met_in_city(const struct requirement *preq,
                                        const struct player *pplayer,
                                        const struct city *pcity);

#endif /* FC__DAI_EFFECTS_H */
