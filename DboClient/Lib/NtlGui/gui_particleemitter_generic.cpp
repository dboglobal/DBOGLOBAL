#include "gui_precomp.h"
#include "gui_particleemitter_generic.h"
#include "gui_renderer.h"
#include "GuiUtil.h"

//START_GUI
//
//CParticleEmitter_Generic::CParticleEmitter_Generic( CParticleEmitter* pSelf )
//{
//	m_bStart = FALSE;
//	
//	m_fElapsedTime = 0.0f;
//
//	m_dwEmitMaxCount = 0xffffffff;
//	m_dwEmitCurCount = 0;
//
//	m_fEmitRate1 = 0.0f;
//	m_fEmitRate2 = 0.0f;
//
//	m_fLifeTime1 = 0.0f;
//	m_fLifeTime2 = 0.0f;
//
//	m_rtSize1.SetRectWH( 0, 0, 32, 32 );
//	m_rtSize2.SetRectWH( 0, 0, 32, 32 );
//
//	m_ucRed1 = m_ucRed2 = m_ucGreen1 = m_ucGreen2 = m_ucBlue1 = m_ucBlue2 = m_ucAlpha1 = m_ucAlpha2 = 255;
//
//	m_fZRot1 = 0.0f;
//	m_fZRot2 = 0.0f;
//
//	m_ucSurfaceIndex1 = 0;
//	m_ucSurfaceIndex2 = 0;
//
//	m_pParticleEmitter = pSelf;
//	m_pParticleEmitter->Enable( false );
//
//	m_SlotMove	= m_pParticleEmitter->SigMove().Connect( this, &CParticleEmitter_Generic::OnMove );
//	m_SlotPaint	= m_pParticleEmitter->SigPaint().Connect( this, &CParticleEmitter_Generic::OnPaint );
//	m_SlotSetOption = m_pParticleEmitter->SigSetOptions().Connect( this, &CParticleEmitter_Generic::OnSetOptions );
//}
//
//CParticleEmitter_Generic::~CParticleEmitter_Generic(VOID)
//{
//	std::list<CParticle*>::iterator it;
//
//	for( it = m_listParticle.begin() ; it != m_listParticle.end() ; ++it )
//	{	
//		delete (*it);
//	}
//
//	m_listParticle.clear();
//	m_vecSurface.clear();
//	m_vecControl.clear();
//}
//
//VOID CParticleEmitter_Generic::Start(VOID)
//{
//	m_bStart = TRUE;
//	m_fElapsedTime = 0.0f;
//	m_dwEmitCurCount = 0;
//	CreateParticle();
//}
//
//VOID CParticleEmitter_Generic::End(VOID)
//{
//	m_bStart = FALSE;
//
//	std::list<CParticle*>::iterator it;	
//
//	for( it = m_listParticle.begin() ; it != m_listParticle.end() ; ++it )
//	{	
//		delete (*it);
//	}
//
//	m_listParticle.clear();
//}
//
//VOID CParticleEmitter_Generic::Update( FLOAT fElapsed )
//{
//	if( m_bStart )
//	{
//		std::list<CParticle*>::iterator it;
//
//		for( it = m_listParticle.begin() ; it != m_listParticle.end() ;  )
//		{
//			CParticle* pParticle = (*it);
//
//			if( !pParticle->Update( fElapsed ) )
//			{
//				delete pParticle;
//				it = m_listParticle.erase( it );
//				continue;
//			}
//	
//			++it;
//		}
//
//		m_fElapsedTime += fElapsed;
//
//		if( m_fElapsedTime > RandomNumber( m_fEmitRate1, m_fEmitRate2 ) )
//		{
//			CreateParticle();
//			m_fElapsedTime = 0.0f;
//		}
//	}
//}
//
//VOID CParticleEmitter_Generic::CreateParticle(VOID)
//{
//	if( ++m_dwEmitCurCount > m_dwEmitMaxCount )
//		return;
//
//	CParticle*	pParticle = new CParticle( m_pParticleEmitter, RandomNumber( m_fLifeTime1, m_fLifeTime2 ) );
//	CSurface	surface = m_vecSurface[RandomNumber( (INT)m_ucSurfaceIndex1, (INT)m_ucSurfaceIndex2 ) ];
//	
//	CRectangle	rtParticle;
//	INT			nWidth = RandomNumber( m_rtSize1.GetWidth(), m_rtSize2.GetWidth() );
//	INT			nHeight= RandomNumber( m_rtSize1.GetHeight(), m_rtSize2.GetHeight() );
//
//	rtParticle.left		= RandomNumber( m_rtEmitterPos.left, m_rtEmitterPos.right ) - nWidth / 2;
//	rtParticle.top		= RandomNumber( m_rtEmitterPos.top, m_rtEmitterPos.bottom ) - nHeight / 2;
//	rtParticle.right	= rtParticle.left + nWidth;
//	rtParticle.bottom	= rtParticle.top + nHeight;
//
//	pParticle->SetPosition( rtParticle );
//
//	surface.m_SnapShot.rtRect	= rtParticle;
//	surface.m_SnapShot.uRed		= (BYTE)RandomNumber( (INT)m_ucRed1, (INT)m_ucRed2 );
//	surface.m_SnapShot.uGreen	= (BYTE)RandomNumber( (INT)m_ucGreen1, (INT)m_ucGreen2 );
//	surface.m_SnapShot.uBlue	= (BYTE)RandomNumber( (INT)m_ucBlue1, (INT)m_ucBlue2 );
//	surface.m_SnapShot.uAlpha	= (BYTE)RandomNumber( (INT)m_ucAlpha1, (INT)m_ucAlpha2 );
//	surface.m_fAngle			= RandomNumber( m_fZRot1, m_fZRot2 );
//
//	pParticle->SetSurface( surface );
//	m_listParticle.push_back( pParticle );
//	
//	std::vector<stControlData>::iterator it;
//	
//	for( it = m_vecControl.begin() ; it!= m_vecControl.end() ; ++it )
//	{
//		pParticle->AddControl( CParticleControlFactory::CreateControlFactory( (*it).nID, &(*it).stAttr ) );	
//	}
//}
//
//VOID CParticleEmitter_Generic::OnSetOptions( const CComponentOptions& options )
//{
//	// option에서 control을 스크립트로 빼낼 수 있도록.
//}
//
//VOID CParticleEmitter_Generic::OnPaint(VOID)
//{
//	if( m_bStart )
//	{
//		std::list<CParticle*>::iterator it;
//
//		for( it = m_listParticle.begin() ; it != m_listParticle.end() ; ++it )
//		{
//			CParticle* pParticle = (*it);
//			g_GuiRenderer.RenderQueue( &pParticle->m_Surface.m_SnapShot, pParticle->m_Surface.m_pTexture );
//		}
//	}
//}
//
//VOID CParticleEmitter_Generic::OnMove( INT nX,INT nY )
//{
//	CRectangle rtScreen = m_pParticleEmitter->GetScreenRect();
//		
//	std::list<CParticle*>::iterator it;
//
//	for( it = m_listParticle.begin() ; it != m_listParticle.end() ; ++it )
//	{
//		CParticle* pParticle = (*it);
//
//		pParticle->m_Surface.m_SnapShot.rtRect.left		= (INT)( pParticle->m_fLeft + rtScreen.left );
//		pParticle->m_Surface.m_SnapShot.rtRect.top		= (INT)( pParticle->m_fTop + rtScreen.top );
//		pParticle->m_Surface.m_SnapShot.rtRect.right	= (INT)( pParticle->m_fRight + rtScreen.left );
//		pParticle->m_Surface.m_SnapShot.rtRect.bottom	= (INT)( pParticle->m_fBottom + rtScreen.top );
//	}
//}
//
//
//END_GUI