/**********************************************************************
 *
 * File :     rpdpvs.h
 *
 * Abstract : Handles all syncronisation between RW and dPVS by
 *            chaining the dirty-frame syncronisation of RW.
 *            Geometry,Atomic and Camera all have dPVS structures
 *
 **********************************************************************
 *
 * This file is a product of Criterion Software Ltd.
 *
 * This file is provided as is with no warranties of any kind and is
 * provided without any obligation on Criterion Software Ltd.
 * to assist in its use or modification.
 *
 * Criterion Software Ltd. will not, under any
 * circumstances, be liable for any lost revenue or other damages arising
 * from the use of this file.
 *
 * Copyright (c) 2001 Criterion Software Ltd.
 * All Rights Reserved.
 *
 * RenderWare is a trademark of Criterion Software Ltd.
 *
 ************************************************************************/

#if (!defined(RPDPVS_H))
#define RPDPVS_H
/* Doxygen plugin groups. */

/******************************************************************************
 *  Include files
 */
#include <rwcore.h>
#include <rpworld.h>

#include "rpdpvs.rpe"          /* automatically generated header file */

#if (defined(__cplusplus))
#endif /* (defined(__cplusplus)) */

/**
 * \ingroup rpdpvs
 * \ref RpDPVSGetTimeCallBack is the function called by dPVS when it needs
 *  to know the elapsed time.
 *
 * \warning This function is needed by dPVS. The timer function
 *  should provide a mimimum 20Hz timer.
 *
 * \return time value
 */
typedef             RwReal(*RpDPVSGetTimeCallBack) (void);

/**
 * \ingroup rpdpvs
 * \ref RpDPVSVisibleAtomicCallBack is the function called by dPVS when an
 * atomic is found to be visible.This allows the user to perform specific
 * actions on the visible atomics.
 *
 * \param atomic pointer to a visible atomic
 */
typedef void        (*RpDPVSVisibleAtomicCallBack) (RpAtomic * atomic);

/**
 * \ingroup rpdpvs
 * \ref RpDPVSVisibleWorldSectorCallBack is the function called by dPVS when
 * an world sector is found to be visible. This allows the user to perform
 * specific action on the visible world sectors.
 *
 * \param sector pointer to a world sector
 */
typedef void        (*RpDPVSVisibleWorldSectorCallBack) (RpWorldSector *
                                                         sector);

/**
 * \ingroup rpdpvs
 * \ref RpDPVSDebugOutput2DLinesCallBack is the function called by dPVS when it
 * needs to render 2D lines. It provides a way for the user to render debugging
 * output.
 *
 * \param  verts pointer a table of type RwV2d
 * \param  color color of the line
 */
typedef void        (*RpDPVSDebugOutput2DLinesCallBack) (const RwV2d *
                                                         verts,
                                                         const RwRGBA
                                                         color);

/**
 * \ingroup rpdpvs
 * \ref RpDPVSDebugOutput2DLinesCallBack is the function called by dPVS when it
 * needs to render 3D lines. It provides a way for the user to render debugging output.
 *
 * \param  verts pointer a table of type RwV3d
 * \param  color color of the line
 */
typedef void        (*RpDPVSDebugOutput3DLinesCallBack) (const RwV3d *
                                                         verts,
                                                         const RwRGBA
                                                         color);

/**
 * \ingroup rpdpvs
 * \ref RpDPVSDebugOutputBufferCallBack is the function called by dPVS when it needs
 * to display a buffer,
for example a depth buffer or a occulusion buffer. It provides
 *a way for the user to render debugging output.
 *
 * \param  buffer      pointer to an 8bit buffer
 * \param  bw          width of the buffer
 * \param  bh          height of the buffer
 * \param  bufferType  type of the buffer
 */
typedef void        (*RpDPVSDebugOutputBufferCallBack) (const unsigned
                                                        char *buffer,
                                                        int bw, int bh,
                                                        RwUInt32
                                                        bufferType);

