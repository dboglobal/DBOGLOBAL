#include "precomp_dboclient.h"
#include "QuestMessageGui.h"

// core
#include "NtlDebug.h"

// shared
#include "QuestTextDataTable.h"
#include "TimeQuestTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEventFunc.h"
#include "NtlSobAvatar.h"
#include "NtlSLApi.h"
#include "TableContainer.h"
#include "NtlCameraManager.h"
#include "NtlCameraController.h"
#include "NtlSLLogic.h"

#include "DisplayStringManager.h"

// client
#include "DialogManager.h"
#include "DboEvent.h"
#include "QuestGui.h"
#include "QuestProposalGui.h"


#define QUEST_MESSAGE_FONT_INTERVAL		(0.025f)
#define QUEST_MESSAGE_FONT_PRESENT_NUM	(1)			// 추가 -by Kell(09. 07. 15)
#define QUEST_MESSAGE_BTN_PUSLE			(1.0f)

//RwBool stMESSAGEBUTTONITEM::m_bSyncState = TRUE;
//RwReal stMESSAGEBUTTONITEM::m_fSyncTime = 0.0f;

//VOID stMESSAGEBUTTONITEM::Update( RwReal fElapsedTime )
//{
//	if( m_Effect.IsWork() )
//	{
//		m_Effect.Update( fElapsedTime );
//		m_fSyncTime = m_Effect.GetCurrentPulseTime();
//		m_bSyncState = m_Effect.GetState();
//	}
//}

VOID stMESSAGEBUTTONITEM::Enable( RwBool bEnable )
{
	if( bEnable )
	{
		m_pBtn->ClickEnable( TRUE );
		//m_Effect.StartProc( TRUE );
		//m_Effect.SetBeginState( m_bSyncState );
		//m_Effect.SetCurrentPulseTime( m_fSyncTime );
	}
	else
	{
		m_pBtn->ClickEnable( FALSE );
		//m_Effect.EndProc();
	}
}

CQuestMessageGui::CQuestMessageGui( const RwChar* pName )
: CNtlPLGui( pName )
{
	m_phbxMessage = NULL;
	m_pQuestGui = NULL;
	m_bSendFailTS = FALSE;
	m_nReadPage = -1;
	m_eType = INVALID_BYTE;

	InitVisitData();
}

CQuestMessageGui::~CQuestMessageGui(VOID)
{

}

RwBool CQuestMessageGui::Create( CQuestGui* pQuestGui )
{
	NTL_FUNCTION( "CQuestListGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\QuestMessage.srf", "gui\\QuestMessage.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );

	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );

	m_phbxMessage = (gui::CHtmlBox*)GetComponent( "hbxMessage" );
	m_pstbPage = (gui::CStaticBox*)GetComponent( "stbPageInfo" );
	m_pstbQuestTitle = reinterpret_cast<gui::CStaticBox*>( GetComponent( "stbQuestTitle" ) );

	m_MessageButton[NEXT].m_pBtn = (gui::CButton*)GetComponent( "btnNext" );
	m_MessageButton[PREV].m_pBtn = (gui::CButton*)GetComponent( "btnPrev" );
	m_pbtnCancel = (gui::CButton*)GetComponent( "btnCancel" );	
	m_slotMove = m_pThis->SigMove().Connect( this, &CQuestMessageGui::OnMove );
	m_slotPaint = m_pbtnCancel->SigPaint().Connect( this, &CQuestMessageGui::OnPaint );

	m_slotClickNextButton = m_MessageButton[NEXT].m_pBtn->SigClicked().Connect( this, &CQuestMessageGui::OnClickNextButton );
	m_slotClickPrevButton = m_MessageButton[PREV].m_pBtn->SigClicked().Connect( this, &CQuestMessageGui::OnClickPrevButton );
	m_slotClickCancelButton = m_pbtnCancel->SigClicked().Connect( this, &CQuestMessageGui::OnClickCancelButton );	
	m_slotHtmlPageDone = m_phbxMessage->SigPageDone().Connect( this, &CQuestMessageGui::OnHtmlPageDone );
	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CQuestMessageGui::OnCaptureMouseDown );

	// PresentNum 의 갯수를 Define으로 수정합니다. -by Kell(09. 07. 15)
	m_phbxMessage->SetIntervalTextEnable( QUEST_MESSAGE_FONT_INTERVAL );
	m_phbxMessage->SetPresentNum( QUEST_MESSAGE_FONT_PRESENT_NUM );
	m_phbxMessage->SetLineSpace( 11 );

	m_pbtnSkip = (gui::CButton*)GetComponent("btnSkip");
	m_pbtnSkip->SetText(GetDisplayStringManager()->GetString("DST_QUESTMESSAGE_TEXT_SKIP"));
	m_slotClickSkipButton = m_pbtnSkip->SigClicked().Connect(this, &CQuestMessageGui::OnClickSkipButton);

	Show( false );

	m_pQuestGui = pQuestGui;

	LinkMsg( g_EventQuestMessage );
	LinkMsg( g_EventQuestMessage2 );
	
	NTL_RETURN( TRUE );
}

