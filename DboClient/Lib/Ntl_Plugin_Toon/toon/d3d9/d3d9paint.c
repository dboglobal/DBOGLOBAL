#include <d3d9.h>

#include "rwplcore.h"
#include "rwcore.h"
#include "rpworld.h"

#include "rpplugin.h"
#include "rpdbgerr.h"

#include "toonink.h"
#include "toonpaint.h"
#include "toonmaterial.h"
#include "toongeo.h"
#include "toon.h"
#include "edgerender.h"
#include "brutesilhouette.h"
#include "crease.h"

#include "d3d9toon.h"

#include "toonpixelshader.h"
#include "toonpixelshadernobase.h"
#include "toonpixelshadernocolor.h"
#include "toonpixelshadernobasenocolor.h"

#include "toonsilhouettevertexshader.h"

//by Hodong(2006.1.2)
#include "NtlMaterialExt.h"

#include <d3dx9.h>

RpAtomicCallBackRender RpNtlToonNotEdgeTwoSideCallBack;			
RpAtomicCallBackRender RpNtlToonEdgeTwoSideCallBack;
RpAtomicCallBackRender RpNtlToonDefaultCallBack;

#define COLORSCALAR 0.003921568627450980392156862745098f /* 1.0f/ 255.0f */

#define RENDERLINES


#if (defined(__GNUC__) && defined(__cplusplus))
#define D3DMatrixInitMacro(_XX, _XY, _XZ, _XW,  \
	_YX, _YY, _YZ, _YW,  \
	_ZX, _ZY, _ZZ, _ZW,  \
	_WX, _WY, _WZ, _WW ) \
{                                             \
{ { {   (_XX), (_XY), (_XZ), (_XW) },       \
{   (_YX), (_YY), (_YZ), (_YW) },       \
{   (_ZX), (_ZY), (_ZZ), (_ZW) },       \
{   (_WX), (_WY), (_WZ), (_WW) }        \
} }                                         \
}
#endif /* (defined(__GNUC__) && defined(__cplusplus)) */


#if (!defined(D3DMatrixInitMacro))
#define D3DMatrixInitMacro(_XX, _XY, _XZ, _XW,  \
	_YX, _YY, _YZ, _YW,  \
	_ZX, _ZY, _ZZ, _ZW,  \
	_WX, _WY, _WZ, _WW ) \
{                                             \
	(_XX), (_XY), (_XZ), (_XW),                \
	(_YX), (_YY), (_YZ), (_YW),                \
	(_ZX), (_ZY), (_ZZ), (_ZW),                \
	(_WX), (_WY), (_WZ), (_WW)                 \
}
#endif /* (!defined(D3DMatrixInitMacro)) */

typedef void (*PaintRender)(const RxD3D9ResEntryHeader  *resEntryHeader,
							const RxD3D9InstanceData *mesh,
							RwUInt32 flags,
							const RpToonPaint *paint);

typedef void (*PaintClose)(void);

static RxPipeline   *D3D9ToonAtomicPipeline = NULL,
*D3D9ToonSectorPipeline = NULL;

static D3DMATRIX    D3D9CameraSpaceLightmatrix =
D3DMatrixInitMacro(0.0f, 0.0f, 0.0f, 0.0f,
				   0.0f, 1.0f, 0.0f, 0.0f,
				   0.0f, 0.0f, 1.0f, 0.0f,
				   0.0f, 0.0f, 0.0f, 1.0f);

static D3DMATRIX    mD3D9IdentityMatrix =
D3DMatrixInitMacro(0.0f, 0.0f, 0.0f, 0.0f,
				   0.0f, 1.0f, 0.0f, 0.0f,
				   0.0f, 0.0f, 1.0f, 0.0f,
				   0.0f, 0.0f, 0.0f, 1.0f);

static RwBool   VideoCardSupportsMultitexture = TRUE;

static RwV3d *_rpToonD3D9TempVertexPositions = NULL;

static void *D3D9ToonPixelShader = NULL;
static void *D3D9ToonPixelShaderNoBase = NULL;
static void *D3D9ToonPixelShaderNoColor = NULL;
static void *D3D9ToonPixelShaderNoBaseNoColor = NULL;

static void *FastSilhouetteVertexShader = NULL;

/**
* \ingroup rptoond3d9
* \page rptoond3d9optimized Optimized Silhouette Rendering
*
* We provide two toon silhouette rendering solutions for this platform.
* The generic version supports a large number of features.
* Many of these features have been removed in the optimized version and,
* as a result, a great deal of extra optimisations can be made.
* The features not supported in the optimized version are:
* \li Crease edges.
* \li Per vertex thickness.
* \li An ink per edge.
*
* This is the limitation for the supported features:
* \li The model \e does need average normals.
*
*/

/****************************************************************************
FindFirstGlobalLight
*/
static RpLight *
FindFirstGlobalLight(void)
{
	RwLLLink    *cur, *end;
	RpWorld     *world;

	RWFUNCTION(RWSTRING("FindFirstGlobalLight"));

	world = (RpWorld *)RWSRCGLOBAL(curWorld);

	cur = rwLinkListGetFirstLLLink(&world->directionalLightList);
	end = rwLinkListGetTerminator(&world->directionalLightList);
	while (cur != end)
	{
		RpLight *light;

		light = rwLLLinkGetData(cur, RpLight, inWorld);

		/* NB light may actually be a dummyTie from a enclosing ForAll */
		if (light && (rwObjectTestFlags(light, rpLIGHTLIGHTATOMICS)))
		{
			switch (RpLightGetType(light))
			{
			case rpLIGHTAMBIENT:
				/* nothing */
				break;

			case rpLIGHTDIRECTIONAL:
				RWRETURN(light);
				break;

			default:
				RWASSERT(0); /* unsupported light type */
			}
		}

		/* Next */
		cur = rwLLLinkGetNext(cur);
	} /* while */

	RWRETURN(0);
}

