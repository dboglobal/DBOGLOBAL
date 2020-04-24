#include "precomp_dboclient.h"
#include "MultiDialogGui.h"

// core
#include "NtlDebug.h"

// shared
#include "NPCTable.h"
#include "TextAllTable.h"
#include "QuestTextDataTable.h"
#include "ObjectTable.h"
#include "MerchantTable.h"
#include "ItemTable.h"
#include "TableContainer.h"
#include "ItemMixMachineTable.h"

// sound
#include "GUISoundDefine.h"

// gui
#include "GuiUtil.h"
#include "AltarGui.h"
#include "NtlWorldConceptDBC.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSlEventFunc.h"
#include "NtlSLLogicDef.h"
#include "NtlSLLogic.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSobManager.h"
#include "NtlSLApi.h"
#include "NtlSobNpc.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobAvatar.h"
#include "NtlSobTriggerObject.h"
#include "NtlSobTriggerObjectAttr.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "DboTSCQAgency.h"
#include "DboTSCTAgency.h"
#include "NtlSLApi.h"
#include "NtlGuild.h"
#include "NtlWareHouse.h"
#include "NtlWorldConcept.h"
#include "NtlCameraController.h"
#include "NtlWorldConcept.h"
#include "NtlFSMUtil.h"
#include "NtlSobDynamicObject.h"
#include "NtlSobDynamicObjectAttr.h"


// dbo
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "ChatBalloonManager.h"
#include "QuestGui.h"
#include "DisplayStringManager.h"
#include "DboGlobal.h"
#include "DojoInfoGui.h"
#include "DboLogic.h"
#include "AlarmManager.h"

//test
#include "NtlMath.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Size Define

#define COMMANDLIST_X					9
#define COMMANDLIST_Y					23	
#define COMMANDLIST_WIDTH				255
#define COMMANDLIST_HEIGHT				138

#define COMMANDLIST_LINE_HEIGHT			22
#define COMMANDLIST_LINE_MARGIN			7
#define COMMANDLIST_CHILD_XMARGIN		0
#define COMMANDLIST_SCROLLBAR_WIDTH		12
#define COMMANDLIST_SLIDER_HEIGHT		26

#define COMMANDNODE_X					0
#define COMMANDNODE_Y					0
#define COMMANDNODE_WIDTH				239
#define COMMANDNODE_HEIGHT				22

#define COMMANDTEXT_X					30

#define COMMANDNODEHTML_X				32				
#define COMMANDNODEHTML_Y				3	
#define COMMANDNODEHTML_WIDTH			217
#define COMMANDNODEHTML_HEIGHT			20

#define BRANCHCOMMANDNODEHTML_X			13				
#define BRANCHCOMMANDNODEHTML_Y			3	
#define BRANCHCOMMANDNODEHTML_WIDTH		226
#define BRANCHCOMMANDNODEHTML_HEIGHT	20

#define COMMANDNODEICON_X				15
#define COMMANDNODEICON_Y				12

#define COMMANDNODE_DOWNCOORD_DIFFX		1
#define COMMANDNODE_DOWNCOORD_DIFFY		1

#define SERVERSELECTNODE_USERCOUNT_X	193
#define SERVERSELECTNODE_USERCOUNT_Y	4

#define MULTIDIALOG_MSGBOX_Y_OFFSET		10

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CMultiDialogGui

CMultiDialogGui::CMultiDialogGui(VOID)
: m_pCommandList( NULL ), m_hTargetSerialID( INVALID_SERIAL_ID ), m_pQuestGui( NULL ), m_pstbMsg( NULL )
{

}

CMultiDialogGui::CMultiDialogGui( const RwChar* pName )
: CNtlPLGui( pName ), m_pCommandList( NULL ), m_hTargetSerialID( INVALID_SERIAL_ID ), m_pQuestGui( NULL ), m_pstbMsg( NULL )
{

}

CMultiDialogGui::~CMultiDialogGui(VOID)
{

}