#if (defined(__cplusplus))
extern              "C"
{
#endif /* (defined(__cplusplus)) */

/* flags for passing onto DPVS */
#define rpDPVS_OCCLUDER_SILHOUETTES    1
#define rpDPVS_OBJECT_BOUNDS           2
#define rpDPVS_COVERAGE_BUFFER         3
#define rpDPVS_DEPTH_BUFFER            4
#define rpDPVS_FULLBLOCKS_BUFFER       5

/**
 * \ingroup rpdpvs
 * \ref RpDPVSAtomicFlag represents the different flags that can modify the 
 * behaviors of atomics during the visibility queries.
 */
enum RpDPVSAtomicFlag
{
    rpDPVS_ATOMIC_ENABLED = 0,          /**<if the ENABLED flag is set to FALSE,
                                         * it will be interpreted as if the 
                                         * atomic did not exist at all during 
                                         * visibility queries. Non-enabled 
                                         * objects have zero processing overhead
                                         * during visibility traversals. Large 
                                         * portions of a scene can thus be 
                                         * disabled and ensure that the 
                                         * disabled objects will not slow down 
                                         * the visibility processing.*/
    rpDPVS_ATOMIC_CONTRIBUTION_CULLING, /**<An object can be subjected to 
                                         * contribution culling unless the 
                                         * CONTRIBUTION_CULLING flag is 
                                         * disabled. See \ref RpDPVSSetCoverage 
                                         * for mores information on contribution
                                         *  culling.*/
    rpDPVS_ATOMIC_UNBOUNDED,            /**<If the UNBOUNDED flag is set, the 
                                         * object is treated as having an 
                                         * infinite size, regardless of its 
                                         * actual size. An unbounded object is 
                                         * always visible if its world sector 
                                         * is visible. No view frustum or 
                                         * occlusion culling is ever performed. 
                                         * The unbounded atomic is never used 
                                         * as an occluder.*/
    rpDPVS_ATOMIC_FORCEENUMSIZE = RWFORCEENUMSIZEINT
};
typedef enum RpDPVSAtomicFlag RpDPVSAtomicFlag;


enum RpDPVSAddAtomicHints
{
    rpDPVS_USE_GEOMETRY = 0,
    rpDPVS_USE_BOUNDING_BOX,
    rpDPVS_USE_BOUNDING_SPHERE,
    rpDPVS_USE_FORCEENUMSIZE = RWFORCEENUMSIZEINT
};

/**
 * \ingroup rpdpvs
 * \ref RpDPVSAddAtomicHints allows the user to specify the
 * way dPVS should handle an atomic.
 *
 */
typedef enum RpDPVSAddAtomicHints RpDPVSAddAtomicHints;

/* controlling rendering */
extern void         
RpDPVSAtomicUseParentVisibility(RpAtomic * atomic);

extern void         
RpDPVSAddFlags(RwUInt32 flags);

extern void         
RpDPVSRemoveFlags(RwUInt32 flags);

extern void         
RpDPVSCommand(const RwChar * message);

extern RwCamera    *
RpDPVSSetCoverage(RwCamera * camera,
                  RwReal w,
                  RwReal h, RwReal opacity);

extern RwCamera    *
RpDPVSSetCulling(RwCamera * camera,
                 RwBool enable);

extern RpWorld     *
RpDPVSWorldRemoveAtomic(RpWorld * world,
                        RpAtomic * atomic);

extern RpWorld     *
RpDPVSWorldAddOccludingAtomic(RpWorld * world,
                              RpAtomic *atomic);

extern RpWorld    *
RpDPVSWorldAddNonOccludingAtomic(RpWorld *world,
                                 RpAtomic *atomic,
                                 RwUInt32 hints);

extern RpWorld     *
RpDPVSWorldRemoveClump(RpWorld * world,
                        RpClump * clump);

extern RpWorld     *
RpDPVSWorldAddOccludingClump(RpWorld * world,
                             RpClump * clump);

extern RpWorld     *
RpDPVSWorldAddNonOccludingClump(RpWorld * world,
                               RpClump * clump,
                               RwUInt32 hints);

extern RpWorld     *
RpDPVSWorldRemoveCamera(RpWorld * world,
                        RwCamera * camera);

extern RpWorld     *
RpDPVSWorldAddCamera(RpWorld * world,
                     RwCamera * camera);

/* render the objects in view */
extern void
RpDPVSWorldRender(RpWorld * world);

extern void         
RpDPVSStatisticsGet(RwInt32 * viscount,
                    RwInt32 * cullcount,
                    RwInt32 * ocount, 
                    RwInt32 * synccount);

/* initisalisation */
extern RwInt32      
RpDPVSPluginAttach(void);

extern void         
RpDPVSOpen(void);

extern void
RpDPVSSetGetTimeCallBack(RpDPVSGetTimeCallBack getTimeCallBack);

extern void
RpDPVSSetVisibleAtomicCallBack(const RpDPVSVisibleAtomicCallBack
                               visibleAtomicCallBack);

extern RpDPVSVisibleAtomicCallBack 
RpDPVSGetVisibleAtomicCallBack(void);

extern void
RpDPVSSetVisibleWorldSectorCallBack(const
                                    RpDPVSVisibleWorldSectorCallBack
                                    visibleWorldSectorCallBack);

extern RpDPVSVisibleWorldSectorCallBack
RpDPVSGetVisibleWorldSectorCallBack(void);

extern void
RpDPVSSetDebugOutput2DLinesCallBack(const
                                    RpDPVSDebugOutput2DLinesCallBack
                                    debugOutput2DLinesCallBack);

extern RpDPVSDebugOutput2DLinesCallBack
RpDPVSGetDebugOutput2DLinesCallBack(void);

extern void
RpDPVSSetDebugOutput3DLinesCallBack(const RpDPVSDebugOutput3DLinesCallBack
                                    debugOutput3DLinesCallBack);

extern RpDPVSDebugOutput3DLinesCallBack
RpDPVSGetDebugOutput3DLinesCallBack(void);

extern void
RpDPVSSetDebugOutputBufferCallBack(const RpDPVSDebugOutputBufferCallBack 
                                   debugOutputBufferCallBack);

extern RpDPVSDebugOutputBufferCallBack
RpDPVSGetDebugOutputBufferCallBack(void);

extern RpAtomic *
RpDPVSAtomicSetFlag(RpAtomic *atomic, RwUInt32 flag, RwBool value);

extern RwBool
RpDPVSAtomicGetFlag(RpAtomic *atomic, RwUInt32 flag);

extern void         RpDPVSClose(void);

#if (defined(__cplusplus))
}  /* extern              "C" */
#endif /* (defined(__cplusplus)) */


/* alias through RpDPVS plugin */
#if (!defined(_RPDPVS))
#define RpWorldRender           RpDPVSWorldRender
#define RpWorldRemoveAtomic     RpDPVSWorldRemoveAtomic
#if (defined(_RPDPVS_FORCE_NONOCCLUDE))
#define RpWorldAddAtomic(w,c)   RpDPVSWorldAddNonOccludingAtomic(w,c,rpDPVS_USE_BOUNDING_BOX)
#define RpWorldAddClump(w,c)    RpDPVSWorldAddNonOccludingClump(w,c,rpDPVS_USE_BOUNDING_BOX)
#else /* (defined(_RPDPVS_FORCE_NONOCCLUDE)) */
#define RpWorldAddAtomic        RpDPVSWorldAddOccludingAtomic
#define RpWorldAddClump         RpDPVSWorldAddOccludingClump
#endif /* (defined(_RPDPVS_FORCE_NONOCCLUDE)) */
#define RpWorldRemoveClump      RpDPVSWorldRemoveClump
#define RpWorldRemoveCamera     RpDPVSWorldRemoveCamera
#define RpWorldAddCamera        RpDPVSWorldAddCamera
#endif /* (!defined(_RPDPVS)) */

#endif                          /* RPDPVS_H */
