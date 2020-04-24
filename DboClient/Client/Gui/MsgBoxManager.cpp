#include "precomp_dboclient.h"
#include "MsgBoxManager.h"

// Shared
#include "NtlTeleport.h"
#include "NPCTable.h"

// Presentation
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"
#include "NtlPLEventGenerator.h"

// Simulation
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSLEventFunc.h"
#include "NtlCameraController.h"
#include "InputActionMap.h"
#include "NtlSLPacketGenerator.h"
#include "NtlSLApi.h"

#include "NtlSobItemAttr.h"

// Dbo
#include "DboGlobal.h"
#include "DialogManager.h"
#include "DboEventGenerator.h"
#include "DboLogic.h"
#include "DboEvent.h"
#include "MsgBoxGui.h"
#include "LogInStageState.h"
#include "GUISoundDefine.h"
#include "CommunityGui.h"
#include "ChatGui.h"
#include "ChannelChangeGui.h"
#include "DisplayStringManager.h"
#include "AlarmManager.h"
#include "CharStageState.h"
#include "LobbyManager.h"

#include "CommercialExtendGui.h"

#define MSGBOXMGR_DEFAULT_ALPHA			255
#define MSGBOXMGR_BEHIND_POPUP_ALPHA	100

CMsgBoxManager* CMsgBoxManager::m_pInstance = NULL;

CMsgBoxManager::CMsgBoxManager(VOID)
{
}

CMsgBoxManager::~CMsgBoxManager(VOID)
{
}

RwBool CMsgBoxManager::CreateInstance(VOID)
{
	NTL_FUNCTION( "CMsgBoxManager::CreateInstance" );

	if( !m_pInstance )
	{
		m_pInstance = NTL_NEW CMsgBoxManager;
		NTL_ASSERT( m_pInstance, "CMsgBoxManager::CreateInstance : Instance Allocate Fail" );
		m_pInstance->LinkEvent();
	}

	NTL_RETURN( TRUE );
}

VOID CMsgBoxManager::DeleteInstance(VOID)
{
	if( m_pInstance )
	{
		m_pInstance->UnLinkEvent();
		m_pInstance->DeleteAllMsgBox();
		NTL_DELETE( m_pInstance );
	}
}

CMsgBoxManager* CMsgBoxManager::GetInstance(VOID)
{
	return m_pInstance;
}

VOID CMsgBoxManager::DeleteMsgBox(const char* strStringID)
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox->IsMsgBoxStringID(strStringID) )
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			iter = m_listMsgBox.erase( iter );			
		}
		else
			++iter;
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox->IsMsgBoxStringID( strStringID ) )
		{
			RwBool bPassPopupToNextMsgBox = FALSE;

			if( iter == m_listPopupMsgBox.begin() )
				bPassPopupToNextMsgBox = TRUE;

			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			iter = m_listPopupMsgBox.erase( iter );			

			if( bPassPopupToNextMsgBox )
			{
				// 아직 Popup이 남아있을떄
				if( iter != m_listPopupMsgBox.end() )
				{
					CMsgBoxGui* pMsgBox = (*iter);
					pMsgBox->SetAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
					pMsgBox->Popup( true );
				}				
				// Popup Msg가 없음.
				else
				{
					SetMsgBoxAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
				}
			}			
		}
		else
			++iter;
	}
}

VOID CMsgBoxManager::DeleteAllMsgBoxExceptNetConnectBox(VOID)
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		//if( !pMsgBox->IsNetConnectBox() )
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			iter = m_listMsgBox.erase( iter );			
		}
		//else
		//	++iter;
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		//if( !pMsgBox->IsNetConnectBox() )
		{
			RwBool bPassPopupToNextMsgBox = FALSE;

			if( iter == m_listPopupMsgBox.begin() )
				bPassPopupToNextMsgBox = TRUE;

			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			iter = m_listPopupMsgBox.erase( iter );			

			if( bPassPopupToNextMsgBox )
			{
				// 아직 Popup이 남아있을떄
				if( iter != m_listPopupMsgBox.end() )
				{
					CMsgBoxGui* pMsgBox = (*iter);
					pMsgBox->SetAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
					pMsgBox->Popup( true );
				}				
				// Popup Msg가 없음.
				else
				{
					SetMsgBoxAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
				}
			}			
		}
		//else
		//	++iter;
	}	
}

VOID CMsgBoxManager::DeleteNetConnectBox(VOID)
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		//if( pMsgBox->IsNetConnectBox() )
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			iter = m_listMsgBox.erase( iter );			
		}
		//else
		//	++iter;
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		//if( pMsgBox->IsNetConnectBox() )
		{
			RwBool bPassPopupToNextMsgBox = FALSE;

			if( iter == m_listPopupMsgBox.begin() )
				bPassPopupToNextMsgBox = TRUE;

			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			iter = m_listPopupMsgBox.erase( iter );			

			if( bPassPopupToNextMsgBox )
			{
				// 아직 Popup이 남아있을떄
				if( iter != m_listPopupMsgBox.end() )
				{
					CMsgBoxGui* pMsgBox = (*iter);
					pMsgBox->SetAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
					pMsgBox->Popup( true );
				}				
				// Popup Msg가 없음.
				else
				{
					SetMsgBoxAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
				}
			}			
		}
		//else
		//	++iter;
	}	
}

CMsgBoxGui* CMsgBoxManager::GetMsgBox(char* strStringID, LIST_MSGBOX* pList /* = NULL  */ )
{
	CMsgBoxGui* pSearchMsgBox = NULL;
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox->IsMsgBoxStringID( strStringID ) )
		{
			pSearchMsgBox = pMsgBox;
			if( pList )
				pList->push_back( pMsgBox );
		}		
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox->IsMsgBoxStringID( strStringID ) )
		{
			pSearchMsgBox = pMsgBox;
			if( pList )
				pList->push_back( pMsgBox );
		}		
	}

	return pSearchMsgBox;
}

VOID CMsgBoxManager::ShowAllMsgBox( bool bShow )
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);
		pMsgBox->Show( bShow );	
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);
		pMsgBox->Show( bShow );		
	}
}

RwBool CMsgBoxManager::IsShowMsgBox(VOID)
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);
		if( pMsgBox->IsShow() )
			return TRUE;		
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);
		if( pMsgBox->IsShow() )
			return TRUE;
	}

	return FALSE;
}

RwBool CMsgBoxManager::IsShowPopupMsgBox(VOID)
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);
		if( pMsgBox->IsShow() )
			return TRUE;
	}

	return FALSE;
}

VOID CMsgBoxManager::DeleteMsgBox( CMsgBoxGui* pDeletedMsgBox )
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox == pDeletedMsgBox )
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			m_listMsgBox.erase( iter );
			return;
		}
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox == pDeletedMsgBox )
		{
			RwBool bPassPopupToNextMsgBox = FALSE;

			if( iter == m_listPopupMsgBox.begin() )
				bPassPopupToNextMsgBox = TRUE;

			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
			iter = m_listPopupMsgBox.erase( iter );
			
			if( bPassPopupToNextMsgBox )
			{
				// 아직 Popup이 남아있을떄
				if( iter != m_listPopupMsgBox.end() )
				{
					CMsgBoxGui* pMsgBox = (*iter);
					pMsgBox->SetAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
					pMsgBox->Popup( true );
				}				
				// Popup Msg가 없음.
				else
				{
					SetMsgBoxAlpha( (RwUInt8)MSGBOXMGR_DEFAULT_ALPHA );
				}
			}		

			return;
		}
	}
}

