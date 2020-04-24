#include "precomp_dboclient.h"
#include "ChatDisplayGui.h"

// core
#include "NtlDebug.h"
#include "NtlMath.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLGuiManager.h"

// dbo
#include "DboEvent.h"
#include "DisplayStringManager.h"
#include "IconMoveManager.h"
#include "ChatReceiver.h"
#include "ChatGui.h"
#include "DialogPriority.h"
#include "DialogManager.h"
#include "DboGlobal.h"
#include "CursorManager.h"

#define dMOUSE_CHECK_SIZE				15

#define dDEFAULT_ALPHA					1
#define dMAX_ALPHA						(255 - dDEFAULT_ALPHA)
#define dMIN_ALPHA						(1 - dDEFAULT_ALPHA)

#define dTIME_CHANGE_DELAY_ALPHA		.5f
#define dTIME_CHANGE_ALPHA				.5f


CChatDisplayGui::CChatDisplayGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_pChatGui(NULL)
{
}

CChatDisplayGui::~CChatDisplayGui()
{

}

RwBool CChatDisplayGui::Create(CChatGui* pChatGui)
{
	NTL_FUNCTION( "CChatDisplayGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\ChatDisplay.srf", "gui\\ChatDisplay.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_CHATTING);

	m_pChatGui = pChatGui;

	// Output line
	m_pOutDisplay = (gui::COutputBox*)GetComponent("outputDisplay");
	m_pOutDisplay->SetLineSpace(CHAT_DISP_LINE_GAP);
	m_pOutDisplay->SetMaxLine(CHAT_DISP_MAX_LINE);

	// 최하단 내용 보기 버튼
	m_pLastButton = (gui::CButton*)GetComponent("btnLastButton");
	m_slotLastButton = m_pLastButton->SigClicked().Connect(this, &CChatDisplayGui::OnClicked_LastButton);	

	// init value
	m_ResizingInfo.eType			= INVALID_RESIZE_TYPE;	

	m_AlphaInfo.binCleintRect		= FALSE;
	m_AlphaInfo.byAlphaWeight		= 100;
	m_Tracking.bTracking			= FALSE;

	m_bLock							= TRUE;

	m_tPositionInfo.iOldScreenHeight= 0;

	Component_Locate();
	SetAlpha(m_AlphaInfo.byAlphaWeight);

	for(RwInt8 i = 0 ; i < NUM_ALL_CHAT_TYPE ; ++i )
		m_EnableLogType[i] = FALSE;

	// sig
	m_hMouseMove			= CInputHandler::GetInstance()->LinkMouseMove(this, &CChatDisplayGui::MouseMoveHandler);
	m_hMouseUp				= CInputHandler::GetInstance()->LinkMouseUp(this, &CChatDisplayGui::MouseUpHandler);
	m_slotCaptureWheelMove	= GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CChatDisplayGui::OnCaptureWheelMove );

	m_slotMouseDown		= m_pThis->SigMouseDown().Connect( this, &CChatDisplayGui::OnMouseDown );
	m_slotMouseUp		= m_pThis->SigMouseUp().Connect( this, &CChatDisplayGui::OnMouseUp );
	m_slotMove			= m_pThis->SigMove().Connect( this, &CChatDisplayGui::OnMove );
	m_slotResize		= m_pThis->SigResize().Connect( this, &CChatDisplayGui::OnResize );
	m_slotMoveEnter		= m_pThis->SigMouseEnter().Connect( this, &CChatDisplayGui::OnMouseEnter );
	m_slotMouseMove		= m_pThis->SigMouseMove().Connect( this, &CChatDisplayGui::OnMouseMove );
	m_slotMouseLeave	= m_pThis->SigMouseLeave().Connect( this, &CChatDisplayGui::OnMouseLeave );

	m_slotOutPutMouseDown	= m_pOutDisplay->SigMouseDown().Connect( this, &CChatDisplayGui::OnMouseDown );
	m_slotOutPutMouseUp		= m_pOutDisplay->SigMouseUp().Connect( this, &CChatDisplayGui::OnMouseUp );
	m_slotOutPutMoveEnter	= m_pOutDisplay->SigMouseEnter().Connect( this, &CChatDisplayGui::OnMouseEnter );
	m_slotOutPutMouseMove	= m_pOutDisplay->SigMouseMove().Connect( this, &CChatDisplayGui::OnMouseMove );
	m_slotOutPutMouseLeave	= m_pOutDisplay->SigMouseLeave().Connect( this, &CChatDisplayGui::OnMouseLeave );

	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CChatDisplayGui::OnPaint );


	LinkMsg(g_EventResize);

	NTL_RETURN(TRUE);
}

