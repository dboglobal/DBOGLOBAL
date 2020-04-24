#include "precomp_dboclient.h"
#include "DialogManager.h"

// core
#include "NtlDebug.h"

// sound
#include "GUISoundDefine.h"

// gui
#include "gui_component.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"
#include "NtlPLEvent.h"
#include "NtlPLCinematicObject.h"

// simulation
#include "InputActionMap.h"
#include "NtlSob.h"
#include "NtlSobNpc.h"
#include "NtlSLLogic.h"
#include "NtlInventory.h"
#include "NtlSLGlobal.h"
#include "NtlSobAvatar.h"
#include "NtlSobManager.h"
#include "NtlWorldConcept.h"	
#include "NtlWorldConceptNPCCommu.h"
#include "NtlSLEvent.h"
#include "NtlSobProxy.h"

// dbo
#include "GameStage.h"
#include "StageManager.h"
#include "AlarmManager.h"
#include "CursorManager.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "MinimapGui.h"
#include "WorldMap.h"
#include "MultiDialogGui.h"
#include "PetStatusBarGui.h"
#include "PetSkillWindowGui.h"
#include "AltarGui.h"
#include "DBCRewardGUI.h"
#include "ChatBalloonManager.h"
#include "SideDialogManager.h"
#include "InfoWndManager.h"
#include "PopupManager.h"
#include "DiceManager.h"


namespace
{
	#define dMOVALBE_DEFAULT_POSITION_GAP			(15)
};


CDialogManager* CDialogManager::m_pInstance = NULL;


CDialogManager* CDialogManager::GetInstance()
{
	return m_pInstance;
}

CDialogManager::CDialogManager()
:m_eMode(DIALOGMODE_UNKNOWN)
,m_iOldWidth(0)
,m_iOldHeight(0)
,m_bExpectingCloseNPCDialog(FALSE)
,m_bESCSkip(FALSE)
,m_bDialogMovable(FALSE)
,m_bCanVisible_OnOberserver(FALSE)
, m_pClickedItem(NULL)
{	
	NTL_PRE(m_pInstance == 0);
}

CDialogManager::~CDialogManager()
{	
	m_OpenedTarget.pOpenedTarget = NULL;	
}

VOID CDialogManager::CreateInstance(gui::CGUIManager* pGuiManager)
{
	NTL_FUNCTION("CDialogManager::CreateInstance");

	m_pInstance = NTL_NEW CDialogManager;
	NTL_ASSERT( m_pInstance, "CDialogManager::CreateInstance : Instance Allocate Fail" );

	m_pInstance->m_mapDialog.clear();
	m_pInstance->m_listRaiseDialogbyUser.clear();

	m_pInstance->m_OpenedTarget.pOpenedTarget = NULL;

	m_pInstance->m_iRegularStrartY = 0;

	m_pInstance->RegisteRegularDialog();
	m_pInstance->RegisteEntireFocusingDialog();

	// by Kell : 액션별로 다이얼로그를 처리한다.
	GetInputActionMap()->LinkDialogHotKey( m_pInstance, &CDialogManager::HandleDialogAction );

	// create side dialog manager
	CSideDialogManager::CreateInstance();
	GetDialogManager()->RegistDialog(DIALOG_SIDEDIALOG_MANAGER, GetSideDialogManager(), &CSideDialogManager::SwitchDialog );

	// 마우스 처리를 위해
	m_pInstance->m_slotMouse = pGuiManager->SigCaptureMouseDown().Connect( m_pInstance, &CDialogManager::HandleMouse );

	// Link 
	m_pInstance->LinkMsg(g_EventResize);
	m_pInstance->LinkMsg(g_EventDialog);
	m_pInstance->LinkMsg(g_EventChangeWorldConceptState);
	m_pInstance->LinkMsg(g_EventShowTutorialNfy);
	m_pInstance->LinkMsg(g_EventDialogMove);
	m_pInstance->LinkMsg(g_EventNPCCommuExit);

	NTL_RETURNVOID();	
}

VOID CDialogManager::DestoryInstance()
{
	NTL_FUNCTION("CDialogManager::DestoryInstance");

	CSideDialogManager::DestoryInstance();

	DIALOG_MAP_ITER it = m_pInstance->m_mapDialog.begin();
	for( ; it != m_pInstance->m_mapDialog.end() ; ++it )
	{
		NTL_DELETE(it->second.pCallSwitch);
	}

	m_pInstance->m_mapDialog.clear();
	m_pInstance->m_listRaiseDialogbyUser.clear();
	m_pInstance->m_mapEntireFocuing.clear();
	m_pInstance->m_listRequestedOpenDialogInNarration.clear();

	GetInputActionMap()->UnLinkDialogHotKey();

	// UnLink
	m_pInstance->UnLinkMsg(g_EventResize);
	m_pInstance->UnLinkMsg(g_EventDialog);
	m_pInstance->UnLinkMsg(g_EventChangeWorldConceptState);
	m_pInstance->UnLinkMsg(g_EventShowTutorialNfy);	
	m_pInstance->UnLinkMsg(g_EventDialogMove);
	m_pInstance->UnLinkMsg(g_EventNPCCommuExit);

	NTL_DELETE(m_pInstance);

	NTL_RETURNVOID();
}

VOID CDialogManager::Update(RwReal fElapsed)
{
	if( !m_pInstance )
		return;

	m_bESCSkip = FALSE;

	// Close the NPC dialog when it is more than a distance away from the target that opened the NPC window
	if( m_OpenedTarget.pOpenedTarget )
	{
		CNtlSobAvatar* pAvatar = reinterpret_cast<CNtlSobAvatar*>( GetNtlSLGlobal()->GetSobAvatar() );
		if( FALSE == Logic_InFollowRange(pAvatar, m_OpenedTarget.v3Pos, NTL_NPC_COMMUNITY_OUT_RANGE ) )
		{
			NTL_ASSERT(GetOpendNPCDIalog() != DIALOG_UNKNOWN, "CDialogManager::Update, Any npc dialog not opened");
			CDboEventGenerator::DialogEvent(DIALOGEVENT_NPC_BYEBYE, DIALOG_UNKNOWN, GetOpendNPCDIalog());
			m_OpenedTarget.pOpenedTarget = NULL;
		}
	}	

	// 일부 창들은 서버로 부터 닫기 응답이 와야지만 닫힌다.
	// 따라서 DialogMananger의 Update에서 창이 닫힌 이후에 멀티 다이얼로그를 열어준다
	if( m_bExpectingCloseNPCDialog )
	{
		RwInt32 iDialog = GetOpendNPCDIalog();
		if( iDialog == DIALOG_UNKNOWN )
		{
			m_bExpectingCloseNPCDialog = FALSE;

			CMultiDialogGui* pGui = reinterpret_cast<CMultiDialogGui*>( GetDialog(DIALOG_MULTIDIALOG) );
			OpenDialog(DIALOG_MULTIDIALOG, pGui->GetTargetSerial() );
		}		
	}

	GetSideDialogManager()->Update(fElapsed);
}

