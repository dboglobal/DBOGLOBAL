/*
 * Cameras define how and what things can be seen. They also define the
 * depth and width of the view by the use of clip-planes and the view
 * window.
 */

/**
 * \ingroup rwcamera
 * \page rwcameraoverview RwCamera Overview
 *
 * \par Requirements
 * \li \b Headers: rwcore.h
 * \li \b Libraries: rwcore.lib
 *
 * \subsection cameraoverview Overview
 *
 * RenderWare Graphics utilizes a standard virtual camera model and this
 * object represents the virtual camera. Usually, a camera object
 * requires a frame and two rasters. The frame is required so the camera
 * can be positioned within world space.
 *
 * The rasters will usually be of type \ref rwRASTERTYPECAMERA and
 * \ref rwRASTERTYPEZBUFFER, representing the frame buffer and Z buffer
 * respectively. Alternatively, a camera can reference a sub-raster
 * stored within another camera to allow split-screen or
 * picture-in-picture rendering.
 *
 * \par Creating a Camera
 * A camera is typically created as follows:
 * -# Create and allocate a raster of type \ref rwRASTERTYPECAMERA
 * -# Create and allocate another raster of type \ref rwRASTERTYPEZBUFFER
 * -# Create a camera object using \ref RwCameraCreate
 * -# Attach the two rasters to the camer object using \ref RwCameraSetRaster
 *    and \ref RwCameraSetZRaster
 * -# Create a frame obejct and attact the frame to the camera using
 *    \ref RwCameraSetFrame
 *
 * \par Rendering to a Texture
 * RenderWare Graphics allows rendering to a texture.  This
 * requires using a frame buffer raster of type
 * \ref rwRASTERTYPECAMERATEXTURE.  In addition, rendering to a texture
 * functionality may require platform-specific API calls - you should
 * check any platform-specific RenderWare Graphics documentation provided
 * for more information on this feature.
 *
 * \par More Information
 * More information can be found in the \e Cameras chapter of the User
 * Guide.
 *
 */

/****************************************************************************
 Includes
 */

#include <stdlib.h>

#include "batypes.h"
#include "batypehf.h"
#include "balibtyp.h"
#include "bamemory.h"
#include "basync.h"
#include "badebug.h"
#include "batkreg.h"
#include "bamatrix.h"
#include "bavector.h"
#include "batype.h"

#include "batkreg.h"
#include "badevice.h"
#include "baimmedi.h"
#include "baresour.h"
#include "batextur.h"

/*
 * "bapipe.h" requires
 * typedef struct _RwTexture RwTexture
 * from  "batextur.h"
 */
#include "bacamera.h"
#include "batextur.h"
#include "bapipe.h"

#if (defined(RWEVALUATION))
#include "bacamval.h"
#endif /* (defined(RWEVALUATION)) */

/****************************************************************************
 Local Types
 */

/****************************************************************************
 Local (Static) Prototypes
 */

/****************************************************************************
 Local Defines
 */

#define RWCAMERAGLOBAL(var)                     \
   (RWPLUGINOFFSET(rwCameraGlobals,             \
    RwEngineInstance,                           \
    cameraModule.globalsOffset)->var)

#if (defined(RWDEBUG))
#define MATRIXISORTHONORMAL(result,mpMatrix)                            \
MACRO_START                                                             \
{                                                                       \
    RwMatrixTolerance tolerance;                                        \
    RwEngineGetMatrixTolerances(&tolerance);                            \
    result = (mpMatrix) && (tolerance.Normal >=                         \
                          _rwMatrixNormalError((mpMatrix)));            \
    result &=(mpMatrix) && (tolerance.Orthogonal >=                     \
                          _rwMatrixOrthogonalError((mpMatrix)));       \
}                                                                       \
MACRO_STOP
#endif /* (defined(RWDEBUG))     */

/****************************************************************************
 Globals (across program)
 */

RwPluginRegistry    cameraTKList =
    { sizeof(RwCamera),
      sizeof(RwCamera),
      0,
      0,
      (RwPluginRegEntry *)NULL,
      (RwPluginRegEntry *)NULL };

/****************************************************************************
 Local (static) Globals
 */

static RwModuleInfo cameraModule;

#ifdef RWDEBUG
static RwBool inCameraBeginEndUpdate;
#endif /* RWDEBUG */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Creating/Destruction

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/***************************************************************************
 _CameraUpdateZShiftScale

 Updates the transformation taking camera Z of 1/Z to screen space Z buffer Z

 On entry   : Camera
 On exit    : Camera pointer on success
 */