VOID CChatDisplayGui::Destroy()
{
	NTL_FUNCTION("CChatDisplayGui::Destroy");

	CInputHandler::GetInstance()->UnLinkMouseMove(m_hMouseMove);
	CInputHandler::GetInstance()->UnLinkMouseUp(m_hMouseUp);
	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Disconnect( m_slotCaptureWheelMove );

	UnLinkMsg(g_EventResize);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CChatDisplayGui::Update( RwReal fElapsed )
{
	if( m_AlphaInfo.byAlpha == dMAX_ALPHA )
		return;

	m_AlphaInfo.fElapsed += fElapsed;

	if( m_AlphaInfo.fElapsed < dTIME_CHANGE_DELAY_ALPHA )
		return;

	if( m_AlphaInfo.fElapsed >= dTIME_CHANGE_DELAY_ALPHA + dTIME_CHANGE_ALPHA )
		m_AlphaInfo.fElapsed = dTIME_CHANGE_DELAY_ALPHA + dTIME_CHANGE_ALPHA;


	RwReal fDeltaTime = (m_AlphaInfo.fElapsed - dTIME_CHANGE_DELAY_ALPHA) / dTIME_CHANGE_ALPHA;
	RwUInt32 iMinAlpha = (RwUInt32)m_AlphaInfo.byAlphaWeight;
	RwUInt32 iMaxAlpha = (RwUInt8)(dMAX_ALPHA * ((RwReal)m_AlphaInfo.byAlphaWeight * 0.05f));
    iMinAlpha = min(iMinAlpha, dMAX_ALPHA);
    iMaxAlpha = min(iMaxAlpha, dMAX_ALPHA);

	RwUInt8 byAlpha = (RwUInt8)CNtlMath::Interpolation((RwReal)iMinAlpha, (RwReal)iMaxAlpha, fDeltaTime);

	SetAlpha(byAlpha);	
}

VOID CChatDisplayGui::SetAlpha(RwUInt8 byAlpha)
{
	m_AlphaInfo.byAlpha = byAlpha;
	m_pThis->SetAlpha(m_AlphaInfo.byAlpha + dDEFAULT_ALPHA);
}

VOID CChatDisplayGui::Component_Locate()
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	CRectangle rtOutDisplay = m_pOutDisplay->GetPosition();
	RwInt32 iButtonHeight = m_pLastButton->GetHeight();

	rtOutDisplay.right	= rtScreen.GetWidth() - rtOutDisplay.left;
	rtOutDisplay.bottom	= rtScreen.GetHeight();

	m_pOutDisplay->SetPosition(rtOutDisplay);
	m_pLastButton->SetPosition(5, rtScreen.GetHeight() - iButtonHeight - 5);
}

VOID CChatDisplayGui::Show(bool bShow)
{
	if( !bShow )
	{
		CursorCheck();
		m_ResizingInfo.eType		= INVALID_RESIZE_TYPE;
		m_Tracking.bTracking		= FALSE;
	}

	CNtlPLGui::Show(bShow);
}