RwBool CMultiDialogGui::Create( CQuestGui* pQuestGui )
{
	NTL_FUNCTION( "CMultiDialogGui::Create" );

	if( !CNtlPLGui::Create( "", "gui\\MultiDialog.srf", "gui\\MultiDialog.frm" ) )
		NTL_RETURN( FALSE );

	CNtlPLGui::CreateComponents( GetNtlGuiManager()->GetGuiManager() );
	m_pThis = (gui::CDialog*)GetComponent( "dlgMain" );
	m_pstbMsg = (gui::CStaticBox*)GetComponent( "stbMsg" );
	m_MsgBackImage.SetType( CWindowby3::WT_HORIZONTAL );

	m_MsgBackImage.SetSurface( 0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfSecondTextBackup" ) );
	m_MsgBackImage.SetSurface( 1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfSecondTextBackCenter" ) );
	m_MsgBackImage.SetSurface( 2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfSecondTextBackDown" ) );

	m_slotCaptureMouseDown = GetNtlGuiManager()->GetGuiManager()->SigCaptureMouseDown().Connect( this, &CMultiDialogGui::OnCaptureMouseDown );
	m_slotCaptureWheelMove = GetNtlGuiManager()->GetGuiManager()->SigCaptureWheelMove().Connect( this, &CMultiDialogGui::OnCaptureWheelMove );

	m_slotMsgResize = m_pstbMsg->SigResize().Connect( this, &CMultiDialogGui::OnMsgResize );
	m_slotMsgMove = m_pstbMsg->SigMove().Connect( this, &CMultiDialogGui::OnMsgMove );
	m_slotPaint = m_pThis->SigPaint().Connect( this, &CMultiDialogGui::OnPaint );

	SetDialogType( TYPE_NONE );	

	Show( false );

	m_pCommandList = NTL_NEW CCommandList;
	CRectangle rect;
	rect.SetRectWH( COMMANDLIST_X, COMMANDLIST_Y, COMMANDLIST_WIDTH, COMMANDLIST_HEIGHT );
	if( !m_pCommandList->Create( rect, m_pThis, COMMANDLIST_LINE_HEIGHT, COMMANDLIST_LINE_MARGIN, COMMANDLIST_CHILD_XMARGIN, COMMANDLIST_SCROLLBAR_WIDTH, this ) )
		NTL_RETURN( FALSE );

	LinkMsg( g_EventSobNpcCommunity, 0 );
	LinkMsg( g_EventSobTriggerObjCommunity, 0 );
	LinkMsg( g_EventSobDynamicObjCommunity, 0 );
	LinkMsg( g_EventCharObjDelete, 0 );
	LinkMsg( g_EventMsgBoxResult, 0 );
	LinkMsg( g_EventTSMudosaTeleport_Req );
	LinkMsg( g_EventBudokaiMudosaInfoRes );
	LinkMsg( g_EventGambleAniEnd );
	LinkMsg( g_EventShopGambleBuyRes );

	m_pQuestGui = pQuestGui;

	NTL_RETURN( TRUE );
}

VOID CMultiDialogGui::Destroy(VOID)
{
	UnLinkMsg( g_EventSobNpcCommunity );
	UnLinkMsg( g_EventSobTriggerObjCommunity );
	UnLinkMsg( g_EventSobDynamicObjCommunity );
	UnLinkMsg( g_EventCharObjDelete );
	UnLinkMsg( g_EventMsgBoxResult );
	UnLinkMsg( g_EventTSMudosaTeleport_Req );
	UnLinkMsg( g_EventBudokaiMudosaInfoRes );
	UnLinkMsg( g_EventGambleAniEnd );
	UnLinkMsg( g_EventShopGambleBuyRes );

	if( m_pCommandList )
	{
		m_pCommandList->Destroy();
		NTL_DELETE( m_pCommandList );
	}	

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();
}

RwInt32 CMultiDialogGui::SwitchDialog( bool bOpen )
{
	// 무조건 닫는다.
	m_pstbMsg->Show( false );	
	m_MsgBackImage.Show( false );

	if( bOpen )
	{
		if( m_stMultiDialogData.eBalloonType == sMultiDialogData::TYPE_NONE )
		{
			if( m_eDlgType == TYPE_NONE )	
			{
				//GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG, FALSE );
				m_stMultiDialogData.Clear();
				m_hTargetSerialID = INVALID_SERIAL_ID;
				m_eDlgType = TYPE_NONE;	
				return -1;
			}
		}
		else
		{
			CNtlSob* pTargetObject = GetNtlSobManager()->GetSobObject( m_hTargetSerialID );		

			if( m_stMultiDialogData.eBalloonType == sMultiDialogData::TYPE_NPC_BALLOON )
			{
				if( pTargetObject )
				{
					GetBalloonManager()->AddNPCBalloon( pTargetObject, m_stMultiDialogData.wstrText.c_str(), BALLOON_NPC_DISPTIME, FALSE, m_stMultiDialogData.bTimeProc, 3 );					
				}
				else
				{
					SetMsgBox( m_stMultiDialogData.wstrText );					
					m_MsgBackImage.Show( true );
				}				
			}
			else if( m_stMultiDialogData.eBalloonType == sMultiDialogData::TYPE_QUEST_BALLOON )
			{
				if( pTargetObject )
				{
					GetBalloonManager()->AddQuestBalloon( pTargetObject, m_stMultiDialogData.wstrText, m_stMultiDialogData.uiQuestID, m_stMultiDialogData.bTimeProc );					
				}
				else
				{
					SetMsgBox( m_stMultiDialogData.wstrText );					
					m_MsgBackImage.Show( true );
				}							
			}
			else if( m_stMultiDialogData.eBalloonType == sMultiDialogData::TYPE_JUST_NPC_BALLOON )
			{
				if( pTargetObject )
				{
					GetBalloonManager()->AddNPCBalloon( pTargetObject, m_stMultiDialogData.wstrText.c_str(), BALLOON_NPC_DISPTIME, FALSE, m_stMultiDialogData.bTimeProc, 3 );
					//GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG, FALSE );					
				}				
				m_stMultiDialogData.Clear();
				m_hTargetSerialID = INVALID_SERIAL_ID;
				m_eDlgType = TYPE_NONE;	
				return -1;
			}
			else if( m_stMultiDialogData.eBalloonType == sMultiDialogData::TYPE_JUST_QUEST_BALLOON )
			{
				if( pTargetObject )
				{
					GetBalloonManager()->AddQuestBalloon( pTargetObject, m_stMultiDialogData.wstrText, m_stMultiDialogData.uiQuestID, m_stMultiDialogData.bTimeProc );
					//GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG, FALSE );					
				}				
				m_stMultiDialogData.Clear();
				m_hTargetSerialID = INVALID_SERIAL_ID;
				m_eDlgType = TYPE_NONE;	
				return -1;
			}			
			else if( m_stMultiDialogData.eBalloonType == sMultiDialogData::TYPE_JUST_MSG )
			{
				SetMsgBox( m_stMultiDialogData.wstrText );					
				m_MsgBackImage.Show( true );
			}
		}	

		m_stMultiDialogData.Clear();
		RaiseTop();
	}
	else
	{
		CNtlSob* pSobObject = GetNtlSobManager()->GetSobObject( m_hTargetSerialID );

		if( m_eDlgType == TYPE_NPCQUESTTALK )
		{
			CNtlSLEventGenerator::QuestNPCDialog_Res( m_pCommandList->GetTSKey() );

			if( pSobObject )
			{
				if( GetBalloonManager()->HasQuestBalloon( pSobObject ) )
					GetBalloonManager()->FadeOutBalloon( pSobObject, CBalloonGui::FIRST_TYPE_LAYER );
			}			
		}
		else if( m_eDlgType == TYPE_OBJQUESTTALK )
		{
			CNtlSLEventGenerator::QuestObjDialog_Res( m_pCommandList->GetTSKey() );
			if( pSobObject )
			{
				if( GetBalloonManager()->HasQuestBalloon( pSobObject ) )
					GetBalloonManager()->FadeOutBalloon( pSobObject, CBalloonGui::FIRST_TYPE_LAYER );
			}
		}		
		else if( m_eDlgType == TYPE_USERSELECT )
		{
			m_pCommandList->SendUserSelect( NTL_TS_TC_ID_INVALID, false );
			if( pSobObject )
			{
				if( GetBalloonManager()->HasQuestBalloon( pSobObject ) )
					GetBalloonManager()->FadeOutBalloon( pSobObject, CBalloonGui::FIRST_TYPE_LAYER );
			}			
		}
		else if( m_eDlgType == TYPE_NPCTALK || m_eDlgType == TYPE_QUESTCAT )
		{
			if( pSobObject )
			{
				if( GetBalloonManager()->HasNPCBalloon( pSobObject ) )
					GetBalloonManager()->FadeOutBalloon( pSobObject, CBalloonGui::FIRST_TYPE_LAYER );
			}			
		}
		// Gamble 일 경우 겜블 확인 창을 닫아준다.
		else if( m_eDlgType == TYPE_GAMBLE )
		{
			if( pSobObject )
			{
				if( GetDialogManager()->IsOpenDialog( DIALOG_TBGAMBLERESULT ) )
					GetDialogManager()->CloseDialog( DIALOG_TBGAMBLERESULT );

				if( GetBalloonManager()->HasNPCBalloon( pSobObject ) )
					GetBalloonManager()->FadeOutBalloon( pSobObject, CBalloonGui::FIRST_TYPE_LAYER );
			}
		}

		m_stMultiDialogData.Clear();
		m_hTargetSerialID = INVALID_SERIAL_ID;
		m_eDlgType = TYPE_NONE;				
	}

	Show( bOpen );

	return 1;
}

VOID CMultiDialogGui::SendTriggerAction( vecdef_QUEST_INFO_LIST* pTriggerList, RwUInt8 eEventGenType, RwUInt32 uiOwnerID )
{
	RwInt32 nTriggerCount = (RwInt32)( pTriggerList->size() );
	if( nTriggerCount <= 0 )
		return;

	for( RwInt32 i = 0 ; i < nTriggerCount ; ++i )
	{
		CNtlSLEventGenerator::DoQuest( (*pTriggerList)[i], (eEVENT_GEN_TYPE)eEventGenType, uiOwnerID, TRUE );
	}
}

VOID CMultiDialogGui::ReserveDialog( std::wstring& wstrText, RwUInt32 uiQuestID, RwInt32 eType, RwBool bTimeProc )
{
	m_stMultiDialogData.eBalloonType = eType;
	m_stMultiDialogData.wstrText = wstrText;
	m_stMultiDialogData.bTimeProc = bTimeProc;
	m_stMultiDialogData.uiQuestID = uiQuestID;
}

VOID CMultiDialogGui::ReserveDialog( std::wstring& wstrText, RwInt32 eType, RwBool bTimeProc )
{
	m_stMultiDialogData.eBalloonType = eType;
	m_stMultiDialogData.wstrText = wstrText;
	m_stMultiDialogData.bTimeProc = bTimeProc;
}

VOID CMultiDialogGui::SetMsgBox( std::wstring& wstrText )
{
	m_pstbMsg->SetText( gui::GetHtmlFromMemoryString( wstrText.c_str(), wstrText.size() ).c_str() );
	m_pstbMsg->Show( true );
}

VOID CMultiDialogGui::HandleEvents( RWS::CMsg& msg )
{
	if( msg.Id == g_EventSobNpcCommunity )
	{
		SNtlEventSobNpcCommunity* pData = reinterpret_cast<SNtlEventSobNpcCommunity*>( msg.pData );

		// Block all triggers related dialogs when they are on.
		// Prevent window from being clicked before it opens
		if (GetDialogManager()->IsTriggerSystemDialogOpen())
		{
			return;
		}

		CNtlSobNpc* pNPC = reinterpret_cast<CNtlSobNpc*>( GetNtlSobManager()->GetSobObject( pData->hSerialId ) );
		DBO_ASSERT( pNPC, "CMultiDialogGui::HandleEvents : Invalid NPC SerialID" );

		// Understand the state of the character
		if( pNPC->IsClientUIDisable() )
			return;

		m_hTargetSerialID = pData->hSerialId;
		CNtlSobNpcAttr* pNPCAttr = reinterpret_cast<CNtlSobNpcAttr*>( pNPC->GetSobAttr() );
		sNPC_TBLDAT* pTableData = pNPCAttr->GetNpcTbl();

		// NPC quest and job check.
		RwUInt64	dwMultiDialogJobFlag = NPC_FUNC_FLAG_MERCHANT | NPC_FUNC_FLAG_GUARD | NPC_FUNC_FLAG_SKILL_TRAINER | NPC_FUNC_FLAG_BANKER |
			NPC_FUNC_FLAG_GUILD_MANAGER | NPC_FUNC_FLAG_SUMMON_PET | NPC_FUNC_FLAG_TIME_QUEST | NPC_FUNC_FLAG_PORTAL |
			NPC_FUNC_FLAG_BUDOHSI_MERCHANT | NPC_FUNC_FLAG_GAMBLE_MERCHANT |
			NPC_FUNC_FLAG_DOJO_MANAGER | NPC_FUNC_FLAG_DOJO_MERCHANT | NPC_FUNC_FLAG_BATTLE_DUNGEON_MANAGER | NPC_FUNC_FLAG_ITEM_UPGRADE | NPC_FUNC_FLAG_ITEM_EXCHANGE;
		RwBool		bEnableQuest = FALSE;	

		if ( GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency() == NULL || GetNtlSLGlobal()->GetSobAvatar()->GetPCTriggerAgency() == NULL)
		{
			return;
		}

		SGET_QUEST_INFO* pGetQuestList = GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GetQuestInfoList( (eEVENT_GEN_TYPE)pData->byInputType, pData->uiTriggerId );
		SGET_QUEST_INFO* pGetTriggerList = GetNtlSLGlobal()->GetSobAvatar()->GetPCTriggerAgency()->GetQuestInfoList( (eEVENT_GEN_TYPE)pData->byInputType, pData->uiTriggerId );

		vecdef_QUEST_INFO_LIST* vQuestList = &pGetQuestList->QuestInfoList;
		vecdef_QUEST_INFO_LIST* vTriggerList = &pGetTriggerList->QuestInfoList;

		if( vQuestList->size() > 0 )
		{
			bEnableQuest = TRUE;
		}
		else
		{
			SendTriggerAction( vTriggerList, pData->byInputType, pData->uiTriggerId );
		}

	//	DBO_WARNING_MESSAGE("npc idx: " << pTableData->tblidx << ", hHandle: " << pData->hSerialId << ", pTableData->byJob: " << (int)pTableData->byJob);
	//	for (int f = 0; f < NPC_FUNC_COUNT; f++)
	//	{
	//		if (pTableData->dwFunc_Bit_Flag & MAKE_BIT_FLAG64(f))
	//			DBO_WARNING_MESSAGE("has bitflag.. flag: " << f);
	//	}

		if( pTableData->dwFunc_Bit_Flag & dwMultiDialogJobFlag || bEnableQuest )
		{
			// World Concept Check Second world concept is under way
			if( GetNtlWorldConcept()->IsActivePlayGrade( WORLD_CONCEPT_SECOND_GRADE ) )
			{
				// Release the TargetSerialID you set and print a message related to WorldConcept.
				m_hTargetSerialID = INVALID_SERIAL_ID;
				Logic_WarnningWorldConceptActionMessage( WORLD_CONCEPT_SECOND_GRADE );
				return;
			}

			m_pCommandList->ClearNodes();			

			//! Quest is the first.
			if( bEnableQuest )
			{
				m_pCommandList->SetID( pData->uiTriggerId );
				m_pCommandList->SetInputType( pData->byInputType );
				m_pCommandList->SendQuest();
			}

			if (pTableData->byJob == NPC_JOB_ITEM_UPGRADE) // is upgrade npc?
			{
				m_pCommandList->RegistCommand(CCommandList::UPGRADE_COMMAND);
				m_pCommandList->RegistCommand(CCommandList::SMITH_CHANGE_BATTLE_ATTRIBUTE_COMMAND);
			//	m_pCommandList->RegistCommand(CCommandList::SMITH_TRANSFER_ITEM_GRADE);
			}
			else if (pTableData->byJob == NPC_JOB_EVENT_NPC) // is event manager npc?
			{
				m_pCommandList->RegistCommand(CCommandList::EVENT_REWARD_COMMAND);
			}
			else if (pTableData->byJob == NPC_JOB_DWC_TELEPORT) // is dwc porter?
			{
				m_pCommandList->RegistCommand(CCommandList::DWC_COMMAND);
			}
			else if (pTableData->byJob == NPC_JOB_BATTLE_DUNGEON_MANAGER)
			{
				if (pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_BATTLE_DUNGEON_MANAGER)
				{
					m_pCommandList->RegistCommand(CCommandList::CCBD_COMMAND);
				}
			}

			// 제조 기술 초기화

			if (pTableData->byJob == NPC_JOB_VENDING_MACHINE)
			{
				if (pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_SUMMON_PET)
				{
					SetDialogType(TYPE_NONE);

					CNtlSLEventGenerator::CameraNpc(pData->hSerialId);
					GetNtlWorldConcept()->AddWorldPlayConcept(WORLD_PLAY_NPC_COMMU);

					// npc handle Save (used in FSM - fluorspar)
					CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(GetNtlSLGlobal()->GetSobAvatar()->GetBehaviorData());
					SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
					pCtrlStuff->uExtra.sTeleport.hTeleportNpc = pData->hSerialId;

					GetDialogManager()->OpenDialog(DIALOG_BIND_SEALING, pData->hSerialId);

					return;
				}
			}

			if( pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_TIME_QUEST )
			{
				m_pCommandList->RegistCommand( CCommandList::TMQ_COMMAND );
			}

			if (pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_GUARD)
			{
				m_pCommandList->RegistCommand(CCommandList::UD_COMMAND);
			}

			if( pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_SKILL_TRAINER )
			{
				m_pCommandList->RegistCommand( CCommandList::SKILLTRAINER_COMMAND, pTableData->byJob );
			}

			if( pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_DOJO_MANAGER )
			{
				CNtlSobAvatar* pAvatar	= GetNtlSLGlobal()->GetSobAvatar();
				if( pAvatar )
				{
					CNtlGuild*	pGuild		= pAvatar->GetGuild();
					CNtlDojo*	pAvatarDojo	= pAvatar->GetDojo();
					sDOJO_INFO*	pDOJO_INFO	= pAvatarDojo->GetDojoInfo( pTableData->contentsTblidx );

					if( !pDOJO_INFO || INVALID_GUILDID == pDOJO_INFO->guildID )
					{
						m_pCommandList->RegistCommand( CCommandList::GUILD_DOJO_ESTABLISHMENT, pTableData->byJob );
					}
					else
					{
						m_pCommandList->RegistCommand( CCommandList::GUILD_DOJO_INFO, pTableData->byJob );
					}

					m_pCommandList->RegistCommand( CCommandList::GUILD_DOJO_POINTUSE, pTableData->byJob );
				}
			}

			if( pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_MERCHANT ||
				pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_DOJO_MERCHANT )
			{
				// The First Prize Merchant has the NPC_FUNC_FLAG_MERCHANT and NPC_FUNC_FLAG_BUDOHSI_MERCHANT flags
				// Call the RegistCommand function from the NPC_FUNC_FLAG_BUDOHSI_MERCHANT flag checkpoint
				if( pTableData->byJob != NPC_JOB_BUDOHSI_MERCHANT && pTableData->byJob != NPC_JOB_BUDOHSI_MERCHANT2 && pTableData->byJob != NPC_JOB_BUDOHSI_MERCHANT3)
				{
					m_pCommandList->RegistCommand( CCommandList::MERCHANT_COMMAND, pTableData->byJob );
				}
			}

			if (pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_ITEM_EXCHANGE)
			{
				m_pCommandList->RegistCommand(CCommandList::EVENT_ITEM_EXCHANGE_COMMAND, pTableData->byJob);
			}


			if( pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_BUDOHSI_MERCHANT )
			{
				m_pCommandList->RegistCommand( CCommandList::BUDOHSI_MERCHANT_COMMAND, pTableData->byJob );				
			}

			if( pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_BANKER )
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( pAvatar )
				{
					m_pCommandList->RegistCommand( CCommandList::BANKER_COMMAND );

					if( GetNtlSLGlobal()->GetSobAvatar()->GetGuildWarehouse()->CanUseGuileWarehouse() )
						m_pCommandList->RegistCommand( CCommandList::GUILD_BANK_COMMAND );

					m_pCommandList->RegistCommand( CCommandList::BUY_BANK_COMMAND );

					// First, when you talk to the warehouse NPC, you get your warehouse information from the server.
					// You need to know the warehouse information so you can find out which warehouse you can buy more

					CNtlWarehouse* pWarehouse = pAvatar->GetWarehouse();
					if( pWarehouse->GetSlotSerailID(0) == INVALID_SERIAL_ID )
						GetDboGlobal()->GetGamePacketGenerator()->SendBankLoadReq( pNPC->GetSerialID() );
				}								
			}

			if( pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_GUILD_MANAGER )
			{
				m_pCommandList->RegistCommand( CCommandList::GUILD_COMMAND );				
			}

			if(pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_PORTAL)       // 포탈맨
			{
				m_pCommandList->RegistCommand( CCommandList::PORTAL_COMMAND );	
			}
			
			// Gamble 비용은 Merchant Table의 Need_Mileage 필드의 값을 참조하는 것으로 변경한다.
			// -by Kell ( 09. 07. 22 )
			if(pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_GAMBLE_MERCHANT )
			{
				m_pCommandList->RegistCommand( CCommandList::GAMBLE_BUY_COMMNAD );	
				m_pCommandList->RegistCommand( CCommandList::EXIT_COMMAND );

				// Merchant Table의 가장 첫번째 Index를 참조한다.
				if( pTableData->amerchant_Tblidx[0] == INVALID_TBLIDX )
				{
					DBO_FAIL( "[GAMBLE] NPC Table havn't merchant table index." << pTableData->tblidx );
					return;
				}
				
				sMERCHANT_TBLDAT* pMerchantTblDat = (sMERCHANT_TBLDAT*)API_GetTableContainer()->GetMerchantTable()->FindData( pTableData->amerchant_Tblidx[0] );
				if( !pMerchantTblDat )
				{
					DBO_FAIL( "[GAMBLE] Merchant table index is invalid." << pTableData->amerchant_Tblidx[0] );
					return;
				}

				WCHAR awcBuffer[256];

				if (pMerchantTblDat->bySell_Type == MERCHANT_SELL_TYPE_GAMBLE)
				{
					swprintf_s(awcBuffer, 256, GetDisplayStringManager()->GetString("DST_GAMBLE_GUIDE"), pMerchantTblDat->dwNeedMileage);
				}
				else if (pMerchantTblDat->bySell_Type == MERCHANT_SELL_TYPE_GAMBLE_ZENNY)
				{
					swprintf_s(awcBuffer, 256, GetDisplayStringManager()->GetString("DST_GAMBLE_GUIDE_ZENNY"), Logic_FormatZeni(pMerchantTblDat->dwNeedMileage));
				}

				ReserveDialog( std::wstring(awcBuffer), sMultiDialogData::TYPE_JUST_MSG, FALSE );

				SetDialogType( TYPE_NPCTALK );
				if( GetDialogManager()->OpenDialog( DIALOG_MULTIDIALOG, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() ) )
				{
					CNtlSLEventGenerator::CameraNpc( pData->hSerialId );
					GetNtlWorldConcept()->AddWorldPlayConcept( WORLD_PLAY_NPC_COMMU );

					// Npc handle 저장( FSM 에서 사용 )
					CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(GetNtlSLGlobal()->GetSobAvatar()->GetBehaviorData()); 
					SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
					pCtrlStuff->uExtra.sGamble.hGambleNpc = pData->hSerialId;
				}				

				return;
			}

			// exit
			m_pCommandList->RegistCommand( CCommandList::EXIT_COMMAND );

			// Dialog내용을 입력.
			CTextTable* pNPCText = API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();
			std::wstring wstrText;
			pNPCText->GetText( pTableData->Dialog_Script_Index, &wstrText );

			if (NULL == GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency())
			{
				return;
			}

			if( GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->HasEventNPCInProgressQuest( pTableData->tblidx ) )
				ReserveDialog( std::wstring(), sMultiDialogData::TYPE_NONE, FALSE );
			else
				ReserveDialog( wstrText, sMultiDialogData::TYPE_NPC_BALLOON, FALSE );			

			// Dialog Print
			SetDialogType( TYPE_NPCTALK );
			if( GetDialogManager()->OpenDialog( DIALOG_MULTIDIALOG, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() ) )
			{
				CNtlSLEventGenerator::CameraNpc( pData->hSerialId );
				GetNtlWorldConcept()->AddWorldPlayConcept( WORLD_PLAY_NPC_COMMU );

				// npc handle Save (used in FSM - fluorspar)
				CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(GetNtlSLGlobal()->GetSobAvatar()->GetBehaviorData()); 
				SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
				pCtrlStuff->uExtra.sTeleport.hTeleportNpc = pData->hSerialId;
			}
		}
		else
		{
			if (pTableData->Dialog_Script_Index == INVALID_TBLIDX)
			{
				return;
			}

			CTextTable* pNPCTextTable = API_GetTableContainer()->GetTextAllTable()->GetNPCTbl();
			std::wstring wstrText;
			pNPCTextTable->GetText( pTableData->Dialog_Script_Index, &wstrText );

			if (NULL == GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency())
			{
				return;
			}
			if( GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->HasEventNPCInProgressQuest( pTableData->tblidx ) )
				ReserveDialog( std::wstring(), sMultiDialogData::TYPE_NONE, FALSE );
			else
				ReserveDialog( wstrText, sMultiDialogData::TYPE_JUST_NPC_BALLOON, TRUE );

			// It is a Dialog output command, but it is used only for closing the NPCDialog, etc. immediately after it is actually opened.
			SetDialogType( TYPE_NONE );
			GetDialogManager()->OpenDialog( DIALOG_MULTIDIALOG, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );
		}

		// by daneos. We send packet when we update camera to face npc (CNtlSLEventGenerator::CameraNpc( pData->hSerialId );)
		//// Use this if you are currently running npc (fluorspar)
		//CNtlSobActor *pSobActor = reinterpret_cast<CNtlSobActor*>( GetNtlSobManager()->GetSobObject(pData->hSerialId) );
		//if(pSobActor)
		//{
		//	if (pTableData->dwFunc_Bit_Flag & NPC_FUNC_FLAG_FACING)
		//	{
		//		API_GetSLPacketGenerator()->SendCharTargetFacing(pData->hSerialId, true);
		//	}
		//}
	}
	else if( msg.Id == g_EventSobTriggerObjCommunity )
	{
		SNtlEventSobTriggerObjCommunity* pData = reinterpret_cast<SNtlEventSobTriggerObjCommunity*>( msg.pData );

		// When all triggers are turned on, they are blocked in the multi dialog.
		if( GetDialogManager()->IsTriggerSystemDialogOpen() || m_hTargetSerialID == pData->hSerialId )
			return;

		if (NULL == GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency() ||
			NULL == GetNtlSLGlobal()->GetSobAvatar()->GetPCTriggerAgency())
		{
			DBO_WARNING_MESSAGE("GetQuestAgency == NULL || GetPCTriggerAgency == NULL");
			return;
		}

		SGET_QUEST_INFO* pGetQuestList = GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GetQuestInfoList( (eEVENT_GEN_TYPE)pData->byInputType, pData->uiTriggerId );
		SGET_QUEST_INFO* pGetTriggerList = GetNtlSLGlobal()->GetSobAvatar()->GetPCTriggerAgency()->GetQuestInfoList( (eEVENT_GEN_TYPE)pData->byInputType, pData->uiTriggerId );

		vecdef_QUEST_INFO_LIST* vQuestList = &pGetQuestList->QuestInfoList;
		vecdef_QUEST_INFO_LIST* vTriggerList = &pGetTriggerList->QuestInfoList;

		if( vQuestList->size() > 0 )
		{
			m_hTargetSerialID = pData->hSerialId;

			m_pCommandList->SetID( pData->uiTriggerId );
			m_pCommandList->SetInputType( pData->byInputType );
			m_pCommandList->TriggerObjectSendQuest();

			// Dialog Print
			SetDialogType(TYPE_NPCTALK);
			if (GetDialogManager()->OpenDialog(DIALOG_MULTIDIALOG, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID()))
			{
				CNtlSLEventGenerator::CameraNpc(pData->hSerialId);
				GetNtlWorldConcept()->AddWorldPlayConcept(WORLD_PLAY_NPC_COMMU);

				// npc handle Save (used in FSM - fluorspar)
				CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(GetNtlSLGlobal()->GetSobAvatar()->GetBehaviorData());
				SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
				pCtrlStuff->uExtra.sTeleport.hTeleportNpc = pData->hSerialId;
			}
		}
		else
		{
			if( pGetTriggerList->dwType & eDBO_TRIGGER_OBJECT_FUNC_BIND )
			{
				CNtlSobTriggerObject* pTriggerObj = reinterpret_cast<CNtlSobTriggerObject*>( GetNtlSobManager()->GetSobObject( pData->hSerialId ) );
				DBO_ASSERT( pTriggerObj, "CMultiDialogGui::HandleEvents : TriggerObj Serial( " << pData->hSerialId << " ) Does not exist." );
				CNtlSobTriggerObjectAttr* pTriggerObjAttr = reinterpret_cast<CNtlSobTriggerObjectAttr*>( pTriggerObj->GetSobAttr() );

				sMsgBoxData data;
				data.uiTblIdx = pTriggerObjAttr->GetTriggerObjectTbl()->tblidx;
				GetAlarmManager()->AlarmMessage( "DST_BIND_TEXT", FALSE, &data );
			}

			if( pGetTriggerList->dwType & eDBO_TRIGGER_OBJECT_FUNC_PORTAL )
			{
				// 포탈
			}

			if( pGetTriggerList->dwType & eDBO_TRIGGER_OBJECT_FUNC_SCOUTER )
			{

			}

			if (pGetTriggerList->dwType & eDBO_TRIGGER_OBJECT_FUNC_AUCTION_HOUSE)
			{
				CDboEventGenerator::NPCDialogOpen(pData->hSerialId, DIALOG_AUCTION_HOUSE);
			}

			if( pGetTriggerList->dwType & eDBO_TRIGGER_OBJECT_FUNC_SELECTION )
			{

			}

			if( pGetTriggerList->dwType & eDBO_TRIGGER_OBJECT_FUNC_RANK_BATTLE )
			{
				CDboEventGenerator::NPCDialogOpen( pData->hSerialId, DIALOG_RBBOARD );
			}

			if( pGetTriggerList->dwType & eDBO_TRIGGER_OBJECT_FUNC_MAIL_BOX )
			{
				CDboEventGenerator::NPCDialogOpen( pData->hSerialId, DIALOG_MAILSYSTEM );
			}

			if((pGetTriggerList->dwType & eDBO_TRIGGER_OBJECT_FUNC_NORMAL_DRAGONBALL) ||
				(pGetTriggerList->dwType & eDBO_TRIGGER_OBJECT_FUNC_RAID_DRAGONBALL))
			{
				// 드래곤볼 용신 소환
				GetNtlWorldConcept()->AddWorldPlayConcept(WORLD_PLAY_DRAGONBALL_COLLECT);
				CNtlWorldConceptDBC* pWorldConceptDBC = (CNtlWorldConceptDBC*)GetNtlWorldConcept()->GetWorldConceptController(WORLD_PLAY_DRAGONBALL_COLLECT);
				pWorldConceptDBC->SetAltarSerialID(pData->hSerialId);

				CAltarGui::CreateInstance();				
				GetDialogManager()->OpenDialog(DIALOG_DBC_ALTAR);
			}

			if( pGetTriggerList->dwType & eDBO_TRIGGER_OBJECT_FUNC_HOIPOIMIX )
			{
				m_hTargetSerialID = pData->hSerialId;
							
				m_pCommandList->ClearNodes();			

				m_pCommandList->RegistCommand( CCommandList::HOIPOIMIX_CREATE_COMMAND );

				// 나가기는 기본.
				m_pCommandList->RegistCommand( CCommandList::EXIT_COMMAND );

				// Dialog 출력
				SetDialogType( TYPE_NPCTALK );
				if( GetDialogManager()->OpenDialog( DIALOG_MULTIDIALOG, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() ) )
				{
					CNtlSLEventGenerator::CameraNpc( pData->hSerialId );
					GetNtlWorldConcept()->AddWorldPlayConcept( WORLD_PLAY_NPC_COMMU );

					// npc handle 저장(FSM에서 사용 - 형석)
					CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(GetNtlSLGlobal()->GetSobAvatar()->GetBehaviorData()); 
					SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
					pCtrlStuff->uExtra.sTeleport.hTeleportNpc = pData->hSerialId;
				}				
			}

			SendTriggerAction( vTriggerList, pData->byInputType, pData->uiTriggerId );
		}

		//
		API_GetSLPacketGenerator()->SendTSExcuteTriggerObject( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), pData->hSerialId, pData->byInputType, 0xffffffff );
	}
	else if( msg.Id == g_EventQuestDirectForward )
	{
		// peessi : User Select인 경우에만 들어온다.

		SNtlEventQuestDirect_Forward* pData = reinterpret_cast<SNtlEventQuestDirect_Forward*>( msg.pData );
		SNtlEventQuestUserSelectDialog_Req* pReqData = &pData->sUserSelect;

		// peessi : MultiDialog가 열려있다면, 커뮤니티 상태 이거나, 퀘스트 다이얼로그, 유저 선택중 하나 이므로 취소.
		if( IsShow() )
		{
			SNtlEventQuestDirect_Echo stEcho;
			stEcho.sProposal.bResult = false;
			stEcho.sProposal.sTSKey	= pReqData->sTSKey;
			stEcho.eTDType = ETD_QuestUserSelect;
			stEcho.pTCUnit = pData->pTCUnit;

			CNtlSLEventGenerator::QuestTriggerDirectEcho( &stEcho );
			return;
		}

		// Serial 입력.
		if( pReqData->uiTargetType == eUSER_SEL_TARGET_TYPE_NPC )
			m_hTargetSerialID = GetNtlSobManager()->GetNpcTriggerSerialFromId( pReqData->uiTargetTblIdx );
		else if( pReqData->uiTargetType == eUSER_SEL_TARGET_TYPE_OBJECT )
			m_hTargetSerialID = GetNtlSobManager()->GetObjectTriggerSerialFromId( pReqData->uiTargetTblIdx );

		// 모드
		m_pCommandList->ClearNodes();
		m_pCommandList->SetTCUnit( pData->pTCUnit );
		m_pCommandList->SetTSKey( pReqData->sTSKey );

		CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
		sQUEST_TEXT_DATA_TBLDAT* pQuestText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pReqData->uiConv ) );
		if( pQuestText )
		{
			ReserveDialog( pQuestText->wstrText, pReqData->uiConv, sMultiDialogData::TYPE_QUEST_BALLOON, FALSE );			
		}

		for( RwInt32 i = 0 ; i < pReqData->nBranchCnt ; ++i )
		{	
			m_pCommandList->RegistQuestCommand( pReqData->asBranchInfo[i].uiBranchText, pReqData->asBranchInfo[i].tcID );
		}
		m_pCommandList->RegistExitCommand( CCommandNode::BRANCHDIALOG_NODE );

		SetDialogType( TYPE_USERSELECT );	
		GetDialogManager()->OpenDialog( DIALOG_MULTIDIALOG, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );
	}
	else if( msg.Id == g_EventQuestNPCDialog_Req )
	{
		SNtlEventQuestNPCDialog_Req* pData = reinterpret_cast<SNtlEventQuestNPCDialog_Req*>( msg.pData );

		// peessi : If MultiDialog is open, it is either a community state, a quest dialog, or a user selection..
		if( IsShow() )
		{
			CNtlSLEventGenerator::QuestNPCDialog_Res( pData->sTSKey );
			return;
		}

		RwUInt32 hSerial = GetNtlSobManager()->GetNpcTriggerSerialFromId( pData->uiNPCIdx );
		if( hSerial == INVALID_SERIAL_ID )
		{
			CNtlSLEventGenerator::QuestNPCDialog_Res( pData->sTSKey );
			return;
		}

		m_hTargetSerialID = hSerial;

		if( pData->eNPCConvType == eNPCCONV_TYPE_DIALOGBOX )
		{
			m_pCommandList->ClearNodes();
			m_pCommandList->SetTSKey( pData->sTSKey );
			m_pCommandList->RegistExitCommand( CCommandNode::QUESTDIALOG_NODE );

			CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable()	;
			sQUEST_TEXT_DATA_TBLDAT* pQuestText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pData->uiDesc ) );

			SetDialogType( TYPE_NPCQUESTTALK );

			if( pQuestTextTable )
				ReserveDialog( pQuestText->wstrText, pData->uiDesc, sMultiDialogData::TYPE_QUEST_BALLOON, FALSE );				
			else
				ReserveDialog( std::wstring( L"InvalidQuestText" ), pData->uiDesc, sMultiDialogData::TYPE_QUEST_BALLOON, FALSE );				
		}
		else if( pData->eNPCConvType == eNPCCONV_TYPE_TOOLTIP )
		{
			CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
			sQUEST_TEXT_DATA_TBLDAT* pQuestText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pData->uiDesc ) );

			SetDialogType( TYPE_NONE );

			if( pQuestText )
				ReserveDialog( pQuestText->wstrText, pData->uiDesc, sMultiDialogData::TYPE_JUST_QUEST_BALLOON, TRUE );			
			else
				ReserveDialog( std::wstring( L"InvalidQuestText" ), pData->uiDesc, sMultiDialogData::TYPE_JUST_QUEST_BALLOON, TRUE );			

			CNtlSLEventGenerator::QuestNPCDialog_Res( pData->sTSKey );
		}		

		GetDialogManager()->OpenDialog( DIALOG_MULTIDIALOG, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );
	}
	else if( msg.Id == g_EventQuestObjDialog_Req )
	{
		SNtlEventQuestObjDialog_Req* pData = reinterpret_cast<SNtlEventQuestObjDialog_Req*>( msg.pData );

		// peessi : If MultiDialog is open, it is either a community state, a quest dialog, or a user selection.
		if( IsShow() )
		{
			CNtlSLEventGenerator::QuestObjDialog_Res( pData->sTSKey );
			return;
		}

		RwUInt32 hSerial = GetNtlSobManager()->GetObjectTriggerSerialFromId( pData->uiObjIdx );
		if( hSerial == INVALID_SERIAL_ID )
		{
			CNtlSLEventGenerator::QuestObjDialog_Res( pData->sTSKey );
			return;
		}

		m_hTargetSerialID = hSerial;

		if( pData->eObjConvType == eOBJCONV_TYPE_DIALOGBOX )
		{
			m_pCommandList->ClearNodes();
			m_pCommandList->SetTSKey( pData->sTSKey );
			m_pCommandList->RegistExitCommand( CCommandNode::QUESTDIALOG_NODE );

			CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable()	;
			sQUEST_TEXT_DATA_TBLDAT* pQuestText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pData->uiDesc ) );

			SetDialogType( TYPE_OBJQUESTTALK );

			if( pQuestText )
				ReserveDialog( pQuestText->wstrText, pData->uiDesc, sMultiDialogData::TYPE_QUEST_BALLOON, FALSE );			
			else
				ReserveDialog( std::wstring( L"Invalid QuestText" ), pData->uiDesc, sMultiDialogData::TYPE_QUEST_BALLOON, FALSE );			
		}
		else if( pData->eObjConvType == eOBJCONV_TYPE_TOOLTIP )
		{
			CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
			sQUEST_TEXT_DATA_TBLDAT* pQuestText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( pData->uiDesc ) );

			SetDialogType( TYPE_NONE );	

			if( pQuestText )
				ReserveDialog( pQuestText->wstrText, pData->uiDesc, sMultiDialogData::TYPE_JUST_QUEST_BALLOON, TRUE );			
			else
				ReserveDialog( std::wstring( L"Invalid QuestText" ), pData->uiDesc, sMultiDialogData::TYPE_JUST_QUEST_BALLOON, TRUE );			

			CNtlSLEventGenerator::QuestObjDialog_Res( pData->sTSKey );
		}		

		GetDialogManager()->OpenDialog( DIALOG_MULTIDIALOG, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );
	}
	else if( msg.Id == g_EventDialog )
	{
		SDboEventDialog* pData = reinterpret_cast<SDboEventDialog*>( msg.pData );

		if( pData->iType == DIALOGEVENT_NPC_BYEBYE && pData->iDestDialog == DIALOG_MULTIDIALOG )
		{
			if( m_eDlgType == TYPE_USERSELECT )
			{
				m_pCommandList->SendUserSelect( NTL_TS_TC_ID_INVALID, false );
			}
			else if( m_eDlgType == TYPE_SERVERSELECT )
			{
				CNtlSLEventGenerator::TSMudosaTeleport_Res( m_pCommandList->GetTSKey(), INVALID_BYTE, TRUE );				
			}
			else
			{
				Logic_CancelNpcFacing();
			}			

			GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );			
		}		
	}
	else if( msg.Id == g_EventCharObjDelete )
	{
		RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>( msg.pData );

		if( m_hTargetSerialID != INVALID_SERIAL_ID && *pDeleteSerial == m_hTargetSerialID )
		{
			if( m_eDlgType == TYPE_USERSELECT )
				m_pCommandList->SendUserSelect( NTL_TS_TC_ID_INVALID, false );
			else
			{
				Logic_CancelNpcFacing();
			}			

			GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );			
		}
	}
	else if( msg.Id == g_EventMsgBoxResult )
	{
		SDboEventMsgBoxResult *pMsgBoxResult = reinterpret_cast<SDboEventMsgBoxResult*>( msg.pData );

		if (pMsgBoxResult->eResult == MBR_OK)
		{
			if (pMsgBoxResult->strID == "DST_BIND_TEXT")
			{
				API_GetSLPacketGenerator()->SendCharBindReq(pMsgBoxResult->pData->uiTblIdx);
			}
		}
	}
	else if( msg.Id == g_EventUnregQuest_Nfy )
	{
		switch( m_eDlgType )
		{
		case TYPE_USERSELECT:			
		case TYPE_NPCQUESTTALK:
		case TYPE_OBJQUESTTALK:
			{
				SNtlEventUnregQuest_Nfy* pDeleteData = reinterpret_cast<SNtlEventUnregQuest_Nfy*>( msg.pData );

				if( pDeleteData->sTSKey.tID == m_pCommandList->GetTSKey().tID )
				{
					GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );
				}
			}
			break;
		case TYPE_QUESTCAT:
			{
				// 리스트 재출력
				m_pCommandList->SendQuest();
			}
			break;
		default:
			return;
		}
	}
	else if( msg.Id == g_EventTSRemovingTMQQuest_Nfy )
	{
		switch( m_eDlgType )
		{
		case TYPE_USERSELECT:
		case TYPE_NPCQUESTTALK:
		case TYPE_OBJQUESTTALK:
			{
				SNtlEventTSRemovingTMQQuest_Nfy* pData = reinterpret_cast<SNtlEventTSRemovingTMQQuest_Nfy*>( msg.pData );

				if( pData->tRmvTSId == m_pCommandList->GetTSKey().tID )
				{
					GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );
				}
			}
			break;
		case TYPE_QUESTCAT:
			{
				// 리스트 재출력
				m_pCommandList->SendQuest();
			}
			break;
		}
	}
	else if( msg.Id == g_EventTSMudosaTeleport_Req )
	{
		// 무도사 리스트 안내 패킷 실행 무도사 패킷 받고 그걸로 실행
		SNtlEventTSMudosaTeleport_Req* pData = reinterpret_cast<SNtlEventTSMudosaTeleport_Req*>( msg.pData );
		m_pCommandList->SetTSKey( pData->sTSKey );

		GetDboGlobal()->GetGamePacketGenerator()->SendBudokaiMudosaInfoReq();
	}
	else if( msg.Id == g_EventBudokaiMudosaInfoRes )
	{
		SNtlEventBudokaiMudosaInfoRes* pData = reinterpret_cast<SNtlEventBudokaiMudosaInfoRes*>( msg.pData );

		m_pCommandList->ClearNodes();

		for( RwInt32 i = 0 ; i < pData->byMudosaCount ; ++i )
		{
			WCHAR awcBuffer[256];
			swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_MUDOSA_INFO" ), i );
			m_pCommandList->RegistServerSelectCommand( i, awcBuffer, pData->aMudosaInfo[i].wCurrentUserCount );			
		}		

		m_pCommandList->RegistExitCommand( CCommandNode::SERVERSELECT_MODE );

		SetDialogType( TYPE_SERVERSELECT );
		ReserveDialog( std::wstring( GetDisplayStringManager()->GetString( "DST_BUDOKAI_MUDOSA_TELEPOR_INFO" ) ), sMultiDialogData::TYPE_JUST_MSG, FALSE );
		GetDialogManager()->OpenDialog( DIALOG_MULTIDIALOG, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );
	}
	// 겜블 NPC의 애니메이션이 끝났을 경우 이제 서버쪽으로 패킷을 요청해야 한다.
	else if( msg.Id == g_EventGambleAniEnd )
	{
		SNtlEventGambleAniEnd* pData = reinterpret_cast<SNtlEventGambleAniEnd*>( msg.pData );

		GetDboGlobal()->GetGamePacketGenerator()->SendShopGambleBuyReq( pData->hGambleNpc );
	}
	else if( msg.Id == g_EventShopGambleBuyRes )
	{
		SDboEventShopGambleBuyRes* pResult = reinterpret_cast<SDboEventShopGambleBuyRes*>( msg.pData );

		m_hTargetSerialID = pResult->hHandle;

		CNtlSobNpc* pNPC = reinterpret_cast<CNtlSobNpc*>( GetNtlSobManager()->GetSobObject( GetTargetSerial() ) );
		DBO_ASSERT( pNPC, "CMultiDialogGui::HandleEvents - Invalid NPC SerialID" );

		m_pCommandList->ClearNodes();
		m_pCommandList->RegistCommand( CCommandList::GAMBLE_BUY_COMMNAD );	
		m_pCommandList->RegistCommand( CCommandList::EXIT_COMMAND );

		CNtlSobNpcAttr* pNPCAttr = reinterpret_cast<CNtlSobNpcAttr*>( pNPC->GetSobAttr() );
		sNPC_TBLDAT* pTableData = pNPCAttr->GetNpcTbl();

		// Merchant Table의 가장 첫번째 Index를 참조한다.
		if( pTableData->amerchant_Tblidx[0] == INVALID_TBLIDX )
		{
			DBO_FAIL( "[GAMBLE] NPC Table havn't merchant table index." << pTableData->tblidx );
			return;
		}

		sMERCHANT_TBLDAT* pMerchantTblDat = (sMERCHANT_TBLDAT*)API_GetTableContainer()->GetMerchantTable()->FindData( pTableData->amerchant_Tblidx[0] );
		if( !pMerchantTblDat )
		{
			DBO_FAIL( "[GAMBLE] Merchant table index is invalid." << pTableData->amerchant_Tblidx[0] );
			return;
		}

		SetDialogType( TYPE_GAMBLE );
		WCHAR awcBuffer[256];

		if (pMerchantTblDat->bySell_Type == MERCHANT_SELL_TYPE_GAMBLE)
		{
			swprintf_s(awcBuffer, 256, GetDisplayStringManager()->GetString("DST_GAMBLE_GUIDE"), pMerchantTblDat->dwNeedMileage);
		}
		else if (pMerchantTblDat->bySell_Type == MERCHANT_SELL_TYPE_GAMBLE_ZENNY)
		{
			swprintf_s(awcBuffer, 256, GetDisplayStringManager()->GetString("DST_GAMBLE_GUIDE_ZENNY"), Logic_FormatZeni(pMerchantTblDat->dwNeedMileage));
		}

		ReserveDialog( std::wstring(awcBuffer), sMultiDialogData::TYPE_JUST_MSG, FALSE );
		GetDialogManager()->OpenDialog( DIALOG_MULTIDIALOG, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() );

		if( pResult->wResultCode != GAME_SUCCESS )
		{
			GetAlarmManager()->AlarmMessage(Logic_GetResultCodeString(pResult->wResultCode, ""), TRUE );
			return;
		}

		CNtlSobItem* pSobItem =  reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(pResult->hItem));
		if( pSobItem == NULL )
		{
			DBO_ASSERT( pNPC, "CMultiDialogGui::HandleEvents - Invalid Item Handle" );
			return;
		}
	}
	else if( msg.Id == g_EventSobDynamicObjCommunity )
	{
		SNtlEventSobDynamicObjCommunity* pData = reinterpret_cast<SNtlEventSobDynamicObjCommunity*>( msg.pData );

		// 모든 트리거 관련 창이 켜져 있을때 시발점인 멀티다이얼로그에서 막는다. 
		// 창이 열리기 전에 연속 클릭되는 것을 방지
		if( GetDialogManager()->IsTriggerSystemDialogOpen() || m_hTargetSerialID == pData->hSerialId )
			return;

		CNtlSobDynamicObject* pObj = reinterpret_cast<CNtlSobDynamicObject*>( GetNtlSobManager()->GetSobObject( pData->hSerialId ) );
		DBO_ASSERT( pObj, "CSkillWindowGui::HandleEvents : Invalid NPC SerialID" );

		m_hTargetSerialID = pData->hSerialId;
		CNtlSobDynamicObjectAttr* pObjAttr = reinterpret_cast<CNtlSobDynamicObjectAttr*>( pObj->GetSobAttr() );

		m_pCommandList->ClearNodes();

		RwUInt8 byDynamicObjectType = pObjAttr->GetDynamicObjectType();
		switch( byDynamicObjectType )
		{
			case eDBO_DYNAMIC_OBJECT_TYPE_HOIPOI_MIX_MACHINE:
			{
				// HoipoiMix Table
				CItemMixMachineTable* pMachineTable = API_GetTableContainer()->GetItemMixMachineTable();
				NTL_ASSERT( pMachineTable, "ItemMixMachine Table is null");

				sITEM_MIX_MACHINE_TBLDAT* pTblData = (sITEM_MIX_MACHINE_TBLDAT*)pMachineTable->FindData( pObjAttr->GetTableIndexForType() );
				NTL_ASSERT( pTblData, "ItemMixMachine Table index is invalid." << pObjAttr->GetTableIndexForType() );

				m_pCommandList->RegistCommand( CCommandList::HOIPOIMIX_CREATE_COMMAND );

				
			}
			break;
			case eDBO_DYNAMIC_OBJECT_TYPE_CAPSULE_HOUSE:
			{
			}
			break;
			default:
			{
			}
			break;
		}
			
		// 나가기는 기본.
		m_pCommandList->RegistCommand( CCommandList::EXIT_COMMAND );

		// Dialog 출력
		SetDialogType( TYPE_NPCTALK );
		if( GetDialogManager()->OpenDialog( DIALOG_MULTIDIALOG, GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() ) )
		{
			CNtlSLEventGenerator::CameraNpc( pData->hSerialId );
			GetNtlWorldConcept()->AddWorldPlayConcept( WORLD_PLAY_NPC_COMMU );

			// npc handle 저장(FSM에서 사용 - 형석)
			CNtlBeCharData *pBeData = reinterpret_cast<CNtlBeCharData*>(GetNtlSLGlobal()->GetSobAvatar()->GetBehaviorData()); 
			SCtrlStuff *pCtrlStuff = pBeData->GetCtrlStuff();
			pCtrlStuff->uExtra.sTeleport.hTeleportNpc = pData->hSerialId;
		}
	}
}

