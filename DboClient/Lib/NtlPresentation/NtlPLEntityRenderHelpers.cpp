#include "precomp_ntlpresentation.h"
#include "NtlPLEntityRenderHelpers.h"
#include "NtlAtomic.h"
#include "NtlDebug.h"
#include "NtlPLMaterialExtWrapper.h"
#include "NtlMath.h"
#include <string>

static RpAtomic* SetToonDefaultAllAtomics(RpAtomic *pAtomic, void *data)
{
	SToonData *pData = (SToonData *)data;
	NTL_ASSERTE(pData != NULL);
	NTL_ASSERTE(pData->pToonInk != NULL);
	
	RpToonGeo *g = NULL;
	g = RpToonGeometryGetToonGeo( RpAtomicGetGeometry(pAtomic) );
	if(g != NULL)
	{
		RpToonAtomicEnable(pAtomic);
		RpToonGeoSetSilhouetteInk(g, pData->pToonInk);

		if(pData->pToonPaint)
			RpToonGeoSetPaint(g, pData->pToonPaint);
		
		if( RpSkinGeometryGetSkin( RpAtomicGetGeometry(pAtomic)) )
 			RpSkinAtomicSetType(pAtomic, rpSKINTYPETOON);
		
		RwInt32 uiRenderFlag = RpNtlAtomicGetFlag(pAtomic);

		if(uiRenderFlag & NTL_TOON_NOT_EDGE)
		{
			RpAtomicCallBackRender renderCallback = (RpAtomicCallBackRender)RpNtlToonNotEdgeTwoSideCallBack;
			RpAtomicSetRenderCallBack(pAtomic, renderCallback);
		}
		else if(uiRenderFlag & NTL_TOON_DETAIL_EDGE)
		{
			RpAtomicCallBackRender renderCallback = (RpAtomicCallBackRender)RpNtlToonEdgeTwoSideCallBack;
			RpAtomicSetRenderCallBack(pAtomic, renderCallback);
		}

		if(!(uiRenderFlag & NTL_TOON_NOT_EDGE) && !(uiRenderFlag & NTL_TOON_DETAIL_EDGE))	
		{
			RpAtomicCallBackRender renderCallback = (RpAtomicCallBackRender)RpD3D9ToonFastSilhouetteAtomicRenderCallback;
			RpAtomicSetRenderCallBack(pAtomic, renderCallback);
		}
    }
	return pAtomic;
}

void Helper_SetToonClump(RpClump *pClump, SToonData *pToonData)
{
	NTL_ASSERTE(pClump != NULL);
	RpClumpForAllAtomics(pClump, SetToonDefaultAllAtomics, pToonData);
}

void Helper_SetToonAtomic(RpAtomic *pAtomic, SToonData *pToonData)
{
	NTL_ASSERTE(pAtomic);

	RpToonGeo *g = NULL;
	g = RpToonGeometryGetToonGeo( RpAtomicGetGeometry(pAtomic) );
	if(g != NULL)
	{
		RpToonAtomicEnable(pAtomic);
		RpToonGeoSetSilhouetteInk(g, pToonData->pToonInk);

		if(pToonData->pToonPaint)
			RpToonGeoSetPaint(g, pToonData->pToonPaint);

		if( RpSkinGeometryGetSkin( RpAtomicGetGeometry(pAtomic)) )
 			RpSkinAtomicSetType(pAtomic, rpSKINTYPETOON);

		RwInt32 uiRenderFlag = RpNtlAtomicGetFlag(pAtomic);
	}
}

static RpAtomic *GetCloneAtomicList(RpAtomic *pAtomic, void *pData)
{
	ENTITY_ATOMIC_VEC *pAtomicList = (ENTITY_ATOMIC_VEC *)pData;
	
	int count = RpGeometryGetUserDataArrayCount(RpAtomicGetGeometry(pAtomic));
	RpUserDataArray *pUserData = RpGeometryGetUserDataArray(RpAtomicGetGeometry(pAtomic), 0);
	char *chBuffer = RpUserDataArrayGetString(pUserData, 0);
	
	if(count > 1)
	{
		SEntityAtomicData *pAtomicData = NTL_NEW SEntityAtomicData;
		pAtomicData->pData = RpAtomicClone(pAtomic);
		pAtomicData->bClone = TRUE;

		RwFrame *pFrame = RpAtomicGetFrame(pAtomic);
		if(pFrame)
		{
			pAtomicData->pFrame = RpAtomicGetFrame(pAtomic);
		}
		pAtomicList->push_back(pAtomicData);

	}
	return pAtomic;
}