VOID CChatDisplayGui::AddText(RwUInt8 byChatType, const WCHAR* pwcText)
{
	if( byChatType >= NUM_ALL_CHAT_TYPE )
		return;	

	if( m_EnableLogType[byChatType] == FALSE )
		return;

	RwUInt32 uiColor;

	switch( byChatType )
	{
		case CHAT_TYPE_GENERAL:		uiColor = CHATGUI_COLOR_GLOBAL;		break;
		case CHAT_TYPE_WHISPER:		uiColor = CHATGUI_COLOR_WHISPER;	break;
		case CHAT_TYPE_PARTY:		uiColor = CHATGUI_COLOR_PARTY;		break;
		case CHAT_TYPE_GUILD:		uiColor = CHATGUI_COLOR_GUILD;		break;
		case CHAT_TYPE_TRADE:		uiColor = CHATGUI_COLOR_TRADE;		break;
		case CHAT_TYPE_FIND_PARTY:	uiColor = CHATGUI_COLOR_FIND_PARTY;	break;
		case CHAT_TYPE_SHOUT:		uiColor = CHATGUI_COLOR_SHOUT;		break;
		case CHAT_TYPE_SYSTEM:		uiColor = CHATGUI_COLOR_SYSTEM_WARN;		break;
		case CHAT_TYPE_NOTIFY:		uiColor = CHATGUI_COLOR_SYSTEM_NOTICE;		break;
		default:
			{
				DBO_FAIL("Wrong Chat type : " << (int)byChatType);
				uiColor = CHATGUI_COLOR_GLOBAL;
				break;
			}
	}

	m_pOutDisplay->AddText(pwcText, uiColor);
}

VOID CChatDisplayGui::AddText(RwUInt8 byChatType, const WCHAR * pwcText, unsigned int uiColor)
{
	if (byChatType >= NUM_ALL_CHAT_TYPE)
		return;

	if (m_EnableLogType[byChatType] == FALSE)
		return;

	m_pOutDisplay->AddText(pwcText, uiColor);
}

VOID CChatDisplayGui::Refresh()
{
	m_pOutDisplay->Clear();

	CChatReceiver* pChatReceiver = m_pChatGui->GetReceiver();

	ITER_CHATLOG it_ChatLog = pChatReceiver->Begin();
	for( ; it_ChatLog != pChatReceiver->End() ; ++it_ChatLog )
	{
		sChatLog* pChatLog = *it_ChatLog;

		if (m_EnableLogType[pChatLog->byChatType])
		{
			AddText(pChatLog->byChatType, pChatLog->wstrString.c_str());
		}
	}
}

VOID CChatDisplayGui::SetAlphaWeight(RwUInt8 byAlpha)
{
	m_AlphaInfo.byAlphaWeight = byAlpha;
	SetAlpha(byAlpha);
}

VOID CChatDisplayGui::EnableLogType(eChatType eType, RwBool bEnable)
{
	if( eType >= NUM_ALL_CHAT_TYPE )
		return;

	m_EnableLogType[eType] = bEnable;	
}

VOID CChatDisplayGui::Lock(bool bLock)
{	
	m_bLock = bLock;

	gui::CScrollBar* pScrollbar = m_pOutDisplay->GetVerticalScrollBar();
	pScrollbar->Show(!m_bLock);
	Enable(!m_bLock);
}

VOID CChatDisplayGui::PageUp()
{
	for( RwInt32 i = 0 ; i < m_pOutDisplay->GetVisibleLineCount() ; ++i )
		m_pOutDisplay->PrevLine();
}

VOID CChatDisplayGui::PageDown()
{
	for( RwInt32 i = 0 ; i < m_pOutDisplay->GetVisibleLineCount() ; ++i )
		m_pOutDisplay->NextLine();
}

