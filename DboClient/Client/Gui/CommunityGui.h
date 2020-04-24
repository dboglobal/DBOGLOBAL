/******************************************************************************
* File			: CCommunityGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 9. 13
* Abstract		: 
*****************************************************************************
* Desc			: 커뮤니티 관련 다이얼로그 형틀
*				  각각의 페이지 내용은 PageType에 따라 세팅되고
*				  메세지와 이벤트도 활성화된 Page에 전달된다.
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// gui
#include "gui_staticbox.h"
#include "gui_inputbox.h"

// presentation
#include "NtlPLGui.h"

// simulation
#include "NtlSLDef.h"

// client
#include "SlotGui.h"
#include "Windowby3.h"

class CCommunityBase;

class CCommunityGui : public CNtlPLGui, public RWS::CEventHandler
{
public:

	enum eCommunityPageType
	{
		PAGETYPE_GUILD,
		//PAGETYPE_RAID,

		NUM_PAGE,

		INVALID_PAGE
	};
	
	CCommunityGui(const RwChar* pName);
	virtual ~CCommunityGui();

	RwBool		Create();
	VOID		Destroy();

	RwInt32		SwitchDialog(bool bOpen);			///< DialogManager에서의 Open/Close

	VOID		SetPageType(RwInt32 iPageType);			///< 해당 페이지를 활성화 시킨다.
	RwInt32		GetPageType();							///< 활성화된 페이지 인덱스 반환

	CHARACTERID	GetGuildSelectedID();

protected:
	CCommunityGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			SetupPage(RwInt32 iPage);

	VOID			CloseButtonClicked(gui::CComponent* pComponent);	///< 닫기 버튼을 눌렀다.
	VOID			OnTabChanged(RwInt32 nIndex, RwInt32 nOldIndex);	///< 탭 버튼을 눌렀다.

	VOID			OnPaint();

	VOID			OnMouseDown(const CKey& key);
	VOID			OnMouseUp(const CKey& key);
	VOID			OnMove( RwInt32 iOldX, RwInt32 iOldY );
	VOID			OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY);
	VOID			OnMouseLeave(gui::CComponent* pComponent);
	VOID			OnCaptureMouseDown(const CKey& key);
	VOID			OnClickedBtnHelp( gui::CComponent* pComponent );

protected:	
	gui::CSlot			m_slotTabChanged;
	gui::CSlot			m_slotMouseDown;
	gui::CSlot			m_slotMouseUp;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotMouseMove;
	gui::CSlot			m_slotMouseLeave;
	gui::CSlot			m_slotPaint;	
	gui::CSlot			m_slotCaptureMouseDown;
	gui::CSlot			m_slotCloseButton;

	gui::CStaticBox*	m_pDialogName;				///< 다이얼로그의 이름 스태틱
	gui::CButton*		m_pExitButton;				///< 창닫기 버튼
	CWindowby3			m_BackLineSurface;			///< 백라인

	gui::CPanel*		m_pDumyPanel;				///< 더미

	gui::CTabButton*	m_pTabButton;				///< 탭 버튼

	gui::CButton*		m_pBtnHelp;
	gui::CSlot			m_slotClickedBtnHelp;

	RwInt32				m_iPageType;				///< 현재의 페이지 타입

	///< 각 탭별 페이지
	CCommunityBase*		m_pTabPage[NUM_PAGE];		///< 각 탭의 GUI
};