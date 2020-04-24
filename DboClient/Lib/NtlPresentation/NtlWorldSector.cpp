#include "precomp_ntlpresentation.h"
#include "ntlworldsector.h"
#include "NtlWorldSectorLOD.h"
#include "NtlWorldMaterialPlugin.h"
#include "ntlworldcommon.h"
#include "NtlPLSceneManager.h"
#include "NtlOccluderManager.h"
#include "NtlDebug.h"
#include "NtlWorldShadow.h"
#include "NtlPLEvent.h"
#include "NtlPLEventGenerator.h"
#include "NtlPLDecalVolume.h"
#include "NtlPLDecal.h"
#include "NtlPLPlant.h"
#include "NtlPLWorldLight_Base.h"
#include "NtlPLWorldLight_Sphere.h"
#include "NtlPLWorldLight_Box.h"
#include "NtlPLOccluder_Base.h"
#include "NtlPLOccluder_Quad.h"
#include "NtlPLHeatHaze.h"
#include "NtlPLLightObject.h"

#include "NtlWorldSectorPVS.h"
#include "FileMem.h"

#include "NtlPLWorldEntity.h"
#include "NtlWorldFieldManager.h"


#ifdef dNTL_WORLD_TOOL_MODE

#include "NtlWorldSLManger.h"
#include "NtlWorldPathEngineManager.h"

#endif


CNtlWorldSector::CNtlWorldSector(void)
{
	m_LODAttr.MipMapLevel	= 1;//dGET_SECTOR_LOD()->GetTotLvlCnt();
	m_LODAttr.TSectionLevel = m_LODAttr.MipMapLevel;
	m_LODAttr.LSectionLevel = m_LODAttr.MipMapLevel;
	m_LODAttr.RSectionLevel = m_LODAttr.MipMapLevel;
	m_LODAttr.BSectionLevel = m_LODAttr.MipMapLevel;

	m_pAtomic				= NULL;
	m_pWorldSector			= NULL;
	m_pWater				= NULL;
	m_pNtlWorldShadow		= NULL;
	m_pNtlWorldSectorPVS	= NTL_NEW CNtlWorldSectorPVS;

#ifdef dNTL_WORLD_TOOL_MODE

	m_pClrDiffusePalette	= NULL;
	m_pNtlWorldSL			= NULL;
#endif

	m_pNtlWorldPathEngineData = NULL;

	m_bLoaded				= FALSE;
	m_IsCulled				= FALSE;
	m_bFullFogged			= FALSE;
	m_bCullCheck			= FALSE;
	//m_IsOccluded			= FALSE;
	m_IsTileTransparency	= FALSE;

	m_pNtlPLEntityShoreLine = NULL;

	for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		m_pTextureAlpha[i] = NULL;
	}
}

CNtlWorldSector::~CNtlWorldSector(void)
{
	// be careful with this
	//m_pWorldSector = NULL;
	for (int i = 0; i < static_cast<int>(m_vecNtlPLEntityLightObjectDummy.size()); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecNtlPLEntityLightObjectDummy[i]);
	}
	m_vecNtlPLEntityLightObjectDummy.clear();

	NTL_DELETE(m_pNtlWorldSectorPVS);

	// Momery 상의 남는 것이 있는지 확인해야 한다.
	DBO_ASSERT(m_pAtomic == NULL, "Sector Memory Leak.");
	DBO_ASSERT(m_pWater == NULL, "Sector Memory Leak.");
	DBO_ASSERT(m_vecPlantSet.size() == 0, "Sector Memory Leak.");
	DBO_ASSERT(m_vecNtlPLEntityOrnament.size() == 0, "Sector Memory Leak.");
	DBO_ASSERT(m_vecNtlPLEntityEffect.size() == 0, "Sector Memory Leak.");
	DBO_ASSERT(m_vecNtlPLEntitySound.size() == 0, "Sector Memory Leak.");
	DBO_ASSERT(m_vecNtlPLEntitySoundBGM.size() == 0, "Sector Memory Leak.");
	DBO_ASSERT(m_pNtlPLEntityShoreLine == NULL, "Sector Memory Leak.");
	DBO_ASSERT(m_vecNtlPLEntityDecal.size() == 0, "Sector Memory Leak.");
	DBO_ASSERT(m_vecNtlPLEntityWorldLight_Sphere.size() == 0, "Sector Memory Leak.");
	DBO_ASSERT(m_vecNtlPLEntityWorldLight_Box.size() == 0, "Sector Memory Leak.");
	DBO_ASSERT(m_vecNtlPLEntityOccluder_Quad.size() == 0, "Sector Memory Leak.");
	DBO_ASSERT(m_vecNtlPLEntityHeatHaze.size() == 0, "Sector Memory Leak.");
	DBO_ASSERT(m_vecNtlPLEntityLightObject.size() == 0, "Sector Memory Leak.");
	DBO_ASSERT(m_vecNtlPLEntityLightObjectDummy.size() == 0, "Sector Memory Leak.");
	DBO_ASSERT(m_vecNtlPLEntityGameProperty.size() == 0, "Sector Memory Leak.");
	DBO_ASSERT(m_vecNtlPLEntityDecalVolume.size() == 0, "Sector Memory Leak.");
	DBO_ASSERT(m_vecNtlPLEntityDojo.size() == 0, "Sector Memory Leak.");
	DBO_ASSERT(m_pNtlWorldShadow == NULL, "Sector Memory Leak.");
	DBO_ASSERT(m_pNtlWorldSectorPVS == NULL, "Sector Memory Leak.");
}	

void CNtlWorldSector::Init()
{
	MaterialContructor();
}

void CNtlWorldSector::Free()
{
	MaterialDestructor();
}

