#include "precomp_dboclient.h"
#include "GMQuickSlotGui.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSobProxy.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlWorldConceptTB.h"
#include "InputActionMap.h"
#include "NtlSLLogic.h"
#include "NtlCameraController.h"
#include "NtlCameraManager.h"

// dbo
#include "DialogPriority.h"
#include "GMPopupGui.h"
#include "GMPlayerInfoGui.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "DboGlobal.h"


CGMQuickSlotGui::CGMQuickSlotGui(const RwChar* pName)
:CNtlPLGui(pName)
,m_eMenuGroup(MENU_GROUP_MAIN)
,m_pPopupGui(NULL)
,m_byPopupSlot(INVALID_BYTE)
,m_bAvatarShow(TRUE)
{

}

CGMQuickSlotGui::~CGMQuickSlotGui()
{

}

RwBool CGMQuickSlotGui::Create()
{
	NTL_FUNCTION( "CGMQuickSlotGui::Create" );

	if(!CNtlPLGui::Create("gui\\Observer.rsr", "gui\\GMQuickSlot.srf", "gui\\GMQuickSlot.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority( dDIALOGPRIORITY_QUICKSLOT );



	//////////////////////////////////////////////////////////////////////////
	//				Main Group
	//////////////////////////////////////////////////////////////////////////

	// GM 캐릭터 보이기 버튼
	m_pCharShowButton = (gui::CButton*)GetComponent("btnCharShow");
	m_pCharShowButton->SetToolTip( GetDisplayStringManager()->GetString("DST_OBSERVER_SHOW_AVATAR") );
	m_slotCharShowButton = m_pCharShowButton->SigClicked().Connect( this, &CGMQuickSlotGui::OnClicked_CharShowHide );

	// GM 캐릭터 숨기기 버튼
	m_pCharHideButton = (gui::CButton*)GetComponent("btnCharHide");
	m_pCharHideButton->SetToolTip( GetDisplayStringManager()->GetString("DST_OBSERVER_HIDE_AVATAR") );
	m_slotCharHideButton = m_pCharHideButton->SigClicked().Connect( this, &CGMQuickSlotGui::OnClicked_CharShowHide );

	// 천하제일 무도회 메뉴 상위 버튼
	m_pTenkaichiButton = (gui::CButton*)GetComponent("btnTenkaichi");
	m_pTenkaichiButton->SetToolTip( GetDisplayStringManager()->GetString("DST_OBSERVER_TENKAICHI") );
	m_slotTenkaichiButton = m_pTenkaichiButton->SigClicked().Connect( this, &CGMQuickSlotGui::OnClicked_Tenkaichi );

	// 아바타 이동 버튼
	m_pPortalButton = (gui::CButton*)GetComponent("btnPortal");
	m_pPortalButton->SetToolTip( GetDisplayStringManager()->GetString("DST_OBSERVER_PORTAL") );
	m_slotPortalButton = m_pPortalButton->SigClicked().Connect( this, &CGMQuickSlotGui::OnClicked_Portal );

	// 프리 카메라 버튼
	m_pFreeCameraButton = (gui::CButton*)GetComponent("btnFreeCamera");
	m_pFreeCameraButton->SetToolTip( GetDisplayStringManager()->GetString("DST_OBSERVER_FREE_CAMERA") );
	m_slotFreeCameraButton = m_pFreeCameraButton->SigClicked().Connect( this, &CGMQuickSlotGui::OnClicked_FreeCamera );

	// 3인칭 카메라 버튼
	m_pNormalCameraButton = (gui::CButton*)GetComponent("btnNormalCamera");
	m_pNormalCameraButton->SetToolTip( GetDisplayStringManager()->GetString("DST_OBSERVER_NORMAL_CAMERA") );
	m_slotNormalCameraButton = m_pNormalCameraButton->SigClicked().Connect( this, &CGMQuickSlotGui::OnClicked_FreeCamera );



	//////////////////////////////////////////////////////////////////////////
	//				Tenkaichi Group
	//////////////////////////////////////////////////////////////////////////

	// 천하제일 무도회 카메라 위치 변경 버튼
	m_pTenkaichi_CameraLocButton = (gui::CButton*)GetComponent("btnCameraLoc");
	m_pTenkaichi_CameraLocButton->SetToolTip( GetDisplayStringManager()->GetString("DST_OBSERVER_CAMERA_LOC") );
	m_slotTenkaichi_CameraLoc = m_pTenkaichi_CameraLocButton->SigClicked().Connect( this, &CGMQuickSlotGui::OnClicked_Tenkaichi_CameraLoc );

	// 천하제일 무도회 카메라 이동 버튼
	m_pTenkaichi_CameraMoveButton = (gui::CButton*)GetComponent("btnCameraMove");
	m_pTenkaichi_CameraMoveButton->SetToolTip( GetDisplayStringManager()->GetString("DST_OBSERVER_CAMERA_MOVE") );
	m_slotTenkaichi_CameraMove = m_pTenkaichi_CameraMoveButton->SigClicked().Connect( this, &CGMQuickSlotGui::OnClicked_Tenkaichi_CameraMove );

	// 천하제일 무도회 정해진 메뉴 Show 버튼
	m_pTenkaichi_MenuShowButton = (gui::CButton*)GetComponent("btnMenuShow");
	m_pTenkaichi_MenuShowButton->SetToolTip( GetDisplayStringManager()->GetString("DST_OBSERVER_MENU_ON") );
	m_pTenkaichi_MenuShowButton->Show(false);
	m_slotTenkaichi_MenuShow = m_pTenkaichi_MenuShowButton->SigClicked().Connect( this, &CGMQuickSlotGui::OnClicked_Tenkaichi_MenuShowHide );
	
	// 천하제일 무도회 정해진 메뉴 Hide 버튼
	m_pTenkaichi_MenuHideButton = (gui::CButton*)GetComponent("btnMenuHide");
	m_pTenkaichi_MenuHideButton->SetToolTip( GetDisplayStringManager()->GetString("DST_OBSERVER_MENU_OFF") );
	m_slotTenkaichi_MenuHide = m_pTenkaichi_MenuHideButton->SigClicked().Connect( this, &CGMQuickSlotGui::OnClicked_Tenkaichi_MenuShowHide );

	// 천하제일 무도회 선수 정보 버튼
	m_pTenkaichi_PlayerButton = (gui::CButton*)GetComponent("btnPlayerInfo");
	m_pTenkaichi_PlayerButton->SetToolTip( GetDisplayStringManager()->GetString("DST_OBSERVER_PLAYER_INFO") );
	m_slotTenkaichi_Player = m_pTenkaichi_PlayerButton->SigClicked().Connect( this, &CGMQuickSlotGui::OnClicked_Tenkaichi_Player );

	// 천하제일 무도회 대진표 보기 버튼
	m_pTenkaichi_TournamentButton = (gui::CButton*)GetComponent("btnTournament");
	m_pTenkaichi_TournamentButton->SetToolTip( GetDisplayStringManager()->GetString("DST_OBSERVER_TOURNAMENT") );
	m_slotTenkaichi_Tournament = m_pTenkaichi_TournamentButton->SigClicked().Connect( this, &CGMQuickSlotGui::OnClicked_Tenkaichi_Tournament );



	m_pReturnButton = (gui::CButton*)GetComponent("btnReturn");
	m_pReturnButton->SetToolTip( GetDisplayStringManager()->GetString("DST_OBSERVER_CAMERA_LOC") );
	m_slotReturn = m_pReturnButton->SigClicked().Connect( this, &CGMQuickSlotGui::OnClicked_Return );


	// GM Popup 메뉴
	m_pPopupGui = NTL_NEW CGMPopupGui("GMPopup");
	if(!m_pPopupGui->Create())
	{
		m_pPopupGui->Destroy(); 
		NTL_DELETE(m_pPopupGui);
		NTL_RETURN(FALSE);
	}
	CNtlPLGuiManager::GetInstance()->AddGui(m_pPopupGui); 


	SetMenuGroup(MENU_GROUP_MAIN);


	// sig
	m_slotMove			= m_pThis->SigMove().Connect( this, &CGMQuickSlotGui::OnMove );

	GetInputActionMap()->LinkSkillQuickSlotUp( this, &CGMQuickSlotGui::ActionMapQuickSlotUp );
	GetInputActionMap()->LinkSkillQuickSlotDown( this, &CGMQuickSlotGui::ActionMapQuickSlotDown );


	LinkMsg(g_EventObserverMenu);
	LinkMsg(g_EventSobServerUpdateCondition);
	LinkMsg(g_EventWorldChange);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CGMQuickSlotGui::Destroy()
{
	NTL_FUNCTION("CGMQuickSlotGui::Destroy");

	if(m_pPopupGui)
	{
		CNtlPLGuiManager::GetInstance()->RemoveGui(m_pPopupGui);
		m_pPopupGui->Destroy();
		NTL_DELETE(m_pPopupGui);
	}

	MAP_POPUP::iterator it = m_mapPlayerInfo.begin();
	for( ; it != m_mapPlayerInfo.end() ; ++it )
	{
		CPlayerInfoGui* pPlayerInfo = it->second;
		pPlayerInfo->Destroy();
		NTL_DELETE(pPlayerInfo);
	}
	m_mapPlayerInfo.clear();


	GetInputActionMap()->UnLinkSkillQuickSlotDown();
	GetInputActionMap()->UnLinkSkillQuickSlotUp();


	UnLinkMsg(g_EventObserverMenu);
	UnLinkMsg(g_EventSobServerUpdateCondition);
	UnLinkMsg(g_EventWorldChange);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy(); 

	NTL_RETURNVOID();
}

VOID CGMQuickSlotGui::SetMenuGroup(eMenuGroup eGroup)
{
	m_eMenuGroup = eGroup;

	switch(eGroup)
	{
	case MENU_GROUP_MAIN:
		{
			ShowMainButtons(true);
			ShowTenkaichiButtons(false);
			break;
		}
	case MENU_GROUP_TENKAICHI:
		{
			ShowMainButtons(false);
			ShowTenkaichiButtons(true);
			break;
		}
	default:
		{
			NTL_ASSERT(false, "CGMQuickSlotGui::SetMenuGroup, Invalid group");
			return;
		}
	}
}

VOID CGMQuickSlotGui::ShowMainButtons(bool bShow)
{
	m_pTenkaichiButton->Show(bShow);

	if( bShow )
	{
		CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSLGlobal()->GetSobAvatar() );
		if( Logic_IsCondition(pActor, CHARCOND_FLAG_TRANSPARENT) )
		{
			m_pCharShowButton->Show(true);
			m_pCharHideButton->Show(false);
		}
		else
		{
			m_pCharShowButton->Show(false);
			m_pCharHideButton->Show(true);						
		}

		if( GetNtlGameCameraManager()->IsActiveFreeCamera() )
		{
			m_pFreeCameraButton->Show(false);
			m_pNormalCameraButton->Show(true);			
		}
		else
		{
			m_pFreeCameraButton->Show(true);
			m_pNormalCameraButton->Show(false);
		}
	}
	else
	{
		m_pCharShowButton->Show(false);
		m_pCharHideButton->Show(false);

		m_pFreeCameraButton->Show(false);
		m_pNormalCameraButton->Show(false);
	}
}

VOID CGMQuickSlotGui::ShowTenkaichiButtons(bool bShow)
{
	m_pTenkaichi_CameraLocButton->Show(bShow);
	m_pTenkaichi_CameraMoveButton->Show(bShow);	
	m_pTenkaichi_PlayerButton->Show(bShow);
	m_pTenkaichi_TournamentButton->Show(bShow);

	if( bShow )
	{
		if( GetDialogManager()->IsOpenDialog(DIALOG_HP) )
		{
			m_pTenkaichi_MenuShowButton->Show(false);
			m_pTenkaichi_MenuHideButton->Show(true);
		}
		else
		{
			m_pTenkaichi_MenuShowButton->Show(true);
			m_pTenkaichi_MenuHideButton->Show(false);
		}
	}
	else
	{
		m_pTenkaichi_MenuShowButton->Show(false);
		m_pTenkaichi_MenuHideButton->Show(false);
	}
}

VOID CGMQuickSlotGui::AddPlayerInfo(SERIAL_HANDLE hPlayer)
{
	MAP_POPUP::iterator it = m_mapPlayerInfo.find(hPlayer);
	if( it != m_mapPlayerInfo.end() )
		return;

	CPlayerInfoGui* pPlayerInfo = NTL_NEW CPlayerInfoGui("GMPopup");
	if( !pPlayerInfo->Create() )
	{
		pPlayerInfo->Destroy();
		NTL_DELETE(pPlayerInfo);
		return;
	}

	if( pPlayerInfo->SetPlayerSerial(hPlayer) == FALSE )
	{
		pPlayerInfo->Destroy();
		NTL_DELETE(pPlayerInfo);
		return;
	}

	pPlayerInfo->SwitchDialog(true);
	m_mapPlayerInfo[hPlayer] = pPlayerInfo;
}

VOID CGMQuickSlotGui::RemovePlayerInfo(SERIAL_HANDLE hPlayer)
{
	MAP_POPUP::iterator it = m_mapPlayerInfo.find(hPlayer);
	if( it == m_mapPlayerInfo.end() )
		return;

	CPlayerInfoGui* pPlayerInfo = it->second;
	pPlayerInfo->Destroy();
	NTL_DELETE(pPlayerInfo);

	m_mapPlayerInfo.erase(it);
}

VOID CGMQuickSlotGui::ShowPopup(RwBool bShow, RwUInt8 bySlot)
{
	if( bShow )
	{
		m_byPopupSlot = bySlot;

		CRectangle rtScreen = m_pThis->GetScreenRect();
		m_pPopupGui->SetPosition(rtScreen.left + m_byPopupSlot*35 + 1, rtScreen.top - m_pPopupGui->GetHeight());
		m_pPopupGui->SwitchDialog(true);		
	}
	else
	{
		if( m_byPopupSlot != INVALID_BYTE )
		{
			m_byPopupSlot = INVALID_BYTE;
			m_pPopupGui->SwitchDialog(false);
		}		
	}
}

VOID CGMQuickSlotGui::OnClicked_CharShowHide( gui::CComponent* pComponent )
{
	CNtlSobActor* pActor = reinterpret_cast<CNtlSobActor*>( GetNtlSLGlobal()->GetSobAvatar() );
	if( pActor )
	{
		if( Logic_IsCondition(pActor, CHARCOND_FLAG_TRANSPARENT) )
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendServerCommand(L"@hide 0");
		}
		else
		{
			GetDboGlobal()->GetGamePacketGenerator()->SendServerCommand(L"@hide 1");			
		}
	}

	ShowPopup(FALSE, 0);
}

VOID CGMQuickSlotGui::OnClicked_Tenkaichi( gui::CComponent* pComponent )
{
	SetMenuGroup(MENU_GROUP_TENKAICHI);
	ShowPopup(FALSE, 0);
}

VOID CGMQuickSlotGui::OnClicked_Portal( gui::CComponent* pComponent )
{
	// GM 이동
	m_pPopupGui->PopupType(CGMPopupGui::POPUP_PORTAL);	
	ShowPopup(TRUE, 2);
}

VOID CGMQuickSlotGui::OnClicked_FreeCamera( gui::CComponent* pComponent )
{
	// Free Camera 전환
	if( GetNtlGameCameraManager()->IsActiveFreeCamera() )
	{
		CNtlSLEventGenerator::CameraControlDelete(CAMERA_CONTROL_FREE);
		m_pFreeCameraButton->Show(true);
		m_pNormalCameraButton->Show(false);
	}
	else
	{
		CNtlSLEventGenerator::CameraFree();
		m_pFreeCameraButton->Show(false);
		m_pNormalCameraButton->Show(true);		
	}	
}

VOID CGMQuickSlotGui::OnClicked_Tenkaichi_CameraLoc( gui::CComponent* pComponent )
{
	// 카메라 위치 변경
	m_pPopupGui->PopupType(CGMPopupGui::POPUP_CAMERA_LOC);
	ShowPopup(TRUE, 0);
}

VOID CGMQuickSlotGui::OnClicked_Tenkaichi_CameraMove( gui::CComponent* pComponent )
{
	// 카메라 이동하기
	m_pPopupGui->PopupType(CGMPopupGui::POPUP_CAMERA_MOVE);
	ShowPopup(TRUE, 1);
}

VOID CGMQuickSlotGui::OnClicked_Tenkaichi_MenuShowHide( gui::CComponent* pComponent )
{
	// 정해진 메뉴 Show/Hide
	if( GetDialogManager()->IsOpenDialog(DIALOG_HP) )
	{
		m_pTenkaichi_MenuShowButton->Show(true);
		m_pTenkaichi_MenuHideButton->Show(false);

		GetDialogManager()->Disable_OnObserver(true);

		GetDialogManager()->SwitchBag(false);
		GetDialogManager()->CloseDialog(DIALOG_HP);		
		GetDialogManager()->CloseDialog(DIALOG_MINIMAP);
		GetDialogManager()->CloseDialog(DIALOG_PET_STATUSBAR);
		GetDialogManager()->CloseDialog(DIALOG_PET_SKILL_SLOT);
		GetDialogManager()->CloseDialog(DIALOG_CHAT);
	}
	else
	{
		m_pTenkaichi_MenuShowButton->Show(false);
		m_pTenkaichi_MenuHideButton->Show(true);

		GetDialogManager()->Disable_OnObserver(false);

		GetDialogManager()->OpenDialog(DIALOG_HP);		
		GetDialogManager()->OpenDialog(DIALOG_MINIMAP);
		GetDialogManager()->OpenDialog(DIALOG_PET_STATUSBAR);
		GetDialogManager()->OpenDialog(DIALOG_PET_SKILL_SLOT);
		GetDialogManager()->OpenDialog(DIALOG_CHAT);
	}

	ShowPopup(FALSE, 0);
}

VOID CGMQuickSlotGui::OnClicked_Tenkaichi_Player( gui::CComponent* pComponent )
{
	// 선수 정보 보기
	m_pPopupGui->PopupType(CGMPopupGui::POPUP_PLAYER_INFO);
	ShowPopup(TRUE, 3);
}

VOID CGMQuickSlotGui::OnClicked_Tenkaichi_Tournament( gui::CComponent* pComponent )
{
	// 천하제일 무도회 대진표
	GetDialogManager()->OpenDialog(DIALOG_BUDOKAI_TOURNAMENT);
	ShowPopup(FALSE, 0);
}

VOID CGMQuickSlotGui::OnClicked_Return( gui::CComponent* pComponent )
{
	switch(m_eMenuGroup)
	{
	case MENU_GROUP_MAIN:			break;
	case MENU_GROUP_TENKAICHI:		SetMenuGroup(MENU_GROUP_MAIN);	break;
	default:	NTL_ASSERT(false, "CGMQuickSlotGui::OnClicked_Return, Invalid Menu group");
	}	
}

VOID CGMQuickSlotGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	if( m_byPopupSlot != INVALID_INDEX )
	{
		ShowPopup(TRUE, m_byPopupSlot);
	}
}

