#include "precomp_ntlpresentation.h"
#include "NtlPLApi.h"
#include "NtlPLGlobal.h"
#include "NtlMath.h"
#include "NtlPLAttach.h"
#include "NtlPLEntityFreeList.h"
#include "NtlResourceEffect.h"
#include "NtlEffectSystemFreeList.h"
#include "NtlPLAtomicSorter.h"
#include "NtlPLResourceManager.h"
#include "NtlPLResourcePack.h"
#include "NtlPLResourceScheduling.h"
#include "NtlPLEmblemMaker.h"
#include "NtlPLPalette.h"
#include "NtlDefaultItemData.h"

RwBool API_PLEffectCreate(void)
{
	CNtlEffectSystemFreeList::m_strClumpTexturePath = PATH_EFFECT_TEXTURE;
 	CNtlEffectSystemFreeList::m_UvAnimationFileManager.AddExt(".uva");
	CNtlEffectSystemFreeList::m_UvAnimationFileManager.LoadPath(PATH_EFFECT_RESOURCE);

	CNtlEffectSystemFreeList::Create();

	return TRUE;
}

void API_PLEffectDestroy(void)
{
    CNtlEffectSystemFreeList::Destroy();
	CNtlResourceScheduleManager::DeAllocFreeList();
}

RwBool API_PLInit(void)
{
	CNtlPLGlobal::Init();
	if(!CNtlPLEntityFreeList::AllocFreeList())
		return FALSE;

	if(!CNtlPLAttach::AllocFreeList())
		return FALSE;

	if(!CNtlResourceScheduleManager::AllocFreeList())
		return FALSE;

	if(!API_PLEffectCreate())
		return FALSE;

	return TRUE;
}

RwTexDictionary* TexDictDestroyCB(RwTexDictionary *texDict, void *data __RWUNUSED__)
{
	RwTexDictionaryDestroy(texDict);

	return(texDict);
}

void API_PLTerminate(void)
{
	CNtlDefaultItemTable::GetInstance().Destroy();
	CNtlPLPalette::DestoryInstance();
	CNtlPLEmblemMaker::DestoryInstance();
	CNtlPLResourceManager::GetInstance()->Destroy();
	API_PLEffectDestroy();
	CNtlPLEntityFreeList::DeAllocFreeList();
	CNtlPLAttach::DeAllocFreeList();
	CNtlPLGlobal::Terminate();
	CNtlResourceScheduleManager::DeAllocFreeList();

	// Destroy any existing texture dictionaries (so that lightmaps don't get mistakenly carried across from the old world to the new one)
	RwTexDictionaryForAllTexDictionaries(TexDictDestroyCB, NULL);
}

RwChar* API_PL_PathnameCreate(const RwChar *srcBuffer)
{
	RwChar *dstBuffer;
	RwChar *charToConvert;

	dstBuffer = (RwChar *)RwMalloc(sizeof(RwChar) * (rwstrlen(srcBuffer) + 1),
		rwID_NAOBJECT);

	if( dstBuffer )
	{
		rwstrcpy(dstBuffer, srcBuffer);

		while( (charToConvert = rwstrchr(dstBuffer, '/')) )
		{
			*charToConvert = '\\';
		}
#if 0
		while( (charToConvert = rwstrchr(dstBuffer, ':')) )
		{
			*charToConvert = '\\';
		}
#endif
	}

	return dstBuffer;
}

void API_PL_PathnameDestroy(RwChar * buffer)
{
	if( buffer )
	{
		RwFree(buffer);
	}

	return;
}

