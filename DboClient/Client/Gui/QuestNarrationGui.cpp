#include "precomp_dboclient.h"
#include "QuestNarrationGui.h"

// core
#include "NtlDebug.h"

// shared
#include "QuestTextDataTable.h"
#include "NPCTable.h"
#include "MobTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"
#include "NtlPLEventGenerator.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "DboTSCoreDefine.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlCameraController.h"

// dbo
#include "DboEvent.h"
#include "DialogManager.h"
#include "DboLogic.h"
#include "DboEventGenerator.h"

#include "GUISoundDefine.h"

#define QUEST_NARRATION_TEXT_INTERVAL	0.025f
#define QUEST_NARRATION_TEXT_FONTNUM	1			// 3 -> 1 로 수정 -by Kell(09. 07. 15)

#define QUEST_NARRATION_PCMESSAGE_MARGIN_X	60
#define QUEST_NARRATION_PCMESSAGE_POS_Y		40
#define QUEST_NARRATION_PCMESSAGE_HEIGHT	105

#define QUEST_NARRATION_PCMESSAGE_NAMEPOS_POS_Y		10
#define QUEST_NARRATION_PCMESSAGE_NAMEPOS_MARGIN_X	58
#define QUEST_NARRATION_PCMESSAGE_NAMEPOS_HEIGHT	20 

#define QUEST_NARRATION_PCMESSAGE_PCSELECT_HEIGHT	25
#define QUEST_NARRATION_PCMESSAGE_PCSELECT_OFFSET_Y	1
#define QUEST_NARRATION_PCMESSAGE_PCSELECT_MIRGIN_X	60
#define QUEST_NARRATION_PCMESSAGE_PCSELECT_POS_Y	40
#define QUEST_NARRATION_PCMESSAGE_PCSELECT_WHOLE_HEIGHT 105

CQuestNarrationGui::CQuestNarrationGui(VOID)
{
	Init();
}

CQuestNarrationGui::~CQuestNarrationGui(VOID)
{

}

VOID CQuestNarrationGui::Init(VOID)
{
	InitQuestData();

	m_eUIState = STATE_NONE;
	m_eTSState = eNARRATION_PROGRESS_STATE_INVALID;
	m_eTextOutType = eNARRATION_DIALOG_DIR_TYPE_INVALID;
	m_uiMaxLifeTime = INVALID_DWORD;
	m_uiCurrentTime = 0;

	m_flagOwnerType = 0;
	m_eCurrentType = NONE;

	m_bResult = true;
	m_bySelectID = INVALID_BYTE;
	//m_bShowPCMessageOnPCSelect = FALSE;
}

VOID CQuestNarrationGui::InitQuestData(VOID)
{
	m_pTCUnit = NULL;
	m_TSKey.Init();
}

RwBool CQuestNarrationGui::Create( CQuestGui* pQuestGui )
{
	m_pCinematicObject = GetNtlPLCinematicObject();
	m_pQuestGui = pQuestGui;

	m_slotPCFrameUp = m_pCinematicObject->GetPCFrame()->SigMouseUp().Connect( this, &CQuestNarrationGui::OnMouseUp );
	m_slotMouseUp = m_pCinematicObject->GetDialog()->SigMouseUp().Connect( this, &CQuestNarrationGui::OnMouseUp );
	m_slotHtmlPageDone = m_pCinematicObject->GetHtmlText()->SigPageDone().Connect( this, &CQuestNarrationGui::OnHtmlPageDone );
	m_slotHtmlPCPageDone = m_pCinematicObject->GetPCMessage()->SigPageDone().Connect( this, &CQuestNarrationGui::OnHtmlPCPageDone );
	m_slotPCSelect1 = m_pCinematicObject->GetPCSelect( 0 )->SigClicked().Connect( this, &CQuestNarrationGui::OnPCCSelect1 );
	m_slotPCSelect2 = m_pCinematicObject->GetPCSelect( 1 )->SigClicked().Connect( this, &CQuestNarrationGui::OnPCCSelect2 );
	m_slotPCSelect3 = m_pCinematicObject->GetPCSelect( 2 )->SigClicked().Connect( this, &CQuestNarrationGui::OnPCCSelect3 );
	m_slotPCSelect4 = m_pCinematicObject->GetPCSelect( 3 )->SigClicked().Connect( this, &CQuestNarrationGui::OnPCCSelect4 );
	m_slotFocusSelect1 = m_pCinematicObject->GetPCSelect( 0 )->SigMouseEnter().Connect( this, &CQuestNarrationGui::OnFocusSelect1 );
	m_slotFocusSelect2 = m_pCinematicObject->GetPCSelect( 1 )->SigMouseEnter().Connect( this, &CQuestNarrationGui::OnFocusSelect2 );
	m_slotFocusSelect3 = m_pCinematicObject->GetPCSelect( 2 )->SigMouseEnter().Connect( this, &CQuestNarrationGui::OnFocusSelect3 );
	m_slotFocusSelect4 = m_pCinematicObject->GetPCSelect( 3 )->SigMouseEnter().Connect( this, &CQuestNarrationGui::OnFocusSelect4 );

	NTL_RETURN( TRUE );
}

VOID CQuestNarrationGui::Destroy(VOID)
{

}