static RwCamera    *
CameraUpdateZShiftScale(RwCamera * camera)
{
    RwReal              zOutInf = RwIm2DGetNearScreenZ();
    RwReal              zOutSup = RwIm2DGetFarScreenZ();
    RwReal              zInInf;
    RwReal              zInSup;

    RWFUNCTION(RWSTRING("CameraUpdateZShiftScale"));
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    /* Find the input range */
    switch (camera->projectionType)
    {
        case (rwPARALLEL):
            {
                zInSup = camera->farPlane;
                zInInf = camera->nearPlane;
                break;
            }

        default:
            /* Should never happen, but do the perspective thing if it does */
        case (rwPERSPECTIVE):
            {
                zInSup = ((RwReal)1) / (camera->farPlane);
                zInInf = ((RwReal)1) / (camera->nearPlane);
                break;
            }
    }

    {
        RwReal              zScale;
        RwReal              zShift;

        /* We don't have the resolution to calculate the scale and shift
         * for all the resolution;
         * I have tried repeatedly to get code that does it OK,
         * and have failed.
         * Therefore, we create some buffer zones at each end
         * of the Z buffer to cope with the inaccuracies.
         * Say goodbye to 0.02% of the Z buffer range!
         */

        RwSplitBits         zInWid;
        RwReal              zOutWid = zOutSup - zOutInf;
        const RwReal        zDelta = zOutWid * ((RwReal) 0.0001);


        zOutSup = zOutSup - zDelta;
        zOutInf = zOutInf + zDelta;

        zOutWid = zOutSup - zOutInf;

        zInWid.nReal = zInSup - zInInf;
        RWASSERT(0 != zInWid.nInt);

        /* Set up mapping between the ranges */

        zScale = (zOutWid / zInWid.nReal);
        zShift = ( ( (RwReal)0.5) *
                   ( (zOutSup + zOutInf) - zScale * (zInSup + zInInf) ) );

        camera->zScale = zScale;
        camera->zShift = zShift;

    }


    RWRETURN(camera);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Finding the cameras viewing frustum

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 *  _CameraBuildPerspClipPlanes
 *
 *  Top View (looking neg y)
 *
 *           8     5
 *                 |
 *                /|
 *       4     1 / |    The right side
 *              /  |
 *             |   |
 *            /|   |
 *          ./ |   |              ----> Look at
 *           \ |   |
 *            \|   |                beneath above
 *             |   |
 *       3     2\  |
 *               \ |    The left side
 *                \|
 *                 |
 *           7     6
 *
 *  On entry   : Camera
 *
 *  On exit    : 6 planes making up the viewing frustum
 *             : Bounding box bll
 *             : Bounding box fur
 */

static void
CameraBuildPerspClipPlanes(RwCamera * camera)
{
    RwV3d               vTmp, vTmp2, vRight, vUp, vCOP;
    RwMatrix           *cameraLTM =
        _rwFrameGetLTM((RwFrame *) rwObjectGetParent(camera));
    RwReal              recip;
    RwReal              scale;
    RwInt32             i;
    RwV3d              *frustumVerts = camera->frustumCorners;
    RwFrustumPlane     *frustumPlanes = camera->frustumPlanes;

    RWFUNCTION(RWSTRING("CameraBuildPerspClipPlanes"));
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    /* COP */
    scale = -camera->viewOffset.x;
    RwV3dScaleMacro(&vCOP, &cameraLTM->right, scale);
    RwV3dIncrementScaledMacro(&vCOP, &cameraLTM->up, camera->viewOffset.y);

    /* get to top corner of viewwindow */
    RwV3dScaleMacro(&vRight, &cameraLTM->right, camera->viewWindow.x);
    RwV3dAddMacro(&vTmp, &cameraLTM->at, &vRight);

    RwV3dScaleMacro(&vUp, &cameraLTM->up, camera->viewWindow.y);
    RwV3dAddMacro(&vTmp, &vTmp, &vUp);

    /* now walk perimeter */
    RwV3dScaleMacro(&vRight, &vRight, 2.0f);
    RwV3dScaleMacro(&vUp, &vUp, 2.0f);
    frustumVerts[0] = vTmp;
    RwV3dSubMacro(&vTmp, &vTmp, &vRight);
    frustumVerts[1] = vTmp;
    RwV3dSubMacro(&vTmp, &vTmp, &vUp);
    frustumVerts[2] = vTmp;
    RwV3dAddMacro(&vTmp, &vTmp, &vRight);
    frustumVerts[3] = vTmp;

    /* scale out to near & far clip-planes and translate */
    for (i = 0; i < 4; i++)
    {
        RwV3d              *target;

        /* near */
        target = &frustumVerts[i];
        RwV3dSubMacro(&vTmp, target, &vCOP);
        RwV3dAddMacro(target, &vCOP, &cameraLTM->pos);
        RwV3dIncrementScaledMacro(target, &vTmp, camera->nearPlane);

        /* far */
        target = &frustumVerts[4 + i];
        RwV3dAddMacro(target, &vCOP, &cameraLTM->pos);
        RwV3dIncrementScaledMacro(target, &vTmp, camera->farPlane);
    }

    /* Build the planes */
    /* NB We always use the longest frustum edges
     * to ensure we don't suffer from accuracy problems */

    /* Far clipping plane */
    frustumPlanes[0].plane.normal = cameraLTM->at;
    frustumPlanes[0].plane.distance =
        RwV3dDotProductMacro(&frustumVerts[4], &cameraLTM->at);
    RWPLANESETCLOSEST(frustumPlanes[0]);

    /* Near clipping plane */
    RwV3dNegateMacro(&frustumPlanes[1].plane.normal,
                     &frustumPlanes[0].plane.normal);
    frustumPlanes[1].plane.distance =
        RwV3dDotProductMacro(&frustumVerts[0],
                             &frustumPlanes[1].plane.normal);
    RWPLANESETCLOSEST(frustumPlanes[1]);

    /* Left plane */
    RwV3dSubMacro(&vTmp, &frustumVerts[1], &frustumVerts[5]);
    RwV3dSubMacro(&vTmp2, &frustumVerts[6], &frustumVerts[5]);
    RwV3dCrossProductMacro(&frustumPlanes[2].plane.normal, &vTmp, &vTmp2);
    _rwV3dNormalizeMacro(recip,
                         &frustumPlanes[2].plane.normal,
                         &frustumPlanes[2].plane.normal);
    frustumPlanes[2].plane.distance =
        RwV3dDotProductMacro(&frustumVerts[1],
                             &frustumPlanes[2].plane.normal);
    RWPLANESETCLOSEST(frustumPlanes[2]);

    /* Top plane */
    RwV3dSubMacro(&vTmp2, &frustumVerts[4], &frustumVerts[5]);
    RwV3dCrossProductMacro(&frustumPlanes[3].plane.normal, &vTmp2, &vTmp);
    _rwV3dNormalizeMacro(recip,
                         &frustumPlanes[3].plane.normal,
                         &frustumPlanes[3].plane.normal);
    frustumPlanes[3].plane.distance =
        RwV3dDotProductMacro(&frustumVerts[1],
                             &frustumPlanes[3].plane.normal);
    RWPLANESETCLOSEST(frustumPlanes[3]);

    /* Right plane */
    RwV3dSubMacro(&vTmp, &frustumVerts[3], &frustumVerts[7]);
    RwV3dSubMacro(&vTmp2, &frustumVerts[4], &frustumVerts[7]);
    RwV3dCrossProductMacro(&frustumPlanes[4].plane.normal, &vTmp, &vTmp2);
    _rwV3dNormalizeMacro(recip,
                         &frustumPlanes[4].plane.normal,
                         &frustumPlanes[4].plane.normal);
    frustumPlanes[4].plane.distance =
        RwV3dDotProductMacro(&frustumVerts[3],
                             &frustumPlanes[4].plane.normal);
    RWPLANESETCLOSEST(frustumPlanes[4]);

    /* Bottom plane */
    RwV3dSubMacro(&vTmp2, &frustumVerts[6], &frustumVerts[7]);
    RwV3dCrossProductMacro(&frustumPlanes[5].plane.normal, &vTmp2, &vTmp);
    _rwV3dNormalizeMacro(recip,
                         &frustumPlanes[5].plane.normal,
                         &frustumPlanes[5].plane.normal);
    frustumPlanes[5].plane.distance =
        RwV3dDotProductMacro(&frustumVerts[3],
                             &frustumPlanes[5].plane.normal);
    RWPLANESETCLOSEST(frustumPlanes[5]);

    RWRETURNVOID();
}

/****************************************************************************
 _CameraBuildPerspViewMatrix

 On entry   : camera
            : local transformation matrix of camera
            : View matrix out
 On exit    : camera pointer on success
 */

static RwCamera    *
CameraBuildPerspViewMatrix(RwCamera * camera)
{
    RwReal              scale;
    RwV3d               vVector;
    const RwMatrix     *cameraLTM;
    RwMatrix           *viewMatrix;

    RWFUNCTION(RWSTRING("CameraBuildPerspViewMatrix"));

    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    cameraLTM = _rwFrameGetLTM((RwFrame *) rwObjectGetParent(camera));
    viewMatrix = &camera->viewMatrix;

    RWASSERT(cameraLTM);

    /*INDENT OFF */

    /*
     * Builds the concatenation of the following matrices:
     *
     *  [ [ -Right_X,    Up_X,      At_X,    0 ]       re-orient
     *    [ -Right_Y,    Up_Y,      At_Y,    0 ]
     *    [ -Right_Z,    Up_Z,      At_Z,    0 ]
     *    [ pos Right, -(pos Up), -(pos At), 1 ] ]
     *  [ [   1,      0,    0, 0 ]                     offset eye
     *    [   0,      1,    0, 0 ]
     *    [ off_x,  off_y,  1, 0 ]
     *    [ -off_x, -off_y, 0, 1 ] ]
     *  [ [ 0.5 / width,      0,       0, 0 ]          scale for view window
     *    [      0,      0.5 / height, 0, 0 ]
     *    [      0,           0,       1, 0 ]
     *    [      0,           0,       0, 1 ] ]
     *  [ [ 1, 0,  0, 0 ]                              project & flip y
     *    [ 0, -1, 0, 0 ]
     *    [ 0, 0,  1, 1 ]                              DIFFERS FROM PARALLEL
     *    [ 0, 0,  0, 0 ] ]
     *  [ [  1,   0,  0, 0 ]                           xform XY
     *    [  0,   1,  0, 0 ]                           from [-0.5..0.5]^2
     *    [  0,   0,  1, 0 ]                           to   [0..1]^2
     *    [ 0.5, 0.5, 0, 1 ] ]
     */

    /*INDENT ON */

    /* At */
    scale = ((camera->recipViewWindow.x) * ((RwReal) ((-0.5))));
    RwV3dScaleMacro(&vVector, &cameraLTM->right, scale);

    scale = (((RwReal) ((0.5))) - (((scale) * (camera->viewOffset.x))));
    RwV3dIncrementScaledMacro(&vVector, &cameraLTM->at, scale);

    viewMatrix->right.x = vVector.x;
    viewMatrix->up.x = vVector.y;
    viewMatrix->at.x = vVector.z;
    viewMatrix->pos.x =
        (((RwReal) ((0.5))) -
         (((scale) + (RwV3dDotProductMacro(&cameraLTM->pos, &vVector)))));

    /* Up */
    scale = ((camera->recipViewWindow.y) * ((RwReal) ((-0.5))));
    RwV3dScaleMacro(&vVector, &cameraLTM->up, scale);

    scale = (((RwReal) ((0.5))) + (((scale) * (camera->viewOffset.y))));
    RwV3dIncrementScaledMacro(&vVector, &cameraLTM->at, scale);

    viewMatrix->right.y = vVector.x;
    viewMatrix->up.y = vVector.y;
    viewMatrix->at.y = vVector.z;
    viewMatrix->pos.y =
        (((RwReal) ((0.5))) -
         (((scale) + (RwV3dDotProductMacro(&cameraLTM->pos, &vVector)))));

    /* At */
    viewMatrix->right.z = cameraLTM->at.x;
    viewMatrix->up.z = cameraLTM->at.y;
    viewMatrix->at.z = cameraLTM->at.z;
    viewMatrix->pos.z =
        -RwV3dDotProductMacro(&cameraLTM->pos, &cameraLTM->at);

    /* Mark matrix as needing updating */
    /* RwMatrixUpdate(viewMatrix); */
    RwMatrixOptimize(viewMatrix, (RwMatrixTolerance *) NULL);

    /* All done */
    RWRETURN(camera);
}

/****************************************************************************
 _CameraBuildParallelClipPlanes
 *  Top View (looking neg y)
 *
 *         4        8
 *          1        5         The right
 *       ____________
 *      |   |        |
 *      .   |        |              ----> Look at
 *      |   |        |
 *      |___|________|                beneath above
 *
 *          2        6         The left
 *         3        7
 *
 *  On entry   : Camera
 *
 *  On exit    : 6 planes making up the viewing frustum
 *             : Bounding box bll
 *             : Bounding box fur
 */

/* 1 Bll 0 Fur */

static void
CameraBuildParallelClipPlanes(RwCamera * camera)
{
    RwReal              width, height;
    RwReal              nearPlane, farPlane;
    RwReal              offsetx, offsety;

    RwReal              offset;
    RwV3d               vTmp, vTmp2;
    RwReal              recip;

    RwV3d              *frustumVerts = camera->frustumCorners;
    RwMatrix           *cameraLTM =
        _rwFrameGetLTM((RwFrame *) rwObjectGetParent(camera));
    RwFrustumPlane     *frustumPlanes = camera->frustumPlanes;

    RWFUNCTION(RWSTRING("CameraBuildParallelClipPlanes"));
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    /* Get camera view width/height */
    width = camera->viewWindow.x;
    height = camera->viewWindow.y;

    /* Get the clip-planes */
    nearPlane = camera->nearPlane;
    farPlane = camera->farPlane;

    /* Get the view offset */
    offsetx = - camera->viewOffset.x;
    offsety = camera->viewOffset.y;

    /* Set up the frustum planes */
    frustumVerts[0].z = frustumVerts[1].z =
        frustumVerts[2].z = frustumVerts[3].z = nearPlane;
    frustumVerts[4].z = frustumVerts[5].z =
        frustumVerts[6].z = frustumVerts[7].z = farPlane;

    /* We change the offset value to take acount of view offset shear */
    offset = (1.0f - nearPlane) * offsetx;
    frustumVerts[0].x = frustumVerts[3].x = width + offset;
    frustumVerts[1].x = frustumVerts[2].x = - width + offset;

    offset = (1.0f - farPlane) * offsetx;
    frustumVerts[4].x = frustumVerts[7].x = width + offset;
    frustumVerts[5].x = frustumVerts[6].x = - width + offset;

    offset = (1.0f - nearPlane) * offsety;
    frustumVerts[0].y = frustumVerts[1].y = height + offset;
    frustumVerts[2].y = frustumVerts[3].y = - height + offset;

    offset = (1.0f - farPlane) * offsety;
    frustumVerts[4].y = frustumVerts[5].y = height + offset;
    frustumVerts[6].y = frustumVerts[7].y = - height + offset;

    /* Project into world space */
    RwV3dTransformPoints(frustumVerts, frustumVerts, 8, cameraLTM);

    /* Build the planes */

    /* Far clipping plane */
    frustumPlanes[0].plane.normal = cameraLTM->at;
    frustumPlanes[0].plane.distance =
        RwV3dDotProductMacro(&frustumVerts[4],
                             &frustumPlanes[0].plane.normal);
    RWPLANESETCLOSEST(frustumPlanes[0]);

    /* Near clipping plane */
    RwV3dNegateMacro(&frustumPlanes[1].plane.normal,
                     &frustumPlanes[0].plane.normal);
    frustumPlanes[1].plane.distance =
        RwV3dDotProductMacro(&frustumVerts[0],
                             &frustumPlanes[1].plane.normal);
    RWPLANESETCLOSEST(frustumPlanes[1]);

    /* Left plane */
    RwV3dSubMacro(&vTmp, &frustumVerts[1], &frustumVerts[5]);
    RwV3dSubMacro(&vTmp2, &frustumVerts[6], &frustumVerts[5]);
    RwV3dCrossProductMacro(&frustumPlanes[2].plane.normal, &vTmp, &vTmp2);
    _rwV3dNormalizeMacro(recip,
                         &frustumPlanes[2].plane.normal,
                         &frustumPlanes[2].plane.normal);
    frustumPlanes[2].plane.distance =
        RwV3dDotProductMacro(&frustumVerts[1],
                             &frustumPlanes[2].plane.normal);
    RWPLANESETCLOSEST(frustumPlanes[2]);

    /* Top plane */
    RwV3dSubMacro(&vTmp2, &frustumVerts[4], &frustumVerts[5]);
    RwV3dCrossProductMacro(&frustumPlanes[3].plane.normal, &vTmp2, &vTmp);
    _rwV3dNormalizeMacro(recip,
                         &frustumPlanes[3].plane.normal,
                         &frustumPlanes[3].plane.normal);
    frustumPlanes[3].plane.distance =
        RwV3dDotProductMacro(&frustumVerts[1],
                             &frustumPlanes[3].plane.normal);
    RWPLANESETCLOSEST(frustumPlanes[3]);

    /* Right plane */
    RwV3dNegateMacro(&frustumPlanes[4].plane.normal,
                     &frustumPlanes[2].plane.normal);
    frustumPlanes[4].plane.distance =
        RwV3dDotProductMacro(&frustumVerts[3],
                             &frustumPlanes[4].plane.normal);
    RWPLANESETCLOSEST(frustumPlanes[4]);

    /* Bottom plane */
    RwV3dNegateMacro(&frustumPlanes[5].plane.normal,
                     &frustumPlanes[3].plane.normal);
    frustumPlanes[5].plane.distance =
        RwV3dDotProductMacro(&frustumVerts[3],
                             &frustumPlanes[5].plane.normal);
    RWPLANESETCLOSEST(frustumPlanes[5]);

    RWRETURNVOID();
}

/****************************************************************************
 _CameraBuildParallelViewMatrix

 On entry   : camera
            : local transformation matrix of camera
            : View matrix out
 On exit    : camera pointer on success
 */

static RwCamera    *
CameraBuildParallelViewMatrix(RwCamera * camera)
{
    RwReal              scale;
    RwV3d               vVector;
    const RwMatrix     *cameraLTM;
    RwMatrix           *viewMatrix;

    RWFUNCTION(RWSTRING("CameraBuildParallelViewMatrix"));

    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    cameraLTM = _rwFrameGetLTM((RwFrame *) rwObjectGetParent(camera));
    viewMatrix = &camera->viewMatrix;

    RWASSERT(cameraLTM);

    /*INDENT OFF */

    /*
     * Builds the concatenation of the following matrices:
     *
     *  [ [ -Right_X,    Up_X,      At_X,    0 ]       re-orient
     *    [ -Right_Y,    Up_Y,      At_Y,    0 ]
     *    [ -Right_Z,    Up_Z,      At_Z,    0 ]
     *    [ pos Right, -(pos Up), -(pos At), 1 ] ]
     *  [ [   1,      0,    0, 0 ]                     offset eye
     *    [   0,      1,    0, 0 ]
     *    [ off_x,  off_y,  1, 0 ]
     *    [ -off_x, -off_y, 0, 1 ] ]
     *  [ [ 0.5 / width,      0,       0, 0 ]          scale for view window
     *    [      0,      0.5 / height, 0, 0 ]
     *    [      0,           0,       1, 0 ]
     *    [      0,           0,       0, 1 ] ]
     *  [ [ 1, 0,  0, 0 ]                              project & flip y
     *    [ 0, -1, 0, 0 ]
     *    [ 0, 0,  1, 0 ]                              DIFFERS FROM PARALLEL
     *    [ 0, 0,  0, 1 ] ]
     *  [ [  1,   0,  0, 0 ]                           xform XY
     *    [  0,   1,  0, 0 ]                           from [-0.5..0.5]^2
     *    [  0,   0,  1, 0 ]                           to   [0..1]^2
     *    [ 0.5, 0.5, 0, 1 ] ]
     */

    /*INDENT ON */

    /* At */
    scale = ((camera->recipViewWindow.x) * ((RwReal) ((-0.5))));
    RwV3dScaleMacro(&vVector, &cameraLTM->right, scale);

    scale = -((scale) * (camera->viewOffset.x));
    RwV3dIncrementScaledMacro(&vVector, &cameraLTM->at, scale);

    viewMatrix->right.x = vVector.x;
    viewMatrix->up.x = vVector.y;
    viewMatrix->at.x = vVector.z;
    viewMatrix->pos.x =
        (((RwReal) ((0.5))) -
         (((scale) + (RwV3dDotProductMacro(&cameraLTM->pos, &vVector)))));

    /* Up */
    scale = ((camera->recipViewWindow.y) * ((RwReal) ((-0.5))));
    RwV3dScaleMacro(&vVector, &cameraLTM->up, scale);

    scale = ((scale) * (camera->viewOffset.y));
    RwV3dIncrementScaledMacro(&vVector, &cameraLTM->at, scale);

    viewMatrix->right.y = vVector.x;
    viewMatrix->up.y = vVector.y;
    viewMatrix->at.y = vVector.z;
    viewMatrix->pos.y =
        (((RwReal) ((0.5))) -
         (((scale) + (RwV3dDotProductMacro(&cameraLTM->pos, &vVector)))));

    /* At */
    viewMatrix->right.z = cameraLTM->at.x;
    viewMatrix->up.z = cameraLTM->at.y;
    viewMatrix->at.z = cameraLTM->at.z;
    viewMatrix->pos.z =
        -RwV3dDotProductMacro(&cameraLTM->pos, &cameraLTM->at);

    /* Mark matrix as needing updating */
    /* RwMatrixUpdate(viewMatrix); */
    RwMatrixOptimize(viewMatrix, (RwMatrixTolerance *) NULL);

    /* All done */
    RWRETURN(camera);
}

/***************************************************************************
 _CameraSync

 Allows the world to update it's data in the camera (for atomic sectors in the frustum, eg)

 On entry   : Camera
 On exit    :
 */

static RwObjectHasFrame *
CameraSync(RwObjectHasFrame * type)
{
    RwCamera           *camera = (RwCamera *) type;

    RWFUNCTION(RWSTRING("CameraSync"));

    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    /* Make sure the view matrix and frustum are up to date */
    if (camera->projectionType == rwPERSPECTIVE)
    {
        CameraBuildPerspViewMatrix(camera);
        CameraBuildPerspClipPlanes(camera);
    }
    else /* (camera->projectionType == rwPARALLEL) */
    {
        CameraBuildParallelViewMatrix(camera);
        CameraBuildParallelClipPlanes(camera);
    }

    /* Find the BBox for the view frustum - we just calculated the frustum vertices */
    RwBBoxCalculate(&camera->frustumBoundBox, camera->frustumCorners, 8);

    RWRETURN(type);
}


/***************************************************************************
 CameraEndUpdate

 Camera module end camera update function - this may be daisy chained from
 the world module.

 On entry   : Camera
 On exit    : Camera pointer on success
 */

static RwCamera    *
CameraEndUpdate(RwCamera * camera)
{
    RwStandardFunc      CameraEndUpdateFunc;

    RWFUNCTION(RWSTRING("CameraEndUpdate"));
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);
    RWASSERT(camera == RWSRCGLOBAL(curCamera));

#if (defined(RWEVALUATION))
    _rwCameraValRender(camera);
#endif /* (defined(RWEVALUATION)) */

    /* Tell the device ! */
    CameraEndUpdateFunc = RWSRCGLOBAL(stdFunc[rwSTANDARDCAMERAENDUPDATE]);
    RWASSERT(NULL != CameraEndUpdateFunc);

    if (CameraEndUpdateFunc(NULL, camera, 0))
    {
        /* No longer have a current camera */
        RWSRCGLOBAL(curCamera) = NULL;

        RWRETURN(camera);
    }

    /* Device failed to end camera update cliche */
    RWRETURN((RwCamera *)NULL);
}

