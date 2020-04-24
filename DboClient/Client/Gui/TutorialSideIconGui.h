/******************************************************************************
* File			: TutorialSideIconGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 1. 22
* Abstract		: 
*****************************************************************************
* Desc			: 튜토리얼 사이드 아이콘을 누르면 사이드 뷰를 보여주는 것이
*				  아니라 튜토리얼을 종료할 것인지 묻는 메세지 박스를 띄운다
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "SideIconGui.h"

class CTutorialSideIconGui : public CSideIconBase
{
public:
	CTutorialSideIconGui(const RwChar* pName);
	virtual ~CTutorialSideIconGui();

	RwBool		Create();
	VOID		Destroy();

	virtual VOID	OnSideViewClosed();
	virtual VOID	OnIconButtonClicked(gui::CComponent* pComponent);	

protected:		
	gui::CSlot			m_slotIconButton;

	gui::CButton*		m_pIconButton;
};


class CTutorialSideViewGui : public CSideViewBase
{
public:
	CTutorialSideViewGui(const RwChar* pName);
	virtual ~CTutorialSideViewGui();

	RwBool		Create();
	VOID		Destroy();

	virtual VOID	OnPressESC();
	virtual VOID	OnSideViewOpen(const void* pData);
	virtual VOID	OnSideViewClose();
	virtual VOID	OnSideViewLocate(const CRectangle& rectSideIcon);	
};