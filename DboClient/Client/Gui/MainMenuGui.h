////////////////////////////////////////////////////////////////////////////////
// File: MainMenuGui.h
// Desc: MainMenu
//
// 2006.5.4 Peessi@hitel.net   
//
// To Be Desired:
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __MAINMENU_GUI_H__
#define __MAINMENU_GUI_H__

#include "NtlPLGui.h"

class CMainMenuGui;
class CMainMenuButtonGui;

class CMainMenuGroup
{
public:
	CMainMenuGroup(VOID);
	~CMainMenuGroup(VOID);

	RwBool	Create(VOID);
	VOID	Destroy(VOID);
			
private:
	VOID OnCaptureMouseDown( const CKey& key );

	gui::CSlot		m_slotCaptureMouseDown;

	CMainMenuGui*			m_pMainMenuGui;
	CMainMenuButtonGui*		m_pMainMenuButtonGui;
};

class CMainMenuGui : public CNtlPLGui
{
public:
//! Constructor & Destructor
	CMainMenuGui(VOID);
	CMainMenuGui( const RwChar* pName );
	~CMainMenuGui(VOID);

//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	RwInt32 SwitchDialog(bool bOpen);

private:
//! Callback
	VOID OnClickedHelp( gui::CComponent* pComponent );
	VOID OnClickedSystem( gui::CComponent* pComponent );
	VOID OnClickedInven( gui::CComponent* pComponent );
	VOID OnClickedStatus( gui::CComponent* pComponent );
	VOID OnClickedQuest( gui::CComponent* pComponent );
	VOID OnClickedSkill( gui::CComponent* pComponent );
	VOID OnClickedCommunity( gui::CComponent* pComponent );
	VOID OnClickedPartySearch(gui::CComponent* pComponent);
	VOID OnClickedRankBoard( gui::CComponent* pComponent );
	VOID OnClickedMascot(gui::CComponent* pComponent);
	VOID OnClickedFriendList( gui::CComponent* pComponent );
	VOID OnClickedChannelChange( gui::CComponent* pComponent );
	VOID OnClickedLogin( gui::CComponent* pComponent );
	VOID OnClickedExit( gui::CComponent* pComponent );
	VOID OnCaptureMouseDown( const CKey& key );

	gui::CSlot		m_slotClickedHelp;
	gui::CSlot		m_slotClickedSystem;
	gui::CSlot		m_slotClickedStatus;
	gui::CSlot		m_slotClickedInven;
	gui::CSlot		m_slotClickedQuest;
	gui::CSlot		m_slotClickedSkill;
	gui::CSlot		m_slotClickedCommunity;
	gui::CSlot		m_slotClickedPartySearch;
	gui::CSlot		m_slotClickedRankBoard;
	gui::CSlot		m_slotClickedMascot;
	gui::CSlot		m_slotClickedFriendList;
	gui::CSlot		m_slotClickedChannelChange;
	gui::CSlot		m_slotClickedLogin;
	gui::CSlot		m_slotClickedExit;
	gui::CSlot		m_slotCaptureMouseDown;
	
//! Variables
	gui::CButton*	m_pbtnHelp;
	gui::CButton*	m_pbtnSystem;
	gui::CButton*	m_pbtnStatus;
	gui::CButton*	m_pBtnInven;
	gui::CButton*	m_pbtnQuest;
	gui::CButton*	m_pbtnSkill;
	gui::CButton*	m_pbtnCommunity;
	gui::CButton*	m_pbtnPartySearch;
	gui::CButton*	m_pbtnRankBoard;
	gui::CButton*	m_pbtnMascot;
	gui::CButton*	m_pbtnFriendList;
	gui::CButton*	m_pbtnChannelChange;
	gui::CButton*	m_pbtnLogin;
	gui::CButton*	m_pbtnExit;
};

class CMainMenuButtonGui : public CNtlPLGui
{
public:
//! Constructor & Destructor
	CMainMenuButtonGui(VOID);
	CMainMenuButtonGui( const RwChar* pName );
	~CMainMenuButtonGui(VOID);

//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	RwInt32 SwitchDialog( bool bOpen );

private:
//! Event
	VOID OnClickedMain( gui::CComponent* pComponent );
	VOID OnPaint(VOID);
	gui::CSlot		m_slotPaint;
	
	gui::CSlot		m_slotClickedMain;

//! Variables
	gui::CButton*	m_pbtnMain;
};

#endif