////////////////////////////////////////////////////////////////////////////////
// Name: class gui:CButton
// Desc: Button
//			 
// 2006.07.6 Fixed Peessi@hitel.net   
//				1. Render routine changes, code cleanup.
//
// To Be Desired: 
////////////////////////////////////////////////////////////////////////////////

#ifndef __GUI_BUTTON_H__
#define __GUI_BUTTON_H__

#include "gui_component.h"
#include "gui_surface.h"

START_GUI

class CStaticBox;
class CButton_Generic;

//: Button component
class CButton : public CComponent
{
//! Construction:
public:

	//: Button Constructor
	CButton( CComponent* pParent, CSurfaceManager* pSurfaceManager, bool bToggleMode = false );
	CButton( CRectangle& rtRect, CComponent* pParent, CSurfaceManager* pSurfaceManager, bool bToggleMode = false );
	CButton( CRectangle& ptRect, const std::string& text, CComponent* pParent, CSurfaceManager* pSurfaceManager,
			 bool bToggleMode = false );
	CButton( CRectangle& ptRect, const std::wstring& text, CComponent* pParent, CSurfaceManager* pSurfaceManager,
			 bool bToggleMode = false );

	//: Button Constructor
	CButton( CRectangle& ptRect, const std::string& text, CSurface Up, CSurface Down, CSurface Disable, CSurface Focus,
			 COLORREF colorUpText, COLORREF colorDownText, COLORREF colorFocusText, COLORREF colorDisableText,
			 INT nCoordTextDownX, INT nCoordTextDownY, CComponent* pParent, CSurfaceManager* pSurfaceManager, bool bToggleMode = false );

	CButton( CRectangle& ptRect, const std::wstring& text, CSurface Up, CSurface Down, CSurface Disable, CSurface Focus,
		COLORREF colorUpText, COLORREF colorDownText, COLORREF colorFocusText, COLORREF colorDisableText,
		INT nCoordTextDownX, INT nCoordTextDownY, CComponent* pParent, CSurfaceManager* pSurfaceManager, bool bToggleMode = false );
	
	//: Button Destructor
	virtual ~CButton(VOID);

//! Attributes:
public:

	//: Returns true if the button is a toggle button.
	bool IsToggleButton(VOID) const;

	//: Returns true if the button pressed down, or false if it is standing up. 
	bool IsDown(VOID) const;

	//: Returns true if Click Enable is TRUE;
	BOOL IsClickEnable(VOID) const;

	//: Retruns true if focused;
	BOOL IsFocused(VOID) const;

	//: Returns the surface used when button is in the up-state.
	std::list<CSurface>* GetSurfaceUp(VOID) const;

	//: Returns the surface used when button is in the down-state.
	std::list<CSurface>* GetSurfaceDown(VOID) const;

	//: Returns the surface used when button is highlighted.
	std::list<CSurface>* GetSurfaceFocus(VOID) const;

	//: Returns the surface used when button is disabled.
	std::list<CSurface>* GetSurfaceDisable(VOID) const;

	//: Returns Text Component
	CStaticBox* GetTextComponent(VOID) const;

//! Operations:
public:
	//: Sets the button text.
	VOID SetText( const std::string& text );
	VOID SetText( const std::wstring& text );
	void SetText(const int nNumber);

	//: Sets the Button text color.
	VOID SetTextUpColor( COLORREF Color );
	VOID SetTextUpAlpha( BYTE ucAlpha );

	VOID SetTextDownColor( COLORREF Color );
	VOID SetTextDownAlpha( BYTE ucAlpha );

	VOID SetTextFocusColor( COLORREF Color );
	VOID SetTextFocusAlpha( BYTE ucAlpha );

	VOID SetTextDisableColor( COLORREF Color );
	VOID SetTextDisableAlpha( BYTE ucAlpha );

	VOID SetTextFont( char* pFaceName, INT nHeight, INT nAttributes );

	VOID SetTextStyle( DWORD dwStyle );

	VOID SetTextEffectMode( INT nMode );

	VOID SetTextEffectColor( COLORREF color );

	VOID SetTextEffectValue( INT nValue );

	VOID ClearText(VOID);

	VOID SetTextCoord( INT nX, INT nY );

	VOID SetTextDownCoordDiff( INT nX, INT nY );		// 버튼다운시 텍스트가 이동하는 값.

	VOID ApplyText(VOID);								// 모든 정보가 다 입력된 후 한번 실행해준다.

	//: If toggle is true, pressing the button toggles it between an On and and Off state. 
	//: If toggle is false, pressing the button causes an action, then the button returns to the unpressed state. 
	VOID SetToggleMode(bool toggle = true);

	//: Sets the state of the button to pressed down if enable is true or to standing up if enable is false. 
	//: The pressed(), released() and toggled() signals are not emitted by this function. 
	VOID SetDown(bool enable = true);

	//: Toggle the button.
	//: The toggled() signal are emitted by this function.
	VOID Toggle(VOID);

	//: Sets the surface used when button is in the up-state.
	VOID AddSurfaceUp(CSurface Surface);
	
	//: Set the surface used when button is in the down-state.
	VOID AddSurfaceDown(CSurface Surface);
	
	//: Sets the surface used when button is highlighted.
	VOID AddSurfaceFocus(CSurface Surface);
	
	//: Sets the surface used when button is disabled.
	VOID AddSurfaceDisabled(CSurface Surface);
	
	VOID Enable(bool bEnable = true);

	VOID ClickEnable( BOOL bEnable = TRUE );

	VOID SetClickSound( const CHAR* szClickSoundFilename );
	VOID SetFocusSound( const CHAR* szFocusSoundFIlename );
	VOID SetDisableSound( const CHAR* szDisableSoundFilename );

	VOID SetClippingMode( bool bClipping ); 
//! Signals:
public:
	//: This signal is emitted whenever a toggle button changes status.
	//- bool on - Is true if the button is on, or false otherwise.
	CSignal_v2<CComponent*,bool>& SigToggled(VOID);

	//: This signal is emitted when the button is activated (i.e. first pressed down and then released when the mouse cursor is inside the button).
	CSignal_v1<CComponent*>& SigClicked(VOID);
	
	//: This signal is emitted when the button is pressed down. 
	CSignal_v1<CComponent*>& SigPressed(VOID);

	//: This signal is emitted when the button is released.
	CSignal_v1<CComponent*>& SigReleased(VOID);

	//: This signal is emitted when the button is focused(true) or just way out of focus.
	CSignal_v2<CComponent*,bool>& SigFocused(VOID);

//! Implementation:
private:

	VOID ResizeSurfaceUp(VOID);
	VOID ResizeSurfaceDown(VOID);
	VOID ResizeSurfaceFocus(VOID);
	VOID ResizeSurfaceDisable(VOID);

//: Signals
private:

	VOID OnSetOptions(const CComponentOptions& options);
	VOID OnPaint(VOID);
	VOID OnGetPreferredSize(CPos &size);
	VOID OnMove(int nX,int nY);	
	VOID OnResize( INT nOldWidth, INT nOldHeight );
	
	CSlot m_SlotSetOption;
	CSlot m_SlotPaint;
	CSlot m_SlotGetPreferredSize;
	CSlot m_SlotMove;
	CSlot m_SlotResize;

private:

	CStaticBox*		m_pButtonText;

	CButton_Generic *m_pImpl;
	friend class CButton_Generic;	
};

END_GUI

#endif
