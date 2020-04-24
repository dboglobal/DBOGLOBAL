#include "gui_precomp.h"
#include "GuiUtil.h"
#include "gui_particleaffector.h"
#include "gui_surfacemanager.h"
#include "NtlCoreUtil.h"

START_GUI

///////////////////////////////////////////////////////////////////////////////////////////////////
// Base Class : CParticleAffector
///////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief Construction
*/
CParticleAffector::CParticleAffector(PARTICLELIST* pParList )
{
	m_nAffectorID = 0;
	m_pParticleList = pParList;
}

/**
* \brief Destruction
*/
CParticleAffector::~CParticleAffector()
{
}

/**
* \brief Affector를 업데이트 한다. ( 파티클을 생성하거나 Affector의 행동을 결정한다. )
* \param fElapsed		(float) 이전 프레임에서의 경과시간
*/
void CParticleAffector::Update( float fElapsed )
{
	
}

void CParticleAffector::Update( float fElapsed, CParticle* pParticle )
{

}
///////////////////////////////////////////////////////////////////////////////////////////////////
// Class : CGravityAffector
// 중력의 펙터
///////////////////////////////////////////////////////////////////////////////////////////////////
CGravityAffector::CGravityAffector( PARTICLELIST* pParList)
: CParticleAffector( pParList )
{
	m_v2dGravityDir.x = 0;
	m_v2dGravityDir.y = 1;
	RwV2dScale(&m_v2dGravityDir, &m_v2dGravityDir, 100.f );
}

CGravityAffector::~CGravityAffector()
{

}

void CGravityAffector::SetGravity( RwV2d v2dGravityDir, float fWeight )
{
	RwV2dNormalize(&m_v2dGravityDir, &v2dGravityDir);
	RwV2dScale(&m_v2dGravityDir, &m_v2dGravityDir, fWeight );
}

void CGravityAffector::Update( float fElapsed )
{
	// Particle Update
	if( !m_pParticleList->empty() )
	{
		PARTICLELISTIT it = m_pParticleList->begin();
		while( it != m_pParticleList->end() )
		{
			RwV2d v2dDir;
			RwV2dScale( &v2dDir, &m_v2dGravityDir, fElapsed );
			RwV2dAdd( &(*it).m_v2dDir, &(*it).m_v2dDir, &v2dDir );

			it++;
		}
	}
}

void CGravityAffector::Update( float fElapsed, CParticle* pParticle )
{
	RwV2d v2dDir;
	RwV2dScale( &v2dDir, &m_v2dGravityDir, fElapsed );
	RwV2dAdd( &(pParticle->m_v2dDir), &(pParticle->m_v2dDir), &v2dDir );
}
///////////////////////////////////////////////////////////////////////////////////////////////////
// Class : CColorAffector
// 색상변화 affector
///////////////////////////////////////////////////////////////////////////////////////////////////

CColorAffector::CColorAffector( PARTICLELIST* pParList )
: CParticleAffector( pParList )
{
	m_uRed = 255;
	m_uGreen = 255;
	m_uBlue = 255;
	m_uAlpha = 255;

	m_bRed = FALSE;
	m_bGreen = FALSE;
	m_bBlue = FALSE;
	m_bAlpha = FALSE;
}

CColorAffector::~CColorAffector()
{

}

void CColorAffector::SetColor( unsigned char uRed, unsigned char uGreen, unsigned char uBlue, unsigned char uAlpha, BOOL bRed /*= TRUE*/, BOOL bGreen /*= TRUE*/, BOOL bBlue /*= TRUE*/, BOOL bAlpha /*= TRUE*/ )
{
	m_uRed = uRed;
	m_uGreen = uGreen;
	m_uBlue = uBlue;
	m_uAlpha = uAlpha;

	m_bRed = bRed;
	m_bGreen = bGreen;
	m_bBlue = bBlue;
	m_bAlpha = bAlpha;
}

void CColorAffector::Update( float fElapsed )
{
	// Particle Update
	if( !m_pParticleList->empty() )
	{
		PARTICLELISTIT it = m_pParticleList->begin();

		PlaneSnapShot*	pSnap;
		PlaneSnapShot*	pOriginSnap;
		while( it != m_pParticleList->end() )
		{
			pSnap = &(*it).m_surParticle.m_SnapShot;
			pOriginSnap = &(*it).m_OriginSnapShot;

			if( m_bRed )
				pSnap->uRed = (unsigned char)GetResultLineInterpolation( (*it).m_fAge, (*it).m_fLifeTime, pOriginSnap->uRed, m_uRed );

			if( m_bGreen )
				pSnap->uGreen = (unsigned char)GetResultLineInterpolation( (*it).m_fAge, (*it).m_fLifeTime, pOriginSnap->uGreen, m_uGreen );

			if( m_bBlue )
				pSnap->uBlue = (unsigned char)GetResultLineInterpolation( (*it).m_fAge, (*it).m_fLifeTime, pOriginSnap->uBlue, m_uBlue );

			if( m_bAlpha )
				pSnap->uAlpha = (unsigned char)GetResultLineInterpolation( (*it).m_fAge, (*it).m_fLifeTime, pOriginSnap->uAlpha, m_uAlpha );

			it++;
		}
	}
}

void CColorAffector::Update( float fElapsed, CParticle* pParticle )
{
	PlaneSnapShot& pSnap = pParticle->m_surParticle.m_SnapShot;
	PlaneSnapShot& pOriginSnap = pParticle->m_OriginSnapShot;

	if( m_bRed )
		pSnap.uRed = (unsigned char)GetResultLineInterpolation( pParticle->m_fAge, pParticle->m_fLifeTime, pOriginSnap.uRed, m_uRed );

	if( m_bGreen )
		pSnap.uGreen = (unsigned char)GetResultLineInterpolation( pParticle->m_fAge, pParticle->m_fLifeTime, pOriginSnap.uGreen, m_uGreen );

	if( m_bBlue )
		pSnap.uBlue = (unsigned char)GetResultLineInterpolation( pParticle->m_fAge, pParticle->m_fLifeTime, pOriginSnap.uBlue, m_uBlue );

	if( m_bAlpha )
		pSnap.uAlpha = (unsigned char)GetResultLineInterpolation( pParticle->m_fAge, pParticle->m_fLifeTime, pOriginSnap.uAlpha, m_uAlpha );
}
END_GUI