//VOID CMultiDialogGui::OnClickExitButton( gui::CComponent* pComponent )
//{
//	GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );
//}

VOID CMultiDialogGui::OnCaptureMouseDown( const CKey& key )
{
	CAPTURE_MOUSEDOWN_RAISE( DIALOG_MULTIDIALOG, key.m_fX, key.m_fY );
}

VOID CMultiDialogGui::OnCaptureWheelMove( RwInt32 nFlag, RwInt16 sDelta, CPos& pos )
{
	CAPTURE_MOUSEWHEELMOVE_SCROLLBAR( IsShow(), m_pThis, m_pThis, m_pCommandList->GetScrollBar(), sDelta, pos );
}

VOID CMultiDialogGui::OnMsgResize( RwInt32 nOldCX, RwInt32 nOldCY )
{
	CRectangle rtMsg = m_pstbMsg->GetScreenRect();
	m_MsgBackImage.SetSize( rtMsg.GetWidth() + 10, rtMsg.GetHeight() + 9 );
	m_pstbMsg->SetPosition( 0, -( rtMsg.GetHeight() + MULTIDIALOG_MSGBOX_Y_OFFSET ) );
}

VOID CMultiDialogGui::OnMsgMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rtMsg = m_pstbMsg->GetScreenRect();

	m_MsgBackImage.SetPosition( rtMsg.left, rtMsg.top );
}

