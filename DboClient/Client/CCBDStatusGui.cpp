#include "precomp_dboclient.h"
#include "CCBDStatusGui.h"

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
#include "SideDialogManager.h"

// simul
#include "NtlWorldConceptCCBD.h"


CCCBDStatusGui* CCCBDStatusGui::m_pInstance = NULL;


CCCBDStatusGui::CCCBDStatusGui(const RwChar * pName)
	:CNtlPLGui(pName)
{
	Init();
}

CCCBDStatusGui::~CCCBDStatusGui()
{
}


VOID CCCBDStatusGui::CreateInstance()
{
	if (m_pInstance)
		return;

	m_pInstance = NTL_NEW CCCBDStatusGui("CCBDStatusGui");
	if (!m_pInstance->Create())
		NTL_DELETE(m_pInstance);

	CNtlPLGuiManager::GetInstance()->AddGui(m_pInstance);
	GetSideDialogManager()->RegistDialog(SDIALOG_CCBD_STATUS, m_pInstance, &CCCBDStatusGui::SwitchDialog);
}

VOID CCCBDStatusGui::DestroyInstance()
{
	if (!m_pInstance)
		return;

	GetSideDialogManager()->UnRegistDialog(SDIALOG_CCBD_STATUS);
	GetNtlGuiManager()->RemoveGui(m_pInstance);
	m_pInstance->Destroy();
	NTL_DELETE(m_pInstance);
}


void CCCBDStatusGui::Init()
{
	m_bCountDown = false;
	m_bHasCountDown = false;
	m_fRemainTime = 0.0f;
}