VOID CDialogManager::UnRegistDialog(int iDialog)
{
	DialogInfo* pDialogInfo = GetDialogInfo(iDialog);
	if(!pDialogInfo)
		return;

	LIST_OPENED_REGULAR_ITER it_OpenedRegularDilaog = m_listOpenedRegularDilaog.begin();
	for( ; it_OpenedRegularDilaog != m_listOpenedRegularDilaog.end() ; ++it_OpenedRegularDilaog )
	{
		if( *it_OpenedRegularDilaog == iDialog )
		{
			m_listOpenedRegularDilaog.erase(it_OpenedRegularDilaog);
			break;
		}
	}

	DIALOG_RAISE::iterator it_Raise = m_listRaiseDialogbyUser.begin();
	for( ; it_Raise != m_listRaiseDialogbyUser.end() ; ++it_Raise )
	{
		if( *it_Raise == iDialog )
		{
			m_listRaiseDialogbyUser.erase(it_Raise);
			break;
		}
	}	

	NTL_DELETE(pDialogInfo->pCallSwitch);
	m_mapDialog.erase(iDialog);

	if( IsEFDialog((eDialogType)iDialog) )
	{
		if( IsOpenEFDialog() == FALSE )
			CloseDialog(DIALOG_BACKBOARD);
	}
}

VOID CDialogManager::OpenGMDialog()
{
	for(RwInt32 i = DIALOG_GM_FIRST ; i <= DIALOG_GM_LAST ; ++i )
	{
		DialogInfo* pDialogInfo = GetDialogInfo(i);
		if(pDialogInfo)
			OpenDialog(i, INVALID_SERIAL_ID, FALSE);
	}
}

VOID CDialogManager::OpenDefaultDialog()
{
	GetSideDialogManager()->OpenDefaultDialog();

	// all side dialogs that can be opened are opened and go to the side dialog control
	// Must open
	for(RwInt32 i = DIALOG_DEFAULT_FIRST ; i <= DIALOG_DEFAULT_LAST ; ++i )
	{
		DialogInfo* pDialogInfo = GetDialogInfo(i);
		if(pDialogInfo)
			OpenDialog(i, INVALID_SERIAL_ID, FALSE);
	}

	// Pet 이 있을 경우 펫 관련 Dialog 들도 Default Dialog와 같은 방식으로 열어준다.
	 if( CPetStatusBarGui::GetInstance()->GetPetID() != INVALID_SERIAL_ID )
		 OpenDialog( DIALOG_PET_STATUSBAR );

	 if( CPetSkillWindowGui::GetInstance()->GetInfoFlag() )
		 OpenDialog( DIALOG_PET_SKILL_SLOT );

}

VOID CDialogManager::CloseGMDialog()
{
	for(RwInt32 i = DIALOG_GM_FIRST ; i <= DIALOG_GM_LAST ; ++i )
		CloseDialog(i, FALSE);
}

VOID CDialogManager::CloseNotDefaultDialog()
{
	CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_EXIT, Logic_GetAvatarHandle());
	GetDiceManager()->CancelAll();

	for(RwInt32 i = DIALOG_EVENT_FIRST ; i <= DIALOG_LAST ; ++i )
		CloseDialog(i, FALSE);
}


VOID CDialogManager::CloseAll()
{
	// 서버로 부터 패킷을 받아 닫히는 dialog를 위해서 컨셉을 미리 종료한다
	CNtlWorldConceptController* pWorldConcept = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_SECOND_GRADE);
	
	if( pWorldConcept )
	{
		switch( pWorldConcept->GetConceptType() )
		{
		case WORLD_PLAY_NPC_COMMU:
			{
				CNtlWorldConceptNPCCOMMU* pWorldConceptNPCCOMMU = reinterpret_cast<CNtlWorldConceptNPCCOMMU*>( pWorldConcept );
				pWorldConceptNPCCOMMU->ChangeState(WORLD_STATE_EXIT);
				break;
			}
		case WORLD_PLAY_TRADE:
			{
				pWorldConcept->ChangeState(WORLD_STATE_EXIT);
				break;
			}
		}
	}

	CNtlSLEventGenerator::ScouterEvent(SCOUTER_EVENT_EXIT, Logic_GetAvatarHandle());
	GetDiceManager()->CancelAll();

	for(RwInt32 i = DIALOG_FIRST ; i <= DIALOG_LAST ; ++i )
	{
		if( IsNPCDialog( i ) && IsOpenDialog( i ) )
			CDboEventGenerator::DialogEvent(DIALOGEVENT_NPC_BYEBYE, DIALOG_UNKNOWN, i);

		CloseDialog(i, FALSE);
	}

	GetSideDialogManager()->CloseAll();
}

VOID CDialogManager::SwitchBag(RwBool bOpen)
{
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	for( RwInt32 i = DIALOG_CAPSULE_1 ; i <=  DIALOG_CAPSULE_NETPY ; ++i )
	{
		if( pInventory->GetBagItem(i - DIALOG_CAPSULE_1) == INVALID_SERIAL_ID )
			continue;

		if(bOpen)
			OpenDialog(i, INVALID_SERIAL_ID, FALSE);
		else
			CloseDialog(i, FALSE);
	}

	// 연속해서 소리가 5번 나는 것을 방지하기 위해
	if( bOpen )
		PlayOpenSound(DIALOG_CAPSULE_1);
	else
		PlayCloseSound(DIALOG_CAPSULE_1);
}

/**
* \brief Capsult kit를 Index로 Switch Dialog한다.
*/
VOID CDialogManager::SwitchBagByIndex( RwInt32 nIndex )
{
	if( nIndex < 0 && nIndex >=  DIALOG_CAPSULE_NETPY-DIALOG_CAPSULE_1 )
	{
		NTL_ASSERTFAIL( "CDialogManager::SwitchBag - Invalid bag index" );
		return;
	}

	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();

	// 없는 Capsule kti라면 아무런 수행을 하지 않는다.
	if( pInventory->GetBagItem( nIndex ) == INVALID_SERIAL_ID )
		return;

	// SwitchDialog
	SwitchDialog( DIALOG_CAPSULE_1+nIndex );
}

