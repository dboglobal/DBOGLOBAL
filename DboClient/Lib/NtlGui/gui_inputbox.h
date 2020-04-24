#ifndef __GUI_INPUTBOX_H__
#define __GUI_INPUTBOX_H__

#include "gui_component.h"
#include "gui_surface.h"
#include "eventtimer.h"

typedef void (*CallGuiInputBoxFocus)(bool bGotFocus);

void LinkGuiInputBoxFocus(CallGuiInputBoxFocus fn);
void UnLinkGuiInputBoxFocus(void);

enum INPUT_STYLE
{
	IS_PASSWORD = 0x01,
	IS_READONLY = 0x02,
	IS_NUMBERONLY = 0x04
};

enum IMESTATE { IMEUI_STATE_OFF, IMEUI_STATE_ON, IMEUI_STATE_ENGLISH };
enum IMELOCALSTATE { INDICATOR_NON_IME, INDICATOR_CHS, INDICATOR_CHT, INDICATOR_KOREAN, INDICATOR_JAPANESE };

START_GUI

class CScrollBar;
class CInputBox_Generic;

//: Inputbox component
class CInputBox : public CComponent
{	
//! Construction:
public:
	//: InputBox Constructor
	CInputBox(CComponent *pParent,
						CSurfaceManager *pSurfaceManager,
						DWORD dwStyle=0);

	//: InputBox Constructor
	CInputBox(const CRectangle &rtRect,
						CComponent *pParent,
						CSurfaceManager *pSurfaceManager,
						DWORD dwStyle=0);

	//: InputBox Destructor
	virtual ~CInputBox();

//! Attributes:
public:
	//: Returns the inputfield text.
	const WCHAR* GetText(VOID) const;

	//: Returns the length of the inputfield text.
	int GetLength(VOID) const;

	//: Returns the maximum permitted length of the text.
	int GetMaxLength(VOID) const;

	//: Returns the surface used.
	std::list<CSurface>* GetSurface(VOID) const;

	//: Returns Current Saved IME State
	INT GetCurrentIMEState(VOID) const;

	BOOL IsNumberOnly(VOID) const;

//! Operations:
public:

	//: Sets the line edit text to text, clears the selection and moves the cursor to the end of the line.
	//: If necessary the text is truncated to get_max_length().
	VOID SetText(const char *text);

	//: Sets the line edit text to text, clears the selection and moves the cursor to the end of the line.
	//: If necessary the text is truncated to get_max_length().
	VOID SetText(const std::string &text);

	//: Sets the line edit contents to a plain text containing the printed value of num.
	VOID SetText(int number);

	//: Sets the line edit text to text, clears the selection and moves the cursor to the end of the line.
	//: If necessary the text is truncated to get_max_length().
	VOID SetText(const WCHAR *text);

	//: Set the maximum length of the text in the editor.
	VOID SetMaxLength(int length);

	//: Syntactic sugar for set_text("").
	VOID Clear();

	//: Sets the password flag to on. 
	VOID SetPasswordMode(BOOL bPwMode);

	VOID SetNumberOnly(BOOL bNumberMode);

	//: Sets the multiline flag to on.
	VOID SetMultilineMode(BOOL bMultilineMode);

	//: Sets the multiline interval.
	VOID SetLineInterval(INT nInterval);

	//: Sets the readonly flag to on.
	VOID SetReadOnly(BOOL bReadOnly);

	//: Sets the surface.
	VOID AddSurface(CSurface Surface);

	//: Set the text color
	VOID SetTextColor(COLORREF color);

	//: Set the text background color
	VOID SetTextBkColor(COLORREF color);  

	//: Set the text background mode
	//: BK_OPAQUE or BK_TRANSPARENT
	VOID SetTextBkMode(int nBkMode);
	
	//: Set Caret Draw show
	VOID ShowCaret(VOID);

	//: Set Caret Draw hide
	VOID HideCaret(VOID);

	//: Set Caret Size
	VOID SetCaretSize(int nWidth,int nHeight);

	//: Set Caret Color
	VOID SetCaretColor(BYTE byRed,BYTE byGreen,BYTE byBlue,BYTE byAlpha);

	//: Set IME english.
	static VOID SetEnglishMode (VOID);

	static VOID SetEnglishModeDefault();

	//: Set IME hangle.
	static VOID SetNativeMode (VOID);

	//: Set IME DefaultNativeMode
	static VOID SetNativeModeDefault(VOID);

//! Signals:
public:
	//: This signal is emitted whenever text is changed in the inputfield.
	//- const std::string & - Contains the new string.
	CSignal_v1<const std::string &> &SigChanged(VOID);

	//: This signal is emitted when enter is pressed in the inputfield.
	CSignal_v0 &SigReturnPressed(VOID);

	//: This signal is emitted when space is released in the inputfield.
	CSignal_v0& SigSpaceUp();

	//: This signal is emitted whenever some character is changed/added/removed, cursor moved etc.
	CSignal_v0 &SigActivity(VOID);

	//: This singal is emitted to all of inputbox when ime changed.
	static CSignal_v1<INT>& SigIMEChanged(VOID);

	// virtual operator
public:

	//: Called when SetAlpha Function 
	CSignal_v1<int>& SigCharChanged(VOID);

	//: This signal is emitted when esc key up in the inputfield.
	CSignal_v0 &SigEscKeyUp(VOID);
	
	virtual LRESULT OnWindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam); 

//! Implementation:
private:

	VOID ResizeSurface(VOID);

	VOID CalcScrollBarRect( CRectangle* prtScrollBar, INT nScrollBarWidth );

// Signals
private:

	VOID OnSetOptions(const CComponentOptions &options);
	VOID OnPaint(VOID);
	VOID OnGotFocus(VOID);
	VOID OnLostFocus(VOID);
	VOID OnKeyDown(CComponent *, CInputDevice *, const CKey &);
	VOID OnKeyUp(CComponent *, CInputDevice *, const CKey &);
	VOID OnMove(int nOldX,int nOldY);

	VOID OnTimer(VOID);

	CSlot m_SlotSetOptions;
	CSlot m_SlotPaint;
	CSlot m_SlotGotFocus;
	CSlot m_SlotLostFocus;
	CSlot m_SlotKeyDown;
	CSlot m_SlotKeyUp;
	CSlot m_SlotMove;

	//! Variables
	CScrollBar*		m_pScrollBar;
		
	CTimerHandle m_hTimer;

	CInputBox(const CInputBox &copy) : CComponent(NULL, NULL) { return; } // disallow copy construction.
	CInputBox_Generic *m_pImpl;
	friend class CInputBox_Generic;	
};

END_GUI

#endif
