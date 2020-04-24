/*****************************************************************************
* File			: gui_particlesystem.h
* Author		: HaeSung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 10. 10
* Abstract		: 
*****************************************************************************
* Desc          : 하나의 파티클 시스템
*****************************************************************************/

#ifndef _GUI_PARTICLE_SYSTEM_H_
#define _GUI_PARTICLE_SYSTEM_H_

START_GUI

#include "gui_particle.h"
#include "gui_surface.h"
#include "gui_particle_doc.h"

class CParticleDoc;
class CParticleEmitter;
class CGravityAffector;
class CColorAffector;
class CParticlePath;
struct SCtrlPoint;
struct SPathInfo;

/**
* \brief 파티클 시스템
*/
class CParticleSystem
{
public:
	CParticleSystem( CRectangle* pRect, CSurfaceManager* pSurfaceMgr );
	virtual ~CParticleSystem();

	void	Play( BOOL bRepeat );
	void	Stop();

	void	Update( float fElapsed );
	void	Render();

	BOOL	IsRepeat();
	BOOL	IsPlay();

	std::string	GetName();

	void	SetLife( float fLife );
	void	SetLimit( int nLimit );

	VOID	AddPointEmitter( int nPosX, int nPosY, 
		float fLifeMin, float fLifeMax, 
		float fVelMin, float fVelMax, 
		float fEmissivity, 
		RwV2d& v2dDir, float fRadiationAngle,
		float fRotateAngleMin, float fRotateAngleMax,
		float fScaleMin, float fScaleMax,
		unsigned char uRedMin, unsigned char uRedMax,
		unsigned char uGreenMin, unsigned char uGreenMax,
		unsigned char uBlueMin, unsigned char uBlueMax,
		unsigned char uAlphaMin, unsigned char uAlphaMax,
		const char* surpage, const char* surface );

	VOID	AddBoxEmitter( int nPosX, int nPosY, 
		int nWidth, int nHeight,
		float fLifeMin, float fLifeMax, 
		float fVelMin, float fVelMax, 
		float fEmissivity, 
		RwV2d& v2dDir, float fRadiationAngle,
		float fRotateAngleMin, float fRotateAngleMax,
		float fScaleMin, float fScaleMax,
		unsigned char uRedMin, unsigned char uRedMax,
		unsigned char uGreenMin, unsigned char uGreenMax,
		unsigned char uBlueMin, unsigned char uBlueMax,
		unsigned char uAlphaMin, unsigned char uAlphaMax,
		const char* surpage, const char* surface );

	VOID	AddGravityAffector( RwV2d& v2dGravityDir, float fWeight );
	VOID	AddColorAffector( unsigned char uRed, unsigned char uGreen, unsigned char uBlue, unsigned char uAlpha,
		BOOL bRed = TRUE, BOOL bGreen = TRUE, BOOL bBlue = TRUE, BOOL bAlpha = TRUE );
	VOID	AddPath( SPathInfo& sPathInfo, SCtrlPoint* pBuffer, int nBufferSize );

	// Interface
public:
	VOID	SetPosition( int nNewX, int nNewY );
	
protected:
	PARTICLELIST		m_listParticle;
	int					m_nParticleLimit;
	
	CParticlePath*		m_pPath;
	CParticleEmitter*	m_pEmitter;
	CGravityAffector*	m_pGravity;
	CColorAffector*		m_pColor;

	float				m_fPlayTime;
	float				m_fLifeTime;

	BOOL				m_bPlay;
	BOOL				m_bRepeat;
	BOOL				m_bFirstUpdate;

	int					m_nPosX;
	int					m_nPosY;

	std::string			m_strParticleName;

	CSurfaceManager*	m_pSurfaceMgr;
};

END_GUI

#endif//
