/***************************************************************************
 * Module  : bacamera.h                                                    *
 *                                                                         *
 * Purpose : Camera functions                                              *
 *                                                                         *
 **************************************************************************/

#ifndef RWCAMERA_H
#define RWCAMERA_H

/****************************************************************************
 Includes
 */

#include "bamemory.h"
#include "batypehf.h"
#include "bamatrix.h"
#include "baraster.h"
#include "babbox.h"
#include "baframe.h"
#include "bacolor.h"

/* RWPUBLIC */

/****************************************************************************
 Defines
 */

/* Type ID */
#define rwCAMERA 4

/* RWPUBLICEND */

/* Used by some drivers for overdraw frustums */

#if defined(__GNUC__) \
    && ((__GNUC__ > 2 ) || (__GNUC__ == 2) && (__GNUC_MINOR__ == 96))

#define RWPLANESETCLOSEST(p)                            \
    (p).closestX = (((p).plane.normal.x)>=((RwReal)0)); \
    (p).closestY = (((p).plane.normal.y)>=((RwReal)0)); \
    (p).closestZ = (((p).plane.normal.z)>=((RwReal)0))

#else
/* This version is not -fstrict-alias safe */
/* 1 Bll 0 Fur */

#define RWPLANESETCLOSEST(p)                                                 \
    (p).closestX = (RwUInt8)(((*(RwInt32 *)(&(p).plane.normal.x))>>31) +1);  \
    (p).closestY = (RwUInt8)(((*(RwInt32 *)(&(p).plane.normal.y))>>31) +1);  \
    (p).closestZ = (RwUInt8)(((*(RwInt32 *)(&(p).plane.normal.z))>>31) +1)

#endif

/* RWPUBLIC */


/****************************************************************************
 <macro/inline functionality
 */

#define RwCameraGetViewOffsetMacro(_camera)                     \
    (&((_camera)->viewOffset))

#define RwCameraSetRasterMacro(_camera, _raster)                \
    (((_camera)->frameBuffer = (_raster)), (_camera))

#define RwCameraSetRasterVoidMacro(_camera, _raster)            \
MACRO_START                                                     \
{                                                               \
    (_camera)->frameBuffer = (_raster);                         \
}                                                               \
MACRO_STOP

#define RwCameraGetRasterMacro(_camera)                         \
    ((_camera)->frameBuffer)

#define RwCameraSetZRasterMacro(_camera, _raster)               \
    (((_camera)->zBuffer = (_raster)), (_camera))

#define RwCameraSetZRasterVoidMacro(_camera, _raster)           \
MACRO_START                                                     \
{                                                               \
    (_camera)->zBuffer = (_raster);                             \
}                                                               \
MACRO_STOP

#define RwCameraGetZRasterMacro(_camera)                        \
    ((_camera)->zBuffer)

#define RwCameraGetNearClipPlaneMacro(_camera)                  \
    ((_camera)->nearPlane)

#define RwCameraGetFarClipPlaneMacro(_camera)                   \
    ((_camera)->farPlane)

#define RwCameraSetFogDistanceMacro(_camera, _distance)         \
    (((_camera)->fogPlane = (_distance)), (_camera))

#define RwCameraGetFogDistanceMacro(_camera)                    \
    ((_camera)->fogPlane)

#define RwCameraGetCurrentCameraMacro()                         \
    ((RwCamera *)RWSRCGLOBAL(curCamera))

#define RwCameraGetProjectionMacro(_camera)                     \
    ((_camera)->projectionType)

#define RwCameraGetViewWindowMacro(_camera)                     \
    (&((_camera)->viewWindow))

#define RwCameraGetViewMatrixMacro(_camera)                     \
    (&((_camera)->viewMatrix))

#define RwCameraSetFrameMacro(_camera, _frame)                  \
    (_rwObjectHasFrameSetFrame((_camera), (_frame)), (_camera))

#define RwCameraSetFrameVoidMacro(_camera, _frame)      \
MACRO_START                                             \
{                                                       \
    _rwObjectHasFrameSetFrame((_camera), (_frame));     \
}                                                       \
MACRO_STOP


#define RwCameraGetFrameMacro(_camera)                          \
    ((RwFrame *)rwObjectGetParent((_camera)))

#if !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

#define RwCameraGetViewOffset(_camera)                          \
    RwCameraGetViewOffsetMacro(_camera)

#define RwCameraSetRaster(_camera, _raster)                     \
    RwCameraSetRasterMacro(_camera, _raster)

#define RwCameraGetRaster(_camera)                              \
    RwCameraGetRasterMacro(_camera)

#define RwCameraSetZRaster(_camera, _raster)                    \
    RwCameraSetZRasterMacro(_camera, _raster)

#define RwCameraGetZRaster(_camera)                             \
    RwCameraGetZRasterMacro(_camera)

#define RwCameraGetNearClipPlane(_camera)                       \
    RwCameraGetNearClipPlaneMacro(_camera)

#define RwCameraGetFarClipPlane(_camera)                        \
    RwCameraGetFarClipPlaneMacro(_camera)

