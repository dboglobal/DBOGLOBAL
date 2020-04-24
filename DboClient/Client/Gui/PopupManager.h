////////////////////////////////////////////////////////////////////////////////
// File: PopupManager.h
// Desc: Popup
//
// 2006.06.7 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __POPUP_MANAGER_H__
#define __POPUP_MANAGER_H__

#include "ceventhandler.h"
#include "position.h"

class CPopupGui;

class CPopupManager : public RWS::CEventHandler
{
public:
	//! Enumeration
	enum ePOPUPSTYLE
	{
		ICON_POPUP = 0,
		CALC_POPUP,

		NUM_POPUPGUI
	};
	
private:
	//! Constructor & Destructor
	CPopupManager(VOID);
public:
	~CPopupManager(VOID);
	//! Access
	static RwBool CreateInstance(VOID);
	static VOID   DeleteInstance(VOID);

	static CPopupManager* GetInstance(VOID);

	//! Attribute
	RwBool IsVisiblePopupGui(VOID);
	RwBool IsVisiblePopupGui( RwInt32 ePopupStyle );
	
	//! Operation
	VOID AllPopupClose(VOID);
	
private:
	//! Implementation
	VOID LinkEvent(VOID);
	VOID UnLinkEvent(VOID);

	//! Event
	VOID HandleEvents( RWS::CMsg &msg );

	//! CallBack
		
	//! Variable
	CPopupGui*	m_apPopup[NUM_POPUPGUI];
	RwInt32		m_eShowedPopupPlace;	
	CPos		m_Pos;
	
	static CPopupManager* m_pInstance;
};

static CPopupManager* GetPopupManager(VOID)
{
	return CPopupManager::GetInstance();
}

#endif//__ICON_MOVE_MANAGER_H__