#include "precomp_ntlpresentation.h"
#include "NtlPLGlobal.h"
#include "NtlPLEntity.h"
#include "NtlCoreUtil.h"

#include "NtlPLOptionManager.h"

using std::ifstream;
#include <fstream>

// Character Scheduling load test global variable
RwReal		g_fCharScheduleTime = 0.3f;


RpWorld*					CNtlPLGlobal::m_pRpWorld = 0;
RwCamera*					CNtlPLGlobal::m_RwCamera = 0;
RpWorldSector*				CNtlPLGlobal::m_RpWorldSector = 0;
RwV3d						CNtlPLGlobal::m_vZeroV3;
RwMatrix					CNtlPLGlobal::m_matIden;	
RwV3d						CNtlPLGlobal::m_vIdenV3;
RwV3d						CNtlPLGlobal::m_vXAxisV3;
RwV3d						CNtlPLGlobal::m_vYAxisV3;
RwV3d						CNtlPLGlobal::m_vZAxisV3;
RwMatrix					CNtlPLGlobal::m_matNameBox;

std::string					CNtlPLGlobal::m_strAppPath = "";

RwBool						CNtlPLGlobal::m_bCollObjVisible = FALSE;
RwReal						CNtlPLGlobal::m_fDistFiterFrequency = 2.0f;	
RwReal						CNtlPLGlobal::m_fDistFiterObjFrequency = 1.0f;	// 오브젝트 페이드 블렌딩 타임
RwBool						CNtlPLGlobal::m_bWorldTerrainVisible = TRUE;
RwBool						CNtlPLGlobal::m_bWorldSkyVisible = TRUE;
RwBool						CNtlPLGlobal::m_bCharacterUpdateVisible = FALSE;
RwBool						CNtlPLGlobal::m_bDecalRender = TRUE;
RwReal						CNtlPLGlobal::m_fLensFlareForDNRatio = 1.0f;
CNtlPLEntity*				CNtlPLGlobal::m_pWHEntity = NULL;	
RwBool						CNtlPLGlobal::m_UseTerrainPVSMode = TRUE;
RwUInt32					CNtlPLGlobal::m_uiRenderFlag = 0x00000000;
RwBool						CNtlPLGlobal::m_IsMinimapRendering = FALSE;
RwUInt32					CNtlPLGlobal::m_uiMinimapLayer = NTL_PLEFLAG_MINIMAP_LAYER_NONE;
RwBool						CNtlPLGlobal::m_bObjectRender = TRUE;              ///< Object Render 유무 플래그
RwBool						CNtlPLGlobal::m_bRenderScouter = TRUE;
RwBool          		    CNtlPLGlobal::m_bEntityNotCulling = FALSE;
						
RwReal						CNtlPLGlobal::m_fIndoorPlaneHeight = 0.0f;

D3DMATRIX					CNtlPLGlobal::m_mD3D9IdentityMatrix = {	0.0f, 0.0f, 0.0f, 0.0f,
																	0.0f, 1.0f, 0.0f, 0.0f,
																	0.0f, 0.0f, 1.0f, 0.0f,
																	0.0f, 0.0f, 0.0f, 1.0f };

std::vector<std::string>	CNtlPLGlobal::m_vecPathMinimapObject;

RwUInt16					CNtlPLGlobal::m_uiRenderFrame = -1;

RwUInt32					CNtlPLGlobal::m_uiCullTestCnt		= 0;
RwUInt32					CNtlPLGlobal::m_uiCullTestAtomicCnt = 0;
RwUInt32					CNtlPLGlobal::m_uiRenderAtomicCnt	= 0;

RwInt32						CNtlPLGlobal::m_iCurrentPVSLayer	= -1;

void CNtlPLGlobal::Init(void)
{
	m_vZeroV3.x = 0.0f;
	m_vZeroV3.y = 0.0f;
	m_vZeroV3.z = 0.0f;

	m_vIdenV3.x = 1.0f;
	m_vIdenV3.y = 1.0f;
	m_vIdenV3.z = 1.0f;

	m_vXAxisV3.x = 1.0f;
	m_vXAxisV3.y = 0.0f;
	m_vXAxisV3.z = 0.0f;

	m_vYAxisV3.x = 0.0f;
	m_vYAxisV3.y = 1.0f;
	m_vYAxisV3.z = 0.0f;

	m_vZAxisV3.x = 0.0f;
	m_vZAxisV3.y = 0.0f;
	m_vZAxisV3.z = 1.0f;

	RwMatrixSetIdentity(&m_matIden);

	RwChar	szPath[NTL_MAX_DIR_PATH];
	GetCurrentDirectory( NTL_MAX_DIR_PATH, szPath );

	m_strAppPath.assign( szPath );

	m_fLensFlareForDNRatio = 1.0f;

	CNtlPLOptionManager::CreateInstance();
}

void CNtlPLGlobal::Terminate(void)
{
	CNtlPLOptionManager::DestroyInstance();

	m_pRpWorld = 0;
	m_RwCamera = 0;

	m_vecPathMinimapObject.clear();
}

void CNtlPLGlobal::SetActiveWorld(const RpWorld *pRpWorld)
{
	m_pRpWorld = const_cast<RpWorld*>(pRpWorld);
}

void CNtlPLGlobal::SetActiveCamera(const RwCamera *pRwCamera)
{
	m_RwCamera = const_cast<RwCamera*>(pRwCamera);
}

void CNtlPLGlobal::LoadMinimapPassObjectList(const RwChar* pFilename)
{
	m_vecPathMinimapObject.clear();

	char			acBuffer[NTL_MAX_DIR_PATH];
	std::ifstream	ifs;
	ifs.open(pFilename);
	if (ifs)
	{
		while(!ifs.eof())
		{
			ifs.getline(acBuffer, NTL_MAX_DIR_PATH);
			m_vecPathMinimapObject.push_back(acBuffer);
		}
		ifs.close();
	}
}