INT CQuestNarrationGui::StateUpdate( RwUInt32 uiElapsedTime )
{
	switch( m_eUIState )
	{
	case STATE_BEGIN: BeginStateDirection(); break;
	case STATE_ING:	IngStateDirection(); break;
	case STATE_END:	EndStateDirection(); break;
	case STATE_NONE:
	case STATE_WAIT_RES: return 1;
	}

	if( m_uiMaxLifeTime != 0xFFFFFFFF )
	{
		m_uiCurrentTime += uiElapsedTime;

		if( m_uiMaxLifeTime < m_uiCurrentTime )
		{
			m_uiMaxLifeTime = 0xFFFFFFFF;
			m_uiCurrentTime = 0;

			if( m_eTSState == eNARRATION_PROGRESS_STATE_END ||
				m_eTSState == eNARRATION_PROGRESS_STATE_SINGLE )
			{
				m_bResult = true;
				SetState( STATE_END, m_eCurrentType );
			}
			else
			{
				SendResultToTS( true, m_bySelectID );				
			}		
		}
	}

	return 1;
}

VOID CQuestNarrationGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventQuestDirectForward )
	{
		SNtlEventQuestDirect_Forward* pData = reinterpret_cast<SNtlEventQuestDirect_Forward*>( msg.pData );

		if( pData->eTDType == ETD_QuestNarration )
		{
			if( ( pData->sNarration.eProgState == eNARRATION_PROGRESS_STATE_START ||
				pData->sNarration.eProgState == eNARRATION_PROGRESS_STATE_SINGLE ) )
				CNtlPLEventGenerator::ResetCinematic( SNtlEventResetCinematic::QUESTNARRATION );

			// 다시 보내주어야할 Data
			m_pTCUnit = pData->pTCUnit;
			m_TSKey = pData->sNarration.sTSKey;

			SetUIData( &pData->sNarration );
		}
		else if( pData->eTDType == ETD_QuestNarrationUserSelect )
		{
			if( ( pData->sNarrationUserSelect.eProgState == eUSER_SEL_PROGRESS_STATE_START ||
				pData->sNarrationUserSelect.eProgState == eUSER_SEL_PROGRESS_STATE_SINGLE ) )
				CNtlPLEventGenerator::ResetCinematic( SNtlEventResetCinematic::QUESTNARRATION );

			// 다시 보내주어야할 Data
			m_pTCUnit = pData->pTCUnit;
			m_TSKey = pData->sNarrationUserSelect.sTSKey;

			SetUIData( &pData->sNarrationUserSelect );
		}		
	}
	else if( msg.Id == g_EventESCinNarrationMode )
	{
		if( m_eUIState == STATE_ING || m_eUIState == STATE_IDLE )
		{
			m_bResult = false;

			SetState( STATE_END, ALL );

			//if( m_flagOwnerType & NPCMOB )
			//	SetState( STATE_END, NPCMOB );

			//if( m_flagOwnerType & PC )
			//	SetState( STATE_END, PC );

			//if( m_flagOwnerType & PC_SELECT )
			//	SetState( STATE_END, PC_SELECT );
		}			
	}
	else if( msg.Id == g_EventResetCinematic )
	{
		SNtlEventResetCinematic* pData = reinterpret_cast<SNtlEventResetCinematic*>( msg.pData );

		if( pData->eDemandResetObject != SNtlEventResetCinematic::QUESTNARRATION )
		{
			if( m_eUIState != STATE_NONE )
			{
				SendResultToTS( false );
				SetState( STATE_NONE, NONE );				
			}
		}		
	}
	else if( msg.Id == g_EventTSRemovingTMQQuest_Nfy )
	{
		SNtlEventTSRemovingTMQQuest_Nfy* pData = reinterpret_cast<SNtlEventTSRemovingTMQQuest_Nfy*>( msg.pData );

		if( pData->tRmvTSId == m_TSKey.tID )
		{
			SendResultToTS( false );
			SetState( STATE_NONE, NONE );
		}
	}
}