static RpAtomic *GetAtomicList(RpAtomic *pAtomic, void *pData)
{
	ENTITY_ATOMIC_VEC *pAtomicList = (ENTITY_ATOMIC_VEC *)pData;
	
	int count = RpGeometryGetUserDataArrayCount(RpAtomicGetGeometry(pAtomic));
	RpUserDataArray *pUserData = RpGeometryGetUserDataArray(RpAtomicGetGeometry(pAtomic), 0);
	char *chBuffer = RpUserDataArrayGetString(pUserData, 0);
	
	if(count > 1)
	{
		SEntityAtomicData *pAtomicData = NTL_NEW SEntityAtomicData;
		pAtomicData->pData = pAtomic;

        RwFrame *pFrame = RpAtomicGetFrame(pAtomic);
        if(pFrame)
        {
            pAtomicData->pFrame = RpAtomicGetFrame(pAtomic);
        }

		pAtomicList->push_back(pAtomicData);
	}
	return pAtomic;
}

void Helper_SetClumpAllAtomics(RpClump *pClump, ENTITY_ATOMIC_VEC *pVecAtomicList, bool bClone)
{
	if(bClone)
	{
		RpClumpForAllAtomics(pClump, GetCloneAtomicList, pVecAtomicList);
	}
	else
	{
		RpClumpForAllAtomics(pClump, GetAtomicList, pVecAtomicList);
	}
}


void Helper_DestroyAllAtomics(ENTITY_ATOMIC_VEC *pAtomicList)
{
	for(RwUInt32 i = 0; i < pAtomicList->size(); i++)
	{
		if((*pAtomicList)[i]->bClone)
		{
			RpAtomic *pAtomic = (*pAtomicList)[i]->pData;
			RpAtomicDestroy(pAtomic);  //이상하게 에러가 난다. 음 우선 딴거하고 좀더 작업을 하자
		}

		NTL_DELETE ((*pAtomicList)[i]);
	}

	pAtomicList->clear();
}

static RpAtomic *GetRemoveDestoryAtomic(RpAtomic *pAtomic, void *pData)
{
	RpClump *pClump = RpAtomicGetClump (pAtomic);
	RpClumpRemoveAtomic(pClump, pAtomic);
	RpAtomicDestroy(pAtomic);

	return pAtomic;
}

void Helper_RemoveClumpAllAtomics(RpClump *pClump)
{
	RpClumpForAllAtomics(pClump, GetRemoveDestoryAtomic, NULL);
}

