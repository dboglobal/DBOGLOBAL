#include "precomp_dboclient.h"
#include "QuestGui.h"

// Core
#include "NtlDebug.h"

// Table
#include "QuestTextDataTable.h"
#include "TableContainer.h"

// Simulation
#include "NtlSLApi.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSLLogic.h"

// Presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"

// DBO
#include "DboEvent.h"
#include "QuestListGui.h"
#include "QuestProgressGui.h"
#include "QuestProposalGui.h"
#include "QuestRewardGui.h"
#include "QuestIndicatorGui.h"
#include "QuestInventoryGui.h"
#include "QuestNarrationGui.h"
#include "QuestMessageGui.h"
#include "MultiDialogGui.h"
#include "DialogDefine.h"
#include "DialogManager.h"
#include "SideDialogManager.h"
#include "GUISoundDefine.h"
#include "AlarmManager.h"
#include "DisplayStringManager.h"

CQuestGui::CQuestGui(VOID)
: m_pQuestList( NULL ), m_pQuestProgress( NULL ), m_pQuestProposal( NULL ), m_pQuestIndicator( NULL ),
  m_pMultiDialog( NULL ), m_pQuestReward( NULL), m_pQuestInventory( NULL ),
  m_pQuestNarration( NULL )
{

}

CQuestGui::~CQuestGui(VOID)
{

}