#define RwCameraSetFogDistance(_camera, _distance)              \
    RwCameraSetFogDistanceMacro(_camera, _distance)

#define RwCameraGetFogDistance(_camera)                         \
    RwCameraGetFogDistanceMacro(_camera)

#define RwCameraGetCurrentCamera()                              \
    RwCameraGetCurrentCameraMacro()

#define RwCameraGetProjection(_camera)                          \
    RwCameraGetProjectionMacro(_camera)

#define RwCameraGetViewWindow(_camera)                          \
    RwCameraGetViewWindowMacro(_camera)

#define RwCameraGetViewMatrix(_camera)                          \
    RwCameraGetViewMatrixMacro(_camera)

#define RwCameraSetFrame(_camera, _frame)                       \
    RwCameraSetFrameMacro(_camera, _frame)

#define RwCameraGetFrame(_camera)                               \
    RwCameraGetFrameMacro(_camera)

#endif /* !(defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */


/****************************************************************************
 Global Types
 */

/**
 * \ingroup rwcamera
 * RwCameraClearMode 
 * Camera clear flags */
enum RwCameraClearMode
{
    rwCAMERACLEARIMAGE = 0x1,   /**<Clear the frame buffer */
    rwCAMERACLEARZ = 0x2,       /**<Clear the Z buffer */
    rwCAMERACLEARSTENCIL = 0x4, /**<\if xbox   Clear the stencil buffer \endif
                                  * \if d3d8   Clear the stencil buffer \endif
                                  * \if d3d9   Clear the stencil buffer \endif
                                  * \if opengl Clear the stencil buffer \endif
                                  */
    rwCAMERACLEARMODEFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwCameraClearMode RwCameraClearMode;

/**
 * \ingroup rwcamera
 * RwCameraProjection 
 * This type represents the options available for 
 * setting the camera projection model, either perspective projection or 
* parallel projection (see API function \ref RwCameraSetProjection)*/
enum RwCameraProjection
{
    rwNACAMERAPROJECTION = 0,   /**<Invalid projection */
    rwPERSPECTIVE = 1,          /**<Perspective projection */
    rwPARALLEL = 2,             /**<Parallel projection */
    rwCAMERAPROJECTIONFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwCameraProjection RwCameraProjection;

/**
 * \ingroup rwcamera
 * RwFrustumTestResult 
 * This type represents the results from a 
 * camera frustum test on a given sphere (see API function 
 * \ref RwCameraFrustumTestSphere)*/
enum RwFrustumTestResult
{
    rwSPHEREOUTSIDE = 0,    /**<Outside the frustum */
    rwSPHEREBOUNDARY = 1,   /**<On the boundary of the frustum */
    rwSPHEREINSIDE = 2,     /**<Inside the frustum */
    rwFRUSTUMTESTRESULTFORCEENUMSIZEINT = RWFORCEENUMSIZEINT
};
typedef enum RwFrustumTestResult RwFrustumTestResult;

/* RWPUBLICEND */

typedef struct rwCameraGlobals rwCameraGlobals;
struct rwCameraGlobals
{
        RwFreeList         *cameraFreeList;
};

/* RWPUBLIC */

/**
 * \ingroup rwcamera
 * \struct RwCamera
 * Camera object for rendering a view. 
 * This should be considered an opaque type.
 * Use the RwCamera API functions to access.
 */
typedef struct RwCamera RWALIGN(RwCamera, rwMATRIXALIGNMENT);

/* This allows a world to overload functionality */
typedef RwCamera   *(*RwCameraBeginUpdateFunc) (RwCamera * camera);
typedef RwCamera   *(*RwCameraEndUpdateFunc) (RwCamera * camera);

typedef struct RwFrustumPlane RwFrustumPlane;

#if (!defined(DOXYGEN))
/*
 * Structure describing a frustrum plane.
 */
struct RwFrustumPlane
{
        RwPlane             plane;
        RwUInt8             closestX;
        RwUInt8             closestY;
        RwUInt8             closestZ;
        RwUInt8             pad;
};


struct RwCamera
{
        RwObjectHasFrame    object;

        /* Parallel or perspective projection */
        RwCameraProjection  projectionType;

        /* Start/end update functions */
        RwCameraBeginUpdateFunc beginUpdate;
        RwCameraEndUpdateFunc endUpdate;

        /* The view matrix */
        RwMatrix            viewMatrix;

        /* The cameras image buffer */
        RwRaster           *frameBuffer;

        /* The Z buffer */
        RwRaster           *zBuffer;

        /* Cameras mathmatical characteristics */
        RwV2d               viewWindow;
        RwV2d               recipViewWindow;
        RwV2d               viewOffset;
        RwReal              nearPlane;
        RwReal              farPlane;
        RwReal              fogPlane;

        /* Transformation to turn camera z or 1/z into a Z buffer z */
        RwReal              zScale, zShift;

        /* The clip-planes making up the viewing frustum */
        RwFrustumPlane      frustumPlanes[6];
        RwBBox              frustumBoundBox;