static void
UpdateCameraSpaceLightMatrix(void)
{
	RWFUNCTION(RWSTRING("UpdateCameraSpaceLightMatrix"));

	if (NULL != RWSRCGLOBAL(curWorld))
	{
		RpLight *dirLight = FindFirstGlobalLight();
		//RWASSERT((dirLight != 0) && "No directional light found");
		if (dirLight)
		{
			/* Set the lights direction, in D3D's "camera space" to match D3DTSS_TCI_CAMERASPACENORMAL */
			RwCamera    *camera;
			RwFrame     *cameraFrame;
			RwFrame     *lightFrame;
			RwMatrix    inverseCamLTM;
			RwV3d       *at, transLight;

			camera = RwCameraGetCurrentCamera();
			RWASSERT(0 != camera);

			cameraFrame = RwCameraGetFrame(camera);
			RWASSERT(0 != cameraFrame);

			lightFrame = RpLightGetFrame(dirLight);


			RwMatrixInvert(&inverseCamLTM, RwFrameGetLTM(cameraFrame));

			at = RwMatrixGetAt(RwFrameGetLTM(lightFrame));

			RwV3dTransformVector (
				&transLight,
				at,
				&inverseCamLTM );

			D3D9CameraSpaceLightmatrix._11 = transLight.x;
			D3D9CameraSpaceLightmatrix._21 = -transLight.y;
			D3D9CameraSpaceLightmatrix._31 = -transLight.z;
		}
	} /* if world */

	RWRETURNVOID();
}

static void
FlatRender( const RxD3D9ResEntryHeader  *resEntryHeader,
		   const RxD3D9InstanceData *mesh,
		   RwUInt32 flags,
		   const RpToonPaint *paint __RWUNUSED__)
{
	RWFUNCTION(RWSTRING("FlatRender"));

	/* use the material color or not depending on the flags */
	if (flags & rxGEOMETRY_MODULATE)
	{
		/* modulate against material color */
		const RwRGBA  *color = &mesh->material->color;
		D3DCOLOR    matColor;

		matColor = (((RwUInt32)color->alpha) << 24) |
			(((RwUInt32)color->red) << 16) |
			(((RwUInt32)color->green) << 8) |
			color->blue;

		RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, matColor );
	}
	else
	{
		/* just modulate against opaque white (does nothing) */
		RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, 0xffffffff );
	}

	if ( (flags & (rxGEOMETRY_TEXTURED | rpGEOMETRYTEXTURED2)) != 0 &&
		mesh->material->texture != NULL )
	{
		RwD3D9SetTexture(mesh->material->texture, 0);

		/* use ordinary UV set for texture 0 */
		RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0 );

		/* blend material color with texture color */
		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);
	}
	else
	{
		RwD3D9SetTexture(NULL, 0);

		/* just pick up material color */
		RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
		RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
		RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
	}

	if (resEntryHeader->indexBuffer != NULL)
	{
		/* Draw the indexed primitive */
		RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
			mesh->baseIndex,
			0, mesh->numVertices,
			mesh->startIndex,
			mesh->numPrimitives);
	}
	else
	{
		RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
			mesh->baseIndex,
			mesh->numPrimitives);
	}

	RWRETURNVOID();
}

static void
FlatClose(void)
{
	RWFUNCTION(RWSTRING("FlatClose"));

	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	RWRETURNVOID();
}

static D3DMATRIX    D3D9IdentyMatrix = D3DMatrixInitMacro( 0.0f, 0.0f, 0.0f, 0.0f,
														  0.0f, 1.0f, 0.0f, 0.0f,
														  0.0f, 0.0f, 1.0f, 0.0f,
														  0.0f, 0.0f, 0.0f, 1.0f);

static void ToonShadeRender(const RxD3D9ResEntryHeader* resEntryHeader, const RxD3D9InstanceData *mesh, RwUInt32 flags, const RpToonPaint* paint)
{
	RwTexture*		multiTexture = NULL;
	const RwRGBA*	color = &mesh->material->color;
	sMatExtInfo		Param; 

	/*
	Load up the gradient texture w/ transform to do a dot product of light direction with normals,
	texture addressing mode will perform the clamping for us.
	In the case where there's no light set, this matrix will pick a texel with uv coordinate (0,0)
	which should be the darkest shade for that texture.
	*/

	RWFUNCTION(RWSTRING("ToonShadeRender"));

	// go through the proper renderpipe
	Param.flags					= flags;
	Param.resEntryHeader		= resEntryHeader;
	Param.mesh					= mesh;
	Param.pMaterial				= mesh->material;
	Param.pCamSpaceLightMatrix	= &D3D9CameraSpaceLightmatrix;
	Param.pToonTex				= paint->gradient;
	Param.pSkinColor			= RpNtlMaterialExtGetSkinColor(mesh->material);
	Param.pEmblemTex			= RpNtlMaterialExtGetEmblemTex(mesh->material);
	Param.pDogiColor			= RpNtlMaterialExtGetDogiColor(mesh->material);
	RpNtlMaterialExecuteRenderCB(&Param);

	RWRETURNVOID();
}

static void
ToonShadeClose(void)
{
	RWFUNCTION(RWSTRING("ToonShadeClose"));

	/* turn off uv generation */
	RwD3D9SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );

	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	RwD3D9SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

	RwD3D9SetTexture(NULL, 1);

	RwD3D9SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	RwD3D9SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	RwD3D9SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);

	//HoDong
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE);

	RWRETURNVOID();
}

static void
ToonShadePSRender( const RxD3D9ResEntryHeader  *resEntryHeader,
				  const RxD3D9InstanceData *mesh,
				  RwUInt32 flags,
				  const RpToonPaint *paint )
{
	RWFUNCTION(RWSTRING("ToonShadePSRender"));

	//by HoDong(2006.1.2)
	ToonShadeRender(resEntryHeader, mesh, flags, paint);

	RWRETURNVOID();
}


static void
ToonShadePSClose(void)
{
	ToonShadeClose();
}

static PaintRender _paintRender[RPTOON_PAINTTYPE_COUNT] =
{
	FlatRender,
	ToonShadeRender
};

static PaintClose _paintClose[RPTOON_PAINTTYPE_COUNT] =
{
	FlatClose,
	ToonShadeClose
};