void CNtlWorldSector::DeleteEntity(CNtlPLEntity* pNtlPLEntity, RwInt32 MenuIdx)
{
	NTL_FUNCTION("CNtlWorldSector::DeleteEntity");

	switch(MenuIdx)
	{
	// doodads
	case 0:
		{
			for(int i = 0; i < static_cast<int>(m_vecNtlPLEntityOrnament.size()); ++i)
			{
				if(pNtlPLEntity == m_vecNtlPLEntityOrnament[i])
				{
					m_vecNtlPLEntityOrnament.erase(m_vecNtlPLEntityOrnament.begin() + i);
					NTL_RETURNVOID();
				}
			}
			//DBO_ASSERT((FALSE), "Exist Entity.");
		}
		break;
	// effects
	case 1:
		{
			for(int i = 0; i < static_cast<int>(m_vecNtlPLEntityEffect.size()); ++i)
			{
				if(pNtlPLEntity == m_vecNtlPLEntityEffect[i])
				{
					m_vecNtlPLEntityEffect.erase(m_vecNtlPLEntityEffect.begin() + i);
					NTL_RETURNVOID();
				}
			}
			//DBO_ASSERT((FALSE), "Exist Entity.");
		}
		break;
	// sound effects
	case 2:
		{
			for(int i = 0; i < static_cast<int>(m_vecNtlPLEntitySound.size()); ++i)
			{
				if(pNtlPLEntity == m_vecNtlPLEntitySound[i])
				{
					m_vecNtlPLEntitySound.erase(m_vecNtlPLEntitySound.begin() + i);
					NTL_RETURNVOID();
				}
			}
			for(int i = 0; i < static_cast<int>(m_vecNtlPLEntitySoundBGM.size()); ++i)
			{
				if(pNtlPLEntity == m_vecNtlPLEntitySoundBGM[i])
				{
					m_vecNtlPLEntitySoundBGM.erase(m_vecNtlPLEntitySoundBGM.begin() + i);
					NTL_RETURNVOID();
				}
			}
			//DBO_ASSERT((FALSE), "Exist Entity.");
		}
		break;
		// CZ-SS
	// soft spot lights
// 	case 3:
// 		{
// 			for(int i = 0; i < static_cast<int>(m_vecNtlPLEntitySS.size()); ++i)
// 			{
// 				if(pNtlPLEntity == m_vecNtlPLEntitySS[i])
// 				{
// 					m_vecNtlPLEntitySS.erase(m_vecNtlPLEntitySS.begin() + i);
// 					NTL_RETURNVOID();
// 				}
// 			}
// 		}
// 		break;
	// decals
	case 4:
		{
			for(int i = 0; i < static_cast<int>(m_vecNtlPLEntityDecal.size()); ++i)
			{
				if(pNtlPLEntity == m_vecNtlPLEntityDecal[i])
				{
					m_vecNtlPLEntityDecal.erase(m_vecNtlPLEntityDecal.begin() + i);
					NTL_RETURNVOID();
				}
			}
			//DBO_ASSERT((FALSE), "Exist Entity.");
		}
		break;
	// world light
	case 5:
		{
			CNtlPLWorldLight_Base* pWorldLight = (CNtlPLWorldLight_Base*)pNtlPLEntity;

			if (pWorldLight->GetWorldLightType() == EPLWORLDLIGHT_SPHERE)
			{
				for(int i = 0; i < static_cast<int>(m_vecNtlPLEntityWorldLight_Sphere.size()); ++i)
				{
					if(pNtlPLEntity == m_vecNtlPLEntityWorldLight_Sphere[i])
					{
						m_vecNtlPLEntityWorldLight_Sphere.erase(m_vecNtlPLEntityWorldLight_Sphere.begin() + i);
						NTL_RETURNVOID();
					}
				}
			}
			else if (pWorldLight->GetWorldLightType() == EPLWORLDLIGHT_BOX)
			{
				for(int i = 0; i < static_cast<int>(m_vecNtlPLEntityWorldLight_Box.size()); ++i)
				{
					if(pNtlPLEntity == m_vecNtlPLEntityWorldLight_Box[i])
					{
						m_vecNtlPLEntityWorldLight_Box.erase(m_vecNtlPLEntityWorldLight_Box.begin() + i);
						NTL_RETURNVOID();
					}
				}
			}
			//DBO_ASSERT((FALSE), "Exist Entity.");
		}
		break;
	case 6:
		{
			CNtlPLOccluder_Base* pOccluder = (CNtlPLOccluder_Base*)pNtlPLEntity;

			if (pOccluder->GetOccluderType() == EPLOCCLUDER_QUAD)
			{
				for (int i = 0; i < static_cast<int>(m_vecNtlPLEntityOccluder_Quad.size()); ++i)
				{
					if (pNtlPLEntity == m_vecNtlPLEntityOccluder_Quad[i])
					{
						m_vecNtlPLEntityOccluder_Quad.erase(m_vecNtlPLEntityOccluder_Quad.begin() + i);
						NTL_RETURNVOID();
					}
				}
			}
			//DBO_ASSERT((FALSE), "Exist Entity.");
		}
		break;
	case 7:
		{
			for(int i = 0; i < static_cast<int>(m_vecNtlPLEntityHeatHaze.size()); ++i)
			{
				if(pNtlPLEntity == m_vecNtlPLEntityHeatHaze[i])
				{
					m_vecNtlPLEntityHeatHaze.erase(m_vecNtlPLEntityHeatHaze.begin() + i);
					NTL_RETURNVOID();
				}
			}
			//DBO_ASSERT((FALSE), "Exist Entity.");
		}
		break;
	case 8:
		{
			for (int i = 0; i < static_cast<int>(m_vecNtlPLEntityLightObject.size()); ++i)
			{
				if (pNtlPLEntity == m_vecNtlPLEntityLightObject[i])
				{
					m_vecNtlPLEntityLightObject.erase(m_vecNtlPLEntityLightObject.begin() + i);
					NTL_RETURNVOID();
				}
			}
			for (int i = 0; i < static_cast<int>(m_vecNtlPLEntityLightObjectDummy.size()); ++i)
			{
				if (pNtlPLEntity == m_vecNtlPLEntityLightObjectDummy[i])
				{
					m_vecNtlPLEntityLightObjectDummy.erase(m_vecNtlPLEntityLightObjectDummy.begin() + i);
					NTL_RETURNVOID();
				}
			}
			//DBO_ASSERT((FALSE), "Exist Entity.");
		}
		break;
	case 9:
		{
			for (int i = 0; i < static_cast<int>(m_vecNtlPLEntityGameProperty.size()); ++i)
			{
				if (pNtlPLEntity == m_vecNtlPLEntityGameProperty[i])
				{
					m_vecNtlPLEntityGameProperty.erase(m_vecNtlPLEntityGameProperty.begin() + i);
					NTL_RETURNVOID();
				}
			}
			//DBO_ASSERT((FALSE), "Exist Entity.");
		}
		break;
	case 10:
		{
			for (int i = 0; i < static_cast<int>(m_vecNtlPLEntityDecalVolume.size()); ++i)
			{
				if (pNtlPLEntity == m_vecNtlPLEntityDecalVolume[i])
				{
					m_vecNtlPLEntityDecalVolume.erase(m_vecNtlPLEntityDecalVolume.begin() + i);
					NTL_RETURNVOID();
				}
			}
			//DBO_ASSERT((FALSE), "Exist Entity.");
		}
		break;
	case 11:
		{
			for (int i = 0; i < static_cast<int>(m_vecNtlPLEntityDojo.size()); ++i)
			{
				if (pNtlPLEntity == m_vecNtlPLEntityDojo[i])
				{
					m_vecNtlPLEntityDojo.erase(m_vecNtlPLEntityDojo.begin() + i);
					NTL_RETURNVOID();
				}
			}
			//DBO_ASSERT((FALSE), "Exist Entity.");
		}
		break;
	}	

	NTL_RETURNVOID();
}

