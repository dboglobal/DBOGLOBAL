#include "precomp_ntlpresentation.h"
#include "NtlPLGlobal.h"
#include "NtlPLLight.h"
#include "NtlPLEvent.h"
#include "NtlWorldCommon.h"
#include "NtlWorldInterface.h"
#include "NtlPLWorldState.h"

CNtlPLLight::CNtlPLLight( void )
{
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD | NTL_PLEFLAG_NOT_PROPERTY_USED | NTL_PLEFLAG_NOT_VISIBLE);
	SetLayer(PLENTITY_LAYER_NONE);
	SetClassType(PLENTITY_LIGHT);

	m_pLtAmbient									= NULL;
	m_ClrAmbient[0].red								= 0.0f;
	m_ClrAmbient[0].green							= 0.0f;
	m_ClrAmbient[0].blue							= 0.0f;
	m_ClrAmbient[0].alpha							= 0.0f;
	m_RestTime4Change[dNTL_PL_DIRECTIONAL_LIGHT_NUM]= -999.0f;

	for (int i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
	{
		m_pLtDirectional[i]				= NULL;
		m_ClrDirectional[i][0].red		= 0.0f;
		m_ClrDirectional[i][0].green	= 0.0f;
		m_ClrDirectional[i][0].blue		= 0.0f;
		m_ClrDirectional[i][0].alpha	= 0.0f;
		m_AngDirectional[i][0].x		= 135.0f;
		m_AngDirectional[i][0].y		= 45.0f;
		m_AngDirectional[i][0].z		= 0.0f;
		m_RestTime4Change[i]			= -999.0f;
	}	
}

CNtlPLLight::~CNtlPLLight( void )
{
}

RwBool CNtlPLLight::Create(const SPLEntityCreateParam * pParam)
{
	NTL_FUNCTION("CNtlPLLight::Create");

	LinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED, 0);
	LinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED, 0);

	m_pLtAmbient = RpLightCreate(rpLIGHTAMBIENT);
	DBO_ASSERT(m_pLtAmbient, "Light create failed.");

	RpLightSetColor(m_pLtAmbient, &m_ClrAmbient[0]);
	CNtlWorldRpLightManager::GetInstance()->RequestCreateRpLight(m_pLtAmbient, this);

	for (int i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
	{
		m_pLtDirectional[i] = RpLightCreate(rpLIGHTDIRECTIONAL);
		DBO_ASSERT(m_pLtDirectional, "Light create failed.");

		RwFrame *pFrame = (RwFrame*)NULL;
		RpLightSetColor(m_pLtDirectional[i], &m_ClrDirectional[i][0]);

		pFrame = RwFrameCreate();
		DBO_ASSERT(pFrame, "Frame create failed.");
			
		RwFrameRotate(pFrame, &CNtlPLGlobal::m_vXAxisV3, m_AngDirectional[i][0].x, rwCOMBINEREPLACE);
		RwFrameRotate(pFrame, &CNtlPLGlobal::m_vYAxisV3, m_AngDirectional[i][0].y, rwCOMBINEPOSTCONCAT);
		RpLightSetFrame(m_pLtDirectional[i], pFrame);

		CNtlWorldRpLightManager::GetInstance()->RequestCreateRpLight(m_pLtDirectional[i], this);
	}
	
	NTL_RETURN(TRUE);
}


void CNtlPLLight::Destroy( void ) 
{
	NTL_FUNCTION("CNtlPLLight::Destroy");

	UnLinkMsg(NPEI_IS_ANOTHER_FIELD_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED);
	UnLinkMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED);

	if(m_pLtAmbient)
	{
		// remove deque event ( m_pLtAmbient )
		CNtlWorldRpLightManager::GetInstance()->RequestDestroyRpLight(m_pLtAmbient);
		RpLightDestroy(m_pLtAmbient);
		m_pLtAmbient = NULL;
	}

	for (int i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
	{
		if (m_pLtDirectional[i])
		{
			// remove deque event ( m_pLtDirectional[i] )
			CNtlWorldRpLightManager::GetInstance()->RequestDestroyRpLight(m_pLtDirectional[i]);
			RwFrame *frame = RpLightGetFrame(m_pLtDirectional[i]);
			RpLightSetFrame(m_pLtDirectional[i], NULL);
			RwFrameDestroy(frame);
			RpLightDestroy(m_pLtDirectional[i]);
			m_pLtDirectional[i] = NULL;
		}
	}

	NTL_RETURNVOID();
}


