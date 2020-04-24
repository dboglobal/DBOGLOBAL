#include "precomp_dboclient.h"
#include "DoRepairSideIconGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"

// Sound
#include "GUISoundDefine.h"

// client
#include "DboGlobal.h"
#include "DisplayStringManager.h"

RwUInt32 flagDoRepairState = DOREPAIR_STATE_NONE;

CDoRepairSideIconGui::CDoRepairSideIconGui( const RwChar* pName )
: CSideIconBase( pName ), m_pbtnIcon( NULL )
{

}

CDoRepairSideIconGui::~CDoRepairSideIconGui(VOID)
{

}

RwBool CDoRepairSideIconGui::Create(VOID)
{
	NTL_FUNCTION( "CDoRepairSideIconGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\DoRepairSide.srf", "gui\\DoRepairSideIcon.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pbtnIcon = (gui::CButton*)GetComponent( "btnIcon" );
	
	m_slotIconButtonClicked = m_pbtnIcon->SigClicked().Connect( this, &CDoRepairSideIconGui::OnIconButtonClicked );
	m_slotPaint = m_pbtnIcon->SigPaint().Connect( this, &CDoRepairSideIconGui::OnPaint );
	m_slotMove = m_pThis->SigMove().Connect( this, &CDoRepairSideIconGui::OnMove );

	// FlickEffect
	gui::CSurface surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "DoRepairSide.srf", "srfDoRepairSideIconPulse" );
	m_feEffect.SetSurface( surface );
	m_feEffect.SetTime( 0.0f, 0.5f );
	m_feEffect.SetAlpha( 0, 255 );
		
	LinkMsg( g_EventSobInfoUpdate, 0 );
	
	Show( false );

	NTL_RETURN( TRUE );
}

VOID CDoRepairSideIconGui::Destroy(VOID)
{
	UnLinkMsg( g_EventSobInfoUpdate );
	
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CDoRepairSideIconGui::Update( RwReal fElapsedTime )
{
	m_feEffect.Update( fElapsedTime );
}

VOID CDoRepairSideIconGui::OnSideViewClosed(VOID)
{

}

VOID CDoRepairSideIconGui::HandleEvents( RWS::CMsg &msg )
{
	// Equip Á¤º¸
	if( msg.Id == g_EventSobInfoUpdate )
	{
		SNtlEventSobInfoUpdate* pUpdate = reinterpret_cast<SNtlEventSobInfoUpdate*>( msg.pData );

		if( pUpdate->hSerialId != GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() )
			return;

		if( pUpdate->uiUpdateType & EVENT_AIUT_ITEM )
		{
			RwBool bNeedToOpen = FALSE;
			flagDoRepairState = DOREPAIR_STATE_NONE;

			for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
			{
				SERIAL_HANDLE hEquipSerial = GetNtlSLGlobal()->GetSobAvatar()->GetInventory()->GetEquipItem( i );

				if( hEquipSerial != INVALID_SERIAL_ID )
				{
					CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hEquipSerial ) );
					CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );

					if( (RwReal)pItemAttr->GetDur() / (RwReal)pItemAttr->GetMaxDur() <= ITEM_DURATION_WARNING )
					{
						bNeedToOpen = TRUE;						
						flagDoRepairState |= DOREPAIR_STATE_FIX;
					}
					
					if( pItemAttr->IsExpired() )
					{
						bNeedToOpen = TRUE;
						flagDoRepairState |= DOREPAIR_STATE_EXTEND;							
					}
				}				
			}

			if( bNeedToOpen )
			{
				if( !m_pThis->IsVisible() )
				{
					if( m_feEffect.IsWork() == FALSE )
					{
						Logic_PlayGUISound(GSD_SYSTEM_WINDOW_ALREAT);
					}

					Show( true );
					GetNtlGuiManager()->AddUpdateFunc( this );
					m_feEffect.StartProc( TRUE );
				}
			}
			else
			{
				if( m_pThis->IsVisible() )
				{
					Show( false );
					GetNtlGuiManager()->RemoveUpdateFunc( this );
					m_feEffect.EndProc();
				}				
			}
		}
	}
}