RwInt32 CGMQuickSlotGui::ActionMapQuickSlotDown( RwUInt32 uiKey )
{
	return 1;
}

RwInt32 CGMQuickSlotGui::ActionMapQuickSlotUp( RwUInt32 uiKey )
{
	if( GetNtlWorldConcept()->IsActivePlayConcept(WORLD_PLAY_DIRECT) )
		return 1;

	if( GetDialogManager()->IsMode(DIALOGMODE_NARRATION) )
		return 1;

	if( m_eMenuGroup == MENU_GROUP_MAIN )
	{
		switch(uiKey)
		{
		case 0:
			{
				if( m_bAvatarShow )
					OnClicked_CharShowHide(NULL);
				else
					OnClicked_CharShowHide(NULL);

				return 1;
			}
		case 1:
			{
				OnClicked_Tenkaichi(NULL);
				return 1;
			}
		case 2:
			{
				OnClicked_Portal(NULL);
				return 1;
			}
		case 3:
			{
				OnClicked_FreeCamera(NULL);
				return 1;
			}
		}
			
	}
	else if( m_eMenuGroup == MENU_GROUP_TENKAICHI )
	{
		switch(uiKey)
		{
		case 0:
			{
				OnClicked_Tenkaichi_CameraLoc(NULL);
				return 1;
			}
		case 1:
			{
				OnClicked_Tenkaichi_CameraMove(NULL);
				return 1;
			}
		case 2:
			{
				if( GetDialogManager()->IsOpenDialog(DIALOG_HP) )
					OnClicked_Tenkaichi_MenuShowHide(m_pTenkaichi_MenuHideButton);
				else
					OnClicked_Tenkaichi_MenuShowHide(m_pTenkaichi_MenuShowButton);

				return 1;
			}
		case 3:
			{
				OnClicked_Tenkaichi_Player(NULL);
				return 1;
			}
		case 4:
			{
				OnClicked_Tenkaichi_Tournament(NULL);
				return 1;
			}
		}
	}

	NTL_ASSERT(false, "CGMQuickSlotGui::ActionMapQuickSlotUp, Invalid GMQuickSlot index : " << uiKey);
	return 1;
}

