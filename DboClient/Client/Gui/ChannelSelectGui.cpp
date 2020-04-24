#include "precomp_dboclient.h"
#include "ChannelSelectGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "CharStageState.h"
#include "DboGlobal.h"
#include "DisplayStringManager.h"
#include "DialogDefine.h"
#include "AlarmManager.h"
#include "LobbyManager.h"


#define dCHANNEL_HEIGHT_BUTTON_GAP		23
#define dCHANNEL_HEIGHT_GAP				23

#define dCHANNEL_SLOT_X					21
#define dCHANNEL_SLOT_START_Y			109
#define dCHANNEL_TEXT__CHANNEL_NUM_X	32
#define dCHANNEL_TEXT__CHANNEL_NUM_ADJUST_Y		(-4)
#define dCHANNEL_TEXT__CHANNEL_STATE_X	139
#define dCHANNEL_TEXT_START_Y			112
#define dCHANNEL_GAUGE_X				139
#define dCHANNEL_GAUGE_START_Y			117

#define dMAX_VISIBLE_COUNT		9


CChannelSelectGui::CChannelSelectGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_byFocusIndex(INVALID_BYTE)
,m_byDownIndex(INVALID_BYTE)
,m_bySelectIndex(INVALID_BYTE)
,m_byMouseDownSlot(INVALID_BYTE)
{
}

CChannelSelectGui::~CChannelSelectGui()
{

}

RwBool CChannelSelectGui::Create()
{
	NTL_FUNCTION( "CChannelSelectGui::Create" );

	if(!CNtlPLGui::Create("", "gui\\CharChannelSelect.srf", "gui\\CharChannelSelect.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;

	rect.SetRect(52, 6, 172, 26);
	m_pSelectChannel = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_LEFT);
	m_pSelectChannel->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pSelectChannel->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_CHOICE_CHANNEL"));
	m_pSelectChannel->Enable(false);


	rect.SetRect(33, 77, 213, 91);
	m_pServerName = NTL_NEW gui::CStaticBox(rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER);
	m_pServerName->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
	m_pServerName->SetTextColor(RGB(255, 192, 0));
	m_pServerName->Enable(false);

	// 스크롤
	m_pScrollBar = (gui::CScrollBar*)GetComponent("scbScroll");
	m_slotServerScrollChanged		= m_pScrollBar->SigValueChanged().Connect( this, &CChannelSelectGui::OnScrollChanged );
	m_slotServerScrollSliderMoved	= m_pScrollBar->SigSliderMoved().Connect( this, &CChannelSelectGui::OnScrollChanged );

	// 확인 버튼
	m_pOkButton = (gui::CButton*)GetComponent("btnOk");
	m_pOkButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pOkButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pOkButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_OK"));
	m_slotOKButton = m_pOkButton->SigClicked().Connect( this, &CChannelSelectGui::OnClickOKButton );

	// 취소 버튼
	m_pCancelButton = (gui::CButton*)GetComponent("btnCancel");
	m_pCancelButton->SetTextFocusColor(INFOCOLOR_LOBBY_FOC);
	m_pCancelButton->SetTextDownColor(INFOCOLOR_LOBBY_DOWN);
	m_pCancelButton->SetClickSound("System\\SYSTEM_CANCEL_LT.wav");
	m_pCancelButton->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_CANCLE2"));
	m_slotCancelButton = m_pCancelButton->SigClicked().Connect( this, &CChannelSelectGui::OnClickCancleButton );

	m_srfFocusBar.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharChannelSelect.srf", "srfFocusBar" ));
	m_srfSelectBar.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharChannelSelect.srf", "srfSelectBar" ));
	m_srfDownBar.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharChannelSelect.srf", "srfDownBar" ));

	// dafault set
	// avooo's comment : 마지막에 접속했던 서버가 없는 상태에서 게임에 들어갔다가 로비로 나오면 여전히 마지막에 접속했던 서버의 인덱스가 없다.
	//				     이 때 GUI상에 채널 정보가 표시되지 않는다. 그러나, 실제로는 채널 정보를 가지고 있으므로 GUI 채널 정보를
	//					 구축하자.(마지막에 접속했던 서버의 인덱스는 인증 서버 접속시, 캐릭터 서버 접속시 서버로 부터 받는다.)
	SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);

	if( pLobby && pLobby->GetChannelCount() > 0 )
		CreateChannels();


	Show(false);
	
	LinkMsg(g_EventCharStageStateEnter);
	LinkMsg(g_EventLobbyMessage);

	// sig	
	m_slotCaptureWheelMove	= GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CChannelSelectGui::OnCaptureWheelMove );
	m_slotMouseDown			= m_pThis->SigMouseDown().Connect( this, &CChannelSelectGui::OnMouseDown );
	m_slotMouseUp			= m_pThis->SigMouseUp().Connect( this, &CChannelSelectGui::OnMouseUp );	
	m_slotMouseMove			= m_pThis->SigMouseMove().Connect( this, &CChannelSelectGui::OnMouseMove );
	m_slotMouseLeave		= m_pThis->SigMouseLeave().Connect( this, &CChannelSelectGui::OnMouseLeave );
	m_slotPaint				= m_pThis->SigPaint().Connect( this, &CChannelSelectGui::OnPaint );

	NTL_RETURN(TRUE);
}

