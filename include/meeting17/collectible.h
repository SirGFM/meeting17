
#ifndef __COLLECTIBLE_STRUCT_H__

enum enColliectibleFlags {
    CF_IS_ALIVE = 0x00001
};

struct stCollectiblePublicCtx {
    enum enColliectibleFlags flags;
};
typedef struct stCollectiblePublicCtx collectibleCtx;

#define __COLLECTIBLE_STRUCT_H__
#endif /* __COLLECTIBLE_STRUCT_H__ */

#ifndef __COLLECTIBLE_H__
#define __COLLECTIBLE_H__

#include <base/error.h>
#include <conf/type.h>

collectibleCtx* collectible_init();
void collectible_clean(collectibleCtx*);
err collectible_revive(collectibleCtx* _pPubCtx, int x, int y, type t);
err collectible_update(collectibleCtx*);
err collectible_draw(collectibleCtx*);

#endif /* __COLLECTIBLE_H__ */