RwBool CQuestGui::Create(VOID)
{
	NTL_FUNCTION( "CQuestGui::Create" );

	// Link Event
	LinkMsg( g_EventRegQuest_Nfy, 0 );
	LinkMsg( g_EventUnregQuest_Nfy, 0 );
	LinkMsg( g_EventUpdateQuestState_Nfy, 0 );
	LinkMsg( g_EventUpdateQuestProgressInfo_Nfy, 0 );
	LinkMsg( g_EventShowIndicator_Nfy, 0 );
	LinkMsg( g_EventShowQuestWindow_Nfy, 0 );
	LinkMsg( g_EventQuestDirectForward, 0 );
	LinkMsg( g_EventHideIndicator, 0 );
	LinkMsg( g_EventQuestNPCDialog_Req, 0 );
	LinkMsg( g_EventQuestObjDialog_Req, 0 );
	LinkMsg( g_EventESCinNarrationMode, 0 );
	LinkMsg( g_EventDialog, 0 );
	LinkMsg( g_EventTSRemovingTMQQuest_Nfy, 0 );
	LinkMsg( g_EventQuestAcceptProposal_Nfy, 0 );
	LinkMsg( g_EventQuestAcceptReward_Nfy, 0 );
	LinkMsg( g_EventQuestAcceptGiveUp_Nfy, 0 );
	LinkMsg( g_EventResetCinematic, 0 );
	LinkMsg( g_EventMsgBoxResult );
	LinkMsg( g_EventGameServerChangeOut );
	
	// Quest Indicator 타이밍
	LinkMsg( g_EventChangeWorldConceptState, 0 ); // QuestIndicator show
	//LinkMsg( g_EventRBMatch, 0 );				  // QuestIndicator hide
	LinkMsg(g_EventResize);
			
	// Component Create
	m_pQuestList = NTL_NEW CQuestListGui( "QuestListGui" );
	if( !m_pQuestList->Create( this ) )
	{
		m_pQuestList->Destroy();
		NTL_DELETE( m_pQuestList );
		NTL_RETURN( FALSE );
	}
	GetNtlGuiManager()->AddGui( m_pQuestList );
	GetDialogManager()->RegistDialog( DIALOG_QUESTLIST, m_pQuestList, &CQuestListGui::SwitchDialog );

	m_pMultiDialog = NTL_NEW CMultiDialogGui( "MultiDialogGui" );
	if( !m_pMultiDialog->Create( this ) )
	{
		m_pMultiDialog->Destroy();
		NTL_DELETE( m_pMultiDialog );
		NTL_RETURN( FALSE );
	}
	GetNtlGuiManager()->AddGui( m_pMultiDialog );
	GetDialogManager()->RegistDialog( DIALOG_MULTIDIALOG, m_pMultiDialog, &CMultiDialogGui::SwitchDialog );

	m_pQuestProposal = NTL_NEW CQuestProposalGui( "QuestProposalGui" );
	if( !m_pQuestProposal->Create( this ) )
	{
		m_pQuestProposal->Destroy();
		NTL_DELETE( m_pQuestProposal );
		NTL_RETURN( FALSE );
	}
	GetNtlGuiManager()->AddGui( m_pQuestProposal );
	GetDialogManager()->RegistDialog( DIALOG_QUESTPROPOSAL, m_pQuestProposal, &CQuestProposalGui::SwitchDialog );

	m_pQuestReward = NTL_NEW CQuestRewardGui( "QuestRewardGui" );
	if( !m_pQuestReward->Create( this ) )
	{
		m_pQuestReward->Destroy();
		NTL_DELETE( m_pQuestReward );
		NTL_RETURN( FALSE );
	}
	GetNtlGuiManager()->AddGui( m_pQuestReward );
	GetDialogManager()->RegistDialog( DIALOG_QUESTREWARD, m_pQuestReward, &CQuestRewardGui::SwitchDialog );

	m_pQuestProgress = NTL_NEW CQuestProgressGui( "QuestProgressGui" );
	if( !m_pQuestProgress->Create( this ) )
	{
		m_pQuestProgress->Destroy();
		NTL_DELETE( m_pQuestProgress );
		NTL_RETURN( FALSE );
	}
	GetNtlGuiManager()->AddGui( m_pQuestProgress );
	GetDialogManager()->RegistDialog( DIALOG_QUESTPROGRESS, m_pQuestProgress, &CQuestProgressGui::SwitchDialog );

	m_pQuestIndicator = NTL_NEW CQuestIndicatorGui( "QuestIndicatorGui" );
	if( !m_pQuestIndicator->Create( this ) )
	{
		m_pQuestIndicator->Destroy();
		NTL_DELETE( m_pQuestIndicator );
		NTL_RETURN( FALSE );
	}
	GetNtlGuiManager()->AddGui( m_pQuestIndicator );
	GetSideDialogManager()->RegistDialog( SDIALOG_QUEST_INDICATOR, m_pQuestIndicator, &CQuestIndicatorGui::SwitchDialog );
	
	m_pQuestMessage = NTL_NEW CQuestMessageGui( "QuestMessageGui" );
	if( !m_pQuestMessage->Create( this ) )
	{
		m_pQuestMessage->Destroy();
		NTL_DELETE( m_pQuestMessage );
		NTL_RETURN( FALSE );
	}
	GetNtlGuiManager()->AddGui( m_pQuestMessage );
	GetDialogManager()->RegistDialog( DIALOG_QUESTMESSAGE, m_pQuestMessage, &CQuestMessageGui::SwitchDialog );

	m_pQuestNarration = NTL_NEW CQuestNarrationGui();
	if( !m_pQuestNarration->Create( this ) )
	{
		m_pQuestNarration->Destroy();
		NTL_DELETE( m_pQuestNarration );
		NTL_RETURN( FALSE );
	}
	
	NTL_RETURN( TRUE );
}

