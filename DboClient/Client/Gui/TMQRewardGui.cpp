#include "precomp_dboclient.h"
#include "TMQRewardGui.h"

// core
#include "NtlDebug.h"

// share
#include "TimeQuestTable.h"
#include "TableContainer.h"
#include "SkillTable.h"
#include "HTBSetTable.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLEventfunc.h"
#include "NtlSLTMQ.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"

// dbo
#include "DialogManager.h"
#include "IconMoveManager.h"
#include "DisplayStringManager.h"
#include "InfoWndManager.h"
#include "QuestGuiDef.h"
#include "ResultNarrationGui.h"
#include "CounterGui.h"
#include "DboEventGenerator.h"
#include "AlarmManager.h"
#include "DboGlobal.h"

namespace
{
	// Number Effect
	#define dNUMEFFECT_START_RATE			30.f
	#define dNUMEFFECT_END_RATE				1.f
	#define dNUMEFFECT_TIME					0.5f

	// 각 CNumberGui가 한자리 숫자만을 표시하기에 부득이 여기서 하드코딩으로 숫자간 간격을 조절한다
	#define dNUM_GAP						-2
};

CTMQRewardGui* CTMQRewardGui::m_pInstance = NULL;


CTMQRewardGui::CTMQRewardGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_pNarrationGui(NULL)
,m_pCounterGui(NULL)
{
}

CTMQRewardGui::~CTMQRewardGui()
{
}

RwBool CTMQRewardGui::Create()
{
	NTL_FUNCTION( "CTMQRewardGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\TMQReward.frm"))
		NTL_RETURN( FALSE  );

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");

	CRectangle rect;
	CRectangle rtScreen = m_pThis->GetScreenRect();

	// Additional compensation confirmation button
	m_pConfirmButton = (gui::CButton*)GetComponent( "btnConfirm" );
	m_pConfirmButton->SetText( GetDisplayStringManager()->GetString("DST_LOBBY_OK") );
	m_pConfirmButton->Show(false);
	m_slotConfirmButton = m_pConfirmButton->SigClicked().Connect(this, &CTMQRewardGui::OnConfirmButton);

	// Background Flash
	m_pFlashBackground = (gui::CFlash*)GetComponent("flaBackground");
	m_pFlashBackground->SetResizeType(gui::CFlash::EXACTFIT);

	// Flashing TMQ Results
	m_pFlashResult = (gui::CFlash*)GetComponent("flaResult");
	m_pFlashResult->SetFrameCallback(TRUE);	
	m_slotFrameCallbackResult = m_pFlashResult->SigFSCallBack().Connect( this, &CTMQRewardGui::OnFrameCallback_Result );



	m_srfEXPText.SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPText" ) );

	m_ExpNumberGui.Create(ND_CENTER);
	m_ExpNumberGui.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum0" ));
	m_ExpNumberGui.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum1" ));
	m_ExpNumberGui.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum2" ));
	m_ExpNumberGui.SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum3" ));
	m_ExpNumberGui.SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum4" ));
	m_ExpNumberGui.SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum5" ));
	m_ExpNumberGui.SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum6" ));
	m_ExpNumberGui.SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum7" ));
	m_ExpNumberGui.SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum8" ));
	m_ExpNumberGui.SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfEXPNum9" ));
	m_ExpNumberGui.SetNumGap(5);

	Init();

	// sig
	//m_slotMove			= m_pThis->SigMove().Connect( this, &CTMQRewardGui::OnMove );
	m_slotPaint			= m_pFlashBackground->SigPaint().Connect( this, &CTMQRewardGui::OnPaint );

	LinkMsg(g_EventQuestDirectForward);
	LinkMsg(g_EventTMQCleintState);	
	LinkMsg(g_EventTMQNotify);
	LinkMsg(g_EventResize);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CTMQRewardGui::Destroy()
{
	NTL_FUNCTION("CTMQRewardGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	if( m_pNarrationGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pNarrationGui );
		m_pNarrationGui->Destroy();
		NTL_DELETE( m_pNarrationGui );
	}

	if( m_pCounterGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pCounterGui );
		m_pCounterGui->Destroy();
		NTL_DELETE( m_pCounterGui );
	}

	UnLinkMsg(g_EventQuestDirectForward);
	UnLinkMsg(g_EventTMQCleintState);
	UnLinkMsg(g_EventTMQNotify);
	UnLinkMsg(g_EventResize);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTMQRewardGui::CreateInstance()
{
	if( m_pInstance )
		return;

	m_pInstance = NTL_NEW CTMQRewardGui("TMQRewardGui");
	if( !m_pInstance->Create() )
		NTL_DELETE(m_pInstance);

	CNtlPLGuiManager::GetInstance()->AddGui(m_pInstance);	
}

