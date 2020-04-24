/******************************************************************************
* File			: ServerSelectGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 12. 11
* Update		: 2007. 10. 12
* Abstract		: 
*****************************************************************************
* Desc			: ServerSelectGui
*****************************************************************************/

#pragma once

#include <list>

// core
#include "ceventhandler.h"

// gui
#include "gui_button.h"
#include "gui_progressbar.h"
#include "gui_scrollbar.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "Windowby3.h"
#include "SlotGui.h"
#include "LobbyManager.h"


class CServerSelectGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	struct CServerItem
	{	
		gui::CStaticBox*	pServerName;
		gui::CStaticBox*	pCharCount;
		gui::CStaticBox*	pState;

		SERVER_HANDLE		hServer;
		wchar_t				awcServerName[NTL_MAX_SIZE_SERVER_FARM_NAME_UNICODE + 1];	///< 서버 이름
		RwUInt8				byCharacterCount;			///< 소유한 캐릭터
		RwUInt32			uiUserRate;					///< 유저 접속 비율
	};

	typedef std::list<CServerItem*>					SERVERITEM_LIST;
	typedef std::list<CServerItem*>::iterator		SERVERITEM_ITER;


	CServerSelectGui(const RwChar* pName);
	virtual ~CServerSelectGui();

	RwBool		Create();
	VOID		Destroy();

	// 캐릭터 스테이지(로비)에서의 GUI의 컨트롤의 위한 함수
	VOID		SwitchDialog(bool bShow);

protected:
	CServerSelectGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID		CreateServerInfo();
	VOID		DestroyServerInfo();
	VOID		RefreshServerInfo(SERVER_HANDLE hServer);

	VOID		SetState(CServerItem* pServerItem, CLobby* pLobbyInfo);
	
	VOID		LocateComponent(RwInt32 iWidth, RwInt32 iHeight);
	CServerItem* GetSelectedSetverItem();
	VOID		EnableButtons(bool bEnable);

	VOID		UpdateList(RwInt32 iOffset);

	RwUInt8		PtinSlot(RwInt32 iX, RwInt32 iY);
	RwUInt8		Slot_to_Index(RwUInt8 bySlot);
	VOID		FocusEffect(RwUInt8 bySlot);
	VOID		CalcDownBarPosition(RwUInt8 bySlot);
	VOID		CalcSelectBarPosition(RwUInt8 bySlot);

	VOID		OnScrollChanged(RwInt32 iOffset);
	VOID		OnCaptureWheelMove(RwInt32 iFlag, RwInt16 sDelta, CPos& pos);	

	VOID		OnClickedServerButton(gui::CComponent* pComponent);
	VOID		OnClickedCharacterButton(gui::CComponent* pComponent);
	VOID		OnClickedServerStateButton(gui::CComponent* pComponent);
	VOID		OnClickedOKButton(gui::CComponent* pComponent);
	VOID		OnClickedCancleButton(gui::CComponent* pComponent);
	VOID		OnMouseDown(const CKey& key);
	VOID		OnMouseUp(const CKey& key);
	VOID		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID		OnMouseLeave(gui::CComponent* pComponent);
	VOID		OnPostPaint();

protected:
	///< 서버 리스트를 정렬하기 위한 어댑터 클래스
	class CServerListSort
	{
	public:
		///< 이름 오름차순으로 정렬
		static bool CompareAscentName(CServerItem* pServerItem, CServerItem* pServerItem2)
		{
			return (wcscmp(pServerItem->awcServerName, pServerItem2->awcServerName) < 0 );
		}		

		///< 이름 내림차순으로 정렬
		static bool CompareDecentName(CServerItem* pServerItem, CServerItem* pServerItem2)
		{
			return (wcscmp(pServerItem->awcServerName, pServerItem2->awcServerName) > 0 );
		}

		///< 캐릭터수 오름차순으로 정렬
		static bool CompareAscentCharacter(CServerItem* pServerItem, CServerItem* pServerItem2)
		{
			return (pServerItem->byCharacterCount < pServerItem2->byCharacterCount);
		}

		///< 캐릭터수 내림차순으로 정렬
		static bool CompareDecentCharacter(CServerItem* pServerItem, CServerItem* pServerItem2)
		{
			return (pServerItem->byCharacterCount > pServerItem2->byCharacterCount);
		}

		///< 서버 상태 오름차순으로 정렬
		static bool CompareAscentState(CServerItem* pServerItem, CServerItem* pServerItem2)
		{
			return ( pServerItem->uiUserRate < pServerItem2->uiUserRate);
		}		

		///< 서버 상태 내림차순으로 정렬
		static bool CompareDecentState(CServerItem* pServerItem, CServerItem* pServerItem2)
		{
			return ( pServerItem->uiUserRate > pServerItem2->uiUserRate);
		}
	};

protected:
	gui::CSlot			m_slotServerScrollChanged;
	gui::CSlot			m_slotServerScrollSliderMoved;	
	gui::CSlot			m_slotCaptureWheelMove;
	gui::CSlot			m_slotServerDown;
	gui::CSlot			m_slotCharDown;
	gui::CSlot			m_slotStateDown;
	gui::CSlot			m_slotOKDown;
	gui::CSlot			m_slotCancelDown;	
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPostPaint;

	gui::CStaticBox*	m_pTitleStatic;

	gui::CStaticBox*	m_pSelectServer;			///< '서버를 선택하세요'

	gui::CButton*		m_pServerButton;			///< 서버 이름 정렬 버튼
	gui::CButton*		m_pCharButton;				///< 캐릭터 수 정렬 버튼
	gui::CButton*		m_pStateButton;				///< 서버 상태 정렬 버튼
	gui::CButton*		m_pOKButton;				///< 확인 버튼
	gui::CButton*		m_pCancelButton;			///< 취소 버튼

	CSurfaceGui			m_srfServerArrowUp;
	CSurfaceGui			m_srfServerArrowDown;
	CSurfaceGui			m_srfCharArrowUp;
	CSurfaceGui			m_srfCharArrowDown;
	CSurfaceGui			m_srfStateArrowUp;
	CSurfaceGui			m_srfStateArrowDown;
	CSurfaceGui			m_srfFocusBar;
	CSurfaceGui			m_srfSelectBar;
	CSurfaceGui			m_srfDownBar;

	gui::CScrollBar*	m_pScrollBar;

	RwBool				m_bServerAscendingSort;		///< 서버의 오름차순인지 여부
	RwBool				m_bCharacterAscendingSort;	///< 캐릭터의 오름차순인지 여부
	RwBool				m_bChannelAscendingSort;	///< 채널의 오름차순이지 여부
	
	RwUInt8				m_byFocusIndex;
	RwUInt8				m_byDownIndex;
	RwUInt8				m_bySelectIndex;
	RwUInt8				m_byMouseDownSlot;

	SERVERITEM_LIST		m_listServerItem;
};