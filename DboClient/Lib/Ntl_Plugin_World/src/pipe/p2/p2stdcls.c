/******************************************************************************
 *                                                                            *
 *  Module  :   p2stdcls.c                                                    *
 *                                                                            *
 *  Purpose :   Initialise the values of the global core cluster definitions  *
 *                                                                            *
 ******************************************************************************/

/****************************************************************************
 Includes
 */

#include "batypes.h"
#include "balibtyp.h"
#include "badebug.h"
#include "bamemory.h"
#include "p2core.h"
#include "p2stdcls.h"


/****************************************************************************
 Globals (across program)
 */

#define _DEFAULTSTRIDE     ((RwUInt32)0)
#define _DEFAULTATTRIBUTES ((RwUInt32)0)
#define _ATTRIBUTESET      ((const char *)NULL)

static RwChar       _RxObjSpace3DVertices_csl[] =
    RWSTRING("RxObjSpace3DVertices.csl");
RxClusterDefinition RxClObjSpace3DVertices =
{ /* Uses the RxObjSpace3DVertex type */
    _RxObjSpace3DVertices_csl,
    _DEFAULTSTRIDE, _DEFAULTATTRIBUTES, _ATTRIBUTESET
};

static RwChar       _RxCamSpace3DVertices_csl[] =
    RWSTRING("RxCamSpace3DVertices.csl");
RxClusterDefinition RxClCamSpace3DVertices =
{ /* Uses the RxCamSpace3DVertex type */
    _RxCamSpace3DVertices_csl,
    _DEFAULTSTRIDE, _DEFAULTATTRIBUTES, _ATTRIBUTESET
};

static RwChar       _RxScrSpace2DVertices_csl[] =
    RWSTRING("RxScrSpace2DVertices.csl");
RxClusterDefinition RxClScrSpace2DVertices =
{ /* Uses the RxScrSpace2DVertex type */
    _RxScrSpace2DVertices_csl,
    _DEFAULTSTRIDE, _DEFAULTATTRIBUTES, _ATTRIBUTESET
};

static RwChar       _RxInterpolants_csl[] =
    RWSTRING("RxInterpolants.csl");
RxClusterDefinition RxClInterpolants =
{ /* Uses the RxInterp type */
    _RxInterpolants_csl, _DEFAULTSTRIDE, _DEFAULTATTRIBUTES, _ATTRIBUTESET
};

static RwChar       _MeshState_csl[] =
    RWSTRING("MeshState.csl");
RxClusterDefinition RxClMeshState =
{ /* Uses the RxMeshStateVector type */
    _MeshState_csl, _DEFAULTSTRIDE, _DEFAULTATTRIBUTES, _ATTRIBUTESET
};

static RwChar       _RenderState_csl[] =
    RWSTRING("RenderState.csl");
RxClusterDefinition RxClRenderState =
{ /* Uses the RxRenderStateVector type */
    _RenderState_csl, _DEFAULTSTRIDE, _DEFAULTATTRIBUTES, _ATTRIBUTESET
};

static RwChar       _Indices_csl[] =
    RWSTRING("Indices.csl");
RxClusterDefinition RxClIndices =
{ /* Uses the RxVertexIndex type */
    _Indices_csl, _DEFAULTSTRIDE, _DEFAULTATTRIBUTES, _ATTRIBUTESET
};

static RwChar       _RxScatter_csl[] =
    RWSTRING("RxScatter.csl");
RxClusterDefinition RxClScatter =
{ /* Uses the RxScatter type */
    _RxScatter_csl, _DEFAULTSTRIDE, _DEFAULTATTRIBUTES, _ATTRIBUTESET
};

static RwChar       _RxUVs_csl[] =
    RWSTRING("RxUVs.csl");
RxClusterDefinition RxClUVs =
{ /* Uses the RxUV type */
    _RxUVs_csl, _DEFAULTSTRIDE, _DEFAULTATTRIBUTES, _ATTRIBUTESET
};

static RwChar       _RxVSteps_csl[] =
    RWSTRING("RxVSteps.csl");
RxClusterDefinition RxClVSteps =
{ /* Uses the RxVStep type */
    _RxVSteps_csl, _DEFAULTSTRIDE, _DEFAULTATTRIBUTES, _ATTRIBUTESET
};

static RwChar       _RGBAs_csl[] =
    RWSTRING("RGBAs.csl");
RxClusterDefinition RxClRGBAs =
{ /* Uses the RwRGBAReal type */
    _RGBAs_csl, _DEFAULTSTRIDE, _DEFAULTATTRIBUTES, _ATTRIBUTESET
};

static RwChar       _CamNorms_csl[] =
    RWSTRING("CamNorms.csl");

RxClusterDefinition RxClCamNorms =
{ /* Uses the RxCamNorm type */
    _CamNorms_csl, _DEFAULTSTRIDE, _DEFAULTATTRIBUTES, _ATTRIBUTESET
};


/* NOTE: this is currently not used in any nodes that ship with the SDK */
static RwChar       _RxTriPlanes_csl[] =
    RWSTRING("RxTriPlanes.csl");
RxClusterDefinition RxClTriPlanes =
{ /* Uses the RxTriPlane type */
    _RxTriPlanes_csl, _DEFAULTSTRIDE, _DEFAULTATTRIBUTES, _ATTRIBUTESET
};