VOID CQuestGui::Destroy(VOID)
{
	// Unlink Event
	UnLinkMsg( g_EventRegQuest_Nfy );
	UnLinkMsg( g_EventUnregQuest_Nfy );
	UnLinkMsg( g_EventUpdateQuestState_Nfy );
	UnLinkMsg( g_EventUpdateQuestProgressInfo_Nfy );
	UnLinkMsg( g_EventShowIndicator_Nfy );
	UnLinkMsg( g_EventShowQuestWindow_Nfy );
	UnLinkMsg( g_EventQuestDirectForward );
	UnLinkMsg( g_EventHideIndicator );
	UnLinkMsg( g_EventQuestNPCDialog_Req );
	UnLinkMsg( g_EventQuestObjDialog_Req );
	UnLinkMsg( g_EventESCinNarrationMode );
	UnLinkMsg( g_EventDialog );
	UnLinkMsg( g_EventTSRemovingTMQQuest_Nfy );
	UnLinkMsg( g_EventQuestAcceptProposal_Nfy );
	UnLinkMsg( g_EventQuestAcceptReward_Nfy );
	UnLinkMsg( g_EventQuestAcceptGiveUp_Nfy );
	UnLinkMsg( g_EventResetCinematic );
	UnLinkMsg( g_EventMsgBoxResult );
	UnLinkMsg( g_EventGameServerChangeOut );

	// Quest Indicator 타이밍
	UnLinkMsg( g_EventChangeWorldConceptState );
	//UnLinkMsg( g_EventRBMatch );
	UnLinkMsg(g_EventResize);

		
	// Component Destroy
	if( m_pQuestList )
	{
		GetNtlGuiManager()->RemoveGui( m_pQuestList );
		m_pQuestList->Destroy();
		NTL_DELETE( m_pQuestList );
	}

	if( m_pMultiDialog )
	{
		GetNtlGuiManager()->RemoveGui( m_pMultiDialog );
		m_pMultiDialog->Destroy();
		NTL_DELETE( m_pMultiDialog );
	}

	if( m_pQuestProposal )
	{
		GetNtlGuiManager()->RemoveGui( m_pQuestProposal );
		m_pQuestProposal->Destroy();
		NTL_DELETE( m_pQuestProposal );
	}

	if( m_pQuestReward )
	{
		GetNtlGuiManager()->RemoveGui( m_pQuestReward );
		m_pQuestReward->Destroy();
		NTL_DELETE( m_pQuestReward );
	}

	if( m_pQuestProgress )
	{
		GetNtlGuiManager()->RemoveGui( m_pQuestProgress );
		m_pQuestProgress->Destroy();
		NTL_DELETE( m_pQuestProgress );
	}

	if( m_pQuestIndicator )
	{
		GetNtlGuiManager()->RemoveGui( m_pQuestIndicator );
		GetSideDialogManager()->UnRegistDialog( SDIALOG_QUEST_INDICATOR );
		m_pQuestIndicator->Destroy();
		NTL_DELETE( m_pQuestIndicator );
	}

	if( m_pQuestMessage )
	{
		GetNtlGuiManager()->RemoveGui( m_pQuestMessage );
		m_pQuestMessage->Destroy();
		NTL_DELETE( m_pQuestMessage );
	}

	if( m_pQuestNarration )
	{
		m_pQuestNarration->Destroy();
		NTL_DELETE( m_pQuestNarration );
	}
}

