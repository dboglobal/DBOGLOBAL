#include "precomp_ntlpresentation.h"

#include "../Ntl_Plugin_World/world/NtlPluginWorldGlobal.h"

#include "NtlDebug.h"

#include "NtlWorldCommon.h"
#include "NtlWorldInterface.h"

#include "NtlWorldFieldManager.h"

#include "NtlPLPlant.h"
#include "NtlPLObject.h"
#include "NtlPLHeatHaze.h"
#include "NtlPLWorldEntity.h"

#include "NtlPLSceneManager.h"
//#include "NtlSpecularManager.h"

#include "NtlPLWeatherHandler.h"

#include "NtlPLOptionManager.h"

CNtlPLOptionManager* CNtlPLOptionManager::g_pOptionManager = NULL;

void CNtlPLOptionManager::CreateInstance()
{
	if (!g_pOptionManager)
	{
		g_pOptionManager = NTL_NEW CNtlPLOptionManager;
	}
}

void CNtlPLOptionManager::DestroyInstance()
{
	NTL_DELETE(g_pOptionManager);
}

CNtlPLOptionManager::CNtlPLOptionManager()
{
	m_fFarObject		= 512.0f;
	m_fFarTerrain		= 512.0f;
	m_bWaterSpecular	= TRUE;
	m_uiWeatherLevel	= 0;

#ifdef dNTL_WORLD_COMBINE
	m_bTerrainCombine	= TRUE;
#endif
}

CNtlPLOptionManager::~CNtlPLOptionManager()
{
}

RwBool CNtlPLOptionManager::Create()
{
	return TRUE;
}

void CNtlPLOptionManager::Destroy()
{
}

RwBool CNtlPLOptionManager::Update(const RwCamera* pCamera, RwReal fElapsed)
{
// #ifdef dNTL_WORLD_TOOL_MODE
// 	if (!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_VISIBILTITY_DIST))
// 	{
// 		CNtlPLObject::SetFarPlane(pCamera->farPlane);
// 	}
// 	else
// #endif
//	{
//		CNtlPLObject::SetFarPlane((m_fFarObject < pCamera->farPlane ? m_fFarObject : pCamera->farPlane));
//	}
	
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNtlPLOptionManager::SetObjectFar(RwReal fFar)
{
	m_fFarObject = fFar;
	
	CLAMP(m_fFarTerrain, 50.0f, 2048.0f);
}

RwReal CNtlPLOptionManager::GetObjectFar()
{
	//return m_fFarObject;
	if (m_fFarObject > m_fFarTerrain)
	{
		return m_fFarTerrain;
	}
	return m_fFarObject;
}

void CNtlPLOptionManager::SetTerrainFar(RwReal fFar)
{
	m_fFarTerrain = fFar;

	CLAMP(m_fFarTerrain, 50.0f, 2048.0f);
}

RwReal CNtlPLOptionManager::GetTerrainFar()
{
	return m_fFarTerrain;
}

void CNtlPLOptionManager::SetTerrainShadow(RwBool bRender)
{
	CNtlWorldObjVisionAnalyzer::GetInstance()->SetCheck(eNWOVA_PROJECTION_SHADOW, bRender);
}

RwBool CNtlPLOptionManager::GetTerrainShadow()
{
	return CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PROJECTION_SHADOW);
}

void CNtlPLOptionManager::SetWaterSpecular(RwBool bRender)
{
	m_bWaterSpecular = bRender;
}

RwBool CNtlPLOptionManager::GetWaterSpecular()
{
	return m_bWaterSpecular;
}

void CNtlPLOptionManager::SetWeatherLevel(RwUInt32 uiLevel)
{
	m_uiWeatherLevel = uiLevel;
	CLAMP(m_uiWeatherLevel, 0, dNTL_PL_WEATHER_PROP_NUM);

	if (GetSceneManager() && GetSceneManager()->GetWorld() &&
		GetSceneManager()->GetWorld()->GetWorldFieldMgr())
	{
		GetSceneManager()->GetWorld()->GetWorldFieldMgr()->GetWeatherHandler()->RefreshWeather();
	}
}

RwUInt32 CNtlPLOptionManager::GetWeatherLevel()
{
	return m_uiWeatherLevel;
}

void CNtlPLOptionManager::SetPlantClipDist(RwReal fDist)
{
	GetSceneManager()->GetPlant()->SetClipDistance(fDist);
}

RwReal CNtlPLOptionManager::GetPlantClipDist()
{
	return GetSceneManager()->GetPlant()->GetClipDistance();
}

void CNtlPLOptionManager::SetHeatHaze(RwBool bUse)
{
	CNtlPostEffectCamera::GetInstance()->GetHeatHaze()->SetEnabled(bUse);
}

RwBool CNtlPLOptionManager::GetHeatHaze()
{
	return CNtlPostEffectCamera::GetInstance()->GetHeatHaze()->GetEnabled();
}

void CNtlPLOptionManager::SetTextureQuality(RwUInt32 uiQualityLevel)
{
	SetNtlDDSMipmapSkipCnt(uiQualityLevel);
}

RwUInt32 CNtlPLOptionManager::GetTextureQuality()
{
	return GetNtlDDSMipmapSkipCnt();
}

#ifdef dNTL_WORLD_COMBINE
void CNtlPLOptionManager::SetTerrainTextrueCombine(RwBool bUse)
{
	m_bTerrainCombine = bUse;
}

RwBool CNtlPLOptionManager::GetTerrainTextrueCombine()
{
	return m_bTerrainCombine;
}
#endif