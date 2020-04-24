#ifndef __GUI_FRAME_H__
#define __GUI_FRAME_H__

#include "gui_component.h"
#include "gui_surface.h"

START_GUI

class CFrame_Generic;
class CSurfaceManager;

//: Frame component
class CFrame : public CComponent
{
//! Construction:
public:
	//: Frame Constructor
	CFrame(CComponent *pParent,
					CSurfaceManager *pSurfaceManager);

	//: Frame Constructor
	CFrame(const CRectangle &rtPos,
			CComponent *pParent,
			CSurfaceManager *pSurfaceManager);

	//: Frame Destructor
	virtual ~CFrame();

//! Attributes:
public:
	//: Is fill enabled
	bool IsFillEnabled(VOID) const;

	//: frame surface
	VOID AddSurface(CSurface surface);

//! Operations:
public:
	//: Enable Fill
	VOID EnableFill(bool bEnabled = true);

	//: Returns surface list
	std::list<CSurface>* GetSurface(VOID);

//! Implementation:

private:

	VOID ResizeSurface(VOID);

//! Signals:
private:

	VOID OnPaint(VOID);
	VOID OnMove(int nX,int nY);

	CSlot m_SlotPaint;
	CSlot m_SlotMove;

private:

	CFrame(const CFrame &copy) : CComponent(NULL,NULL) { return; } // disallow copy construction.
	CFrame_Generic *m_pImpl;
};  

END_GUI

#endif