void Helper_SetAtomicAllMaterialSkinInfo(RpAtomic *pAtomic)
{
	RpGeometry *pGeom = RpAtomicGetGeometry(pAtomic);
	if(pGeom)
	{
		RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);

		for(RwInt32 m = 0; m < nMatCount; m++)
		{
			RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, m);
			if(pMaterial)
			{
				RwChar *chMatName = Helper_MaterialName(pMaterial);
                if(!chMatName)
                    continue;

                RwChar chBuffer[64] = {0,};
                if(chMatName)
                {
				    strcpy_s(chBuffer, chMatName);
				    for(RwUInt32 n = 0; n < strlen(chBuffer); n++)
				    {
					    if(chBuffer[n] == '_')
					    {
						    chBuffer[n] = 0;
						    break;
					    }
				    }
                }


				// temporary; for human, namek skin_jacket are different from majin's one
				// DISABLED BY DANEOS
                /*RwBool	MajinCheck	= FALSE;
                if(pMaterial->texture)
                {
				    std::string strTmp(const_cast<RwChar*>(pMaterial->texture->name), 3);
				    if(!strTmp.compare("A_M"))
				    {
					    MajinCheck = TRUE;
				    }
                }*/
				
				if(_stricmp(chBuffer, "skin") == 0)
				{
					if(!_stricmp(chMatName, "skin_jacket") /*&& MajinCheck*/) // BY DANEOS
					{
						// there would be armors here
						NtlMatExtSetFlag(pMaterial, NtlMatExtGetFlag(pMaterial) | rpNTL_MATERIAL_MIXED);
					}

					NtlMatExtSetFlag(pMaterial, NtlMatExtGetFlag(pMaterial) | rpNTL_MATERIAL_SKIN_COLOR);
				}
				else if(_stricmp(chBuffer, "hair") == 0)
					NtlMatExtSetFlag(pMaterial, NtlMatExtGetFlag(pMaterial) | rpNTL_MATERIAL_HEAD_COLOR);
				else if(_stricmp(chMatName, "dogi_belt") == 0)
					NtlMatExtSetFlag(pMaterial, NtlMatExtGetFlag(pMaterial) | rpNTL_MATERIAL_BELT_COLOR);
				else if(_stricmp(chMatName, "dogi_emblem") == 0)
					NtlMatExtSetFlag(pMaterial, NtlMatExtGetFlag(pMaterial) | rpNTL_MATERIAL_EMBLEM);
				else if(_stricmp(chMatName, "dogi_pants") == 0)
					NtlMatExtSetFlag(pMaterial, NtlMatExtGetFlag(pMaterial) | rpNTL_MATERIAL_DOGI_PANTS);
				else if(_stricmp(chMatName, "dogi_skirt") == 0)
					NtlMatExtSetFlag(pMaterial, NtlMatExtGetFlag(pMaterial) | rpNTL_MATERIAL_DOGI_SKIRT);
				else if (_stricmp(chMatName, "Costume_Emblem") == 0)
					NtlMatExtSetFlag(pMaterial, NtlMatExtGetFlag(pMaterial) | rpNTL_MATERIAL_COSTUME_EMBLEM);
				else
				{
					// there would be armors here
					NtlMatExtSetFlag(pMaterial, NtlMatExtGetFlag(pMaterial) | rpNTL_MATERIAL_MIXED);
				}
			}
		}
	}
}

void Helper_SetToonAtomicRender(RpAtomic *pAtomic, SToonData *pToonData)
{
	NTL_ASSERTE(pAtomic);

	RpToonGeo *g = NULL;
	g = RpToonGeometryGetToonGeo( RpAtomicGetGeometry(pAtomic) );
	if(g != NULL)
	{
		RpToonGeoSetSilhouetteInk(g, pToonData->pToonInk);
		if(pToonData->pToonPaint)
			RpToonGeoSetPaint(g, pToonData->pToonPaint);
	}
}

static RpAtomic *SetHierarchyForSkinAtomic(RpAtomic *atomic, void *data)
{
    RpSkinAtomicSetHAnimHierarchy(atomic, (RpHAnimHierarchy *)data);
	return atomic;
}

void Helper_SetHierarchyClumpSkin(RpClump *pClump, RpHAnimHierarchy *pHierarchy)
{
	NTL_ASSERTE(pClump != NULL);
	NTL_ASSERTE(pHierarchy != NULL);
	RpClumpForAllAtomics( pClump, SetHierarchyForSkinAtomic, (void *)pHierarchy);
}

static RwFrame *GetChildFrameHierarchy(RwFrame *frame, void *data)
{    
    RpHAnimHierarchy *hierarchy = *(RpHAnimHierarchy **)data;

	hierarchy = RpHAnimFrameGetHierarchy(frame);
    if( hierarchy == NULL )
    {
        RwFrameForAllChildren(frame, GetChildFrameHierarchy, data);

        return frame;
    }

    *(void **)data = (void *)hierarchy;

    return NULL;
}

RpHAnimHierarchy *Helper_GetHierarchyClump(RpClump *pClump)
{
	NTL_ASSERTE(pClump != NULL);
	RpHAnimHierarchy *pHierarchy = NULL;

	RwFrameForAllChildren( RpClumpGetFrame(pClump), 
		                   GetChildFrameHierarchy, 
						   (void *)&pHierarchy);

	return pHierarchy;
}

RwChar *Helper_BoneName(RwFrame *pFrame)
{
	RpUserDataArray *pUserData = RwFrameGetUserDataArray(pFrame, 0);
	if(pUserData != NULL)
	{
		char *chBuffer = RpUserDataArrayGetString(pUserData, 0);
		if(chBuffer != NULL)
			return chBuffer;
	}
	return NULL;
}

