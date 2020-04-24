/******************************************************************************
* File			: CCommunityBase.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 3. 13
* Abstract		: 
*****************************************************************************
* Desc			: 커뮤니티 다이얼로그의 관련 탭 base class
*****************************************************************************/

#pragma once

#include "NtlPLGui.h"

class CCommunityBase
{
public:
	virtual ~CCommunityBase(VOID) {}

	virtual RwBool		Create(CNtlPLGui* pParent) = 0;
	virtual VOID		Destroy() = 0;

	virtual VOID		SwitchDialog(bool bOpen) = 0;

	virtual VOID		Clear() = 0;
	virtual VOID		ActiveTab(bool bActive) = 0;

	virtual VOID		HandleEvents( RWS::CMsg &msg ) = 0;

	virtual VOID		OnMouseDown(const CKey& key) = 0;
	virtual VOID		OnMouseUp(const CKey& key) = 0;
	virtual VOID		OnMove(RwInt32 nX, RwInt32 nY) = 0;
	virtual VOID		OnMouseMove(RwInt32 nFlags, RwInt32 nX, RwInt32 nY) = 0;
	virtual VOID		OnMouseLeave(gui::CComponent* pComponent) = 0;

	virtual VOID		OnPaint() = 0;
};