RpClump * API_PL_LoadClump( const RwChar * filename )
{
	RwStream    *stream = NULL;
	RpClump     *clump = NULL;
	RwChar	    *pathName;
	void		*fp = NULL;

	//   
	pathName = API_PL_PathnameCreate(filename);

	SPackResFileData sPackFileData;
	BOOL bPack = GetNtlResourcePackManager()->LoadObject(filename, sPackFileData);

	if(bPack)
	{
		fp = RwFopen(sPackFileData.strPackFileName.c_str(), "rb");
		if(fp)
		{
			RwFseek(fp, sPackFileData.uiOffset, RTFILE_POS_BEGIN);
			stream = RwStreamOpen(rwSTREAMFILE, rwSTREAMREAD, fp);
		}
	}
	else
	{	
		stream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, pathName);
	}

	if(stream)
	{
		RwStreamFindChunk(stream, rwID_CLUMP, NULL, NULL);
		clump = (RpClump*)RpClumpStreamRead(stream);
		RwStreamClose(stream, NULL);
	}

	if(fp)
	{
		RwFclose(fp);
		fp = NULL;
	}

	API_PL_PathnameDestroy(pathName);

	if (clump == NULL)
	{
		DBO_WARNING_MESSAGE("fail: pathName: " << pathName << " filename: " << filename);
	}

	return clump;
}

RtDict*	API_PL_LoadUVAnimDict(const RwChar* strFullName)
{
	// Modify Fluorite(2007.03.21)
	// Desc: added code for resource pack.

    RtDict* pDict = NULL;
	RwStream* pStream = NULL;
	void *fp = NULL;

	SPackResFileData sPackFileData;
	RwBool bPack = GetNtlResourcePackManager()->LoadObject(strFullName, sPackFileData);
	if(bPack)
	{
		fp = RwFopen(sPackFileData.strPackFileName.c_str(), "rb");
		if(fp == NULL)
			return NULL;

		RwFseek(fp, sPackFileData.uiOffset, RTFILE_POS_BEGIN);

		pStream = RwStreamOpen(rwSTREAMFILE, rwSTREAMREAD, fp);
	}
	else
		pStream = RwStreamOpen(rwSTREAMFILENAME, rwSTREAMREAD, strFullName);

	if(pStream)
	{
		if(RwStreamFindChunk(pStream, rwID_UVANIMDICT, NULL, NULL))
		{
			pDict = RtDictSchemaStreamReadDict(RpUVAnimGetDictSchema(), pStream);
		}
		RwStreamClose(pStream, NULL);
	}

	if(fp)
		RwFclose(fp);

	return pDict;
}

RpAtomic * API_PL_LoadAtomic( const RwChar *filename, const RwChar * pTexturePath )
{
	RwImageSetPath( pTexturePath );

	RpClump *		clump = API_PL_LoadClump( filename );

	if( clump == NULL )
	{
		return NULL;
	}

	if( RpClumpGetNumAtomics( clump ) == 0 )
	{
		return NULL;
	}

	int count = RpClumpGetNumAtomics( clump );

	RwLLLink *	pLink = rwLinkListGetFirstLLLink( &(clump->atomicList) );
	RpAtomic *	pAtomic = rwLLLinkGetData( pLink, RpAtomic, inClumpLink );

	pAtomic = RpAtomicClone( pAtomic );

	RpAtomicSetFrame( pAtomic, RwFrameCreate() );

	RpGeometry * pGeometry = RpAtomicGetGeometry( pAtomic );
	//	RpCollisionGeometryBuildData( pGeometry, NULL );

	RpClumpDestroy( clump );

	return pAtomic;
}

RwBool API_PL_DeleteAtomic(RpAtomic*	pAtomic)
{
	NTL_FUNCTION("API_PLDeleteAtomic");

	RwFrame* pFrame = RpAtomicGetFrame(pAtomic);
	RpAtomicSetFrame(pAtomic, NULL);
	RwFrameDestroy(pFrame);
	RpAtomicDestroy(pAtomic);

	NTL_RETURN(TRUE);
}

RwBool API_PL_TranslationAtomic(RpAtomic*	pAtomic, RwV3d& Pos, RwOpCombineType OpCombineType)
{
	RwFrame* pFrame = RpAtomicGetFrame(pAtomic);
	pFrame = RwFrameTranslate(pFrame, &Pos, OpCombineType);
	RwFrameUpdateObjects(pFrame);

	return TRUE;
}

RwBool API_PL_RotationAtomic(RpAtomic*	pAtomic, RwReal Rot, RwOpCombineType OpCombineType)
{
	RwFrame* pFrame = RpAtomicGetFrame(pAtomic);
	RwFrameRotate(pFrame, &CNtlPLGlobal::m_vYAxisV3, Rot, OpCombineType);
	RwFrameUpdateObjects(pFrame);

	return TRUE;
}

