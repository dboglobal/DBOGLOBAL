#ifndef __GUI_GUIMANAGER_H__
#define __GUI_GUIMANAGER_H__

#include "gui_define.h"
#include "gui_component.h"

START_GUI

class CComponentManager;
class CGUIManager_Generic;

//: Root component.
// The GUI manager component is the 'root component' of any GUI system. It
// is responsible channeling input and output from the general clanlib display
// API and into the GUI system.
// The GUI manager must to be the top-level component in any component tree.
// Modal dialog interfaces in clanGUI is archived by constructing a GUI manager
// with a parent component. This will disable the input of the previous GUI, but
// will keep calling its painting signals.

class CGUIManager : public CComponent
{
//! Construction:
public:
	//: GUI Manager Constructor
	CGUIManager(CRectangle rect);
	
	//: GUI Manager Destructor
	virtual ~CGUIManager();

//! Attributes:
public:
	//: Returns the currently mouse captured component
	CComponent *GetCapturedMouse(VOID);

	//: Returns the currently focused component.
	CComponent *GetFocus(VOID);

	//: Returns the currently focused component.
	CComponent *GetMouseOver(VOID);

//! Operations:
public:

	//: Popop component Add.
	VOID AddPopup(CComponent *pComponent);

	//: Popup component Remove.
	VOID RemovePopup(CComponent *pComponent);

	//: Sets the focus component of the gui.
	VOID SetFocus(CComponent *pComponent);
	
	//: Draws the GUI once.
	VOID Show(VOID);

	//: Redraws the GUI continuosly.
	VOID Run(VOID);

	//: Break run loop.
	VOID Quit(VOID);

	//: Tells the GUI to start accepting input.
	VOID EnableInput(void* key);
	
	//: Tells the GUI to stop accepting input.
	VOID DisableInput(void* key);

	//: Sets the GUI in capture mode. [should we move this out of API space? -- mbn]
	VOID GuiCaptureMouse(CComponent *pComponent);

	//: Takes GUI out of capture mode. [should we move this out of API space? -- mbn]
	VOID GuiReleaseMouse(VOID);

	//: input procedure
	VOID Input(VOID);

	//: window event procedure. 
	LRESULT Win32EventProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//: Signals
public:
	
	CSignal_v3<CComponent*, CInputDevice *, const CKey &>& SigCaptureKeyDown(VOID);
	CSignal_v3<CComponent*, CInputDevice *, const CKey &>& SigCaptureKeyUp(VOID);
	CSignal_v3<int,int,int>& SigCaptureMouseMove(VOID);
	CSignal_v1<const CKey&>& SigCaptureMouseUp(VOID);
	CSignal_v1<const CKey&>& SigCaptureMouseDown(VOID);
	CSignal_v1<const CKey&>& SigCaptureEndMouseDown(VOID);
	CSignal_v3<int,short,CPos&>& SigCaptureWheelMove(VOID);

	CGUIManager_Generic* GetpGUIManager_Generic() const				{	return m_pImpl;		}		/// woosungs_test
//! Implementation:
private:

	struct WIN32EVENT
	{
		HWND hWnd;
		UINT uMsg;
		WPARAM wParam;
		LPARAM lParam;
		WIN32EVENT(HWND hwnd,UINT uM,WPARAM wP,LPARAM lP)
		{
			hWnd = hwnd,uMsg = uM,wParam = wP,lParam = lP;
		}
	};

	std::list<WIN32EVENT> m_stlInput;

	CGUIManager_Generic *m_pImpl;

	VOID InputPush(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	//: input process
	bool InputProcess(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

END_GUI

#endif