        /* Points on the tips of the view frustum */
        RwV3d               frustumCorners[8];
};
#endif /* (!defined(DOXYGEN)) */

/**
 * \ingroup rwcamera
 * \ref RwCameraCallBack type represents a function called from any camera
 * iterator that may be implemented in plugins. This function should return a
 * pointer to the current camera to indicate success. The callback may return
 * NULL to terminate further callbacks on other cameras.
 * 
 * \param  camera   Pointer to the current camera, supplied by iterator.
 * \param  data  Pointer to developer-defined data structure.
 *
 * \return Pointer to the current camera, or NULL If not found.
 */
typedef RwCamera *(*RwCameraCallBack)(RwCamera *camera, void *data);

/* RWPUBLICEND */

/****************************************************************************
 Globals across the program
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

extern RwPluginRegistry cameraTKList;

#ifdef    __cplusplus
}
#endif                          /* __cplusplus */

/* RWPUBLIC */

/****************************************************************************
 Function prototypes
 */

#ifdef    __cplusplus
extern              "C"
{
#endif                          /* __cplusplus */

    /* Rendering */
extern RwCamera    *RwCameraBeginUpdate(RwCamera * camera);
extern RwCamera    *RwCameraEndUpdate(RwCamera * camera);

extern RwCamera    *RwCameraClear(RwCamera * camera, RwRGBA * colour,
                                  RwInt32 clearMode);

/* Displaying results */
extern RwCamera    *RwCameraShowRaster(RwCamera * camera, void *pDev,
                                       RwUInt32 flags);

/* Creation and destruction */
extern void RwCameraSetFreeListCreateParams( RwInt32 blockSize,
                                            RwInt32 numBlocksToPrealloc );
extern RwBool       RwCameraDestroy(RwCamera * camera);
extern RwCamera    *RwCameraCreate(void);
extern RwCamera    *RwCameraClone(RwCamera * camera);

/* Offset */
extern RwCamera    *RwCameraSetViewOffset(RwCamera *camera,
                                          const RwV2d *offset);

/* View window */
extern RwCamera    *RwCameraSetViewWindow(RwCamera *camera,
                                          const RwV2d *viewWindow);

/* Projection */
extern RwCamera    *RwCameraSetProjection(RwCamera *camera,
                                          RwCameraProjection projection);

/* Clip planes */
extern RwCamera    *RwCameraSetNearClipPlane(RwCamera *camera, RwReal nearClip);
extern RwCamera    *RwCameraSetFarClipPlane(RwCamera *camera, RwReal farClip);

/* Attaching toolkits */
extern RwInt32      RwCameraRegisterPlugin(RwInt32 size,
                                           RwUInt32 pluginID,
                                           RwPluginObjectConstructor
                                           constructCB,
                                           RwPluginObjectDestructor
                                           destructCB,
                                           RwPluginObjectCopy copyCB);
extern RwInt32      RwCameraGetPluginOffset(RwUInt32 pluginID);
extern RwBool       RwCameraValidatePlugins(const RwCamera * camera);

/* Frustum testing */
extern RwFrustumTestResult RwCameraFrustumTestSphere(const RwCamera *
                                                     camera,
                                                     const RwSphere *
                                                     sphere);

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/* Offset */
extern const RwV2d *RwCameraGetViewOffset(const RwCamera *camera);

/* Rasters */
extern RwCamera    *RwCameraSetRaster(RwCamera *camera, RwRaster *raster);
extern RwRaster    *RwCameraGetRaster(const RwCamera *camera);
extern RwCamera    *RwCameraSetZRaster(RwCamera *camera, RwRaster *zRaster);
extern RwRaster    *RwCameraGetZRaster(const RwCamera *camera);

/* Clip planes */
extern RwReal       RwCameraGetNearClipPlane(const RwCamera *camera);
extern RwReal       RwCameraGetFarClipPlane(const RwCamera *camera);
extern RwCamera    *RwCameraSetFogDistance(RwCamera *camera, RwReal fogDistance);
extern RwReal       RwCameraGetFogDistance(const RwCamera *camera);

extern RwCamera    *RwCameraGetCurrentCamera(void);

/* Projection */
extern RwCameraProjection RwCameraGetProjection(const RwCamera *camera);

/* View window */
extern const RwV2d *RwCameraGetViewWindow(const RwCamera *camera);

extern RwMatrix    *RwCameraGetViewMatrix(RwCamera *camera);

/* Frames */
extern RwCamera    *RwCameraSetFrame(RwCamera *camera, RwFrame *frame);
extern RwFrame     *RwCameraGetFrame(const RwCamera *camera);
#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/* RWPUBLICEND */

/* Open closing */
extern void        *_rwCameraClose(void *instance, RwInt32 offset,
                                   RwInt32 size);
extern void        *_rwCameraOpen(void *instance, RwInt32 offset,
                                  RwInt32 size);

/* RWPUBLIC */
#ifdef    __cplusplus
}
#endif                          /* __cplusplus */
/* RWPUBLICEND */

#endif                          /* RWCAMERA_H */