RwTexture* API_PL_AtomicGetTexture(RpAtomic* pAtomic)
{
	RpGeometry*	pGeometry	= RpAtomicGetGeometry(pAtomic);
	RpMaterial*	pMaterial	= RpGeometryGetMaterial(pGeometry, 0);

	return RpMaterialGetTexture(pMaterial);
}

RwBool API_PL_AtomicSetTexture(RpAtomic* pAtomic, RwTexture* pTexture)
{
	RpGeometry*	pGeometry	= RpAtomicGetGeometry(pAtomic);
	RpMaterial*	pMaterial	= RpGeometryGetMaterial(pGeometry, 0);

	RpMaterialSetTexture(pMaterial, pTexture);

	return TRUE;
}

void API_PL_AtomicSetGeoFlags(RpAtomic* pAtomic, RwUInt32 Flags)
{
	RpGeometry*	pGeometry	= RpAtomicGetGeometry(pAtomic);
	RpMaterial*	pMaterial	= RpGeometryGetMaterial(pGeometry, 0);

	RpGeometrySetFlags(pGeometry, Flags);
}

RpMaterial*	API_PL_AtomicGetMaterial(RpAtomic* pAtomic)
{
	RpGeometry*	pGeometry	= RpAtomicGetGeometry(pAtomic);

	return RpGeometryGetMaterial(pGeometry, 0);
}

RwBool API_PL_DeleteTexture(RwTexture* pTexture)
{
	if(pTexture)
	{
		RwTextureDestroy(pTexture);
	}

	return TRUE;
}

void API_PL_AtomicSetMaterialSetAlpha(RpAtomic* pAtomic, RwUInt8 AlphaValue)
{
	RpMaterial* pMaterial = API_PL_AtomicGetMaterial(pAtomic);
	RwRGBA		RGBATmp;
	memcpy(&RGBATmp, RpMaterialGetColor(pMaterial), sizeof(RwRGBA));
	RGBATmp.alpha = AlphaValue;
	RpMaterialSetColor(pMaterial, &RGBATmp);
}

void API_PL_8BColorClamp(RwUInt8* pDst, RwInt32 Value)
{
	if((RwInt32)*pDst + Value < 0)
	{
		*pDst = 0;
		return;
	}
	else if((RwInt32)*pDst + Value > 255)
	{
		*pDst = 255;
		return;
	}

	*pDst = (RwInt32)*pDst + Value;
}

void API_PL_1BColorClamp(RwReal* pDst, RwReal Value)
{
	if(*pDst + Value < 0.0f)
	{
		*pDst = 0;
		return;
	}
	else if(*pDst + Value > 1.0f)
	{
		*pDst = 1.0f;
		return;
	}

	*pDst = *pDst + Value;
}

void API_PL_RasterSaveImage(const RwRaster *pRaster, const RwChar *pszFileName, RwInt32 _Width, RwInt32 _Height)    ///< Raster를 Image로 저장(.png, .bmp)
{
	RwImage *image;
	RwImage *imageDst;
	RwInt32 x, y;

	if( pRaster )
	{
		x			= RwRasterGetWidth(pRaster);
		y			= RwRasterGetHeight(pRaster);
		image		= RwImageCreate(x, y, 32);
		imageDst	= RwImageCreate(_Width, _Height, 32);

		if( image )
		{
			RwImageAllocatePixels(image);
			RwImageAllocatePixels(imageDst);
			RwImageSetFromRaster(image, const_cast<RwRaster *>(pRaster));
			RwImageResample(imageDst, image);
			RwImageWrite(imageDst, pszFileName);
			RwImageDestroy(image);
			RwImageDestroy(imageDst);
		}
	}
}

/**
 * Raster를 Image로 저장을 한다.
 * \param pRaster: Image로 저장할 Raster
 * \param pszFileName: 저장할 파일 이름 확장명(.png, .bmp)에 따라서 저장이 된다.
 */
