#include "precomp_dboclient.h"
#include "ServerSelectGui.h"

#include <algorithm>

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLDef.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DisplayStringManager.h"
#include "CharStageState.h"
#include "DialogDefine.h"
#include "AlarmManager.h"


#define dMAX_VISIBLE_COUNT					10
#define dSERVER_HEIGHT_BUTTON_GAP			24
#define dSERVER_HEIGHT_GAP					24
#define dARROW_X_ADJUST_FROM_BUTTON_RIGHT	18

#define dSERVER_SLOT_X						22
#define dSERVER_SLOT_START_Y				108
#define dCHANNEL_TEXT__SERVER_NAME_X		50
#define dCHANNEL_TEXT__SERVER_CHARNUM_X		210
#define dCHANNEL_TEXT__SERVER_STATE_X		292
#define dCHANNEL_TEXT_START_Y				115


CServerSelectGui::CServerSelectGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_bServerAscendingSort(FALSE)
,m_bCharacterAscendingSort(FALSE)
,m_bChannelAscendingSort(FALSE)
,m_byFocusIndex(INVALID_BYTE)
,m_byDownIndex(INVALID_BYTE)
,m_bySelectIndex(INVALID_BYTE)
,m_byMouseDownSlot(INVALID_BYTE)
{
}

CServerSelectGui::~CServerSelectGui()
{

}

RwBool CServerSelectGui::Create()
{
	NTL_FUNCTION( "CServerSelectGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\ServerSelect.srf", "gui\\ServerSelect.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	rect.SetRect(52, 6, 172, 26);
	m_pTitleStatic = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pTitleStatic->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pTitleStatic->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_CHOICE_SERVER"));

	// '서버를 선택하세요'
	rect.SetRect(195, 41, 395, 55);
	m_pSelectServer = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_RIGHT );
	m_pSelectServer->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pSelectServer->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_YOU_CAN_CHOICE_SERVER"));
	m_pSelectServer->Enable(false);

	// 서버 이름 정렬 버튼
	m_pServerButton = (gui::CButton*)GetComponent("btnServer");
	m_pServerButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pServerButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pServerButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER"));
	m_slotServerDown = m_pServerButton->SigClicked().Connect( this, &CServerSelectGui::OnClickedServerButton );

	// 캐릭터 수 정렬 버튼
	m_pCharButton = (gui::CButton*)GetComponent("btnChar");
	m_pCharButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pCharButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pCharButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_CHAR"));
	m_slotCharDown = m_pCharButton->SigClicked().Connect( this, &CServerSelectGui::OnClickedCharacterButton );

	// 접속자 수 정렬 버튼
	m_pStateButton = (gui::CButton*)GetComponent("btnState");
	m_pStateButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pStateButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pStateButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_STATE"));
	m_slotStateDown = m_pStateButton->SigClicked().Connect( this, &CServerSelectGui::OnClickedServerStateButton );

	// 스크롤
	m_pScrollBar = (gui::CScrollBar*)GetComponent("scbScroll");
	m_slotServerScrollChanged		= m_pScrollBar->SigValueChanged().Connect( this, &CServerSelectGui::OnScrollChanged );
	m_slotServerScrollSliderMoved	= m_pScrollBar->SigSliderMoved().Connect( this, &CServerSelectGui::OnScrollChanged );

	// 확인 버튼
	m_pOKButton = (gui::CButton*)GetComponent("btnOk");
	m_pOKButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pOKButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pOKButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_OK"));
	m_slotOKDown = m_pOKButton->SigClicked().Connect( this, &CServerSelectGui::OnClickedOKButton );

	// 취소 버튼
	m_pCancelButton = (gui::CButton*)GetComponent("btnCancel");
	m_pCancelButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pCancelButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pCancelButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_CANCLE2"));
	m_slotCancelDown = m_pCancelButton->SigClicked().Connect( this, &CServerSelectGui::OnClickedCancleButton );

	// 화살표
	m_srfServerArrowUp		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ServerSelect.srf", "srfUpArrow" ));
	m_srfServerArrowDown	.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ServerSelect.srf", "srfDownArrow" ));
	m_srfCharArrowUp		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ServerSelect.srf", "srfUpArrow" ));
	m_srfCharArrowDown		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ServerSelect.srf", "srfDownArrow" ));
	m_srfStateArrowUp		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ServerSelect.srf", "srfUpArrow" ));
	m_srfStateArrowDown		.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ServerSelect.srf", "srfDownArrow" ));
	m_srfFocusBar			.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ServerSelect.srf", "srfFocusBar" ));
	m_srfSelectBar			.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ServerSelect.srf", "srfSelectBar" ));
	m_srfDownBar			.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ServerSelect.srf", "srfDownBar" ));

	Show(false);

	LocateComponent(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

	// sig
	m_slotCaptureWheelMove	= GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CServerSelectGui::OnCaptureWheelMove );
	m_slotMouseDown			= m_pThis->SigMouseDown().Connect( this, &CServerSelectGui::OnMouseDown );
	m_slotMouseUp			= m_pThis->SigMouseUp().Connect( this, &CServerSelectGui::OnMouseUp );	
	m_slotMouseMove			= m_pThis->SigMouseMove().Connect( this, &CServerSelectGui::OnMouseMove );
	m_slotMouseLeave		= m_pThis->SigMouseLeave().Connect( this, &CServerSelectGui::OnMouseLeave );
	m_slotPostPaint			= m_pSelectServer->SigPaint().Connect( this, &CServerSelectGui::OnPostPaint );

	LinkMsg(g_EventCharStageStateEnter);
	LinkMsg(g_EventCharStageStateExit);
	LinkMsg(g_EventLobbyMessage);
	LinkMsg(g_EventResize);

	NTL_RETURN(TRUE);
}

