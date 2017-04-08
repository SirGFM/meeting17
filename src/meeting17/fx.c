
#include <base/error.h>
#include <base/game.h>
#include <base/gfx.h>
#include <conf/type.h>
#include <GFraMe/gfmError.h>
#include <GFraMe/gfmGroup.h>
#include <GFraMe/gfmSprite.h>
#include <GFraMe/gfmSpriteset.h>
#include <meeting17/fx.h>
#include <string.h>

#define MAX_FX_NUM            4096
#define DEFAULT_DEATH_ON_EXIT 1
#define DEFAULT_TTL           4000

#define JELLY_ANIM(f) \
  f,f,f,f, f,f,f,f, f+128,f+256,f+128,f+256, f+384,f+512,f+640,f+768, f+896

static int pFxAnimData[] = {
  /*                    len|fps|loop|data... */
  /*   FXA_RED_JELLY  */ 17, 20,  0 , JELLY_ANIM(1024)
, /* FXA_ORANGE_JELLY */ 17, 20,  0 , JELLY_ANIM(1025)
, /* FXA_YELLOW_JELLY */ 17, 20,  0 , JELLY_ANIM(1026)
, /*  FXA_GREEN_JELLY */ 17, 20,  0 , JELLY_ANIM(1027)
, /*  FXA_CYAN_JELLY  */ 17, 20,  0 , JELLY_ANIM(1028)
, /*  FXA_BLUE_JELLY  */ 17, 20,  0 , JELLY_ANIM(1029)
, /* FXA_PURPLE_JELLY */ 17, 20,  0 , JELLY_ANIM(1030)
};
static int fxAnimDataLen = sizeof(pFxAnimData) / sizeof(int);

enum enFxFlags {
    EF_STARTED = 0x1
};

/** Declare the effects context (!!!GLOBALLY!!!) */
struct stFxCtx {
    enum enFxFlags flags;
    gfmGroup *pGroup;
};
struct stFxCtx fx = {0};

err fx_init() {
    gfmRV rv;

    /* Ensure its not initialized */
    fx_clean();

    rv = gfmGroup_getNew(&fx.pGroup);
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);

    rv = gfmGroup_setDefType(fx.pGroup, T_FX);
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);
    rv = gfmGroup_setDefSpriteset(fx.pGroup, gfx.pSset8x8);
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);
    rv = gfmGroup_setDefDimensions(fx.pGroup, 8/*w*/, 8/*h*/, 0/*offx*/, 0/*offy*/);
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);
    rv = gfmGroup_setDeathOnLeave(fx.pGroup, DEFAULT_DEATH_ON_EXIT);
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);
    rv = gfmGroup_setDeathOnTime(fx.pGroup, DEFAULT_TTL);
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);
    rv = gfmGroup_setDefAnimData(fx.pGroup, pFxAnimData, fxAnimDataLen);
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);

    rv =  gfmGroup_setDrawOrder(fx.pGroup, gfmDrawOrder_newestFirst);
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);
    rv = gfmGroup_setCollisionQuality(fx.pGroup, gfmCollisionQuality_none);
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);

    rv = gfmGroup_preCache(fx.pGroup, MAX_FX_NUM, MAX_FX_NUM);
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);

    fx.flags |= EF_STARTED;
    return ERR_OK;
}

void fx_clean() {
    if (!(fx.flags & EF_STARTED)) {
        /* Hasn't been started, do nothing... */
        return;
    }

    gfmGroup_free(&fx.pGroup);

    memset(&fx, 0x0, sizeof(struct stFxCtx));
}

err fx_update() {
    ASSERT(gfmGroup_update(fx.pGroup, game.pCtx) == GFMRV_OK, ERR_GFMERR);

    return ERR_OK;
}

err fx_draw() {
    ASSERT(gfmGroup_draw(fx.pGroup, game.pCtx) == GFMRV_OK, ERR_GFMERR);

    return ERR_OK;
}

/** Spawn an effect with a non-default time-to-live */
gfmSprite* fx_spawn_ttl(gfmSpriteset *pSset, type t, int x, int y, int w, int h
        , fxAnim anim, int ttl) {
    gfmSprite *pSpr;

    gfmGroup_setDeathOnTime(fx.pGroup, ttl);
    pSpr = fx_spawn(pSset, t, x, y, w, h, anim);
    gfmGroup_setDeathOnTime(fx.pGroup, DEFAULT_TTL);

    return pSpr;
}

gfmSprite* fx_spawn(gfmSpriteset *pSset, type t, int x, int y, int w, int h
        , fxAnim anim) {
    gfmSprite *pSpr;
    gfmRV rv;

    rv = gfmGroup_recycle(&pSpr, fx.pGroup);
    ASSERT(rv == GFMRV_OK, 0);
    rv = gfmSprite_setPosition(pSpr, x, y);
    ASSERT(rv == GFMRV_OK, 0);
    rv = gfmSprite_setDimensions(pSpr, w, h);
    ASSERT(rv == GFMRV_OK, 0);
    rv = gfmSprite_setOffset(pSpr, 0/*offx*/, 0/*offy*/);
    ASSERT(rv == GFMRV_OK, 0);
    rv = gfmSprite_setType(pSpr, t);
    ASSERT(rv == GFMRV_OK, 0);
    rv = gfmSprite_setSpriteset(pSpr, pSset);
    ASSERT(rv == GFMRV_OK, 0);
    rv = gfmSprite_playAnimation(pSpr, anim);
    ASSERT(rv == GFMRV_OK, 0);
    rv = gfmSprite_resetAnimation(pSpr);
    ASSERT(rv == GFMRV_OK, 0);

    return pSpr;
}

