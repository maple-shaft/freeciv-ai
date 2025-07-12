
#ifndef FC__DAISETTLER_H
#define FC__DAISETTLER_H

/* utility */
#include "support.h"            /* bool type */

/* common */
#include "ai.h"
#include "city.h"
#include "fc_types.h"

struct tile_data_cache {
  short food;   /* Food output of the tile */
  short trade;  /* Trade output of the tile */
  short shield; /* Shield output of the tile */

  int sum;      /* Weighted sum of the tile output */

  int reserved; /* Reservation for this tile; used by print_citymap() */

  int turn;     /* The turn the values were calculated */
};

struct ai_settler {
  struct tile_data_cache_hash *tdc_hash;
};

struct cityresult {
  struct tile *tile;
  adv_want total;              /* Total value of position */
  adv_want result;             /* Amortized and adjusted total value */
  int corruption, waste;
  bool overseas;               /* Have to use boat to get there */
  bool virt_boat;              /* Virtual boat was used in search,
                                * so need to build one */

  struct {
    struct tile_data_cache *tdc;  /* Values of city center; link to the data
                                   * in tdc_hash. */
  } city_center;

  struct {
    struct tile *tile;            /* Best other tile */
    int cindex;                   /* City-relative index for other tile */
    struct tile_data_cache *tdc;  /* Value of best other tile; link to the
                                   * data in tdc_hash. */
  } best_other;

  adv_want remaining;             /* Value of all other tiles */

  /* Save the result for print_citymap(). */
  struct tile_data_cache_hash *tdc_hash;

  int city_radius_sq;             /* Current squared radius of the city */
};

struct ai_data;
struct ai_plr;
struct tile_data_cache;
struct tile_data_cache *tile_data_cache_new(void);
struct tile_data_cache *tile_data_cache_copy(const struct tile_data_cache *ptdc);
struct cityresult *city_desirability(struct ai_type *ait,
                                     struct player *pplayer,
                                     struct unit *punit, struct tile *ptile);
static struct cityresult *settler_map_iterate(struct ai_type *ait,
                                              struct pf_parameter *parameter,
                                              struct unit *punit,
                                              int boat_cost);
static struct cityresult *find_best_city_placement(struct ai_type *ait,
                                                   struct unit *punit,
                                                   bool look_for_boat,
                                                   bool use_virt_boat);
static enum cb_error_level mapleai_do_build_city(struct ai_type *ait,
                                             struct player *pplayer,
                                             struct unit *punit);

void mapleai_auto_settler_init(struct ai_plr *ai);
void mapleai_auto_settler_free(struct ai_plr *ai);

void mapleai_auto_settler_reset(struct ai_type *ait, struct player *pplayer);
void mapleai_auto_settler_run(struct ai_type *ait, const struct civ_map *nmap,
                          struct player *pplayer,
                          struct unit *punit, struct workermap *state);
void mapleai_auto_settler_cont(struct ai_type *ait, const struct civ_map *nmap,
                           struct player *pplayer,
                           struct unit *punit, struct workermap *state);

void contemplate_new_city(struct ai_type *ait, struct city *pcity);

#endif /* FC__DAISETTLER_H */
