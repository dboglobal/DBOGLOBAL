#include "precomp_dboclient.h"
#include "TimeNotifyGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLEvent.h"
#include "NtlPLGuiManager.h"

// simulation
#include "DboTSCQCtrl.h"
#include "DboTSCQAgency.h"
#include "NtlSLApi.h"
#include "NtlWorldConceptTB.h"

// dbo
#include "DialogManager.h"
#include "DboEventGenerator.h"
#include "Dboglobal.h"

namespace
{
	#define dGUI_YPOS			126
	#define dTIME_LIMIT_GAP		2
};

//#define BUDOKAI_TIME_DEBUG
//#define RANKBATTLE_TIME_DEBUG

CTimeNotifyGui::CTimeNotifyGui(const RwChar* pName)
:CNtlPLGui(pName)
{
}

CTimeNotifyGui::~CTimeNotifyGui()
{
	Destroy();
}

RwBool CTimeNotifyGui::Create()
{
	NTL_FUNCTION( "CTimeNotifyGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\TimeNotify.frm"))
		NTL_RETURN( FALSE  );

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_NOTIFY);

	CRectangle rect;

	// 분
	m_MinuteNumber[DISPLAY_TYPE_NORMAL].Create(ND_LEFT, NVD_TOP, 0, dNCFLAG_NONE, 2);
	m_MinuteNumber[DISPLAY_TYPE_NORMAL].SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_0" ));
	m_MinuteNumber[DISPLAY_TYPE_NORMAL].SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_1" ));
	m_MinuteNumber[DISPLAY_TYPE_NORMAL].SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_2" ));
	m_MinuteNumber[DISPLAY_TYPE_NORMAL].SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_3" ));
	m_MinuteNumber[DISPLAY_TYPE_NORMAL].SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_4" ));
	m_MinuteNumber[DISPLAY_TYPE_NORMAL].SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_5" ));
	m_MinuteNumber[DISPLAY_TYPE_NORMAL].SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_6" ));
	m_MinuteNumber[DISPLAY_TYPE_NORMAL].SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_7" ));
	m_MinuteNumber[DISPLAY_TYPE_NORMAL].SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_8" ));
	m_MinuteNumber[DISPLAY_TYPE_NORMAL].SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_9" ));
	m_MinuteNumber[DISPLAY_TYPE_NORMAL].SetNumGap(-5);

	m_MinuteNumber[DISPLAY_TYPE_WARNING].Create(ND_LEFT, NVD_TOP, 0, dNCFLAG_NONE, 2);
	m_MinuteNumber[DISPLAY_TYPE_WARNING].SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_0" ));
	m_MinuteNumber[DISPLAY_TYPE_WARNING].SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_1" ));
	m_MinuteNumber[DISPLAY_TYPE_WARNING].SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_2" ));
	m_MinuteNumber[DISPLAY_TYPE_WARNING].SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_3" ));
	m_MinuteNumber[DISPLAY_TYPE_WARNING].SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_4" ));
	m_MinuteNumber[DISPLAY_TYPE_WARNING].SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_5" ));
	m_MinuteNumber[DISPLAY_TYPE_WARNING].SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_6" ));
	m_MinuteNumber[DISPLAY_TYPE_WARNING].SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_7" ));
	m_MinuteNumber[DISPLAY_TYPE_WARNING].SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_8" ));
	m_MinuteNumber[DISPLAY_TYPE_WARNING].SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_9" ));
	m_MinuteNumber[DISPLAY_TYPE_WARNING].SetNumGap(-5);

	// 처음에 이미지들이 배치되지 않은 상태로 화면에 그려지는 경우가 있어서 임의의 숫자로 초기화 해준다
	m_MinuteNumber[DISPLAY_TYPE_NORMAL].SetNumber(0);
	m_MinuteNumber[DISPLAY_TYPE_WARNING].SetNumber(0);

	// 초
	m_SecondNumber[DISPLAY_TYPE_NORMAL].Create(ND_RIGHT, NVD_TOP, 0, dNCFLAG_NONE, 2);
	m_SecondNumber[DISPLAY_TYPE_NORMAL].SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_0" ));
	m_SecondNumber[DISPLAY_TYPE_NORMAL].SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_1" ));
	m_SecondNumber[DISPLAY_TYPE_NORMAL].SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_2" ));
	m_SecondNumber[DISPLAY_TYPE_NORMAL].SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_3" ));
	m_SecondNumber[DISPLAY_TYPE_NORMAL].SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_4" ));
	m_SecondNumber[DISPLAY_TYPE_NORMAL].SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_5" ));
	m_SecondNumber[DISPLAY_TYPE_NORMAL].SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_6" ));
	m_SecondNumber[DISPLAY_TYPE_NORMAL].SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_7" ));
	m_SecondNumber[DISPLAY_TYPE_NORMAL].SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_8" ));
	m_SecondNumber[DISPLAY_TYPE_NORMAL].SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_9" ));
	m_SecondNumber[DISPLAY_TYPE_NORMAL].SetNumGap(-5);

	m_SecondNumber[DISPLAY_TYPE_WARNING].Create(ND_RIGHT, NVD_TOP, 0, dNCFLAG_NONE, 2);
	m_SecondNumber[DISPLAY_TYPE_WARNING].SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_0" ));
	m_SecondNumber[DISPLAY_TYPE_WARNING].SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_1" ));
	m_SecondNumber[DISPLAY_TYPE_WARNING].SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_2" ));
	m_SecondNumber[DISPLAY_TYPE_WARNING].SetSurface(3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_3" ));
	m_SecondNumber[DISPLAY_TYPE_WARNING].SetSurface(4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_4" ));
	m_SecondNumber[DISPLAY_TYPE_WARNING].SetSurface(5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_5" ));
	m_SecondNumber[DISPLAY_TYPE_WARNING].SetSurface(6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_6" ));
	m_SecondNumber[DISPLAY_TYPE_WARNING].SetSurface(7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_7" ));
	m_SecondNumber[DISPLAY_TYPE_WARNING].SetSurface(8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_8" ));
	m_SecondNumber[DISPLAY_TYPE_WARNING].SetSurface(9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_9" ));
	m_SecondNumber[DISPLAY_TYPE_WARNING].SetNumGap(-5);

	// 처음에 이미지들이 배치되지 않은 상태로 화면에 그려지는 경우가 있어서 임의의 숫자로 초기화 해준다
	m_SecondNumber[DISPLAY_TYPE_NORMAL].SetNumber(0);
	m_SecondNumber[DISPLAY_TYPE_WARNING].SetNumber(0);

	// 콜론
	m_srfColon[DISPLAY_TYPE_NORMAL].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimit_colon" ) );

	m_srfColon[DISPLAY_TYPE_WARNING].SetSurface( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "CommonNumber.srf", "srfTimeLimitWarning_colon" ) );

	// default
	m_TimeInfo.byType			= TIME_TYPE_INVALID;
	m_TimeInfo.fElapsed			= 0.f;
	m_TimeInfo.sTSKey.tID		= NTL_TS_T_ID_INVALID;
	m_TimeInfo.uiSortType		= (RwUInt32)eQUEST_SORT_TYPE_INVALID;

	GetNtlGuiManager()->AddUpdateFunc( this );

	// sig	
	m_slotPaint			= m_pThis->SigPaint().Connect( this, &CTimeNotifyGui::OnPaint );

	Show(false);

	LinkMsg(g_EventRegQuest_Nfy);
	LinkMsg(g_EventUpdateQuestProgressInfo_Nfy);
	LinkMsg(g_EventGiveUpQuest);
	LinkMsg(g_EventUnregQuest_Nfy);
	LinkMsg(g_EventTSRemovingTMQQuest_Nfy);
	LinkMsg(g_EventChangeWorldConceptState);
	LinkMsg(g_EventRBBattleStateUpdate);
	LinkMsg(g_EventRBBattleStageFinish);
	LinkMsg(g_EventMinorMatchStateUpdate);
	LinkMsg(g_EventMinorMatchStageFinish);
	LinkMsg(g_EventMajorMatchStateUpdate);
	LinkMsg(g_EventMajorMatchStageFinish);
	LinkMsg(g_EventFinalMatchStateUpdate);
	LinkMsg(g_EventFinalMatchStageFinish);
	LinkMsg(g_EventResize);

	NTL_RETURN(TRUE);
}

VOID CTimeNotifyGui::Destroy()
{
	NTL_FUNCTION("CTimeNotifyGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	UnLinkMsg(g_EventRegQuest_Nfy);
	UnLinkMsg(g_EventUpdateQuestProgressInfo_Nfy);
	UnLinkMsg(g_EventGiveUpQuest);
	UnLinkMsg(g_EventUnregQuest_Nfy);
	UnLinkMsg(g_EventTSRemovingTMQQuest_Nfy);
	UnLinkMsg(g_EventChangeWorldConceptState);
	UnLinkMsg(g_EventRBBattleStateUpdate);
	UnLinkMsg(g_EventRBBattleStageFinish);
	UnLinkMsg(g_EventMinorMatchStateUpdate);
	UnLinkMsg(g_EventMinorMatchStageFinish);
	UnLinkMsg(g_EventMajorMatchStateUpdate);
	UnLinkMsg(g_EventMajorMatchStageFinish);
	UnLinkMsg(g_EventFinalMatchStateUpdate);
	UnLinkMsg(g_EventFinalMatchStageFinish);
	UnLinkMsg(g_EventResize);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CTimeNotifyGui::Update(RwReal fElapsed)
{
	if( !IsShow() )
		return;

	m_TimeInfo.fElapsed -= fElapsed;

	if( m_TimeInfo.fElapsed < 0.f )
		m_TimeInfo.fElapsed = 0.f;

	SetDisplayType();

	m_MinuteNumber[m_byDisplayType].SetNumber((RwInt32)m_TimeInfo.fElapsed/60);
	m_SecondNumber[m_byDisplayType].SetNumber((RwInt32)m_TimeInfo.fElapsed%60);

	if( m_TimeInfo.fElapsed == 0.f )
		GetDialogManager()->CloseDialog(DIALOG_TIME_NOTIFY);
}

VOID CTimeNotifyGui::SetDisplayType()
{
	if( m_TimeInfo.fElapsed >= 60.f )
		m_byDisplayType = DISPLAY_TYPE_NORMAL;
	else
		m_byDisplayType = DISPLAY_TYPE_WARNING;
}

VOID CTimeNotifyGui::LocateComponent()
{
	SetDisplayType();

	if( m_TimeInfo.byType == TIME_TYPE_TMQ_QUEST_LIMIT_TIME ||
		m_TimeInfo.byType == TIME_TYPE_RANK_BATTLE_LIMIT_TIME ||
		m_TimeInfo.byType == TIME_TYPE_BUDOKAI_LIMIT_TIME )
	{
		RwInt32 iCleintWidth = GetDboGlobal()->GetScreenWidth();
		RwInt32 iXPos;
		RwInt32 iYPos = dGUI_YPOS;
		
		iXPos = (iCleintWidth - m_srfColon[m_byDisplayType].GetWidth())/2 - dTIME_LIMIT_GAP;

		m_MinuteNumber[DISPLAY_TYPE_NORMAL].SetPosition(iXPos, iYPos);
		m_MinuteNumber[DISPLAY_TYPE_WARNING].SetPosition(iXPos, iYPos);

		iXPos += dTIME_LIMIT_GAP;
		m_srfColon[DISPLAY_TYPE_NORMAL].SetPosition(iXPos, iYPos);
		m_srfColon[DISPLAY_TYPE_WARNING].SetPosition(iXPos, iYPos);

		iXPos += m_srfColon[DISPLAY_TYPE_WARNING].GetWidth() + dTIME_LIMIT_GAP;
		m_SecondNumber[DISPLAY_TYPE_NORMAL].SetPosition(iXPos, iYPos);
		m_SecondNumber[DISPLAY_TYPE_WARNING].SetPosition(iXPos, iYPos);
	}	
}

VOID CTimeNotifyGui::OnPaint()
{
	if( m_TimeInfo.byType == TIME_TYPE_TMQ_QUEST_LIMIT_TIME ||
		m_TimeInfo.byType == TIME_TYPE_RANK_BATTLE_LIMIT_TIME ||
		m_TimeInfo.byType == TIME_TYPE_BUDOKAI_LIMIT_TIME )
	{		
		m_MinuteNumber[m_byDisplayType].Render();
		m_srfColon[m_byDisplayType].Render();
		m_SecondNumber[m_byDisplayType].Render();
	}	
}

RwInt32 CTimeNotifyGui::SwitchDialog(bool bOpen)
{	
	if( bOpen )
	{
		Show(true);
	}
	else
	{
		Show(false);

		m_TimeInfo.byType		= TIME_TYPE_INVALID;
		m_TimeInfo.fElapsed		= 0.f;

		m_TimeInfo.sTSKey.tID	= NTL_TS_T_ID_INVALID;
		m_TimeInfo.uiSortType	= (RwUInt32)eQUEST_SORT_TYPE_INVALID;
	}
	
	return 1;
}

VOID CTimeNotifyGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CTimeNotifyGui::HandleEvents");

	if( msg.Id == g_EventRegQuest_Nfy )
	{
		SNtlEventRegQuest_Nfy* pEvent = reinterpret_cast<SNtlEventRegQuest_Nfy*>(msg.pData);

		CDboTSCQAgency* pQuestAgency = API_GetQuestAgency();

		if ( NULL == pQuestAgency ) NTL_RETURNVOID();

		if( pEvent->eQuestSortType == eQUEST_SORT_TYPE_TIMEMACHINE )
		{
			CDboTSCQCtrl* pQuestCtr = reinterpret_cast<CDboTSCQCtrl*>(pQuestAgency->FindProgressTrigger( pEvent->sTSKey.tID ));
			NTL_ASSERT(pQuestCtr, "CTimeNotifyGui::HandleEvents, Not exist ts data of id : " << pEvent->sTSKey.tID);

			if( pQuestCtr->GetLimitTime() != 0xffffffff )
			{
				m_TimeInfo.byType			= TIME_TYPE_TMQ_QUEST_LIMIT_TIME;
				m_TimeInfo.fElapsed			= (RwReal)pQuestCtr->GetLimitTime() / 1000.f;
				m_TimeInfo.sTSKey			= pEvent->sTSKey;
				m_TimeInfo.uiSortType		= eQUEST_SORT_TYPE_TIMEMACHINE;
				
				LocateComponent();
				GetDialogManager()->OpenDialog(DIALOG_TIME_NOTIFY);
			}
		}
	}
	else if( msg.Id == g_EventUpdateQuestProgressInfo_Nfy )
	{
		if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_TIME_MACHINE) == FALSE )
			NTL_RETURNVOID();


		SNtlEventUpdateQuestProgressInfo_Nfy* pEvent = reinterpret_cast<SNtlEventUpdateQuestProgressInfo_Nfy*>(msg.pData);

		if( pEvent->uiTimeLimit != 0xffffffff )
		{
			m_TimeInfo.byType			= TIME_TYPE_TMQ_QUEST_LIMIT_TIME;
			m_TimeInfo.fElapsed			= (RwReal)pEvent->uiTimeLimit / 1000.f;
			m_TimeInfo.sTSKey			= pEvent->sTSKey;
			m_TimeInfo.uiSortType		= eQUEST_SORT_TYPE_TIMEMACHINE;

			LocateComponent();
			GetDialogManager()->OpenDialog(DIALOG_TIME_NOTIFY);
		}
	}
	else if ( msg.Id == g_EventGiveUpQuest )
	{
		SNtlEventUpdateQuestProgressInfo_Nfy* pEvent = reinterpret_cast<SNtlEventUpdateQuestProgressInfo_Nfy*>(msg.pData);

		if( m_TimeInfo.sTSKey.tID == pEvent->sTSKey.tID )
			GetDialogManager()->CloseDialog(DIALOG_TIME_NOTIFY);
	}
	else if( msg.Id == g_EventUnregQuest_Nfy )
	{
		SNtlEventUnregQuest_Nfy* pEvent = reinterpret_cast<SNtlEventUnregQuest_Nfy*>(msg.pData);

		if( m_TimeInfo.sTSKey.tID == pEvent->sTSKey.tID )
			GetDialogManager()->CloseDialog(DIALOG_TIME_NOTIFY);
	}
	else if( msg.Id == g_EventTSRemovingTMQQuest_Nfy )
	{
		SNtlEventTSRemovingTMQQuest_Nfy* pEvent = reinterpret_cast<SNtlEventTSRemovingTMQQuest_Nfy*>( msg.pData );

		if( m_TimeInfo.sTSKey.tID == pEvent->tRmvTSId )
			GetDialogManager()->CloseDialog(DIALOG_TIME_NOTIFY);
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		if( pEvent->eWorldConcept != WORLD_PLAY_TIME_MACHINE )
			NTL_RETURNVOID();

		if( pEvent->uiState != WORLD_STATE_EXIT )
			NTL_RETURNVOID();

		if( m_TimeInfo.byType == TIME_TYPE_TMQ_QUEST_LIMIT_TIME )
			GetDialogManager()->CloseDialog(DIALOG_TIME_NOTIFY);
	}
	else if( msg.Id == g_EventRBBattleStateUpdate )
	{
		if( GetNtlWorldConcept()->IsActivePlayConcept( WORLD_PLAY_RANK_BATTLE ) )
		{
			SNtlEventRBBattleStateUpdate* pData = reinterpret_cast<SNtlEventRBBattleStateUpdate*>( msg.pData );
			sRANKBATTLE_TBLDAT* pTblDat = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle()->GetTblData();

			if( pData->byBattleState == RANKBATTLE_BATTLESTATE_STAGE_RUN) // battle duration timer
			{
				m_TimeInfo.byType			= TIME_TYPE_RANK_BATTLE_LIMIT_TIME;
				m_TimeInfo.fElapsed			= (RwReal)( pTblDat->dwStageRunTime );

				LocateComponent();
				GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
			}	
#ifdef RANKBATTLE_TIME_DEBUG
			else if( pData->byBattleState == RANKBATTLE_BATTLESTATE_WAIT )
			{
				m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
				m_TimeInfo.fElapsed = (RwReal)( pTblDat->dwWaitTime ) + 0.99f;

				LocateComponent();
				GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
			}
			else if( pData->byBattleState == RANKBATTLE_BATTLESTATE_DIRECTION )
			{
				m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
				m_TimeInfo.fElapsed = (RwReal)( pTblDat->dwDirectionTime ) + 0.99f;

				LocateComponent();
				GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
			}			
			else if( pData->byBattleState == RANKBATTLE_BATTLESTATE_STAGE_PREPARE)
			{
				m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
				m_TimeInfo.fElapsed = (RwReal)( pTblDat->dwStageReadyTime ) + 0.99f;

				LocateComponent();
				GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
			}
			else if( pData->byBattleState == RANKBATTLE_BATTLESTATE_STAGE_RUN)
			{
				m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
				m_TimeInfo.fElapsed = (RwReal)( pTblDat->dwStageFinishTime ) + 0.99f;

				LocateComponent();
				GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
			}
			else if( pData->byBattleState == RANKBATTLE_BATTLESTATE_STAGE_FINISH)
			{
				m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
				m_TimeInfo.fElapsed = (RwReal)( pTblDat->dwMatchFinishTime ) + 0.99f;

				LocateComponent();
				GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
			}			
			else if( pData->byBattleState == RANKBATTLE_BATTLESTATE_END )
			{
				if( m_TimeInfo.byType == TIME_TYPE_BUDOKAI_LIMIT_TIME )
					GetDialogManager()->CloseDialog( DIALOG_TIME_NOTIFY );
			}
#endif
		}
	}
	else if( msg.Id == g_EventRBBattleStageFinish )
	{
#ifndef RANKBATTLE_TIME_DEBUG
		if( m_TimeInfo.byType == TIME_TYPE_RANK_BATTLE_LIMIT_TIME )
			GetDialogManager()->CloseDialog( DIALOG_TIME_NOTIFY );
#endif
	}
	else if( msg.Id == g_EventMinorMatchStateUpdate )
	{
		SNtlEventMinorMatchStateUpdate* pData = reinterpret_cast<SNtlEventMinorMatchStateUpdate*>( msg.pData );

		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		if( pData->bEnterState && pData->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_RUN )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwStageRunTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
#ifdef BUDOKAI_TIME_DEBUG
		else if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_WAIT )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwWaitTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_DIRECTION )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwDirectionTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_MATCH_READY )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwMatchReadyTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_READY )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pData->dwRemainTime / 1000.f );

			//// 관전자인지 참가자인지 판별
			//stTBudokaiMember* pMember = pTBWorldConcept->FindMember( Logic_GetAvatarHandle() );
			//if( pMember )
			//	m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwStageReadyTime ) + 0.99f;
			//else
				
			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_STAGE_FINISH )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwStageFinishTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_MATCH_FINISH )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwMatchFinishTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_WAIT_MINORMATCH )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai()->GetBudokaiTblData()->dwMinorMatch_WaitTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_MINORMATCH_STATE_END )
		{
			if( m_TimeInfo.byType == TIME_TYPE_BUDOKAI_LIMIT_TIME )
				GetDialogManager()->CloseDialog( DIALOG_TIME_NOTIFY );
		}