VOID CMultiDialogGui::OnPaint(VOID)
{
	m_MsgBackImage.Render();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCommandList

CCommandList::CCommandList(VOID)
: ClearProc( &CCommandList::ClearState_No ), m_uiId( 0xffffffff ), m_byInputType( 0xff ), m_pTCUnit( NULL )
{

}

CCommandList::~CCommandList(VOID)
{

}

RwBool CCommandList::Create( CRectangle& rect, gui::CComponent* pParent, RwInt32 nLineHeight, RwInt32 nLineMargin, RwInt32 nChildXMargin, RwInt32 nScrollBarWidth, CMultiDialogGui* pOwnerGui )
{
	if( !CGuiLineTree::Create( rect, pParent, nLineHeight, nLineMargin, nChildXMargin, nScrollBarWidth ) )
		return FALSE;

	m_pScroll->AddSurfaceLayout( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfScrollLayout" ) );
	m_pScroll->AddSurfaceSlider( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfScrollSmall" ) );
	m_pScroll->SetButtonSize( 0, 0 );
	m_pScroll->SetSliderSize( COMMANDLIST_SCROLLBAR_WIDTH, COMMANDLIST_SLIDER_HEIGHT );

	m_slotPaint = m_pLineTree->SigPaint().Connect( this, &CCommandList::OnClear );

	m_pOwnerGui = pOwnerGui;

	return TRUE;
}

VOID CCommandList::RegistCommand( RwInt32 nID, RwUInt8 ucJob /* = 0xFF  */)
{
	gui::CSurface	surface;
	std::wstring	wstrText;

	switch( nID )
	{
		case EXIT_COMMAND:
		{
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfExitIcon");
			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_EXIT");
		}
		break;

		case MERCHANT_COMMAND:
		{
			surface = Logic_GetNPCClassIconSurface(ucJob, FALSE);

			/*switch (ucJob)
			{


				case NPC_JOB_WEAPON_MERCHANT: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfWeaponMerchantIcon"); break;
				case NPC_JOB_ARMOR_MERCHANT: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfArmorMerchantIcon"); break;
				case NPC_JOB_GOODS_MERCHANT: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfMiscMerchantIcon"); break;
				case NPC_JOB_SCOUTER_MERCHANT: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfScouter_Merchant"); break;
				case NPC_JOB_DOGI_MERCHANT: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfDogiMerchantIcon"); break;
				case NPC_JOB_SPECIAL_WEAPON_MERCHANT: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfSPWeaponMerchantIcon"); break;
				case NPC_JOB_SPECIAL_ARMOR_MERCHANT: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfSPArmorMerchantIcon"); break;
				case NPC_JOB_SPECIAL_GOODS_MERCHANT: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfSPMiscMerchantIcon"); break;
				case NPC_JOB_SPECIAL_SCOUTER_MERCHANT: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfSPScouterMerchantIcon"); break;
				case NPC_JOB_SPECIAL_FOODS_MERCHANT: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfSPFoodMerchantIcon"); break;
				case NPC_JOB_SUB_WEAPON_MERCHANT: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfSubWeaponMerchantIcon"); break;
				case NPC_JOB_DOJO_MANAGER: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfSubWeaponMerchantIcon"); break;
				case NPC_JOB_DOJO_MERCHANT: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfSubWeaponMerchantIcon"); break;
				case NPC_JOB_DOJO_SEAL: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfSubWeaponMerchantIcon"); break;
				case NPC_JOB_MIX_MASTER: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfSubWeaponMerchantIcon"); break; ///< 변경해야함

				case NPC_JOB_VEHICLE_MERCHANT: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfVehicle_Merchant"); break;

				case NPC_JOB_SCOUTER_MERCHANT2: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfScouter_Merchant"); break;

				default: DBO_FAIL("NPC Table Job, JobFlag Error: " << (int)ucJob); break;
			}*/

			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_SHOP");
		}
		break;

		case SKILLTRAINER_COMMAND:
		{
			if (ucJob >= NPC_JOB_SKILL_TRAINER_HFI && ucJob <= NPC_JOB_SKILL_TRAINER_MWO)
				surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfSkillMasterIcon");
			else if (ucJob >= NPC_JOB_GRAND_SKILL_TRAINER_HFI && ucJob <= NPC_JOB_GRAND_SKILL_TRAINER_MWO)
				surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfGrandSkillMasterIcon");
			else
				DBO_FAIL("NPC Table Job, JobFlag Error: " << (int)ucJob);

			//wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_SKILL"); // in beta to learn skill from NPC
			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_SKILL_INIT");
		}
		break;

		case BANKER_COMMAND:
		{
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfWareHouseIcon");
			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_BANK");
		}
		break;

		case BUY_BANK_COMMAND:
		{
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfWareHouseIcon");
			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_BUY_BANK");
		}
		break;

		case QUEST_COMMAND:
		{
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfQuestIcon");
			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_QUEST");
		}
		break;

		case UPGRADE_COMMAND:
		{
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfWeaponMerchantIcon");
			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_UPGRADE");
		}
		break;

		case GUILD_COMMAND:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfGuildIcon");
			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_GUILD");
			break;

		case TMQ_COMMAND:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfTimeMachineIcon");
			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_TMQ");
			break;

		case UD_COMMAND:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfTimeMachineIcon");
			wstrText = GetDisplayStringManager()->GetString("DST_ULTIMATE_DUNGEON_TITLE");
			break;

		case GUILD_BANK_COMMAND:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfWareHouseIcon");
			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_OPEN_GUILD_BANK");
			break;

		case BUDOHSI_MERCHANT_COMMAND:
		{
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfSubWeaponMerchantIcon");
			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_SHOP");
		}
		break;

		case EVENT_ITEM_EXCHANGE_COMMAND:
		{
			if (ucJob == NPC_JOB_MIX_MASTER2)
			{
				wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_SHOP");
				surface = Logic_GetNPCClassIconSurface(ucJob, FALSE);
			}
			else
			{
				surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("GameCommon.srf", "srfItemSwapIcon");
				wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_ITEMSWAP");
			}
		}
		break;

		case EVENT_REWARD_COMMAND:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfEventReward");
			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_EVENTREWARD");
		break;

		case GUILD_DOJO_ESTABLISHMENT:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfGambleIcon");
			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_DOJO_ESTABLISHMENT");
		break;
	
		case GUILD_DOJO_INFO:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfGambleIcon");
			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_DOJO_INFO");
		break;
	
		case GUILD_DOJO_POINTUSE:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfGambleIcon");
			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDMENU_POINT_USE");
		break;

		case GUILD_DOGI:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfGambleIcon");
			wstrText = GetDisplayStringManager()->GetString("DST_DOGI_SETUP");
		break;
	
		case PORTAL_COMMAND:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfKeeperIcon" );        
			wstrText = GetDisplayStringManager()->GetString( "DST_MULTIDIALOG_MENU_PORTAL" );
		break;

		case GAMBLE_BUY_COMMNAD:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfGambleIcon" );
			wstrText = GetDisplayStringManager()->GetString( "DST_GAMBLE_COMMAND" );
		break;

		case CCBD_COMMAND:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfKeeperIcon");
			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_MENU_CCBD");
		break;

		case HOIPOIMIX_CREATE_COMMAND:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfHoipoiCreateIcon" );
			wstrText = GetDisplayStringManager()->GetString( "DST_HOIPOIMIX_CREATE" );
		break;

		case DWC_COMMAND:
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfHoipoiMixJobReset");
			wstrText = GetDisplayStringManager()->GetString("DST_DWC_FIELD_ENTER_COMMAND");
		break;


		case SMITH_CHANGE_BATTLE_ATTRIBUTE_COMMAND:
		{
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfEventItemChangeBattleAttributeicon");
			wstrText = GetDisplayStringManager()->GetString("DST_ITEMPROPERTY_TRANSFER");
		}
		break;

		case SMITH_TRANSFER_ITEM_GRADE:
		{
			surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfEventItemGradeTransfericon");
			wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_UPDATE_ITEMGRADETRANSFER");
		}
		break;

		default:
			return;
	}

	m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, nID, surface, wstrText, CCommandNode::MULTIDIALOG_NODE ) );
}

VOID CCommandList::RegistQuestCommand( RwUInt32 nTextID, RwUInt32 nTCID )
{
	gui::CSurface	surface;

	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfQuestIcon" );
	m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, nTCID, surface, nTextID ) );
}

