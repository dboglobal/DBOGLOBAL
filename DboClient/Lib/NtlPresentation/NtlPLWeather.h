#pragma once

#include "NtlPLEntity.h"
#include "NtlPLWeatherDefine.h"
#include "NtlPLWeatherProperty.h"
#include "NtlPLWeatherController.h"

class CNtlPLWeather : public CNtlPLEntity
{
public:
	CNtlPLWeather();
	virtual ~CNtlPLWeather();

public:
	virtual RwBool				Create(const SPLEntityCreateParam* pParam = NULL);
	virtual void				Destroy();

	virtual RwBool				Update(RwReal fElapsed);
	virtual RwBool				Render();

	virtual RwBool				SetProperty(const CNtlPLProperty *pProperty);

	virtual RwInt32				GetCountParticle()									{ if (m_pWeatherController) { return m_pWeatherController->GetCountParticle(); } return 0; }
	virtual RwInt32				GetCountRender()									{ if (m_pWeatherController) { return m_pWeatherController->GetCountRender(); } return 0; }
	
	virtual void				SetFadeState(RwUInt32 uiFadeState, RwReal fFadeTime);
	virtual RwInt32				GetFadeState()										{ return m_uiFadeState; }
	
	virtual RwBool				UpdateFade(RwReal fElapsed);

	virtual SPLWeatherEmitter*	GetWeatherEmitter()									{ return m_pWeatherController->GetWeatherEmitter(); }
	
protected:
	CNtlPLWeatherProperty*					m_pProperty;

	// FadeSystem
	RwUInt32								m_uiFadeState;
	RwReal									m_fFadeTime;
	RwReal									m_fFadeDeltaTime;

	// Property
	RwReal									m_fEmitterDeltaTime;

	CNtlPLWeatherController*				m_pWeatherController;
};