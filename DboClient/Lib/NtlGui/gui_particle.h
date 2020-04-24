/*****************************************************************************
* File			: gui_particle.h
* Author		: HaeSung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 9. 18
* Abstract		: 
*****************************************************************************
* Desc          : 
* DBO의 파티클
* gui_particle.h .cpp
* gui_particlebox.h .cpp
* gui_particlebox_generic.h .cpp
* gui_particleparser.h .cpp
* gui_particlepage.h .cpp
*****************************************************************************/

#ifndef _GUI_PARTICLE_H_
#define	_GUI_PARTICLE_H_

START_GUI

/**
* \ingroup gui
* Objects representing particles
*/
class CParticle
{
public:
	CParticle( RwV2d& v2dPos, RwV2d& v2dDir, float fLifeTime, float fVel,
		float fRotate, float fScale,
		unsigned char uRed, unsigned char uGreen, unsigned char uBlue,
		unsigned char uAlpha,
		CSurface surface );

	virtual ~CParticle();

	void	Update( float fElapsed );
	BOOL	IsDie()						{ return m_bDie; }

	float		m_fLifeTime;		///< 생명(초)
	float		m_fAge;

	RwV2d		m_v2dPos;			///< 위치
	RwV2d		m_v2dDir;			///< 속도
	
	CSurface		m_surParticle;	///< 서페이스
	PlaneSnapShot	m_OriginSnapShot;

	int			m_nWidth;			///< 서페이스 크기
	int			m_nHeight;

	float		m_fRotate;			///< Rotate 될 수치

	float		m_fScale;			///< Scale값

protected:	
	BOOL		m_bDie;				///< 죽었나?
};

///< Type
typedef std::list<CParticle>					PARTICLELIST;
typedef	std::list<CParticle>::iterator			PARTICLELISTIT;

END_GUI

#endif//_GUI_PARTICLE_H_