void API_PL_RasterSaveImage(const RwRaster *pRaster, const RwChar *pszFileName)
{
	RwImage *image;
	RwInt32 x, y;
	
	if( pRaster )
	{
        x = RwRasterGetWidth(pRaster);
        y = RwRasterGetHeight(pRaster);
        image = RwImageCreate(x, y, 32);
        if( image )
        {
            RwImageAllocatePixels(image);
            RwImageSetFromRaster(image, const_cast<RwRaster *>(pRaster));
            RwImageWrite(image, pszFileName);
            RwImageDestroy(image);
        }
	}
}

/**
 * Camera의 Target을 Look At 할 수 있도록 Camera의 Matrix을 변경한다.
 * Camera의 Up이 y가 -1이 위쪽이다. 조심해서 사용
 */
void API_PL_CameraLookAt( RwCamera* pCamera, const RwV3d* pSetCameraPos, const RwV3d* pTarget, const RwV3d* pSentInUp, RwReal fAtAxisRot /* = 0.0f */ )
{
	const RwV3d xInternalUp = { 0.0f, -1.0f, 0.0f };
	RwV3d vUp;
	RwFrame *pCameraFrame;
	RwMatrix *pCameraMatrix;
	
	if( pSentInUp == NULL )
	{
		vUp = xInternalUp;
	}
	else
	{
		vUp = *pSentInUp;
	}

	pCameraFrame = RwCameraGetFrame( pCamera );
	pCameraMatrix = RwFrameGetMatrix( pCameraFrame );

	*RwMatrixGetPos( pCameraMatrix ) = *pSetCameraPos;

	RwV3dSubMacro( RwMatrixGetAt( pCameraMatrix ), pTarget, RwMatrixGetPos( pCameraMatrix ) );
	RwV3dNormalize( RwMatrixGetAt( pCameraMatrix ), RwMatrixGetAt( pCameraMatrix ) );

	if( fAtAxisRot != 0.0f)
	{
		RwV3d vAt;
		RwV3dAssign(&vAt, &pCameraMatrix->at);
		RwMatrix *pRot = RwMatrixCreate();
		RwMatrixRotate(pRot, &vAt, fAtAxisRot, rwCOMBINEPOSTCONCAT);
		RwMatrixUpdate( pRot );
		RwV3dTransformVector(&vUp, &vUp, pRot); 
		RwMatrixDestroy(pRot);
	}
	
	RwV3dCrossProduct( RwMatrixGetRight( pCameraMatrix ),RwMatrixGetAt( pCameraMatrix ), &vUp );
	RwV3dNormalize( RwMatrixGetRight( pCameraMatrix ), RwMatrixGetRight( pCameraMatrix ) );

	RwV3dCrossProduct( RwMatrixGetUp( pCameraMatrix ),RwMatrixGetAt( pCameraMatrix ),RwMatrixGetRight( pCameraMatrix ) );
	RwV3dNormalize( RwMatrixGetUp( pCameraMatrix ), RwMatrixGetUp( pCameraMatrix ) );

	RwMatrixUpdate( pCameraMatrix );
	RwFrameUpdateObjects( pCameraFrame );	
}

void API_PL_CameraMatrixLookAt( RwMatrix *pMat, const RwV3d* pSetCameraPos, const RwV3d* pTarget)
{
	const RwV3d xInternalUp = { 0.0f, -1.0f, 0.0f };
	const RwV3d *pUp;
		
	pUp = &xInternalUp;
	
	*RwMatrixGetPos( pMat ) = *pSetCameraPos;

	RwV3dSubMacro( RwMatrixGetAt( pMat ), pTarget, RwMatrixGetPos( pMat ) );
	RwV3dNormalize( RwMatrixGetAt( pMat ), RwMatrixGetAt( pMat ) );

	RwV3dCrossProduct( RwMatrixGetRight( pMat ),RwMatrixGetAt( pMat ), pUp );
	RwV3dNormalize( RwMatrixGetRight( pMat ), RwMatrixGetRight( pMat ) );

	RwV3dCrossProduct( RwMatrixGetUp( pMat ),RwMatrixGetAt( pMat ),RwMatrixGetRight( pMat ) );
	RwV3dNormalize( RwMatrixGetUp( pMat ), RwMatrixGetUp( pMat ) );

	RwMatrixUpdate( pMat );
}