void CNtlWorldSector::MaterialCopier(void *dstObject, const void *srcObject)
{
	NTL_FUNCTION("CNtlWorldSector::MaterialCopier");

	RwUInt32 i = 0;
	sRpNtlWorldSectorSplat *pDestSectorSplat;
	const sRpNtlWorldSectorSplat *pSrcSectorSplat;

	pDestSectorSplat = dGetNtlWorldSectorSplatInfo(dstObject, LocalOffset);
	pSrcSectorSplat  = dGetNtlWorldSectorSplatInfoConst(srcObject, LocalOffset);

	if(pSrcSectorSplat->pNtlWorldSector->m_pTextureBase)
	{
		pDestSectorSplat->pNtlWorldSector->m_pTextureBase = pSrcSectorSplat->pNtlWorldSector->m_pTextureBase;
		RwTextureAddRef(pDestSectorSplat->pNtlWorldSector->m_pTextureBase);
	}

	for(i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		if(pSrcSectorSplat->pNtlWorldSector->m_pTextureSplat[i])
		{
			pDestSectorSplat->pNtlWorldSector->m_pTextureSplat[i] = pSrcSectorSplat->pNtlWorldSector->m_pTextureSplat[i];
			RwTextureAddRef(pDestSectorSplat->pNtlWorldSector->m_pTextureSplat[i]);
		}

		if(pSrcSectorSplat->pNtlWorldSector->m_pTextureAlpha[i])
		{
			pDestSectorSplat->pNtlWorldSector->m_pTextureAlpha[i] = pSrcSectorSplat->pNtlWorldSector->m_pTextureAlpha[i];
			RwTextureAddRef(pDestSectorSplat->pNtlWorldSector->m_pTextureAlpha[i]);
		}

		pDestSectorSplat->pNtlWorldSector->m_TextureMat[i][0] = pSrcSectorSplat->pNtlWorldSector->m_TextureMat[i][0];
		pDestSectorSplat->pNtlWorldSector->m_TextureMat[i][1] = pSrcSectorSplat->pNtlWorldSector->m_TextureMat[i][1];
	}

	if(pSrcSectorSplat->pNtlWorldSector)
		pDestSectorSplat->pNtlWorldSector = pSrcSectorSplat->pNtlWorldSector;

	NTL_RETURNVOID();
}

