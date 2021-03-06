/**
 * @file include/conf/type.h
 *
 * Define all in-game types.
 */
#ifndef __CONF_TYPE_H__
#define __CONF_TYPE_H__

#include <GFraMe/gfmTypes.h>

/** Mask that return only the base type */
#define T_BASEMASK  0x0000001f
/** Mask that return the proper 16 bit type */
#define T_MASK      0x0000ffff
/** Number of bits per type */
#define T_BITS      16
/**
 * How many bits there are for any given "base type". Different types that share
 * the same base one will be rendered within the quadtree with the same color.
 */
#define T_BASE_NBITS 5

/** Retrieve an object's type (mask out all non-type bits) */
#define TYPE(type) \
    (type & T_MASK)
#define BASETYPE(type) \
    (type & T_BASEMASK)

/* Examples from my current main project */
enum enType {
      T_HAZARD      = gfmType_reserved_2  /* ( 5) pink */
    , T_PLAYER      = gfmType_reserved_3  /* ( 6) light blue */
    , T_FLOOR       = gfmType_reserved_5  /* ( 8) purple */
    , T_ENEMY       = gfmType_reserved_7  /* (10) light red */
    , T_COLLECTIBLE = gfmType_reserved_9  /* (12) blue */
    , T_FX          = gfmType_reserved_10 /* (13) dirty yellow/green */

    , T_RED_BREAD    = (1 << T_BASE_NBITS) | T_COLLECTIBLE
    , T_ORANGE_BREAD = (2 << T_BASE_NBITS) | T_COLLECTIBLE
    , T_YELLOW_BREAD = (3 << T_BASE_NBITS) | T_COLLECTIBLE
    , T_GREEN_BREAD  = (4 << T_BASE_NBITS) | T_COLLECTIBLE
    , T_CYAN_BREAD   = (5 << T_BASE_NBITS) | T_COLLECTIBLE
    , T_BLUE_BREAD   = (6 << T_BASE_NBITS) | T_COLLECTIBLE
    , T_PURPLE_BREAD = (7 << T_BASE_NBITS) | T_COLLECTIBLE

    , T_RED_JELLY_FX    = (1 << T_BASE_NBITS) | T_FX
    , T_ORANGE_JELLY_FX = (2 << T_BASE_NBITS) | T_FX
    , T_YELLOW_JELLY_FX = (3 << T_BASE_NBITS) | T_FX
    , T_GREEN_JELLY_FX  = (4 << T_BASE_NBITS) | T_FX
    , T_CYAN_JELLY_FX   = (5 << T_BASE_NBITS) | T_FX
    , T_BLUE_JELLY_FX   = (6 << T_BASE_NBITS) | T_FX
    , T_PURPLE_JELLY_FX = (7 << T_BASE_NBITS) | T_FX
};
typedef enum enType type;

#endif /* __CONF_TYPE_H__ */

