#ifndef __GUI_GUIMANAGER_GENERIC_H__
#define __GUI_GUIMANAGER_GENERIC_H__

#include "gui_define.h"
#include "gui_guimanager.h"
#include "inputdevice.h"
#include "mouse.h"
#include "key.h"

START_GUI

class CGUIManager_Generic
{
// Construction:
public:
	CGUIManager_Generic(CGUIManager *pOwner);

	~CGUIManager_Generic();

// Attributes:
public:

	CComponent *GetCapturedMouse(VOID);
	CComponent *GetFocus(VOID);
	CComponent *GetMouseOver(VOID);

// Operations:
public:

	VOID SetFocus(CComponent *pComponent);

	VOID AddPopup(CComponent *pComponent);

	VOID RemovePopup(CComponent *pComponent);

	VOID Run(VOID);
	
	VOID Show(VOID);
	
	bool IsEnableInput(VOID);

	VOID EnableInput(void* key);
	
	VOID DisableInput(void* key);
	
	VOID GuiCaptureMouse(CComponent *pComponent);
	
	VOID GuiReleaseMouse(VOID);
	
	VOID Quit(VOID);
	
// Implementation:
public:

	VOID OnKeyPress(CInputDevice *, const CKey&);
	VOID OnCharPress(CInputDevice *, const CKey&);
	VOID OnKeyRelease(CInputDevice *, const CKey&);
	VOID OnMouseMove(int,int,int);
	VOID OnMouseWheel(int nFalgs,short shDelta,int nX,int nY);
	VOID OnMouseDown(const CKey&,int,int);
	VOID OnMouseUp(const CKey&,int,int);

	VOID SendMsg(int nMsg,int lParam,int wParam);
	LRESULT OnWindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

	CComponent* FindFocusComponent(CInputDevice *pDevice, const CKey &key);
	VOID DoMouseEnterLeave(int nX, int nY, CComponent *pNewFocus);
	CComponent* TabNext(VOID);
	CComponent* TabPrevious(VOID);
	int FindHighestTabID(const CComponent *pComponent);
	
private:

	volatile bool m_bQuitRun;
	CGUIManager *m_pOwner;
	CGUIManager *m_pParent;
	
	int m_nCurrTabID;
	
	std::list<CComponent*> m_listPopup;
	
	CComponent *m_pCompFocus;
	CComponent *m_pCompCaptureMouse;
	CComponent *m_pCompMouseOver;
	
	CComponent *m_pCompDrag;
	int			m_nDataDrag;
	bool		m_bDrag;
	CPos		m_ptDragStartPos;

	static float m_fZoomFactor_Width;
	static float m_fZoomFactor_Height;

	typedef std::map<DWORD64,INT>			DISABLE_INPUT;
	typedef std::map<DWORD64,INT>::iterator	DISABLE_INPUT_IT;
	DISABLE_INPUT	m_mapDisableInput;

public:

	CSignal_v3<CComponent*, CInputDevice *, const CKey &> m_SigCaptureKeyDown;
	CSignal_v3<CComponent*, CInputDevice *, const CKey &> m_SigCaptureKeyUp;
	CSignal_v3<int,int,int>		m_SigCaptureMouseMove;
	CSignal_v1<const CKey &>	m_SigCaptureMouseUp;
	CSignal_v1<const CKey&>		m_SigCaptureMouseDown;
	CSignal_v1<const CKey&>		m_SigCaptureEndMouseDown;
	CSignal_v3<int,short,CPos&> m_SigCaptureWheelMove;
			
//: Signals
private:

	CSlot m_SlotBeforeRemove;

	VOID OnBeforeRemove(CComponent *pComp);
};

END_GUI

#endif