VOID CServerSelectGui::Destroy()
{
	NTL_FUNCTION("CServerSelectGui::Destroy");

	UnLinkMsg(g_EventCharStageStateEnter);
	UnLinkMsg(g_EventCharStageStateExit);
	UnLinkMsg(g_EventLobbyMessage);
	UnLinkMsg(g_EventResize);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	SERVERITEM_ITER it = m_listServerItem.begin();
	for( ; it != m_listServerItem.end() ; ++it )
	{
		CServerItem* pServerItem = *it;
		NTL_DELETE(pServerItem);
	}
	m_listServerItem.clear();

	NTL_RETURNVOID();
}

VOID CServerSelectGui::SwitchDialog(bool bShow)
{
	Show(bShow);
}

VOID CServerSelectGui::CreateServerInfo()
{	
	ITER_LOBBY	it_Lobby		= GetLobbyManager()->GetLobbyBegin();
	ITER_LOBBY	it_Lobby_End	= GetLobbyManager()->GetLobbyEnd();

	RwInt32		iTextY		= dCHANNEL_TEXT_START_Y;
	RwInt32		iButtonY	= dSERVER_SLOT_START_Y;
	CRectangle	rect;

	for( RwUInt8 i = 0 ; it_Lobby != it_Lobby_End ; ++it_Lobby, ++i )
	{
		CServerItem* pServetItem = NTL_NEW CServerItem();
        m_listServerItem.push_back(pServetItem);

		CLobby* pLobby = it_Lobby->second;

		// 서버 아이디
		pServetItem->hServer = pLobby->GetServerHandle();

		// 서버 이름
		rect.SetRectWH(dCHANNEL_TEXT__SERVER_NAME_X, iTextY, 130, 14);
		pServetItem->pServerName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );	
		pServetItem->pServerName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		pServetItem->pServerName->SetText(pLobby->GetServerName());
		pServetItem->pServerName->Enable(false);

		// 캐릭터 수
		rect.SetRectWH(dCHANNEL_TEXT__SERVER_CHARNUM_X, iTextY, 40, 14);
		pServetItem->pCharCount = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );	
		pServetItem->pCharCount->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		pServetItem->pCharCount->SetText(pLobby->GetCharacterCount());
		pServetItem->pCharCount->Enable(false);

		// 서버 상태
		rect.SetRectWH(dCHANNEL_TEXT__SERVER_STATE_X, iTextY, 100, 14);
		pServetItem->pState = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );	
		pServetItem->pState->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);		
		pServetItem->pState->Enable(false);


		// 서버 리스트 정렬을 위한 데이터 저장
		size_t nameSize = sizeof(WCHAR) * (NTL_MAX_SIZE_SERVER_FARM_NAME_UNICODE + 1);
		memcpy_s(pServetItem->awcServerName, nameSize, pLobby->GetServerName(), nameSize);
		pServetItem->byCharacterCount	= pLobby->GetCharacterCount();
		pServetItem->uiUserRate			= pLobby->GetServerUseRate();

		// 서버 상태 표시
		SetState(pServetItem, pLobby);

		iTextY		+= dSERVER_HEIGHT_GAP;
		iButtonY	+= dSERVER_HEIGHT_BUTTON_GAP;
	}
}