VOID CCommandList::RegistServerSelectCommand( RwInt32 nID, WCHAR* pText, RwUInt16 wCurrentUserCount )
{
	m_pRootNode->AddChildNode( NTL_NEW CServerSelectNode( this, nID, pText, wCurrentUserCount ) );
}

VOID CCommandList::RegistExitCommand( RwInt32 eNodeType )
{
	gui::CSurface surface = surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfExitIcon" );
	m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, EXIT_COMMAND, surface, GetDisplayStringManager()->GetString( "DST_MULTIDIALOG_MENU_EXIT" ), (CCommandNode::NODE_TYPE)eNodeType ) );
}

VOID CCommandList::SetID( RwUInt32 uiId )
{
	m_uiId = uiId;
}

VOID CCommandList::SetInputType( RwUInt8 byInputType )
{
	m_byInputType = byInputType;
}

VOID CCommandList::SetTSKey( sTS_KEY& TSKey )
{
	m_TSKey = TSKey;
}

VOID CCommandList::SetTCUnit( CNtlTCUnit* pTCUnit )
{
	m_pTCUnit = pTCUnit;
}

RwUInt32 CCommandList::GetID(VOID)
{
	return m_uiId;
}

RwUInt8 CCommandList::GetInputType(VOID)
{
	return m_byInputType;
}

sTS_KEY& CCommandList::GetTSKey(VOID)
{
	return m_TSKey;
}

CNtlTCUnit* CCommandList::GetTCUnit(VOID)
{
	return m_pTCUnit;
}

VOID CCommandList::SelfClearNodes(VOID)
{
	ClearProc = &CCommandList::ClearState_Just;
}

VOID CCommandList::SendQuest(VOID)
{
	//ClearProc = &CCommandList::ClearState_ClearAndMakeQuest;

	// use this instead of ClearState_ClearAndMakeQuest so quests are shown properly
	if (NULL == GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency())
		return;

	SGET_QUEST_INFO* pGetQuestList = GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GetQuestInfoList((eEVENT_GEN_TYPE)m_byInputType, m_uiId);
	vecdef_QUEST_INFO_LIST* vQuestList = &pGetQuestList->QuestInfoList;
	vecdef_QUEST_INFO_LIST::iterator it;

	for (it = vQuestList->begin(); it != vQuestList->end(); ++it)
	{
		sQUEST_INFO sQuestInfo = (*it);
		gui::CSurface surface;

		switch (sQuestInfo.eProgType)
		{
			case eQUEST_PROGRESS_TYPE_NEW: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfNewQuest"); break;
			case eQUEST_PROGRESS_TYPE_PROGRESS: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfProgressQuest"); break;
			case eQUEST_PROGRESS_TYPE_REWARD: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface("MultiDialog.srf", "srfRewardQuest"); break;
		}

		m_pRootNode->AddChildNode(NTL_NEW CCommandNode(this, sQuestInfo.sKey.tID, surface, sQuestInfo));
	}
}

void CCommandList::TriggerObjectSendQuest()
{
	ClearProc = &CCommandList::ClearState_ClearAndMakeQuest;
}

VOID CCommandList::SendGuild(VOID)
{
	ClearProc = &CCommandList::ClearState_ClearAndMakeGuildMenu;
}

VOID CCommandList::SendGuildPoint(VOID)
{
	ClearProc = &CCommandList::ClearState_ClearAndMakeGuildPointMenu;
}

VOID CCommandList::SendGuildContribution(VOID)
{
	ClearProc = &CCommandList::ClearState_ClearAndMakeGuildContributionMenu;
}

VOID CCommandList::SendBuyBankMenu(VOID)
{
	ClearProc = &CCommandList::ClearState_ClearAndMakeBuyBankMenu;
}

VOID CCommandList::SendUserSelect( RwInt32 nSelectID, bool bResult )
{
	SNtlEventQuestUserSelectDialog_Res stResult;
	stResult.bResult	= bResult;
	stResult.sTSKey		= GetTSKey();
	stResult.tcSelID	= (NTL_TS_TC_ID)nSelectID;

	SNtlEventQuestDirect_Echo stEcho;
	stEcho.sUserSelect	= stResult;
	stEcho.eTDType		= ETD_QuestUserSelect;
	stEcho.pTCUnit		= GetTCUnit();

	if( stEcho.pTCUnit )
		CNtlSLEventGenerator::QuestTriggerDirectEcho( &stEcho );

	SetTCUnit( NULL );
}

VOID CCommandList::ClearState_No(VOID)
{

}

VOID CCommandList::ClearState_Just(VOID)
{
	CGuiLineTree::ClearNodes();

	ClearProc = &CCommandList::ClearState_No;
}

VOID CCommandList::ClearState_ClearAndMakeQuest(VOID)
{
	CGuiLineTree::ClearNodes();
	m_pOwnerGui->SetDialogType( CMultiDialogGui::TYPE_QUESTCAT );

	if ( NULL == GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency() )
		return;

	SGET_QUEST_INFO* pGetQuestList = GetNtlSLGlobal()->GetSobAvatar()->GetQuestAgency()->GetQuestInfoList( (eEVENT_GEN_TYPE)m_byInputType, m_uiId );
	vecdef_QUEST_INFO_LIST* vQuestList = &pGetQuestList->QuestInfoList;
	vecdef_QUEST_INFO_LIST::iterator it;

	for( it = vQuestList->begin() ; it != vQuestList->end() ; ++it )
	{
		sQUEST_INFO sQuestInfo = (*it);
		gui::CSurface surface;

		switch( sQuestInfo.eProgType )
		{
			case eQUEST_PROGRESS_TYPE_NEW: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfNewQuest" ); break;
			case eQUEST_PROGRESS_TYPE_PROGRESS: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfProgressQuest" ); break;
			case eQUEST_PROGRESS_TYPE_REWARD: surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfRewardQuest" ); break;
		}

		m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, sQuestInfo.sKey.tID, surface, sQuestInfo));
	}

	gui::CSurface	surface;

	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfExitIcon" );
	m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, EXIT_COMMAND, surface, GetDisplayStringManager()->GetString( "DST_MULTIDIALOG_MENU_EXIT" ), CCommandNode::QUESTCATALOG_NODE ) );	

	ClearProc = &CCommandList::ClearState_No;
}

VOID CCommandList::ClearState_ClearAndMakeGuildMenu(VOID)
{
	CGuiLineTree::ClearNodes();

	gui::CSurface	surface;
	std::wstring	wstrText;

	BOOL bGuildLeader = Logic_I_am_GuildLeader();

	CNtlGuild* pGuild = GetNtlSLGlobal()->GetSobAvatar()->GetGuild();

	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfGuildIcon" );

	if( pGuild->IsHaveGroup() )
	{
		if(bGuildLeader)
		{
			if (pGuild->IsBeingDisband())
			{
				wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDMENU_CANCEL_DISSOLVE");
				m_pRootNode->AddChildNode(NTL_NEW CCommandNode(this, GUILD_CANCEL_DISSOLVE, surface, wstrText, CCommandNode::GUILD_MODE));
			}
			else
			{
				wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDMENU_DISSOLVE");
				m_pRootNode->AddChildNode(NTL_NEW CCommandNode(this, GUILD_DISSOLVE, surface, wstrText, CCommandNode::GUILD_MODE));
			}

			wstrText = GetDisplayStringManager()->GetString( "DST_MULTIDIALOG_GUILDMENU_POINT_USE" );
			m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, GUILD_POINTUSE, surface, wstrText, CCommandNode::GUILD_MODE ) );

		}

		wstrText = GetDisplayStringManager()->GetString( "DST_MULTIDIALOG_GUILDMENU_CONTIBUTION" );
		m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, GUILD_CONTRIBUTION_ZENNY, surface, wstrText, CCommandNode::GUILD_MODE ) );

		if( pGuild->IsExistFunction(DBO_GUILD_FUNCTION_CAN_MAKE_DOGI) )
		{
			surface		= GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfGambleIcon" );
			wstrText	= GetDisplayStringManager()->GetString( "DST_DOGI_SETUP" );

			m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, GUILD_DOGI, surface, wstrText, CCommandNode::GUILD_MODE ) );
		}
	}
	else
	{
		wstrText = GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDMENU_MAKE");
		m_pRootNode->AddChildNode(NTL_NEW CCommandNode(this, GUILD_MAKE, surface, wstrText, CCommandNode::GUILD_MODE));
	}

	wstrText = GetDisplayStringManager()->GetString( "DST_MULTIDIALOG_MENU_EXIT" );
	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfExitIcon" );
	m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, EXIT_COMMAND, surface, wstrText, CCommandNode::GUILD_MODE ) );

	ClearProc = &CCommandList::ClearState_No;
}

VOID CCommandList::ClearState_ClearAndMakeGuildPointMenu(VOID)
{
	CGuiLineTree::ClearNodes();

	std::wstring	wstrText;
	gui::CSurface	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfGuildIcon" );
	CNtlGuild* pGuild = GetNtlSLGlobal()->GetSobAvatar()->GetGuild();
	SERIAL_HANDLE hTarget = Logic_GetAvatarTargetHandle();
	CNtlSobNpc* pSobNPC = reinterpret_cast<CNtlSobNpc*>( GetNtlSobManager()->GetSobObject(hTarget) );
	if( NULL == pSobNPC )
	{
		DBO_ASSERT(false, "CCommandList::ClearState_ClearAndMakeGuildPointMenu, Not exist sob npc of handle : " << hTarget);
		return;
	}
	CNtlSobNpcAttr* pSobNPCAttr = reinterpret_cast<CNtlSobNpcAttr*>( pSobNPC->GetSobAttr() );
	sNPC_TBLDAT* pNPC_TBLDAT = pSobNPCAttr->GetNpcTbl();

	if( pNPC_TBLDAT->byJob == NPC_JOB_GUILD_MANAGER )
	{
		for(RwUInt8 i = DBO_GUILD_FUNCTION_MAX_MEMBER_30; i < DBO_GUILD_FUNCTION_COUNT ; ++i)
		{
			if( pGuild->EnableUIGuildFunction((eDBO_GUILD_FUNCTION)i) )
			{
				wstrText = Logic_GetGuildFunctionString(i );
				m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, GUILDFUNCTION_MAX_MEMBER_30 + i, surface, wstrText, CCommandNode::GUILDPOINT_MODE ) );
			}
		}
	}
	else if( pNPC_TBLDAT->byJob == NPC_JOB_DOJO_MANAGER )
	{
		for(RwUInt8 i = DBO_GUILD_FUNCTION_MAX_MEMBER_30; i < DBO_GUILD_FUNCTION_COUNT ; ++i)
		{
			if( pGuild->EnableUIDojoFunction((eDBO_GUILD_FUNCTION)i) )
			{
				wstrText = Logic_GetGuildFunctionString(i );
				m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, GUILDFUNCTION_MAX_MEMBER_30 + i, surface, wstrText, CCommandNode::GUILDPOINT_MODE ) );
			}
		}
	}	

	wstrText = GetDisplayStringManager()->GetString( "DST_MULTIDIALOG_MENU_EXIT" );
	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfExitIcon" );
	m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, EXIT_COMMAND, surface, wstrText, CCommandNode::GUILDPOINT_MODE ) );

	ClearProc = &CCommandList::ClearState_No;
}