VOID CDialogManager::RaiseDialogbyUser(RwInt32 iDialog)
{
	if( IsRegularDialog((eDialogType)iDialog) )
	{
		if( IsHaveAttribute((eDialogType)iDialog, dRDA_NOT_ESC) )
			return;
	}
	else
	{
		if( iDialog <= DIALOG_EVENT_LAST )
			return;

		if( iDialog >= DIALOG_NPCDIALOG_FIRST )
		{
			if( iDialog != DIALOG_MULTIDIALOG && iDialog != DIALOG_ITEM_OPTIONRESTRUCTURE && iDialog != DIALOG_QUICK_TELEPORT)
				return;
		}			
	}


	DIALOG_RAISE::iterator it = m_listRaiseDialogbyUser.begin();
	for( ; it != m_listRaiseDialogbyUser.end() ; ++it )
	{
		if( iDialog == *it )
		{
			// 기존에 이미 있다면 가장 뒤로..
			m_listRaiseDialogbyUser.erase(it);
			m_listRaiseDialogbyUser.push_back(iDialog);
			return;
		}
	}

	// 새로운 다이얼로그가 화면에 나타난다면
	m_listRaiseDialogbyUser.push_back(iDialog);
}

VOID CDialogManager::HideDialogbyUser(RwInt32 iDialog)
{
	DIALOG_RAISE::iterator it = m_listRaiseDialogbyUser.begin();
	for( ; it != m_listRaiseDialogbyUser.end() ; ++it )
	{
		if( iDialog == *it )
		{
			m_listRaiseDialogbyUser.erase(it);
			return;
		}
	}	
}

RwInt32 CDialogManager::GetOpendNPCDIalog()
{
	for( RwInt32 i = DIALOG_NPCDIALOG_FIRST ; i <= DIALOG_NPCDIALOG_LAST ; ++i )
	{
		if( IsOpenDialog(i) )
			return i;
	}

	return DIALOG_UNKNOWN;
}

CNtlSob* CDialogManager::GetOpenedTarget()
{
	return m_OpenedTarget.pOpenedTarget;
}

CDialogManager::DialogInfo* CDialogManager::GetDialogInfo(int iDialog)
{	
	DIALOG_MAP_ITER it = m_mapDialog.find(iDialog);
	if( it != m_mapDialog.end() )
		return &( it->second );

	return NULL;
}

CNtlPLGui* CDialogManager::GetDialog(int iDialog)
{	
	DIALOG_MAP_ITER it = m_mapDialog.find(iDialog);
	if( it != m_mapDialog.end() )
		return it->second.pDialog ;

	return NULL;
}

RwInt32 CDialogManager::GetTopDialog()
{
	if( m_listRaiseDialogbyUser.size() == 0 )
		return DIALOG_UNKNOWN;

	return m_listRaiseDialogbyUser.back();
}

VOID CDialogManager::HandleMouse(const CKey& key)
{
	if( IsMode(DIALOGMODE_NARRATION) || IsMode(DIALOGMODE_SKILL_SELECT))
		return;

	switch(key.m_nID)
	{
		case UD_RIGHT_BUTTON:
		{			
			OffMode();
		}
		break;
	}
}

VOID CDialogManager::OnMode(eDialogMode eMode)
{
	if( m_eMode == eMode )
		return;

	if (eMode > DIALOGMODE_UNKNOWN)
	{
		DBO_FAIL("Wrong dialog mode type : " << eMode);
		return;
	}

	m_eMode = eMode;

	switch(eMode)
	{
		case DIALOGMODE_ITEM_REPAIR:
		{
			OpenDialog(DIALOG_STATUS);
			GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_ITEM_REPAIR );
			break;
		}
		case DIALOGMODE_ITEM_IDENTIFICATION:
		{
			GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_ITEM_IDENTIFICATION );
			break;
		}
		case DIALOGMODE_NPCSHOP_ITEM_IDENTIFICATION:
		{
			GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_ITEM_IDENTIFICATION );
			break;
		}
		case DIALOGMODE_CHATTING_RESIZE_HORI:
		{
			GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_CHAT_RESIZE_HORI );
			break;
		}
		case DIALOGMODE_CHATTING_RESIZE_RIGHTUP:
		{
			GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_CHAT_RESIZE_RIGHTUP );
			break;
		}
		case DIALOGMODE_CHATTING_RESIZE_RIGHTDOWN:
		{
			GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_CHAT_RESIZE_RIGHTDOWN );
			break;
		}
		case DIALOGMODE_CHATTING_RESIZE_VERT:
		{
			GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_CHAT_RESIZE_VERT );
			break;
		}
		case DIALOGMODE_NARRATION:
		{
			GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_INVALID_CURSOR );

			// 인포윈도우, 팝업다이얼로그 hide
			GetInfoWndManager()->ShowInfoWindow( FALSE );		
			GetPopupManager()->AllPopupClose();

			// 나레이션이 흐를 때는 모든 다이얼로그를 닫는다
			CloseAll();

			// 나레이션중에는 서버말풍선만 표시한다.
			GetBalloonManager()->SetVisibleBalloonType( CBalloonGui::TYPE_SERVER );

			// 채팅창의 포커스를 지운다.
			CDboEventGenerator::ChatGuiMode( FALSE );

			// 입력된 인풋 리셋
			GetInputActionMap()->Reset();

			// Can not input key input
			
			break;
		}
		case DIALOGMODE_ITEM_DISASSEMBLE:
		{
			GetCursorManager()->SetDesignatedCursor(CCursorManager::CS_ITEM_DISASSEMBLE);
		}
		break;

		case DIALOGMODE_ITEM_BEAD:
		{
			GetCursorManager()->SetDesignatedCursor(CCursorManager::CS_ITEM_USE_DOGIBALL);
		}
		break;

		case DIALOGMODE_SKILL_SELECT:
		{
			GetCursorManager()->SetDesignatedCursor(CCursorManager::CS_SKILL_ATTACK);

			GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy()->EnablePicking(TRUE);
		}
		break;

		case DIALOGMODE_UNKNOWN:
		break;

		default:break;
	}	
}