RwInt32 CGMQuickSlotGui::SwitchDialog(bool bOpen)
{
	if( bOpen )
	{
		Show(true);

		MAP_POPUP::iterator it = m_mapPlayerInfo.begin();
		for( ; it != m_mapPlayerInfo.end() ; ++it )
		{
			CPlayerInfoGui* pPlayerInfo = it->second;
			pPlayerInfo->SwitchDialog(true);
		}
	}
	else
	{
		Show(false);
		ShowPopup(FALSE, 0);
		
		MAP_POPUP::iterator it = m_mapPlayerInfo.begin();
		for( ; it != m_mapPlayerInfo.end() ; ++it )
		{
			CPlayerInfoGui* pPlayerInfo = it->second;
			pPlayerInfo->SwitchDialog(false);
		}
	}	
	
	return 1;
}

VOID CGMQuickSlotGui::HandleEvents( RWS::CMsg &msg )
{
	NTL_FUNCTION("CGMQuickSlotGui::HandleEvents");

	if( msg.Id == g_EventObserverMenu )
	{
		SDboEventObserverMenu* pEvent = reinterpret_cast<SDboEventObserverMenu*>( msg.pData );
		switch( pEvent->byEventType )
		{
		case OBSERVER_SHOW_PLAYER_INFO:
			{
				AddPlayerInfo(pEvent->hHandle);
				break;
			}
		case OBSERVER_REMOVE_PLAYERINFO_GUI:
			{
				RemovePlayerInfo(pEvent->hHandle);
				break;
			}
		case OBSERVER_SELECT_POPUPMENU:
			{
				ShowPopup(FALSE, 0);
				break;
			}
		case OBSERVER_TENKAICHI_MATCH_DATA_REQ:
			{
				Enable(false);
				break;
			}
		case OBSERVER_TENKAICHI_MATCH_DATA_RES:
			{
				Enable(true);
				break;
			}
		case OBSERVER_TENKAICHI_MATCH_DATA:
			{
				if( pEvent->uiValue == BUDOKAI_MATCH_TYPE_INDIVIDIAUL )
				{
					m_pPopupGui->PopupType( CGMPopupGui::POPUP_BUDOKAI_INDIVIDIAUL );
					ShowPopup(TRUE, 2);
				}
				else if( pEvent->uiValue == BUDOKAI_MATCH_TYPE_TEAM )
				{
					m_pPopupGui->PopupType( CGMPopupGui::POPUP_BUDOKAI_TEAM );
					ShowPopup(TRUE, 2);
				}

				break;
			}
		}			
	}
	else if( msg.Id == g_EventSobServerUpdateCondition )
	{
		SNtlEventSobServerUpdateCondition* pEvent = reinterpret_cast<SNtlEventSobServerUpdateCondition*>( msg.pData );

		if( Logic_GetAvatarHandle() != pEvent->hSerialId )
			NTL_RETURNVOID();

		if( BIT_FLAG_TEST(pEvent->dwCondition, CHARCOND_FLAG_TRANSPARENT) )
		{
			m_pCharShowButton->Show(true);
			m_pCharHideButton->Show(false);
		}
		else
		{
			m_pCharShowButton->Show(false);
			m_pCharHideButton->Show(true);			
		}
	}
	else if( msg.Id == g_EventWorldChange )
	{
		SNtlEventWorldChange* pEvent = reinterpret_cast<SNtlEventWorldChange*>( msg.pData );

		if( pEvent->bWorldChange )
		{
			// 다시 설정한다
			SetMenuGroup(m_eMenuGroup);
		}
	}

	NTL_RETURNVOID();
}