VOID CQuestNarrationGui::SetUIData( SNtlEventQuestNarrationDialog_Req* pData )
{
	if( !CheckTSState( pData->eProgState, PC ) )
		return;		

	//m_bShowPCMessageOnPCSelect = FALSE;				
	m_eTSState = pData->eProgState;
	m_eTextOutType = pData->eDialogDirType;

	OWNERTYPE eOwnerType = NONE;
	if( pData->eOwnerType == eNARRATION_OWNER_NPC || pData->eOwnerType == eNARRATION_OWNER_MOB )
		eOwnerType = NPCMOB;
	else if( pData->eOwnerType == eNARRATION_OWNER_PC )
		eOwnerType = PC;
	else
		NTL_ASSERTFAIL( "CQuestNarrationGui::SetUIData : Invalid eOwnerType, tcID : " << pData->sTSKey.tID );

	if( m_eTSState == eNARRATION_PROGRESS_STATE_START || m_eTSState == eNARRATION_PROGRESS_STATE_SINGLE )
	{
		SetState( STATE_BEGIN, eOwnerType );
	}
	else if( m_eTSState == eNARRATION_PROGRESS_STATE_END && pData->uiDialog == INVALID_TBLIDX )
	{
		SetState( STATE_END, eOwnerType );
		return;
	}
	else
	{
		SetState( STATE_ING, eOwnerType );
	}

	// 이미지 로드 및 이름 출력
	std::string strFileName;

	if( pData->eOwnerType == eNARRATION_OWNER_NPC )
	{
		sNPC_TBLDAT* pNPCData = reinterpret_cast<sNPC_TBLDAT*>( API_GetTableContainer()->GetNpcTable()->FindData( pData->uiOwnerIdx ) );	
		CTextTable* pNPCText = API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();
		if( !m_pCinematicObject->SetImageTexture( pNPCData->szILLust, pData->eOwnerState ) )
		{
			// 그림이 없으면 기본값 출력. 이래도 없으면 흰색 그림 출력
			m_pCinematicObject->SetImageTexture( pNPCData->szILLust, 0 );				
		}		
		m_pCinematicObject->ShowImage( TRUE );			
		m_pCinematicObject->SetName( pNPCText->GetText( pNPCData->Name ).c_str() );
	}
	else if( pData->eOwnerType == eNARRATION_OWNER_MOB )
	{
		sMOB_TBLDAT* pMobData = reinterpret_cast<sMOB_TBLDAT*>( API_GetTableContainer()->GetMobTable()->FindData( pData->uiOwnerIdx ) );
		CTextTable* pMobText = API_GetTableContainer()->GetTextAllTable()->GetMobTbl();
		if( !m_pCinematicObject->SetImageTexture( pMobData->szILLust, pData->eOwnerState ) )
		{
			// 그림이 없으면 기본값 출력. 이래도 없으면 흰색 그림 출력
			m_pCinematicObject->SetImageTexture( pMobData->szILLust, 0 );				
		}
		m_pCinematicObject->ShowImage( TRUE );			
		m_pCinematicObject->SetName( pMobText->GetText( pMobData->Name ).c_str() );
	}	
	else  // eNARRATION_OWNER_PC
	{
		m_pCinematicObject->SetPCName( Logic_GetName( reinterpret_cast<CNtlSob*>( GetNtlSLGlobal()->GetSobAvatar() ) ) );

		//RwInt32 nWidth = m_pCinematicObject->GetWidth();
		//CRectangle rect;
		//rect.SetRectWH( QUEST_NARRATION_PCMESSAGE_MARGIN_X, QUEST_NARRATION_PCMESSAGE_POS_Y,
		//				nWidth - 2 * QUEST_NARRATION_PCMESSAGE_MARGIN_X, QUEST_NARRATION_PCMESSAGE_HEIGHT );
		//m_pCinematicObject->GetPCMessage()->SetPosition( rect );


		// 선택 모드 닫기.
		for( RwUInt8 i = 0 ; i < CINEMATIC_MAX_PC_SELECT ; ++i )
			m_pCinematicObject->GetPCSelect( i )->Show( false );

		m_pCinematicObject->GetPCSelectFocus()->Show( false );
	}

	// 풍선대화 세팅.
	if( eOwnerType == NPCMOB )
	{
		if( pData->eGUIType == eNARRATION_GUI_TYPE_NORMAL )
		{
			m_pCinematicObject->SetBalloonType( CNtlPLCinematicObject::BALLOON_TYPE_NORMAL );		
		}
		else if( pData->eGUIType == eNARRATION_GUI_TYPE_BOMB )
		{
			m_pCinematicObject->SetBalloonType( CNtlPLCinematicObject::BALLOON_TYPE_BOMB );		
		}
	}

	// 대화 출력스타일 세팅
	if( eOwnerType == NPCMOB )
	{
		if( pData->eDialogDirType == eNARRATION_DIALOG_DIR_TYPE_NORMAL )
		{
			m_pCinematicObject->GetHtmlText()->SetAlpha( 255 );
			m_pCinematicObject->GetHtmlText()->SetIntervalTextDisable();
		}
		else if( pData->eDialogDirType == eNARRATION_DIALOG_DIR_TYPE_BLENDING )
		{
			m_pCinematicObject->GetHtmlText()->SetAlpha( 0 );
			m_pCinematicObject->GetHtmlText()->SetIntervalTextDisable();
		}
		else 
		{
			m_pCinematicObject->GetHtmlText()->SetIntervalTextEnable( QUEST_NARRATION_TEXT_INTERVAL );
			m_pCinematicObject->GetHtmlText()->SetPresentNum( QUEST_NARRATION_TEXT_FONTNUM );
		}
	}
	else
	{
		if( pData->eDialogDirType == eNARRATION_DIALOG_DIR_TYPE_NORMAL )
		{
			m_pCinematicObject->GetPCMessage()->SetAlpha( 255 );
			m_pCinematicObject->GetPCMessage()->SetIntervalTextDisable();			
		}
		else if( pData->eDialogDirType == eNARRATION_DIALOG_DIR_TYPE_BLENDING )
		{
			m_pCinematicObject->GetPCMessage()->SetAlpha( 0 );
			m_pCinematicObject->GetPCMessage()->SetIntervalTextDisable();
		}
		else
		{
			m_pCinematicObject->GetPCMessage()->SetIntervalTextEnable( QUEST_NARRATION_TEXT_INTERVAL );
			m_pCinematicObject->GetPCMessage()->SetPresentNum( QUEST_NARRATION_TEXT_FONTNUM );
		}
	}

	// 대화 입력
	sQUEST_TEXT_DATA_TBLDAT* pTextData = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( API_GetTableContainer()->GetQuestTextDataTable()->FindData( pData->uiDialog ) );
	if( pTextData )
	{
		if( eOwnerType == NPCMOB )
			m_pCinematicObject->SetHtmlText( pTextData->wstrText.c_str(), pTextData->wstrText.size() );
		else
			m_pCinematicObject->SetPCMessage( pTextData->wstrText.c_str(), pTextData->wstrText.size() );
	}
	else
	{
		if( pData->uiDialog != INVALID_TBLIDX )
		{
			WCHAR buf[256];
			swprintf_s( buf, 256, L"QuestTextTable (%u) is missing", pData->uiDialog );
			if( eOwnerType == NPCMOB )
				m_pCinematicObject->SetHtmlText( buf, wcslen( buf ) );
			else
				m_pCinematicObject->SetPCMessage( buf, wcslen( buf ) );
		}		
		else
			m_pCinematicObject->GetBalloon()->Show( false );	// 대화 없는 경우는 말풍선을 표시하지 않음.
	}	

	// 시간 입력
	m_uiMaxLifeTime = pData->uiMaxLifeTime - (RwUInt32)CINEMATIC_MOVETIME * 1000;	
	m_uiCurrentTime = 0;
}