VOID CQuestMessageGui::Destroy(VOID)
{
	UnLinkMsg( g_EventQuestMessage );
	UnLinkMsg( g_EventQuestMessage2 );

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

VOID CQuestMessageGui::Update( RwReal fElapsed )
{
	m_phbxMessage->Update( fElapsed );
}

RwInt32 CQuestMessageGui::SwitchDialog( bool bOpen )
{
	Show( bOpen );

	if( bOpen )
	{
		m_bSendFailTS = FALSE;
		GetNtlGuiManager()->AddUpdateFunc( this );
	}
	else
	{
		InitVisitData();		

		if( m_bSendFailTS )
		{
			m_pQuestGui->GetQuestProposalGui()->SendEchoEvent( false );
			m_bSendFailTS = FALSE;
		}

		if( m_eType == QUEST_MESSAGE_VISIT_QUEST )
		{
			Logic_CancelNpcFacing();
		}

		GetNtlGuiManager()->RemoveUpdateFunc( this );			
	}

	return 1;
}

void CQuestMessageGui::LastPageProc()
{
	if (m_eType == QUEST_MESSAGE_QUEST_TEXT_TABLE)
	{
		if (!GetDialogManager()->IsOpenDialog(DIALOG_QUESTPROPOSAL) &&
			!GetDialogManager()->OpenDialog(DIALOG_QUESTPROPOSAL, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID()))
		{
			m_bSendFailTS = TRUE;
		}
	}
	else if (m_eType == QUEST_MESSAGE_VISIT_QUEST)
	{
		CNtlSLEventGenerator::DoQuest(m_QuestInfo, (eEVENT_GEN_TYPE)m_uiEventGenType, m_uiID, TRUE);
	}

	GetDialogManager()->CloseDialog(DIALOG_QUESTMESSAGE);
}

VOID CQuestMessageGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventQuestDirectForward )
	{
		SNtlEventQuestDirect_Forward* pData = reinterpret_cast<SNtlEventQuestDirect_Forward*>( msg.pData );

		// 중복해서 Proposal이 날라오면 새로들어온 녀석은 무시. 응답은 Proposal에서 보낸다. 
		if( IsShow() )
			return;
		
		if( pData->sProposal.bDisplaySummary )
		{
			CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
			sQUEST_TEXT_DATA_TBLDAT* pQuestText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pData->sProposal.uiQuestContents ) );
			sQUEST_TEXT_DATA_TBLDAT* pQuestTitle = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pData->sProposal.uiQuestTitle ) );

			if( pQuestText )
			{
				m_phbxMessage->SetHtmlFromMemory( pQuestText->wstrText.c_str(), pQuestText->wstrText.size() );
				m_phbxMessage->SetImmediate( FALSE );
				SetPageInfo();
			}
			else if( pData->sProposal.uiQuestContents != INVALID_TBLIDX && Logic_IsUIDevInfoVisible() )
			{
				char buf[256];
				sprintf_s( buf, 256, "CQuestMessageGui::HandleEvents : Invalid QuestTextTable ID : %u", pData->sProposal.uiQuestContents );
				m_phbxMessage->SetHtmlFromMemory( buf, strlen( buf ) );			
			}

			if( pQuestTitle )
			{
				m_pstbQuestTitle->SetText( gui::GetHtmlFromMemoryString( pQuestTitle->wstrText.c_str(), pQuestTitle->wstrText.size() ).c_str() );
			}
			else if( pData->sProposal.uiQuestTitle != INVALID_TBLIDX && Logic_IsUIDevInfoVisible() )
			{
				char buf[256];
				sprintf_s( buf, 256, "CQuestMessageGui::HandleEvents : Invalid QuestTitleTable ID : %u", pData->sProposal.uiQuestTitle );
				m_pstbQuestTitle->SetText( buf );
			}		

			m_nReadPage = -1;
			m_eType = QUEST_MESSAGE_QUEST_TEXT_TABLE;

			// Visit 퀘스트 관련 데이터 초기화
			m_uiEventGenType = (RwUInt32)eEVENT_GEN_TYPE_INVALID;
			m_uiID = 0;

			// 열려 있는 경우도 생각해야함. 위에서 열려있지 않은 경우에만 들어오므로 이것만 검사
			if( !GetDialogManager()->OpenDialog( DIALOG_QUESTMESSAGE, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() ) )
				m_pQuestGui->GetQuestProposalGui()->SendEchoEvent( false );
		}		
	}
	else if( msg.Id == g_EventQuestMessage )
	{
		if( IsShow() && m_eType == QUEST_MESSAGE_QUEST_TEXT_TABLE )
			return;

		SDboEventQuestMessage* pData = reinterpret_cast<SDboEventQuestMessage*>( msg.pData );
		CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();

		// 제목
		CNtlTMQ* pTMQ = GetNtlSLGlobal()->GetSobAvatar()->GetTMQ();
		sTIMEQUEST_TBLDAT* pTIMEQUEST_TBLDAT = pTMQ->GetTMQTable();
		sTIMEQUEST_DATASET* pTIMEQUEST_DATASET = &pTIMEQUEST_TBLDAT->sTimeQuestDataset[pTMQ->GetDifficult()];
		sQUEST_TEXT_DATA_TBLDAT* pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pTIMEQUEST_DATASET->nameTblidx ) );
		if( pQUEST_TEXT_DATA_TBLDAT )
		{
			std::wstring& wstrText = gui::GetHtmlFromMemoryString(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), pQUEST_TEXT_DATA_TBLDAT->wstrText.size());
			m_pstbQuestTitle->SetText( wstrText.c_str() );
		}
		else
		{
			DBO_WARNING_MESSAGE("Not exist Quest text data table of index : " << pTIMEQUEST_DATASET->nameTblidx);
		}		


		TBLIDX uiQuestTextIndex = pData->uiValue2 + 100 * (pData->uiValue + 21000);
		sQUEST_TEXT_DATA_TBLDAT* pQUEST_TEXT_DATA_TBLDAT_Content = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( uiQuestTextIndex ) );
		if( !pQUEST_TEXT_DATA_TBLDAT_Content )
		{
			if( uiQuestTextIndex != INVALID_TBLIDX && Logic_IsUIDevInfoVisible() )
			{
				char acBuffer[128] = "";
				sprintf_s(acBuffer, 128, "Not exist quest text data of index : %u", uiQuestTextIndex);
				m_phbxMessage->SetHtmlFromMemory( acBuffer, strlen(acBuffer) );
			}			
		}
		else
		{
			m_phbxMessage->SetHtmlFromMemory( pQUEST_TEXT_DATA_TBLDAT_Content->wstrText.c_str(),
											  pQUEST_TEXT_DATA_TBLDAT_Content->wstrText.size() );
		}
		
		m_phbxMessage->SetImmediate( FALSE );
		SetPageInfo();		

		m_nReadPage = -1;
		m_eType = QUEST_MESSAGE_TMQ_SCRIPT;

		// Visit 퀘스트 관련 데이터 초기화
		m_uiEventGenType = (RwUInt32)eEVENT_GEN_TYPE_INVALID;
		m_uiID = 0;

		// DirectForward는 QuestGui에서 다이얼로그를 오픈하지만 여기선 직접해야함.
		GetDialogManager()->OpenDialog( DIALOG_QUESTMESSAGE );	
	}
	else if( msg.Id == g_EventQuestMessage2 )
	{
		if( IsShow() && m_eType == QUEST_MESSAGE_QUEST_TEXT_TABLE )
			return;

		SDboEventQuestMessage2* pData = reinterpret_cast<SDboEventQuestMessage2*>( msg.pData );
		CQuestTextDataTable* pQuestTable = API_GetTableContainer()->GetQuestTextDataTable();
		sQUEST_TEXT_DATA_TBLDAT* pQUEST_TEXT_DATA_TBLDAT = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTable->FindData(pData->pQuestInfo->uiVisitDiag) );
		if( pQUEST_TEXT_DATA_TBLDAT )
		{
			m_phbxMessage->SetHtmlFromMemory(pQUEST_TEXT_DATA_TBLDAT->wstrText.c_str(), pQUEST_TEXT_DATA_TBLDAT->wstrText.size());
		}
		else
		{
			if( pData->pQuestInfo->uiVisitDiag != INVALID_TBLIDX && Logic_IsUIDevInfoVisible() )
			{
				WCHAR awcBuffer[64] = L"";
				swprintf_s(awcBuffer, 64, L"Not exist quest text table of index : %u", pData->pQuestInfo->uiVisitDiag);
				m_phbxMessage->SetHtmlFromMemory(awcBuffer, 64);
			}
		}

		sQUEST_TEXT_DATA_TBLDAT* pQuestTitle = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTable->FindData( pData->pQuestInfo->uiVisitTitle ) );
		
		if( pQuestTitle )
		{
			m_pstbQuestTitle->SetText( gui::GetHtmlFromMemoryString( pQuestTitle->wstrText.c_str(), pQuestTitle->wstrText.size() ).c_str() );
		}
		else if( pData->pQuestInfo->uiVisitTitle != INVALID_TBLIDX && Logic_IsUIDevInfoVisible() )
		{
			char buf[256];
			sprintf_s( buf, 256, "CQuestMessageGui::HandleEvents : Invalid QuestTitleTable ID : %u", pData->pQuestInfo->uiVisitTitle );
			m_pstbQuestTitle->SetText( buf );
		}		
		
		m_phbxMessage->SetImmediate( FALSE );
		SetPageInfo();

		m_nReadPage = -1;
		m_eType = QUEST_MESSAGE_VISIT_QUEST;

		m_QuestInfo			= *pData->pQuestInfo;
		m_uiEventGenType	= pData->uiEventGenType;
		m_uiID				= pData->uiID;

		// DirectForward는 QuestGui에서 다이얼로그를 오픈하지만 여기선 직접해야함.
		GetDialogManager()->OpenDialog( DIALOG_QUESTMESSAGE );
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pData = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		if( pData->eWorldConcept == WORLD_PLAY_TIME_MACHINE )
		{
			if( pData->uiState == WORLD_STATE_EXIT )
			{
				GetDialogManager()->CloseDialog( DIALOG_QUESTMESSAGE, FALSE );
			}	
		}
	}
	else if( msg.Id == g_EventDialog )
	{
		SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>( msg.pData );

		if( pData->iType == DIALOGEVENT_NPC_BYEBYE && pData->iDestDialog == DIALOG_QUESTMESSAGE )
		{
			OnClickCancelButton( NULL );
		}
	}
}

