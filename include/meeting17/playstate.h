
#ifndef __PLAYSTATE_H__
#define __PLAYSTATE_H__

#include <base/error.h>
#include <conf/type.h>

err playstate_init();
void playstate_clean();
err playstate_update();
err playstate_draw();
err playstate_spawnCollectible(int x, int y, type t);

#endif /* __PLAYSTATE_H__ */

