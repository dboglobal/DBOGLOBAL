/******************************************************************************
* File			: BudokaiSideIconGui.h
* Author		: Haesung, Cho
* Copyright		: (주)NTL
* Date			: 2008. 4. 24
* Abstract		: 
*****************************************************************************
* Desc			: 천하제일무도회 공지 SideIcon
*****************************************************************************/

#ifndef _BUDOKAI_SIDE_ICON_GUI_H_
#define _BUDOKAI_SIDE_ICON_GUI_H_

#include "ceventhandler.h"
#include "NtlPLGui.h"
#include "SideIconGui.h"
#include "Windowby3.h"

// Time
#define dBUDOKAI_NOTICE_SYSTEM_TIME		5.0f		// 시스템 공지 시간
#define dBUDOKAI_NOTICE_GM_TIME			7.0f		// GM Time 공지 시간

#define dBUDOKAI_DEFAULT_HEIGHT			60

#define dBUDOKAI_NOTICE_UNIT_MAX					10
#define dBUDOKAI_NOTICE_UNIT_TEXT_COLOR				RGB( 255, 255, 255 )
#define dBUDOKAI_NOTICE_UNIT_MARGIN_X_FROM_VIEW		10

#define dBUDOKAI_NOTICE_UNIT_WIDTH					230
#define dBUDOKAI_NOTICE_UNIT_HEIGHT					15
#define dBUDOKAI_NOTICE_UNIT_HEIGHT_OFFSET_TOP		30
#define dBUDOKAI_NOTICE_UNIT_HEIGHT_OFFSET_BOTTOM	40

class CBudokaiSideViewUnit
{
public:
	CBudokaiSideViewUnit( gui::CComponent *pParentGui, const WCHAR* pwcString, RwReal fLimitTime );
	virtual ~CBudokaiSideViewUnit();

	void	Update(RwReal fElapsed);
	RwBool	IsLive()				{ return m_bLife; }

	void	SetPositionFromParent(RwInt32 nPosX, RwInt32 nPosY);
	RwInt32	GetHeight()				{ return m_pStbSideViewUnit->GetHeight(); }

protected:
	gui::CStaticBox*		m_pStbSideViewUnit;

	RwReal					m_fCurrentTime;
	RwReal					m_fLimitTime;
	RwBool					m_bLife;
};


class CBudokaiSideIconGui : public CSideIconBase, public RWS::CEventHandler
{
public:
	CBudokaiSideIconGui( const RwChar* pName );
	~CBudokaiSideIconGui(VOID);

	RwBool	Create(VOID);
	VOID	Destroy(VOID);
	VOID	Update( RwReal fElapsedTime );

	VOID	OnSideViewClosed(VOID);

	VOID	HandleEvents( RWS::CMsg& msg );

protected:
	

	VOID	OnIconButtonClicked( gui::CComponent* pComponent );
	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );

	gui::CSlot		m_slotIconButtonClicked;
	gui::CSlot		m_slotPaint;
	gui::CSlot		m_slotMove;

	gui::CButton*			m_pBtnIcon;
	gui::CFlickerEffect		m_feEffect;
};

class CBudokaiSideViewGui : public CSideViewBase
{
public:
	CBudokaiSideViewGui( const RwChar* pName );
	~CBudokaiSideViewGui(VOID);

	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Update( RwReal fElapsedTime );

	VOID	OnPressESC(VOID);
	VOID	OnSideViewOpen( const VOID* pData );
	VOID	OnSideViewReceive( void* pData );
	VOID	OnSideViewClose(VOID);
	VOID	OnSideViewLocate( const CRectangle& rtSideIcon );

protected:
	typedef std::list< CBudokaiSideViewUnit* >				ListUnits;
	typedef std::list< CBudokaiSideViewUnit* >::iterator	ListUnitsIt;

	ListUnits	m_listUnit;

	RwBool	CreateUnit( const WCHAR* pString, RwReal fLifeTime );
	RwBool	DestroyUnit( CBudokaiSideViewUnit* pUnit );

protected:
	VOID	OnPaint();
	VOID	OnMove(RwInt32 nOldX, RwInt32 nOldY);
	VOID	OnClickClose( gui::CComponent* pComponent );
	VOID	ShowNotice( RwUInt32 tblidx );
	VOID	ShowNotice( const WCHAR* pwcString );

	gui::CSlot			m_slotClose;
	gui::CSlot			m_slotMove;
	gui::CSlot			m_slotPaint;

	gui::CButton*		m_pBtnClose;
	gui::CStaticBox*	m_pStbTitle;
	gui::CStaticBox*	m_pStbText;

	CWindowby3			m_backPanel;

	CRectangle			m_rectIcon;
};

#endif//_BUDOKAI_SIDE_ICON_GUI_H
