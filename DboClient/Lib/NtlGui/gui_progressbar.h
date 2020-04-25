#ifndef __GUI_PROGRESSBAR_H__
#define __GUI_PROGRESSBAR_H__

#include "gui_component.h"
#include "gui_surface.h"

START_GUI

class CProgressBar_Generic;

//: Frame component
class CProgressBar : public CComponent
{
//! Construction:
public:
	//: Panel Constructor
	CProgressBar(CComponent *pParent,
							CSurfaceManager *pSurfaceManager);

	//: Panel Constructor
	CProgressBar(const CRectangle &rtPos,
							CComponent *pParent,
							CSurfaceManager *pSurfaceManager);

	//: ProgressBar Constructor
	CProgressBar(const CRectangle &rtPos,
							CComponent *pParent,
							CSurfaceManager *pSurfaceManager,
							CSurface surface);

	//: Panel Destructor
	virtual ~CProgressBar();

//! Attributes:
public:

	//: ProgressBar surface
	VOID AddSurface(CSurface surface);
	VOID ClearSurface(VOID);
		
//! Operations:
public:

	//: Sets the progressbar range.
	VOID SetRange(int nMin,int nMax);

	//: Returns the progressbar range
	VOID GetRange(int& nMin,int& nMax);

	//: Set the current position
	VOID SetPos(int nPos);

	//: Returns the progressbar current position
	int GetPos(VOID);

	//: Returns surface pointer
	std::list<CSurface>* GetSurface(VOID);	
	
//! Implementation:

private:

	VOID SetHorPos(int nPos);
	VOID SetVerPos(int nPos);

	VOID ResizeSurface(VOID);

// Signals
private:

	struct PROGRESS_BUFFER
	{
		int nValue;
		float fUV;
	};

	VOID OnSetOptions(const CComponentOptions& options);
	VOID OnPaint(VOID);
	VOID OnMove(int nX,int nY);

	CSlot m_SlotSetOption;
	CSlot m_SlotPaint;
	CSlot	m_SlotMove;

public:

	CProgressBar(const CProgressBar &copy) : CComponent(NULL,NULL) { return; } // disallow copy construction.
	CProgressBar_Generic *m_pImpl;
};  

END_GUI

#endif
