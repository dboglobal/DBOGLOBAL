#include "precomp_dboclient.h"
#include "ChannelChangeGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"
#include "DialogManager.h"
#include "LobbyManager.h"


#define dCHANNEL_HEIGHT_BUTTON_GAP		23
#define dCHANNEL_HEIGHT_GAP				23

#define dCHANNEL_SLOT_X					21
#define dCHANNEL_SLOT_START_Y			109
#define dCHANNEL_TEXT__CHANNEL_NUM_X	32
#define dCHANNEL_TEXT__CHANNEL_NUM_ADJUST_Y		(-4)
#define dCHANNEL_TEXT__CHANNEL_STATE_X	120
#define dCHANNEL_TEXT_START_Y			112
#define dCHANNEL_GAUGE_X				108
#define dCHANNEL_GAUGE_START_Y			117
#define dCHANNEL_GAUGE_BACK_X			108
#define dCHANNEL_GAUGE_BACK_START_Y		117

#define dMAX_VISIBLE_COUNT		9


CChannelChangeGui::CChannelChangeGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_byFocusIndex(INVALID_BYTE)
,m_byDownIndex(INVALID_BYTE)
,m_bySelectIndex(INVALID_BYTE)
,m_byMouseDownSlot(INVALID_BYTE)
{
}

CChannelChangeGui::~CChannelChangeGui()
{

}

RwBool CChannelChangeGui::Create()
{
	NTL_FUNCTION( "CChannelChangeGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\ChannelChange.srf", "gui\\ChannelChange.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority( dDIALOGPRIORITY_CHANNEL_CHANGE );

	CRectangle rect;

	rect.SetRectWH(DBOGUI_DIALOG_TITLE_X, DBOGUI_DIALOG_TITLE_Y, 145, 14);
	m_pDialogName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT );
	m_pDialogName->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pDialogName->SetText(GetDisplayStringManager()->GetString("DST_MAINMENU_CHANNEL_CHANGE"));
	m_pDialogName->Enable(false);

	m_pExitButton = (gui::CButton*)GetComponent("ExitButton");
	m_slotCloseButton = m_pExitButton->SigClicked().Connect( this, &CChannelChangeGui::OnClickCancleButton );

	// '채널선택'
	rect.SetRectWH(145, 40, 83, 14);
	m_pChannelSelect = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pChannelSelect->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pChannelSelect->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_CHOICE_CHANNEL"));
	m_pChannelSelect->Enable(false);

	// 서버 이름
	rect.SetRectWH(30, 77, 186, 14);
	m_pServerlName = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
	m_pServerlName->CreateFontStd( DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pServerlName->SetTextColor(RGB(255, 192, 0));
	m_pServerlName->Enable(false);

	// 스크롤
	m_pScrollBar = (gui::CScrollBar*)GetComponent("scbScroll");
	m_slotServerScrollChanged		= m_pScrollBar->SigValueChanged().Connect( this, &CChannelChangeGui::OnScrollChanged );
	m_slotServerScrollSliderMoved	= m_pScrollBar->SigSliderMoved().Connect( this, &CChannelChangeGui::OnScrollChanged );

	// 확인 버튼
	m_pOkButton = (gui::CButton*)GetComponent("btnOk");
	m_pOkButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pOkButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pOkButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_OK"));
	m_slotOKButton = m_pOkButton->SigClicked().Connect( this, &CChannelChangeGui::OnClickOKButton );

	// 취소 버튼
	m_pCancelButton = (gui::CButton*)GetComponent("btnCancel");
	m_pCancelButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pCancelButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pCancelButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_CANCLE2"));
	m_slotCancelButton = m_pCancelButton->SigClicked().Connect( this, &CChannelChangeGui::OnClickCancleButton );

	m_srfFocusBar.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ChannelChange.srf", "srfFocusBar" ));
	m_srfSelectBar.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ChannelChange.srf", "srfSelectBar" ));
	m_srfDownBar.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ChannelChange.srf", "srfDownBar" ));

	
	CreateChannels();
	SetServerName();

	Show(false);
	
	// sig	
	m_slotCaptureWheelMove	= GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CChannelChangeGui::OnCaptureWheelMove );
	m_slotMouseDown			= m_pThis->SigMouseDown().Connect( this, &CChannelChangeGui::OnMouseDown );
	m_slotMouseUp			= m_pThis->SigMouseUp().Connect( this, &CChannelChangeGui::OnMouseUp );	
	m_slotMove				= m_pThis->SigMove().Connect( this, &CChannelChangeGui::OnMove );	
	m_slotMouseMove			= m_pThis->SigMouseMove().Connect( this, &CChannelChangeGui::OnMouseMove );
	m_slotMouseLeave		= m_pThis->SigMouseLeave().Connect( this, &CChannelChangeGui::OnMouseLeave );
	m_slotPaint				= m_pThis->SigPaint().Connect( this, &CChannelChangeGui::OnPaint );

	LinkMsg(g_EventGameServerChannelChanged);

	NTL_RETURN(TRUE);
}

