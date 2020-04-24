#include "precomp_dboclient.h"
#include "CCBDNotifyGui.h"


// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlDTCinematicManager.h"

//client
#include "DboGlobal.h"
#include "DialogPriority.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"
#include "CounterGui.h"

//shared
#include "NtlCCBD.h"



CCCBDNotifyGui::CCCBDNotifyGui(const RwChar* pName)
	:CNtlPLGui(pName)
	, m_pCounterGui(NULL)
{
}

CCCBDNotifyGui::~CCCBDNotifyGui()
{
}

RwBool CCCBDNotifyGui::Create()
{
	NTL_FUNCTION("CCCBDNotifyGui::Create");

	if (!CNtlPLGui::Create("", "", "gui\\CCBDNotifyGui.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_NOTIFY);


	m_pflashNumber = (gui::CFlash*)GetComponent("flashNumber");
	m_slotFlashNumberEnd = m_pflashNumber->SigMovieEnd().Connect(this, &CCCBDNotifyGui::OnMovieEnd_Notify);

	m_pflashNumber2 = (gui::CFlash*)GetComponent("flashNumber2");
	m_slotFlashNumber2End = m_pflashNumber2->SigMovieEnd().Connect(this, &CCCBDNotifyGui::OnMovieEnd_Notify);

	m_pflashNumber3 = (gui::CFlash*)GetComponent("flashNumber3");
	m_slotFlashNumber3End = m_pflashNumber3->SigMovieEnd().Connect(this, &CCCBDNotifyGui::OnMovieEnd_Notify);


	m_pflashNotify = (gui::CFlash*)GetComponent("flashNotify");
	m_slotFlashNotifyEnd = m_pflashNotify->SigMovieEnd().Connect(this, &CCCBDNotifyGui::OnMovieEnd_Notify);

	m_pflashNotify2 = (gui::CFlash*)GetComponent("flashNotify2");
	m_slotFlashNotify2End = m_pflashNotify2->SigMovieEnd().Connect(this, &CCCBDNotifyGui::OnMovieEnd_Notify);

	m_pflashOridinal = (gui::CFlash*)GetComponent("flashOridinal");
	m_slotFlashOridinalEnd = m_pflashOridinal->SigMovieEnd().Connect(this, &CCCBDNotifyGui::OnMovieEnd_Notify);

	m_psttAim = (gui::CStaticBox*)GetComponent("sttAim");
	m_psttAim->SetTextColor(INFOCOLOR_7);
	m_psttAim->SetText(L"ABABCVBFVBDFHSHF");
	m_psttAim->Show(false);


	GetNtlGuiManager()->AddUpdateFunc(this);

	// sig
	

	LinkMsg(g_EventBattleDungeonStageClear);
	LinkMsg(g_EventBattleDungeonFail);
	LinkMsg(g_EventResize);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CCCBDNotifyGui::Destroy()
{
	NTL_FUNCTION("CCCBDNotifyGui::Destroy");
	
	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck(this);

	UnLinkMsg(g_EventBattleDungeonStageClear);
	UnLinkMsg(g_EventBattleDungeonFail);
	UnLinkMsg(g_EventResize);

	if (m_pCounterGui)
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui(m_pCounterGui);
		m_pCounterGui->Destroy();
		NTL_DELETE(m_pCounterGui);
	}

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CCCBDNotifyGui::Update(RwReal fElapsed)
{
	if (IsShow() == FALSE)
		return;

	m_pflashNumber->Update(fElapsed);
	m_pflashNumber2->Update(fElapsed);
	m_pflashNumber3->Update(fElapsed);
	m_pflashNotify->Update(fElapsed);
	m_pflashNotify2->Update(fElapsed);
	m_pflashOridinal->Update(fElapsed);
}

RwBool CCCBDNotifyGui::LoadFlash(gui::CFlash* pFlash, const char* acFileName)
{
	if (pFlash->Load(acFileName) == false)
	{
		DBO_WARNING_MESSAGE("Not eixst file : " << acFileName);

		FinishFlash();
		GetDialogManager()->CloseDialog(DIALOG_CCBD_NOTIFY);

		return FALSE;
	}

	LocateComponent();

	pFlash->SetFrameCallback(FALSE);
	pFlash->PlayMovie(TRUE);

	m_strFileName = acFileName;

	return TRUE;
}

VOID CCCBDNotifyGui::LocateComponent()
{
	CRectangle rtFlash = m_pflashNumber->GetFrameResolution();
	RwInt32 iWidth = rtFlash.GetWidth();
	RwInt32 iHeight = rtFlash.GetHeight();
	RwInt32 iCleintWidth = (RwInt32)GetDboGlobal()->GetScreenWidth();
	RwInt32 iCleintHeight = (RwInt32)GetDboGlobal()->GetScreenHeight();

	rtFlash.left = (iCleintWidth - iWidth) / 4;
	rtFlash.right = rtFlash.left + iWidth;
	rtFlash.top = (iCleintHeight - iHeight) / 5;
	rtFlash.bottom = rtFlash.top + iHeight;

	m_pflashNumber->SetPosition(rtFlash);
	m_pflashNumber->SetSize(iWidth, iHeight);


	rtFlash = m_pflashNumber2->GetFrameResolution();
	iWidth = rtFlash.GetWidth();
	iHeight = rtFlash.GetHeight();

	rtFlash.left = (iCleintWidth - iWidth) / 3;
	rtFlash.right = rtFlash.left + iWidth;
	rtFlash.top = (iCleintHeight - iHeight) / 5;
	rtFlash.bottom = rtFlash.top + iHeight;

	m_pflashNumber2->SetPosition(rtFlash);
	m_pflashNumber2->SetSize(iWidth, iHeight);

	rtFlash = m_pflashNumber3->GetFrameResolution();
	iWidth = rtFlash.GetWidth();
	iHeight = rtFlash.GetHeight();

	rtFlash.left = (iCleintWidth - iWidth) / 2;
	rtFlash.right = rtFlash.left + iWidth;
	rtFlash.top = (iCleintHeight - iHeight) / 5;
	rtFlash.bottom = rtFlash.top + iHeight;

	m_pflashNumber3->SetPosition(rtFlash);
	m_pflashNumber3->SetSize(iWidth, iHeight);



	rtFlash = m_pflashNotify->GetFrameResolution();
	iWidth = rtFlash.GetWidth();
	iHeight = rtFlash.GetHeight();
	rtFlash.left = (iCleintWidth - iWidth) / 2;
	rtFlash.right = rtFlash.left + iWidth;
	rtFlash.top = (iCleintHeight - iHeight) / 5;
	rtFlash.bottom = rtFlash.top + iHeight;
	m_pflashNotify->SetPosition(rtFlash);

	rtFlash.left = (iCleintWidth - iWidth) / 2;
	rtFlash.right = rtFlash.left + iWidth;
	rtFlash.top = (iCleintHeight - iHeight) / 5;
	rtFlash.bottom = rtFlash.top + iHeight;
	m_pflashNotify2->SetPosition(rtFlash);

	rtFlash.left = 0;
	rtFlash.top = 0;
	rtFlash.right = iCleintWidth;
	rtFlash.bottom = iCleintHeight;
	m_pflashNotify2->SetPosition(rtFlash);


	rtFlash = m_pflashOridinal->GetFrameResolution();
	iWidth = rtFlash.GetWidth();
	iHeight = rtFlash.GetHeight();

	rtFlash.left = (iCleintWidth - iWidth) / 2;
	rtFlash.right = rtFlash.left + iWidth;
	rtFlash.top = (iCleintHeight - iHeight) / 5;
	rtFlash.bottom = rtFlash.top + iHeight;

	m_pflashOridinal->SetPosition(rtFlash);
	m_pflashOridinal->SetSize(iWidth, iHeight);


	m_pThis->SetWidth(iCleintWidth);
	m_pThis->SetHeight(iCleintHeight);

	if (m_pCounterGui)
	{
		m_pCounterGui->SetPosition(5, iCleintHeight - 125);
	}
}

VOID CCCBDNotifyGui::FinishFlash()
{
	m_strFileName.clear();

	m_pflashNumber->Unload();
	m_pflashNumber2->Unload();
	m_pflashNumber3->Unload();

	m_pflashNotify->Unload();
	m_pflashNotify2->Unload();

	m_pflashOridinal->Unload();

	Enable(false);
}

void CCCBDNotifyGui::OnMovieEnd_Notify(gui::CComponent* pComponent)
{
	Enable(false);

	GetDialogManager()->CloseDialog(DIALOG_CCBD_NOTIFY);
}


RwInt32 CCCBDNotifyGui::SwitchDialog(bool bOpen)
{
	if (bOpen)
	{
	}
	else
	{
		FinishFlash();
	}

	Show(bOpen);
	return 1;
}

VOID CCCBDNotifyGui::HandleEvents(RWS::CMsg &msg)
{
	NTL_FUNCTION("CCCBDNotifyGui::HandleEvents");

	if (msg.Id == g_EventBattleDungeonStageClear)
	{
		FinishFlash();

		CNtlCCBD* pCCBD = GetNtlSLGlobal()->GetSobAvatar()->GetCCBD();

		if (pCCBD->GetStage() == CCBD_MAX_STAGE)
		{
			LoadFlash(m_pflashNotify, "CCBD_all_clear.swf");
		}
		else if (IsCCBDBossStage(pCCBD->GetStage()))
		{
			LoadFlash(m_pflashNotify, "CCBD_Boss_clear.swf");
		}
		else
		{
			LoadFlash(m_pflashNotify, "fs002.swf");
		}

		GetDialogManager()->OpenDialog(DIALOG_CCBD_NOTIFY);
	}
	else if (msg.Id == g_EventBattleDungeonFail)
	{
		FinishFlash();
		LoadFlash(m_pflashNotify, "fs003.swf"); // needs to be above the counter gui, or counter gui numbers wont work

		m_pCounterGui = NTL_NEW CCounterGui("CounterGui");
		if (m_pCounterGui->Create(COUNTER_DISPLAY_2, CCBD_FAIL_TIMER_IN_MS / 1000, COUNTER_QUADRANT_4) == FALSE)
		{
			m_pCounterGui->Destroy();
			NTL_DELETE(m_pCounterGui);
		}
		else
		{
			CNtlPLGuiManager::GetInstance()->AddGui(m_pCounterGui);
			m_pCounterGui->AlarmSound(TRUE, CCBD_FAIL_TIMER_IN_MS / 1000);

			LocateComponent();
		}

		GetDialogManager()->OpenDialog(DIALOG_CCBD_NOTIFY);
	}
	else if (msg.Id == g_EventResize)
	{
		LocateComponent();
	}

	NTL_RETURNVOID();
}