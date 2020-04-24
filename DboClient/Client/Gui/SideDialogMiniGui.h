/******************************************************************************
* File			: SideDialogMiniGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2008. 2. 19
* Abstract		: 
*****************************************************************************
* Desc			: 사이드 다이얼로그가 최소화됬을 때의 공통 GUI
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presetation
#include "NtlPLGui.h"


class CSideDialogMiniGui : public CNtlPLGui
{
public:	
	CSideDialogMiniGui(const RwChar* pName);
	virtual ~CSideDialogMiniGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		Active(RwUInt32 uiSideDialogType, const WCHAR* pwcDialogName);
	VOID		Inactive();

	RwUInt32	GetSideDialogType();

protected:
	CSideDialogMiniGui() {}

	VOID			OnClicked_NormalizeButton(gui::CComponent* pComponent);	

protected:
	gui::CSlot			m_slotNormalizeButton;	

	gui::CButton*		m_pNormalizeButton;
	gui::CStaticBox*	m_pDialogName;

	RwUInt32			m_uiSideDialogType;
};