VOID CServerSelectGui::DestroyServerInfo()
{
	SERVERITEM_ITER it = m_listServerItem.begin();
	for( ; it != m_listServerItem.end() ; ++it )
	{
		CServerItem* pServerItem = *it;

		NTL_DELETE(pServerItem->pServerName);
		NTL_DELETE(pServerItem->pCharCount);
		NTL_DELETE(pServerItem->pState);
		NTL_DELETE(pServerItem);
	}

	m_listServerItem.clear();
}

VOID CServerSelectGui::RefreshServerInfo(SERVER_HANDLE hServer)
{
	// 에잇! 지저분해라
	SERVERITEM_ITER it = m_listServerItem.begin();
	for( ; it != m_listServerItem.end() ; ++it )
	{
		CServerItem* pServerItem = *it;

		if( pServerItem->hServer == hServer )
		{
			CLobby* pLobby = GetLobbyManager()->GetLobby(hServer);
			if( pLobby )
			{
				pServerItem->pCharCount->SetText(pLobby->GetCharacterCount());
				SetState(pServerItem, pLobby);
			}
			
			break;
		}
	}
}

VOID CServerSelectGui::SetState(CServerItem* pServerItem, CLobby* pLobby)
{
	eDBO_SERVER_STATUS	eServerStatus	= pLobby->GetServerStatus();
	RwUInt32			uiUseRate		= pLobby->GetServerUseRate();

	switch(eServerStatus)
	{
	case DBO_SERVER_STATUS_UP:
		{
			if( uiUseRate < 50 )
			{
				pServerItem->pState->SetTextColor(RGB(94, 255, 89), TRUE);
				pServerItem->pState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_NOT_BUSY"));
			}
			else if( uiUseRate < 80 )
			{
				pServerItem->pState->SetTextColor(RGB(94, 255, 89), TRUE);
				pServerItem->pState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_NORMAL"));
			}
			else if( uiUseRate < 100 )
			{
				pServerItem->pState->SetTextColor(RGB(94, 255, 89), TRUE);
				pServerItem->pState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_BUSY"));
			}
			else
			{
				pServerItem->pState->SetTextColor(RGB(94, 255, 89), TRUE);
				pServerItem->pState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_EXCESS"));
			}

			break;
		}
	case DBO_SERVER_STATUS_DOWN:
		{
			pServerItem->pState->SetTextColor(RGB(212, 15, 45), TRUE);
			pServerItem->pState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_CHECK"));
			break;
		}
	case DBO_SERVER_STATUS_LOCKED:
		{
			pServerItem->pState->SetTextColor(RGB(23, 125, 212), TRUE);
			pServerItem->pState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_LOCK"));
			break;
		}
	default:
		{
			DBO_FAIL("Wrong server status");
			break;
		}
	}
}

VOID CServerSelectGui::LocateComponent(RwInt32 iWidth, RwInt32 iHeight)
{
	CRectangle rect;
	RwInt32 iXPos = (iWidth - m_pThis->GetWidth())/2;
	RwInt32 iYPos = (iHeight - m_pThis->GetHeight())/2;

	m_pThis->SetPosition(iXPos, iYPos);

	rect = m_pServerButton->GetPosition();
	m_srfServerArrowUp		.SetPosition(iXPos + rect.right - dARROW_X_ADJUST_FROM_BUTTON_RIGHT, iYPos + 79);
	m_srfServerArrowDown	.SetPosition(iXPos + rect.right - dARROW_X_ADJUST_FROM_BUTTON_RIGHT, iYPos + 79);

	rect = m_pCharButton->GetPosition();
	m_srfCharArrowUp		.SetPosition(iXPos + rect.right - dARROW_X_ADJUST_FROM_BUTTON_RIGHT, iYPos + 79);
	m_srfCharArrowDown		.SetPosition(iXPos + rect.right - dARROW_X_ADJUST_FROM_BUTTON_RIGHT, iYPos + 79);

	rect = m_pStateButton->GetPosition();
	m_srfStateArrowUp		.SetPosition(iXPos + rect.right - dARROW_X_ADJUST_FROM_BUTTON_RIGHT, iYPos + 79);
	m_srfStateArrowDown		.SetPosition(iXPos + rect.right - dARROW_X_ADJUST_FROM_BUTTON_RIGHT, iYPos + 79);

	UpdateList(m_pScrollBar->GetValue());

	CalcSelectBarPosition(m_bySelectIndex - (RwUInt8)m_pScrollBar->GetValue());
}

