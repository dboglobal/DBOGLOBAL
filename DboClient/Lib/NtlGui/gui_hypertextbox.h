#ifndef __GUI_HYPERTEXTBOX_H__
#define __GUI_HYPERTEXTBOX_H__

#include "gui_component.h"
#include "gui_surface.h"


START_GUI

#define HYPERTEXTBOX_SCROLLBAR_SIZE		16

class CHyperTextBox_Generic;

//: OutputBox component
class CHyperTextBox : public CComponent
{
//! Construction:
public:

	//: OutputBox Constructor
	CHyperTextBox(CComponent *pParent,
								CSurfaceManager *pSurfaceManager,
								DWORD dwStyle=0);

	//: OutputBox Constructor
	CHyperTextBox(CComponent *pParent,
								CSurfaceManager *pSurfaceManager,
								int nStartRow,						// 출력이 시작될 row
								int nStartColumn,					// 출력이 시작될 column
								int nInterval,
								DWORD dwStyle=0);

	//: OutputBox Constructor
	CHyperTextBox(const CRectangle &rtRect,
								CComponent *pParent,
								CSurfaceManager *pSurfaceManager,
								int nStartRow,						// 출력이 시작될 row
								int nStartColumn,					// 출력이 시작될 column
								int nInterval,
								DWORD dwStyle=0);

	//: OutputBox Destructor
	virtual ~CHyperTextBox();

//! Attributes:
public:
	
	//: Returns the surface used.
	std::list<CSurface>* GetSurface(VOID) const;

	//: Return the vertical scroll bar pointer
	CScrollBar* GetVerticalScrollBar(VOID) const;

//! Operations:
public:

	//: font를 생성한다.
	VOID CreateFont(TCHAR * szFaceName, int nHeight, int nAttributes = 0);

	//: Sets the line edit text to text, clears the selection and moves the cursor to the end of the line.
	//: If necessary the text is truncated to get_max_length().
	VOID SetTextKey(const std::string& key);
	
	//: Set the row
	VOID SetRow (int nRow);

	//: Set the Column
	VOID SetColumn (int nColumn);

	//: Set the Interval
	VOID SetInterval (int nInterval);

	//: Syntactic sugar for set_text("").
	VOID Clear();

	//: Sets the surface.
	VOID AddSurface(CSurface Surface);

//! Signals:
public:

	CSignal_v1<CNode*> &SigNodeChanged(VOID);

public:

	VOID OnSetOptions(const CComponentOptions &options);
	VOID OnPaint(VOID);
	VOID OnMove(int nOldX,int nOldY);

	CSlot m_SlotSetOptions;
	CSlot m_SlotPaint;
	CSlot m_SlotMove;
	
//! Implementation:
private:

	VOID ResizeSurface(VOID);

	CScrollBar *m_pSBVer;

	CHyperTextBox(const CHyperTextBox &copy) : CComponent(NULL, NULL) { return; } // disallow copy construction.
	CHyperTextBox_Generic *m_pImpl;
};

END_GUI

#endif