VOID CChannelSelectGui::Destroy()
{
	NTL_FUNCTION("CChannelSelectGui::Destroy");

	UnLinkMsg(g_EventCharStageStateEnter);
	UnLinkMsg(g_EventLobbyMessage);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	CHANNEL_ITER it = m_listChannel.begin();
	for( ; it != m_listChannel.end() ; ++it )
	{
		sChannelGui* pChannelGui = *it;
		NTL_DELETE(pChannelGui);
	}
	m_listChannel.clear();

	NTL_RETURNVOID();
}

VOID CChannelSelectGui::CreateChannels()
{
	RwInt32			iButtonY		= dCHANNEL_SLOT_START_Y;
	RwInt32			iTextY			= dCHANNEL_TEXT_START_Y;
	RwInt32			iGaugeY			= dCHANNEL_GAUGE_START_Y;
	WCHAR			awcBuffer[64]	= L"";
	CRectangle		rect;
	SERVER_HANDLE	hServer			= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby			= GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		return;
	}

	for( RwUInt8 i = 0 ; i < pLobby->GetChannelCount() ; ++i )
	{
		sChannelGui* pChannelGui = NTL_NEW sChannelGui();
        m_listChannel.push_back(pChannelGui);

		sDBO_GAME_SERVER_CHANNEL_INFO* pDBO_GAME_SERVER_CHANNEL_INFO = pLobby->GetChannel(i);
		if( !pDBO_GAME_SERVER_CHANNEL_INFO )
		{
			DBO_FAIL("Not exist channel of index : " << i);
			continue;
		}

		// 채널 ID
		pChannelGui->byChannelID = pDBO_GAME_SERVER_CHANNEL_INFO->byServerChannelIndex;

		// 채널 사용 게이지
		rect.SetRectWH(dCHANNEL_GAUGE_X, iGaugeY, 68, 8);
		pChannelGui->pUseGauge =  NTL_NEW gui::CProgressBar( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharChannelSelect.srf", "srfGauge" )  );
		pChannelGui->pUseGauge->Enable(false);
		pChannelGui->pUseGauge->SetRange(0, pDBO_GAME_SERVER_CHANNEL_INFO->dwMaxLoad);
		pChannelGui->pUseGauge->SetPos(pDBO_GAME_SERVER_CHANNEL_INFO->dwLoad);

		// 채널 번호
		rect.SetRectWH(dCHANNEL_TEXT__CHANNEL_NUM_X, iTextY + dCHANNEL_TEXT__CHANNEL_NUM_ADJUST_Y, 100, 25);
		pChannelGui->pChannelNum = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );	
		pChannelGui->pChannelNum->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		swprintf_s(awcBuffer, 64, L"%s %d", GetDisplayStringManager()->GetString("DST_LOBBY_CHANNEL"), i+1);
		pChannelGui->pChannelNum->SetTextColor(RGB(255, 192, 0));
		pChannelGui->pChannelNum->SetText(awcBuffer);
		pChannelGui->pChannelNum->Enable(false);

		// 채널 상태
		rect.SetRectWH(dCHANNEL_TEXT__CHANNEL_STATE_X, iTextY, 68, 14);
		pChannelGui->pChannelState = NTL_NEW gui::CStaticBox( rect, m_pThis, GetNtlGuiManager()->GetSurfaceManager(), COMP_TEXT_CENTER );
		pChannelGui->pChannelState->CreateFontStd(DEFAULT_FONT, DEFAULT_FONT_SIZE, DEFAULT_FONT_ATTR);
		SetChannelState(pChannelGui, pDBO_GAME_SERVER_CHANNEL_INFO);
		pChannelGui->pChannelState->Enable(false);

		// 채널 사용 게이지 배경
		pChannelGui->srfGaugeBack.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CharChannelSelect.srf", "srfGaugeBack" ));

		iButtonY	+= dCHANNEL_HEIGHT_BUTTON_GAP;
		iTextY		+= dCHANNEL_HEIGHT_GAP;
		iGaugeY		+= dCHANNEL_HEIGHT_GAP;

		
	}

	UpdateList(pLobby->GetSelectedChannelIndex());
}