CChatDisplayGui::eResizeType CChatDisplayGui::CheckResizePosition(RwInt32 iX, RwInt32 iY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	CRectangle rtRect;

	// 우상단
	rtRect = rtScreen;
	rtRect.left = rtRect.right - dMOUSE_CHECK_SIZE;
	rtRect.bottom = rtScreen.top + dMOUSE_CHECK_SIZE;	

	if( rtRect.PtInRect(iX, iY) )
		return RESIZE_TYPE_RIGHT_TOP;

	// 우하단
	rtRect = rtScreen;
	rtRect.left = rtRect.right - dMOUSE_CHECK_SIZE;
	rtRect.top = rtScreen.bottom - dMOUSE_CHECK_SIZE;

	if( rtRect.PtInRect(iX, iY) )
		return RESIZE_TYPE_RIGHT_BOTTOM;

	// 상단
	rtRect = rtScreen;
	rtRect.bottom = rtScreen.top + dMOUSE_CHECK_SIZE;

	if( rtRect.PtInRect(iX, iY) )
		return RESIZE_TYPE_TOP;

	// 하단
	rtRect = rtScreen;
	rtRect.top = rtScreen.bottom - dMOUSE_CHECK_SIZE;

	if( rtRect.PtInRect(iX, iY) )
		return RESIZE_TYPE_BOTTOM;

	// 우측
	rtRect = rtScreen;
	rtRect.left = rtScreen.right - dMOUSE_CHECK_SIZE;

	if( rtRect.PtInRect(iX, iY) )
		return RESIZE_TYPE_RIGHT;

	return INVALID_RESIZE_TYPE;
}

VOID CChatDisplayGui::CursorCheck()
{
	if( GetDialogManager()->IsMode(DIALOGMODE_UNKNOWN) != FALSE )
	{
		RwUInt8 byCursorState = GetCursorManager()->GetCursorState();

		if( byCursorState == CCursorManager::CS_CHAT_RESIZE_HORI ||
			byCursorState == CCursorManager::CS_CHAT_RESIZE_VERT ||
			byCursorState == CCursorManager::CS_CHAT_RESIZE_RIGHTUP ||
			byCursorState == CCursorManager::CS_CHAT_RESIZE_RIGHTDOWN )
		{
			GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_INVALID_CURSOR );
		}
	}

}