VOID CQuestNarrationGui::SetUIData( SNtlEventQuestNarrationUserSelectDialog_Req* pData )
{
	if( !CheckTSState( pData->eProgState, PC_SELECT ) )
		return;		

	m_eTSState = pData->eProgState;
	// peessitemp: 문자 출력스타일이 없으므로 일단 이렇게...
	m_eTextOutType = eNARRATION_DIALOG_DIR_TYPE_INVALID;

	if( m_eTSState == eNARRATION_PROGRESS_STATE_START || m_eTSState == eNARRATION_PROGRESS_STATE_SINGLE )
	{
		SetState( STATE_BEGIN, PC_SELECT );
	}
	else
	{
		SetState( STATE_ING, PC_SELECT );
	}

	// 이름 출력
	m_pCinematicObject->SetPCName( Logic_GetName( reinterpret_cast<CNtlSob*>( GetNtlSLGlobal()->GetSobAvatar() ) ) );		


	sQUEST_TEXT_DATA_TBLDAT* pTextData = NULL;

	//// 말풍선 출력
	//if( pData->uiConv == INVALID_TBLIDX )
	//{
	//	m_bShowPCMessageOnPCSelect = FALSE;				
	//}
	//else
	//{
	//	pTextData = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( API_GetTableContainer()->GetQuestTextDataTable()->FindData( pData->uiConv ) );

	//	m_bShowPCMessageOnPCSelect = TRUE;
	//	m_pCinematicObject->GetImageFrame()->Show( true );

	//	if( pTextData )
	//	{
	//		m_pCinematicObject->SetHtmlText( pTextData->wstrText.c_str(), pTextData->wstrText.size() );
	//	}
	//	else
	//	{
	//		WCHAR buf[256];
	//		swprintf_s( buf, 256, L"Narration Branch Dialog QuestTextTable (%u) is missing", pData->uiConv );				
	//		m_pCinematicObject->SetHtmlText( pTextData->wstrText.c_str(), pTextData->wstrText.size() );			
	//	}		
	//}

	// 대화선택 세팅.
	for( RwUInt8 i = 0 ; i < CINEMATIC_MAX_PC_SELECT ; ++i )
	{
		if( i >= pData->nBranchCnt )
			break;
		else
		{
			pTextData = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( API_GetTableContainer()->GetQuestTextDataTable()->FindData( pData->asBranchInfo[i].uiBranchText ) );

			if( pTextData )
			{
				m_pCinematicObject->SetPCSelect( i, gui::GetHtmlFromMemoryString( pTextData->wstrText.c_str(), pTextData->wstrText.size() ) );				
			}
			else			
			{
				if( Logic_IsUIDevInfoVisible() )
				{
					WCHAR buf[256];
					swprintf_s( buf, 256, L"QuestTextTable : %u idx is Invalid", pData->asBranchInfo[i].uiBranchText );
					m_pCinematicObject->SetPCSelect( i, buf );				
				}				
				else
				{
					m_pCinematicObject->SetPCSelect( i, L" " );
				}
			}

			m_byPCSelectID[i] = pData->asBranchInfo[i].tcID;			
		}	
	}

	RwInt32 nPCFrameTextRegionHeight = m_pCinematicObject->GetPCFrame()->GetHeight() - 19;	// MagicalNumber : 이미지로 가리는 부분이 19 pixel 만큼.
	m_pCinematicObject->SetPCSelectRegion( (RwUInt8)pData->nBranchCnt, (RwUInt8)nPCFrameTextRegionHeight, TRUE );

	m_bySelectID = m_byPCSelectID[0];
	m_pCinematicObject->SetPCSelectValue( 0 );
	m_pCinematicObject->GetPCSelectFocus()->Show( true );

	// 시간 입력
	m_uiMaxLifeTime = 0xFFFFFFFF;
	m_uiCurrentTime = 0;
}

VOID CQuestNarrationGui::SendResultToTS( bool bResult, RwUInt8 bySelectID /* = 0xff  */ )
{
	SNtlEventQuestDirect_Echo stEcho;

	if( m_eCurrentType == NPCMOB || m_eCurrentType == PC )
	{
		stEcho.sNarration.sTSKey = m_TSKey;
		stEcho.sNarration.bResult = bResult;
		stEcho.pTCUnit = m_pTCUnit;
		stEcho.eTDType = ETD_QuestNarration;
	}
	else
	{
		stEcho.sNarrationUserSelect.sTSKey = m_TSKey;
		stEcho.sNarrationUserSelect.bResult = bResult;
		stEcho.sNarrationUserSelect.tcSelID = bySelectID;
		stEcho.pTCUnit = m_pTCUnit;
		stEcho.eTDType = ETD_QuestNarrationUserSelect;
	}

	if( m_pTCUnit )
	{
		CNtlSLEventGenerator::QuestTriggerDirectEcho( &stEcho );
		SetState( STATE_WAIT_RES, m_eCurrentType );
		InitQuestData();
	}	
}