VOID CDialogManager::OffMode()
{	
	eDialogMode eMode = m_eMode;

	m_eMode = DIALOGMODE_UNKNOWN;

	switch( eMode )
	{
		case DIALOGMODE_ITEM_REPAIR:
		case DIALOGMODE_ITEM_IDENTIFICATION:
		case DIALOGMODE_NPCSHOP_ITEM_IDENTIFICATION:
		case DIALOGMODE_CHATTING_RESIZE_HORI:
		case DIALOGMODE_CHATTING_RESIZE_RIGHTUP:
		case DIALOGMODE_CHATTING_RESIZE_RIGHTDOWN:
		case DIALOGMODE_CHATTING_RESIZE_VERT:
		case DIALOGMODE_ITEM_DISASSEMBLE:
		{
			GetCursorManager()->SetDesignatedCursor( CCursorManager::CS_INVALID_CURSOR );
		}
		break;
		case DIALOGMODE_ITEM_BEAD:
		{
			if(m_pClickedItem)
				CDboEventGenerator::EnableItemIcon(TRUE, PLACE_BAG, m_pClickedItem->GetItemSlotIdx(), m_pClickedItem->GetParentItemSlotIdx());

			SetClickedItem(NULL);
			GetCursorManager()->SetDesignatedCursor(CCursorManager::CS_INVALID_CURSOR);
		}
		break;
		case DIALOGMODE_SKILL_SELECT:
		{
			if (GetNtlSLGlobal()->IsSkillSelectMode())
				CNtlSLEventGenerator::ActionSkillSelectMode(false, INVALID_SERIAL_ID);

			GetNtlSLGlobal()->GetSobAvatar()->GetSobProxy()->EnablePicking(FALSE);

			GetCursorManager()->SetDesignatedCursor(CCursorManager::CS_INVALID_CURSOR);
		}
		break;

		case DIALOGMODE_NARRATION:
		{
			// 월드컨셉 판단은 퀘스트 인디케이터에서 한다.
			GetSideDialogManager()->OpenDialog(SDIALOG_QUEST_INDICATOR);

			// 말풍선 제한 해제
			GetBalloonManager()->SetVisibleBalloonType(CBalloonGui::ALL);

			// 나레이션 모드시 열기 요구된 창 열기
			LIST_REQUESTED_OPENDIALOG_IN_NARRATION_ITER iter;
			for (iter = m_listRequestedOpenDialogInNarration.begin(); iter != m_listRequestedOpenDialogInNarration.end(); ++iter)
			{
				sRequestedOpenDialog* pData = &(*iter);

				OpenDialog(pData->eRequestedDialogType, pData->hSerial, pData->bSound);
			}
			m_listRequestedOpenDialogInNarration.clear();

		}
		break;

		default: break;
	}
}

eDialogMode CDialogManager::GetMode()
{
	return m_eMode;
}

RwInt32 CDialogManager::HowManyOpenMaxRegular()
{
	RwInt32 iMaxOpen = dDAFAULT_REGULAR_COUNT + ((RwInt32)GetDboGlobal()->GetScreenWidth() - 1024) / dNEED_SIZE_MORE_REGULAR;
	return iMaxOpen;
}

RwInt32 CDialogManager::GetPosY_of_HalfRegular()
{
	RwInt32 iCommonDialogHeight = GetDialog(DIALOG_COMMUNITY)->GetHeight();
	RwInt32 iPosY = m_iRegularStrartY + iCommonDialogHeight/2;
	return iPosY;
}

VOID CDialogManager::Disable_OnObserver(RwBool bVisible)
{
	m_bCanVisible_OnOberserver = bVisible;
}

VOID CDialogManager::LocateBag(RwInt32 iScreenWidth, RwInt32 iScreenHeight, RwBool bForce /* = FALSE */)
{
	if( !bForce && m_bDialogMovable )
		return;

	RwInt32 iX = 0, iY = 0;
	RwUInt8 byBagCount = 0;
	CNtlPLGui* pGui;

	for( RwInt32 i = DIALOG_CAPSULE_1 ; i <=  DIALOG_CAPSULE_NETPY ; ++i )
	{
		pGui = GetDialog(i);

		if( !pGui )
			continue;

		if( !bForce && pGui->IsShow() == FALSE )
			continue;

		// 첫 번째 가방의 위치를 정한다
		if( byBagCount == 0 )
		{
			iX = iScreenWidth - dDIALOG_CLEINT_EDGE_GAP - pGui->GetWidth();
			iY = iScreenHeight - 100;
		}		

		iY -= pGui->GetHeight();

		// by daneos: To fix 32 slot bag top is not in screen
		if (iY < 0)
			iY = 0;

		pGui->SetPosition(iX, iY);

		// Upper/lower gap
		iY -= 2;


		++byBagCount;

		if( byBagCount == 2 || byBagCount == 4)
		{
			iX -= 2 + pGui->GetWidth(); // 3: Left/right gap
			iY = iScreenHeight - 100;
		}
	}
}


