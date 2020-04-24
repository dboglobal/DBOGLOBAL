#include "precomp_dboclient.h"
#include "CCBDRewardGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// dbo
#include "DisplayStringManager.h"
#include "DboEvent.h"
#include "IconMoveManager.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "InfoWndManager.h"
#include "AlarmManager.h"
#include "DboEventGenerator.h"
#include "DboGlobal.h"

// shared
#include "NtlCCBD.h"



CCCBDRewardGui::CCCBDRewardGui(const RwChar * pName)
	:CNtlPLGui(pName)
{
	Init();
}

CCCBDRewardGui::~CCCBDRewardGui()
{
}



void CCCBDRewardGui::Init()
{
	m_fRewardCountdown = CCBD_REWARD_COUNTDOWN;
	m_bLastStage = false;
	m_bySelection = NTL_PARTY_SELECT_TYPE_NONE;

	m_bShowItemInfo = false;
}

int CCCBDRewardGui::Create()
{
	DBO_WARNING_MESSAGE("CCCBDRewardGui::Create");
	NTL_FUNCTION("CCCBDRewardGui::Create");

	// Load window
	if (!CNtlPLGui::Create("gui\\CCBDRewardGui.rsr", "gui\\CCBDRewardGui.srf", "gui\\CCBDRewardGui.frm"))
		return FALSE;

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	// Get Component
	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_psttTitle = (gui::CStaticBox*)GetComponent("sttTitle");
	m_psttTitle->SetText(GetDisplayStringManager()->GetString("DST_CCBD_REWARD_TITLE"));

	m_pstbRewardName = (gui::CStaticBox*)GetComponent("stbRewardName"); // DST_CCBD_REWARD_LEVEL
	m_pstbRewardName->SetTextColor(INFOCOLOR_6);

	m_pstbLeftTimeStatic = (gui::CStaticBox*)GetComponent("stbLeftTimeStatic"); // can remove ?

	m_pstbLeftTime = (gui::CStaticBox*)GetComponent("stbLeftTime"); // countdown
	m_pstbLeftTime->SetTextColor(INFOCOLOR_6);
	m_pstbLeftTime->SetText((int)m_fRewardCountdown);

	m_phbxExplain = (gui::CHtmlBox*)GetComponent("hbxExplain");
	const WCHAR* wchExplain = GetDisplayStringManager()->GetString("DST_CCBD_REWARD_EXPLAIN");
	m_phbxExplain->SetHtmlFromMemory(wchExplain, wcslen(wchExplain));

	m_pstbCharName = (gui::CStaticBox*)GetComponent("stbCharName");
	m_pstbCharName->SetText(GetDisplayStringManager()->GetString("DST_CCBD_AGREE_CHAR_NAME"));

	m_pstbState = (gui::CStaticBox*)GetComponent("stbState");
	m_pstbState->SetText(GetDisplayStringManager()->GetString("DST_CCBD_AGREE_STATE"));

	m_pstbRewardItem = (gui::CStaticBox*)GetComponent("stbRewardItem");
	m_pstbRewardItem->SetText(GetDisplayStringManager()->GetString("DST_CCBD_REWARD_ITEM"));
	m_pstbRewardItem->Show(false); // visible only at last stage


	CRectangle rtScreen = m_pThis->GetScreenRect();

	char acBuffer[128] = "";
	int nY = 192; // height

	for (int i = 0; i < NTL_MAX_MEMBER_IN_PARTY; i++)
	{
		sPARTY_MEMBER* pMember = new sPARTY_MEMBER;
		pMember->bySelectState = NTL_PARTY_SELECT_TYPE_NONE;
		pMember->hHandle = INVALID_SERIAL_ID;

		sprintf_s(acBuffer, 128, "stbMemberName_%d", i + 1);
		pMember->pstbMemberName = (gui::CStaticBox*)GetComponent(acBuffer);

		sprintf_s(acBuffer, 128, "stbMemberState_%d", i + 1);
		pMember->pstbMemberState = (gui::CStaticBox*)GetComponent(acBuffer);

		for (int e = 0; e < NTL_PARTY_SELECT_TYPE_COUNT; e++)
		{
			sprintf_s(acBuffer, 128, "srfCharState_%d", e + 1);

			pMember->srfSelectState[e].SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CCBDRewardGui.srf", acBuffer));
			pMember->srfSelectState[e].SetPositionfromParent(28, nY);
			pMember->srfSelectState[e].SetPositionbyParent(rtScreen.left, rtScreen.top);
		}

		m_members.push_back(pMember);

		nY += 30;
	}


	m_pstbChoiceReward = (gui::CStaticBox*)GetComponent("stbChoiceReward");
	m_pstbChoiceReward->SetText(GetDisplayStringManager()->GetString("DST_CCBD_REWARD_EXPLAIN2"));


	m_srfNameBar.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CCBDRewardGui.srf", "srfNameBar"));
	m_srfNameBar.SetPositionfromParent(0, 163);
	m_srfNameBar.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_srfHideSelectButtons.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CCBDRewardGui.srf", "srfCCBDBoardBack_Last"));
	m_srfHideSelectButtons.SetPositionfromParent(0, 372);
	m_srfHideSelectButtons.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_srfChoiceRewardBar.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CCBDRewardGui.srf", "srfChoiceRewardBar"));
	m_srfChoiceRewardBar.SetPositionfromParent(0, 349);
	m_srfChoiceRewardBar.SetPositionbyParent(rtScreen.left, rtScreen.top);

	m_pbtnChoiceReward = (gui::CButton*)GetComponent("btnChoiceReward");
	m_pbtnChoiceReward->SetText(GetDisplayStringManager()->GetString("DST_CCBD_REWARD_SELECT_REWARD"));
	m_pbtnChoiceReward->SetToggleMode(false);
	m_slotClickedBtnChoiceReward = m_pbtnChoiceReward->SigClicked().Connect(this, &CCCBDRewardGui::OnClickedBtnChoiceReward);

	m_pbtnChallengeNext = (gui::CButton*)GetComponent("btnChallengeNext");
	m_pbtnChallengeNext->SetText(GetDisplayStringManager()->GetString("DST_CCBD_REWARD_SELECT_CHALLENGE"));
	m_pbtnChallengeNext->SetToggleMode(false);
	m_slotClickedBtnChallengeNext = m_pbtnChallengeNext->SigClicked().Connect(this, &CCCBDRewardGui::OnClickedBtnChallengeNext);

	m_pbtnExit = (gui::CButton*)GetComponent("btnExit");
	m_pbtnExit->SetText(GetDisplayStringManager()->GetString("DST_CCBD_REWARD_SELECT_MOVE"));
	m_slotClickedBtnExit = m_pbtnExit->SigClicked().Connect(this, &CCCBDRewardGui::OnClickedBtnExit);
	m_pbtnExit->Show(false);

	m_slotMove = m_pThis->SigMove().Connect(this, &CCCBDRewardGui::OnMove);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CCCBDRewardGui::OnPaint);
	m_slotMouseMove = m_pThis->SigMouseMove().Connect(this, &CCCBDRewardGui::OnMouseMove);

	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	LinkMsg(g_EventPartySelectStateInit);
	LinkMsg(g_EventPartySelectState);
	LinkMsg(g_EventPartyMemberDel, 0, 0x7999); // 0x7999 so its called AFTER being called in party system
	LinkMsg(g_EventBattleDungeonRewardFinish);

	Show(false);

	return TRUE;
}