VOID CQuestMessageGui::SetPageInfo(VOID)
{
	m_pstbPage->Format( L"%d / %d", m_phbxMessage->GetCurrentPage() + 1, m_phbxMessage->GetIntervalPageNums() );

	if( m_phbxMessage->GetCurrentPage() == 0 )
		m_MessageButton[PREV].Enable( FALSE );
	else
		m_MessageButton[PREV].Enable( TRUE );
}

VOID CQuestMessageGui::InitVisitData(VOID)
{
	m_uiEventGenType = (RwUInt32)eEVENT_GEN_TYPE_INVALID;
	m_uiID = 0;
}

VOID CQuestMessageGui::OnMove( RwInt32 nX, RwInt32 nY )
{
	//for( RwInt32 i = 0 ; i < BTNCOUNT ; ++i )
	//{
	//	CRectangle rtScreen = m_MessageButton[i].m_pBtn->GetScreenRect();
	//	m_MessageButton[i].m_Effect.SetCenterPos( rtScreen.left + rtScreen.GetWidth() / 2, rtScreen.top + rtScreen.GetHeight() / 2 );
	//}
}

VOID CQuestMessageGui::OnPaint(VOID)
{
}

VOID CQuestMessageGui::OnClickNextButton( gui::CComponent* pComponent )
{
	// 마지막 페이지
	if( m_phbxMessage->GetCurrentPage() + 1 >= m_phbxMessage->GetIntervalPageNums() )
	{
		LastPageProc();
	}
	else
	{
		if( m_phbxMessage->GetCurrentPage() >= m_nReadPage )
			m_phbxMessage->SetImmediate( FALSE );		
		else 
			m_phbxMessage->SetImmediate( TRUE );	

		if( m_phbxMessage->SetNextPage() )
			SetPageInfo();
		else
			NTL_ASSERTFAIL( "CQuestMessageGui::OnClickNextButton : Unexpected result" );	
	}		
}

