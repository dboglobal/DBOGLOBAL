/******************************************************************************
* File			: GMSideIconGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 11. 17
* Abstract		: 
*****************************************************************************
* Desc			: GM 사이드 아이콘
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "SideIconGui.h"

class CGMSideIconGui : public CSideIconBase
{
public:
	CGMSideIconGui(const RwChar* pName);
	virtual ~CGMSideIconGui();

	RwBool		Create();
	VOID		Destroy();

	virtual VOID	OnSideViewClosed();
	virtual VOID	OnIconButtonClicked(gui::CComponent* pComponent);	

protected:		
	gui::CSlot			m_slotIconButton;

	gui::CButton*		m_pIconButton;
};


class CGMSideViewGui : public CSideViewBase
{
public:
	CGMSideViewGui(const RwChar* pName);
	virtual ~CGMSideViewGui();

	RwBool		Create();
	VOID		Destroy();

	virtual VOID	OnPressESC();
	virtual VOID	OnSideViewOpen(const void* pData);
	virtual VOID	OnSideViewClose();
	virtual VOID	OnSideViewLocate(const CRectangle& rectSideIcon);	
};