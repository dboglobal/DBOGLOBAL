#include "gui_precomp.h"
#include "gui_particlesystem.h"

#include "gui_renderer.h"

#include "gui_particlebox.h"
#include "gui_particleemitter.h"
#include "gui_particleaffector.h"
#include "gui_particlepath.h"

#include "GuiUtil.h"

START_GUI

/**
* \brief  파티클 시스템의 생성자
* \param pRect			(CRectangle*) 파티클 시스템의 위치정보
* \param pSurfaceMgr	(CSurfaceManager*) 파티클 시스템에서 사용될 서페이스 매니저
*/
CParticleSystem::CParticleSystem( CRectangle* pRect, CSurfaceManager* pSurfaceMgr )
{
	// 초기화
	m_nParticleLimit = 100;

	m_pPath = NULL;
	m_pEmitter = NULL;
	m_pGravity = NULL;
	m_pColor = NULL;

	m_fPlayTime = 0.0f;
	m_fLifeTime = 5.0f;

	m_bPlay = FALSE;
	m_bRepeat = FALSE;
	m_bFirstUpdate = TRUE;

	m_nPosX = pRect->left;
	m_nPosY = pRect->top;

	m_pSurfaceMgr = pSurfaceMgr;
}

CParticleSystem::~CParticleSystem()
{
	NTL_DELETE( m_pPath );
	NTL_DELETE( m_pEmitter );
	NTL_DELETE( m_pGravity );
	NTL_DELETE( m_pColor );

	m_pSurfaceMgr = NULL;
}

void CParticleSystem::Play( BOOL bRepeat )
{
	m_bPlay = TRUE;
	m_bRepeat = bRepeat;
	m_fPlayTime = 0.0f;
	m_bFirstUpdate = TRUE;
}

void CParticleSystem::Stop()
{
	m_bPlay = FALSE;
	m_bRepeat = FALSE;
	m_fPlayTime = 0.0f;
	m_bFirstUpdate = FALSE;
}

void CParticleSystem::Update( float fElapsed )
{
	// 파티클과 어펙터는 Play 중이 아니더라도 업데이트 한다.
	// Particle Update
	if( !m_listParticle.empty() )
	{
		PARTICLELISTIT itParticle = m_listParticle.begin();
		while( itParticle != m_listParticle.end() )
		{
			(*itParticle).Update( fElapsed );

			if( m_pGravity )
			{
				m_pGravity->Update( fElapsed, &(*itParticle) );
			}

			if( m_pColor )
			{
				m_pColor->Update( fElapsed, &(*itParticle) );
			}
	
			if( (*itParticle).IsDie() )
			{
				itParticle = m_listParticle.erase( itParticle );
				continue;
			}
			else
			{
				itParticle++;
				continue;
			}
		}
	}

	if( !m_bPlay )
		return;
	
	m_fPlayTime += fElapsed;

	// 첫번째 업데이트는 포함시키지 않는다.
	if( m_bFirstUpdate )
	{
		m_bFirstUpdate = FALSE;
		m_fPlayTime = 0.0f;
	}

	// 반복 모드라면 계속해서 되풀이
	if( m_fPlayTime > m_fLifeTime )
	{
		if( !m_bRepeat )
		{
			Stop();
			
			return;
		}
	}


	if( m_pPath )
	{
		RwV2d v2dPathPos;
		m_pPath->FrameMove( m_fPlayTime, &v2dPathPos, m_bRepeat );

		if( m_pEmitter )
			m_pEmitter->SetPosition( m_nPosX + (int)v2dPathPos.x, m_nPosY + (int)v2dPathPos.y );
	}

	if( (int)m_listParticle.size() < m_nParticleLimit )
	{
		if( m_pEmitter )
			m_pEmitter->Update( fElapsed );
		// TODO : 이미터에 따라서 업데이트
	}
}

void CParticleSystem::Render()
{
	if( !m_listParticle.empty() )
	{
		// Todo : 개선사항은 한번에 렌더링 하는 코드로 고쳐야 한다는 것
		PARTICLELISTIT itParticle = m_listParticle.begin();
		while( itParticle != m_listParticle.end() )
		{
			// Render
			g_GuiRenderer.RenderQueue( &(*itParticle).m_surParticle.m_SnapShot, 
				(*itParticle).m_surParticle.m_pTexture, false, (*itParticle).m_surParticle.m_fAngle );

			itParticle++;
		}
	}
}

BOOL CParticleSystem::IsRepeat()
{
	return m_bRepeat;
}

BOOL CParticleSystem::IsPlay()
{
	return m_bPlay;
}

std::string CParticleSystem::GetName()
{
	return m_strParticleName;
}

void CParticleSystem::SetLife( float fLife )
{
	m_fLifeTime = fLife;
}

void CParticleSystem::SetLimit( int nLimit )
{
	m_nParticleLimit = nLimit;
}