void CCCBDRewardGui::Destroy()
{
	NTL_FUNCTION("CCCBDRewardGui::Destroy");

	UnLinkMsg(g_EventPartySelectStateInit);
	UnLinkMsg(g_EventPartySelectState);
	UnLinkMsg(g_EventPartyMemberDel);
	UnLinkMsg(g_EventBattleDungeonRewardFinish);

	for (std::vector<sPARTY_MEMBER*>::iterator it = m_members.begin(); it != m_members.end(); it++)
	{
		sPARTY_MEMBER* pMember = *it;
		delete pMember;
	}
	m_members.clear();

	m_selectState.clear();

	m_ItemSlot.Destroy();

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

int CCCBDRewardGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		GetNtlGuiManager()->AddUpdateFunc(this);

		Show(true);
	}
	else
	{
		GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck(this);

		ShowItemInfoWindow(false);

		m_selectState.clear();

		Show(false);
	}

	NTL_RETURN(TRUE);
}

void CCCBDRewardGui::HandleEvents(RWS::CMsg & msg)
{
	if (msg.Id == g_EventPartySelectStateInit)
	{
		if (IsShow())
		{
			DBO_WARNING_MESSAGE("Can not init ccbd reward because the reward is already visible");
			return;
		}

		SNtlEventPartySelectStateInit* pEvent = reinterpret_cast<SNtlEventPartySelectStateInit*>(msg.pData);

		if (pEvent->bySelectState == NTL_PARTY_SELECT_TYPE_CCBD_REWARD)
		{
			Init();

			if (pEvent->bLastStage)
			{
				m_bLastStage = true;

				CRectangle rtScreen = m_pThis->GetScreenRect();

				// hide some default stuff
				m_pbtnChoiceReward->Show(false);
				m_pbtnChallengeNext->Show(false);
				m_pstbChoiceReward->Show(false);
				m_pstbCharName->Show(false);
				m_pstbState->Show(false);

				m_pstbRewardItem->Show(true);

				// show last stage data
				m_pbtnExit->Show(true);

				m_pstbRewardName->SetText(GetDisplayStringManager()->GetString("DST_CCBD_REWARD_LAST_LEVEL"));

				m_psttTitle->SetText(GetDisplayStringManager()->GetString("DST_CCBD_REWARD_TITLE_LAST"));
				const WCHAR* wchExplain = GetDisplayStringManager()->GetString("DST_CCBD_REWARD_LAST_EXPLAIN");
				m_phbxExplain->SetHtmlFromMemory(wchExplain, wcslen(wchExplain));


				m_srfNameBar.SetPositionfromParent(0, 248);
				m_srfNameBar.SetPositionbyParent(rtScreen.left, rtScreen.top);


				// create item
				pEvent->dwParam = 7000001;
				// get item
				sITEM_TBLDAT* pItemTbldat = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(pEvent->dwParam);

				m_ItemSlot.Create(m_pThis, DIALOG_CCBD_REWARD, REGULAR_SLOT_ITEM_TABLE, SDS_COUNT);
				m_ItemSlot.SetSize(NTL_ITEM_ICON_SIZE);
				m_ItemSlot.SetPosition_fromParent(115, 285);
				m_ItemSlot.SetParentPosition(rtScreen.left, rtScreen.top);
				m_ItemSlot.SetIcon(pItemTbldat->tblidx, 1);
			}
			else
			{
				CNtlCCBD* pCCBD = GetNtlSLGlobal()->GetSobAvatar()->GetCCBD();

				m_pstbRewardName->Format(GetDisplayStringManager()->GetString("DST_CCBD_REWARD_LEVEL"), pCCBD->GetStage());

				LoadPartyMembers();
			}

			// open the dialog
			GetDialogManager()->OpenDialog(DIALOG_CCBD_REWARD);
		}
	}
	else if (msg.Id == g_EventPartySelectState)
	{
		if (!IsShow())
		{
			DBO_WARNING_MESSAGE("Can not process ccbd reward because the reward is not visible");
			return;
		}

		if (m_bLastStage)
			return;

		SNtlEventPartySelectState* pEvent = reinterpret_cast<SNtlEventPartySelectState*>(msg.pData);

		SelectState(pEvent->hTarget, pEvent->bySelectState);

		// unset buttons
		if (pEvent->hTarget == Logic_GetAvatarHandle())
		{
			m_bySelection = pEvent->bySelectState;

			if(m_bySelection == NTL_PARTY_SELECT_TYPE_CCBD_START)
				m_pbtnChoiceReward->SetDown(false);
			else if (m_bySelection == NTL_PARTY_SELECT_TYPE_CCBD_REWARD)
				m_pbtnChallengeNext->SetDown(false);
		}
	}
	else if (msg.Id == g_EventPartyMemberDel)
	{
		if (!IsShow())
			return;

		if (m_bLastStage)
			return;

		ReloadPartyMember();
	}
	else if (msg.Id == g_EventBattleDungeonRewardFinish)
	{
		GetDialogManager()->CloseDialog(DIALOG_CCBD_REWARD);
	}
}

