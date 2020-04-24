#ifndef __GUI_ANIPANEL_H__
#define __GUI_ANIPANEL_H__

#include "gui_component.h"
#include "gui_surface.h"

START_GUI

class CAniPanel_Generic;

//: Ani Panel component
class CAniPanel : public CComponent
{
//! Construction:
public:
	//: Panel Constructor
	CAniPanel(CComponent *pParent,
						CSurfaceManager *pSurfaceManager);

	//: Panel Constructor
	CAniPanel(const CRectangle &rtPos,
						CComponent *pParent,
						CSurfaceManager *pSurfaceManager);

	//: Panel Constructor
	CAniPanel(const CRectangle &rtPos,
						CComponent *pParent,
						CSurfaceManager *pSurfaceManager,
						CSurface surface);

	//: Panel Destructor
	virtual ~CAniPanel();

//! Attributes:
public:
	//: Is fill enabled
	bool IsFillEnabled(VOID) const;

	//: panel surface
	VOID AddSurface(CSurface surface);
		
//! Operations:
public:

	//: Enable Fill
	VOID EnableFill(bool bEnabled = true);

	//: Set Delta time 
	VOID SetDeltaTime(DWORD dwDelta);

	//: Set Animation
	VOID SetAnimation(bool bAni);

	//: Returns surface pointer
	std::list<CSurface>* GetSurface(VOID);

public:

	VOID OnPaint(VOID);
	
//! Implementation:
private:

	VOID ResizeSurface(VOID);


//! Implementation:
private:

	VOID OnSetOptions(const CComponentOptions& options);
	VOID OnMove(int nX,int nY);

	CSlot m_SlotSetOption;
	CSlot m_SlotPaint;
	CSlot	m_SlotMove;

public:

	CAniPanel(const CAniPanel &copy) : CComponent(NULL,NULL) { return; } // disallow copy construction.
	CAniPanel_Generic *m_pImpl;
};  

END_GUI

#endif
