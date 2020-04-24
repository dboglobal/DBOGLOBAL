#include "precomp_ntlpresentation.h"
#include "NtlPLGlobal.h"
#include "NtlPLRenderState.h"
#include "NtlPLFog.h"
#include "ntlworldcommon.h"
#include "ntlworldinterface.h"
#include "NtlPLEvent.h"
#include "NtlCoreUtil.h"
#include "NtlPLApi.h"
#include "NtlPLWorldState.h"

#include "NtlPLOptionManager.h"


void CNtlPLFog::Initialize( void )
{
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED);
	SetLayer(PLENTITY_LAYER_FOG);
	SetClassType(PLENTITY_FOG);

	m_eType				= rwFOGTYPELINEAR;

	m_RGBA.red			= 255;
	m_RGBA.green		= 255;
	m_RGBA.blue			= 255;
	m_RGBA.alpha		= 255;

	m_Plane[0]			= 10.0f;
	m_Plane[1]			= 456.0f;

	m_bSwitch			= TRUE;
	m_RestTime4Change	= -999.0f;
}

void CNtlPLFog::HandleEvents(RWS::CMsg &pMsg)
{
	if (CheckEventsWorldEffectChanged(pMsg))
	{
		sNTL_FIELD_PROP* pNtlFieldProp	= reinterpret_cast<sNTL_FIELD_PROP*>(pMsg.pData);

		if(m_RGBA.red	== pNtlFieldProp->_FogColor.red &&
			m_RGBA.green == pNtlFieldProp->_FogColor.green &&
			m_RGBA.blue	== pNtlFieldProp->_FogColor.blue &&
			static_cast<int>(m_Plane[0])	== static_cast<int>(pNtlFieldProp->_FogCamPlane[0]) &&
			static_cast<int>(m_Plane[1])	== static_cast<int>(pNtlFieldProp->_FogCamPlane[1]))
		{
			// Same fog properties, just ignore messages
			return;
		}
		else
		{
			m_RestTime4Change	= dFOG_EFFECT_SWITCHING_TIME;

			m_FogRGBA4Change	= pNtlFieldProp->_FogColor;
			m_FogPlanes[0]		= pNtlFieldProp->_FogCamPlane[0];
			m_FogPlanes[1]		= pNtlFieldProp->_FogCamPlane[1];

			m_FogRGBA4ChangeOld = m_RGBA;
			m_FogPlanesOld[0]	= m_Plane[0];
			m_FogPlanesOld[1]	= m_Plane[1];
		}
	}
}

RwBool CNtlPLFog::Create( const SPLEntityCreateParam * pParam )
{
	NTL_FUNCTION( "CNtlPLFog::Create" );

	LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED, 0);

	NTL_RETURN( TRUE );
}


void CNtlPLFog::Destroy( void )
{
	RwRenderStateSet( rwRENDERSTATEFOGENABLE, (void *)FALSE );

	UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED);

	return;
}

RwBool CNtlPLFog::Update(RwReal fElapsed)
{
	if(m_RestTime4Change >= 0.0f)
	{
		m_RestTime4Change -= g_GetElapsedTime();
		if(m_RestTime4Change < 0)
		{
			m_RestTime4Change	= -999.0f;

			m_RGBA				= m_FogRGBA4Change;
			m_Plane[0]			= m_FogPlanes[0];
			m_Plane[1]			= m_FogPlanes[1];
		}
		else
		{
			m_RGBA.red		= RpNtlWorldUInt8PropLerp(m_FogRGBA4ChangeOld.red, m_FogRGBA4Change.red, m_RestTime4Change, dFOG_EFFECT_SWITCHING_TIME);
			m_RGBA.green	= RpNtlWorldUInt8PropLerp(m_FogRGBA4ChangeOld.green, m_FogRGBA4Change.green, m_RestTime4Change, dFOG_EFFECT_SWITCHING_TIME);
			m_RGBA.blue		= RpNtlWorldUInt8PropLerp(m_FogRGBA4ChangeOld.blue, m_FogRGBA4Change.blue, m_RestTime4Change, dFOG_EFFECT_SWITCHING_TIME);
			m_Plane[0]		= RpNtlWorldRealPropLerp(m_FogPlanesOld[0], m_FogPlanes[0], m_RestTime4Change, dFOG_EFFECT_SWITCHING_TIME);
			m_Plane[1]		= RpNtlWorldRealPropLerp(m_FogPlanesOld[1], m_FogPlanes[1], m_RestTime4Change, dFOG_EFFECT_SWITCHING_TIME);
		}
	}

	// DN effect
	RwInt32 TFactor = -static_cast<RwUInt8>(dGET_WORLD_PARAM()->ClrDayAndNight & 0x000000ff);
	::CopyMemory(&m_RGBA4DN, &m_RGBA, sizeof(RwRGBA));
	::API_PL_8BColorClamp(&m_RGBA4DN.red, TFactor);
	::API_PL_8BColorClamp(&m_RGBA4DN.green, TFactor);
	::API_PL_8BColorClamp(&m_RGBA4DN.blue, TFactor);

	return TRUE;
}

RwBool CNtlPLFog::IsWorking()
{
	return m_bSwitch;
}

void CNtlPLFog::Switch()
{
	m_bSwitch = !m_bSwitch;
}

RwBool CNtlPLFog::Render( void )
{
	RwReal fPlaneNear	= m_Plane[0] - 512.0f + dFOG_EFFECT_FAR;
	RwReal fPlaneFar	= dFOG_EFFECT_FAR;

	fPlaneNear	= CNtlPLGlobal::m_RwCamera->nearPlane + (fPlaneNear > dFOG_EFFECT_NEAR ? fPlaneNear : (m_Plane[0] < dFOG_EFFECT_NEAR ? m_Plane[0] : dFOG_EFFECT_NEAR));
	fPlaneFar	= CNtlPLGlobal::m_RwCamera->nearPlane + (fPlaneFar < m_Plane[1] ? fPlaneFar : m_Plane[1]);

	CLAMP(fPlaneNear, dFOG_EFFECT_NEAR, m_Plane[0]);
	CLAMP(fPlaneFar, dFOG_EFFECT_NEAR, m_Plane[1]);

	BegLinearFogRenderState(RWRGBALONG(m_RGBA4DN.red, m_RGBA4DN.green, m_RGBA4DN.blue, 255), fPlaneNear, fPlaneFar);

	if(!m_bSwitch)
	{
		EndLinearFogRenderState();
	}
	return TRUE;
}