VOID CDoRepairSideIconGui::OnIconButtonClicked( gui::CComponent* pComponent )
{
	CSideIconGui::GetInstance()->OpenSideView( this, SIDEVIEW_DOREPAIR, NULL );	
}

VOID CDoRepairSideIconGui::OnPaint(VOID)
{
	m_feEffect.Render();
}

VOID CDoRepairSideIconGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rect = m_pThis->GetScreenRect();

	RwInt32 nCenterX, nCenterY;
	gui::CSurface surface = m_feEffect.GetSurface();
	surface.m_Original.rtRect.GetCenterPosition( &nCenterX, &nCenterY );
	m_feEffect.SetCenterPos( rect.left + nCenterX, rect.top + nCenterY );
}

CDoRepairSideViewGui::CDoRepairSideViewGui( const RwChar* pName )
: CSideViewBase( pName ), m_pbtnClose( NULL ), m_pstbInfo( NULL ) 
{

}

CDoRepairSideViewGui::~CDoRepairSideViewGui(VOID)
{

}

RwBool CDoRepairSideViewGui::Create(VOID)
{
	NTL_FUNCTION( "CDoRepairSideViewGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\DoRepairSide.srf", "gui\\DoRepairSideView.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pbtnClose = (gui::CButton*)GetComponent( "btnClose" );
	m_pstbInfo = (gui::CStaticBox*)GetComponent( "stbInfo" );

	m_slotClose = m_pbtnClose->SigClicked().Connect( this, &CDoRepairSideViewGui::OnClickClose );

	//m_pstbInfo->SetText( GetDisplayStringManager()->GetString( "DST_ITEM_DOFIX_SIDE_TEXT" ) );
	//m_pThis->SetSize( m_pstbInfo->GetWidth() + m_pbtnClose->GetWidth() + 5, m_pstbInfo->GetHeight() + 13 );
	//m_pbtnClose->SetPosition( m_pThis->GetWidth() - 19, 6 );
		
	Show( false );

	NTL_RETURN( TRUE );
}

VOID CDoRepairSideViewGui::Destroy(VOID)
{
	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CDoRepairSideViewGui::OnPressESC(VOID)
{

}

VOID CDoRepairSideViewGui::OnSideViewOpen( const VOID* pData )
{
	WCHAR buf[GUI_TEXT_BUFFER_SIZE];
	RwBool bNextLine = FALSE;
	m_pstbInfo->Clear();

	if( flagDoRepairState & DOREPAIR_STATE_FIX )
	{
		wcscpy_s( buf, GUI_TEXT_BUFFER_SIZE, GetDisplayStringManager()->GetString( "DST_ITEM_DOFIX_SIDE_TEXT" ) );
		bNextLine = TRUE;
	}

	if( flagDoRepairState & DOREPAIR_STATE_EXTEND )
	{
		if( bNextLine )
		{
			wcscat_s( buf, GUI_TEXT_BUFFER_SIZE, L"\\n" );
			bNextLine = FALSE;
		}

		wcscat_s( buf, GUI_TEXT_BUFFER_SIZE, GetDisplayStringManager()->GetString( "DST_ITEM_DOEXTEND_SIDE_TEXT" ) );		
	}

	m_pstbInfo->SetText( buf );

	m_pThis->SetSize( m_pstbInfo->GetWidth() + m_pbtnClose->GetWidth() + 5, m_pstbInfo->GetHeight() + 13 );
	m_pbtnClose->SetPosition( m_pThis->GetWidth() - 19, 6 );

	Show( true );
}

VOID CDoRepairSideViewGui::OnSideViewClose(VOID)
{
	Show( false );	
}

VOID CDoRepairSideViewGui::OnSideViewLocate( const CRectangle& rtSideIcon )
{
	RwInt32 nWidth = m_pThis->GetWidth();
	RwInt32 nHeight = m_pThis->GetHeight();

	m_pThis->SetPosition( rtSideIcon.right - nWidth, rtSideIcon.top - nHeight );	
}

VOID CDoRepairSideViewGui::OnClickClose( gui::CComponent* pComponent )
{
	OnSideViewClose();
}