VOID CChannelSelectGui::DestroyChannels()
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

VOID CChannelSelectGui::RefreshChannel(sDBO_GAME_SERVER_CHANNEL_INFO* pChannelInfo)
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

RwUInt8 CChannelSelectGui::GetSelectedChannelIndex()
{
	return m_bySelectIndex;
}

VOID CChannelSelectGui::SetAlpha(RwUInt8 byAlpha)
{

}

VOID CChannelSelectGui::SetPosition(RwInt32 iXPos, RwInt32 iYPos)
{
	m_pThis->SetPosition(iXPos, iYPos);

	UpdateList(m_pScrollBar->GetValue());
}

VOID CChannelSelectGui::EnableButtons(bool bEnable)
{
	m_pOkButton		->ClickEnable(bEnable);
	m_pCancelButton	->ClickEnable(bEnable);
	m_pScrollBar	->Enable(bEnable);
}

VOID CChannelSelectGui::SetServerInfo()
{
	SERVER_HANDLE	hServer = GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby = GetLobbyManager()->GetLobby(hServer);
	if (!pLobby)
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		return;
	}

	m_pServerName->Format(L"%s", pLobby->GetServerName());
}

VOID CChannelSelectGui::SetChannelState(sChannelGui* pChannelGui, sDBO_GAME_SERVER_CHANNEL_INFO* pChannelInfo)
{
	switch( pChannelInfo->byServerStatus )
	{
	case DBO_SERVER_STATUS_UP:
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

			pChannelGui->pUseGauge->SetPos(pChannelInfo->dwLoad);

			break;
		}
	case DBO_SERVER_STATUS_DOWN:
		{
			pChannelGui->pUseGauge->SetPos(0);
			pChannelGui->pChannelState->SetTextColor(RGB(212, 15, 45), TRUE);
			pChannelGui->pChannelState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_CHECK"));

			break;
		}
	case DBO_SERVER_STATUS_LOCKED:
		{
			pChannelGui->pUseGauge->SetPos(pChannelInfo->dwLoad);
			pChannelGui->pChannelState->SetTextColor(RGB(23, 125, 212), TRUE);
			pChannelGui->pChannelState->SetText(GetDisplayStringManager()->GetString("DST_LOBBY_SERVER_LOCK"));
			break;
		}
	}
}

VOID CChannelSelectGui::UpdateList(RwInt32 iOffset)
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


	CRectangle		rtScreen	= m_pThis->GetScreenRect();
	RwInt32			iButtonY	= dCHANNEL_SLOT_START_Y;
	RwInt32			iTextY		= dCHANNEL_TEXT_START_Y;
	RwInt32			iGaugeY		= dCHANNEL_GAUGE_START_Y;
	RwInt32			iGaugeBackY	= rtScreen.top + dCHANNEL_GAUGE_START_Y;
	CHANNEL_ITER	it			= m_listChannel.begin();

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
			pChannelGui->srfGaugeBack.SetPosition(rtScreen.left + dCHANNEL_GAUGE_X, iGaugeBackY);

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

VOID CChannelSelectGui::FocusEffect(RwUInt8 bySlot)
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

VOID CChannelSelectGui::CalcDownBarPosition(RwUInt8 bySlot)
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

VOID CChannelSelectGui::CalcSelectBarPosition(RwUInt8 bySlot)
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
			CDboEventGenerator::LobbyEvent(LMT_SELECT_CHANNEL, m_bySelectIndex);

			return;
		}
		
	}

	m_bySelectIndex = INVALID_BYTE;
	CDboEventGenerator::LobbyEvent(LMT_SELECT_CHANNEL, m_bySelectIndex);
}

VOID CChannelSelectGui::OnScrollChanged(RwInt32 iOffset)
{
	UpdateList(iOffset);
}