VOID CChannelChangeGui::Destroy()
{
	NTL_FUNCTION("CChannelChangeGui::Destroy");

	CHANNEL_ITER it = m_listChannel.begin();
	for( ; it != m_listChannel.end() ; ++it )
	{
		sChannelGui* pChannelGui = *it;
		NTL_DELETE(pChannelGui);
	}
	m_listChannel.clear();

	UnLinkMsg(g_EventGameServerChannelChanged);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CChannelChangeGui::CreateChannels()
{
	RwInt32		iButtonY		= dCHANNEL_SLOT_START_Y;
	RwInt32		iTextY			= dCHANNEL_TEXT_START_Y;
	RwInt32		iGaugeY			= dCHANNEL_GAUGE_START_Y;
	WCHAR		awcBuffer[64]	= L"";
	CRectangle	rect;

	SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		return;
	}

	for( RwUInt8 i = 0 ; i < pLobby->GetChannelCount() ; ++i )
	{
		sChannelGui* pChannelGui = NTL_NEW sChannelGui;
        m_listChannel.push_back(pChannelGui);

		sDBO_GAME_SERVER_CHANNEL_INFO* pDBO_GAME_SERVER_CHANNEL_INFO = pLobby->GetChannel(i);
		if( !pDBO_GAME_SERVER_CHANNEL_INFO )
			continue;

		// Channel ID
		pChannelGui->byChannelID = pDBO_GAME_SERVER_CHANNEL_INFO->byServerChannelIndex;

		// Channel Usage Gauge
		rect.SetRectWH(dCHANNEL_GAUGE_X, iGaugeY, 110, 9);
		pChannelGui->pUseGauge =  NTL_NEW gui::CProgressBar( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ChannelChange.srf", "srfGauge" )  );
		pChannelGui->pUseGauge->Enable(false);
		pChannelGui->pUseGauge->SetRange(0, pDBO_GAME_SERVER_CHANNEL_INFO->dwMaxLoad);
		pChannelGui->pUseGauge->SetPos(pDBO_GAME_SERVER_CHANNEL_INFO->dwLoad);

		// Channel number
		rect.SetRectWH(dCHANNEL_TEXT__CHANNEL_NUM_X, iTextY + dCHANNEL_TEXT__CHANNEL_NUM_ADJUST_Y, 70, 25);
		pChannelGui->pChannelNum = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );	
		pChannelGui->pChannelNum->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		swprintf_s(awcBuffer, 64, L"%s %d", GetDisplayStringManager()->GetString("DST_LOBBY_CHANNEL"), i + 1); 
		pChannelGui->pChannelNum->SetTextColor(RGB(255, 192, 0));
		pChannelGui->pChannelNum->SetText(awcBuffer);
		pChannelGui->pChannelNum->Enable(false);

		// Channel status
		rect.SetRectWH(dCHANNEL_TEXT__CHANNEL_STATE_X, iTextY, 65, 14);
		pChannelGui->pChannelState = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );	
		pChannelGui->pChannelState->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		SetChannelState(pChannelGui, pDBO_GAME_SERVER_CHANNEL_INFO);
		pChannelGui->pChannelState->Enable(false);

		// Channel usage gauge background
		pChannelGui->srfGaugeBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "ChannelChange.srf", "srfGaugeBack" ));

		iButtonY	+= dCHANNEL_HEIGHT_BUTTON_GAP;
		iTextY		+= dCHANNEL_HEIGHT_GAP;
		iGaugeY		+= dCHANNEL_HEIGHT_GAP;
	}

	UpdateList(pLobby->GetSelectedChannelIndex());
	CalcSelectBarPosition(pLobby->GetSelectedChannelIndex());
}

