
#include <base/error.h>
#include <base/game.h>

#include <meeting17/playstate.h>

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

    playstate.flags |= PF_STARTED;
    return ERR_OK;
}

void playstate_clean() {
    if (!(playstate.flags & PF_STARTED)) {
        /* Do nothing, playstate not started */
        return;
    }

    playstate.flags &= ~PF_STARTED;
}

err playstate_update() {
    return ERR_OK;
}

err playstate_draw() {
    return ERR_OK;
}