RwChar *Helper_AtomicName(RpAtomic *pAtomic)
{
	int count = RpGeometryGetUserDataArrayCount(RpAtomicGetGeometry(pAtomic));
	RpUserDataArray *pUserData = RpGeometryGetUserDataArray(RpAtomicGetGeometry(pAtomic), 0);
	char *chBuffer = RpUserDataArrayGetString(pUserData, 0);
	if(chBuffer != NULL)
		return chBuffer;

	return NULL;
}

RwChar *Helper_MaterialName(RpMaterial *pMaterial)
{
	RpUserDataArray *pUserData = RpMaterialGetUserDataArray(pMaterial, 0);
    if(!pUserData)
        return NULL;

	char *chBuffer = RpUserDataArrayGetString(pUserData, 0);
	if(chBuffer != NULL)
		return chBuffer;

	return NULL;
}

static RwBool AlphaTestPalettizedTexture(RwRaster *raster)
{
    RwImage *image = NULL;
    RwInt32 width, height, depth;

    /*
     * Create a device-independent image from the texture's 
     * device-dependent raster...
     */

    width = RwRasterGetWidth(raster);
    height = RwRasterGetHeight(raster);
    depth = RwRasterGetDepth(raster);

    image = RwImageCreate(width, height, depth);
    if( image )
    {
        RwRGBA *palette = NULL;
        RwInt32 paletteSize, i;

        RwImageAllocatePixels(image);
        RwImageSetFromRaster(image, raster);

        /*
         * Run through the palette entries looking for an alpha
         * value less than 255...
         */
        palette = RwImageGetPalette(image);
        
        paletteSize = 1 << depth;

        for(i=0; i<paletteSize; i++)
        {
            if( palette->alpha < 255 )
            {
                /*
                 * One of the palette alphas is less than 255 so we
                 * can stop checking the remaining entries...
                 */
                RwImageDestroy(image);

                return TRUE;
            }

            palette++;
        }

        RwImageDestroy(image);   
    }

    return FALSE;
}

static RwBool
AlphaTestTrueColorTexture(RwRaster *raster)
{
    RwImage *image;
    RwInt32 width, height;

    /*
     * Create a device-independent image from the texture's 
     * device-dependent raster...
     */

    width = RwRasterGetWidth(raster);
    height = RwRasterGetHeight(raster);

    image = RwImageCreate(width, height, 32);
    if( image )
    {
        RwInt32 x, y;
        RwUInt8 *imagePixels;
        RwUInt32 *currentPixel;

        RwImageAllocatePixels(image);
        RwImageSetFromRaster(image, raster);
        imagePixels = RwImageGetPixels(image);

        /*
         * Run through the pixel data looking for an alpha
         * value less than 255...
         */
        for(y=0; y<height; y++)
        {
            currentPixel = (RwUInt32 *)imagePixels;

            for(x=0; x<width; x++)
            {
                RwUInt32 pixel;
                RwUInt8 alpha;

                /*
                 * Pixel format for RwImage:
                 * Red: bits 0 - 7
                 * Green: bits 8 - 15
                 * Blue: bits 16 - 23
                 * Alpha: bits 24 - 31
                 */
                pixel = *currentPixel;
                alpha = (RwUInt8)((pixel >> 24) & 0xFF);

                if( alpha < 255 )
                {
                    /*
                     * One of the image's pixel alpha value is less than
                     * 255 so we can stop checking the remaining pixels...
                     */
                    RwImageDestroy(image);

                    return TRUE;
                }
    
                currentPixel++;
            }

            /*
             * Jump to the beginning of the next pixel row...
             */
            imagePixels += RwImageGetStride(image);
        }

        RwImageDestroy(image);   
    }
    
    return FALSE;
}

