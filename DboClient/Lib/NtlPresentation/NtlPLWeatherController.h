#pragma once

#include "NtlPLWeatherProperty.h"

class CNtlPLWeatherController
{
protected:
	RwUInt32					m_eWeatherUpdateFlag;

	SPLWeatherEmitter*			m_pWeatherEmitter;
	RwV3d						m_vWeatherEmitter;	
	RwReal						m_fCreateTime;

	PARTICLE_LIST				m_listParticle;

	SOUND_HANDLE				m_hGlobalSound;
	RwInt32						m_iParticleSoundNum;
	RwReal						m_fParticleSoundTime;

	RwReal						m_fCollisionLineScale;

	typedef RwBool (CNtlPLWeatherController::*CB_WEATHER_COLLISION)(SPLWeatherParticle* pParticle);

	CB_WEATHER_COLLISION		m_CBParticleCollisionTime;
	CB_WEATHER_COLLISION		m_CBParticleCollisionXRay;

	RwV3d						m_vErrorPos;
	RwV3d						m_vEmitterLerp;

	
public:
	CNtlPLWeatherController();
	virtual ~CNtlPLWeatherController();

	virtual RwBool				Create(CNtlPLWeatherProperty* pWeatherProperty, SPLWeatherEmitter* pWeatherEmitter);
	virtual void				Destroy();								
	virtual RwBool				Update(RwReal fElapsed, RwReal fFade = 0.0f);
	virtual RwBool				Render()			{ return TRUE; }
	virtual RwInt32				GetCountParticle()	{ return m_listParticle.size(); }
	virtual RwInt32				GetCountRender()	{ return 0; }

	virtual SPLWeatherEmitter*	GetWeatherEmitter()	{ return m_pWeatherEmitter; }

protected:
	virtual RwBool				UpdateEmitterPos();

	virtual RwBool				UpdateParticleCreate(RwReal fElapsed, RwReal fFade);
	virtual RwBool				UpdateParticleDelete(SPLWeatherParticle* pParticle, PARTICLE_LIST_IT& itParticleList);
	virtual RwBool				UpdateParticleTime(SPLWeatherParticle* pParticle, RwReal fElapsed);
	virtual RwBool				UpdateParticleFrustom(SPLWeatherParticle* pParticle, RwReal fElapsed);
	virtual RwBool				UpdateParticlePos(SPLWeatherParticle* pParticle, RwReal fElapsed);
	virtual RwBool				UpdateParticleMatrix(SPLWeatherParticle* pParticle, RwReal fElapsed);
	virtual RwBool				UpdateParticleColor(SPLWeatherParticle* pParticle, RwReal fElapsed);
	
	virtual SPLWeatherParticle*	CreateParticle(RwInt32 iIndex, RwInt32 iNum);
	virtual SPLWeatherParticle*	DeleteParticle(SPLWeatherParticle* pParticle);

	virtual RwBool				SendParticleEvent(SPLWeatherParticle* pParticle, RwUInt32 eEvent);
	
	virtual RwBool				CalcParticleCollisionTimeOutdoor(SPLWeatherParticle* pParticle);
	virtual RwBool				CalcParticleCollisionXRayOutdoor(SPLWeatherParticle* pParticle);

	virtual RwBool				CalcParticleCollisionTimeIndoor(SPLWeatherParticle* pParticle);
	virtual RwBool				CalcParticleCollisionXRayIndoor(SPLWeatherParticle* pParticle);
};

//////////////////////////////////////////////////////////////////////////
// CNtlPLWeatherControllerPTank :: Rw PTank System Use
//////////////////////////////////////////////////////////////////////////
class CNtlPLWeatherControllerPTank : public CNtlPLWeatherController
{
public:
	CNtlPLWeatherControllerPTank();
	virtual ~CNtlPLWeatherControllerPTank();	

	virtual RwBool				Create(CNtlPLWeatherProperty* pWeatherProperty, SPLWeatherEmitter* pWeatherEmitter);
	virtual void				Destroy();
	virtual RwBool				Render();
	virtual RwBool				Update(RwReal fElapsed, RwReal fFade = 0.0f);

	virtual RwInt32				GetCountRender()	{ return RpPTankAtomicGetActiveParticlesCount(m_pAtmPTank); }

protected:
	RpAtomic*					m_pAtmPTank;
	RwFrame*					m_pFrmPTank;
	RwTexture*					m_pTexPTank;
};