#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "SideIconGui.h"


class CSCSSideIconGui : public CSideIconBase
{
public:
	CSCSSideIconGui( const RwChar* pName );
	virtual ~CSCSSideIconGui();

	RwBool	Create();
	VOID	Destroy();

	virtual VOID	OnSideViewClosed( );
	virtual VOID	OnIconButtonClicked(gui::CComponent* pComponent);
	

protected:
	gui::CSlot		m_slotIconButton;
	gui::CButton*	m_pIconButton;
};

class CSCSSideViewGui : public CSideViewBase
{
public:
	CSCSSideViewGui( const RwChar* pName );
	virtual	~CSCSSideViewGui();

	RwBool			Create();
	VOID			Destroy();

	virtual VOID	OnPressESC();
	virtual VOID	OnSideViewOpen(const void* pData);
	virtual VOID	OnSideViewClose();
	virtual VOID	OnSideViewLocate(const CRectangle& rectSideIcon);
};