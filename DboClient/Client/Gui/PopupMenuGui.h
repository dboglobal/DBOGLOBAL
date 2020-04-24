////////////////////////////////////////////////////////////////////////////////
// File: PopupMenuGui.h
// Desc: PopupMenu
//
// 2006.05.31 Peessi@hitel.net   
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __POPUP_MENU_GUI_H__
#define __POPUP_MENU_GUI_H__

#include "ceventhandler.h"
#include "gui_precomp.h"
#include "NtlPLGui.h"
#include "NtlSLDef.h"
#include "DboDef.h"

class CPopupGui : public CNtlPLGui
{
public:
	//! Constructor & Destructor
	CPopupGui(VOID) {}
	CPopupGui( RwChar* szName ) : CNtlPLGui( szName ) {}
	virtual ~CPopupGui(VOID) {}

	//! Operation
	virtual RwBool	Create(VOID) = 0;
	virtual VOID	Destroy(VOID) = 0;

	//! Attribute
	SERIAL_HANDLE GetSerial(VOID) { return m_hSerial; }
	EPlace		  GetPlace(VOID) { return m_ePlace; }
	RwInt32		  GetSlotIdx(VOID) { return m_nSlotIdx; }
	
	//! Event
	virtual VOID	ShowEvent( RWS::CMsg& msg ) = 0;

protected:
	//! Implement
	VOID CaptureMouseDown( const CKey& key );

	//! Variables
	SERIAL_HANDLE	m_hSerial;
	EPlace			m_ePlace;
	RwInt32			m_nSlotIdx;	
};

class CIconPopupGui;

class CIconPopupItem
{
public:
//! Constructor & Destructor
	CIconPopupItem( CIconPopupGui* pIconPopup, gui::CButton* pButton, EIconPopupWork eWork );
	~CIconPopupItem(VOID);

//! Attribute
	VOID SetPosition( RwInt32 nX, RwInt32 nY );
	VOID SetColor( DWORD dwColorUp, DWORD dwColorDown, DWORD dwColorFoc, DWORD dwColorDis );
	VOID Show( bool bShow );

private:
//! Callback
	VOID OnClick( gui::CComponent* pComponent );

	gui::CSlot		m_slotClick;

//! Variables
	CIconPopupGui*	m_pIconPopup;
	EIconPopupWork  m_eWork;

	gui::CButton*	m_pButton;
};

class CIconPopupGui : public CPopupGui
{
public:
//! Constructor & Destructor
	CIconPopupGui(VOID);
	CIconPopupGui( RwChar* szName );
	~CIconPopupGui(VOID);

//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

//! Attribute
	CRectangle GetScreenRect(VOID) { return m_pThis->GetScreenRect(); }

//! Event
	VOID	ShowEvent( RWS::CMsg& msg );

private:
//! Implementation
	VOID	Init(VOID);
	VOID	SelectItem( CIconPopupItem* pItem );
	VOID	SetColor( CIconPopupItem* pItem, DWORD dwColorUp, DWORD dwColorDown, DWORD dwColorFoc, DWORD dwColorDis );
	VOID	CalcRect( RwInt32 nXPos, RwInt32 nYPos );
	VOID	ResetItem(VOID);

	const WCHAR* GetPopupMenuName(RwInt32 num);
	const WCHAR* GetPopupMenuTooltip(RwInt32 num);

	
//! Callback
	VOID	OnCaptureMouseDown( const CKey& key );
	VOID	OnCaptureKeyUp( gui::CComponent* pComponent, CInputDevice* pInputDevice, const CKey& key );
	
	gui::CSlot	m_slotCaptureMouseDown;
	gui::CSlot	m_slotCaptureKeyUp;

//! Variables
	CIconPopupItem* m_apMenu[NUM_PMW];

	RwInt32			m_nSeletecItemCount;
};

#define CALCPOPUP_MAX_CIPHER	10

class CCalcPopupGui : public CPopupGui
{
public:
//! Enumuration
	enum BUTTONINDEX { BUTTON_CLEAR = 10, BUTTON_OK, NUM_BUTTON };
	
//! Constructor & Destructor
	CCalcPopupGui(VOID);
	CCalcPopupGui( RwChar* szName );
	~CCalcPopupGui(VOID);

//! Operation
	RwBool	Create(VOID);
	VOID	Destroy(VOID);

	VOID	Show( bool bShow );

//! Event
	VOID	ShowEvent( RWS::CMsg& msg );

private:
//! Implementation
	VOID	Init(VOID);
	
//! Callback
	VOID	OnButtonClick( gui::CComponent* pComponent );
	VOID	OnButtonClearClick( gui::CComponent* pComponent );
	VOID	OnButtonOkClick( gui::CComponent* pComponent );
	VOID	OnCaptureMouseDown( const CKey& key );

	VOID	OnCaptureKeyDown( gui::CComponent* pComponent, CInputDevice* pInputDevice, const CKey& key );
	VOID	OnCaptureKeyUp( gui::CComponent* pComponent, CInputDevice* pInputDevice, const CKey& key );

	gui::CSlot	m_slotButton[NUM_BUTTON];
	gui::CSlot	m_slotCaptureMouseDown;

	gui::CSlot	m_slotCaptureKeyDown;
	gui::CSlot	m_slotCaptureKeyUp;

//! Variables
	gui::CButton*		m_apButton[NUM_BUTTON];
	gui::CStaticBox*	m_pStaticBox;
	RwUInt32			m_uiValue;
	RwUInt32			m_uiMaxValue;
	bool				m_bAllowNull;
};

#endif//__POPUP_MENU_GUI_H__
