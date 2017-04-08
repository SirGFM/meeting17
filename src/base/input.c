/**
 * @file src/base/input.c
 */
#include <base/collision.h>
#include <base/error.h>
#include <base/game.h>
#include <base/input.h>
#include <conf/input_list.h>

#include <GFraMe/gfmError.h>

#include <string.h>

enum enInputNames {
#define X_GPAD(...)
#define X_KEY(name, ...) enInput_##name,
    X_RELEASE_BUTTON_LIST
    X_SYSTEM_BUTTON_LIST
    enInput_count,
    X_DEBUG_BUTTON_LIST
#undef X_KEY
#undef X_GPAD
};
typedef enum enInputNames inputNames;

/**
 * Handle every input that require an immediate action (i.e, those that are more
 * like flags, instead of being interpreted during the game loop).
 */
void handleInput() {
    if (DID_JUST_PRESS(pause)) {
        /* TODO Pause the game */
    }

    if (DID_JUST_RELEASE(fullscreen)) {
        gfmRV rv;

        /* TODO Refactor this keeping the current state */
        rv = gfm_setWindowed(game.pCtx);
        if (rv == GFMRV_WINDOW_MODE_UNCHANGED) {
            gfm_setFullscreen(game.pCtx);
        }
    }
}

#if defined(DEBUG)
/**
 * Handle the debug controls of the game's simulation. These allow the update
 * loop to be paused/resumed or even stepped.
 */
void handleDebugInput() {
    if (DID_JUST_RELEASE(dbgResetFps)) {
        gfm_resetFPS(game.pCtx);
    }

    if (DID_JUST_RELEASE(dbgPause)) {
        /* Toggle pause/resume update loop */
        if (game.debugRunState == DBG_PAUSED) {
            game.debugRunState = DBG_RUNNING;
        }
        else {
            game.debugRunState = DBG_PAUSED;
        }
    }

    if (DID_JUST_RELEASE(dbgStep)) {
        /* Single step & pause update loop */
        game.debugRunState = DBG_STEP;
    }

    if (DID_JUST_RELEASE(qt)) {
        /* Toggle quadtree visibility */
        collision.flags ^= CF_VISIBLE;
    }

    if (DID_JUST_RELEASE(gif)) {
        gfmRV rv;

        rv = gfm_didExportGif(game.pCtx);
        if (rv == GFMRV_TRUE || rv == GFMRV_GIF_OPERATION_NOT_ACTIVE) {
            rv = gfm_recordGif(game.pCtx, 10000 /* ms */, "anim.gif", 8, 0);
        }
    }
}
#endif

/** Retrieve the state of every button */
err updateInput() {
    /** List of buttons, used to easily iterate through all virtual buttons */
    button *pButtons;
    inputNames i;

    i = 0;
    pButtons = (button*)(&input);
    /* Iterate through all buttons and update their state */
    while (i < enInput_count) {
        gfmRV rv;

        rv = gfm_getKeyState(&pButtons[i].state, &pButtons[i].numPressed
                , game.pCtx, pButtons[i].handle);
        ASSERT(rv == GFMRV_OK, ERR_GFMERR);

        i++;
    }

    return ERR_OK;
}

/** Forcefully update every debug button */
err updateDebugInput() {
    gfmInput *pInput;
    gfmRV rv;

    rv = gfm_getInput(&pInput, game.pCtx);
    ASSERT(rv == GFMRV_OK, rv);

#define X_GPAD(...)
#define X_KEY(name, ...) \
    rv = gfmInput_updateVKey(pInput, input.name.handle); \
    ASSERT(rv == GFMRV_OK, ERR_GFMERR); \
    rv = gfm_getKeyState(&input.name.state, &input.name.numPressed, game.pCtx \
                , input.name.handle); \
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);
    X_DEBUG_BUTTON_LIST
#undef X_KEY
#undef X_GPAD

    return ERR_OK;
}

/** Initialize every button with their default mapping */
err initInput() {
    gfmRV rv;

    /* Create virtual keys for every input */
#define X_GPAD(...)
#define X_KEY(name, ...) \
    rv = gfm_addVirtualKey(&input.name.handle, game.pCtx); \
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);
    X_BUTTON_LIST
#undef X_KEY
#undef X_GPAD

    /* Bind every key */
#define X_GPAD(...)
#define X_KEY(name, key, ...) \
    rv = gfm_bindInput(game.pCtx, input.name.handle, key); \
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);
    X_BUTTON_LIST
    X_ALTERNATE_BUTTON_MAPPING
#undef X_KEY
#undef X_GPAD

#define X_KEY(...)
#define X_GPAD(name, button, port, ...) \
    rv = gfm_bindGamepadInput(game.pCtx, input.name.handle, button, port); \
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);
    X_BUTTON_LIST
    X_ALTERNATE_BUTTON_MAPPING
#undef X_GPAD
#undef X_KEY

    return ERR_OK;
}

