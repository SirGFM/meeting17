
#include <base/error.h>
#include <base/game.h>
#include <base/gfx.h>
#include <conf/type.h>
#include <GFraMe/gfmError.h>
#include <GFraMe/gfmGroup.h>
#include <meeting17/fx.h>
#include <string.h>

#define MAX_FX_NUM            4096
#define DEFAULT_DEATH_ON_EXIT 1
#define DEFAULT_TTL           4000

#define JELLY_ANIM(f) \
  f,f,f,f, f+128,f+256,f+128,f+256, f+384,f+512,f+640,f+768

static int pFxAnimData[] = {
  /*                    len|fps|loop|data... */
  /*   FXA_RED_JELLY  */ 12, 12,  0 , JELLY_ANIM(1024)
, /* FXA_ORANGE_JELLY */ 12, 12,  0 , JELLY_ANIM(1025)
, /* FXA_YELLOW_JELLY */ 12, 12,  0 , JELLY_ANIM(1026)
, /*  FXA_GREEN_JELLY */ 12, 12,  0 , JELLY_ANIM(1027)
, /*  FXA_CYAN_JELLY  */ 12, 12,  0 , JELLY_ANIM(1028)
, /*  FXA_BLUE_JELLY  */ 12, 12,  0 , JELLY_ANIM(1029)
, /* FXA_PURPLE_JELLY */ 12, 12,  0 , JELLY_ANIM(1030)
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

err fx_spawn() {
    return ERR_OK;
}