RwBool CCCBDStatusGui::Create()
{
	NTL_FUNCTION("CCCBDStatusGui::Create");

	// Load window
	if (!CNtlPLGui::Create("gui\\CCBDStatusGui.rsr", "gui\\CCBDStatusGui.srf", "gui\\CCBDStatusGui.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	// Get Component
	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	m_pstbDialogName = (gui::CStaticBox*)GetComponent("stbDialogName");
	m_pstbDialogName->SetText(GetDisplayStringManager()->GetString("DST_CCBD"));

	m_pbtnMinimize = (gui::CButton*)GetComponent("btnMinimize");
	m_slotClickedBtnMinimize = m_pbtnMinimize->SigClicked().Connect(this, &CCCBDStatusGui::OnClickedBtnMinimize);

	m_pstbFloor = (gui::CStaticBox*)GetComponent("stbFloor");
	m_pstbFloor->SetTextColor(INFOCOLOR_3);

	m_pstbAimStatic = (gui::CStaticBox*)GetComponent("stbAimStatic");
	m_pstbAimStatic->SetTextColor(INFOCOLOR_3);

	m_pstbAim = (gui::CStaticBox*)GetComponent("stbAim");

	m_pstbLeftTimeStatic = (gui::CStaticBox*)GetComponent("stbLeftTimeStatic");
	m_pstbLeftTimeStatic->SetTextColor(INFOCOLOR_3);
	m_pstbLeftTimeStatic->SetText(GetDisplayStringManager()->GetString("DST_TMQ_STATUS_REMAIN_TIME"));
	m_pstbLeftTimeStatic->Show(false);

	m_ppnlDisableItem = (gui::CPanel*)GetComponent("pnlDisableItem");
	m_ppnlDisableItem->SetToolTip(GetDisplayStringManager()->GetString("DST_CCBD_STATUS_DISABLE_ITEM"));

	m_ppnlDisableChargeLPEP = (gui::CPanel*)GetComponent("pnlDisableChargeLPEP");
	m_ppnlDisableChargeLPEP->SetToolTip(GetDisplayStringManager()->GetString("DST_CCBD_STATUS_DISABLE_CHARGE_LPEP"));

	m_ppnlDisableGetEXP = (gui::CPanel*)GetComponent("pnlDisableGetEXP");
	m_ppnlDisableGetEXP->SetToolTip(GetDisplayStringManager()->GetString("DST_CCBD_STATUS_DISABLE_GET_EXP"));


	m_LeftTimeMinuteNumber.Create(ND_RIGHT, NVD_TOP, -5, dNCFLAG_NONE, 2);
	m_LeftTimeMinuteNumber.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_0"));
	m_LeftTimeMinuteNumber.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_1"));
	m_LeftTimeMinuteNumber.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_2"));
	m_LeftTimeMinuteNumber.SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_3"));
	m_LeftTimeMinuteNumber.SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_4"));
	m_LeftTimeMinuteNumber.SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_5"));
	m_LeftTimeMinuteNumber.SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_6"));
	m_LeftTimeMinuteNumber.SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_7"));
	m_LeftTimeMinuteNumber.SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_8"));
	m_LeftTimeMinuteNumber.SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_9"));
	m_LeftTimeMinuteNumber.SetNumber(0);

	m_srfRemainSemiColon.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSemiColon"));

	m_LeftTimeSecondNumber.Create(ND_RIGHT, NVD_TOP, -5, dNCFLAG_NONE, 2);
	m_LeftTimeSecondNumber.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_0"));
	m_LeftTimeSecondNumber.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_1"));
	m_LeftTimeSecondNumber.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_2"));
	m_LeftTimeSecondNumber.SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_3"));
	m_LeftTimeSecondNumber.SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_4"));
	m_LeftTimeSecondNumber.SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_5"));
	m_LeftTimeSecondNumber.SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_6"));
	m_LeftTimeSecondNumber.SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_7"));
	m_LeftTimeSecondNumber.SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_8"));
	m_LeftTimeSecondNumber.SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber.srf", "srfSideNum_9"));
	m_LeftTimeSecondNumber.SetNumber(0);

	int iXMinutePos = 35;
	m_LeftTimeMinuteNumber.SetPosition(iXMinutePos, 56);
	m_srfRemainSemiColon.SetPositionfromParent(iXMinutePos + m_LeftTimeMinuteNumber.GetWidth(), 56);
	m_LeftTimeSecondNumber.SetPosition(iXMinutePos + m_LeftTimeMinuteNumber.GetWidth() + m_srfRemainSemiColon.GetWidth(), 56);


	m_slotMove = m_pThis->SigMove().Connect(this, &CCCBDStatusGui::OnMove);
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CCCBDStatusGui::OnPaint);


	// Dialog Priority
	m_pThis->SetPriority(dDIALOGPRIORITY_DEFAULT);

	// Update
	GetNtlGuiManager()->AddUpdateFunc(this);


	LinkMsg(g_EventCharReady);
	LinkMsg(g_EventBattleDungeonStateUpdate);
	LinkMsg(g_EventBattleDungeonLimitTime);


	Show(false);

	NTL_RETURN(TRUE);
}

void CCCBDStatusGui::Destroy()
{
	NTL_FUNCTION("CCCBDStatusGui::Destroy");

	Init();

	GetNtlGuiManager()->RemoveUpdateFunc(this);

	UnLinkMsg(g_EventCharReady);
	UnLinkMsg(g_EventBattleDungeonStateUpdate);
	UnLinkMsg(g_EventBattleDungeonLimitTime);


	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

RwInt32 CCCBDStatusGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
		if (GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_CCBD) == FALSE)
			return -1;

		CNtlWorldConceptCCBD* pWorldConcept = reinterpret_cast<CNtlWorldConceptCCBD*>(GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_CCBD));
		if (!pWorldConcept)
		{
			DBO_FAIL("Not exist CCBD world concept");
			return -1;
		}

		Show(true);
	}
	else
	{
		Init();

		Show(false);
	}

	NTL_RETURN(TRUE);
}

