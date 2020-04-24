/*****************************************************************************
*
* File			: ESCMenuGui.h
* Author		: HyungSuk, Jang
* Copyright	: (аж)NTL
* Date			: 2005. 9. 28	
* Abstract		: DBO MessageBox gui.
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __ESCMENU_GUI_H__
#define __ESCMENU_GUI_H__

#include "NtlPLGui.h"
#include "ceventhandler.h"

class CESCMenuGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CESCMenuGui( const RwChar* pName );
	~CESCMenuGui(VOID);

	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	RwInt32	SwitchDialog( bool bOpen );

	VOID	HandleEvents( RWS::CMsg &msg );
	
private:
	VOID	OnClickedHelp( gui::CComponent* pComponent );
	VOID	OnClickedOption( gui::CComponent* pComponent );
	VOID	OnClickedChangeChannel( gui::CComponent* pComponent );
	VOID	OnClickedLogOut( gui::CComponent* pComponent );
	VOID	OnClickedGameExit( gui::CComponent* pComponent );
	
	gui::CSlot			m_slotClickHelp;
	gui::CSlot			m_slotClickOption;
	gui::CSlot			m_slotClickChangeChannel;
	gui::CSlot			m_slotClickLogOut;
	gui::CSlot			m_slotClickGameExit;

	gui::CButton*		m_pbtnHelp;
	gui::CButton*		m_pbtnOption;
	gui::CButton*		m_pbtnChangeChannel;
	gui::CButton*		m_pbtnLogOut;
	gui::CButton*		m_pbtnGameExit;	
};

#endif