////////////////////////////////////////////////////////////////////////////////
// File: DoRepairSideIconGui.h
// Desc: Rank Battle Side Icon
//
// 2007.8.14 Peessi@hitel.net   
////////////////////////////////////////////////////////////////////////////////

#ifndef __DO_REPAIR_SIDE_ICON_GUI_H__
#define __DO_REPAIR_SIDE_ICON_GUI_H__

#include "ceventhandler.h"
#include "NtlPLGui.h"
#include "SideIconGui.h"

enum DoRepairSideState { DOREPAIR_STATE_NONE = 0x00, DOREPAIR_STATE_FIX = 0x01, DOREPAIR_STATE_EXTEND = 0x02 };

class CDoRepairSideIconGui : public CSideIconBase, public RWS::CEventHandler
{
public:
	CDoRepairSideIconGui( const RwChar* pName );
	~CDoRepairSideIconGui(VOID);

	RwBool	Create(VOID);
	VOID	Destroy(VOID);
	VOID	Update( RwReal fElapsedTime );

	VOID	OnSideViewClosed(VOID);	

private:
	VOID	HandleEvents( RWS::CMsg& msg );	

	VOID	OnIconButtonClicked( gui::CComponent* pComponent );
	VOID	OnPaint(VOID);
	VOID	OnMove( RwInt32 nOldX, RwInt32 nOldY );
		
	gui::CSlot		m_slotIconButtonClicked;
	gui::CSlot		m_slotPaint;
	gui::CSlot		m_slotMove;
	
	gui::CButton*	m_pbtnIcon;	
	gui::CFlickerEffect	m_feEffect;
};

class CDoRepairSideViewGui : public CSideViewBase
{
public:
	CDoRepairSideViewGui( const RwChar* pName );
	~CDoRepairSideViewGui(VOID);

	RwBool	Create(VOID);
	VOID	Destroy(VOID);
	
	VOID	OnPressESC(VOID);
	VOID	OnSideViewOpen( const VOID* pData );
	VOID	OnSideViewClose(VOID);
	VOID	OnSideViewLocate( const CRectangle& rtSideIcon );

private:
	
	VOID	OnClickClose( gui::CComponent* pComponent );

	gui::CSlot	m_slotClose;

	gui::CButton*		m_pbtnClose;
	gui::CStaticBox*	m_pstbInfo;
};

#endif//__DO_REPAIR_SIDE_ICON_GUI_H__