void CCCBDStatusGui::HandleEvents(RWS::CMsg & msg)
{
	if (msg.Id == g_EventCharReady)
	{
		if (GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_CCBD) == FALSE)
			NTL_RETURNVOID();


		if (IsShow() == FALSE)
		{
			// Open Status GUI
			GetSideDialogManager()->OpenDialog(SDIALOG_CCBD_STATUS);
		}
	}
	else if (msg.Id == g_EventBattleDungeonStateUpdate)
	{
		if (IsShow() == FALSE)
		{
			DBO_WARNING_MESSAGE("Can not process ccbd state update because the status is not visible");
			return;
		}

		SNtlEventBattleDungeonStateUpdate* pEvent = reinterpret_cast<SNtlEventBattleDungeonStateUpdate*>(msg.pData);

		CTextTable* pTextTable = API_GetTableContainer()->GetTextAllTable()->GetCSTextTble();

		// set floor
		m_pstbFloor->Format(GetDisplayStringManager()->GetString("DST_CCBD_AGREE_CHALLENGE_STAGE"), pEvent->byStage);

		// set title text
		m_pstbAimStatic->SetText(pTextTable->GetText(pEvent->titleTblidx).c_str());

		// set sub title text
		m_pstbAim->SetText(pTextTable->GetText(pEvent->subTitleTblidx).c_str());

		// set time limit
		if (pEvent->dwLimitTime > 0)
		{
			m_fRemainTime = (float)pEvent->dwLimitTime;
			m_bCountDown = false;
			m_bHasCountDown = true;

			m_pstbLeftTimeStatic->Show(true);
		}
	}
	else if (msg.Id == g_EventBattleDungeonLimitTime)
	{
		if (IsShow() == FALSE)
		{
			DBO_WARNING_MESSAGE("Can not process ccbd state update because the status is not visible");
			return;
		}

		SNtlEventBattleDungeonLimitTime* pEvent = reinterpret_cast<SNtlEventBattleDungeonLimitTime*>(msg.pData);

		if (pEvent->bStart)
		{
			m_pstbLeftTimeStatic->Show(true);

			// create the countdown
			m_bCountDown = true;
			m_bHasCountDown = true;
			m_fRemainTime = (float)pEvent->dwLimitTime;
		}
		else
		{
			m_pstbLeftTimeStatic->Show(false);

			// destroy the countdown
			m_bCountDown = false;
			m_fRemainTime = 0.0f;

			m_bHasCountDown = false;
		}
	}
}

void CCCBDStatusGui::Update(float fElapsed)
{
	if (IsShow() == FALSE)
		return;

	if (m_bCountDown == false)
		return;

	m_fRemainTime -= fElapsed;
	if (m_fRemainTime < 0.f)
		m_fRemainTime = 0.0f;

	int iMinute = (RwInt32)m_fRemainTime / 60;
	int iSecond = (RwInt32)m_fRemainTime % 60;

	m_LeftTimeMinuteNumber.SetNumber(iMinute);
	m_LeftTimeSecondNumber.SetNumber(iSecond);
}

void CCCBDStatusGui::OnPaint()
{
	if (m_bHasCountDown) // only render if countdown is available
	{
		m_LeftTimeMinuteNumber.Render();
		m_LeftTimeSecondNumber.Render();
		m_srfRemainSemiColon.Render();
	}
}

void CCCBDStatusGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen = m_pThis->GetScreenRect();

	m_LeftTimeMinuteNumber.SetParentPos(rtScreen.left, rtScreen.top);
	m_LeftTimeSecondNumber.SetParentPos(rtScreen.left, rtScreen.top);
	m_srfRemainSemiColon.SetPositionbyParent(rtScreen.left, rtScreen.top);
}

void CCCBDStatusGui::OnClickedBtnMinimize(gui::CComponent * pComponent)
{
	CDboEventGenerator::SideDialogEvent(SDIALOG_MESSAGE_MINIMIZE, SDIALOG_CCBD_STATUS, DIALOG_SIDEDIALOG_CONTROLLER);
}