void CCCBDRewardGui::Update(float fElapsed)
{
	if (!IsShow())
		return;

	m_fRewardCountdown -= fElapsed;
	if (m_fRewardCountdown < 0.f)
		m_fRewardCountdown = 0.0f;

	m_pstbLeftTime->SetText((int)m_fRewardCountdown);
}

void CCCBDRewardGui::LoadPartyMembers()
{
	CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();

	int i = 0;

	COMMUNITY_ITER it_list = pParty->Begin();
	for (; it_list != pParty->End(); it_list++)
	{
		sPARTY_MEMBER* pMember = m_members[i];

		pMember->bySelectState = NTL_PARTY_SELECT_TYPE_NONE;
		pMember->hHandle = (*it_list)->uiKey;

		pMember->pstbMemberName->SetText((*it_list)->wszMemberName);
		pMember->pstbMemberState->SetText(GetDisplayStringManager()->GetString("DST_CCBD_REWARD_STATE_SELECTING"));

		++i;
	}
}

void CCCBDRewardGui::ReloadPartyMember()
{
	CNtlParty* pParty = GetNtlSLGlobal()->GetSobAvatar()->GetParty();

	int i = 0;

	COMMUNITY_ITER it_list = pParty->Begin();
	for (; it_list != pParty->End(); it_list++, i++)
	{
		sPARTY_MEMBER* pMember = m_members[i];

		pMember->hHandle = (*it_list)->uiKey;
		pMember->bySelectState = GetSelectState(pMember->hHandle);

		pMember->pstbMemberName->SetText((*it_list)->wszMemberName);

		SetStateText(pMember->pstbMemberState, pMember->bySelectState);
	}

	for (; i < NTL_MAX_MEMBER_IN_PARTY; i++)
	{
		sPARTY_MEMBER* pMember = m_members[i];
		pMember->bySelectState = NTL_PARTY_SELECT_TYPE_NONE;
		pMember->hHandle = INVALID_SERIAL_ID;

		pMember->pstbMemberName->Clear();
		pMember->pstbMemberState->Clear();
	}
}