CServerSelectGui::CServerItem* CServerSelectGui::GetSelectedSetverItem()
{
	if( m_bySelectIndex == INVALID_BYTE )
		return NULL;

	RwInt8 byCount = 0;
	SERVERITEM_ITER it = m_listServerItem.begin();
	for( ; it != m_listServerItem.end() ; ++it, ++byCount )
	{
		if( byCount == m_bySelectIndex )
			return *it;
	}

	return NULL;
}

VOID CServerSelectGui::EnableButtons(bool bEnable)
{
	m_pSelectServer		->Enable(bEnable);
	m_pServerButton		->ClickEnable(bEnable);
	m_pCharButton		->ClickEnable(bEnable);
	m_pStateButton		->ClickEnable(bEnable);
	m_pOKButton			->ClickEnable(bEnable);
	m_pCancelButton		->ClickEnable(bEnable);
}

VOID CServerSelectGui::UpdateList(RwInt32 iOffset)
{
	if( m_listServerItem.size() <= dMAX_VISIBLE_COUNT )
	{
		m_pScrollBar->Show(false);
		iOffset = 0;
	}
	else
	{
		m_pScrollBar->Show(true);		
	}


	CRectangle rtScreen = m_pThis->GetScreenRect();
	RwInt32 iTextY = dCHANNEL_TEXT_START_Y;
	RwInt32 iButtonY = dSERVER_SLOT_START_Y;
	SERVERITEM_ITER it = m_listServerItem.begin();

	for( RwInt32 iCount = 0 ; it != m_listServerItem.end() ; ++it, ++iCount)
	{
		CServerItem* pServerItem = *it;

		if( iOffset <= iCount && iCount < iOffset + dMAX_VISIBLE_COUNT )
		{			
			pServerItem->pServerName->Show(true);
			pServerItem->pCharCount->Show(true);
			pServerItem->pState->Show(true);
			
			pServerItem->pServerName->SetPosition(dCHANNEL_TEXT__SERVER_NAME_X, iTextY);
			pServerItem->pCharCount->SetPosition(dCHANNEL_TEXT__SERVER_CHARNUM_X, iTextY);
			pServerItem->pState->SetPosition(dCHANNEL_TEXT__SERVER_STATE_X, iTextY);

			iTextY		+= dSERVER_HEIGHT_GAP;
			iButtonY	+= dSERVER_HEIGHT_BUTTON_GAP;
		}
		else
		{			
			pServerItem->pServerName->Show(false);
			pServerItem->pCharCount->Show(false);
			pServerItem->pState->Show(false);
		}
	}

	if( m_listServerItem.size() > dMAX_VISIBLE_COUNT )
		m_pScrollBar->SetMaxValue( m_listServerItem.size() - dMAX_VISIBLE_COUNT);

	m_pScrollBar->SetValue(iOffset);
}

RwUInt8 CServerSelectGui::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	RwInt32 iWidth = m_srfFocusBar.GetWidth();	

	for( RwUInt8 i = 0 ; i < dMAX_VISIBLE_COUNT ; ++i )
	{
		if( dSERVER_SLOT_X <= iX && iX <= iWidth )
		{
			if( (dSERVER_SLOT_START_Y + i*dSERVER_HEIGHT_BUTTON_GAP) <= iY &&
				iY <= (dSERVER_SLOT_START_Y + (i+1)*dSERVER_HEIGHT_BUTTON_GAP) )
				return i;
		}
	}

	return INVALID_BYTE;
}

RwUInt8 CServerSelectGui::Slot_to_Index(RwUInt8 bySlot)
{
	if( bySlot == INVALID_BYTE )
		return INVALID_BYTE;

	RwUInt8 byIndex = (RwUInt8)m_pScrollBar->GetValue() + bySlot;

	if( (RwUInt8)m_listServerItem.size() <= byIndex )
		return INVALID_BYTE;

	return byIndex;
}

