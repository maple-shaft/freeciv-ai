#ifndef FC__DAIPLAYER_H
#define FC__DAIPLAYER_H

/* common */
#include "city.h"
#include "player.h"

struct player;
struct ai_plr;

void maple_ai_player_alloc(struct ai_type *ait, struct player *pplayer);
void maple_ai_player_free(struct ai_type *ait, struct player *pplayer);
void maple_ai_player_save(struct ai_type *ait, const char *aitstr,
                     struct player *pplayer, struct section_file *file,
                     int plrno);
void mapleai_player_save_relations(struct ai_type *ait, const char *aitstr,
                               struct player *pplayer, struct player *other,
                               struct section_file *file, int plrno);
void mapleai_player_load(struct ai_type *ait, const char *aitstr,
                     struct player *pplayer, const struct section_file *file,
                     int plrno);
void mapleai_player_load_relations(struct ai_type *ait, const char *aitstr,
                               struct player *pplayer, struct player *other,
                               const struct section_file *file, int plrno);

void mapleai_player_copy(struct ai_type *ait,
                     struct player *original, struct player *created);
void mapleai_gained_control(struct ai_type *ait, struct player *pplayer);


#endif /* FC__DAIPLAYER_H */
