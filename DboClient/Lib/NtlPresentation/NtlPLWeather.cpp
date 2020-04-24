#include "precomp_ntlpresentation.h"

#include "NtlDebug.h"
#include "NtlWorldCommon.h"

#include "NtlPLResourceManager.h"

#include "NtlPLWeatherProperty.h"

#include "NtlPLWeather.h"


CNtlPLWeather::CNtlPLWeather()
:m_pProperty(NULL)
,m_uiFadeState(EPLWEATHER_FADE_NONE)
,m_fFadeDeltaTime(0.0f)
,m_fFadeTime(0.0f)
,m_fEmitterDeltaTime(0.0f)
,m_pWeatherController(NULL)
{
	SetFlags(NTL_PLEFLAG_NOT_ADD_WORLD);
	SetLayer(PLENTITY_LAYER_WEATHER);
	SetClassType(PLENTITY_WEATHER);
}

CNtlPLWeather::~CNtlPLWeather()
{
}

RwBool CNtlPLWeather::Create(const SPLEntityCreateParam* pParam)
{
	NTL_FUNCTION("CNtlPLWeather::Create");

	CNtlPLWeatherProperty::EMITTER_MAP_IT itEmitter = m_pProperty->m_mapEmitter.find((RwInt32)pParam->_pUserData);
	if (itEmitter == m_pProperty->m_mapEmitter.end())
	{
		NTL_RETURN(FALSE);
	}

	// Texture가 없을 경우 Visual이 없는 것으로 판단
	// 기본 Controller를 사용한다.

	SPLWeatherEmitter& sPLWeatherEmitter = itEmitter->second;
	if (sPLWeatherEmitter.strParticleTextureName.length())
	{
		m_pWeatherController = NTL_NEW CNtlPLWeatherControllerPTank;
		m_pWeatherController->Create(m_pProperty, &sPLWeatherEmitter);
	}
	else
	{
		m_pWeatherController = NTL_NEW CNtlPLWeatherController;
		m_pWeatherController->Create(m_pProperty, &sPLWeatherEmitter);
	}

	NTL_RETURN(TRUE);
}

void CNtlPLWeather::Destroy()
{
	NTL_FUNCTION("CNtlPLWeather::Destroy");

	if (m_pWeatherController)
	{
		m_pWeatherController->Destroy();
		NTL_DELETE(m_pWeatherController);
	}

	NTL_RETURNVOID();
}

RwBool CNtlPLWeather::Update(RwReal fElapsed)
{
	NTL_FUNCTION("CNtlPLWeather::Update");

	UpdateFade(fElapsed);

	if (m_pWeatherController && IsVisible())
	{
		NTL_RETURN(m_pWeatherController->Update(fElapsed, m_fEmitterDeltaTime));
	}

	NTL_RETURN(TRUE);	
}

RwBool CNtlPLWeather::Render()
{
	NTL_FUNCTION("CNtlPLWeather::Render");
	
	if (m_pWeatherController)
	{
		NTL_RETURN(m_pWeatherController->Render());
	}

	NTL_RETURN(TRUE);	
}

RwBool CNtlPLWeather::SetProperty(const CNtlPLProperty *pProperty)
{
	NTL_FUNCTION("CNtlPLWeather::SetProperty");

	m_pProperty	= reinterpret_cast<CNtlPLWeatherProperty*>(const_cast<CNtlPLProperty*>(pProperty));

	NTL_RETURN(TRUE);	
}

RwBool CNtlPLWeather::UpdateFade(RwReal fElapsed)
{
	if ((m_uiFadeState != EPLWEATHER_FADE_OUT) && (m_uiFadeState != EPLWEATHER_FADE_IN))
	{
		return FALSE;
	}

	m_fFadeDeltaTime += fElapsed / m_fFadeTime;
	
	switch (m_uiFadeState)
	{
	case EPLWEATHER_FADE_OUT:
		m_fEmitterDeltaTime = 1.0f - m_fFadeDeltaTime;

		if (m_fEmitterDeltaTime <= 0.0f)
		{
			m_fEmitterDeltaTime	= 0;
			m_fFadeDeltaTime	= 0.0f;
			m_uiFadeState		= EPLWEATHER_FADE_OUT_DONE;
		}
		break;
	case EPLWEATHER_FADE_IN:
		m_fEmitterDeltaTime = m_fFadeDeltaTime;
		
		if (m_fEmitterDeltaTime >= 1.0f)
		{
			m_fEmitterDeltaTime	= 1.0f;
			m_fFadeDeltaTime	= 0.0f;
			m_uiFadeState		= EPLWEATHER_FADE_IN_DONE;
		}
		break;
	}

	return TRUE;
}

void CNtlPLWeather::SetFadeState(RwUInt32 uiFadeState, RwReal fFadeTime)
{
	m_uiFadeState	= uiFadeState;
	m_fFadeTime		= (fFadeTime > 0.0f ? fFadeTime : 0.01f);
}