void CNtlWorldSector::MaterialContructor()
{
	NTL_FUNCTION("CNtlWorldSector::MaterialContructor");

	RwV3d vPos;
	vPos.x = DatumPoint.x;
	vPos.y = DatumPoint.y;
	vPos.z = DatumPoint.z;

	m_vecNtlPLEntityOrnament.clear();
	m_vecNtlPLEntityEffect.clear();
	m_vecNtlPLEntitySound.clear();
	m_vecNtlPLEntitySoundBGM.clear();
	// CZ-SS
	//m_vecNtlPLEntitySS.clear();
	m_vecNtlPLEntityDecal.clear();
	m_vecNtlPLEntityWorldLight_Sphere.clear();
	m_vecNtlPLEntityWorldLight_Box.clear();
	m_vecNtlPLEntityOccluder_Quad.clear();
	m_vecNtlPLEntityHeatHaze.clear();
	m_vecNtlPLEntityLightObject.clear();
	m_vecNtlPLEntityGameProperty.clear();
	m_vecNtlPLEntityDecalVolume.clear();
	m_vecNtlPLEntityDojo.clear();
	m_pNtlPLEntityShoreLine = NULL;

	m_pAtomic			= NULL;
	m_pWater			= NULL;
	m_pTextureBase		= NULL;

#ifdef dNTL_WORLD_COMBINE
	m_pTextureCombine	= NULL;
#endif

	m_vecPlantSet.clear();

	// Initialize base, detail texture layers
	for(int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		m_pTextureSplat[i] = NULL;
		m_pTextureAlpha[i] = NULL;

		D3DXMatrixIdentity(&m_TextureMat[i][0]);
		D3DXMatrixIdentity(&m_TextureMat[i][1]);

		m_TextureMatAng[i] = 0.0f;
		m_TextureMatAdd[i] = dGET_WORLD_PARAM()->WorldTexAddr;
	}

	m_TextureMatAngBase = 0.0f;
	m_TextureMatAddBase = dGET_WORLD_PARAM()->WorldTexAddr;
	float			UVWrap = m_TextureMatAddBase;
	D3DXMATRIXA16	Temp;

	D3DXMatrixIdentity(&m_TextureMatBase[0]);
	m_TextureMatBase[0]._11 *= UVWrap;
	m_TextureMatBase[0]._22 *= UVWrap;
	m_TextureMatBase[1] = m_TextureMatBase[0];

	float Error = 0.5f * UVWrap;
	D3DXMatrixTranslation(&Temp, -Error, -Error, 0);
	D3DXMatrixMultiply(&m_TextureMatBase[1], &m_TextureMatBase[1], &Temp);
	D3DXMatrixRotationZ(&Temp, m_TextureMatAngBase);
	D3DXMatrixMultiply(&m_TextureMatBase[1], &m_TextureMatBase[1], &Temp);
	D3DXMatrixTranslation(&Temp, Error, Error, 0);
	D3DXMatrixMultiply(&m_TextureMatBase[1], &m_TextureMatBase[1], &Temp);

	m_TextureMatBase[1].m[2][0] = m_TextureMatBase[1].m[3][0];
	m_TextureMatBase[1].m[2][1] = m_TextureMatBase[1].m[3][1];
	m_TextureMatBase[1].m[3][0] = 0.0f;
	m_TextureMatBase[1].m[3][1] = 0.0f;

#ifdef dNTL_WORLD_COMBINE
	D3DXMatrixIdentity(&m_TextureMatCombine);

	D3DXMatrixTranslation(&Temp, -0.5f, -0.5f, 0);
	D3DXMatrixMultiply(&m_TextureMatCombine, &m_TextureMatCombine, &Temp);
	D3DXMatrixTranslation(&Temp, 0.5f, 0.5f, 0);
	D3DXMatrixMultiply(&m_TextureMatCombine, &m_TextureMatCombine, &Temp);

	m_TextureMatCombine.m[2][0] = m_TextureMatCombine.m[3][0];
	m_TextureMatCombine.m[2][1] = m_TextureMatCombine.m[3][1];
	m_TextureMatCombine.m[3][0] = 0.0f;
	m_TextureMatCombine.m[3][1] = 0.0f;
#endif

	// shadow
	m_pNtlWorldShadow = NTL_NEW CNtlWorldShadow;

#ifdef dNTL_WORLD_TOOL_MODE

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("swap");

	RwReal	XBasedOn0	= DatumPoint.x + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwReal	ZBasedOn0	= DatumPoint.z + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwInt32 WidthNum	= (int)(XBasedOn0 / (float)dGET_WORLD_PARAM()->WorldSectorSize);
	RwInt32 DepthNum	= (int)(ZBasedOn0 / (float)dGET_WORLD_PARAM()->WorldSectorSize); 
	RwInt32 RetIdx		= WidthNum + (DepthNum * dGET_WORLD_PARAM()->WorldSectorNum);
	RwInt32 NumVert		= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;

	// diffuse
	char chrRetIdx[40];
	::sprintf_s(chrRetIdx, 40, "%d_sec.dif", RetIdx);

	FILE* pFile;
	if(::fopen_s(&pFile, chrRetIdx, "rb"))
	{	
		m_pClrDiffusePalette = NTL_NEW DWORD [NumVert];
		for(int i = 0; i < NumVert; ++i)
		{
			m_pClrDiffusePalette[i] = (dGET_WORLD_PARAM()->ClrBaseAmbient[0] << 16)	|
									(dGET_WORLD_PARAM()->ClrBaseAmbient[1] << 8)	|
									(dGET_WORLD_PARAM()->ClrBaseAmbient[2] << 0)	|
									0xff000000;
		}
	}
	else
	{
		m_pClrDiffusePalette = NTL_NEW DWORD [NumVert];
		::fread(m_pClrDiffusePalette, sizeof(DWORD) * NumVert, 1, pFile);
		fclose(pFile);
		
	}

	// Slope Lighting Property
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("swap");
	_chdir("sl");

	::sprintf_s(chrRetIdx, 40, "%d_sec.sl", RetIdx);

	if(::fopen_s(&pFile, chrRetIdx, "rb"))
	{
		m_pNtlWorldSL = NTL_NEW sNTL_WORLD_SL;
	}
	else
	{
		m_pNtlWorldSL = NTL_NEW sNTL_WORLD_SL;
		::fread(m_pNtlWorldSL, sizeof(sNTL_WORLD_SL), 1, pFile);
		fclose(pFile);		
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	// projection shadow map
	m_pNtlWorldShadow->LoadSwapFile(RetIdx);

	// projection NtlPathEngine
	m_pNtlWorldPathEngineData = NULL;
	if (CNtlWorldPathEngineManager::GetInstance()->GetPathEngineMode())
	{
		RwInt32 iDataSize = (dGET_WORLD_PARAM()->WorldSectorSize / CNtlWorldPathEngineManager::GetInstance()->GetChunkSize())
			* (dGET_WORLD_PARAM()->WorldSectorSize / CNtlWorldPathEngineManager::GetInstance()->GetChunkSize());

		m_pNtlWorldPathEngineData = NTL_NEW RwReal [iDataSize];

		for (int i = 0; i < iDataSize; ++i)
			m_pNtlWorldPathEngineData[i] = RwRealMAXVAL;
	}
	CNtlWorldPathEngineManager::GetInstance()->LoadSwapFile(RetIdx, m_pNtlWorldPathEngineData);
#endif

	m_bLoaded = FALSE;

	NTL_RETURNVOID();
}

void CNtlWorldSector::MaterialDestructor(RwBool SaveSwapInToolMode /*= TRUE*/)
{
	NTL_FUNCTION("CNtlWorldSector::MaterialDestructor");

	RwV3d vPos;
	vPos.x = DatumPoint.x;
	vPos.y = DatumPoint.y;
	vPos.z = DatumPoint.z;


	if(SaveSwapInToolMode)
	{
#ifdef dNTL_WORLD_TOOL_MODE
		SaveSwapFile();
// 		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
// 		if(_chdir("swap") == -1)
// 		{
// 			// create folders
// 			_mkdir("swap");
// 			_chdir("swap");
// 		}
// 
// 		RwReal	XBasedOn0	= DatumPoint.x + dGET_WORLD_PARAM()->WorldSizeHalf;
// 		RwReal	ZBasedOn0	= DatumPoint.z + dGET_WORLD_PARAM()->WorldSizeHalf;
// 		RwInt32 WidthNum	= (int)(XBasedOn0 / (float)dGET_WORLD_PARAM()->WorldSectorSize);
// 		RwInt32 DepthNum	= (int)(ZBasedOn0 / (float)dGET_WORLD_PARAM()->WorldSectorSize); 
// 		RwInt32 RetIdx		= WidthNum + (DepthNum * dGET_WORLD_PARAM()->WorldSectorNum);
// 		RwInt32 NumVert		= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;
// 
// 		char chrRetIdx[40];
// 		::sprintf_s(chrRetIdx, 40, "%d_sec.dif", RetIdx);
// 
// 		FILE* pFile;
// 		if(::fopen_s(&pFile, chrRetIdx, "wb"))
// 		{
// 			DBO_TRACE(FALSE, "file open failed. (" << chrRetIdx << ")");
// 		}
// 		else
// 		{
// 			::fwrite(m_pClrDiffusePalette, sizeof(DWORD) * NumVert, 1, pFile);
// 			fclose(pFile);
// 		}
// 
// 		// Slope Lighting Property
// 		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
// 		if(_chdir("swap") == -1)
// 		{
// 			// create folders
// 			_mkdir("swap");
// 			_chdir("swap");
// 		}
// 
// 		if(_chdir("sl") == -1)
// 		{
// 			// create folders
// 			_mkdir("sl");
// 			_chdir("sl");
// 		}
// 
// 		::sprintf_s(chrRetIdx, 40, "%d_sec.sl", RetIdx);
// 
// 		if(::fopen_s(&pFile, chrRetIdx, "wb"))
// 		{
// 			DBO_TRACE(FALSE, "file open failed. (" << chrRetIdx << ")");
// 		}
// 		else
// 		{
// 			::fwrite(m_pNtlWorldSL, sizeof(sNTL_WORLD_SL), 1, pFile);
// 			::fclose(pFile);
// 		}
// 
// 		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
// 
// 		// projection shadow prop.
// 		m_pNtlWorldShadow->SaveSwapFile(RetIdx);
// 
// 		// occluder quad
// 		CNtlPLOccluder_Quad::SaveSwapFile(DatumPoint.x, DatumPoint.y, DatumPoint.z);
// 
// 		CNtlWorldPathEngineManager::GetInstance()->SaveSwapFile(RetIdx, m_pNtlWorldPathEngineData);
// 
// 		CNtlPLHeatHazeObject::SaveSwapFile(DatumPoint.x, DatumPoint.y, DatumPoint.z);
// 
// 		CNtlPLLightObject::SaveSwapFile(DatumPoint.x, DatumPoint.y, DatumPoint.z);
// 
// 		CNtlPLObject::SaveSwapFile(DatumPoint.x, DatumPoint.y, DatumPoint.z);
// 
// 		CNtlInstanceEffect::SaveSwapFile(DatumPoint.x, DatumPoint.y, DatumPoint.z);
#endif
	}

	DestroyOccluderProxy();

#ifndef dNTL_WORLD_TOOL_MODE

	if(m_vecNtlPLEntityOrnament.size())
	{
		sNTL_EVENT_TRI_DOODADS NtlEventTriDoodads;
		NtlEventTriDoodads.FlgTriDoodads = 1;
		NtlEventTriDoodads.vecDat.reserve(10);

		for(int i = 0; i < static_cast<int>(m_vecNtlPLEntityOrnament.size()); ++i)
		{
			if(m_vecNtlPLEntityOrnament[i]->GetFlags() & NTL_PLEFLAG_TRIGGER)
			{
				NtlEventTriDoodads.vecDat.push_back(m_vecNtlPLEntityOrnament[i]);
			}
		}

		if(NtlEventTriDoodads.vecDat.size())
		{
			CNtlPLEventGenerator::OnTriDoodadsEvent(&NtlEventTriDoodads);
			NtlEventTriDoodads.vecDat.clear();
		}
	}

#endif

	for(int i = 0; i < static_cast<int>(m_vecNtlPLEntityOrnament.size()); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecNtlPLEntityOrnament[i]);
	}
	m_vecNtlPLEntityOrnament.clear();

	for(int i = 0; i < static_cast<int>(m_vecNtlPLEntityEffect.size()); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecNtlPLEntityEffect[i]);
	}
	m_vecNtlPLEntityEffect.clear();

	for(int i = 0; i < static_cast<int>(m_vecNtlPLEntitySound.size()); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecNtlPLEntitySound[i]);
	}
	m_vecNtlPLEntitySound.clear();

	for(int i = 0; i < static_cast<int>(m_vecNtlPLEntitySoundBGM.size()); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecNtlPLEntitySoundBGM[i]);
	}
	m_vecNtlPLEntitySoundBGM.clear();

	// CZ-SS