VOID CServerSelectGui::FocusEffect(RwUInt8 bySlot)
{
	if( bySlot != INVALID_BYTE )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		RwInt32 iY = dSERVER_SLOT_START_Y + bySlot*dSERVER_HEIGHT_BUTTON_GAP;		

		m_srfFocusBar.SetRectWH(rtScreen.left + dSERVER_SLOT_X, rtScreen.top + iY, m_srfFocusBar.GetWidth(), m_srfFocusBar.GetHeight());

		m_byFocusIndex = (RwUInt8)m_pScrollBar->GetValue() + bySlot;
	}
	else
	{
		m_byFocusIndex = INVALID_BYTE;
	}
}

VOID CServerSelectGui::CalcDownBarPosition(RwUInt8 bySlot)
{
	if( bySlot != INVALID_BYTE )
	{
		RwUInt8 byIndex = Slot_to_Index(bySlot);
		if( byIndex != INVALID_BYTE )
		{
			if( byIndex >= 0 && byIndex > (RwUInt8)m_listServerItem.size() - (RwUInt8)m_pScrollBar->GetValue() - 1 )
			{
				m_byDownIndex = INVALID_BYTE;
				return;
			}

			CRectangle rtScreen = m_pThis->GetScreenRect();
			RwInt32 iY = dSERVER_SLOT_START_Y + bySlot*dSERVER_HEIGHT_BUTTON_GAP;

			m_srfDownBar.SetRectWH(rtScreen.left + dSERVER_SLOT_X, rtScreen.top + iY, m_srfFocusBar.GetWidth(), m_srfFocusBar.GetHeight());
			m_byDownIndex = byIndex;

			return;
		}
	}

	m_byDownIndex = INVALID_BYTE;
}

VOID CServerSelectGui::CalcSelectBarPosition(RwUInt8 bySlot)
{
	if( bySlot != INVALID_BYTE )
	{
		RwUInt8 byIndex = Slot_to_Index(bySlot);
		if( byIndex != INVALID_BYTE )
		{
			CRectangle rtScreen = m_pThis->GetScreenRect();
			RwInt32 iY = dSERVER_SLOT_START_Y + bySlot*dSERVER_HEIGHT_BUTTON_GAP;		

			m_srfSelectBar.SetRectWH(rtScreen.left + dSERVER_SLOT_X, rtScreen.top + iY, m_srfFocusBar.GetWidth(), m_srfFocusBar.GetHeight());
			m_bySelectIndex = byIndex;

			return;
		}
	}

	m_bySelectIndex = INVALID_BYTE;
}

VOID CServerSelectGui::OnScrollChanged(RwInt32 iOffset)
{
	UpdateList(iOffset);
}

VOID CServerSelectGui::OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos)
{
	if( !IsShow() )
		return;

	if( m_pThis->GetParent()->GetChildComponentReverseAt( pos.x, pos.y ) != m_pThis )
		return;

	if( m_pThis->PosInRect( pos.x, pos.y ) != gui::CComponent::INRECT )
		return;

	RwInt32 iValue = m_pScrollBar->GetValue();
	RwInt32 iMaxValue = m_pScrollBar->GetMaxValue();

	if( sDelta < 0 )
		++iValue;
	else
		--iValue;

	if( iValue < 0 )
		iValue = 0;
	else if( iValue > iMaxValue )
		iValue = iMaxValue;

	m_pScrollBar->SetValue( iValue );
}

VOID CServerSelectGui::OnClickedServerButton(gui::CComponent* pComponent)
{
	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_SERVER_IDLE )
		return;


	// 접속자 수에 따라 정렬한다
	if(m_bServerAscendingSort)
		std::stable_sort(m_listServerItem.begin(), m_listServerItem.end(), CServerListSort::CompareAscentName);
	else
		std::stable_sort(m_listServerItem.begin(), m_listServerItem.end(), CServerListSort::CompareDecentName);

	m_bServerAscendingSort = !m_bServerAscendingSort;

	RwUInt8 byScrollOffset = (RwUInt8)m_pScrollBar->GetValue();
	UpdateList(byScrollOffset);
}