VOID CChatDisplayGui::ResizeDisplay(RwInt32 iCurMouseX, RwInt32 iCurMouseY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();
	RwInt32 iNewX = iCurMouseX - m_ResizingInfo.iConfirmXPos;
	RwInt32 iNewY = iCurMouseY - m_ResizingInfo.iConfirmYPos;

	// 높이 조절
	if( m_ResizingInfo.eType == RESIZE_TYPE_TOP ||
		m_ResizingInfo.eType == RESIZE_TYPE_RIGHT_TOP )
	{
		rtScreen.top += iNewY;		

		if( (rtScreen.bottom - rtScreen.top) < CHAT_DISP_MINIMUM_HEIGHT )
		{
			// 최소 크기
			RwInt32 iRealTop = rtScreen.bottom - CHAT_DISP_MINIMUM_HEIGHT;
			iNewY += iRealTop - rtScreen.top;
			rtScreen.top = iRealTop;
		}
		else if( (rtScreen.bottom - rtScreen.top) > CHAT_DISP_MAXIMUM_HEIGHT )
		{
			// 최대 크기
			RwInt32 iRealTop = rtScreen.bottom - CHAT_DISP_MAXIMUM_HEIGHT;
			iNewY += iRealTop - rtScreen.top;
			rtScreen.top = iRealTop;
		}
	}
	else if( m_ResizingInfo.eType == RESIZE_TYPE_RIGHT_BOTTOM ||
		m_ResizingInfo.eType == RESIZE_TYPE_BOTTOM )
	{
		rtScreen.bottom += iNewY;		

		if( (rtScreen.bottom - rtScreen.top) < CHAT_DISP_MINIMUM_HEIGHT )
		{
			// 최소 크기
			RwInt32 iRealBottom = rtScreen.top + CHAT_DISP_MINIMUM_HEIGHT;
			iNewY += iRealBottom - rtScreen.bottom;
			rtScreen.bottom = iRealBottom;
		}
		else if( (rtScreen.bottom - rtScreen.top) > CHAT_DISP_MAXIMUM_HEIGHT )
		{
			// 최대 크기
			RwInt32 iRealBottom = rtScreen.top + CHAT_DISP_MAXIMUM_HEIGHT;
			iNewY += iRealBottom - rtScreen.bottom;
			rtScreen.bottom = iRealBottom;
		}
	}

	// 넓이 조절
	if( m_ResizingInfo.eType == RESIZE_TYPE_RIGHT_TOP ||
		m_ResizingInfo.eType == RESIZE_TYPE_RIGHT ||
		m_ResizingInfo.eType == RESIZE_TYPE_RIGHT_BOTTOM )
	{
		rtScreen.right += iNewX;

		if( (rtScreen.right - rtScreen.left) < CHAT_DISP_MINIMUM_WIDTH )
		{
			// 최소 크기
			RwInt32 iRealRight = rtScreen.left + CHAT_DISP_MINIMUM_WIDTH;
			iNewX += iRealRight - rtScreen.right;
			rtScreen.right = iRealRight;
		}
		else if( (rtScreen.right - rtScreen.left) > CHAT_DISP_MAXIMUM_WIDTH )
		{
			// 최대 크기
			RwInt32 iRealRight = rtScreen.left + CHAT_DISP_MAXIMUM_WIDTH;
			iNewX += iRealRight - rtScreen.right;
			rtScreen.right = iRealRight;
		}
	}

	// 사이즈가 변경이 되었는가
	if( GetPosition() != rtScreen )
	{
		if( m_byDisplayGuiIndex == 0 )
		{
			// 채팅 입력창도 사이즈를 같이 변경한다
			CChatGui* pChatGui = reinterpret_cast<CChatGui*>( GetDialogManager()->GetDialog(DIALOG_CHAT) );
			CRectangle rtChatScreen = pChatGui->GetPosition();

			rtChatScreen.right = rtChatScreen.left + rtScreen.GetWidth();

			rtChatScreen.SetRect(rtChatScreen.left, rtChatScreen.top, rtChatScreen.right, rtChatScreen.bottom);
			pChatGui->SetPosition(rtChatScreen);

			SetPosition(rtScreen);
		}
		else
		{
			SetPosition(rtScreen);
		}		

		if( m_ResizingInfo.eType == RESIZE_TYPE_TOP ||
			m_ResizingInfo.eType == RESIZE_TYPE_RIGHT_TOP ||
			m_ResizingInfo.eType == RESIZE_TYPE_RIGHT_BOTTOM ||
			m_ResizingInfo.eType == RESIZE_TYPE_BOTTOM )
			m_ResizingInfo.iConfirmYPos += iNewY;

		if( m_ResizingInfo.eType == RESIZE_TYPE_RIGHT_TOP ||
			m_ResizingInfo.eType == RESIZE_TYPE_RIGHT ||
			m_ResizingInfo.eType == RESIZE_TYPE_RIGHT_BOTTOM )
			m_ResizingInfo.iConfirmXPos += iNewX;
	}
}

VOID CChatDisplayGui::Event_MouseEnter()
{
	GetNtlGuiManager()->AddUpdateFunc( this );
	m_AlphaInfo.fElapsed		= 0.f;
	m_AlphaInfo.binCleintRect	= TRUE;
}

VOID CChatDisplayGui::Event_MouseLeave()
{
	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );
	SetAlpha(m_AlphaInfo.byAlphaWeight);

	m_AlphaInfo.binCleintRect	= FALSE;
}

VOID CChatDisplayGui::OnClicked_LastButton(gui::CComponent* pComponent)
{
	m_pOutDisplay->LastLine();
}