// 	for(int i = 0; i < static_cast<int>(m_vecNtlPLEntitySS.size()); ++i)
// 	{
// 		GetSceneManager()->DeleteEntity(m_vecNtlPLEntitySS[i]);
// 	}
// 	m_vecNtlPLEntitySS.clear();

	for(int i = 0; i < static_cast<int>(m_vecNtlPLEntityDecal.size()); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecNtlPLEntityDecal[i]);
	}
	m_vecNtlPLEntityDecal.clear();

	for(int i = 0; i < static_cast<int>(m_vecNtlPLEntityWorldLight_Sphere.size()); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecNtlPLEntityWorldLight_Sphere[i]);
	}
	m_vecNtlPLEntityWorldLight_Sphere.clear();

	for(int i = 0; i < static_cast<int>(m_vecNtlPLEntityWorldLight_Box.size()); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecNtlPLEntityWorldLight_Box[i]);
	}
	m_vecNtlPLEntityWorldLight_Box.clear();

	// occluder quad
	for(int i = 0; i < static_cast<int>(m_vecNtlPLEntityOccluder_Quad.size()); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecNtlPLEntityOccluder_Quad[i]);
	}
	m_vecNtlPLEntityOccluder_Quad.clear();

	for(int i = 0; i < static_cast<int>(m_vecNtlPLEntityHeatHaze.size()); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecNtlPLEntityHeatHaze[i]);
	}
	m_vecNtlPLEntityHeatHaze.clear();

	for (int i = 0; i < static_cast<int>(m_vecNtlPLEntityLightObject.size()); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecNtlPLEntityLightObject[i]);
	}
	m_vecNtlPLEntityLightObject.clear();

	for (int i = 0; i < static_cast<int>(m_vecNtlPLEntityGameProperty.size()); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecNtlPLEntityGameProperty[i]);
	}
	m_vecNtlPLEntityGameProperty.clear();

	for (int i = 0; i < static_cast<int>(m_vecNtlPLEntityDecalVolume.size()); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecNtlPLEntityDecalVolume[i]);
	}
	m_vecNtlPLEntityDecalVolume.clear();

	for (int i = 0; i < static_cast<int>(m_vecNtlPLEntityDojo.size()); ++i)
	{
		GetSceneManager()->DeleteEntity(m_vecNtlPLEntityDojo[i]);
	}
	m_vecNtlPLEntityDojo.clear();

	if(m_pNtlPLEntityShoreLine)
	{
		GetSceneManager()->DeleteEntity(m_pNtlPLEntityShoreLine);
		m_pNtlPLEntityShoreLine = NULL;
	}

	GetSceneManager()->GetPlant()->OnUnload(this);
	for (int i = 0; i < static_cast<int>(m_vecPlantSet.size()); ++i)
	{
		sSECTOR_PLANT_SET_ATTR* pPlantSet = m_vecPlantSet.at(i);
		NTL_DELETE(pPlantSet);
	}
	m_vecPlantSet.clear();

	if(m_pTextureBase)
	{
		RwTextureDestroy(m_pTextureBase);
		m_pTextureBase = NULL;
	}