/**
 * 3D좌표를 2D좌표로 변환한다.
 * \param v3DPos 변환할 3D좌표
 * \param nWidth View의 넓이
 * \param nHeight View의 높이
 * \param bOutSideReturn 이 값이 TRUE이면 카메라 밖에 있는 마이너스 값도 리턴해준다. (디폴트는 False). 다만 이때 3D좌표가 카메라 뒤편이면 (9999,9999)를 반환한다.
 * return 변환된 2D좌표. ((0,0)이면 카메라 밖에 있다)
 */
RwV2d API_PL_Calc3DPosTo2D( RwV3d* v3DPos, RwInt32 nWidth, RwInt32 nHeight, RwBool bOutSideReturn)
{
    RwV2d v2dReturn;
    v2dReturn.x = 0.0f;
    v2dReturn.y = 0.0f;

    RwSphere sphere;
	sphere.center = *v3DPos;    
    sphere.radius = 1.0f;

	if(!bOutSideReturn)
	{
		if( RwCameraFrustumTestSphere( CNtlPLGlobal::m_RwCamera, &sphere ) == rwSPHEREOUTSIDE )
			return v2dReturn;
	}

    RwV3d v2dPoint;
    RwMatrix* pViewMatrix = RwCameraGetViewMatrix( CNtlPLGlobal::m_RwCamera );
    RwV3dTransformPoints( &v2dPoint, &sphere.center, 1, pViewMatrix );

	if(v2dPoint.z <= 0.0f)	// z가 마이너스이면 카메라 뒤편에 있다. 이때는 (9999, 9999)를 반환한다.
	{
		v2dReturn.x = 9999.0f;
		v2dReturn.y = 9999.0f;
		return v2dReturn;
	}

    v2dPoint.x = ( v2dPoint.x / v2dPoint.z ) * nWidth;
    v2dPoint.y = ( v2dPoint.y / v2dPoint.z ) * nHeight;

    v2dReturn.x =  v2dPoint.x + 0.5f;
    v2dReturn.y =  v2dPoint.y + 0.5f;

    return v2dReturn;
}