VOID CServerSelectGui::OnClickedCharacterButton(gui::CComponent* pComponent)
{
	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_SERVER_IDLE )
		return;

	
	// 접속자 수에 따라 정렬한다
	if(m_bCharacterAscendingSort)
		std::stable_sort(m_listServerItem.begin(), m_listServerItem.end(), CServerListSort::CompareAscentCharacter);
	else
		std::stable_sort(m_listServerItem.begin(), m_listServerItem.end(), CServerListSort::CompareDecentCharacter);

	m_bCharacterAscendingSort = !m_bCharacterAscendingSort;

	RwInt32 iScrollOffset = (RwInt32)m_pScrollBar->GetValue();
	UpdateList(iScrollOffset);
}

VOID CServerSelectGui::OnClickedServerStateButton(gui::CComponent* pComponent)
{
	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_SERVER_IDLE )
		return;


	// 접속자 수에 따라 정렬한다
	if(m_bChannelAscendingSort)
		std::stable_sort(m_listServerItem.begin(), m_listServerItem.end(), CServerListSort::CompareAscentState);
	else
		std::stable_sort(m_listServerItem.begin(), m_listServerItem.end(), CServerListSort::CompareDecentState);

	m_bChannelAscendingSort = !m_bChannelAscendingSort;

	RwInt32 iScrollOffset = (RwInt32)m_pScrollBar->GetValue();
	UpdateList(iScrollOffset);
}

VOID CServerSelectGui::OnClickedOKButton(gui::CComponent* pComponent)
{
	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_SERVER_IDLE )
		return;


	CServerItem* pServerItem = GetSelectedSetverItem();

	if( pServerItem == NULL )
	{		
		// 서버를 선택하십시요
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_MUST_CHOICE_SERVER" );
		return;
	}

	if( GetLobbyManager()->GetLobbyCount() > 0 )
	{		
		CLobby* pLobby = GetLobbyManager()->GetLobby(pServerItem->hServer);

		// 셀렉트 서버 저장
		GetLobbyManager()->SetSelectedServerHandle(pServerItem->hServer);

		if(pLobby->GetCharacterCount() == 0 && pLobby->GetChannelCount() == 0)
		{
			// 캐릭터 정보를 요청한다. 다음의 2가지 패킷이 응답으로 온다
			// CU_SERVER_CHANNEL_INFO, CU_CHARACTER_LOAD_RES	
			SConnectData *pConData = GetDboGlobal()->GetConnectData();
			GetDboGlobal()->GetLobbyPacketGenerator()->SendCharLoadReq(pConData->uiAccountId,
														GetLobbyManager()->GetSelectedServerHandle());

			GetLobbyManager()->SetWaitChannelInfo(TRUE);

			GetCharStageState()->ChangeState(CHAR_STATE_SELECT_INFOMATION_FROM_SERVER_LIST);
		}
		else
		{	
			GetCharStageState()->ReservateState(CHAR_STATE_SELECT_ENTER);
			GetCharStageState()->ChangeState(CHAR_STATE_SERVER_EXIT);	
		}
	}
	else
	{
		GetCharStageState()->ReservateState(CHAR_STATE_SELECT_ENTER);
		GetCharStageState()->ChangeState(CHAR_STATE_SERVER_EXIT);	
	}	
}

VOID CServerSelectGui::OnClickedCancleButton(gui::CComponent* pComponent)
{
	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_SERVER_IDLE )
		return;

	// 신규 유저는 혹은 마지막으로 접속했던 서버가 사라져서
	// 게임 시작시 바로 서버 셀렉트 화면으로 왔다면 로그인으로 돌아간다
	if( GetLobbyManager()->GetSelectedServerHandle() == INVALID_SERVERFARMID )
	{
		GetDboGlobal()->GetLobbyPacketGenerator()->SendCharDisconnectReq(false);
		GetCharStageState()->ReservateState(CHAR_STATE_RETURN_LOGIN);
	}

	GetCharStageState()->ChangeState(CHAR_STATE_SERVER_EXIT);
}

