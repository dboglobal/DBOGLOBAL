#ifndef __GUI_DIALOG_GENERIC_H__
#define __GUI_DIALOG_GENERIC_H__

#include "gui_dialog.h"
#include "gui_surface.h"

START_GUI

class CDialog_Generic
{
public:
// Construction & Destruction
	CDialog_Generic(CDialog *pSelf, BOOL bDynamic);
	~CDialog_Generic();

// Operation:
	VOID AddSurface( CSurface surface );

// Attributes:

// Implementation:
	VOID CreateDynamicOutline(VOID);
	VOID DeleteDynamicOutline(VOID);

	VOID ResizeSurface(VOID);

//! CallBack
	VOID OnResize( INT nOldCX, INT nOldCY );
	VOID OnSetOptions(const CComponentOptions& options);
	VOID OnPaint(VOID);
	VOID OnMove(INT nX,INT nY);
	VOID OnMouseDown(const CKey& key);
	VOID OnMouseUp(const CKey& key);
	VOID OnMouseMove(INT nFlags,INT nX,INT nY);
	VOID OnSetAlpha( BYTE ucAlpha );
	VOID OnSetColor( BYTE ucRed, BYTE ucGreen, BYTE ucBlue );

	CSlot m_slotResize;
	CSlot m_SlotSetOption;
	CSlot m_SlotPaint;
	CSlot m_SlotMove;
	CSlot m_SlotMouseDown;
	CSlot m_SlotMouseUp;
	CSlot m_SlotMouseMove;
	CSlot m_SlotSetAlpha;
	CSlot m_SlotSetColor;
		
public:
//! Variable
	BOOL m_bTitleBar;
	BOOL m_bTracking;
	BOOL m_bMovable;

	DWORD	m_dwTrackingTime;

	CRectangle m_rtFrame;
	CPos m_ptCapturePos;

	std::list<CSurface> m_stlSurface;
	CDynamicOutline* m_pOutline;

	CDialog *m_pDialog;
};  

END_GUI

#endif
