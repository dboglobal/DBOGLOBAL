
/**************************************************************************
 *                                                                         *
 * Module  : baclump.h                                                     *
 *                                                                         *
 * Purpose : clump abstract type                                           *
 *                                                                         *
 **************************************************************************/

#ifndef RWCLUMP_H
#define RWCLUMP_H

/****************************************************************************
 Includes
 */

#include <rwcore.h>
#include "bageomet.h"
#include "basector.h"
#include "balight.h"

/* RWPUBLIC */

/*
 * Clump and atomic handling.
 * Clumps and atomics are the movable rendered objects in the world
 *
 * Copyright (c) 1998 Criterion Software Ltd.
 */


/****************************************************************************
 Defines
 */

/****************************** Object type ID ******************************/

/* Type IDs */

#define rpATOMIC 1
#define rpCLUMP 2

/* Interpolator flags */
enum RpInterpolatorFlag
{
    rpINTERPOLATORDIRTYINSTANCE = 0x01,
    rpINTERPOLATORDIRTYSPHERE = 0x02,
    rpINTERPOLATORNOFRAMEDIRTY = 0x04,
    rpINTERPOLATORFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpInterpolatorFlag rpInterpolatorFlag;

/**
 * \ingroup rpatomic
 * The bit-field type RpAtomicFlag specifies the options available for
 * controlling the behavior of atomics. See API function \ref RpAtomicSetFlags.
 *
 * \see RpAtomicSetFlags
 * \see RpAtomicGetFlags
 * \see RpWorldSectorForAllCollisionAtomics
 */

enum RpAtomicFlag
{
    rpATOMICCOLLISIONTEST = 0x01, /**<A generic collision flag to indicate
                                   * that the atomic should be considered
                                   * in collision tests.
                                   */
    rpATOMICRENDER = 0x04,      /**<The atomic is rendered if it is
                                 * in the view frustum.
                                 */

    rpATOMICFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpAtomicFlag RpAtomicFlag;

/**
 * \ingroup rpatomic
 * \ref RpAtomicSetGeomFlag defines how an atomic references a new geometry
 *
 * \see RpAtomicSetGeometry
 */
enum RpAtomicSetGeomFlag
{
    rpATOMICSAMEBOUNDINGSPHERE = 0x01, /**<The bounding-sphere for the
                                        * new geometry is assumed to be the
                                        * same as the old one, if any, and
                                        * should not be recalculated.
                                        */

    rpATOMICSETGEOMFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpAtomicSetGeomFlag RpAtomicSetGeomFlag;

enum RpAtomicPrivateFlag
{
    rpATOMICPRIVATEWORLDBOUNDDIRTY = 0x01,
    rpATOMICPRIVATEFLAGFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RpAtomicPrivateFlag rpAtomicPrivateFlag;

/* RWPUBLICEND */

/****************************************************************************
 Global Types
 */

/* Interface */

typedef struct rpClumpGlobals rpClumpGlobals;
struct rpClumpGlobals
{
    RwFreeList         *atomicFreeList;
    RwFreeList         *clumpFreeList;
};

/* RWPUBLIC */

/**
 * \ingroup rpatomic
 * \struct RpAtomic
 * Atomic Geometry object. This should be
 * considered an opaque type. Use the RpAtomic API functions to access.
 */
typedef struct RpAtomic RpAtomic;

/**
 * \ingroup rpinterpolator
 * \struct RpInterpolator
 * Morph Target Interpolator.
 * This should be considered an opaque type.
 * Use the RpInterpolator API functions to access.
 */
typedef struct RpInterpolator RpInterpolator;

#if (!defined(DOXYGEN))
struct RpInterpolator
{
        RwInt32             flags; /**< flags */
        RwInt16             startMorphTarget; /**< startMorphTarget */
        RwInt16             endMorphTarget; /**< endMorphTarget */
        RwReal              time; /**< time */
        RwReal              recipTime; /**< recipTime */
        RwReal              position; /**< position */
};
#endif /* (!defined(DOXYGEN)) */

/* More callbacks */

/**
 * \ingroup rpclump
 * \struct RpClump
 * Clump Geometry object. This should be
 * considered an opaque type. Use the RpClump API functions to access.
 */
typedef struct RpClump RpClump;

/**
 * \ingroup rpclump
 * RpClumpCallBack represents the function called from 
 * \ref RwCameraForAllClumpsInFrustum when a clump lies inside the current 
 * camera's view frustum. It is also the callback type used with
 * \ref RpWorldForAllClumps. 
 *
 * This function should return a pointer to the current clump to indicate 
 * success. The callback may return NULL to terminate further callbacks on 
 * the clumps.
 *
 * \param  clump   Pointer to the current clump, supplied by
 *                 iterator.
 * \param  data    Pointer to developer-defined data structure.
 *
 * \return Pointer to the current clump.
 *
 * \see RpWorldForAllClumps
 * \see RpClumpGetCallBack
 * \see RpClumpSetCallBack
 * \see RwCameraForAllClumpsInFrustum
 */
typedef RpClump    *(*RpClumpCallBack) (RpClump * clump, void *data);

#if (!defined(DOXYGEN))
struct RpClump
{
        RwObject            object;