#endif
	}
	else if( msg.Id == g_EventMinorMatchStageFinish )
	{
#ifndef BUDOKAI_TIME_DEBUG
		if( m_TimeInfo.byType == TIME_TYPE_BUDOKAI_LIMIT_TIME )
			GetDialogManager()->CloseDialog( DIALOG_TIME_NOTIFY );
#endif
	}
	else if( msg.Id == g_EventMajorMatchStateUpdate )
	{
		SNtlEventMajorMatchStateUpdate* pData = reinterpret_cast<SNtlEventMajorMatchStateUpdate*>( msg.pData );

		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		if( pData->bEnterState && pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_RUN )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwStageRunTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
#ifdef BUDOKAI_TIME_DEBUG
		else if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_WAIT )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwWaitTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_DIRECTION )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwDirectionTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_MATCH_READY )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwMatchReadyTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_READY )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwStageReadyTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_STAGE_FINISH )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwStageFinishTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_MATCH_FINISH )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwMatchFinishTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_MAJORMATCH_STATE_END )
		{
			if( m_TimeInfo.byType == TIME_TYPE_BUDOKAI_LIMIT_TIME )
				GetDialogManager()->CloseDialog( DIALOG_TIME_NOTIFY );
		}
#endif
	}
	else if( msg.Id == g_EventMajorMatchStageFinish )
	{
		SNtlEventMajorMatchStageFinish* pData = reinterpret_cast<SNtlEventMajorMatchStageFinish*>( msg.pData );

#ifndef BUDOKAI_TIME_DEBUG
		// 옵저버 모드일 경우 경기 중간에도 나올 수 있다.
		if( !pData->bIsObserver )
		{
			if( m_TimeInfo.byType == TIME_TYPE_BUDOKAI_LIMIT_TIME )
				GetDialogManager()->CloseDialog( DIALOG_TIME_NOTIFY );
		}
#endif
	}
	else if( msg.Id == g_EventFinalMatchStateUpdate )
	{
		SNtlEventFinalMatchStateUpdate* pData = reinterpret_cast<SNtlEventFinalMatchStateUpdate*>( msg.pData );

		CNtlWorldConceptTB* pTBWorldConcept = reinterpret_cast<CNtlWorldConceptTB*>( GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_T_BUDOKAI ) );
		NTL_ASSERT( pTBWorldConcept, "CNtlTBudokai::HandleEvents : must World concept is valid" );		

		if( pData->bEnterState && pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_RUN )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwStageRunTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
