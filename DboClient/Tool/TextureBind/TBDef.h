#ifndef __TB_DEF_H__
#define __TB_DEF_H__

#define TB_EPSILON 0.0001f
#define TB_PI	3.1415926535897932384626433832795028841971693993751f
#define TB_DEG2RAD(_x) ((_x) * ( ((RwReal)TB_PI) / ((RwReal)180) ))
#define TB_RAD2DEG(_x) ((_x) * ( ((RwReal)180) / ((RwReal)TB_PI) ))

static const RwV3d  XAxis = { 1.0f, 0.0f, 0.0f };
static const RwV3d  YAxis = { 0.0f, 1.0f, 0.0f };
static const RwV3d  ZAxis = { 0.0f, 0.0f, 1.0f };
static const RwV3d  ZeroAxis = { 0.0f, 0.0f, 0.0f };

// define
#define rsRESOURCESDEFAULTARENASIZE (4<<20)

#endif//__TB_DEF_H__