static void
_rpToonD3D9UpdateVertexInfoFromVertexBuffer( RpToonGeo *toonGeo, RwResEntry *repEntry )
{
	/*
	The D3D9 Atomic pipe has just done some skinning/morphing into a vertex buffer.
	Lock it, and copy it into a format the line renders like.
	Yes, I know this is crap.
	Remove it at the first opportunity, e.g. with a vertex shader line renderer,
	or even by splitting the positions & normals into separate streams of RwV3ds
	(probably would make the behaviour of the skinner/morpher more write combining cache friendly as a bonus.)
	or meddle with the line render code to take a stream of positions & normals with a stride
	(will be a nuisance and possibly slow as well.)
	*/
	RxD3D9ResEntryHeader    *resEntryHeader;

	RWFUNCTION( RWSTRING( "_rpToonD3D9UpdateVertexInfoFromVertexBuffer" ) );

	resEntryHeader = (RxD3D9ResEntryHeader *)(repEntry + 1);

	if (resEntryHeader->totalNumVertex)
	{
		RxD3D9VertexStream      *vertexStream;
		RwInt32                 stride;
		RwUInt8                 *vertexData;
		RwV3d                   *position;
		RwV3d                   *normal;

		/* I suppose positions are always in the first stream */
		vertexStream = &(resEntryHeader->vertexStream[0]);

		stride = vertexStream->stride;

		if (SUCCEEDED(IDirect3DVertexBuffer9_Lock(
			(LPDIRECT3DVERTEXBUFFER9)(vertexStream->vertexBuffer),
			vertexStream->offset,
			(resEntryHeader->totalNumVertex) * stride,
			&vertexData,
			D3DLOCK_NOSYSLOCK)))
		{
			D3DVERTEXELEMENT9   declaration[18];
			RwUInt32            positionType, positionOffset;
			RwUInt32            normalType, normalOffset;
			RwUInt32            numVertices, n;
			const RwUInt8       *data;

			if (_rpToonD3D9TempVertexPositions == NULL)
			{
				_rpToonD3D9TempVertexPositions = (RwV3d *)RwMalloc(sizeof(RwV3d) * RPTOON_MAX_VERTICES,
					rwID_TOONPLUGIN | rwMEMHINTDUR_EVENT);
				RWASSERT((_rpToonD3D9TempVertexPositions != NULL) && "RwMalloc failed");
			}

			position = _rpToonD3D9TempVertexPositions;
			normal = toonGeo->extrusionVertexNormals;

			/* Calculates the texture coords offset */
			IDirect3DVertexDeclaration9_GetDeclaration(
				(LPDIRECT3DVERTEXDECLARATION9)resEntryHeader->vertexDeclaration,
				declaration,
				&n);

			RWASSERT(n < 18);

			/* Find positions and offsets */
			n = 0;
			while (declaration[n].Usage != D3DDECLUSAGE_POSITION ||
				declaration[n].UsageIndex != 0)
			{
				n++;
			}

			RWASSERT(n < 18);

			positionType = declaration[n].Type;
			positionOffset = declaration[n].Offset;

			/* normals */
			n = 0;
			while ((declaration[n].Usage != D3DDECLUSAGE_NORMAL ||
				declaration[n].UsageIndex != 0) &&
				declaration[n].Type != D3DDECLTYPE_UNUSED)
			{
				n++;
			}

			RWASSERT(n < 18);

			if (declaration[n].Type != D3DDECLTYPE_UNUSED)
			{
				normalType = declaration[n].Type;
				normalOffset = declaration[n].Offset;
			}
			else
			{
				normal = NULL;
			}

			data = vertexData;
			numVertices = resEntryHeader->totalNumVertex;

			if (positionType == D3DDECLTYPE_FLOAT3)
			{
				do
				{
					*position = *((const RwV3d *)(data + positionOffset));

					data += stride;
					position++;
				}
				while (--numVertices);
			}
			else
			{
				do
				{
					_rpD3D9VertexDeclarationUnInstV3d(positionType, position, data + positionOffset);

					data += stride;
					position++;
				}
				while (--numVertices);
			}

			if (normal != NULL)
			{
				data = vertexData;
				numVertices = resEntryHeader->totalNumVertex;

				if (normalType == D3DDECLTYPE_FLOAT3)
				{
					do
					{
						*normal = *((const RwV3d *)(data + normalOffset));

						data += stride;
						normal++;
					}
					while (--numVertices);
				}
				else
				{
					do
					{
						_rpD3D9VertexDeclarationUnInstV3d(normalType, normal, data + normalOffset);

						data += stride;
						normal++;
					}
					while (--numVertices);
				}
			}

			/* Unlock the vertex buffer */
			IDirect3DVertexBuffer9_Unlock((LPDIRECT3DVERTEXBUFFER9)(vertexStream->vertexBuffer));
		}
	}

	RWRETURNVOID();
}