CMsgBoxGui* CMsgBoxManager::CreateMsgBox( RwBool bPopup, const char* StringID)
{
	CMsgBoxGui* pMsgBox = NTL_NEW CMsgBoxGui( "MsgBoxGui" );
	NTL_ASSERT( pMsgBox, "CMsgBoxManager::CreateInstance : MsgBox Allocate Fail" );
	if( !pMsgBox->Create(StringID) )
	{
		pMsgBox->Destroy();
		NTL_DELETE( pMsgBox );
		return NULL;
	}
	GetNtlGuiManager()->AddGui( pMsgBox );

	if( bPopup )
	{
		SetMsgBoxAlpha( MSGBOXMGR_BEHIND_POPUP_ALPHA );

		if( !m_listPopupMsgBox.empty() )
			m_listPopupMsgBox.front()->Popup( false );

		pMsgBox->Popup( true );
		m_listPopupMsgBox.push_front( pMsgBox );
	}
	else
		m_listMsgBox.push_back( pMsgBox );

	return pMsgBox;
}

VOID CMsgBoxManager::DeleteAllMsgBox(VOID)
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox )
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
		}
	}

	m_listMsgBox.clear();

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox )
		{
			CNtlPLGuiManager::GetInstance()->RemoveGui( pMsgBox );
			pMsgBox->Destroy();
			NTL_DELETE( pMsgBox );
		}
	}

	m_listPopupMsgBox.clear();
}

VOID CMsgBoxManager::MsgBoxShowProc( RwBool bPopup, RwBool bHasInput, std::list<sMsgBoxCustomBtn>* plistCustomBtn, const WCHAR* szText, const char* StringID,
									sMsgBoxData* pData, RwReal fShowTime, RwBool bDuplicate, RwBool bUpdate)
{
	SConnectData* pConnectData = GetDboGlobal()->GetConnectData();
	if( pConnectData->sGameCon.bBanishment )
		return;

	if (!StringID)
	{
		NTL_ASSERT(FALSE, "StringID is null");
	}

	if(strlen(StringID) == 0)
	{
		NTL_ASSERT(FALSE, "StringID's length is zero");
	}

	if (!szText)
	{
		NTL_ASSERT(FALSE, "Text is null");
	}

	if (wcslen(szText) == 0)
	{
		NTL_ASSERT(FALSE, "Text's length is zero");
	}

	/*if (pData == NULL)
	{
		DBO_WARNING_MESSAGE("Alarm data is null, string ID : " << StringID);
	}*/

	CMsgBoxGui* pMsgBox = GetMsgBox((char*)StringID);
	RwBool bCreateMsgBox = TRUE;

	if( pMsgBox )
	{
		if( !bDuplicate )
		{
			if( bUpdate )
			{
				pMsgBox->UpdateMsgBox( szText, pData );			
			}		
			else
			{
				CDboEventGenerator::MsgBoxResult( FALSE, StringID, pData );
			}

			bCreateMsgBox = FALSE;
		}	
	}

	if( bCreateMsgBox )
	{
		pMsgBox = CreateMsgBox( bPopup, StringID);

		pMsgBox->SetBoxData( bPopup, bHasInput, plistCustomBtn, szText, StringID, pData, fShowTime );

		Logic_PlayGUISound(GSD_SYSTEM_MESSAGEBOX);
	}
}

VOID CMsgBoxManager::SetMsgBoxAlpha( RwUInt8 byAlpha )
{
	LIST_MSGBOX_ITER iter;

	for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox )
			pMsgBox->SetAlpha( byAlpha );		
	}

	for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
	{
		CMsgBoxGui* pMsgBox = (*iter);

		if( pMsgBox )
			pMsgBox->SetAlpha( byAlpha );		
	}
}