#ifdef dNTL_WORLD_COMBINE
	if (m_pTextureCombine)
	{
		RwTextureDestroy(m_pTextureCombine);
		m_pTextureCombine = NULL;
	}
#endif

	for(int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		if(m_pTextureSplat[i])
		{
			RwTextureDestroy(m_pTextureSplat[i]);
			m_pTextureSplat[i] = NULL;
		}

		if(m_pTextureAlpha[i])
		{
			RwTextureDestroy(m_pTextureAlpha[i]);
			m_pTextureAlpha[i] = NULL;
		}
	}

	if(m_pAtomic)
	{
		RpAtomicSetFrame(m_pAtomic, NULL);
		RpAtomicDestroy(m_pAtomic);
		m_pAtomic = NULL;
	}	

	NTL_DELETE(m_pWater);			

#ifdef dNTL_WORLD_TOOL_MODE

	// release
	//delete [] m_pClrDiffusePalette;	m_pClrDiffusePalette = NULL;
	NTL_ARRAY_DELETE(m_pClrDiffusePalette);
	NTL_DELETE(m_pNtlWorldSL);

	NTL_DELETE(m_pNtlWorldPathEngineData);

#endif

	// shadow obj. should be saved before deleting
	NTL_DELETE(m_pNtlWorldShadow);

	m_bLoaded = FALSE;

	NTL_RETURNVOID();
}

#ifdef dNTL_WORLD_TOOL_MODE

void CNtlWorldSector::SaveSwapFile()
{
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	if(_chdir("swap") == -1)
	{
		// create folders
		_mkdir("swap");
		_chdir("swap");
	}

	RwReal	XBasedOn0	= DatumPoint.x + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwReal	ZBasedOn0	= DatumPoint.z + dGET_WORLD_PARAM()->WorldSizeHalf;
	RwInt32 WidthNum	= (int)(XBasedOn0 / (float)dGET_WORLD_PARAM()->WorldSectorSize);
	RwInt32 DepthNum	= (int)(ZBasedOn0 / (float)dGET_WORLD_PARAM()->WorldSectorSize); 
	RwInt32 RetIdx		= WidthNum + (DepthNum * dGET_WORLD_PARAM()->WorldSectorNum);
	RwInt32 NumVert		= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;

	char chrRetIdx[40];
	::sprintf_s(chrRetIdx, 40, "%d_sec.dif", RetIdx);

	FILE* pFile;
	if(::fopen_s(&pFile, chrRetIdx, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << chrRetIdx << ")");
	}
	else
	{
		::fwrite(m_pClrDiffusePalette, sizeof(DWORD) * NumVert, 1, pFile);
		fclose(pFile);
	}

	// Slope Lighting Property
	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	if(_chdir("swap") == -1)
	{
		// create folders
		_mkdir("swap");
		_chdir("swap");
	}

	if(_chdir("sl") == -1)
	{
		// create folders
		_mkdir("sl");
		_chdir("sl");
	}

	::sprintf_s(chrRetIdx, 40, "%d_sec.sl", RetIdx);

	if(::fopen_s(&pFile, chrRetIdx, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << chrRetIdx << ")");
	}
	else
	{
		::fwrite(m_pNtlWorldSL, sizeof(sNTL_WORLD_SL), 1, pFile);
		::fclose(pFile);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	// projection shadow prop.
	m_pNtlWorldShadow->SaveSwapFile(RetIdx);

	// OCcluderQuad Swap
	CNtlPLOccluder_Quad::SaveSwapFile(DatumPoint.x, DatumPoint.y, DatumPoint.z);

	CNtlWorldPathEngineManager::GetInstance()->SaveSwapFile(RetIdx, m_pNtlWorldPathEngineData);

	CNtlPLHeatHazeObject::SaveSwapFile(DatumPoint.x, DatumPoint.y, DatumPoint.z);

	CNtlPLLightObject::SaveSwapFile(DatumPoint.x, DatumPoint.y, DatumPoint.z);

	CNtlPLObject::SaveSwapFile(DatumPoint.x, DatumPoint.y, DatumPoint.z);

	CNtlInstanceEffect::SaveSwapFile(DatumPoint.x, DatumPoint.y, DatumPoint.z);
}

void CNtlWorldSector::SetTransparentTile(RwBool _IsTileTransparency, RwV3d& Pos)
{
	RwV3d SPos;
	SPos.x = DatumPoint.x - (static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize) / 2.0f);
	SPos.z = DatumPoint.z - (static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize) / 2.0f);

	RwInt32 XCnt	= static_cast<RwInt32>((Pos.x - SPos.x) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize));
	RwInt32 ZCnt	= static_cast<RwInt32>((Pos.z - SPos.z) / static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize));
	XCnt			= abs(XCnt);
	ZCnt			= abs(ZCnt);

	// reverse order
	XCnt = dGET_WORLD_PARAM()->WorldSectorTileNum - XCnt - 1;

	RpGeometryLock(m_pAtomic->geometry, rpGEOMETRYLOCKPRELIGHT);
	{
		RwRGBA *pPrelights = RpGeometryGetPreLightColors(m_pAtomic->geometry);

		//|ㅡㅡㅡㅡㅡ| 4
		//|		     | 3
		//|		     | 2
		//|		     | 1
		//|ㅡㅡㅡㅡㅡ| 0
		// 0 1 2 3 4

		RwInt32 ArrayIdx = dGET_WORLD_PARAM()->WorldSectorVertNum * ZCnt + XCnt;
		if(ArrayIdx < 0 || ArrayIdx > dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum)
		{
			DBO_TRACE(FALSE, "CNtlWorldSector::SetTransparentTile, Wrong indexing.(" << ArrayIdx << ")");
		}

		if(_IsTileTransparency)
		{
			pPrelights[ArrayIdx].alpha												= 0;
			pPrelights[ArrayIdx + 1].alpha											= 0;
			pPrelights[ArrayIdx + dGET_WORLD_PARAM()->WorldSectorVertNum].alpha		= 0;
			pPrelights[ArrayIdx + dGET_WORLD_PARAM()->WorldSectorVertNum + 1].alpha	= 0;
			m_IsTileTransparency = TRUE;
		}
		else
		{
			pPrelights[ArrayIdx].alpha												= 255;
			pPrelights[ArrayIdx + 1].alpha											= 255;
			pPrelights[ArrayIdx + dGET_WORLD_PARAM()->WorldSectorVertNum].alpha		= 255;
			pPrelights[ArrayIdx + dGET_WORLD_PARAM()->WorldSectorVertNum + 1].alpha	= 255;
		}

		m_IsTileTransparency = FALSE;
		for (int z = 0; z < dGET_WORLD_PARAM()->WorldSectorVertNum; ++z)
		{
			for (int x = 0; x < dGET_WORLD_PARAM()->WorldSectorVertNum; ++x)
			{
				RwInt32 Idx = dGET_WORLD_PARAM()->WorldSectorVertNum * z + x;
				if (pPrelights[Idx].alpha <= 0)
				{
					m_IsTileTransparency = TRUE;
					break;
				}
			}
		}
	}
	RpGeometryUnlock(m_pAtomic->geometry);

	// verify sector tiles transparency
}