VOID CDialogManager::LocationDialogs(RwInt32 iWidth, RwInt32 iHeight)
{
	CNtlPLGui* pGui = NULL;
	CRectangle rect;
	RwInt32 iX = 0;
	RwInt32 iY = 0;
	RwInt32 iMinimapBottom = 0;

	pGui = GetDialog(DIALOG_MINIMAP);
	if( pGui )
	{
		iMinimapBottom = pGui->GetHeight();
		pGui->SetPosition(iWidth - (pGui->GetWidth() + 10), dDIALOG_CLEINT_EDGE_GAP);
	}	

	// 사이드 다이얼로그 컨트롤러
	pGui = GetDialog(DIALOG_SIDEDIALOG_CONTROLLER);
	if( pGui )
		pGui->SetPosition(iWidth - (pGui->GetWidth() + dDIALOG_CLEINT_EDGE_GAP), 188);

	// 사이드 다이얼로그
	pGui = GetDialog(DIALOG_SIDEDIALOG_MANAGER);
	if( pGui )
		pGui->SetPosition(iWidth - pGui->GetWidth() - dDIALOG_CLEINT_EDGE_GAP, GetDialog(DIALOG_SIDEDIALOG_CONTROLLER)->GetPosition().bottom);

	// 사이드 아이콘
	pGui = GetDialog(DIALOG_SIDEICON);
	if( pGui )
		pGui->SetPosition(iWidth, iHeight - 53);

	pGui = GetDialog(DIALOG_PETITION_SATISFACTION);
	if( pGui )
	{
		CNtlPLGui* pMinimap = GetDialog(DIALOG_MINIMAP);
		pGui->SetPosition(iWidth - pMinimap->GetWidth() - pGui->GetWidth() - 10, dDIALOG_CLEINT_EDGE_GAP );
	}

	pGui = GetDialog(DIALOG_TARGER_UI);
	if (pGui)
		pGui->SetPosition(iWidth / 2 - pGui->GetWidth() / 2 + 23, dDIALOG_CLEINT_EDGE_GAP);

	pGui = GetDialog(DIALOG_HP);
	if (pGui)
	{
		pGui->SetPosition(dDIALOG_CLEINT_EDGE_GAP + 5 , dDIALOG_CLEINT_EDGE_GAP);
		pGui->Resize(0, 0); // this is required to get the "OnMove" callback
		
	}
	
	pGui = GetDialog(DIALOG_MAINMENUBUTTON);
	if( pGui )
	{
		iX = iWidth - (pGui->GetWidth() + dDIALOG_CLEINT_EDGE_GAP);
		iY = iHeight - (pGui->GetHeight() + dDIALOG_CLEINT_EDGE_GAP);
		pGui->SetPosition(iX, iY);
	}
	
	pGui = GetDialog(DIALOG_EXP);
	if( pGui )
	{
		iY = iHeight - pGui->GetHeight() - dDIALOG_CLEINT_EDGE_GAP;
		pGui->SetPosition(iX - pGui->GetWidth() + 7, iY);
	}
	
	iX -= 38;
	iY -= 34;

	pGui = GetDialog(DIALOG_BAGSLOT);
	if( pGui )
	{
		iX -= pGui->GetWidth() + 1;
		iY -= 3;
		pGui->SetPosition(iX, iY);
	}

	pGui = GetDialog(DIALOG_MAINMENU);
	if( pGui )
		pGui->SetPosition(iWidth - 10 - pGui->GetWidth(), iY - 25 - pGui->GetHeight() );
	
	pGui = GetDialog(DIALOG_QUICKSLOT);
	if( pGui )
	{
		iX -= pGui->GetWidth() + 4;
		iY -= (pGui->GetHeight() - GetDialog(DIALOG_BAGSLOT)->GetHeight());
		pGui->SetPosition(iX, iY);
	}

	pGui = GetDialog(DIALOG_WORLDMAP);
	if( pGui )
		pGui->SetPosition(CRectangle(0, 0, iWidth, iHeight));
	
	pGui = GetDialog(DIALOG_DBC_ALTAR);
	if(pGui)
	{		 
		pGui->SetPosition((iWidth - pGui->GetWidth())/2, (iHeight - pGui->GetHeight())/2);
	}

	pGui = GetDialog(DIALOG_DBC_REWARD);
	if(pGui)
	{
		pGui->SetPosition(iWidth - 430, iHeight - 320);
	}

	pGui = GetDialog(DIALOG_PET_STATUSBAR);
    if(pGui)
    {
		pGui->SetPosition(0, 126);
    }

	pGui = GetDialog(DIALOG_PET_SKILL_SLOT);
    if(pGui)
    {
		CNtlPLGui* pQuickSlotGui = GetDialogManager()->GetDialog(DIALOG_QUICKSLOT);
		rect = pQuickSlotGui->GetPosition();

		pGui->SetPosition(rect.left, rect.top - 38);
    }

    // NPC Portal Dialog
    pGui = GetDialog(DIALOG_PORTAL);
    if(pGui)
    {
        pGui->SetPosition(iWidth/2 - pGui->GetWidth()/2, iHeight/2 - pGui->GetHeight()/2);
    }

	pGui = GetDialog(DIALOG_HLSHOP_BUY);
	if (pGui)
	{
		pGui->SetPosition(iWidth / 2 - pGui->GetWidth() / 2, iHeight / 2 - pGui->GetHeight() / 2);
	}

	pGui = GetDialog(DIALOG_HLSHOP_BUY_CONFIRM);
	if (pGui)
	{
		pGui->SetPosition(iWidth / 2 - pGui->GetWidth() / 2, iHeight / 2 - pGui->GetHeight() / 2);
	}

	pGui = GetDialog(DIALOG_HLSHOP_GIFT);
	if (pGui)
	{
		pGui->SetPosition(iWidth / 2 - pGui->GetWidth() / 2, iHeight / 2 - pGui->GetHeight() / 2);
	}

	pGui = GetDialog(DIALOG_HLSHOP_GIFT_CONFIRM);
	if (pGui)
	{
		pGui->SetPosition(iWidth / 2 - pGui->GetWidth() / 2, iHeight / 2 - pGui->GetHeight() / 2);
	}

	pGui = GetDialog(DIALOG_AUCTION_HOUSE);
	if (pGui)
	{
		pGui->SetPosition(iWidth / 2 - pGui->GetWidth() / 2, iHeight / 2 - pGui->GetHeight() / 2);
	}

	pGui = GetDialog(DIALOG_CCBD_BOARD);
	if (pGui)
	{
		pGui->SetPosition(iWidth / 2 - pGui->GetWidth() / 2, iHeight / 2 - pGui->GetHeight() / 2);
	}

	pGui = GetDialog(DIALOG_INTERFACE_SHAKE);
	if( pGui )
		pGui->SetPosition( (iWidth/2) - (pGui->GetWidth()/2), (iHeight/2) + 100 );

    // Commu Target
    pGui = GetDialog(DIALOG_COMMU_TARGET);
    if(pGui)
        pGui->SetPosition(iWidth / 2  - pGui->GetWidth() / 2 , 3);

    pGui = GetDialog(DIALOG_TELECAST);
    if(pGui)
    {
        pGui->SetPosition(iWidth - pGui->GetWidth(), pGui->GetPosition().top);
    }

	pGui = GetDialog(DIALOG_TUTORIAL_MESSAGE);
	if(pGui)
	{
		rect.SetRectWH(0, 0, iWidth, iHeight);
		pGui->SetPosition(rect);
	}

	pGui = GetDialog(DIALOG_BUDOKAI_NEWS);
	if(pGui)
		pGui->SetPosition( (iWidth-pGui->GetWidth())/2, (iHeight-pGui->GetHeight())/2 );

	pGui = GetDialog(DIALOG_BUDOKAI_REQUEST);
	if(pGui)
		pGui->SetPosition( (iWidth-pGui->GetWidth())/2, (iHeight-pGui->GetHeight())/2 );

	pGui = GetDialog(DIALOG_BUDOKAI_TOURNAMENT);
	if(pGui)
		pGui->SetPosition( (iWidth-pGui->GetWidth())/2, (iHeight-pGui->GetHeight())/2 );
	
	// Dice
	pGui = GetDialog(DIALOG_DICE_MANAGER);
	if( pGui )
	{
		rect = GetDialog(DIALOG_QUICKSLOT)->GetPosition();
		pGui->SetPosition(rect.left, rect.top - rect.GetHeight() - 4); // 펫 스킬 창 위쪽으로
	}	

	pGui = GetDialog(DIALOG_CHANNGEL_CHANGE);
	if( pGui )
		pGui->SetPosition((iWidth - pGui->GetWidth())/2, (iHeight - pGui->GetHeight())/2 );

	pGui = GetDialog(DIALOG_VEHICLE);
	if( pGui )
	{		
		pGui->SetPosition((iWidth - pGui->GetWidth())/2, iHeight - 190);
	}	

	pGui = GetDialog(DIALOG_PLAYER_TITLE);
	if (pGui)
		pGui->SetPosition((iWidth - pGui->GetWidth()) / 2, (iHeight - pGui->GetHeight()) / 2);


	///// GM gui //////////////////////////////////////////////////////////////////////////

	pGui = GetDialog(DIALOG_GM_QUICKSLOT);
	if( pGui )
		pGui->SetPosition((iWidth - pGui->GetWidth())/2, iHeight - pGui->GetHeight() - dDIALOG_CLEINT_EDGE_GAP);


	///// for regulear dialog //////////////////////////////////////////////////////////////////////////

	pGui = GetDialog(DIALOG_COMMUNITY);
	if( pGui )
		m_iRegularStrartY = (iHeight - pGui->GetHeight() )/2;


	///// Movable or not //////////////////////////////////////////////////////////////////////////
	
	if( m_bDialogMovable )
		Locate_UserPosition(iWidth, iHeight);		
	else
		Locate_AutoPosition(iWidth, iHeight);


	///// Entire Focusing Dialog //////////////////////////////////////////////////////////////////////////
	
	FE_MAP_ITER it_Entire = m_mapEntireFocuing.begin();
	for( ; it_Entire != m_mapEntireFocuing.end() ; ++it_Entire )
	{
		pGui = GetDialog(it_Entire->first);
		if(pGui)
		{
			pGui->SetPosition((iWidth - pGui->GetWidth())/2, (iHeight - pGui->GetHeight())/2);
		}
	}

	m_iOldWidth		= iWidth;
	m_iOldHeight	= iHeight;
}

