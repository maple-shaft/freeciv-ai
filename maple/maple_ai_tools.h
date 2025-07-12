
#ifndef FC__DAITOOLS_H
#define FC__DAITOOLS_H

/* utility */
#include "support.h"            /* bool type */

/* server/advisors */
#include "advchoice.h"

/* ai/default */
#include "maple_ai_unit.h"            /* enum ai_unit_task */

struct pf_path;
struct pf_parameter;
struct pft_amphibious;

struct adv_risk_cost;

#define DAI_WANT_BELOW_MIL_EMERGENCY (1000.0)
#define DAI_WANT_MILITARY_EMERGENCY  (DAI_WANT_BELOW_MIL_EMERGENCY + 0.1)
#define DAI_WANT_DOMESTIC_MAX (DAI_WANT_MILITARY_EMERGENCY / 4 * 3)

const char *mapleai_unit_task_rule_name(const enum ai_unit_task task);

adv_want military_amortize(struct player *pplayer, struct city *pcity,
                           adv_want value, int delay, int build_cost);
int stack_cost(struct unit *pattacker, struct unit *pdefender);

void mapleai_unit_move_or_attack(struct ai_type *ait, struct unit *punit,
                             struct tile *ptile, struct pf_path *path,
                             int step);

void mapleai_fill_unit_param(struct ai_type *ait,
                         struct pf_parameter *parameter,
                         struct adv_risk_cost *risk_cost,
                         struct unit *punit, struct tile *ptile);
bool mapleai_gothere(struct ai_type *ait, struct player *pplayer,
                 struct unit *punit, struct tile *dst_tile);
struct tile *immediate_destination(struct unit *punit,
                                   struct tile *dest_tile);
void mapleai_log_path(struct unit *punit,
                  struct pf_path *path, struct pf_parameter *parameter);
bool mapleai_unit_goto_constrained(struct ai_type *ait, struct unit *punit,
                               struct tile *ptile,
                               struct pf_parameter *parameter);
bool mapleai_unit_goto(struct ai_type *ait, struct unit *punit, struct tile *ptile);
bool goto_is_sane(struct unit *punit, struct tile *ptile);

void mapleai_unit_new_task(struct ai_type *ait, struct unit *punit,
                       enum ai_unit_task task, struct tile *ptile);
void mapleai_unit_new_adv_task(struct ai_type *ait, struct unit *punit,
                           enum adv_unit_task task, struct tile *ptile);
bool mapleai_unit_make_homecity(struct unit *punit, struct city *pcity);
bool mapleai_unit_attack(struct ai_type *ait, struct unit *punit,
                     struct tile *ptile);
bool mapleai_unit_move(struct ai_type *ait, struct unit *punit, struct tile *ptile);

void mapleai_government_change(struct player *pplayer, struct government *gov);
int mapleai_gold_reserve(struct player *pplayer);

void adjust_choice(int pct, struct adv_choice *choice);

bool mapleai_choose_role_unit(struct ai_type *ait, struct player *pplayer,
                          struct city *pcity, struct adv_choice *choice,
                          enum choice_type type, int role, int want,
                          bool need_boat);
void mapleai_build_adv_override(struct ai_type *ait, struct city *pcity,
                            struct adv_choice *choice);
bool mapleai_assess_military_unhappiness(const struct civ_map *nmap,
                                     struct city *pcity);

void mapleai_consider_plr_dangerous(struct ai_type *ait, struct player *plr1,
                                struct player *plr2,
                                enum override_bool *result);

#endif /* FC__DAITOOLS_H */
