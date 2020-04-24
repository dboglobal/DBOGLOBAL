/***************************************************************************
 *                                                                         *
 * Module  : batypehf.h                                                    *
 *                                                                         *
 * Purpose : Generic type with frames handling                             *
 *                                                                         *
 **************************************************************************/

#ifndef RWTYPEHF_H
#define RWTYPEHF_H

/****************************************************************************
 Includes
 */

#include "batype.h"
#include "baframe.h"

/****************************************************************************
 Defines
 */

/****************************************************************************
 Global Types
 */

/* RWPUBLIC */

typedef struct RwObjectHasFrame RwObjectHasFrame;
typedef RwObjectHasFrame * (*RwObjectHasFrameSyncFunction)(RwObjectHasFrame *object);

#if (!defined(DOXYGEN))
struct RwObjectHasFrame
{
    RwObject                     object;
    RwLLLink                     lFrame;
    RwObjectHasFrameSyncFunction sync;
};
#endif /* (!defined(DOXYGEN)) */

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* Frames */
extern void _rwObjectHasFrameSetFrame(void *object, RwFrame *frame);
extern void _rwObjectHasFrameReleaseFrame(void *object);

/* ObjectHASFRAME METHODS */
#define rwObjectHasFrameInitialize(o, type, subtype, syncFunc)  \
MACRO_START                                                     \
{                                                               \
    rwObjectInitialize(o, type, subtype);                       \
    ((RwObjectHasFrame *)o)->sync = syncFunc;                   \
}                                                               \
MACRO_STOP

#define rwObjectHasFrameSync(o) \
    ((RwObjectHasFrame *)(o))->sync(o)

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* Compatibility macros */

#define rwObjectHasFrameSetFrame(object, frame) \
        _rwObjectHasFrameSetFrame(object, frame)
#define rwObjectHasFrameReleaseFrame(object) \
        _rwObjectHasFrameReleaseFrame(object)


/* RWPUBLICEND */

#endif /* RWTYPEHF_H */