VOID CDialogManager::Locate_AutoPosition(RwInt32 iWidth, RwInt32 iHeight)
{
	CNtlPLGui* pGui;
	CRectangle rect;
	RwInt32 iX;	

	// 가방
	LocateBag(iWidth, iHeight);

	// Regular Dialog
	RwInt32 iNeedCloseRegularCount = m_listOpenedRegularDilaog.size() - HowManyOpenMaxRegular();
	if( iNeedCloseRegularCount > 0 )
	{
		// 클라이언트의 폭이 줄어들어서 열려있는 레귤러 다이얼로그를 닫아야 하는 경우이다
		// 혹은 다이얼로그 m_bMovable이 TRUE에서 FALSE로 바뀌얼을 때이다
		LIST_OPENED_REGULAR_DIALOG tempList = m_listOpenedRegularDilaog;
		LIST_OPENED_REGULAR_ITER it_temp = tempList.begin();

		for( ; it_temp != tempList.end() ; ++it_temp )
		{
			if( IsHaveAttribute(*it_temp, dRDA_EXCLUSIVE) == FALSE && CloseDialog(*it_temp) )
			{
				--iNeedCloseRegularCount;

				if( iNeedCloseRegularCount == 0 )
					break;
			}
		}
	}

	iX = dREGULARDIALOG_FIRST_X;
	LIST_OPENED_REGULAR_ITER it_OpenedRegularDilaog = m_listOpenedRegularDilaog.begin();
	for( ; it_OpenedRegularDilaog != m_listOpenedRegularDilaog.end() ; ++it_OpenedRegularDilaog )
	{
		eDialogType dialogType = *it_OpenedRegularDilaog;

		pGui = GetDialog(dialogType);


		if( IsHaveAttribute(dialogType, dRDA_HALFSIZE) )
			pGui->SetPosition(iX, GetPosY_of_HalfRegular());
		else
			pGui->SetPosition(iX, m_iRegularStrartY);


		iX = pGui->GetLinkedArea().right + dREGULARDIALOG_GAP;
	}

	// DIALOG_TUTORIAL_DIALOG_FOCUS는 다른 다이얼로그의 위치를 알아야 하니 항상 가장 나중에 옮기도록 하자
	pGui = GetDialog(DIALOG_TUTORIAL_DIALOG_FOCUS);
	if(pGui)
	{
		rect.SetRectWH(0, 0, iWidth, iHeight);
		pGui->SetPosition(rect);
	}
}

VOID CDialogManager::Locate_UserPosition(RwInt32 iWidth, RwInt32 iHeight)
{
	CNtlPLGui* pGui;
	CRectangle rect;

	// 창고와 길드 창고는 레귤러 다이얼로그이지만 Movable 기능이 활성화되어 있지 않아도
	// 레귤러 다이얼로그의 위치에 있는다
	pGui = GetDialog(DIALOG_WAREHOUSEBAR);
	if( pGui )
		pGui->SetPosition(dREGULARDIALOG_FIRST_X, m_iRegularStrartY);

	pGui = GetDialog(DIALOG_GUILD_WAREHOUSEBAR);
	if( pGui )
		pGui->SetPosition(dREGULARDIALOG_FIRST_X, m_iRegularStrartY);


	RwInt32 iAdjustX = iWidth - m_iOldWidth;
	RwInt32 iAdjustY = iHeight - m_iOldHeight;

	if( iAdjustX > 0 && iAdjustY > 0 )
		return;


	REGULAR_MAP_ITER it_Regular = m_mapRegular.begin();
	for( ; it_Regular != m_mapRegular.end() ; ++it_Regular )
	{
		pGui = GetDialog(it_Regular->first);
		if( !pGui )
			continue;

		Logic_LocateDialog_in_CleintRect(pGui);
	}
}

VOID CDialogManager::Locate_UserDefaultPosition(RwInt32 iWidth, RwInt32 iHeight)
{
	if( !m_bDialogMovable )
		return;

#define dXPOSITION_LIMIT_FROM_WIDTH			300
#define dYPOSITION_LIMIT_FROM_HEIGHT		200
#define dYPOSITION_START					100
#define dPOSITION_GAP						20

	std::list<CNtlPLGui*> listCloseRegular;
	RwInt32 iXPos = dREGULARDIALOG_FIRST_X;
	RwInt32 iYPos = dYPOSITION_START;
	CNtlPLGui* pGui;
	REGULAR_MAP_ITER it_Regular = m_mapRegular.begin();

	// 화면에 보여지고 있는 레귤러 다이얼로그의 위치 조정
	for( ; it_Regular != m_mapRegular.end() ; ++it_Regular )
	{
		eDialogType eType = it_Regular->first;

		pGui = GetDialog(eType);
		if( !pGui )
			continue;

		if( pGui->IsShow() == FALSE )
		{
			listCloseRegular.push_back(pGui);
			continue;
		}

		pGui->SetPosition(iXPos, iYPos);
		
		iXPos += dPOSITION_GAP;
		iYPos += dPOSITION_GAP;

		if( iXPos > iWidth - dXPOSITION_LIMIT_FROM_WIDTH ||
			iYPos > iHeight - dYPOSITION_LIMIT_FROM_HEIGHT )
		{
			iXPos = dREGULARDIALOG_FIRST_X;
			iYPos = dYPOSITION_START;
		}

		pGui->RaiseLinked();
		RaiseDialogbyUser(eType);
	}

	// 화면에 보이지 않는 레귤러 다이얼로그의 위치 조정
	iXPos = dREGULARDIALOG_FIRST_X;
	iYPos = dYPOSITION_START;

	std::list<CNtlPLGui*>::iterator it_close = listCloseRegular.begin();
	for( ; it_close != listCloseRegular.end() ; ++it_close )
	{
		pGui = *it_close;

		pGui->SetPosition(iXPos, iYPos);

		iXPos += dPOSITION_GAP;
		iYPos += dPOSITION_GAP;

		if( iXPos > iWidth - dXPOSITION_LIMIT_FROM_WIDTH ||
			iYPos > iHeight - dYPOSITION_LIMIT_FROM_HEIGHT )
		{
			iXPos = dREGULARDIALOG_FIRST_X;
			iYPos = dYPOSITION_START;
		}
	}
}