VOID CChannelChangeGui::DestroyChannels()
{
	CHANNEL_ITER it = m_listChannel.begin();
	for( ; it != m_listChannel.end() ; ++it )
	{
		sChannelGui* pChannelGui = *it;

		NTL_DELETE(pChannelGui->pUseGauge);
		NTL_DELETE(pChannelGui->pChannelNum);
		NTL_DELETE(pChannelGui->pChannelState);
	}
	m_listChannel.clear();
}

VOID CChannelChangeGui::RefreshChannel(sDBO_GAME_SERVER_CHANNEL_INFO* pChannelInfo)
{
	CHANNEL_ITER it = m_listChannel.begin();
	for( ; it != m_listChannel.end() ; ++it )
	{
		sChannelGui* pChannelGui = *it;

		if( pChannelGui->byChannelID == pChannelInfo->byServerChannelIndex )
		{
			pChannelGui->pUseGauge->SetRange(0, pChannelInfo->dwMaxLoad);
			pChannelGui->pUseGauge->SetPos(pChannelInfo->dwLoad);
			SetChannelState(pChannelGui, pChannelInfo);
			break;
		}
	}
}

VOID CChannelChangeGui::SetServerName()
{
	SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		return;
	}

	m_pServerlName->Format(L"%s  %s %d", pLobby->GetServerName(),
										 GetDisplayStringManager()->GetString("DST_LOBBY_CHANNEL"),
										 pLobby->GetSelectedChannelIndex() + 1);
}

RwUInt8 CChannelChangeGui::GetSelectedChannelIndex()
{
	return m_bySelectIndex;
}

VOID CChannelChangeGui::SetChannelState(sChannelGui* pChannelGui, sDBO_GAME_SERVER_CHANNEL_INFO* pChannelInfo)
{
	if( pChannelInfo->byServerStatus == DBO_SERVER_STATUS_UP )
	{
		if( pChannelInfo->dwLoad < 50 )
		{
			pChannelGui->pChannelState->SetTextColor(RGB(94, 255, 89), TRUE);
			pChannelGui->pChannelState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_NOT_BUSY"));
		}
		else if( pChannelInfo->dwLoad < 80 )
		{
			pChannelGui->pChannelState->SetTextColor(RGB(94, 255, 89), TRUE);
			pChannelGui->pChannelState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_NORMAL"));
		}
		else if( pChannelInfo->dwLoad < 100 )
		{
			pChannelGui->pChannelState->SetTextColor(RGB(94, 255, 89), TRUE);
			pChannelGui->pChannelState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_BUSY"));
		}
		else if( pChannelInfo->dwLoad >= 100 )
		{
			pChannelGui->pChannelState->SetTextColor(RGB(94, 255, 89), TRUE);
			pChannelGui->pChannelState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_EXCESS"));
		}
		else
		{
			pChannelGui->pChannelState->Clear();
		}

		pChannelGui->pUseGauge->SetPos(pChannelInfo->dwLoad);
	}
	else if( pChannelInfo->byServerStatus == DBO_SERVER_STATUS_DOWN )
	{
		pChannelGui->pUseGauge->SetPos(0);
		pChannelGui->pChannelState->SetTextColor(RGB(212, 15, 45), TRUE);
		pChannelGui->pChannelState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_CHECK"));
	}
	else if( pChannelInfo->byServerStatus == DBO_SERVER_STATUS_LOCKED )
	{
		pChannelGui->pUseGauge->SetPos(pChannelInfo->dwLoad);
		pChannelGui->pChannelState->SetTextColor(RGB(23, 125, 212), TRUE);
		pChannelGui->pChannelState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_LOCK"));
	}
}

