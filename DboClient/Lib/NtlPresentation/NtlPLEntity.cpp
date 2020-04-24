#include "precomp_ntlpresentation.h"
#include "NtlPLEntity.h"

// core
#include "NtlDebug.h"

// presentaiton
#include "NtlPLElapsedController.h"
#include "NtlPLSceneManager.h"
#include "NtlPLEntityBlend.h"

CNtlPLEntity::CNtlPLEntity() 
{ 
#ifdef dNTL_WORLD_CULLING_NEW
	m_uiRenderFrame		= -1;
#else
	m_uiCullIID			= 0xffffffff;
#endif
	
	m_uiClassID			= PLENTITY_INVALID_TYPE; 
	m_uiLayer			= PLENTITY_LAYER_INVALID;
	m_uiMinimapLayer	= NTL_PLEFLAG_MINIMAP_LAYER_NONE;
	m_uiFlags			= 0; 
	m_uiSID				= 0xffffffff;
	m_uiSerialID		= 0xffffffff;
	m_ObjNameIdx		= 0xffffffff;
	m_byPickOrder		= NTL_PICK_ORDER_NORMAL;

	m_uiCullFlags		= 0;

	m_fWeightAlpha				= 1.0f;
	m_fWeightElapsedTime		= 1.0f;
	m_fVisibleCullingDistance	= 500.0f;
	m_pElapController			= NULL;
	m_pBlendController			= NULL;
}


CNtlPLEntity::~CNtlPLEntity()
{
	if(m_pElapController)
	{
		NTL_DELETE(m_pElapController);
	}

	if(m_pBlendController)
	{
		NTL_DELETE(m_pBlendController);
	}
}


RwBool CNtlPLEntity::Update(RwReal fElapsed)
{
	if(m_pElapController)
	{
		m_pElapController->Update(fElapsed);
		if(m_pElapController->IsFinish())
		{
			NTL_DELETE(m_pElapController);
		}
	}

	if(m_pBlendController)
	{
		m_pBlendController->Update(fElapsed);        

		if(m_pBlendController->IsFinish())
		{
			NTL_DELETE(m_pBlendController);
		}
	}

	return TRUE;
}

#ifdef dNTL_WORLD_CULLING_NEW
RwBool CNtlPLEntity::CullingTest(RwCamera* pRwCamera, RwUInt16 uiRenderFrame)
{
#ifdef _DEBUG
	++CNtlPLGlobal::m_uiCullTestCnt;
#endif

	// Culiing Test를 시도 한다면 Culling을 사용하겠다는 의미다. 초기값을 변경한다.
	if (m_uiRenderFrame != uiRenderFrame)
	{
		m_uiRenderFrame= uiRenderFrame;
		// PVS Flag는 기존의 내용을 유지 해야 한다.
		// 한 Frame에서 한번만 시도 되어야 하며, 한번만 내용이 적용 되어야 한다.
		m_uiCullFlags &= NTL_PLEFLAG_CULLED_PVS;
	}

	return ((m_uiFlags & NTL_PLEFLAG_NOT_VISIBLE) || (m_uiCullFlags && CNtlPLGlobal::m_UseTerrainPVSMode));
}
#else
RwBool CNtlPLEntity::CullingTest(RwCamera* pRwCamera)
{
#ifdef _DEBUG
	++CNtlPLGlobal::m_uiCullTestCnt;
#endif

	m_uiCullFlags &= NTL_PLEFLAG_CULLED_PVS;
	return ((m_uiFlags & NTL_PLEFLAG_NOT_VISIBLE) || (m_uiCullFlags && CNtlPLGlobal::m_UseTerrainPVSMode));
}
#endif

/*RwBool CNtlPLEntity::CullingTest(RwCamera* pRwCamera, const RpAtomic* pAtomic)
{
	if ((m_uiFlags & NTL_PLEFLAG_NOT_VISIBLE) || (m_uiCullFlags && CNtlPLGlobal::m_UseTerrainPVSMode))
	{
		return TRUE;
	}
	else if (!RwCameraFrustumTestSphere(pRwCamera, &pAtomic->worldBoundingSphere))
	{
		return TRUE;
	}
	return FALSE;
}*/


void CNtlPLEntity::SetWeightAlpha(RwReal fWeightValue)
{
	m_fWeightAlpha = fWeightValue;
}

void CNtlPLEntity::SetVisibleCullingDistance(RwReal fDistance)
{
	/*
#define LIMIT_DISTANCE_FILTER_DIST	450.0f

	if(m_uiClassID != PLENTITY_OBJECT)
		m_fVisibleCullingDistance = fDistance; 
	else
	{
		if(m_fVisibleCullingDistance > LIMIT_DISTANCE_FILTER_DIST)
			if(fDistance < LIMIT_DISTANCE_FILTER_DIST)
				GetSceneManager()->AddDistanceFilter(this);
		else
			if(fDistance > LIMIT_DISTANCE_FILTER_DIST)
				GetSceneManager()->RemoveDistanceFilter(this);

		m_fVisibleCullingDistance = fDistance;
	}
	*/

	m_fVisibleCullingDistance = fDistance;
}

CNtlPLEntityBlendController* CNtlPLEntity::GetAlphaBlendController(void)
{
	if(!m_pBlendController)
	{
		m_pBlendController = NTL_NEW CNtlPLEntityBlendController(this);
	}

	return m_pBlendController;
}

void CNtlPLEntity::CreateWeightElapsedController(RwReal fLifeTime, RwReal fWeightValue) 
{
	if(!m_pElapController)
	{
		m_pElapController = NTL_NEW CNtlElapsedControllerGroup;
		m_pElapController->SetControlEntity(this);
	}

	m_pElapController->CreateController(fLifeTime, fWeightValue);
}


void CNtlPLEntity::DeleteWeightElapsedController(void)
{
	if(m_pElapController)
	{
		NTL_DELETE(m_pElapController);
	}
}