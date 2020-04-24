/******************************************************************************
* File			: DojoInfo_Base.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2009. 1. 16
* Abstract		: 
*****************************************************************************
* Desc			: 도장 정보 창의 탭 상위 클래스
*****************************************************************************/

#pragma once

// shared
#include "NtlSharedType.h"

// core
#include "ceventhandler.h"

// presentatione
#include "NtlPLGui.h"

class CDojoInfo_Base
{
public:
	virtual ~CDojoInfo_Base(VOID) {}

	virtual RwBool		Create(CNtlPLGui* pParent) = 0;
	virtual VOID		Update(RwReal fElapsed) = 0;
	virtual VOID		Destroy() = 0;

	virtual VOID		Clear() = 0;
	virtual VOID		ActiveTab(bool bActive) = 0;	

	virtual VOID		OnMouseDown(const CKey& key) = 0;
	virtual VOID		OnMouseUp(const CKey& key) = 0;
	virtual VOID		OnMove(RwInt32 nX, RwInt32 nY) = 0;
	virtual VOID		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY) = 0;
	virtual VOID		OnMouseLeave(gui::CComponent* pComponent) = 0;

	virtual VOID		OnPaint() = 0;

	virtual VOID		SwitchDialog(bool bOpen, RwUInt32 uiDojoTableIndex = INVALID_TBLIDX) = 0;
	virtual VOID		HandleEvents( RWS::CMsg &msg ) = 0;
};