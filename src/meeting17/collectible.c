#include <base/error.h>
#include <base/game.h>
#include <base/gfx.h>
#include <conf/type.h>
#include <GFraMe/gfmSprite.h>
#include <meeting17/collectible.h>
#include <meeting17/fx.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PARTICLES_PER_FRAME 4

static double fallSpeed = 100;

struct stCollectibleCtx {
    struct stCollectiblePublicCtx pub;
    gfmSprite *pSelf;
};

collectibleCtx* collectible_init() {
    struct stCollectibleCtx *pCtx;
    gfmRV rv;

    pCtx = malloc(sizeof(struct stCollectibleCtx));
    ASSERT(pCtx != 0, 0);
    memset(pCtx, 0x0, sizeof(struct stCollectibleCtx));

    rv = gfmSprite_getNew(&pCtx->pSelf);
    ASSERT_TO(rv == GFMRV_OK, NOOP(), __fail);

    return (collectibleCtx*)pCtx;
__fail:
    free(pCtx);
    return 0;
}

void collectible_clean(collectibleCtx *_pPubCtx) {
    struct stCollectibleCtx *pCtx = (struct stCollectibleCtx*)_pPubCtx;

    gfmSprite_free(&pCtx->pSelf);
    free(pCtx);
}

err collectible_revive(collectibleCtx* _pPubCtx, int x, int y, type t) {
    struct stCollectibleCtx *pCtx = (struct stCollectibleCtx*)_pPubCtx;
    gfmRV rv;

    gfmSprite_resetObject(pCtx->pSelf);
    rv = gfmSprite_init(pCtx->pSelf, x, y, 8/*w*/, 8/*h*/, gfx.pSset8x8
            , 0/*offx*/, 0/*offy*/, 0/*child*/, t);
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);
    rv = gfmSprite_setFrame(pCtx->pSelf, 66);
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);

    gfmSprite_setPosition(pCtx->pSelf, x, y);
    gfmSprite_setVerticalVelocity(pCtx->pSelf, fallSpeed);
    pCtx->pub.flags |= CF_IS_ALIVE;

    return ERR_OK;
}

static uint32_t _prng_timer;

err collectible_update(collectibleCtx *_pPubCtx) {
    void *pVoid;
    struct stCollectibleCtx *pCtx = (struct stCollectibleCtx*)_pPubCtx;
    gfmRV rv;
    int t, i, x, y;
    fxAnim anim;

    if (!(pCtx->pub.flags & CF_IS_ALIVE)) {
        /* Not alive, stop updating it... */
        return ERR_OK;
    }

    rv = gfmSprite_update(pCtx->pSelf, game.pCtx);
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);
    if (gfmCamera_isSpriteInside(game.pCamera, pCtx->pSelf) == GFMRV_FALSE) {
        /* Just left the screen, kill it and exit */
        pCtx->pub.flags &= ~CF_IS_ALIVE;
        return ERR_OK;
    }

    rv = gfmSprite_getChild(&pVoid, &t, pCtx->pSelf);
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);

#define CASE(color) \
    case T_ ## color ## _BREAD: { \
        anim = FXA_ ## color ## _JELLY; \
        t = T_ ## color ## _JELLY_FX; \
    } break

    switch (TYPE(t)) {
        CASE(RED);
        CASE(ORANGE);
        CASE(YELLOW);
        CASE(GREEN);
        CASE(CYAN);
        CASE(BLUE);
        CASE(PURPLE);
        default: ASSERT(0, ERR_ARGUMENTBAD);
    }

    /* Alloc some particles every frame... just because */
    rv = gfmSprite_getCenter(&x, &y, pCtx->pSelf);
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);
    i = PARTICLES_PER_FRAME;
    _prng_timer += (uint32_t)(game.elapsed % 7);
    _prng_timer %= 1723377; /* Guessed number, mostly likely not a prime */
    while (i > 0) {
        int cx, cy;

        cx = x - 3 + _prng_timer % 6;
        cy = y + _prng_timer % 3;
        fx_spawn(gfx.pSset2x2, t, cx, cy, 2/*w*/, 2/*h*/, anim);

        i--;
    }

    return ERR_OK;
}

err collectible_draw(collectibleCtx *_pPubCtx) {
    struct stCollectibleCtx *pCtx = (struct stCollectibleCtx*)_pPubCtx;
    gfmRV rv;

    if (!(pCtx->pub.flags & CF_IS_ALIVE)) {
        /* Not alive, stop updating it... */
        return ERR_OK;
    }
    rv = gfmSprite_draw(pCtx->pSelf, game.pCtx);
    ASSERT(rv == GFMRV_OK, ERR_GFMERR);

    return ERR_OK;
}

