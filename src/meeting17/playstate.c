
#include <base/error.h>
#include <base/game.h>
#include <conf/type.h>
#include <meeting17/collectible.h>
#include <meeting17/fx.h>
#include <meeting17/playstate.h>
#include <string.h>

#define MAX_COLLECTIBLE 32

enum enPlaystateFlags {
    PF_STARTED = 0x01
  , PF_MAX
};

/** Declare the playstate context (!!!GLOBALLY!!!) */
struct stPlaystateCtx {
    enum enPlaystateFlags flags;
    collectibleCtx *collectibles[MAX_COLLECTIBLE];
};
struct stPlaystateCtx playstate = {0};


err playstate_init() {
    int i;

    /* If playstate was initialized, clean it up */
    playstate_clean();

    fx_init();

    /* Initialize collectibles that may be recycled */
    i = 0;
    while (i < MAX_COLLECTIBLE) {
        playstate.collectibles[i] = collectible_init();
        ASSERT(playstate.collectibles[i] != 0, ERR_ALLOC_FAILED);
        i++;
    }

    playstate.flags |= PF_STARTED;
    return ERR_OK;
}

void playstate_clean() {
    int i;

    if (!(playstate.flags & PF_STARTED)) {
        /* Do nothing, playstate not started */
        return;
    }

    i = 0;
    while (i < MAX_COLLECTIBLE) {
        if (playstate.collectibles[i] != 0) {
            collectible_clean(playstate.collectibles[i]);
        }
        i++;
    }

    fx_clean();

    memset(&playstate, 0x0, sizeof(struct stPlaystateCtx));
}

err playstate_update() {
    err erv;
    int i;

    i = 0;
    while (i < MAX_COLLECTIBLE) {
        collectible_update(playstate.collectibles[i]);
        i++;
    }
    erv = fx_update();
    ASSERT(erv == ERR_OK, erv);

    return ERR_OK;
}

err playstate_draw() {
    err erv;
    int i;

    i = 0;
    while (i < MAX_COLLECTIBLE) {
        collectible_draw(playstate.collectibles[i]);
        i++;
    }
    erv = fx_draw();
    ASSERT(erv == ERR_OK, erv);

    return ERR_OK;
}

err playstate_spawnCollectible(int x, int y, type t) {
    int i;

    i = 0;
    while (i < MAX_COLLECTIBLE) {
        if (!(playstate.collectibles[i]->flags & CF_IS_ALIVE)) {
            return collectible_revive(playstate.collectibles[i], x, y, t);
        }
        i++;
    }

    return ERR_BAD_ACCESS;
}