VOID CQuestGui::HandleEvents( RWS::CMsg &msg )
{
	if( msg.Id == g_EventRegQuest_Nfy )
	{
		m_pQuestList->HandleEvents( msg );
		m_pQuestProgress->HandleEvents( msg );
	}
	else if( msg.Id == g_EventUnregQuest_Nfy )
	{
		m_pQuestList->HandleEvents( msg );
		m_pQuestIndicator->HandleEvents( msg );

		// 떠있는 퀘스트 관련 윈도우 삭제
		m_pQuestProposal->HandleEvents( msg );
		m_pQuestProgress->HandleEvents( msg );
		m_pQuestReward->HandleEvents( msg );
		m_pMultiDialog->HandleEvents( msg );
	}
	else if( msg.Id == g_EventTSRemovingTMQQuest_Nfy )
	{
		// TMQ Quest( List에 등록되지 않는 )시 윈도우 종료
		m_pQuestProposal->HandleEvents( msg );
		m_pQuestReward->HandleEvents( msg );
		m_pMultiDialog->HandleEvents( msg );
		m_pQuestIndicator->HandleEvents( msg );
		m_pQuestNarration->HandleEvents( msg );
	}
	else if( msg.Id == g_EventUpdateQuestState_Nfy )
	{
		SNtlEventUpdateQuestState_Nfy* pData = reinterpret_cast<SNtlEventUpdateQuestState_Nfy*>( msg.pData );
		CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();

		m_pQuestList->HandleEvents( msg );
		m_pQuestIndicator->HandleEvents( msg );		
		m_pQuestProgress->HandleEvents( msg );

		if( !pData->bOutStateMsg )
			return;

		if( pData->uiQuestTitle == INVALID_TBLIDX )
			return;

		sQUEST_TEXT_DATA_TBLDAT* pQuestTitle = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pData->uiQuestTitle ) );
		std::wstring wstrQuestTitle;

		if( pQuestTitle )
			wstrQuestTitle = pQuestTitle->wstrText;
		else
		{
			WCHAR wBuf[256];
			swprintf_s( wBuf, 256, L"%d QuestTextData is Missing", pData->uiQuestTitle );
			wstrQuestTitle = wBuf;
		}
		
		if( ( pData->uiQuestState & eTS_PROG_STATE_PROGRESS_FAILED && pData->uiUpdatedQuestFlag & eTS_PROG_STATE_PROGRESS_FAILED ) ||
			( pData->uiQuestState & eTS_PROG_STATE_ERROR_BLOCK && pData->uiUpdatedQuestFlag & eTS_PROG_STATE_ERROR_BLOCK ) )
		{
			GetAlarmManager()->FormattedAlarmMessage( "DST_QUEST_FAILED", FALSE, NULL, gui::GetHtmlFromMemoryString( wstrQuestTitle.c_str(), wstrQuestTitle.size() ).c_str() );
		}		
		else if( pData->uiQuestState & eTS_PROG_STATE_SUCCESS && pData->uiUpdatedQuestFlag & eTS_PROG_STATE_SUCCESS)
		{
			if( pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_MOB_KILL_ITEM_CNT ||
				pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_OBJECT_ITEM ||
				pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_PUBLIC_MOB_ITEM_CNT ) 
			{
				GetAlarmManager()->FormattedAlarmMessage( "DST_QUEST_SUCCESS_COLLECTION", FALSE, NULL, gui::GetHtmlFromMemoryString( wstrQuestTitle.c_str(), wstrQuestTitle.size() ).c_str() );
			}
			else if( pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_MOB_KILL_CNT )
			{
				GetAlarmManager()->FormattedAlarmMessage( "DST_QUEST_SUCCESS_MOBHUNT", FALSE, NULL, gui::GetHtmlFromMemoryString( wstrQuestTitle.c_str(), wstrQuestTitle.size() ).c_str() );
			}
			else if( pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_DELIVERY_ITEM )
			{
				GetAlarmManager()->FormattedAlarmMessage( "DST_QUEST_SUCCESS_DELIVERY", FALSE, NULL, gui::GetHtmlFromMemoryString( wstrQuestTitle.c_str(), wstrQuestTitle.size() ).c_str() );
			}
			else if( pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_CUSTOM_EVT_CNT )
			{
				GetAlarmManager()->FormattedAlarmMessage( "DST_QUEST_SUCCESS_CUSTOM", FALSE, NULL, gui::GetHtmlFromMemoryString( wstrQuestTitle.c_str(), wstrQuestTitle.size() ).c_str() );
			}
			else if( pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_VISIT )
			{
				GetAlarmManager()->FormattedAlarmMessage( "DST_QUEST_SUCCESS_VISIT", FALSE, NULL, gui::GetHtmlFromMemoryString( wstrQuestTitle.c_str(), wstrQuestTitle.size() ).c_str() );
			}
			else if (pData->eEvtInfoType == eSTOC_EVT_DATA_TYPE_NPCHUNT)
			{
				GetAlarmManager()->FormattedAlarmMessage("DST_QUEST_SUCCESS_NPCHUNT", FALSE, NULL, gui::GetHtmlFromMemoryString(wstrQuestTitle.c_str(), wstrQuestTitle.size()).c_str());
			}
		}		
	}
	else if( msg.Id == g_EventUpdateQuestProgressInfo_Nfy ) 
	{
		m_pQuestIndicator->HandleEvents( msg );		
		m_pQuestProgress->HandleEvents( msg );			
	}
	else if( msg.Id == g_EventShowIndicator_Nfy ||
			 msg.Id == g_EventHideIndicator /*||
			 msg.Id == g_EventRBMatch*/ )			  
	{
		m_pQuestIndicator->HandleEvents( msg );
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		m_pQuestMessage->HandleEvents( msg );
		m_pQuestIndicator->HandleEvents( msg );
		m_pQuestList->HandleEvents( msg );
	}
	else if( msg.Id == g_EventShowQuestWindow_Nfy )
	{
		m_pQuestProgress->HandleEvents( msg );		
	}
	else if( msg.Id == g_EventQuestDirectForward )
	{
		SNtlEventQuestDirect_Forward* pData = reinterpret_cast<SNtlEventQuestDirect_Forward*>( msg.pData );	
		CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->GetWorldConceptController( WORLD_PLAY_DIRECT );

		switch( pData->eTDType )
		{
		case ETD_QuestProposal:
			if( pWorldConcept )
			{
				SNtlEventQuestDirect_Echo stEcho;
				stEcho.sProposal.bResult = false;
				stEcho.sProposal.sTSKey	= pData->sProposal.sTSKey;
				stEcho.eTDType = ETD_QuestProposal;
				stEcho.pTCUnit = pData->pTCUnit;

				CNtlSLEventGenerator::QuestTriggerDirectEcho( &stEcho );
			}
			else
			{
				m_pQuestProposal->HandleEvents( msg );
				m_pQuestMessage->HandleEvents( msg );
			}			
			break;
		case ETD_QuestUserSelect:					
			if( pWorldConcept )
			{
				SNtlEventQuestDirect_Echo stEcho;
				stEcho.sProposal.bResult = false;
				stEcho.sProposal.sTSKey	= pData->sUserSelect.sTSKey;
				stEcho.eTDType = ETD_QuestUserSelect;
				stEcho.pTCUnit = pData->pTCUnit;

				CNtlSLEventGenerator::QuestTriggerDirectEcho( &stEcho );
			}
			else
			{
				m_pMultiDialog->HandleEvents( msg );
			}			
			break;
		case ETD_QuestReward:
			if( pData->sReward.eRewardType != eREWARD_CONTAINER_TYPE_QUEST )
				return;

			if( pWorldConcept )
			{
				SNtlEventQuestDirect_Echo stEcho;
				stEcho.sProposal.bResult = false;
				stEcho.sProposal.sTSKey	= pData->sReward.sTSKey;
				stEcho.eTDType = ETD_QuestReward;
				stEcho.pTCUnit = pData->pTCUnit;

				CNtlSLEventGenerator::QuestTriggerDirectEcho( &stEcho );	
			}
			else
			{
				m_pQuestReward->HandleEvents( msg );
				if( m_pQuestProgress->IsShow() )
					m_pQuestProgress->HandleEvents( msg );
			}			
			break;
		case ETD_QuestNarration:
			m_pQuestNarration->HandleEvents( msg );
			break;			
		case ETD_QuestNarrationUserSelect:
			m_pQuestNarration->HandleEvents( msg );
			break;
		}
	}
	else if( msg.Id == g_EventQuestNPCDialog_Req || msg.Id == g_EventQuestObjDialog_Req )
	{
		m_pMultiDialog->HandleEvents( msg );
	}
	else if( msg.Id == g_EventESCinNarrationMode )
	{
		m_pQuestNarration->HandleEvents( msg );
	}
	else if( msg.Id == g_EventDialog )
	{
		m_pMultiDialog->HandleEvents( msg );
		m_pQuestProposal->HandleEvents( msg );
		m_pQuestReward->HandleEvents( msg );
		m_pQuestMessage->HandleEvents( msg );
	}
	else if( msg.Id == g_EventQuestAcceptProposal_Nfy )
	{
		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_JINGLE_MUSIC;
		tSoundParam.pcFileName		= GSD_JINGLE_QUEST_START;

		GetSoundManager()->Play(&tSoundParam);
	}
	else if( msg.Id == g_EventQuestAcceptReward_Nfy )
	{
		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_JINGLE_MUSIC;
		tSoundParam.pcFileName		= GSD_JINGLE_QUEST_FINISH;

		GetSoundManager()->Play(&tSoundParam);
		
		SNtlEventQuestAcceptReward_Nfy* pData = reinterpret_cast<SNtlEventQuestAcceptReward_Nfy*>( msg.pData );
		if( pData->bOutStateMsg && pData->uiQTitle != INVALID_TBLIDX )
		{
			CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
			sQUEST_TEXT_DATA_TBLDAT* pQuestTitle = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pData->uiQTitle ) );

			if( pQuestTitle )
				GetAlarmManager()->FormattedAlarmMessage( "DST_QUEST_COMPLETE", FALSE, NULL, gui::GetHtmlFromMemoryString( pQuestTitle->wstrText.c_str(), pQuestTitle->wstrText.size() ).c_str() );
		}		
	}
	else if( msg.Id == g_EventQuestAcceptGiveUp_Nfy )
	{
		sNtlSoundPlayParameta tSoundParam;
		tSoundParam.iChannelGroup	= CHANNEL_GROUP_JINGLE_MUSIC;
		tSoundParam.pcFileName		= GSD_SYSTEM_QUEST_GIVE_UP;

		GetSoundManager()->Play(&tSoundParam);

		SNtlEventQuestAcceptGiveUp_Nfy* pData = reinterpret_cast<SNtlEventQuestAcceptGiveUp_Nfy*>( msg.pData );
		if( pData->uiQTitle != INVALID_TBLIDX )
		{
			CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
			sQUEST_TEXT_DATA_TBLDAT* pQuestTitle = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pData->uiQTitle ) );
			
			if( pQuestTitle )
				GetAlarmManager()->FormattedAlarmMessage( "DST_QUEST_GAVEUP_MESSAGE", FALSE, NULL, gui::GetHtmlFromMemoryString( pQuestTitle->wstrText.c_str(), pQuestTitle->wstrText.size() ).c_str() );
		}		
	}
	else if( msg.Id == g_EventResetCinematic )
	{
		m_pQuestNarration->HandleEvents( msg );
	}
	else if( msg.Id == g_EventGameServerChangeOut )
	{
		m_pQuestList->HandleEvents( msg );
	}
	else if( msg.Id == g_EventMsgBoxResult )
	{
		SDboEventMsgBoxResult *pMsgBoxResult = reinterpret_cast<SDboEventMsgBoxResult*>( msg.pData );

		if (pMsgBoxResult->eResult == MBR_OK)
		{
			if (pMsgBoxResult->strID == "DST_QUEST_GIVEUP_MESSAGE")
			{
				Logic_PlayGUISound(GSD_SYSTEM_QUEST_GIVE_UP);
				CNtlSLEventGenerator::GiveUpQuest(pMsgBoxResult->pData->sTSKey);
			}
		}
	}
	else if (msg.Id == g_EventResize)
	{
		m_pQuestIndicator->HandleEvents(msg);
	}
}