VOID CServerSelectGui::OnMouseDown(const CKey& key)
{
	m_byMouseDownSlot = PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if( m_byMouseDownSlot == INVALID_BYTE )
		return;

	RwInt8 byIndex = Slot_to_Index(m_byMouseDownSlot);
	if( byIndex == INVALID_BYTE )
	{
		m_byMouseDownSlot = INVALID_BYTE;
		return;
	}

	if( byIndex >= 0 && byIndex > (RwUInt8)(m_listServerItem.size() - m_pScrollBar->GetValue() - 1) )
	{
		m_byMouseDownSlot = INVALID_BYTE;
		return;
	}

	CalcDownBarPosition(m_byMouseDownSlot);

	if( key.m_nState == CKey::DBLPRESSED )
	{
		CalcSelectBarPosition(m_byMouseDownSlot);		
		OnClickedOKButton(NULL);
		m_byDownIndex = INVALID_BYTE;
		m_byMouseDownSlot = INVALID_BYTE;
	}	
}

VOID CServerSelectGui::OnMouseUp(const CKey& key)
{
	CalcDownBarPosition(INVALID_BYTE);

	if( m_byMouseDownSlot == INVALID_BYTE )
		return;

	RwUInt8 m_bySlot = PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if( m_byMouseDownSlot != m_bySlot )
	{
		m_byMouseDownSlot = INVALID_BYTE;
		return;
	}

	CalcSelectBarPosition(m_byMouseDownSlot);

	m_byMouseDownSlot = INVALID_BYTE;
}

VOID CServerSelectGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	RwUInt8 byCurSlot = PtinSlot(nX, nY);

	FocusEffect(byCurSlot);
}

VOID CServerSelectGui::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(INVALID_BYTE);
	CalcDownBarPosition(INVALID_BYTE);
}

VOID CServerSelectGui::OnPostPaint()
{
	if( m_bServerAscendingSort )
		m_srfServerArrowUp.Render();
	else
		m_srfServerArrowDown.Render();

	if( m_bCharacterAscendingSort )
		m_srfCharArrowUp.Render();
	else
		m_srfCharArrowDown.Render();

	if( m_bChannelAscendingSort )
		m_srfStateArrowUp.Render();
	else
		m_srfStateArrowDown.Render();



	if( m_byDownIndex != INVALID_BYTE )
		m_srfDownBar.Render();

	if( m_bySelectIndex != INVALID_BYTE )
	{
		if( m_bySelectIndex != m_byDownIndex )
			m_srfSelectBar.Render();
	}

	if( m_byFocusIndex != INVALID_BYTE )
	{
		if( m_byFocusIndex != m_byDownIndex )
		{
			if( m_byFocusIndex != m_bySelectIndex )
			{
				m_srfFocusBar.Render();
			}
		}
	}	
}

VOID CServerSelectGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CServerSelectGui::HandleEvents");

	if( msg.Id == g_EventCharStageStateEnter )
	{
		SDboEventCharStageStateEnter* pEvent = reinterpret_cast<SDboEventCharStageStateEnter*>( msg.pData );

		if( pEvent->byState == CHAR_STATE_SERVER_IDLE )
			EnableButtons(true);
		else
			EnableButtons(false);

		if( pEvent->byState == CHAR_STATE_SERVER_ENTER )
		{
			SwitchDialog(true);
		}		
	}
	else if( msg.Id == g_EventCharStageStateExit )
	{
		SDboEventCharStageStateExit* pEvent = reinterpret_cast<SDboEventCharStageStateExit*>( msg.pData );
		if( pEvent->byState == CHAR_STATE_SERVER_EXIT )
		{
			// The new user or the last server to connect is gone
			// If you come to the server select screen right at the start of the game,
			if( GetLobbyManager()->GetSelectedServerHandle() == INVALID_SERVERFARMID )
				CDboEventGenerator::LobbyEvent(LMT_DESTORY_LOBBY_WORLD);

			SwitchDialog(false);
		}
	}
	else if( msg.Id == g_EventLobbyMessage )
	{
		SDboEventLobbyMessage* pEvent = reinterpret_cast<SDboEventLobbyMessage*>( msg.pData );
		switch(pEvent->byMessage)
		{
		case LMT_UPDATE_SERVER:
			{				
				DestroyServerInfo();
				CreateServerInfo();

				break;
			}
		case LMT_REFRESH_SERVER:
			{
				RefreshServerInfo((SERVER_HANDLE)pEvent->fValue);
				break;
			}
		}
	}
	else if(msg.Id == g_EventResize)
	{
		SNtlPLEventResize* pEvent = reinterpret_cast<SNtlPLEventResize*>( msg.pData );		
		LocateComponent(pEvent->iWidht, pEvent->iHeight);	
	}

	NTL_RETURNVOID();
}