VOID CMsgBoxManager::HandleEvents( RWS::CMsg &msg )
{
	if( msg.Id == g_EventMsgBoxShow )
	{
		//this wont work for some because strKey is NULL
		SDboEventMsgBoxShow* pMsgBoxShow = reinterpret_cast<SDboEventMsgBoxShow*>( msg.pData ); 

		MsgBoxShowProc( pMsgBoxShow->bPopup, pMsgBoxShow->bHasInput, pMsgBoxShow->plistCustomBtn, pMsgBoxShow->wstrString.c_str(), pMsgBoxShow->strKey.c_str(),
						pMsgBoxShow->pData, pMsgBoxShow->fShowTime, pMsgBoxShow->bAcceptDuplicate, pMsgBoxShow->bUpdate );
	}
	else if( msg.Id == g_EventMsgBoxResult )
	{
		/* // TO DO */ //by daneos
		SDboEventMsgBoxResult *pMsgBoxResult = reinterpret_cast<SDboEventMsgBoxResult*>( msg.pData );
		
		DBO_WARNING_MESSAGE("pMsgBoxResult: " << pMsgBoxResult->strID.c_str());

		//case MBW_NET_CONNECT_FAIL:
		//case MBW_NET_DISCONNECT:
		//case MBW_GAME_EXIT:
		//case MBW_LOGIN_CONNECT_FAIL:
		//case MBW_AUTHKEY_FAIL:
		//case MBW_CHARACTRE_CONNECT_FAIL:
		//case MBW_POST_QUIT_MESSAGE:			
		//	{
		//		if( pMsgBoxResult->eResult == MBR_OK )
		//			PostQuitMessage(0);				
		//	}	
		//	break;
		if (pMsgBoxResult->strID == "DST_SERVER_DISCONNECT" || pMsgBoxResult->strID == "DST_GAME_EXIT" || pMsgBoxResult->strID == "DST_LOBBY_NO_RELPY_SERVER")
		{
			if (pMsgBoxResult->eResult == MBR_OK)
				PostQuitMessage(0);
		}

		//case MBW_CAHR_SERVER_EXIT
		else if (pMsgBoxResult->strID == "DST_CHAR_SERVER_EXIT")
		{
			if (pMsgBoxResult->eResult == MBR_OK)
			{
			//	GetNtlWorldConcept()->EnableAction(FALSE);
			//	GetNtlGuiManager()->GetGuiManager()->DisableInput();
			//	CNtlPLEventGenerator::CinematicFadeOut(CINEMATIC_FADEINOUT_SCREEN_PANEL, 0.5f, 1, 1, 1, 220);
				GetDboGlobal()->GetGamePacketGenerator()->SendCharServerExitReq();
			}  
		}

		else if (pMsgBoxResult->strID == "DST_REGEN_PLAYER_MAINWORLD_WITH_POPO")
		{
			if (pMsgBoxResult->eResult == MBR_OK)
			{
				if (_wcsicmp(pMsgBoxResult->pData->pwcText, L"DST_REGEN_MB_MAINWORLD_MOVE_TO_POPOSTONE_BTN") == 0)
				{
					GetDboGlobal()->GetGamePacketGenerator()->SendCharRevivalReq(DBO_REVIVAL_REQUEST_TYPE_TELEPORT_SOMEWHERE);
				}
				else // DST_REGEN_MB_MAINWORLD_USE_POPOSTONE_CHUNK_BTN
				{
					BYTE byPlace = INVALID_BYTE;
					BYTE byPos = INVALID_BYTE;

					if (Logic_FindInventoryItemByItemType(ITEM_TYPE_CRESCENT_POPO, &byPlace, &byPos))
					{
						GetDboGlobal()->GetGamePacketGenerator()->SendCharCrescentPopoRevivalReq(Logic_ConvertBagIdxToContainerType(byPlace), byPos);
					}
					else
					{
						GetAlarmManager()->AlarmMessage("DST_COMMERCIAL_HAVE_NOT_CRESCENT_POPO");
					}
				}
			}
		}

		else if (pMsgBoxResult->strID == "DST_ITEM_OPTION_RESTRUCTURE_CONFIRM_MESSAGE")
		{
			if (pMsgBoxResult->eResult == MBR_OK)
			{
				CDboEventGenerator::ItemChangeOptionConfirmation();
			}
		}
		else if(pMsgBoxResult->strID == "DST_REGEN_PLAYER_CCBD"
			|| pMsgBoxResult->strID == "DST_REGEN_PLAYER_DOJO"
			|| pMsgBoxResult->strID == "DST_REGEN_PLAYER_MAINWORLD"
			|| pMsgBoxResult->strID == "DST_REGEN_PLAYER_PARTY_DUNGEON"
			|| pMsgBoxResult->strID == "DST_REGEN_PLAYER_TMQ")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
				GetDboGlobal()->GetGamePacketGenerator()->SendCharRevivalReq( DBO_REVIVAL_REQUEST_TYPE_TELEPORT_SOMEWHERE );
		}
		else if(pMsgBoxResult->strID == "DST_REGEN_PLAYER_TLQ")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
				GetDboGlobal()->GetGamePacketGenerator()->SendCharRevivalReq( DBO_REVIVAL_REQUEST_TYPE_CURRENT_POSITION );	
			else
				GetDboGlobal()->GetGamePacketGenerator()->SendCharRevivalReq(DBO_REVIVAL_REQUEST_TYPE_TELEPORT_SOMEWHERE);
		}			
		else if(pMsgBoxResult->strID == "DST_PARTY_PARTY_INVITE")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
				GetDboGlobal()->GetGamePacketGenerator()->SendPartyResponseInvite(NTL_INVITATION_RESPONSE_ACCEPT);
			else if( pMsgBoxResult->eResult == MBR_CANCEL )
				GetDboGlobal()->GetGamePacketGenerator()->SendPartyResponseInvite(NTL_INVITATION_RESPONSE_DECLINE);
		}

		else if (pMsgBoxResult->strID == "DST_WAREHOUSE_WANT_YOU_BUY")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
			{
				// 창고 NPC Serial
				SERIAL_HANDLE hNPCHandle = pMsgBoxResult->pData->hHandle;

				// The Merchant Tabe index in the first index of the NPC table
				GetDboGlobal()->GetGamePacketGenerator()->SendBankBuy(hNPCHandle, 0, pMsgBoxResult->pData->byIndex);

				Logic_CancelNpcFacing();
			}
			else if( pMsgBoxResult->eResult == MBR_CANCEL )
			{
				Logic_CancelNpcFacing();
			}
		}
		else if(pMsgBoxResult->strID == "DST_WAREHOUSE_WANT_YOU_BUY_COMMON")
		{	
			if( pMsgBoxResult->eResult == MBR_OK )
			{
				// 창고 NPC Serial
				SERIAL_HANDLE hNPCHandle = pMsgBoxResult->pData->hHandle;

				// The Merchant Tabe index in the first index of the NPC table
				GetDboGlobal()->GetGamePacketGenerator()->SendBankBuy(hNPCHandle, 0, 3);

				Logic_CancelNpcFacing();
			}		
			else if( pMsgBoxResult->eResult == MBR_CANCEL )
			{
				Logic_CancelNpcFacing();
			}
		}
		else if(pMsgBoxResult->strID == "DST_FREEPVP_CHALLENGE")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
			{
				if(GetNtlWorldConcept()->IsEnableAction(E_ACTION_CAN_COMMUNICATION) )
					GetDboGlobal()->GetGamePacketGenerator()->SendFreeBattleAcceptRes(ACCEPT_RES_TYPE_OK);
				else
					Logic_WarnningWorldConceptActionMessage( WORLD_CONCEPT_SECOND_GRADE );
			}
			else if( pMsgBoxResult->eResult == MBR_CANCEL )
				GetDboGlobal()->GetGamePacketGenerator()->SendFreeBattleAcceptRes(ACCEPT_RES_TYPE_DENY);
			else
				GetDboGlobal()->GetGamePacketGenerator()->SendFreeBattleAcceptRes(ACCEPT_RES_TYPE_CANCEL);			
		}

		else if (pMsgBoxResult->strID == "DST_PARTY_CREATE")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
			{
				if (!pMsgBoxResult->pData->pwcText)
					return;

				RwInt32 iLength = wcslen(pMsgBoxResult->pData->pwcText);

				if (iLength < NTL_MIN_SIZE_PARTY_NAME || iLength > NTL_MAX_SIZE_PARTY_NAME)
				{
					GetAlarmManager()->AlarmMessage("DST_PARTY_NAME_RULE");
					return;
				}

				if( GetChattingFilter()->IsAnyInvalidText(pMsgBoxResult->pData->pwcText) )
				{
					GetAlarmManager()->AlarmMessage("DST_LOBBY_CAN_NOT_USE_THE_WORD");
					return;
				}

				GetDboGlobal()->GetGamePacketGenerator()->SendPartyCreate(pMsgBoxResult->pData->pwcText);
			}			
		}

		else if(pMsgBoxResult->strID == "DST_PARTY_LEAVE_ASK")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
				GetDboGlobal()->GetGamePacketGenerator()->SendPartyLeave();				
		}
		
		
		//case MBW_PARTY_RELEASE_CHARM:
		//	{
		//		if( pMsgBoxResult->eResult == MBR_OK )
		//		{
		//			sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );
		//			GetDboGlobal()->GetGamePacketGenerator()->SendPartyDeActivateCharm((RwUInt8)pMsgBoxData->uiParam);
		//		}				
		//	}
		//	break;

		else if (pMsgBoxResult->strID == "DST_STACKITEM_CONFIRM_DROP" || pMsgBoxResult->strID == "DST_ITEM_CONFIRM_DROP")
		{
			if (pMsgBoxResult->eResult == MBR_OK)
			{
				RwBool bPacketLock = FALSE;

				if( !Logic_ItemDeleteProc( &pMsgBoxResult->pData->sItemInfo, &bPacketLock ) )
					CDboEventGenerator::EnableItemIcon( TRUE, pMsgBoxResult->pData->sItemInfo.ePlace, pMsgBoxResult->pData->sItemInfo.nSlotIdx, pMsgBoxResult->pData->sItemInfo.nPlaceIdx );
				else if( bPacketLock )
				{
					LIST_MSGBOX listDropConfirmMsg;
					LIST_MSGBOX_ITER iter;
					GetMsgBox((char*)pMsgBoxResult->strID.c_str(), &listDropConfirmMsg );

					for( iter = listDropConfirmMsg.begin() ; iter != listDropConfirmMsg.end() ; ++iter )
					{
						CMsgBoxGui* pMsgBox = (*iter);
									
						if( pMsgBoxResult->pData->sItemInfo.hSerial == pMsgBox->GetMsgBoxData()->sItemInfo.hSerial )
						{
							pMsgBox->DoNotHide();
							break;
						}
					}
				}
			}
			else
			{
				CDboEventGenerator::EnableItemIcon(TRUE, pMsgBoxResult->pData->sItemInfo.ePlace, pMsgBoxResult->pData->sItemInfo.nSlotIdx, pMsgBoxResult->pData->sItemInfo.nPlaceIdx);
			}
		}
		else if (pMsgBoxResult->strID == "DST_COMMERCIAL_MB_DELETE_TERM_LIMETED_ITEM_MSG")
		{
			if (pMsgBoxResult->eResult == MBR_OK)
			{
				if (pMsgBoxResult->pData->pwcText && wcscmp(pMsgBoxResult->pData->pwcText, L"DST_COMMERCIAL_MB_UPDATE_TERM_LIMETED_ITEM_BTN") == 0)
				{
					CDboEventGenerator::EnableItemIcon(TRUE, pMsgBoxResult->pData->sItemInfo.ePlace, pMsgBoxResult->pData->sItemInfo.nSlotIdx, pMsgBoxResult->pData->sItemInfo.nPlaceIdx);

					CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(pMsgBoxResult->pData->sItemInfo.hSerial));
					if (pItem)
					{
						CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());
						if (pItemAttr)
						{
							sITEM_TBLDAT* pITEM_TBLDAT = pItemAttr->GetItemTbl();
							if (pITEM_TBLDAT)
							{
								CDboEventGenerator::OpenCommercialExtendGui(
									Logic_ItemGetGUI_EXTEND_MODEByCommonPointType(pITEM_TBLDAT->byCommonPointType), /// OR  INVALID_TBLIDX
									pITEM_TBLDAT->tblidx);
							}
						}
					}
				}
				else // DST_MSG_BTN_OK
				{
					RwBool bPacketLock = FALSE;

					if (!Logic_ItemDeleteProc(&pMsgBoxResult->pData->sItemInfo, &bPacketLock))
					{
						CDboEventGenerator::EnableItemIcon(TRUE, pMsgBoxResult->pData->sItemInfo.ePlace, pMsgBoxResult->pData->sItemInfo.nSlotIdx, pMsgBoxResult->pData->sItemInfo.nPlaceIdx);
					}
					else if (bPacketLock)
					{
						LIST_MSGBOX listDropConfirmMsg;
						LIST_MSGBOX_ITER iter;
						GetMsgBox((char*)pMsgBoxResult->strID.c_str(), &listDropConfirmMsg);

						for (iter = listDropConfirmMsg.begin(); iter != listDropConfirmMsg.end(); ++iter)
						{
							CMsgBoxGui* pMsgBox = (*iter);

							if (pMsgBoxResult->pData->sItemInfo.hSerial == pMsgBox->GetMsgBoxData()->sItemInfo.hSerial)
							{
								pMsgBox->DoNotHide();
								break;
							}
						}
					}
				}
			}
			else
			{
				CDboEventGenerator::EnableItemIcon(TRUE, pMsgBoxResult->pData->sItemInfo.ePlace, pMsgBoxResult->pData->sItemInfo.nSlotIdx, pMsgBoxResult->pData->sItemInfo.nPlaceIdx);
			}
		}

		else if (pMsgBoxResult->strID == "DST_STACKITEM_CONFIRM_DROP_ON_REMOTE_SELL") // to do
		{

		}

		else if (pMsgBoxResult->strID == "DST_TRADE_ACCEPT_TRADE")
		{
			if (pMsgBoxResult->eResult == MBR_OK)
				GetDboGlobal()->GetGamePacketGenerator()->SendTradeOkReq(GetDboGlobal()->GetAskedPCSerial(), ACCEPT_RES_TYPE_OK);				
			else if( pMsgBoxResult->eResult == MBR_CANCEL )
				GetDboGlobal()->GetGamePacketGenerator()->SendTradeOkReq(GetDboGlobal()->GetAskedPCSerial(), ACCEPT_RES_TYPE_DENY);
			else 
				GetDboGlobal()->GetGamePacketGenerator()->SendTradeOkReq(GetDboGlobal()->GetAskedPCSerial(), ACCEPT_RES_TYPE_CANCEL);
		}

		else if (pMsgBoxResult->strID == "DST_GUILD_DO_MAKE")
		{
			Logic_CancelNpcFacing();

			if (pMsgBoxResult->eResult == MBR_OK)
			{
				const WCHAR* pwcText = pMsgBoxResult->pData->pwcText;

				if (!pwcText)
					return;

				RwInt32 iLength = wcslen(pwcText);

				if (iLength < NTL_MIN_SIZE_GUILD_NAME || iLength > NTL_MAX_SIZE_GUILD_NAME)
				{
					GetAlarmManager()->AlarmMessage("DST_GUILD_NAME_RULE");
					return;
				}

				if (GetChattingFilter()->IsAnyInvalidText(pwcText))
				{
					GetAlarmManager()->AlarmMessage("DST_LOBBY_CAN_NOT_USE_THE_WORD");
					return;
				}

				GetDboGlobal()->GetGamePacketGenerator()->SendGuildCreateReq(Logic_GetAvatarTargetHandle(), pwcText);

			}
		}
		else if (pMsgBoxResult->strID == "DST_GUILD_DO_DISSOLVE")
		{
			Logic_CancelNpcFacing();

			if (pMsgBoxResult->eResult == MBR_OK)
			{
				GetDboGlobal()->GetChatPacketGenerator()->SendGuild_Disband_Req();
			}
		}
		else if(pMsgBoxResult->strID == "DST_GUILD_INVITE_REQ")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
				GetDboGlobal()->GetChatPacketGenerator()->SendGuild_Response_Invitation(NTL_INVITATION_RESPONSE_ACCEPT);				
			else if( pMsgBoxResult->eResult == MBR_CANCEL )
				GetDboGlobal()->GetChatPacketGenerator()->SendGuild_Response_Invitation(NTL_INVITATION_RESPONSE_DECLINE);
			// PeessiMsgBoxOpenCancel
		}
		else if (pMsgBoxResult->strID == "DST_GUILD_ENTRUST_MASTER")
		{
			if (pMsgBoxResult->eResult == MBR_OK)
			{
				CCommunityGui* pCommunityGui = reinterpret_cast<CCommunityGui*>(GetDialogManager()->GetDialog(DIALOG_COMMUNITY));
				CHARACTERID charID = pCommunityGui->GetGuildSelectedID();

				GetDboGlobal()->GetChatPacketGenerator()->SendGuild_Change_Guild_Master_Req(charID);
			}
		}
		else if(pMsgBoxResult->strID == "DST_GUILD_APPOINT_SECOND_MASTER")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
			{
				CCommunityGui* pCommunityGui = reinterpret_cast<CCommunityGui*>(GetDialogManager()->GetDialog(DIALOG_COMMUNITY));
				CHARACTERID charID = pCommunityGui->GetGuildSelectedID();

				GetDboGlobal()->GetChatPacketGenerator()->SendGuild_Appoint_Second_Master_Req(charID);
			}				
		}
		else if(pMsgBoxResult->strID == "DST_GUILD_DISMISS_SECOND_MASTER_REQ")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
			{
				CCommunityGui* pCommunityGui = reinterpret_cast<CCommunityGui*>(GetDialogManager()->GetDialog(DIALOG_COMMUNITY));
				CHARACTERID charID = pCommunityGui->GetGuildSelectedID();

				GetDboGlobal()->GetChatPacketGenerator()->SendGuild_DIsmiss_Second_Master_Req(charID);
			}				
		}
		else if(pMsgBoxResult->strID == "DST_GUILD_KICK_OUT")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
			{
				CCommunityGui* pCommunityGui = reinterpret_cast<CCommunityGui*>(GetDialogManager()->GetDialog(DIALOG_COMMUNITY));
				CHARACTERID charID = pCommunityGui->GetGuildSelectedID();

				GetDboGlobal()->GetChatPacketGenerator()->SendGuild_Kick_Out_Req(charID);
			}
		}
		else if(pMsgBoxResult->strID == "DST_GUILD_LEAVE_ASK")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
				GetDboGlobal()->GetChatPacketGenerator()->SendGuild_Leave_Req();				
		}
		else if(pMsgBoxResult->strID == "DST_GUILD_ADD_FUNCTION")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
			{
				sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );
				GetDboGlobal()->GetGamePacketGenerator()->SendGuildFunctionAddReq(Logic_GetAvatarTargetHandle(), pMsgBoxData->byIndex);
			}				
			else if( pMsgBoxResult->eResult == MBR_CANCEL )
			{
				Logic_CancelNpcFacing();
			}
		}
		else if(pMsgBoxResult->strID == "DST_GUILD_CONTRIBUTION_ZENNY")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
			{
				sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );
				GetDboGlobal()->GetGamePacketGenerator()->SendGuildGiveZenny(Logic_GetAvatarTargetHandle(), (RwUInt32)pMsgBoxData->uiParam);
			}				
			else if( pMsgBoxResult->eResult == MBR_CANCEL )
			{
				Logic_CancelNpcFacing();
			}
		}
		else if(pMsgBoxResult->strID == "DST_GUILD_CHANGE_GUILD_NAME")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
			{
				sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );

				if(!pMsgBoxData->pwcText)
					return;

				RwInt32 iLength = wcslen(pMsgBoxData->pwcText);

				if (iLength < NTL_MIN_SIZE_GUILD_NAME || iLength > NTL_MAX_SIZE_GUILD_NAME)
				{
					GetAlarmManager()->AlarmMessage("DST_GUILD_NAME_RULE");
					return;
				}

				if( GetChattingFilter()->IsAnyInvalidText(pMsgBoxData->pwcText) )
				{
					GetAlarmManager()->AlarmMessage("DST_LOBBY_CAN_NOT_USE_THE_WORD");
					return;
				}

				GetDboGlobal()->GetGamePacketGenerator()->SendGuildChangeNameReq(pMsgBoxData->pwcText, pMsgBoxData->sItemInfo.nPlaceIdx, pMsgBoxData->sItemInfo.nSlotIdx);
			}				
			else if( pMsgBoxResult->eResult == MBR_CANCEL )
			{
				Logic_CancelNpcFacing();
			}
		}
		else if (pMsgBoxResult->strID == "DST_RENAME_MB_CHARAC_RENAME")
		{
			if (pMsgBoxResult->eResult != MBR_OK)
				return;

			sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>(pMsgBoxResult->pData);

			if (!pMsgBoxData->pwcText)
				return;

			RwInt32 iLength = wcslen(pMsgBoxData->pwcText);

			if (iLength < NTL_MIN_SIZE_CHAR_NAME || iLength > NTL_MAX_SIZE_CHAR_NAME)
			{
				GetAlarmManager()->AlarmMessage("DST_CHARACTER_NAME_RULE");
				return;
			}

			if (GetChattingFilter()->IsAnyInvalidText(pMsgBoxData->pwcText))
			{
				GetAlarmManager()->AlarmMessage("DST_LOBBY_CAN_NOT_USE_THE_WORD");
				return;
			}

			GetDboGlobal()->GetGamePacketGenerator()->SendCharRenameReq(pMsgBoxData->pwcText, pMsgBoxData->sItemInfo.nPlaceIdx, pMsgBoxData->sItemInfo.nSlotIdx);
		}

		else if(pMsgBoxResult->strID == "DST_TUTORIAL_ASK_END")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
				GetDboGlobal()->GetGamePacketGenerator()->SendTutorialPlayQuitReq();				
		} 


		else if(pMsgBoxResult->strID == "DST_BUS_ASK_GET_ON")
        {
			if( pMsgBoxResult->eResult == MBR_OK )
			{
				sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );
				CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>( pMsgBoxData->pData );
				API_GetSLPacketGenerator()->SendRideOnBus(pActor->GetSerialID());
			}
		}
		else if(pMsgBoxResult->strID == "DST_BUS_ASK_GET_OFF")
		{	
			if( pMsgBoxResult->eResult == MBR_OK )
				API_GetSLPacketGenerator()->SendRideOffBus();
		}
		
		else if(pMsgBoxResult->strID == "DST_ASK_CHANGE_CHANNEL")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
			{
				SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
				CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
				if( !pLobby )
				{
					DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
					return;
				}

				CChannelChangeGui* pGui = reinterpret_cast<CChannelChangeGui*>( GetDialogManager()->GetDialog(DIALOG_CHANNGEL_CHANGE) );

				if( pGui->GetSelectedChannelIndex() == pLobby->GetSelectedChannelIndex() )
				{
					GetAlarmManager()->AlarmMessage("DST_ALREADY_SAME_CHANNEL");
					return;
				}

				sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );

				GetDboGlobal()->GetGamePacketGenerator()->SendChannelChangeReq(pMsgBoxData->byIndex);
			}				
		}

		///*case MBW_HOIPOIMIX_SET_FARE:
		//	{
		//		if( pMsgBoxResult->eResult == MBR_OK )
		//		{
		//			sMsgBoxData* pData = (sMsgBoxData*)pMsgBoxResult->pData;

		//			RwUInt8 bySetFare = (RwUInt8)_wtoi( pData->pwcText );

		//			if( bySetFare < 50 || bySetFare > 200 )
		//			{
		//				GetAlarmManager()->AlarmMessage( DST_HOIPOIMIX_GUIDE_FARERATE_ERROR ); 
		//				break;
		//			}

		//			GetDboGlobal()->GetGamePacketGenerator()->SendHoipoiMixItemMachineFare(
		//				pData->hExtendHandle, bySetFare );
		//		}
		//	}
		//	break;*/
		else if(pMsgBoxResult->strID == "DST_VEHICLE_ASK_GET_OFF")
		{
			if( pMsgBoxResult->eResult == MBR_OK)
				API_GetSLPacketGenerator()->SendVehicle_EndReq();
		}

		else if (pMsgBoxResult->strID == "DST_CCBD_LEAVE_CCBD")
		{
			if (pMsgBoxResult->eResult == MBR_OK)
				GetDboGlobal()->GetGamePacketGenerator()->SendBattleDungeonLeaveReq();
		}

		else if (pMsgBoxResult->strID == "DST_TUTORIAL_ASK_BEGIN")
		{
			if (MBR_OK == pMsgBoxResult->eResult)
				GetDboGlobal()->SetEnterTutorial(TRUE);
			else if (pMsgBoxResult->eResult == MBR_CANCEL)
				GetDboGlobal()->SetEnterTutorial(FALSE);

			CDboEventGenerator::LobbyEvent(LMT_GAME_SERVER_WAIT_CHECK_REQ);
		}
		else if (pMsgBoxResult->strID == "DST_LOBBY_ADD_CHARACTER")
		{
			if (pMsgBoxResult->eResult == MBR_OK)
			{
				CDboEventGenerator::LobbyEvent(LMT_CREATE_CHARACTER);
			}
		}
		else if(pMsgBoxResult->strID == "DST_LOBBY_DEL_CHARACTER")
		{
			DBO_WARNING_MESSAGE("ABC");

			if( MBR_OK == pMsgBoxResult->eResult )
			{
				const WCHAR* pwcText = pMsgBoxResult->pData->pwcText;

				if( !pwcText )
					return;

				if( wcslen(pwcText) == 0 )
				{
					GetAlarmManager()->AlarmMessage("DST_LOBBY_EMPTYNAME");
					return;
				}

				// 캐릭터 삭제
				SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
				CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
				if( !pLobby )
				{
					DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
					return;
				}

				sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( pLobby->GetSelectedCharacterIndex() );
				if( !pLOBBY_CHARACTER )
				{
					DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
					return;
				}


				CHARACTERID charID = pLOBBY_CHARACTER->tSummary.charId;

				GetDboGlobal()->GetLobbyPacketGenerator()->SendCharDelReq(charID, pwcText);
				GetCharStageState()->ChangeState(CHAR_STATE_SELECT_DEL_CHAR);
			}				
		}
			
		else if(pMsgBoxResult->strID == "DST_LOBBY_RECOVERY_CHAR")
		{
			if( MBR_OK == pMsgBoxResult->eResult )
			{
				const WCHAR* pwcText = pMsgBoxResult->pData->pwcText;

				if( !pwcText )
					return;

				if( wcslen(pwcText) == 0 )
				{
					GetAlarmManager()->AlarmMessage("DST_LOBBY_EMPTYNAME");
					return;
				}

				// 캐릭터 복구
				SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
				CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
				if( !pLobby )
				{
					DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
					return;
				}

				sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( pLobby->GetSelectedCharacterIndex() );
				if( !pLOBBY_CHARACTER )
				{
					DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
					return;
				}


				GetDboGlobal()->GetLobbyPacketGenerator()->SendCharDelCancelReq(pLOBBY_CHARACTER->tSummary.charId, pwcText);
				GetCharStageState()->ChangeState(CHAR_STATE_SELECT_RECOVERY_CHAR);			
			}				
		}

		else if(pMsgBoxResult->strID == "DST_LOBBY_WAIT_GAME_SERVER_CONNECT")
		{
			if( MBR_CANCEL == pMsgBoxResult->eResult )
			{
				GetDboGlobal()->GetLobbyPacketGenerator()->SendCharConnectGameServer_WaitCancelReq( GetCharStageState()->GetSelectedChannelIndex() );
				GetCharStageState()->ChangeState(CHAR_STATE_CANCEL_WAIT_GAME_SERVER_CONNECT);
			}
		}
		else if(pMsgBoxResult->strID == "DST_DOJO_ASK_ESTABLISHMENT")
		{
			if( MBR_OK == pMsgBoxResult->eResult )
			{
				SERIAL_HANDLE hNPC = Logic_GetAvatarTargetHandle();
				sNPC_TBLDAT* pNPC_TBLDAT = Logic_GetNPCDataFromSob(hNPC);
				if( pNPC_TBLDAT )
				{	
					GetDboGlobal()->GetGamePacketGenerator()->SendDojo_CreateReq(hNPC);
				}
				else
				{
					DBO_WARNING_MESSAGE("Not exist NPC table of handle : " << hNPC);

					Logic_CancelNpcFacing();
				}					
			}
			else
			{
				Logic_CancelNpcFacing();
			}
		}

		else if(pMsgBoxResult->strID == "DST_DOJO_ASK_UPGRAGE_WITH_ZENNY_AND_HOIPOI")
		{
			if( MBR_OK == pMsgBoxResult->eResult )
			{
				sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );
				GetDboGlobal()->GetGamePacketGenerator()->SendDojo_FunctionAddReq(Logic_GetAvatarTargetHandle(),
														pMsgBoxData->ucData[0],
														pMsgBoxData->ucData[1],
														pMsgBoxData->ucData[2]);
			}
			else
			{
				CDboEventGenerator::EnableItemIcon(TRUE, PLACE_DOJO_UPRAGE, 0);
			}
		}
		else if(pMsgBoxResult->strID == "DST_DOJO_ADD_FUNCTION")
		{
			if( pMsgBoxResult->eResult == MBR_OK )
			{
				sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );
				GetDboGlobal()->GetGamePacketGenerator()->SendDojo_FunctionAddReq(Logic_GetAvatarTargetHandle(),
																					pMsgBoxData->byIndex,
																					INVALID_BYTE,
																					INVALID_BYTE);
			}				
			else if( pMsgBoxResult->eResult == MBR_CANCEL )
			{
				Logic_CancelNpcFacing();
			}
		}
		else if(pMsgBoxResult->strID == "DST_DOJO_ASK_UPGRAGE_WITH_ZENNY")
		{
			if( MBR_OK == pMsgBoxResult->eResult )
			{
				sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>( pMsgBoxResult->pData );
				GetDboGlobal()->GetGamePacketGenerator()->SendDojo_FunctionAddReq(Logic_GetAvatarTargetHandle(),
														pMsgBoxData->ucData[0],
														INVALID_BYTE,
														INVALID_BYTE);
			}
			else
			{
				CDboEventGenerator::EnableItemIcon(TRUE, PLACE_DOJO_UPRAGE, 0);
			}
		}
		else if(pMsgBoxResult->strID == "DST_DOJO_ADD_TENKAI_SEED")
		{
			if( MBR_OK == pMsgBoxResult->eResult )
			{
				const WCHAR* pwcName = pMsgBoxResult->pData->pwcText;

				if (!pwcName)
				{
					GetAlarmManager()->AlarmMessage("DST_LOBBY_EMPTYNAME");
					return;
				}

				RwInt32 iLength = wcslen(pwcName);

				if (iLength < NTL_MIN_SIZE_CHAR_NAME || iLength > NTL_MAX_SIZE_CHAR_NAME)
				{
					GetAlarmManager()->AlarmMessage("DST_CHARACTER_NAME_RULE");
					return;
				}

				if (GetChattingFilter()->IsAnyInvalidText(pwcName))
				{
					GetAlarmManager()->AlarmMessage("DST_LOBBY_CAN_NOT_USE_THE_WORD");
					return;
				}

				GetDboGlobal()->GetChatPacketGenerator()->SendDojo_BudokaiSeedAddReq(pwcName);
			}
		}
		//case MBW_DOGI_ASK_APPLY_CHANGE:
		//	{
		//		if( MBR_OK == pMsgBoxResult->eResult )
		//		{
		//			CNtlSobAvatar*	pAvatar	= GetNtlSLGlobal()->GetSobAvatar();
		//			CNtlGuild*		pGuild	= pAvatar->GetGuild();
		//			sDBO_DOGI_DATA	tDogiData;

		//			tDogiData.guildId		= pGuild->GetGuildID();
		//			tDogiData.byType		= INVALID_BYTE;
		//			tDogiData.byGuildColor	= pMsgBoxResult->pData->byIndex;
		//			tDogiData.byDojoColor	= pGuild->GetDojoDogiColorIndex();

		//			if( pGuild->IsChangedGuildDogiColor() )
		//			{
		//				GetDboGlobal()->GetGamePacketGenerator()->SendGuildDogi_ChagneReq(pMsgBoxResult->pData->hHandle, &tDogiData);						
		//			}
		//			else
		//			{
		//				GetDboGlobal()->GetGamePacketGenerator()->SendGuildDogi_CreateReq(pMsgBoxResult->pData->hHandle, &tDogiData);
		//			}
		//		}
		//		else
		//		{
		//			CDboEventGenerator::EnableItemIcon(TRUE, PLACE_DOGI, 0);
		//		}
		//		break;
		//	}
		//case MBW_DOJO_DOGI_ASK_APPLY_CHANGE:
		//	{
		//		if( MBR_OK == pMsgBoxResult->eResult )
		//		{
		//			CNtlSobAvatar*	pAvatar	= GetNtlSLGlobal()->GetSobAvatar();
		//			CNtlGuild*		pGuild	= pAvatar->GetGuild();
		//			sDBO_DOGI_DATA	tDogiData;

		//			tDogiData.guildId		= pGuild->GetGuildID();
		//			tDogiData.byType		= INVALID_BYTE;
		//			tDogiData.byGuildColor	= pGuild->GetGuildDogiColorIndex();
		//			tDogiData.byDojoColor	= pMsgBoxResult->pData->byIndex;

		//			if( pGuild->IsChangedDojoDogiColor() )
		//			{
		//				GetDboGlobal()->GetGamePacketGenerator()->SendDogi_ChagneReq(pMsgBoxResult->pData->hHandle, &tDogiData);
		//			}
		//			else
		//			{
		//				GetDboGlobal()->GetGamePacketGenerator()->SendDogi_CreateReq(pMsgBoxResult->pData->hHandle, &tDogiData);
		//			}					
		//		}
		//		else
		//		{
		//			CDboEventGenerator::EnableItemIcon(TRUE, PLACE_DOGI, 0);
		//		}
		//		break;
		//	}
		else if (pMsgBoxResult->strID == "DST_DOJO_SCRAMBLE_ACCEPT_CHALLENGE")
		{
			if (MBR_OK == pMsgBoxResult->eResult)
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendDojo_ScrambleResponseReq(true);
			}
			else
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendDojo_ScrambleResponseReq(false);
			}
		}
		else if(pMsgBoxResult->strID == "DST_DOJO_SCRAMBLE_ASK_CHALLENGE")
		{
			if( MBR_OK == pMsgBoxResult->eResult )
			{
				SERIAL_HANDLE hTarget = Logic_GetAvatarTargetHandle();
				GetDboGlobal()->GetGamePacketGenerator()->SendDojo_ScrambleReq(hTarget);
			}
		}
		else if(pMsgBoxResult->strID == "DST_DOJO_SCRAMBLE_ASK_TELEPORT")
		{
			if( MBR_OK == pMsgBoxResult->eResult )
			{
				CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
				if( !pAvatar )
				{
					DBO_FAIL("Not exist avatar instance");
					return;
				}

				CNtlDojo*		pDojo			= pAvatar->GetDojo();
				sSCRAMBLE_INFO*	pSCRAMBLE_INFO	= pDojo->GetScramble();

				if (TELEPORT_TYPE_INVALID == pSCRAMBLE_INFO->byTeleportProposalIndex)
				{
					DBO_WARNING_MESSAGE("byTeleportProposalIndex is INVALID");
					return;
				}

				GetDboGlobal()->GetGamePacketGenerator()->SendTeleportConfirmReq( true, pSCRAMBLE_INFO->byTeleportProposalIndex );
			}
		}
		//case MBW_HOIPOIMIX_SKILL_SET_MSGBOX:
		//	{
		//		if( pMsgBoxResult->eResult == MBR_OK )
		//		{
		//			GetDboGlobal()->GetGamePacketGenerator()->SendHoipoiMixJobSetReq( pMsgBoxResult->pData->hExtendHandle, pMsgBoxResult->pData->byIndex );
		//		}
		//	}
		//	break;
		//case MBW_HOIPOIMIX_SKILL_RESET_MSGBOX:
		//	{
		//		 if( pMsgBoxResult->eResult == MBR_OK )
		//		 {
		//			GetDboGlobal()->GetGamePacketGenerator()->SendHoipoiMixJobResetReq( pMsgBoxResult->pData->hExtendHandle, pMsgBoxResult->pData->byIndex );
		//		 }
		//	}
		else if(pMsgBoxResult->strID == "DST_LOBBY_ASK_RENAME")
		{
			if( MBR_OK != pMsgBoxResult->eResult )
				return;

			const WCHAR* pwcText = pMsgBoxResult->pData->pwcText;

			if( FALSE == Logic_IsUsableCharacterName_with_WarningMessage(pwcText) )
				return;

			SERVER_HANDLE	hServer	= GetLobbyManager()->GetSelectedServerHandle();
			CLobby*			pLobby	= GetLobbyManager()->GetLobby(hServer);
			if( !pLobby )
			{
				DBO_FAIL("Not exist lobby infomation of server handle : " << hServer);
				return;
			}

			sLOBBY_CHARACTER* pLOBBY_CHARACTER = pLobby->GetCharacter( pLobby->GetSelectedCharacterIndex() );
			if( !pLOBBY_CHARACTER )
			{
				DBO_FAIL("Not exist character of index : " << pLobby->GetSelectedCharacterIndex());
				return;
			}


			CHARACTERID charID = pLOBBY_CHARACTER->tSummary.charId;

			GetDboGlobal()->GetLobbyPacketGenerator()->SendCharRenameReq(charID, GetACP(), (WCHAR*)pwcText);
			GetCharStageState()->ChangeState(CHAR_STATE_SELECT_RENAME_REQ);
			GetLobbyManager()->SetReservedRename(pwcText);
		}
		else if (pMsgBoxResult->strID == "DST_SEALING_EXTRACT_CONFIRM_BOX")
		{
			if (pMsgBoxResult->eResult == MBR_OK)
			{
				GetDboGlobal()->GetGamePacketGenerator()->SendItemSealExtractReq((RwUInt8)pMsgBoxResult->pData->sItemInfo.nPlaceIdx, (RwUInt8)pMsgBoxResult->pData->sItemInfo.nSlotIdx);
			}
		}
		else if (pMsgBoxResult->strID == "DST_REVERTED_MB_CONFIRM_ACTIVATE")
		{
			if (pMsgBoxResult->eResult == MBR_OK)
			{
				sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>(pMsgBoxResult->pData);

				if (pMsgBoxData->sMoveEquipInfo.bBank) // move item from bank to equipment
				{
					if (GetDboGlobal()->GetGamePacketGenerator()->SendBankMove(pMsgBoxData->sMoveEquipInfo.hNPCHandle,
						pMsgBoxData->sMoveEquipInfo.bySrcPlace, pMsgBoxData->sMoveEquipInfo.bySrcPos, CONTAINER_TYPE_EQUIP, pMsgBoxData->sMoveEquipInfo.byDestPos) == false)
					{
						GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
					}
				}
				else if (pMsgBoxData->sMoveEquipInfo.bGuildBank) // move item from guild bank to equipment
				{
					if (GetDboGlobal()->GetGamePacketGenerator()->SendGuildWarehouseMoveReq(pMsgBoxData->sMoveEquipInfo.hNPCHandle,
						pMsgBoxData->sMoveEquipInfo.bySrcPlace, pMsgBoxData->sMoveEquipInfo.bySrcPos, CONTAINER_TYPE_EQUIP, pMsgBoxData->sMoveEquipInfo.byDestPos) == false)
					{
						GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
					}
				}
				else // move item from inventory to equipment
				{
					if (GetDboGlobal()->GetGamePacketGenerator()->SendItemMoveReq(pMsgBoxData->sMoveEquipInfo.bySrcPlace, pMsgBoxData->sMoveEquipInfo.bySrcPos,
						CONTAINER_TYPE_EQUIP, pMsgBoxData->sMoveEquipInfo.byDestPos) == false)
					{
						GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
					}
				}
			}
		}
		else if (pMsgBoxResult->strID == "DST_NPC_SKILL_POINT_RESET_MSG")
		{
			if (pMsgBoxResult->eResult != MBR_OK)
				return;

			sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>(pMsgBoxResult->pData);

			if (GetDboGlobal()->GetGamePacketGenerator()->SendSkillInitReq(pMsgBoxData->hHandle) == false)
			{
				GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
			}
		}
		else if (pMsgBoxResult->strID == "DST_COMMERCIAL_SKILL_POINT_RESET_PLUS_MSG")
		{
			if (pMsgBoxResult->eResult != MBR_OK)
				return;

			sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>(pMsgBoxResult->pData);

			if (GetDboGlobal()->GetGamePacketGenerator()->SendSkillResetPlusReq((BYTE)pMsgBoxData->sItemInfo.nPlaceIdx, (BYTE)pMsgBoxData->sItemInfo.nSlotIdx) == false)
			{
				GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
			}
		}
		else if (pMsgBoxResult->strID == "DST_ITEM_BEAD_DESTROY_CONFIR_MESSAGE")
		{
			if (pMsgBoxResult->eResult != MBR_OK)
				return;

			sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>(pMsgBoxResult->pData);

			if(GetDboGlobal()->GetGamePacketGenerator()->SendItemSocketDestroyBeadReq((BYTE)pMsgBoxData->sItemInfo.nPlaceIdx, (BYTE)pMsgBoxData->sItemInfo.nSlotIdx) == false)
			{
				GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
			}
		}
		else if (pMsgBoxResult->strID == "DST_NPCSHOP_ITEMSWAP_BUY_CONFIRM")
		{
			if (pMsgBoxResult->eResult != MBR_OK)
				return;

			sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>(pMsgBoxResult->pData);

			if(GetDboGlobal()->GetGamePacketGenerator()->SendItemExchangeReq(pMsgBoxData->hExtendHandle, pMsgBoxData->sShopBuy.byItemPos, pMsgBoxData->sShopBuy.byMerchantTab, pMsgBoxData->sShopBuy.byStack) == false)
			{
				GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
			}
		}
		else if (pMsgBoxResult->strID == "DST_NPCSHOP_FREE_ITEMSWAP_BUY_CONFIRM")
		{
			if (pMsgBoxResult->eResult != MBR_OK)
				return;

			sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>(pMsgBoxResult->pData);

			if (GetDboGlobal()->GetGamePacketGenerator()->SendItemExchangeReq(pMsgBoxData->hExtendHandle, pMsgBoxData->sShopBuy.byItemPos, pMsgBoxData->sShopBuy.byMerchantTab, pMsgBoxData->sShopBuy.byStack) == false)
			{
				GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
			}
		}
		else if (pMsgBoxResult->strID == "DST_PORTAL_CONFIRM")
		{
			if (pMsgBoxResult->eResult != MBR_OK)
				return;

			sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>(pMsgBoxResult->pData);

			if (GetDboGlobal()->GetGamePacketGenerator()->SendPortalReq(pMsgBoxData->hHandle, pMsgBoxData->byIndex) == false)
			{
				GetAlarmManager()->AlarmMessage("DST_ITEM_CAN_NOT_MOVE_ERROR_PACKET_SEND");
			}
		}
		else if (pMsgBoxResult->strID == "DST_YADRAT_MB_GET_ITEM")
		{
			if (pMsgBoxResult->eResult != MBR_OK)
				return;

			sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>(pMsgBoxResult->pData);

			GetDboGlobal()->GetGamePacketGenerator()->SendCashItemMoveReq(pMsgBoxData->uiParam);
		}
		else if (pMsgBoxResult->strID == "DST_YADRAT_MB_ITEM_USE_HLS_ITEM_TYPE_DEFAULT")
		{
			if (pMsgBoxResult->eResult != MBR_OK)
				return;

			sMsgBoxData* pMsgBoxData = reinterpret_cast<sMsgBoxData*>(pMsgBoxResult->pData);

			GetDboGlobal()->GetGamePacketGenerator()->SendCashItemUseReq(pMsgBoxData->uiParam);
		}
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pData = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );

		if (pData->uiState == WORLD_STATE_EXIT)
		{
			switch (pData->eWorldConcept)
			{
				case WORLD_PLAY_TUTORIAL:
				case WORLD_PLAY_PARTY_DUNGEON:
				case WORLD_PLAY_TLQ:
				{
					DeleteMsgBox("DST_NPC_SERVER_MSG_SHUTDOWN_NOTICE");
				}
				break;

				case WORLD_PLAY_TIME_MACHINE:
				{
					DeleteMsgBox("DST_REGEN_PLAYER_TMQ");
				}
				break;

				case WORLD_PLAY_CCBD:
				{
					DeleteMsgBox("DST_CCBD_LEAVE_CCBD");
				}
				break;
			}
		}
	}
	else if( msg.Id == g_EventGameServerChangeOut )
	{
		DeleteAllMsgBoxExceptNetConnectBox();
	}
	else if( msg.Id == g_EventSobRevivalNotify )
	{
		GetMsgBoxManager()->DeleteMsgBox("DST_REGEN_PLAYER_MAINWORLD_WITH_POPO");
		GetMsgBoxManager()->DeleteMsgBox( "DST_REGEN_PLAYER_MAINWORLD" );
		GetMsgBoxManager()->DeleteMsgBox( "DST_REGEN_PLAYER_TLQ" );	
		GetMsgBoxManager()->DeleteMsgBox( "DST_REGEN_PLAYER_TMQ" );
		GetMsgBoxManager()->DeleteMsgBox( "DST_REGEN_PLAYER_PARTY_DUNGEON" );
		GetMsgBoxManager()->DeleteMsgBox( "DST_REGEN_PLAYER_DOJO" );
		GetMsgBoxManager()->DeleteMsgBox("DST_REGEN_PLAYER_CCBD");
	}	
	else if( msg.Id == g_EventResize )
	{
		SNtlPLEventResize* pPacket = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		
		LIST_MSGBOX_ITER iter;

		for( iter = m_listMsgBox.begin() ; iter != m_listMsgBox.end() ; ++iter )
		{
			CMsgBoxGui* pMsgBox = (*iter);
			pMsgBox->PositionAlign( pPacket->iWidht, pPacket->iHeight );
		}

		for( iter = m_listPopupMsgBox.begin() ; iter != m_listPopupMsgBox.end() ; ++iter )
		{
			CMsgBoxGui* pMsgBox = (*iter);
			pMsgBox->PositionAlign( pPacket->iWidht, pPacket->iHeight );
		}	
	}
}

VOID CMsgBoxManager::LinkEvent(VOID)
{
	// Event Link
	LinkMsg( g_EventMsgBoxShow );
	LinkMsg( g_EventMsgBoxResult );	
	LinkMsg( g_EventChangeWorldConceptState );
	LinkMsg( g_EventSobRevivalNotify );
	LinkMsg( g_EventGameServerChangeOut );
	LinkMsg( g_EventResize );
}

VOID CMsgBoxManager::UnLinkEvent(VOID)
{
	// Event Unlink
	UnLinkMsg( g_EventMsgBoxShow );
	UnLinkMsg( g_EventMsgBoxResult );
	UnLinkMsg( g_EventChangeWorldConceptState );
	UnLinkMsg( g_EventSobRevivalNotify );
	UnLinkMsg( g_EventGameServerChangeOut );
	UnLinkMsg( g_EventResize );
}