VOID CQuestNarrationGui::SetState( STATE eState, OWNERTYPE eOwnerType )
{
	if( m_eUIState == eState && m_eCurrentType == eOwnerType )
		return;

	switch( eState )
	{
	case STATE_NONE:
		m_flagOwnerType = 0;	
		m_pCinematicObject->GetHtmlText()->SetIntervalTextDisable();		
		GetDialogManager()->OffMode();
		GetNtlPLCinematicObject()->SwitchDialog( false );
		GetDialogManager()->OpenGMDialog();
		GetDialogManager()->OpenDefaultDialog();
		GetNtlGuiManager()->RemoveUpdateFunc( m_pCinematicObject );
		GetNtlWorldConcept()->RemoveWorldPlayConcept( WORLD_PLAY_DIRECT );
		m_pCinematicObject->UnLinkUpdateCallBack();
		break;
	case STATE_BEGIN:
		if( !GetDialogManager()->IsMode( DIALOGMODE_NARRATION ) )
		{
			GetDialogManager()->OnMode( DIALOGMODE_NARRATION );		
			GetNtlWorldConcept()->AddWorldPlayConcept( WORLD_PLAY_DIRECT );
			GetNtlPLCinematicObject()->SwitchDialog( true );
		}		

		if( eOwnerType == NPCMOB )
		{
			m_pCinematicObject->SetFocusOnNPCBar( TRUE );

			if( m_flagOwnerType & PC || m_flagOwnerType & PC_SELECT )
			{
				//m_pCinematicObject->GetPCAniClick()->Show( false );
				//m_pCinematicObject->GetPCAniClick()->SetAnimation( false );
				//m_pCinematicObject->GetImageFrame()->Raise();				
			}			
			else
			{
				m_pCinematicObject->CinematicFadeOut( FALSE, TRUE );				
				m_pCinematicObject->CinematicBackFadeOut( TRUE );
				m_pCinematicObject->GetPCFrame()->Show( false );
			}			

			m_pCinematicObject->GetHtmlText()->Show( false );
			m_pCinematicObject->GetText()->Show( false );
			m_pCinematicObject->GetImageFrame()->Show( true );
			m_pCinematicObject->ShowImage( FALSE );
			m_pCinematicObject->GetBalloon()->Show( true );	
			//m_pCinematicObject->GetClickAni()->Show( false );
			m_pCinematicObject->GetImageFrame()->SetAlpha( 0 );
			m_pCinematicObject->SetImageAlpha( 0 );
		}		
		else if( eOwnerType == PC || eOwnerType == PC_SELECT )
		{
			m_pCinematicObject->SetFocusOnNPCBar( FALSE );

			if( m_flagOwnerType & NPCMOB )
			{
				//m_pCinematicObject->GetClickAni()->Show( false );
				//m_pCinematicObject->GetClickAni()->SetAnimation( false );
				//m_pCinematicObject->GetPCFrame()->Raise();
			}			
			else
			{
				m_pCinematicObject->CinematicFadeOut( FALSE, TRUE );
				m_pCinematicObject->CinematicBackFadeOut( TRUE );
				m_pCinematicObject->GetImageFrame()->Show( false );				
			}

			m_pCinematicObject->GetPCMessage()->Show( false );
			//m_pCinematicObject->GetPCAniClick()->Show( false );
			m_pCinematicObject->GetPCPCName()->Show( true );
			m_pCinematicObject->GetPCSelectFocus()->Show( false );
			m_pCinematicObject->GetPCFrame()->Show( true );
		}

		m_flagOwnerType |= eOwnerType;

		m_pCinematicObject->ResetCinematicCurrentTime();
		GetNtlGuiManager()->AddUpdateFunc( m_pCinematicObject );
		m_pCinematicObject->LinkUpdateCallBack( this, &CQuestNarrationGui::StateUpdate );
		break;
	case STATE_ING:		
		if( eOwnerType == NPCMOB )
		{
			//m_pCinematicObject->GetPCAniClick()->Show( false );
			//m_pCinematicObject->GetClickAni()->Show( true );
			//m_pCinematicObject->GetClickAni()->SetAnimation( true );
			m_pCinematicObject->GetHtmlText()->Show( true );

			m_pCinematicObject->SetFocusOnNPCBar( TRUE );
			//if( m_eCurrentType == PC || m_eCurrentType == PC_SELECT )
			//	m_pCinematicObject->GetImageFrame()->Raise();
		}
		else if( eOwnerType == PC )
		{
			//m_pCinematicObject->GetClickAni()->Show( false );
			//m_pCinematicObject->GetPCAniClick()->Show( true ); 
			//m_pCinematicObject->GetPCAniClick()->SetAnimation( true );			
			m_pCinematicObject->GetPCMessage()->Show( true );
			m_pCinematicObject->GetPCPCName()->Show( true );

			m_pCinematicObject->SetFocusOnNPCBar( FALSE );
			//if( m_eCurrentType == NPCMOB )
			//	m_pCinematicObject->GetPCFrame()->Raise();
		}
		else if( eOwnerType == PC_SELECT )
		{
			//m_pCinematicObject->GetClickAni()->Show( false );
			//m_pCinematicObject->GetPCAniClick()->Show( false ); 
			//m_pCinematicObject->GetPCAniClick()->SetAnimation( false );			
			m_pCinematicObject->GetPCMessage()->Show( false );
			m_pCinematicObject->GetPCPCName()->Show( true );			

			m_pCinematicObject->SetFocusOnNPCBar( FALSE );
			//if( m_eCurrentType == NPCMOB )
			//	m_pCinematicObject->GetPCFrame()->Raise();
		}

		m_pCinematicObject->ResetCinematicCurrentTime();
		break;
	case STATE_END:
		if( eOwnerType == NPCMOB )
		{
			//m_pCinematicObject->GetClickAni()->Show( false );
			//m_pCinematicObject->GetClickAni()->SetAnimation( false );
			m_pCinematicObject->GetImageFrame()->SetAlpha( 255 );
			m_pCinematicObject->SetImageAlpha( 255 );

			if( !( ( m_flagOwnerType & PC ) || ( m_flagOwnerType & PC_SELECT ) ) )
			{
				m_pCinematicObject->CinematicFadeIn( FALSE, TRUE );
				m_pCinematicObject->CinematicBackFadeIn( TRUE );
			}

			//m_flagOwnerType &= ~NPCMOB;
		}
		else if( eOwnerType == PC || eOwnerType == PC_SELECT )
		{
			//m_pCinematicObject->GetPCAniClick()->Show( false );
			//m_pCinematicObject->GetPCAniClick()->SetAnimation( false );
			m_pCinematicObject->GetPCFrame()->SetAlpha( 255 );

			if( !( m_flagOwnerType & NPCMOB ) )
			{
				m_pCinematicObject->CinematicFadeIn( FALSE, TRUE );
				m_pCinematicObject->CinematicBackFadeIn( TRUE );
			}

			//m_flagOwnerType &= ~( PC | PC_SELECT );
		}	
		else
		{
			m_pCinematicObject->CinematicFadeIn( FALSE, TRUE );
			m_pCinematicObject->CinematicBackFadeIn( TRUE );

			if( m_flagOwnerType & NPCMOB ) 
			{
				m_pCinematicObject->GetImageFrame()->SetAlpha( 255 );
				m_pCinematicObject->SetImageAlpha( 255 );
			}

			if( m_flagOwnerType & ( PC | PC_SELECT ) )
			{	
				m_pCinematicObject->GetPCFrame()->SetAlpha( 255 );
			}
		}

		m_pCinematicObject->ResetCinematicCurrentTime();
		break;		
	}

	m_eUIState = eState;
	m_eCurrentType = eOwnerType;
}