RpAtomic* API_PL_RenderWireMesh( RpAtomic* atomic, void* data ) 
{
	RpGeometry *geometry;

	geometry = RpAtomicGetGeometry(atomic);

	if( geometry )
	{
		RwReal interpPos = 1.0f, invInterpPos = 0.0f;
		RpMorphTarget *morphTarget;
		RwInt32 nkf;
		RpTriangle *triangle;
		RwV3d *vertPosStart = (RwV3d *)NULL;
		RwV3d *vertPosEnd = (RwV3d *)NULL;
		RwMatrix *LTM;
		RwInt32 numTri, numImmVert, i;
		RwIm3DVertex *imVertex;
		RwRGBA Color = {0, 255, 255, 255};

		if (data != NULL)
		{
			Color = *((RwRGBA*)data);
		}

		nkf = RpGeometryGetNumMorphTargets(geometry);
		numTri = RpGeometryGetNumTriangles(geometry);
		imVertex = (RwIm3DVertex *)RwMalloc(numTri * 6 * sizeof(RwIm3DVertex),
			rwID_NAOBJECT);

		if( nkf > 1 )
		{
			RpInterpolator *interp;
			RwInt32 startMorphTarget, endMorphTarget;

			interp = RpAtomicGetInterpolator(atomic);

			interpPos = RpInterpolatorGetValue(interp) / RpInterpolatorGetScale(interp);

			invInterpPos = 1.0f - interpPos;

			startMorphTarget = RpInterpolatorGetStartMorphTarget(interp);
			endMorphTarget = RpInterpolatorGetEndMorphTarget(interp);

			morphTarget = RpGeometryGetMorphTarget(geometry, startMorphTarget);
			vertPosStart = RpMorphTargetGetVertices(morphTarget);

			morphTarget = RpGeometryGetMorphTarget(geometry, endMorphTarget);
			vertPosEnd = RpMorphTargetGetVertices(morphTarget);
		}
		else
		{
			morphTarget = RpGeometryGetMorphTarget(geometry, 0);
			vertPosStart = RpMorphTargetGetVertices(morphTarget);
		}

		triangle = RpGeometryGetTriangles(geometry);

		for( i = 0; i < numTri; i++ )
		{
			RwUInt16 vert0, vert1, vert2;
			RwV3d vertPos[3];

			RpGeometryTriangleGetVertexIndices(geometry, triangle, &vert0, &vert1, &vert2);

			if( nkf > 1 )
			{
				RwV3d tempVec1, tempVec2;

				RwV3dScale(&tempVec1, &vertPosStart[vert0], invInterpPos);
				RwV3dScale(&tempVec2, &vertPosEnd[vert0], interpPos);
				RwV3dAdd(&vertPos[0], &tempVec1, &tempVec2);

				RwV3dScale(&tempVec1, &vertPosStart[vert1], invInterpPos);
				RwV3dScale(&tempVec2, &vertPosEnd[vert1], interpPos);
				RwV3dAdd(&vertPos[1], &tempVec1, &tempVec2);

				RwV3dScale(&tempVec1, &vertPosStart[vert2], invInterpPos);
				RwV3dScale(&tempVec2, &vertPosEnd[vert2], interpPos);
				RwV3dAdd(&vertPos[2], &tempVec1, &tempVec2);

			}
			else
			{
				vertPos[0] = vertPosStart[vert0];
				vertPos[1] = vertPosStart[vert1];
				vertPos[2] = vertPosStart[vert2];
			}

			RwIm3DVertexSetPos(&imVertex[i*6+0], vertPos[0].x, vertPos[0].y, vertPos[0].z);
			RwIm3DVertexSetRGBA (&imVertex[i*6+0], Color.red, Color.green, Color.blue, Color.alpha);
			RwIm3DVertexSetPos(&imVertex[i*6+1], vertPos[1].x, vertPos[1].y, vertPos[1].z);
			RwIm3DVertexSetRGBA(&imVertex[i*6+1], Color.red, Color.green, Color.blue, Color.alpha);


			RwIm3DVertexSetPos(&imVertex[i*6+2], vertPos[1].x, vertPos[1].y, vertPos[1].z);
			RwIm3DVertexSetRGBA(&imVertex[i*6+2], Color.red, Color.green, Color.blue, Color.alpha);
			RwIm3DVertexSetPos(&imVertex[i*6+3], vertPos[2].x, vertPos[2].y, vertPos[2].z);
			RwIm3DVertexSetRGBA(&imVertex[i*6+3], Color.red, Color.green, Color.blue, Color.alpha);

			RwIm3DVertexSetPos(&imVertex[i*6+4], vertPos[2].x, vertPos[2].y, vertPos[2].z);
			RwIm3DVertexSetRGBA(&imVertex[i*6+4], Color.red, Color.green, Color.blue, Color.alpha);
			RwIm3DVertexSetPos(&imVertex[i*6+5], vertPos[0].x, vertPos[0].y, vertPos[0].z);
			RwIm3DVertexSetRGBA(&imVertex[i*6+5], Color.red, Color.green, Color.blue, Color.alpha);

			triangle++;
		}

		RwRenderStateSet(rwRENDERSTATETEXTURERASTER, NULL);
		RwRenderStateSet(rwRENDERSTATEZTESTENABLE, (void *)TRUE);
		RwRenderStateSet(rwRENDERSTATEZWRITEENABLE, (void *)TRUE);

		i = 0;
		numImmVert = numTri * 6;
		LTM = RwFrameGetLTM(RpAtomicGetFrame(atomic));
		while (numImmVert > DISPATCHSIZE)
		{
			if( RwIm3DTransform(&imVertex[i], DISPATCHSIZE, LTM, 0) )
			{
				RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);

				RwIm3DEnd();
			}

			numImmVert -= DISPATCHSIZE;
			i += DISPATCHSIZE;
		}

		if( RwIm3DTransform(&imVertex[i], numImmVert, LTM, 0) )
		{
			RwIm3DRenderPrimitive(rwPRIMTYPELINELIST);

			RwIm3DEnd();
		}

		RwFree(imVertex);
	}

	return atomic;
}