#endif

const WORD* CNtlWorldSector::GetBodySystemIndexBuff(gbDxGenLODIdx *pLODIdx)
{
	bool T, L, R, B;
	if(m_LODAttr.TSectionLevel < m_LODAttr.MipMapLevel) T = true; else T = false;
	if(m_LODAttr.LSectionLevel < m_LODAttr.MipMapLevel) L = true; else L = false;
	if(m_LODAttr.RSectionLevel < m_LODAttr.MipMapLevel) R = true; else R = false;
	if(m_LODAttr.BSectionLevel < m_LODAttr.MipMapLevel) B = true; else B = false;

	return pLODIdx->GetSystemBody(m_LODAttr.MipMapLevel, T, L, R, B);
}

const WORD* CNtlWorldSector::GetLinkSystemIndexBuff(gbDxGenLODIdx *pLODIdx, int Dir)
{
	if(Dir == CD_TOP)
		if(m_LODAttr.MipMapLevel > m_LODAttr.TSectionLevel)
			return pLODIdx->GetSystemLink(m_LODAttr.MipMapLevel, m_LODAttr.TSectionLevel, CD_TOP);
	if(Dir == CD_LEFT)
		if(m_LODAttr.MipMapLevel > m_LODAttr.LSectionLevel)
			return pLODIdx->GetSystemLink(m_LODAttr.MipMapLevel, m_LODAttr.LSectionLevel, CD_LEFT);
	if(Dir == CD_RIGHT)
		if(m_LODAttr.MipMapLevel > m_LODAttr.RSectionLevel)
			return pLODIdx->GetSystemLink(m_LODAttr.MipMapLevel, m_LODAttr.RSectionLevel, CD_RIGHT);
	if(Dir == CD_BOTTOM)
		if(m_LODAttr.MipMapLevel > m_LODAttr.BSectionLevel)
			return pLODIdx->GetSystemLink(m_LODAttr.MipMapLevel, m_LODAttr.BSectionLevel, CD_BOTTOM);

	return NULL;
}

const LPDIRECT3DINDEXBUFFER9 CNtlWorldSector::GetBodyIndexBuff(gbDxGenLODIdx *pLODIdx)
{
	bool T, L, R, B;
	if(m_LODAttr.TSectionLevel < m_LODAttr.MipMapLevel) T = true; else T = false;
	if(m_LODAttr.LSectionLevel < m_LODAttr.MipMapLevel) L = true; else L = false;
	if(m_LODAttr.RSectionLevel < m_LODAttr.MipMapLevel) R = true; else R = false;
	if(m_LODAttr.BSectionLevel < m_LODAttr.MipMapLevel) B = true; else B = false;

	return pLODIdx->GetBody(m_LODAttr.MipMapLevel, T, L, R, B);
}

const LPDIRECT3DINDEXBUFFER9 CNtlWorldSector::GetLinkIndexBuff(gbDxGenLODIdx *pLODIdx, int Dir)
{
	if(Dir == CD_TOP)
		if(m_LODAttr.MipMapLevel > m_LODAttr.TSectionLevel)
			return pLODIdx->GetLink(m_LODAttr.MipMapLevel, m_LODAttr.TSectionLevel, CD_TOP);
	if(Dir == CD_LEFT)
		if(m_LODAttr.MipMapLevel > m_LODAttr.LSectionLevel)
			return pLODIdx->GetLink(m_LODAttr.MipMapLevel, m_LODAttr.LSectionLevel, CD_LEFT);
	if(Dir == CD_RIGHT)
		if(m_LODAttr.MipMapLevel > m_LODAttr.RSectionLevel)
			return pLODIdx->GetLink(m_LODAttr.MipMapLevel, m_LODAttr.RSectionLevel, CD_RIGHT);
	if(Dir == CD_BOTTOM)
		if(m_LODAttr.MipMapLevel > m_LODAttr.BSectionLevel)
			return pLODIdx->GetLink(m_LODAttr.MipMapLevel, m_LODAttr.BSectionLevel, CD_BOTTOM);

	return NULL;
}

