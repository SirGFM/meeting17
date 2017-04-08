
#ifndef __FX_H__
#define __FX_H__

#include <base/error.h>

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

err fx_init();
void fx_clean();
err fx_update();
err fx_draw();
err fx_spawn();

#endif /* __FX_H__ */