RwBool	Helper_AtomicTestAlpha(RpAtomic *pAtomic)
{
	NTL_ASSERTE(pAtomic);

	RpGeometry *pGeometry = RpAtomicGetGeometry(pAtomic);
	if( RpGeometryGetFlags(pGeometry) & rpGEOMETRYPRELIT)
	{
		RwRGBA *color;
		RwInt32	i;

		color = RpGeometryGetPreLightColors(pGeometry);

        i = RpGeometryGetNumVertices(pGeometry)-1;

        while( i >= 0 )
        {
            if( color[i].alpha < 255 )
            {
                return TRUE;
            }

            i--;
        }
	}

	RwInt32 nMatNum = RpGeometryGetNumMaterials(pGeometry);

	for(RwInt32 i = 0; i < nMatNum; i++)
	{
		RpMaterial *pMaterial = RpGeometryGetMaterial(pGeometry, i);

		const RwRGBA *pColor = RpMaterialGetColor(pMaterial);

		if(pColor->alpha < 255)
			return TRUE;

		RwTexture *pTexture;
		pTexture = RpMaterialGetTexture(pMaterial);

		RwRaster	*pRaster;
		RwInt32		nRasterFormat;
		
		pRaster			= RwTextureGetRaster(pTexture);
		nRasterFormat	= RwRasterGetFormat(pRaster);

		
		if( (nRasterFormat & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT8888 ||
			(nRasterFormat & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT4444 ||
			(nRasterFormat & rwRASTERFORMATPIXELFORMATMASK) == rwRASTERFORMAT1555 )
		{
			if( (nRasterFormat & rwRASTERFORMATPAL4) ||
				(nRasterFormat & rwRASTERFORMATPAL8) )
			{
				return AlphaTestPalettizedTexture(pRaster);
			}
			else
			{
				return AlphaTestTrueColorTexture(pRaster);
			}
		}
	}

	
	return FALSE;
}

static RwFrame* GetChildFrame(RwFrame *pFrame, void *pData)
{
	FRAME_MAP *pTable = (FRAME_MAP *)pData;
	
	RpUserDataArray *pUserData = RwFrameGetUserDataArray(pFrame, 0);
	if(pUserData != NULL)
	{
		char *chBuffer = RpUserDataArrayGetString(pUserData, 0);
		if(chBuffer != NULL)
		{
			(*pTable)[chBuffer] = pFrame;
		}
		RwFrameForAllChildren(pFrame, GetChildFrame, pData);
	}
	
	return pFrame;
}

static RpAtomic* IsArmorType(RpAtomic *pAtomic, void *pData)
{
	RwBool* pUser = static_cast<RwBool*>(pData);

	if(*pUser)
	{
		return pAtomic;
	}

	RpGeometry* pGeom = RpAtomicGetGeometry(pAtomic);
	if(pGeom)
	{
		RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);

		for(RwInt32 m = 0; m < nMatCount; m++)
		{
			RpMaterial* pMaterial = RpGeometryGetMaterial(pGeom, m);
			if(pMaterial)
			{
				if(NtlMatExtGetFlag(pMaterial) & rpNTL_MATERIAL_MIXED)
				{
					*pUser = TRUE;
				}
			}
		}
	}

	return pAtomic;
}

RwBool Helper_IsArmorType(RpClump *pClump)
{
	RwBool UserFlag = FALSE;
	RpClumpForAllAtomics(pClump, IsArmorType, &UserFlag);

	return UserFlag;
}

RwBool Helper_GetBoneList(RpClump *pClump, FRAME_MAP *pFrame)
{
	RwFrameForAllChildren( RpClumpGetFrame(pClump), GetChildFrame, pFrame);	
	
	if(pFrame->size() == 0)
		return FALSE;

	return TRUE;
}


static RpAtomic *AtomicAddBoundingSphere(RpAtomic *atomic, void *data)
{
	RwSphere* clumpSphere = (RwSphere *)(data);
    RwSphere* atomicSphere = RpAtomicGetBoundingSphere(atomic);
    RwMatrix* atomicLTM = RwFrameGetLTM(RpAtomicGetFrame(atomic));
    
    RwV3d vLength = clumpSphere->center - atomicLTM->pos;
    RwReal fLength = RwV3dLength(&vLength);
    fLength += atomicSphere->radius;

    clumpSphere->radius = max(clumpSphere->radius, fLength);

    return atomic;
}

void Helper_ClumpCalcBoundingSphere(RpClump *pClump, RwSphere *pSphere)
{
    RwMatrix* clumpLTM = RwFrameGetLTM(RpClumpGetFrame(pClump));

    pSphere->center = clumpLTM->pos;
    pSphere->radius = 0.0f;
	RpClumpForAllAtomics(pClump, AtomicAddBoundingSphere, (void *)pSphere);
}


const RwInt32 iMaxVertexCount = 100;
void Helper_ClumpBoundingSphereRender(RpClump *pClump, RwSphere *pSphere)
{
	RwIm3DVertex imVertex[iMaxVertexCount + 1];
    RwV3d center;
    RwInt32 i;
    RwReal radius, constant;
	
	
    RwMatrix *ltm;

    ltm = RwFrameGetLTM(RpClumpGetFrame(pClump));

	RwV3dTransformPoint(&center, &(pSphere->center), ltm);

    radius = pSphere->radius;
    
	constant = 2.0f * rwPI / iMaxVertexCount;

    for(i=0; i<iMaxVertexCount+1; i++)
    {
        RwV3d point;

        point.x = center.x + radius * (RwReal)RwCos(i * constant);
        point.y = center.y + radius * (RwReal)RwSin(i * constant); 
        point.z = center.z;

        RwIm3DVertexSetPos(&imVertex[i], point.x, point.y, point.z);
        RwIm3DVertexSetRGBA(&imVertex[i], 196, 196, 0, 255);
    }

    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *)rwSHADEMODEFLAT);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)NULL);

    if( RwIm3DTransform(imVertex, iMaxVertexCount + 1, NULL, rwIM3D_ALLOPAQUE) )
    {
        RwIm3DRenderPrimitive(rwPRIMTYPEPOLYLINE);

        RwIm3DEnd();
    }

    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *)rwSHADEMODEGOURAUD);

    return;
}