VOID CCommandList::ClearState_ClearAndMakeGuildContributionMenu(VOID)
{
	CGuiLineTree::ClearNodes();

	std::wstring	wstrText;
	gui::CSurface	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfGuildIcon" );	
	WCHAR awcResult[128] = L"";
	
	swprintf_s(awcResult, 128, GetDisplayStringManager()->GetString( "DST_MULTIDIALOG_GUILDMENU_CONTIBUTION_TITLE" ), Logic_FormatZeni(dGUILD_CONTRIBUTION_ZENNY_50000));
	m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, GUILD_CONTRIBUTION_CONTRIBUTION_50000, surface, awcResult, CCommandNode::GUILDCONTRIBUTION_MODE ) );

	swprintf_s(awcResult, 128, GetDisplayStringManager()->GetString( "DST_MULTIDIALOG_GUILDMENU_CONTIBUTION_TITLE" ), Logic_FormatZeni(dGUILD_CONTRIBUTION_ZENNY_100000));
	m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, GUILD_CONTRIBUTION_CONTRIBUTION_100000, surface, awcResult, CCommandNode::GUILDCONTRIBUTION_MODE ) );

	swprintf_s(awcResult, 128, GetDisplayStringManager()->GetString( "DST_MULTIDIALOG_GUILDMENU_CONTIBUTION_TITLE" ), Logic_FormatZeni(dGUILD_CONTRIBUTION_ZENNY_500000));
	m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, GUILD_CONTRIBUTION_CONTRIBUTION_500000, surface, awcResult, CCommandNode::GUILDCONTRIBUTION_MODE ) );

	swprintf_s(awcResult, 128, GetDisplayStringManager()->GetString( "DST_MULTIDIALOG_GUILDMENU_CONTIBUTION_TITLE" ), Logic_FormatZeni(dGUILD_CONTRIBUTION_ZENNY_1000000));
	m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, GUILD_CONTRIBUTION_CONTRIBUTION_1000000, surface, awcResult, CCommandNode::GUILDCONTRIBUTION_MODE ) );

	swprintf_s(awcResult, 128, GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDMENU_CONTIBUTION_TITLE"), Logic_FormatZeni(dGUILD_CONTRIBUTION_ZENNY_10000000));
	m_pRootNode->AddChildNode(NTL_NEW CCommandNode(this, GUILD_CONTRIBUTION_CONTRIBUTION_10000000, surface, awcResult, CCommandNode::GUILDCONTRIBUTION_MODE));


	swprintf_s(awcResult, 128, GetDisplayStringManager()->GetString( "DST_MULTIDIALOG_MENU_EXIT" ));
	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfExitIcon" );
	m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, EXIT_COMMAND, surface, awcResult, CCommandNode::GUILDCONTRIBUTION_MODE ) );

	ClearProc = &CCommandList::ClearState_No;
}

VOID CCommandList::ClearState_ClearAndMakeBuyBankMenu(VOID)
{
	CGuiLineTree::ClearNodes();

	WCHAR wbuf[256] = { 0, };

	gui::CSurface	surface;
	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfWareHouseIcon" );

	CNtlWarehouse* pWarehouse = GetNtlSLGlobal()->GetSobAvatar()->GetWarehouse();

	// 게임에 처음 들어가서 창고NPC를 클릭하면 서버로부터 창고 데이터를 받는다.
	// 그 데이터를 전부 받기 전에 이 코드로 들어온다면 부정확한 정보로 하위코드를 실행하지 않도록 하자
	if( pWarehouse->IsRecieveData_from_Server() == FALSE )
	{
		GetAlarmManager()->AlarmMessage("DST_PLEASE_RETRY");
		GetDialogManager()->CloseDialog(DIALOG_MULTIDIALOG);
		Logic_CancelNpcFacing();
		return;
	}

	// avooo's command : According to the intention of planning, one warehouse has basically.
	//					Thus, the warehouse index that can be purchased starts at 1
	for( RwUInt8 i = 1 ; i < NTL_MAX_BANKSLOT_COUNT-1 ; ++i )
	{
		SERIAL_HANDLE hWarehouseSlotHandle = pWarehouse->GetSlotSerailID( i );

		if( hWarehouseSlotHandle == INVALID_SERIAL_ID )
		{
			sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob( m_pOwnerGui->GetTargetSerial() );

			sMERCHANT_TBLDAT* pMERCHANT_TBLDAT = Logic_GetMerchantDataFromTable( pNPC_TBLDAT->amerchant_Tblidx[0] );
			DBO_ASSERT( pNPC_TBLDAT->amerchant_Tblidx[0] != INVALID_SERIAL_ID, "Not exist merchant index in NPC table");

			sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromTable( pMERCHANT_TBLDAT->aitem_Tblidx[i] );
			DBO_ASSERT( pMERCHANT_TBLDAT->aitem_Tblidx[i] != INVALID_SERIAL_ID, "Not exist item index in Merchant table");

			if( i < NTL_MAX_BANKSLOT_COUNT - 1 )
			{
				swprintf_s( wbuf, 256, GetDisplayStringManager()->GetString( "DST_MULTIDIALOG_BUYBANKMENU_BUYBANK" ), i + 1, Logic_FormatZeni(pITEM_TBLDAT->dwCost) );
				m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, i, surface, wbuf, CCommandNode::BUYBANK_MODE ) );
			}			
		}
	}

	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfExitIcon" );
	m_pRootNode->AddChildNode( NTL_NEW CCommandNode( this, EXIT_COMMAND, surface, GetDisplayStringManager()->GetString( "DST_MULTIDIALOG_MENU_EXIT" ), CCommandNode::BUYBANK_MODE ) );

	ClearProc = &CCommandList::ClearState_No;
}