RwInt32	CChatDisplayGui::MouseMoveHandler(RwUInt32 uiMouseData)
{
	// GUI에 포커스가 없거나 채팅창이 Enable 상태여도 로직이 동작하도록
	// InputHandler에서 직접 이벤트를 받아들인다
	if( !IsShow() )
		return 1;

	SMouseData* pMouseData = (SMouseData*)uiMouseData;

	if( m_pThis->PosInRect( pMouseData->iPosX, pMouseData->iPosY ) == gui::CComponent::INRECT )
	{
		gui::CComponent* pFocusComponent = m_pThis->GetParent()->GetChildComponentReverseAt( pMouseData->iPosX, pMouseData->iPosY );
		if( pFocusComponent != NULL )
		{
			if( pFocusComponent != m_pThis )
				return 1;
		}

		if( !m_AlphaInfo.binCleintRect )
			Event_MouseEnter();			
	}
	else
	{
		if( m_ResizingInfo.eType != INVALID_RESIZE_TYPE )
			return 1;

		if( m_AlphaInfo.binCleintRect )
			Event_MouseLeave();			
	}

	return 1;
}

RwInt32	CChatDisplayGui::MouseUpHandler(RwUInt32 uiMouseData)
{
	SMouseData* pMouseData = (SMouseData*)uiMouseData;
	
	if( BIT_FLAG_TEST(pMouseData->uiFlags, MF_LBUTTON) )
		return 1;

	if( m_ResizingInfo.eType == INVALID_RESIZE_TYPE )
		return 1;

	m_Tracking.bTracking = FALSE;
	m_pThis->ReleaseMouse();
	GetDialogManager()->OffMode();
	m_ResizingInfo.eType = INVALID_RESIZE_TYPE;	

	Component_Locate();

	return 1;
}

VOID CChatDisplayGui::OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos)
{
	if( !IsShow() )
		return;

	if (m_pThis->GetParent()->GetChildComponentReverseAt(pos.x, pos.y) != m_pThis)
		return;

	if( m_pThis->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT )
		return;

	if( sDelta < 0 )
		m_pOutDisplay->NextLine();
	else
		m_pOutDisplay->PrevLine();

	CDboEventGenerator::DialogEvent(DIALOGEVENT_SKIP_MOUSE_WHEEL, DIALOG_CHAT);
}