/***************************************************************************
 CameraBeginUpdate

 Camera module begin camera update function - this may be daisy chained from
 the world module.

 On entry   : Camera
 On exit    : Camera pointer on success
 */

static RwCamera    *
CameraBeginUpdate(RwCamera * camera)
{
    RwStandardFunc      CameraBeginUpdateFunc;

    RWFUNCTION(RWSTRING("CameraBeginUpdate"));
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);
    RWASSERT(camera->frameBuffer);
    RWASSERT(camera->nearPlane > 0.0f);
    RWASSERT(camera->nearPlane < camera->farPlane);

    if (camera->zBuffer)
    {
        /* Make sure the framebuffer/zbuffer parent resolutions are the same */
        RWASSERT(camera->frameBuffer->parent->width ==
                 camera->zBuffer->parent->width);
        RWASSERT(camera->frameBuffer->parent->height ==
                 camera->zBuffer->parent->height);
    }

    /* OK, we are go... */
    RWSRCGLOBAL(curCamera) = (void *) camera;

    /* Sync up all of the objects which have moved,
     * before we call the device */
    _rwFrameSyncDirty();

    /* We currently do NOT support non-orthonormal camera transforms (though this
     * is inconvenient for developers attaching cameras to frames and is also perhaps
     * unnecessary - since we concatenate camera and object matrices in the end, a
     * scaled object matrix is equivalent to a (differently) scaled camera matrix) */