VOID CChannelChangeGui::UpdateList(RwInt32 iOffset)
{
	if( m_listChannel.size() <= dMAX_VISIBLE_COUNT )
	{
		m_pScrollBar->Show(false);
		iOffset = 0;
	}
	else
	{
		m_pScrollBar->Show(true);		
	}


	CRectangle rtScreen = m_pThis->GetScreenRect();
	RwInt32 iButtonY = dCHANNEL_SLOT_START_Y;
	RwInt32 iTextY = dCHANNEL_TEXT_START_Y;
	RwInt32 iGaugeY = dCHANNEL_GAUGE_START_Y;
	RwInt32 iGaugeBackY = rtScreen.top + dCHANNEL_GAUGE_BACK_START_Y;
	CHANNEL_ITER it = m_listChannel.begin();

	for( RwInt32 iCount = 0 ; it != m_listChannel.end() ; ++it, ++iCount)
	{
		sChannelGui* pChannelGui = *it;

		if( iOffset <= iCount && iCount < iOffset + dMAX_VISIBLE_COUNT )
		{
			pChannelGui->pUseGauge->Show(true);
			pChannelGui->pChannelNum->Show(true);
			pChannelGui->pChannelState->Show(true);

			pChannelGui->pUseGauge->SetPosition(dCHANNEL_GAUGE_X, iGaugeY);			
			pChannelGui->pChannelNum->SetPosition(dCHANNEL_TEXT__CHANNEL_NUM_X, iTextY + dCHANNEL_TEXT__CHANNEL_NUM_ADJUST_Y);
			pChannelGui->pChannelState->SetPosition(dCHANNEL_TEXT__CHANNEL_STATE_X, iTextY);
			pChannelGui->srfGaugeBack.SetPosition(rtScreen.left + dCHANNEL_GAUGE_BACK_X, iGaugeBackY);

			iButtonY	+= dCHANNEL_HEIGHT_BUTTON_GAP;
			iTextY		+= dCHANNEL_HEIGHT_GAP;
			iGaugeY		+= dCHANNEL_HEIGHT_GAP;
			iGaugeBackY	+= dCHANNEL_HEIGHT_GAP;
		}
		else
		{
			pChannelGui->pUseGauge->Show(false);
			pChannelGui->pChannelNum->Show(false);
			pChannelGui->pChannelState->Show(false);
		}
	}

	if( m_listChannel.size() > dMAX_VISIBLE_COUNT )
		m_pScrollBar->SetMaxValue( m_listChannel.size() - dMAX_VISIBLE_COUNT);

	m_pScrollBar->SetValue(iOffset);
}

VOID CChannelChangeGui::FocusEffect(RwUInt8 bySlot)
{
	if( bySlot != INVALID_BYTE )
	{
		CRectangle rtScreen = m_pThis->GetScreenRect();
		RwInt32 iY = dCHANNEL_SLOT_START_Y + bySlot*dCHANNEL_HEIGHT_BUTTON_GAP;		

		m_srfFocusBar.SetRectWH(rtScreen.left + dCHANNEL_SLOT_X, rtScreen.top + iY, m_srfFocusBar.GetWidth(), m_srfFocusBar.GetHeight());

		m_byFocusIndex = (RwUInt8)m_pScrollBar->GetValue() + bySlot;
	}
	else
	{
		m_byFocusIndex = INVALID_BYTE;
	}
}

VOID CChannelChangeGui::CalcDownBarPosition(RwUInt8 bySlot)
{
	if( bySlot != INVALID_BYTE )
	{
		RwUInt8 byIndex = Slot_to_Index(bySlot);
		if( byIndex != INVALID_BYTE )
		{
			if( byIndex >= 0 && byIndex > (RwUInt8)m_listChannel.size() - (RwUInt8)m_pScrollBar->GetValue() - 1 )
			{
				m_byDownIndex = INVALID_BYTE;
				return;
			}

			CRectangle rtScreen = m_pThis->GetScreenRect();
			RwInt32 iY = dCHANNEL_SLOT_START_Y + bySlot*dCHANNEL_HEIGHT_BUTTON_GAP;		

			m_srfDownBar.SetRectWH(rtScreen.left + dCHANNEL_SLOT_X, rtScreen.top + iY, m_srfFocusBar.GetWidth(), m_srfFocusBar.GetHeight());
			m_byDownIndex = byIndex;

			return;
		}
	}

	m_byDownIndex = INVALID_BYTE;
}

VOID CChannelChangeGui::CalcSelectBarPosition(RwUInt8 bySlot)
{
	if( bySlot != INVALID_BYTE )
	{
		RwUInt8 byIndex = Slot_to_Index(bySlot);
		if( byIndex != INVALID_BYTE )
		{
			CRectangle rtScreen = m_pThis->GetScreenRect();
			RwInt32 iY = dCHANNEL_SLOT_START_Y + bySlot*dCHANNEL_HEIGHT_BUTTON_GAP;		

			m_srfSelectBar.SetRectWH(rtScreen.left + dCHANNEL_SLOT_X, rtScreen.top + iY, m_srfFocusBar.GetWidth(), m_srfFocusBar.GetHeight());
			m_bySelectIndex = byIndex;

			return;
		}
		
	}

	m_bySelectIndex = INVALID_BYTE;
}

VOID CChannelChangeGui::OnScrollChanged(RwInt32 iOffset)
{
	UpdateList(iOffset);
}

VOID CChannelChangeGui::OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos)
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

