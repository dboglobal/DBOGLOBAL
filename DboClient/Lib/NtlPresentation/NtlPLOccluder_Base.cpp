#include "precomp_ntlpresentation.h"

#include "NtlWorldCommon.h"

#include "NtlOccluderManager.h"

#include "NtlPLOccluder_Base.h"

CNtlPLOccluder_Base::CNtlPLOccluder_Base()
{
#ifdef dNTL_WORLD_TOOL_MODE
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED);
#else
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED | NTL_PLEFLAG_NOT_VISIBLE);
#endif

	SetLayer(PLENTITY_LAYER_OCCLUDER);
	SetClassType(PLENTITY_OCCLUDER);

	m_vPos.x		= 0.0f;
	m_vPos.y		= 0.0f;
	m_vPos.z		= 0.0f;

	m_eOccluderFlag	= EPLOCCLUDER_FUNC_TERRAIN | EPLOCCLUDER_FUNC_OBJECT;
	m_bRunable		= FALSE;

	m_ePlaneType	= EPLOCCLUDER_PLANE_ONESIDE;

#ifdef dNTL_WORLD_TOOL_MODE
	m_bToolUI		= FALSE;
	m_iIndexPicked	= -1;
#endif
}

CNtlPLOccluder_Base::~CNtlPLOccluder_Base()
{
}

RwBool CNtlPLOccluder_Base::Update(RwReal fElapsed)
{
	NTL_FUNCTION("CNtlPLOccluder_Base::Update");

	switch (m_ePlaneType)
	{
	case EPLOCCLUDER_PLANE_ONESIDE:
		UpdateOneside(fElapsed);
		break;
	case EPLOCCLUDER_PLANE_TWOSIDE:
		UpdateTwoside(fElapsed);
		break;
	case EPLOCCLUDER_PLANE_BILLBOARD:
		UpdateBillboard(fElapsed);
		break;
	}

	m_bRunable = (!IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS)) && VisibilityTest();
	if (m_bRunable)
	{
		CNtlOccluderManager::GetInstance()->AddOccluderVisiblity(this);
	}

	NTL_RETURN(TRUE);
}

RwBool CNtlPLOccluder_Base::Render()
{
	NTL_FUNCTION("CNtlPLOccluder::Render");

	NTL_RETURN(TRUE);
}

RwBool CNtlPLOccluder_Base::SetProperty(const CNtlPLProperty *pData)
{
	NTL_FUNCTION("CNtlPLOccluder::SetProperty");

	NTL_RETURN(TRUE);
}

void CNtlPLOccluder_Base::SetOccluderFuncFlag(RwUInt32 eFlag)
{
	m_eOccluderFlag = eFlag | EPLOCCLUDER_FUNC_TERRAIN | EPLOCCLUDER_FUNC_OBJECT | EPLOCCLUDER_FUNC_CHARACTER;
}

void CNtlPLOccluder_Base::SetOccluderFuncFlag(RwUInt32 eFlag, RwBool bFlag)
{
	if (bFlag)
	{
		m_eOccluderFlag = m_eOccluderFlag | eFlag;
	}
	else
	{
		m_eOccluderFlag = m_eOccluderFlag & (~eFlag);
	}
}

#ifdef dNTL_WORLD_TOOL_MODE
void CNtlPLOccluder_Base::SetOccluderPVS(RwBool bPVS)
{
	if (IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS) == bPVS)
	{
		return;
	}

	CNtlOccluderManager::GetInstance()->ClearOccluderProxyList(this);
	
	SetOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS, bPVS);
	m_iIndexPicked	= -1;
}
#endif