VOID CTMQRewardGui::DestroyInstance()
{
	if( !m_pInstance )
		return;

	GetNtlGuiManager()->RemoveGui(m_pInstance);
	m_pInstance->Destroy();
	NTL_DELETE(m_pInstance);
}

VOID CTMQRewardGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	m_pFlashBackground->Update(fElapsed);
	m_pFlashResult->Update(fElapsed);

	m_NumberZoomHelper.Update(fElapsed);
}

VOID CTMQRewardGui::Init()
{
	m_pConfirmButton->Show(false);
}

VOID CTMQRewardGui::LoadFlash(const char* pcFileName, gui::CFlash* pFlash)
{
	if( pFlash->Load(pcFileName) == false )
	{
		if( pcFileName )
		{
			char acBuffer[256] = "";
			sprintf_s(acBuffer, 256, "CTMQRewardGui::LoadFlash, Not exist %s flash file", pcFileName);
			NtlLogFilePrint(acBuffer);
		}		
	}

	pFlash->PlayMovie(FALSE);

	CRectangle rtRect = pFlash->GetFrameResolution();
	pFlash->SetSize(rtRect.GetWidth(), rtRect.GetHeight());
}

VOID CTMQRewardGui::OnConfirmButton(gui::CComponent* pComponent)
{	
	if( m_pNarrationGui )
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui( m_pNarrationGui );
		m_pNarrationGui->Destroy();
		NTL_DELETE( m_pNarrationGui );
	}


	GetDialogManager()->OpenGMDialog();
	GetDialogManager()->OpenDefaultDialog();
	GetNtlGuiManager()->RemoveUpdateFuncWithoutRefCheck( this );

	SwitchDialog(false);
}

VOID CTMQRewardGui::OnFrameCallback_Result(const char* pcParam, const char* pcParam2)
{
	//DBO_WARNING_MESSAGE("pcParam: " << pcParam);
	//DBO_WARNING_MESSAGE("pcParam2: " << pcParam2);
	if( strcmp(pcParam, "TextEnd1") == 0 )
	{
		m_pFlashResult->Invoke("Mission_Result", "%d", 1);
	}
	else if( strcmp(pcParam, "TextEnd_cleartime") == 0 )
	{
	//	DBO_WARNING_MESSAGE("PLAY ZOOM NUMBER EFFECT");
		m_NumberZoomHelper.RemoveAll();

		m_NumberZoomHelper.AddNumberGui(&m_ExpNumberGui, dNUMEFFECT_START_RATE, dNUMEFFECT_END_RATE, dNUMEFFECT_TIME);

		m_NumberZoomHelper.Play(TRUE);
	}	
}


VOID CTMQRewardGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	CRectangle rtScreen(0, 0, GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());
	CRectangle rtRect;
	RwInt32 iPibotX = rtScreen.GetWidth()/2;
	RwInt32 iPibotY = rtScreen.GetHeight()/2;

	m_pThis->SetPosition(rtScreen);

	m_pConfirmButton->SetPosition(iPibotX + 300, iPibotY + 60);

	// 배경 플래쉬
	rtRect = rtScreen;
	rtRect.top		= (rtScreen.GetHeight() - m_pFlashResult->GetHeight())/2;
	rtRect.bottom	= rtRect.top + m_pFlashBackground->GetHeight();
	m_pFlashBackground->SetPosition(rtRect);

	// 결과 플래쉬
	m_pFlashResult->SetPosition((rtScreen.GetWidth() - m_pFlashResult->GetWidth())/2,
								(rtScreen.GetHeight() - m_pFlashResult->GetHeight())/2);


	rtRect = m_pThis->GetScreenRect();

	m_ExpNumberGui.SetPosition(iPibotX - m_ExpNumberGui.GetWidth(), iPibotY + 20);
	m_srfEXPText.SetPosition(iPibotX - m_srfEXPText.GetWidth() / 2, iPibotY - 40);

	if (m_pCounterGui)
		m_pCounterGui->SetPosition(5, rtScreen.bottom - 125);
}