static void
_rpToonD3D9RenderExtrudedSilhouette(RpToonGeo *toonGeo,
									RwResEntry *repEntry,
									RwMatrix *ltmMatrix)
{
	const RxD3D9ResEntryHeader  *resEntryHeader;
	const RxD3D9InstanceData    *mesh;
	RwInt32                 numMeshes;
	RwV4d matrixConstants[8];
	const RpToonInk *lastInk = NULL;
	RwCullMode  oldCullMode;
	RwUInt32  oldZFunc;
	RwUInt32 inkColor;

	RWFUNCTION(RWSTRING("_rpToonD3D9RenderExtrudedSilhouette"));

	_rwD3D9VSSetActiveWorldMatrix(ltmMatrix);

	_rwD3D9VSGetProjectionTransposedMatrix(matrixConstants + 0);
	_rwD3D9VSGetWorldViewTransposedMatrix(matrixConstants + 4);

	RwD3D9SetVertexShaderConstant(0, matrixConstants, 7);

	RwD3D9GetRenderState(D3DRS_ZFUNC, &oldZFunc);
	RwD3D9SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);

	RwRenderStateGet(rwRENDERSTATECULLMODE, (void*)&oldCullMode);
	RwRenderStateSet(rwRENDERSTATECULLMODE, (void*)rwCULLMODECULLFRONT);

	/* Disable vertex alpha blending */
	RwRenderStateSet(rwRENDERSTATEVERTEXALPHAENABLE, (void *)FALSE);

	/* Remove any texture */
	RwD3D9SetTexture(NULL, 0);

	/* Disable any pixel shader */
	RwD3D9SetPixelShader(NULL);

	/* Set color ops */
	RwD3D9SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG2);
	RwD3D9SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG2);
	RwD3D9SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

	/* render each mesh */
	resEntryHeader = (const RxD3D9ResEntryHeader *)(repEntry + 1);

	/* Set indices */
	RwD3D9SetIndices(resEntryHeader->indexBuffer);

	/* Vertex buffers */
	_rwD3D9SetStreams(resEntryHeader->vertexStream,
		resEntryHeader->useOffsets);

	/* Vertex declaration */
	RwD3D9SetVertexDeclaration(resEntryHeader->vertexDeclaration);

	/* Set vertex shader */
	RwD3D9SetVertexShader(FastSilhouetteVertexShader);

	numMeshes = resEntryHeader->numMeshes;
	mesh = (const RxD3D9InstanceData *)(resEntryHeader + 1);
	while (numMeshes--)
	{
		const RpToonMaterial *toonMat;
		const RpToonInk *currentInk = NULL;

		toonMat = *RPTOONMATERIALGETCONSTDATA(mesh->material);

		if (toonMat)
		{
			currentInk = toonGeo->inks[toonMat->silhouetteInkID];
		}
		else
		{
			currentInk = toonGeo->inks[toonGeo->edgeInkIDs[0].inkId[RPTOON_INKTYPE_SILHOUETTE]];
		}

		if (currentInk != NULL &&
			currentInk->farScale_factor != 0.0f)
		{
			if (lastInk != currentInk)
			{
				lastInk = currentInk;

				inkColor = (((RwUInt32)currentInk->color.alpha) << 24) |
					(((RwUInt32)currentInk->color.red) << 16) |
					(((RwUInt32)currentInk->color.green) << 8) |
					currentInk->color.blue;

				RwD3D9SetRenderState(D3DRS_TEXTUREFACTOR, inkColor);

				matrixConstants[7].x = (currentInk->farScale_factor) *
					(currentInk->perspectiveScale_b);

				matrixConstants[7].y = (currentInk->farScale_factor) *
					((currentInk->perspectiveScale_a) -
					(currentInk->perspectiveScale_b) * (currentInk->perspectiveScale_zMin));

				matrixConstants[7].z = 0.0f;
				matrixConstants[7].w = 1.0f;

				RwD3D9SetVertexShaderConstant(7, matrixConstants + 7, 1);
			}


			if (resEntryHeader->indexBuffer != NULL)
			{
				/* Draw the indexed primitive */
				RwD3D9DrawIndexedPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
					mesh->baseIndex,
					0, mesh->numVertices,
					mesh->startIndex,
					mesh->numPrimitives);
			}
			else
			{
				RwD3D9DrawPrimitive((D3DPRIMITIVETYPE)resEntryHeader->primType,
					mesh->baseIndex,
					mesh->numPrimitives);
			}


		}

		mesh++;
	}

	RwRenderStateSet(rwRENDERSTATECULLMODE, (void *)oldCullMode);

	RwD3D9SetRenderState(D3DRS_ZFUNC, oldZFunc);

	RWRETURNVOID();
}

void
_rpToonD3D9RenderCallback(RwResEntry *repEntry,
						  void *object,
						  RwUInt8 type,
						  RwUInt32 flags)
{
	const RxD3D9ResEntryHeader  *resEntryHeader;
	const RxD3D9InstanceData    *mesh;
	RwInt32                 numMeshes;
	RpToonGeo               *toonGeo;
	RwCamera                *cam;
	RpToonPaintType         lastPaintType = RPTOON_PAINTTYPE_COUNT;

	RWFUNCTION(RWSTRING("_rpToonD3D9RenderCallback"));

	/* Get current camera */
	cam = RwCameraGetCurrentCamera();
	RWASSERT(cam);

	/* grab the toon geometry info because it stores paint info we need */
	if (type == rpATOMIC)
	{
		RpAtomic    *atomic = (RpAtomic *)object;
		RpGeometry *g;

		/*
		count		= RpGeometryGetUserDataArrayCount(RpAtomicGetGeometry(pAtomic));
		pUserData	= RpGeometryGetUserDataArray(RpAtomicGetGeometry(pAtomic), 0);
		chBuffer	= RpUserDataArrayGetString(pUserData, 0);
		if(chBuffer != NULL)
			strcpy_s(g_ToonErrStr[0], 128, chBuffer);
		else
			strcpy_s(g_ToonErrStr[0], 128, "Atomic name is NULL");
			*/

		g = RpAtomicGetGeometry(atomic);

		// woody
		if(g)
		{
			//strcpy_s(g_ToonErrStr[0], 64, RpGeometryGetMaterial(g, 0)->texture->name);
		}
		else
		{
			strcpy_s(g_ToonErrStr[0], 64, "atomic is NULL");
		}

		RWASSERT((g != NULL) && "atomic can't have null geometry!");

		toonGeo = RpToonGeometryGetToonGeo(g);
		RWASSERT((toonGeo != NULL) && "atomic can't have null toon geometry!");

		/* Check clipping */
		if (RwD3D9CameraIsSphereFullyInsideFrustum(cam, RpAtomicGetWorldBoundingSphere(atomic)))
		{
			RwD3D9SetRenderState(D3DRS_CLIPPING, FALSE);
		}
		else
		{
			RwD3D9SetRenderState(D3DRS_CLIPPING, TRUE);
		}
	}
	else
	{
		RpWorldSector *sector = (RpWorldSector *)object;

		RWASSERT(sector != NULL);

		toonGeo = RpToonWorldSectorGetToonGeo(sector);
		RWASSERT((toonGeo != NULL) && "sector can't have null toon geometry!");

		/* Check clipping */
		if (RwD3D9CameraIsBBoxFullyInsideFrustum(cam, RpWorldSectorGetTightBBox(sector)))
		{
			RwD3D9SetRenderState(D3DRS_CLIPPING, FALSE);
		}
		else
		{
			RwD3D9SetRenderState(D3DRS_CLIPPING, TRUE);
		}
	}

	RWASSERT((toonGeo->paint != 0) && "must have a paint!");

	/* Update camera space light matrix if needed */
	if (flags & rxGEOMETRY_LIGHT)
	{
		UpdateCameraSpaceLightMatrix();
	}

	/* Default pixel shader */
	RwD3D9SetPixelShader(NULL);

	/* Set shared index buffer */
	resEntryHeader = (const RxD3D9ResEntryHeader *)(repEntry + 1);

	RwD3D9SetIndices(resEntryHeader->indexBuffer);

	/* Vertex buffers */
	_rwD3D9SetStreams(resEntryHeader->vertexStream,
		resEntryHeader->useOffsets);

	/* Vertex declaration */
	RwD3D9SetVertexDeclaration(resEntryHeader->vertexDeclaration);

	/* render each mesh */
	numMeshes = resEntryHeader->numMeshes;
	mesh = (const RxD3D9InstanceData *)(resEntryHeader + 1);
	while (numMeshes--)
	{
		RpToonPaint     *thePaint;
		RpToonPaintType thePaintType;

		const RpToonMaterial *toonMat = *RPTOONMATERIALGETCONSTDATA( mesh->material );

		if (mesh->vertexAlpha ||
			(0xFF != mesh->material->color.alpha))
		{
			_rwD3D9RenderStateVertexAlphaEnable(TRUE);
		}

		//외부에서 지정한 AlphaEnable이 지정이 안된다.(by HoDong)
		//else
		//{
		//_rwD3D9RenderStateVertexAlphaEnable(FALSE);
		//}

		/* use the toon material paint override if there is one */

		if (toonMat && toonMat->overrideGeometryPaint && toonMat->paint)
		{
			thePaint = toonMat->paint;
		}
		/* otherwise use the geometry paint */
		else
		{
			thePaint = toonGeo->paint;
		}

		if ( NULL == thePaint )
		{
			RWRETURNVOID();
		}

		/* Vertex shader */
		RwD3D9SetVertexShader(mesh->vertexShader);

		// woody
		if(thePaint)
		{
			//strcpy_s(g_ToonErrStr[1], 64, mesh->material->texture->name);
		}
		else
		{
			if(!mesh)
				//strcpy_s(g_ToonErrStr[1], 64, mesh->material->texture->name);
			//else
				strcpy_s(g_ToonErrStr[1], 64, "PAINT, MESH are NULL");
		}

		/* use the paint's paint type unless lightings turned off, in which case force flat */
		thePaintType = thePaint->type;

		if ( (flags & rxGEOMETRY_LIGHT) == 0 ||
			(thePaintType == RPTOON_PAINTTYPE_TOONSHADE &&
			thePaint->gradient == 0) )
		{
			thePaintType = RPTOON_PAINTTYPE_FLAT;
		}

		if (lastPaintType != thePaintType)
		{
			if (lastPaintType != RPTOON_PAINTTYPE_COUNT)
			{
				(_paintClose[lastPaintType])();
			}

			lastPaintType = thePaintType;
		}

		(_paintRender[thePaintType])(resEntryHeader, mesh, flags, thePaint); //callback

		mesh++;
	}

	if (lastPaintType != RPTOON_PAINTTYPE_COUNT)
	{
		(_paintClose[lastPaintType])();
	}

	RWRETURNVOID();
}