#ifdef RWDEBUG
    {    
        static RwBool messaged = FALSE;
        RwBool CameraFrameIsOrthonormal;

        MATRIXISORTHONORMAL(CameraFrameIsOrthonormal,
                            RwFrameGetLTM(RwCameraGetFrame(camera))
                            );

        if( (TRUE == CameraFrameIsOrthonormal) && 
            (FALSE == (rwMATRIXTYPEORTHONORMAL & 
                        (RwFrameGetLTM(RwCameraGetFrame(camera)))->flags) ) &&
            (FALSE == messaged)
          )
        {
            RWMESSAGE((RWSTRING("The current camera frame is orthonormal but "
                "the matching matrix flags are not set. This could "
                "result in performance loss as orthonormal "
                "matrix operation optimization won't be used")));
            
            messaged = TRUE;
        }
        else
        {
            RWASSERT(TRUE == CameraFrameIsOrthonormal);
        }            
    }
#endif

    /* Set context */
    CameraBeginUpdateFunc = RWSRCGLOBAL(stdFunc[rwSTANDARDCAMERABEGINUPDATE]);
    RWASSERT(NULL != CameraBeginUpdateFunc);

    if (CameraBeginUpdateFunc(NULL, camera, 0))
    {
#if 0
        /* Hook the pipeline to let it set itself up */
        rwGetCorePipeInterface()->pipeInitForCamera(camera);
#endif /* 0/1 */
        _rwPipeInitForCamera(camera);

        /* Rendering mode */
        RWRETURN(camera);
    }
    else
    {
        /* Couldn't push context */
        RWRETURN((RwCamera *)NULL);
    }
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Opening and closing

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/****************************************************************************
 _rwCameraClose

 On entry   :
 On exit    : TRUE on success
 */

void               *
_rwCameraClose(void *instance,
               RwInt32 __RWUNUSED__ offset,
               RwInt32 __RWUNUSED__ size )
{
    RWFUNCTION(RWSTRING("_rwCameraClose"));

    if (RWCAMERAGLOBAL(cameraFreeList))
    {
        RwFreeListDestroy(RWCAMERAGLOBAL(cameraFreeList));
        RWCAMERAGLOBAL(cameraFreeList) = (RwFreeList *)NULL;
    }

    /* One less module instance */
    cameraModule.numInstances--;

    /* Success */
    RWRETURN(instance);
}

static RwInt32 _rwCameraFreeListBlockSize = 4,
               _rwCameraFreeListPreallocBlocks = 1;
static RwFreeList _rwCameraFreeList;

/**
 * \ingroup rwcamera
 * \ref RwCameraSetFreeListCreateParams allows the developer to specify
 * how many \ref RwCamera s to preallocate space for.
 * Call before \ref RwEngineInit.
 *
 * \param blockSize  number of entries per freelist block.
 * \param numBlocksToPrealloc  number of blocks to allocate on
 * \ref RwFreeListCreateAndPreallocateSpace.
 *
 * \see RwFreeList
 *
 */
void
RwCameraSetFreeListCreateParams( RwInt32 blockSize, RwInt32 numBlocksToPrealloc )
{
#if 0
    /*
     * Can not use debugging macros since the debugger is not initialized before
     * RwEngineInit.
     */
    /* Do NOT comment out RWAPIFUNCTION as gnumake verify will not function */
    RWAPIFUNCTION(RWSTRING("RwCameraSetFreeListCreateParams"));
#endif
    _rwCameraFreeListBlockSize = blockSize;
    _rwCameraFreeListPreallocBlocks = numBlocksToPrealloc;

#if 0
    RWRETURNVOID();
#endif
}

/****************************************************************************
 _rwCameraOpen

 On entry   :
 On exit    : TRUE on success
 */

void               *
_rwCameraOpen(void * instance,
              RwInt32 offset,
              RwInt32 __RWUNUSED__ size)
{
    RWFUNCTION(RWSTRING("_rwCameraOpen"));

    /* Store the data block offset (the same for all instances) */
    cameraModule.globalsOffset = offset;

    /* Create the cameras */
    RWCAMERAGLOBAL(cameraFreeList) =
        RwFreeListCreateAndPreallocateSpace(cameraTKList.sizeOfStruct, _rwCameraFreeListBlockSize,
        rwMATRIXALIGNMENT, _rwCameraFreeListPreallocBlocks, &_rwCameraFreeList,
        rwMEMHINTDUR_GLOBAL | rwID_CAMERA);

    if (!RWCAMERAGLOBAL(cameraFreeList))
    {
        /* Failure */
        RWRETURN(NULL);
    }

    /* One more module instance */
    cameraModule.numInstances++;

#ifdef RWDEBUG
    inCameraBeginEndUpdate = FALSE;
#endif /* RWDEBUG */

    /* Success */
    RWRETURN(instance);
}

/**
 * \ingroup rwcamera
 * \ref RwCameraEndUpdate is used to indicate that 3D rendering to the
 * specified camera has been completed.
 *
 * Note that \ref RwCameraBeginUpdate and \ref RwCameraEndUpdate work as a
 * pair and only between these calls can any rendering be properly performed.
 *
 * \param camera  Pointer to the camera used for 3D rendering.
 *
 * \return Returns pointer to the camera used for rendering or NULL if
 * an error occurred.
 *
 * \see RwCameraBeginUpdate
 * \see RwCameraGetCurrentCamera
 * \see RwCameraShowRaster
 * \see RwRenderStateGet
 * \see RwRenderStateSet
 *
 */
