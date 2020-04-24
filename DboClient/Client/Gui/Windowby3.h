/******************************************************************************
* File			: CWindowby3.h
* Author		: Hong SungBock
* Copyright		: (주)NTL
* Date			: 2006. 12. 8
* Abstract		: 
* Update		: 
*****************************************************************************
* Desc			: 큰 윈도우의 리소스를 수평 혹은 수직으로 3등분한 것을 표현한다
*				  스크립트로 미리 사이즈를 지정하지 않았다면 SetRect, SetRectWH,
*				  SetSize등을 호출한 뒤에 SetPositionfromParent함수를 호출하자
*****************************************************************************/

#pragma once

#include "gui_Surface.h"
#include "SurfaceGui.h"

class CWindowby3
{
public:
	enum
	{
		WINDOWS_NUM = 3
	};

	enum eWinType
	{
		WT_VERTICAL,
		WT_HORIZONTAL
	};

	CWindowby3() : m_byWinType(WT_VERTICAL), m_bSetSize(false) {}
	CWindowby3(const CWindowby3& windows) {}
	virtual ~CWindowby3() {}

	VOID			SetType(RwUInt8 byWinType);

	VOID			SetRect(const CRectangle& rect);
	VOID			SetRectWH(RwInt32 iPosX, RwInt32 iPosY, RwInt32 iWidth, RwInt32 iHeight);
	VOID			SetSize(RwUInt32 uiWidth, RwUInt32 uiHeight);
	VOID			SetAlpha(RwUInt8 byAlpha);
	VOID			SetSurface(RwInt8 byIndex, gui::CSurface& surface);	///< 서페이스 등록
	VOID			SetPosition(RwInt32 iPosX, RwInt32 iPosY);			///< 윈도우의 형태를 유지한체 옮긴다
	VOID			SetPositionfromParent(RwInt32 iPosX, RwInt32 iPosY);
	VOID			SetPositionbyParent(RwInt32 iParentPosX, RwInt32 iParentPosY);

	RwInt32			GetPositionX();
	RwInt32			GetPositionY();
	RwInt32			GetWidth();
	RwInt32			GetHeight();
	CPos			GetPosfromParent();

	VOID			Show(bool bShow);
	VOID			Render(bool bTopRender = false);

	RwBool			PtInRect(int iPosX, int iPosY);

protected:
	CSurfaceGui			m_Window[WINDOWS_NUM];
	RwUInt8				m_byWinType;
	RwBool				m_bSetSize;
};