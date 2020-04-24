////////////////////////////////////////////////////////////////////////////////
// Name: class gui:CDialog
// Desc: Dialog. 
//
// 2006.06.8 Peessi@hitel.net   - DynamicOutline 접합 및 정리.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef __GUI_DIALOG_H__
#define __GUI_DIALOG_H__

#include "gui_component.h"
#include "gui_surface.h"

START_GUI

class CDialog_Generic;

//: Dialog component
class CDialog : public CComponent
{
//! Construction:
public:
	//: Dialog Constructor
	CDialog( CComponent* pParent, CSurfaceManager* pSurfaceManager );
	CDialog( const CRectangle& rtPos, CComponent* pParent, CSurfaceManager* pSurfaceManager, bool bDynamic = FALSE );
	CDialog( const CRectangle &rtPos, CComponent* pParent, CSurfaceManager* pSurfaceManager, CSurface surface, bool bDynamic = FALSE );
	//: Dialog Destructor
	virtual ~CDialog();

//! Attributes:
	//: Returns surface pointer
	std::list<CSurface>* GetSurface(VOID);

//! Operations:
	VOID SetFrameSize(const CRectangle& rtRect);
	VOID AddSurface(CSurface surface);
	VOID AddDynamicSurfaces( CSurface& surCenter, CSurface& surLTC, CSurface& surRTC, CSurface& surLBC, CSurface& surRBC,
							 CSurface& surLL, CSurface& surTL, CSurface& surRL, CSurface& surBL );
	VOID SetMovable(BOOL bMovable);
	
// virtual operator
	VOID TopTabOrder(VOID);
	
private:
//! Implementation:
	
//! Signals

protected:
//! Variables
	CDialog_Generic *m_pImpl;

private:
//! Etc
	CDialog(const CPanel &copy) : CComponent(NULL,NULL) { return; } // disallow copy construction.
	
	friend class CDialog_Generic;	
};  

END_GUI

#endif