INT CQuestNarrationGui::BeginStateDirection(VOID)
{
	RwReal fCurrentTime = m_pCinematicObject->GetCinematicCurrentTime();

	if( fCurrentTime < CINEMATIC_MOVETIME )
	{
		RwUInt8 ucAlpha = (RwUInt8)gui::GetResultLineInterpolation( fCurrentTime, CINEMATIC_MOVETIME, 0.0f, 255.0f );

		if( m_eCurrentType == NPCMOB )
		{
			if( !( m_flagOwnerType & PC || m_flagOwnerType & PC_SELECT ) )
			{
				m_pCinematicObject->CinematicFadeIn( TRUE, FALSE );
				m_pCinematicObject->CinematicBackFadeIn( FALSE, fCurrentTime );
			}

			m_pCinematicObject->GetImageFrame()->SetAlpha( ucAlpha );
			m_pCinematicObject->SetImageAlpha( ucAlpha );
		}		
		else
		{
			if( !( m_flagOwnerType & NPCMOB ) )
			{
				m_pCinematicObject->CinematicFadeIn( TRUE, FALSE );
				m_pCinematicObject->CinematicBackFadeIn( FALSE, fCurrentTime );
			}

			//if( m_bShowPCMessageOnPCSelect )
			//{
			//	m_pCinematicObject->GetImageFrame()->SetAlpha( ucAlpha );
			//	m_pCinematicObject->SetImageAlpha( ucAlpha );
			//}

			m_pCinematicObject->GetPCFrame()->SetAlpha( ucAlpha );
		}
	}
	else
	{
		if( m_eCurrentType == NPCMOB )
		{
			if( !( m_flagOwnerType & PC || m_flagOwnerType & PC_SELECT ) )
			{
				m_pCinematicObject->CinematicFadeIn( TRUE, TRUE );
				m_pCinematicObject->CinematicBackFadeIn( TRUE );
			}

			m_pCinematicObject->GetImageFrame()->SetAlpha( 255 );
			m_pCinematicObject->SetImageAlpha( 255 );
		}
		else
		{
			if( !( m_flagOwnerType & NPCMOB ) )
			{
				m_pCinematicObject->CinematicFadeIn( TRUE, TRUE );
				m_pCinematicObject->CinematicBackFadeIn( TRUE );
			}

			m_pCinematicObject->GetPCFrame()->SetAlpha( 255 );

			//if( m_bShowPCMessageOnPCSelect )
			//{
			//	m_pCinematicObject->GetImageFrame()->SetAlpha( 255 );
			//	m_pCinematicObject->SetImageAlpha( 255 );
			//}
		}

		SetState( STATE_ING, m_eCurrentType );		
	}

	return 1;
}

