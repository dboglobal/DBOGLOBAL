#include "StdAfx.h"
#include "MiniMapManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlWE.h"
#include "NtlPLWorldEntity.h"
#include "ntlplapi.h"
#include "NtlWeControlUi.h"
#include "SpawnManager.h"
#include "NtlPLDef.h"

CMiniMapManager::CMiniMapManager(void)
{
	m_MMProp			= NULL;
	m_TexSize			= 512;
	m_CreateAll			= TRUE;
	m_IsCreating		= FALSE;
}

CMiniMapManager::~CMiniMapManager(void)
{
}

RwBool CMiniMapManager::Create(RwInt32 FieldIdx)
{
	m_IsCreating = TRUE;

	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
	RwV3d					MPos		= pFields[FieldIdx].GetSPos();

	// move to new field area and update(camera, app.)
	MPos.x	+=	dGET_WORLD_PARAM()->WorldSectorSize;
	MPos.z	+=	dGET_WORLD_PARAM()->WorldSectorSize;
	MPos.y	=	(*RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)))).y;

	pFieldMgr->SetAnotherField(TRUE);
	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlPLWorldEntity()->SetPortalPosition(MPos);
	CNtlWeControlUi::GetInstance().Update();
	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.OnIdle();

	// create minimap the camera will see
	
	
	RwRaster*	pMiniMapRaster	= RwRasterCreate(CMiniMapManager::GetInstance()->GetTexSize(), CMiniMapManager::GetInstance()->GetTexSize(), 32, rwRASTERTYPECAMERATEXTURE);
	RwRaster*	pMiniMapRasterZ	= RwRasterCreate(CMiniMapManager::GetInstance()->GetTexSize(), CMiniMapManager::GetInstance()->GetTexSize(), 32, rwRASTERTYPEZBUFFER);
	RwRaster*	pSrcRaster		= RwCameraGetRaster(CNtlPLGlobal::m_RwCamera);
	RwRaster*	pSrcRasterZ		= RwCameraGetZRaster(CNtlPLGlobal::m_RwCamera);

	RwCameraSetRaster(CNtlPLGlobal::m_RwCamera, pMiniMapRaster);
	RwCameraSetZRaster(CNtlPLGlobal::m_RwCamera, pMiniMapRasterZ);

	SaveMinimap(FieldIdx, NTL_PLEFLAG_MINIMAP_LAYER_TWO);
	SaveMinimap(FieldIdx, NTL_PLEFLAG_MINIMAP_LAYER_ONE);

	RwCameraSetRaster(CNtlPLGlobal::m_RwCamera, pSrcRaster);
	RwCameraSetZRaster(CNtlPLGlobal::m_RwCamera, pSrcRasterZ);
	RwRasterDestroy(pMiniMapRaster);
	RwRasterDestroy(pMiniMapRasterZ);

	m_IsCreating = FALSE;

	return TRUE;
}

void CMiniMapManager::SaveMinimap(RwInt32 FieldIdx, RwUInt32 uiLayer)
{
	if (uiLayer == NTL_PLEFLAG_MINIMAP_LAYER_NONE)
	{
		return;
	}
	else if (uiLayer == NTL_PLEFLAG_MINIMAP_LAYER_TWO)
	{
		// Effect 및 Object 중에 LAYER_TWO로 설정되어 있는 것이 있는가 확인한다.
		// 다른 Entity들은 기본적으로 TWO를 설정할 수 없다.
		CNtlWorldFieldManager*	pMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();
		CNtlWorldSector*		pSectors = pMgr->m_pSectors;
	
		RwBool		IsIn	= FALSE;
		RwV3d		vPosS	= const_cast<CNtlWorldField*>(pMgr->GetFields())[FieldIdx].GetSPos();
		RwV3d		vPosE;

		vPosE.x		= vPosS.x + dGET_WORLD_PARAM()->WorldFieldSize + dGET_WORLD_PARAM()->WorldSectorSize;
		vPosE.z		= vPosS.z + dGET_WORLD_PARAM()->WorldFieldSize + dGET_WORLD_PARAM()->WorldSectorSize;

		vPosS.x		= vPosS.x - dGET_WORLD_PARAM()->WorldSectorSize;
		vPosS.z		= vPosS.z - dGET_WORLD_PARAM()->WorldSectorSize;

		for (RwReal z = vPosS.z; z < vPosE.z; z += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for (RwReal x = vPosS.x; x < vPosE.x; x += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				RwV3d vPos; vPos.x = x; vPos.z = z;

				RwInt32 iSectorIdx = pMgr->GetSectorIdx(vPos);
				if (iSectorIdx != -1)
				{
					CNtlWorldSector* pSector = &pSectors[iSectorIdx];

					for (int i = 0; i < (int)pSector->m_vecNtlPLEntityOrnament.size(); ++i)
					{
						if (pSector->m_vecNtlPLEntityOrnament.at(i)->GetMinimapLayer() & NTL_PLEFLAG_MINIMAP_LAYER_TWO)
						{
							IsIn |= TRUE;
							goto LAYER_TWO_CHECK;
						}
					}
					for (int i = 0; i < (int)pSector->m_vecNtlPLEntityEffect.size(); ++i)
					{
						if (pSector->m_vecNtlPLEntityEffect.at(i)->GetMinimapLayer() & NTL_PLEFLAG_MINIMAP_LAYER_TWO)
						{
							IsIn |= TRUE;
							goto LAYER_TWO_CHECK;
						}
					}
				}
			}
		}

LAYER_TWO_CHECK:
		if (IsIn == FALSE)
		{
			return;
		}
	}

	CNtlPLGlobal::m_uiMinimapLayer = uiLayer;

	RwRGBA rgbaBackgroundColor = { 0, 0, 0, 0 };
	if (RwCameraClear(CNtlPLGlobal::m_RwCamera, &rgbaBackgroundColor, rwCAMERACLEARZ | rwCAMERACLEARIMAGE))
	{
		if(RwCameraBeginUpdate(CNtlPLGlobal::m_RwCamera))
		{
			if(((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.m_pVisualManager)
			{
				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.m_pVisualManager->Render();
				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->Render();

				GetSpawnInstance()->RenderMMProp(GetMMProp(), FieldIdx);
			}

			RwCameraEndUpdate(RwCameraGetCurrentCamera());
		}
	}

	RwChar acFilename[256];	
	switch (uiLayer)
	{
	case NTL_PLEFLAG_MINIMAP_LAYER_ONE:
		sprintf_s(acFilename, 256, "Tool\\SABER\\Texture\\Mini Map\\%.4d.bmp", FieldIdx);
		break;
	case NTL_PLEFLAG_MINIMAP_LAYER_TWO:
		sprintf_s(acFilename, 256, "Tool\\SABER\\Texture\\Mini Map\\%.4d_S.bmp", FieldIdx);
		break;
	}	

	API_PL_RasterSaveImage(RwCameraGetRaster(CNtlPLGlobal::m_RwCamera), acFilename);

	CNtlPLGlobal::m_uiMinimapLayer = NTL_PLEFLAG_MINIMAP_LAYER_NONE;
}