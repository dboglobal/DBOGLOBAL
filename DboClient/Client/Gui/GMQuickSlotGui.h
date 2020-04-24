/******************************************************************************
* File			: ScouterGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 11. 17
* Abstract		: 
*****************************************************************************
* Desc			: GM 게임 단축 메뉴 인터페이스
*****************************************************************************/

#pragma once

class CGMPopupGui;
class CPlayerInfoGui;

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"


class CGMQuickSlotGui : public CNtlPLGui, public RWS::CEventHandler
{
public:	
	enum eMenuGroup
	{
		MENU_GROUP_MAIN,
		MENU_GROUP_TENKAICHI,
	};

	typedef std::map<SERIAL_HANDLE, CPlayerInfoGui*>					MAP_POPUP;


	CGMQuickSlotGui(const RwChar* pName);
	virtual ~CGMQuickSlotGui();

	RwBool		Create();
	VOID		Destroy();

	CGMPopupGui*	GetPopupGui() { return m_pPopupGui; }
	RwInt32		SwitchDialog(bool bOpen);

protected:
	CGMQuickSlotGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			SetMenuGroup(eMenuGroup eGroup);
	VOID			ShowMainButtons(bool bShow);
	VOID			ShowTenkaichiButtons(bool bShow);

	VOID			AddPlayerInfo(SERIAL_HANDLE hPlayer);
	VOID			RemovePlayerInfo(SERIAL_HANDLE hPlayer);

	VOID			ShowPopup(RwBool bShow, RwUInt8 bySlot);

	VOID			OnClicked_CharShowHide( gui::CComponent* pComponent );
	VOID			OnClicked_Tenkaichi( gui::CComponent* pComponent );
	VOID			OnClicked_Portal( gui::CComponent* pComponent );
	VOID			OnClicked_FreeCamera( gui::CComponent* pComponent );

	VOID			OnClicked_Tenkaichi_CameraLoc( gui::CComponent* pComponent );
	VOID			OnClicked_Tenkaichi_CameraMove( gui::CComponent* pComponent );
	VOID			OnClicked_Tenkaichi_MenuShowHide( gui::CComponent* pComponent );
	VOID			OnClicked_Tenkaichi_Player( gui::CComponent* pComponent );
	VOID			OnClicked_Tenkaichi_Tournament( gui::CComponent* pComponent );
	VOID			OnClicked_Return( gui::CComponent* pComponent );

	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);

	RwInt32			ActionMapQuickSlotDown( RwUInt32 uiKey );
	RwInt32			ActionMapQuickSlotUp( RwUInt32 uiKey );

protected:
	gui::CSlot			m_slotCharShowButton;
	gui::CSlot			m_slotCharHideButton;
	gui::CSlot			m_slotTenkaichiButton;
	gui::CSlot			m_slotPortalButton;
	gui::CSlot			m_slotFreeCameraButton;
	gui::CSlot			m_slotNormalCameraButton;

	gui::CSlot			m_slotTenkaichi_CameraLoc;
	gui::CSlot			m_slotTenkaichi_CameraMove;
	gui::CSlot			m_slotTenkaichi_MenuShow;
	gui::CSlot			m_slotTenkaichi_MenuHide;
	gui::CSlot			m_slotTenkaichi_Player;
	gui::CSlot			m_slotTenkaichi_Tournament;
	gui::CSlot			m_slotReturn;
	gui::CSlot			m_slotMove;

	gui::CButton*		m_pCharShowButton;				///< GM 캐릭터 보이기 버튼
	gui::CButton*		m_pCharHideButton;				///< GM 캐릭터 숨기기 버튼
	gui::CButton*		m_pTenkaichiButton;				///< 천하제일 무도회 메뉴 상위 버튼
	gui::CButton*		m_pPortalButton;				///< 아바타 이동 버튼
	gui::CButton*		m_pFreeCameraButton;			///< 프리 카메라 버튼
	gui::CButton*		m_pNormalCameraButton;			///< 3인칭 카메라 버튼

	gui::CButton*		m_pTenkaichi_CameraLocButton;	///< 천하제일 무도회 카메라 위치 변경 버튼
	gui::CButton*		m_pTenkaichi_CameraMoveButton;	///< 천하제일 무도회 카메라 이동 버튼
	gui::CButton*		m_pTenkaichi_MenuShowButton;	///< 천하제일 무도회 정해진 메뉴 Show 버튼
	gui::CButton*		m_pTenkaichi_MenuHideButton;	///< 천하제일 무도회 정해진 메뉴 Hide 버튼
	gui::CButton*		m_pTenkaichi_PlayerButton;		///< 천하제일 무도회 선수 정보 버튼
	gui::CButton*		m_pTenkaichi_TournamentButton;	///< 천하제일 무도회 대진표 보기 버튼
	gui::CButton*		m_pReturnButton;				///< 돌아가기 버튼

	CGMPopupGui*		m_pPopupGui;					///< GM Popup 메뉴

	eMenuGroup			m_eMenuGroup;
	RwBool				m_bAvatarShow;
	RwInt8				m_byPopupSlot;					///< 팝업 메뉴가 나온 슬롯의 인덱스

	MAP_POPUP			m_mapPlayerInfo;
};