VOID CChannelChangeGui::OnClickOKButton(gui::CComponent* pComponent)
{
	if( m_bySelectIndex == INVALID_BYTE )
	{
		// 채널을 선택하십시요
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_MUST_CHOICE_CHANNEL" );
		return;
	}

	GetDialogManager()->CloseDialog(DIALOG_CHANNGEL_CHANGE);

	sMsgBoxData data;
	data.byIndex = m_bySelectIndex;

	// 채널을 변경하시겠습니까?
	GetAlarmManager()->AlarmMessage( "DST_ASK_CHANGE_CHANNEL", FALSE, &data );
}

VOID CChannelChangeGui::OnClickCancleButton(gui::CComponent* pComponent)
{
	GetDialogManager()->CloseDialog(DIALOG_CHANNGEL_CHANGE);
}

RwUInt8 CChannelChangeGui::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	RwInt32 iWidth = m_srfFocusBar.GetWidth();

	for( RwUInt8 i = 0 ; i < dMAX_VISIBLE_COUNT ; ++i )
	{
		if( dCHANNEL_SLOT_X <= iX && iX <= iWidth )
		{
			if( (dCHANNEL_SLOT_START_Y + i*dCHANNEL_HEIGHT_BUTTON_GAP) <= iY &&
				iY <= (dCHANNEL_SLOT_START_Y + (i+1)*dCHANNEL_HEIGHT_BUTTON_GAP) )
				return i;
		}
	}

	return INVALID_BYTE;
}

RwUInt8 CChannelChangeGui::Slot_to_Index(RwUInt8 bySlot)
{
	if( bySlot == INVALID_BYTE )
		return INVALID_BYTE;

	RwUInt8 byIndex = (RwUInt8)m_pScrollBar->GetValue() + bySlot;

	if( (RwUInt8)m_listChannel.size() <= byIndex )
		return INVALID_BYTE;

	return byIndex;
}

VOID CChannelChangeGui::OnMouseDown(const CKey& key)
{
	m_byMouseDownSlot = PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if( m_byMouseDownSlot == INVALID_BYTE )
		return;

	RwUInt8 byIndex = Slot_to_Index(m_byMouseDownSlot);
	if( byIndex == INVALID_BYTE )
	{
		m_byMouseDownSlot = INVALID_BYTE;
		return;
	}

	if( byIndex >= 0 && byIndex > (RwUInt8)m_listChannel.size() - (RwUInt8)m_pScrollBar->GetValue() - 1 )
	{
		m_byMouseDownSlot = INVALID_BYTE;
		return;
	}

	CalcDownBarPosition(m_byMouseDownSlot);

	if( key.m_nState == CKey::DBLPRESSED )
	{
		CalcSelectBarPosition(m_byMouseDownSlot);
		OnClickOKButton(NULL);
		m_byDownIndex = INVALID_BYTE;
		m_byMouseDownSlot = INVALID_BYTE;
	}	
}

VOID CChannelChangeGui::OnMouseUp(const CKey& key)
{
	CalcDownBarPosition(INVALID_BYTE);

	if( m_byMouseDownSlot == INVALID_BYTE )
		return;

	RwInt8 m_bySlot = PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if( m_byMouseDownSlot != m_bySlot )
	{
		m_byMouseDownSlot = INVALID_BYTE;
		return;
	}

	CalcSelectBarPosition(m_byMouseDownSlot);

	m_byMouseDownSlot = INVALID_BYTE;
}

VOID CChannelChangeGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	UpdateList(m_pScrollBar->GetValue());
	CalcSelectBarPosition(m_bySelectIndex);
}

VOID CChannelChangeGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	RwInt8 byCurSlot = PtinSlot(nX, nY);

	FocusEffect(byCurSlot);
}

VOID CChannelChangeGui::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(INVALID_BYTE);
	CalcDownBarPosition(INVALID_BYTE);
}

VOID CChannelChangeGui::OnPaint()
{
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

	CHANNEL_ITER it = m_listChannel.begin();
	for( ; it != m_listChannel.end() ; ++it )
	{
		if( (*it)->pChannelNum->IsVisible() )
			(*it)->srfGaugeBack.Render();
	}
}

RwInt32 CChannelChangeGui::SwitchDialog(bool bOpen)
{
	Show(bOpen);
	return 1;
}

VOID CChannelChangeGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CChannelChangeGui::HandleEvents");

	if( msg.Id == g_EventGameServerChannelChanged )
	{
		SetServerName();
	}

	NTL_RETURNVOID();
}