void CCCBDRewardGui::SelectState(HOBJECT hHandle, BYTE byState)
{
	for (int i = 0; i < NTL_MAX_MEMBER_IN_PARTY; i++)
	{
		sPARTY_MEMBER* pMember = m_members[i];

		if (pMember->hHandle == hHandle)
		{
			pMember->bySelectState = byState;
			SetStateText(pMember->pstbMemberState, pMember->bySelectState);

			m_selectState[hHandle] = byState;

			break;
		}
	}
}

BYTE CCCBDRewardGui::GetSelectState(HOBJECT hHandle)
{
	std::map<HOBJECT, BYTE>::iterator it = m_selectState.find(hHandle);
	if (it == m_selectState.end())
		return NTL_PARTY_SELECT_TYPE_NONE;

	return it->second;
}

void CCCBDRewardGui::SetStateText(gui::CStaticBox * pStateBox, BYTE byState)
{
	if (byState == NTL_PARTY_SELECT_TYPE_NONE)
	{
		pStateBox->SetText(GetDisplayStringManager()->GetString("DST_CCBD_REWARD_STATE_SELECTING"));
	}
	else if (byState == NTL_PARTY_SELECT_TYPE_CCBD_START)
	{
		pStateBox->SetText(GetDisplayStringManager()->GetString("DST_CCBD_REWARD_STATE_CHALLENGE_NEXT"));
	}
	else if (byState == NTL_PARTY_SELECT_TYPE_CCBD_REWARD)
	{
		pStateBox->SetText(GetDisplayStringManager()->GetString("DST_CCBD_REWARD_STATE_SELECTED_REWARD"));
	}
}

void CCCBDRewardGui::ShowItemInfoWindow(bool bIsShow)
{
	if (bIsShow)
	{
		if (m_bShowItemInfo)
			return;

		CRectangle rtScreen = m_pThis->GetScreenRect();

		GetInfoWndManager()->ShowInfoWindow(TRUE, CInfoWndManager::INFOWND_TABLE_ITEM,
			rtScreen.left + m_ItemSlot.GetX_fromParent(),
			rtScreen.top + m_ItemSlot.GetY_fromParent() - 20,
			m_ItemSlot.GetItemTable(), DIALOG_CCBD_REWARD);
	}
	else
	{
		if (m_bShowItemInfo == false)
			return;

		if (GetInfoWndManager()->GetRequestGui() == DIALOG_CCBD_REWARD)
			GetInfoWndManager()->ShowInfoWindow(FALSE);
	}

	m_bShowItemInfo = bIsShow;
}

