#include "precomp_dboclient.h"
#include "FlashNotifyGui.h"

// core
#include "NtlDebug.h"

// presetation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// simulation
#include "NtlSLTMQ.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlWorldConceptRB.h"

// dbo
#include "DboEvent.h"
#include "DialogManager.h"
#include "DboGlobal.h"
#include "DboEventGenerator.h"

namespace
{
	RwUInt8 byTempProgressType;
	RwUInt8 byTempMessageValue;
	RwUInt8 byTempMessageType;
	std::string TempString;
	std::string TempString2;
}


CFlashNotifyGui::CFlashNotifyGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_bFinishNotify(TRUE)
{
}

CFlashNotifyGui::~CFlashNotifyGui()
{

}

RwBool CFlashNotifyGui::Create()
{
	NTL_FUNCTION( "CFlashNotifyGui::Create" );

	if(!CNtlPLGui::Create("", "", "gui\\FlashNotify.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(dDIALOGPRIORITY_NOTIFY);

	// 측정시의 배경
	m_pFlashBackground = (gui::CFlash*)GetComponent("flaBackground");
	
	// sig	
	m_slotMovieEnd		= m_pFlashBackground->SigMovieEnd().Connect( this, &CFlashNotifyGui::OnMovieEnd );

	GetNtlGuiManager()->AddUpdateFunc( this );

	Show(false);

	LinkMsg(g_EventFlashNotify);
	LinkMsg(g_EventFlashNotifyString);
	LinkMsg(g_EventRBBattleStateUpdate);
	//LinkMsg(g_EventRBBattleStageFinish);
	//LinkMsg(g_EventRBBattleMatchFinish); 
	LinkMsg(g_EventResize);
	LinkMsg(g_EventTMQCleintState);
	LinkMsg(g_EventRBForcedEnd);
	LinkMsg(g_EventRBBattleEnd);
    LinkMsg(g_EventSobConvertClass);
    LinkMsg(g_EventSobInfoUpdate);
	LinkMsg(g_EventUpdateQuestState_Nfy);
	LinkMsg(g_EventQuestAcceptReward_Nfy);
	LinkMsg(g_EventChangeWorldConceptState);	
	LinkMsg(g_EventHoipoiMixInfoUpdate);

	NTL_RETURN(TRUE);
}

VOID CFlashNotifyGui::Destroy()
{
	NTL_FUNCTION("CFlashNotifyGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFunc( this );

	m_pFlashBackground->Unload();

	UnLinkMsg(g_EventFlashNotify);
	UnLinkMsg(g_EventFlashNotifyString);
	UnLinkMsg(g_EventRBBattleStateUpdate);
	//UnLinkMsg(g_EventRBBattleStageFinish);
	//UnLinkMsg(g_EventRBBattleMatchFinish);
	UnLinkMsg(g_EventResize);
	UnLinkMsg(g_EventTMQCleintState);
	UnLinkMsg(g_EventRBForcedEnd);
	UnLinkMsg(g_EventRBBattleEnd);
    UnLinkMsg(g_EventSobConvertClass);
    UnLinkMsg(g_EventSobInfoUpdate);
	UnLinkMsg(g_EventUpdateQuestState_Nfy);
	UnLinkMsg(g_EventQuestAcceptReward_Nfy);
	UnLinkMsg(g_EventChangeWorldConceptState);
	UnLinkMsg(g_EventHoipoiMixInfoUpdate);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CFlashNotifyGui::Update(RwReal fElapsed)
{
	if( IsShow() == FALSE )
		return;

	m_pFlashBackground->Update(fElapsed);
}

RwBool CFlashNotifyGui::LoadFlash(const char* acFileName, RwBool bFrameCallback /* = FALSE */)
{
	if( m_pFlashBackground->Load(acFileName) == false )
	{
		NTL_ASSERT(false, "CFlashNotifyGui::LoadFlash, Not eixst file : " << acFileName << ". You can continue the game.");
		FinishFlash();
		
		return FALSE;
	}
	
	LocateComponent();
	m_pFlashBackground->SetFrameCallback(bFrameCallback); 
	m_pFlashBackground->PlayMovie(TRUE);

	return TRUE;
}

VOID CFlashNotifyGui::LocateComponent()
{
	CRectangle rtFlash = m_pFlashBackground->GetFrameResolution();
	RwInt32 iWidth = rtFlash.GetWidth();
	RwInt32 iHeight = rtFlash.GetHeight();

	rtFlash.left	= ((RwInt32)GetDboGlobal()->GetScreenWidth() - iWidth)/2;
	rtFlash.right	= rtFlash.left + iWidth;
	rtFlash.top		= ((RwInt32)GetDboGlobal()->GetScreenHeight() - iHeight)/2;
	rtFlash.bottom	= rtFlash.top + iHeight;

	m_pFlashBackground->SetPosition(rtFlash);
}

VOID  CFlashNotifyGui::FinishFlash()
{
	m_pFlashBackground->Unload();
	m_strSecondFile.clear();

	m_bFinishNotify = TRUE;
}

VOID CFlashNotifyGui::OnMovieEnd(gui::CComponent* pComponent)
{
	if( m_strSecondFile.size() > 0 )
	{
		m_pFlashBackground->Unload();

		if( LoadFlash(m_strSecondFile.c_str()) )
			m_strSecondFile.clear();
	}
	else
	{
		m_pFlashBackground->Unload();

		GetDialogManager()->CloseDialog(DIALOG_FLASH_NOTIFY);

		if( m_bFinishNotify )
		{
			CDboEventGenerator::FlashFinishNotify(byTempProgressType, byTempMessageValue, byTempMessageType,
												 (char*)TempString.c_str(),
												 (char*)TempString2.c_str());

			byTempProgressType = dINVALID_FLASH_NOTIFY_FILE_INDEX;
			byTempMessageValue = dINVALID_FLASH_NOTIFY_FILE_INDEX;;
			byTempMessageType = dINVALID_FLASH_NOTIFY_FILE_INDEX;
			TempString = "";
			TempString2 = "";
		}
	}
}

RwInt32 CFlashNotifyGui::SwitchDialog(bool bOpen)
{	
	if( bOpen )
	{
		Show(true);
	}
	else
	{
		Show(false);
		FinishFlash();
	}

	return 1;
}

VOID CFlashNotifyGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION( "CFlashNotifyGui::HandleEvents" );

	if( msg.Id == g_EventFlashNotify )
	{
		SDboEventFlashNotify* pEvent = reinterpret_cast<SDboEventFlashNotify*>( msg.pData );

		if( pEvent->byProgressType == dINVALID_FLASH_NOTIFY_FILE_INDEX &&			
			pEvent->byMessageValue == dINVALID_FLASH_NOTIFY_FILE_INDEX &&
			pEvent->byMessageType == dINVALID_FLASH_NOTIFY_FILE_INDEX )
		{
			FinishFlash();
		}
		else
		{
			char acFileName[256] = "";
			sprintf_s(acFileName, 256, "fn%03d%03d.swf", pEvent->byProgressType, pEvent->byMessageValue);

			GetDialogManager()->OpenDialog(DIALOG_FLASH_NOTIFY);

			LoadFlash(acFileName);

			if( pEvent->byMessageType != dINVALID_FLASH_NOTIFY_FILE_INDEX )
			{
				sprintf_s(acFileName, 256, "fs%03d.swf", pEvent->byMessageType);
				m_strSecondFile = acFileName;
			}

			byTempProgressType = pEvent->byProgressType;
			byTempMessageValue = pEvent->byMessageValue;;
			byTempMessageType = pEvent->byMessageType;
		}		
	}
	else if( msg.Id == g_EventFlashNotifyString )
	{
		SDboEventFlashNotifyString* pEvent = reinterpret_cast<SDboEventFlashNotifyString*>( msg.pData );

		if( pEvent->pcFlashFile )
		{
			GetDialogManager()->OpenDialog(DIALOG_FLASH_NOTIFY);

			LoadFlash(pEvent->pcFlashFile);

			if( pEvent->pcSecondFlashFile && strlen(pEvent->pcSecondFlashFile) > 0 )
				m_strSecondFile = pEvent->pcSecondFlashFile;			

			TempString			= pEvent->pcFlashFile;
			TempString2			= m_strSecondFile;
			m_bFinishNotify		= pEvent->bFinishNotify;
		}
		else
		{
			FinishFlash();
		}
	}
	else if( msg.Id == g_EventRBBattleStateUpdate )
	{
		SNtlEventRBBattleStateUpdate* pData = reinterpret_cast<SNtlEventRBBattleStateUpdate*>( msg.pData );

		CNtlWorldConceptRB* pRBWorldConcept = reinterpret_cast<CNtlWorldConceptRB*>(GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_RANK_BATTLE));

		switch( pData->byBattleState )
		{
			case RANKBATTLE_BATTLESTATE_WAIT:
			{
				GetDialogManager()->OpenDialog(DIALOG_FLASH_NOTIFY);
				LoadFlash("TMQ_PleaseWait.swf");
			}
			break;
			case RANKBATTLE_BATTLESTATE_DIRECTION:
			{
				FinishFlash();
			}
			break;
			case RANKBATTLE_BATTLESTATE_STAGE_PREPARE:
			{
				FinishFlash();

				if (pData->byStage == 0)
				{
					pRBWorldConcept->SetFirstMatchStart(true);
				}
			}
			break;
			case RANKBATTLE_BATTLESTATE_STAGE_READY:
			{
				RwUInt8 byStage = pData->byStage + 1;

				if (byStage > 5)
					byStage = 5;

				if (byStage < 1)
					byStage = 1;

				RwChar buf[GUI_TEXT_BUFFER_SIZE];
				sprintf_s(buf, GUI_TEXT_BUFFER_SIZE, "RankBattle_Round_%d.swf", byStage);

				GetDialogManager()->OpenDialog(DIALOG_FLASH_NOTIFY);
				LoadFlash(buf);
			}
			break;
			case RANKBATTLE_BATTLESTATE_STAGE_RUN:
			{
				FinishFlash();

				GetDialogManager()->OpenDialog(DIALOG_FLASH_NOTIFY);
				LoadFlash("RankBattle_Fight.swf", TRUE);
			}
			break;
		}
	}
	//else if( msg.Id == g_EventRBBattleStageFinish )
	//{
	//	SNtlEventRBBattleStageFinish* pData = reinterpret_cast<SNtlEventRBBattleStageFinish*>( msg.pData );
	//	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();
	//	RwBool bRoomOwner = pRankBattle->IsRoomOwner();

	//	switch( pData->byBattleResult )
	//	{
	//	case RANKBATTLE_MATCH_WIN_OWNER:
	//		if( bRoomOwner )
	//		{
	//			GetDialogManager()->OpenDialog( DIALOG_FLASH_NOTIFY );
	//			LoadFlash( "RankBattle_You_Win.swf" );
	//		}
	//		else
	//		{
	//			GetDialogManager()->OpenDialog( DIALOG_FLASH_NOTIFY );
	//			LoadFlash( "RankBattle_You_Lose.swf" );
	//		}
	//		break; 
	//	case RANKBATTLE_MATCH_WIN_CHALLENGER:
	//		if( bRoomOwner )
	//		{
	//			GetDialogManager()->OpenDialog( DIALOG_FLASH_NOTIFY );
	//			LoadFlash( "RankBattle_You_Lose.swf" );
	//		}
	//		else
	//		{
	//			GetDialogManager()->OpenDialog( DIALOG_FLASH_NOTIFY );
	//			LoadFlash( "RankBattle_You_Win.swf" );				
	//		}
	//		break;
	//	case RANKBATTLE_MATCH_DRAW:
	//		GetDialogManager()->OpenDialog( DIALOG_FLASH_NOTIFY );
	//		LoadFlash( "RankBattle_Draw.swf" );							 
	//		break;
	//	}		

	//	//m_strSecondFile = "TMQ_PleaseWait.swf";
	//}
	//else if( msg.Id == g_EventRBBattleMatchFinish )
	//{
	//	CNtlRankBattle* pRankBattle = GetNtlSLGlobal()->GetSobAvatar()->GetRankBattle();

	//	if( pRankBattle->GetResult() == CNtlRankBattle::WIN )
	//	{
	//		GetDialogManager()->OpenDialog( DIALOG_FLASH_NOTIFY );
	//		LoadFlash( "RankBattle_Title2.swf" );
	//		m_strSecondFile = "RankBattle_You_Win.swf";
	//		TempString = "RankBattleResult";
	//	}
	//	else if( pRankBattle->GetResult() == CNtlRankBattle::LOSE )
	//	{
	//		GetDialogManager()->OpenDialog( DIALOG_FLASH_NOTIFY );
	//		LoadFlash( "RankBattle_Title2.swf" );
	//		m_strSecondFile =  "RankBattle_You_Lose.swf";
	//		TempString = "RankBattleResult";
	//	}
	//	else if( pRankBattle->GetResult() == CNtlRankBattle::DRAW )
	//	{
	//		GetDialogManager()->OpenDialog( DIALOG_FLASH_NOTIFY );
	//		LoadFlash( "RankBattle_Title2.swf" );
	//		m_strSecondFile = "RankBattle_Draw.swf";	
	//		TempString = "RankBattleResult";			
	//	}
	//}
	else if( msg.Id == g_EventRBForcedEnd || msg.Id == g_EventRBBattleEnd )
	{
		FinishFlash();
	}
	else if( msg.Id == g_EventResize )
	{
		//SNtlPLEventResize* pEvent = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		LocateComponent();
	}
	else if( msg.Id == g_EventTMQCleintState )
	{
		SNtlEventTMQServerState* pEvent = reinterpret_cast<SNtlEventTMQServerState*>( msg.pData );

		if( pEvent->eWorldConcept != WORLD_PLAY_TIME_MACHINE )
			NTL_RETURNVOID();

		if( pEvent->uiState == WORLD_STATE_TMQ_TERRAIN_READY )
		{
			// please wait
			CDboEventGenerator::FlashNotifyString(dINVALID_FLASH_NOTIFY_FILE_PLEASE_WAIT);
		}
		else if( pEvent->uiState == WORLD_STATE_TMQ_ARRIVE )
		{
			// please wait 플래쉬를 끈다
			CDboEventGenerator::FlashNotifyString(NULL);
		}
		else if( pEvent->uiState == WORLD_STATE_TMQ_FAIL )
		{
			// play flash "timemachine quest fail"
			CDboEventGenerator::FlashNotify(0, 0, 3);
		}
	}
    else if(msg.Id == g_EventSobConvertClass)       // 직업 체인지시의 플래시 연출
    {
        // 자신의 이벤트일 경우에만 플래시를 재생한다.
        SNtlEventSobConvertClass* pData = (SNtlEventSobConvertClass*)msg.pData;
        if(pData->hSerialId == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID())
        {
            CDboEventGenerator::FlashNotifyString(dINVALID_FLASH_NOTIFY_FILE_CLASS_CHANGE);
        }        
    }
    else if(msg.Id == g_EventSobInfoUpdate)
    {
        // 레벨업 플래시 연출
        SNtlEventSobInfoUpdate* pData = (SNtlEventSobInfoUpdate*)msg.pData;
        if(pData->hSerialId == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() &&
           pData->uiUpdateType == (EVENT_AIUT_ATTR | EVENT_AIUT_ATTR_LEVELUP))
        {
            CDboEventGenerator::FlashNotifyString(dINVALID_FLASH_NOTIFY_FILE_LEVEL_UP);
        }
    }
	else if(msg.Id == g_EventHoipoiMixInfoUpdate )
	{
		SNtlEventHoipoiMixInfoUpdate* pData = (SNtlEventHoipoiMixInfoUpdate*)msg.pData;
		if( pData->byType == eHOIPOIMIX_INFO_LEVELUP )
		{
			CDboEventGenerator::FlashNotifyString(dINVALID_FLASH_NOTIFY_FILE_HOIPOIMIX_LEVEL_UP);
		}
	}
	else if( msg.Id == g_EventUpdateQuestState_Nfy )
	{
		SNtlEventUpdateQuestState_Nfy* pData = reinterpret_cast<SNtlEventUpdateQuestState_Nfy*>( msg.pData );

		if( pData->bOutStateMsg )
		{
			if( ( pData->uiQuestState & eTS_PROG_STATE_PROGRESS_FAILED && pData->uiUpdatedQuestFlag & eTS_PROG_STATE_PROGRESS_FAILED ) ||
				( pData->uiQuestState & eTS_PROG_STATE_ERROR_BLOCK && pData->uiUpdatedQuestFlag & eTS_PROG_STATE_ERROR_BLOCK ) )
			{
				LoadFlash( "Quest_Fail.swf" );
				GetDialogManager()->OpenDialog( DIALOG_FLASH_NOTIFY );
			}				
		}		
	}
	else if( msg.Id == g_EventQuestAcceptReward_Nfy )
	{
		SNtlEventQuestAcceptReward_Nfy* pData = reinterpret_cast<SNtlEventQuestAcceptReward_Nfy*>( msg.pData );
		
		if( pData->bOutStateMsg )
		{
			LoadFlash( "Quest_Success.swf" );
			GetDialogManager()->OpenDialog( DIALOG_FLASH_NOTIFY ); // by daneos
		}		
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pState = reinterpret_cast< SNtlEventWorldConceptState*>( msg.pData );

		if( pState->eWorldConcept == WORLD_PLAY_FREEPVP )
		{
			if( pState->uiState == WORLD_STATE_ENTER )
			{
				LoadFlash( "freebattle_start.swf" );
				GetDialogManager()->OpenDialog( DIALOG_FLASH_NOTIFY );
			}
		}
	}

	NTL_RETURNVOID();
}