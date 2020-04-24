#include "precomp_dboclient.h"
#include "MsgBoxGui.h"

// core
#include "NtlDebug.h"
#include "NtlSysEvent.h"
#include "NtlStringUtil.h"

// presenation
#include "NtlPLGuiManager.h"

// simulation 
#include "NtlSLEvent.h"
#include "InputActionMap.h"

// shared
#include "NtlTeleport.h"

// dbo
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboPacketGenerator.h"
#include "DialogManager.h"
#include "ChatGui.h"
#include "DisplayStringManager.h"
#include "MsgBoxManager.h"

#define MSGBOX_DISPLAYTEXT_YOFFSET	5
#define MSGBOX_HEIGHT_OFFSET	( 80 + 2 * MSGBOX_DISPLAYTEXT_YOFFSET ) 
#define MSGBOX_HEIGHT_OFFSET_FOR_INPUT		30
#define MSGBOX_DISPLAYTEXT_YPOS	( 33 + MSGBOX_DISPLAYTEXT_YOFFSET )	
#define MSGBOX_INPUT_YPOS_OFFSET_FROM_DISPLAY_BOTTOM	5	
#define MSGBOX_BTN_YPOS_OFFSET_FROM_DISPLAY_BOTTOM	( 9 + MSGBOX_DISPLAYTEXT_YOFFSET )

CMsgBoxGui::CMsgBoxGui()
{
	Init();	
}

CMsgBoxGui::CMsgBoxGui(const RwChar *pName)
:CNtlPLGui(pName)
{
	Init();
}

CMsgBoxGui::~CMsgBoxGui()
{
}

RwBool CMsgBoxGui::Create(const char* StringID)
{
	NTL_FUNCTION("CMsgBoxGui::Create");

	if(!CNtlPLGui::Create("gui\\MsgBox.rsr", "gui\\MsgBox.srf", "gui\\MsgBox.frm"))
		NTL_RETURN(FALSE);

	m_strStringID = StringID;

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
			
	m_pbtnOk = (gui::CButton*)GetComponent ("btnOk");
	m_pbtnCancel = (gui::CButton*)GetComponent ("btnCancel");
	m_phtmDisplay = (gui::CHtmlBox*)GetComponent ("htmlDisplay");
	m_pstbTitle = (gui::CStaticBox*)GetComponent( "stbTitle" );
	m_pInput = (gui::CInputBox*)GetComponent( "InputBox" );
	m_pInput->SetCaretSize(dINPUTBOX_CARET_WIDTH, dINPUTBOX_CARET_HEIGHT);
	
	m_slotReturn = m_pInput->SigReturnPressed().Connect(this, &CMsgBoxGui::OnReturn);
	m_slotGotFocus = m_pInput->SigGotFocus().Connect(this, &CMsgBoxGui::OnGotFocus);
	m_slotLostFocus = m_pInput->SigLostFocus().Connect(this, &CMsgBoxGui::OnLostFocus);
	m_slotOk = m_pbtnOk->SigClicked().Connect(this, &CMsgBoxGui::OnClickedOk);	
	m_slotCancel = m_pbtnCancel->SigClicked().Connect(this, &CMsgBoxGui::OnClickedCancel);	
	m_slotMove = m_pThis->SigMove().Connect( this, &CMsgBoxGui::OnMove );
	m_slotPaint = m_pThis->SigPaint().Connect( this, &CMsgBoxGui::OnPaint );

	m_rtOk = m_pbtnOk->GetPosition();
	m_rtCancel = m_pbtnCancel->GetPosition();

	m_srfBack.SetType(CWindowby3::WT_HORIZONTAL);
	m_srfBack.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfMsgBackUp" ) );
	m_srfBack.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfMsgBackMid" ) );
	m_srfBack.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfMsgBackDown" ) );

	m_srfBlackPanel.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfBlackPanel" ) );
	m_srfBlackPanel.Show( FALSE );

	m_srfSlotBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MsgBox.srf", "srfSlotBack"));
	m_srfSlotBack.Show(FALSE);
		
	m_pstbTitle->SetText( GetDisplayStringManager()->GetString( "DST_MSG_BOX_TITLE" ) );
	m_pbtnOk->SetText( GetDisplayStringManager()->GetString( "DST_MSG_BTN_OK" ) );
	m_pbtnCancel->SetText( GetDisplayStringManager()->GetString( "DST_MSG_BTN_CANCEL" ) );

	GetNtlGuiManager()->AddUpdateFunc( this );	
	
	NTL_RETURN(TRUE);
}

