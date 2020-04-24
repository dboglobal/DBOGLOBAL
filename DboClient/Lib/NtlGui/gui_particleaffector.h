/*****************************************************************************
* File			: gui_particleaffector.h
* Author		: HaeSung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 9. 19
* Abstract		: 
*****************************************************************************
* Desc          : DBO 파티클의 영향을 끼치는 클래스
*****************************************************************************/

#ifndef _GUI_PARTICLE_AFFECTOR_H_
#define _GUI_PARTICLE_AFFECTOR_H_

#include "gui_particle.h"

START_GUI

class CSurfaceManager;

/**
* \brief Particle의 Fector
*/
class CParticleAffector
{
public:
	CParticleAffector(PARTICLELIST* pParList);
	virtual ~CParticleAffector();

	virtual void		Update(float fElapsed);
	virtual void		Update(float fElapsed, CParticle* pParticle);

	int					m_nAffectorID;

protected:
	// Emitter의 속성
	PARTICLELIST*		m_pParticleList;
	CSurfaceManager*	m_pSurfaceManager;
};

class CGravityAffector : public CParticleAffector
{
public:
	CGravityAffector(PARTICLELIST* pParList);
	virtual ~CGravityAffector();

	virtual void SetGravity( RwV2d v2dGravityDir, float fWeight);

	virtual void Update(float fElapsed);
	virtual void Update(float fElapsed, CParticle* pParticle);

	RwV2d		m_v2dGravityDir;
};

class CColorAffector : public CParticleAffector
{
public:
	CColorAffector(PARTICLELIST* pParList);
	virtual ~CColorAffector();

	virtual void SetColor( unsigned char uRed, unsigned char uGreen, unsigned char uBlue, unsigned char uAlpha,
		BOOL bRed = TRUE, BOOL bGreen = TRUE, BOOL bBlue = TRUE, BOOL bAlpha = TRUE);

	virtual void Update(float fElapsed);
	virtual void Update(float fElapsed, CParticle* pParticle);

	unsigned char	m_uRed;
	unsigned char	m_uGreen;
	unsigned char	m_uBlue;
	unsigned char	m_uAlpha;

	BOOL			m_bRed;
	BOOL			m_bGreen;
	BOOL			m_bBlue;
	BOOL			m_bAlpha;
};

END_GUI

#endif//_GUI_PARTICLE_AFFECTOR_H_