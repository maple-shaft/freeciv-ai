
#ifndef FC__DAICITY_H
#define FC__DAICITY_H

/* common */
#include "effects.h"  /* enum effect_type */
#include "fc_types.h"

/* server/advisors */
#include "advchoice.h"

struct adv_data;
struct tech_vector;

struct ai_activity_cache; /* Defined and only used within daicity.c */

/* Who's coming to kill us, for attack co-ordination */
struct ai_invasion {
  int attack;         /* Units capable of attacking city */
  int occupy;         /* Units capable of occupying city */
};

struct ai_city {
  adv_want worth; /* Cache city worth here, sum of all weighted incomes */

  int building_turn;            /* Only recalculate every Nth turn */
  int building_wait;            /* For weighting values */
#define BUILDING_WAIT_MINIMUM (1)

  struct adv_choice choice;     /* To spend gold in the right place only */

  struct ai_invasion invasion;
  int attack, bcost; /* This is also for invasion - total power and value of
                      * all units coming to kill us. */

  unsigned int danger;          /* Danger to be compared to assess_defense */
  unsigned int grave_danger;    /* Danger, should show positive feedback */
  unsigned int urgency;         /* How close the danger is; if zero,
                                   bodyguards can leave */
  int wallvalue;                /* How much it helps for defenders to be
                                   ground units */

  int distance_to_wonder_city;  /* Wondercity will set this for us,
                                   avoiding paradox */

  bool celebrate;               /* Try to celebrate in this city */
  bool diplomat_threat;         /* Enemy diplomat or spy is near the city */
  bool has_diplomat;            /* This city has diplomat or spy defender */

  /* These values are for builder (UTYF_WORKERS) and founder (UTYF_CITIES) units.
   * Negative values indicate that the city needs a boat first;
   * -value is the degree of want in that case. */
  bool founder_boat;            /* City founder will need a boat */
  int founder_turn;             /* Only recalculate every Nth turn */
  int founder_want;
  int worker_want;
  struct unit_type *worker_type;
};

#define MAPLEAI_WANT_BELOW_MIL_EMERGENCY (1000.0)
#define MAPLEAI_WANT_MILITARY_EMERGENCY  (MAPLEAI_WANT_BELOW_MIL_EMERGENCY + 0.1)
#define MAPLEAI_WANT_DOMESTIC_MAX (MAPLEAI_WANT_MILITARY_EMERGENCY / 4 * 3)

void mapleai_manage_cities(struct ai_type *ait, struct player *pplayer);

void mapleai_city_alloc(struct ai_type *ait, struct city *pcity);
void mapleai_city_free(struct ai_type *ait, struct city *pcity);

struct section_file;
void mapleai_city_save(struct ai_type *ait, const char *aitstr,
                   struct section_file *file,
                   const struct city *pcity, const char *citystr);
void mapleai_city_load(struct ai_type *ait, const char *aitstr,
                   const struct section_file *file,
                   struct city *pcity, const char *citystr);

void want_techs_for_improvement_effect(struct ai_type *ait,
                                       struct player *pplayer,
                                       const struct city *pcity,
                                       const struct impr_type *pimprove,
                                       struct tech_vector *needed_techs,
                                       adv_want building_want);

void dont_want_tech_obsoleting_impr(struct ai_type *ait,
                                    struct player *pplayer,
                                    const struct city *pcity,
                                    const struct impr_type *pimprove,
                                    adv_want building_want);

void mapleai_build_adv_init(struct ai_type *ait, struct player *pplayer);
void mapleai_build_adv_adjust(struct ai_type *ait, struct player *pplayer,
                          struct city *wonder_city);

void mapleai_build_adv_override(struct ai_type *ait, struct city *pcity,
                            struct adv_choice *choice);

void mapleai_consider_wonder_city(struct ai_type *ait, struct city *pcity, bool *result);

bool mapleai_can_city_build_improvement_later(const struct city *pcity,
                                          const struct impr_type *pimprove);
bool mapleai_can_player_build_improvement_later(const struct player *p,
                                       const struct impr_type *pimprove);

Impr_type_id mapleai_find_source_building(struct city *pcity,
                                      enum effect_type effect_type,
                                      const struct unit_type *utype);

adv_want mapleai_city_want(struct player *pplayer, struct city *acity,
                       struct adv_data *adv, struct impr_type *pimprove);

#endif /* FC__DAICITY_H */