#ifdef BUDOKAI_TIME_DEBUG
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_WAIT )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwWaitTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_DIRECTION )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwDirectionTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_MATCH_READY )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwMatchReadyTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_READY )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwStageReadyTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_STAGE_FINISH )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwStageFinishTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_MATCH_FINISH )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( pTBWorldConcept->GetMatchTbl()->dwMatchFinishTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_FINALDIRECTION )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai()->GetBudokaiTblData()->dwFinalMatchDirectionTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_AWARDING )
		{
			m_TimeInfo.byType	= TIME_TYPE_BUDOKAI_LIMIT_TIME;
			m_TimeInfo.fElapsed = (RwReal)( GetNtlSLGlobal()->GetSobAvatar()->GetTenkaichiBudokai()->GetBudokaiTblData()->dwAwardingTime ) + 0.99f;

			LocateComponent();
			GetDialogManager()->OpenDialog( DIALOG_TIME_NOTIFY );
		}
		else if( pData->byMatchState == BUDOKAI_FINALMATCH_STATE_END )
		{
			if( m_TimeInfo.byType == TIME_TYPE_BUDOKAI_LIMIT_TIME )
				GetDialogManager()->CloseDialog( DIALOG_TIME_NOTIFY );
		}
#endif
	}
	else if( msg.Id == g_EventFinalMatchStageFinish )
	{
		SNtlEventFinalMatchStageFinish* pData = reinterpret_cast<SNtlEventFinalMatchStageFinish*>(msg.pData);
#ifndef BUDOKAI_TIME_DEBUG
		if( !pData->bIsObserver )
		{
			if( m_TimeInfo.byType == TIME_TYPE_BUDOKAI_LIMIT_TIME )
				GetDialogManager()->CloseDialog( DIALOG_TIME_NOTIFY );		
		}
#endif
	}
	else if( msg.Id == g_EventResize )
	{
		//SNtlPLEventResize* pEvent = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		LocateComponent();
	}

	NTL_RETURNVOID();
}