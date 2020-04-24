/******************************************************************************************************************************
*
* ntlworldsectorinfo.h
*
* copyright (c) 2005 ntl-inc Ltd.
*
* author : woody1019
*
* purpose : 
*
******************************************************************************************************************************/


#include "precomp_ntlpresentation.h"
#include "ntlworldsectorinfo.h"
#include "ntlworldinterface.h"
#include "NtlWorldMaterialPlugin.h"
#include "ntlplresourcemanager.h"
#include "NtlDebug.h"
#include "NtlWorldSLManger.h"
#include "NtlWorldSector.h"
#include "NtlPLObject.h"
#include "NtlPLWater.h"
#include "NtlMath.h"
#include "NtlPLSceneManager.h"
#include "NtlPLDef.h"
#include "NtlPLVisualManager.h"
#include "NtlWorldShadow.h"
#include "NtlInstanceEffect.h"
#include "NtlWorldPaletteDefine.h"
#include "NtlPLSound.h"
#include "NtlPLEvent.h"
#include "NtlPLEventGenerator.h"
#include "NtlPLSoftSpot.h"
#include "NtlPLShoreLine.h"
#include "NtlPLWorldDecal.h"
#include "NtlPLDecalVolume.h"
#include "NtlPLPlant.h"
#include "NtlPLWorldLight_Base.h"
#include "NtlPLWorldLight_Sphere.h"
#include "NtlPLWorldLight_Box.h"
#include "NtlPLOccluder_Base.h"
#include "NtlPLOccluder_Quad.h"
#include "NtlPLHeatHaze.h"
#include "NtlPLLightObject.h"
#include "NtlPLGameProperty.h"
#include "NtlPLDojo.h"

#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RwWorld.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

CNtlWorldSectorInfo*	CNtlWorldSectorInfo::s_pInstance	= NULL;
RwBool					CNtlWorldSectorInfo::s_bTrueFlag	= TRUE;
RwBool					CNtlWorldSectorInfo::s_bFalseFlag	= FALSE;

CNtlWorldSectorInfo* CNtlWorldSectorInfo::CreateInstance()
{
	if (s_pInstance == NULL)
	{
		s_pInstance = NTL_NEW CNtlWorldSectorInfo;
	}
	return s_pInstance;
}		

void CNtlWorldSectorInfo::DestroyInstance()
{
	NTL_DELETE(s_pInstance);
}

CNtlWorldSectorInfo* CNtlWorldSectorInfo::GetInstance()
{
	return s_pInstance;
}

CNtlWorldSectorInfo::CNtlWorldSectorInfo()
{
}

CNtlWorldSectorInfo::~CNtlWorldSectorInfo()
{
}

#ifdef dNTL_WORLD_TOOL_MODE
RwBool CNtlWorldSectorInfo::NtlWorldSectorGeometryCreate(CNtlWorldSector *pNtlWorldSector)
{
	RpGeometry *pGeometry;
	RpMaterial *pMaterial;
	RwSurfaceProperties SurfProp;
	RpMorphTarget *pMorphTarget;
	RwSphere BoundingSphere;
	RwTexCoords *pTexCoord;
	RwV3d *pVList;
	RpTriangle *pTList;
	RwV2d SPos, EPos;
	RwInt16 CurPtIndices[4];
	RwRGBA *pPrelights;
	RwInt32 m, n;
	DWORD *pDCPalette = pNtlWorldSector->m_pClrDiffusePalette;

	// get sector pointer
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector);

	// create geometry
	pGeometry = RpGeometryCreate(dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum,
		dGET_WORLD_PARAM()->WorldSectorPolyNum,					 
		rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYTEXTURED);

	pPrelights = RpGeometryGetPreLightColors(pGeometry);

	// There's only one morph target, with index 0...
	pMorphTarget = RpGeometryGetMorphTarget(pGeometry, 0);

	// must be added 2 make a rw mesh
	pMaterial = RpMaterialCreate();
	SurfProp.ambient  = 1.0f;
	SurfProp.diffuse  = 1.0f;
	SurfProp.specular = 1.0f;
	RpMaterialSetSurfaceProperties(pMaterial, &SurfProp);

	NtlWorldSectorMaterialExtensionCreate(pNtlSector, pMaterial);

	// create vertices & indices
	pVList = RpMorphTargetGetVertices(pMorphTarget);
	pTList = RpGeometryGetTriangles(pGeometry);
	pTexCoord = RpGeometryGetVertexTexCoords(pGeometry, rwTEXTURECOORDINATEINDEX0);

	SPos.x = pNtlWorldSector->m_pWorldSector->boundingBox.inf.x;
	SPos.y = pNtlWorldSector->m_pWorldSector->boundingBox.inf.z;
	EPos.x = pNtlWorldSector->m_pWorldSector->boundingBox.sup.x;
	EPos.y = pNtlWorldSector->m_pWorldSector->boundingBox.sup.z;

	// open default heightfield by 24bits raw file
	RwChar Result[NTL_MAX_DIR_PATH] = {0,};
	strcpy_s(Result, dGET_WORLD_PARAM()->CurWorkingFolderName);
	strcat_s(Result, NTL_MAX_DIR_PATH, "\\Tool\\SABER\\Texture\\Heightfield\\");
	strcat_s(Result, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldRAW);
	strcat_s(Result, NTL_MAX_DIR_PATH, ".raw");

	FILE* pFileNtlWorldHF;
	::fopen_s(&pFileNtlWorldHF, Result, "rb");

	DBO_ASSERT(pFileNtlWorldHF, "file open failed. (" << Result << ")");

	// 4 lod indexing
	RwInt32 NPosSXBy0 = (RwInt32)(EPos.x + (float)dGET_WORLD_PARAM()->WorldSizeHalf);
	RwInt32 NPosEXBy0 = (RwInt32)(SPos.x + (float)dGET_WORLD_PARAM()->WorldSizeHalf);
	RwInt32 CurRealPt;

	// 고쳐야 한다 순서가 틀리다
	for(int i = (RwInt32)SPos.y; i <= (RwInt32)EPos.y; i += dGET_WORLD_PARAM()->WorldSectorTileSize)
	{
		for(int j = NPosSXBy0; j >= NPosEXBy0; j -= dGET_WORLD_PARAM()->WorldSectorTileSize)
		{
			CurRealPt = (RwInt32)(j - (float)dGET_WORLD_PARAM()->WorldSizeHalf);

			pVList->x = (RwReal)CurRealPt;
			pVList->z = (RwReal)i;

			dGET_WORLD_SLMGR()->SetVert(*pPrelights, pDCPalette, NULL, pVList->x, pVList->z, pFileNtlWorldHF);

			if(pFileNtlWorldHF)
				pVList->y = NtlWorldHFLoadFromFile(pFileNtlWorldHF, pVList->x, pVList->z);
			else
				pVList->y = 0.0f;

			m = (RwInt32)((pVList->z - SPos.y) / dGET_WORLD_PARAM()->WorldSectorTileSize);
			n = (RwInt32)((pVList->x - SPos.x) / dGET_WORLD_PARAM()->WorldSectorTileSize);
			pTexCoord->u = NtlCommonTexCoord[n + m * dGET_WORLD_PARAM()->WorldSectorVertNum].u;
			pTexCoord->v = NtlCommonTexCoord[n + m * dGET_WORLD_PARAM()->WorldSectorVertNum].v;

			pDCPalette++;
			pPrelights++;
			pVList++;
			pTexCoord++;
		}
	}

	if(pFileNtlWorldHF)
		fclose(pFileNtlWorldHF);

	//
	// <3>--<1>
	//  |  / |
	//  | /  |
	// <2>--<0>
	// 

	//
	// <3>--<2>
	//  |  / |
	//  | /  |
	// <0>--<1>
	// 

	for(int i = 0; i < dGET_WORLD_PARAM()->WorldSectorTileNum; ++i)
	{
		for(int j = 0; j < dGET_WORLD_PARAM()->WorldSectorTileNum; ++j)
		{
			CurPtIndices[0] = j + (i * dGET_WORLD_PARAM()->WorldSectorVertNum);
			CurPtIndices[1] = CurPtIndices[0] + 1;
			CurPtIndices[3] = CurPtIndices[0] + dGET_WORLD_PARAM()->WorldSectorVertNum;
			CurPtIndices[2] = CurPtIndices[3] + 1;

			RpGeometryTriangleSetVertexIndices(pGeometry, pTList, CurPtIndices[0], CurPtIndices[2], CurPtIndices[3]);
			RpGeometryTriangleSetMaterial(pGeometry, pTList++, pMaterial);

			RpGeometryTriangleSetVertexIndices(pGeometry, pTList, CurPtIndices[2], CurPtIndices[0], CurPtIndices[1]);
			RpGeometryTriangleSetMaterial(pGeometry, pTList++, pMaterial);
		}
	}

	// Calculate a bounding sphere for the atomic
	RpMorphTargetCalcBoundingSphere(pMorphTarget, &BoundingSphere);
	RpMorphTargetSetBoundingSphere(pMorphTarget, &BoundingSphere);

	RpGeometryUnlock(pGeometry);

	// Create an atomic from this geometry
	pNtlWorldSector->m_pAtomic = RpAtomicCreate();

	// change pipeline
	RpAtomicSetPipeline(pNtlWorldSector->m_pAtomic, D3D9NtlWorldSectorAtomicPipeline);

	// set frame and geometry
	RpAtomicSetFrame(pNtlWorldSector->m_pAtomic, dNTL_WORLD_SECTOR_LOCAL(RpWorldSectorGetWorld(pNtlWorldSector->m_pWorldSector), pParentFrame));
	RpAtomicSetGeometry(pNtlWorldSector->m_pAtomic, pGeometry, 0);

	// Remove all local references to ref-counted objects
	RpGeometryDestroy(pGeometry);
	RpMaterialDestroy(pMaterial);

	return TRUE;
}
#endif

RwBool CNtlWorldSectorInfo::NtlWorldSectorGeometryDelete(CNtlWorldSector *pNtlWorldSector)
{
	NTL_FUNCTION("_RpNtlWorldSectorGeometryDelete");

	// 인도어
	switch (GetSceneManager()->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		{
			sNtlWorldSector*& pNtlSector = dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector);
			if(pNtlSector)
			{
				pNtlWorldSector->MaterialDestructor();
				RwFreeListFree(dNTL_WORLD_GLOBAL(pSectorFreelist), pNtlSector);
				dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector) = NULL;
			}
		}
		break;
	case AW_RWWORLD:
		{
			pNtlWorldSector->MaterialDestructor();
		}
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlWorldSectorInfo::NtlWorldSectorGeometryDeleteWithoutSwapFileSaving(CNtlWorldSector *pNtlWorldSector)
{
	NTL_FUNCTION("_RpNtlWorldSectorGeometryDeleteWithoutSwapFileSaving");

	// 인도어
	switch (GetSceneManager()->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		{
			sNtlWorldSector*& pNtlSector = dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector);
			if(pNtlSector)
			{
				pNtlWorldSector->MaterialDestructor(FALSE);
				RwFreeListFree(dNTL_WORLD_GLOBAL(pSectorFreelist), pNtlSector);
				dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector) = NULL;
			}
		}
		break;
	case AW_RWWORLD:
		{
			pNtlWorldSector->MaterialDestructor(FALSE);
		}
		break;
	}


	NTL_RETURN(TRUE);
}

RwBool CNtlWorldSectorInfo::NtlWorldSectorGeometryLoadFromFile(CNtlWorldSector *pNtlWorldSector, FILE* pFile, sCUR_FIELD_TEX_INFO& CurFieldTexInfo)
{
	return OutdoorSectorLoadFromFile(pFile, pNtlWorldSector, &CurFieldTexInfo);
}

RwBool CNtlWorldSectorInfo::NtlWorldSectorGeometrySaveIntoFile(CNtlWorldSector *pNtlWorldSector, FILE* pFile)
{
	return OutdoorSectorSaveIntoFile(pFile, pNtlWorldSector);
}

RwBool CNtlWorldSectorInfo::NtlWorldSectorGeometryLoadFromFileIndoor(CNtlWorldSector *pNtlWorldSector, FILE* pFile)
{
	return IndoorSectorLoadFromFile(pFile, pNtlWorldSector);
}

RwBool CNtlWorldSectorInfo::NtlWorldSectorGeometrySaveIntoFileIndoor(CNtlWorldSector *pNtlWorldSector, FILE* pFile)
{
	return IndoorSectorSaveIntoFile(pFile, pNtlWorldSector);
}



RwBool CNtlWorldSectorInfo::NtlWorldSectorUpdateTexLyr(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RwInt32 IdxLyr, RwInt32 IdxMenu)
{
	switch(IdxMenu)
	{
	case 0:
		{
			if(!IdxLyr)
			{
				RwTextureDestroy(pNtlWorldSector->m_pTextureBase);
				pNtlWorldSector->m_pTextureBase = NULL;
				pNtlWorldSector->m_pTextureBase = CNtlPLResourceManager::GetInstance()->LoadTexture(CurFieldTexInfo.BLT.StrName, dGET_WORLD_PARAM()->CurDefaultTexPath);
				DBO_ASSERT(pNtlWorldSector->m_pTextureBase, "Texture load failed.");

				NtlWorldSectorUpdateTexMat(CurFieldTexInfo, pNtlWorldSector, -1);
			}
			else
			{
				RwInt32 IdxDTL = IdxLyr - 1;

				if(pNtlWorldSector->m_pTextureSplat[IdxDTL]) RwTextureDestroy(pNtlWorldSector->m_pTextureSplat[IdxDTL]); pNtlWorldSector->m_pTextureSplat[IdxDTL] = NULL;
				if(pNtlWorldSector->m_pTextureAlpha[IdxDTL]) RwTextureDestroy(pNtlWorldSector->m_pTextureAlpha[IdxDTL]); pNtlWorldSector->m_pTextureAlpha[IdxDTL] = NULL;

				pNtlWorldSector->m_pTextureSplat[IdxDTL] = CNtlPLResourceManager::GetInstance()->LoadTexture(CurFieldTexInfo.DLT[IdxDTL].StrName, dGET_WORLD_PARAM()->CurDefaultTexPath);
				DBO_ASSERT(pNtlWorldSector->m_pTextureSplat[IdxDTL], "Texture load failed.");

				NtlWorldSectorUpdateTexMat(CurFieldTexInfo, pNtlWorldSector, IdxDTL);
			}
		}
		break;

	case 1:
		{
			if(!IdxLyr)
			{
				RwTextureDestroy(pNtlWorldSector->m_pTextureBase);
				pNtlWorldSector->m_pTextureBase = NULL;
				pNtlWorldSector->m_pTextureBase = CNtlPLResourceManager::GetInstance()->LoadTexture("NTL", dGET_WORLD_PARAM()->CurDefaultTexPath);
				DBO_ASSERT(pNtlWorldSector->m_pTextureBase, "Texture load failed.");
			}
			else
			{
				RwInt32 IdxDTL = IdxLyr - 1;

				RwTextureDestroy(pNtlWorldSector->m_pTextureSplat[IdxDTL]);
				pNtlWorldSector->m_pTextureSplat[IdxDTL] = NULL;
			}
		}
		break;

	case 2:
		{
			if(!IdxLyr)
			{
				RwTextureDestroy(pNtlWorldSector->m_pTextureBase);
				pNtlWorldSector->m_pTextureBase = NULL;
				pNtlWorldSector->m_pTextureBase = CNtlPLResourceManager::GetInstance()->LoadTexture(CurFieldTexInfo.BLT.StrName, dGET_WORLD_PARAM()->CurDefaultTexPath);
				DBO_ASSERT(pNtlWorldSector->m_pTextureBase, "Texture load failed.");

				NtlWorldSectorUpdateTexMat(CurFieldTexInfo, pNtlWorldSector, -1);
			}
			else
			{
				RwInt32 IdxDTL = IdxLyr - 1;

				if(pNtlWorldSector->m_pTextureSplat[IdxDTL])
				{
					RwTextureDestroy(pNtlWorldSector->m_pTextureSplat[IdxDTL]); pNtlWorldSector->m_pTextureSplat[IdxDTL] = NULL;
				}

				pNtlWorldSector->m_pTextureSplat[IdxDTL] = CNtlPLResourceManager::GetInstance()->LoadTexture(CurFieldTexInfo.DLT[IdxDTL].StrName, dGET_WORLD_PARAM()->CurDefaultTexPath);
				DBO_ASSERT(pNtlWorldSector->m_pTextureSplat[IdxDTL], "Texture load failed.");

				NtlWorldSectorUpdateTexMat(CurFieldTexInfo, pNtlWorldSector, IdxDTL);
			}
		}
		break;
	}

	return TRUE;
}

RwBool CNtlWorldSectorInfo::NtlWorldSectorUpdateTexMat(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RwInt32 IdxLyr)
{
	// Base Layer
	if(IdxLyr == -1)
	{
		float			UVWrap = CurFieldTexInfo.BLT.AngTexAdd;
		D3DXMATRIXA16	Temp;

		D3DXMatrixIdentity(&pNtlWorldSector->m_TextureMatBase[0]);
		pNtlWorldSector->m_TextureMatBase[0]._11 *= UVWrap;
		pNtlWorldSector->m_TextureMatBase[0]._22 *= UVWrap;
		pNtlWorldSector->m_TextureMatBase[1] = pNtlWorldSector->m_TextureMatBase[0];

		float Error = 0.5f * UVWrap;
		D3DXMatrixTranslation(&Temp, -Error, -Error, 0);
		D3DXMatrixMultiply(&pNtlWorldSector->m_TextureMatBase[1], &pNtlWorldSector->m_TextureMatBase[1], &Temp);
		D3DXMatrixRotationZ(&Temp, CurFieldTexInfo.BLT.AngTexMat);
		D3DXMatrixMultiply(&pNtlWorldSector->m_TextureMatBase[1], &pNtlWorldSector->m_TextureMatBase[1], &Temp);
		D3DXMatrixTranslation(&Temp, Error, Error, 0);
		D3DXMatrixMultiply(&pNtlWorldSector->m_TextureMatBase[1], &pNtlWorldSector->m_TextureMatBase[1], &Temp);

		pNtlWorldSector->m_TextureMatBase[1].m[2][0] = pNtlWorldSector->m_TextureMatBase[1].m[3][0];
		pNtlWorldSector->m_TextureMatBase[1].m[2][1] = pNtlWorldSector->m_TextureMatBase[1].m[3][1];
		pNtlWorldSector->m_TextureMatBase[1].m[3][0] = 0.0f;
		pNtlWorldSector->m_TextureMatBase[1].m[3][1] = 0.0f;

		pNtlWorldSector->m_TextureMatAngBase = CurFieldTexInfo.BLT.AngTexMat;
		pNtlWorldSector->m_TextureMatAddBase = CurFieldTexInfo.BLT.AngTexAdd;
	}
	else
	{
		float			UVWrap = CurFieldTexInfo.DLT[IdxLyr].AngTexAdd;
		D3DXMATRIXA16	Temp;

		D3DXMatrixIdentity(&pNtlWorldSector->m_TextureMat[IdxLyr][0]);
		pNtlWorldSector->m_TextureMat[IdxLyr][1] = pNtlWorldSector->m_TextureMat[IdxLyr][0];
		pNtlWorldSector->m_TextureMat[IdxLyr][1]._11 = UVWrap;
		pNtlWorldSector->m_TextureMat[IdxLyr][1]._22 = UVWrap;

		float Error = 0.5f * UVWrap;
		D3DXMatrixTranslation(&Temp, -Error, -Error, 0);
		D3DXMatrixMultiply(&pNtlWorldSector->m_TextureMat[IdxLyr][1], &pNtlWorldSector->m_TextureMat[IdxLyr][1], &Temp);
		D3DXMatrixRotationZ(&Temp, CurFieldTexInfo.DLT[IdxLyr].AngTexMat);
		D3DXMatrixMultiply(&pNtlWorldSector->m_TextureMat[IdxLyr][1], &pNtlWorldSector->m_TextureMat[IdxLyr][1], &Temp);
		D3DXMatrixTranslation(&Temp, Error, Error, 0);
		D3DXMatrixMultiply(&pNtlWorldSector->m_TextureMat[IdxLyr][1], &pNtlWorldSector->m_TextureMat[IdxLyr][1], &Temp);

		pNtlWorldSector->m_TextureMat[IdxLyr][1].m[2][0] = pNtlWorldSector->m_TextureMat[IdxLyr][1].m[3][0];
		pNtlWorldSector->m_TextureMat[IdxLyr][1].m[2][1] = pNtlWorldSector->m_TextureMat[IdxLyr][1].m[3][1];
		pNtlWorldSector->m_TextureMat[IdxLyr][1].m[3][0] = 0.0f;
		pNtlWorldSector->m_TextureMat[IdxLyr][1].m[3][1] = 0.0f;

		pNtlWorldSector->m_TextureMatAng[IdxLyr] = CurFieldTexInfo.DLT[IdxLyr].AngTexMat;
		pNtlWorldSector->m_TextureMatAdd[IdxLyr] = CurFieldTexInfo.DLT[IdxLyr].AngTexAdd;
	}

	return TRUE;
}

void CNtlWorldSectorInfo::NtlWorldSectorMaterialExtensionCreate(sNtlWorldSector *pNtlSector, RpMaterial *pMaterial)
{
	NTL_FUNCTION("_RpNtlWorldSectorMaterialExtensionCreate");

	CNtlWorldSector *pNtlWorldSector = pNtlSector->pNtlWorldSector;

	// apply default textures 4 base layer
	pNtlWorldSector->m_pTextureBase = CNtlPLResourceManager::GetInstance()->LoadTexture("NTL", dGET_WORLD_PARAM()->CurDefaultTexPath);
	DBO_ASSERT(pNtlWorldSector->m_pTextureBase, "Texture load failed.");

	// initialize material plugin extension
	sRpNtlWorldSectorSplat NtlWorlSectorSplat;
	memset(&NtlWorlSectorSplat, 0, sizeof(sRpNtlWorldSectorSplat));
	NtlWorlSectorSplat.pNtlWorldSector = pNtlWorldSector;
	RpNtlWorldSetSectorSplat(pMaterial, &NtlWorlSectorSplat);

	NTL_RETURNVOID();
}

RwReal CNtlWorldSectorInfo::NtlWorldHFLoadFromFile(FILE* pFile, RwReal PosX, RwReal PosY)
{
	NTL_FUNCTION("_RpNtlWorldHFLoadFromFile");

	RwUInt8 Value[3];
	RwReal Result = 0;
	RwInt32 WidthNum, DepthNum;
	RwInt32 NPosX = static_cast<RwInt32>(PosX);
	RwInt32 NPosZ = static_cast<RwInt32>(PosY);
	RwInt32 XBasedOn0 = NPosX + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwInt32 ZBasedOn0 = NPosZ + dGET_WORLD_PARAM()->WorldSizeHalf;
	WidthNum = dGET_WORLD_PARAM()->WorldHeightfieldNum - XBasedOn0 / dGET_WORLD_PARAM()->WorldSectorTileSize;
	DepthNum = dGET_WORLD_PARAM()->WorldHeightfieldNum - ZBasedOn0 / dGET_WORLD_PARAM()->WorldSectorTileSize;

	fseek(pFile, (sizeof(RwUInt8) * 3) * (WidthNum + DepthNum * (dGET_WORLD_PARAM()->WorldHeightfieldNum + 1)), SEEK_SET);
	fread(&Value, sizeof(RwUInt8), 3, pFile);

	NTL_RETURN((float)(Value[0] + Value[1] + Value[2]) / 3.0f  - 128.f);
}

// void CNtlWorldSectorInfo::NtlWorldSectorMaterialExtensionRead(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RpMaterial *pMaterial, FILE* pFile)
// {
// 	NTL_FUNCTION("_RpNtlWorldSectorMaterialExtensionRead");
// 
// 	if(pFile)
// 	{
// 		RwBool BoolFlag;
// 		BYTE *pAlphaValueArray = NTL_NEW BYTE [dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum];
// 
// 		RwChar ReadBuff[rwTEXTUREBASENAMELENGTH] = {0,};
// 		::fread(ReadBuff, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
// 		pNtlWorldSector->m_pTextureBase	= CNtlPLResourceManager::GetInstance()->LoadTexture(ReadBuff, dGET_WORLD_PARAM()->CurDefaultTexPath); 
// 		DBO_ASSERT(pNtlWorldSector->m_pTextureBase, "Texture load failed.");
// 
// #ifdef dNTL_WORLD_COMBINE
// 		/*if (CNtlPLGlobal::m_bCombine)
// 		{
// 		RwInt32 WidthNum, DepthNum;
// 		RwReal XBasedOn0 = pNtlWorldSector->DatumPoint.x + dGET_WORLD_PARAM()->WorldSizeHalf;
// 		RwReal ZBasedOn0 = pNtlWorldSector->DatumPoint.z + dGET_WORLD_PARAM()->WorldSizeHalf;
// 		WidthNum = (int)(XBasedOn0 / (float)dGET_WORLD_PARAM()->WorldSectorSize);
// 		DepthNum = (int)(ZBasedOn0 / (float)dGET_WORLD_PARAM()->WorldSectorSize); 
// 
// 		RwInt32 iFieldIdx = WidthNum + (DepthNum * dGET_WORLD_PARAM()->WorldSectorNum);
// 
// 		RwChar	acCombineTex[NTL_MAX_DIR_PATH];
// 		RwChar	acCombineTexPath[NTL_MAX_DIR_PATH];
// 
// 		sprintf_s(acCombineTexPath, NTL_MAX_DIR_PATH, "%s\\cmap\\", dGET_WORLD_PARAM()->WorldProjectFolderName);
// 		sprintf_s(acCombineTex, NTL_MAX_DIR_PATH, "%.8d.png", iFieldIdx);
// 
// 		pNtlWorldSector->m_pTextureCombine	= CNtlPLResourceManager::GetInstance()->LoadTexture(acCombineTex, acCombineTexPath);
// 		}*/
// #endif
// 
// 		::fread(&pNtlWorldSector->m_TextureMatBase[0], sizeof(D3DXMATRIX), 1, pFile);
// 		::fread(&pNtlWorldSector->m_TextureMatBase[1], sizeof(D3DXMATRIX), 1, pFile);
// 		::fread(&pNtlWorldSector->m_TextureMatAngBase, sizeof(RwReal), 1, pFile);
// 		::fread(&pNtlWorldSector->m_TextureMatAddBase, sizeof(RwUInt8), 1, pFile);
// 
// 
// #ifdef dNTL_WORLD_TOOL_MODE
// 
// 		CurFieldTexInfo.IdxCurLT		= 0;
// 		CurFieldTexInfo.BLT.AngTexMat	= pNtlWorldSector->m_TextureMatAngBase;
// 		CurFieldTexInfo.BLT.AngTexAdd	= pNtlWorldSector->m_TextureMatAddBase;
// 		strcpy_s(CurFieldTexInfo.BLT.StrName, rwTEXTUREBASENAMELENGTH, ReadBuff);
// 
// #endif
// 
// 		for(int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
// 		{
// 			fread(&BoolFlag, sizeof(RwBool), 1, pFile);
// 			if(BoolFlag)
// 			{
// 				fread(ReadBuff, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
// 				pNtlWorldSector->m_pTextureSplat[i] = CNtlPLResourceManager::GetInstance()->LoadTexture(ReadBuff, dGET_WORLD_PARAM()->CurDefaultTexPath); 
// 				DBO_ASSERT(pNtlWorldSector->m_pTextureSplat[i], "Texture load failed.");
// 
// #ifdef dNTL_WORLD_TOOL_MODE
// 
// 				CurFieldTexInfo.IdxCurLT = i + 1;
// 				strcpy_s(CurFieldTexInfo.DLT[i].StrName, ReadBuff);
// 
// #endif
// 
// 				fread(&BoolFlag, sizeof(RwBool), 1, pFile);
// 				if(BoolFlag)
// 				{
// 					fread(ReadBuff, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
// 					pNtlWorldSector->m_pTextureAlpha[i] = \
// 						CNtlPLResourceManager::GetInstance()->CreateTexture(ReadBuff,
// 						dGET_WORLD_PARAM()->WorldSectorTileNum,
// 						dGET_WORLD_PARAM()->WorldSectorTileNum,
// 						dNTL_SPLAT_ALPHA_SIZE,
// 						rwRASTERFORMAT8888);
// 
// 					// Renderware is weird
// 					RwTextureSetAddressing(pNtlWorldSector->m_pTextureAlpha[i], rwTEXTUREADDRESSCLAMP);
// 					RwTextureSetFilterMode(pNtlWorldSector->m_pTextureAlpha[i], rwFILTERLINEAR);
// 
// 					fread(pAlphaValueArray, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum, 1, pFile);
// 					CNtlPLResourceManager::GetInstance()->LoadAlphaBits(pNtlWorldSector->m_pTextureAlpha[i], pAlphaValueArray);
// 				}
// 
// 				fread(&pNtlWorldSector->m_TextureMat[i][0], sizeof(D3DXMATRIX), 1, pFile);
// 				fread(&pNtlWorldSector->m_TextureMat[i][1], sizeof(D3DXMATRIX), 1, pFile);
// 				fread(&pNtlWorldSector->m_TextureMatAng[i], sizeof(RwReal), 1, pFile);
// 				fread(&pNtlWorldSector->m_TextureMatAdd[i], sizeof(RwUInt8), 1, pFile);
// 
// #ifdef dNTL_WORLD_TOOL_MODE
// 
// 				CurFieldTexInfo.DLT[i].AngTexMat	= pNtlWorldSector->m_TextureMatAng[i];
// 				CurFieldTexInfo.DLT[i].AngTexAdd	= pNtlWorldSector->m_TextureMatAdd[i];
// 
// #endif
// 			}
// 		}
// 
// 		NTL_ARRAY_DELETE(pAlphaValueArray);
// 	}
// 
// 	// set material extension info.
// 	sRpNtlWorldSectorSplat NtlWorlSectorSplat;
// 	memset(&NtlWorlSectorSplat, 0, sizeof(sRpNtlWorldSectorSplat));
// 	NtlWorlSectorSplat.pNtlWorldSector = pNtlWorldSector;
// 	RpNtlWorldSetSectorSplat(pMaterial, &NtlWorlSectorSplat);
// 
// 	NTL_RETURNVOID();
// }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwBool CNtlWorldSectorInfo::OutdoorSectorLoadFromFile(FILE* pFile, CNtlWorldSector *pNtlWorldSector, sCUR_FIELD_TEX_INFO* pCurFieldTexInfo)
{
	DBO_ASSERTE(pFile);
	DBO_ASSERTE(pNtlWorldSector);
	DBO_ASSERTE(pCurFieldTexInfo);

	SectorMeshLoadFromFile(pFile, pNtlWorldSector, pCurFieldTexInfo);
	SectorObjectLoadFromFile(pFile, pNtlWorldSector, AW_HEGITHFIELD);
	SectorWaterLoadFromFile(pFile, pNtlWorldSector);
	SectorShadowLoadFromFile(pFile, pNtlWorldSector);
	SectorEffectLoadFromFile(pFile, pNtlWorldSector);
	SectorSoundEffectLoadFromFile(pFile, pNtlWorldSector);
	SectorCullLoadFromFile(pFile, pNtlWorldSector);
	SectorTileTransparencyLoadFromFile(pFile, pNtlWorldSector);
	SectorShoreLineLoadFromFile(pFile, pNtlWorldSector);
	SectorDecalLoadFromFile(pFile, pNtlWorldSector);
	SectorPlantLoadFromFile(pFile, pNtlWorldSector);
	SectorWorldLightLoadFromFile(pFile, pNtlWorldSector);
	SectorOccluderLoadFromFile(pFile, pNtlWorldSector);
	SectorHeatHazeObjectLoadFromFile(pFile, pNtlWorldSector);
	SectorLightObjectLoadFromFile(pFile, pNtlWorldSector);
	SectorDojoLoadFromFile(pFile, pNtlWorldSector);
	
	return TRUE;
}

RwBool CNtlWorldSectorInfo::OutdoorSectorSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	DBO_ASSERTE(pFile);
	DBO_ASSERTE(pNtlWorldSector);

	SectorMeshSaveIntoFile(pFile, pNtlWorldSector);
	SectorObjectSaveIntoFile(pFile, pNtlWorldSector, AW_HEGITHFIELD);
	SectorWaterSaveIntoFile(pFile, pNtlWorldSector);
	SectorShadowSaveIntoFile(pFile, pNtlWorldSector);
	SectorEffectSaveIntoFile(pFile, pNtlWorldSector);
	SectorSoundEffectSaveIntoFile(pFile, pNtlWorldSector);
	SectorCullSaveIntoFile(pFile, pNtlWorldSector);
	SectorTileTransparencySaveIntoFile(pFile, pNtlWorldSector);
	SectorShoreLineSaveIntoFile(pFile, pNtlWorldSector);
	SectorDecalSaveIntoFile(pFile, pNtlWorldSector);
	SectorPlantSaveIntoFile(pFile, pNtlWorldSector);
	SectorWorldLightSaveIntoFile(pFile, pNtlWorldSector);
	SectorOccluderSaveIntoFile(pFile, pNtlWorldSector);
	SectorHeatHazeObjectSaveIntoFile(pFile, pNtlWorldSector);
	SectorLightObjectSaveIntoFile(pFile, pNtlWorldSector);
	SectorDojoSaveIntoFile(pFile, pNtlWorldSector);

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::OutdoorSectorSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	DBO_ASSERTE(pFile);
	DBO_ASSERTE(pFileMem);

	pFileMem = SectorMeshSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorObjectSaveIntoFileFromFileMem(pFile, pFileMem, AW_HEGITHFIELD);
	pFileMem = SectorWaterSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorShadowSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorEffectSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorSoundEffectSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorCullSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorTileTransparencySaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorShoreLineSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorDecalSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorPlantSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorWorldLightSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorOccluderSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorHeatHazeObjectSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorLightObjectSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorDojoSaveIntoFileFromFileMem(pFile, pFileMem);

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::OutdoorSectorSkipToFile(FILE* pFile)
{
	DBO_ASSERTE(pFile);

	SectorMeshSkipToFile(pFile);
	SectorObjectSkipToFile(pFile, AW_HEGITHFIELD);
	SectorWaterSkipToFile(pFile);
	SectorShadowSkipToFile(pFile);
	SectorEffectSkipToFile(pFile);
	SectorSoundEffectSkipToFile(pFile);
	SectorCullSkipToFile(pFile);
	SectorTileTransparencySkipToFile(pFile);
	SectorShoreLineSkipToFile(pFile);
	SectorDecalSkipToFile(pFile);
	SectorPlantSkipToFile(pFile);
	SectorWorldLightSkipToFile(pFile);
	SectorOccluderSkipToFile(pFile);
	SectorHeatHazeObjectSkipToFile(pFile);
	SectorLightObjectSkipToFile(pFile);
	SectorDojoSkipToFile(pFile);

	return ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::OutdoorSectorSkipToFileMem(BYTE* pFileMem)
{
	DBO_ASSERTE(pFileMem);

	pFileMem = SectorMeshSkipToFileMem(pFileMem);
	pFileMem = SectorObjectSkipToFileMem(pFileMem, AW_HEGITHFIELD);
	pFileMem = SectorWaterSkipToFileMem(pFileMem);
	pFileMem = SectorShadowSkipToFileMem(pFileMem);
	pFileMem = SectorEffectSkipToFileMem(pFileMem);
	pFileMem = SectorSoundEffectSkipToFileMem(pFileMem);
	pFileMem = SectorCullSkipToFileMem(pFileMem);
	pFileMem = SectorTileTransparencySkipToFileMem(pFileMem);
	pFileMem = SectorShoreLineSkipToFileMem(pFileMem);
	pFileMem = SectorDecalSkipToFileMem(pFileMem);
	pFileMem = SectorPlantSkipToFileMem(pFileMem);
	pFileMem = SectorWorldLightSkipToFileMem(pFileMem);
	pFileMem = SectorOccluderSkipToFileMem(pFileMem);
	pFileMem = SectorHeatHazeObjectSkipToFileMem(pFileMem);
	pFileMem = SectorLightObjectSkipToFileMem(pFileMem);
	pFileMem = SectorDojoSkipToFileMem(pFileMem);

	return pFileMem;
}

RwBool CNtlWorldSectorInfo::IndoorSectorLoadFromFile(FILE* pFile, CNtlWorldSector *pNtlWorldSector)
{
	DBO_ASSERTE(pFile);
	DBO_ASSERTE(pNtlWorldSector);

	SectorObjectLoadFromFile(pFile, pNtlWorldSector, AW_RWWORLD);
	SectorWaterLoadFromFile(pFile, pNtlWorldSector);
	SectorShadowLoadFromFile(pFile, pNtlWorldSector); // 최적화시 빼야 합니다.
	SectorEffectLoadFromFile(pFile, pNtlWorldSector);
	SectorSoundEffectLoadFromFile(pFile, pNtlWorldSector);
	SectorCullLoadFromFile(pFile, pNtlWorldSector); // 최적화시 빼야 합니다.
	SectorTileTransparencyLoadFromFile(pFile, pNtlWorldSector); // 최적화시 빼야 합니다.
	SectorShoreLineLoadFromFile(pFile, pNtlWorldSector);
	SectorDecalLoadFromFile(pFile, pNtlWorldSector);
	SectorPlantLoadFromFile(pFile, pNtlWorldSector);
	SectorWorldLightLoadFromFile(pFile, pNtlWorldSector);
	SectorOccluderLoadFromFile(pFile, pNtlWorldSector);
	SectorHeatHazeObjectLoadFromFile(pFile, pNtlWorldSector);
	SectorLightObjectLoadFromFile(pFile, pNtlWorldSector);
	SectorDecalVolumeLoadFromFile(pFile, pNtlWorldSector);
	SectorGamePropertyLoadFromFile(pFile, pNtlWorldSector);
	SectorDojoLoadFromFile(pFile, pNtlWorldSector);

	return TRUE;
}

RwBool CNtlWorldSectorInfo::IndoorSectorSaveIntoFile(FILE* pFile, CNtlWorldSector *pNtlWorldSector)
{
	DBO_ASSERTE(pFile);
	DBO_ASSERTE(pNtlWorldSector);

	SectorObjectSaveIntoFile(pFile, pNtlWorldSector, AW_RWWORLD);
	SectorWaterSaveIntoFile(pFile, pNtlWorldSector);
	SectorShadowSaveIntoFile(pFile, pNtlWorldSector); // 최적화시 빼야 합니다.
	SectorEffectSaveIntoFile(pFile, pNtlWorldSector);
	SectorSoundEffectSaveIntoFile(pFile, pNtlWorldSector);
	SectorCullSaveIntoFile(pFile, pNtlWorldSector); // 최적화 시 빼야 합니다.
	SectorTileTransparencySaveIntoFile(pFile, pNtlWorldSector); // 최적화 시 빼야 합니다.
	SectorShoreLineSaveIntoFile(pFile, pNtlWorldSector);
	SectorDecalSaveIntoFile(pFile, pNtlWorldSector);
	SectorPlantSaveIntoFile(pFile, pNtlWorldSector);
	SectorWorldLightSaveIntoFile(pFile, pNtlWorldSector);
	SectorOccluderSaveIntoFile(pFile, pNtlWorldSector);
	SectorHeatHazeObjectSaveIntoFile(pFile, pNtlWorldSector);
	SectorLightObjectSaveIntoFile(pFile, pNtlWorldSector);
	SectorDecalVolumeSaveIntoFile(pFile, pNtlWorldSector);
	SectorGamePropertySaveIntoFile(pFile, pNtlWorldSector);
	SectorDojoSaveIntoFile(pFile, pNtlWorldSector);

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::IndoorSectorSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	DBO_ASSERTE(pFile);
	DBO_ASSERTE(pFileMem);

	pFileMem = SectorObjectSaveIntoFileFromFileMem(pFile, pFileMem, AW_RWWORLD);
	pFileMem = SectorWaterSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorShadowSaveIntoFileFromFileMem(pFile, pFileMem); // 최적화시 빼야 합니다.
	pFileMem = SectorEffectSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorSoundEffectSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorCullSaveIntoFileFromFileMem(pFile, pFileMem); // 최적화 시 빼야 합니다.
	pFileMem = SectorTileTransparencySaveIntoFileFromFileMem(pFile, pFileMem); // 최적화 시 빼야 합니다.
	pFileMem = SectorShoreLineSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorDecalSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorPlantSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorWorldLightSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorOccluderSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorHeatHazeObjectSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorLightObjectSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorDecalVolumeSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorGamePropertySaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorDojoSaveIntoFileFromFileMem(pFile, pFileMem);

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::IndoorSectorSkipToFile(FILE* pFile)
{
	DBO_ASSERTE(pFile);

	SectorObjectSkipToFile(pFile, AW_RWWORLD);
	SectorWaterSkipToFile(pFile);
	SectorShadowSkipToFile(pFile); // 최적화시 빼야 합니다.
	SectorEffectSkipToFile(pFile);
	SectorSoundEffectSkipToFile(pFile);
	SectorCullSkipToFile(pFile); // 최적화 시 빼야 합니다.
	SectorTileTransparencySkipToFile(pFile); // 최적화 시 빼야 합니다.
	SectorShoreLineSkipToFile(pFile);
	SectorDecalSkipToFile(pFile);
	SectorPlantSkipToFile(pFile);
	SectorWorldLightSkipToFile(pFile);
	SectorOccluderSkipToFile(pFile);
	SectorHeatHazeObjectSkipToFile(pFile);
	SectorLightObjectSkipToFile(pFile);
	SectorDecalVolumeSkipToFile(pFile);
	SectorGamePropertySkipToFile(pFile);
	SectorDojoSkipToFile(pFile);

	return ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::IndoorSectorSkipToFileMem(BYTE* pFileMem)
{
	DBO_ASSERTE(pFileMem);

	pFileMem = SectorObjectSkipToFileMem(pFileMem, AW_RWWORLD);
	pFileMem = SectorWaterSkipToFileMem(pFileMem);
	pFileMem = SectorShadowSkipToFileMem(pFileMem); // 최적화시 빼야 합니다.
	pFileMem = SectorEffectSkipToFileMem(pFileMem);
	pFileMem = SectorSoundEffectSkipToFileMem(pFileMem);
	pFileMem = SectorCullSkipToFileMem(pFileMem); // 최적화 시 빼야 합니다.
	pFileMem = SectorTileTransparencySkipToFileMem(pFileMem); // 최적화 시 빼야 합니다.
	pFileMem = SectorShoreLineSkipToFileMem(pFileMem);
	pFileMem = SectorDecalSkipToFileMem(pFileMem);
	pFileMem = SectorPlantSkipToFileMem(pFileMem);
	pFileMem = SectorWorldLightSkipToFileMem(pFileMem);
	pFileMem = SectorOccluderSkipToFileMem(pFileMem);
	pFileMem = SectorHeatHazeObjectSkipToFileMem(pFileMem);
	pFileMem = SectorLightObjectSkipToFileMem(pFileMem);
	pFileMem = SectorDecalVolumeSkipToFileMem(pFileMem);
	pFileMem = SectorGamePropertySkipToFileMem(pFileMem);
	pFileMem = SectorDojoSkipToFileMem(pFileMem);

	return pFileMem;
}

RwBool CNtlWorldSectorInfo::SectorMeshLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector, sCUR_FIELD_TEX_INFO* pCurFieldTexInfo)
{
	RpGeometry*					pGeometry		= NULL;
	RpMaterial*					pMaterial		= NULL;
	RwSurfaceProperties			SurfProp;
	RpMorphTarget*				pMorphTarget	= NULL;
	RwTexCoords*				pTexCoord		= NULL;
	RwV3d*						pVertexList		= NULL;
	RpTriangle*					pTriangleList	= NULL;
	RwRGBA*						pPrelights		= NULL;

	pGeometry			= RpGeometryCreate(dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, dGET_WORLD_PARAM()->WorldSectorPolyNum, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYTEXTURED);
	pMorphTarget		= RpGeometryGetMorphTarget(pGeometry, 0);
	pMaterial			= RpMaterialCreate();

	SurfProp.ambient	= 1.0f;
	SurfProp.diffuse	= 1.0f;
	SurfProp.specular	= 1.0f;
	RpMaterialSetSurfaceProperties(pMaterial, &SurfProp);

	pPrelights			= RpGeometryGetPreLightColors(pGeometry);
	pVertexList			= RpMorphTargetGetVertices(pMorphTarget);
	pTriangleList		= RpGeometryGetTriangles(pGeometry);
	pTexCoord			= RpGeometryGetVertexTexCoords(pGeometry, rwTEXTURECOORDINATEINDEX0);

	SectorMaterialLoadFromFile(pFile, pNtlWorldSector, pMaterial, pCurFieldTexInfo);
	SectorHeightfieldLoadFromFile(pFile, pVertexList);
	SectorPrelightsLoadFromFile(pFile, pPrelights);

	RwV2d vSPos, vEPos;
	vSPos.x = pNtlWorldSector->m_pWorldSector->boundingBox.inf.x;
	vSPos.y = pNtlWorldSector->m_pWorldSector->boundingBox.inf.z;
	vEPos.x = pNtlWorldSector->m_pWorldSector->boundingBox.sup.x;
	vEPos.y = pNtlWorldSector->m_pWorldSector->boundingBox.sup.z;

	// 4 lod indexing
	RwInt32 NPosSXBy0 = (RwInt32)(vEPos.x + (float)dGET_WORLD_PARAM()->WorldSizeHalf);
	RwInt32 NPosEXBy0 = (RwInt32)(vSPos.x + (float)dGET_WORLD_PARAM()->WorldSizeHalf);

	for(int i = (RwInt32)vSPos.y; i <= (RwInt32)vEPos.y; i += dGET_WORLD_PARAM()->WorldSectorTileSize)
	{
		for(int j = NPosSXBy0; j >= NPosEXBy0; j -= dGET_WORLD_PARAM()->WorldSectorTileSize)
		{
			RwInt32 m = (RwInt32)((pVertexList->z - vSPos.y) / dGET_WORLD_PARAM()->WorldSectorTileSize);
			RwInt32 n = (RwInt32)((pVertexList->x - vSPos.x) / dGET_WORLD_PARAM()->WorldSectorTileSize);
			pTexCoord->u = NtlCommonTexCoord[n + m * dGET_WORLD_PARAM()->WorldSectorVertNum].u;
			pTexCoord->v = NtlCommonTexCoord[n + m * dGET_WORLD_PARAM()->WorldSectorVertNum].v;

			pPrelights++;
			pVertexList++;
			pTexCoord++;
		}
	}

	RwInt16 CurPtIndices[4];
	for(int i = 0; i < dGET_WORLD_PARAM()->WorldSectorTileNum; ++i)
	{
		for(int j = 0; j < dGET_WORLD_PARAM()->WorldSectorTileNum; ++j)
		{
			CurPtIndices[0] = j + (i * dGET_WORLD_PARAM()->WorldSectorVertNum);
			CurPtIndices[1] = CurPtIndices[0] + 1;
			CurPtIndices[3] = CurPtIndices[0] + dGET_WORLD_PARAM()->WorldSectorVertNum;
			CurPtIndices[2] = CurPtIndices[3] + 1;

			RpGeometryTriangleSetVertexIndices(pGeometry, pTriangleList, CurPtIndices[0], CurPtIndices[2], CurPtIndices[3]);
			RpGeometryTriangleSetMaterial(pGeometry, pTriangleList++, pMaterial);
			RpGeometryTriangleSetVertexIndices(pGeometry, pTriangleList, CurPtIndices[2], CurPtIndices[0], CurPtIndices[1]);
			RpGeometryTriangleSetMaterial(pGeometry, pTriangleList++, pMaterial);
		}
	}

	RwSphere boundingSphere;
	RpMorphTargetCalcBoundingSphere(pMorphTarget, &boundingSphere);
	RpMorphTargetSetBoundingSphere(pMorphTarget, &boundingSphere);

	RpGeometryUnlock(pGeometry);

	pNtlWorldSector->m_pAtomic = RpAtomicCreate();

	RpAtomicSetPipeline(pNtlWorldSector->m_pAtomic, D3D9NtlWorldSectorAtomicPipeline);
	RpAtomicSetFrame(pNtlWorldSector->m_pAtomic, dNTL_WORLD_SECTOR_LOCAL(RpWorldSectorGetWorld(pNtlWorldSector->m_pWorldSector), pParentFrame));
	RpAtomicSetGeometry(pNtlWorldSector->m_pAtomic, pGeometry, 0);

	RpGeometryDestroy(pGeometry);
	RpMaterialDestroy(pMaterial);

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorMeshSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	SectorMaterialSaveIntoFile(pFile, pNtlWorldSector);
	SectorHeightfieldSaveIntoFile(pFile, pNtlWorldSector);
	SectorPrelightsSaveIntoFile(pFile, pNtlWorldSector);

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorMeshSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	pFileMem = SectorMaterialSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorHeightfieldSaveIntoFileFromFileMem(pFile, pFileMem);
	pFileMem = SectorPrelightsSaveIntoFileFromFileMem(pFile, pFileMem);

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorMeshSkipToFile(FILE* pFile)
{
	SectorMaterialSkipToFile(pFile);
	SectorHeightfieldSkipToFile(pFile);
	SectorPrelightsSkipToFile(pFile);

	return ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorMeshSkipToFileMem(BYTE* pFileMem)
{
	pFileMem = SectorMaterialSkipToFileMem(pFileMem);
	pFileMem = SectorHeightfieldSkipToFileMem(pFileMem);
	pFileMem = SectorPrelightsSkipToFileMem(pFileMem);

	return pFileMem;
}

RwBool CNtlWorldSectorInfo::SectorMaterialLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector, RpMaterial* pMaterial, sCUR_FIELD_TEX_INFO* pCurFieldTexInfo)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		BYTE *pAlphaValueArray = NTL_NEW BYTE [dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum];

		RwChar		ReadBuff[rwTEXTUREBASENAMELENGTH] = { 0, };
		RwUInt32	uiLength = 0;	

		fread(&uiLength, sizeof(RwUInt32), 1, pFile);

		fread(ReadBuff, sizeof(RwChar) * uiLength, 1, pFile);
		ReadBuff[uiLength] = '\0'; // String Indicates the end.

		pNtlWorldSector->m_pTextureBase	= CNtlPLResourceManager::GetInstance()->LoadTexture(ReadBuff, dGET_WORLD_PARAM()->CurDefaultTexPath); 
		DBO_ASSERT(pNtlWorldSector->m_pTextureBase, "Texture load failed.");

		fread(&pNtlWorldSector->m_TextureMatBase[0], sizeof(D3DXMATRIX), 1, pFile);
		fread(&pNtlWorldSector->m_TextureMatBase[1], sizeof(D3DXMATRIX), 1, pFile);
		fread(&pNtlWorldSector->m_TextureMatAngBase, sizeof(RwReal), 1, pFile);
		fread(&pNtlWorldSector->m_TextureMatAddBase, sizeof(RwUInt8), 1, pFile);

#ifdef dNTL_WORLD_TOOL_MODE
		pCurFieldTexInfo->IdxCurLT		= 0;
		pCurFieldTexInfo->BLT.AngTexMat	= pNtlWorldSector->m_TextureMatAngBase;
		pCurFieldTexInfo->BLT.AngTexAdd	= pNtlWorldSector->m_TextureMatAddBase;
		strcpy_s(pCurFieldTexInfo->BLT.StrName, rwTEXTUREBASENAMELENGTH, ReadBuff);
#endif

		RwBool bBool = FALSE;
		for (RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
		{
			fread(&bBool, sizeof(RwBool), 1, pFile);
			if(bBool)
			{
				fread(&uiLength, sizeof(RwUInt32), 1, pFile);
				fread(ReadBuff, sizeof(RwChar) * uiLength, 1, pFile);
				ReadBuff[uiLength] = '\0'; // String 끝을 표시해준다.

				pNtlWorldSector->m_pTextureSplat[i] = CNtlPLResourceManager::GetInstance()->LoadTexture(ReadBuff, dGET_WORLD_PARAM()->CurDefaultTexPath); 
				DBO_ASSERT(pNtlWorldSector->m_pTextureSplat[i], "Texture load failed.");

#ifdef dNTL_WORLD_TOOL_MODE
				pCurFieldTexInfo->IdxCurLT = i + 1;
				strcpy_s(pCurFieldTexInfo->DLT[i].StrName, 32, ReadBuff);
#endif

				fread(&bBool, sizeof(RwBool), 1, pFile);
				if(bBool)
				{
					sprintf_s(ReadBuff, rwTEXTUREBASENAMELENGTH, "NTL");
					pNtlWorldSector->m_pTextureAlpha[i] = CNtlPLResourceManager::GetInstance()->CreateTexture(ReadBuff,
						dGET_WORLD_PARAM()->WorldSectorTileNum, dGET_WORLD_PARAM()->WorldSectorTileNum,	dNTL_SPLAT_ALPHA_SIZE, rwRASTERFORMAT8888);

					// Renderware is weird
					RwTextureSetAddressing(pNtlWorldSector->m_pTextureAlpha[i], rwTEXTUREADDRESSCLAMP);
					RwTextureSetFilterMode(pNtlWorldSector->m_pTextureAlpha[i], rwFILTERLINEAR);

					fread(pAlphaValueArray, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum, 1, pFile);
					CNtlPLResourceManager::GetInstance()->LoadAlphaBits(pNtlWorldSector->m_pTextureAlpha[i], pAlphaValueArray);
				}

				fread(&pNtlWorldSector->m_TextureMat[i][0], sizeof(D3DXMATRIX), 1, pFile);
				fread(&pNtlWorldSector->m_TextureMat[i][1], sizeof(D3DXMATRIX), 1, pFile);
				fread(&pNtlWorldSector->m_TextureMatAng[i], sizeof(RwReal), 1, pFile);
				fread(&pNtlWorldSector->m_TextureMatAdd[i], sizeof(RwUInt8), 1, pFile);

#ifdef dNTL_WORLD_TOOL_MODE
				pCurFieldTexInfo->DLT[i].AngTexMat	= pNtlWorldSector->m_TextureMatAng[i];
				pCurFieldTexInfo->DLT[i].AngTexAdd	= pNtlWorldSector->m_TextureMatAdd[i];
#endif
			}
		}

		NTL_ARRAY_DELETE(pAlphaValueArray);

		// set material extension info.
		sRpNtlWorldSectorSplat NtlWorlSectorSplat;
		memset(&NtlWorlSectorSplat, 0, sizeof(sRpNtlWorldSectorSplat));
		NtlWorlSectorSplat.pNtlWorldSector = pNtlWorldSector;
		RpNtlWorldSetSectorSplat(pMaterial, &NtlWorlSectorSplat);
	}

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorMaterialSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		
		// texture base layer
		uiLength = strlen(pNtlWorldSector->m_pTextureBase->name);
		fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		fwrite(pNtlWorldSector->m_pTextureBase->name, sizeof(RwChar) * uiLength, 1, pFile);
		fwrite(&pNtlWorldSector->m_TextureMatBase[0], sizeof(D3DXMATRIX), 1, pFile);
		fwrite(&pNtlWorldSector->m_TextureMatBase[1], sizeof(D3DXMATRIX), 1, pFile);
		fwrite(&pNtlWorldSector->m_TextureMatAngBase, sizeof(RwReal), 1, pFile);
		fwrite(&pNtlWorldSector->m_TextureMatAddBase, sizeof(RwUInt8), 1, pFile);

		for(int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
		{
			if(pNtlWorldSector->m_pTextureSplat[i])
			{
				fwrite(&s_bTrueFlag, sizeof(RwBool), 1, pFile);		

				uiLength = strlen(pNtlWorldSector->m_pTextureSplat[i]->name);
				fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
				fwrite(pNtlWorldSector->m_pTextureSplat[i]->name, sizeof(RwChar) * uiLength, 1, pFile);

				if(pNtlWorldSector->m_pTextureAlpha[i])
				{
					fwrite(&s_bTrueFlag, sizeof(RwBool), 1, pFile);
					CNtlPLResourceManager::GetInstance()->SaveAlphaBits(pNtlWorldSector->m_pTextureAlpha[i], pFile);
				}
				else
				{
					fwrite(&s_bFalseFlag, sizeof(RwBool), 1, pFile);
				}

				// texture matrix info; [0] 4 alpha map, [1] 4 detail map
				fwrite(&pNtlWorldSector->m_TextureMat[i][0], sizeof(D3DXMATRIX), 1, pFile);
				fwrite(&pNtlWorldSector->m_TextureMat[i][1], sizeof(D3DXMATRIX), 1, pFile);
				fwrite(&pNtlWorldSector->m_TextureMatAng[i], sizeof(RwReal), 1, pFile);
				fwrite(&pNtlWorldSector->m_TextureMatAdd[i], sizeof(RwUInt8), 1, pFile);
			}
			else
			{

				fwrite(&s_bFalseFlag, sizeof(RwBool), 1, pFile);
			}
		}
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorMaterialSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		RwInt32 LenCopy = (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH) + (sizeof(D3DXMATRIX) * 2) + (sizeof(RwReal)) + (sizeof(RwUInt8));

		fwrite(pFileMem, LenCopy, 1, pFile);
		pFileMem += LenCopy;

		// texture detail layer
		for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
		{
			RwBool bBooleanFlag;
			CopyMemory(&bBooleanFlag, pFileMem, sizeof(RwBool));
			fwrite(&bBooleanFlag, sizeof(RwBool), 1, pFile);
			pFileMem += sizeof(RwBool);

			if(bBooleanFlag)
			{
				fwrite(pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
				pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

				CopyMemory(&bBooleanFlag, pFileMem, sizeof(RwBool));
				fwrite(&bBooleanFlag, sizeof(RwBool), 1, pFile);
				pFileMem += sizeof(RwBool);

				if(bBooleanFlag)
				{
					fwrite(pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
					pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

					fwrite(pFileMem, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum, 1, pFile);
					pFileMem += sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum;
				}

				LenCopy = (sizeof(D3DXMATRIX) * 2) + sizeof(RwReal) + sizeof(RwUInt8);
				fwrite(pFileMem, LenCopy, 1, pFile);
				pFileMem += LenCopy;
			}
		}
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));		

		RwInt32 LenCopy = sizeof(RwUInt32) + (sizeof(RwChar) * uiLength) + (sizeof(D3DXMATRIX) * 2) + (sizeof(RwReal)) + (sizeof(RwUInt8));

		fwrite(pFileMem, LenCopy, 1, pFile);
		pFileMem += LenCopy;

		// texture detail layer
		for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
		{
			RwBool bBooleanFlag;
			CopyMemory(&bBooleanFlag, pFileMem, sizeof(RwBool));
			fwrite(&bBooleanFlag, sizeof(RwBool), 1, pFile);
			pFileMem += sizeof(RwBool);

			if(bBooleanFlag)
			{
				CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
				fwrite(pFileMem, sizeof(RwUInt32) + (sizeof(RwChar) * uiLength), 1, pFile);
				pFileMem += sizeof(RwUInt32) + (sizeof(RwChar) * uiLength);

				CopyMemory(&bBooleanFlag, pFileMem, sizeof(RwBool));
				fwrite(&bBooleanFlag, sizeof(RwBool), 1, pFile);
				pFileMem += sizeof(RwBool);

				if(bBooleanFlag)
				{
					fwrite(pFileMem, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum, 1, pFile);
					pFileMem += sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum;
				}

				LenCopy = (sizeof(D3DXMATRIX) * 2) + sizeof(RwReal) + sizeof(RwUInt8);
				fwrite(pFileMem, LenCopy, 1, pFile);
				pFileMem += LenCopy;
			}
		}
	}

	return pFileMem;
}

BYTE* CNtlWorldSectorInfo::SectorMaterialSaveIntoFileFromFileMemSplatTexMaxBlend(FILE* pFile, BYTE* pFileMem, RwInt32 iStartIdx2Del)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		RwInt32 LenCopy = (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH) + (sizeof(D3DXMATRIX) * 2) + (sizeof(RwReal)) + (sizeof(RwUInt8));

		fwrite(pFileMem, LenCopy, 1, pFile);
		pFileMem += LenCopy;

		// temporary variables
		RwBool		SplatTexExists							= FALSE;
		RwChar		SplatTexName[rwTEXTUREBASENAMELENGTH]	= {0,};
		RwBool		SplatATexExists							= FALSE;
		RwChar		SplatATexName[rwTEXTUREBASENAMELENGTH]	= {0,};
		BYTE*		pSplatAlphaTex							= NTL_NEW BYTE [dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum];
		D3DXMATRIX	SplatTexMat[2];
		RwReal		SplatAng;
		RwUInt8		SplatAdd;

		// texture detail layer
		for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
		{
			CopyMemory(&SplatTexExists, pFileMem, sizeof(RwBool));
			pFileMem += sizeof(RwBool);

			if(SplatTexExists)
			{
				CopyMemory(SplatTexName, pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);
				pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

				CopyMemory(&SplatATexExists, pFileMem, sizeof(RwBool));
				pFileMem += sizeof(RwBool);

				if(SplatATexExists)
				{
					CopyMemory(SplatATexName, pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);
					pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

					CopyMemory(pSplatAlphaTex, pFileMem, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);
					pFileMem += sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum;
				}

				CopyMemory(&SplatTexMat[0], pFileMem, sizeof(D3DXMATRIX));
				pFileMem += sizeof(D3DXMATRIX);

				CopyMemory(&SplatTexMat[1], pFileMem, sizeof(D3DXMATRIX));
				pFileMem += sizeof(D3DXMATRIX);

				CopyMemory(&SplatAng, pFileMem, sizeof(RwReal));
				pFileMem += sizeof(RwReal);

				CopyMemory(&SplatAdd, pFileMem, sizeof(RwUInt8));
				pFileMem += sizeof(RwUInt8); 
			}

			if(i <= iStartIdx2Del)
			{
				fwrite(&SplatTexExists, sizeof(RwBool), 1, pFile);
				if(SplatTexExists)
				{
					fwrite(SplatTexName, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);

					fwrite(&s_bFalseFlag, sizeof(RwBool), 1, pFile);

					fwrite(&SplatTexMat[0], sizeof(D3DXMATRIX), 1, pFile);
					fwrite(&SplatTexMat[1], sizeof(D3DXMATRIX), 1, pFile);
					fwrite(&SplatAng, sizeof(RwReal), 1, pFile);
					fwrite(&SplatAdd, sizeof(RwUInt8), 1, pFile);
				}
			}
			else
			{
				fwrite(&SplatTexExists, sizeof(RwBool), 1, pFile);

				if(SplatTexExists)
				{
					fwrite(SplatTexName, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
					fwrite(&SplatATexExists, sizeof(RwBool), 1, pFile);

					if(SplatATexExists)
					{
						fwrite(SplatATexName, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
						fwrite(pSplatAlphaTex, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum, 1, pFile);
					}

					fwrite(&SplatTexMat[0], sizeof(D3DXMATRIX), 1, pFile);
					fwrite(&SplatTexMat[1], sizeof(D3DXMATRIX), 1, pFile);
					fwrite(&SplatAng, sizeof(RwReal), 1, pFile);
					fwrite(&SplatAdd, sizeof(RwUInt8), 1, pFile);
				}
			}
		}

		NTL_ARRAY_DELETE(pSplatAlphaTex);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));		

		RwInt32 LenCopy = sizeof(RwUInt32) + (sizeof(RwChar) * uiLength) + (sizeof(D3DXMATRIX) * 2) + (sizeof(RwReal)) + (sizeof(RwUInt8));

		fwrite(pFileMem, LenCopy, 1, pFile);
		pFileMem += LenCopy;

		// temporary variables
		RwBool		SplatTexExists							= FALSE;
		RwChar		SplatTexName[rwTEXTUREBASENAMELENGTH]	= {0,};
		RwUInt32	SplatTexNameLength						= 0;
		RwBool		SplatATexExists							= FALSE;
		RwChar		SplatATexName[rwTEXTUREBASENAMELENGTH]	= {0,};
		BYTE*		pSplatAlphaTex							= NTL_NEW BYTE [dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum];
		D3DXMATRIX	SplatTexMat[2];
		RwReal		SplatAng;
		RwUInt8		SplatAdd;

		// texture detail layer
		for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
		{
			CopyMemory(&SplatTexExists, pFileMem, sizeof(RwBool));
			pFileMem += sizeof(RwBool);

			if(SplatTexExists)
			{
				CopyMemory(&SplatTexNameLength, pFileMem, sizeof(RwUInt32));
				pFileMem += sizeof(RwUInt32);

				CopyMemory(SplatTexName, pFileMem, sizeof(RwChar) * SplatTexNameLength);
				pFileMem += sizeof(RwChar) * SplatTexNameLength;

				CopyMemory(&SplatATexExists, pFileMem, sizeof(RwBool));
				pFileMem += sizeof(RwBool);

				if(SplatATexExists)
				{					
					CopyMemory(pSplatAlphaTex, pFileMem, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);
					pFileMem += sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum;
				}

				CopyMemory(&SplatTexMat[0], pFileMem, sizeof(D3DXMATRIX));
				pFileMem += sizeof(D3DXMATRIX);

				CopyMemory(&SplatTexMat[1], pFileMem, sizeof(D3DXMATRIX));
				pFileMem += sizeof(D3DXMATRIX);

				CopyMemory(&SplatAng, pFileMem, sizeof(RwReal));
				pFileMem += sizeof(RwReal);

				CopyMemory(&SplatAdd, pFileMem, sizeof(RwUInt8));
				pFileMem += sizeof(RwUInt8); 
			}

			if(i <= iStartIdx2Del)
			{
				fwrite(&SplatTexExists, sizeof(RwBool), 1, pFile);
				if(SplatTexExists)
				{
					
					fwrite(&SplatTexNameLength, sizeof(RwUInt32), 1, pFile);
					fwrite(SplatTexName, sizeof(RwChar) * SplatTexNameLength, 1, pFile);

					fwrite(&s_bFalseFlag, sizeof(RwBool), 1, pFile);

					fwrite(&SplatTexMat[0], sizeof(D3DXMATRIX), 1, pFile);
					fwrite(&SplatTexMat[1], sizeof(D3DXMATRIX), 1, pFile);
					fwrite(&SplatAng, sizeof(RwReal), 1, pFile);
					fwrite(&SplatAdd, sizeof(RwUInt8), 1, pFile);
				}
			}
			else
			{
				fwrite(&SplatTexExists, sizeof(RwBool), 1, pFile);

				if(SplatTexExists)
				{
					fwrite(&SplatTexNameLength, sizeof(RwUInt32), 1, pFile);
					fwrite(SplatTexName, sizeof(RwChar) * SplatTexNameLength, 1, pFile);

					fwrite(&SplatATexExists, sizeof(RwBool), 1, pFile);

					if(SplatATexExists)
					{						
						fwrite(pSplatAlphaTex, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum, 1, pFile);
					}

					fwrite(&SplatTexMat[0], sizeof(D3DXMATRIX), 1, pFile);
					fwrite(&SplatTexMat[1], sizeof(D3DXMATRIX), 1, pFile);
					fwrite(&SplatAng, sizeof(RwReal), 1, pFile);
					fwrite(&SplatAdd, sizeof(RwUInt8), 1, pFile);
				}
			}
		}

		NTL_ARRAY_DELETE(pSplatAlphaTex);
	}

	return pFileMem;
}

BYTE* CNtlWorldSectorInfo::SectorMaterialSaveIntoFileFromFileMemSplatTexMinBlend(FILE* pFile, BYTE* pFileMem, RwInt32 iIndices2Del[dNTL_SPLAT_LAYER_NUM])
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		RwInt32 LenCopy = (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH) + (sizeof(D3DXMATRIX) * 2) + (sizeof(RwReal)) + (sizeof(RwUInt8));

		fwrite(pFileMem, LenCopy, 1, pFile);
		pFileMem += LenCopy;

		// temporary variables
		RwBool		SplatTexExists							= FALSE;
		RwChar		SplatTexName[rwTEXTUREBASENAMELENGTH]	= {0,};
		RwBool		SplatATexExists							= FALSE;
		RwChar		SplatATexName[rwTEXTUREBASENAMELENGTH]	= {0,};
		BYTE*		pSplatAlphaTex							= NTL_NEW BYTE [dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum];
		D3DXMATRIX	SplatTexMat[2];
		RwReal		SplatAng;
		RwUInt8		SplatAdd;

		// texture detail layer
		for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
		{
			CopyMemory(&SplatTexExists, pFileMem, sizeof(RwBool));
			pFileMem += sizeof(RwBool);

			if(SplatTexExists)
			{
				CopyMemory(SplatTexName, pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);
				pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

				CopyMemory(&SplatATexExists, pFileMem, sizeof(RwBool));
				pFileMem += sizeof(RwBool);

				if(SplatATexExists)
				{
					CopyMemory(SplatATexName, pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);
					pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

					CopyMemory(pSplatAlphaTex, pFileMem, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);
					pFileMem += sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum;
				}

				CopyMemory(&SplatTexMat[0], pFileMem, sizeof(D3DXMATRIX));
				pFileMem += sizeof(D3DXMATRIX);

				CopyMemory(&SplatTexMat[1], pFileMem, sizeof(D3DXMATRIX));
				pFileMem += sizeof(D3DXMATRIX);

				CopyMemory(&SplatAng, pFileMem, sizeof(RwReal));
				pFileMem += sizeof(RwReal);

				CopyMemory(&SplatAdd, pFileMem, sizeof(RwUInt8));
				pFileMem += sizeof(RwUInt8); 
			}

			if(iIndices2Del[i])
			{
				fwrite(&SplatTexExists, sizeof(RwBool), 1, pFile);

				if(SplatTexExists)
				{
					fwrite(SplatTexName, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);

					fwrite(&s_bFalseFlag, sizeof(RwBool), 1, pFile);

					fwrite(&SplatTexMat[0], sizeof(D3DXMATRIX), 1, pFile);
					fwrite(&SplatTexMat[1], sizeof(D3DXMATRIX), 1, pFile);
					fwrite(&SplatAng, sizeof(RwReal), 1, pFile);
					fwrite(&SplatAdd, sizeof(RwUInt8), 1, pFile);
				}
			}
			else
			{
				fwrite(&SplatTexExists, sizeof(RwBool), 1, pFile);

				if(SplatTexExists)
				{
					fwrite(SplatTexName, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
					fwrite(&SplatATexExists, sizeof(RwBool), 1, pFile);

					if(SplatATexExists)
					{
						fwrite(SplatATexName, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
						fwrite(pSplatAlphaTex, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum, 1, pFile);
					}

					fwrite(&SplatTexMat[0], sizeof(D3DXMATRIX), 1, pFile);
					fwrite(&SplatTexMat[1], sizeof(D3DXMATRIX), 1, pFile);
					fwrite(&SplatAng, sizeof(RwReal), 1, pFile);
					fwrite(&SplatAdd, sizeof(RwUInt8), 1, pFile);
				}
			}
		}

		NTL_ARRAY_DELETE(pSplatAlphaTex);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));		

		RwInt32 LenCopy = sizeof(RwUInt32) + (sizeof(RwChar) * uiLength) + (sizeof(D3DXMATRIX) * 2) + (sizeof(RwReal)) + (sizeof(RwUInt8));

		fwrite(pFileMem, LenCopy, 1, pFile);
		pFileMem += LenCopy;

		// temporary variables
		RwBool		SplatTexExists							= FALSE;
		RwChar		SplatTexName[rwTEXTUREBASENAMELENGTH]	= {0,};
		RwUInt32	SplatTexNameLength						= 0;
		RwBool		SplatATexExists							= FALSE;
		RwChar		SplatATexName[rwTEXTUREBASENAMELENGTH]	= {0,};
		BYTE*		pSplatAlphaTex							= NTL_NEW BYTE [dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum];
		D3DXMATRIX	SplatTexMat[2];
		RwReal		SplatAng;
		RwUInt8		SplatAdd;

		// texture detail layer
		for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
		{
			CopyMemory(&SplatTexExists, pFileMem, sizeof(RwBool));
			pFileMem += sizeof(RwBool);

			if(SplatTexExists)
			{
				CopyMemory(&SplatTexNameLength, pFileMem, sizeof(RwUInt32));
				pFileMem  += sizeof(RwUInt32);

				CopyMemory(SplatTexName, pFileMem, sizeof(RwChar) * SplatTexNameLength);
				pFileMem += sizeof(RwChar) * SplatTexNameLength;

				CopyMemory(&SplatATexExists, pFileMem, sizeof(RwBool));
				pFileMem += sizeof(RwBool);

				if(SplatATexExists)
				{					
					CopyMemory(pSplatAlphaTex, pFileMem, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);
					pFileMem += sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum;
				}

				CopyMemory(&SplatTexMat[0], pFileMem, sizeof(D3DXMATRIX));
				pFileMem += sizeof(D3DXMATRIX);

				CopyMemory(&SplatTexMat[1], pFileMem, sizeof(D3DXMATRIX));
				pFileMem += sizeof(D3DXMATRIX);

				CopyMemory(&SplatAng, pFileMem, sizeof(RwReal));
				pFileMem += sizeof(RwReal);

				CopyMemory(&SplatAdd, pFileMem, sizeof(RwUInt8));
				pFileMem += sizeof(RwUInt8); 
			}

			if(iIndices2Del[i])
			{
				fwrite(&SplatTexExists, sizeof(RwBool), 1, pFile);

				if(SplatTexExists)
				{
					fwrite(&SplatTexNameLength, sizeof(RwUInt32), 1, pFile);
					fwrite(SplatTexName, sizeof(RwChar) * SplatTexNameLength, 1, pFile);

					fwrite(&s_bFalseFlag, sizeof(RwBool), 1, pFile);

					fwrite(&SplatTexMat[0], sizeof(D3DXMATRIX), 1, pFile);
					fwrite(&SplatTexMat[1], sizeof(D3DXMATRIX), 1, pFile);
					fwrite(&SplatAng, sizeof(RwReal), 1, pFile);
					fwrite(&SplatAdd, sizeof(RwUInt8), 1, pFile);
				}
			}
			else
			{
				fwrite(&SplatTexExists, sizeof(RwBool), 1, pFile);

				if(SplatTexExists)
				{
					fwrite(&SplatTexNameLength, sizeof(RwUInt32), 1, pFile);
					fwrite(SplatTexName, sizeof(RwChar) * SplatTexNameLength, 1, pFile);

					fwrite(&SplatATexExists, sizeof(RwBool), 1, pFile);

					if(SplatATexExists)
					{					
						fwrite(pSplatAlphaTex, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum, 1, pFile);
					}

					fwrite(&SplatTexMat[0], sizeof(D3DXMATRIX), 1, pFile);
					fwrite(&SplatTexMat[1], sizeof(D3DXMATRIX), 1, pFile);
					fwrite(&SplatAng, sizeof(RwReal), 1, pFile);
					fwrite(&SplatAdd, sizeof(RwUInt8), 1, pFile);
				}
			}
		}

		NTL_ARRAY_DELETE(pSplatAlphaTex);
	}

	return pFileMem;
}


RwInt32 CNtlWorldSectorInfo::SectorMaterialSkipToFile(FILE* pFile)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwBool		TmpBool;
		RwUInt32	uiLength = 0;

		::fread(&uiLength, sizeof(RwUInt32), 1, pFile);
		::fseek(pFile, (sizeof(RwChar) * uiLength) + (sizeof(D3DXMATRIX) * 2) +	sizeof(RwReal) + sizeof(RwUInt8), SEEK_CUR);

		for(int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
		{
			::fread(&TmpBool, sizeof(RwBool), 1, pFile);

			if(TmpBool)
			{
				::fread(&uiLength, sizeof(RwUInt32), 1, pFile);
				::fseek(pFile, sizeof(RwChar) * uiLength, SEEK_CUR);
				::fread(&TmpBool, sizeof(RwBool), 1, pFile);

				if(TmpBool)
				{
					::fseek(pFile, (sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum), SEEK_CUR);
				}

				::fseek(pFile, (sizeof(D3DXMATRIX) * 2) + sizeof(RwReal) + sizeof(RwUInt8), SEEK_CUR);
			}
		}
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorMaterialSkipToFileMem(BYTE* pFileMem)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		RwInt32 LenCopy = (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH) + (sizeof(D3DXMATRIX) * 2) + (sizeof(RwReal)) + (sizeof(RwUInt8));

		pFileMem += LenCopy;

		// texture detail layer
		for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
		{
			RwBool bBooleanFlag;
			CopyMemory(&bBooleanFlag, pFileMem, sizeof(RwBool));
			pFileMem += sizeof(RwBool);

			if(bBooleanFlag)
			{
				pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

				CopyMemory(&bBooleanFlag, pFileMem, sizeof(RwBool));
				pFileMem += sizeof(RwBool);

				if(bBooleanFlag)
				{
					pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;
					pFileMem += sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum;
				}

				LenCopy = (sizeof(D3DXMATRIX) * 2) + sizeof(RwReal) + sizeof(RwUInt8);
				pFileMem += LenCopy;
			}
		}
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));		

		RwInt32 LenCopy = sizeof(RwUInt32) + (sizeof(RwChar) * uiLength) + (sizeof(D3DXMATRIX) * 2) + (sizeof(RwReal)) + (sizeof(RwUInt8));

		pFileMem += LenCopy;

		// texture detail layer
		for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
		{
			RwBool bBooleanFlag;
			CopyMemory(&bBooleanFlag, pFileMem, sizeof(RwBool));
			pFileMem += sizeof(RwBool);

			if(bBooleanFlag)
			{
				CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));		
				pFileMem += sizeof(RwUInt32) + (sizeof(RwChar) * uiLength);

				CopyMemory(&bBooleanFlag, pFileMem, sizeof(RwBool));
				pFileMem += sizeof(RwBool);

				if(bBooleanFlag)
				{
					pFileMem += sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum;
				}

				LenCopy = (sizeof(D3DXMATRIX) * 2) + sizeof(RwReal) + sizeof(RwUInt8);
				pFileMem += LenCopy;
			}
		}
	}

	return pFileMem;
}

RwBool CNtlWorldSectorInfo::SectorHeightfieldLoadFromFile(FILE* pFile, RwV3d* pVertexList)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwV3d vBasedPos;
		fread(&vBasedPos,sizeof(RwV3d), 1, pFile);

		for (int z = 0; z < dGET_WORLD_PARAM()->WorldSectorVertNum; ++z)
		{
			for (int x = 0; x < dGET_WORLD_PARAM()->WorldSectorVertNum; ++x)
			{
				pVertexList->x = vBasedPos.x - x * static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
				pVertexList->z = vBasedPos.z + z * static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
				fread(&pVertexList->y,sizeof(RwReal), 1, pFile);
				pVertexList++;
			}
		}
	}

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorHeightfieldSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwV3d* pVertexList = RpMorphTargetGetVertices(pNtlWorldSector->m_pAtomic->geometry->morphTarget);

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
	{
		fwrite(pVertexList, sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, pFile);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
	{
		fwrite(pVertexList, sizeof(RwV3d), 1, pFile);
		for (int z = 0; z < dGET_WORLD_PARAM()->WorldSectorVertNum; ++z)
		{
			for (int x = 0; x < dGET_WORLD_PARAM()->WorldSectorVertNum; ++x)
			{
				fwrite(&pVertexList->y, sizeof(RwReal), 1, pFile);
				pVertexList++;
			}
		}
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorHeightfieldSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		RwInt32 LenHeightfieldFileSize = (sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum);

		fwrite(pFileMem, LenHeightfieldFileSize, 1, pFile);
		pFileMem += LenHeightfieldFileSize;
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwInt32 LenHeightfieldFileSize = sizeof(RwV3d) + (sizeof(RwReal) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum);

		fwrite(pFileMem, LenHeightfieldFileSize, 1, pFile);
		pFileMem += LenHeightfieldFileSize;
	}

	return pFileMem;
}

void CNtlWorldSectorInfo::SectorHeightfieldSaveIntoFileFromFileMemDiff(FILE* pFile, BYTE* pFileMemDst, BYTE* pFileMemSrc)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		RwInt32 NumVert = dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;
		RwV3d	PosDst, PosSrc;
		RwV3d*	pBufVert = NTL_NEW RwV3d [NumVert];

		for(RwInt32 i = 0; i < NumVert; ++i)
		{
			CopyMemory(&PosSrc, pFileMemSrc, sizeof(RwV3d));
			CopyMemory(&PosDst, pFileMemDst, sizeof(RwV3d));
			pFileMemSrc += sizeof(RwV3d);
			pFileMemDst += sizeof(RwV3d);

			PosDst.y = PosSrc.y;
			CopyMemory(&pBufVert[i], &PosDst, sizeof(RwV3d));
		}

		fwrite(pBufVert, sizeof(RwV3d) * NumVert, 1, pFile);

		NTL_ARRAY_DELETE(pBufVert);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		fwrite(pFileMemDst, sizeof(RwV3d), 1, pFile);
		pFileMemDst += sizeof(RwV3d);
		pFileMemSrc += sizeof(RwV3d);

		fwrite(pFileMemSrc, sizeof(RwReal) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, pFile);
	}
}

RwInt32 CNtlWorldSectorInfo::SectorHeightfieldSkipToFile(FILE* pFile)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		::fseek(pFile, sizeof(RwV3d) + (sizeof(RwReal) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum), SEEK_CUR);
	}

	return ::ftell(pFile);
}

RwInt32	CNtlWorldSectorInfo::SectorHeightfieldSkipToFileGetVertex(FILE* pFile, RwInt32 iStartIdx, RwInt32 iCount, RwV3d* pResult)
{
	RwInt32 iPostSkipCnt = (dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum) - (iStartIdx + iCount);

	DBO_ASSERTE(iPostSkipCnt >= 0);
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwV3d vBasedPos;
		::fread(&vBasedPos,sizeof(RwV3d), 1, pFile);
		::fseek(pFile, sizeof(RwReal) * iStartIdx, SEEK_CUR);
		for (int z = iStartIdx / dGET_WORLD_PARAM()->WorldSectorVertNum; z < dGET_WORLD_PARAM()->WorldSectorVertNum; ++z)
		{
			if (iCount <= 0)
			{
				break;
			}
			for (int x = iStartIdx % dGET_WORLD_PARAM()->WorldSectorVertNum; x < dGET_WORLD_PARAM()->WorldSectorVertNum; ++x)
			{
				if (iCount-- <= 0)
				{
					break;
				}

				pResult->x = vBasedPos.x - x * static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
				pResult->z = vBasedPos.z + z * static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
				fread(&pResult->y,sizeof(RwReal), 1, pFile);
				pResult++;		
			}			
		}		
		::fseek(pFile, sizeof(RwReal) * iPostSkipCnt, SEEK_CUR);
	}
	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorHeightfieldSkipToFileMem(BYTE* pFileMem)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		pFileMem += (sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		pFileMem += sizeof(RwV3d) + (sizeof(RwReal) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum);
	}

	return pFileMem;
}

BYTE* CNtlWorldSectorInfo::SectorHeightfieldSkipToFileMemGetVertex(BYTE* pFileMem, RwInt32 iStartIdx, RwInt32 iCount, RwV3d* pResult)
{
	RwInt32 iPostSkipCnt = (dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum) - (iStartIdx + iCount);

	DBO_ASSERTE(iPostSkipCnt >= 0);
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{		
		pFileMem += sizeof(RwV3d) * iStartIdx;
		CopyMemory(pResult, pFileMem, sizeof(RwV3d) * iCount);
		pFileMem += sizeof(RwV3d) * iCount;
		pFileMem += sizeof(RwV3d) * iPostSkipCnt;
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwV3d vBasedPos;
		CopyMemory(&vBasedPos, pFileMem, sizeof(RwV3d));
		pFileMem += sizeof(RwV3d);

		pFileMem += sizeof(RwReal) * iStartIdx;
		for (int z = iStartIdx / dGET_WORLD_PARAM()->WorldSectorVertNum; z < dGET_WORLD_PARAM()->WorldSectorVertNum; ++z)
		{
			if (iCount <= 0)
			{
				break;
			}
			for (int x = iStartIdx % dGET_WORLD_PARAM()->WorldSectorVertNum; x < dGET_WORLD_PARAM()->WorldSectorVertNum; ++x)
			{
				if (iCount-- <= 0)
				{
					break;
				}

				pResult->x = vBasedPos.x - x * static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
				pResult->z = vBasedPos.z + z * static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
				CopyMemory(&pResult->y, pFileMem, sizeof(RwReal));
				pFileMem += sizeof(RwReal);
				pResult++;		
			}

		}		
		pFileMem += sizeof(RwReal) * iPostSkipCnt;
	}
	return pFileMem;
}

RwBool CNtlWorldSectorInfo::SectorPrelightsLoadFromFile(FILE* pFile, RwRGBA* pPrelights)
{
	// load prelight color
	fread(pPrelights, sizeof(RwRGBA) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, pFile);

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorPrelightsSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwRGBA* pPrelights = RpGeometryGetPreLightColors(pNtlWorldSector->m_pAtomic->geometry);
	fwrite(pPrelights, sizeof(RwRGBA) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, pFile);

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorPrelightsSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	RwInt32 NumVert = dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;

	fwrite(pFileMem, sizeof(RwRGBA) * NumVert, 1, pFile);	
	pFileMem += (sizeof(RwRGBA) * NumVert);

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorPrelightsSkipToFile(FILE* pFile)
{
	::fseek(pFile, sizeof(RwRGBA) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, SEEK_CUR);

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorPrelightsSkipToFileMem(BYTE* pFileMem)
{
	RwInt32 NumVert = dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;

	pFileMem += (sizeof(RwRGBA) * NumVert);

	return pFileMem;
}

RwBool CNtlWorldSectorInfo::SectorObjectLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector, EActiveWorldType eActiveWorldType)
{
	DBO_ASSERTE(pNtlWorldSector->m_vecNtlPLEntityOrnament.empty());

	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);
#ifndef dNTL_WORLD_TOOL_MODE
	sNTL_EVENT_TRI_DOODADS NtlEventTriDoodads;
	if(iNum)
	{
		NtlEventTriDoodads.FlgTriDoodads = 0;
		NtlEventTriDoodads.vecDat.reserve(10);
	}
#endif
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLObject* pNtlPLObject = ObjectLoadFromFile(pFile, eActiveWorldType);
		if (pNtlPLObject)
		{
#ifndef dNTL_WORLD_TOOL_MODE
			if ((pNtlPLObject->GetFlags() & NTL_PLEFLAG_TRIGGER) == NTL_PLEFLAG_TRIGGER)
			{
				NtlEventTriDoodads.vecDat.push_back(pNtlPLObject);
			}
#endif

			pNtlWorldSector->m_vecNtlPLEntityOrnament.push_back(pNtlPLObject);
		}
	}

#ifndef dNTL_WORLD_TOOL_MODE
	if(NtlEventTriDoodads.vecDat.size())
	{
		CNtlPLEventGenerator::OnTriDoodadsEvent(&NtlEventTriDoodads);
		NtlEventTriDoodads.vecDat.clear();
	}
#endif

#ifdef dNTL_WORLD_TOOL_MODE
	CNtlPLObject::LoadSwapFile(pNtlWorldSector->DatumPoint.x, pNtlWorldSector->DatumPoint.y, pNtlWorldSector->DatumPoint.z);
#endif

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorObjectSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector, EActiveWorldType eActiveWorldType)
{
	RwInt32 iNum = 0;

	iNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityOrnament.size());
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLObject* pNtlPLObject = static_cast<CNtlPLObject*>(pNtlWorldSector->m_vecNtlPLEntityOrnament.at(i));		

		ObjectSaveIntoFile(pFile, pNtlPLObject, eActiveWorldType);		
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorObjectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwInt32 iIdxSrcField /* = -1 */, RwInt32 iIdxDstField /* = -1 */, CNtlWorldFieldManager* pWorldFieldMgr /* = NULL */)
{
	RwInt32 iNum = 0;

	RwV3d	vOffset;
	if (pWorldFieldMgr)
	{
		vOffset = pWorldFieldMgr->GetFieldRelativeCoord(iIdxSrcField, iIdxDstField);
	}
	else
	{
		vOffset.z = vOffset.x = 0.0f;
	}
	vOffset.y = 0.0f;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	::fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = ObjectSaveIntoFileFromFileMem(pFile, pFileMem, eActiveWorldType, &vOffset);		
	}

	return pFileMem;
}

BYTE* CNtlWorldSectorInfo::SectorObjectSaveIntoFileFromFileMemRevisionPos(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwBBox* pBBox)
{
	RwInt32 iNum = 0;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	::fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = ObjectSaveIntoFileFromFileMemRevisionPos(pFile, pFileMem, eActiveWorldType, pBBox);		
	}

	return pFileMem;
}

BYTE* CNtlWorldSectorInfo::SectorObjectSaveIntoFileFromFileMemPEC(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType)
{
	RwInt32 iNum	= 0;
	RwInt32 iPECCnt = 0;

	BYTE*	pRewind = pFileMem;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		RwBool bResult = FALSE;
		pFileMem = ObjectSkipToFileMemGetEnabledPEC(pFileMem, eActiveWorldType, &bResult);		
		if (bResult)
		{
			++iPECCnt;
		}
	}

	fwrite(&iPECCnt, sizeof(RwInt32), 1, pFile);

	pFileMem = pRewind;
	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);	
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = ObjectSaveIntoFileFromFileMemPEC(pFile, pFileMem, eActiveWorldType);
	}

	return pFileMem;
}

BYTE* CNtlWorldSectorInfo::SectorObjectSaveIntoFileFromFileMemVisibilityDistance(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwReal fVD, RwReal fMinVD, RwReal fMaxVD)
{
	RwInt32 iNum = 0;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	::fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = ObjectSaveIntoFileFromFileMemVisibilityDistance(pFile, pFileMem, eActiveWorldType, fVD, fMinVD, fMaxVD);		
	}

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorObjectSkipToFile(FILE* pFile, EActiveWorldType eActiveWorldType)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);

	for (RwInt32 i = 0; i < iNum; ++i)
	{
		ObjectSkipToFile(pFile, eActiveWorldType);
	}

	return ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorObjectSkipToFileMem(BYTE* pFileMem, EActiveWorldType eActiveWorldType)
{
	RwInt32 iNum = 0;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = ObjectSkipToFileMem(pFileMem, eActiveWorldType);		
	}

	return pFileMem;
}

CNtlPLObject* CNtlWorldSectorInfo::ObjectLoadFromFile(FILE* pFile, EActiveWorldType eActiveWorldType)
{		
	RwChar			acName[128];
	RwV3d			avSRT[3];
	RwUInt32		uiObjectType = EPL_OBJECT_TYPE_OUTDOOR_OBJECT;
	
	{
		RwUInt32 uiLength = 0;

		fread(&uiLength, sizeof(RwUInt32), 1, pFile);
		fread(acName, sizeof(RwChar) * uiLength, 1, pFile);
		acName[uiLength] = '\0';
	}
	
	fread(&avSRT[0], sizeof(RwV3d), 1, pFile);
	fread(&avSRT[1], sizeof(RwV3d), 1, pFile);
	fread(&avSRT[2], sizeof(RwV3d), 1, pFile);
	fread(&uiObjectType, sizeof(RwUInt32), 1, pFile);

	SPLObjectCreateParam sCreateParam;
	sCreateParam.pPos		= &avSRT[2];
	sCreateParam.bLoadMap	= dGET_WORLD_PARAM()->Loading;

	RwBool bLoadScheduling	= CNtlPLResourceManager::GetInstance()->IsLoadScheduling();
	if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
		uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
	{
		sCreateParam.bLoadMap = TRUE;
		CNtlPLResourceManager::GetInstance()->SetLoadScheduling(FALSE);
	}		

	CNtlPLObject* pNtlPLObject = static_cast<CNtlPLObject*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, acName, &sCreateParam));
	DBO_ASSERT(pNtlPLObject, "Entity create failed.");

	CNtlPLResourceManager::GetInstance()->SetLoadScheduling(bLoadScheduling);

	pNtlPLObject->SetScale(&avSRT[0]);
	pNtlPLObject->SetRotate(avSRT[1].x, avSRT[1].y, avSRT[1].z);
	pNtlPLObject->SetPosition(&avSRT[2]);

	pNtlPLObject->SetObjectType(uiObjectType);

	if (pNtlPLObject->GetPlayAnim())
	{
		pNtlPLObject->SetPlayAnimTime(NtlRandomNumber(0.0f, 1.0f));
	}

#ifdef dNTL_WORLD_TOOL_MODE
	// doodads visibility distance culling mode setting
	pNtlPLObject->SetFadeEnable(TRUE);
#endif

	pNtlPLObject->LoadFromFile(pFile, eActiveWorldType);

	return pNtlPLObject;
}

RwBool CNtlWorldSectorInfo::ObjectSaveIntoFile(FILE* pFile, CNtlPLObject* pNtlPLObject, EActiveWorldType eActiveWorldType)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
	{
		RwChar		acName[128];
		RwUInt32	uiLength = 0;

		strcpy_s(acName, 128, const_cast<RwChar*>(pNtlPLObject->GetName()));
		uiLength = strlen(acName);

		fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		fwrite(acName, sizeof(RwChar) * uiLength, 1, pFile);
	}

	RwV3d avSRT[3];
	avSRT[0] = *pNtlPLObject->GetScale();
	avSRT[1] = *pNtlPLObject->GetRotate();
	avSRT[2] = pNtlPLObject->GetPosition();
	fwrite(&avSRT[0], sizeof(RwV3d), 1, pFile);
	fwrite(&avSRT[1], sizeof(RwV3d), 1, pFile);
	fwrite(&avSRT[2], sizeof(RwV3d), 1, pFile);

	RwUInt32 uiObjectType = pNtlPLObject->GetObjectType();
	fwrite(&uiObjectType, sizeof(RwUInt32), 1, pFile);

	return pNtlPLObject->SaveIntoFile(pFile, eActiveWorldType);
}

BYTE* CNtlWorldSectorInfo::ObjectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwV3d* pvOffset)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		// Obj. name
		::fwrite(pFileMem, sizeof(RwChar) * 128, 1, pFile);
		pFileMem += sizeof(RwChar) * 128;
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		// Obj. name
		RwUInt32 uiLength = 0;
		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		::fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);

		::fwrite(pFileMem, (sizeof(RwChar) * uiLength), 1, pFile);
		pFileMem += (sizeof(RwChar) * uiLength);		
	}

	// Scaling, Rotation, Translation
	::fwrite(pFileMem, sizeof(RwV3d), 1, pFile);
	pFileMem += sizeof(RwV3d);

	::fwrite(pFileMem, sizeof(RwV3d), 1, pFile);
	pFileMem += sizeof(RwV3d);

	RwV3d vPos;
	CopyMemory(&vPos, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	if (pvOffset)
	{
		vPos.x += pvOffset->x;
		vPos.y += pvOffset->y;
		vPos.z += pvOffset->z;
	}
	::fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	RwUInt32 uiObjectType;		
	CopyMemory(&uiObjectType, pFileMem, sizeof(RwUInt32));
	::fwrite(&uiObjectType, sizeof(RwUInt32), 1, pFile);
	pFileMem += sizeof(RwUInt32);

	return CNtlPLObject::SaveIntoFileFromFileMem(pFile, pFileMem, eActiveWorldType, uiObjectType);
}

BYTE* CNtlWorldSectorInfo::ObjectSaveIntoFileFromFileMemRevisionPos(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwBBox* pBBox)
{
	RwChar acName[128];
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		// Obj. name
		::fwrite(pFileMem, sizeof(RwChar) * 128, 1, pFile);
		CopyMemory(acName, pFileMem, sizeof(RwChar) * 128);	
		pFileMem += sizeof(RwChar) * 128;
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		// Obj. name
		RwUInt32 uiLength = 0;
				
		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		::fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);

		CopyMemory(acName, pFileMem, sizeof(RwChar) * 128);	
		acName[uiLength] = '\0';

		::fwrite(acName, (sizeof(RwChar) * uiLength), 1, pFile);
		pFileMem += (sizeof(RwChar) * uiLength);		
	}

	// Scaling, Rotation, Translation
	::fwrite(pFileMem, sizeof(RwV3d), 1, pFile);
	pFileMem += sizeof(RwV3d);

	::fwrite(pFileMem, sizeof(RwV3d), 1, pFile);
	pFileMem += sizeof(RwV3d);

	RwV3d vPos;
	CopyMemory(&vPos, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	if (!(vPos.x >= pBBox->inf.x &&
		vPos.z >= pBBox->inf.z &&
		vPos.x < pBBox->sup.x &&
		vPos.z < pBBox->sup.z))
	{
		vPos.x = pBBox->inf.x + (dGET_WORLD_PARAM()->WorldSectorSize / 2);
		vPos.z = pBBox->inf.z + (dGET_WORLD_PARAM()->WorldSectorSize / 2);

		DBO_TRACE((FALSE), "InvalidSectorLink : " << acName << "(" << vPos.x << "," << vPos.y << "," << vPos.z << ")");
	}
	::fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	RwUInt32 uiObjectType;
	CopyMemory(&uiObjectType, pFileMem, sizeof(RwUInt32));
	::fwrite(&uiObjectType, sizeof(RwUInt32), 1, pFile);
	pFileMem += sizeof(RwUInt32);

	return CNtlPLObject::SaveIntoFileFromFileMem(pFile, pFileMem, eActiveWorldType, uiObjectType);
}

BYTE* CNtlWorldSectorInfo::ObjectSaveIntoFileFromFileMemPEC(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType)
{
	RwChar		acName[128] = { 0, };
	RwV3d		avSRT[3];

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		CopyMemory(acName, pFileMem, (sizeof(RwChar) * 128));			
		pFileMem += (sizeof(RwChar) * 128);		
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32	uiLength = 0;

		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));			
		pFileMem += sizeof(RwUInt32);

		CopyMemory(acName, pFileMem, (sizeof(RwChar) * uiLength));
		pFileMem += (sizeof(RwChar) * uiLength);
		acName[uiLength] = '\0';
	}
	
	CopyMemory(&avSRT[0], pFileMem, sizeof(RwV3d));				
	pFileMem += sizeof(RwV3d);

	CopyMemory(&avSRT[1], pFileMem, sizeof(RwV3d));				
	pFileMem += sizeof(RwV3d);

	CopyMemory(&avSRT[2], pFileMem, sizeof(RwV3d));			
	pFileMem += sizeof(RwV3d);

	RwUInt32 uiObjectType;		
	CopyMemory(&uiObjectType, pFileMem, sizeof(RwUInt32));
	pFileMem += sizeof(RwUInt32);

	RwBool bResult = FALSE;
	pFileMem = CNtlPLObject::SkipToFileMemGetEnabledPEC(pFileMem, eActiveWorldType, uiObjectType, &bResult);
	if (bResult)
	{
		fwrite(acName, (sizeof(RwChar) * 128), 1, pFile);
		fwrite(&avSRT[0], sizeof(RwV3d), 1, pFile);
		fwrite(&avSRT[1], sizeof(RwV3d), 1, pFile);
		fwrite(&avSRT[2], sizeof(RwV3d), 1, pFile);
	}

	return pFileMem;
}

BYTE* CNtlWorldSectorInfo::ObjectSaveIntoFileFromFileMemVisibilityDistance(FILE* pFile, BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwReal fVD, RwReal fMinVD, RwReal fMaxVD)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		// Obj. name
		::fwrite(pFileMem, sizeof(RwChar) * 128, 1, pFile);
		pFileMem += sizeof(RwChar) * 128;
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		::fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);

		::fwrite(pFileMem, sizeof(RwChar) * uiLength, 1, pFile);
		pFileMem += sizeof(RwChar) * uiLength;
	}

	// Scaling, Rotation, Translation
	::fwrite(pFileMem, sizeof(RwV3d), 1, pFile);
	pFileMem += sizeof(RwV3d);

	::fwrite(pFileMem, sizeof(RwV3d), 1, pFile);
	pFileMem += sizeof(RwV3d);

	RwV3d vPos;
	CopyMemory(&vPos, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	::fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	RwUInt32 uiObjectType;		
	CopyMemory(&uiObjectType, pFileMem, sizeof(RwUInt32));
	::fwrite(&uiObjectType, sizeof(RwUInt32), 1, pFile);
	pFileMem += sizeof(RwUInt32);

	return CNtlPLObject::SaveIntoFileFromFileMemVisibilityDistance(pFile, pFileMem, eActiveWorldType, uiObjectType, fVD, fMinVD, fMaxVD);
}

RwInt32 CNtlWorldSectorInfo::ObjectSkipToFile(FILE* pFile, EActiveWorldType eActiveWorldType)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		fread(&uiLength, sizeof(RwUInt32), 1, pFile);
		::fseek(pFile, (sizeof(RwChar) * uiLength), SEEK_CUR); // Obj. Name
	}

	::fseek(pFile, (sizeof(RwV3d) * 3), SEEK_CUR); // Scaling, Rotation, Translation

	RwUInt32 uiObjectType;
	::fread(&uiObjectType, sizeof(RwUInt32), 1,pFile);

	return CNtlPLObject::SkipToFile(pFile, eActiveWorldType, uiObjectType);
}

BYTE* CNtlWorldSectorInfo::ObjectSkipToFileMem(BYTE* pFileMem, EActiveWorldType eActiveWorldType)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
	// Obj. name
	pFileMem += sizeof(RwChar) * 128;
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		pFileMem +=sizeof(RwUInt32);

		// Obj. name
		pFileMem += sizeof(RwChar) * uiLength;
	}

	// Scaling, Rotation, Translation
	pFileMem += sizeof(RwV3d) * 3;

	RwUInt32 uiObjectType;		
	CopyMemory(&uiObjectType, pFileMem, sizeof(RwUInt32));
	pFileMem += sizeof(RwUInt32);

	return CNtlPLObject::SkipToFileMem(pFileMem, eActiveWorldType, uiObjectType);
}

BYTE* CNtlWorldSectorInfo::ObjectSkipToFileMemGetEnabledTrigger(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwBool* pResult)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		// Obj. name
		pFileMem += sizeof(RwChar) * 128;
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		pFileMem +=sizeof(RwUInt32);

		// Obj. name
		pFileMem += sizeof(RwChar) * uiLength;
	}

	// Scaling, Rotation, Translation
	pFileMem += sizeof(RwV3d) * 3;

	RwUInt32 uiObjectType;		
	CopyMemory(&uiObjectType, pFileMem, sizeof(RwUInt32));
	pFileMem += sizeof(RwUInt32);

	return CNtlPLObject::SkipToFileMemGetEnabledTrigger(pFileMem, eActiveWorldType, uiObjectType, pResult);
}

BYTE* CNtlWorldSectorInfo::ObjectSkipToFileMemGetEnabledPEC(BYTE* pFileMem, EActiveWorldType eActiveWorldType, RwBool* pResult)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		// Obj. name
		pFileMem += sizeof(RwChar) * 128;
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		pFileMem +=sizeof(RwUInt32);

		// Obj. name
		pFileMem += sizeof(RwChar) * uiLength;
	}

	// Scaling, Rotation, Translation
	pFileMem += sizeof(RwV3d) * 3;

	RwUInt32 uiObjectType;		
	CopyMemory(&uiObjectType, pFileMem, sizeof(RwUInt32));
	pFileMem += sizeof(RwUInt32);

	return CNtlPLObject::SkipToFileMemGetEnabledPEC(pFileMem, eActiveWorldType, uiObjectType, pResult);
}

void CNtlWorldSectorInfo::SectorObjectSaveIntoFileFromFileMemDiff(FILE* pFile, BYTE* pFileMemDst, BYTE* pFileMemSrc, EActiveWorldType eActiveWorldType, RwInt32 iIdxSrcField /* = -1 */, RwInt32 iIdxDstField /* = -1 */, CNtlWorldFieldManager* pWorldFieldMgr /* = NULL */)
{
	RwInt32 iNum		= 0;
	BYTE*	pFileMem	= NULL;

	RwInt32 iDstTriggerCnt		= 0;
	RwInt32 iSrcNonTriggerCnt	= 0;

	RwV3d	vOffset;
	if (pWorldFieldMgr)
	{
		vOffset = pWorldFieldMgr->GetFieldRelativeCoord(iIdxSrcField, iIdxDstField);
	}
	else
	{
		vOffset.z = vOffset.x = 0.0f;
	}
	vOffset.y = 0.0f;

	pFileMem = pFileMemDst;
	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);	
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		RwBool bEnabled = FALSE;
		pFileMem = ObjectSkipToFileMemGetEnabledTrigger(pFileMem, eActiveWorldType, &bEnabled);
		if (bEnabled)
		{
			++iDstTriggerCnt;
		}
	}

	pFileMem = pFileMemSrc;
	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);	
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		RwBool bEnabled = FALSE;
		pFileMem = ObjectSkipToFileMemGetEnabledTrigger(pFileMem, eActiveWorldType, &bEnabled);
		if (!bEnabled)
		{
			++iSrcNonTriggerCnt;
		}
	}

	// Merge 된 후 갯수를 fwrite 한다.
	iNum = iDstTriggerCnt + iSrcNonTriggerCnt;
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);

	// dest trigger save
	pFileMem = pFileMemDst;
	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);	
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		RwBool	bEnabled;
		BYTE*	pRewind = pFileMem;

		pFileMem = ObjectSkipToFileMemGetEnabledTrigger(pFileMem, eActiveWorldType, &bEnabled);
		if (bEnabled)
		{
			// Trigger 라면 저장한다. pFileMem은 이미 file pos가 Skip 된 상태이므로 rewind pos로 해결하자.			
			pFileMem = ObjectSaveIntoFileFromFileMem(pFile, pRewind, eActiveWorldType); 
		}
	}

	pFileMem = pFileMemSrc;
	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);	
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		RwBool	bEnabled;
		BYTE*	pRewind = pFileMem;

		pFileMem = ObjectSkipToFileMemGetEnabledTrigger(pFileMem, eActiveWorldType, &bEnabled);
		if (!bEnabled)
		{
			// Trigger 아니라면 저장한다. pFileMem은 이미 file pos가 Skip 된 상태이므로 rewind pos로 해결하자.
			// FieldIdx가 다를 경우 vOffset으로 Object의 위치를 다르게 저장해야 한다.
			pFileMem = ObjectSaveIntoFileFromFileMem(pFile, pRewind, eActiveWorldType, &vOffset);
		}
	}
}

RwBool CNtlWorldSectorInfo::SectorWaterLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwBool bBooleanFlag;
	fread(&bBooleanFlag, sizeof(RwBool), 1, pFile);

	if(bBooleanFlag)
	{
		sSECTOR_WATER_ATTR SectorWaterAttr;
		fread(&SectorWaterAttr._Height,							sizeof(RwReal), 1,	pFile);
		fread(&SectorWaterAttr._RGBA,							sizeof(RwRGBA), 1,	pFile);
		fread(&SectorWaterAttr._RenderstateSrc,					sizeof(DWORD),	1,	pFile);
		fread(&SectorWaterAttr._RenderstateDst,					sizeof(DWORD),	1,	pFile);
		fread(&SectorWaterAttr._RotTrans,						sizeof(RwReal),	1,	pFile);
		fread(&SectorWaterAttr._SclTrans,						sizeof(RwReal),	1,	pFile);
		fread(&SectorWaterAttr._DepthMapSize,					sizeof(RwInt32),1,	pFile);
		fread(&SectorWaterAttr._DepthMapHeightVariation[0],		sizeof(RwReal),	1,	pFile);
		fread(&SectorWaterAttr._DepthMapHeightVariation[1],		sizeof(RwReal),	1,	pFile);
		fread(&SectorWaterAttr._IdxSequence,					sizeof(RwInt32),1,	pFile);

		CNtlPLVisualManager* pSceneManager = static_cast<CNtlPLVisualManager*>(GetSceneManager());
		pSceneManager->GetWater()->OnCreate(pNtlWorldSector, SectorWaterAttr);

		::fread(&bBooleanFlag, sizeof(RwBool), 1, pFile);
		if(bBooleanFlag)
		{
			pNtlWorldSector->m_pWater->_pDepthMap = CNtlPLResourceManager::GetInstance()->CreateTexture("DepthMap",
				SectorWaterAttr._DepthMapSize, SectorWaterAttr._DepthMapSize, 32, rwRASTERFORMAT8888);

			RwTextureSetAddressing(pNtlWorldSector->m_pWater->_pDepthMap, rwTEXTUREADDRESSCLAMP);
			RwTextureSetFilterMode(pNtlWorldSector->m_pWater->_pDepthMap, rwFILTERLINEAR);

			BYTE* pTmp = NTL_NEW BYTE [SectorWaterAttr._DepthMapSize * SectorWaterAttr._DepthMapSize];
			::fread(pTmp, sizeof(BYTE) * SectorWaterAttr._DepthMapSize * SectorWaterAttr._DepthMapSize, 1, pFile);
			CNtlPLResourceManager::GetInstance()->LoadAlphaBits(pNtlWorldSector->m_pWater->_pDepthMap, pTmp);
			NTL_ARRAY_DELETE(pTmp);
		}
	}

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorWaterSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	sSECTOR_WATER_ATTR* pSectorWaterAttr = pNtlWorldSector->m_pWater;

	if(pSectorWaterAttr)
	{
		fwrite(&s_bTrueFlag,									sizeof(RwBool),		1, pFile);
		fwrite(&pSectorWaterAttr->_Height,						sizeof(RwReal),		1, pFile);
		fwrite(&pSectorWaterAttr->_RGBA,						sizeof(RwRGBA),		1, pFile);
		fwrite(&pSectorWaterAttr->_RenderstateSrc,				sizeof(DWORD),		1, pFile);
		fwrite(&pSectorWaterAttr->_RenderstateDst,				sizeof(DWORD),		1, pFile);
		fwrite(&pSectorWaterAttr->_RotTrans,					sizeof(RwReal),		1, pFile);
		fwrite(&pSectorWaterAttr->_SclTrans,					sizeof(RwReal),		1, pFile);
		fwrite(&pSectorWaterAttr->_DepthMapSize,				sizeof(RwInt32),	1, pFile);
		fwrite(&pSectorWaterAttr->_DepthMapHeightVariation[0],	sizeof(RwReal),		1, pFile);
		fwrite(&pSectorWaterAttr->_DepthMapHeightVariation[1],	sizeof(RwReal),		1, pFile);
		fwrite(&pSectorWaterAttr->_IdxSequence,					sizeof(RwInt32),	1, pFile);

		if(pSectorWaterAttr->_pDepthMap)
		{
			fwrite(&s_bTrueFlag, sizeof(RwBool), 1, pFile);
			CNtlPLResourceManager::GetInstance()->SaveAlphaBits(pSectorWaterAttr->_pDepthMap, pFile);
		}
		else
		{
			fwrite(&s_bFalseFlag, sizeof(RwBool), 1, pFile);
		}
	}
	else
	{
		fwrite(&s_bFalseFlag, sizeof(RwBool), 1, pFile);
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorWaterSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	// water existence
	RwInt32 bBooleanFlag;
	CopyMemory(&bBooleanFlag, pFileMem, sizeof(RwBool));
	::fwrite(&bBooleanFlag, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);

	if (bBooleanFlag)
	{
		RwInt32 SizMemBlock =	sizeof(RwReal) +		// height
			sizeof(RwRGBA) +		// RGBA
			(sizeof(DWORD) * 2) +	// renderstate source, dst
			(sizeof(RwReal) * 2);	// rotate, scale

		::fwrite(pFileMem, SizMemBlock, 1, pFile);
		pFileMem += SizMemBlock;

		// depth map size
		RwInt32 iSize = 0;
		CopyMemory(&iSize, pFileMem, sizeof(RwInt32));
		::fwrite(&iSize, sizeof(RwInt32), 1, pFile);
		pFileMem += sizeof(RwInt32);

		// depth map height variation, texture squence index
		::fwrite(pFileMem, (sizeof(RwReal) * 2) + sizeof(RwInt32), 1, pFile);
		pFileMem += (sizeof(RwReal) * 2) + sizeof(RwInt32);

		// depth map existence
		CopyMemory(&bBooleanFlag, pFileMem, sizeof(RwBool));
		::fwrite(&bBooleanFlag, sizeof(RwBool), 1, pFile);
		pFileMem += sizeof(RwBool);

		if(bBooleanFlag)
		{
			// depth map alpha bits
			::fwrite(pFileMem, (sizeof(BYTE) * iSize * iSize), 1, pFile);
			pFileMem += (sizeof(BYTE) * iSize * iSize);
		}
	}

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorWaterSkipToFile(FILE* pFile)
{
	RwBool BooleanFlag;
	::fread(&BooleanFlag, sizeof(RwBool), 1, pFile);

	if(BooleanFlag)
	{
		::fseek(pFile,	sizeof(RwReal) + sizeof(RwRGBA) + (sizeof(DWORD) * 2) + (sizeof(RwReal) * 2), SEEK_CUR);

		RwInt32 Size;
		::fread(&Size, sizeof(RwInt32), 1, pFile);
		::fseek(pFile, (sizeof(RwReal) * 2) + sizeof(RwInt32), SEEK_CUR);

		::fread(&BooleanFlag, sizeof(RwBool), 1, pFile);
		if(BooleanFlag)
		{
			::fseek(pFile, sizeof(BYTE) * Size * Size, SEEK_CUR);
		}
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorWaterSkipToFileMem(BYTE* pFileMem)
{
	// water existence
	RwInt32 bBooleanFlag;
	CopyMemory(&bBooleanFlag, pFileMem, sizeof(RwBool));
	pFileMem += sizeof(RwBool);

	if (bBooleanFlag)
	{
		RwInt32 SizMemBlock =	sizeof(RwReal) +		// height
			sizeof(RwRGBA) +		// RGBA
			(sizeof(DWORD) * 2) +	// renderstate source, dst
			(sizeof(RwReal) * 2);	// rotate, scale
		pFileMem += SizMemBlock;

		// depth map size
		RwInt32 iSize = 0;
		CopyMemory(&iSize, pFileMem, sizeof(RwInt32));
		pFileMem += sizeof(RwInt32);

		// depth map height variation, texture squence index
		pFileMem += (sizeof(RwReal) * 2) + sizeof(RwInt32);

		// depth map existence
		CopyMemory(&bBooleanFlag, pFileMem, sizeof(RwBool));
		pFileMem += sizeof(RwBool);

		if(bBooleanFlag)
		{
			// depth map alpha bits
			pFileMem += (sizeof(BYTE) * iSize * iSize);
		}
	}

	return pFileMem;
}

RwBool CNtlWorldSectorInfo::SectorShadowLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwBool bBooleanFlag;
	fread(&bBooleanFlag, sizeof(RwBool), 1, pFile);

	if(bBooleanFlag)
	{
		CNtlWorldShadow* pNtlWorldShadow = pNtlWorldSector->m_pNtlWorldShadow;

		fread(&pNtlWorldShadow->m_rgbaShadow, sizeof(RwRGBA), 1, pFile);
		fread(&pNtlWorldShadow->m_CamViewWindowX, sizeof(RwReal), 1, pFile);
		fread(&pNtlWorldShadow->m_matLight, sizeof(RwMatrix), 1, pFile);

		RwInt32 iSize;
		fread(&iSize, sizeof(RwInt32), 1, pFile);		

		pNtlWorldShadow->m_pTexShadow = CNtlPLResourceManager::GetInstance()->CreateTexture("ShadowMap", iSize, iSize, 32, rwRASTERFORMAT8888);
		DBO_ASSERT(pNtlWorldShadow->m_pTexShadow, "Texture load failed.");

		RwTextureSetFilterMode(pNtlWorldShadow->m_pTexShadow, rwFILTERLINEAR);
		RwTextureSetAddressing(pNtlWorldShadow->m_pTexShadow, rwTEXTUREADDRESSCLAMP);

		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION_OLD))
		{
			// 내부에서 8888 포맷과 888 포맷에 따라 Load가 다르다.
			CNtlPLResourceManager::GetInstance()->LoadTexture(pNtlWorldShadow->m_pTexShadow, pFile);		
		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
		{
			// 내부에서 8888 포맷과 888 포맷에 따라 Load가 다르다.
			CNtlPLResourceManager::GetInstance()->LoadTextureShadowMap(pNtlWorldShadow->m_pTexShadow, pFile);
		}		
	}	

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorShadowSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	CNtlWorldShadow* pNtlWorldShadow = pNtlWorldSector->m_pNtlWorldShadow;

	if(pNtlWorldShadow->m_pTexShadow)
	{
		::fwrite(&s_bTrueFlag, sizeof(RwBool), 1, pFile);
		::fwrite(&pNtlWorldShadow->m_rgbaShadow, sizeof(RwRGBA), 1, pFile);
		::fwrite(&pNtlWorldShadow->m_CamViewWindowX, sizeof(RwReal), 1, pFile);
		::fwrite(&pNtlWorldShadow->m_matLight, sizeof(RwMatrix), 1, pFile);

		RwInt32 Size = RwRasterGetWidth(pNtlWorldShadow->m_pTexShadow->raster);
		::fwrite(&Size, sizeof(RwInt32), 1, pFile);

		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
		{
			CNtlPLResourceManager::GetInstance()->SaveTexture(pNtlWorldShadow->m_pTexShadow, pFile);
		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
		{
			CNtlPLResourceManager::GetInstance()->SaveTextureShadowMap(pNtlWorldShadow->m_pTexShadow, pFile);
		}				
	}
	else
	{
		::fwrite(&s_bFalseFlag, sizeof(RwBool), 1, pFile);
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorShadowSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	RwBool bBooleanFlag;
	// shadow existence
	CopyMemory(&bBooleanFlag, pFileMem, sizeof(RwBool));
	::fwrite(&bBooleanFlag, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);

	if (bBooleanFlag)
	{
		fwrite(pFileMem, sizeof(RwRGBA), 1, pFile);
		pFileMem += sizeof(RwRGBA);

		// camera view window x
		::fwrite(pFileMem, sizeof(RwReal), 1, pFile);
		pFileMem += sizeof(RwReal);

		::fwrite(pFileMem, sizeof(RwMatrix), 1, pFile);
		pFileMem += sizeof(RwMatrix);

		// render2texture size
		RwInt32 iSize = 0;
		CopyMemory(&iSize, pFileMem, sizeof(RwInt32));
		::fwrite(&iSize, sizeof(RwInt32), 1, pFile);
		pFileMem += sizeof(RwInt32);

		// argb bits		
		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
		{
			::fwrite(pFileMem, (sizeof(DWORD) * iSize * iSize), 1, pFile);
			pFileMem += (sizeof(DWORD) * iSize * iSize);
		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
		{
			::fwrite(pFileMem, (sizeof(RwUInt8) * 3 * iSize * iSize), 1, pFile);
			pFileMem += (sizeof(RwUInt8) * 3 * iSize * iSize);
		}
	}

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorShadowSkipToFile(FILE* pFile)
{
	RwBool BooleanFlag;
	::fread(&BooleanFlag, sizeof(RwBool), 1, pFile);

	if(BooleanFlag)
	{
		::fseek(pFile, sizeof(RwRGBA), SEEK_CUR);

		::fseek(pFile, sizeof(RwReal) + sizeof(RwMatrix), SEEK_CUR);

		RwInt32 Size;
		::fread(&Size, sizeof(RwInt32), 1, pFile);
		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
		{
			::fseek(pFile, sizeof(RwUInt8) * 3 * Size * Size, SEEK_CUR);
		}
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorShadowSkipToFileMem(BYTE* pFileMem)
{
	RwBool bBooleanFlag;
	// shadow existence
	CopyMemory(&bBooleanFlag, pFileMem, sizeof(RwBool));
	pFileMem += sizeof(RwBool);

	if (bBooleanFlag)
	{
		pFileMem += sizeof(RwRGBA);

		// camera view window x
		pFileMem += sizeof(RwReal);

		pFileMem += sizeof(RwMatrix);

		// render2texture size
		RwInt32 iSize = 0;
		CopyMemory(&iSize, pFileMem, sizeof(RwInt32));
		pFileMem += sizeof(RwInt32);

		// argb bits		
		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
		{
			pFileMem += (sizeof(DWORD) * iSize * iSize);
		}
		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
		{
			pFileMem += (sizeof(RwUInt8) * 3 * iSize * iSize);
		}
	}

	return pFileMem;
}

// RwBool CNtlWorldSectorInfo::SectorShadowLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
// {
// 	RwBool bBooleanFlag;
// 	fread(&bBooleanFlag, sizeof(RwBool), 1, pFile);
// 
// 	if(bBooleanFlag)
// 	{
// 		CNtlWorldShadow* pNtlWorldShadow = pNtlWorldSector->m_pNtlWorldShadow;
// 
// 		fread(&pNtlWorldShadow->m_rgbaShadow, sizeof(RwRGBA), 1, pFile);
// 		fread(&pNtlWorldShadow->m_CamViewWindowX, sizeof(RwReal), 1, pFile);
// 		fread(&pNtlWorldShadow->m_matLight, sizeof(RwMatrix), 1, pFile);
// 
// 		RwInt32 iSize;
// 		fread(&iSize, sizeof(RwInt32), 1, pFile);
// 
// 		pNtlWorldShadow->m_pTexShadow = CNtlPLResourceManager::GetInstance()->CreateTexture("ShadowMap", iSize, iSize, 32, rwRASTERFORMAT8888);
// 		DBO_ASSERT(pNtlWorldShadow->m_pTexShadow, "Texture load failed.");
// 
// 		RwTextureSetFilterMode(pNtlWorldShadow->m_pTexShadow, rwFILTERLINEAR);
// 		RwTextureSetAddressing(pNtlWorldShadow->m_pTexShadow, rwTEXTUREADDRESSCLAMP);
// 
// 		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION_OLD))
// 		{
// 			CNtlPLResourceManager::GetInstance()->LoadTexture(pNtlWorldShadow->m_pTexShadow, pFile);		
// 		}
// 		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
// 		{
// 			CNtlPLResourceManager::GetInstance()->LoadAlphaBitsShadowMap(pNtlWorldShadow->m_pTexShadow, pFile, &pNtlWorldShadow->m_rgbaShadow);
// 		}		
// 	}	
// 
// 	return TRUE;
// }
// 
// RwBool CNtlWorldSectorInfo::SectorShadowSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
// {
// 	CNtlWorldShadow* pNtlWorldShadow = pNtlWorldSector->m_pNtlWorldShadow;
// 
// 	if(pNtlWorldShadow->m_pTexShadow)
// 	{
// 		::fwrite(&s_bTrueFlag, sizeof(RwBool), 1, pFile);
// 		::fwrite(&pNtlWorldShadow->m_rgbaShadow, sizeof(RwRGBA), 1, pFile);
// 		::fwrite(&pNtlWorldShadow->m_CamViewWindowX, sizeof(RwReal), 1, pFile);
// 		::fwrite(&pNtlWorldShadow->m_matLight, sizeof(RwMatrix), 1, pFile);
// 
// 		RwInt32 Size = RwRasterGetWidth(pNtlWorldShadow->m_pTexShadow->raster);
// 		::fwrite(&Size, sizeof(RwInt32), 1, pFile);
// 
// 		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
// 		{
// 			CNtlPLResourceManager::GetInstance()->SaveTexture(pNtlWorldShadow->m_pTexShadow, pFile);
// 		}
// 		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
// 		{
// 			CNtlPLResourceManager::GetInstance()->SaveAlphaBitsShadowMap(pNtlWorldShadow->m_pTexShadow, pFile);
// 		}				
// 	}
// 	else
// 	{
// 		::fwrite(&s_bFalseFlag, sizeof(RwBool), 1, pFile);
// 	}
// 
// 	return TRUE;
// }
// 
// BYTE* CNtlWorldSectorInfo::SectorShadowSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
// {
// 	RwBool bBooleanFlag;
// 	// shadow existence
// 	CopyMemory(&bBooleanFlag, pFileMem, sizeof(RwBool));
// 	::fwrite(&bBooleanFlag, sizeof(RwBool), 1, pFile);
// 	pFileMem += sizeof(RwBool);
// 
// 	if (bBooleanFlag)
// 	{
// 		fwrite(pFileMem, sizeof(RwRGBA), 1, pFile);
// 		pFileMem += sizeof(RwRGBA);
// 
// 		// camera view window x
// 		::fwrite(pFileMem, sizeof(RwReal), 1, pFile);
// 		pFileMem += sizeof(RwReal);
// 
// 		::fwrite(pFileMem, sizeof(RwMatrix), 1, pFile);
// 		pFileMem += sizeof(RwMatrix);
// 
// 		// render2texture size
// 		RwInt32 iSize = 0;
// 		CopyMemory(&iSize, pFileMem, sizeof(RwInt32));
// 		::fwrite(&iSize, sizeof(RwInt32), 1, pFile);
// 		pFileMem += sizeof(RwInt32);
// 
// 		// argb bits		
// 		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
// 		{
// 			::fwrite(pFileMem, (sizeof(DWORD) * iSize * iSize), 1, pFile);
// 			pFileMem += (sizeof(DWORD) * iSize * iSize);
// 		}
// 		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
// 		{
// 			::fwrite(pFileMem, (sizeof(RwUInt8) * iSize * iSize), 1, pFile);
// 			pFileMem += (sizeof(RwUInt8) * iSize * iSize);
// 		}
// 	}
// 
// 	return pFileMem;
// }
// 
// RwInt32 CNtlWorldSectorInfo::SectorShadowSkipToFile(FILE* pFile)
// {
// 	RwBool BooleanFlag;
// 	::fread(&BooleanFlag, sizeof(RwBool), 1, pFile);
// 	if(BooleanFlag)
// 	{
// 		::fseek(pFile, sizeof(RwRGBA), SEEK_CUR);
// 
// 		::fseek(pFile, sizeof(RwReal) + sizeof(RwMatrix), SEEK_CUR);
// 
// 		RwInt32 Size;
// 		::fread(&Size, sizeof(RwInt32), 1, pFile);
// 
// 		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION_OLD))
// 		{
// 			::fseek(pFile, sizeof(DWORD) * Size * Size, SEEK_CUR);
// 		}
// 		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
// 		{
// 			::fseek(pFile, sizeof(RwUInt8) * Size * Size, SEEK_CUR);
// 		}
// 	}
// 
// 	return ::ftell(pFile);
// }
// 
// BYTE* CNtlWorldSectorInfo::SectorShadowSkipToFileMem(BYTE* pFileMem)
// {
// 	RwBool bBooleanFlag;
// 	// shadow existence
// 	CopyMemory(&bBooleanFlag, pFileMem, sizeof(RwBool));
// 	pFileMem += sizeof(RwBool);
// 
// 	if (bBooleanFlag)
// 	{
// 		pFileMem += sizeof(RwRGBA);
// 
// 		// camera view window x
// 		pFileMem += sizeof(RwReal);
// 
// 		pFileMem += sizeof(RwMatrix);
// 
// 		// render2texture size
// 		RwInt32 iSize = 0;
// 		CopyMemory(&iSize, pFileMem, sizeof(RwInt32));
// 		pFileMem += sizeof(RwInt32);
// 
// 		// argb bits		
// 		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
// 		{
// 			pFileMem += (sizeof(DWORD) * iSize * iSize);
// 		}
// 		else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
// 		{
// 			pFileMem += (sizeof(RwUInt8) * iSize * iSize);
// 		}
// 	}
// 
// 	return pFileMem;
// }

RwBool CNtlWorldSectorInfo::SectorEffectLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);
	for(RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlInstanceEffect* pNtlInstanceEffect = EffectLoadFromFile(pFile);
		if (pNtlInstanceEffect)
		{
			pNtlWorldSector->m_vecNtlPLEntityEffect.push_back(pNtlInstanceEffect);		
		}
	}

#ifdef dNTL_WORLD_TOOL_MODE
	CNtlInstanceEffect::LoadSwapFile(pNtlWorldSector->DatumPoint.x, pNtlWorldSector->DatumPoint.y, pNtlWorldSector->DatumPoint.z);
#endif

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorEffectSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	iNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityEffect.size());
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlInstanceEffect* pNtlInstanceEffect = static_cast<CNtlInstanceEffect*>(pNtlWorldSector->m_vecNtlPLEntityEffect.at(i));

		EffectSaveIntoFile(pFile, pNtlInstanceEffect);
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorEffectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField /* = -1 */, RwInt32 iIdxDstField /* = -1 */, CNtlWorldFieldManager* pWorldFieldMgr /* = NULL */)
{
	RwInt32 iNum = 0;

	RwV3d	vOffset;
	if (pWorldFieldMgr)
	{
		vOffset = pWorldFieldMgr->GetFieldRelativeCoord(iIdxSrcField, iIdxDstField);
	}
	else
	{
		vOffset.z = vOffset.x = 0.0f;
	}
	vOffset.y = 0.0f;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	::fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = EffectSaveIntoFileFromFileMem(pFile, pFileMem, &vOffset);		
	}

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorEffectSkipToFile(FILE* pFile)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);

	for (RwInt32 i = 0; i < iNum; ++i)
	{
		EffectSkipToFile(pFile);
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorEffectSkipToFileMem(BYTE* pFileMem)
{
	RwInt32 iNum = 0;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = EffectSkipToFileMem(pFileMem);		
	}

	return pFileMem;
}

CNtlInstanceEffect* CNtlWorldSectorInfo::EffectLoadFromFile(FILE* pFile)
{
	RwChar		acName[128];
	RwV3d		avSRT[3];

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		
		fread(&uiLength, sizeof(RwUInt32), 1, pFile);
		fread(acName, sizeof(RwChar) * uiLength, 1, pFile);
		acName[uiLength] = '\0';
	}
	
	fread(&avSRT[0], sizeof(RwV3d), 1, pFile);
	fread(&avSRT[1], sizeof(RwV3d), 1, pFile);
	fread(&avSRT[2], sizeof(RwV3d), 1, pFile);

	SPLEntityCreateParam sCreateParam;
	sCreateParam.pPos		= &avSRT[2];
//	DBO_WARNING_MESSAGE("acName: " << acName);
	CNtlInstanceEffect* pNtlInstanceEffect = static_cast<CNtlInstanceEffect*>(GetSceneManager()->CreateEntity(PLENTITY_EFFECT, acName, &sCreateParam));
	DBO_ASSERT(pNtlInstanceEffect, "Entity create failed.");

#ifdef dNTL_WORLD_TOOL_MODE
	pNtlInstanceEffect->SetVisible(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_EFFECTS));

	if(dGET_CUR_PALETTE() == ePM_EFFECT)
	{
		pNtlInstanceEffect->SetVisibleBoundingVolume(TRUE);
	}
	else
	{
		pNtlInstanceEffect->SetVisibleBoundingVolume(FALSE);
	}
#endif

	pNtlInstanceEffect->SetScale(avSRT[0].x);
	pNtlInstanceEffect->SetRotate(avSRT[1].x, avSRT[1].y, avSRT[1].z);
	pNtlInstanceEffect->SetPosition(&avSRT[2]);

	pNtlInstanceEffect->LoadFromFile(pFile);

	return pNtlInstanceEffect;
}

RwBool CNtlWorldSectorInfo::EffectSaveIntoFile(FILE* pFile, CNtlInstanceEffect* pNtlInstanceEffect)
{
	RwChar acName[128];
	::strcpy_s(acName, const_cast<RwChar*>(pNtlInstanceEffect->GetName()));

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
	{
		fwrite(acName, sizeof(RwChar) * 128, 1, pFile);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = strlen(acName);
		
		fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		fwrite(acName, sizeof(RwChar) * uiLength, 1, pFile);
	}

	RwV3d		avSRT[3];
	avSRT[0].x	= avSRT[0].y = avSRT[0].z = pNtlInstanceEffect->GetScale();
	avSRT[1]	= *pNtlInstanceEffect->GetRotate();
	avSRT[2]	= pNtlInstanceEffect->GetPosition();
	fwrite(&avSRT[0], sizeof(RwV3d), 1, pFile);
	fwrite(&avSRT[1], sizeof(RwV3d), 1, pFile);
	fwrite(&avSRT[2], sizeof(RwV3d), 1, pFile);

	return pNtlInstanceEffect->SaveIntoFile(pFile);
}

BYTE* CNtlWorldSectorInfo::EffectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	// Obj. Name
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		fwrite(pFileMem, (sizeof(RwChar) * 128), 1, pFile);
		pFileMem += (sizeof(RwChar) * 128);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		
		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);

		fwrite(pFileMem, (sizeof(RwChar) * uiLength), 1, pFile);
		pFileMem += sizeof(RwChar) * uiLength;
	}

	RwInt32 SizMemBlock = sizeof(RwV3d) + sizeof(RwV3d); // Scaling, Rotation
		 
	fwrite(pFileMem, SizMemBlock, 1, pFile);
	pFileMem += SizMemBlock;

	RwV3d vPos;
	CopyMemory(&vPos, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	if (pvOffset)
	{
		vPos.x += pvOffset->x;
		vPos.y += pvOffset->y;
		vPos.z += pvOffset->z;
	}
	::fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	return CNtlInstanceEffect::SaveIntoFileFromFileMem(pFile, pFileMem);
}


RwInt32 CNtlWorldSectorInfo::EffectSkipToFile(FILE* pFile)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		::fread(&uiLength, sizeof(RwUInt32), 1, pFile);

		::fseek(pFile, (sizeof(RwChar) * uiLength), SEEK_CUR);
	}

	RwInt32	SkipRange = (sizeof(RwV3d) * 3);		// Scaling, Rotation, Translation

	::fseek(pFile, SkipRange, SEEK_CUR);

	return CNtlInstanceEffect::SkipToFile(pFile);
}

BYTE* CNtlWorldSectorInfo::EffectSkipToFileMem(BYTE* pFileMem)
{	
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		// Obj. Name
		pFileMem += (sizeof(RwChar) * 128);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		pFileMem += sizeof(RwUInt32);

		// Obj. Name
		pFileMem += sizeof(RwChar) * uiLength;
	}

	RwInt32 SizMemBlock =	sizeof(RwV3d) +				// Scaling
							sizeof(RwV3d) + 			// Rotation
							sizeof(RwV3d);				// Translation

	pFileMem += SizMemBlock;

	return CNtlInstanceEffect::SkipToFileMem(pFileMem);
}

RwBool CNtlWorldSectorInfo::SectorSoundEffectLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);
	for(RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLSound* pNtlPLSound = SoundEffectLoadFromFile(pFile);
		if (pNtlPLSound)
		{
			pNtlWorldSector->m_vecNtlPLEntitySound.push_back(pNtlPLSound);
		}
	}

	fread(&iNum, sizeof(RwInt32), 1, pFile);
	for(RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLSoundBGM* pNtlPLSoundBGM = SoundEffectBGMLoadFromFile(pFile);
		if (pNtlPLSoundBGM)
		{
			pNtlWorldSector->m_vecNtlPLEntitySoundBGM.push_back(pNtlPLSoundBGM);
		}
	}

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorSoundEffectSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	iNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntitySound.size());
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLSound* pNtlPLSound = static_cast<CNtlPLSound*>(pNtlWorldSector->m_vecNtlPLEntitySound.at(i));		

		SoundEffectSaveIntoFile(pFile, pNtlPLSound);		
	}

	iNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntitySoundBGM.size());
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLSoundBGM* pNtlPLSoundBGM = static_cast<CNtlPLSoundBGM*>(pNtlWorldSector->m_vecNtlPLEntitySoundBGM.at(i));		

		SoundEffectBGMSaveIntoFile(pFile, pNtlPLSoundBGM);		
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorSoundEffectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField /* = -1 */, RwInt32 iIdxDstField /* = -1 */, CNtlWorldFieldManager* pWorldFieldMgr /* = NULL */)
{
	RwInt32 iNum = 0;

	RwV3d	vOffset;
	if (pWorldFieldMgr)
	{
		vOffset = pWorldFieldMgr->GetFieldRelativeCoord(iIdxSrcField, iIdxDstField);
	}
	else
	{
		vOffset.z = vOffset.x = 0.0f;
	}
	vOffset.y = 0.0f;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	::fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = SoundEffectSaveIntoFileFromFileMem(pFile, pFileMem, &vOffset);		
	}

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	::fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = SoundEffectBGMSaveIntoFileFromFileMem(pFile, pFileMem, &vOffset);		
	}

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorSoundEffectSkipToFile(FILE* pFile)
{
	RwInt32 iNum = 0;

	::fread(&iNum, sizeof(RwInt32), 1, pFile);

	for (RwInt32 i = 0; i < iNum; ++i)
	{
		SoundEffectSkipToFile(pFile);
	}

	::fread(&iNum, sizeof(RwInt32), 1, pFile);

	for (RwInt32 i = 0; i < iNum; ++i)
	{
		SoundEffectBGMSkipToFile(pFile);
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorSoundEffectSkipToFileMem(BYTE* pFileMem)
{
	RwInt32 iNum = 0;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = SoundEffectSkipToFileMem(pFileMem);		
	}

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = SoundEffectBGMSkipToFileMem(pFileMem);		
	}

	return pFileMem;
}

CNtlPLSound* CNtlWorldSectorInfo::SoundEffectLoadFromFile(FILE* pFile)
{
	RwV3d	vPos;		
	RwReal	fVolume;
	RwReal	fRadiusMin;
	RwReal	fRadiusMax;
	RwChar  acName[32] = {0,};

	 if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		fread(&uiLength, sizeof(RwUInt32), 1, pFile);
		fread(acName, sizeof(RwChar) * uiLength, 1, pFile);
		acName[uiLength] = '\0';
	}

	fread(&fVolume, sizeof(RwReal), 1, pFile);
	fread(&fRadiusMin, sizeof(RwReal), 1, pFile);
	fread(&fRadiusMax, sizeof(RwReal), 1, pFile);
	fread(&vPos, sizeof(RwV3d), 1, pFile);

	SPLEntityCreateParam sCreateParam;
	sCreateParam.pPos		= &vPos;

	CNtlPLSound* pNtlPLSound = static_cast<CNtlPLSound*>(GetSceneManager()->CreateEntity(PLENTITY_SOUND, "NULL", &sCreateParam));
	DBO_ASSERT(pNtlPLSound, "Entity create failed.");

	pNtlPLSound->GetVolume()		= fVolume;
	pNtlPLSound->GetRadius(TRUE)	= fRadiusMin;
	pNtlPLSound->GetRadius(FALSE)	= fRadiusMax;
	pNtlPLSound->PlaySound(acName);

	return pNtlPLSound;
}

RwBool CNtlWorldSectorInfo::SoundEffectSaveIntoFile(FILE* pFile, CNtlPLSound* pNtlPLSound)
{
	RwChar acName[32] = {0,};
	strcpy_s(acName, 32, pNtlPLSound->GetName());

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
	{
		fwrite(acName, sizeof(RwChar) * 32, 1, pFile);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = strlen(acName);
		fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		fwrite(acName, sizeof(RwChar) * uiLength, 1, pFile);
	}
	
	RwReal fVolume = pNtlPLSound->GetVolume();
	fwrite(&fVolume, sizeof(RwReal), 1, pFile);

	RwReal fRadiusMin = pNtlPLSound->GetRadius(TRUE);
	fwrite(&fRadiusMin, sizeof(RwReal), 1, pFile);

	RwReal fRadiusMax = pNtlPLSound->GetRadius(FALSE);
	fwrite(&fRadiusMax, sizeof(RwReal), 1, pFile);

	RwV3d vPos = pNtlPLSound->GetPosition();
	fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SoundEffectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	// Obj. Name
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		fwrite(pFileMem, (sizeof(RwChar) * 32), 1, pFile);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);

		fwrite(pFileMem, (sizeof(RwChar) * uiLength), 1, pFile);
		pFileMem += (sizeof(RwChar) * uiLength);
	}	

	RwInt32 SizMemBlock =	sizeof(RwReal) +				// Vol
							sizeof(RwReal) +				// RadiusMin
							sizeof(RwReal);					// RadiusMax

	fwrite(pFileMem, SizMemBlock, 1, pFile);
	pFileMem += SizMemBlock;

	RwV3d vPos;
	CopyMemory(&vPos, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	if (pvOffset)
	{
		vPos.x += pvOffset->x;
		vPos.y += pvOffset->y;
		vPos.z += pvOffset->z;
	}
	::fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SoundEffectSkipToFile(FILE* pFile)
{
	// Obj. Name
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION_OLD))
	{
		fseek(pFile, (sizeof(RwChar) * 32), SEEK_CUR); 		
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		fread(&uiLength, sizeof(RwUInt32), 1, pFile);
		fseek(pFile, (sizeof(RwChar) * uiLength), SEEK_CUR);
	}

	RwInt32 SkipRange = (sizeof(RwReal) * 3) +		// Vol, Radius Min, Max
						(sizeof(RwV3d));			// Translation

	::fseek(pFile, SkipRange, SEEK_CUR);

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SoundEffectSkipToFileMem(BYTE* pFileMem)
{
	// Obj. Name
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		pFileMem += (sizeof(RwChar) * 32);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		pFileMem += sizeof(RwUInt32);

		pFileMem += (sizeof(RwChar) * uiLength);
	}

	RwInt32 SizMemBlock =	sizeof(RwReal) +			// Vol
							sizeof(RwReal) +			// RadiusMin
							sizeof(RwReal) +			// RadiusMax
							sizeof(RwV3d);				// Translation

	pFileMem += SizMemBlock;

	return pFileMem;
}

CNtlPLSoundBGM* CNtlWorldSectorInfo::SoundEffectBGMLoadFromFile(FILE* pFile)
{
	RwV3d	vPos;		
	RwReal	fRadius;
	RwChar  acName[32] = {0,};

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		fread(&uiLength, sizeof(RwUInt32), 1, pFile);
		fread(acName, sizeof(RwChar) * uiLength, 1, pFile);
		acName[uiLength] = '\0';
	}

	fread(&fRadius, sizeof(RwReal), 1, pFile);
	fread(&vPos, sizeof(RwV3d), 1, pFile);

	SPLEntityCreateParam sCreateParam;
	sCreateParam.pPos		= &vPos;

	CNtlPLSoundBGM* pNtlPLSoundBGM = static_cast<CNtlPLSoundBGM*>(GetSceneManager()->CreateEntity(PLENTITY_SOUND_BGM, "NULL", &sCreateParam));
	DBO_ASSERT(pNtlPLSoundBGM, "Entity create failed.");

	pNtlPLSoundBGM->GetRadius() = fRadius;
	pNtlPLSoundBGM->PlaySound(acName);

	return pNtlPLSoundBGM;
}

RwBool CNtlWorldSectorInfo::SoundEffectBGMSaveIntoFile(FILE* pFile, CNtlPLSoundBGM* pNtlPLSoundBGM)
{
	RwChar acName[32] = {0,};
	strcpy_s(acName, 32, pNtlPLSoundBGM->GetName());

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
	{
		fwrite(acName, sizeof(RwChar) * 32, 1, pFile);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = strlen(acName);
		fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		fwrite(acName, sizeof(RwChar) * uiLength, 1, pFile);
	}

	RwReal fRadius = pNtlPLSoundBGM->GetRadius();
	fwrite(&fRadius, sizeof(RwReal), 1, pFile);

	RwV3d vPos = pNtlPLSoundBGM->GetPosition();
	fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SoundEffectBGMSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	// Obj. Name
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		fwrite(pFileMem, (sizeof(RwChar) * 32), 1, pFile);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);

		fwrite(pFileMem, (sizeof(RwChar) * uiLength), 1, pFile);
		pFileMem += (sizeof(RwChar) * uiLength);
	}

	// Radius
	fwrite(pFileMem, sizeof(RwReal), 1, pFile);
	pFileMem += sizeof(RwReal);

	RwV3d vPos;
	CopyMemory(&vPos, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	if (pvOffset)
	{
		vPos.x += pvOffset->x;
		vPos.y += pvOffset->y;
		vPos.z += pvOffset->z;
	}
	::fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SoundEffectBGMSkipToFile(FILE* pFile)
{
	// Obj. Name
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION_OLD))
	{
		fseek(pFile, (sizeof(RwChar) * 32), SEEK_CUR); 		
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		fread(&uiLength, sizeof(RwUInt32), 1, pFile);
		fseek(pFile, (sizeof(RwChar) * uiLength), SEEK_CUR);
	}

	RwInt32 SkipRange = (sizeof(RwReal)) +			// Radius
						(sizeof(RwV3d));			// Translation

	::fseek(pFile, SkipRange, SEEK_CUR);

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SoundEffectBGMSkipToFileMem(BYTE* pFileMem)
{
	// Obj. Name
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		pFileMem += (sizeof(RwChar) * 32);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		pFileMem += sizeof(RwUInt32);

		pFileMem += (sizeof(RwChar) * uiLength);
	}

	RwInt32 SizMemBlock =	sizeof(RwReal) +			// Radius
							sizeof(RwV3d);				// Translation

	pFileMem += SizMemBlock;

	return pFileMem;
}

RwBool CNtlWorldSectorInfo::SectorCullLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	fread(&pNtlWorldSector->m_IsCulled, sizeof(RwBool), 1, pFile);

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorCullSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	fwrite(&pNtlWorldSector->m_IsCulled, sizeof(RwBool), 1, pFile);

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorCullSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	fwrite(pFileMem, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorCullSkipToFile(FILE* pFile)
{
	::fseek(pFile, sizeof(RwBool), SEEK_CUR);

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorCullSkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(RwBool);

	return pFileMem;
}

RwBool CNtlWorldSectorInfo::SectorTileTransparencyLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	fread(&pNtlWorldSector->m_IsTileTransparency, sizeof(RwBool), 1, pFile);

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorTileTransparencySaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	fwrite(&pNtlWorldSector->m_IsTileTransparency, sizeof(RwBool), 1, pFile);

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorTileTransparencySaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	fwrite(pFileMem, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorTileTransparencySkipToFile(FILE* pFile)
{
	::fseek(pFile, sizeof(RwBool), SEEK_CUR);

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorTileTransparencySkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(RwBool);

	return pFileMem;
}

RwBool CNtlWorldSectorInfo::SectorShoreLineLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwBool bIsThereShoreLine = FALSE;

	fread(&bIsThereShoreLine, sizeof(RwBool), 1, pFile);
	if(bIsThereShoreLine)
	{
		RwV3d vPos;
		vPos.x = pNtlWorldSector->DatumPoint.x;
		vPos.y = pNtlWorldSector->DatumPoint.y;
		vPos.z = pNtlWorldSector->DatumPoint.z;

		pNtlWorldSector->m_pNtlPLEntityShoreLine = ShoreLineLoadFromFile(pFile, &vPos);				
	}

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorShoreLineSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	CNtlPLShoreLine* pNtlPLShoreLine = static_cast<CNtlPLShoreLine*>(pNtlWorldSector->m_pNtlPLEntityShoreLine);

	if (pNtlPLShoreLine && pNtlPLShoreLine->IsValidShoreline())
	{
		fwrite(&s_bTrueFlag, sizeof(RwBool), 1, pFile);

		ShoreLineSaveIntoFile(pFile, pNtlPLShoreLine);
	}
	else
	{
		fwrite(&s_bFalseFlag, sizeof(RwBool), 1, pFile);
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorShoreLineSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField /* = -1 */, RwInt32 iIdxDstField /* = -1 */, CNtlWorldFieldManager* pWorldFieldMgr /* = NULL */)
{
	RwBool bIsThereShoreLine;

	RwV3d	vOffset;
	if (pWorldFieldMgr)
	{
		vOffset = pWorldFieldMgr->GetFieldRelativeCoord(iIdxSrcField, iIdxDstField);
	}
	else
	{
		vOffset.z = vOffset.x = 0.0f;
	}
	vOffset.y = 0.0f;

	CopyMemory(&bIsThereShoreLine, pFileMem, sizeof(RwBool));
	::fwrite(&bIsThereShoreLine, sizeof(RwBool), 1, pFile);
	pFileMem += sizeof(RwBool);
	if (bIsThereShoreLine)
	{
		pFileMem = ShoreLineSaveIntoFileFromFileMem(pFile, pFileMem, &vOffset);		
	}

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorShoreLineSkipToFile(FILE* pFile)
{
	RwBool bIsThereShoreLine = FALSE;

	fread(&bIsThereShoreLine, sizeof(RwBool), 1, pFile);
	if (bIsThereShoreLine)
	{
		return ShoreLineSkipToFile(pFile);
	}
	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorShoreLineSkipToFileMem(BYTE* pFileMem)
{
	RwBool bIsThereShoreLine;

	CopyMemory(&bIsThereShoreLine, pFileMem, sizeof(RwBool));
	pFileMem += sizeof(RwBool);
	if (bIsThereShoreLine)
	{
		pFileMem = ShoreLineSkipToFileMem(pFileMem);		
	}

	return pFileMem;
}

CNtlPLShoreLine* CNtlWorldSectorInfo::ShoreLineLoadFromFile(FILE* pFile, RwV3d* pvPos)
{
	SPLEntityCreateParam sCreateParam;
	sCreateParam.pPos		= pvPos;

	CNtlPLShoreLine* pNtlPLShoreLine = static_cast<CNtlPLShoreLine*>(GetSceneManager()->CreateEntity(PLENTITY_SHORELINE, NTL_PLEN_SHORELINE, &sCreateParam));
	DBO_ASSERT(pNtlPLShoreLine, "Entity create failed.");

	pNtlPLShoreLine->LoadFromFile(pFile);

	return pNtlPLShoreLine;
}

RwBool CNtlWorldSectorInfo::ShoreLineSaveIntoFile(FILE* pFile, CNtlPLShoreLine* pNtlPLShoreLine)
{
	return pNtlPLShoreLine->SaveIntoFile(pFile);
}

BYTE* CNtlWorldSectorInfo::ShoreLineSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	return CNtlPLShoreLine::SaveIntoFileFromFileMem(pFile, pFileMem, pvOffset);
}

RwBool CNtlWorldSectorInfo::ShoreLineSkipToFile(FILE* pFile)
{
	return CNtlPLShoreLine::SkipToFile(pFile);
}

BYTE* CNtlWorldSectorInfo::ShoreLineSkipToFileMem(BYTE* pFileMem)
{
	return CNtlPLShoreLine::SkipToFileMem(pFileMem);
}

RwBool CNtlWorldSectorInfo::SectorDecalLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);
	for (int i = 0; i < iNum; ++i)
	{
		CNtlPLWorldDecal* pNtlPLWorldDecal = DecalLoadFromFile(pFile);
		if (pNtlPLWorldDecal)
		{
			pNtlWorldSector->m_vecNtlPLEntityDecal.push_back(pNtlPLWorldDecal);
		}
	}

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorDecalSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	iNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityDecal.size());
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLWorldDecal* pNtlPLWorldDecal = static_cast<CNtlPLWorldDecal*>(pNtlWorldSector->m_vecNtlPLEntityDecal.at(i));		

		DecalSaveIntoFile(pFile, pNtlPLWorldDecal);		
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorDecalSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField /* = -1 */, RwInt32 iIdxDstField /* = -1 */, CNtlWorldFieldManager* pWorldFieldMgr /* = NULL */)
{
	RwInt32 iNum = 0;

	RwV3d	vOffset;
	if (pWorldFieldMgr)
	{
		vOffset = pWorldFieldMgr->GetFieldRelativeCoord(iIdxSrcField, iIdxDstField);
	}
	else
	{
		vOffset.z = vOffset.x = 0.0f;
	}
	vOffset.y = 0.0f;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	::fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = DecalSaveIntoFileFromFileMem(pFile, pFileMem, &vOffset);		
	}
	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorDecalSkipToFile(FILE* pFile)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);

	for (RwInt32 i = 0; i < iNum; ++i)
	{
		DecalSkipToFile(pFile);
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorDecalSkipToFileMem(BYTE* pFileMem)
{
	RwInt32 iNum = 0;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = DecalSkipToFileMem(pFileMem);		
	}
	return pFileMem;
}

CNtlPLWorldDecal* CNtlWorldSectorInfo::DecalLoadFromFile(FILE* pFile)
{
	sDECAL_PROP_PARAM sDecalPropParam;

	CNtlPLWorldDecal::Load(sDecalPropParam, pFile);

	SPLDecalCreateParam sCreateParam;
	sCreateParam.pTexName	= sDecalPropParam._TexName;
	sCreateParam.pTexPath	= ".\\Texture\\NtlWe\\Decal\\";
	sCreateParam.nTexNameLen = sDecalPropParam._TexNameLen;

	RwReal fWidth	= sDecalPropParam._SizeX;
	RwReal fHeight	= sDecalPropParam._SizeZ;
	if(fWidth > fHeight)
	{
		sCreateParam.fIntersectionRadius = fWidth;
	}
	else
	{
		sCreateParam.fIntersectionRadius = fHeight;
	}

	sCreateParam.vSize.x			= sDecalPropParam._SizeX;
	sCreateParam.vSize.z			= sDecalPropParam._SizeZ;
	sCreateParam.fScale				= sDecalPropParam._TexScale;
	sCreateParam.fVisibleSquaredDist= sDecalPropParam._VisibilityDist;

	string CurTexName = sDecalPropParam._TexName;
	string W_Prefix("w_");
	string C_Prefix("c_");

	if(-1 == CurTexName.find_first_of(W_Prefix) && -1 == CurTexName.find_first_of(C_Prefix))
	{
		DBO_TRACE(FALSE, "Decal textures should've got prefix; w_, c_");
	}
	else
	{
		RwInt32 IdxWPrefix = CurTexName.find_first_of(W_Prefix);
		RwInt32 IdxCPrefix = CurTexName.find_first_of(C_Prefix);

		if(!IdxWPrefix)
		{
			sCreateParam.rwWrapType = rwTEXTUREADDRESSWRAP;
		}
		else if(!IdxCPrefix)
		{
			sCreateParam.rwWrapType = rwTEXTUREADDRESSCLAMP;
		}
		else
		{
			DBO_TRACE(FALSE, "Decal textures should've got prefix; w_, c_");
		}
	}

	if(sDecalPropParam._DecalMode == 0)
	{
		sCreateParam.nFlagEffect = E_FLAG_DECAL_RENDER_WORLD;
	}
	else if(sDecalPropParam._DecalMode == 1)
	{
		sCreateParam.nFlagEffect = E_FLAG_DECAL_RENDER_OBJECT;
	}
	else if(sDecalPropParam._DecalMode == 2)
	{
		sCreateParam.nFlagEffect = E_FLAG_DECAL_RENDER_WORLD | E_FLAG_DECAL_RENDER_OBJECT;
	}
	else if(sDecalPropParam._DecalMode == 3)
	{
		sCreateParam.nFlagEffect = E_FLAG_DECAL_RENDER_OBJECT | E_FLAG_DECAL_WATER;
	}

	sCreateParam.eDecalType = DECAL_TERRAIN;

	CNtlPLWorldDecal* pNtlPLWorldDecal = static_cast<CNtlPLWorldDecal*>(GetSceneManager()->CreateEntity(PLENTITY_DECAL, "NULL", &sCreateParam));
	DBO_ASSERT(pNtlPLWorldDecal, "Entity create failed.");

	pNtlPLWorldDecal->SetRotate(sDecalPropParam._TexAngle);
	pNtlPLWorldDecal->SetColor(sDecalPropParam._RGBA.red, sDecalPropParam._RGBA.green, sDecalPropParam._RGBA.blue);
	pNtlPLWorldDecal->SetAlpha(sDecalPropParam._RGBA.alpha);
	pNtlPLWorldDecal->SetAlphaBlend(TRUE, rwBLENDSRCALPHA, rwBLENDINVSRCALPHA);
	pNtlPLWorldDecal->SetPosition(&sDecalPropParam._Pos);
	pNtlPLWorldDecal->SetName(sDecalPropParam._TexName);
	pNtlPLWorldDecal->SetNameLen(sDecalPropParam._TexNameLen);

	if(sDecalPropParam._UVAniCheck)
	{
		if(sDecalPropParam._UVAniPatternIdx == 0)
		{
			SNtlPrtStdEmitterPrtUVAnim EmitterUVAnim;
			EmitterUVAnim.vDir.x		= 0.0f;
			EmitterUVAnim.vDir.y		= 0.0f;
			EmitterUVAnim.vDir.z		= 1.0f;
			EmitterUVAnim.fVelocity		= sDecalPropParam._UVAniSpeed;
			EmitterUVAnim.fMaxOffset	= 0.0f;
			pNtlPLWorldDecal->SetUVAnimAction(EmitterUVAnim);
		}
		else if(sDecalPropParam._UVAniPatternIdx == 1)
		{
			SNtlPrtStdEmitterPrtUVAnim EmitterUVAnim;
			EmitterUVAnim.vDir.x		= 0.0f;
			EmitterUVAnim.vDir.y		= 0.0f;
			EmitterUVAnim.vDir.z		= 1.0f;
			EmitterUVAnim.fVelocity		= sDecalPropParam._UVAniSpeed;
			EmitterUVAnim.fMaxOffset	= sDecalPropParam._UVAniOffset;
			pNtlPLWorldDecal->SetUVAnimAction(EmitterUVAnim);
		}
		else if(sDecalPropParam._UVAniPatternIdx == 2)
		{
			pNtlPLWorldDecal->Set2DRoateAction(sDecalPropParam._UVAniOffset, sDecalPropParam._UVAniSpeed);
		}
		else if(sDecalPropParam._UVAniPatternIdx == 3)
		{
			pNtlPLWorldDecal->SetSizeAction(sDecalPropParam._TexScale, sDecalPropParam._UVAniOffset, sDecalPropParam._UVAniSpeed);
		}
	}

	return pNtlPLWorldDecal;
}

RwBool CNtlWorldSectorInfo::DecalSaveIntoFile(FILE* pFile, CNtlPLWorldDecal* pNtlPLWorldDecal)
{
	return pNtlPLWorldDecal->SaveIntoFile(pFile);
}

BYTE* CNtlWorldSectorInfo::DecalSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	return CNtlPLWorldDecal::SaveIntoFileFromFileMem(pFile, pFileMem, pvOffset);
}

RwInt32 CNtlWorldSectorInfo::DecalSkipToFile(FILE* pFile)
{
	return CNtlPLWorldDecal::SkipToFile(pFile);
}

BYTE* CNtlWorldSectorInfo::DecalSkipToFileMem(BYTE* pFileMem)
{
	return CNtlPLWorldDecal::SkipToFileMem(pFileMem);
}

RwBool CNtlWorldSectorInfo::SectorPlantLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		sSECTOR_PLANT_SET_ATTR* pPlantSet = NTL_NEW sSECTOR_PLANT_SET_ATTR;
		RwInt32					iPlantObjNum;			

		fread(&pPlantSet->iSetIdx, sizeof(RwInt32), 1, pFile); // Set Index
		fread(&iPlantObjNum, sizeof(RwInt32), 1, pFile); // Obj Num

		for (int j = 0; j < iPlantObjNum; ++j)
		{
			sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR* pPlantObj = NTL_NEW sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR;

			fread(&pPlantObj->iTileIdx, sizeof(RwInt32), 1, pFile); // Tile Index

			pPlantSet->vecPlantObj.push_back(pPlantObj);
		}			
		pNtlWorldSector->m_vecPlantSet.push_back(pPlantSet);
	}

	GetSceneManager()->GetPlant()->OnLoad(pNtlWorldSector);

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorPlantSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	iNum = static_cast<RwInt32>(pNtlWorldSector->m_vecPlantSet.size());
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		sSECTOR_PLANT_SET_ATTR*	pPlantSet		= pNtlWorldSector->m_vecPlantSet.at(i);
		RwInt32					iPlantObjNum	= static_cast<RwInt32>(pPlantSet->vecPlantObj.size());

		fwrite(&pPlantSet->iSetIdx, sizeof(RwInt32), 1, pFile); // Set Index
		fwrite(&iPlantObjNum, sizeof(RwInt32), 1, pFile); // Obj Num

		for (int j = 0; j < iPlantObjNum; ++j)
		{
			fwrite(&pPlantSet->vecPlantObj.at(j)->iTileIdx, sizeof(RwInt32), 1, pFile); // Tile Index				
		}	
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorPlantSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem)
{
	RwInt32 iNum;	
	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		RwInt32 iPlnatSet;
		CopyMemory(&iPlnatSet, pFileMem, sizeof(RwInt32));
		pFileMem += sizeof(RwInt32);
		fwrite(&iPlnatSet, sizeof(RwInt32), 1, pFile);

		RwInt32 iPlantObjNum;			
		CopyMemory(&iPlantObjNum, pFileMem, sizeof(RwInt32));
		pFileMem += sizeof(RwInt32);
		fwrite(&iPlantObjNum, sizeof(RwInt32), 1, pFile);

		for (RwInt32 j = 0; j < iPlantObjNum; ++j)
		{
			RwInt32 iTileIdx;
			CopyMemory(&iTileIdx, pFileMem, sizeof(RwInt32));
			pFileMem += sizeof(RwInt32);
			fwrite(&iTileIdx, sizeof(RwInt32), 1, pFile);
		}		
	}

	return pFileMem;
}

RwBool CNtlWorldSectorInfo::SectorPlantSkipToFile(FILE* pFile)
{
	RwInt32 iNum = 0;

	::fread(&iNum, sizeof(RwInt32), 1, pFile);

	for (int i = 0; i < iNum; ++i)
	{
		// skip set id
		::fseek(pFile, sizeof(RwInt32), SEEK_CUR);

		// skip set num
		RwInt32 iPlantObjNum;			
		::fread(&iPlantObjNum, sizeof(RwInt32), 1, pFile);
		::fseek(pFile, sizeof(RwInt32) * iPlantObjNum, SEEK_CUR);
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorPlantSkipToFileMem(BYTE* pFileMem)
{
	RwInt32 iNum;	
	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem += sizeof(RwInt32);

		RwInt32 iPlantObjNum;			
		CopyMemory(&iPlantObjNum, pFileMem, sizeof(RwInt32));
		pFileMem += sizeof(RwInt32);

		for (RwInt32 j = 0; j < iPlantObjNum; ++j)
		{
			pFileMem += sizeof(RwInt32);
		}		
	}

	return pFileMem;
}

RwBool CNtlWorldSectorInfo::SectorWorldLightLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLWorldLight_Sphere* pNtlPLWorldLight_Sphere = WorldLightSphereLoadFromFile(pFile);		
		if (pNtlPLWorldLight_Sphere)
		{
			pNtlWorldSector->m_vecNtlPLEntityWorldLight_Sphere.push_back(pNtlPLWorldLight_Sphere);
		}
	}

	fread(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLWorldLight_Box* pNtlPLWorldLight_Box = WorldLightBoxLoadFromFile(pFile);		
		if (pNtlPLWorldLight_Box)
		{
			pNtlWorldSector->m_vecNtlPLEntityWorldLight_Box.push_back(pNtlPLWorldLight_Box);
		}
	}

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorWorldLightSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	iNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityWorldLight_Sphere.size());
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLWorldLight_Sphere* pNtlPLWorldLight_Sphere = static_cast<CNtlPLWorldLight_Sphere*>(pNtlWorldSector->m_vecNtlPLEntityWorldLight_Sphere.at(i));		

		WorldLightSphereSaveIntoFile(pFile, pNtlPLWorldLight_Sphere);		
	}

	iNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityWorldLight_Box.size());
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLWorldLight_Box* pNtlPLWorldLight_Box = static_cast<CNtlPLWorldLight_Box*>(pNtlWorldSector->m_vecNtlPLEntityWorldLight_Box.at(i));		

		WorldLightBoxSaveIntoFile(pFile, pNtlPLWorldLight_Box);		
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorWorldLightSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField /* = -1 */, RwInt32 iIdxDstField /* = -1 */, CNtlWorldFieldManager* pWorldFieldMgr /* = NULL */)
{
	RwInt32 iNum = 0;

	RwV3d	vOffset;
	if (pWorldFieldMgr)
	{
		vOffset = pWorldFieldMgr->GetFieldRelativeCoord(iIdxSrcField, iIdxDstField);
	}
	else
	{
		vOffset.z = vOffset.x = 0.0f;
	}
	vOffset.y = 0.0f;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	::fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = WorldLightSphereSaveIntoFileFromFileMem(pFile, pFileMem, &vOffset);		
	}

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	::fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = WorldLightBoxSaveIntoFileFromFileMem(pFile, pFileMem, &vOffset);		
	}

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorWorldLightSkipToFile(FILE* pFile)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);

	for (RwInt32 i = 0; i < iNum; ++i)
	{
		WorldLightSphereSkipToFile(pFile);
	}

	fread(&iNum, sizeof(RwInt32), 1, pFile);

	for (RwInt32 i = 0; i < iNum; ++i)
	{
		WorldLightBoxSkipToFile(pFile);
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorWorldLightSkipToFileMem(BYTE* pFileMem)
{
	RwInt32 iNum = 0;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = WorldLightSphereSkipToFileMem(pFileMem);		
	}

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = WorldLightBoxSkipToFileMem(pFileMem);		
	}

	return pFileMem;
}

CNtlPLWorldLight_Sphere* CNtlWorldSectorInfo::WorldLightSphereLoadFromFile(FILE* pFile)
{
	RwV3d				vPos;
	RwRGBA				clr;
	RwReal				fRadius;
	EPLWorldLightType	eType = EPLWORLDLIGHT_SPHERE;

	fread(&vPos, sizeof(RwV3d), 1, pFile);
	fread(&clr, sizeof(RwRGBA), 1, pFile);
	fread(&fRadius, sizeof(RwReal), 1, pFile);

	SPLWorldLightCreateParam_Sphere createParam;
	createParam.pPos		= &vPos;
	createParam._pUserData	= &eType;
	createParam.uiRed		= clr.red;
	createParam.uiGreen		= clr.green;
	createParam.uiBlue		= clr.blue;
	createParam.fRadius		= fRadius;

	CNtlPLWorldLight_Sphere* pNtlPLWorldLight_Sphere = static_cast<CNtlPLWorldLight_Sphere*>(GetSceneManager()->CreateEntity(PLENTITY_WORLD_LIGHT, "NULL", &createParam));
	DBO_ASSERT(pNtlPLWorldLight_Sphere, "Entity create failed.");

	return pNtlPLWorldLight_Sphere;
}

RwBool CNtlWorldSectorInfo::WorldLightSphereSaveIntoFile(FILE* pFile, CNtlPLWorldLight_Sphere* pNtlPLWorldLightSphere)
{
	RwV3d	vPos	= pNtlPLWorldLightSphere->GetPosition();
	RwRGBA	clr		= pNtlPLWorldLightSphere->GetColor();
	RwReal	fRadius	= pNtlPLWorldLightSphere->GetRadius();		

	fwrite(&vPos, sizeof(RwV3d), 1, pFile);
	fwrite(&clr, sizeof(RwRGBA), 1, pFile);
	fwrite(&fRadius, sizeof(RwReal), 1, pFile);

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::WorldLightSphereSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	RwV3d vPos;
	CopyMemory(&vPos, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	if (pvOffset)
	{
		vPos.x += pvOffset->x;
		vPos.y += pvOffset->y;
		vPos.z += pvOffset->z;
	}
	::fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	RwRGBA	clr;
	CopyMemory(&clr, pFileMem, sizeof(RwRGBA));
	pFileMem += sizeof(RwRGBA);
	fwrite(&clr, sizeof(RwRGBA), 1, pFile);

	RwReal	fRadius;
	CopyMemory(&fRadius, pFileMem, sizeof(RwReal));
	pFileMem += sizeof(RwReal);
	fwrite(&fRadius, sizeof(RwReal), 1, pFile);

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::WorldLightSphereSkipToFile(FILE* pFile)
{
	RwInt32 SkipRange = sizeof(RwV3d) + sizeof(RwRGBA) + sizeof(RwReal);

	// skip set id
	::fseek(pFile, SkipRange, SEEK_CUR);

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::WorldLightSphereSkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(RwV3d);
	pFileMem += sizeof(RwRGBA);
	pFileMem += sizeof(RwReal);

	return pFileMem;
}

CNtlPLWorldLight_Box* CNtlWorldSectorInfo::WorldLightBoxLoadFromFile(FILE* pFile)
{
	RwV3d				vPos;
	RwRGBA				clr;
	RwV3d				vRot;
	RwV3d				vScale;
	EPLWorldLightType	eType = EPLWORLDLIGHT_BOX;

	fread(&vPos, sizeof(RwV3d), 1, pFile);
	fread(&clr, sizeof(RwRGBA), 1, pFile);
	fread(&vRot, sizeof(RwV3d), 1, pFile);
	fread(&vScale, sizeof(RwV3d), 1, pFile);	

	SPLWorldLightCreateParam_Box createParam;
	createParam.pPos		= &vPos;
	createParam._pUserData	= &eType;
	createParam.uiRed		= clr.red;
	createParam.uiGreen		= clr.green;
	createParam.uiBlue		= clr.blue;
	createParam.vScale		= vScale;
	createParam.vRotate		= vRot;

	CNtlPLWorldLight_Box* pNtlPLWorldLight_Box = static_cast<CNtlPLWorldLight_Box*>(GetSceneManager()->CreateEntity(PLENTITY_WORLD_LIGHT, "NULL", &createParam));
	DBO_ASSERT(pNtlPLWorldLight_Box, "Entity create failed.");

	return pNtlPLWorldLight_Box;
}

RwBool CNtlWorldSectorInfo::WorldLightBoxSaveIntoFile(FILE* pFile, CNtlPLWorldLight_Box* pNtlPLWorldLightBox)
{
	RwV3d	vPos	= pNtlPLWorldLightBox->GetPosition();
	RwRGBA	clr		= pNtlPLWorldLightBox->GetColor();
	RwV3d	vRot	= *pNtlPLWorldLightBox->GetRotate();
	RwV3d	vScale	= *pNtlPLWorldLightBox->GetScaleVector();

	fwrite(&vPos, sizeof(RwV3d), 1, pFile);
	fwrite(&clr, sizeof(RwRGBA), 1, pFile);
	fwrite(&vRot, sizeof(RwV3d), 1, pFile);
	fwrite(&vScale, sizeof(RwV3d), 1, pFile);

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::WorldLightBoxSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	RwV3d vPos;
	CopyMemory(&vPos, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	if (pvOffset)
	{
		vPos.x += pvOffset->x;
		vPos.y += pvOffset->y;
		vPos.z += pvOffset->z;
	}
	::fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	RwRGBA	clr;
	CopyMemory(&clr, pFileMem, sizeof(RwRGBA));
	pFileMem += sizeof(RwRGBA);
	fwrite(&clr, sizeof(RwRGBA), 1, pFile);

	RwV3d	vRot;
	CopyMemory(&vRot, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	fwrite(&vRot, sizeof(RwV3d), 1, pFile);

	RwV3d	vScale;
	CopyMemory(&vScale, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	fwrite(&vScale, sizeof(RwV3d), 1, pFile);

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::WorldLightBoxSkipToFile(FILE* pFile)
{
	RwInt32 SkipRange = sizeof(RwV3d) + sizeof(RwRGBA) + sizeof(RwV3d) + sizeof(RwV3d);

	// skip set id
	::fseek(pFile, SkipRange, SEEK_CUR);

	return ::ftell(pFile);

}

BYTE* CNtlWorldSectorInfo::WorldLightBoxSkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(RwV3d);
	pFileMem += sizeof(RwRGBA);
	pFileMem += sizeof(RwV3d);
	pFileMem += sizeof(RwV3d);

	return pFileMem;
}

RwBool CNtlWorldSectorInfo::SectorOccluderLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLOccluder_Quad* pNtlPLOccluder = OccluderLoadFromFile(pFile);
		if (pNtlPLOccluder)
		{
			pNtlWorldSector->m_vecNtlPLEntityOccluder_Quad.push_back(pNtlPLOccluder);		
		}
	}
	pNtlWorldSector->CreateOccluderProxy(1.0f, 1);

#ifdef dNTL_WORLD_TOOL_MODE
	CNtlPLOccluder_Quad::LoadSwapFile(pNtlWorldSector->DatumPoint.x, pNtlWorldSector->DatumPoint.y, pNtlWorldSector->DatumPoint.z);
#endif

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorOccluderSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	iNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityOccluder_Quad.size());
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLOccluder_Quad* pNtlPLOccluder = static_cast<CNtlPLOccluder_Quad*>(pNtlWorldSector->m_vecNtlPLEntityOccluder_Quad.at(i));		

		OccluderSaveIntoFile(pFile, pNtlPLOccluder);		
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorOccluderSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField /* = -1 */, RwInt32 iIdxDstField /* = -1 */, CNtlWorldFieldManager* pWorldFieldMgr /* = NULL */)
{
	RwInt32 iNum = 0;

	RwV3d	vOffset;
	if (pWorldFieldMgr)
	{
		vOffset = pWorldFieldMgr->GetFieldRelativeCoord(iIdxSrcField, iIdxDstField);
	}
	else
	{
		vOffset.z = vOffset.x = 0.0f;
	}
	vOffset.y = 0.0f;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	::fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = OccluderSaveIntoFileFromFileMem(pFile, pFileMem, &vOffset);
	}

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorOccluderSkipToFile(FILE* pFile)
{
	RwInt32 iNum = 0; 

	fread(&iNum, sizeof(RwInt32), 1, pFile);

	for (RwInt32 i = 0; i < iNum; ++i)
	{
		OccluderSkipToFile(pFile);
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorOccluderSkipToFileMem(BYTE* pFileMem)
{
	RwInt32 iNum = 0;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = OccluderSkipToFileMem(pFileMem);
	}

	return pFileMem;
}

CNtlPLOccluder_Quad* CNtlWorldSectorInfo::OccluderLoadFromFile(FILE* pFile)
{
	RwV3d				vPos;	
	RwUInt32			eFuncFlag;
	EPLOccluderType		eOccluderType = EPLOCCLUDER_QUAD;


	fread(&eFuncFlag, sizeof(RwUInt32), 1, pFile);

#ifndef dNTL_WORLD_TOOL_MODE
	if (eFuncFlag & EPLOCCLUDER_FUNC_PVS)
	{
		fseek(pFile, sizeof(RwV3d), SEEK_CUR);
		fseek(pFile, sizeof(RwInt32), SEEK_CUR);

		if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
		{
			RwUInt32 uiLength = 0;
			fread(&uiLength, sizeof(RwUInt32), 1, pFile);
			fseek(pFile, sizeof(RwChar) * uiLength, SEEK_CUR);
		}
		
		fseek(pFile, sizeof(RwV3d), SEEK_CUR);
		fseek(pFile, sizeof(RwV3d), SEEK_CUR);
		fseek(pFile, sizeof(RwV3d), SEEK_CUR);
		fseek(pFile, sizeof(RwV3d), SEEK_CUR);

		return NULL;
	}
#endif
	fread(&vPos, sizeof(RwV3d), 1, pFile);

	SPLEntityCreateParam CreateParam;
	CreateParam.pPos		= &vPos;
	CreateParam._pUserData	= &eOccluderType;

	CNtlPLOccluder_Quad* pNtlPLOccluder = static_cast<CNtlPLOccluder_Quad*>(GetSceneManager()->CreateEntity(PLENTITY_OCCLUDER, "NULL", &CreateParam));
	DBO_ASSERT(pNtlPLOccluder, "Entity create failed.");

	pNtlPLOccluder->SetOccluderFuncFlag(eFuncFlag);

	pNtlPLOccluder->LoadFromFile(pFile);	

	return pNtlPLOccluder;
}

RwBool CNtlWorldSectorInfo::OccluderSaveIntoFile(FILE* pFile, CNtlPLOccluder_Quad* pNtlPLOccluder)
{
	RwUInt32 eFuncFlag = pNtlPLOccluder->GetOccluderFuncFlag();
	fwrite(&eFuncFlag, sizeof(RwUInt32), 1, pFile);

	RwV3d vPos = pNtlPLOccluder->GetPosition();
	fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	return pNtlPLOccluder->SaveIntoFile(pFile);
}

BYTE* CNtlWorldSectorInfo::OccluderSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	RwUInt32	eFuncFlag;
	CopyMemory(&eFuncFlag, pFileMem, sizeof(RwUInt32));
	pFileMem += sizeof(RwUInt32);
	::fwrite(&eFuncFlag, sizeof(RwUInt32), 1, pFile);

	RwV3d vPos;
	CopyMemory(&vPos, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	if (pvOffset)
	{
		vPos.x += pvOffset->x;
		vPos.y += pvOffset->y;
		vPos.z += pvOffset->z;
	}
	::fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	return CNtlPLOccluder_Quad::SaveIntoFileFromFileMem(pFile, pFileMem);
}

RwInt32 CNtlWorldSectorInfo::OccluderSkipToFile(FILE* pFile)
{
	::fseek(pFile, sizeof(RwUInt32), SEEK_CUR);

	::fseek(pFile, sizeof(RwV3d), SEEK_CUR);

	return CNtlPLOccluder_Quad::SkipToFile(pFile);
}

BYTE* CNtlWorldSectorInfo::OccluderSkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(RwUInt32);
	pFileMem += sizeof(RwV3d);

	return CNtlPLOccluder_Quad::SkipToFileMem(pFileMem);
}

RwBool CNtlWorldSectorInfo::SectorHeatHazeObjectLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLHeatHazeObject* pNtlPLHeatHazeObject = HeatHazeObjectLoadFromFile(pFile);
		if (pNtlPLHeatHazeObject)
		{
			pNtlWorldSector->m_vecNtlPLEntityHeatHaze.push_back(pNtlPLHeatHazeObject);
		}
	}

#ifdef dNTL_WORLD_TOOL_MODE
	CNtlPLHeatHazeObject::LoadSwapFile(pNtlWorldSector->DatumPoint.x, pNtlWorldSector->DatumPoint.y, pNtlWorldSector->DatumPoint.z);
#endif

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorHeatHazeObjectSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	iNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityHeatHaze.size());
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLHeatHazeObject* pNtlPLHeatHazeObject = static_cast<CNtlPLHeatHazeObject*>(pNtlWorldSector->m_vecNtlPLEntityHeatHaze.at(i));

		HeatHazeObjectSaveIntoFile(pFile, pNtlPLHeatHazeObject);
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorHeatHazeObjectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField /* = -1 */, RwInt32 iIdxDstField /* = -1 */, CNtlWorldFieldManager* pWorldFieldMgr /* = NULL */)
{
	RwInt32 iNum = 0;

	RwV3d	vOffset;
	if (pWorldFieldMgr)
	{
		vOffset = pWorldFieldMgr->GetFieldRelativeCoord(iIdxSrcField, iIdxDstField);
	}
	else
	{
		vOffset.z = vOffset.x = 0.0f;
	}
	vOffset.y = 0.0f;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	::fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = HeatHazeObjectSaveIntoFileFromFileMem(pFile, pFileMem, &vOffset);
	}

	return pFileMem;
}


RwInt32 CNtlWorldSectorInfo::SectorHeatHazeObjectSkipToFile(FILE* pFile)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);

	for (RwInt32 i = 0; i < iNum; ++i)
	{
		HeatHazeObjectSkipToFile(pFile);
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorHeatHazeObjectSkipToFileMem(BYTE* pFileMem)
{
	RwInt32 iNum = 0;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = HeatHazeObjectSkipToFileMem(pFileMem);
	}

	return pFileMem;
}

CNtlPLHeatHazeObject* CNtlWorldSectorInfo::HeatHazeObjectLoadFromFile(FILE* pFile)
{
	SPLEntityCreateParam sCreateParam;
	sCreateParam.pPos		= NULL;

	CNtlPLHeatHazeObject* pNtlPLHeatHazeObject = static_cast<CNtlPLHeatHazeObject*>(GetSceneManager()->CreateEntity(PLENTITY_HEATHAZE, "NULL", &sCreateParam));
	DBO_ASSERT(pNtlPLHeatHazeObject, "Entity create failed.");

	pNtlPLHeatHazeObject->LoadFromFile(pFile);

	return pNtlPLHeatHazeObject;
}

RwBool CNtlWorldSectorInfo::HeatHazeObjectSaveIntoFile(FILE* pFile, CNtlPLHeatHazeObject* pNtlPLHeatHazeObject)
{
	return pNtlPLHeatHazeObject->SaveIntoFile(pFile);
}

BYTE* CNtlWorldSectorInfo::HeatHazeObjectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	return CNtlPLHeatHazeObject::SaveIntoFileFromFileMem(pFile, pFileMem, pvOffset);
}

RwInt32 CNtlWorldSectorInfo::HeatHazeObjectSkipToFile(FILE* pFile)
{
	return CNtlPLHeatHazeObject::SkipToFile(pFile);
}

BYTE* CNtlWorldSectorInfo::HeatHazeObjectSkipToFileMem(BYTE* pFileMem)
{
	return CNtlPLHeatHazeObject::SkipToFileMem(pFileMem);
}

RwBool CNtlWorldSectorInfo::SectorLightObjectLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLLightObject* pNtlPLLightObject = LightObjectLoadFromFile(pFile);
		if (pNtlPLLightObject)
		{
			pNtlWorldSector->m_vecNtlPLEntityLightObject.push_back(pNtlPLLightObject);
		}
	}

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorLightObjectSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	iNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityLightObject.size());
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLLightObject* pNtlPLLightObject = static_cast<CNtlPLLightObject*>(pNtlWorldSector->m_vecNtlPLEntityLightObject.at(i));

		LightObjectSaveIntoFile(pFile, pNtlPLLightObject);
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorLightObjectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField /* = -1 */, RwInt32 iIdxDstField /* = -1 */, CNtlWorldFieldManager* pWorldFieldMgr /* = NULL */)
{
	RwInt32 iNum = 0;

	RwV3d	vOffset;
	if (pWorldFieldMgr)
	{
		vOffset = pWorldFieldMgr->GetFieldRelativeCoord(iIdxSrcField, iIdxDstField);
	}
	else
	{
		vOffset.z = vOffset.x = 0.0f;
	}
	vOffset.y = 0.0f;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	::fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = LightObjectSaveIntoFileFromFileMem(pFile, pFileMem, &vOffset);
	}

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorLightObjectSkipToFile(FILE* pFile)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);

	for (RwInt32 i = 0; i < iNum; ++i)
	{
		LightObjectSkipToFile(pFile);
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorLightObjectSkipToFileMem(BYTE* pFileMem)
{
	RwInt32 iNum = 0;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = LightObjectSkipToFileMem(pFileMem);
	}

	return pFileMem;
}

CNtlPLLightObject* CNtlWorldSectorInfo::LightObjectLoadFromFile(FILE* pFile)
{
	SPLLightObjectCreateParam sCreateParam;

	fread(&sCreateParam.rpLightType, sizeof(RpLightType), 1, pFile);
	fread(&sCreateParam.bRealLight, sizeof(RwBool), 1, pFile);

	RwV3d vPos;
	fread(&vPos, sizeof(RwV3d), 1, pFile);
	sCreateParam.pPos = &vPos;

	fread(&sCreateParam.vRotate, sizeof(RwV3d), 1, pFile);

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwRGBA rgba;
		fread(&rgba, sizeof(RwRGBA), 1, pFile);
		RwRGBARealFromRwRGBA(&sCreateParam.rgbafColor, &rgba);
	}

	fread(&sCreateParam.fRadius, sizeof(RwReal), 1, pFile);
	fread(&sCreateParam.fAngle, sizeof(RwReal), 1, pFile);

	fread(&sCreateParam.bDecal, sizeof(RwBool), 1, pFile);
	fread(&sCreateParam.bWorldLight, sizeof(RwBool), 1, pFile);

	CNtlPLLightObject* pNtlPLLightObject = static_cast<CNtlPLLightObject*>(GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, "NULL", &sCreateParam));
	DBO_ASSERT(pNtlPLLightObject, "Entity create failed.");

	return pNtlPLLightObject;	
}

RwBool CNtlWorldSectorInfo::LightObjectSaveIntoFile(FILE* pFile, CNtlPLLightObject* pNtlPLLightObject)
{
	RpLightType eType = pNtlPLLightObject->GetLightType();
	fwrite(&eType, sizeof(RpLightType), 1, pFile);

	RwBool bIsRealLight = pNtlPLLightObject->IsRealLight();
	fwrite(&bIsRealLight, sizeof(RwBool), 1, pFile);

	RwV3d vPos = pNtlPLLightObject->GetPosition();;
	fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	RwV3d vRot = *pNtlPLLightObject->GetRotate();
	fwrite(&vRot, sizeof(RwV3d), 1, pFile);

	RwRGBAReal clrf = pNtlPLLightObject->GetColor();
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
	{
		fwrite(&clrf, sizeof(RwRGBAReal), 1, pFile);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
	{
		RwRGBA rgba;
		RwRGBAFromRwRGBAReal(&rgba, &clrf);
		fwrite(&rgba, sizeof(RwRGBA), 1, pFile);
	}		 

	RwReal fRadius = pNtlPLLightObject->GetRadius();
	fwrite(&fRadius, sizeof(RwReal), 1, pFile);

	RwReal fAngle = pNtlPLLightObject->GetAngle();
	fwrite(&fAngle, sizeof(RwReal), 1, pFile);

	RwBool bIsEnabledDecal = pNtlPLLightObject->IsEnabledDecal();
	fwrite(&bIsEnabledDecal, sizeof(RwBool), 1, pFile);

	RwBool bIsEnabledWorldLight = pNtlPLLightObject->IsEnabledWorldLight();
	fwrite(&bIsEnabledWorldLight, sizeof(RwBool), 1, pFile);

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::LightObjectSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	RpLightType eType;
	CopyMemory(&eType, pFileMem, sizeof(RpLightType));
	pFileMem += sizeof(RpLightType);
	::fwrite(&eType, sizeof(RpLightType), 1, pFile);

	RwBool bIsRealLight;
	CopyMemory(&bIsRealLight, pFileMem, sizeof(RwBool));
	pFileMem += sizeof(RwBool);
	::fwrite(&bIsRealLight, sizeof(RwBool), 1, pFile);

	RwV3d vPos;
	CopyMemory(&vPos, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	if (pvOffset)
	{
		vPos.x += pvOffset->x;
		vPos.y += pvOffset->y;
		vPos.z += pvOffset->z;
	}
	::fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	RwV3d vRot;
	CopyMemory(&vRot, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	::fwrite(&vRot, sizeof(RwV3d), 1, pFile);

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		RwRGBAReal clrf;
		CopyMemory(&clrf, pFileMem, sizeof(RwRGBAReal));
		pFileMem += sizeof(RwRGBAReal);
		::fwrite(&clrf, sizeof(RwRGBAReal), 1, pFile);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwRGBA rgba;
		CopyMemory(&rgba, pFileMem, sizeof(RwRGBA));
		pFileMem += sizeof(RwRGBA);
		::fwrite(&rgba, sizeof(RwRGBA), 1, pFile);
	}

	RwReal fRadius;
	CopyMemory(&fRadius, pFileMem, sizeof(RwReal));
	pFileMem += sizeof(RwReal);
	::fwrite(&fRadius, sizeof(RwReal), 1, pFile);

	RwReal fAngle;
	CopyMemory(&fAngle, pFileMem, sizeof(RwReal));
	pFileMem += sizeof(RwReal);
	::fwrite(&fAngle, sizeof(RwReal), 1, pFile);

	RwBool bIsEnabledDecal;
	CopyMemory(&bIsEnabledDecal, pFileMem, sizeof(RwBool));
	pFileMem += sizeof(RwBool);
	::fwrite(&bIsEnabledDecal, sizeof(RwBool), 1, pFile);

	RwBool bIsEnabledWorldLight;
	CopyMemory(&bIsEnabledWorldLight, pFileMem, sizeof(RwBool));
	pFileMem += sizeof(RwBool);
	::fwrite(&bIsEnabledWorldLight, sizeof(RwBool), 1, pFile);

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::LightObjectSkipToFile(FILE* pFile)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwInt32 SkipRange = sizeof(RpLightType) + sizeof(RwBool) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwRGBA) + sizeof(RwReal) +
			sizeof(RwReal) + sizeof(RwBool) + sizeof(RwBool);

		// skip set id
		::fseek(pFile, SkipRange, SEEK_CUR);
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::LightObjectSkipToFileMem(BYTE* pFileMem)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		pFileMem += sizeof(RpLightType) + sizeof(RwBool) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwRGBAReal) + sizeof(RwReal) +
			sizeof(RwReal) + sizeof(RwBool) + sizeof(RwBool);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		pFileMem += sizeof(RpLightType) + sizeof(RwBool) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwRGBA) + sizeof(RwReal) +
			sizeof(RwReal) + sizeof(RwBool) + sizeof(RwBool);
	}	

	return pFileMem;
}

RwBool CNtlWorldSectorInfo::SectorDecalVolumeLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLDecalVolume* pNtlPLDecalVolume = DecalVolumeLoadFromFile(pFile);
		if (pNtlPLDecalVolume)
		{
			pNtlWorldSector->m_vecNtlPLEntityDecalVolume.push_back(pNtlPLDecalVolume);
		}
	}

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorDecalVolumeSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	iNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityDecalVolume.size());
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLDecalVolume* pNtlPLDecalVolume = static_cast<CNtlPLDecalVolume*>(pNtlWorldSector->m_vecNtlPLEntityDecalVolume.at(i));

		DecalVolumeSaveIntoFile(pFile, pNtlPLDecalVolume);
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorDecalVolumeSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField /* = -1 */, RwInt32 iIdxDstField /* = -1 */, CNtlWorldFieldManager* pWorldFieldMgr /* = NULL */)
{
	RwInt32 iNum = 0;

	RwV3d	vOffset;
	if (pWorldFieldMgr)
	{
		vOffset = pWorldFieldMgr->GetFieldRelativeCoord(iIdxSrcField, iIdxDstField);
	}
	else
	{
		vOffset.z = vOffset.x = 0.0f;
	}
	vOffset.y = 0.0f;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	::fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = DecalVolumeSaveIntoFileFromFileMem(pFile, pFileMem, &vOffset);
	}

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorDecalVolumeSkipToFile(FILE* pFile)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		DecalVolumeSkipToFile(pFile);
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorDecalVolumeSkipToFileMem(BYTE* pFileMem)
{
	RwInt32 iNum = 0;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = DecalVolumeSkipToFileMem(pFileMem);
	}

	return pFileMem;
}

CNtlPLDecalVolume* CNtlWorldSectorInfo::DecalVolumeLoadFromFile(FILE* pFile)
{
	SPLDecalVolumeCreateParam sCreateParam;

	 if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		fread(&uiLength, sizeof(RwUInt32), 1, pFile);
		fread(sCreateParam.acTextureName, sizeof(RwChar) * uiLength, 1, pFile);
		sCreateParam.acTextureName[uiLength] = '\0';
	}

	RwV3d vPos;
	fread(&vPos, sizeof(RwV3d), 1, pFile);
	sCreateParam.pPos = &vPos;

	fread(&sCreateParam.vRotation, sizeof(RwV3d), 1, pFile);
	fread(&sCreateParam.vScale, sizeof(RwV3d), 1, pFile);
	fread(&sCreateParam.clrDiffuse, sizeof(RwRGBA), 1, pFile);
	fread(&sCreateParam.fAppendOffset, sizeof(RwReal), 1, pFile);
	fread(&sCreateParam.fVisibleDist, sizeof(RwReal), 1, pFile);
	fread(&sCreateParam.uiDecalVolumeFlags, sizeof(RwUInt32), 1, pFile);

	CNtlPLDecalVolume* pNtlPLDecalVolume = static_cast<CNtlPLDecalVolume*>(GetSceneManager()->CreateEntity(PLENTITY_DECAL_VOLUME, "NULL", &sCreateParam));
	DBO_ASSERT(pNtlPLDecalVolume, "Entity create failed.");

	return pNtlPLDecalVolume;
}

RwBool CNtlWorldSectorInfo::DecalVolumeSaveIntoFile(FILE* pFile, CNtlPLDecalVolume* pNtlPLDecalVolume)
{
	RwChar acTextureName[128];
	strcpy_s(acTextureName, 128, pNtlPLDecalVolume->GetTextureName());

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
	{
		fwrite(acTextureName, sizeof(RwChar) * 128, 1, pFile);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = strlen(acTextureName);
		
		fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		fwrite(acTextureName, sizeof(RwChar) * uiLength, 1, pFile);
	}

	RwV3d vPos = pNtlPLDecalVolume->GetPosition();
	fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	RwV3d vRot = *pNtlPLDecalVolume->GetRotate();
	fwrite(&vRot, sizeof(RwV3d), 1, pFile);

	RwV3d vScale = *pNtlPLDecalVolume->GetScale();
	fwrite(&vScale, sizeof(RwV3d), 1, pFile);

	RwRGBA clr = *pNtlPLDecalVolume->GetColor();
	fwrite(&clr, sizeof(RwRGBA), 1, pFile);

	RwReal fAppendOffset = pNtlPLDecalVolume->GetAppendOffset();
	fwrite(&fAppendOffset, sizeof(RwReal), 1, pFile);

	RwReal fVisibilityDistance = pNtlPLDecalVolume->GetVisibleDist();
	fwrite(&fVisibilityDistance, sizeof(RwReal), 1, pFile);

	RwUInt32 uiDecalVolumeFlags = pNtlPLDecalVolume->GetDecalVolumeFlags();
	fwrite(&uiDecalVolumeFlags, sizeof(RwUInt32), 1, pFile);

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::DecalVolumeSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	RwChar acTextureName[128];

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		CopyMemory(acTextureName, pFileMem, sizeof(RwChar) * 128);
		pFileMem += sizeof(RwChar) * 128;
		::fwrite(acTextureName, sizeof(RwChar) * 128, 1, pFile);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		::fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);
		
		CopyMemory(acTextureName, pFileMem, sizeof(RwChar) * uiLength);
		acTextureName[uiLength] = '\0';
		pFileMem += sizeof(RwChar) * uiLength;
		::fwrite(acTextureName, sizeof(RwChar) * uiLength, 1, pFile);
	}

	RwV3d vPos;
	CopyMemory(&vPos, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	if (pvOffset)
	{
		vPos.x += pvOffset->x;
		vPos.y += pvOffset->y;
		vPos.z += pvOffset->z;
	}
	::fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	RwV3d vRot;
	CopyMemory(&vRot, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	::fwrite(&vRot, sizeof(RwV3d), 1, pFile);

	RwV3d vScale;
	CopyMemory(&vScale, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	::fwrite(&vScale, sizeof(RwV3d), 1, pFile);

	RwRGBA clr;
	CopyMemory(&clr, pFileMem, sizeof(RwRGBA));
	pFileMem += sizeof(RwRGBA);
	::fwrite(&clr, sizeof(RwRGBA), 1, pFile);

	RwReal fAppendOffset;
	CopyMemory(&fAppendOffset, pFileMem, sizeof(RwReal));
	pFileMem += sizeof(RwReal);
	::fwrite(&fAppendOffset, sizeof(RwReal), 1, pFile);

	RwReal fVisibilityDistance;
	CopyMemory(&fVisibilityDistance, pFileMem, sizeof(RwReal));
	pFileMem += sizeof(RwReal);
	::fwrite(&fVisibilityDistance, sizeof(RwReal), 1, pFile);

	RwUInt32 uiDecalVolumeFlags;
	CopyMemory(&uiDecalVolumeFlags, pFileMem, sizeof(RwUInt32));
	pFileMem += sizeof(RwUInt32);
	::fwrite(&uiDecalVolumeFlags, sizeof(RwUInt32), 1, pFile);

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::DecalVolumeSkipToFile(FILE* pFile)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION_OLD))
	{
	RwInt32 SkipRange = (sizeof(RwChar) * 128) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) +
		sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwUInt32);

	// skip set id
	::fseek(pFile, SkipRange, SEEK_CUR);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		fread(&uiLength, sizeof(RwUInt32), 1, pFile);
		
		RwInt32 SkipRange = (sizeof(RwChar) * uiLength) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) +
			sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwUInt32);

		// skip set id
		::fseek(pFile, SkipRange, SEEK_CUR);
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::DecalVolumeSkipToFileMem(BYTE* pFileMem)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		pFileMem += (sizeof(RwChar) * 128) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) +
			sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwUInt32);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		pFileMem += sizeof(RwUInt32);

		pFileMem += (sizeof(RwChar) * uiLength) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) +
			sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwUInt32);
	}	

	return pFileMem;
}

RwBool CNtlWorldSectorInfo::SectorGamePropertyLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLGameProperty* pNtlPLGameProperty = GamePropertyLoadFromFile(pFile);
		if (pNtlPLGameProperty)
		{
			pNtlWorldSector->m_vecNtlPLEntityGameProperty.push_back(pNtlPLGameProperty);
		}
	}

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorGamePropertySaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	iNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityGameProperty.size());
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLGameProperty* pNtlPLGameProperty = static_cast<CNtlPLGameProperty*>(pNtlWorldSector->m_vecNtlPLEntityGameProperty.at(i));

		GamePropertySaveIntoFile(pFile, pNtlPLGameProperty);
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorGamePropertySaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField /* = -1 */, RwInt32 iIdxDstField /* = -1 */, CNtlWorldFieldManager* pWorldFieldMgr /* = NULL */)
{
	RwInt32 iNum = 0;

	RwV3d	vOffset;
	if (pWorldFieldMgr)
	{
		vOffset = pWorldFieldMgr->GetFieldRelativeCoord(iIdxSrcField, iIdxDstField);
	}
	else
	{
		vOffset.z = vOffset.x = 0.0f;
	}
	vOffset.y = 0.0f;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	::fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = GamePropertySaveIntoFileFromFileMem(pFile, pFileMem, &vOffset);
	}

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorGamePropertySkipToFile(FILE* pFile)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		GamePropertySkipToFile(pFile);
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorGamePropertySkipToFileMem(BYTE* pFileMem)
{
	RwInt32 iNum = 0;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = GamePropertySkipToFileMem(pFileMem);
	}

	return pFileMem;
}

CNtlPLGameProperty* CNtlWorldSectorInfo::GamePropertyLoadFromFile(FILE* pFile)
{
	SPLGamePropertyCreateParam sCreateParam;

	fread(&sCreateParam.eShapeType, sizeof(RwUInt32), 1, pFile);

	RwV3d vPos;
	fread(&vPos, sizeof(RwV3d), 1, pFile);
	sCreateParam.pPos = &vPos;

	fread(&sCreateParam.vSize, sizeof(RwV3d), 1, pFile);
	fread(&sCreateParam.uiProperty, sizeof(RwUInt32), 1, pFile);

	CNtlPLGameProperty* pNtlPLGameProperty = static_cast<CNtlPLGameProperty*>(GetSceneManager()->CreateEntity(PLENTITY_GAME_PROPERTY, "NULL", &sCreateParam));
	DBO_ASSERT(pNtlPLGameProperty, "Entity create failed.");

	return pNtlPLGameProperty;
}

RwBool CNtlWorldSectorInfo::GamePropertySaveIntoFile(FILE* pFile, CNtlPLGameProperty* pNtlPLGameProperty)
{
	RwUInt32 uiShapeType = pNtlPLGameProperty->GetShapeType();
	fwrite(&uiShapeType, sizeof(RwUInt32), 1, pFile);

	RwV3d vPos = pNtlPLGameProperty->GetPosition();
	fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	RwV3d vSize = pNtlPLGameProperty->GetSize();
	fwrite(&vSize, sizeof(RwV3d), 1, pFile);

	RwUInt32 uiGameProperty = pNtlPLGameProperty->GetGameProperty();
	fwrite(&uiGameProperty, sizeof(RwUInt32), 1, pFile);

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::GamePropertySaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	RwUInt32 eShapeType;
	CopyMemory(&eShapeType, pFileMem, sizeof(RwUInt32));
	pFileMem += sizeof(RwUInt32);
	::fwrite(&eShapeType, sizeof(RwUInt32), 1, pFile);

	RwV3d vPos;
	CopyMemory(&vPos, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	if (pvOffset)
	{
		vPos.x += pvOffset->x;
		vPos.y += pvOffset->y;
		vPos.z += pvOffset->z;
	}
	::fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	RwV3d vSize;
	CopyMemory(&vSize, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	::fwrite(&vSize, sizeof(RwV3d), 1, pFile);

	RwUInt32 uiProperty;
	CopyMemory(&uiProperty, pFileMem, sizeof(RwUInt32));
	pFileMem += sizeof(RwUInt32);
	::fwrite(&uiProperty, sizeof(RwUInt32), 1, pFile);

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::GamePropertySkipToFile(FILE* pFile)
{
	RwInt32 SkipRange = sizeof(RwUInt32) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwUInt32);

	::fseek(pFile, SkipRange, SEEK_CUR);

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::GamePropertySkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(RwUInt32) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwUInt32);

	return pFileMem;
}

RwBool CNtlWorldSectorInfo::SectorDojoLoadFromFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLDojo* pNtlPLDojo = DojoLoadFromFile(pFile);
		if (pNtlPLDojo)
		{
			pNtlWorldSector->m_vecNtlPLEntityDojo.push_back(pNtlPLDojo);
		}
	}

	return TRUE;
}

RwBool CNtlWorldSectorInfo::SectorDojoSaveIntoFile(FILE* pFile, CNtlWorldSector* pNtlWorldSector)
{
	RwInt32 iNum = 0;

	iNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityDojo.size());
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		CNtlPLDojo* pNtlPLDojo = static_cast<CNtlPLDojo*>(pNtlWorldSector->m_vecNtlPLEntityDojo.at(i));

		DojoSaveIntoFile(pFile, pNtlPLDojo);
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::SectorDojoSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwInt32 iIdxSrcField /* = -1 */, RwInt32 iIdxDstField /* = -1 */, CNtlWorldFieldManager* pWorldFieldMgr /* = NULL */)
{
	RwInt32 iNum = 0;

	RwV3d	vOffset;
	if (pWorldFieldMgr)
	{
		vOffset = pWorldFieldMgr->GetFieldRelativeCoord(iIdxSrcField, iIdxDstField);
	}
	else
	{
		vOffset.z = vOffset.x = 0.0f;
	}
	vOffset.y = 0.0f;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	::fwrite(&iNum, sizeof(RwInt32), 1, pFile);
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = DojoSaveIntoFileFromFileMem(pFile, pFileMem, &vOffset);
	}

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::SectorDojoSkipToFile(FILE* pFile)
{
	RwInt32 iNum = 0;

	fread(&iNum, sizeof(RwInt32), 1, pFile);

	for (RwInt32 i = 0; i < iNum; ++i)
	{
		DojoSkipToFile(pFile);
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::SectorDojoSkipToFileMem(BYTE* pFileMem)
{
	RwInt32 iNum = 0;

	CopyMemory(&iNum, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	for (RwInt32 i = 0; i < iNum; ++i)
	{
		pFileMem = DojoSkipToFileMem(pFileMem);
	}

	return pFileMem;
}

CNtlPLDojo* CNtlWorldSectorInfo::DojoLoadFromFile(FILE* pFile)
{
	SPLDojoCreateParam sCreateParam;

	RwV3d vScale;
	RwV3d vRot;
	RwV3d vPos;

	fread(&vScale, sizeof(RwV3d), 1, pFile);
	fread(&vRot, sizeof(RwV3d), 1, pFile);
	fread(&vPos, sizeof(RwV3d), 1, pFile);

	sCreateParam.pPos = &vPos;

	fread(&sCreateParam.iDojoID, sizeof(RwInt32), 1, pFile);
	fread(&sCreateParam.fFadeInTime, sizeof(RwReal), 1, pFile);
	fread(&sCreateParam.fFadeOutTime, sizeof(RwReal), 1, pFile);

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
		{
			RwUInt32 uiLength = 0;

			fread(&uiLength, sizeof(RwUInt32), 1, pFile);
			fread(sCreateParam.acObjectName[i], sizeof(RwChar) * uiLength, 1, pFile);
			sCreateParam.acObjectName[i][uiLength] = '\0';

			fread(&uiLength, sizeof(RwUInt32), 1, pFile);
			fread(sCreateParam.acEffectNameUp[i], sizeof(RwChar) * uiLength, 1, pFile);
			sCreateParam.acEffectNameUp[i][uiLength] = '\0';

			fread(&uiLength, sizeof(RwUInt32), 1, pFile);
			fread(sCreateParam.acEffectNameDown[i], sizeof(RwChar) * uiLength, 1, pFile);
			sCreateParam.acEffectNameDown[i][uiLength] = '\0';
		}
	}

	CNtlPLDojo* pNtlPLDojo = static_cast<CNtlPLDojo*>(GetSceneManager()->CreateEntity(PLENTITY_DOJO, "NULL", &sCreateParam));
	DBO_ASSERT(pNtlPLDojo, "Entity create failed.");

	pNtlPLDojo->SetPosition(&vPos);
	pNtlPLDojo->SetRotate(vRot.x, vRot.y, vRot.z);
	pNtlPLDojo->SetScaleVector(&vScale);

	return pNtlPLDojo;
}

RwBool CNtlWorldSectorInfo::DojoSaveIntoFile(FILE* pFile, CNtlPLDojo* pNtlPLDojo)
{
	RwV3d vScale	= *pNtlPLDojo->GetScaleVector();
	RwV3d vRot		= *pNtlPLDojo->GetRotate();
	RwV3d vPos		= pNtlPLDojo->GetPosition();

	fwrite(&vScale, sizeof(RwV3d), 1, pFile);
	fwrite(&vRot, sizeof(RwV3d), 1, pFile);
	fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	RwInt32 iDojoID		= pNtlPLDojo->GetDojoID();
	RwReal	fFadeInTime	= pNtlPLDojo->GetFadeInTime();
	RwReal	fFadeOutTime= pNtlPLDojo->GetFadeOutTime();

	fwrite(&iDojoID, sizeof(RwInt32), 1, pFile);
	fwrite(&fFadeInTime, sizeof(RwReal), 1, pFile);
	fwrite(&fFadeOutTime, sizeof(RwReal), 1, pFile);

	RwChar acTemp[dPL_DOJO_RES_NAME_LEN];

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
	{
		for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
		{
			strcpy_s(acTemp, dPL_DOJO_RES_NAME_LEN, pNtlPLDojo->GetResObjectName(i));
			fwrite(acTemp, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);
			strcpy_s(acTemp, dPL_DOJO_RES_NAME_LEN, pNtlPLDojo->GetResEffectNameUp(i));
			fwrite(acTemp, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);
			strcpy_s(acTemp, dPL_DOJO_RES_NAME_LEN, pNtlPLDojo->GetResEffectNameDown(i));
			fwrite(acTemp, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);
		}
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
	{
		for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
		{
			RwUInt32 uiLength = 0;

			strcpy_s(acTemp, dPL_DOJO_RES_NAME_LEN, pNtlPLDojo->GetResObjectName(i));			
			uiLength = strlen(acTemp);
			fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
			fwrite(acTemp, sizeof(RwChar) * uiLength, 1, pFile);

			strcpy_s(acTemp, dPL_DOJO_RES_NAME_LEN, pNtlPLDojo->GetResEffectNameUp(i));
			uiLength = strlen(acTemp);
			fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
			fwrite(acTemp, sizeof(RwChar) * uiLength, 1, pFile);

			strcpy_s(acTemp, dPL_DOJO_RES_NAME_LEN, pNtlPLDojo->GetResEffectNameDown(i));
			uiLength = strlen(acTemp);
			fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
			fwrite(acTemp, sizeof(RwChar) * uiLength, 1, pFile);
		}
	}

	return TRUE;
}

BYTE* CNtlWorldSectorInfo::DojoSaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	RwV3d vScale;
	CopyMemory(&vScale, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	::fwrite(&vScale, sizeof(RwV3d), 1, pFile);

	RwV3d vRot;
	CopyMemory(&vRot, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	::fwrite(&vRot, sizeof(RwV3d), 1, pFile);

	RwV3d vPos;
	CopyMemory(&vPos, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	if (pvOffset)
	{
		vPos.x += pvOffset->x;
		vPos.y += pvOffset->y;
		vPos.z += pvOffset->z;
	}
	::fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	RwInt32 iDojoID;
	CopyMemory(&iDojoID, pFileMem, sizeof(RwInt32));
	pFileMem += sizeof(RwInt32);
	::fwrite(&iDojoID, sizeof(RwInt32), 1, pFile);

	RwReal fFade;
	CopyMemory(&fFade, pFileMem, sizeof(RwReal));
	pFileMem += sizeof(RwReal);
	::fwrite(&fFade, sizeof(RwReal), 1, pFile);

	CopyMemory(&fFade, pFileMem, sizeof(RwReal));
	pFileMem += sizeof(RwReal);
	::fwrite(&fFade, sizeof(RwReal), 1, pFile);

	RwChar acTemp[dPL_DOJO_RES_NAME_LEN];

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
		{
			// object name
			CopyMemory(acTemp, pFileMem, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN);
			pFileMem += sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN;
			::fwrite(acTemp, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);

			// upgrade effect name
			CopyMemory(acTemp, pFileMem, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN);
			pFileMem += sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN;
			::fwrite(acTemp, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);

			// downgrade effect name
			CopyMemory(acTemp, pFileMem, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN);
			pFileMem += sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN;
			::fwrite(acTemp, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);
		}
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
		{
			RwUInt32 uiLength = 0;

			// object name
			CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
			fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
			pFileMem += sizeof(RwUInt32);
			
			CopyMemory(acTemp, pFileMem, sizeof(RwChar) * uiLength);
			acTemp[uiLength] = '\0';			
			::fwrite(acTemp, sizeof(RwChar) * uiLength, 1, pFile);
			pFileMem += sizeof(RwChar) * uiLength;

			// upgrade effect name
			CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
			fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
			pFileMem += sizeof(RwUInt32);

			CopyMemory(acTemp, pFileMem, sizeof(RwChar) * uiLength);
			acTemp[uiLength] = '\0';			
			::fwrite(acTemp, sizeof(RwChar) * uiLength, 1, pFile);
			pFileMem += sizeof(RwChar) * uiLength;

			// downgrade effect name
			CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
			fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
			pFileMem += sizeof(RwUInt32);

			CopyMemory(acTemp, pFileMem, sizeof(RwChar) * uiLength);
			acTemp[uiLength] = '\0';			
			::fwrite(acTemp, sizeof(RwChar) * uiLength, 1, pFile);
			pFileMem += sizeof(RwChar) * uiLength;
		}
	}

	return pFileMem;
}

RwInt32 CNtlWorldSectorInfo::DojoSkipToFile(FILE* pFile)
{
	RwInt32 SkipRange = sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwInt32) + sizeof(RwReal) + sizeof(RwReal);

	::fseek(pFile, SkipRange, SEEK_CUR);

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
		{
			RwUInt32 uiLength = 0;

			fread(&uiLength, sizeof(RwUInt32), 1, pFile);
			fseek(pFile, sizeof(RwChar) * uiLength, SEEK_CUR);

			fread(&uiLength, sizeof(RwUInt32), 1, pFile);
			fseek(pFile, sizeof(RwChar) * uiLength, SEEK_CUR);

			fread(&uiLength, sizeof(RwUInt32), 1, pFile);
			fseek(pFile, sizeof(RwChar) * uiLength, SEEK_CUR);
		}
	}

	return ::ftell(pFile);
}

BYTE* CNtlWorldSectorInfo::DojoSkipToFileMem(BYTE* pFileMem)
{
	pFileMem += sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwInt32) + sizeof(RwReal) + sizeof(RwReal);
	
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		for (int j = 0; j < dPL_DOJO_MAX_LEVEL; ++j)
		{
			pFileMem += sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN;
			pFileMem += sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN;
			pFileMem += sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN;
		}
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
		{
			RwUInt32 uiLength = 0;

			// object name
			CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
			pFileMem += sizeof(RwUInt32);
			pFileMem += sizeof(RwChar) * uiLength;

			// upgrade effect name
			CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
			pFileMem += sizeof(RwUInt32);
			pFileMem += sizeof(RwChar) * uiLength;

			// downgrade effect name
			CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
			pFileMem += sizeof(RwUInt32);
			pFileMem += sizeof(RwChar) * uiLength;
		}
	}

	return pFileMem;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

CNtlWorldFileAccessor		g_FA;
CNtlWorldFileMemAccessor	g_FMA;

RwReal _RpNtlWorldHFLoadFromFile(FILE* pFile, RwReal PosX, RwReal PosY)
{
	NTL_FUNCTION("_RpNtlWorldHFLoadFromFile");

	RwUInt8 Value[3];
	RwReal Result = 0;
	RwInt32 WidthNum, DepthNum;
	RwInt32 NPosX = static_cast<RwInt32>(PosX);
	RwInt32 NPosZ = static_cast<RwInt32>(PosY);
	RwInt32 XBasedOn0 = NPosX + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwInt32 ZBasedOn0 = NPosZ + dGET_WORLD_PARAM()->WorldSizeHalf;
	WidthNum = dGET_WORLD_PARAM()->WorldHeightfieldNum - XBasedOn0 / dGET_WORLD_PARAM()->WorldSectorTileSize;
	DepthNum = dGET_WORLD_PARAM()->WorldHeightfieldNum - ZBasedOn0 / dGET_WORLD_PARAM()->WorldSectorTileSize;

	fseek(pFile, (sizeof(RwUInt8) * 3) * (WidthNum + DepthNum * (dGET_WORLD_PARAM()->WorldHeightfieldNum + 1)), SEEK_SET);
	fread(&Value, sizeof(RwUInt8), 3, pFile);

	NTL_RETURN((float)(Value[0] + Value[1] + Value[2]) / 3.0f  - 128.f);
}

void _RpNtlWorldSectorMaterialExtensionRead(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RpMaterial *pMaterial, FILE *pFile)
{
	NTL_FUNCTION("_RpNtlWorldSectorMaterialExtensionRead");

	if(pFile)
	{
		RwBool BoolFlag;
		BYTE *pAlphaValueArray = NTL_NEW BYTE [dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum];

		RwChar ReadBuff[rwTEXTUREBASENAMELENGTH] = {0,};
		::fread(ReadBuff, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
		pNtlWorldSector->m_pTextureBase	= CNtlPLResourceManager::GetInstance()->LoadTexture(ReadBuff, dGET_WORLD_PARAM()->CurDefaultTexPath); 
		DBO_ASSERT(pNtlWorldSector->m_pTextureBase, "Texture load failed.");

#ifdef dNTL_WORLD_COMBINE
		/*if (CNtlPLGlobal::m_bCombine)
		{
		RwInt32 WidthNum, DepthNum;
		RwReal XBasedOn0 = pNtlWorldSector->DatumPoint.x + dGET_WORLD_PARAM()->WorldSizeHalf;
		RwReal ZBasedOn0 = pNtlWorldSector->DatumPoint.z + dGET_WORLD_PARAM()->WorldSizeHalf;
		WidthNum = (int)(XBasedOn0 / (float)dGET_WORLD_PARAM()->WorldSectorSize);
		DepthNum = (int)(ZBasedOn0 / (float)dGET_WORLD_PARAM()->WorldSectorSize); 

		RwInt32 iFieldIdx = WidthNum + (DepthNum * dGET_WORLD_PARAM()->WorldSectorNum);

		RwChar	acCombineTex[NTL_MAX_DIR_PATH];
		RwChar	acCombineTexPath[NTL_MAX_DIR_PATH];

		sprintf_s(acCombineTexPath, NTL_MAX_DIR_PATH, "%s\\cmap\\", dGET_WORLD_PARAM()->WorldProjectFolderName);
		sprintf_s(acCombineTex, NTL_MAX_DIR_PATH, "%.8d.png", iFieldIdx);

		pNtlWorldSector->m_pTextureCombine	= CNtlPLResourceManager::GetInstance()->LoadTexture(acCombineTex, acCombineTexPath);
		}*/
#endif

		::fread(&pNtlWorldSector->m_TextureMatBase[0], sizeof(D3DXMATRIX), 1, pFile);
		::fread(&pNtlWorldSector->m_TextureMatBase[1], sizeof(D3DXMATRIX), 1, pFile);
		::fread(&pNtlWorldSector->m_TextureMatAngBase, sizeof(RwReal), 1, pFile);
		::fread(&pNtlWorldSector->m_TextureMatAddBase, sizeof(RwUInt8), 1, pFile);


#ifdef dNTL_WORLD_TOOL_MODE

		CurFieldTexInfo.IdxCurLT		= 0;
		CurFieldTexInfo.BLT.AngTexMat	= pNtlWorldSector->m_TextureMatAngBase;
		CurFieldTexInfo.BLT.AngTexAdd	= pNtlWorldSector->m_TextureMatAddBase;
		strcpy_s(CurFieldTexInfo.BLT.StrName, rwTEXTUREBASENAMELENGTH, ReadBuff);

#endif

		for(int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
		{
			fread(&BoolFlag, sizeof(RwBool), 1, pFile);
			if(BoolFlag)
			{
				fread(ReadBuff, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
				pNtlWorldSector->m_pTextureSplat[i] = CNtlPLResourceManager::GetInstance()->LoadTexture(ReadBuff, dGET_WORLD_PARAM()->CurDefaultTexPath); 
				DBO_ASSERT(pNtlWorldSector->m_pTextureSplat[i], "Texture load failed.");

#ifdef dNTL_WORLD_TOOL_MODE

				CurFieldTexInfo.IdxCurLT = i + 1;
				strcpy_s(CurFieldTexInfo.DLT[i].StrName, ReadBuff);

#endif

				fread(&BoolFlag, sizeof(RwBool), 1, pFile);
				if(BoolFlag)
				{
					fread(ReadBuff, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
					pNtlWorldSector->m_pTextureAlpha[i] = \
						CNtlPLResourceManager::GetInstance()->CreateTexture(ReadBuff,
						dGET_WORLD_PARAM()->WorldSectorTileNum,
						dGET_WORLD_PARAM()->WorldSectorTileNum,
						dNTL_SPLAT_ALPHA_SIZE,
						rwRASTERFORMAT8888);

					// Renderware is weird
					RwTextureSetAddressing(pNtlWorldSector->m_pTextureAlpha[i], rwTEXTUREADDRESSCLAMP);
					RwTextureSetFilterMode(pNtlWorldSector->m_pTextureAlpha[i], rwFILTERLINEAR);

					fread(pAlphaValueArray, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum, 1, pFile);
					CNtlPLResourceManager::GetInstance()->LoadAlphaBits(pNtlWorldSector->m_pTextureAlpha[i], pAlphaValueArray);
				}

				fread(&pNtlWorldSector->m_TextureMat[i][0], sizeof(D3DXMATRIX), 1, pFile);
				fread(&pNtlWorldSector->m_TextureMat[i][1], sizeof(D3DXMATRIX), 1, pFile);
				fread(&pNtlWorldSector->m_TextureMatAng[i], sizeof(RwReal), 1, pFile);
				fread(&pNtlWorldSector->m_TextureMatAdd[i], sizeof(RwUInt8), 1, pFile);

#ifdef dNTL_WORLD_TOOL_MODE

				CurFieldTexInfo.DLT[i].AngTexMat	= pNtlWorldSector->m_TextureMatAng[i];
				CurFieldTexInfo.DLT[i].AngTexAdd	= pNtlWorldSector->m_TextureMatAdd[i];

#endif
			}
		}

		NTL_ARRAY_DELETE(pAlphaValueArray);
	}

	// set material extension info.
	sRpNtlWorldSectorSplat NtlWorlSectorSplat;
	memset(&NtlWorlSectorSplat, 0, sizeof(sRpNtlWorldSectorSplat));
	NtlWorlSectorSplat.pNtlWorldSector = pNtlWorldSector;
	RpNtlWorldSetSectorSplat(pMaterial, &NtlWorlSectorSplat);

	NTL_RETURNVOID();
}

void _RpNtlWorldSectorMaterialExtensionCreate(sNtlWorldSector *pNtlSector, RpMaterial *pMaterial)
{
	NTL_FUNCTION("_RpNtlWorldSectorMaterialExtensionCreate");

	CNtlWorldSector *pNtlWorldSector = pNtlSector->pNtlWorldSector;

	// apply default textures 4 base layer
	pNtlWorldSector->m_pTextureBase = CNtlPLResourceManager::GetInstance()->LoadTexture("NTL", dGET_WORLD_PARAM()->CurDefaultTexPath);
	DBO_ASSERT(pNtlWorldSector->m_pTextureBase, "Texture load failed.");

	// initialize material plugin extension
	sRpNtlWorldSectorSplat NtlWorlSectorSplat;
	memset(&NtlWorlSectorSplat, 0, sizeof(sRpNtlWorldSectorSplat));
	NtlWorlSectorSplat.pNtlWorldSector = pNtlWorldSector;
	RpNtlWorldSetSectorSplat(pMaterial, &NtlWorlSectorSplat);

	NTL_RETURNVOID();
}

RwBool _RpNtlWorldSectorGeometrySaveIntoFile(RpWorldSector *pWorldSector, FILE* pFile)
{
	/*
	WorldFileFormat - SectorInfo
	SectorInfo를 File로 Save하는 부분이다.
	*/

	NTL_FUNCTION("_RpNtlWorldSectorGeometrySaveIntoFile");

	RwUInt32			MilePostGUID = 0;
	RwV3d*				pVList;
	RwBool				TrueFlag	= TRUE;
	RwBool				ZeroFlag	= FALSE;
	sNtlWorldSector*	pNtlSector	= dNTL_WORLD_LOCAL(pWorldSector, pNtlSector);

	if(!pNtlSector)
	{
		DBO_TRACE(FALSE, "pNtlSector is NULL at _RpNtlWorldSectorGeometrySaveIntoFile");
		return FALSE;
	}


	CNtlWorldSector *pNtlWorldSector = pNtlSector->pNtlWorldSector;

	if(pFile)
	{
		// texture base layer
		fwrite(pNtlWorldSector->m_pTextureBase->name, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
		fwrite(&pNtlWorldSector->m_TextureMatBase[0], sizeof(D3DXMATRIX), 1, pFile);
		fwrite(&pNtlWorldSector->m_TextureMatBase[1], sizeof(D3DXMATRIX), 1, pFile);
		fwrite(&pNtlWorldSector->m_TextureMatAngBase, sizeof(RwReal), 1, pFile);
		fwrite(&pNtlWorldSector->m_TextureMatAddBase, sizeof(RwUInt8), 1, pFile);

		for(int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
		{
			if(pNtlWorldSector->m_pTextureSplat[i])
			{
				fwrite(&TrueFlag, sizeof(RwBool), 1, pFile);		
				fwrite(pNtlWorldSector->m_pTextureSplat[i]->name, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);

				if(pNtlWorldSector->m_pTextureAlpha[i])
				{
					fwrite(&TrueFlag, sizeof(RwBool), 1, pFile);
					fwrite(pNtlWorldSector->m_pTextureAlpha[i]->name, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
					CNtlPLResourceManager::GetInstance()->SaveAlphaBits(pNtlWorldSector->m_pTextureAlpha[i], pFile);
				}
				else
				{
					fwrite(&ZeroFlag, sizeof(RwBool), 1, pFile);
				}

				// texture matrix info; [0] 4 alpha map, [1] 4 detail map
				fwrite(&pNtlWorldSector->m_TextureMat[i][0], sizeof(D3DXMATRIX), 1, pFile);
				fwrite(&pNtlWorldSector->m_TextureMat[i][1], sizeof(D3DXMATRIX), 1, pFile);
				fwrite(&pNtlWorldSector->m_TextureMatAng[i], sizeof(RwReal), 1, pFile);
				fwrite(&pNtlWorldSector->m_TextureMatAdd[i], sizeof(RwUInt8), 1, pFile);
			}
			else
				fwrite(&ZeroFlag, sizeof(RwBool), 1, pFile);
		}

		// save heightfield info.
		pVList = RpMorphTargetGetVertices(pNtlWorldSector->m_pAtomic->geometry->morphTarget);
		fwrite(pVList, sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, pFile);

		// save prelit
		RwRGBA *pPrelights = RpGeometryGetPreLightColors(pNtlWorldSector->m_pAtomic->geometry);
		fwrite(pPrelights, sizeof(RwRGBA) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, pFile);

		// map obj.; ornament
		RwInt32 NumMapObj = (RwInt32)pNtlWorldSector->m_vecNtlPLEntityOrnament.size();
		fwrite(&NumMapObj, sizeof(RwInt32), 1, pFile);

		CNtlPLObject	*pNtlPLObject;
		RwV3d			SRT[3];
		RwChar			Name[128];
		RwBool			EnableFlag;
		RwUInt32		SID;
		RwUInt32		ObjectID;
		RwReal			VisibilityDist;
		DWORD			ObjNameIdx;

		for(int i = 0; i < NumMapObj; ++i)
		{
			pNtlPLObject = static_cast<CNtlPLObject*>(pNtlWorldSector->m_vecNtlPLEntityOrnament[i]);
			CNtlMath::MathRwV3dAssign(&SRT[0], pNtlPLObject->GetScale()->x, pNtlPLObject->GetScale()->y, pNtlPLObject->GetScale()->z);
			CNtlMath::MathRwV3dAssign(&SRT[1], pNtlPLObject->GetRotate()->x, pNtlPLObject->GetRotate()->y, pNtlPLObject->GetRotate()->z);
			CNtlMath::MathRwV3dAssign(&SRT[2], pNtlPLObject->GetPosition().x, pNtlPLObject->GetPosition().y, pNtlPLObject->GetPosition().z);
			::strcpy_s(Name, const_cast<RwChar*>(pNtlPLObject->GetName()));
			fwrite(&Name, sizeof(RwChar) * 128, 1, pFile);
			fwrite(&SRT[0], sizeof(RwV3d), 1, pFile);
			fwrite(&SRT[1], sizeof(RwV3d), 1, pFile);
			fwrite(&SRT[2], sizeof(RwV3d), 1, pFile);

			RwUInt32 uiObjectType = pNtlPLObject->GetObjectType();
			fwrite(&uiObjectType, sizeof(RwUInt32), 1, pFile);

			if (pNtlPLObject->GetObjectTypePtr())
			{
				pNtlPLObject->GetObjectTypePtr()->SaveIntoFile(pFile);
			}

			// serial id
			SID = pNtlPLObject->GetSID();
			fwrite(&SID, sizeof(RwUInt32), 1, pFile);

			ObjectID = pNtlPLObject->GetObjectSerialID();
#ifdef dNTL_WORLD_FORMAT_CONVERT // 월드 포맷 변환시dGET_WORLD_PARAM()->WorldObjectSerialID 저장되어야 한다.
			if (ObjectID == 0)
			{
				ObjectID = dGET_WORLD_PARAM()->WorldObjectSerialID;
				++dGET_WORLD_PARAM()->WorldObjectSerialID;
			}
			fwrite(&ObjectID, sizeof(RwUInt32), 1, pFile);
#endif

			// object name index
			ObjNameIdx = pNtlPLObject->GetObjNameIdx();
			fwrite(&ObjNameIdx, sizeof(DWORD), 1, pFile);

			// visibility distance
			VisibilityDist = pNtlPLObject->GetVisibleCullingDistance();
			fwrite(&VisibilityDist, sizeof(RwReal), 1, pFile);

			// enable trigger
			EnableFlag = pNtlPLObject->GetFlags() & NTL_PLEFLAG_TRIGGER; 
			fwrite(&EnableFlag, sizeof(RwBool), 1, pFile);

			// enable selection
			EnableFlag = pNtlPLObject->GetFlags() & NTL_PLEFLAG_SELECTION; 
			fwrite(&EnableFlag, sizeof(RwBool), 1, pFile);

			// enable alterable; dynamic obj.s.
			EnableFlag = pNtlPLObject->GetFlags() & NTL_PLEFLAG_ALTERABLE; 
			fwrite(&EnableFlag, sizeof(RwBool), 1, pFile);

			// enable portal
			EnableFlag = pNtlPLObject->GetFlags() & NTL_PLEFLAG_PORTAL; 
			fwrite(&EnableFlag, sizeof(RwBool), 1, pFile);

			// enable path engine collision
			EnableFlag = pNtlPLObject->GetFlags() & NTL_PLEFLAG_PECOLLISION;
			fwrite(&EnableFlag, sizeof(RwBool), 1, pFile);

			// enable Shadow Prop
			EnableFlag = pNtlPLObject->GetFlags() & NTL_PLEFLAG_SHADOW_PROP;
			/*if ((Name[3] == 'n' || Name[3] == 'N') &&
			Name[4] == '_' &&
			(Name[5] == 't' || Name[5] == 'T') &&
			(Name[6] == 'r' || Name[6] == 'R') &&
			(Name[7] == 'e' || Name[7] == 'E') &&
			(Name[8] == 'e' || Name[8] == 'E') &&
			Name[9] == '_')
			{
			EnableFlag = TRUE;
			}*/			
			fwrite(&EnableFlag, sizeof(RwBool), 1, pFile);

			// object PS map
			pNtlPLObject->SavePSMap(pFile);

			// milepost GUID
			MilePostGUID = pNtlPLObject->GetMilepostID();
			EnableFlag = (pNtlPLObject->GetProperty()->GetObjectType() == E_OBJECT_MILEPOST && MilePostGUID);
			fwrite(&EnableFlag, sizeof(RwBool), 1, pFile);
			if(EnableFlag)
			{
				fwrite(&MilePostGUID, sizeof(RwUInt32), 1, pFile);
			}

			// attach lightojbect
			pNtlPLObject->SaveLightObjectAttachData(pFile);
		}

		// water
		sSECTOR_WATER_ATTR* pSectorWaterAttr = pNtlWorldSector->m_pWater;
		if(pSectorWaterAttr)
		{
			::fwrite(&TrueFlag,											sizeof(RwBool), 1, pFile);
			::fwrite(&pSectorWaterAttr->_Height,						sizeof(RwReal), 1, pFile);
			::fwrite(&pSectorWaterAttr->_RGBA,							sizeof(RwRGBA), 1, pFile);
			::fwrite(&pSectorWaterAttr->_RenderstateSrc,				sizeof(DWORD), 1, pFile);
			::fwrite(&pSectorWaterAttr->_RenderstateDst,				sizeof(DWORD), 1, pFile);
			::fwrite(&pSectorWaterAttr->_RotTrans,						sizeof(RwReal), 1, pFile);
			::fwrite(&pSectorWaterAttr->_SclTrans,						sizeof(RwReal), 1, pFile);
			::fwrite(&pSectorWaterAttr->_DepthMapSize,					sizeof(RwInt32), 1, pFile);
			::fwrite(&pSectorWaterAttr->_DepthMapHeightVariation[0],	sizeof(RwReal), 1, pFile);
			::fwrite(&pSectorWaterAttr->_DepthMapHeightVariation[1],	sizeof(RwReal), 1, pFile);
			::fwrite(&pSectorWaterAttr->_IdxSequence, sizeof(RwInt32), 1, pFile);

			if(pSectorWaterAttr->_pDepthMap)
			{
				::fwrite(&TrueFlag, sizeof(RwBool), 1, pFile);
				CNtlPLResourceManager::GetInstance()->SaveAlphaBits(pSectorWaterAttr->_pDepthMap, pFile);
			}
			else
			{
				::fwrite(&ZeroFlag, sizeof(RwBool), 1, pFile);
			}
		}
		else
		{
			::fwrite(&ZeroFlag, sizeof(RwBool), 1, pFile);
		}

		// shadow
		CNtlWorldShadow* pNtlWorldShadow = pNtlWorldSector->m_pNtlWorldShadow;
		if(pNtlWorldShadow->m_pTexShadow)
		{
			::fwrite(&TrueFlag, sizeof(RwBool), 1, pFile);
			::fwrite(&pNtlWorldShadow->m_rgbaShadow, sizeof(RwRGBA), 1, pFile);
			::fwrite(&pNtlWorldShadow->m_CamViewWindowX, sizeof(RwReal), 1, pFile);
			::fwrite(&pNtlWorldShadow->m_matLight, sizeof(RwMatrix), 1, pFile);

			RwInt32 Size = RwRasterGetWidth(pNtlWorldShadow->m_pTexShadow->raster);
			::fwrite(&Size, sizeof(RwInt32), 1, pFile);
			CNtlPLResourceManager::GetInstance()->SaveTexture(pNtlWorldShadow->m_pTexShadow, pFile);
		}
		else
		{
			::fwrite(&ZeroFlag, sizeof(RwBool), 1, pFile);
		}

		// effects
		CNtlPLEntity *pEntity;
		RwInt32 NumEff = (RwInt32)pNtlWorldSector->m_vecNtlPLEntityEffect.size();
		fwrite(&NumEff, sizeof(RwInt32), 1, pFile);

		for(int i = 0; i < NumEff; ++i)
		{
			pEntity = pNtlWorldSector->m_vecNtlPLEntityEffect[i];
			CNtlMath::MathRwV3dAssign(&SRT[0], pEntity->GetScale(), pEntity->GetScale(), pEntity->GetScale());
			CNtlMath::MathRwV3dAssign(&SRT[1], pEntity->GetRotate()->x, pEntity->GetRotate()->y, pEntity->GetRotate()->z);
			CNtlMath::MathRwV3dAssign(&SRT[2], pEntity->GetPosition().x, pEntity->GetPosition().y, pEntity->GetPosition().z);
			::strcpy_s(Name, const_cast<RwChar*>(pEntity->GetName()));
			fwrite(&Name, sizeof(RwChar) * 128, 1, pFile);
			fwrite(&SRT[0], sizeof(RwV3d), 1, pFile);
			fwrite(&SRT[1], sizeof(RwV3d), 1, pFile);
			fwrite(&SRT[2], sizeof(RwV3d), 1, pFile);

			// visibility distance
			VisibilityDist = pEntity->GetVisibleCullingDistance();
			fwrite(&VisibilityDist, sizeof(RwReal), 1, pFile);
		}

		// enviroment sound
		RwInt32 NumSound	= static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntitySound.size());
		fwrite(&NumSound, sizeof(RwInt32), 1, pFile);

		for(int i = 0; i < NumSound; ++i)
		{
			RwV3d	Pos			= static_cast<CNtlPLSound*>(pNtlWorldSector->m_vecNtlPLEntitySound[i])->GetPosition();
			RwReal	Vol			= static_cast<CNtlPLSound*>(pNtlWorldSector->m_vecNtlPLEntitySound[i])->GetVolume();
			RwReal	RadiusMin	= static_cast<CNtlPLSound*>(pNtlWorldSector->m_vecNtlPLEntitySound[i])->GetRadius(TRUE);
			RwReal	RadiusMax	= static_cast<CNtlPLSound*>(pNtlWorldSector->m_vecNtlPLEntitySound[i])->GetRadius(FALSE);

			RwChar	NameSound[32] = {0,};
			strcpy_s(NameSound, 32, static_cast<CNtlPLSound*>(pNtlWorldSector->m_vecNtlPLEntitySound[i])->GetName());

			fwrite(NameSound, sizeof(RwChar) * 32, 1, pFile);
			fwrite(&Vol, sizeof(RwReal), 1, pFile);
			fwrite(&RadiusMin, sizeof(RwReal), 1, pFile);
			fwrite(&RadiusMax, sizeof(RwReal), 1, pFile);
			fwrite(&Pos, sizeof(RwV3d), 1, pFile);
		}

		// bgm sound
		// CZ-BGM
		RwInt32 NumSoundBGM	= static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntitySoundBGM.size());
		fwrite(&NumSoundBGM, sizeof(RwInt32), 1, pFile);

		for(int i = 0; i < NumSoundBGM; ++i)
		{
			RwV3d	Pos			= static_cast<CNtlPLSoundBGM*>(pNtlWorldSector->m_vecNtlPLEntitySoundBGM[i])->GetPosition();
			//RwReal	Vol			= static_cast<CNtlPLSoundBGM*>(pNtlWorldSector->m_vecNtlPLEntitySoundBGM[i])->GetVolume();
			RwReal	Radius		= static_cast<CNtlPLSoundBGM*>(pNtlWorldSector->m_vecNtlPLEntitySoundBGM[i])->GetRadius();

			RwChar	NameSound[32] = {0,};
			strcpy_s(NameSound, 32, static_cast<CNtlPLSoundBGM*>(pNtlWorldSector->m_vecNtlPLEntitySoundBGM[i])->GetName());

			fwrite(NameSound, sizeof(RwChar) * 32, 1, pFile);
			//fwrite(&Vol, sizeof(RwReal), 1, pFile);
			fwrite(&Radius, sizeof(RwReal), 1, pFile);
			fwrite(&Pos, sizeof(RwV3d), 1, pFile);
		}

		// soft edged spot light
		// 		RwV3d		SSPos;
		// 		RwV3d*		pSSRot;
		// 		RwInt32		SSNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntitySS.size());
		// 		RwRGBAReal	SSClr;
		// 		RwReal		SSAng, SSRad;
		// 		CNtlPLSpot*	SSEntity;
		// 
		// 		fwrite(&SSNum, sizeof(RwInt32), 1, pFile);
		// 
		// 		for(int i = 0; i < SSNum; ++i)
		// 		{
		// 			SSEntity	= static_cast<CNtlPLSpot*>(pNtlWorldSector->m_vecNtlPLEntitySS[i]);
		// 			SSPos		= SSEntity->GetPosition();
		// 			pSSRot		= SSEntity->GetRotate();
		// 			SSClr		= SSEntity->GetColor();
		// 			SSAng		= SSEntity->GetAngle();
		// 			SSRad		= SSEntity->GetRadius();
		// 
		// 			fwrite(&SSPos, sizeof(RwV3d), 1, pFile);
		// 			fwrite(pSSRot, sizeof(RwV3d), 1, pFile);
		// 			fwrite(&SSClr, sizeof(RwRGBAReal), 1, pFile);
		// 			fwrite(&SSAng, sizeof(RwReal), 1, pFile);
		// 			fwrite(&SSRad, sizeof(RwReal), 1, pFile);
		// 		}

		// Sector Cull
		fwrite(&pNtlWorldSector->m_IsCulled, sizeof(RwBool), 1, pFile);

		// Tile Transparency
		fwrite(&pNtlWorldSector->m_IsTileTransparency, sizeof(RwBool), 1, pFile);

		// Shoreline
		if(pNtlWorldSector->m_pNtlPLEntityShoreLine && static_cast<CNtlPLShoreLine*>(pNtlWorldSector->m_pNtlPLEntityShoreLine)->IsValidShoreline())
		{
			fwrite(&TrueFlag, sizeof(RwBool), 1, pFile);

			CNtlPLShoreLine* pSL = static_cast<CNtlPLShoreLine*>(pNtlWorldSector->m_pNtlPLEntityShoreLine);
			pSL->SaveIntoFile(pFile);
		}
		else
		{
			fwrite(&ZeroFlag, sizeof(RwBool), 1, pFile);
		}

		// decals
		RwInt32	EntityNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityDecal.size());

		fwrite(&EntityNum, sizeof(RwInt32), 1, pFile);

		for(int i = 0; i < EntityNum; ++i)
		{
			static_cast<CNtlPLWorldDecal*>(pNtlWorldSector->m_vecNtlPLEntityDecal[i])->SaveIntoFile(pFile);
		}

		// plant
		RwInt32 iPlantSetNum = static_cast<RwInt32>(pNtlWorldSector->m_vecPlantSet.size());

		fwrite(&iPlantSetNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iPlantSetNum; ++i)
		{
			sSECTOR_PLANT_SET_ATTR*	pPlantSet		= pNtlWorldSector->m_vecPlantSet.at(i);
			RwInt32					iPlantObjNum	= static_cast<RwInt32>(pPlantSet->vecPlantObj.size());

			fwrite(&pPlantSet->iSetIdx, sizeof(RwInt32), 1, pFile); // Set Index
			fwrite(&iPlantObjNum, sizeof(RwInt32), 1, pFile); // Obj Num

			for (int j = 0; j < iPlantObjNum; ++j)
			{
				fwrite(&pPlantSet->vecPlantObj.at(j)->iTileIdx, sizeof(RwInt32), 1, pFile); // Tile Index				
			}			
		}

		// world light sphere
		RwInt32 iWLSphereNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityWorldLight_Sphere.size());

		fwrite(&iWLSphereNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iWLSphereNum; ++i)
		{
			CNtlPLWorldLight_Sphere* pWLSphere = (CNtlPLWorldLight_Sphere*)pNtlWorldSector->m_vecNtlPLEntityWorldLight_Sphere.at(i);

			RwV3d	vPos	= pWLSphere->GetPosition();
			RwRGBA	clr		= pWLSphere->GetColor();
			RwReal	fRadius	= pWLSphere->GetRadius();		

			fwrite(&vPos, sizeof(RwV3d), 1, pFile);
			fwrite(&clr, sizeof(RwRGBA), 1, pFile);
			fwrite(&fRadius, sizeof(RwReal), 1, pFile);
		}

		// world light box
		RwInt32 iWLBoxNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityWorldLight_Box.size());

		fwrite(&iWLBoxNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iWLBoxNum; ++i)
		{
			CNtlPLWorldLight_Box* pWLBox = (CNtlPLWorldLight_Box*)pNtlWorldSector->m_vecNtlPLEntityWorldLight_Box.at(i);

			RwV3d	vPos	= pWLBox->GetPosition();
			RwRGBA	clr		= pWLBox->GetColor();
			RwV3d	vRot	= *pWLBox->GetRotate();
			RwV3d	vScale	= *pWLBox->GetScaleVector();

			fwrite(&vPos, sizeof(RwV3d), 1, pFile);
			fwrite(&clr, sizeof(RwRGBA), 1, pFile);
			fwrite(&vRot, sizeof(RwV3d), 1, pFile);
			fwrite(&vScale, sizeof(RwV3d), 1, pFile);			
		}

		// occluder
		RwInt32 iOccluderQuadNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityOccluder_Quad.size());

		fwrite(&iOccluderQuadNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iOccluderQuadNum; ++i)
		{
			CNtlPLOccluder_Quad* pOccluder = (CNtlPLOccluder_Quad*)pNtlWorldSector->m_vecNtlPLEntityOccluder_Quad.at(i);

			RwUInt32	eFuncFlag	= pOccluder->GetOccluderFuncFlag();
			RwV3d		vPos		= pOccluder->GetPosition();
			RwInt32		iPlaneType	= pOccluder->GetOccluderPlaneType();

			RwChar		acTemp[128];
			strcpy_s(acTemp, 128, pOccluder->GetTriggerName());

			fwrite(&eFuncFlag, sizeof(RwUInt32), 1, pFile);
			fwrite(&vPos, sizeof(RwV3d), 1, pFile);
			fwrite(&iPlaneType, sizeof(RwInt32), 1, pFile);
			fwrite(acTemp, sizeof(RwChar) * 128, 1, pFile);
			for (int j = 0; j < 4; ++j)
			{
				fwrite(pOccluder->GetVertexOriginal(j), sizeof(RwV3d), 1, pFile);
			}
		}

		// heathaze
		RwInt32 iHeatHazeObjectNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityHeatHaze.size());

		fwrite(&iHeatHazeObjectNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < iHeatHazeObjectNum; ++i)
		{
			CNtlPLHeatHazeObject* pHeatHaze = (CNtlPLHeatHazeObject*)pNtlWorldSector->m_vecNtlPLEntityHeatHaze.at(i);

			for (int j = 0; j < 4; ++j)
			{
				fwrite(pHeatHaze->GetVertex(j), sizeof(RwV3d), 1, pFile);
			}

			RwReal fDistance = pHeatHaze->GetDistance();
			fwrite(&fDistance, sizeof(RwReal), 1, pFile);

			fwrite(pHeatHaze->HeatHazeData(), sizeof(sNTL_HEATHAZE), 1, pFile);
		}

		// light object
		RwInt32 iLightObjectNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityLightObject.size());
		fwrite(&iLightObjectNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iLightObjectNum; ++i)
		{
			CNtlPLLightObject* pLightObject = (CNtlPLLightObject*)pNtlWorldSector->m_vecNtlPLEntityLightObject.at(i);

			RpLightType eType;
			eType = pLightObject->GetLightType();
			fwrite(&eType, sizeof(RpLightType), 1, pFile);

			RwBool bTemp;
			bTemp = pLightObject->IsRealLight();
			fwrite(&bTemp, sizeof(RwBool), 1, pFile);

			RwV3d vTemp;
			vTemp = pLightObject->GetPosition();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

			vTemp = *pLightObject->GetRotate();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

			RwRGBAReal rgbafTemp;
			rgbafTemp = pLightObject->GetColor();
			fwrite(&rgbafTemp, sizeof(RwRGBAReal), 1, pFile);

			RwReal fTemp;
			fTemp = pLightObject->GetRadius();
			fwrite(&fTemp, sizeof(RwReal), 1, pFile);

			fTemp = pLightObject->GetAngle();
			fwrite(&fTemp, sizeof(RwReal), 1, pFile);

			// CZ-LO
			bTemp = pLightObject->IsEnabledDecal();
			fwrite(&bTemp, sizeof(RwBool), 1, pFile);

			bTemp = pLightObject->IsEnabledWorldLight();
			fwrite(&bTemp, sizeof(RwBool), 1, pFile);
		}

		// dojo
		RwInt32 iDojoNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityDojo.size());

		fwrite(&iDojoNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < iDojoNum; ++i)
		{
			CNtlPLDojo* pDojo = (CNtlPLDojo*)pNtlWorldSector->m_vecNtlPLEntityDojo.at(i);

			RwV3d vScale	= *pDojo->GetScaleVector();
			RwV3d vRot		= *pDojo->GetRotate();
			RwV3d vPos		= pDojo->GetPosition();

			fwrite(&vScale, sizeof(RwV3d), 1, pFile);
			fwrite(&vRot, sizeof(RwV3d), 1, pFile);
			fwrite(&vPos, sizeof(RwV3d), 1, pFile);

			RwInt32 iDojoID		= pDojo->GetDojoID();
			RwReal	fFadeInTime	= pDojo->GetFadeInTime();
			RwReal	fFadeOutTime= pDojo->GetFadeOutTime();

			fwrite(&iDojoID, sizeof(RwInt32), 1, pFile);
			fwrite(&fFadeInTime, sizeof(RwReal), 1, pFile);
			fwrite(&fFadeOutTime, sizeof(RwReal), 1, pFile);

			RwChar acTemp[dPL_DOJO_RES_NAME_LEN];
			for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
			{
				strcpy_s(acTemp, dPL_DOJO_RES_NAME_LEN, pDojo->GetResObjectName(i));
				fwrite(acTemp, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);
				strcpy_s(acTemp, dPL_DOJO_RES_NAME_LEN, pDojo->GetResEffectNameUp(i));
				fwrite(acTemp, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);
				strcpy_s(acTemp, dPL_DOJO_RES_NAME_LEN, pDojo->GetResEffectNameDown(i));
				fwrite(acTemp, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);
			}
		}		
	}

	NTL_RETURN(TRUE);
}

RpAtomic* AtomicBuildDataCB(RpAtomic *pAtomic, void *data)
{
	if(RpCollisionGeometryBuildData(RpAtomicGetGeometry(pAtomic), NULL) == (RpGeometry *)NULL)
	{
		int woody = 0;
	}

	return NULL;
}

RwBool _RpNtlWorldSectorReadDummy(FILE* pFile)
{
	if(!pFile)
	{
		return FALSE;
	}

	g_FA.Skip_Sector(pFile);

	return TRUE;
}

RwBool _RpNtlWorldSectorReadMaterialDummy(FILE* pFile)
{
	if (!pFile)
	{
		return FALSE;
	}

	g_FA.Skip_Material(pFile);

	return TRUE;
}

RwBool _RpNtlWorldSectorMapObjLoadFromFile(RwV3d* pBuffer, FILE* pFile)
{
	NTL_FUNCTION("_RpNtlWorldSectorMapObjLoadFromFile");

	_RpNtlWorldSectorReadMaterialDummy(pFile);

	::fread(pBuffer, sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, pFile);
	// skip rest of sector file stream for the rest sectors to load
	::fseek(pFile, sizeof(RwRGBA) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, SEEK_CUR);

	// skip rest of sector file stream for the rest sectors to load
	::fseek(pFile, sizeof(RwRGBA) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, SEEK_CUR);

	RwInt32 TmpObjNum;
	fread(&TmpObjNum, sizeof(RwInt32), 1, pFile);

	for(int i = 0; i < TmpObjNum; ++i)
	{
		::fseek(pFile, sizeof(RwInt32) + (sizeof(RwChar) * 128) + (sizeof(RwV3d) * 3), SEEK_CUR);
	}

	NTL_RETURN(TRUE);
}

RwBool _RpNtlWorldGetFilePosDoodads(FILE* pFile)
{
	if(!pFile)
		return FALSE;

	g_FA.Skip_Material(pFile);
	g_FA.Skip_Heightfield(pFile);
	g_FA.Skip_Diffuse(pFile);

	/*
	// skip material
	_RpNtlWorldSectorReadMaterialDummy(pFile);

	// skip heightfield
	::fseek(pFile, sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, SEEK_CUR);

	// skip rest of sector file stream for the rest sectors to load
	::fseek(pFile, sizeof(RwRGBA) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, SEEK_CUR);
	*/

	return TRUE;
}

RwBool _RpNtlWorldSectorGeometryLoadFromFile(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, FILE* pFile)
{
	/*
	WorldFileFormat - SectorInfo
	SectorInfo를 File로부터 Load 하는 부분 이다.
	*/

	NTL_FUNCTION("_RpNtlWorldSectorGeometryLoadFromFile");

	RpGeometry *pGeometry;
	RpMaterial *pMaterial;
	RwSurfaceProperties SurfProp;
	RpMorphTarget *pMorphTarget;
	RwSphere BoundingSphere;
	RwTexCoords *pTexCoord;
	RwV3d *pVList;
	RpTriangle *pTList;
	RwV2d SPos, EPos;
	RwInt16 CurPtIndices[4];
	RwInt32 m, n;
	RwRGBA *pPrelights;

	pGeometry			= RpGeometryCreate(dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, dGET_WORLD_PARAM()->WorldSectorPolyNum, rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYTEXTURED);
	pMorphTarget		= RpGeometryGetMorphTarget(pGeometry, 0);
	pMaterial			= RpMaterialCreate();
	SurfProp.ambient	= 1.0f;
	SurfProp.diffuse	= 1.0f;
	SurfProp.specular	= 1.0f;
	RpMaterialSetSurfaceProperties(pMaterial, &SurfProp);

	pPrelights = RpGeometryGetPreLightColors(pGeometry);
	pVList = RpMorphTargetGetVertices(pMorphTarget);
	pTList = RpGeometryGetTriangles(pGeometry);
	pTexCoord = RpGeometryGetVertexTexCoords(pGeometry, rwTEXTURECOORDINATEINDEX0);

	_RpNtlWorldSectorMaterialExtensionRead(CurFieldTexInfo, pNtlWorldSector, pMaterial, pFile);

	if(pFile)
	{
		// load heightfield info.
		fread(pVList ,sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, pFile);

		// load prelit
		fread(pPrelights, sizeof(RwRGBA) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, 1, pFile);

#ifdef dNTL_WORLD_TOOL_MODE
		// 		for (int i = 0; i < dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum; ++i)
		// 		{
		// 			if (pPrelights[i].alpha != 0)
		// 			{
		// 				pPrelights[i].alpha = 255;
		// 			}
		// 		}
#endif

		// map obj.; ornament
		RwInt32 NumMapObj;
		fread(&NumMapObj, sizeof(RwInt32), 1, pFile);

		RwV3d		SRT[3];
		RwChar		Name[128];
		RwBool		EnableFlag;
		RwUInt32	SID;
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
		RwUInt32	ObjectID;
#endif
		RwReal		VisibilityDist;
		RwUInt32	MilePostGUID = 0;

		SPLObjectCreateParam PLEntityCreateParam;
		CNtlPLEntity *pNtlPLEntity = NULL;
		pNtlWorldSector->m_vecNtlPLEntityOrnament.clear();

#ifndef dNTL_WORLD_TOOL_MODE

		sNTL_EVENT_TRI_DOODADS	NtlEventTriDoodads;

		if(NumMapObj)
		{
			NtlEventTriDoodads.FlgTriDoodads = 0;
			NtlEventTriDoodads.vecDat.reserve(10);
		}
#endif

		for(int i = 0; i < NumMapObj; ++i)
		{
			fread(Name, sizeof(RwChar) * 128, 1, pFile);
			fread(&SRT[0], sizeof(RwV3d), 1, pFile);
			fread(&SRT[1], sizeof(RwV3d), 1, pFile);
			fread(&SRT[2], sizeof(RwV3d), 1, pFile);

			DBO_ASSERT(strlen(Name), "Sector Object Load : Name Ivalid.");

			RwUInt32 uiObjectType = EPL_OBJECT_TYPE_OUTDOOR_OBJECT;
			fread(&uiObjectType, sizeof(RwUInt32), 1, pFile);

			PLEntityCreateParam.pPos		= &SRT[2];
			PLEntityCreateParam.bLoadMap	= dGET_WORLD_PARAM()->Loading;

			RwBool bLoadScheduling = CNtlPLResourceManager::GetInstance()->IsLoadScheduling();
			if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
				uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
			{
				PLEntityCreateParam.bLoadMap = TRUE;
				CNtlPLResourceManager::GetInstance()->SetLoadScheduling(FALSE);
			}		

			pNtlPLEntity = static_cast<CNtlPLEntity*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, Name, &PLEntityCreateParam));
			DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

			CNtlPLResourceManager::GetInstance()->SetLoadScheduling(bLoadScheduling);

			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetObjectType(uiObjectType);
			if (static_cast<CNtlPLObject*>(pNtlPLEntity)->GetObjectTypePtr())
			{
				static_cast<CNtlPLObject*>(pNtlPLEntity)->GetObjectTypePtr()->LoadFromFile(pFile);
			}		

			if(static_cast<CNtlPLObject*>(pNtlPLEntity)->GetPlayAnim())
			{
				static_cast<CNtlPLObject*>(pNtlPLEntity)->SetPlayAnimTime(NtlRandomNumber(0.0f, 1.0f));
			}

#ifdef dNTL_WORLD_TOOL_MODE
			{
				// If current doodads visual mode is set
				//pNtlPLEntity->SetVisible(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_DOODADS));

				// doodads visibility distance culling mode setting
				static_cast<CNtlPLObject*>(pNtlPLEntity)->SetFadeEnable(TRUE);
			}
#endif

			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetScale(&SRT[0]);
			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetRotate(SRT[1].x, SRT[1].y, SRT[1].z);
			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetPosition(&SRT[2]);

			// serial id
			fread(&SID, sizeof(RwUInt32), 1, pFile);
			pNtlPLEntity->SetSID(SID);

			// object id
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
			fread(&ObjectID, sizeof(RwUInt32), 1, pFile);
			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetObjectSerialID(ObjectID);
#endif

			// object name index
			DWORD ObjNameIdx;
			fread(&ObjNameIdx, sizeof(DWORD), 1, pFile);
			pNtlPLEntity->SetObjNameIdx(ObjNameIdx);

			// visibility distance
			fread(&VisibilityDist, sizeof(RwReal), 1, pFile);
			pNtlPLEntity->SetVisibleCullingDistance(VisibilityDist);

			// enable trigger
			fread(&EnableFlag, sizeof(RwBool), 1, pFile);
			if(EnableFlag)
			{
				pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_TRIGGER);

#ifndef dNTL_WORLD_TOOL_MODE
				NtlEventTriDoodads.vecDat.push_back(pNtlPLEntity);
#endif
			}

			// enable selection
			fread(&EnableFlag, sizeof(RwBool), 1, pFile);
			if(EnableFlag)
			{
				pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_SELECTION);
			}

			pNtlWorldSector->m_vecNtlPLEntityOrnament.push_back(pNtlPLEntity);

			// enable alterable; dynamic obj.s.
			fread(&EnableFlag, sizeof(RwBool), 1, pFile);
			if(EnableFlag)
			{
				pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_ALTERABLE);
			}

			// enable portal
			fread(&EnableFlag, sizeof(RwBool), 1, pFile);
			if(EnableFlag)
			{
				pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_PORTAL);
			}

			// enable path engine collision
			fread(&EnableFlag, sizeof(RwBool), 1, pFile);
			if(EnableFlag)
			{
				pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_PECOLLISION);
			}

			// enable shadow prop
			fread(&EnableFlag, sizeof(RwBool), 1, pFile);
			if(EnableFlag)
			{
				pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_SHADOW_PROP);
			}

			// object PS map
			static_cast<CNtlPLObject*>(pNtlPLEntity)->LoadPSMap(pFile);

			if(!GetNtlResourceManager()->IsLoadScheduling())
			{
				static_cast<CNtlPLObject*>(pNtlPLEntity)->SetAtomicPSMap();
			}

			// milepost GUID
			fread(&EnableFlag, sizeof(RwBool), 1, pFile);
			if(EnableFlag)
			{
				fread(&MilePostGUID, sizeof(RwUInt32), 1, pFile);
				static_cast<CNtlPLObject*>(pNtlPLEntity)->SetMilepostID(MilePostGUID);
			}

			// attach lightojbect
			static_cast<CNtlPLObject*>(pNtlPLEntity)->LoadLightObjectAttachData(pFile);
		}
#ifdef dNTL_WORLD_TOOL_MODE
		CNtlPLObject::LoadSwapFile(pNtlWorldSector->DatumPoint.x, pNtlWorldSector->DatumPoint.y, pNtlWorldSector->DatumPoint.z);
#endif

#ifndef dNTL_WORLD_TOOL_MODE

		if(NtlEventTriDoodads.vecDat.size())
		{
			CNtlPLEventGenerator::OnTriDoodadsEvent(&NtlEventTriDoodads);
			NtlEventTriDoodads.vecDat.clear();
		}

#endif

		// water
		RwBool BooleanFlag;
		::fread(&BooleanFlag, sizeof(RwBool), 1, pFile);

		if(BooleanFlag)
		{
			sSECTOR_WATER_ATTR SectorWaterAttr;
			::fread(&SectorWaterAttr._Height,						sizeof(RwReal), 1, pFile);
			::fread(&SectorWaterAttr._RGBA,							sizeof(RwRGBA), 1, pFile);
			::fread(&SectorWaterAttr._RenderstateSrc,				sizeof(DWORD), 1, pFile);
			::fread(&SectorWaterAttr._RenderstateDst,				sizeof(DWORD), 1, pFile);
			::fread(&SectorWaterAttr._RotTrans,						sizeof(RwReal), 1, pFile);
			::fread(&SectorWaterAttr._SclTrans,						sizeof(RwReal), 1, pFile);
			::fread(&SectorWaterAttr._DepthMapSize,					sizeof(RwInt32), 1, pFile);
			::fread(&SectorWaterAttr._DepthMapHeightVariation[0],	sizeof(RwReal), 1, pFile);
			::fread(&SectorWaterAttr._DepthMapHeightVariation[1],	sizeof(RwReal), 1, pFile);
			::fread(&SectorWaterAttr._IdxSequence,					sizeof(RwInt32), 1, pFile);

			CNtlPLVisualManager* pSceneManager = static_cast<CNtlPLVisualManager*>(GetSceneManager());
			pSceneManager->GetWater()->OnCreate(pNtlWorldSector, SectorWaterAttr);

			::fread(&BooleanFlag, sizeof(RwBool), 1, pFile);
			if(BooleanFlag)
			{
				pNtlWorldSector->m_pWater->_pDepthMap =	CNtlPLResourceManager::GetInstance()->CreateTexture("DepthMap",
					SectorWaterAttr._DepthMapSize,
					SectorWaterAttr._DepthMapSize,
					32,
					rwRASTERFORMAT8888);

				RwTextureSetAddressing(pNtlWorldSector->m_pWater->_pDepthMap, rwTEXTUREADDRESSCLAMP);
				RwTextureSetFilterMode(pNtlWorldSector->m_pWater->_pDepthMap, rwFILTERLINEAR);

				BYTE* pTmp = NTL_NEW BYTE [SectorWaterAttr._DepthMapSize * SectorWaterAttr._DepthMapSize];
				::fread(pTmp, sizeof(BYTE) * SectorWaterAttr._DepthMapSize * SectorWaterAttr._DepthMapSize, 1, pFile);
				CNtlPLResourceManager::GetInstance()->LoadAlphaBits(pNtlWorldSector->m_pWater->_pDepthMap, pTmp);
				NTL_ARRAY_DELETE(pTmp);
			}
		}

		// shadow
		::fread(&BooleanFlag, sizeof(RwBool), 1, pFile);
		if(BooleanFlag)
		{
			CNtlWorldShadow* pNtlWorldShadow = pNtlWorldSector->m_pNtlWorldShadow;

			::fread(&pNtlWorldShadow->m_rgbaShadow, sizeof(RwRGBA), 1, pFile);
			::fread(&pNtlWorldShadow->m_CamViewWindowX, sizeof(RwReal), 1, pFile);
			::fread(&pNtlWorldShadow->m_matLight, sizeof(RwMatrix), 1, pFile);

			RwInt32 Size;
			::fread(&Size, sizeof(RwInt32), 1, pFile);

			pNtlWorldShadow->m_pTexShadow = CNtlPLResourceManager::GetInstance()->CreateTexture("ShadowMap",
				Size,
				Size,
				32,
				rwRASTERFORMAT8888);

			RwTextureSetFilterMode(pNtlWorldShadow->m_pTexShadow, rwFILTERLINEAR);
			RwTextureSetAddressing(pNtlWorldShadow->m_pTexShadow, rwTEXTUREADDRESSCLAMP);

			CNtlPLResourceManager::GetInstance()->LoadTexture(pNtlWorldShadow->m_pTexShadow, pFile);
			DBO_ASSERT(pNtlWorldShadow->m_pTexShadow, "Texture load failed.");
		}

		// effects
		RwInt32 NumEff;
		fread(&NumEff, sizeof(RwInt32), 1, pFile);

		for(int i = 0; i < NumEff; ++i)
		{
			fread(Name, sizeof(RwChar) * 128, 1, pFile);
			fread(&SRT[0], sizeof(RwV3d), 1, pFile);
			fread(&SRT[1], sizeof(RwV3d), 1, pFile);
			fread(&SRT[2], sizeof(RwV3d), 1, pFile);

			PLEntityCreateParam.pPos = &SRT[2];
			pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, Name, &PLEntityCreateParam);
			DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

#ifdef dNTL_WORLD_TOOL_MODE

			pNtlPLEntity->SetVisible(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_EFFECTS));

			if(dGET_CUR_PALETTE() == ePM_EFFECT)
			{
				(static_cast<CNtlInstanceEffect*>(pNtlPLEntity))->SetVisibleBoundingVolume(TRUE);
			}
			else
			{
				(static_cast<CNtlInstanceEffect*>(pNtlPLEntity))->SetVisibleBoundingVolume(FALSE);
			}

#endif

			pNtlPLEntity->SetScale(SRT[0].x);
			pNtlPLEntity->SetRotate(SRT[1].x, SRT[1].y, SRT[1].z);
			pNtlPLEntity->SetPosition(&SRT[2]);

			// visibility distance
			fread(&VisibilityDist, sizeof(RwReal), 1, pFile);
			pNtlPLEntity->SetVisibleCullingDistance(VisibilityDist);

			pNtlWorldSector->m_vecNtlPLEntityEffect.push_back(pNtlPLEntity);
		}
#ifdef dNTL_WORLD_TOOL_MODE
		CNtlInstanceEffect::LoadSwapFile(pNtlWorldSector->DatumPoint.x, pNtlWorldSector->DatumPoint.y, pNtlWorldSector->DatumPoint.z);
#endif

		// enviroment sound		
		RwInt32 NumSound;
		fread(&NumSound, sizeof(RwInt32), 1, pFile);

		for(int i = 0; i < NumSound; ++i)
		{
			RwV3d	Pos;		
			RwReal	Vol;
			RwReal	RadiusMin, RadiusMax;
			RwChar  NameSound[32] = {0,};

			fread(NameSound, sizeof(RwChar) * 32, 1, pFile);
			fread(&Vol, sizeof(RwReal), 1, pFile);
			fread(&RadiusMin, sizeof(RwReal), 1, pFile);
			fread(&RadiusMax, sizeof(RwReal), 1, pFile);
			fread(&Pos, sizeof(RwV3d), 1, pFile);

			PLEntityCreateParam.pPos = &Pos;

			CNtlPLEntity *pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_SOUND, "NULL", &PLEntityCreateParam);
			DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

			static_cast<CNtlPLSound*>(pNtlPLEntity)->GetVolume() = Vol;
			static_cast<CNtlPLSound*>(pNtlPLEntity)->GetRadius(TRUE) = RadiusMin;
			static_cast<CNtlPLSound*>(pNtlPLEntity)->GetRadius(FALSE) = RadiusMax;
			static_cast<CNtlPLSound*>(pNtlPLEntity)->PlaySound(NameSound);
			pNtlWorldSector->m_vecNtlPLEntitySound.push_back(pNtlPLEntity);
		}

		// bgm sound
		// CZ-BGM
		RwInt32 NumSoundBGM;
		fread(&NumSoundBGM, sizeof(RwInt32), 1, pFile);

		for(int i = 0; i < NumSoundBGM; ++i)
		{
			RwV3d	Pos;		
			//RwReal	Vol;
			RwReal	Radius;
			RwChar  NameSound[32] = {0,};

			fread(NameSound, sizeof(RwChar) * 32, 1, pFile);
			//fread(&Vol, sizeof(RwReal), 1, pFile);
			fread(&Radius, sizeof(RwReal), 1, pFile);
			fread(&Pos, sizeof(RwV3d), 1, pFile);

			PLEntityCreateParam.pPos = &Pos;

			CNtlPLEntity *pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_SOUND_BGM, "NULL", &PLEntityCreateParam);
			DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

			//static_cast<CNtlPLSoundBGM*>(pNtlPLEntity)->GetVolume() = Vol;
			static_cast<CNtlPLSoundBGM*>(pNtlPLEntity)->GetRadius() = Radius;
			static_cast<CNtlPLSoundBGM*>(pNtlPLEntity)->PlaySound(NameSound);
			pNtlWorldSector->m_vecNtlPLEntitySoundBGM.push_back(pNtlPLEntity);
		}

		// soft edged spot light
		// 		RwV3d		SSPos;
		// 		RwV3d		SSRot;
		// 		RwInt32		SSNum = 0;
		// 		RwRGBAReal	SSClr;
		// 		RwReal		SSAng, SSRad;
		// 
		// 		fread(&SSNum, sizeof(RwInt32), 1, pFile);
		// 
		// 		for(int i = 0; i < SSNum; ++i)
		// 		{
		// 			fread(&SSPos, sizeof(RwV3d), 1, pFile);
		// 			fread(&SSRot, sizeof(RwV3d), 1, pFile);
		// 			fread(&SSClr, sizeof(RwRGBAReal), 1, pFile);
		// 			fread(&SSAng, sizeof(RwReal), 1, pFile);
		// 			fread(&SSRad, sizeof(RwReal), 1, pFile);
		// 
		// 			SPLEntityCreateParam CreateParam;
		// 			CreateParam.pPos = &SSPos;
		// 
		// 			CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_SPOT_LIGHT, NTL_PLEN_SPOT_LIGHT, &CreateParam);
		// 			DBO_ASSERT(pNtlPLEntity, "Entity create failed.");
		// 
		// 			CNtlPLSpot* pSpotEntity = static_cast<CNtlPLSpot*>(pNtlPLEntity);
		// 
		// 			pSpotEntity->SetColor(SSClr);
		// 			pSpotEntity->SetAngle(SSAng);
		// 			pSpotEntity->SetRadius(SSRad);
		// 			pSpotEntity->SetPosition(&SSPos);
		// 			pSpotEntity->SetRotate(SSRot.x, SSRot.y, SSRot.z);
		// 
		// 			pNtlWorldSector->m_vecNtlPLEntitySS.push_back(pNtlPLEntity);
		// 		}

		// light object soft spot
		// 		RwInt32 iSSLightObjectNum;
		// 		fread(&iSSLightObjectNum, sizeof(RwInt32), 1, pFile);
		// 
		// 		for (int i = 0; i < iSSLightObjectNum; ++i)
		// 		{
		// 			SPLLightObjectCreateParam CreateParam;
		// 			CreateParam.rpLightType = rpLIGHTSPOTSOFT;
		// 			CreateParam.bRealLight	= TRUE;
		// 
		// 			RwV3d vPos;
		// 			fread(&vPos, sizeof(RwV3d), 1, pFile);
		// 			CreateParam.pPos = &vPos;
		// 
		// 			fread(&CreateParam.vRotate, sizeof(RwV3d), 1, pFile);
		// 			fread(&CreateParam.rgbafColor, sizeof(RwRGBAReal), 1, pFile);			
		// 			fread(&CreateParam.fAngle, sizeof(RwReal), 1, pFile);
		// 			fread(&CreateParam.fRadius, sizeof(RwReal), 1, pFile);
		// 
		// 			CNtlPLLightObject* pNtlPLLightObject = static_cast<CNtlPLLightObject*>(GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, "NULL", &CreateParam));
		// 			DBO_ASSERT(pNtlPLLightObject, "Entity create failed.");
		// 
		// 			pNtlWorldSector->m_vecNtlPLEntityLightObject.push_back(pNtlPLLightObject);
		// 		}

		// Sector Cull
		fread(&pNtlWorldSector->m_IsCulled, sizeof(RwBool), 1, pFile);

		// Tile Transparency
		fread(&pNtlWorldSector->m_IsTileTransparency, sizeof(RwBool), 1, pFile);

		// Shoreline
		RwBool IsThereShoreLine;
		fread(&IsThereShoreLine, sizeof(RwBool), 1, pFile);
		if(IsThereShoreLine)
		{
			SPLEntityCreateParam	PLEntityCreateParam;
			RwV3d					Pos;

			Pos.x						= pNtlWorldSector->DatumPoint.x;
			Pos.y						= pNtlWorldSector->DatumPoint.y;
			Pos.z						= pNtlWorldSector->DatumPoint.z;
			PLEntityCreateParam.pPos	= &Pos;

			pNtlWorldSector->m_pNtlPLEntityShoreLine = static_cast<CNtlPLShoreLine*>(GetSceneManager()->CreateEntity(PLENTITY_SHORELINE, NTL_PLEN_SHORELINE, &PLEntityCreateParam));			
			DBO_ASSERT(pNtlWorldSector->m_pNtlPLEntityShoreLine, "Entity create failed.");

			static_cast<CNtlPLShoreLine*>(pNtlWorldSector->m_pNtlPLEntityShoreLine)->LoadFromFile(pFile);
		}

		// Decals
		CNtlPLWorldDecal*	pNtlPLWorldDecal;
		RwInt32				EntityNum;
		sDECAL_PROP_PARAM	DecalPropParam;
		SPLDecalCreateParam CreateParam;

		fread(&EntityNum, sizeof(RwInt32), 1, pFile);

		for(int i = 0; i < EntityNum; ++i)
		{
			CNtlPLWorldDecal::Load(DecalPropParam, pFile);

			CreateParam.pTexName	= DecalPropParam._TexName;
			CreateParam.pTexPath	= ".\\Texture\\NtlWe\\Decal\\";

			RwReal Width	= DecalPropParam._SizeX;
			RwReal Height	= DecalPropParam._SizeZ;
			if(Width > Height)
			{
				CreateParam.fIntersectionRadius = Width;
			}
			else
			{
				CreateParam.fIntersectionRadius = Height;
			}

			CreateParam.vSize.x	= DecalPropParam._SizeX;
			CreateParam.vSize.z	= DecalPropParam._SizeZ;

			CreateParam.fScale				= DecalPropParam._TexScale;
			CreateParam.fVisibleSquaredDist	= DecalPropParam._VisibilityDist;

			string CurTexName = DecalPropParam._TexName;
			string W_Prefix("w_");
			string C_Prefix("c_");

			if(-1 == CurTexName.find_first_of(W_Prefix) && -1 == CurTexName.find_first_of(C_Prefix))
			{
				DBO_TRACE(FALSE, "Decal textures should've got prefix; w_, c_");
			}
			else
			{
				RwInt32 IdxWPrefix = CurTexName.find_first_of(W_Prefix);
				RwInt32 IdxCPrefix = CurTexName.find_first_of(C_Prefix);

				if(!IdxWPrefix)
				{
					CreateParam.rwWrapType = rwTEXTUREADDRESSWRAP;
				}
				else if(!IdxCPrefix)
				{
					CreateParam.rwWrapType = rwTEXTUREADDRESSCLAMP;
				}
				else
				{
					DBO_TRACE(FALSE, "Decal textures should've got prefix; w_, c_");
				}
			}

			if(DecalPropParam._DecalMode == 0)
			{
				CreateParam.nFlagEffect = E_FLAG_DECAL_RENDER_WORLD;
			}
			else if(DecalPropParam._DecalMode == 1)
			{
				CreateParam.nFlagEffect = E_FLAG_DECAL_RENDER_OBJECT;
			}
			else if(DecalPropParam._DecalMode == 2)
			{
				CreateParam.nFlagEffect = E_FLAG_DECAL_RENDER_WORLD | E_FLAG_DECAL_RENDER_OBJECT;
			}
			else if(DecalPropParam._DecalMode == 3)
			{
				CreateParam.nFlagEffect = E_FLAG_DECAL_RENDER_OBJECT | E_FLAG_DECAL_WATER;
			}

			CreateParam.eDecalType = DECAL_TERRAIN;

			pNtlPLWorldDecal = static_cast<CNtlPLWorldDecal*>(GetSceneManager()->CreateEntity(PLENTITY_DECAL, "NULL", &CreateParam));
			DBO_ASSERT(pNtlPLWorldDecal, "Entity create failed.");

			pNtlWorldSector->InsertIn2World(pNtlPLWorldDecal, 4);

			pNtlPLWorldDecal->SetRotate(DecalPropParam._TexAngle);
			pNtlPLWorldDecal->SetColor(DecalPropParam._RGBA.red, DecalPropParam._RGBA.green, DecalPropParam._RGBA.blue);
			pNtlPLWorldDecal->SetAlpha(DecalPropParam._RGBA.alpha);
			pNtlPLWorldDecal->SetAlphaBlend(TRUE, rwBLENDSRCALPHA, rwBLENDINVSRCALPHA);
			pNtlPLWorldDecal->SetPosition(&DecalPropParam._Pos);
			pNtlPLWorldDecal->SetName(DecalPropParam._TexName);

			if(DecalPropParam._UVAniCheck)
			{
				if(DecalPropParam._UVAniPatternIdx == 0)
				{
					SNtlPrtStdEmitterPrtUVAnim EmitterUVAnim;
					EmitterUVAnim.vDir.x		= 0.0f;
					EmitterUVAnim.vDir.y		= 0.0f;
					EmitterUVAnim.vDir.z		= 1.0f;
					EmitterUVAnim.fVelocity		= DecalPropParam._UVAniSpeed;
					EmitterUVAnim.fMaxOffset	= 0.0f;
					pNtlPLWorldDecal->SetUVAnimAction(EmitterUVAnim);
				}
				else if(DecalPropParam._UVAniPatternIdx == 1)
				{
					SNtlPrtStdEmitterPrtUVAnim EmitterUVAnim;
					EmitterUVAnim.vDir.x		= 0.0f;
					EmitterUVAnim.vDir.y		= 0.0f;
					EmitterUVAnim.vDir.z		= 1.0f;
					EmitterUVAnim.fVelocity		= DecalPropParam._UVAniSpeed;
					EmitterUVAnim.fMaxOffset	= DecalPropParam._UVAniOffset;
					pNtlPLWorldDecal->SetUVAnimAction(EmitterUVAnim);
				}
				else if(DecalPropParam._UVAniPatternIdx == 2)
				{
					pNtlPLWorldDecal->Set2DRoateAction(DecalPropParam._UVAniOffset, DecalPropParam._UVAniSpeed);
				}
				else if(DecalPropParam._UVAniPatternIdx == 3)
				{
					pNtlPLWorldDecal->SetSizeAction(DecalPropParam._TexScale, DecalPropParam._UVAniOffset, DecalPropParam._UVAniSpeed);
				}
			}
		}

		// plant
		RwInt32 iPlantSetNum;
		fread(&iPlantSetNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < iPlantSetNum; ++i)
		{
			sSECTOR_PLANT_SET_ATTR* pPlantSet = NTL_NEW sSECTOR_PLANT_SET_ATTR;
			RwInt32					iPlantObjNum;			

			fread(&pPlantSet->iSetIdx, sizeof(RwInt32), 1, pFile); // Set Index
			fread(&iPlantObjNum, sizeof(RwInt32), 1, pFile); // Obj Num

			for (int j = 0; j < iPlantObjNum; ++j)
			{
				sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR* pPlantObj = NTL_NEW sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR;

				fread(&pPlantObj->iTileIdx, sizeof(RwInt32), 1, pFile); // Tile Index

				pPlantSet->vecPlantObj.push_back(pPlantObj);
			}			
			pNtlWorldSector->m_vecPlantSet.push_back(pPlantSet);
		}

		// world light sphere
		RwInt32 iWLSphereNum;
		fread(&iWLSphereNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iWLSphereNum; ++i)
		{
			RwV3d				vPos;
			RwRGBA				clr;
			RwReal				fRadius;
			EPLWorldLightType	eType = EPLWORLDLIGHT_SPHERE;

			fread(&vPos, sizeof(RwV3d), 1, pFile);
			fread(&clr, sizeof(RwRGBA), 1, pFile);
			fread(&fRadius, sizeof(RwReal), 1, pFile);

			SPLWorldLightCreateParam_Sphere CreateParam;
			CreateParam.pPos		= &vPos;
			CreateParam._pUserData	= &eType;
			CreateParam.uiRed		= clr.red;
			CreateParam.uiGreen		= clr.green;
			CreateParam.uiBlue		= clr.blue;
			CreateParam.fRadius		= fRadius;

			CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_WORLD_LIGHT, "NULL", &CreateParam);
			DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

			pNtlWorldSector->m_vecNtlPLEntityWorldLight_Sphere.push_back(pNtlPLEntity);
		}

		// world light box
		RwInt32 iWLBoxNum;
		fread(&iWLBoxNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iWLBoxNum; ++i)
		{
			RwV3d				vPos;
			RwRGBA				clr;
			RwV3d				vRot;
			RwV3d				vScale;
			EPLWorldLightType	eType = EPLWORLDLIGHT_BOX;

			fread(&vPos, sizeof(RwV3d), 1, pFile);
			fread(&clr, sizeof(RwRGBA), 1, pFile);
			fread(&vRot, sizeof(RwV3d), 1, pFile);
			fread(&vScale, sizeof(RwV3d), 1, pFile);	

			SPLWorldLightCreateParam_Box CreateParam;
			CreateParam.pPos		= &vPos;
			CreateParam._pUserData	= &eType;
			CreateParam.uiRed		= clr.red;
			CreateParam.uiGreen		= clr.green;
			CreateParam.uiBlue		= clr.blue;
			CreateParam.vScale		= vScale;
			CreateParam.vRotate		= vRot;

			CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_WORLD_LIGHT, "NULL", &CreateParam);
			DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

			pNtlWorldSector->m_vecNtlPLEntityWorldLight_Box.push_back(pNtlPLEntity);
		}

		// occluder
		RwInt32 iOccluderQuadNum;
		fread(&iOccluderQuadNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iOccluderQuadNum; ++i)
		{
			RwUInt32			eFuncFlag;
			RwV3d				vPos;
			RwInt32				iPlaneType;
			RwV3d				vVertex;
			EPLOccluderType		eType = EPLOCCLUDER_QUAD;
			RwChar				acTemp[128];

			fread(&eFuncFlag, sizeof(RwUInt32), 1, pFile);

#ifndef dNTL_WORLD_TOOL_MODE
			if (eFuncFlag & EPLOCCLUDER_FUNC_PVS)
			{
				fseek(pFile, sizeof(RwV3d), SEEK_CUR);
				fseek(pFile, sizeof(RwInt32), SEEK_CUR);
				fseek(pFile, sizeof(RwV3d), SEEK_CUR);
				fseek(pFile, sizeof(RwV3d), SEEK_CUR);
				fseek(pFile, sizeof(RwV3d), SEEK_CUR);
				fseek(pFile, sizeof(RwV3d), SEEK_CUR);

				continue;
			}
#endif
			fread(&vPos, sizeof(RwV3d), 1, pFile);
			fread(&iPlaneType, sizeof(RwInt32), 1, pFile);
			fread(acTemp, sizeof(RwChar) * 128, 1, pFile);

			SPLEntityCreateParam CreateParam;
			CreateParam.pPos		= &vPos;
			CreateParam._pUserData	= &eType;

			CNtlPLOccluder_Quad* pNtlPLOccluder = static_cast<CNtlPLOccluder_Quad*>(GetSceneManager()->CreateEntity(PLENTITY_OCCLUDER, "NULL", &CreateParam));
			DBO_ASSERT(pNtlPLOccluder, "Entity create failed.");

			for (int j = 0; j < 4; ++j)
			{
				fread(&vVertex, sizeof(RwV3d), 1, pFile);
				pNtlPLOccluder->SetVertexOriginal(j, &vVertex);
			}

			pNtlPLOccluder->SetOccluderFuncFlag(eFuncFlag);
			pNtlPLOccluder->SetOccluderPlaneType(iPlaneType);
			pNtlPLOccluder->SetTriggerName(acTemp);
			pNtlPLOccluder->CalcBoundingSphere();
			pNtlWorldSector->m_vecNtlPLEntityOccluder_Quad.push_back(pNtlPLOccluder);
		}
#ifdef dNTL_WORLD_TOOL_MODE
		CNtlPLOccluder_Quad::LoadSwapFile(pNtlWorldSector->DatumPoint.x, pNtlWorldSector->DatumPoint.y, pNtlWorldSector->DatumPoint.z);
#endif

		// heat haze
		RwInt32 iHeatHazeNum;
		fread(&iHeatHazeNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iHeatHazeNum; ++i)
		{
			SPLEntityCreateParam CreateParam; CreateParam.pPos = NULL;

			CNtlPLHeatHazeObject* pNtlPLHeatHaze = static_cast<CNtlPLHeatHazeObject*>(GetSceneManager()->CreateEntity(PLENTITY_HEATHAZE, "NULL", &CreateParam));
			DBO_ASSERT(pNtlPLHeatHaze, "Entity create failed.");

			RwV3d vVertex;
			for (int j = 0; j < 4; ++j)
			{
				fread(&vVertex, sizeof(RwV3d), 1, pFile);
				pNtlPLHeatHaze->SetVertex(j, &vVertex);
			}

			RwReal fDistance;
			fread(&fDistance, sizeof(RwReal), 1, pFile);
			pNtlPLHeatHaze->SetDistance(fDistance);

			fread(pNtlPLHeatHaze->HeatHazeData(), sizeof(sNTL_HEATHAZE), 1, pFile);
			pNtlPLHeatHaze->CalcBoundingSphere();

			pNtlWorldSector->m_vecNtlPLEntityHeatHaze.push_back(pNtlPLHeatHaze);
		}
#ifdef dNTL_WORLD_TOOL_MODE
		CNtlPLHeatHazeObject::LoadSwapFile(pNtlWorldSector->DatumPoint.x, pNtlWorldSector->DatumPoint.y, pNtlWorldSector->DatumPoint.z);
#endif

		// light object
		RwInt32 iLightObjectNum;
		fread(&iLightObjectNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iLightObjectNum; ++i)
		{
			SPLLightObjectCreateParam CreateParam;

			fread(&CreateParam.rpLightType, sizeof(RpLightType), 1, pFile);
			fread(&CreateParam.bRealLight, sizeof(RwBool), 1, pFile);

			RwV3d vPos;
			fread(&vPos, sizeof(RwV3d), 1, pFile);
			CreateParam.pPos = &vPos;

			fread(&CreateParam.vRotate, sizeof(RwV3d), 1, pFile);
			fread(&CreateParam.rgbafColor, sizeof(RwRGBAReal), 1, pFile);
			fread(&CreateParam.fRadius, sizeof(RwReal), 1, pFile);
			fread(&CreateParam.fAngle, sizeof(RwReal), 1, pFile);

			// CZ-LO
			//CreateParam.bDecal		= FALSE;
			//CreateParam.bWorldLight = FALSE;
			fread(&CreateParam.bDecal, sizeof(RwBool), 1, pFile);
			fread(&CreateParam.bWorldLight, sizeof(RwBool), 1, pFile);

			CNtlPLLightObject* pNtlPLLightObject = static_cast<CNtlPLLightObject*>(GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, "NULL", &CreateParam));
			DBO_ASSERT(pNtlPLLightObject, "Entity create failed.");

			pNtlWorldSector->m_vecNtlPLEntityLightObject.push_back(pNtlPLLightObject);
		}

		// dojo
		RwInt32 iDojoNum;
		fread(&iDojoNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < iDojoNum; ++i)
		{
			SPLDojoCreateParam CreateParam;
			RwV3d vScale;
			RwV3d vRot;
			RwV3d vPos;

			fread(&vScale, sizeof(RwV3d), 1, pFile);
			fread(&vRot, sizeof(RwV3d), 1, pFile);
			fread(&vPos, sizeof(RwV3d), 1, pFile);

			CreateParam.pPos = &vPos;

			fread(&CreateParam.iDojoID, sizeof(RwInt32), 1, pFile);
			fread(&CreateParam.fFadeInTime, sizeof(RwReal), 1, pFile);
			fread(&CreateParam.fFadeOutTime, sizeof(RwReal), 1, pFile);

			for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
			{				
				fread(CreateParam.acObjectName[i], sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);
				fread(CreateParam.acEffectNameUp[i], sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);
				fread(CreateParam.acEffectNameDown[i], sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);
			}

			CNtlPLDojo* pNtlPLDojo = static_cast<CNtlPLDojo*>(GetSceneManager()->CreateEntity(PLENTITY_DOJO, "NULL", &CreateParam));
			DBO_ASSERT(pNtlPLDojo, "Entity create failed.");

			pNtlPLDojo->SetPosition(&vPos);
			pNtlPLDojo->SetRotate(vRot.x, vRot.y, vRot.z);
			pNtlPLDojo->SetScaleVector(&vScale);

			pNtlWorldSector->m_vecNtlPLEntityDojo.push_back(pNtlPLDojo);
		}
	}

	SPos.x = pNtlWorldSector->m_pWorldSector->boundingBox.inf.x;
	SPos.y = pNtlWorldSector->m_pWorldSector->boundingBox.inf.z;
	EPos.x = pNtlWorldSector->m_pWorldSector->boundingBox.sup.x;
	EPos.y = pNtlWorldSector->m_pWorldSector->boundingBox.sup.z;

	// 4 lod indexing
	RwInt32 NPosSXBy0 = (RwInt32)(EPos.x + (float)dGET_WORLD_PARAM()->WorldSizeHalf);
	RwInt32 NPosEXBy0 = (RwInt32)(SPos.x + (float)dGET_WORLD_PARAM()->WorldSizeHalf);

	for(int i = (RwInt32)SPos.y; i <= (RwInt32)EPos.y; i += dGET_WORLD_PARAM()->WorldSectorTileSize)
	{
		for(int j = NPosSXBy0; j >= NPosEXBy0; j -= dGET_WORLD_PARAM()->WorldSectorTileSize)
		{
			m = (RwInt32)((pVList->z - SPos.y) / dGET_WORLD_PARAM()->WorldSectorTileSize);
			n = (RwInt32)((pVList->x - SPos.x) / dGET_WORLD_PARAM()->WorldSectorTileSize);
			pTexCoord->u = NtlCommonTexCoord[n + m * dGET_WORLD_PARAM()->WorldSectorVertNum].u;
			pTexCoord->v = NtlCommonTexCoord[n + m * dGET_WORLD_PARAM()->WorldSectorVertNum].v;

			pPrelights++;
			pVList++;
			pTexCoord++;
		}
	}

	for(int i = 0; i < dGET_WORLD_PARAM()->WorldSectorTileNum; ++i)
	{
		for(int j = 0; j < dGET_WORLD_PARAM()->WorldSectorTileNum; ++j)
		{
			CurPtIndices[0] = j + (i * dGET_WORLD_PARAM()->WorldSectorVertNum);
			CurPtIndices[1] = CurPtIndices[0] + 1;
			CurPtIndices[3] = CurPtIndices[0] + dGET_WORLD_PARAM()->WorldSectorVertNum;
			CurPtIndices[2] = CurPtIndices[3] + 1;

			RpGeometryTriangleSetVertexIndices(pGeometry, pTList, CurPtIndices[0], CurPtIndices[2], CurPtIndices[3]);
			RpGeometryTriangleSetMaterial(pGeometry, pTList++, pMaterial);
			RpGeometryTriangleSetVertexIndices(pGeometry, pTList, CurPtIndices[2], CurPtIndices[0], CurPtIndices[1]);
			RpGeometryTriangleSetMaterial(pGeometry, pTList++, pMaterial);
		}
	}

	RpMorphTargetCalcBoundingSphere(pMorphTarget, &BoundingSphere);
	RpMorphTargetSetBoundingSphere(pMorphTarget, &BoundingSphere);

	RpGeometryUnlock(pGeometry);

	pNtlWorldSector->m_pAtomic = RpAtomicCreate();

	RpAtomicSetPipeline(pNtlWorldSector->m_pAtomic, D3D9NtlWorldSectorAtomicPipeline);
	RpAtomicSetFrame(pNtlWorldSector->m_pAtomic, dNTL_WORLD_SECTOR_LOCAL(RpWorldSectorGetWorld(pNtlWorldSector->m_pWorldSector), pParentFrame));
	RpAtomicSetGeometry(pNtlWorldSector->m_pAtomic, pGeometry, 0);

	RpGeometryDestroy(pGeometry);
	RpMaterialDestroy(pMaterial);

	// Plant Load
	GetSceneManager()->GetPlant()->OnLoad(pNtlWorldSector);

	// occluder Setting
	pNtlWorldSector->CreateOccluderProxy(1.0f, 1);

	NTL_RETURN(TRUE);
}

RwBool _RpNtlWorldSectorGeometryDeleteWithoutSwapFileSaving(CNtlWorldSector *pNtlWorldSector)
{
	NTL_FUNCTION("_RpNtlWorldSectorGeometryDeleteWithoutSwapFileSaving");

	// 	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector);
	// 
	// 	if(pNtlSector)
	// 	{
	// 		pNtlWorldSector->MaterialDestructor(FALSE);
	// 		RwFreeListFree(dNTL_WORLD_GLOBAL(pSectorFreelist), dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector));
	// 		dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector) = NULL;
	// 	}

	// 인도어
	switch (GetSceneManager()->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		{
			sNtlWorldSector*& pNtlSector = dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector);
			if(pNtlSector)
			{
				pNtlWorldSector->MaterialDestructor(FALSE);
				RwFreeListFree(dNTL_WORLD_GLOBAL(pSectorFreelist), pNtlSector);
				dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector) = NULL;
			}
		}
		break;
	case AW_RWWORLD:
		{
			pNtlWorldSector->MaterialDestructor(FALSE);
		}
		break;
	}


	NTL_RETURN(TRUE);
}

RwBool _RpNtlWorldSectorGeometryDelete(CNtlWorldSector *pNtlWorldSector)
{
	NTL_FUNCTION("_RpNtlWorldSectorGeometryDelete");

	// 	RwInt32 level = 0;
	// 	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector);
	// 
	// 	if(pNtlSector)
	// 	{
	// 		pNtlWorldSector->MaterialDestructor();
	// 		RwFreeListFree(dNTL_WORLD_GLOBAL(pSectorFreelist), dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector));
	// 		dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector) = NULL;
	// 	}

	// 인도어
	switch (GetSceneManager()->GetActiveWorldType())
	{
	case AW_HEGITHFIELD:
		{
			sNtlWorldSector*& pNtlSector = dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector);
			if(pNtlSector)
			{
				pNtlWorldSector->MaterialDestructor();
				RwFreeListFree(dNTL_WORLD_GLOBAL(pSectorFreelist), pNtlSector);
				dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector) = NULL;
			}
		}
		break;
	case AW_RWWORLD:
		{
			pNtlWorldSector->MaterialDestructor();
		}
	}


	NTL_RETURN(TRUE);
}

#ifdef dNTL_WORLD_TOOL_MODE

RwBool _RpNtlWorldSectorGeometryCreate(CNtlWorldSector *pNtlWorldSector)
{
	RpGeometry *pGeometry;
	RpMaterial *pMaterial;
	RwSurfaceProperties SurfProp;
	RpMorphTarget *pMorphTarget;
	RwSphere BoundingSphere;
	RwTexCoords *pTexCoord;
	RwV3d *pVList;
	RpTriangle *pTList;
	RwV2d SPos, EPos;
	RwInt16 CurPtIndices[4];
	RwRGBA *pPrelights;
	RwInt32 m, n;
	DWORD *pDCPalette = pNtlWorldSector->m_pClrDiffusePalette;

	// get sector pointer
	sNtlWorldSector *pNtlSector = dNTL_WORLD_LOCAL(pNtlWorldSector->m_pWorldSector, pNtlSector);

	// create geometry
	pGeometry = RpGeometryCreate(dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum,
		dGET_WORLD_PARAM()->WorldSectorPolyNum,					 
		rpGEOMETRYPOSITIONS | rpGEOMETRYPRELIT | rpGEOMETRYTEXTURED);

	pPrelights = RpGeometryGetPreLightColors(pGeometry);

	// There's only one morph target, with index 0...
	pMorphTarget = RpGeometryGetMorphTarget(pGeometry, 0);

	// must be added 2 make a rw mesh
	pMaterial = RpMaterialCreate();
	SurfProp.ambient  = 1.0f;
	SurfProp.diffuse  = 1.0f;
	SurfProp.specular = 1.0f;
	RpMaterialSetSurfaceProperties(pMaterial, &SurfProp);

	_RpNtlWorldSectorMaterialExtensionCreate(pNtlSector, pMaterial);

	// create vertices & indices
	pVList = RpMorphTargetGetVertices(pMorphTarget);
	pTList = RpGeometryGetTriangles(pGeometry);
	pTexCoord = RpGeometryGetVertexTexCoords(pGeometry, rwTEXTURECOORDINATEINDEX0);

	SPos.x = pNtlWorldSector->m_pWorldSector->boundingBox.inf.x;
	SPos.y = pNtlWorldSector->m_pWorldSector->boundingBox.inf.z;
	EPos.x = pNtlWorldSector->m_pWorldSector->boundingBox.sup.x;
	EPos.y = pNtlWorldSector->m_pWorldSector->boundingBox.sup.z;

	// open default heightfield by 24bits raw file
	RwChar Result[NTL_MAX_DIR_PATH] = {0,};
	strcpy_s(Result, dGET_WORLD_PARAM()->CurWorkingFolderName);
	strcat_s(Result, NTL_MAX_DIR_PATH, "\\Tool\\SABER\\Texture\\Heightfield\\");
	strcat_s(Result, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldRAW);
	strcat_s(Result, NTL_MAX_DIR_PATH, ".raw");

	FILE* pFileNtlWorldHF;
	::fopen_s(&pFileNtlWorldHF, Result, "rb");

	DBO_ASSERT(pFileNtlWorldHF, "file open failed. (" << Result << ")");

	// 4 lod indexing
	RwInt32 NPosSXBy0 = (RwInt32)(EPos.x + (float)dGET_WORLD_PARAM()->WorldSizeHalf);
	RwInt32 NPosEXBy0 = (RwInt32)(SPos.x + (float)dGET_WORLD_PARAM()->WorldSizeHalf);
	RwInt32 CurRealPt;

	// 고쳐야 한다 순서가 틀리다
	for(int i = (RwInt32)SPos.y; i <= (RwInt32)EPos.y; i += dGET_WORLD_PARAM()->WorldSectorTileSize)
	{
		for(int j = NPosSXBy0; j >= NPosEXBy0; j -= dGET_WORLD_PARAM()->WorldSectorTileSize)
		{
			CurRealPt = (RwInt32)(j - (float)dGET_WORLD_PARAM()->WorldSizeHalf);

			pVList->x = (RwReal)CurRealPt;
			pVList->z = (RwReal)i;

			dGET_WORLD_SLMGR()->SetVert(*pPrelights, pDCPalette, NULL, pVList->x, pVList->z, pFileNtlWorldHF);

			if(pFileNtlWorldHF)
				pVList->y = _RpNtlWorldHFLoadFromFile(pFileNtlWorldHF, pVList->x, pVList->z);
			else
				pVList->y = 0.0f;

			m = (RwInt32)((pVList->z - SPos.y) / dGET_WORLD_PARAM()->WorldSectorTileSize);
			n = (RwInt32)((pVList->x - SPos.x) / dGET_WORLD_PARAM()->WorldSectorTileSize);
			pTexCoord->u = NtlCommonTexCoord[n + m * dGET_WORLD_PARAM()->WorldSectorVertNum].u;
			pTexCoord->v = NtlCommonTexCoord[n + m * dGET_WORLD_PARAM()->WorldSectorVertNum].v;

			pDCPalette++;
			pPrelights++;
			pVList++;
			pTexCoord++;
		}
	}

	if(pFileNtlWorldHF)
		fclose(pFileNtlWorldHF);

	//
	// <3>--<1>
	//  |  / |
	//  | /  |
	// <2>--<0>
	// 

	//
	// <3>--<2>
	//  |  / |
	//  | /  |
	// <0>--<1>
	// 

	for(int i = 0; i < dGET_WORLD_PARAM()->WorldSectorTileNum; ++i)
	{
		for(int j = 0; j < dGET_WORLD_PARAM()->WorldSectorTileNum; ++j)
		{
			CurPtIndices[0] = j + (i * dGET_WORLD_PARAM()->WorldSectorVertNum);
			CurPtIndices[1] = CurPtIndices[0] + 1;
			CurPtIndices[3] = CurPtIndices[0] + dGET_WORLD_PARAM()->WorldSectorVertNum;
			CurPtIndices[2] = CurPtIndices[3] + 1;

			RpGeometryTriangleSetVertexIndices(pGeometry, pTList, CurPtIndices[0], CurPtIndices[2], CurPtIndices[3]);
			RpGeometryTriangleSetMaterial(pGeometry, pTList++, pMaterial);

			RpGeometryTriangleSetVertexIndices(pGeometry, pTList, CurPtIndices[2], CurPtIndices[0], CurPtIndices[1]);
			RpGeometryTriangleSetMaterial(pGeometry, pTList++, pMaterial);
		}
	}

	// Calculate a bounding sphere for the atomic
	RpMorphTargetCalcBoundingSphere(pMorphTarget, &BoundingSphere);
	RpMorphTargetSetBoundingSphere(pMorphTarget, &BoundingSphere);

	RpGeometryUnlock(pGeometry);

	// Create an atomic from this geometry
	pNtlWorldSector->m_pAtomic = RpAtomicCreate();

	// change pipeline
	RpAtomicSetPipeline(pNtlWorldSector->m_pAtomic, D3D9NtlWorldSectorAtomicPipeline);

	// set frame and geometry
	RpAtomicSetFrame(pNtlWorldSector->m_pAtomic, dNTL_WORLD_SECTOR_LOCAL(RpWorldSectorGetWorld(pNtlWorldSector->m_pWorldSector), pParentFrame));
	RpAtomicSetGeometry(pNtlWorldSector->m_pAtomic, pGeometry, 0);

	// Remove all local references to ref-counted objects
	RpGeometryDestroy(pGeometry);
	RpMaterialDestroy(pMaterial);

	return TRUE;
}

#endif

RwBool _RpNtlWorldSectorUpdateTexLyr(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RwInt32 IdxLyr, RwInt32 IdxMenu)
{
	switch(IdxMenu)
	{
	case 0:
		{
			if(!IdxLyr)
			{
				RwTextureDestroy(pNtlWorldSector->m_pTextureBase);
				pNtlWorldSector->m_pTextureBase = NULL;
				pNtlWorldSector->m_pTextureBase = CNtlPLResourceManager::GetInstance()->LoadTexture(CurFieldTexInfo.BLT.StrName, dGET_WORLD_PARAM()->CurDefaultTexPath);
				DBO_ASSERT(pNtlWorldSector->m_pTextureBase, "Texture load failed.");

				_RpNtlWorldSectorUpdateTexMat(CurFieldTexInfo, pNtlWorldSector, -1);
			}
			else
			{
				RwInt32 IdxDTL = IdxLyr - 1;

				if(pNtlWorldSector->m_pTextureSplat[IdxDTL]) RwTextureDestroy(pNtlWorldSector->m_pTextureSplat[IdxDTL]); pNtlWorldSector->m_pTextureSplat[IdxDTL] = NULL;
				if(pNtlWorldSector->m_pTextureAlpha[IdxDTL]) RwTextureDestroy(pNtlWorldSector->m_pTextureAlpha[IdxDTL]); pNtlWorldSector->m_pTextureAlpha[IdxDTL] = NULL;

				pNtlWorldSector->m_pTextureSplat[IdxDTL] = CNtlPLResourceManager::GetInstance()->LoadTexture(CurFieldTexInfo.DLT[IdxDTL].StrName, dGET_WORLD_PARAM()->CurDefaultTexPath);
				DBO_ASSERT(pNtlWorldSector->m_pTextureSplat[IdxDTL], "Texture load failed.");

				_RpNtlWorldSectorUpdateTexMat(CurFieldTexInfo, pNtlWorldSector, IdxDTL);
			}
		}
		break;

	case 1:
		{
			if(!IdxLyr)
			{
				RwTextureDestroy(pNtlWorldSector->m_pTextureBase);
				pNtlWorldSector->m_pTextureBase = NULL;
				pNtlWorldSector->m_pTextureBase = CNtlPLResourceManager::GetInstance()->LoadTexture("NTL", dGET_WORLD_PARAM()->CurDefaultTexPath);
				DBO_ASSERT(pNtlWorldSector->m_pTextureBase, "Texture load failed.");
			}
			else
			{
				RwInt32 IdxDTL = IdxLyr - 1;

				RwTextureDestroy(pNtlWorldSector->m_pTextureSplat[IdxDTL]);
				pNtlWorldSector->m_pTextureSplat[IdxDTL] = NULL;
			}
		}
		break;

	case 2:
		{
			if(!IdxLyr)
			{
				RwTextureDestroy(pNtlWorldSector->m_pTextureBase);
				pNtlWorldSector->m_pTextureBase = NULL;
				pNtlWorldSector->m_pTextureBase = CNtlPLResourceManager::GetInstance()->LoadTexture(CurFieldTexInfo.BLT.StrName, dGET_WORLD_PARAM()->CurDefaultTexPath);
				DBO_ASSERT(pNtlWorldSector->m_pTextureBase, "Texture load failed.");

				_RpNtlWorldSectorUpdateTexMat(CurFieldTexInfo, pNtlWorldSector, -1);
			}
			else
			{
				RwInt32 IdxDTL = IdxLyr - 1;

				if(pNtlWorldSector->m_pTextureSplat[IdxDTL])
				{
					RwTextureDestroy(pNtlWorldSector->m_pTextureSplat[IdxDTL]); pNtlWorldSector->m_pTextureSplat[IdxDTL] = NULL;
				}

				pNtlWorldSector->m_pTextureSplat[IdxDTL] = CNtlPLResourceManager::GetInstance()->LoadTexture(CurFieldTexInfo.DLT[IdxDTL].StrName, dGET_WORLD_PARAM()->CurDefaultTexPath);
				DBO_ASSERT(pNtlWorldSector->m_pTextureSplat[IdxDTL], "Texture load failed.");

				_RpNtlWorldSectorUpdateTexMat(CurFieldTexInfo, pNtlWorldSector, IdxDTL);
			}
		}
		break;
	}

	return TRUE;
}

RwBool _RpNtlWorldSectorUpdateTexMat(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RwInt32 IdxLyr)
{
	// Base Layer
	if(IdxLyr == -1)
	{
		float			UVWrap = CurFieldTexInfo.BLT.AngTexAdd;
		D3DXMATRIXA16	Temp;

		D3DXMatrixIdentity(&pNtlWorldSector->m_TextureMatBase[0]);
		pNtlWorldSector->m_TextureMatBase[0]._11 *= UVWrap;
		pNtlWorldSector->m_TextureMatBase[0]._22 *= UVWrap;
		pNtlWorldSector->m_TextureMatBase[1] = pNtlWorldSector->m_TextureMatBase[0];

		float Error = 0.5f * UVWrap;
		D3DXMatrixTranslation(&Temp, -Error, -Error, 0);
		D3DXMatrixMultiply(&pNtlWorldSector->m_TextureMatBase[1], &pNtlWorldSector->m_TextureMatBase[1], &Temp);
		D3DXMatrixRotationZ(&Temp, CurFieldTexInfo.BLT.AngTexMat);
		D3DXMatrixMultiply(&pNtlWorldSector->m_TextureMatBase[1], &pNtlWorldSector->m_TextureMatBase[1], &Temp);
		D3DXMatrixTranslation(&Temp, Error, Error, 0);
		D3DXMatrixMultiply(&pNtlWorldSector->m_TextureMatBase[1], &pNtlWorldSector->m_TextureMatBase[1], &Temp);

		pNtlWorldSector->m_TextureMatBase[1].m[2][0] = pNtlWorldSector->m_TextureMatBase[1].m[3][0];
		pNtlWorldSector->m_TextureMatBase[1].m[2][1] = pNtlWorldSector->m_TextureMatBase[1].m[3][1];
		pNtlWorldSector->m_TextureMatBase[1].m[3][0] = 0.0f;
		pNtlWorldSector->m_TextureMatBase[1].m[3][1] = 0.0f;

		pNtlWorldSector->m_TextureMatAngBase = CurFieldTexInfo.BLT.AngTexMat;
		pNtlWorldSector->m_TextureMatAddBase = CurFieldTexInfo.BLT.AngTexAdd;
	}
	else
	{
		float			UVWrap = CurFieldTexInfo.DLT[IdxLyr].AngTexAdd;
		D3DXMATRIXA16	Temp;

		D3DXMatrixIdentity(&pNtlWorldSector->m_TextureMat[IdxLyr][0]);
		pNtlWorldSector->m_TextureMat[IdxLyr][1] = pNtlWorldSector->m_TextureMat[IdxLyr][0];
		pNtlWorldSector->m_TextureMat[IdxLyr][1]._11 = UVWrap;
		pNtlWorldSector->m_TextureMat[IdxLyr][1]._22 = UVWrap;

		float Error = 0.5f * UVWrap;
		D3DXMatrixTranslation(&Temp, -Error, -Error, 0);
		D3DXMatrixMultiply(&pNtlWorldSector->m_TextureMat[IdxLyr][1], &pNtlWorldSector->m_TextureMat[IdxLyr][1], &Temp);
		D3DXMatrixRotationZ(&Temp, CurFieldTexInfo.DLT[IdxLyr].AngTexMat);
		D3DXMatrixMultiply(&pNtlWorldSector->m_TextureMat[IdxLyr][1], &pNtlWorldSector->m_TextureMat[IdxLyr][1], &Temp);
		D3DXMatrixTranslation(&Temp, Error, Error, 0);
		D3DXMatrixMultiply(&pNtlWorldSector->m_TextureMat[IdxLyr][1], &pNtlWorldSector->m_TextureMat[IdxLyr][1], &Temp);

		pNtlWorldSector->m_TextureMat[IdxLyr][1].m[2][0] = pNtlWorldSector->m_TextureMat[IdxLyr][1].m[3][0];
		pNtlWorldSector->m_TextureMat[IdxLyr][1].m[2][1] = pNtlWorldSector->m_TextureMat[IdxLyr][1].m[3][1];
		pNtlWorldSector->m_TextureMat[IdxLyr][1].m[3][0] = 0.0f;
		pNtlWorldSector->m_TextureMat[IdxLyr][1].m[3][1] = 0.0f;

		pNtlWorldSector->m_TextureMatAng[IdxLyr] = CurFieldTexInfo.DLT[IdxLyr].AngTexMat;
		pNtlWorldSector->m_TextureMatAdd[IdxLyr] = CurFieldTexInfo.DLT[IdxLyr].AngTexAdd;
	}

	return TRUE;
}

RwBool _RpNtlWorldSectorRender(RpWorldSector *pWorldSector)
{
	return TRUE;
}








RwBool _RpNtlWorldSectorGeometryLoadFromFileIndoor(CNtlWorldSector *pNtlWorldSector, FILE* pFile)
{
	if(pFile)
	{
		// map obj.; ornament
		RwInt32 NumMapObj;
		fread(&NumMapObj, sizeof(RwInt32), 1, pFile);

		RwV3d		SRT[3];
		RwChar		Name[128];
		RwBool		EnableFlag;
		RwUInt32	SID;
		RwUInt32	ObjectID;
		RwReal		VisibilityDist;
		RwUInt32	MilePostGUID = 0;

		SPLObjectCreateParam PLEntityCreateParam;
		CNtlPLEntity *pNtlPLEntity = NULL;
		pNtlWorldSector->m_vecNtlPLEntityOrnament.clear();

#ifndef dNTL_WORLD_TOOL_MODE

		sNTL_EVENT_TRI_DOODADS	NtlEventTriDoodads;

		if(NumMapObj)
		{
			NtlEventTriDoodads.FlgTriDoodads = 0;
			NtlEventTriDoodads.vecDat.reserve(10);
		}
#endif

		for(int i = 0; i < NumMapObj; ++i)
		{
			fread(Name, sizeof(RwChar) * 128, 1, pFile);

			fread(&SRT[0], sizeof(RwV3d), 1, pFile);
			fread(&SRT[1], sizeof(RwV3d), 1, pFile);
			fread(&SRT[2], sizeof(RwV3d), 1, pFile);

			DBO_ASSERT(strlen(Name), "Sector Object Load : Name Ivalid.");

			RwUInt32 uiObjectType = EPL_OBJECT_TYPE_OUTDOOR_OBJECT;
			fread(&uiObjectType, sizeof(RwUInt32), 1, pFile);

			PLEntityCreateParam.pPos		= &SRT[2];
			PLEntityCreateParam.bLoadMap	= dGET_WORLD_PARAM()->Loading;

			RwBool bLoadScheduling = CNtlPLResourceManager::GetInstance()->IsLoadScheduling();
			if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
				uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
			{
				PLEntityCreateParam.bLoadMap = TRUE;
				CNtlPLResourceManager::GetInstance()->SetLoadScheduling(FALSE);
			}		

			pNtlPLEntity = static_cast<CNtlPLEntity*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, Name, &PLEntityCreateParam));
			DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

			CNtlPLResourceManager::GetInstance()->SetLoadScheduling(bLoadScheduling);

			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetObjectType(uiObjectType);
			if (static_cast<CNtlPLObject*>(pNtlPLEntity)->GetObjectTypePtr())
			{
				static_cast<CNtlPLObject*>(pNtlPLEntity)->GetObjectTypePtr()->LoadFromFile(pFile);
			}

			if(static_cast<CNtlPLObject*>(pNtlPLEntity)->GetPlayAnim())
			{
				static_cast<CNtlPLObject*>(pNtlPLEntity)->SetPlayAnimTime(NtlRandomNumber(0.0f, 1.0f));
			}

#ifdef dNTL_WORLD_TOOL_MODE
			{
				// If current doodads visual mode is set
				//pNtlPLEntity->SetVisible(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_DOODADS));

				// doodads visibility distance culling mode setting
				static_cast<CNtlPLObject*>(pNtlPLEntity)->SetFadeEnable(TRUE);
			}
#endif

			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetScale(&SRT[0]);
			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetRotate(SRT[1].x, SRT[1].y, SRT[1].z);
			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetPosition(&SRT[2]);

			// serial id
			fread(&SID, sizeof(RwUInt32), 1, pFile);
			pNtlPLEntity->SetSID(SID);

			// object id
			fread(&ObjectID, sizeof(RwUInt32), 1, pFile);
			static_cast<CNtlPLObject*>(pNtlPLEntity)->SetObjectSerialID(ObjectID);

			// object name index
			DWORD ObjNameIdx;
			fread(&ObjNameIdx, sizeof(DWORD), 1, pFile);
			pNtlPLEntity->SetObjNameIdx(ObjNameIdx);

			// visibility distance
			fread(&VisibilityDist, sizeof(RwReal), 1, pFile);
			pNtlPLEntity->SetVisibleCullingDistance(VisibilityDist);

			// enable trigger
			fread(&EnableFlag, sizeof(RwBool), 1, pFile);
			if(EnableFlag)
			{
				pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_TRIGGER);

#ifndef dNTL_WORLD_TOOL_MODE
				NtlEventTriDoodads.vecDat.push_back(pNtlPLEntity);
#endif
			}


			// enable selection
			fread(&EnableFlag, sizeof(RwBool), 1, pFile);
			if(EnableFlag)
			{
				pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_SELECTION);
			}

			pNtlWorldSector->m_vecNtlPLEntityOrnament.push_back(pNtlPLEntity);

			// enable alterable; dynamic obj.s.
			fread(&EnableFlag, sizeof(RwBool), 1, pFile);
			if(EnableFlag)
			{
				pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_ALTERABLE);
			}

			// enable portal
			fread(&EnableFlag, sizeof(RwBool), 1, pFile);
			if(EnableFlag)
			{
				pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_PORTAL);
			}

			// enable path engine collision
			fread(&EnableFlag, sizeof(RwBool), 1, pFile);
			if(EnableFlag)
			{
				pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_PECOLLISION);
			}

			// enable shadow prop
			fread(&EnableFlag, sizeof(RwBool), 1, pFile);
			if(EnableFlag)
			{
				pNtlPLEntity->SetFlags(pNtlPLEntity->GetFlags() | NTL_PLEFLAG_SHADOW_PROP);
			}

			// object PS map
			static_cast<CNtlPLObject*>(pNtlPLEntity)->LoadPSMap(pFile);

			if(!GetNtlResourceManager()->IsLoadScheduling())
			{
				static_cast<CNtlPLObject*>(pNtlPLEntity)->SetAtomicPSMap();
			}

			// milepost GUID
			fread(&EnableFlag, sizeof(RwBool), 1, pFile);
			if(EnableFlag)
			{
				fread(&MilePostGUID, sizeof(RwUInt32), 1, pFile);
				static_cast<CNtlPLObject*>(pNtlPLEntity)->SetMilepostID(MilePostGUID);
			}

			// attach lightojbect
			static_cast<CNtlPLObject*>(pNtlPLEntity)->LoadLightObjectAttachData(pFile);

			// Illumination
			if(GetNtlResourceManager()->IsLoadScheduling())
				static_cast<CNtlPLObject*>(pNtlPLEntity)->LoadSchedulingLightmapProp(pFile);
			else
				static_cast<CNtlPLObject*>(pNtlPLEntity)->LoadLightmap(pFile);
		}
#ifdef dNTL_WORLD_TOOL_MODE
		CNtlPLObject::LoadSwapFile(pNtlWorldSector->DatumPoint.x, pNtlWorldSector->DatumPoint.y, pNtlWorldSector->DatumPoint.z);
#endif

#ifndef dNTL_WORLD_TOOL_MODE

		if(NtlEventTriDoodads.vecDat.size())
		{
			CNtlPLEventGenerator::OnTriDoodadsEvent(&NtlEventTriDoodads);
			NtlEventTriDoodads.vecDat.clear();
		}

#endif

		// water
		RwBool BooleanFlag;
		::fread(&BooleanFlag, sizeof(RwBool), 1, pFile);

		if(BooleanFlag)
		{
			sSECTOR_WATER_ATTR SectorWaterAttr;
			::fread(&SectorWaterAttr._Height,						sizeof(RwReal), 1, pFile);
			::fread(&SectorWaterAttr._RGBA,							sizeof(RwRGBA), 1, pFile);
			::fread(&SectorWaterAttr._RenderstateSrc,				sizeof(DWORD), 1, pFile);
			::fread(&SectorWaterAttr._RenderstateDst,				sizeof(DWORD), 1, pFile);
			::fread(&SectorWaterAttr._RotTrans,						sizeof(RwReal), 1, pFile);
			::fread(&SectorWaterAttr._SclTrans,						sizeof(RwReal), 1, pFile);
			::fread(&SectorWaterAttr._DepthMapSize,					sizeof(RwInt32), 1, pFile);
			::fread(&SectorWaterAttr._DepthMapHeightVariation[0],	sizeof(RwReal), 1, pFile);
			::fread(&SectorWaterAttr._DepthMapHeightVariation[1],	sizeof(RwReal), 1, pFile);
			::fread(&SectorWaterAttr._IdxSequence,					sizeof(RwInt32), 1, pFile);

			CNtlPLVisualManager* pSceneManager = static_cast<CNtlPLVisualManager*>(GetSceneManager());
			pSceneManager->GetWater()->OnCreate(pNtlWorldSector, SectorWaterAttr);

			::fread(&BooleanFlag, sizeof(RwBool), 1, pFile);
			if(BooleanFlag)
			{
				pNtlWorldSector->m_pWater->_pDepthMap =	CNtlPLResourceManager::GetInstance()->CreateTexture("DepthMap",
					SectorWaterAttr._DepthMapSize,
					SectorWaterAttr._DepthMapSize,
					32,
					rwRASTERFORMAT8888);

				RwTextureSetAddressing(pNtlWorldSector->m_pWater->_pDepthMap, rwTEXTUREADDRESSCLAMP);
				RwTextureSetFilterMode(pNtlWorldSector->m_pWater->_pDepthMap, rwFILTERLINEAR);

				BYTE* pTmp = NTL_NEW BYTE [SectorWaterAttr._DepthMapSize * SectorWaterAttr._DepthMapSize];
				::fread(pTmp, sizeof(BYTE) * SectorWaterAttr._DepthMapSize * SectorWaterAttr._DepthMapSize, 1, pFile);
				CNtlPLResourceManager::GetInstance()->LoadAlphaBits(pNtlWorldSector->m_pWater->_pDepthMap, pTmp);
				NTL_ARRAY_DELETE(pTmp);
			}
		}

		// shadow
		::fread(&BooleanFlag, sizeof(RwBool), 1, pFile);
		if(BooleanFlag)
		{
			CNtlWorldShadow* pNtlWorldShadow = pNtlWorldSector->m_pNtlWorldShadow;

			::fread(&pNtlWorldShadow->m_rgbaShadow, sizeof(RwRGBA), 1, pFile);
			::fread(&pNtlWorldShadow->m_CamViewWindowX, sizeof(RwReal), 1, pFile);
			::fread(&pNtlWorldShadow->m_matLight, sizeof(RwMatrix), 1, pFile);

			RwInt32 Size;
			::fread(&Size, sizeof(RwInt32), 1, pFile);

			pNtlWorldShadow->m_pTexShadow = CNtlPLResourceManager::GetInstance()->CreateTexture("ShadowMap",
				Size,
				Size,
				32,
				rwRASTERFORMAT8888);

			RwTextureSetFilterMode(pNtlWorldShadow->m_pTexShadow, rwFILTERLINEAR);
			RwTextureSetAddressing(pNtlWorldShadow->m_pTexShadow, rwTEXTUREADDRESSCLAMP);

			CNtlPLResourceManager::GetInstance()->LoadTexture(pNtlWorldShadow->m_pTexShadow, pFile);
			DBO_ASSERT(pNtlWorldShadow->m_pTexShadow, "Texture load failed.");
		}

		// effects
		RwInt32 NumEff;
		fread(&NumEff, sizeof(RwInt32), 1, pFile);

		for(int i = 0; i < NumEff; ++i)
		{
			fread(Name, sizeof(RwChar) * 128, 1, pFile);
			fread(&SRT[0], sizeof(RwV3d), 1, pFile);
			fread(&SRT[1], sizeof(RwV3d), 1, pFile);
			fread(&SRT[2], sizeof(RwV3d), 1, pFile);

			PLEntityCreateParam.pPos = &SRT[2];
			pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_EFFECT, Name, &PLEntityCreateParam);
			DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

#ifdef dNTL_WORLD_TOOL_MODE

			pNtlPLEntity->SetVisible(CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_EFFECTS));

			if(dGET_CUR_PALETTE() == ePM_EFFECT)
			{
				(static_cast<CNtlInstanceEffect*>(pNtlPLEntity))->SetVisibleBoundingVolume(TRUE);
			}
			else
			{
				(static_cast<CNtlInstanceEffect*>(pNtlPLEntity))->SetVisibleBoundingVolume(FALSE);
			}

#endif

			pNtlPLEntity->SetScale(SRT[0].x);
			pNtlPLEntity->SetRotate(SRT[1].x, SRT[1].y, SRT[1].z);
			pNtlPLEntity->SetPosition(&SRT[2]);

			// visibility distance
			fread(&VisibilityDist, sizeof(RwReal), 1, pFile);
			pNtlPLEntity->SetVisibleCullingDistance(VisibilityDist);

			pNtlWorldSector->m_vecNtlPLEntityEffect.push_back(pNtlPLEntity);
		}
#ifdef dNTL_WORLD_TOOL_MODE
		CNtlInstanceEffect::LoadSwapFile(pNtlWorldSector->DatumPoint.x, pNtlWorldSector->DatumPoint.y, pNtlWorldSector->DatumPoint.z);
#endif

		// enviroment sound		
		RwInt32 NumSound;
		fread(&NumSound, sizeof(RwInt32), 1, pFile);

		for(int i = 0; i < NumSound; ++i)
		{
			RwV3d	Pos;		
			RwReal	Vol;
			RwReal	RadiusMin, RadiusMax;
			RwChar  NameSound[32] = {0,};

			fread(NameSound, sizeof(RwChar) * 32, 1, pFile);
			fread(&Vol, sizeof(RwReal), 1, pFile);
			fread(&RadiusMin, sizeof(RwReal), 1, pFile);
			fread(&RadiusMax, sizeof(RwReal), 1, pFile);
			fread(&Pos, sizeof(RwV3d), 1, pFile);

			PLEntityCreateParam.pPos = &Pos;

			CNtlPLEntity *pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_SOUND, "NULL", &PLEntityCreateParam);
			DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

			static_cast<CNtlPLSound*>(pNtlPLEntity)->GetVolume() = Vol;
			static_cast<CNtlPLSound*>(pNtlPLEntity)->GetRadius(TRUE) = RadiusMin;
			static_cast<CNtlPLSound*>(pNtlPLEntity)->GetRadius(FALSE) = RadiusMax;
			static_cast<CNtlPLSound*>(pNtlPLEntity)->PlaySound(NameSound);
			pNtlWorldSector->m_vecNtlPLEntitySound.push_back(pNtlPLEntity);
		}

		// bgm sound
		// CZ-BGM
		RwInt32 NumSoundBGM;
		fread(&NumSoundBGM, sizeof(RwInt32), 1, pFile);

		for(int i = 0; i < NumSoundBGM; ++i)
		{
			RwV3d	Pos;		
			//RwReal	Vol;
			RwReal	Radius;
			RwChar  NameSound[32] = {0,};

			fread(NameSound, sizeof(RwChar) * 32, 1, pFile);
			//fread(&Vol, sizeof(RwReal), 1, pFile);
			fread(&Radius, sizeof(RwReal), 1, pFile);
			fread(&Pos, sizeof(RwV3d), 1, pFile);

			PLEntityCreateParam.pPos = &Pos;

			CNtlPLEntity *pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_SOUND_BGM, "NULL", &PLEntityCreateParam);
			DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

			//static_cast<CNtlPLSoundBGM*>(pNtlPLEntity)->GetVolume() = Vol;
			static_cast<CNtlPLSoundBGM*>(pNtlPLEntity)->GetRadius() = Radius;
			static_cast<CNtlPLSoundBGM*>(pNtlPLEntity)->PlaySound(NameSound);
			pNtlWorldSector->m_vecNtlPLEntitySoundBGM.push_back(pNtlPLEntity);
		}

		// soft edged spot light
		// 		RwV3d		SSPos;
		// 		RwV3d		SSRot;
		// 		RwInt32		SSNum = 0;
		// 		RwRGBAReal	SSClr;
		// 		RwReal		SSAng, SSRad;
		// 
		// 		fread(&SSNum, sizeof(RwInt32), 1, pFile);
		// 
		// 		for(int i = 0; i < SSNum; ++i)
		// 		{
		// 			fread(&SSPos, sizeof(RwV3d), 1, pFile);
		// 			fread(&SSRot, sizeof(RwV3d), 1, pFile);
		// 			fread(&SSClr, sizeof(RwRGBAReal), 1, pFile);
		// 			fread(&SSAng, sizeof(RwReal), 1, pFile);
		// 			fread(&SSRad, sizeof(RwReal), 1, pFile);
		// 
		// 			SPLEntityCreateParam CreateParam;
		// 			CreateParam.pPos = &SSPos;
		// 
		// 			CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_SPOT_LIGHT, NTL_PLEN_SPOT_LIGHT, &CreateParam);
		// 			DBO_ASSERT(pNtlPLEntity, "Entity create failed.");
		// 
		// 			CNtlPLSpot* pSpotEntity = static_cast<CNtlPLSpot*>(pNtlPLEntity);
		// 
		// 			pSpotEntity->SetColor(SSClr);
		// 			pSpotEntity->SetAngle(SSAng);
		// 			pSpotEntity->SetRadius(SSRad);
		// 			pSpotEntity->SetPosition(&SSPos);
		// 			pSpotEntity->SetRotate(SSRot.x, SSRot.y, SSRot.z);
		// 
		// 
		// 			pNtlWorldSector->m_vecNtlPLEntitySS.push_back(pNtlPLEntity);
		// 		}

		// Sector Cull
		fread(&pNtlWorldSector->m_IsCulled, sizeof(RwBool), 1, pFile);

		// Tile Transparency
		fread(&pNtlWorldSector->m_IsTileTransparency, sizeof(RwBool), 1, pFile);

		// Shoreline
		RwBool IsThereShoreLine;
		fread(&IsThereShoreLine, sizeof(RwBool), 1, pFile);
		if(IsThereShoreLine)
		{
			SPLEntityCreateParam	PLEntityCreateParam;
			RwV3d					Pos;

			Pos.x						= pNtlWorldSector->DatumPoint.x;
			Pos.y						= pNtlWorldSector->DatumPoint.y;
			Pos.z						= pNtlWorldSector->DatumPoint.z;
			PLEntityCreateParam.pPos	= &Pos;

			pNtlWorldSector->m_pNtlPLEntityShoreLine = static_cast<CNtlPLShoreLine*>(GetSceneManager()->CreateEntity(PLENTITY_SHORELINE, NTL_PLEN_SHORELINE, &PLEntityCreateParam));			
			DBO_ASSERT(pNtlWorldSector->m_pNtlPLEntityShoreLine, "Entity create failed.");

			static_cast<CNtlPLShoreLine*>(pNtlWorldSector->m_pNtlPLEntityShoreLine)->LoadFromFile(pFile);
		}

		// Decals
		CNtlPLWorldDecal*	pNtlPLWorldDecal;
		RwInt32				EntityNum;
		sDECAL_PROP_PARAM	DecalPropParam;
		SPLDecalCreateParam CreateParam;

		fread(&EntityNum, sizeof(RwInt32), 1, pFile);

		for(int i = 0; i < EntityNum; ++i)
		{
			CNtlPLWorldDecal::Load(DecalPropParam, pFile);

			CreateParam.pTexName	= DecalPropParam._TexName;
			CreateParam.pTexPath	= ".\\Texture\\NtlWe\\Decal\\";

			RwReal Width	= DecalPropParam._SizeX;
			RwReal Height	= DecalPropParam._SizeZ;
			if(Width > Height)
			{
				CreateParam.fIntersectionRadius = Width;
			}
			else
			{
				CreateParam.fIntersectionRadius = Height;
			}

			CreateParam.vSize.x	= DecalPropParam._SizeX;
			CreateParam.vSize.z	= DecalPropParam._SizeZ;

			CreateParam.fScale				= DecalPropParam._TexScale;
			CreateParam.fVisibleSquaredDist	= DecalPropParam._VisibilityDist;

			string CurTexName = DecalPropParam._TexName;
			string W_Prefix("w_");
			string C_Prefix("c_");

			if(-1 == CurTexName.find_first_of(W_Prefix) && -1 == CurTexName.find_first_of(C_Prefix))
			{
				DBO_TRACE(FALSE, "Decal textures should've got prefix; w_, c_");
			}
			else
			{
				RwInt32 IdxWPrefix = CurTexName.find_first_of(W_Prefix);
				RwInt32 IdxCPrefix = CurTexName.find_first_of(C_Prefix);

				if(!IdxWPrefix)
				{
					CreateParam.rwWrapType = rwTEXTUREADDRESSWRAP;
				}
				else if(!IdxCPrefix)
				{
					CreateParam.rwWrapType = rwTEXTUREADDRESSCLAMP;
				}
				else
				{
					DBO_TRACE(FALSE, "Decal textures should've got prefix; w_, c_");
				}
			}

			if(DecalPropParam._DecalMode == 0)
			{
				CreateParam.nFlagEffect = E_FLAG_DECAL_RENDER_WORLD;
			}
			else if(DecalPropParam._DecalMode == 1)
			{
				CreateParam.nFlagEffect = E_FLAG_DECAL_RENDER_OBJECT;
			}
			else if(DecalPropParam._DecalMode == 2)
			{
				CreateParam.nFlagEffect = E_FLAG_DECAL_RENDER_WORLD | E_FLAG_DECAL_RENDER_OBJECT;
			}
			else if(DecalPropParam._DecalMode == 3)
			{
				CreateParam.nFlagEffect = E_FLAG_DECAL_RENDER_OBJECT | E_FLAG_DECAL_WATER;
			}

			CreateParam.eDecalType = DECAL_TERRAIN;

			pNtlPLWorldDecal = static_cast<CNtlPLWorldDecal*>(GetSceneManager()->CreateEntity(PLENTITY_DECAL, "NULL", &CreateParam));
			DBO_ASSERT(pNtlPLWorldDecal, "Entity create failed.");

			pNtlWorldSector->InsertIn2World(pNtlPLWorldDecal, 4);

			pNtlPLWorldDecal->SetRotate(DecalPropParam._TexAngle);
			pNtlPLWorldDecal->SetColor(DecalPropParam._RGBA.red, DecalPropParam._RGBA.green, DecalPropParam._RGBA.blue);
			pNtlPLWorldDecal->SetAlpha(DecalPropParam._RGBA.alpha);
			pNtlPLWorldDecal->SetAlphaBlend(TRUE, rwBLENDSRCALPHA, rwBLENDINVSRCALPHA);
			pNtlPLWorldDecal->SetPosition(&DecalPropParam._Pos);
			pNtlPLWorldDecal->SetName(DecalPropParam._TexName);

			if(DecalPropParam._UVAniCheck)
			{
				if(DecalPropParam._UVAniPatternIdx == 0)
				{
					SNtlPrtStdEmitterPrtUVAnim EmitterUVAnim;
					EmitterUVAnim.vDir.x		= 0.0f;
					EmitterUVAnim.vDir.y		= 0.0f;
					EmitterUVAnim.vDir.z		= 1.0f;
					EmitterUVAnim.fVelocity		= DecalPropParam._UVAniSpeed;
					EmitterUVAnim.fMaxOffset	= 0.0f;
					pNtlPLWorldDecal->SetUVAnimAction(EmitterUVAnim);
				}
				else if(DecalPropParam._UVAniPatternIdx == 1)
				{
					SNtlPrtStdEmitterPrtUVAnim EmitterUVAnim;
					EmitterUVAnim.vDir.x		= 0.0f;
					EmitterUVAnim.vDir.y		= 0.0f;
					EmitterUVAnim.vDir.z		= 1.0f;
					EmitterUVAnim.fVelocity		= DecalPropParam._UVAniSpeed;
					EmitterUVAnim.fMaxOffset	= DecalPropParam._UVAniOffset;
					pNtlPLWorldDecal->SetUVAnimAction(EmitterUVAnim);
				}
				else if(DecalPropParam._UVAniPatternIdx == 2)
				{
					pNtlPLWorldDecal->Set2DRoateAction(DecalPropParam._UVAniOffset, DecalPropParam._UVAniSpeed);
				}
				else if(DecalPropParam._UVAniPatternIdx == 3)
				{
					pNtlPLWorldDecal->SetSizeAction(DecalPropParam._TexScale, DecalPropParam._UVAniOffset, DecalPropParam._UVAniSpeed);
				}
			}
		}

		// plant
		RwInt32 iPlantSetNum;
		fread(&iPlantSetNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < iPlantSetNum; ++i)
		{
			sSECTOR_PLANT_SET_ATTR* pPlantSet = NTL_NEW sSECTOR_PLANT_SET_ATTR;
			RwInt32					iPlantObjNum;			

			fread(&pPlantSet->iSetIdx, sizeof(RwInt32), 1, pFile);
			fread(&iPlantObjNum, sizeof(RwInt32), 1, pFile);

			for (int j = 0; j < iPlantObjNum; ++j)
			{
				sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR* pPlantObj = NTL_NEW sSECTOR_PLANT_SET_ATTR::sSECTOR_PLANT_OBJ_ATTR;

				fread(&pPlantObj->iTileIdx, sizeof(RwInt32), 1, pFile);

				pPlantSet->vecPlantObj.push_back(pPlantObj);
			}			
			pNtlWorldSector->m_vecPlantSet.push_back(pPlantSet);
		}

		// world light sphere
		RwInt32 iWLSphereNum;
		fread(&iWLSphereNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iWLSphereNum; ++i)
		{
			RwV3d				vPos;
			RwRGBA				clr;
			RwReal				fRadius;
			EPLWorldLightType	eType = EPLWORLDLIGHT_SPHERE;

			fread(&vPos, sizeof(RwV3d), 1, pFile);
			fread(&clr, sizeof(RwRGBA), 1, pFile);
			fread(&fRadius, sizeof(RwReal), 1, pFile);

			SPLWorldLightCreateParam_Sphere CreateParam;
			CreateParam.pPos		= &vPos;
			CreateParam._pUserData	= &eType;
			CreateParam.uiRed		= clr.red;
			CreateParam.uiGreen		= clr.green;
			CreateParam.uiBlue		= clr.blue;
			CreateParam.fRadius		= fRadius;

			CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_WORLD_LIGHT, "NULL", &CreateParam);
			DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

			pNtlWorldSector->m_vecNtlPLEntityWorldLight_Sphere.push_back(pNtlPLEntity);
		}

		// world light box
		RwInt32 iWLBoxNum;
		fread(&iWLBoxNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iWLBoxNum; ++i)
		{
			RwV3d				vPos;
			RwRGBA				clr;
			RwV3d				vRot;
			RwV3d				vScale;
			EPLWorldLightType	eType = EPLWORLDLIGHT_BOX;

			fread(&vPos, sizeof(RwV3d), 1, pFile);
			fread(&clr, sizeof(RwRGBA), 1, pFile);
			fread(&vRot, sizeof(RwV3d), 1, pFile);
			fread(&vScale, sizeof(RwV3d), 1, pFile);	

			SPLWorldLightCreateParam_Box CreateParam;
			CreateParam.pPos		= &vPos;
			CreateParam._pUserData	= &eType;
			CreateParam.uiRed		= clr.red;
			CreateParam.uiGreen		= clr.green;
			CreateParam.uiBlue		= clr.blue;
			CreateParam.vScale		= vScale;
			CreateParam.vRotate		= vRot;

			CNtlPLEntity* pNtlPLEntity = GetSceneManager()->CreateEntity(PLENTITY_WORLD_LIGHT, "NULL", &CreateParam);
			DBO_ASSERT(pNtlPLEntity, "Entity create failed.");

			pNtlWorldSector->m_vecNtlPLEntityWorldLight_Box.push_back(pNtlPLEntity);
		}

		// occluder
		RwInt32 iOccluderQuadNum;
		fread(&iOccluderQuadNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iOccluderQuadNum; ++i)
		{
			RwUInt32			eFuncFlag;
			RwV3d				vPos;
			RwInt32				iPlaneType;
			RwV3d				vVertex;
			EPLOccluderType		eType = EPLOCCLUDER_QUAD;
			RwChar				acTemp[128];

			fread(&eFuncFlag, sizeof(RwUInt32), 1, pFile);

#ifndef dNTL_WORLD_TOOL_MODE
			if (eFuncFlag & EPLOCCLUDER_FUNC_PVS)
			{
				fseek(pFile, sizeof(RwV3d), SEEK_CUR);
				fseek(pFile, sizeof(RwInt32), SEEK_CUR);
				fseek(pFile, sizeof(RwV3d), SEEK_CUR);
				fseek(pFile, sizeof(RwV3d), SEEK_CUR);
				fseek(pFile, sizeof(RwV3d), SEEK_CUR);
				fseek(pFile, sizeof(RwV3d), SEEK_CUR);

				continue;
			}
#endif
			fread(&vPos, sizeof(RwV3d), 1, pFile);
			fread(&iPlaneType, sizeof(RwInt32), 1, pFile);
			fread(acTemp, sizeof(RwChar) * 128, 1, pFile);

			SPLEntityCreateParam CreateParam;
			CreateParam.pPos		= &vPos;
			CreateParam._pUserData	= &eType;

			CNtlPLOccluder_Quad* pNtlPLOccluder = static_cast<CNtlPLOccluder_Quad*>(GetSceneManager()->CreateEntity(PLENTITY_OCCLUDER, "NULL", &CreateParam));
			DBO_ASSERT(pNtlPLOccluder, "Entity create failed.");

			for (int j = 0; j < 4; ++j)
			{
				fread(&vVertex, sizeof(RwV3d), 1, pFile);
				pNtlPLOccluder->SetVertexOriginal(j, &vVertex);
			}

			pNtlPLOccluder->SetOccluderFuncFlag(eFuncFlag);
			pNtlPLOccluder->SetOccluderPlaneType(iPlaneType);
			pNtlPLOccluder->SetTriggerName(acTemp);
			pNtlPLOccluder->CalcBoundingSphere();
			pNtlWorldSector->m_vecNtlPLEntityOccluder_Quad.push_back(pNtlPLOccluder);
		}
#ifdef dNTL_WORLD_TOOL_MODE
		CNtlPLOccluder_Quad::LoadSwapFile(pNtlWorldSector->DatumPoint.x, pNtlWorldSector->DatumPoint.y, pNtlWorldSector->DatumPoint.z);
#endif

		// heathaze
		RwInt32 iHeatHazeNum;
		fread(&iHeatHazeNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iHeatHazeNum; ++i)
		{
			SPLEntityCreateParam CreateParam; CreateParam.pPos = NULL;

			CNtlPLHeatHazeObject* pNtlPLHeatHaze = static_cast<CNtlPLHeatHazeObject*>(GetSceneManager()->CreateEntity(PLENTITY_HEATHAZE, "NULL", &CreateParam));
			DBO_ASSERT(pNtlPLHeatHaze, "Entity create failed.");

			RwV3d vVertex;
			for (int j = 0; j < 4; ++j)
			{
				fread(&vVertex, sizeof(RwV3d), 1, pFile);
				pNtlPLHeatHaze->SetVertex(j, &vVertex);
			}

			RwReal fDistance;
			fread(&fDistance, sizeof(RwReal), 1, pFile);
			pNtlPLHeatHaze->SetDistance(fDistance);

			fread(pNtlPLHeatHaze->HeatHazeData(), sizeof(sNTL_HEATHAZE), 1, pFile);
			pNtlPLHeatHaze->CalcBoundingSphere();

			pNtlWorldSector->m_vecNtlPLEntityHeatHaze.push_back(pNtlPLHeatHaze);
		}
#ifdef dNTL_WORLD_TOOL_MODE
		CNtlPLHeatHazeObject::LoadSwapFile(pNtlWorldSector->DatumPoint.x, pNtlWorldSector->DatumPoint.y, pNtlWorldSector->DatumPoint.z);
#endif

		// light object
		RwInt32 iLightObjectNum;
		fread(&iLightObjectNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iLightObjectNum; ++i)
		{
			SPLLightObjectCreateParam CreateParam;

			fread(&CreateParam.rpLightType, sizeof(RpLightType), 1, pFile);
			fread(&CreateParam.bRealLight, sizeof(RwBool), 1, pFile);

			RwV3d vPos;
			fread(&vPos, sizeof(RwV3d), 1, pFile);
			CreateParam.pPos = &vPos;

			fread(&CreateParam.vRotate, sizeof(RwV3d), 1, pFile);
			fread(&CreateParam.rgbafColor, sizeof(RwRGBAReal), 1, pFile);
			fread(&CreateParam.fRadius, sizeof(RwReal), 1, pFile);
			fread(&CreateParam.fAngle, sizeof(RwReal), 1, pFile);

			// CZ-LO
			//CreateParam.bDecal		= FALSE;
			//CreateParam.bWorldLight = FALSE;
			fread(&CreateParam.bDecal, sizeof(RwBool), 1, pFile);
			fread(&CreateParam.bWorldLight, sizeof(RwBool), 1, pFile);

			CNtlPLLightObject* pNtlPLLightObject = static_cast<CNtlPLLightObject*>(GetSceneManager()->CreateEntity(PLENTITY_LIGHT_OBJECT, "NULL", &CreateParam));
			DBO_ASSERT(pNtlPLLightObject, "Entity create failed.");

			pNtlWorldSector->m_vecNtlPLEntityLightObject.push_back(pNtlPLLightObject);
		}

		// decal volume
		RwInt32 iDecalVolumeNum;
		fread(&iDecalVolumeNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < iDecalVolumeNum; ++i)
		{
			SPLDecalVolumeCreateParam CreateParam;

			fread(CreateParam.acTextureName, sizeof(RwChar) * 128, 1, pFile);

			RwV3d vPos;
			fread(&vPos, sizeof(RwV3d), 1, pFile);
			CreateParam.pPos = &vPos;

			fread(&CreateParam.vRotation, sizeof(RwV3d), 1, pFile);
			fread(&CreateParam.vScale, sizeof(RwV3d), 1, pFile);
			fread(&CreateParam.clrDiffuse, sizeof(RwRGBA), 1, pFile);
			fread(&CreateParam.fAppendOffset, sizeof(RwReal), 1, pFile);
			fread(&CreateParam.fVisibleDist, sizeof(RwReal), 1, pFile);
			fread(&CreateParam.uiDecalVolumeFlags, sizeof(RwUInt32), 1, pFile);

			CNtlPLDecalVolume* pNtlPLDecalVolume = static_cast<CNtlPLDecalVolume*>(GetSceneManager()->CreateEntity(PLENTITY_DECAL_VOLUME, "NULL", &CreateParam));
			DBO_ASSERT(pNtlPLDecalVolume, "Entity create failed.");

			pNtlWorldSector->m_vecNtlPLEntityDecalVolume.push_back(pNtlPLDecalVolume);
		}

		// game property
		RwInt32 iGamePropertyNum;
		fread(&iGamePropertyNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < iGamePropertyNum; ++i)
		{
			SPLGamePropertyCreateParam CreateParam;

			fread(&CreateParam.eShapeType, sizeof(RwUInt32), 1, pFile);

			RwV3d vPos;
			fread(&vPos, sizeof(RwV3d), 1, pFile);
			CreateParam.pPos = &vPos;

			fread(&CreateParam.vSize, sizeof(RwV3d), 1, pFile);
			fread(&CreateParam.uiProperty, sizeof(RwUInt32), 1, pFile);

			CNtlPLGameProperty* pNtlPLGameProperty = static_cast<CNtlPLGameProperty*>(GetSceneManager()->CreateEntity(PLENTITY_GAME_PROPERTY, "NULL", &CreateParam));
			DBO_ASSERT(pNtlPLGameProperty, "Entity create failed.");

			pNtlWorldSector->m_vecNtlPLEntityGameProperty.push_back(pNtlPLGameProperty);
		}

		// dojo
		RwInt32 iDojoNum;
		fread(&iDojoNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < iDojoNum; ++i)
		{
			SPLDojoCreateParam CreateParam;
			RwV3d vScale;
			RwV3d vRot;
			RwV3d vPos;

			fread(&vScale, sizeof(RwV3d), 1, pFile);
			fread(&vRot, sizeof(RwV3d), 1, pFile);
			fread(&vPos, sizeof(RwV3d), 1, pFile);

			CreateParam.pPos = &vPos;

			fread(&CreateParam.iDojoID, sizeof(RwInt32), 1, pFile);
			fread(&CreateParam.fFadeInTime, sizeof(RwReal), 1, pFile);
			fread(&CreateParam.fFadeOutTime, sizeof(RwReal), 1, pFile);

			for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
			{				
				fread(CreateParam.acObjectName[i], sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);
				fread(CreateParam.acEffectNameUp[i], sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);
				fread(CreateParam.acEffectNameDown[i], sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);
			}

			CNtlPLDojo* pNtlPLDojo = static_cast<CNtlPLDojo*>(GetSceneManager()->CreateEntity(PLENTITY_DOJO, "NULL", &CreateParam));
			DBO_ASSERT(pNtlPLDojo, "Entity create failed.");

			pNtlPLDojo->SetPosition(&vPos);
			pNtlPLDojo->SetRotate(vRot.x, vRot.y, vRot.z);
			pNtlPLDojo->SetScaleVector(&vScale);

			pNtlWorldSector->m_vecNtlPLEntityDojo.push_back(pNtlPLDojo);
		}
	}

	GetSceneManager()->GetPlant()->OnLoad(pNtlWorldSector);

	pNtlWorldSector->CreateOccluderProxy(1.0f, 1);

	return TRUE;
}
RwBool _RpNtlWorldSectorGeometrySaveIntoFileIndoor(CNtlWorldSector *pNtlWorldSector, FILE* pFile)
{
	/*
	WorldFileFormat - SectorInfo
	SectorInfo를 File로 Save하는 부분이다.
	*/

	NTL_FUNCTION("_RpNtlWorldSectorGeometrySaveIntoFile");

	RwUInt32			MilePostGUID = 0;
	RwBool				TrueFlag	= TRUE;
	RwBool				ZeroFlag	= FALSE;

	if(pFile)
	{
		// map obj.; ornament
		RwInt32 NumMapObj = (RwInt32)pNtlWorldSector->m_vecNtlPLEntityOrnament.size();
		fwrite(&NumMapObj, sizeof(RwInt32), 1, pFile);

		CNtlPLObject	*pNtlPLObject;
		RwV3d			SRT[3];
		RwChar			Name[128];
		RwBool			EnableFlag;
		RwUInt32		SID;
		RwUInt32		ObjectID;
		RwReal			VisibilityDist;
		DWORD			ObjNameIdx;

		for(int i = 0; i < NumMapObj; ++i)
		{
			pNtlPLObject = static_cast<CNtlPLObject*>(pNtlWorldSector->m_vecNtlPLEntityOrnament[i]);
			CNtlMath::MathRwV3dAssign(&SRT[0], pNtlPLObject->GetScale()->x, pNtlPLObject->GetScale()->y, pNtlPLObject->GetScale()->z);
			CNtlMath::MathRwV3dAssign(&SRT[1], pNtlPLObject->GetRotate()->x, pNtlPLObject->GetRotate()->y, pNtlPLObject->GetRotate()->z);
			CNtlMath::MathRwV3dAssign(&SRT[2], pNtlPLObject->GetPosition().x, pNtlPLObject->GetPosition().y, pNtlPLObject->GetPosition().z);
			::strcpy_s(Name, const_cast<RwChar*>(pNtlPLObject->GetName()));
			fwrite(&Name, sizeof(RwChar) * 128, 1, pFile);
			fwrite(&SRT[0], sizeof(RwV3d), 1, pFile);
			fwrite(&SRT[1], sizeof(RwV3d), 1, pFile);
			fwrite(&SRT[2], sizeof(RwV3d), 1, pFile);

			RwUInt32 uiObjectType = pNtlPLObject->GetObjectType();
			fwrite(&uiObjectType, sizeof(RwUInt32), 1, pFile);

			if (pNtlPLObject->GetObjectTypePtr())
			{
				pNtlPLObject->GetObjectTypePtr()->SaveIntoFile(pFile);
			}

			// serial id
			SID = pNtlPLObject->GetSID();
			fwrite(&SID, sizeof(RwUInt32), 1, pFile);

			ObjectID = pNtlPLObject->GetObjectSerialID();
			if (ObjectID == 0)
			{
				ObjectID = dGET_WORLD_PARAM()->WorldObjectSerialID;
				++dGET_WORLD_PARAM()->WorldObjectSerialID;
			}
			fwrite(&ObjectID, sizeof(RwUInt32), 1, pFile);

			// object name index
			ObjNameIdx = pNtlPLObject->GetObjNameIdx();
			fwrite(&ObjNameIdx, sizeof(DWORD), 1, pFile);

			// visibility distance
			VisibilityDist = pNtlPLObject->GetVisibleCullingDistance();
			fwrite(&VisibilityDist, sizeof(RwReal), 1, pFile);

			// enable trigger
			EnableFlag = pNtlPLObject->GetFlags() & NTL_PLEFLAG_TRIGGER; 
			fwrite(&EnableFlag, sizeof(RwBool), 1, pFile);

			// enable selection
			EnableFlag = pNtlPLObject->GetFlags() & NTL_PLEFLAG_SELECTION; 
			fwrite(&EnableFlag, sizeof(RwBool), 1, pFile);

			// enable alterable; dynamic obj.s.
			EnableFlag = pNtlPLObject->GetFlags() & NTL_PLEFLAG_ALTERABLE; 
			fwrite(&EnableFlag, sizeof(RwBool), 1, pFile);

			// enable portal
			EnableFlag = pNtlPLObject->GetFlags() & NTL_PLEFLAG_PORTAL; 
			fwrite(&EnableFlag, sizeof(RwBool), 1, pFile);

			// enable path engine collision
			EnableFlag = pNtlPLObject->GetFlags() & NTL_PLEFLAG_PECOLLISION;
			fwrite(&EnableFlag, sizeof(RwBool), 1, pFile);

			// enable Shadow Prop
			EnableFlag = pNtlPLObject->GetFlags() & NTL_PLEFLAG_SHADOW_PROP;
			/*if ((Name[3] == 'n' || Name[3] == 'N') &&
			Name[4] == '_' &&
			(Name[5] == 't' || Name[5] == 'T') &&
			(Name[6] == 'r' || Name[6] == 'R') &&
			(Name[7] == 'e' || Name[7] == 'E') &&
			(Name[8] == 'e' || Name[8] == 'E') &&
			Name[9] == '_')
			{
			EnableFlag = TRUE;
			}*/			
			fwrite(&EnableFlag, sizeof(RwBool), 1, pFile);

			// object PS map
			pNtlPLObject->SavePSMap(pFile);

			// milepost GUID
			MilePostGUID = pNtlPLObject->GetMilepostID();
			EnableFlag = (pNtlPLObject->GetProperty()->GetObjectType() == E_OBJECT_MILEPOST && MilePostGUID);
			fwrite(&EnableFlag, sizeof(RwBool), 1, pFile);
			if(EnableFlag)
			{
				fwrite(&MilePostGUID, sizeof(RwUInt32), 1, pFile);
			}

			// attach lightobject 
			pNtlPLObject->SaveLightObjectAttachData(pFile);

			// lightmap
			pNtlPLObject->SaveLightmap(pFile);
		}

		// water
		sSECTOR_WATER_ATTR* pSectorWaterAttr = pNtlWorldSector->m_pWater;
		if(pSectorWaterAttr)
		{
			::fwrite(&TrueFlag,											sizeof(RwBool), 1, pFile);
			::fwrite(&pSectorWaterAttr->_Height,						sizeof(RwReal), 1, pFile);
			::fwrite(&pSectorWaterAttr->_RGBA,							sizeof(RwRGBA), 1, pFile);
			::fwrite(&pSectorWaterAttr->_RenderstateSrc,				sizeof(DWORD), 1, pFile);
			::fwrite(&pSectorWaterAttr->_RenderstateDst,				sizeof(DWORD), 1, pFile);
			::fwrite(&pSectorWaterAttr->_RotTrans,						sizeof(RwReal), 1, pFile);
			::fwrite(&pSectorWaterAttr->_SclTrans,						sizeof(RwReal), 1, pFile);
			::fwrite(&pSectorWaterAttr->_DepthMapSize,					sizeof(RwInt32), 1, pFile);
			::fwrite(&pSectorWaterAttr->_DepthMapHeightVariation[0],	sizeof(RwReal), 1, pFile);
			::fwrite(&pSectorWaterAttr->_DepthMapHeightVariation[1],	sizeof(RwReal), 1, pFile);
			::fwrite(&pSectorWaterAttr->_IdxSequence, sizeof(RwInt32), 1, pFile);

			if(pSectorWaterAttr->_pDepthMap)
			{
				::fwrite(&TrueFlag, sizeof(RwBool), 1, pFile);
				CNtlPLResourceManager::GetInstance()->SaveAlphaBits(pSectorWaterAttr->_pDepthMap, pFile);
			}
			else
			{
				::fwrite(&ZeroFlag, sizeof(RwBool), 1, pFile);
			}
		}
		else
		{
			::fwrite(&ZeroFlag, sizeof(RwBool), 1, pFile);
		}

		// shadow
		CNtlWorldShadow* pNtlWorldShadow = pNtlWorldSector->m_pNtlWorldShadow;
		if(pNtlWorldShadow->m_pTexShadow)
		{
			::fwrite(&TrueFlag, sizeof(RwBool), 1, pFile);
			::fwrite(&pNtlWorldShadow->m_rgbaShadow, sizeof(RwRGBA), 1, pFile);
			::fwrite(&pNtlWorldShadow->m_CamViewWindowX, sizeof(RwReal), 1, pFile);
			::fwrite(&pNtlWorldShadow->m_matLight, sizeof(RwMatrix), 1, pFile);

			RwInt32 Size = RwRasterGetWidth(pNtlWorldShadow->m_pTexShadow->raster);
			::fwrite(&Size, sizeof(RwInt32), 1, pFile);
			CNtlPLResourceManager::GetInstance()->SaveTexture(pNtlWorldShadow->m_pTexShadow, pFile);
		}
		else
		{
			::fwrite(&ZeroFlag, sizeof(RwBool), 1, pFile);
		}

		// effects
		CNtlPLEntity *pEntity;
		RwInt32 NumEff = (RwInt32)pNtlWorldSector->m_vecNtlPLEntityEffect.size();
		fwrite(&NumEff, sizeof(RwInt32), 1, pFile);

		for(int i = 0; i < NumEff; ++i)
		{
			pEntity = pNtlWorldSector->m_vecNtlPLEntityEffect[i];
			CNtlMath::MathRwV3dAssign(&SRT[0], pEntity->GetScale(), pEntity->GetScale(), pEntity->GetScale());
			CNtlMath::MathRwV3dAssign(&SRT[1], pEntity->GetRotate()->x, pEntity->GetRotate()->y, pEntity->GetRotate()->z);
			CNtlMath::MathRwV3dAssign(&SRT[2], pEntity->GetPosition().x, pEntity->GetPosition().y, pEntity->GetPosition().z);
			::strcpy_s(Name, const_cast<RwChar*>(pEntity->GetName()));
			fwrite(&Name, sizeof(RwChar) * 128, 1, pFile);
			fwrite(&SRT[0], sizeof(RwV3d), 1, pFile);
			fwrite(&SRT[1], sizeof(RwV3d), 1, pFile);
			fwrite(&SRT[2], sizeof(RwV3d), 1, pFile);

			// visibility distance
			VisibilityDist = pEntity->GetVisibleCullingDistance();
			fwrite(&VisibilityDist, sizeof(RwReal), 1, pFile);
		}

		// enviroment sound
		RwInt32 NumSound	= static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntitySound.size());		
		fwrite(&NumSound, sizeof(RwInt32), 1, pFile);

		for(int i = 0; i < NumSound; ++i)
		{
			RwV3d	Pos			= static_cast<CNtlPLSound*>(pNtlWorldSector->m_vecNtlPLEntitySound[i])->GetPosition();
			RwReal	Vol			= static_cast<CNtlPLSound*>(pNtlWorldSector->m_vecNtlPLEntitySound[i])->GetVolume();
			RwReal	RadiusMin	= static_cast<CNtlPLSound*>(pNtlWorldSector->m_vecNtlPLEntitySound[i])->GetRadius(TRUE);
			RwReal	RadiusMax	= static_cast<CNtlPLSound*>(pNtlWorldSector->m_vecNtlPLEntitySound[i])->GetRadius(FALSE);

			RwChar	NameSound[32] = {0,};
			strcpy_s(NameSound, 32, static_cast<CNtlPLSound*>(pNtlWorldSector->m_vecNtlPLEntitySound[i])->GetName());

			fwrite(NameSound, sizeof(RwChar) * 32, 1, pFile);
			fwrite(&Vol, sizeof(RwReal), 1, pFile);
			fwrite(&RadiusMin, sizeof(RwReal), 1, pFile);
			fwrite(&RadiusMax, sizeof(RwReal), 1, pFile);
			fwrite(&Pos, sizeof(RwV3d), 1, pFile);
		}

		// bgm sound
		// CZ-BGM
		RwInt32 NumSoundBGM	= static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntitySoundBGM.size());
		fwrite(&NumSoundBGM, sizeof(RwInt32), 1, pFile);

		for(int i = 0; i < NumSoundBGM; ++i)
		{
			RwV3d	Pos			= static_cast<CNtlPLSoundBGM*>(pNtlWorldSector->m_vecNtlPLEntitySoundBGM[i])->GetPosition();
			//RwReal	Vol			= static_cast<CNtlPLSoundBGM*>(pNtlWorldSector->m_vecNtlPLEntitySoundBGM[i])->GetVolume();
			RwReal	Radius		= static_cast<CNtlPLSoundBGM*>(pNtlWorldSector->m_vecNtlPLEntitySoundBGM[i])->GetRadius();

			RwChar	NameSound[32] = {0,};
			strcpy_s(NameSound, 32, static_cast<CNtlPLSoundBGM*>(pNtlWorldSector->m_vecNtlPLEntitySoundBGM[i])->GetName());

			fwrite(NameSound, sizeof(RwChar) * 32, 1, pFile);
			//fwrite(&Vol, sizeof(RwReal), 1, pFile);
			fwrite(&Radius, sizeof(RwReal), 1, pFile);
			fwrite(&Pos, sizeof(RwV3d), 1, pFile);
		}

		// soft edged spot light
		// 		RwV3d		SSPos;
		// 		RwV3d*		pSSRot;
		// 		RwInt32		SSNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntitySS.size());
		// 		RwRGBAReal	SSClr;
		// 		RwReal		SSAng, SSRad;
		// 		CNtlPLSpot*	SSEntity;
		// 
		// 		fwrite(&SSNum, sizeof(RwInt32), 1, pFile);
		// 
		// 		for(int i = 0; i < SSNum; ++i)
		// 		{
		// 			SSEntity	= static_cast<CNtlPLSpot*>(pNtlWorldSector->m_vecNtlPLEntitySS[i]);
		// 			SSPos		= SSEntity->GetPosition();
		// 			pSSRot		= SSEntity->GetRotate();
		// 			SSClr		= SSEntity->GetColor();
		// 			SSAng		= SSEntity->GetAngle();
		// 			SSRad		= SSEntity->GetRadius();
		// 
		// 			fwrite(&SSPos, sizeof(RwV3d), 1, pFile);
		// 			fwrite(pSSRot, sizeof(RwV3d), 1, pFile);
		// 			fwrite(&SSClr, sizeof(RwRGBAReal), 1, pFile);
		// 			fwrite(&SSAng, sizeof(RwReal), 1, pFile);
		// 			fwrite(&SSRad, sizeof(RwReal), 1, pFile);
		// 		}

		// Sector Cull
		fwrite(&pNtlWorldSector->m_IsCulled, sizeof(RwBool), 1, pFile);

		// Tile Transparency
		fwrite(&pNtlWorldSector->m_IsTileTransparency, sizeof(RwBool), 1, pFile);

		// Shoreline
		if(pNtlWorldSector->m_pNtlPLEntityShoreLine && static_cast<CNtlPLShoreLine*>(pNtlWorldSector->m_pNtlPLEntityShoreLine)->IsValidShoreline())
		{
			fwrite(&TrueFlag, sizeof(RwBool), 1, pFile);

			CNtlPLShoreLine* pSL = static_cast<CNtlPLShoreLine*>(pNtlWorldSector->m_pNtlPLEntityShoreLine);
			pSL->SaveIntoFile(pFile);
		}
		else
		{
			fwrite(&ZeroFlag, sizeof(RwBool), 1, pFile);
		}

		// decals
		RwInt32	EntityNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityDecal.size());

		fwrite(&EntityNum, sizeof(RwInt32), 1, pFile);

		for(int i = 0; i < EntityNum; ++i)
		{
			static_cast<CNtlPLWorldDecal*>(pNtlWorldSector->m_vecNtlPLEntityDecal[i])->SaveIntoFile(pFile);
		}

		// plant
		RwInt32 iPlantSetNum = static_cast<RwInt32>(pNtlWorldSector->m_vecPlantSet.size());

		fwrite(&iPlantSetNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iPlantSetNum; ++i)
		{
			sSECTOR_PLANT_SET_ATTR*	pPlantSet		= pNtlWorldSector->m_vecPlantSet.at(i);
			RwInt32					iPlantObjNum	= static_cast<RwInt32>(pPlantSet->vecPlantObj.size());

			fwrite(&pPlantSet->iSetIdx, sizeof(RwInt32), 1, pFile); // Set Index
			fwrite(&iPlantObjNum, sizeof(RwInt32), 1, pFile); // Obj Num

			for (int j = 0; j < iPlantObjNum; ++j)
			{
				fwrite(&pPlantSet->vecPlantObj.at(j)->iTileIdx, sizeof(RwInt32), 1, pFile); // Tile Index				
			}			
		}

		// world light sphere
		RwInt32 iWLSphereNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityWorldLight_Sphere.size());

		fwrite(&iWLSphereNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iWLSphereNum; ++i)
		{
			CNtlPLWorldLight_Sphere* pWLSphere = (CNtlPLWorldLight_Sphere*)pNtlWorldSector->m_vecNtlPLEntityWorldLight_Sphere.at(i);

			RwV3d	vPos	= pWLSphere->GetPosition();
			RwRGBA	clr		= pWLSphere->GetColor();
			RwReal	fRadius	= pWLSphere->GetRadius();		

			fwrite(&vPos, sizeof(RwV3d), 1, pFile);
			fwrite(&clr, sizeof(RwRGBA), 1, pFile);
			fwrite(&fRadius, sizeof(RwReal), 1, pFile);
		}

		// world light box
		RwInt32 iWLBoxNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityWorldLight_Box.size());

		fwrite(&iWLBoxNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iWLBoxNum; ++i)
		{
			CNtlPLWorldLight_Box* pWLBox = (CNtlPLWorldLight_Box*)pNtlWorldSector->m_vecNtlPLEntityWorldLight_Box.at(i);

			RwV3d	vPos	= pWLBox->GetPosition();
			RwRGBA	clr		= pWLBox->GetColor();
			RwV3d	vRot	= *pWLBox->GetRotate();
			RwV3d	vScale	= *pWLBox->GetScaleVector();

			fwrite(&vPos, sizeof(RwV3d), 1, pFile);
			fwrite(&clr, sizeof(RwRGBA), 1, pFile);
			fwrite(&vRot, sizeof(RwV3d), 1, pFile);
			fwrite(&vScale, sizeof(RwV3d), 1, pFile);			
		}

		// occluder
		RwInt32 iOccluderQuadNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityOccluder_Quad.size());

		fwrite(&iOccluderQuadNum, sizeof(RwInt32), 1, pFile);

		for (int i = 0; i < iOccluderQuadNum; ++i)
		{
			CNtlPLOccluder_Quad* pOccluder = (CNtlPLOccluder_Quad*)pNtlWorldSector->m_vecNtlPLEntityOccluder_Quad.at(i);

			RwUInt32	eFuncFlag	= pOccluder->GetOccluderFuncFlag();
			RwV3d		vPos		= pOccluder->GetPosition();
			RwInt32		iPlaneType	= pOccluder->GetOccluderPlaneType();

			RwChar		acTemp[128];
			strcpy_s(acTemp, 128, pOccluder->GetTriggerName());

			fwrite(&eFuncFlag, sizeof(RwUInt32), 1, pFile);
			fwrite(&vPos, sizeof(RwV3d), 1, pFile);
			fwrite(&iPlaneType, sizeof(RwInt32), 1, pFile);
			fwrite(acTemp, sizeof(RwChar) * 128, 1, pFile);
			for (int j = 0; j < 4; ++j)
			{
				fwrite(pOccluder->GetVertexOriginal(j), sizeof(RwV3d), 1, pFile);
			}
		}

		// heathaze
		RwInt32 iHeatHazeObjectNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityHeatHaze.size());

		fwrite(&iHeatHazeObjectNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < iHeatHazeObjectNum; ++i)
		{
			CNtlPLHeatHazeObject* pHeatHaze = (CNtlPLHeatHazeObject*)pNtlWorldSector->m_vecNtlPLEntityHeatHaze.at(i);

			for (int j = 0; j < 4; ++j)
			{
				fwrite(pHeatHaze->GetVertex(j), sizeof(RwV3d), 1, pFile);
			}

			RwReal fDistance = pHeatHaze->GetDistance();
			fwrite(&fDistance, sizeof(RwReal), 1, pFile);

			fwrite(pHeatHaze->HeatHazeData(), sizeof(sNTL_HEATHAZE), 1, pFile);
		}

		// light object
		RwInt32 iLightObjectNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityLightObject.size());
		fwrite(&iLightObjectNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < iLightObjectNum; ++i)
		{
			CNtlPLLightObject* pLightObject = (CNtlPLLightObject*)pNtlWorldSector->m_vecNtlPLEntityLightObject.at(i);

			RpLightType eType;
			eType = pLightObject->GetLightType();
			fwrite(&eType, sizeof(RpLightType), 1, pFile);

			RwBool bTemp;
			bTemp = pLightObject->IsRealLight();
			fwrite(&bTemp, sizeof(RwBool), 1, pFile);

			RwV3d vTemp;
			vTemp = pLightObject->GetPosition();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

			vTemp = *pLightObject->GetRotate();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

			RwRGBAReal rgbafTemp;
			rgbafTemp = pLightObject->GetColor();
			fwrite(&rgbafTemp, sizeof(RwRGBAReal), 1, pFile);

			RwReal fTemp;
			fTemp = pLightObject->GetRadius();
			fwrite(&fTemp, sizeof(RwReal), 1, pFile);

			fTemp = pLightObject->GetAngle();
			fwrite(&fTemp, sizeof(RwReal), 1, pFile);

			// CZ-LO
			bTemp = pLightObject->IsEnabledDecal();
			fwrite(&bTemp, sizeof(RwBool), 1, pFile);

			bTemp = pLightObject->IsEnabledWorldLight();
			fwrite(&bTemp, sizeof(RwBool), 1, pFile);
		}

		// decal volume
		RwInt32 iDecalVolumeNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityDecalVolume.size());
		fwrite(&iDecalVolumeNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < iDecalVolumeNum; ++i)
		{
			CNtlPLDecalVolume* pDecalVolume = (CNtlPLDecalVolume*)pNtlWorldSector->m_vecNtlPLEntityDecalVolume.at(i);

			RwChar acTextureName[128];
			strcpy_s(acTextureName, 128, pDecalVolume->GetTextureName());
			fwrite(acTextureName, sizeof(RwChar) * 128, 1, pFile);

			RwV3d vTemp;

			vTemp = pDecalVolume->GetPosition();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

			vTemp = *pDecalVolume->GetRotate();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

			vTemp = *pDecalVolume->GetScale();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);


			RwRGBA clrTemp = *pDecalVolume->GetColor();
			fwrite(&clrTemp, sizeof(RwRGBA), 1, pFile);


			RwReal fTemp;

			fTemp = pDecalVolume->GetAppendOffset();
			fwrite(&fTemp, sizeof(RwReal), 1, pFile);

			fTemp = pDecalVolume->GetVisibleDist();
			fwrite(&fTemp, sizeof(RwReal), 1, pFile);

			RwUInt32 uiTemp;
			uiTemp = pDecalVolume->GetDecalVolumeFlags();
			fwrite(&uiTemp, sizeof(RwUInt32), 1, pFile);
		}

		// game property
		RwInt32 iGamePropertyNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityGameProperty.size());
		fwrite(&iGamePropertyNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < iGamePropertyNum; ++i)
		{
			CNtlPLGameProperty* pGameProperty = (CNtlPLGameProperty*)pNtlWorldSector->m_vecNtlPLEntityGameProperty.at(i);

			RwUInt32 uiTemp;
			uiTemp = pGameProperty->GetShapeType();
			fwrite(&uiTemp, sizeof(RwUInt32), 1, pFile);

			RwV3d vTemp;
			vTemp = pGameProperty->GetPosition();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

			vTemp = pGameProperty->GetSize();
			fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

			uiTemp = pGameProperty->GetGameProperty();
			fwrite(&uiTemp, sizeof(RwUInt32), 1, pFile);
		}

		// dojo
		RwInt32 iDojoNum = static_cast<RwInt32>(pNtlWorldSector->m_vecNtlPLEntityDojo.size());

		fwrite(&iDojoNum, sizeof(RwInt32), 1, pFile);
		for (int i = 0; i < iDojoNum; ++i)
		{
			CNtlPLDojo* pDojo = (CNtlPLDojo*)pNtlWorldSector->m_vecNtlPLEntityDojo.at(i);

			RwV3d vScale	= *pDojo->GetScaleVector();
			RwV3d vRot		= *pDojo->GetRotate();
			RwV3d vPos		= pDojo->GetPosition();

			fwrite(&vScale, sizeof(RwV3d), 1, pFile);
			fwrite(&vRot, sizeof(RwV3d), 1, pFile);
			fwrite(&vPos, sizeof(RwV3d), 1, pFile);

			RwInt32 iDojoID		= pDojo->GetDojoID();
			RwReal	fFadeInTime	= pDojo->GetFadeInTime();
			RwReal	fFadeOutTime= pDojo->GetFadeOutTime();

			fwrite(&iDojoID, sizeof(RwInt32), 1, pFile);
			fwrite(&fFadeInTime, sizeof(RwReal), 1, pFile);
			fwrite(&fFadeOutTime, sizeof(RwReal), 1, pFile);

			RwChar acTemp[dPL_DOJO_RES_NAME_LEN];
			for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
			{
				strcpy_s(acTemp, dPL_DOJO_RES_NAME_LEN, pDojo->GetResObjectName(i));
				fwrite(acTemp, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);
				strcpy_s(acTemp, dPL_DOJO_RES_NAME_LEN, pDojo->GetResEffectNameUp(i));
				fwrite(acTemp, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);
				strcpy_s(acTemp, dPL_DOJO_RES_NAME_LEN, pDojo->GetResEffectNameDown(i));
				fwrite(acTemp, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, pFile);
			}
		}
	}

	NTL_RETURN(TRUE);
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif