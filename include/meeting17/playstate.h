
#ifndef __PLAYSTATE_H__
#define __PLAYSTATE_H__

#include <base/error.h>

err playstate_init();
void playstate_clean();
err playstate_update();
err playstate_draw();

#endif /* __PLAYSTATE_H__ */