VOID CChatDisplayGui::OnMouseDown(const CKey& key)
{
	if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_TUTORIAL) )
		return;

	if( GetIconMoveManager()->IsActive() )
		return;

	if( GetDialogManager()->IsMode(DIALOGMODE_UNKNOWN) == FALSE )
		return;

	if( key.m_nID != UD_LEFT_BUTTON )
		return;

	CRectangle rtRect;
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_ResizingInfo.iConfirmXPos		= CMouse::m_nX;
	m_ResizingInfo.iConfirmYPos		= CMouse::m_nY;
	m_ResizingInfo.iConfirmWidth	= rtScreen.GetWidth();
	m_ResizingInfo.iConfirmHeight	= rtScreen.GetHeight();


	if( m_ResizingInfo.eType != INVALID_RESIZE_TYPE )
	{
		m_ResizingInfo.eType = INVALID_RESIZE_TYPE;
		return;
	}

	if( m_bLock )
		return;


	eResizeType eRESIZE_TYPE = CheckResizePosition(m_ResizingInfo.iConfirmXPos, m_ResizingInfo.iConfirmYPos);
	switch( eRESIZE_TYPE )
	{
	case RESIZE_TYPE_RIGHT_TOP:
		{
			// 우상단
			m_ResizingInfo.eType = RESIZE_TYPE_RIGHT_TOP;
			m_pThis->CaptureMouse();
			GetDialogManager()->OnMode(DIALOGMODE_CHATTING_RESIZE_RIGHTUP);
			return;
		}
	case RESIZE_TYPE_RIGHT_BOTTOM:
		{
			// 우하단
			if( m_byDisplayGuiIndex == 0 )
				return;

			m_ResizingInfo.eType = RESIZE_TYPE_RIGHT_BOTTOM;
			m_pThis->CaptureMouse();
			GetDialogManager()->OnMode(DIALOGMODE_CHATTING_RESIZE_RIGHTDOWN);
			return;
		}
	case RESIZE_TYPE_TOP:
		{
			// 상단
			m_ResizingInfo.eType = RESIZE_TYPE_TOP;
			m_pThis->CaptureMouse();
			GetDialogManager()->OnMode(DIALOGMODE_CHATTING_RESIZE_HORI);
			return;
		}
	case RESIZE_TYPE_BOTTOM:
		{
			// 하단
			if( m_byDisplayGuiIndex == 0 )
				return;

			m_ResizingInfo.eType = RESIZE_TYPE_BOTTOM;
			m_pThis->CaptureMouse();
			GetDialogManager()->OnMode(DIALOGMODE_CHATTING_RESIZE_HORI);
			return;
		}
	case RESIZE_TYPE_RIGHT:
		{
			// 우측
			m_ResizingInfo.eType = RESIZE_TYPE_RIGHT;
			m_pThis->CaptureMouse();
			GetDialogManager()->OnMode(DIALOGMODE_CHATTING_RESIZE_VERT);
			return;
		}
	case INVALID_RESIZE_TYPE:
		break;
	default:
		{
			DBO_FAIL("Invalid resize position type");
		}
	}


	// ChatDisplay를 움직일 수 있는지...
	if( !m_bLock )
	{
		m_Tracking.bTracking = TRUE;
		m_Tracking.iCapturedX = CMouse::m_nX;
		m_Tracking.iCapturedY = CMouse::m_nY;
		m_pThis->CaptureMouse();
	}
}

VOID CChatDisplayGui::OnMouseUp(const CKey& key)
{
	if( key.m_nID != UD_LEFT_BUTTON )
		return;

	m_Tracking.bTracking = FALSE;
	m_pThis->ReleaseMouse();
	GetDialogManager()->OffMode();

	if( m_ResizingInfo.eType == INVALID_RESIZE_TYPE )
		return;

	m_ResizingInfo.eType = INVALID_RESIZE_TYPE;	

	Component_Locate();
}

VOID CChatDisplayGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	m_tPositionInfo.iOldScreenHeight = GetDboGlobal()->GetScreenHeight();
	Component_Locate();
}

VOID CChatDisplayGui::OnResize( RwInt32 iOldW, RwInt32 iOldH )
{
	Component_Locate();
}

VOID CChatDisplayGui::OnMouseEnter(gui::CComponent* pComponent)
{
	if( m_AlphaInfo.binCleintRect )
		return;

	CRectangle rtScreen = m_pThis->GetScreenRect();
	if( rtScreen.PtInRect(CMouse::m_nX, CMouse::m_nY) )
	{
		Event_MouseEnter();
	}
}