        /* Information about all the Atomics */
        RwLinkList          atomicList;

        /* Lists of lights and cameras */
        RwLinkList          lightList;
        RwLinkList          cameraList;

        /* The clump in a world */
        RwLLLink            inWorldLink;

        /* Clump frustum callback */
        RpClumpCallBack     callback;
};
#endif /* (!defined(DOXYGEN)) */

/**
 * \ingroup rpatomic
 * \ref RpAtomicCallBackRender represents the function called from
 * \ref RpAtomicRender when the specified atomic
 * lies inside the current camera's view frustum. The default callback
 * initiates execution of the atomic rendering pipeline. This function should
 * return a pointer to the atomic to indicate success.
 *
 * \param  atomic   Pointer to the current atomic, supplied by
 *                  iterator.
 *
 * \return Returns a pointer to the atomic to indicate success.
 *
 * \see RpAtomicRender
 */

typedef RpAtomic   *(*RpAtomicCallBackRender) (RpAtomic * atomic);

#if (!defined(DOXYGEN))
struct RpAtomic
{
    RwObjectHasFrame    object;

    /* Information for an instance */
    RwResEntry         *repEntry;

    /* Triangles making the object */
    RpGeometry         *geometry;

    /* Interpolated bounding sphere (in object space and world space) */
    RwSphere            boundingSphere;
    RwSphere            worldBoundingSphere;

    /* Connections to other atomics */
    RpClump            *clump;
    RwLLLink            inClumpLink;

    /* callbacks */
    RpAtomicCallBackRender renderCallBack;

    /* Interpolation animation pointer */
    RpInterpolator      interpolator;

    /* Counter for checks of "render has occurred already" */
    RwUInt16            renderFrame;
    RwUInt16            pad;

    /* Connections to sectors */
    RwLinkList          llWorldSectorsInAtomic;

    /* The Atomic object pipeline for this Atomic */
    RxPipeline         *pipeline;
};
#endif /* (!defined(DOXYGEN)) */

/**
 * \ingroup rpatomic
 * \ref RpAtomicCallBack represents the function called from \ref RpWorldSectorForAllAtomics and
 * \ref RpClumpForAllAtomics for all atomics in a given world sector or clump.
 * This function should return a pointer to the current atomic to indicate
 * success. The callback may return NULL to terminate further callbacks on
 * the world sector.
 *
 * \param  atomic   Pointer to the current atomic, supplied by
 *                  iterator.
 * \param  data     Pointer to developer-defined data structure.
 *
 * \return Returns a pointer to the current atomic
 */
typedef RpAtomic   *(*RpAtomicCallBack) (RpAtomic * atomic, void *data);

typedef struct RpTie RpTie;

/**
 * \ingroup rpatomic
 * \struct RpTie
 *
 * RpTie is a linked list of atomics inside world sectors. These are
 * created when frame hierarchies are updated as part of an \ref RwCameraBeginUpdate.
 * This is used for frustum culling atomics by world sector.
 * Creation and destruction of RpTies is internal to the world plugin.
 */
struct RpTie
{
    /* Information for an atomic sector */
    RwLLLink            lAtomicInWorldSector; /**< Atomics IN this ATOMIC SECTOR */
    RpAtomic           *apAtom;               /**< An atomic */