VOID CCommandList::OnClear(VOID)
{
	(this->*ClearProc)();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CCommandNode

CCommandNode::CCommandNode( CGuiLineTree* pTree, RwInt32 nID, gui::CSurface surface, std::string strText, NODE_TYPE eType )
: CGuiLineTreeNode( pTree, nID ), m_pbtnCommand( NULL ), m_ppnlIcon( NULL ), m_eNodeType( eType )
{
	// gui component설정 및 surface 설정.
	CRectangle rect;
	rect.SetRectWH( COMMANDNODE_X ,COMMANDNODE_Y, COMMANDNODE_WIDTH, COMMANDNODE_HEIGHT );
	m_pbtnCommand = NTL_NEW gui::CButton( rect, strText, pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pbtnCommand->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfBtnDown" ) );
	m_pbtnCommand->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfBtnFoc" ) );
	m_pbtnCommand->SetTextDownColor( RGB( 113, 113, 113 ) );
	m_pbtnCommand->SetTextFocusColor( RGB( 255, 255, 255 ) );
	m_pbtnCommand->SetTextUpColor( RGB( 220, 220, 220 ) );
	m_pbtnCommand->SetTextDownCoordDiff( COMMANDNODE_DOWNCOORD_DIFFX, COMMANDNODE_DOWNCOORD_DIFFY );
	m_pbtnCommand->SetTextCoord( COMMANDTEXT_X, 0 );
	m_pbtnCommand->SetTextStyle( COMP_TEXT_LEFT );
	m_pbtnCommand->ApplyText();

	rect.SetCenterRect( COMMANDNODEICON_X, COMMANDNODEICON_Y, surface.m_SnapShot.rtRect.GetWidth(), surface.m_SnapShot.rtRect.GetHeight() );
	m_posIcon.SetPos( rect.left, rect.top );
	m_ppnlIcon = NTL_NEW gui::CPanel( rect, pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager(), surface );
	m_ppnlIcon->Enable( false );

	m_slotClickCommand = m_pbtnCommand->SigClicked().Connect( this, &CCommandNode::OnClickCommand );
	m_slotPressedCommand = m_pbtnCommand->SigPressed().Connect( this, &CCommandNode::OnPressedCommand );
	m_slotReleasedCommand = m_pbtnCommand->SigReleased().Connect( this, &CCommandNode::OnReleasedCommand );
}

CCommandNode::CCommandNode( CGuiLineTree* pTree, RwInt32 nID, gui::CSurface surface, std::wstring wstrText, NODE_TYPE eType )
: CGuiLineTreeNode( pTree, nID ), m_pbtnCommand( NULL ), m_ppnlIcon( NULL ), m_eNodeType( eType )
{
	// gui component설정 및 surface 설정.
	CRectangle rect;
	rect.SetRectWH( COMMANDNODE_X ,COMMANDNODE_Y, COMMANDNODE_WIDTH, COMMANDNODE_HEIGHT );
	m_pbtnCommand = NTL_NEW gui::CButton( rect, wstrText, pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pbtnCommand->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfBtnDown" ) );
	m_pbtnCommand->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfBtnFoc" ) );
	m_pbtnCommand->SetTextDownColor( RGB( 113, 113, 113 ) );
	m_pbtnCommand->SetTextFocusColor( RGB( 255, 255, 255 ) );
	m_pbtnCommand->SetTextUpColor( RGB( 220, 220, 220 ) );
	m_pbtnCommand->SetTextDownCoordDiff( COMMANDNODE_DOWNCOORD_DIFFX, COMMANDNODE_DOWNCOORD_DIFFY );
	m_pbtnCommand->SetTextCoord( COMMANDTEXT_X, 0 );
	m_pbtnCommand->SetTextStyle( COMP_TEXT_LEFT );
	m_pbtnCommand->ApplyText();

	rect.SetCenterRect( COMMANDNODEICON_X, COMMANDNODEICON_Y, surface.m_SnapShot.rtRect.GetWidth(), surface.m_SnapShot.rtRect.GetHeight() );
	m_posIcon.SetPos( rect.left, rect.top );
	m_ppnlIcon = NTL_NEW gui::CPanel( rect, pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager(), surface );
	m_ppnlIcon->Enable( false );

	m_slotClickCommand = m_pbtnCommand->SigClicked().Connect( this, &CCommandNode::OnClickCommand );
	m_slotPressedCommand = m_pbtnCommand->SigPressed().Connect( this, &CCommandNode::OnPressedCommand );
	m_slotReleasedCommand = m_pbtnCommand->SigReleased().Connect( this, &CCommandNode::OnReleasedCommand );
}

CCommandNode::CCommandNode( CGuiLineTree* pTree, RwInt32 nID, gui::CSurface surface, RwInt32 nTextID )
: CGuiLineTreeNode( pTree, nID ), m_pbtnCommand( NULL ), m_ppnlIcon( NULL )
{
	// gui component설정 및 surface 설정.
	CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
	sQUEST_TEXT_DATA_TBLDAT* pQuestText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( nTextID ) );
	std::wstring wstrText;

	if( pQuestText )
		wstrText = gui::GetHtmlFromMemoryString( pQuestText->wstrText.c_str(), pQuestText->wstrText.size() );
	else
	{
		WCHAR buf[64];
		swprintf_s( buf, 64, L"QuestTextTable %d is not present!", nTextID );
		wstrText = buf;
	}

	CRectangle rect;
	rect.SetRectWH( COMMANDNODE_X ,COMMANDNODE_Y, COMMANDNODE_WIDTH, COMMANDNODE_HEIGHT );
	m_pbtnCommand = NTL_NEW gui::CButton( rect, wstrText, pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pbtnCommand->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfBtnDown" ) );
	m_pbtnCommand->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfBtnFoc" ) );
	m_pbtnCommand->SetTextDownColor( RGB( 113, 113, 113 ) );
	m_pbtnCommand->SetTextFocusColor( RGB( 255, 255, 255 ) );
	m_pbtnCommand->SetTextUpColor( RGB( 220, 220, 220 ) );
	m_pbtnCommand->SetTextDownCoordDiff( COMMANDNODE_DOWNCOORD_DIFFX, COMMANDNODE_DOWNCOORD_DIFFY );
	m_pbtnCommand->SetTextCoord( COMMANDTEXT_X, 0 );
	m_pbtnCommand->SetTextStyle( COMP_TEXT_LEFT );
	m_pbtnCommand->ApplyText();
	rect.SetRectWH( BRANCHCOMMANDNODEHTML_X ,BRANCHCOMMANDNODEHTML_Y, BRANCHCOMMANDNODEHTML_WIDTH, BRANCHCOMMANDNODEHTML_HEIGHT );

	rect.SetCenterRect( COMMANDNODEICON_X, COMMANDNODEICON_Y, surface.m_SnapShot.rtRect.GetWidth(), surface.m_SnapShot.rtRect.GetHeight() );
	m_posIcon.SetPos( rect.left, rect.top );
	m_ppnlIcon = NTL_NEW gui::CPanel( rect, pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager(), surface );
	m_ppnlIcon->Enable( false );

	m_slotClickCommand = m_pbtnCommand->SigClicked().Connect( this, &CCommandNode::OnClickCommand );
	m_slotPressedCommand = m_pbtnCommand->SigPressed().Connect( this, &CCommandNode::OnPressedCommand );
	m_slotReleasedCommand = m_pbtnCommand->SigReleased().Connect( this, &CCommandNode::OnReleasedCommand );

	// Type설정.
	m_eNodeType = BRANCHDIALOG_NODE;
}

CCommandNode::CCommandNode( CGuiLineTree* pTree, RwInt32 nID, gui::CSurface surface, sQUEST_INFO sData )
: CGuiLineTreeNode( pTree, nID ), m_pbtnCommand( NULL ), m_ppnlIcon( NULL )
{
	// gui component설정 및 surface 설정.
	CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
	sQUEST_TEXT_DATA_TBLDAT* pQuestText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( sData.dwQuestTitle ) );
	std::wstring wstrText;

	if( pQuestText )
		wstrText = gui::GetHtmlFromMemoryString( pQuestText->wstrText.c_str(), pQuestText->wstrText.size() );
	else
	{
		WCHAR buf[64];
		swprintf_s( buf, 64, L"QuestTextTable %u is not present", sData.dwQuestTitle );
		wstrText = buf;
	}

	CRectangle rect;
	rect.SetRectWH( COMMANDNODE_X ,COMMANDNODE_Y, COMMANDNODE_WIDTH, COMMANDNODE_HEIGHT );
	m_pbtnCommand = NTL_NEW gui::CButton( rect, wstrText, pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pbtnCommand->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfBtnDown" ) );
	m_pbtnCommand->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfBtnFoc" ) );
	m_pbtnCommand->SetTextDownColor( RGB( 113, 113, 113 ) );
	m_pbtnCommand->SetTextFocusColor( RGB( 255, 255, 255 ) );
	m_pbtnCommand->SetTextUpColor( RGB( 220, 220, 220 ) );
	m_pbtnCommand->SetTextDownCoordDiff( COMMANDNODE_DOWNCOORD_DIFFX, COMMANDNODE_DOWNCOORD_DIFFY );
	m_pbtnCommand->SetTextCoord( COMMANDTEXT_X, 0 );
	m_pbtnCommand->SetTextStyle( COMP_TEXT_LEFT );
	m_pbtnCommand->ApplyText();
	rect.SetRectWH( BRANCHCOMMANDNODEHTML_X ,BRANCHCOMMANDNODEHTML_Y, BRANCHCOMMANDNODEHTML_WIDTH, BRANCHCOMMANDNODEHTML_HEIGHT );

	rect.SetCenterRect( COMMANDNODEICON_X, COMMANDNODEICON_Y, surface.m_SnapShot.rtRect.GetWidth(), surface.m_SnapShot.rtRect.GetHeight() );
	m_posIcon.SetPos( rect.left, rect.top );
	m_ppnlIcon = NTL_NEW gui::CPanel( rect, pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager(), surface );
	m_ppnlIcon->Enable( false );

	m_slotClickCommand = m_pbtnCommand->SigClicked().Connect( this, &CCommandNode::OnClickCommand );
	m_slotPressedCommand = m_pbtnCommand->SigPressed().Connect( this, &CCommandNode::OnPressedCommand );
	m_slotReleasedCommand = m_pbtnCommand->SigReleased().Connect( this, &CCommandNode::OnReleasedCommand );

	// Type설정.
	m_eNodeType = QUESTCATALOG_NODE;
	m_sData		= sData;
}


CCommandNode::~CCommandNode(VOID)
{
	NTL_DELETE( m_pbtnCommand );
	NTL_DELETE( m_ppnlIcon );
}

VOID CCommandNode::ShowProc(VOID)
{
	CGuiLineTreeNode::ShowProc();

	m_pbtnCommand->Show( true );
	m_pbtnCommand->SetPosition( m_nPosX + COMMANDNODE_X, m_nPosY + COMMANDNODE_Y );

	if( m_ppnlIcon )
	{
		m_ppnlIcon->Show( true );
		m_ppnlIcon->SetPosition( m_nPosX + m_posIcon.x, m_nPosY + m_posIcon.y );
	}	
}

VOID CCommandNode::HideProc(VOID)
{
	CGuiLineTreeNode::HideProc();

	m_pbtnCommand->Show( false );

	if( m_ppnlIcon )
		m_ppnlIcon->Show( false );
}

VOID CCommandNode::OnClickCommand( gui::CComponent* pComponent )
{	
	CCommandList* pNodeMgr = reinterpret_cast<CCommandList*>( m_pMgr );

	if( m_eNodeType == MULTIDIALOG_NODE )
	{
		if( m_nID == CCommandList::QUEST_COMMAND )
		{
			pNodeMgr->SendQuest();
		}
		else if( m_nID == CCommandList::GUILD_COMMAND )
		{
			pNodeMgr->SendGuild();
		}
		else if( m_nID == CCommandList::BUY_BANK_COMMAND )
		{
			pNodeMgr->SendBuyBankMenu();
		}
		else
		{
			SERIAL_HANDLE hSerialID = pNodeMgr->GetOwnerGui()->GetTargetSerial();			
			DBO_ASSERT(hSerialID != INVALID_SERIAL_ID, "CCommandNode::OnClickCommand, Invalid NPC serial ID");

			switch( m_nID )
			{
				case CCommandList::MERCHANT_COMMAND:
				{
					GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG, FALSE );
					CDboEventGenerator::NPCDialogOpen( hSerialID, DIALOG_NPCSHOP );
					break;
				}
				case CCommandList::BUDOHSI_MERCHANT_COMMAND:
				{
					GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG, FALSE );
					CDboEventGenerator::NPCDialogOpen( hSerialID, DIALOG_NPCSHOP );
					break;
				}
				case CCommandList::SKILLTRAINER_COMMAND:
				{
					CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
					if (pAvatar)
					{
						CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>(pAvatar->GetSobAttr());

						sMsgBoxData msgExData;
						msgExData.hHandle = hSerialID;

						GetAlarmManager()->FormattedAlarmMessage("DST_NPC_SKILL_POINT_RESET_MSG", FALSE, &msgExData,
								pAvatarAttr->GetLevel() - 1 - pAvatarAttr->m_uiSp, Logic_FormatZeni(Dbo_GetRequiredZennyForNpcSkillInit(pAvatarAttr->GetLevel())));
					}
				}		
				break;
				case CCommandList::BANKER_COMMAND:
				{
					GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG, FALSE );
					CDboEventGenerator::NPCDialogOpen( hSerialID, DIALOG_WAREHOUSEBAR );
					break;
				}
			case CCommandList::GUILD_BANK_COMMAND:
				{
					GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG, FALSE );
					GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseStart_Req(hSerialID);
					break;
				}
				case CCommandList::UPGRADE_COMMAND:
				{
					GetDialogManager()->CloseDialog(DIALOG_MULTIDIALOG, FALSE);
					CDboEventGenerator::NPCDialogOpen(hSerialID, DIALOG_ITEMUPGRADE);
				}
				break;
				case CCommandList::TMQ_COMMAND:
				{
					GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG, FALSE );
					CDboEventGenerator::NPCDialogOpen( hSerialID, DIALOG_TMQBOARD );
				}
				break;
				case CCommandList::UD_COMMAND:
				{
					GetDialogManager()->CloseDialog(DIALOG_MULTIDIALOG, FALSE);
					CDboEventGenerator::NPCDialogOpen(hSerialID, DIALOG_ULTIMATEDUNGEON);
				}
				break;
			case CCommandList::PORTAL_COMMAND:
				{
					GetDialogManager()->CloseDialog(DIALOG_MULTIDIALOG, FALSE);
					CDboEventGenerator::NPCDialogOpen(hSerialID, DIALOG_PORTAL);
					break;
				}
			case CCommandList::GAMBLE_BUY_COMMNAD:
				{
					if( !Logic_FindEmptyItemSlot() )
					{
						GetAlarmManager()->AlarmMessage( "DST_ITEM_BAG_FULL" );
						return;
					}

					CNtlSobNpc* pNPC = reinterpret_cast<CNtlSobNpc*>( GetNtlSobManager()->GetSobObject( pNodeMgr->GetOwnerGui()->GetTargetSerial() ) );
					DBO_ASSERT( pNPC, "CMultiDialogGui::OnClickCommand : Invalid NPC SerialID" );

					CNtlSobNpcAttr* pNPCAttr = reinterpret_cast<CNtlSobNpcAttr*>( pNPC->GetSobAttr() );
					sNPC_TBLDAT* pTableData = pNPCAttr->GetNpcTbl();

					// Merchant Table의 가장 첫번째 Index를 참조한다.
					if( pTableData->amerchant_Tblidx[0] == INVALID_TBLIDX )
					{
						DBO_FAIL( "[GAMBLE] NPC Table havn't merchant table index." << pTableData->tblidx );
						return;
					}

					sMERCHANT_TBLDAT* pMerchantTblDat = (sMERCHANT_TBLDAT*)API_GetTableContainer()->GetMerchantTable()->FindData( pTableData->amerchant_Tblidx[0] );
					if( !pMerchantTblDat )
					{
						DBO_FAIL( "[GAMBLE] Merchant table index is invalid." << pTableData->amerchant_Tblidx[0] );
						return;
					}

					if (pMerchantTblDat->bySell_Type == MERCHANT_SELL_TYPE_GAMBLE)
					{
						if (Logic_GetMudosaPoint() < pMerchantTblDat->dwNeedMileage)
						{
							GetAlarmManager()->AlarmMessage("DST_GAMBLE_NOT_ENOUGH_MUDOSA_POINT");
							return;
						}
						else if (Logic_GetZenny() < ZENI_GAMBLE_FEE)
						{
							GetAlarmManager()->AlarmMessage("DST_LACK_OF_ZENNY");
							return;
						}
					}
					else if (pMerchantTblDat->bySell_Type == MERCHANT_SELL_TYPE_GAMBLE_ZENNY)
					{
						if (Logic_GetZenny() < pMerchantTblDat->dwNeedMileage)
						{
							GetAlarmManager()->AlarmMessage("DST_LACK_OF_ZENNY");
							return;
						}
					}

					CNtlSLEventGenerator::GambleAniStart( pNodeMgr->GetOwnerGui()->GetTargetSerial() );
					GetDialogManager()->CloseDialog(DIALOG_MULTIDIALOG, FALSE );


					break;
				}
				case CCommandList::CCBD_COMMAND:
				{
					GetDialogManager()->CloseDialog(DIALOG_MULTIDIALOG, FALSE);
					CDboEventGenerator::NPCDialogOpen(hSerialID, DIALOG_CCBD_BOARD);
				}
				break;
			case CCommandList::HOIPOIMIX_CREATE_COMMAND:
				{
					GetDialogManager()->CloseDialog(DIALOG_MULTIDIALOG, FALSE);
					CDboEventGenerator::NPCDialogOpen(hSerialID, DIALOG_HOIPOIMIX_RECIPE);
				}
				break;
			case CCommandList::GUILD_DOJO_ESTABLISHMENT:
				{
					GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG, FALSE );				

					GetAlarmManager()->FormattedAlarmMessage("DST_DOJO_ASK_ESTABLISHMENT", FALSE, NULL, g_aGuildFunctionInfo[DBO_GUILD_FUNCTION_CAN_HAVE_DOJO].dwRequiredZenny );
					break;
				}
			case CCommandList::GUILD_DOJO_INFO:
				{
					CDboEventGenerator::DialogEvent(DIALOGEVENT_CREATE_DOJO_GUI);

					CDojoInfoGui* pDojoInfoGui = reinterpret_cast<CDojoInfoGui*>( GetDialogManager()->GetDialog( DIALOG_DOJO_INFO ) );
					DBO_ASSERT(pDojoInfoGui, "CCommandNode::OnClickCommand, Not exist CDojoInfoGui instance");
					pDojoInfoGui->SetPageType( CDojoInfoGui::TAB_PAGE_INFO );

					GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG, FALSE );
					CDboEventGenerator::NPCDialogOpen( hSerialID, DIALOG_DOJO_INFO );
					break;
				}
			case CCommandList::GUILD_DOJO_POINTUSE:
				{					
					pNodeMgr->SendGuildPoint();	
					break;
				}
				case CCommandList::GUILD_DOGI:
				{
					CDboEventGenerator::DialogEvent(DIALOGEVENT_CREATE_DOJO_GUI);

					GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG, FALSE );
					CDboEventGenerator::NPCDialogOpen( hSerialID, DIALOG_DOGI );
					break;
				}
				case CCommandList::EXIT_COMMAND:
				{
					GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );
					Logic_CancelNpcFacing();
					break;
				}
				case CCommandList::EVENT_ITEM_EXCHANGE_COMMAND:
				{
					GetDialogManager()->CloseDialog(DIALOG_MULTIDIALOG, FALSE);
					CDboEventGenerator::NPCDialogOpen(hSerialID, DIALOG_ITEM_EXCHANGE_SHOP);
					break;
				}
				case CCommandList::SMITH_CHANGE_BATTLE_ATTRIBUTE_COMMAND:
				{
					GetDialogManager()->CloseDialog(DIALOG_MULTIDIALOG, FALSE);
					CDboEventGenerator::NPCDialogOpen(hSerialID, DIALOG_ITEM_CHANGE_BATTLE_ATTRIBUTE);
				}
				break;
				default:
					{
						DBO_FAIL("Wrong command Type : " << m_eNodeType << "ID : " << m_nID);
						break;
					}
				}	
		}
	}
	else if( m_eNodeType == QUESTCATALOG_NODE )
	{	
		if( m_nID == CCommandList::EXIT_COMMAND )
		{
			GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );

			Logic_CancelNpcFacing();
		}
		else
		{
			if( m_sData.bVisitOP )
			{
				// visit quest event
				CDboEventGenerator::QuestMessage( &m_sData, (eEVENT_GEN_TYPE)pNodeMgr->GetInputType(), pNodeMgr->GetID() );
			}
			else
			{
				// normal quest event
				CNtlSLEventGenerator::DoQuest( m_sData, (eEVENT_GEN_TYPE)pNodeMgr->GetInputType(), pNodeMgr->GetID(), TRUE );
			}

			GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG, FALSE );
		}		
	}
	else if( m_eNodeType == QUESTDIALOG_NODE )
	{
		if( m_nID == CCommandList::EXIT_COMMAND )
		{
			GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );
		}
	}
	else if( m_eNodeType == BRANCHDIALOG_NODE )
	{
		if( m_nID == CCommandList::EXIT_COMMAND )
		{
			pNodeMgr->SendUserSelect( NTL_TS_TC_ID_INVALID, false );			
		}
		else
		{
			pNodeMgr->SendUserSelect( m_nID, true );			
		}		

		GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );
	}
	else if( m_eNodeType == GUILD_MODE )
	{
		SERIAL_HANDLE hSerialID = pNodeMgr->GetOwnerGui()->GetTargetSerial();			

		switch( m_nID )
		{
		case CCommandList::GUILD_MAKE:
			{
				GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG, FALSE );
				GetAlarmManager()->AlarmMessage( "DST_GUILD_DO_MAKE" );				
				break;
			}				
		case CCommandList::GUILD_DISSOLVE:
			{
				GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG, FALSE );
				GetAlarmManager()->AlarmMessage( "DST_GUILD_DO_DISSOLVE" );				
				break;
			}		
		case CCommandList::GUILD_CANCEL_DISSOLVE:
			{
				GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG, FALSE );
				GetAlarmManager()->AlarmMessage( "DST_GUILD_CANCEL_DISSOLVE" );
				break;
			}
		case CCommandList::GUILD_POINTUSE:
			{
				pNodeMgr->SendGuildPoint();		
				break;
			}
			/*case CCommandList::GUILD_CHANGE_NAME:
			{
				sMsgBoxData msgExData;
				msgExData.hHandle = hSerialID;

				GetAlarmManager()->AlarmMessage( "DST_GUILD_CHANGE_GUILD_NAME", FALSE, &msgExData );
				GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );				
				break;
			}*/
		case CCommandList::GUILD_CONTRIBUTION_ZENNY:
			{
				pNodeMgr->SendGuildContribution();	
				break;
			}
		case CCommandList::GUILD_DOGI:
			{
				CDboEventGenerator::DialogEvent(DIALOGEVENT_CREATE_DOJO_GUI);
				GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG, FALSE );
				CDboEventGenerator::NPCDialogOpen( hSerialID, DIALOG_DOGI );
				break;
			}
		case CCommandList::EXIT_COMMAND:
			{
				GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );
				Logic_CancelNpcFacing();
				break;
			}				
		}		
	}
	else if( m_eNodeType == GUILDPOINT_MODE )
	{
		switch( m_nID )
		{
		case CCommandList::EXIT_COMMAND:
			{
				GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );
				Logic_CancelNpcFacing();
				break;
			}		
		default:
			{				
			 	SERIAL_HANDLE hNPC = Logic_GetAvatarTargetHandle();
				sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob(hNPC);

				if( pNPC_TBLDAT )
				{
					sMsgBoxData msgExData;
					msgExData.byIndex = (RwUInt8)(m_nID - CCommandList::GUILDFUNCTION_START);

					if( pNPC_TBLDAT->dwFunc_Bit_Flag & NPC_FUNC_FLAG_GUILD_MANAGER )
					{
						eDBO_GUILD_FUNCTION eGuildFunction = static_cast<eDBO_GUILD_FUNCTION>(msgExData.byIndex);
						const sDBO_GUILD_FUNCTION_INFO* guildFuncInfo = Dbo_GetGuildFunctionInfo(eGuildFunction);

						GetAlarmManager()->FormattedAlarmMessage( "DST_GUILD_ADD_FUNCTION", FALSE, &msgExData, Logic_GetGuildFunctionString(msgExData.byIndex), guildFuncInfo->dwRequiredPoint, Logic_FormatZeni(guildFuncInfo->dwRequiredZenny));
					}
					else if( pNPC_TBLDAT->dwFunc_Bit_Flag & NPC_FUNC_FLAG_DOJO_MANAGER )
					{
						switch(msgExData.byIndex)
						{
						//	case DBO_GUILD_FUNCTION_DOJO_UPGRADE_1:
							case DBO_GUILD_FUNCTION_DOJO_UPGRADE_2:
							case DBO_GUILD_FUNCTION_DOJO_UPGRADE_3:
							case DBO_GUILD_FUNCTION_DOJO_UPGRADE_4:
							case DBO_GUILD_FUNCTION_DOJO_UPGRADE_5:
							case DBO_GUILD_FUNCTION_DOJO_UPGRADE_6:
							case DBO_GUILD_FUNCTION_DOJO_UPGRADE_7:
							{
								CDboEventGenerator::DialogEvent(DIALOGEVENT_CREATE_DOJO_GUI);
								GetDialogManager()->OpenDialog(DIALOG_DOJO_UPGRADE);
							}
							break;
							default:
							{
								eDBO_GUILD_FUNCTION eGuildFunction = static_cast<eDBO_GUILD_FUNCTION>(msgExData.byIndex);
								const sDBO_GUILD_FUNCTION_INFO* guildFuncInfo = Dbo_GetGuildFunctionInfo(eGuildFunction);

								GetAlarmManager()->FormattedAlarmMessage( "DST_DOJO_ADD_FUNCTION", FALSE, &msgExData, Logic_GetGuildFunctionString(msgExData.byIndex), guildFuncInfo->dwRequiredPoint, Logic_FormatZeni(guildFuncInfo->dwRequiredZenny));
							}
							break;
						}						
					}
					else
					{
						DBO_FAIL("Invalid NPC function");
					}
				}
				else
				{
					DBO_FAIL("Not exist NPC table of handle : " << hNPC);
				}

				GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );
				break;
			}
		}
	}
	else if( m_eNodeType == GUILDCONTRIBUTION_MODE )
	{
		sMsgBoxData msgExData;

		switch( m_nID )
		{
			case CCommandList::GUILD_CONTRIBUTION_CONTRIBUTION_50000:
			{
				msgExData.uiParam = dGUILD_CONTRIBUTION_ZENNY_50000;

				GetAlarmManager()->FormattedAlarmMessage( "DST_GUILD_CONTRIBUTION_ZENNY", FALSE, &msgExData, Logic_FormatZeni(dGUILD_CONTRIBUTION_ZENNY_50000) );
			}
			break;
			case CCommandList::GUILD_CONTRIBUTION_CONTRIBUTION_100000:
			{
				msgExData.uiParam = dGUILD_CONTRIBUTION_ZENNY_100000;

				GetAlarmManager()->FormattedAlarmMessage( "DST_GUILD_CONTRIBUTION_ZENNY", FALSE, &msgExData, Logic_FormatZeni(dGUILD_CONTRIBUTION_ZENNY_100000) );
			}
			break;
			case CCommandList::GUILD_CONTRIBUTION_CONTRIBUTION_500000:
			{
				msgExData.uiParam = dGUILD_CONTRIBUTION_ZENNY_500000;

				GetAlarmManager()->FormattedAlarmMessage( "DST_GUILD_CONTRIBUTION_ZENNY", FALSE, &msgExData, Logic_FormatZeni(dGUILD_CONTRIBUTION_ZENNY_500000) );
			}
			break;
			case CCommandList::GUILD_CONTRIBUTION_CONTRIBUTION_1000000:
			{
				msgExData.uiParam = dGUILD_CONTRIBUTION_ZENNY_1000000;

				GetAlarmManager()->FormattedAlarmMessage( "DST_GUILD_CONTRIBUTION_ZENNY", FALSE, &msgExData, Logic_FormatZeni(dGUILD_CONTRIBUTION_ZENNY_1000000) );
			}
			break;
			case CCommandList::GUILD_CONTRIBUTION_CONTRIBUTION_10000000:
			{
				msgExData.uiParam = dGUILD_CONTRIBUTION_ZENNY_10000000;

				GetAlarmManager()->FormattedAlarmMessage("DST_GUILD_CONTRIBUTION_ZENNY", FALSE, &msgExData, Logic_FormatZeni(dGUILD_CONTRIBUTION_ZENNY_10000000));
			}
			break;
			case CCommandList::EXIT_COMMAND:
			{
				Logic_CancelNpcFacing();
			}
			break;
		}

		GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );
	}
	else if( m_eNodeType == BUYBANK_MODE )
	{
		switch( m_nID )
		{
		case CCommandList::EXIT_COMMAND:
			{
				Logic_CancelNpcFacing();
				break;
			}
		case CCommandList::BUY_PUBLIC_BAG:
			{
				sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob( pNodeMgr->GetOwnerGui()->GetTargetSerial() );

				sMERCHANT_TBLDAT* pMERCHANT_TBLDAT = Logic_GetMerchantDataFromTable( pNPC_TBLDAT->amerchant_Tblidx[0] );
				DBO_ASSERT( pMERCHANT_TBLDAT, "Not exist merchant index in NPC table");

				// avooo's comment : 아이템 테이블 상의 공유 창고는 3을 가리킨다
				sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromTable( pMERCHANT_TBLDAT->aitem_Tblidx[3] );
				DBO_ASSERT( pITEM_TBLDAT, "Not exist item index in Merchant table");

				SERIAL_HANDLE hNPCHandle = pNodeMgr->GetOwnerGui()->GetTargetSerial();

				// 공유 창고(%d 제니)를 구입하시겠습니까?
				sMsgBoxData data;
				data.hHandle = hNPCHandle;
				GetAlarmManager()->FormattedAlarmMessage( "DST_WAREHOUSE_WANT_YOU_BUY_COMMON", FALSE, &data, Logic_FormatZeni(pITEM_TBLDAT->dwCost) );

				break;
			}
		default:
			{
				sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob( pNodeMgr->GetOwnerGui()->GetTargetSerial() );

				sMERCHANT_TBLDAT* pMERCHANT_TBLDAT = Logic_GetMerchantDataFromTable( pNPC_TBLDAT->amerchant_Tblidx[0] );
				DBO_ASSERT( pMERCHANT_TBLDAT, "Not exist merchant index in NPC table");

				RwUInt8 byIndex = (RwUInt8)GetID();
				sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromTable( pMERCHANT_TBLDAT->aitem_Tblidx[byIndex] );
				DBO_ASSERT( pITEM_TBLDAT, "Not exist item index in Merchant table");



				SERIAL_HANDLE hNPCHandle = pNodeMgr->GetOwnerGui()->GetTargetSerial();

				// Would you like to purchase a warehouse (%s zeni)?
				sMsgBoxData data;
				data.hHandle = hNPCHandle; 
				data.byIndex = byIndex;
				GetAlarmManager()->FormattedAlarmMessage( "DST_WAREHOUSE_WANT_YOU_BUY", FALSE, &data, Logic_FormatZeni(pITEM_TBLDAT->dwCost) );

				break;
			}
		}

		GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );
	}
	else if( m_eNodeType == SERVERSELECT_MODE )
	{
		// 서버이동은 ServerSelectNode에서 처리.
		if( GetID() == CCommandList::EXIT_COMMAND )
		{
			CNtlSLEventGenerator::TSMudosaTeleport_Res( pNodeMgr->GetTSKey(), (RwUInt8)GetID(), TRUE );
			GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );				
		}
	}
}

