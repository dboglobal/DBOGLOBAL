#include "precomp_ntlpresentation.h"
#include "NtlPLChatBalloonManager.h"

// Core
#include "NtlDebug.h"

// Presentation
#include "NtlPLGuiManager.h"
#include "NtlPLApi.h"
#include "NtlPLEvent.h"
#include "NtlPLGlobal.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNtlPLBalloonGui

VOID CNtlPLBalloonGui::SetState( RwInt32 eState )
{
	m_fCurrentTime = 0.0f;
	m_eState = eState;
}

RwInt32 CNtlPLBalloonGui::PriorityCheck( RwUInt32 eType )
{
	// Check Layer
	RwInt32 nResLayer = ( eType >> 12 ) - ( GetType() >> 12 );

	if( nResLayer == 0 )	
	{
		// Check Priority
		RwInt32 nResPriority = eType - GetType();

		if( nResPriority == 0 )
			return EQUAL_PRIORITY;
		else if( nResPriority > 0 )
			return MORE_PRIORITY;
		else 
			return LESS_PRIORITY;
	}
	else if( nResLayer > 0 )
		return UPPER_LAYER;
	else
		return LOWER_LAYER;	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CSpectatorBalloonGui

CSpectatorBalloonGui::CSpectatorBalloonGui( RwV3d& vPos, const WCHAR* pMessage, RwReal fLifeTime, RwReal fFadeInTime, RwInt32 eType, RwUInt8 byBalloonType )
: CNtlPLBalloonGui( fLifeTime, eType ), m_byBalloonType( byBalloonType ), m_wstrString( pMessage ), m_fFadeInTime( fFadeInTime )
{
	GetNtlPLBalloonManager()->GetTestText()->SetText( pMessage );
	RwInt32 nLineCount = GetNtlPLBalloonManager()->GetTestText()->GetLineCount();
	RwInt32 nWidth = GetNtlPLBalloonManager()->GetTestText()->GetFitWidthToString();

	std::wstring strMessage = GetNtlPLBalloonManager()->GetTestText()->GetText();
	gui::RemoveCStyleLineFeed( strMessage );
	gui::ChangeLineFeedToCStyle( strMessage );

	// Resource Name
	RwChar buf[256];

	if( nWidth > 72 || nLineCount > 2 )
	{
		sprintf_s( buf, 256, "textBalloon_0%d_R.swf", byBalloonType + 1 );
		m_bStandardSize = TRUE;
	}
	else
	{
		sprintf_s( buf, 256, "textBalloon_0%d_R_s.swf", byBalloonType + 1 );
		m_bStandardSize = FALSE;
	}

	m_pFlash = NTL_NEW gui::CFlash( CRectangle(), NULL, GetNtlGuiManager()->GetSurfaceManager(), buf, TRUE, TRUE );
	
	if( m_bStandardSize )
	{
		if( nLineCount > 4 )
			nLineCount = 4;		
	}
	else
	{
		if( nLineCount > 2 )
			nLineCount = 2;
	}	

	sprintf_s( buf, 256, "Text_Box0%d.text", nLineCount );
	m_pFlash->SetVariable( buf, strMessage.c_str() );
	m_pFlash->PlayMovie( TRUE );

	m_posOffset.SetPos( 22, -30 );

	m_vPos = vPos;
}

CSpectatorBalloonGui::~CSpectatorBalloonGui(VOID)
{
	NTL_DELETE( m_pFlash );	
}

VOID CSpectatorBalloonGui::FadeoutMessage(VOID)
{
	SetState( FADE_OUT );
}

RwInt32 CSpectatorBalloonGui::Update( RwReal fElapsedTime )
{
	m_fCurrentTime += fElapsedTime;

	FadeEffect();

	if( m_eState == HIDE )
		return CNtlPLBalloonManager::UR_DELETE;

	if( !PositionUpdate() )
		return CNtlPLBalloonManager::UR_NOTSHOW;

	m_pFlash->Update( fElapsedTime );

	return CNtlPLBalloonManager::UR_SHOW;
}

VOID CSpectatorBalloonGui::Paint(VOID)
{
	m_pFlash->Paint();
}

RwBool CSpectatorBalloonGui::PositionUpdate(VOID)
{
	CRectangle rect = GetNtlGuiManager()->GetGuiManager()->GetPosition();
	RwV2d vPos = API_PL_Calc3DPosTo2D( &m_vPos, rect.GetWidth(), rect.GetHeight(), FALSE );
	
	if( vPos.x != 0.0f || vPos.y != 0.0f )
	{
		RwInt32 nX, nY;
		m_pFlash->GetClientRect().GetCenterPosition( &nX, &nY );
		m_pFlash->SetPosition( (RwInt32)vPos.x - nX, (RwInt32)vPos.y - nY );
		return TRUE;
	}	

	return FALSE;	
}

VOID CSpectatorBalloonGui::FadeEffect(VOID)
{
	if( m_eState == SHOW )
	{
		if( m_fCurrentTime > m_fLifeTime )
			SetState( FADE_OUT );
	}
	else if( m_eState == FADE_IN )
	{
		if( m_fCurrentTime < m_fFadeInTime )
		{
			m_pFlash->SetAlpha( (RwUInt8)gui::GetResultLineInterpolation( m_fCurrentTime, m_fFadeInTime, 0.0f, 255.0f ) );			
		}
		else
		{
			m_pFlash->SetAlpha( 255 );
			SetState( SHOW );
		}
	}
	else if( m_eState == FADE_OUT )
	{
		if( m_fCurrentTime < 0.5f )
		{
			m_pFlash->SetAlpha( (RwUInt8)gui::GetResultLineInterpolation( m_fCurrentTime, m_fFadeInTime, 255.0f, 0.0f ) );			
		}
		else
		{
			m_pFlash->SetAlpha( 0 );
			SetState( HIDE );
		}
	}	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CNtlPLBalloonManager

CNtlPLBalloonManager*	CNtlPLBalloonManager::m_pInstance = NULL;
gui::CStaticBox*		CNtlPLBalloonManager::m_pTestText = NULL;

CNtlPLBalloonManager::CNtlPLBalloonManager( const RwChar* pName )
: CNtlPLGui( pName )
{
	m_uiVisibleBalloonType = CNtlPLBalloonGui::ALL;	
}

CNtlPLBalloonManager::~CNtlPLBalloonManager(VOID)
{
}

RwBool CNtlPLBalloonManager::CreateInstance(VOID)
{
	NTL_FUNCTION( "CNtlPLBalloonManager : CreateInstance" );

	if( m_pInstance )
		NTL_RETURN( TRUE );

	m_pInstance = NTL_NEW CNtlPLBalloonManager( "PLBalloonManager" );
	NTL_ASSERT( m_pInstance, "CNtlPLBalloonManager::CreateInstance : Instance Allocate Fail" );

	if( !m_pInstance->Create( "", "gui\\Balloon.srf", "gui\\Balloon.frm" ) )
		NTL_RETURN( FALSE );
	CNtlPLGuiManager::GetInstance()->AddGui( m_pInstance );

	m_pInstance->CreateComponents( GetNtlGuiManager()->GetGuiManager() );
	m_pInstance->m_pThis = reinterpret_cast<gui::CDialog*>( m_pInstance->GetComponent( "dlgMain" ) );
	m_pInstance->m_pThis->Show( true );
	m_pInstance->m_pThis->Enable( false );
	m_pInstance->m_pThis->SetPriority( 200 );

	// TestText 
	CRectangle rect;
	rect.SetRectWH( 400, 300, 211, 70 );
	m_pInstance->m_pTestText = NTL_NEW gui::CStaticBox( rect, m_pInstance->m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pInstance->m_pTestText->CreateFontStd( DEFAULT_FONT, 95, DEFAULT_FONT_ATTR );
	m_pInstance->m_pTestText->Enable( false );
	m_pInstance->m_pTestText->Show( false );

	// Update 연결
	GetNtlGuiManager()->AddUpdateFunc( m_pInstance );

	// CallBack 연결
	m_pInstance->m_slotPaint = m_pInstance->m_pThis->SigPaint().Connect( m_pInstance, &CNtlPLBalloonManager::OnPaint );

	// Event 연결
	m_pInstance->LinkMsg( g_EventShowSpectatorBalloon );
	m_pInstance->LinkMsg( g_EventHideAllSpectatorBalloon );
	
	NTL_RETURN( TRUE );
}

VOID CNtlPLBalloonManager::DeleteInstance(VOID)
{
	if(m_pInstance == NULL)
		return;

	m_pInstance->RemoveAllBalloonData();

	// TestText
	NTL_DELETE( m_pInstance->m_pTestText );

	// Update 연결해제
	GetNtlGuiManager()->RemoveUpdateFunc( m_pInstance );

	// CallBack 연결 해제
	m_pInstance->m_pThis->SigPaint().Disconnect(m_pInstance->m_slotPaint);

	// Event 연결해제
	m_pInstance->UnLinkMsg( g_EventShowSpectatorBalloon );
	m_pInstance->UnLinkMsg( g_EventHideAllSpectatorBalloon );
	
	m_pInstance->DestroyComponents();
	m_pInstance->Destroy();	

	CNtlPLGuiManager::GetInstance()->RemoveGui( m_pInstance );
	NTL_DELETE( m_pInstance );	
}

CNtlPLBalloonManager* CNtlPLBalloonManager::GetInstance(VOID)
{
	return m_pInstance;
}

gui::CStaticBox* CNtlPLBalloonManager::GetTestText(VOID)
{
	return m_pTestText;
}

VOID CNtlPLBalloonManager::Update( RwReal fElapsed )
{
	m_nDisplayCount = 0;

	if( m_listBalloon.empty() )
		return;

	RwInt32 i = 0;
	memset( m_aDiaplsyBalloon, 0, sizeof( m_aDiaplsyBalloon ) );

	for( std::list<CNtlPLBalloonGui*>::iterator it = m_listBalloon.begin() ; it != m_listBalloon.end() ; )
	{
		if( i >= BALLOON_MAX_PRESENT )
			return;

		CNtlPLBalloonGui* pBalloon = (*it);

		RwInt32 nUpdateResult = pBalloon->Update( fElapsed ); 

		if( nUpdateResult == UR_SHOW )
		{
			if( pBalloon->GetType() & m_uiVisibleBalloonType )
			{
				RwV3d vDist;
				RwV3dSubMacro( &vDist, RwMatrixGetPos( &CNtlPLGlobal::m_RwCamera->viewMatrix ), pBalloon->GetPosition() );	
				m_aDiaplsyBalloon[i].fDist = RwV3dLength( &vDist );
				m_aDiaplsyBalloon[i].pBalloon = pBalloon;
				++m_nDisplayCount;
				++it;
				++i;
			}
			else
				++it;
		}
		else if( nUpdateResult == UR_NOTSHOW )	
		{
			++it;
		}
		else
		{
			it = m_listBalloon.erase( it );
			RemoveBalloonData( pBalloon );
		}				
	}

	// 거리순 정렬
	qsort( m_aDiaplsyBalloon, m_nDisplayCount, sizeof( stDISPLAYBALLOON ), &CNtlPLBalloonManager::Compare );
}

RwInt32 CNtlPLBalloonManager::Compare( const VOID* valLeft, const VOID* valRight )
{
	// 거리가 먼 것을 먼저 찍히도록 앞으로 정렬
	stDISPLAYBALLOON* pLeft = (stDISPLAYBALLOON*)valLeft;
	stDISPLAYBALLOON* pRight= (stDISPLAYBALLOON*)valRight;

	if( pLeft->fDist == pRight->fDist ) 
		return 0;

	if( pLeft->fDist < pRight->fDist )
		return 1;

	return -1;
}

VOID CNtlPLBalloonManager::AddSpectatorBalloon( RwV3d& vPos, const WCHAR* pMessage, RwReal fLifeTime, RwReal fFadeInTime, RwInt32 eType, RwUInt8 byBalloonType )
{
	CNtlPLBalloonGui* pBalloon = NULL;
		
	pBalloon = NTL_NEW CSpectatorBalloonGui( vPos, pMessage, fLifeTime, fFadeInTime, eType, byBalloonType );
	AddBalloonData( pBalloon );	
}

VOID CNtlPLBalloonManager::FadeoutAllBalloon(VOID)
{
	std::list<CNtlPLBalloonGui*>::iterator it;

	for( it = m_listBalloon.begin() ; it != m_listBalloon.end() ; ++it )
	{
		CNtlPLBalloonGui* pBalloon = (*it);
		pBalloon->FadeoutMessage();
	}
}

VOID CNtlPLBalloonManager::AddBalloonData( CNtlPLBalloonGui* pBalloon )
{
	m_listBalloon.push_back( pBalloon );	
}

VOID CNtlPLBalloonManager::RemoveBalloonData( CNtlPLBalloonGui* pBalloon )
{
	std::list<CNtlPLBalloonGui*>::iterator it;

	for( it = m_listBalloon.begin() ; it != m_listBalloon.end() ; ++it )
	{
		CNtlPLBalloonGui* pCurrentBalloon = (*it);
		
		if( pBalloon == pCurrentBalloon )
		{
			NTL_DELETE( pBalloon );
			m_listBalloon.erase( it );
			return;
		}
	}	
}

VOID CNtlPLBalloonManager::RemoveAllBalloonData(VOID)
{
	std::list<CNtlPLBalloonGui*>::iterator it;

	for( it = m_listBalloon.begin() ; it != m_listBalloon.end() ; ++it )
	{
		CNtlPLBalloonGui* pBalloon = (*it);
		NTL_DELETE( pBalloon );
	}

	m_listBalloon.clear();
}

VOID CNtlPLBalloonManager::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventShowSpectatorBalloon )
	{
		SNtlEventShowSpectatorBalloon* pData = reinterpret_cast<SNtlEventShowSpectatorBalloon*>( msg.pData );

		AddSpectatorBalloon( pData->vPos, pData->szMessage, pData->fLifeTime, pData->fFadeInTime, CNtlPLBalloonGui::TYPE_SPECTATOR, pData->byBalloonType );
	}
	else if( msg.Id == g_EventHideAllSpectatorBalloon )
	{
		FadeoutAllBalloon();
	}
}

VOID CNtlPLBalloonManager::OnPaint(VOID)
{
	for( RwInt32 i = 0 ; i < m_nDisplayCount ; ++i )
	{
		m_aDiaplsyBalloon[i].pBalloon->Paint();
	}
}