RwBool CNtlPLLight::SetProperty( const CNtlPLProperty * pData )
{
	NTL_FUNCTION("CNtlPLLight::SetProperty");
	NTL_RETURN(TRUE);
}

void CNtlPLLight::HandleEvents(RWS::CMsg &pMsg)
{
	if (CheckEventsWorldEffectChanged(pMsg))
	{
		sNTL_FIELD_PROP* pNtlFieldProp = reinterpret_cast<sNTL_FIELD_PROP*>(pMsg.pData);
		if(static_cast<RwInt32>(m_ClrAmbient[0].red * 255.0f)	!= static_cast<RwInt32>(pNtlFieldProp->_ClrLightAmbient.red	* 255.0f)		||
			static_cast<RwInt32>(m_ClrAmbient[0].green * 255.0f)!= static_cast<RwInt32>(pNtlFieldProp->_ClrLightAmbient.green * 255.0f)		||
			static_cast<RwInt32>(m_ClrAmbient[0].blue * 255.0f)	!= static_cast<RwInt32>(pNtlFieldProp->_ClrLightAmbient.blue * 255.0f))
		{
			m_RestTime4Change[dNTL_PL_DIRECTIONAL_LIGHT_NUM] = dLIGHT_EFFECT_SWITCHING_TIME;

			::CopyMemory(&m_ClrAmbient[1], &m_ClrAmbient[0], sizeof(RwRGBAReal));
			::CopyMemory(&m_ClrAmbient[2], &pNtlFieldProp->_ClrLightAmbient, sizeof(RwRGBAReal));

			// add deque event ( m_pLtAmbient )
			CNtlWorldRpLightManager::GetInstance()->RequestAddRpLight(m_pLtAmbient, this);
		}
		for (int i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
		{
			if (static_cast<RwInt32>(m_ClrDirectional[i][0].red * 255.0f)	!= static_cast<RwInt32>(pNtlFieldProp->_ClrLightDirectional[i].red * 255.0f)	||
				static_cast<RwInt32>(m_ClrDirectional[i][0].green * 255.0f) != static_cast<RwInt32>(pNtlFieldProp->_ClrLightDirectional[i].green * 255.0f)	||
				static_cast<RwInt32>(m_ClrDirectional[i][0].blue * 255.0f)	!= static_cast<RwInt32>(pNtlFieldProp->_ClrLightDirectional[i].blue * 255.0f)	||
				static_cast<RwInt32>(m_AngDirectional[i][0].x) != static_cast<RwInt32>(pNtlFieldProp->_AngLightDirectional[i].x) ||
				static_cast<RwInt32>(m_AngDirectional[i][0].y) != static_cast<RwInt32>(pNtlFieldProp->_AngLightDirectional[i].y) ||
				static_cast<RwInt32>(m_AngDirectional[i][0].z) != static_cast<RwInt32>(pNtlFieldProp->_AngLightDirectional[i].z))
			{
				m_RestTime4Change[i] = dLIGHT_EFFECT_SWITCHING_TIME;

				::CopyMemory(&m_ClrDirectional[i][1], &m_ClrDirectional[i][0], sizeof(RwRGBAReal));
				::CopyMemory(&m_ClrDirectional[i][2], &pNtlFieldProp->_ClrLightDirectional[i], sizeof(RwRGBAReal));

				::CopyMemory(&m_AngDirectional[i][1], &m_AngDirectional[i][0], sizeof(RwV3d));
				::CopyMemory(&m_AngDirectional[i][2], &pNtlFieldProp->_AngLightDirectional[i], sizeof(RwV3d));

				// add deque event ( m_pLtDirectional[i] )
				CNtlWorldRpLightManager::GetInstance()->RequestAddRpLight(m_pLtDirectional[i], this);
			}
			if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
			{
				CNtlWorldRpLightManager::GetInstance()->RequestRemoveRpLight(m_pLtDirectional[1]);
				CNtlWorldRpLightManager::GetInstance()->RequestRemoveRpLight(m_pLtDirectional[2]);
				break;
			}
		}
	}
}

RwBool CNtlPLLight::Update(RwReal fElapsed)
{
	UpdateAmbient(fElapsed);

	UpdateDirectional(fElapsed);

	return TRUE;
}

RwBool CNtlPLLight::UpdateAmbient(RwReal fElapsed)
{
	if(m_RestTime4Change[dNTL_PL_DIRECTIONAL_LIGHT_NUM] >= 0.0f)
	{
		m_RestTime4Change[dNTL_PL_DIRECTIONAL_LIGHT_NUM] -= g_GetElapsedTime();
		if(m_RestTime4Change[dNTL_PL_DIRECTIONAL_LIGHT_NUM] < 0.0f)
		{
			m_RestTime4Change[dNTL_PL_DIRECTIONAL_LIGHT_NUM] = -999.0f;
			::CopyMemory(&m_ClrAmbient[0], &m_ClrAmbient[2], sizeof(RwRGBAReal));
			if (m_ClrAmbient[0].red == 0.0f && m_ClrAmbient[0].green == 0.0f && m_ClrAmbient[0].blue == 0.0f)
			{
				// remove deque event number ( m_pLtAmbient )
				CNtlWorldRpLightManager::GetInstance()->RequestRemoveRpLight(m_pLtAmbient);
			}
		}
		else
		{
			m_ClrAmbient[0].red			= RpNtlWorldRealPropLerp(m_ClrAmbient[1].red,		m_ClrAmbient[2].red,		m_RestTime4Change[dNTL_PL_DIRECTIONAL_LIGHT_NUM], dLIGHT_EFFECT_SWITCHING_TIME);
			m_ClrAmbient[0].green		= RpNtlWorldRealPropLerp(m_ClrAmbient[1].green,		m_ClrAmbient[2].green,		m_RestTime4Change[dNTL_PL_DIRECTIONAL_LIGHT_NUM], dLIGHT_EFFECT_SWITCHING_TIME);
			m_ClrAmbient[0].blue		= RpNtlWorldRealPropLerp(m_ClrAmbient[1].blue,		m_ClrAmbient[2].blue,		m_RestTime4Change[dNTL_PL_DIRECTIONAL_LIGHT_NUM], dLIGHT_EFFECT_SWITCHING_TIME);
		}

		RpLightSetColor(m_pLtAmbient, &m_ClrAmbient[0]);
	}

	RwReal TFactor = -static_cast<RwReal>(dGET_WORLD_PARAM()->ClrDayAndNight & 0x000000ff) / 255.0f;
	::CopyMemory(&m_ClrAmbientBK, &m_ClrAmbient[0],	sizeof(RwRGBAReal));
	::API_PL_1BColorClamp(&m_ClrAmbientBK.red, TFactor);
	::API_PL_1BColorClamp(&m_ClrAmbientBK.green, TFactor);
	::API_PL_1BColorClamp(&m_ClrAmbientBK.blue, TFactor);
	RpLightSetColor(m_pLtAmbient, &m_ClrAmbientBK);

	return TRUE;
}

RwBool CNtlPLLight::UpdateDirectional(RwReal fElapsed)
{
	for (int i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
	{
		if(m_RestTime4Change[i] >= 0.0f)
		{
			m_RestTime4Change[i] -= g_GetElapsedTime();
			if(m_RestTime4Change[i] < 0.0f)
			{
				m_RestTime4Change[i] = -999.0f;
				::CopyMemory(&m_ClrDirectional[i][0], &m_ClrDirectional[i][2], sizeof(RwRGBAReal));
				::CopyMemory(&m_AngDirectional[i][0], &m_AngDirectional[i][2], sizeof(RwV3d));
				if (m_ClrDirectional[i][0].red == 0.0f && m_ClrDirectional[i][0].green == 0.0f && m_ClrDirectional[i][0].blue == 0.0f)
				{
					// remove deque event number ( m_pLtDirectional[i] )
					CNtlWorldRpLightManager::GetInstance()->RequestRemoveRpLight(m_pLtDirectional[i]);
					
				}
			}
			else
			{
				m_ClrDirectional[i][0].red		= RpNtlWorldRealPropLerp(m_ClrDirectional[i][1].red,		m_ClrDirectional[i][2].red,		m_RestTime4Change[i], dLIGHT_EFFECT_SWITCHING_TIME);
				m_ClrDirectional[i][0].green	= RpNtlWorldRealPropLerp(m_ClrDirectional[i][1].green,		m_ClrDirectional[i][2].green,	m_RestTime4Change[i], dLIGHT_EFFECT_SWITCHING_TIME);
				m_ClrDirectional[i][0].blue		= RpNtlWorldRealPropLerp(m_ClrDirectional[i][1].blue,		m_ClrDirectional[i][2].blue,	m_RestTime4Change[i], dLIGHT_EFFECT_SWITCHING_TIME);

				m_AngDirectional[i][0].x		= RpNtlWorldRealPropLerp(m_AngDirectional[i][1].x,		m_AngDirectional[i][2].x,		m_RestTime4Change[i], dLIGHT_EFFECT_SWITCHING_TIME);
				m_AngDirectional[i][0].y		= RpNtlWorldRealPropLerp(m_AngDirectional[i][1].y,		m_AngDirectional[i][2].y,		m_RestTime4Change[i], dLIGHT_EFFECT_SWITCHING_TIME);
				m_AngDirectional[i][0].z		= RpNtlWorldRealPropLerp(m_AngDirectional[i][1].z,		m_AngDirectional[i][2].z,		m_RestTime4Change[i], dLIGHT_EFFECT_SWITCHING_TIME);
			}

			RpLightSetColor(m_pLtDirectional[i], &m_ClrDirectional[i][0]);

			RwFrame* pFrame = RpLightGetFrame(m_pLtDirectional[i]);
			if (pFrame)
			{
				RwFrameRotate(pFrame, &CNtlPLGlobal::m_vXAxisV3, m_AngDirectional[i][0].x, rwCOMBINEREPLACE);
				RwFrameRotate(pFrame, &CNtlPLGlobal::m_vYAxisV3, m_AngDirectional[i][0].y, rwCOMBINEPOSTCONCAT);
				RwFrameUpdateObjects(pFrame);
			}
		}

		RwReal TFactor = -static_cast<RwReal>(dGET_WORLD_PARAM()->ClrDayAndNight & 0x000000ff) / 255.0f;
		::CopyMemory(&m_ClrDirectionalBK[i], &m_ClrDirectional[i][0], sizeof(RwRGBAReal));
		::API_PL_1BColorClamp(&m_ClrDirectionalBK[i].red, TFactor);
		::API_PL_1BColorClamp(&m_ClrDirectionalBK[i].green, TFactor);
		::API_PL_1BColorClamp(&m_ClrDirectionalBK[i].blue, TFactor);
		RpLightSetColor(m_pLtDirectional[i], &m_ClrDirectionalBK[i]);

		if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
		{
			break;
		}
	}

	return TRUE;
}


RwBool CNtlPLLight::Render( void )
{

	return TRUE;
}

#ifdef dNTL_WORLD_TOOL_MODE
void CNtlPLLight::SetRpLightDirectinalColor(RwInt32 iIdx, RwRGBAReal rgbaf)
{
	m_ClrDirectional[iIdx][0]	= rgbaf;
	m_ClrDirectional[iIdx][1]	= rgbaf;
	m_ClrDirectional[iIdx][2]	= rgbaf;
	m_RestTime4Change[iIdx]		= -999.0f;

	RpLightSetColor(m_pLtDirectional[iIdx], &m_ClrDirectional[iIdx][0]);

	CNtlWorldRpLightManager::GetInstance()->RequestAddRpLight(m_pLtDirectional[iIdx], this);
}

void CNtlPLLight::SetRpLightDirectinalAngle(RwInt32 iIdx, RwV3d& vAngle)
{
	m_AngDirectional[iIdx][0]	= vAngle;
	m_AngDirectional[iIdx][1]	= vAngle;
	m_AngDirectional[iIdx][2]	= vAngle;
	m_RestTime4Change[iIdx]		= -999.0f;

	RwFrame* pFrame = RpLightGetFrame(m_pLtDirectional[iIdx]);
	if(pFrame)
	{
		RwFrameRotate(pFrame, &CNtlPLGlobal::m_vXAxisV3, m_AngDirectional[iIdx][0].x, rwCOMBINEREPLACE);
		RwFrameRotate(pFrame, &CNtlPLGlobal::m_vYAxisV3, m_AngDirectional[iIdx][0].y, rwCOMBINEPOSTCONCAT);
		RwFrameUpdateObjects(pFrame);
	}

	CNtlWorldRpLightManager::GetInstance()->RequestAddRpLight(m_pLtDirectional[iIdx], this);
}
#endif

