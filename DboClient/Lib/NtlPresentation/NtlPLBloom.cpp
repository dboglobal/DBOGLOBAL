#include "precomp_ntlpresentation.h"
#include "NtlPLBloom.h"
#include "NtlPLGlobal.h"
#include "NtlPLEvent.h"
#include "NtlWorldCommon.h"
#include "NtlWorldInterface.h"
#include "NtlPostEffectCamera.h"


CNtlPLBloom::CNtlPLBloom(void)
{
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED | NTL_PLEFLAG_NOT_VISIBLE);
	SetLayer(PLENTITY_LAYER_NONE);
	SetClassType(PLENTITY_BLOOM);

	m_fTotalMonoPower	= dMONO_POWER_DEFAULT;
	m_MonoPower[0]		= dMONO_POWER_DEFAULT;
	m_MonoPower[1]		= dMONO_POWER_DEFAULT;
	m_MonoPower[2]		= dMONO_POWER_DEFAULT;
	m_RestTime4Change	= -999.0f;

	m_fTimeFadeIn		= 0.0f;
	m_fTimeFadeOut		= 0.0f;
	m_fTimeLightning	= 0.0f;
}

CNtlPLBloom::~CNtlPLBloom(void)
{
}

RwBool CNtlPLBloom::Create(const SPLEntityCreateParam * pParam)
{
	NTL_FUNCTION("CNtlPLBloom::Create");

	LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED, 0);

	NTL_RETURN(TRUE);
}


void CNtlPLBloom::Destroy( void ) 
{
	NTL_FUNCTION("CNtlPLBloom::Destroy");

	UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED);

	NTL_RETURNVOID();
}


RwBool CNtlPLBloom::SetProperty( const CNtlPLProperty * pData )
{
	return TRUE;
}

void CNtlPLBloom::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == NPEI_IS_ANOTHER_FIELD_CHANGED || pMsg.Id == NPEI_IS_ANOTHER_BLOCK_CHANGED)
	{
		sNTL_FIELD_PROP* pNtlFieldProp = reinterpret_cast<sNTL_FIELD_PROP*>(pMsg.pData);

		RwInt32 SrcCmp = static_cast<RwInt32>(m_MonoPower[0] * 10.0f);
		RwInt32 DstCmp = static_cast<RwInt32>(pNtlFieldProp->_MonoPower * 10.0f);

		if(SrcCmp == DstCmp)
		{
			// Same fog properties, just ignore messages
			return;
		}
		else
		{
			m_RestTime4Change	= dBLOOM_SWITCHING_TIME;
			m_MonoPower[1]		= m_MonoPower[0];
			m_MonoPower[2]		= pNtlFieldProp->_MonoPower;
		}
	}
}

RwBool CNtlPLBloom::Update(RwReal fElapsed)
{
	if(m_RestTime4Change >= 0.0f)
	{
		m_RestTime4Change -= g_GetElapsedTime();
		if(m_RestTime4Change < 0)
		{
			m_RestTime4Change	= -999.0f;
			m_fTotalMonoPower	= m_MonoPower[0] = m_MonoPower[2];
		}
		else
		{
			m_fTotalMonoPower	= m_MonoPower[0] = RpNtlWorldRealPropLerp(m_MonoPower[1], m_MonoPower[2], m_RestTime4Change, dBLOOM_SWITCHING_TIME);
		}
	}

	if (m_fTimeLightning > m_fTimeFadeOut)
	{
		m_fTimeLightning -= g_GetElapsedTime();
		if (m_fTimeLightning >= 0.0f)
		{
			m_fTotalMonoPower = (m_fTimeLightning / m_fTimeFadeIn) * m_MonoPower[0];
		}
		else if (m_fTimeLightning <= m_fTimeFadeOut)
		{
			m_fTotalMonoPower = m_MonoPower[0];
		}
		else
		{
			m_fTotalMonoPower = (m_fTimeLightning / m_fTimeFadeOut) * m_MonoPower[0];
		}
	}

	return TRUE;
}

RwBool CNtlPLBloom::Render(void)
{
	return TRUE;
}

void CNtlPLBloom::RunableLightning(RwReal fTimeFadeIn, RwReal fTimeFadeOut)
{
	m_fTimeFadeIn	= fTimeFadeIn;
	m_fTimeFadeOut	= -fTimeFadeOut;

	m_fTimeLightning = m_fTimeFadeIn * (1.0f - (static_cast<RwReal>(m_fTotalMonoPower) / m_MonoPower[0]));
}