void CCCBDRewardGui::OnPaint()
{
	m_srfNameBar.Render();

	if (m_bLastStage == false)
	{
		m_srfChoiceRewardBar.Render();

		for (std::vector<sPARTY_MEMBER*>::iterator it = m_members.begin(); it != m_members.end(); it++)
		{
			sPARTY_MEMBER* pMember = *it;
			if (pMember->hHandle == INVALID_SERIAL_ID)
				break;

			pMember->srfSelectState[pMember->bySelectState].Render();
		}
	}
	else
	{
		m_ItemSlot.Paint();
		m_srfHideSelectButtons.Render();
	}
}

void CCCBDRewardGui::OnMove(int iOldX, int iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_srfNameBar.SetPositionbyParent(rtScreen.left, rtScreen.top);
	m_srfChoiceRewardBar.SetPositionbyParent(rtScreen.left, rtScreen.top);

	for (std::vector<sPARTY_MEMBER*>::iterator it = m_members.begin(); it != m_members.end(); it++)
	{
		sPARTY_MEMBER* pMember = *it;
		if (pMember->hHandle == INVALID_SERIAL_ID)
			break;

		pMember->srfSelectState[NTL_PARTY_SELECT_TYPE_NONE].SetPositionbyParent(rtScreen.left, rtScreen.top);
		pMember->srfSelectState[NTL_PARTY_SELECT_TYPE_CCBD_START].SetPositionbyParent(rtScreen.left, rtScreen.top);
		pMember->srfSelectState[NTL_PARTY_SELECT_TYPE_CCBD_REWARD].SetPositionbyParent(rtScreen.left, rtScreen.top);
	}

	m_ItemSlot.SetParentPosition(rtScreen.left, rtScreen.top);
	m_srfHideSelectButtons.SetPositionbyParent(rtScreen.left, rtScreen.top);
}

void CCCBDRewardGui::OnMouseMove(int nFlags, int nX, int nY)
{
	if (m_ItemSlot.PtInRect(nX, nY))
	{
		if (m_ItemSlot.GetItemTable() != NULL)
		{
			ShowItemInfoWindow(true);
			return;
		}
	}
	else if(m_bShowItemInfo)
	{
		ShowItemInfoWindow(false);
	}
}

void CCCBDRewardGui::OnClickedBtnChoiceReward(gui::CComponent * pComponent)
{
	if (m_bySelection == NTL_PARTY_SELECT_TYPE_CCBD_REWARD)
		return;

	if (m_fRewardCountdown > 0.f && m_pbtnChoiceReward->IsDown() == false)
	{
		if (GetDboGlobal()->GetGamePacketGenerator()->SendPartySelectStateReq(NTL_PARTY_SELECT_TYPE_CCBD_REWARD))
		{
			m_pbtnChoiceReward->SetDown();
			m_pbtnChallengeNext->SetDown();
		}
	}
}

void CCCBDRewardGui::OnClickedBtnChallengeNext(gui::CComponent * pComponent)
{
	if (m_bySelection == NTL_PARTY_SELECT_TYPE_CCBD_START)
		return;

	if (m_fRewardCountdown > 0.f && m_pbtnChallengeNext->IsDown() == false)
	{
		if (GetDboGlobal()->GetGamePacketGenerator()->SendPartySelectStateReq(NTL_PARTY_SELECT_TYPE_CCBD_START))
		{
			m_pbtnChoiceReward->SetDown();
			m_pbtnChallengeNext->SetDown();
		}
	}
}

void CCCBDRewardGui::OnClickedBtnExit(gui::CComponent * pComponent)
{
	if (m_bLastStage == false)
		return;

	if (m_fRewardCountdown > 0.f && m_pbtnExit->IsDown() == false)
	{
		m_pbtnExit->SetDown();

		GetDboGlobal()->GetGamePacketGenerator()->SendBattleDungeonLeaveReq();
	}
}

