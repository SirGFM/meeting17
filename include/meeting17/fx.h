
#ifndef __FX_H__
#define __FX_H__

#include <base/error.h>
#include <conf/type.h>
#include <GFraMe/gfmSprite.h>
#include <GFraMe/gfmSpriteset.h>

/** List of particle animations */
enum enFxAnim {
    FXA_RED_JELLY = 0
  , FXA_ORANGE_JELLY
  , FXA_YELLOW_JELLY
  , FXA_GREEN_JELLY
  , FXA_CYAN_JELLY
  , FXA_BLUE_JELLY
  , FXA_PURPLE_JELLY
  , FXA_MAX
};
typedef enum enFxAnim fxAnim;

err fx_init();
void fx_clean();
err fx_update();
err fx_draw();
/** Spawn an effect with a non-default time-to-live */
gfmSprite* fx_spawn_ttl(gfmSpriteset *pSset, type t, int x, int y, int w, int h
        , fxAnim anim, int ttl);
gfmSprite* fx_spawn(gfmSpriteset *pSset, type t, int x, int y, int w, int h
        , fxAnim anim);

#endif /* __FX_H__ */