VOID CDialogManager::Locate_MovableDefaultPosition()
{
	// 가방
	LocateBag(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight(), TRUE);

	// Regular Dialog
	CNtlPLGui* pGui = NULL;
	RwInt32 iX = dREGULARDIALOG_FIRST_X;
	RwInt32 iY = m_iRegularStrartY;
	REGULAR_MAP_ITER it_regular = m_mapRegular.begin();

	for( ; it_regular != m_mapRegular.end() ; ++it_regular )
	{
		pGui = GetDialog(it_regular->first);
		
		if( pGui )
		{
			pGui->SetPosition(iX, iY);
			iX += dMOVALBE_DEFAULT_POSITION_GAP;

			if( (1024 - iX) < dNEED_SIZE_MORE_REGULAR )
			{
				iX = dREGULARDIALOG_FIRST_X;
				iY += dMOVALBE_DEFAULT_POSITION_GAP;
			}
		}
	}

	// 창고와 길드 창고는 레귤러 다이얼로그이지만 Movable 기능이 활성화되어 있지 않아도
	// 레귤러 다이얼로그의 위치에 있는다
	pGui = GetDialog(DIALOG_WAREHOUSEBAR);
	if( pGui )
		pGui->SetPosition(dREGULARDIALOG_FIRST_X, m_iRegularStrartY);

	pGui = GetDialog(DIALOG_GUILD_WAREHOUSEBAR);
	if( pGui )
		pGui->SetPosition(dREGULARDIALOG_FIRST_X, m_iRegularStrartY);
}

RwBool CDialogManager::IsExistDialog(RwInt32 iDialog)
{
	if( GetDialogInfo(iDialog) )
		return TRUE;

	return FALSE;
}

RwBool CDialogManager::CanMoveItem_in_Capsule()
{
	if( IsOpenDialog(DIALOG_SHOPING_CART) )
		return FALSE;

	return TRUE;
}

RwBool CDialogManager::CanPickupItem_in_Capsule()
{
	if( IsMode(DIALOGMODE_ITEM_REPAIR) )
		return FALSE;

	return TRUE;
}

RwBool CDialogManager::IsOpenDialog(int iDialog)
{
	DialogInfo* pDialogInfo = GetDialogInfo(iDialog);

	if(!pDialogInfo)
		return FALSE;

	return pDialogInfo->pDialog->IsShow();
}

