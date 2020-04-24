/******************************************************************************
* File			: DBCSideIcon.h
* Author		: agebreak
* Copyright		: (주)NTL
* Date			: 2007. 7. 27
* Abstract		: 
*****************************************************************************
* Desc			: 드래곤볼 수집 관련 사이드 아이콘
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"
#include "NtlSLEvent.h"

// share
#include "NtlTimeQuest.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "Windowby3.h"
#include "SideIconGui.h"

class CDBCSideIconGui : public CSideIconBase, public RWS::CEventHandler
{
public:
	CDBCSideIconGui(const RwChar* pName);
	virtual ~CDBCSideIconGui();

	RwBool		Create();
	VOID		Destroy();

	virtual VOID	OnSideViewClosed();
	virtual VOID	OnIconButtonClicked(gui::CComponent* pComponent);

protected:
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID				SetIconImageType(eDBCSideIconType eType);
	eDBCSideIconType	GetIconImageType();
	RwBool			IsCollectComplete();			///< 드래곤볼을 전부다 수집했는지 체크한다.
	VOID			ResetSheduleInfo();				///< 상태를 서버로 받은 상태로 되돌린다. (드래곤볼 사용후 호출)

protected:			
	gui::CSlot			m_slotNoSeasonButton;
	gui::CSlot			m_slotSeasonButton;
	gui::CSlot			m_slotCollectButton;

	gui::CButton*		m_pBtnNoSeason;			///< 드래곤볼 수집 기간이 아닐때의 표시 아이콘
	gui::CButton*		m_pBtnSeason;			///< 드래곤볼 수집 기간일때의 표시 아이콘
	gui::CButton*		m_pBtnCollect;			///< 드래곤볼을 다 모은 경우의 표시 아이콘

	eDBCSideIconType	m_eIconImageType;		
	RwBool				m_bDBCComplete;			///< 드래곤볼을 다 모았는지 유무
};


class CDBCSideViewGui : public CSideViewBase, RWS::CEventHandler
{
public:
	CDBCSideViewGui(const RwChar* pName);
	virtual ~CDBCSideViewGui();

	RwBool		Create();
	VOID		Destroy();

	virtual VOID	OnPressESC();
	virtual VOID	OnSideViewOpen(const void* pData);
	virtual VOID	OnSideViewClose();
	virtual VOID	OnSideViewLocate(const CRectangle& rectSideIcon);	

protected:
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			SetState(eDBCSideIconType eState);
	VOID			SetNumber(RwInt32 iNumber);
	VOID			LocateComponent();

	VOID			OnExitClicked(gui::CComponent* pComponent);	
	VOID			OnMove(RwInt32 iOldX, RwInt32 iOldY);
	VOID			OnResize( RwInt32 iOldW, RwInt32 iOldH );
	VOID			OnPaint();

protected:		
	gui::CSlot			m_slotCloseButton;	
	gui::CSlot			m_slotPaint;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotResize;

	CWindowby3			m_BackPanel;		///< 배경

	gui::CStaticBox*	m_pNotify;			///< 알림 메세지
	gui::CStaticBox*	m_pTitle;			///< Side View 제목	
	gui::CButton*		m_pExitButton;		///< 닫기 버튼	
};