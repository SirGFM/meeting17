
#include <base/error.h>
#include <base/game.h>
#include <meeting17/fx.h>
#include <meeting17/playstate.h>
#include <string.h>

enum enPlaystateFlags {
    PF_STARTED = 0x01
  , PF_MAX
};

/** Declare the playstate context (!!!GLOBALLY!!!) */
struct stPlaystateCtx {
    enum enPlaystateFlags flags;
};
struct stPlaystateCtx playstate = {0};


err playstate_init() {
    /* If playstate was initialized, clean it up */
    playstate_clean();

    fx_init();

    playstate.flags |= PF_STARTED;
    return ERR_OK;
}

void playstate_clean() {
    if (!(playstate.flags & PF_STARTED)) {
        /* Do nothing, playstate not started */
        return;
    }

    fx_clean();

    memset(&playstate, 0x0, sizeof(struct stPlaystateCtx));
}

err playstate_update() {
    err erv;

    erv = fx_update();
    ASSERT(erv == ERR_OK, erv);

    return ERR_OK;
}

err playstate_draw() {
    err erv;

    erv = fx_draw();
    ASSERT(erv == ERR_OK, erv);

    return ERR_OK;
}

