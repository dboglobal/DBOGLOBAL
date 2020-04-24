#include "precomp_dboclient.h"
#include "TBGambleResult.h"

// Core
#include "NtlPLDef.h"
#include "NtlDebug.h"

// Shared
#include "NtlResultCode.h"

// Gui
#include "gui_guimanager.h"

// Presentation
#include "NtlPLGuiManager.h"

// Simulation
#include "NtlSLLogic.h"
#include "DisplayStringManager.h"

// Dbo
#include "DialogPriority.h"
#include "DialogManager.h"
#include "DboEvent.h"

/**
* \brief Construction
*/
CTBGambleResultGui::CTBGambleResultGui( const RwChar* pName )
: CNtlPLGui( pName )
,m_pBtnClose(NULL)
,m_pPnlSlot(NULL)
,m_pStbMessage(NULL)
{

}

/**
* \brief Destruction
*/
CTBGambleResultGui::~CTBGambleResultGui()
{

}

/**
* \brief Create
*/
RwBool CTBGambleResultGui::Create()
{
	NTL_FUNCTION( "CTBGambleResultGui::Create" );

	if( !CNtlPLGui::Create("", "gui\\TBGambleResult.srf", "gui\\TBGambleResult.frm" ) )
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis				= (gui::CDialog*)GetComponent( "dlgMain" );
	m_pBtnClose			= (gui::CButton*)GetComponent( "btnClose" );
	m_pPnlSlot			= (gui::CPanel*)GetComponent( "panSlot" );
	m_pStbMessage		= (gui::CStaticBox*)GetComponent( "stbMessage" );

	m_pThis->SetPriority( dDIALOGPRIORITY_TBGAMBLERESULT );

	m_sgGambleItem.Create( m_pThis, DIALOG_TBGAMBLERESULT, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
	m_pStbMessage->SetText( GetDisplayStringManager()->GetString( "DST_GAMBLE_GUIDE_RESULT" ) );

	m_slotMove = m_pThis->SigMove().Connect( this, &CTBGambleResultGui::OnMove );
	m_slotPaint = m_pPnlSlot->SigPaint().Connect( this, &CTBGambleResultGui::OnPaint );
	m_slotClickedBtnClose = m_pBtnClose->SigClicked().Connect( this, &CTBGambleResultGui::OnClickedBtnClose );

	LinkMsg( g_EventShopGambleBuyRes );

	Show( false );

	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
VOID CTBGambleResultGui::Destroy()
{
	NTL_FUNCTION("CTBGambleResultGui::Destroy");

	m_sgGambleItem.Destroy();

	UnLinkMsg( g_EventShopGambleBuyRes );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

/**
* \brief HandleEvents
* \param msg		(RWS::CMsg&) 이벤트 메시지 구조체
*/
VOID CTBGambleResultGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CTBGambleResultGui::HandleEvents");

	if( msg.Id == g_EventShopGambleBuyRes )
	{
		SDboEventShopGambleBuyRes* pResult = reinterpret_cast< SDboEventShopGambleBuyRes* >( msg.pData );

		if( pResult->wResultCode == GAME_SUCCESS )
		{
			SetGambleItem( pResult->hItem );
			if( !GetDialogManager()->IsOpenDialog( DIALOG_TBGAMBLERESULT ) )
				GetDialogManager()->OpenDialog( DIALOG_TBGAMBLERESULT );
		}
	}

	NTL_RETURNVOID();
}

/**
* \brief Dialog manager에서 Show/Hide 해주는 interface
*/
RwInt32 CTBGambleResultGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	return TRUE;
}

/**
* \brief Gui의 Slot에 겜블된 아이템의 아이콘을 정의한다.
*/
VOID CTBGambleResultGui::SetGambleItem( RwInt32 hItemHandle )
{
	if( hItemHandle == INVALID_SERIAL_ID )
	{
		NTL_ASSERT( 0, "CTBGambleResultGui::SetGambleItem - Invalid item handle " );
		return;
	}

	m_sgGambleItem.Clear();
	m_sgGambleItem.SetSerialType( REGULAR_SLOT_ITEM_SOB );
	m_sgGambleItem.SetIcon( hItemHandle );
}

/**
* \brief 현재의 부모 Dialog가 Move될때 호출
*/
VOID CTBGambleResultGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rect = m_pPnlSlot->GetScreenRect();
	m_sgGambleItem.SetParentPosition( rect.left + 2, rect.top + 16 );
}

/**
* \brief 현재의 부모 Dialog가 Paint 될때 호출
*/
VOID CTBGambleResultGui::OnPaint()
{
	m_sgGambleItem.Paint();
}

/**
* \brief 닫기 버튼을 클릭 했을 때
*/
VOID CTBGambleResultGui::OnClickedBtnClose( gui::CComponent* pComponent )
{
	GetDialogManager()->CloseDialog( DIALOG_TBGAMBLERESULT );
}
