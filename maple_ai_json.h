#ifndef MAPLE_AI_JSON_H
#define MAPLE_AI_JSON_H

/* common */
#include "fc_types.h"
#include "name_translation.h"
#include "tile.h"
#include "traderoutes.h"
#include "workertask.h"
#include "worklist.h"
#include "base.h"
#include "extras.h"
#include "road.h"
#include "terrain.h"
#include "unitlist.h"

#include "maple_ai_city.h"
#include <jansson.h>

json_t *convert_extra_type(const struct extra_type *t);
json_t *convert_tile(const struct tile *t);
json_t *convert_terrain(const struct terrain *t);
json_t *convert_unit_type(const struct unit_type *t);
//json_t *convert_adv_choice(adv_choice *adv);




#endif
