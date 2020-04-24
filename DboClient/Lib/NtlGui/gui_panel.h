#ifndef __GUI_PANEL_H__
#define __GUI_PANEL_H__

#include "gui_component.h"
#include "gui_surface.h"

START_GUI

class CPanel_Generic;

//: Frame component
class CPanel : public CComponent
{
//! Construction:
public:
	//: Panel Constructor
	CPanel(CComponent *pParent,
				CSurfaceManager *pSurfaceManager);

	//: Panel Constructor
	CPanel(const CRectangle &rtPos,
			CComponent *pParent,
			CSurfaceManager *pSurfaceManager);

	//: Panel Constructor
	CPanel(const CRectangle &rtPos,
				CComponent *pParent,
				CSurfaceManager *pSurfaceManager,
				CSurface surface);

	//: Panel Destructor
	virtual ~CPanel();

//! Operations:
public:
	//: panel surface
	VOID AddSurface(CSurface surface);

    // : Clear All surface
    VOID ClearSurface();

	//: change panel texture
	VOID ChangeTexture(int idx, const char* filename);

	////: panel surface color
	//VOID SetColor(BYTE byRed,BYTE byGreen,BYTE byBlue);

	////: panel surface alpha
	//VOID SetAlpha(BYTE byAlpha);

	//: panel surface angle
	VOID SetAngle(float fAngle);

	VOID SetBlending(DWORD dwBlend);

	//: panel alpha 원본으로 복귀...
	VOID RestoreAlpha(VOID);

	//: Enable Fill
	VOID EnableFill(bool bEnabled = true);

	//: Resize Surface
	VOID ResizeSurface(int nWidth, int nHeight);
	
//! Attributes:
public:
	//: Is fill enabled
	bool IsFillEnabled(VOID) const;

	BYTE GetAlpha(VOID);

	//: Returns surface pointer
	std::list<CSurface>* GetSurface(VOID);
	
//! Implementation:
private:

	VOID ResizeSurface(VOID);

//! Implementation:
private:

	VOID OnSetOptions(const CComponentOptions& options);
	VOID OnPaint(VOID);
	VOID OnMove(int nX,int nY);

	CSlot m_SlotSetOption;
	CSlot m_SlotPaint;
	CSlot m_SlotMove;

public:

	CPanel(const CPanel &copy) : CComponent(NULL,NULL) { return; } // disallow copy construction.
	CPanel_Generic *m_pImpl;
};  

END_GUI

#endif