static RxPipeline *
_rpToonAtomicPipelineCreate(void)
{
	RxPipeline  *pipe;

	RWFUNCTION(RWSTRING("_rpToonAtomicPipelineCreate"));

	pipe = RxPipelineCreate();
	if (pipe)
	{
		RxLockedPipe    *lpipe;

		pipe->pluginId = rwID_TOONPLUGIN;

		lpipe = RxPipelineLock(pipe);
		if (NULL != lpipe)
		{
			RxNodeDefinition    *instanceNode;

			/*
			* Get the instance node definition
			*/
			instanceNode = RxNodeDefinitionGetD3D9AtomicAllInOne();

			/*
			* Add the node to the pipeline
			*/
			lpipe = RxLockedPipeAddFragment(lpipe, NULL, instanceNode, NULL);

			/*
			* Unlock the pipeline
			*/
			lpipe = RxLockedPipeUnlock(lpipe);

			RWRETURN(pipe);
		}

		RxPipelineDestroy(pipe);
	}

	RWRETURN(NULL);
}

static RxPipeline *
_rpToonSectorPipelineCreate(void)
{
	RxPipeline  *pipe;

	RWFUNCTION(RWSTRING("_rpToonSectorPipelineCreate"));

	pipe = RxPipelineCreate();
	if (pipe)
	{
		RxLockedPipe    *lpipe;

		pipe->pluginId = rwID_TOONPLUGIN;

		lpipe = RxPipelineLock(pipe);
		if (NULL != lpipe)
		{
			RxNodeDefinition    *instanceNode;

			/*
			* Get the instance node definition
			*/
			instanceNode = RxNodeDefinitionGetD3D9WorldSectorAllInOne();

			/*
			* Add the node to the pipeline
			*/
			lpipe = RxLockedPipeAddFragment(lpipe, NULL, instanceNode, NULL);

			/*
			* Unlock the pipeline
			*/
			lpipe = RxLockedPipeUnlock(lpipe);

			RWRETURN(pipe);
		}

		RxPipelineDestroy(pipe);
	}

	RWRETURN(NULL);
}

RwBool
_rpToonPipelinesCreate(void)
{
	RxNodeDefinition    *instanceNode;
	RxPipelineNode      *node;
	const D3DCAPS9      *d3dCaps;

	RWFUNCTION(RWSTRING("_rpToonPipelinesCreate"));

	/*
	* Create a new atomic pipeline
	*/
	D3D9ToonAtomicPipeline = _rpToonAtomicPipelineCreate();
	if (!D3D9ToonAtomicPipeline)
	{
		RWRETURN(FALSE);
	}

	instanceNode = RxNodeDefinitionGetD3D9AtomicAllInOne();
	RWASSERT(NULL != instanceNode);

	node = RxPipelineFindNodeByName(D3D9ToonAtomicPipeline, instanceNode->name, NULL, NULL);
	RWASSERT(NULL != node);

	/* Set Lighting callback - just turn it off for now to prevent hw lighting being set up */
	RxD3D9AllInOneSetLightingCallBack(node, NULL);

	RxD3D9AllInOneSetRenderCallBack(node, _rpToonD3D9RenderCallback);


	/*
	* Create a new world sector pipeline:
	*/
	D3D9ToonSectorPipeline = _rpToonSectorPipelineCreate();
	RWASSERT(NULL != D3D9ToonSectorPipeline);

	instanceNode = RxNodeDefinitionGetD3D9WorldSectorAllInOne();
	RWASSERT(NULL != instanceNode);

	node = RxPipelineFindNodeByName(D3D9ToonSectorPipeline, instanceNode->name, NULL, NULL);
	RWASSERT(NULL != node);

	/* Set Lighting callback - just turn it off for now to prevent hw lighting being set up */
	RxD3D9AllInOneSetLightingCallBack(node, NULL);

	RxD3D9AllInOneSetRenderCallBack(node, _rpToonD3D9RenderCallback);


	/*
	* Initialize edge renderer
	*/
	_rpToonEdgeRenderInit();
	_rpToonBruteSilhouetteInit();

	/* Get video card info */
	d3dCaps = (const D3DCAPS9 *)RwD3D9GetCaps();
	VideoCardSupportsMultitexture = (d3dCaps->MaxSimultaneousTextures > 2);

	//by HoDong(2006.12.17)
	if(!VideoCardSupportsMultitexture)
		RwDebugSendMessage(rwDEBUGMESSAGE, "Toon plugin", "Device supports not multitexture 2 and not render cartoon.");


	//by HoDong(2006.1.2)
	if ((d3dCaps->VertexShaderVersion & 0xFFFF) >= 0x0101)
	{
		RwD3D9CreateVertexShader(dwToonSilhouetteVertexShader, &FastSilhouetteVertexShader);
	}


	if ((d3dCaps->PixelShaderVersion & 0xFFFF) >= 0x0200)
	{
		RwDebugSendMessage(rwDEBUGMESSAGE, "Toon plugin", "Device supports pixel shaders version 2_0 and using them to normalize the normal per pixel.");

		RwD3D9CreatePixelShader(dwToonPixelShader,
			&D3D9ToonPixelShader);

		RwD3D9CreatePixelShader(dwToonPixelShaderNoBase,
			&D3D9ToonPixelShaderNoBase);

		RwD3D9CreatePixelShader(dwToonPixelShaderNoColor,
			&D3D9ToonPixelShaderNoColor);

		RwD3D9CreatePixelShader(dwToonPixelShaderNoBaseNoColor,
			&D3D9ToonPixelShaderNoBaseNoColor);

		_paintRender[RPTOON_PAINTTYPE_TOONSHADE] = ToonShadePSRender;

		_paintClose[RPTOON_PAINTTYPE_TOONSHADE] = ToonShadePSClose;
	}

	RWRETURN(TRUE);
}

