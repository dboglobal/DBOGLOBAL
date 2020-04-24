////////////////////////////////////////////////////////////////////////////////
// File: RBSideIconGui.h
// Desc: Rank Battle Side Icon
//
// 2007.8.14 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __RB_SIDE_ICON_GUI_H__
#define __RB_SIDE_ICON_GUI_H__

#include "ceventhandler.h"
#include "NtlPLGui.h"
#include "SideIconGui.h"

class CRBSideIconGui : public CSideIconBase, public RWS::CEventHandler
{
public:
	CRBSideIconGui( const RwChar* pName );
	~CRBSideIconGui(VOID);

	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	OnSideViewClosed(VOID);	

private:
	VOID	HandleEvents( RWS::CMsg& msg );	

	VOID	OnIconButtonClicked( gui::CComponent* pComponent );

	gui::CSlot		m_slotIconButtonClicked;

	gui::CButton*	m_pbtnIcon;
};

class CRBSideViewGui : public CSideViewBase, public RWS::CEventHandler
{
public:
	enum TYPE { STATE_WAIT, STATE_READY, STATE_CANCEL };

	CRBSideViewGui( const RwChar* pName );
	~CRBSideViewGui(VOID);

	RwBool	Create(VOID);
	VOID	Destroy(VOID);
	VOID	Update( RwReal fElapsed );

	VOID	OnPressESC(VOID);
	VOID	OnSideViewOpen( const VOID* pData );
	VOID	OnSideViewClose(VOID);
	VOID	OnSideViewLocate( const CRectangle& rtSideIcon );

private:
	VOID	HandleEvents( RWS::CMsg& msg );

	//VOID	SetWaitData(VOID);								// 입장대기
	//VOID	SetReadyData( VOID* pData, RwInt32 nCount );	// 입장준비
	//VOID	SetCancelData( RwUInt32 uiString, RwUInt32 eType );	// 랭크배틀취소	

	//VOID	SetState( RwInt32 eState );
	VOID	SetCurrentTimeInfo( RwReal fElapsedTIme );
	VOID	SetLifeTimeInfo( RwReal fLifeTime );
			
	//VOID	OnClickCancelRankBattle( gui::CComponent* pComponent );
	VOID	OnClickClose( gui::CComponent* pComponent );

	//gui::CSlot	m_slotCancelRankBattle;
	gui::CSlot	m_slotClose;

	//gui::CButton*		m_pbtnCancelRB;
	gui::CButton*		m_pbtnClose;
	//gui::CStaticBox*	m_pstbRemainTime;
	//gui::CMDStaticBox*	m_pmdbInfo;
	gui::CStaticBox*	m_pstbInfo;

	//RwInt32			m_eState;
	RwReal			m_fCurrentTime;
	RwInt32			m_nCurrentTime;
};

#endif//__RB_SIDE_ICON_GUI_H__