VOID CParticleSystem::AddPointEmitter( int nPosX, int nPosY, float fLifeMin, float fLifeMax,
									  float fVelMin, float fVelMax, float fEmissivity,
									  RwV2d& v2dDir, float fRadiationAngle, float fRotateAngleMin,
									  float fRotateAngleMax, float fScaleMin, float fScaleMax,
									  unsigned char uRedMin, unsigned char uRedMax,
									  unsigned char uGreenMin, unsigned char uGreenMax,
									  unsigned char uBlueMin, unsigned char uBlueMax,
									  unsigned char uAlphaMin, unsigned char uAlphaMax,
									  const char* surpage, const char* surface )
{
	NTL_DELETE( m_pEmitter );

	m_pEmitter = NTL_NEW CPointEmitter( &m_listParticle, m_pSurfaceMgr );

	m_pEmitter->SetPosition( nPosX, nPosY );
	m_pEmitter->SetLifeTime( fLifeMin, fLifeMax );
	m_pEmitter->SetVelocity( fVelMin, fVelMax );
	m_pEmitter->SetEmissivity( fEmissivity );
	m_pEmitter->SetDirection( v2dDir, fRadiationAngle );
	m_pEmitter->SetRotate( fRotateAngleMin, fRotateAngleMax );
	m_pEmitter->SetScale( fScaleMin, fScaleMax );
	m_pEmitter->SetColorRed( uRedMin, uRedMax );
	m_pEmitter->SetColorGreen( uGreenMin, uGreenMax );
	m_pEmitter->SetColorBlue( uBlueMin, uBlueMax );
	m_pEmitter->SetAlpha( uAlphaMin, uAlphaMax );
	m_pEmitter->SetSurface( surpage, surface );
}

VOID CParticleSystem::AddBoxEmitter( int nPosX, int nPosY, int nWidth, int nHeight,
									float fLifeMin, float fLifeMax, float fVelMin, float fVelMax,
									float fEmissivity, RwV2d& v2dDir, float fRadiationAngle,
									float fRotateAngleMin, float fRotateAngleMax, float fScaleMin, float fScaleMax,
									unsigned char uRedMin, unsigned char uRedMax,
									unsigned char uGreenMin, unsigned char uGreenMax,
									unsigned char uBlueMin, unsigned char uBlueMax,
									unsigned char uAlphaMin, unsigned char uAlphaMax,
									const char* surpage, const char* surface )
{
	NTL_DELETE( m_pEmitter );

	m_pEmitter = NTL_NEW CBoxEmitter( &m_listParticle, m_pSurfaceMgr );

	// Box Emitter 만의 속성
	CBoxEmitter* pEmitter = (CBoxEmitter*)m_pEmitter;
	pEmitter->SetWidthHeight( nWidth, nHeight );
	
	m_pEmitter->SetPosition( nPosX, nPosY );
	m_pEmitter->SetLifeTime( fLifeMin, fLifeMax );
	m_pEmitter->SetVelocity( fVelMin, fVelMax );
	m_pEmitter->SetEmissivity( fEmissivity );
	m_pEmitter->SetDirection( v2dDir, fRadiationAngle );
	m_pEmitter->SetRotate( fRotateAngleMin, fRotateAngleMax );
	m_pEmitter->SetScale( fScaleMin, fScaleMax );
	m_pEmitter->SetColorRed( uRedMin, uRedMax );
	m_pEmitter->SetColorGreen( uGreenMin, uGreenMax );
	m_pEmitter->SetColorBlue( uBlueMin, uBlueMax );
	m_pEmitter->SetAlpha( uAlphaMin, uAlphaMax );
	m_pEmitter->SetSurface( surpage, surface );
}

VOID CParticleSystem::AddGravityAffector( RwV2d& v2dGravityDir, float fWeight )
{
	NTL_DELETE( m_pGravity );

	m_pGravity = NTL_NEW CGravityAffector( &m_listParticle );

	m_pGravity->SetGravity( v2dGravityDir, fWeight );
}

VOID CParticleSystem::AddColorAffector( unsigned char uRed, unsigned char uGreen, unsigned char uBlue, unsigned char uAlpha, BOOL bRed /*= TRUE*/, BOOL bGreen /*= TRUE*/, BOOL bBlue /*= TRUE*/, BOOL bAlpha /*= TRUE */ )
{
	NTL_DELETE( m_pColor );

	m_pColor = NTL_NEW CColorAffector( &m_listParticle );

	m_pColor->SetColor( uRed, uGreen, uBlue, uAlpha,
		bRed, bGreen, bBlue, bAlpha );
}

VOID CParticleSystem::AddPath( SPathInfo& sPathInfo, SCtrlPoint* pBuffer, int nBufferSize )
{
	NTL_DELETE( m_pPath );

	m_pPath = NTL_NEW CParticlePath();

	m_pPath->SetPath( sPathInfo, pBuffer );
}

VOID CParticleSystem::SetPosition( int nNewX, int nNewY )
{
	m_nPosX = nNewX;
	m_nPosY = nNewY;

	if( m_pEmitter )
		m_pEmitter->SetPosition( nNewX, nNewY );

	
}


END_GUI