RwCamera *
RwCameraEndUpdate(RwCamera *camera)
{

    RwCamera *result;

    RWAPIFUNCTION(RWSTRING("RwCameraEndUpdate"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    result = camera->endUpdate(camera);
#ifdef RWDEBUG
    inCameraBeginEndUpdate = FALSE;
#endif /* RWDEBUG */

    RWCRTCHECKMEMORY();

    RWRETURN(result);
}

/**
 * \ingroup rwcamera
 * \ref RwCameraBeginUpdate is used to indicate that the specified
 * camera is about to start rendering a view of a scene and that the camera's
 * frame buffer raster is to be the target of any subsequent 3D rendering.
 *
 * When \ref RwCameraBeginUpdate is called, the camera's image raster is pushed
 * onto the context stack, thereby ensuring that the result of any rendering
 * is stored in the camera's own raster.
 *
 * At the same time, all dirty frame hierarchies are synchronized,
 * i.e. the individual Local Transform Matrices (LTMs) are recalculated ready
 * for processing. Rendering to the camera can now take place with all scene
 * objects guaranteed to be in their correct positions and orientations.
 *
 * \param camera  Pointer to the camera to use for 3D rendering.
 *
 * \return Returns a pointer to the camera used for rendering, or NULL if an
 * error occurred.
 *
 * \see RwCameraEndUpdate
 * \see RwCameraForAllClumpsInFrustum
 * \see RwCameraGetCurrentCamera
 * \see RpClumpGetCallBack
 * \see RpClumpSetCallBack
 *
 */
RwCamera *
RwCameraBeginUpdate(RwCamera *camera)
{
    RwCamera *result;
    RWAPIFUNCTION(RWSTRING("RwCameraBeginUpdate"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);


    result = camera->beginUpdate(camera);
#ifdef RWDEBUG
    if (result)
    {
        inCameraBeginEndUpdate = TRUE;
    }
#endif /* RWDEBUG */
    RWRETURN(result);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rwcamera
 * \ref RwCameraGetViewOffset is used to determine the current
 * view-offset of the specified camera.
 *
 * The x- and y-offsets are measured in world units in a plane passing
 * through the camera's viewpoint and parallel to the view-plane, in the
 * negative direction of the camera's 'look-right' and in the positive
 * direction of the camera's 'look-up' vectors respectively. Non-zero
 * offsets move the apex of the view volume whilst its edges remain fixed
 * to the corners of the view-window. This has the effect of shearing the
 * view volume.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param camera  Pointer to the camera whose view offset is required.
 *
 * \return Returns pointer to an RwV2d that specifies the camera's view offset.
 *
 * \see RwCameraClone
 * \see RwCameraCreate
 * \see RwCameraGetViewWindow
 * \see RwCameraSetViewOffset
 * \see RwCameraSetViewWindow
 */
const RwV2d *
RwCameraGetViewOffset(const RwCamera *camera)
{
    RWAPIFUNCTION(RWSTRING("RwCameraGetViewOffset"));

    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    RWRETURN(RwCameraGetViewOffsetMacro(camera));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rwcamera
 * \ref RwCameraSetViewOffset is used to specify the current
 * view-offset of the given camera.
 *
 * The x- and y-offsets are measured in world units in a plane passing
 * through the camera's viewpoint and parallel to the view-plane, in the
 * negative direction of the camera's 'look-right' and in the positive
 * direction of the camera's 'look-up' vectors respectively.
 * Non-zero offsets move the apex of the view volume whilst its edges
 * remain fixed to the corners of the view-window. This has the effect of
 * shearing the view volume.
 *
 * \note The offsets are specified as absolute values from their initial
 * unsheared camera view-point position - successive calls to
 * \ref RwCameraSetViewOffset are not accumulated as relative displacements.
 *
 * \note This function should \e not be called between
 *       \ref RwCameraBeginUpdate and \ref RwCameraEndUpdate.
 *
 * \param camera  Pointer to the camera for which the view offset is to be set.
 * \param offset  An RwV2d specifing the new view window offset.
 *
 * \return Returns pointer to the specified camera.
 *
 * \see RwCameraClone
 * \see RwCameraCreate
 * \see RwCameraGetViewOffset
 * \see RwCameraGetViewWindow
 * \see RwCameraSetViewWindow
 *
 */
RwCamera *
RwCameraSetViewOffset(RwCamera *camera, const RwV2d *offset)
{
    RwFrame *frame;

    RWAPIFUNCTION(RWSTRING("RwCameraSetViewOffset"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);
    RWASSERT(offset);

    RwV2dAssign(&camera->viewOffset, offset);

    /* Force view matrix to be rebuilt */
    frame = (RwFrame *)rwObjectGetParent(camera);
    if (frame)
    {
        RwFrameUpdateObjects(frame);
    }

    RWRETURN(camera);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Rasters

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rwcamera
 * \ref RwCameraSetRaster is used to specify a raster which will serve
 * as the camera's image buffer. Note that the raster must have been defined
 * as type rwRASTERTYPECAMERA or rwRASTERTYPECAMERATEXTURE when it was
 * created.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param camera  Pointer to the camera with the image buffer to be set.
 * \param raster  Pointer to a raster which will become the camera's image buffer.
 * (Set to NULL to detach an existing raster.)
 *
 * \return Returns pointer to the camera.
 *
 * \see RwCameraCreate
 * \see RwCameraClone
 * \see RwCameraGetRaster
 * \see RwCameraGetZRaster
 * \see RwCameraSetZRaster
 * \see RwRasterCreate
 *
 */
RwCamera *
RwCameraSetRaster(RwCamera *camera, RwRaster *raster)
{
    RWAPIFUNCTION(RWSTRING("RwCameraSetRaster"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    if (raster)
    {
        RWASSERT((raster->cType == rwRASTERTYPECAMERA) ||
                 (raster->cType == rwRASTERTYPECAMERATEXTURE));
    }

    RWRETURN(RwCameraSetRasterMacro(camera, raster));
}

/**
 * \ingroup rwcamera
 * \ref RwCameraGetRaster is used to retrieve a pointer to the raster
 * attached to the specified camera which serves as the camera's image buffer.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param camera  Pointer to the camera whose image raster is required.
 *
 * \return Returns pointer to the camera's image raster if successful or NULL
 * if there is an error or if the camera does not have an image raster
 * defined.
 *
 * \see RwCameraSetRaster
 * \see RwCameraGetZRaster
 * \see RwCameraSetZRaster
 * \see RwCameraCreate
 * \see RwCameraClone
 *
 */
RwRaster *
RwCameraGetRaster(const RwCamera *camera)
{
    RWAPIFUNCTION(RWSTRING("RwCameraGetRaster"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    RWRETURN(RwCameraGetRasterMacro(camera));
}

/**
 * \ingroup rwcamera
 * \ref RwCameraSetZRaster is used to specify a raster which will serve
 * as the camera's Z-buffer. Note that the raster must have been defined
 * as type rwRASTERTYPEZBUFFER when it was created.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param camera  Pointer to the camera for which the Z-buffer is to be set.
 * \param zRaster  Pointer to a raster which will become the camera's Z-buffer.
 * (Set to NULL to detach an existing raster.)
 *
 * \return Returns pointer to the camera.
 *
 * \see RwCameraGetZRaster
 * \see RwCameraSetRaster
 * \see RwCameraGetRaster
 * \see RwCameraClone
 * \see RwCameraCreate
 * \see RwRasterCreate
 *
 */
RwCamera *
RwCameraSetZRaster(RwCamera *camera, RwRaster *zRaster)
{
    RWAPIFUNCTION(RWSTRING("RwCameraSetZRaster"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    if (zRaster)
    {
        RWASSERT(zRaster->cType == rwRASTERTYPEZBUFFER);
    }

    RWRETURN(RwCameraSetZRasterMacro(camera, zRaster));
}

/**
 * \ingroup rwcamera
 * \ref RwCameraGetZRaster is used to retrieve a pointer to the raster,
 * if any, attached to the specified camera and serving as the camera's
 * Z-buffer.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param camera  Pointer to the camera for which the Z-buffer raster is required.
 *
 * \return Returns pointer to the camera's Z-buffer raster.
 *
 * \see RwCameraSetZRaster
 * \see RwCameraCreate
 * \see RwCameraGetRaster
 * \see RwCameraSetRaster
 *
 */
RwRaster *
RwCameraGetZRaster(const RwCamera *camera)
{
    RWAPIFUNCTION(RWSTRING("RwCameraGetZRaster"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    RWRETURN(RwCameraGetZRasterMacro(camera));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Clip planes

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwcamera
 * \ref RwCameraSetNearClipPlane is used to set the distance of the
 * specified camera's near clip-plane. This distance is specified in world
 * units and must be greater than zero and less than the far clip-plane.
 *
 * \note This function should \e not be called between
 *       \ref RwCameraBeginUpdate and \ref RwCameraEndUpdate.
 *
 * \param camera  Pointer to the camera for which the near clip-plane is to be set.
 * \param nearClip  An RwReal value equal to the near clip-plane distance.
 *
 * \return Returns a pointer to the camera whose near clip-plane is set.
 *
 * \see RwCameraClone
 * \see RwCameraCreate
 * \see RwCameraGetNearClipPlane
 * \see RwCameraGetFarClipPlane
 * \see RwCameraSetFarClipPlane
 * \see RwCameraGetFogDistance
 * \see RwCameraGetViewWindow
 * \see RwCameraSetFogDistance
 * \see RwCameraSetViewWindow
 *
 */
RwCamera *
RwCameraSetNearClipPlane(RwCamera *camera, RwReal nearClip)
{
    RwFrame *frame;

    RWAPIFUNCTION(RWSTRING("RwCameraSetNearClipPlane"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);
    RWASSERT(nearClip > 0.0f);
    /* Removed assert of nearClip < farClip since we test for valid camera
       state before use, i.e RwCameraBeginUpdate & RwCameraClear */

    camera->nearPlane = nearClip;

    /* Update the Z transformation */
    CameraUpdateZShiftScale(camera);

    /* Force view matrix to be rebuilt */
    frame = (RwFrame *)rwObjectGetParent(camera);
    if (frame)
    {
        RwFrameUpdateObjects(frame);
    }

    RWRETURN(camera);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rwcamera
 * \ref RwCameraGetNearClipPlane returns the distance of the
 * specified camera's near clip-plane. This distance is measured in world
 * units from the camera's position in the direction of the 'look-at'
 * vector. (The near clip-plane distance cannot be zero.)
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param camera  Pointer to the camera from which to retrieve the near clip-plane
 * distance.
 *
 * \return Returns an RwReal value equal to the specified camera's near
 * clip-plane.
 *
 * \see RwCameraClone
 * \see RwCameraCreate
 * \see RwCameraGetFarClipPlane
 * \see RwCameraGetFogDistance
 * \see RwCameraGetViewWindow
 * \see RwCameraSetFarClipPlane
 * \see RwCameraSetFogDistance
 * \see RwCameraSetNearClipPlane
 * \see RwCameraSetViewWindow
 *
 */
RwReal
RwCameraGetNearClipPlane(const RwCamera *camera)
{
    RWAPIFUNCTION(RWSTRING("RwCameraGetNearClipPlane"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    RWRETURN(RwCameraGetNearClipPlaneMacro(camera));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rwcamera
 * \ref RwCameraSetFarClipPlane is used to set the distance of the
 * specified camera's far clip-plane. This distance is specified in world
 * units and must be greater than the near clip-plane.
 *
 * \note This function should \e not be called between
 *       \ref RwCameraBeginUpdate and \ref RwCameraEndUpdate.
 *
 * \param camera  Pointer to the camera for which the far clip-plane is to be set.
 * \param farClip  An RwReal value equal to the far clip-plane distance.
 *
 * \return Returns pointer to the camera for which the far clip-plane has
 * been set.
 *
 * \see RwCameraClone
 * \see RwCameraCreate
 * \see RwCameraGetFarClipPlane
 * \see RwCameraGetFogDistance
 * \see RwCameraGetNearClipPlane
 * \see RwCameraGetViewWindow
 * \see RwCameraSetFogDistance
 * \see RwCameraSetNearClipPlane
 * \see RwCameraSetViewWindow
 *
 */
RwCamera *
RwCameraSetFarClipPlane(RwCamera *camera, RwReal farClip)
{
    RwFrame *frame;

    RWAPIFUNCTION(RWSTRING("RwCameraSetFarClipPlane"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);
    /* Removed assert of farClip > nearClip since we test for valid camera
       state before use, i.e RwCameraBeginUpdate & RwCameraClear */

    camera->farPlane = farClip;

    /* Update the Z transformation */
    CameraUpdateZShiftScale(camera);

    /* Force view matrix to be rebuilt */
    frame = (RwFrame *)rwObjectGetParent(camera);
    if (frame)
    {
        RwFrameUpdateObjects(frame);
    }

    RWRETURN(camera);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rwcamera
 * \ref RwCameraGetFarClipPlane returns the distance of the specified
 * camera's far clip-plane. This distance is measured in world units from
 * the camera's position in the direction of the 'look-at' vector.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param camera  Pointer to the camera for which the far clip-plane distance is
 * required.
 *
 * \return Returns an RwReal value equal to the specified camera's far
 * clip-plane.
 *
 * \see RwCameraClone
 * \see RwCameraCreate
 * \see RwCameraGetFogDistance
 * \see RwCameraGetNearClipPlane
 * \see RwCameraGetViewWindow
 * \see RwCameraSetFarClipPlane
 * \see RwCameraSetFogDistance
 * \see RwCameraSetNearClipPlane
 * \see RwCameraSetViewWindow
 *
 */
RwReal
RwCameraGetFarClipPlane(const RwCamera *camera)
{
    RWAPIFUNCTION(RWSTRING("RwCameraGetFarClipPlane"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    RWRETURN(RwCameraGetFarClipPlaneMacro(camera));
}

/**
 * \ingroup rwcamera
 * \ref RwCameraSetFogDistance is used to set the a camera's fogging
 * distance. This distance is specified in world units and is the distance
 * at which fogging begins to take effect.  The far clip-plane is the
 * distance at which full fog effect is obtained.
 *
 * The fog distance cannot be zero.
 * The default fog distance is 5.0 units.
 *
 * \note This function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \note This function should \e not be called between
 *       \ref RwCameraBeginUpdate and \ref RwCameraEndUpdate.
 *
 * \param camera  Pointer to the camera for which the fogging distance is to be set.
 * \param fogDistance  An RwReal value equal to the fogging distance.
 *
 * \return Returns pointer to the camera for which the fogging distance was
 * set.
 *
 * \see RwCameraClone
 * \see RwCameraCreate
 * \see RwCameraGetFogDistance
 * \see RwCameraGetFarClipPlane
 * \see RwCameraGetNearClipPlane
 * \see RwCameraGetViewWindow
 * \see RwCameraSetFarClipPlane
 * \see RwCameraSetNearClipPlane
 * \see RwCameraSetViewWindow
 *
 */
RwCamera *
RwCameraSetFogDistance(RwCamera *camera, RwReal fogDistance)
{
    RWAPIFUNCTION(RWSTRING("RwCameraSetFogDistance"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    RWRETURN(RwCameraSetFogDistanceMacro(camera, fogDistance));
}

/**
 * \ingroup rwcamera
 * \ref RwCameraGetFogDistance returns the specified camera's fogging
 * distance. This distance is measured in world and is the distance at which
 * fogging begins to take effect.  The far clip-plane is the distance at which
 * the full fog effect is obtained.
 *
 * The fog distance cannot be zero.
 * The default fog distance is 5.0 units.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param camera  Pointer to the camera from which the fogging distance is required.
 *
 * \return Returns an RwReal value equal to the specified camera's fogging
 * distance.
 *
 * \see RwCameraClone
 * \see RwCameraCreate
 * \see RwCameraGetFarClipPlane
 * \see RwCameraGetNearClipPlane
 * \see RwCameraGetViewWindow
 * \see RwCameraSetFarClipPlane
 * \see RwCameraSetFogDistance
 * \see RwCameraSetNearClipPlane
 * \see RwCameraSetViewWindow
 *
 */
RwReal
RwCameraGetFogDistance(const RwCamera *camera)
{
    RWAPIFUNCTION(RWSTRING("RwCameraGetFogDistance"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);

    RWRETURN(RwCameraGetFogDistanceMacro(camera));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Rendering

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rwcamera
 * \ref RwCameraGetCurrentCamera is used to determine which (if any)
 * camera is being used for 3D rendering of the current scene. The current
 * camera is only defined between calls to \ref RwCameraBeginUpdate and
 * \ref RwCameraEndUpdate.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \return Returns a pointer to the current camera if successful or NULL if
 * there is an error or if there is no current camera.
 *
 * \see RwCameraBeginUpdate
 * \see RwCameraEndUpdate
 * \see RwCameraShowRaster
 *
 */
RwCamera *
RwCameraGetCurrentCamera(void)
{
    RWAPIFUNCTION(RWSTRING("RwCameraGetCurrentCamera"));
    RWASSERT(cameraModule.numInstances);

    RWRETURN(RwCameraGetCurrentCameraMacro());
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rwcamera
 * \ref RwCameraFrustumTestSphere is used to check if a specified sphere
 * intersects the view frustum of the given camera. A sphere that is wholly
 * inside the frustum is considered to have intersected the frustum.
 *
 * \param camera  Pointer to the camera with the frustum to be tested.
 * \param sphere  Pointer to the sphere which is to be tested against the frustum.
 *
 * \return Returns an RwFrustumTestResult value indicating the result.
 * Results can be:
 * outside the frustum (rwSPHEREOUTSIDE),
 * on the boundary of the frustum (rwSPHEREBOUNDARY), or
 * totally within the frustum (rwSPHEREINSIDE).
 * Returns (RwSPHEREOUTSIDE) if an error occurred.
 *
 * \see RwCameraForAllClumpsInFrustum
 * \see RpClumpGetCallBack
 * \see RpClumpSetCallBack
 *
 */
RwFrustumTestResult
RwCameraFrustumTestSphere(const RwCamera *camera, const RwSphere *sphere)
{
    const RwFrustumPlane    *frustumPlane;
    RwInt32                 numPlanes;
    RwFrustumTestResult     result;

    RWAPIFUNCTION(RWSTRING("RwCameraFrustumTestSphere"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);
    RWASSERT(sphere);

    /* Assume inside until we decide otherwise */
    result = rwSPHEREINSIDE;

    /* Is it really visible */
    frustumPlane = camera->frustumPlanes;
    numPlanes = 6;
    while (numPlanes--)
    {
        RwReal              nDot;

        nDot =
            RwV3dDotProductMacro(&sphere->center,
                                 &frustumPlane->plane.normal);
        nDot -= frustumPlane->plane.distance;

        if (nDot > sphere->radius)
        {
            /* Outside this plane, so outside frustum */
            RWRETURN(rwSPHEREOUTSIDE);
        }
        else if (nDot > (-sphere->radius))
        {
            /* Crosses the boundary, so mark it as such */
            result = rwSPHEREBOUNDARY;
        }

        /* Next plane */
        frustumPlane++;
    }

    /* Inside all the planes */
    RWRETURN(result);
}

/**
 * \ingroup rwcamera
 * \ref RwCameraClear is used to clear the image and/or Z-buffer rasters
 * associated with the specified camera. This function is typically used to
 * prepare the camera's rasters for receiving the results from a new
 * rendering, just before an \ref RwCameraBeginUpdate / \ref RwCameraEndUpdate
 * block. The image raster can be cleared to a specified color.
 *
 * \note The \ref RwCameraClear function must be called outside of the
 * \ref RwCameraBeginUpdate / \ref RwCameraEndUpdate block.
 *
 * \param camera  Pointer to the camera to clear.
 * \param color   Pointer to an RwRGBA containing the color to clear the image raster
 * with.
 * \param clearMode  A 32-bit integer bit-field specifying which of the camera's rasters
 * to clear. Its value is one (or more OR'd together) of the following values:
 *
 *            \li rwCAMERACLEARIMAGE - Clear the image raster.
 *            \li rwCAMERACLEARZ - Clear the Z-buffer raster.
 * \if xbox   \li rwCAMERACLEARSTENCIL - Clear the stencil buffer. \endif
 * \if d3d8   \li rwCAMERACLEARSTENCIL - Clear the stencil buffer. \endif
 * \if d3d9   \li rwCAMERACLEARSTENCIL - Clear the stencil buffer. \endif
 * \if opengl \li rwCAMERACLEARSTENCIL - Clear the stencil buffer. \endif
 *
 * \return Returns pointer to the specified camera if successful or NULL if
 * an error occurred.
 *
 * \see RwCameraBeginUpdate
 * \see RwCameraCreate
 * \see RwCameraEndUpdate
 * \see RwRasterClear
 * \see RwRasterClearRect
 *
 */
RwCamera *
RwCameraClear(RwCamera *camera, RwRGBA *color, RwInt32 clearMode)
{
    RwStandardFunc      CameraClearFunc;

    RWAPIFUNCTION(RWSTRING("RwCameraClear"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);
    RWASSERT(color || ((~clearMode) & rwCAMERACLEARIMAGE));
    RWASSERT(camera->frameBuffer);
    RWASSERT((rwCAMERACLEARIMAGE | rwCAMERACLEARZ | rwCAMERACLEARSTENCIL) & clearMode);
    /* Assert the clip planes are valid since some platforms will submit
       geometry to perform clears */
    RWASSERT(camera->nearPlane < camera->farPlane);
    RWASSERT(!inCameraBeginEndUpdate);

    CameraClearFunc = RWSRCGLOBAL(stdFunc[rwSTANDARDCAMERACLEAR]);
    RWASSERT(NULL != CameraClearFunc);

    if (CameraClearFunc(camera, color, (RwInt32) clearMode))
    {
        RWRETURN(camera);
    }

    /* Device error */
    RWRETURN((RwCamera *)NULL);
}

/**
 * \ingroup rwcamera
 * \ref RwCameraShowRaster is used to copy the specified camera's image
 * raster to the display. This function often immediately follows a
 * \ref RwCameraBeginUpdate / \ref RwCameraEndUpdate block in order to transfer
 * the new rendering to the display device.
 *
 * \param camera  Pointer to the camera with the image raster to be displayed.
 * \param dev  Device-dependent parameter, e.g. for Windows applications this is
 * a handle ('HWND') to the output window returned by CreateWindow.
 * \param flags  A flags field indicating hints about how to flip.  Valid fields in
 * this flags are:
 *
 *         \li rwRASTERFLIPWAITVSYNC - Flip should happen on next VSync if
 *             possible. (This flag should be used only for full-screen applications.
 *             Specify zero if running in a window.)
 *
 * \return Returns pointer to the specified camera if successful or NULL
 * if there is an error.
 *
 * \see RwCameraBeginUpdate
 * \see RwCameraEndUpdate
 * \see RwRasterShowRaster
 *
 */
RwCamera *
RwCameraShowRaster(RwCamera *camera, void *dev, RwUInt32 flags)
{
    RWAPIFUNCTION(RWSTRING("RwCameraShowRaster"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);
    RWASSERT(camera->frameBuffer);

    if (RwRasterShowRaster(camera->frameBuffer, dev, flags))
    {
        RWRETURN(camera);
    }

    /* Device error */
    RWRETURN((RwCamera *)NULL);
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   Usual Camera stuff

   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

/**
 * \ingroup rwcamera
 * \ref RwCameraSetProjection is used to specify the projection model
 * for the given camera. Two types of projection model are currently
 * supported: perspective and parallel. Perspective projection takes into
 * account the distance of an object from the camera and produces a view
 * very much like we expect from a real camera. Parallel projection presumes
 * that objects maintain their same relative size regardless of distance
 * from the camera. Hence two identical objects at different distances
 * look the same size when viewed with a parallel camera.
 *
 * The default projection model is perspective.
 *
 * \note This function should \e not be called between
 *       \ref RwCameraBeginUpdate and \ref RwCameraEndUpdate.
 *
 * \param camera  Pointer to the camera to adjust
 * \param projection  projection model to use:
 *
 *            \li rwPERSPECTIVE - Use a perspective projection model.
 *            \li rwPARALLEL - Use a parallel projection model.
 *
 * \return Returns pointer to the camera if successful or NULL if there
 * is an error.
 *
 * \see RwCameraClone
 * \see RwCameraCreate
 * \see RwCameraGetProjection
 *
 */
RwCamera *
RwCameraSetProjection(RwCamera *camera, RwCameraProjection projection)
{
    RWAPIFUNCTION(RWSTRING("RwCameraSetProjection"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    switch (projection)
    {
        case (rwPARALLEL):
        case (rwPERSPECTIVE):
            {
                RwFrame            *frame;

                camera->projectionType = projection;

                /* Force the view matrix to be rebuilt */
                frame = (RwFrame *) rwObjectGetParent(camera);
                if (frame)
                {
                    RwFrameUpdateObjects(frame);
                }

                /* Update the Z transformation */
                CameraUpdateZShiftScale(camera);

                RWRETURN(camera);
            }
        default:
            {
                /* Bad parameter type */
                RWERROR((E_RW_BADPARAM,
                         RWSTRING("Invalid projection type specified")));
                RWRETURN((RwCamera *)NULL);
            }
    }
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rwcamera
 * \ref RwCameraGetProjection is used to determine the projection model
 * currently set for the specified camera.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param camera  Pointer to the camera.
 *
 * \return Returns type of projection model that the camera is using, or
 * rwNACAMERAPROJECTION if there is an error:
 *
 * \li rwPERSPECTIVE - Camera is using a perspective projection model.
 * \li rwPARALLEL -  Camera is using a parallel projection model.
 *
 * \see RwCameraClone
 * \see RwCameraCreate
 * \see RwCameraSetProjection
 *
 */
RwCameraProjection
RwCameraGetProjection(const RwCamera *camera)
{
    RWAPIFUNCTION(RWSTRING("RwCameraGetProjection"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    RWRETURN(RwCameraGetProjectionMacro(camera));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rwcamera
 * \ref RwCameraSetViewWindow is used to set the size of the specified
 * camera's view-window (in world units).
 * The specified 2D vector represents the half-width and half-height of the
 * view-window. The default view-window has dimensions (1.0, 1.0) giving a
 * window 2.0 units on each side.
 *
 * This function can be used to control the angular field-of-view of the
 * camera. Larger values give a wider field-of-view while smaller values give
 * a narrower view. Hence changing the size of the view-window gives the
 * effect of a zoom-lens. Note that changing the size of the view-window does
 * not alter the size of the camera's image raster, so unless the aspect
 * ratios match, the resulting image will be distorted to conform to the image
 * raster's aspect ratio.
 *
 * \note This function should \e not be called between
 *       \ref RwCameraBeginUpdate and \ref RwCameraEndUpdate.
 *
 * \param camera  Pointer to the camera.
 * \param viewWindow  An RwV2d describing the view-window dimensions.
 *
 * \return Returns pointer to the specified camera.
 *
 * \see RwCameraGetViewWindow
 * \see RwCameraSetViewOffset
 * \see RwCameraGetViewOffset
 * \see RwCameraCreate
 * \see RwCameraClone
 *
 */
RwCamera *
RwCameraSetViewWindow(RwCamera *camera, const RwV2d *viewWindow)
{
    RwFrame *frame;

    RWAPIFUNCTION(RWSTRING("RwCameraSetViewWindow"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);
    RWASSERT(viewWindow);
    RWASSERT(viewWindow->x > 0.0f);
    RWASSERT(viewWindow->y > 0.0f);

    RwV2dAssign(&camera->viewWindow, viewWindow);
    camera->recipViewWindow.x = 1.0f / camera->viewWindow.x;
    camera->recipViewWindow.y = 1.0f / camera->viewWindow.y;

    /* Force the view matrix to be rebuilt */
    frame = (RwFrame *) rwObjectGetParent(camera);
    if (frame)
    {
        RwFrameUpdateObjects(frame);
    }

    RWRETURN(camera);
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rwcamera
 * \ref RwCameraGetViewWindow is used to retrieve the specified camera's
 * view-window size. View-window sizes are measured in world units.
 *
 * The returned 2D vector represents the half-width and half-height of the
 * view- window. The default view-window has dimensions of (1.0, 1.0) giving
 * a window 2.0 units on each side.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param camera  Pointer to the camera.
 *
 * \return Returns pointer to an RwV2d that describes the camera's view window.
 *
 * \see RwCameraClone
 * \see RwCameraCreate
 * \see RwCameraGetViewOffset
 * \see RwCameraSetViewOffset
 * \see RwCameraSetViewWindow
 *
 */
const RwV2d *
RwCameraGetViewWindow(const RwCamera *camera)
{
    RWAPIFUNCTION(RWSTRING("RwCameraGetViewWindow"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    RWRETURN(RwCameraGetViewWindowMacro(camera));
}

/**
 * \ingroup rwcamera
 * \ref RwCameraGetViewMatrix is used to retrieve the specified
 * camera's view-transformation matrix.
 *
 * This matrix can be used to transform world coordinate positions to camera
 * clip space. The camera's clip space is bounded by the near and far clip
 * planes, and side planes defined as follows:
 *
 * \li x = 0, x = 1, y = 0 and y = 1 for a parallel projection model
 * \li x = 0, x = z, y = 0 and y = z for a perspective projection model
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param camera  Pointer to the camera to query.
 *
 * \return Returns pointer to the specified camera's view matrix.
 *
 */
RwMatrix *
RwCameraGetViewMatrix(RwCamera *camera)
{
    RWAPIFUNCTION(RWSTRING("RwCameraGetViewMatrix"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    RWRETURN(RwCameraGetViewMatrixMacro(camera));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rwcamera
 * \ref RwCameraRegisterPlugin is used to register a plugin and
 * reserve some space within a camera. This must happen after the engine
 * has been initialized but before the engine is opened.
 *
 * \param size  A RwInt32 value equal to the size of the memory block to reserve.
 * \param pluginID  A RwInt32 value equal to the plugin ID (must be unique; used
 *       to identify binary chunks).
 * \param constructCB  Constructor for the plugin data block.
 * \param destructCB  Destructor for the plugin data block.
 * \param copyCB  Copy constructor for the plugin data block.
 *
 * \return Returns an RwInt32 containing the byte offset within the camera of
 * memory reserved for this plugin, or -1 if an error occurred.
 *
 * \see RwCameraValidatePlugins
 * \see RwCameraRegisterPluginStream
 * \see RwCameraSetStreamAlwaysCallBack
 * \see RwCameraGetPluginOffset
 *
 */
RwInt32
RwCameraRegisterPlugin(RwInt32 size,
                       RwUInt32 pluginID,
                       RwPluginObjectConstructor constructCB,
                       RwPluginObjectDestructor destructCB,
                       RwPluginObjectCopy copyCB)
{
    RwInt32 plug;

    RWAPIFUNCTION(RWSTRING("RwCameraRegisterPlugin"));
    RWASSERT(!cameraModule.numInstances);
    RWASSERT(size >= 0);

    /* Everything's cool, so pass it on */
    plug = _rwPluginRegistryAddPlugin(&cameraTKList, size, pluginID,
                                     constructCB, destructCB, copyCB);

    RWRETURN(plug);
}

/**
 * \ingroup rwcamera
 * \ref RwCameraGetPluginOffset is used to get the offset of a previously
 * registered camera plugin.
 *
 * \param pluginID  The plugin ID number identifying the plugin for which the offset is
 * required.
 *
 * \return Returns the data block offset, or -1 if the plugin is not registered.
 *
 * \see RwCameraRegisterPlugin
 * \see RwCameraRegisterPluginStream
 * \see RwCameraSetStreamAlwaysCallBack
 * \see RwCameraValidatePlugins
 *
 */
RwInt32
RwCameraGetPluginOffset(RwUInt32 pluginID)
{
    RwInt32 offset;

    RWAPIFUNCTION(RWSTRING("RwCameraGetPluginOffset"));

    offset = _rwPluginRegistryGetPluginOffset(&cameraTKList, pluginID);

    RWRETURN(offset);
}

/**
 * \ingroup rwcamera
 * \ref RwCameraValidatePlugins is used to validate the plugin memory
 * allocated within the specified camera. This function is useful for
 * determining where memory trampling may be occuring within an application.
 *
 * NOTE: This function only returns a meaningful response
 * under a debug library.
 *
 * \param  camera   Pointer to the camera to validate.
 *
 * \return Returns TRUE if the camera data is valid, FALSE if there is an
 * error, or if the camera data has become corrupt.
 *
 * \see RwCameraRegisterPlugin
 * \see RwCameraRegisterPluginStream
 * \see RwCameraSetStreamAlwaysCallBack
 * \see RwCameraGetPluginOffset
 *
 */
RwBool
RwCameraValidatePlugins(const RwCamera * camera __RWUNUSEDRELEASE__)
{
    RWAPIFUNCTION(RWSTRING("RwCameraValidatePlugins"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

#ifdef RWDEBUG
    {
        RwBool              valid;

        valid = _rwPluginRegistryValidateObject(&cameraTKList, camera);

        RWRETURN(valid);
    }
#else /* RWDEBUG */
    RWRETURN(TRUE);
#endif /* RWDEBUG */
}

#if (defined(RWDEBUG) || defined(RWSUPPRESSINLINE))

/**
 * \ingroup rwcamera
 * \ref RwCameraSetFrame is used to attach a frame to the specified
 * camera so that it can be positioned and oriented, as appropriate, within
 * a world.
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param camera  Pointer to the camera that will have its frame set.
 * \param frame  Pointer to the frame to attach to the camera.
 *
 * \return Returns a pointer to the camera.
 *
 * \see RwCameraCreate
 * \see RwCameraClone
 * \see RwCameraGetFrame
 *
 */
RwCamera *
RwCameraSetFrame(RwCamera *camera, RwFrame *frame)
{
    RWAPIFUNCTION(RWSTRING("RwCameraSetFrame"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    RWRETURN(RwCameraSetFrameMacro(camera, frame));
}

/**
 * \ingroup rwcamera
 * \ref RwCameraGetFrame returns a pointer to the frame attached to the
 * specified camera, (if any).
 *
 * Note that this function is used for debug purposes only and, for
 * efficiency, is available as a macro for final release versions of an
 * application.
 *
 * \param camera  Pointer to the camera to be referenced.
 *
 * \return Returns a pointer to the specified camera's frame.
 *
 * \see RwCameraSetFrame
 * \see RwCameraCreate
 * \see RwCameraClone
 *
 */
RwFrame *
RwCameraGetFrame(const RwCamera *camera)
{
    RWAPIFUNCTION(RWSTRING("RwCameraGetFrame"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    RWRETURN(RwCameraGetFrameMacro(camera));
}

#endif /* (defined(RWDEBUG) || defined(RWSUPPRESSINLINE)) */

/**
 * \ingroup rwcamera
 * \ref RwCameraDestroy destroys the specified camera. It is recommended
 * that the camera's frame, image raster and Z-buffer raster are destroyed
 * first before actually calling \ref RwCameraDestroy as they are not
 * explicitly destroyed by this function.
 *
 * \param camera  Pointer to the camera to destroy.
 *
 * \return Returns TRUE.
 *
 * \see RwCameraCreate
 * \see RwCameraClone
 * \see RwCameraSetFrame
 * \see RwCameraSetRaster
 * \see RwCameraSetZRaster
 * \see RwCameraGetFrame
 * \see RwCameraGetRaster
 * \see RwCameraGetZRaster
 * \see RwFrameDestroy
 * \see RwRasterDestroy
 *
 */
RwBool
RwCameraDestroy(RwCamera *camera)
{
    RWAPIFUNCTION(RWSTRING("RwCameraDestroy"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    /* De-initialize the plugin memory */
    _rwPluginRegistryDeInitObject(&cameraTKList, camera);

    /* Destroy the RW camera */
    _rwObjectHasFrameReleaseFrame(camera);

    RwFreeListFree(RWCAMERAGLOBAL(cameraFreeList), camera);

    RWRETURN(TRUE);
}

/**
 * \ingroup rwcamera
 * \ref RwCameraCreate creates a new camera. Before the camera can be used
 * to render a view of a scene, a frame, image raster and Z-buffer raster must
 * be attached to the camera.
 *
 * The associated frame enables the camera to be positioned and oriented
 * (i.e. pointed) within a world. The image raster is used for storing the
 * rendered image. The Z-buffer raster contains depth information used for
 * hidden surface calculations.
 *
 * Finally, the camera must also be added to the world in which it is to be
 * used for rendering.
 *
 * When a frame is defined, the camera is positioned at the origin and is oriented
 * such that its view direction is along the positive z-axis and 'up' is along
 * the positive y-axis. Transformations applied to the frame can be used to move
 * and re-orient the camera as required.
 *
 * The camera is created with a pre-defined view-window, view-offset and
 * near and far clip-planes. Initially, the view-window is a unit square
 * with a view-offset of zero, giving a 'look-at' direction
 * passing through the center of the rendered image. The near and far clip-planes
 * are positioned at distances of 0.05 and 10.0 units, respectively, from the
 * camera's local origin, measured along the 'look-at' direction. Both the view
 * plane and near and far clip-planes are perpendicular to the view direction.
 *
 * Note that the view-window is fixed at unit distance from the camera and cannot
 * be moved. In this situation changes to the aspect ratio and angular
 * field-of-view of the rendered image can be achieved by defining new values for
 * the width and height of the view-window.
 *
 * The default projection model is rwPERSPECTIVE.
 *
 * \return Returns pointer to the created camera if successful, or NULL
 * if an error occurred.
 *
 * \see RwCameraClone
 * \see RwCameraDestroy
 * \see RwCameraGetProjection
 * \see RwCameraSetFarClipPlane
 * \see RwCameraSetNearClipPlane
 * \see RwCameraSetProjection
 * \see RwCameraSetRaster
 * \see RwCameraSetViewWindow
 * \see RwCameraSetViewOffset
 * \see RwCameraSetZRaster
 * \see RwRasterCreate
 * \see RpWorldAddCamera
 * \see RpWorldRemoveCamera
 *
 */
RwCamera *
RwCameraCreate(void)
{
    RwCamera    *camera;

    RWAPIFUNCTION(RWSTRING("RwCameraCreate"));
    RWASSERT(cameraModule.numInstances);

    camera = (RwCamera *)RwFreeListAlloc(RWCAMERAGLOBAL(cameraFreeList),
                                         rwMEMHINTDUR_EVENT | rwID_CAMERA);
    RWASSERT(RWFREELISTALIGNED(camera, RWCAMERAGLOBAL(cameraFreeList)));
    if (!camera)
    {
        RWRETURN((RwCamera *)NULL);
    }

    /* If we add this to a world, the sync function turns into something else */
    rwObjectHasFrameInitialize(camera, rwCAMERA, 0, CameraSync);

    /* Begin/end camera update functions - must happen before plugin init */
    camera->beginUpdate = CameraBeginUpdate;
    camera->endUpdate = CameraEndUpdate;

    /* Set up the defaults for the camera */
    camera->viewWindow.x = camera->viewWindow.y = 1.0f;
    camera->recipViewWindow.x = camera->recipViewWindow.y = 1.0f;
    camera->viewOffset.x = camera->viewOffset.y = 0.0f;

    /* Clipping planes */
    camera->nearPlane = (RwReal)((0.05));
    camera->farPlane = (RwReal)((10.0));
    camera->fogPlane = (RwReal)((5.0));

    /* Render destination rasters */
    camera->frameBuffer = (RwRaster *)NULL;
    camera->zBuffer = (RwRaster *)NULL;

    /* Set up projection type */
    camera->projectionType = rwPERSPECTIVE;

    /* We can set up the Z buffer mapping now */
    CameraUpdateZShiftScale(camera);

    /* Initialize the view matrix */
    rwMatrixInitialize(&camera->viewMatrix, 0);

    /* Rendered atomics gets set up when we are added to the world */

    /* Initialize the plugin memory */
    _rwPluginRegistryInitObject(&cameraTKList, camera);

    /* Its created ! */
    RWRETURN(camera);
}

/**
 * \ingroup rwcamera
 * \ref RwCameraClone is used to create a copy of the specified
 * camera.
 *
 * Note that the old and new cameras share the same frame, image raster
 * and Z-buffer raster. If the original camera has been added to a world,
 * the new camera is also in that world.
 *
 * The following camera attributes are copied directly:
 * \li view-window dimensions
 * \li view-offset
 * \li near and far clip-plane distances
 * \li fog plane distance
 * \li camera projection type
 *
 * \param camera  Pointer to the camera.
 *
 * \return a pointer to the new camera if successful, or NULL if an error
 * occurred.
 *
 * \see RwCameraCreate
 * \see RwCameraDestroy
 * \see RwCameraGetFrame
 * \see RwCameraGetRaster
 * \see RwCameraGetZRaster
 * \see RwCameraSetFrame
 * \see RwCameraSetRaster
 * \see RwCameraSetZRaster
 *
 */
RwCamera *
RwCameraClone(RwCamera *camera)
{
    RwCamera    *oldCamera, *newCamera;

    RWAPIFUNCTION(RWSTRING("RwCameraClone"));
    RWASSERT(cameraModule.numInstances);
    RWASSERT(camera);
    RWASSERTISTYPE(camera, rwCAMERA);

    oldCamera = camera;

    /* Create the camera */
    if (!(newCamera = RwCameraCreate()))
    {
        RWRETURN((RwCamera *)NULL);
    }

    /* Link to the original frame. */
    RwCameraSetFrame(newCamera, RwCameraGetFrame(oldCamera));

    /* Begin/end camera update functions -
     * must happen before plugin init
     */
    newCamera->beginUpdate = oldCamera->beginUpdate;
    newCamera->endUpdate = oldCamera->endUpdate;

    /* Set up the defaults for the camera */
    newCamera->viewWindow.x = oldCamera->viewWindow.x;
    newCamera->viewWindow.y = oldCamera->viewWindow.y;

    newCamera->recipViewWindow.x = oldCamera->recipViewWindow.x;
    newCamera->recipViewWindow.y = oldCamera->recipViewWindow.y;

    newCamera->viewOffset.x = oldCamera->viewOffset.x;
    newCamera->viewOffset.y = oldCamera->viewOffset.y;

    /* Clipping planes */
    newCamera->nearPlane = oldCamera->nearPlane;
    newCamera->farPlane = oldCamera->farPlane;
    newCamera->fogPlane = oldCamera->fogPlane;

    /* Render destination rasters */
    RwCameraSetRaster(newCamera, RwCameraGetRaster(oldCamera));
    RwCameraSetZRaster(newCamera, RwCameraGetZRaster(oldCamera));

    /* Set up projection type */
    newCamera->projectionType = oldCamera->projectionType;

    /* We can set up the Z buffer mapping now */
    CameraUpdateZShiftScale(newCamera);

    /* Copy the camera's plugin data */
    _rwPluginRegistryCopyObject(&cameraTKList, newCamera, oldCamera);

    /* All done */
    RWRETURN(newCamera);
}