void Helper_ClumpWorldBoundingSphereRender(RwSphere *pSphere, RwUInt8 byR, RwUInt8 byG, RwUInt8 byB, RwUInt8 byA )
{
	RwIm3DVertex imVertex[iMaxVertexCount + 1];
    RwV3d center;
    RwInt32 i;
    RwReal radius, constant;
	
	center.x = pSphere->center.x;
	center.y = pSphere->center.y;
	center.z = pSphere->center.z;

    radius = pSphere->radius;
    
	constant = 2.0f * rwPI / iMaxVertexCount;

    for(i=0; i<iMaxVertexCount+1; i++)
    {
        RwV3d point;

        point.x = center.x + radius * (RwReal)RwCos(i * constant);
        point.y = center.y + radius * (RwReal)RwSin(i * constant); 
        point.z = center.z;

        RwIm3DVertexSetPos(&imVertex[i], point.x, point.y, point.z);
        RwIm3DVertexSetRGBA(&imVertex[i], byR, byG, byB, byA);
    }

    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *)rwSHADEMODEFLAT);
    RwRenderStateSet(rwRENDERSTATETEXTURERASTER, (void *)NULL);

    if( RwIm3DTransform(imVertex, iMaxVertexCount + 1, NULL, rwIM3D_ALLOPAQUE) )
    {
        RwIm3DRenderPrimitive(rwPRIMTYPEPOLYLINE);

        RwIm3DEnd();
    }

    RwRenderStateSet(rwRENDERSTATESHADEMODE, (void *)rwSHADEMODEGOURAUD);

    return;
}

struct sSIMPLE_MATERIAL
{
	RwBool _SimpleMaterial;
};

struct sEMUV_TMP
{
	RwTexture*	_pTexMap;
	RwMatrix*	_pTexMat;
	RwBool		_EMUVWorking;
};

static RpAtomic *ResetClumpAllAtomicsEMUV(RpAtomic *pAtomic, void *data)
{
	RpGeometry *pGeom = RpAtomicGetGeometry(pAtomic);
	if(pGeom)
	{
		RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);

		for(RwInt32 m = 0; m < nMatCount; m++)
		{
			RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, m);
			if(pMaterial)
			{
				RpNtlMaterialExtSetFlag(pMaterial, RpNtlMaterialExtGetFlag(pMaterial) & ~rpNTL_MATERIAL_EMUV);
			}
		}
	}	

	return pAtomic;
}