RwBool
_rpToonPipelinesDestroy(void)
{
	RWFUNCTION(RWSTRING("_rpToonPipelinesDestroy"));

	if (D3D9ToonPixelShaderNoBaseNoColor != NULL)
	{
		RwD3D9DeletePixelShader(D3D9ToonPixelShaderNoBaseNoColor);
		D3D9ToonPixelShaderNoBaseNoColor = NULL;
	}

	if (D3D9ToonPixelShaderNoColor != NULL)
	{
		RwD3D9DeletePixelShader(D3D9ToonPixelShaderNoColor);
		D3D9ToonPixelShaderNoColor = NULL;
	}

	if (D3D9ToonPixelShaderNoBase != NULL)
	{
		RwD3D9DeletePixelShader(D3D9ToonPixelShaderNoBase);
		D3D9ToonPixelShaderNoBase = NULL;
	}

	if (D3D9ToonPixelShader != NULL)
	{
		RwD3D9DeletePixelShader(D3D9ToonPixelShader);
		D3D9ToonPixelShader = NULL;
	}

	if (FastSilhouetteVertexShader != NULL)
	{
		RwD3D9DeleteVertexShader(FastSilhouetteVertexShader);
		FastSilhouetteVertexShader = NULL;
	}

	RxPipelineDestroy(D3D9ToonAtomicPipeline);
	D3D9ToonAtomicPipeline = NULL;

	RxPipelineDestroy(D3D9ToonSectorPipeline);
	D3D9ToonSectorPipeline = NULL;

	if (_rpToonD3D9TempVertexPositions != NULL)
	{
		RwFree(_rpToonD3D9TempVertexPositions);
		_rpToonD3D9TempVertexPositions = NULL;
	}

	_rpToonBruteSilhouetteClean();
	_rpToonEdgeRenderClean();

	RWRETURN(TRUE);
}

RxPipeline *
_rpToonD3D9GetPipeline( void )
{
	RWFUNCTION(RWSTRING("_rpToonD3D9GetPipeline"));
	RWRETURN(D3D9ToonAtomicPipeline);
}

static RwBool
IsMorphing( RpGeometry *g )
{
	RWFUNCTION( RWSTRING( "IsMorphing" ) );

	RWRETURN( RpGeometryGetNumMorphTargets(g) > 1 );
}

static RpAtomic *
ToonAtomicRender( RpAtomic *atomic )
{
	RpToonGeo *toonGeo;
	RwV3d *verts = 0, *normals = 0;
	RwBool faceNormalsInvalid = FALSE;
	RpGeometry *g;
	RwMatrix *lineTransform;

	RWFUNCTION(RWSTRING("ToonAtomicRender"));

	g = RpAtomicGetGeometry(atomic);

	toonGeo = RpToonGeometryGetToonGeo(g);

	/* cell shade set up paint render with custom pipeline */
	(*(toonGeo->defAtomicRenderCallback))(atomic);

	normals = toonGeo->extrusionVertexNormals;

	if (IsMorphing(g) || toonGeo->isSkinned)
	{
		RwResEntry *repEntry;

		/*
		If the mesh was dynamic, update some info for the line renderers.
		now this is gonna be ugly and inefficient, but we can be clever later....
		*/
		RWASSERT( (rpGEOMETRYNORMALS & RpGeometryGetFlags(g)) && "skinned/morphed geometry must have normals for silhouette rendering");

		/* If the geometry has more than one morph target the resEntry in the
		* atomic is used else the resEntry in the geometry */
		if (g->numMorphTargets != 1)
		{
			repEntry = atomic->repEntry;
		}
		else
		{
			repEntry = g->repEntry;
		}

		/* normals are copied over toonGeo->extrusionVertexNormals! */
		_rpToonD3D9UpdateVertexInfoFromVertexBuffer(toonGeo, repEntry);

		faceNormalsInvalid = TRUE;

		verts = _rpToonD3D9TempVertexPositions;
	}
	else
	{
		/* just static data? */
		RpMorphTarget *morph;
		morph = RpGeometryGetMorphTarget(g,0);
		RWASSERT(morph != 0);
		verts = RpMorphTargetGetVertices( morph );

		/* maybe dmorph or some other thing going on in the render callback chain */
		if (toonGeo->vertexPositionsDirty)
		{
			faceNormalsInvalid = TRUE;

			if (RpMorphTargetGetVertexNormals( morph ))
			{
				normals = RpMorphTargetGetVertexNormals( morph );
			}

			toonGeo->vertexPositionsDirty = FALSE;
		}
	}

	_rpToonUpdateAtomicPerspectiveScale(toonGeo, atomic );

	/* need to transform stuff into world space */
	lineTransform = RwFrameGetLTM(RpAtomicGetFrame(atomic));

#ifdef RENDERLINES
	/* edge detection & render */
	{
		ScreenSpaceProjection   ssp;

		GetScreenSpaceProjection(&ssp, lineTransform);

		_rpToonEdgeRenderPushState();
		_rpToonBruteSilhouetteRender( toonGeo, verts, normals, faceNormalsInvalid, lineTransform, &ssp );
		_rpToonCreaseRender( toonGeo, verts, &ssp );
		_rpToonEdgeRenderPopState();
	}
#endif

	RWRETURN(atomic);
}