INT CQuestNarrationGui::IngStateDirection(VOID)
{
	RwReal fCurrentTime = m_pCinematicObject->GetCinematicCurrentTime();

	if( m_eTextOutType == eNARRATION_DIALOG_DIR_TYPE_NORMAL )
	{
		if( fCurrentTime >= CINEMATIC_TEXT_NORMALAPPEARTIME )
			SetState( STATE_IDLE, m_eCurrentType );
	}
	else if( m_eTextOutType == eNARRATION_DIALOG_DIR_TYPE_LINE )
	{
		if( m_eCurrentType == NPCMOB )
			m_pCinematicObject->GetHtmlText()->Update( fCurrentTime );
		else 
			m_pCinematicObject->GetPCMessage()->Update( fCurrentTime );		
	}
	else if( m_eTextOutType == eNARRATION_DIALOG_DIR_TYPE_BLENDING )
	{
		if( fCurrentTime < CINEMATIC_TEXT_APPEARTIME )
		{
			RwUInt8 byAlpha = (RwUInt8)gui::GetResultLineInterpolation( fCurrentTime, CINEMATIC_TEXT_APPEARTIME, 0.0f, 255.0f );

			if( m_eCurrentType == NPCMOB )
				m_pCinematicObject->GetHtmlText()->SetAlpha( byAlpha );
			else
				m_pCinematicObject->GetPCMessage()->SetAlpha( byAlpha );

			//RwChar buf[256];
			//sprintf_s( buf, 256, "Ing State Value : %u", byAlpha );
			//NTL_WARNING( 0, buf );
		}
		else
		{
			if( m_eCurrentType == NPCMOB )
				m_pCinematicObject->GetHtmlText()->SetAlpha( 255 );	
			else
				m_pCinematicObject->GetPCMessage()->SetAlpha( 255 );

			SetState( STATE_IDLE, m_eCurrentType );

			//NTL_WARNING( 0, "Ing State End" );
		}
	}	
	else
	{
		SetState( STATE_IDLE, m_eCurrentType );
	}

	return 1;
}

INT CQuestNarrationGui::EndStateDirection(VOID)
{
	RwReal fCurrentTime = m_pCinematicObject->GetCinematicCurrentTime();

	if( fCurrentTime < CINEMATIC_MOVETIME )
	{
		RwUInt8 ucAlpha = (RwUInt8)gui::GetResultLineInterpolation( fCurrentTime, CINEMATIC_MOVETIME, 255.0f, 0.0f );

		if( m_eCurrentType == NPCMOB )
		{
			if( !( m_flagOwnerType & PC || m_flagOwnerType & PC_SELECT ) )
			{
				m_pCinematicObject->CinematicFadeOut( TRUE, FALSE );
				m_pCinematicObject->CinematicBackFadeOut( FALSE, fCurrentTime );
			}

			m_pCinematicObject->GetImageFrame()->SetAlpha( ucAlpha );
			m_pCinematicObject->SetImageAlpha( ucAlpha );
		}
		else if( m_eCurrentType == PC || m_eCurrentType == PC_SELECT )
		{
			if( !( m_flagOwnerType & NPCMOB ) )
			{
				m_pCinematicObject->CinematicFadeOut( TRUE, FALSE );
				m_pCinematicObject->CinematicBackFadeOut( FALSE, fCurrentTime );
			}

			m_pCinematicObject->GetPCFrame()->SetAlpha( ucAlpha );

			//if( m_bShowPCMessageOnPCSelect )
			//{
			//	m_pCinematicObject->GetImageFrame()->SetAlpha( ucAlpha );
			//	m_pCinematicObject->SetImageAlpha( ucAlpha );
			//}
		}		
		else
		{
			m_pCinematicObject->CinematicFadeOut( TRUE, FALSE );
			m_pCinematicObject->CinematicBackFadeOut( FALSE, fCurrentTime );

			if( m_flagOwnerType & NPCMOB )
			{
				m_pCinematicObject->GetImageFrame()->SetAlpha( ucAlpha );
				m_pCinematicObject->SetImageAlpha( ucAlpha );
			}

			if( m_flagOwnerType & ( PC | PC_SELECT ) )
			{
				m_pCinematicObject->GetPCFrame()->SetAlpha( ucAlpha );
			}	
		}
	}
	else
	{
		SendResultToTS( m_bResult, m_bySelectID );

		if( m_eCurrentType == NPCMOB )
		{
			m_flagOwnerType &= ~NPCMOB;

			if( !( m_flagOwnerType & PC || m_flagOwnerType & PC_SELECT ) )
			{
				m_pCinematicObject->CinematicFadeOut( TRUE, TRUE );
				m_pCinematicObject->CinematicBackFadeOut( TRUE );
				SetState( STATE_NONE, NONE );				
			}

			m_pCinematicObject->GetImageFrame()->SetAlpha( 0 );
			m_pCinematicObject->SetImageAlpha( 0 );			
		}
		else if( m_eCurrentType == PC || m_eCurrentType == PC_SELECT )
		{
			m_flagOwnerType &= ~( PC | PC_SELECT );

			if( !( m_flagOwnerType & NPCMOB ) )
			{
				m_pCinematicObject->CinematicFadeOut( TRUE, TRUE );
				m_pCinematicObject->CinematicBackFadeOut( TRUE );
				SetState( STATE_NONE, NONE );				
			}

			m_pCinematicObject->GetPCFrame()->SetAlpha( 0 );

			//if( m_bShowPCMessageOnPCSelect )
			//{
			//	m_pCinematicObject->GetImageFrame()->SetAlpha( 0 );
			//	m_pCinematicObject->SetImageAlpha( 0 );
			//}			
		}		
		else
		{	
			m_pCinematicObject->CinematicFadeOut( TRUE, TRUE );
			m_pCinematicObject->CinematicBackFadeOut( TRUE );
			m_pCinematicObject->GetImageFrame()->SetAlpha( 0 );
			m_pCinematicObject->SetImageAlpha( 0 );
			m_pCinematicObject->GetPCFrame()->SetAlpha( 0 );
			SetState( STATE_NONE, NONE );				
		}
	}

	return 1;
}