RwBool CDialogManager::IsBagOpen()
{
	for( RwInt32 i = DIALOG_CAPSULE_1 ; i <= DIALOG_CAPSULE_NETPY ; ++i )
	{
		if( IsOpenDialog(i) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

RwBool CDialogManager::IsOpenNPCDialog()
{
	for( RwInt32 iDialog = DIALOG_NPCDIALOG_FIRST ; iDialog <= DIALOG_NPCDIALOG_LAST ; ++iDialog )
	{
		if( IsOpenDialog(iDialog) )
			return TRUE;
	}

	return FALSE;
}

RwBool CDialogManager::IsNPCDialog(RwInt32 iDialog)
{
	for( RwInt32 i = DIALOG_NPCDIALOG_FIRST ; i <= DIALOG_NPCDIALOG_LAST ; ++i )
	{
		if( i == iDialog )
			return TRUE;
	}

	return FALSE;
}

RwBool CDialogManager::IsTriggerSystemDialogOpen()
{
	if( IsOpenDialog( DIALOG_MULTIDIALOG ) ||
		IsOpenDialog( DIALOG_QUESTPROPOSAL ) ||
		IsOpenDialog( DIALOG_QUESTREWARD ) ||
		GetNtlPLCinematicObject()->IsShow() ||
		IsOpenDialog( DIALOG_DBC_ALTAR) ||
		IsOpenDialog( DIALOG_DBC_REWARD) 
	  )
		return TRUE;

	return FALSE;
}

RwBool CDialogManager::IsMode(eDialogMode eMode)
{
	if( m_eMode == eMode )
		return TRUE;

	return FALSE;
}

RwBool CDialogManager::IsRegularDialog(eDialogType eType)
{
	if( m_mapRegular.find(eType) == m_mapRegular.end() )
		return FALSE;

	return TRUE;
}

RwBool CDialogManager::IsEFDialog(eDialogType eType)
{
	if( m_mapEntireFocuing.find(eType) == m_mapEntireFocuing.end() )
		return FALSE;

	return TRUE;
}

RwBool CDialogManager::IsOpenEFDialog()
{
	FE_MAP_ITER it = m_mapEntireFocuing.begin();
	for( ; it != m_mapEntireFocuing.end() ; ++it )
	{
		if( IsOpenDialog(it->first) )
			return TRUE;
	}

	return FALSE;
}

RwBool CDialogManager::IsHaveAttribute(eDialogType eType, RwUInt32 uiAttr)
{
	REGULAR_MAP_ITER it = m_mapRegular.find(eType);

	if( it == m_mapRegular.end() )
		return FALSE;

	if( it->second & uiAttr )
		return TRUE;

    return FALSE;
}

RwBool CDialogManager::CanOpenRegularDialog()
{	
	if( HowManyOpenMaxRegular() > (RwInt32)m_listOpenedRegularDilaog.size() )
		return TRUE;

	// avooo's comment: Regular dialog can be opened on screen when client resize
	// If the limit is exceeded, the LocationDialogs ()
	// Ensure that the regular dialog is closed.
	// Because of this, here is the proper number of open regular dialogs that can be opened on the screen
	// If you can close one regular dialog,
	// can be opened
	LIST_OPENED_REGULAR_ITER it_openedRegular = m_listOpenedRegularDilaog.begin();
	for( ; it_openedRegular != m_listOpenedRegularDilaog.end() ; ++it_openedRegular )
	{
		if( IsHaveAttribute(*it_openedRegular, dRDA_EXCLUSIVE) == FALSE )
			return TRUE;
	}

	return FALSE;
}

RwBool CDialogManager::IsDialogMovable()
{
	return m_bDialogMovable;
}

VOID CDialogManager::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CDialogManager::HandleEvents");

	if( msg.Id == g_EventResize )
	{
		CNtlStage *pStage = CStageManager::GetInstance()->GetActiveStage();
		std::string strStageName = pStage->GetName();

		if( strStageName == GAME_STAGE_NAME )
		{
			CGameStage* pGameStage = reinterpret_cast<CGameStage*>( pStage );

			if( pGameStage->IsActiveLoadingScene() )
			{
				NTL_RETURNVOID();
			}			
		}	

		SNtlPLEventResize* pEvent = reinterpret_cast<SNtlPLEventResize*>( msg.pData );
		LocationDialogs(pEvent->iWidht, pEvent->iHeight);
	}
	else if( msg.Id == g_EventDialog )
	{
		SDboEventDialog* pEvent = reinterpret_cast<SDboEventDialog*>( msg.pData );

		switch( pEvent->iType )
		{
			case DIALOGEVENT_INPUTEDITBOX_LOST_FOCUS_WITH_ESC_KEY:
			{
				m_bESCSkip = TRUE;
				break;
			}
			default:
			break;
		}
	}
	else if( msg.Id == g_EventDialogMove )
	{
		SNtlEventDialogMove* pData = reinterpret_cast<SNtlEventDialogMove*>( msg.pData );

		switch( pData->nType )
		{
		case TRM_DIALOG_DIALOG_MOVABLE:
			{
				ProcessMovableDialogs(TRUE);
				break;
			}
		case TRM_DIALOG_DIALOG_MOVABLE_DEFAULT_POSITION:
			{
				Locate_UserDefaultPosition(GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight());

				CChatGui* pChatGui = reinterpret_cast<CChatGui*>( GetDialog(DIALOG_CHAT) );
				if( pChatGui )
				{
					pChatGui->SetPosition(dDIALOG_CLEINT_EDGE_GAP, GetDboGlobal()->GetScreenHeight() - ( pChatGui->GetHeight() + dDIALOG_CLEINT_EDGE_GAP) );
					pChatGui->SetDismovableDisplayPositioin();
				}
				
				break;
			}
		case TRM_DIALOG_DIALOG_DISMOVABLE:
			{
				ProcessMovableDialogs(FALSE);
				LocationDialogs( GetDboGlobal()->GetScreenWidth(), GetDboGlobal()->GetScreenHeight() );
				break;
			}
		default:
			break;
		}
	}
	else if( msg.Id == g_EventChangeWorldConceptState )
	{
		SNtlEventWorldConceptState* pEvent = reinterpret_cast<SNtlEventWorldConceptState*>( msg.pData );
		if( GetNtlWorldConcept()->IsGrade(WORLD_CONCEPT_FIRST_GRADE, pEvent->eWorldConcept) )
		{
			if( pEvent->uiState == WORLD_STATE_EXIT )
			{
				GetDialogManager()->OpenGMDialog();
				OpenDefaultDialog();
			}
		}
		else if( pEvent->eWorldConcept == WORLD_PLAY_DRAGONBALL_COLLECT && pEvent->uiState == WORLD_STATE_NONE )
		{
			GetDialogManager()->OpenGMDialog();
			OpenDefaultDialog();
		}
	}
	else if( msg.Id == g_EventShowTutorialNfy )
	{
		SNtlEventTutorialNfy* pEvent = reinterpret_cast<SNtlEventTutorialNfy*>( msg.pData );

		if( pEvent->uiMessage == TUTORIAL_HELP_DIALOG_SWITCH )
		{
			if( pEvent->bShow )
				OpenDialog(DIALOG_HELPWND);
			else
				CloseDialog(DIALOG_HELPWND);
		}
		else if( pEvent->uiMessage == TUTORIAL_BAG_SWITCH )
		{
			SwitchBag( pEvent->bShow );
		}
		else if( pEvent->uiMessage == TUTORIAL_SKILL_DIALOG_SWITCH )
		{
			if( pEvent->bShow )
				OpenDialog(DIALOG_SKILL);
			else
				CloseDialog(DIALOG_SKILL);
		}
		else if( pEvent->uiMessage == TUTORIAL_CHARINFO_DIALOG_SWITCH )
		{
			if( pEvent->bShow )
				OpenDialog(DIALOG_STATUS);
			else
				CloseDialog(DIALOG_STATUS);
		}
	}
	else if( msg.Id == g_EventNPCCommuExit )
	{
		RwInt32 nDialog = GetOpendNPCDIalog();

		if( nDialog != DIALOG_UNKNOWN )
		{
			CDboEventGenerator::DialogEvent( DIALOGEVENT_NPC_BYEBYE, DIALOG_UNKNOWN, nDialog );
		}		
	}

	NTL_RETURNVOID();
}
///////////////////////////////////////////////////////////// Test Func
/// woosungs_test
CNtlPLGui* const CDialogManager::GetpDialogTEST(const char* szFrmFileName)
{
	CNtlPLGui* pDialog;
	DIALOG_MAP::iterator it = m_mapDialog.begin();	/// 등록된 dialog 검색
	for( ; it != m_mapDialog.end(); ++it)
	{
		pDialog = it->second.pDialog;
		if(pDialog)
		{								// ex) "gui\PcStatus.frm"
			if(!strncmp(szFrmFileName, pDialog->GetCompPageName(), 150))
				return pDialog;
		}
	}

	return GetSideDialogManager()->GetpDialogTEST(szFrmFileName);
}

RwBool CDialogManager::ShowDialogTEST(RwInt32 iDialog, bool bOpen)
{
	/// OpenDialog 아닌 단순 Show로.....
	CNtlPLGui* pDialog;
	DIALOG_MAP::iterator it = m_mapDialog.find(iDialog);	/// 등록된 dialog 검색
	if( it != m_mapDialog.end() )
	{
		pDialog = it->second.pDialog;
		if(pDialog)
		{
			pDialog->Show(bOpen);
			return TRUE;
		}
	}
	else	/// 없으면 side dialog  검사, open
		return GetSideDialogManager()->ShowDialogTEST(iDialog, bOpen);

	return FALSE;
}

VOID CDialogManager::ShowAllDialogTEST(bool bOpen)	///< DialogManager, SideDialogManager에 등록된 모든 dialog open/close
{
	GetSideDialogManager()->ShowAllDialogTEST(bOpen);

	CNtlPLGui* pDialog;
	for(RwInt32 i = DIALOG_FIRST + 1 ; i < DIALOG_NUM ; ++i )
	{	/// OpenDialog 아닌 단순 Show로.....
		DIALOG_MAP::iterator it = m_mapDialog.find(i);	/// 등록된 dialog 검색
		if( it != m_mapDialog.end() )
		{
			pDialog = it->second.pDialog;
			if(pDialog)
			{
				pDialog->Show(bOpen);
			}
		}
	}
}