/**
* \ingroup rptoond3d9
* \ref RpD3D9ToonFastSilhouetteAtomicRenderCallback is the render callback that needs
* to be set in order to enable the optimized silhouette rendering on an atomic.
*
* \param atomic Pointer to the \ref RpAtomic to be rendered.
*
* \return Returns a pointer to the atomic if successful or NULL if there is an error.
*
* \see RpAtomicSetRenderCallBack
* \see RpD3D9ToonFastSilhouetteWorldSectorRenderCallback
*/
//HoDong
RpAtomic *
RpD3D9ToonFastSilhouetteAtomicRenderCallback(RpAtomic *atomic)
{
	RpToonGeo *toonGeo;
	RwV3d *verts = 0, *normals = 0;
	RwBool faceNormalsInvalid = FALSE;
	RpGeometry *g;
	RwMatrix *lineTransform;

	RWAPIFUNCTION(RWSTRING("RpD3D9ToonFastSilhouetteAtomicRenderCallback"));

	g = RpAtomicGetGeometry(atomic);

	toonGeo = RpToonGeometryGetToonGeo(g);

	/* cell shade set up paint render with custom pipeline */
	AtomicDefaultRenderCallBack(atomic);

	/* edge detection & render */
	if (FastSilhouetteVertexShader != NULL)
	{
		RwResEntry  *repEntry;
		RwMatrix    *ltmMatrix;

		/* If the geometry has more than one morph target the resEntry in the
		* atomic is used else the resEntry in the geometry */
		if (g->numMorphTargets != 1)
		{
			repEntry = atomic->repEntry;
		}
		else
		{
			repEntry = g->repEntry;
		}

		ltmMatrix = RwFrameGetLTM(RpAtomicGetFrame(atomic));

		RwD3D9SetRenderState(D3DRS_CLIPPING, TRUE);

		_rpToonUpdateAtomicPerspectiveScale(toonGeo, atomic);

		_rpToonD3D9RenderExtrudedSilhouette(toonGeo, repEntry, ltmMatrix);
	}
	else
	{
		normals = toonGeo->extrusionVertexNormals;

		if (IsMorphing(g) || toonGeo->isSkinned)
		{
			RwResEntry *repEntry;

			/*
			If the mesh was dynamic, update some info for the line renderers.
			now this is gonna be ugly and inefficient, but we can be clever later....
			*/
			RWASSERT( (rpGEOMETRYNORMALS & RpGeometryGetFlags(g)) && "skinned/morphed geometry must have normals for silhouette rendering");

			/* If the geometry has more than one morph target the resEntry in the
			* atomic is used else the resEntry in the geometry */
			if (g->numMorphTargets != 1)
			{
				repEntry = atomic->repEntry;
			}
			else
			{
				repEntry = g->repEntry;
			}

			/* normals are copied over toonGeo->extrusionVertexNormals! */
			_rpToonD3D9UpdateVertexInfoFromVertexBuffer(toonGeo, repEntry);

			faceNormalsInvalid = TRUE;

			verts = _rpToonD3D9TempVertexPositions;
		}
		else
		{
			/* just static data? */
			RpMorphTarget *morph;
			morph = RpGeometryGetMorphTarget(g,0);
			RWASSERT(morph != 0);
			verts = RpMorphTargetGetVertices( morph );

			/* maybe dmorph or some other thing going on in the render callback chain */
			if (toonGeo->vertexPositionsDirty)
			{
				faceNormalsInvalid = TRUE;

				if (RpMorphTargetGetVertexNormals( morph ))
				{
					normals = RpMorphTargetGetVertexNormals( morph );
				}

				toonGeo->vertexPositionsDirty = FALSE;
			}
		}

		_rpToonUpdateAtomicPerspectiveScale(toonGeo, atomic );

		/* need to transform stuff into world space */
		lineTransform = RwFrameGetLTM(RpAtomicGetFrame(atomic));

		/* edge detection & render */
		{
			ScreenSpaceProjection   ssp;

			GetScreenSpaceProjection(&ssp, lineTransform);

			_rpToonEdgeRenderPushState();
			_rpToonBruteSilhouetteRenderOptimized(toonGeo, verts, normals, faceNormalsInvalid, lineTransform, &ssp);
			_rpToonEdgeRenderPopState();
		}
	}

	RWRETURN(atomic);
}

RpAtomic *
RpD3D9ToonFastNotSilhouetteAtomicRenderCallback(RpAtomic *atomic)
{
	RpToonGeo *toonGeo;
	RwV3d *verts = 0, *normals = 0;
	RwBool faceNormalsInvalid = FALSE;
	RpGeometry *g;


	RWAPIFUNCTION(RWSTRING("RpD3D9ToonFastSilhouetteAtomicRenderCallback"));

	g = RpAtomicGetGeometry(atomic);

	toonGeo = RpToonGeometryGetToonGeo(g);

	/* cell shade set up paint render with custom pipeline */
	AtomicDefaultRenderCallBack(atomic);

	RWRETURN(atomic);
}

void
_rpToonAtomicChainAtomicRenderCallback(RpAtomic *atomic)
{
	RpToonGeo *toonGeo;

	RWFUNCTION(RWSTRING("_rpToonAtomicChainAtomicRenderCallback"));

	RWASSERT(atomic);

	/*
	Don't chain umpteem million times even if the stupid skinning streaming
	code calls us umpteem million times.
	*/
	toonGeo = RpToonGeometryGetToonGeo(RpAtomicGetGeometry(atomic));

	RWASSERT(toonGeo);

	if (RpAtomicGetRenderCallBack(atomic) != ToonAtomicRender)
	{
		if (RpAtomicGetRenderCallBack(atomic) != RpD3D9ToonFastSilhouetteAtomicRenderCallback)
		{
			toonGeo->defAtomicRenderCallback = RpAtomicGetRenderCallBack(atomic);
		}
		else
		{
			toonGeo->defAtomicRenderCallback = AtomicDefaultRenderCallBack;
		}

		toonGeo->defSectorRenderCallback = NULL;

		RpAtomicSetRenderCallBack(atomic, ToonAtomicRender);
	}

	RWRETURNVOID();
}