    /* Information for a atomic */
    RwLLLink            lWorldSectorInAtomic; /**< Atomic sectors HOLDING this atomic */
    RpWorldSector      *worldSector;          /**< A world sector */
};

typedef struct RpClumpChunkInfo RpClumpChunkInfo;
typedef struct RpClumpChunkInfo _rpClump;

#if (!defined(DOXYGEN))
struct RpClumpChunkInfo
{
    RwInt32             numAtomics;
    RwInt32             numLights;
    RwInt32             numCameras;
};

#endif /* (!defined(DOXYGEN)) */

/****************************************************************************
 <macro/inline functionality

 */

/* NB "RpAtomicRender(atom++) will break it */
#define RpAtomicRenderMacro(_atomic)                                    \
    ((_atomic)->renderCallBack(_atomic))

#define RpAtomicGetGeometryMacro(_atomic)                               \
    ((_atomic)->geometry)

#if (!defined(RpAtomicSetRenderCallBackMacro))

/* NB "RpAtomicSetRenderCallBack(atom++, callback)" will break it */
#define RpAtomicSetRenderCallBackMacro(_atomic, _callback)              \
MACRO_START                                                             \
{                                                                       \
    (_atomic)->renderCallBack = (_callback);                            \
    if (!(_atomic)->renderCallBack)                                     \
    {                                                                   \
        (_atomic)->renderCallBack = AtomicDefaultRenderCallBack;        \
    }                                                                   \
}                                                                       \
MACRO_STOP

#endif /* (!defined(RpAtomicSetRenderCallBackMacro)) */

#define RpAtomicGetRenderCallBackMacro(_atomic)                         \
    ((_atomic)->renderCallBack)

#define RpAtomicGetInterpolatorMacro(_atomic)                         \
    (&((_atomic)->interpolator))

#define RpInterpolatorGetStartMorphTargetMacro(_intrp)                  \
    ((_intrp)->startMorphTarget)

#define RpInterpolatorGetEndMorphTargetMacro(_intrp)                    \
    ((_intrp)->endMorphTarget)

#define RpInterpolatorGetValueMacro(_intrp)                             \
    ((_intrp)->position)

#define RpInterpolatorGetScaleMacro(_intrp)                             \
    ((_intrp)->time)

/* NB "RpInterpolatorSetStartMorphTarget(interp++, target)" will break it */
#define RpInterpolatorSetStartMorphTargetMacro(_intrp, _target, _atomic)\
    ((_intrp)->startMorphTarget = (RwInt16) (_target),                  \
     (_intrp)->flags |= (RwInt32)(rpINTERPOLATORDIRTYINSTANCE |         \
                                  rpINTERPOLATORDIRTYSPHERE     ),      \
     ((!((_intrp)->flags & rpINTERPOLATORNOFRAMEDIRTY))?                \
         ((RpAtomicGetFrame(_atomic))?                                  \
            (RwFrameUpdateObjects(RpAtomicGetFrame(_atomic))):          \
            (0)):                                                       \
         (0)),                                                          \
     (_intrp))

/* NB "RpInterpolatorSetEndMorphTarget(interp++, target)" will break it */
#define RpInterpolatorSetEndMorphTargetMacro(_intrp, _target, _atomic)  \
    ((_intrp)->endMorphTarget = (RwInt16) (_target),                    \
     (_intrp)->flags |= (RwInt32)(rpINTERPOLATORDIRTYINSTANCE |         \
                                  rpINTERPOLATORDIRTYSPHERE     ),      \
     ((!((_intrp)->flags & rpINTERPOLATORNOFRAMEDIRTY))?                \
         ((RpAtomicGetFrame(_atomic))?                                  \
            (RwFrameUpdateObjects(RpAtomicGetFrame(_atomic))):          \
            (0)):                                                       \
         (0)),                                                          \
     (_intrp))

/* NB "RpInterpolatorSetValue(interp++, value)" will break it */
#define RpInterpolatorSetValueMacro(_intrp, _value, _atomic)            \
    ((_intrp)->position = (_value),                                     \
     (_intrp)->flags |= (RwInt32)(rpINTERPOLATORDIRTYINSTANCE |         \
                                  rpINTERPOLATORDIRTYSPHERE     ),      \
     ((!((_intrp)->flags & rpINTERPOLATORNOFRAMEDIRTY))?                \
         ((RpAtomicGetFrame(_atomic))?                                  \
            (RwFrameUpdateObjects(RpAtomicGetFrame(_atomic))):          \
            (0)):                                                       \
         (0)),                                                          \
     (_intrp))

/* NB "RpInterpolatorSetScale(interp++, *(scale++))" will break it */
#define RpInterpolatorSetScaleMacro(_intrp, _scale, _atomic)            \
    ((_intrp)->time = (_scale),                                         \
     (_intrp)->recipTime = (RwReal) (1.0) / (_scale),                   \
     (_intrp)->flags |= (RwInt32)(rpINTERPOLATORDIRTYINSTANCE |         \
                                  rpINTERPOLATORDIRTYSPHERE     ),      \
     ((!((_intrp)->flags & rpINTERPOLATORNOFRAMEDIRTY))?                \
         ((RpAtomicGetFrame(_atomic))?                                  \
            (RwFrameUpdateObjects(RpAtomicGetFrame(_atomic))):          \
            (0)):                                                       \
         (0)),                                                          \
     (_intrp))

#define RpAtomicGetClumpMacro(_atomic)                                  \
    ((_atomic)->clump)

/* NB "RpAtomicGetBoundingSphere(atomic++)" will break it */
#define RpAtomicGetBoundingSphereMacro(_atomic)                         \
    ((((_atomic)->interpolator.flags & rpINTERPOLATORDIRTYSPHERE)?      \
      _rpAtomicResyncInterpolatedSphere(_atomic), 0: 0),                \
      &((_atomic)->boundingSphere))
#define RpAtomicGetFrameMacro(_atomic)                                  \
    ((RwFrame *) rwObjectGetParent(_atomic))

/* NB "RpClumpSetFrame(clump++, frame)" will break it */
#if (!defined(RpClumpSetFrameMacro))
#define RpClumpSetFrameMacro(_clump, _frame)                            \
    (rwObjectSetParent(_clump, _frame),                                 \
     (_clump))
#endif /* (!defined(RpClumpSetFrameMacro)) */

#if (!defined(RpClumpSetFrameVoidMacro))
#define RpClumpSetFrameVoidMacro(_clump, _frame)                        \
MACRO_START                                                             \
{                                                                       \
    rwObjectSetParent(_clump, _frame);                                  \
}                                                                       \
MACRO_STOP
#endif /* (!defined(RpClumpSetFrameVoidMacro)) */

#define RpClumpGetFrameMacro(_clump)                                    \
    ((RwFrame *) rwObjectGetParent(_clump))

/* NB "RpAtomicSetFlags(atomic++, flags)" will break it */
#if (!defined(RpAtomicSetFlagsMacro))
#define RpAtomicSetFlagsMacro(_atomic, _flags)                          \
    (rwObjectSetFlags(_atomic, _flags),                                 \
     (_atomic))
#endif /* (!defined(RpAtomicSetFlagsMacro)) */

#define RpAtomicGetFlagsMacro(_atomic)                                  \
    (rwObjectGetFlags(_atomic))

#if (! ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ))

#define RpAtomicRender(_atomic) \
    RpAtomicRenderMacro(_atomic)

#define RpAtomicGetGeometry(_atomic) \
    RpAtomicGetGeometryMacro(_atomic)

#define RpAtomicSetRenderCallBack(_atomic, _callback) \
    RpAtomicSetRenderCallBackMacro(_atomic, _callback)

#define RpAtomicGetRenderCallBack(_atomic) \
    RpAtomicGetRenderCallBackMacro(_atomic)

#define RpAtomicGetInterpolator(_atomic) \
    RpAtomicGetInterpolatorMacro(_atomic)

#define RpInterpolatorGetStartMorphTarget(_intrp) \
    RpInterpolatorGetStartMorphTargetMacro(_intrp)

#define RpInterpolatorGetEndMorphTarget(_intrp) \
    RpInterpolatorGetEndMorphTargetMacro(_intrp)

#define RpInterpolatorGetValue(_intrp) \
    RpInterpolatorGetValueMacro(_intrp)

#define RpInterpolatorGetScale(_intrp) \
    RpInterpolatorGetScaleMacro(_intrp)

#define RpInterpolatorSetStartMorphTarget(_intrp, _target, _atomic) \
    RpInterpolatorSetStartMorphTargetMacro(_intrp, _target, _atomic)

#define RpInterpolatorSetEndMorphTarget(_intrp, _target, _atomic) \
    RpInterpolatorSetEndMorphTargetMacro(_intrp, _target, _atomic)

#define RpInterpolatorSetValue(_intrp, _value, _atomic) \
    RpInterpolatorSetValueMacro(_intrp, _value, _atomic)

#define RpInterpolatorSetScale(_intrp, _scale, _atomic) \
    RpInterpolatorSetScaleMacro(_intrp, _scale, _atomic)

#define RpAtomicGetClump(_atomic) \
    RpAtomicGetClumpMacro(_atomic)

#define RpAtomicGetBoundingSphere(_atomic) \
    RpAtomicGetBoundingSphereMacro(_atomic)

#define RpAtomicGetFrame(_atomic) \
    RpAtomicGetFrameMacro(_atomic)

#define RpClumpSetFrame(_clump, _frame) \
    RpClumpSetFrameMacro(_clump, _frame)

#define RpClumpGetFrame(_clump) \
    RpClumpGetFrameMacro(_clump)

#define RpAtomicSetFlags(_atomic, _flags) \
    RpAtomicSetFlagsMacro(_atomic, _flags)

#define RpAtomicGetFlags(_atomic) \
    RpAtomicGetFlagsMacro(_atomic)

#endif /* (! ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) )) */

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

/* RWPUBLICEND */

/* Opening/Closing */
extern void *
_rpClumpOpen(void *instance,
             RwInt32 offset,
             RwInt32 size);
extern void *
_rpClumpClose(void *instance,
             RwInt32 offset,
             RwInt32 size);

extern RwStream *_rpReadAtomicRights(RwStream *s, RwInt32 len, void *obj,
                                     RwInt32 off, RwInt32 size);
extern RwStream *_rpWriteAtomicRights(RwStream *s, RwInt32 len, const void *obj,
                                      RwInt32 off, RwInt32 size);
extern RwInt32 _rpSizeAtomicRights(const void *obj, RwInt32 off, RwInt32 size);

extern RwBool _rpClumpRegisterExtensions(void);

/* RWPUBLIC */

extern void
RpAtomicSetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc );

extern void
RpClumpSetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc );

/* Macro version of RpAtomicSetRenderCallBack needs this */
extern RpAtomic *
AtomicDefaultRenderCallBack(RpAtomic * atomic);

extern void
_rpAtomicResyncInterpolatedSphere(RpAtomic * atomic);

extern const RwSphere *
RpAtomicGetWorldBoundingSphere(RpAtomic *  atomic);

/* Enumeration */
extern RpClump *
RpClumpForAllAtomics(RpClump * clump,
                     RpAtomicCallBack callback,
                     void *pData);

extern RpClump *
RpClumpForAllLights(RpClump * clump,
                    RpLightCallBack callback,
                    void *pData);

extern RpClump *
RpClumpForAllCameras(RpClump * clump,
                     RwCameraCallBack callback,
                     void *pData);

/* Frames */
extern RpAtomic *
RpAtomicSetFrame(RpAtomic * atomic,
                 RwFrame * frame);

/* Create a space marking clump */
extern RpClump *
RpClumpCreateSpace(const RwV3d * position,
                   RwReal radius);

/* Instancing and rendering */
extern RpClump *
RpClumpRender(RpClump * clump);

extern RpClump *
RpClumpRemoveAtomic(RpClump * clump,
                    RpAtomic * atomic);

extern RpClump *
RpClumpAddAtomic(RpClump * clump,
                 RpAtomic * atomic);

extern RpClump *
RpClumpRemoveLight(RpClump * clump,
                   RpLight * light);

extern RpClump *
RpClumpAddLight(RpClump * clump,
                RpLight * light);

extern RpClump *
RpClumpRemoveCamera(RpClump * clump,
                    RwCamera * camera);

extern RpClump *
RpClumpAddCamera(RpClump * clump,
                 RwCamera * camera);

/* Creation and destruction of clumps */
extern RwBool
RpClumpDestroy(RpClump * clump);

extern RpClump *
RpClumpCreate(void);

extern RpClump *
RpClumpClone(RpClump * clump);

/* Creation and destruction of atomics*/
extern RwBool
RpAtomicDestroy(RpAtomic * atomic);

extern RpAtomic *
RpAtomicClone(RpAtomic * atomic);

extern RpAtomic *
RpAtomicCreate(void);

/* Setting and getting geometry for an atomic */
extern RpAtomic *
RpAtomicSetGeometry(RpAtomic * atomic,
                    RpGeometry * geometry,
                    RwUInt32 flags);

/* Frustum callbacks */
extern RpClump *
RpClumpSetCallBack(RpClump * clump,
                   RpClumpCallBack callback);

extern RpClumpCallBack
RpClumpGetCallBack(const RpClump * clump);

/* The number of atomics in a clump */
extern RwInt32
RpClumpGetNumAtomics(RpClump * clump);

extern RwInt32
RpClumpGetNumLights(RpClump * clump);

extern RwInt32
RpClumpGetNumCameras(RpClump * clump);

/* Light and camera extensions */
extern RpClump *
RpLightGetClump(const RpLight *light);

extern RpClump *
RwCameraGetClump(const RwCamera *camera);

/* Binary format */
extern RwUInt32
RpAtomicStreamGetSize(RpAtomic * atomic);

extern RpAtomic *
RpAtomicStreamRead(RwStream * stream);

extern RpAtomic *
RpAtomicStreamWrite(RpAtomic * atomic,
                    RwStream * stream);

extern RwUInt32
RpClumpStreamGetSize(RpClump * clump);

extern RpClump *
RpClumpStreamRead(RwStream * stream);

extern RpClump *
RpClumpStreamWrite(RpClump * clump,
                   RwStream * stream);

extern RpClumpChunkInfo *
_rpClumpChunkInfoRead(RwStream * stream,
                      RpClumpChunkInfo * clumpChunkInfo,
                      RwInt32 * bytesRead);

/* Attaching toolkits */
extern RwInt32
RpAtomicRegisterPlugin(RwInt32 size,
                       RwUInt32 pluginID,
                       RwPluginObjectConstructor constructCB,
                       RwPluginObjectDestructor destructCB,
                       RwPluginObjectCopy copyCB);

extern RwInt32
RpClumpRegisterPlugin(RwInt32 size,
                      RwUInt32 pluginID,
                      RwPluginObjectConstructor constructCB,
                      RwPluginObjectDestructor destructCB,
                      RwPluginObjectCopy copyCB);

extern RwInt32
RpAtomicRegisterPluginStream(RwUInt32 pluginID,
                             RwPluginDataChunkReadCallBack
                             readCB,
                             RwPluginDataChunkWriteCallBack
                             writeCB,
                             RwPluginDataChunkGetSizeCallBack
                             getSizeCB);

extern RwInt32
RpAtomicSetStreamAlwaysCallBack(RwUInt32 pluginID,
                                     RwPluginDataChunkAlwaysCallBack alwaysCB);

extern RwInt32
RpAtomicSetStreamRightsCallBack(RwUInt32 pluginID,
                                     RwPluginDataChunkRightsCallBack rightsCB);

extern RwInt32
RpClumpRegisterPluginStream(RwUInt32 pluginID,
                            RwPluginDataChunkReadCallBack  readCB,
                            RwPluginDataChunkWriteCallBack writeCB,
                            RwPluginDataChunkGetSizeCallBack getSizeCB);

extern RwInt32
RpClumpSetStreamAlwaysCallBack(RwUInt32 pluginID,
                                    RwPluginDataChunkAlwaysCallBack alwaysCB);

extern RwInt32
RpAtomicGetPluginOffset(RwUInt32 pluginID);

extern RwInt32
RpClumpGetPluginOffset(RwUInt32 pluginID);

extern RwBool
RpAtomicValidatePlugins(const RpAtomic * atomic);

extern RwBool
RpClumpValidatePlugins(const RpClump * clump);

#if ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) )
extern RwFrame *
RpAtomicGetFrame(const RpAtomic * atomic);

extern RwFrame *
RpClumpGetFrame(const RpClump * clump);

extern RpClump *
RpClumpSetFrame(RpClump * clump,
                RwFrame * frame);

/* Flags */
extern RpAtomic *
RpAtomicSetFlags(RpAtomic * atomic,
                 RwUInt32 flags);

extern RwUInt32
RpAtomicGetFlags(const RpAtomic * atomic);

extern RwSphere *
RpAtomicGetBoundingSphere(RpAtomic * atomic);

extern RwInt32
RpInterpolatorGetEndMorphTarget(const RpInterpolator * interpolator);

extern RwInt32
RpInterpolatorGetStartMorphTarget(const RpInterpolator * interpolator);

extern RwReal
RpInterpolatorGetValue(const RpInterpolator * interpolator);

extern RwReal
RpInterpolatorGetScale(const RpInterpolator * interpolator);

extern RpInterpolator *
RpInterpolatorSetEndMorphTarget(RpInterpolator * interpolator,
                                RwInt32 morphTarget,
                                RpAtomic * atomic);

extern RpInterpolator *
RpInterpolatorSetStartMorphTarget(RpInterpolator * interpolator,
                                  RwInt32 morphTarget,
                                  RpAtomic * atomic);

extern RpInterpolator *
RpInterpolatorSetValue(RpInterpolator *  interpolator,
                       RwReal value,
                       RpAtomic *atomic);

extern RpInterpolator *
RpInterpolatorSetScale(RpInterpolator * interpolator,
                       RwReal scale,
                       RpAtomic *atomic);

extern RpAtomic *
RpAtomicRender(RpAtomic * atomic);

/* Building clumps */
extern RpClump *
RpAtomicGetClump(const RpAtomic * atomic);

extern RpInterpolator *
RpAtomicGetInterpolator(RpAtomic * atomic);

extern RpGeometry *
RpAtomicGetGeometry(const RpAtomic * atomic);

extern void
RpAtomicSetRenderCallBack(RpAtomic * atomic,
                          RpAtomicCallBackRender callback);

extern RpAtomicCallBackRender
RpAtomicGetRenderCallBack(const RpAtomic * atomic);

#endif

/* ( defined(RWDEBUG) || defined(RWSUPPRESSINLINE) ) */

extern RwBool RpAtomicInstance(RpAtomic *atomic);

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

#define RpClumpChunkInfoRead(stream, clumpChunkInfo, bytesRead) \
       _rpClumpChunkInfoRead(stream, clumpChunkInfo, bytesRead)

/* RWPUBLICEND */

#endif                          /* RWCLUMP_H */