RwBool CQuestNarrationGui::CheckTSState( RwInt32 eTSState, OWNERTYPE eOwnerType )
{
	if( m_eUIState == STATE_NONE )
	{
		if( eTSState != eNARRATION_PROGRESS_STATE_START && eTSState != eNARRATION_PROGRESS_STATE_SINGLE )
		{
			m_eCurrentType = eOwnerType;
			SendResultToTS( false );
			return FALSE;
		}
	}

	return TRUE;
}

VOID CQuestNarrationGui::OnMouseUp( const CKey& key )
{
	if( m_eCurrentType != PC_SELECT )
	{
		if( m_eUIState == STATE_ING )
		{
			if( m_eTextOutType == eNARRATION_DIALOG_DIR_TYPE_BLENDING )
			{
				m_pCinematicObject->SetCinematicCurrentTime( CINEMATIC_TEXT_APPEARTIME );				
			}		
			else if( m_eTextOutType == eNARRATION_DIALOG_DIR_TYPE_LINE )
			{
				m_pCinematicObject->GetHtmlText()->SetSkipPage();
			}			
		}
		else if( m_eUIState == STATE_IDLE )
		{
			if( m_eTSState == eNARRATION_PROGRESS_STATE_END ||
				m_eTSState == eNARRATION_PROGRESS_STATE_SINGLE )
			{
				m_bResult = true;
				SetState( STATE_END, m_eCurrentType );
			}
			else
			{
				SendResultToTS( true );		
			}
		}
	}	
}

VOID CQuestNarrationGui::OnHtmlPageDone( RwInt32 nPage )
{
	if( m_eUIState == STATE_ING )
	{
		if( m_pCinematicObject->GetHtmlText()->GetIntervalPageNums() <= nPage + 1 )
			SetState( STATE_IDLE, m_eCurrentType );
		else
			m_pCinematicObject->GetHtmlText()->SetNextPage();
	}
}

VOID CQuestNarrationGui::OnHtmlPCPageDone( RwInt32 nPage )
{
	if( m_eUIState == STATE_ING )
	{
		if( m_pCinematicObject->GetPCMessage()->GetIntervalPageNums() <= nPage + 1 )
			SetState( STATE_IDLE, m_eCurrentType );
		else
			m_pCinematicObject->GetPCMessage()->SetNextPage();
	}	
}

VOID CQuestNarrationGui::OnPCCSelect1( gui::CComponent* pComponent )
{
	if( m_eTSState == eNARRATION_PROGRESS_STATE_END ||
		m_eTSState == eNARRATION_PROGRESS_STATE_SINGLE )
	{
		m_bResult = true;
		m_bySelectID = m_byPCSelectID[0];
		SetState( STATE_END, m_eCurrentType );
	}
	else
	{
		SendResultToTS( true, m_byPCSelectID[0] );
	}	

	m_pCinematicObject->DisableAllPCSelect();
}

VOID CQuestNarrationGui::OnPCCSelect2( gui::CComponent* pComponent )
{
	if( m_eTSState == eNARRATION_PROGRESS_STATE_END ||
		m_eTSState == eNARRATION_PROGRESS_STATE_SINGLE )
	{
		m_bResult = true;
		m_bySelectID = m_byPCSelectID[1];
		SetState( STATE_END, m_eCurrentType );
	}
	else
	{
		SendResultToTS( true, m_byPCSelectID[1] );
	}	

	m_pCinematicObject->DisableAllPCSelect();
}

VOID CQuestNarrationGui::OnPCCSelect3( gui::CComponent* pComponent )
{
	if( m_eTSState == eNARRATION_PROGRESS_STATE_END ||
		m_eTSState == eNARRATION_PROGRESS_STATE_SINGLE )
	{
		m_bResult = true;
		m_bySelectID = m_byPCSelectID[2];
		SetState( STATE_END, m_eCurrentType );
	}
	else
	{
		SendResultToTS( true, m_byPCSelectID[2] );
	}	

	m_pCinematicObject->DisableAllPCSelect();
}

VOID CQuestNarrationGui::OnPCCSelect4( gui::CComponent* pComponent )
{
	if( m_eTSState == eNARRATION_PROGRESS_STATE_END ||
		m_eTSState == eNARRATION_PROGRESS_STATE_SINGLE )
	{
		m_bResult = true;
		m_bySelectID = m_byPCSelectID[3];
		SetState( STATE_END, m_eCurrentType );
	}
	else
	{
		SendResultToTS( true, m_byPCSelectID[3] );
	}	

	m_pCinematicObject->DisableAllPCSelect();
}

VOID CQuestNarrationGui::OnFocusSelect1( gui::CComponent* pComponent )
{
	m_pCinematicObject->SetPCSelectValue( 0 );
}

VOID CQuestNarrationGui::OnFocusSelect2( gui::CComponent* pComponent )
{
	m_pCinematicObject->SetPCSelectValue( 1 );
}

VOID CQuestNarrationGui::OnFocusSelect3( gui::CComponent* pComponent )
{
	m_pCinematicObject->SetPCSelectValue( 2 );
}

VOID CQuestNarrationGui::OnFocusSelect4( gui::CComponent* pComponent )
{
	m_pCinematicObject->SetPCSelectValue( 3 );
}