VOID CChannelSelectGui::OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos)
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

VOID CChannelSelectGui::OnClickOKButton(gui::CComponent* pComponent)
{
	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_SELECT_IDLE )
		return;

	SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
	CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
	if( !pLobby )
	{
		DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
		return;
	}

	RwUInt8 bySelectedCharIndex = pLobby->GetSelectedCharacterIndex();

	if( FALSE == Logic_IsUsableIndex(bySelectedCharIndex, NTL_MAX_COUNT_USER_CHAR_SLOT, INVALID_BYTE) )
	{
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_MUST_CHOICE_CHAR" );
		return;
	}

	if( m_bySelectIndex == INVALID_BYTE )
	{
		// 채널을 선택하십시요
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_MUST_CHOICE_CHANNEL" );
		return;
	}

	// 채널 다이얼로그 닫기
	Show(false);

	// 게임 시작
	if( FALSE == Logic_IsUsableIndex(bySelectedCharIndex, NTL_MAX_COUNT_USER_CHAR_SLOT) )
	{
		DBO_FAIL("Invalid index : " << bySelectedCharIndex);
		return;
	}

	sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter(bySelectedCharIndex);
	if( !pLOBBY_CHARACTER )
	{
		DBO_FAIL("Not exist character of index : " << bySelectedCharIndex);
		return;
	}

	if( pLOBBY_CHARACTER->bReservatedDelete )
	{
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_RESERVATED_CHAR" );
		return;
	}

	if( pLOBBY_CHARACTER->tSummary.bTutorialFlag )
	{
		// 바로 게임 진입
		CDboEventGenerator::LobbyEvent(LMT_GAME_SERVER_WAIT_CHECK_REQ);
		GetDboGlobal()->SetEnterTutorial( FALSE );
	}
	else
	{
		// 튜토리얼을 진행할지 물어본다
		GetAlarmManager()->AlarmMessage( "DST_TUTORIAL_ASK_BEGIN" );
	}
	
	m_pThis->Popup(false);
}

VOID CChannelSelectGui::OnClickCancleButton(gui::CComponent* pComponent)
{
	if( GetCharStageState()->GetCurrentState() != CHAR_STATE_SELECT_IDLE )
		return;

	// 채널 다이얼로그 닫기
	Show(false);
	m_pThis->Popup(false);
}

RwUInt8 CChannelSelectGui::PtinSlot(RwInt32 iX, RwInt32 iY)
{
	RwInt32 iWidth = m_srfFocusBar.GetWidth();

	for( RwInt8 i = 0 ; i < dMAX_VISIBLE_COUNT ; ++i )
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

RwUInt8 CChannelSelectGui::Slot_to_Index(RwUInt8 bySlot)
{
	if( bySlot == INVALID_BYTE )
		return INVALID_BYTE;

	RwUInt8 byIndex = (RwUInt8)m_pScrollBar->GetValue() + bySlot;

	if( (RwUInt8)m_listChannel.size() <= byIndex )
		return INVALID_BYTE;

	return byIndex;
}

VOID CChannelSelectGui::OnMouseDown(const CKey& key)
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

VOID CChannelSelectGui::OnMouseUp(const CKey& key)
{
	CalcDownBarPosition(INVALID_BYTE);

	if( m_byMouseDownSlot == INVALID_BYTE )
		return;

	RwUInt8 bySlot = PtinSlot((RwInt32)key.m_fX, (RwInt32)key.m_fY);
	if( m_byMouseDownSlot != bySlot )
	{
		m_byMouseDownSlot = INVALID_BYTE;
		return;
	}

	CalcSelectBarPosition(m_byMouseDownSlot);

	m_byMouseDownSlot = INVALID_BYTE;
}

VOID CChannelSelectGui::OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY)
{
	RwUInt8 byCurSlot = PtinSlot(nX, nY);

	FocusEffect(byCurSlot);
}

VOID CChannelSelectGui::OnMouseLeave(gui::CComponent* pComponent)
{
	FocusEffect(INVALID_BYTE);
	CalcDownBarPosition(INVALID_BYTE);
}

VOID CChannelSelectGui::OnPaint()
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