static RpAtomic *SetClumpAllAtomicsEMUV(RpAtomic *pAtomic, void *data)
{
	RpGeometry *pGeom = RpAtomicGetGeometry(pAtomic);
	if(pGeom)
	{
		RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);

		for(RwInt32 m = 0; m < nMatCount; m++)
		{
			RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, m);
			if(pMaterial)
			{
				sEMUV_TMP* pUser = static_cast<sEMUV_TMP*>(data);

				if(pUser->_EMUVWorking)
				{
					if(NtlMatExtGetFlag(pMaterial) & rpNTL_MATERIAL_MIXED)
					{
						RpNtlMaterialExtSetFlag(pMaterial, RpNtlMaterialExtGetFlag(pMaterial) | rpNTL_MATERIAL_EMUV);
					}
				}
				else
				{
					if(NtlMatExtGetFlag(pMaterial) & rpNTL_MATERIAL_MIXED)
					{
						RpNtlMaterialExtSetFlag(pMaterial, RpNtlMaterialExtGetFlag(pMaterial) & ~rpNTL_MATERIAL_EMUV);
					}
				}

				RpNtlMaterialExtSetUVTexture(pMaterial, pUser->_pTexMap);
				RpNtlMaterialExtSetUVTransform(pMaterial, pUser->_pTexMat);
			}
		}
	}	

	return pAtomic;
}
				 
static RpAtomic *SetClumpAllAtomicsSimpleMaterial(RpAtomic *pAtomic, void *data)
{
	RpGeometry *pGeom = RpAtomicGetGeometry(pAtomic);
	if(pGeom)
	{
		RwInt32 nMatCount = RpGeometryGetNumMaterials(pGeom);

		for(RwInt32 m = 0; m < nMatCount; m++)
		{
			RpMaterial *pMaterial = RpGeometryGetMaterial(pGeom, m);
			if(pMaterial)
			{
				sSIMPLE_MATERIAL* pUser = static_cast<sSIMPLE_MATERIAL*>(data);

				if(pUser->_SimpleMaterial)
				{
					RpNtlMaterialExtSetFlag(pMaterial, RpNtlMaterialExtGetFlag(pMaterial) | rpNTL_MATERIAL_SIMPLEMATERIAL);
				}
				else
				{
					RpNtlMaterialExtSetFlag(pMaterial, RpNtlMaterialExtGetFlag(pMaterial) & ~rpNTL_MATERIAL_SIMPLEMATERIAL);
				}
			}
		}
	}	

	return pAtomic;
}

void Helper_ResetClumpAllAtomicsEMUV(RpClump *pClump)
{
	RpClumpForAllAtomics(pClump, ResetClumpAllAtomicsEMUV, NULL);
}

void Helper_SetClumpAllAtomicsEMUV(RpClump *pClump, RwTexture* _pTexMap, RwMatrix* _pTexMat, RwBool _EMUVWorking)
{
	sEMUV_TMP Param;
	Param._pTexMap		= _pTexMap;
	Param._pTexMat		= _pTexMat;
	Param._EMUVWorking	= _EMUVWorking;

	RpClumpForAllAtomics(pClump, SetClumpAllAtomicsEMUV, &Param);
}

void Helper_SetClumpAllAtomicsSimpleMaterial(RpClump* pClump, RwBool _SimpleMaterial)
{
	sSIMPLE_MATERIAL Param;
	Param._SimpleMaterial = _SimpleMaterial;

	RpClumpForAllAtomics(pClump, SetClumpAllAtomicsSimpleMaterial, &Param);
}

RwBool Helper_IsToonClump( RpClump* pClump ) 
{
    if(!pClump)
        return FALSE;

    ENTITY_ATOMIC_VEC vecAtomic;
    Helper_SetClumpAllAtomics(pClump, &vecAtomic);    
    RpToonGeo *pToonGeo = NULL;
    for each(SEntityAtomicData* pData in vecAtomic)
    {
        if(pData->pData)
        {
            pToonGeo = RpToonGeometryGetToonGeo( RpAtomicGetGeometry(pData->pData));
            if(pToonGeo && RpSkinGeometryGetSkin( RpAtomicGetGeometry(pData->pData)))
                return TRUE;
        }
    }

    return FALSE;
}

RpAtomic* Helper_GetEnvMapAtomic( RpClump* pClump ) 
{
    ENTITY_ATOMIC_VEC vecAtomic;
    Helper_SetClumpAllAtomics(pClump, &vecAtomic);    
    for each(SEntityAtomicData* pData in vecAtomic)
    {
        if(RpNtlAtomicGetFlag(pData->pData) & NTL_ENVMAP_TEX)
            return pData->pData;
    }

    return NULL;
}