VOID CMsgBoxGui::Destroy(VOID)
{
	NTL_FUNCTION("CMsgBoxGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );
	DeleteCustomBtnAll();

	if( GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pInput )
		GetNtlGuiManager()->GetGuiManager()->SetFocus( GetNtlGuiManager()->GetGuiManager() );

	CNtlPLGui::DestroyComponents();
    CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CMsgBoxGui::Update( RwReal fElapsed )
{
	if( m_bHide )
	{	
		GetMsgBoxManager()->DeleteMsgBox( this );
	}
	else
	{
		if( m_fShowTime > 0.0f )
		{
			RwInt32 nTime = (RwInt32)fElapsed;
			m_fCurrentTime -= fElapsed;

			if( m_fCurrentTime <= 0.99f )
			{
				ProccessDefault();
			}
			else if( m_fShowTime > 0.0f && nTime != (RwInt32)m_fCurrentTime )
			{
				SetText();
			}		
		}	
	}
}


VOID CMsgBoxGui::Show( bool bShow )
{
	CNtlPLGui::Show( bShow );

	if( bShow )
	{
		//GetNtlGuiManager()->AddUpdateFunc( this );	
	}
	else 
	{
		//GetNtlGuiManager()->RemoveUpdateFunc( this );

		if( GetNtlGuiManager()->GetGuiManager()->GetFocus() == m_pInput )
			GetNtlGuiManager()->GetGuiManager()->SetFocus( GetNtlGuiManager()->GetGuiManager() );
	}
}

VOID CMsgBoxGui::Popup( bool bPopup )
{
	CNtlPLGui::Popup( bPopup );

	if( bPopup )
		m_srfBlackPanel.Show( TRUE );
	else
		m_srfBlackPanel.Show( FALSE );	
}

VOID CMsgBoxGui::CenterAlign( RwUInt32 uiWidth, RwUInt32 uiHeight )
{
	CNtlPLGui::CenterAlign( uiWidth, uiHeight );
	m_srfBlackPanel.SetSize( uiWidth, uiHeight );
}

void CMsgBoxGui::SetBoxData( RwBool bPopup, RwBool bHasInput, std::list<sMsgBoxCustomBtn>* plistCustomBtn, const WCHAR* szText,
	const char* strStringID, sMsgBoxData* pData, RwReal fShowTime)
{
	m_bPopup = bPopup;
	m_strStringID = strStringID;
	m_wstrDisplayText = szText;
	memset(&m_Data, 0, sizeof(sMsgBoxData));

	if( pData )
		memcpy_s( &m_Data, sizeof( sMsgBoxData ), pData, sizeof( sMsgBoxData ) );

	// Btn Show 유무 판별.
	m_pInput->Show( false );
	m_pbtnOk->Show( false );
	m_pbtnCancel->Show( false );


	if( plistCustomBtn )
	{
		std::list<sMsgBoxCustomBtn>::iterator iter;
		for( iter = plistCustomBtn->begin() ; iter != plistCustomBtn->end() ; ++iter )
		{
			sMsgBoxCustomBtn* pCustomBtn = &(*iter);
			
			//DBO_WARNING_MESSAGE("has button: " << pCustomBtn->bHasButton << " name: " << pCustomBtn->strMessage);

			if (pCustomBtn->bHasButton)
			{
				if (pCustomBtn->strMessage == "DST_MSG_BTN_CANCEL")
					m_pbtnCancel->Show(true);
				else if (pCustomBtn->strMessage == "DST_MSG_BTN_OK")
					m_pbtnOk->Show(true);
				else
				{
					AddCustomBtn(pCustomBtn);
				}
			}
		}
	}	

	if(bHasInput)
	{
		m_pInput->Clear();
		m_pInput->SetFocus();
		m_pInput->Show( true );
	}
	
	if( fShowTime > 0.0f )
	{
		m_fShowTime = fShowTime;
		m_fCurrentTime = fShowTime + 0.99f;		
	}	

	if( bPopup )
		m_pThis->SetPriority( dDIALOGPRIORITY_POPUP_MSGBOX );
	else
		m_pThis->SetPriority( dDIALOGPRIORITY_MSGBOX );

	RaiseTop();
	
	SetText();
	MsgBoxSizeProc( TRUE );
	ButtonPositionProc();

	Show( true );
}

VOID CMsgBoxGui::UpdateMsgBox( const WCHAR* szText, sMsgBoxData* pData )
{
	m_wstrDisplayText = szText;

	memset(&m_Data, 0, sizeof(sMsgBoxData));
	
	if( pData )
		memcpy_s( &m_Data, sizeof( sMsgBoxData ), pData, sizeof( sMsgBoxData ) );

	RaiseTop();

	SetText();
	MsgBoxSizeProc( FALSE );
	ButtonPositionProc();

	if( !IsShow() )
		Show( true );
}

RwBool CMsgBoxGui::IsMsgBoxStringID(const char* strStringID )
{
	return strStringID == m_strStringID ? TRUE : FALSE ;
}

VOID CMsgBoxGui::SendOK(VOID)
{
	OnClickedOk( NULL );
}

VOID CMsgBoxGui::SendCancel(VOID)
{
	OnClickedCancel( NULL );
}

VOID CMsgBoxGui::DoNotHide(VOID)
{
	m_bHide = FALSE;
}

VOID CMsgBoxGui::ProccessDefault()
{
	if( !m_bHide )
	{
		if (m_pbtnCancel->IsVisible())
		{
			OnClickedCancel( NULL );
			return;
		}

		if (m_pbtnOk->IsVisible())
		{
			OnClickedOk( NULL );
			return;
		}

		// Ok Cancel이 없는 경우.
		if(m_pbtnCancel->IsVisible() == false && m_pbtnOk->IsVisible() == false)
			m_bHide = TRUE;
		else
			OnClickedOk( NULL );
	}	
}

VOID CMsgBoxGui::PositionAlign( RwInt32 nWidth, RwInt32 nHeight )
{
	SetPosition( (RwInt32)( nWidth * m_fPosXRate ), (RwInt32)( nHeight * m_fPosYRate ) );
	m_srfBlackPanel.SetSize( nWidth, nHeight );
}

VOID CMsgBoxGui::Init(VOID)
{
	m_pbtnOk = NULL;
	m_pbtnCancel = NULL;
	m_phtmDisplay = NULL;
	m_pstbTitle = NULL;

	m_strStringID = "";

	m_fShowTime = 0.0f;
	m_fCurrentTime = 0.0f;
	m_bPopup = FALSE;

	m_bHide = FALSE;

	memset(&m_Data, NULL, sizeof(sMsgBoxData));
}

VOID CMsgBoxGui::SetText(VOID)
{
	if( m_fShowTime == 0.0f )
	{
		m_phtmDisplay->SetHtmlFromMemory( m_wstrDisplayText.c_str(), m_wstrDisplayText.size() );
	}
	else
	{
		WCHAR buf[GUI_TEXT_BUFFER_SIZE] = { 0, };
		WCHAR bufTime[GUI_TEXT_BUFFER_SIZE] = { 0, };
		swprintf_s( bufTime, GUI_TEXT_BUFFER_SIZE, GetDisplayStringManager()->GetString( "DST_MSG_REMAIN_TIME" ), Logic_GetTimeString( m_fCurrentTime ) );
		swprintf_s( buf, GUI_TEXT_BUFFER_SIZE, L"%s%s", m_wstrDisplayText.c_str(), bufTime );
		m_phtmDisplay->SetHtmlFromMemory( buf, wcslen( buf ) );
	}
}

VOID CMsgBoxGui::MsgBoxSizeProc( RwBool bCenterAlign )
{
	CRectangle rect = m_pThis->GetPosition();
	RwInt32 nMessageHeight = 0;
	RwInt32 nMsgBoxHeight = 0;
	nMessageHeight = m_phtmDisplay->GetHeight();
	nMsgBoxHeight = nMessageHeight + MSGBOX_HEIGHT_OFFSET;
	
	if(m_pInput->IsVisible())
	{
		nMsgBoxHeight += MSGBOX_HEIGHT_OFFSET_FOR_INPUT;		
		m_pInput->SetPosition( m_pInput->GetPosition().left, MSGBOX_DISPLAYTEXT_YPOS + nMessageHeight + MSGBOX_INPUT_YPOS_OFFSET_FROM_DISPLAY_BOTTOM + MSGBOX_DISPLAYTEXT_YOFFSET );		
	}

	rect.SetRectWH( rect.left, rect.top, rect.GetWidth(), nMsgBoxHeight );	
	SetPosition( rect );
	m_srfBack.SetSize( rect.GetWidth(), rect.GetHeight() );
	
	if( bCenterAlign )
		CenterAlign( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );	
}

VOID CMsgBoxGui::ButtonPositionProc(VOID)
{
	RwInt32 nButtonCount = 0;
	RwInt32 nCurrentIndex = 0;

	nButtonCount = (RwInt32)m_listCustomBtn.size();
	if( m_pbtnOk->IsVisible() )
		++nButtonCount;

	if( m_pbtnCancel->IsVisible() )
		++nButtonCount;

	//
	if( m_pbtnOk->IsVisible() )
	{
		SetButtonPosition( m_pbtnOk, nCurrentIndex, nButtonCount );
		++nCurrentIndex;
	}

	std::list<sMsgBoxCustomBtnData>::iterator iter;
	for( iter = m_listCustomBtn.begin() ; iter != m_listCustomBtn.end() ; ++iter )
	{
		gui::CComponent* pComponent = (*iter).m_pComponent;
		SetButtonPosition( pComponent, nCurrentIndex, nButtonCount );
		++nCurrentIndex;
	}

	if( m_pbtnCancel->IsVisible() )
	{
		SetButtonPosition( m_pbtnCancel, nCurrentIndex, nButtonCount );
		++nCurrentIndex;
	}
}

VOID CMsgBoxGui::SetButtonPosition( gui::CComponent* pComponent, RwInt32 nIndex, RwInt32 nTotalButtonCount )
{
	RwInt32 nWidth = GetWidth() / nTotalButtonCount;
	RwInt32 nXPos = 0;
	RwInt32 nYPos = m_phtmDisplay->GetHeight() + MSGBOX_DISPLAYTEXT_YPOS + MSGBOX_BTN_YPOS_OFFSET_FROM_DISPLAY_BOTTOM;

	if(m_pInput->IsVisible())
		nYPos += MSGBOX_HEIGHT_OFFSET_FOR_INPUT;

	//nXPos = ( nWidth * nIndex ) + ( nWidth - pComponent->GetWidth() ) / 2;// disabled by daneos
	nXPos = ( GetWidth() / ( nTotalButtonCount + 1 ) ) * ( nIndex + 1 ) - ( pComponent->GetWidth() / 2 ); // enabled by daneos

	pComponent->SetPosition( nXPos, nYPos );
}

VOID CMsgBoxGui::CalcPosRate(VOID)
{
	CRectangle rect = GetPosition();
	m_fPosXRate = (RwReal)rect.left / (RwReal)GetDboGlobal()->GetScreenWidth();
	m_fPosYRate = (RwReal)rect.top / (RwReal)GetDboGlobal()->GetScreenHeight();
}

VOID CMsgBoxGui::AddCustomBtn( sMsgBoxCustomBtn* pCustomBtnData )
{
	gui::CButton* pButton = NULL;
	sMsgBoxCustomBtnData sData;
	CRectangle rect;
	rect.SetRectWH( 0, 0, m_pbtnOk->GetWidth(), m_pbtnOk->GetHeight() );

	pButton = NTL_NEW gui::CButton( rect, GetDisplayStringManager()->GetString( pCustomBtnData->strMessage ), m_pThis, GetNtlGuiManager()->GetSurfaceManager() );
	pButton->AddSurfaceUp( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfCommonBtnUp" ) );
	pButton->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfCommonBtnDown" ) );
	pButton->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MsgBox.srf", "srfCommonBtnFoc" ) );	

	sData.strMessage = pCustomBtnData->strMessage;
	sData.m_slotClickCustomButton = pButton->SigClicked().Connect( this, &CMsgBoxGui::OnClickedCustomBtn );
	sData.m_pComponent = pButton;

	m_listCustomBtn.push_back( sData );	
}

VOID CMsgBoxGui::DeleteCustomBtnAll(VOID)
{
	std::list<sMsgBoxCustomBtnData>::iterator iter;
	for( iter = m_listCustomBtn.begin() ; iter != m_listCustomBtn.end() ; ++iter )
	{
		gui::CComponent* pComponent = (*iter).m_pComponent;
		NTL_DELETE( pComponent );
	}

	m_listCustomBtn.clear();
}

void CMsgBoxGui::OnClickedOk( gui::CComponent* pComponent )
{
	if( !m_bHide )
	{
		m_Data.pwcText = NULL;

		if(m_pInput->IsVisible())
		{		
			if( m_pInput->GetLength() == 0 )
			{
				// I have not entered anything.
				GetAlarmManager()->AlarmMessage("DST_MSG_MUST_INPUT_MESSAGE");
				return;
			}

			m_Data.pwcText = (const WCHAR*)m_pInput->GetText();
		}

		m_bHide = TRUE;
		CDboEventGenerator::MsgBoxResult(MBR_OK, m_strStringID.c_str(), &m_Data );
	}	
}

void CMsgBoxGui::OnClickedCancel( gui::CComponent* pComponent )
{
	if( !m_bHide )
	{
		m_bHide = TRUE;

		CDboEventGenerator::MsgBoxResult(MBR_CANCEL, m_strStringID.c_str(), &m_Data);
	}	
}

void CMsgBoxGui::OnClickedCustomBtn( gui::CComponent* pComponent )
{
	if( !m_bHide )
	{
		std::list<sMsgBoxCustomBtnData>::iterator iter;
		for( iter = m_listCustomBtn.begin() ; iter != m_listCustomBtn.end() ; ++iter )
		{
			gui::CComponent* pCurComponent = (*iter).m_pComponent;
			if( pComponent == pCurComponent )
			{
				sMsgBoxCustomBtnData* pData = &(*iter);

				m_Data.pwcText = Ntl_MB2WC(pData->strMessage.c_str());
			//	DBO_WARNING_MESSAGE("pData->strMessage: " << pData->strMessage.c_str());
				if (m_strStringID == "DST_COMMERCIAL_MB_USE_TERM_LIMETED_ITEM_MSG")
					m_bHide = TRUE;
				else if (m_strStringID == "DST_TELEPORT_COMMON_CONFIRM_MSG")
					m_bHide = TRUE;
				else if (m_strStringID == "DST_TELEPORT_FINAL_CONFIRM_MSG")
					m_bHide = TRUE;
				else if (m_strStringID == "DST_COMMERCIAL_MB_DELETE_TERM_LIMETED_ITEM_MSG")
					m_bHide = TRUE;
				else if (m_strStringID == "DST_COMMERCIAL_MB_UPDATE_TERM_LIMETED_ITEM_MSG")
					m_bHide = TRUE;

				CDboEventGenerator::MsgBoxResult(MBR_OK, m_strStringID.c_str(), &m_Data );

				Ntl_CleanUpHeapStringW(m_Data.pwcText); // clean memory

				break;
			}		
		}
	}	
}

void  CMsgBoxGui::OnReturn()
{
	OnClickedOk(NULL);
}

void  CMsgBoxGui::OnGotFocus()
{
	GetDboGlobal()->GetGameData()->bChatMode = TRUE;
}

void  CMsgBoxGui::OnLostFocus()
{
	GetDboGlobal()->GetGameData()->bChatMode = FALSE;
}

VOID CMsgBoxGui::OnMove( RwInt32 iOldX, RwInt32 iOldY )
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	CRectangle rtRect;

	m_srfBack.SetPosition(rtScreen.left, rtScreen.top);	
	CalcPosRate();
}

VOID CMsgBoxGui::OnPaint(VOID)
{
	m_srfBack.Render();
	m_srfBlackPanel.Render();
}

VOID CMsgBoxGui::OnSetAlpha( RwUInt8 byAlpha )
{
	m_srfBack.SetAlpha( byAlpha );
}