const int CNtlWorldSector::GetBodyPrimitiveNum(gbDxGenLODIdx *pLODIdx)
{
	bool T, L, R, B;
	if(m_LODAttr.TSectionLevel < m_LODAttr.MipMapLevel) T = true; else T = false;
	if(m_LODAttr.LSectionLevel < m_LODAttr.MipMapLevel) L = true; else L = false;
	if(m_LODAttr.RSectionLevel < m_LODAttr.MipMapLevel) R = true; else R = false;
	if(m_LODAttr.BSectionLevel < m_LODAttr.MipMapLevel) B = true; else B = false;

	return pLODIdx->GetBodyPrimitiveNum(m_LODAttr.MipMapLevel, T, L, R, B);
}

const int CNtlWorldSector::GetLinkPrimitiveNum(gbDxGenLODIdx *pLODIdx, int Dir)
{
	if(Dir == CD_TOP)
		return pLODIdx->GetLinkPrimitiveNum(m_LODAttr.MipMapLevel, m_LODAttr.TSectionLevel, CD_TOP);
	if(Dir == CD_LEFT)
		return pLODIdx->GetLinkPrimitiveNum(m_LODAttr.MipMapLevel, m_LODAttr.LSectionLevel, CD_LEFT);
	if(Dir == CD_RIGHT)
		return pLODIdx->GetLinkPrimitiveNum(m_LODAttr.MipMapLevel, m_LODAttr.RSectionLevel, CD_RIGHT);
	if(Dir == CD_BOTTOM)
		return pLODIdx->GetLinkPrimitiveNum(m_LODAttr.MipMapLevel, m_LODAttr.BSectionLevel, CD_BOTTOM);

	return NULL;
}

void CNtlWorldSector::InsertIn2World(CNtlPLEntity *pNtlPLEntity, RwInt32 IdxMenu)
{
	NTL_FUNCTION("CNtlWorldSector::InsertIn2World");

	switch(IdxMenu)
	{
	case 0:
		{
			m_vecNtlPLEntityOrnament.push_back(pNtlPLEntity);
		}
		break;
	case 1:
		{
			m_vecNtlPLEntityEffect.push_back(pNtlPLEntity);
		}
		break;
	case 2:
		{
			if (pNtlPLEntity->GetClassType() == PLENTITY_SOUND)
			{
				m_vecNtlPLEntitySound.push_back(pNtlPLEntity);
			}
			else if (pNtlPLEntity->GetClassType() == PLENTITY_SOUND_BGM)
			{
				m_vecNtlPLEntitySoundBGM.push_back(pNtlPLEntity);
			}			
		}
		break;
		// CZ-SS
// 	case 3:
// 		{
// 			m_vecNtlPLEntitySS.push_back(pNtlPLEntity);
// 		}
// 		break;
	case 4:
		{
			m_vecNtlPLEntityDecal.push_back(pNtlPLEntity);
		}
		break;
	case 5:
		{
			CNtlPLWorldLight_Base* pWorldLight = (CNtlPLWorldLight_Base*)pNtlPLEntity;
			switch (pWorldLight->GetWorldLightType())
			{
			case EPLWORLDLIGHT_SPHERE:
				{
					m_vecNtlPLEntityWorldLight_Sphere.push_back(pNtlPLEntity);
					break;
				}
			case EPLWORLDLIGHT_BOX:
				{
					m_vecNtlPLEntityWorldLight_Box.push_back(pNtlPLEntity);
					break;
				}
			default:
				NTL_ASSERT(FALSE, "WorldLightType Is Not Define");
			}
		}
		break;
	case 6:
		{
			CNtlPLOccluder_Base* pOccluder = (CNtlPLOccluder_Base*)pNtlPLEntity;
			switch (pOccluder->GetOccluderType())
			{
			case EPLOCCLUDER_QUAD:
				{
					m_vecNtlPLEntityOccluder_Quad.push_back(pNtlPLEntity);
				}
				break;
			default: 
				NTL_ASSERT(FALSE, "OccluderType Is Not Define");
			}
		}
		break;
	case 7:
		{
			m_vecNtlPLEntityHeatHaze.push_back(pNtlPLEntity);
		}
		break;
	case 8:
		{
			if (((CNtlPLLightObject*)pNtlPLEntity)->IsRealLight())
			{
				m_vecNtlPLEntityLightObject.push_back(pNtlPLEntity);
			}
			else
			{
				m_vecNtlPLEntityLightObjectDummy.push_back(pNtlPLEntity);
			}
		}
		break;
	case 9:
		{
			m_vecNtlPLEntityGameProperty.push_back(pNtlPLEntity);
		}
		break;
	case 10:
		{
			m_vecNtlPLEntityDecalVolume.push_back(pNtlPLEntity);
		}
		break;
	case 11:
		{
			m_vecNtlPLEntityDojo.push_back(pNtlPLEntity);
		}
		break;
	}

	NTL_RETURNVOID();
}

void CNtlWorldSector::DeleteShadow()
{
	m_pNtlWorldShadow->Delete();
}

void CNtlWorldSector::UpdateShadow()
{
	if(!m_pNtlWorldShadow)
		return;

	m_pNtlWorldShadow->Update();
}

void CNtlWorldSector::Update()
{
	UpdateShadow();
}

VOID CNtlWorldSector::CreateSectorPVS(RwInt32 _SelfSectorIdx, RwReal fEpsilsonHeight)
{
	/*
	DestroySectorPVS(_SelfSectorIdx);
	m_pNtlWorldSectorPVS = NTL_NEW CNtlWorldSectorPVS;
	m_pNtlWorldSectorPVS->CreatePVS(_SelfSectorIdx);
	*/
	m_pNtlWorldSectorPVS->CreatePVS(_SelfSectorIdx, fEpsilsonHeight);
}

VOID CNtlWorldSector::DestroySectorPVS(RwInt32 _SelfSectorIdx)
{
	/*
	if(m_pNtlWorldSectorPVS)
	{
		m_pNtlWorldSectorPVS->DestroyPVS(_SelfSectorIdx);
		NTL_DELETE(m_pNtlWorldSectorPVS);
	}
	*/
	m_pNtlWorldSectorPVS->DestroyPVS(_SelfSectorIdx);
}