void
_rpToonAtomicChainSkinnedAtomicRenderCallback(RpAtomic *atomic)
{
	RpToonGeo *toonGeo;

	RWFUNCTION(RWSTRING("_rpToonAtomicChainSkinnedAtomicRenderCallback"));

	toonGeo = RpToonGeometryGetToonGeo(RpAtomicGetGeometry(atomic));

	RWASSERT(toonGeo);

	toonGeo->isSkinned = TRUE;

	_rpToonAtomicChainAtomicRenderCallback(atomic);

	RWRETURNVOID();
}

RwBool
_rpToonAtomicPipelinesAttach(RpAtomic *atomic)
{
	RxPipeline *pipeline;
	RpAtomic *success;

	RWFUNCTION(RWSTRING("_rpToonAtomicPipelinesAttach"));

	pipeline = D3D9ToonAtomicPipeline;
	RWASSERT(NULL != pipeline);

	success = RpAtomicSetPipeline(atomic, pipeline);

	_rpToonAtomicChainAtomicRenderCallback(atomic);


	//by HoDong(2006.1.2)
	RpNtlToonNotEdgeTwoSideCallBack = RpD3D9ToonFastNotSilhouetteAtomicRenderCallback;			
	RpNtlToonEdgeTwoSideCallBack    = ToonAtomicRender;
	RpNtlToonDefaultCallBack        = RpD3D9ToonFastSilhouetteAtomicRenderCallback;

	RWRETURN(success!=0);
}

static RpWorldSector *
ToonSectorRender( RpWorldSector *sector )
{
	RpToonGeo *toonGeo;

	RWFUNCTION(RWSTRING("ToonSectorRender"));

	toonGeo = RpToonWorldSectorGetToonGeo(sector);

	/* cell shade set up paint render with custom pipeline */
	(*(toonGeo->defSectorRenderCallback))(sector);

	/* sector might be a space filling one */
	if (toonGeo->numVerts)
	{
		/* edge detection & render */
		RWASSERT(sector->vertices && "sector has no vertices!");

		_rpToonUpdateWorldPerspectiveScale(toonGeo);

#ifdef RENDERLINES
		{
			ScreenSpaceProjection   ssp;

			GetScreenSpaceProjection(&ssp, NULL);

			_rpToonEdgeRenderPushState();
			_rpToonBruteSilhouetteRender( toonGeo, sector->vertices, toonGeo->extrusionVertexNormals, FALSE, NULL, &ssp );
			_rpToonCreaseRender( toonGeo, sector->vertices, &ssp );
			_rpToonEdgeRenderPopState();
		}
#endif
	}

	RWRETURN(sector);
}

/**
* \ingroup rptoond3d9
* \ref RpD3D9ToonFastSilhouetteWorldSectorRenderCallback is the render callback that needs
* to be set in order to enable the optimized silhouette rendering on a world sector.
*
* \param sector Pointer to the \ref RpWorldSector to be rendered.
*
* \return Returns a pointer to the world sector if successful or NULL if there is an error.
*
* \see RpWorldSetSectorRenderCallBack
* \see RpD3D9ToonFastSilhouetteAtomicRenderCallback
*/
RpWorldSector *
RpD3D9ToonFastSilhouetteWorldSectorRenderCallback( RpWorldSector *sector )
{
	RpToonGeo *toonGeo;

	RWAPIFUNCTION(RWSTRING("RpD3D9ToonFastSilhouetteWorldSectorRenderCallback"));

	/* cell shade set up paint render with custom pipeline */
	_rpSectorDefaultRenderCallBack(sector);

	toonGeo = RpToonWorldSectorGetToonGeo(sector);

	/* sector might be a space filling one */
#ifdef RENDERLINES
	if (toonGeo->numVerts)
	{
		/* edge detection & render */
		RWASSERT(sector->vertices && "sector has no vertices!");

		_rpToonUpdateWorldPerspectiveScale(toonGeo);

		if (FastSilhouetteVertexShader != NULL)
		{
			RwD3D9SetRenderState(D3DRS_CLIPPING, TRUE);

			_rpToonD3D9RenderExtrudedSilhouette(toonGeo, sector->repEntry, NULL);
		}
		else
		{
			ScreenSpaceProjection   ssp;

			GetScreenSpaceProjection(&ssp, NULL);

			_rpToonEdgeRenderPushState();
			_rpToonBruteSilhouetteRenderOptimized(toonGeo, sector->vertices, toonGeo->extrusionVertexNormals, FALSE, NULL, &ssp);
			_rpToonEdgeRenderPopState();
		}
	}
#endif

	RWRETURN(sector);
}

RwBool
_rpToonSectorPipelinesAttach(RpWorldSector *sector)
{
	RxPipeline *pipeline;
	RpWorldSector *success;
	RpWorld *world;
	RpToonGeo *toonGeo;
	static RpWorld *lastWorld = 0;
	static RpWorldSectorCallBackRender defSectorRenderCallback = 0;

	RWFUNCTION(RWSTRING("_rpToonSectorPipelinesAttach"));

	pipeline = D3D9ToonSectorPipeline;
	RWASSERT(NULL != pipeline);

	RWASSERT(sector != NULL);

	success = RpWorldSectorSetPipeline(sector, pipeline);

	toonGeo = RpToonWorldSectorGetToonGeo(sector);

	world = RpWorldSectorGetWorld(sector);
	RWASSERT(world != NULL);

	if (!lastWorld)
	{
		lastWorld = world;
		defSectorRenderCallback = RpWorldGetSectorRenderCallBack(world);
	}

	RWASSERT((lastWorld == world) && "multiple worlds TODO");

	/*    if (ToonSectorRender != RpWorldGetSectorRenderCallBack(world)) */
	{
		toonGeo->defSectorRenderCallback = defSectorRenderCallback; /* RpWorldGetSectorRenderCallBack(world); */

		RpWorldSetSectorRenderCallBack(world, ToonSectorRender);
	}

	toonGeo->defAtomicRenderCallback = NULL;

	RWRETURN(success!=0);
}