VOID CCommandNode::OnPressedCommand( gui::CComponent* pComponent )
{
	CRectangle rect;

	if( m_ppnlIcon )
	{
		rect = m_ppnlIcon->GetPosition();
		m_ppnlIcon->SetPosition( rect.left + COMMANDNODE_DOWNCOORD_DIFFX, rect.top + COMMANDNODE_DOWNCOORD_DIFFY );		
	}
}

VOID CCommandNode::OnReleasedCommand( gui::CComponent* pComponent )
{
	CRectangle rect;

	if( m_ppnlIcon )
	{
		rect = m_ppnlIcon->GetPosition();
		m_ppnlIcon->SetPosition( rect.left - COMMANDNODE_DOWNCOORD_DIFFX, rect.top - COMMANDNODE_DOWNCOORD_DIFFY );				
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CServerSelectNode

CServerSelectNode::CServerSelectNode( CGuiLineTree* pTree, RwInt32 nID, WCHAR* pText, RwInt32 nCurrentUserCount )
: CGuiLineTreeNode( pTree, nID ), m_pbtnCommand( NULL ), m_ppnlIcon( NULL ), m_ppnlUserCountBack( NULL ), m_ppgbUserCount( NULL )
{
	// gui component설정 및 surface 설정.
	CRectangle rect;
	rect.SetRectWH( COMMANDNODE_X ,COMMANDNODE_Y, COMMANDNODE_WIDTH, COMMANDNODE_HEIGHT );
	m_pbtnCommand = NTL_NEW gui::CButton( rect, pText, pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_pbtnCommand->AddSurfaceDown( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfBtnDown" ) );
	m_pbtnCommand->AddSurfaceFocus( GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfBtnFoc" ) );
	m_pbtnCommand->SetTextDownColor( RGB( 113, 113, 113 ) );
	m_pbtnCommand->SetTextFocusColor( RGB( 255, 255, 255 ) );
	m_pbtnCommand->SetTextUpColor( RGB( 220, 220, 220 ) );
	m_pbtnCommand->SetTextDownCoordDiff( COMMANDNODE_DOWNCOORD_DIFFX, COMMANDNODE_DOWNCOORD_DIFFY );
	m_pbtnCommand->SetTextCoord( COMMANDTEXT_X, 0 );
	m_pbtnCommand->SetTextStyle( COMP_TEXT_LEFT );
	m_pbtnCommand->ApplyText();

	gui::CSurface surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfTimeMachineIcon" );
	rect.SetCenterRect( COMMANDNODEICON_X, COMMANDNODEICON_Y, surface.m_SnapShot.rtRect.GetWidth(), surface.m_SnapShot.rtRect.GetHeight() );
	m_posIcon.SetPos( rect.left, rect.top );
	m_ppnlIcon = NTL_NEW gui::CPanel( rect, pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager(), surface );
	m_ppnlIcon->Enable( false );

	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfUserCountBack" );
	rect.SetRectWH( SERVERSELECTNODE_USERCOUNT_X, SERVERSELECTNODE_USERCOUNT_Y, surface.m_SnapShot.rtRect.GetWidth(), surface.m_SnapShot.rtRect.GetHeight() );
	m_posUserCount.SetPos( rect.left, rect.top );
	m_ppnlUserCountBack = NTL_NEW gui::CPanel( rect, pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager(), surface );
	m_ppnlUserCountBack->Enable( false );

	surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "MultiDialog.srf", "srfUserCount" );
	rect.SetRectWH( SERVERSELECTNODE_USERCOUNT_X, SERVERSELECTNODE_USERCOUNT_Y, surface.m_SnapShot.rtRect.GetWidth(), surface.m_SnapShot.rtRect.GetHeight() );
	m_ppgbUserCount = NTL_NEW gui::CProgressBar( rect, pTree->GetParentGui(), GetNtlGuiManager()->GetSurfaceManager() );
	m_ppgbUserCount->AddSurface( surface );
	m_ppgbUserCount->SetRange( 0, BUDOKAI_MAX_MUDOSA_PLAYER_COUNT );
	m_ppgbUserCount->SetPos( nCurrentUserCount );
	m_ppgbUserCount->Enable( false );

	m_slotClickCommand = m_pbtnCommand->SigClicked().Connect( this, &CServerSelectNode::OnClickCommand );
	m_slotPressedCommand = m_pbtnCommand->SigPressed().Connect( this, &CServerSelectNode::OnPressedCommand );
	m_slotReleasedCommand = m_pbtnCommand->SigReleased().Connect( this, &CServerSelectNode::OnReleasedCommand );
}

CServerSelectNode::~CServerSelectNode(VOID)
{
	NTL_DELETE( m_pbtnCommand );
	NTL_DELETE( m_ppnlIcon );
	NTL_DELETE( m_ppnlUserCountBack );
	NTL_DELETE( m_ppgbUserCount );
}

VOID CServerSelectNode::ShowProc(VOID)
{
	CGuiLineTreeNode::ShowProc();

	m_pbtnCommand->Show( true );
	m_pbtnCommand->SetPosition( m_nPosX + COMMANDNODE_X, m_nPosY + COMMANDNODE_Y );

	m_ppnlIcon->Show( true );
	m_ppnlIcon->SetPosition( m_nPosX + m_posIcon.x, m_nPosY + m_posIcon.y );

	m_ppnlUserCountBack->Show( true );
	m_ppnlUserCountBack->SetPosition( m_nPosX + m_posUserCount.x, m_nPosY + m_posUserCount.y );

	m_ppgbUserCount->Show( true );
	m_ppgbUserCount->SetPosition( m_nPosX + m_posUserCount.x, m_nPosY + m_posUserCount.y );
}

VOID CServerSelectNode::HideProc(VOID)
{
	CGuiLineTreeNode::HideProc();

	m_pbtnCommand->Show( false );

	m_ppnlIcon->Show( false );

	m_ppnlUserCountBack->Show( false );
	m_ppgbUserCount->Show( false ); 
}

VOID CServerSelectNode::OnClickCommand( gui::CComponent* pComponent )
{
	CCommandList* pNodeMgr = reinterpret_cast<CCommandList*>( m_pMgr );

	CNtlSLEventGenerator::TSMudosaTeleport_Res( pNodeMgr->GetTSKey(), (RwUInt8)GetID(), FALSE );
	GetDialogManager()->CloseDialog( DIALOG_MULTIDIALOG );
}

VOID CServerSelectNode::OnPressedCommand( gui::CComponent* pComponent )
{
	CRectangle rect;

	rect = m_ppnlIcon->GetPosition();
	m_ppnlIcon->SetPosition( rect.left + COMMANDNODE_DOWNCOORD_DIFFX, rect.top + COMMANDNODE_DOWNCOORD_DIFFY );			
	m_ppnlUserCountBack->SetPosition( rect.left + COMMANDNODE_DOWNCOORD_DIFFX, rect.top + COMMANDNODE_DOWNCOORD_DIFFY );			
	m_ppgbUserCount->SetPosition( rect.left + COMMANDNODE_DOWNCOORD_DIFFX, rect.top + COMMANDNODE_DOWNCOORD_DIFFY );			
}

VOID CServerSelectNode::OnReleasedCommand( gui::CComponent* pComponent )
{
	CRectangle rect;

	rect = m_ppnlIcon->GetPosition();
	m_ppnlIcon->SetPosition( rect.left - COMMANDNODE_DOWNCOORD_DIFFX, rect.top - COMMANDNODE_DOWNCOORD_DIFFY );					
	m_ppnlUserCountBack->SetPosition( rect.left + COMMANDNODE_DOWNCOORD_DIFFX, rect.top + COMMANDNODE_DOWNCOORD_DIFFY );			
	m_ppgbUserCount->SetPosition( rect.left + COMMANDNODE_DOWNCOORD_DIFFX, rect.top + COMMANDNODE_DOWNCOORD_DIFFY );			
}