VOID CQuestMessageGui::OnClickPrevButton( gui::CComponent* pComponent )
{
	if( m_phbxMessage->GetCurrentPage() == 0 )
		return;
	
	if( m_phbxMessage->SetPrevPage() )
		SetPageInfo();
	else
		NTL_ASSERTFAIL( "CQuestMessageGui::OnClickPrevButton : Unexpected result" );
	
	m_phbxMessage->SetImmediate( TRUE );

	//if( m_phbxMessage->GetCurrentPage() == 0 )
	//{
	//	m_MessageButton[PREV].Enable( FALSE );		
	//}
	//else
	//{
	//	m_MessageButton[PREV].Enable( TRUE );		
	//}	
}

VOID CQuestMessageGui::OnClickCancelButton( gui::CComponent* pComponent )
{
	//for( RwInt32 i = 0 ; i < BTNCOUNT ; ++i )
	//{
	//	m_MessageButton[i].Enable( FALSE );
	//}	
	
	if( m_eType == QUEST_MESSAGE_QUEST_TEXT_TABLE )
	{
		m_bSendFailTS = TRUE;			
	}

	GetDialogManager()->CloseDialog( DIALOG_QUESTMESSAGE );	
}

VOID CQuestMessageGui::OnClickSkipButton(gui::CComponent * pComponent)
{
	LastPageProc();
}

VOID CQuestMessageGui::OnHtmlPageDone( RwInt32 nPage )
{
	//m_MessageButton[NEXT].Enable( TRUE );		
	
	if( m_nReadPage < nPage )
		m_nReadPage = nPage;
}

VOID CQuestMessageGui::OnCaptureMouseDown( const CKey& key )
{
	CAPTURE_MOUSEDOWN_RAISE( DIALOG_QUESTMESSAGE, key.m_fX, key.m_fY );
}