VOID CChatDisplayGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	if( m_bLock )
		return;

	if( m_ResizingInfo.eType == INVALID_RESIZE_TYPE )
	{
		if( GetDialogManager()->IsMode(DIALOGMODE_UNKNOWN) == false )
			return;

		CRectangle rtScreen = m_pThis->GetScreenRect();
		eResizeType eType = CheckResizePosition(rtScreen.left + nX, rtScreen.top + nY);

		switch( eType )
		{
		case RESIZE_TYPE_TOP:
			{
				GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_CHAT_RESIZE_HORI );
				break;
			}
		case RESIZE_TYPE_BOTTOM:
			{
				if( m_byDisplayGuiIndex == 0 )
					break;

				GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_CHAT_RESIZE_HORI );
				break;
			}
		case RESIZE_TYPE_RIGHT_TOP:
			{
				GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_CHAT_RESIZE_RIGHTUP );
				break;
			}
		case RESIZE_TYPE_RIGHT:
			{
				GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_CHAT_RESIZE_VERT );
				break;
			}
		case RESIZE_TYPE_RIGHT_BOTTOM:
			{
				if( m_byDisplayGuiIndex == 0 )
					break;

				GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_CHAT_RESIZE_RIGHTDOWN );
				break;
			}
		case INVALID_RESIZE_TYPE:
			{
				GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_INVALID_CURSOR );
				break;
			}
		default:
			{
				NTL_ASSERT(false, "CChatDisplayGui::OnMouseMove, Wrong resize type");
				break;
			}
		}
	}
	else
	{
		ResizeDisplay(CMouse::m_nX, CMouse::m_nY);	
	}

	if( m_Tracking.bTracking )
	{
		if( m_Tracking.iCapturedX != CMouse::m_nX ||
			m_Tracking.iCapturedY != CMouse::m_nY )
		{			
			

			if( m_byDisplayGuiIndex == 0 )
			{
				CChatGui* pChatGui = reinterpret_cast<CChatGui*>( GetDialogManager()->GetDialog(DIALOG_CHAT) );
				if( !pChatGui )
					return;

				// 채팅을 입력하는 창도 같이 움직인다
				CRectangle rtChatScreen = pChatGui->GetPosition();
				RwInt32 iX = rtChatScreen.left + (CMouse::m_nX - m_Tracking.iCapturedX);
				RwInt32 iY = rtChatScreen.top + (CMouse::m_nY - m_Tracking.iCapturedY);

				pChatGui->SetPosition(iX, iY);
			}
			else
			{
				CRectangle rtScreen = m_pThis->GetScreenRect();
				RwInt32 iX = rtScreen.left + (CMouse::m_nX - m_Tracking.iCapturedX);
				RwInt32 iY = rtScreen.top + (CMouse::m_nY - m_Tracking.iCapturedY);

				SetPosition(iX, iY);
			}
			

			m_Tracking.iCapturedX = CMouse::m_nX;
			m_Tracking.iCapturedY = CMouse::m_nY;
		}
	}
}

VOID CChatDisplayGui::OnMouseLeave(gui::CComponent* pComponent)
{
	CursorCheck();
	
	if( !m_AlphaInfo.binCleintRect )
		return;

	if( m_ResizingInfo.eType != INVALID_RESIZE_TYPE )
		return;	

	CRectangle rtScreen = m_pThis->GetScreenRect();
	if( rtScreen.PtInRect(CMouse::m_nX, CMouse::m_nY) )
	{
		if( m_pThis->GetParent()->GetChildComponentReverseAt(CMouse::m_nX, CMouse::m_nY) == m_pThis )
			return;
	}

	Event_MouseLeave();
}

VOID CChatDisplayGui::OnPaint()
{
}

VOID CChatDisplayGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CChatDisplayGui::HandleEvents");

	if( msg.Id == g_EventResize )
	{
		if( m_byDisplayGuiIndex == 0 )
			NTL_RETURNVOID();

		RwInt32 iNewX, iNewY;
		CRectangle rtScreen = m_pThis->GetScreenRect();
		RwInt32 iCleintWidth = GetDboGlobal()->GetScreenWidth();
		RwInt32 iCleintHeight = GetDboGlobal()->GetScreenHeight();

		if( abs(rtScreen.left) < 100 )
		{
			iNewX = dDIALOG_CLEINT_EDGE_GAP;
		}
		else
		{
			if( rtScreen.right >= iCleintWidth )
			{
				iNewX = iCleintWidth - 150;
			}
			else
			{
				iNewX = rtScreen.left;
			}
		}

		iNewY = rtScreen.top + (iCleintHeight - m_tPositionInfo.iOldScreenHeight);

		if( iNewX != rtScreen.left || iNewY != rtScreen.top )
		{				
			m_pThis->SetPosition(iNewX, iNewY);
		}
	}

	NTL_RETURNVOID();
}