VOID CTMQRewardGui::OnPaint()
{
	m_ExpNumberGui.Render();
	m_srfEXPText.Render();
}

RwInt32 CTMQRewardGui::SwitchDialog(bool bOpen)
{	
	if( bOpen )
	{
		Show(true);
	}
	else
	{
		Show(false);
	}

	return 1;
}

VOID CTMQRewardGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CTMQRewardGui::HandleEvents");

	if( msg.Id == g_EventQuestDirectForward )
	{
		const SNtlEventQuestDirect_Forward* pEvent = reinterpret_cast<const SNtlEventQuestDirect_Forward*>( msg.pData );

		if( pEvent->eTDType != ETD_QuestReward )
			return;

		if( pEvent->sReward.eRewardType != eREWARD_CONTAINER_TYPE_TMQ )
			return;

		Init();

		m_ExpNumberGui.SetNumber(pEvent->sReward.uiRewardExp); // to do tmq exp reward

		LoadFlash("TMQ_result_bg.swf", m_pFlashBackground);
		LoadFlash("TMQ_result.swf", m_pFlashResult);

		m_pFlashBackground->PlayMovie(true);
		m_pFlashResult->PlayMovie(true);

		m_pConfirmButton->Show(true);

		// As soon as the reward window is opened, all other dialogs are closed
		GetDialogManager()->CloseAll();
		SwitchDialog(true);

		// NPC 나레이션
		if( !m_pNarrationGui )
		{
			m_pNarrationGui = NTL_NEW CResultNarrationGui("TMQNarrationGui");
			if( !m_pNarrationGui->Create() )
			{
				m_pNarrationGui->Destroy();
				NTL_DELETE(m_pNarrationGui);
			}
			CNtlPLGuiManager::GetInstance()->AddGui( m_pNarrationGui );

			const WCHAR* pwcText = GetDisplayStringManager()->GetString("DST_TMQ_RESULT_NARRATION");
			if( pwcText )
				m_pNarrationGui->SetData("TMQ_result_npc.swf", pwcText);
		}

		OnMove(0, 0);
	}
	else if( msg.Id == g_EventTMQNotify )
	{
		SNtlEventTMQNotify* pEvent = reinterpret_cast<SNtlEventTMQNotify*>( msg.pData );

		// 제한시간
		if( pEvent->uiEventType == TET_TIME_UNTIL_TELEPORT_TO_MAINWORLD )
		{
			float fLimitedTime = (float)pEvent->uiValue/1000.f;
			GetNtlGuiManager()->AddUpdateFunc( this );

			// bottom left countdown
			m_pCounterGui = NTL_NEW CCounterGui("CounterGui");
			if (!m_pCounterGui->Create(COUNTER_DISPLAY_2, fLimitedTime, COUNTER_QUADRANT_4))
			{
				m_pCounterGui->Destroy();
				NTL_DELETE(m_pCounterGui);
			}
			CNtlPLGuiManager::GetInstance()->AddGui(m_pCounterGui);
			m_pCounterGui->AlarmSound(TRUE, fLimitedTime);

			m_pCounterGui->SetPosition(5, GetDboGlobal()->GetScreenHeight() - 125);
		}
	}
	else if( msg.Id == g_EventTMQCleintState )
	{
		SNtlEventTMQServerState* pEvent = reinterpret_cast<SNtlEventTMQServerState*>( msg.pData );

		if( pEvent->eWorldConcept != WORLD_PLAY_TIME_MACHINE )
			NTL_RETURNVOID();

		if( pEvent->uiState == WORLD_STATE_TMQ_FAIL)
			SwitchDialog(false);
	}
	else if (msg.Id == g_EventResize)
	{
		if (IsShow())
		{
			OnMove(0, 0);
		}
	}

	NTL_RETURNVOID();
}