VOID CChannelSelectGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CChannelSelectGui::HandleEvents");

	if( msg.Id == g_EventCharStageStateEnter )
	{
		SDboEventCharStageStateEnter* pEvent = reinterpret_cast<SDboEventCharStageStateEnter*>( msg.pData );

		if( pEvent->byState == CHAR_STATE_SELECT_IDLE )
			EnableButtons(true);
		else
			EnableButtons(false);

		if( pEvent->byState == CHAR_STATE_SELECT_ENTER )
		{
			SetServerInfo();
		}
	}
	else if( msg.Id == g_EventLobbyMessage )
	{
		SDboEventLobbyMessage* pEvent = reinterpret_cast<SDboEventLobbyMessage*>( msg.pData );

		switch(pEvent->byMessage)
		{
		case LMT_UPDATE_CHANNEL:
			{
				DestroyChannels();
				CreateChannels();
				break;
			}
		case LMT_REFRESH_CHANEL:
			{
				SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
				CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
				if( !pLobby )
				{
					DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
					NTL_RETURNVOID();
				}

				for( RwUInt8 i = 0 ; i < NTL_MAX_SERVER_CHANNEL_COUNT_IN_SERVER_FARM ; ++i )
				{
					sDBO_GAME_SERVER_CHANNEL_INFO* pDBO_GAME_SERVER_CHANNEL_INFO = pLobby->GetChannel(i);
					if( !pDBO_GAME_SERVER_CHANNEL_INFO )
					{
						DBO_FAIL("Not exist channel of index : " << i);
						continue;
					}

					if( pDBO_GAME_SERVER_CHANNEL_INFO->byServerChannelIndex == pEvent->uiExData )
					{
						RefreshChannel(pDBO_GAME_SERVER_CHANNEL_INFO);
						break;
					}
				}

				break;
			}
		case LMT_GAME_SERVER_WAIT_CHECK_REQ:
			{
				SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
				CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
				if( !pLobby )
				{
					DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
					NTL_RETURNVOID();
				}

				sDBO_GAME_SERVER_CHANNEL_INFO* pDBO_GAME_SERVER_CHANNEL_INFO = pLobby->GetChannel(m_bySelectIndex);
				if( !pDBO_GAME_SERVER_CHANNEL_INFO )
				{
					DBO_FAIL("Not exist channel of index : " << m_bySelectIndex);
					NTL_RETURNVOID();
				}

				if( GetDboGlobal()->GetLobbyPacketGenerator()->SendCharConnectGameServer_WaitCheckReq(pDBO_GAME_SERVER_CHANNEL_INFO->byServerChannelIndex) )
				{
					GetCharStageState()->ChangeState(CHAR_STATE_GAME_SERVER_WAIT_CHECK_REQ);
				}

				break;
			}
		case LMT_START_CONNECT_GAME_SERVER:
			{
				// 캐릭터 서버로 부터 게임 서버로 연결을 해도 좋다는 패킷을 받은 이후
				SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
				CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
				if( !pLobby )
				{
					DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
					NTL_RETURNVOID();
				}

				RwUInt8 bySelectedCharIndex = pLobby->GetSelectedCharacterIndex();

				if( Logic_IsUsableIndex(bySelectedCharIndex, NTL_MAX_COUNT_USER_CHAR_SLOT, INVALID_BYTE) )
				{
					sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter(bySelectedCharIndex);
					if( !pLOBBY_CHARACTER )
					{
						DBO_FAIL("Not exist character of index : " << bySelectedCharIndex);
						NTL_RETURNVOID();
					}

					sDBO_GAME_SERVER_CHANNEL_INFO* pDBO_GAME_SERVER_CHANNEL_INFO = pLobby->GetChannel(m_bySelectIndex);
					if( !pDBO_GAME_SERVER_CHANNEL_INFO )
					{
						DBO_FAIL("Not exist channel of index : " << m_bySelectIndex);
						NTL_RETURNVOID();
					}

					CHARACTERID	charID					= pLOBBY_CHARACTER->tSummary.charId;
					RwUInt8		byServerChannelIndex	= pDBO_GAME_SERVER_CHANNEL_INFO->byServerChannelIndex;

					if( GetDboGlobal()->GetLobbyPacketGenerator()->SendCharSelectReq(charID, byServerChannelIndex) )
					{
						GetCharStageState()->ChangeState(CHAR_STATE_CHARACTER_SELECT_REQ);
					}
				}
				else
				{
					DBO_FAIL("Invalid index selected character index : " << bySelectedCharIndex );
				}
			}
		}
	}

	NTL_RETURNVOID();
}