/******************************************************************************
* File			: CBackboardGui.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2007. 8. 8
* Abstract		: 
*****************************************************************************
* Desc			: 지정된 색상과 투명도에 따라 화면 전체를 가리는 사각형을 그린다
*****************************************************************************/

#pragma once

// core
#include "ceventhandler.h"

// presentation
#include "NtlPLGui.h"

// dbo
#include "SurfaceGui.h"


class CBackboardGui : public CNtlPLGui, public RWS::CEventHandler
{
public:
	CBackboardGui(const RwChar* pName);
	virtual ~CBackboardGui();

	RwBool		Create();
	VOID		Destroy();

	VOID		SetColor(RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue);
	VOID		SetAlpha(RwUInt8 byAlpha);

	RwInt32		SwitchDialog(bool bOpen);			///< DialogManager에서의 Open/Close

protected:
	CBackboardGui() {}
	virtual VOID	HandleEvents( RWS::CMsg &msg );

	VOID			OnPaint();

protected:	
	gui::CSlot			m_slotPaint;

	CSurfaceGui			m_srfBack;
};