#include "precomp_dboclient.h"
#include "Windowby3.h"

// core
#include "NtlDebug.h"

VOID CWindowby3::SetType(RwUInt8 byWinType)
{
	m_byWinType = byWinType;
}

VOID CWindowby3::SetRect(const CRectangle& rect)
{
	SetRectWH(rect.left, rect.top, rect.GetWidth(), rect.GetHeight());
}

VOID CWindowby3::SetRectWH(RwInt32 iPosX, RwInt32 iPosY, RwInt32 iWidth, RwInt32 iHeight)
{
	if( m_byWinType == WT_VERTICAL )
	{
		m_Window[0].SetRectWH(iPosX, iPosY, m_Window[0].GetWidth(), iHeight);
		m_Window[1].SetRectWH(iPosX + m_Window[0].GetWidth(), iPosY, iWidth - m_Window[0].GetWidth() - m_Window[2].GetWidth(), iHeight);
		m_Window[2].SetRectWH(iPosX + m_Window[0].GetWidth() + m_Window[1].GetWidth(), iPosY, m_Window[2].GetWidth(), iHeight);
	}
	else
	{
		m_Window[0].SetRectWH(iPosX, iPosY, iWidth, m_Window[0].GetHeight());
		m_Window[1].SetRectWH(iPosX, iPosY + m_Window[0].GetHeight(), iWidth, iHeight - m_Window[0].GetHeight() - m_Window[2].GetHeight());
		m_Window[2].SetRectWH(iPosX, iPosY + m_Window[0].GetHeight() + m_Window[1].GetHeight(), iWidth, m_Window[2].GetHeight());
	}	
}

VOID CWindowby3::SetSize(RwUInt32 uiWidth, RwUInt32 uiHeight)
{
	m_bSetSize = TRUE;

	// 윈도우 위치 조정
	CRectangle rect[WINDOWS_NUM];
	m_Window[0].GetRect(rect[0]);
	m_Window[1].GetRect(rect[1]);
	m_Window[2].GetRect(rect[2]);

	if( m_byWinType == WT_VERTICAL )
	{
		RwUInt32 uiCenterWidth = uiWidth - m_Window[0].GetWidth() - m_Window[2].GetWidth();

		m_Window[0].SetSize(rect[0].GetWidth(), uiHeight);
		m_Window[0].GetRect(rect[0]);

		m_Window[1].SetRectWH(rect[0].right, rect[0].top, uiCenterWidth, uiHeight);
		m_Window[1].GetRect(rect[1]);

		m_Window[2].SetRectWH(rect[1].right, rect[1].top, rect[2].GetWidth(), uiHeight);
	}
	else
	{
		RwUInt32 uiCenterHeight = uiHeight- m_Window[0].GetHeight() - m_Window[2].GetHeight();

		m_Window[0].SetSize(uiWidth, m_Window[0].GetHeight());
		m_Window[0].GetRect(rect[0]);

		m_Window[1].SetRectWH(rect[0].left, rect[0].bottom, uiWidth, uiCenterHeight);
		m_Window[1].GetRect(rect[1]);

		m_Window[2].SetRectWH(rect[1].left, rect[1].bottom, uiWidth, rect[2].GetHeight() );
	}
}

VOID CWindowby3::SetAlpha(RwUInt8 byAlpha)
{
	for(RwInt8 i = 0 ; i < WINDOWS_NUM ; ++i)
	{
		m_Window[i].SetAlpha(byAlpha);
	}
}

VOID CWindowby3::SetSurface(RwInt8 byIndex, gui::CSurface& surface)
{
	m_Window[byIndex].SetSurface(surface);
}

VOID CWindowby3::SetPosition(RwInt32 iPosX, RwInt32 iPosY)
{
	RwInt32 iAdjustX = 0;

	for(RwInt8 i = 0 ; i < WINDOWS_NUM ; ++i)
	{
		if( m_byWinType == WT_VERTICAL )
		{
			m_Window[i].SetPosition(iPosX + iAdjustX, iPosY);
			iAdjustX += m_Window[i].GetWidth();
		}
		else
		{
			m_Window[i].SetPosition(iPosX, iPosY + iAdjustX);
			iAdjustX += m_Window[i].GetHeight();
		}
	}
}

VOID CWindowby3::SetPositionfromParent(RwInt32 iPosX, RwInt32 iPosY)
{
	NTL_ASSERT(m_bSetSize, "CWindowby3::SetPositionfromParent, Not excute CWindowby3::SetSize");

	CRectangle rect;
	RwInt32 iAdjust = 0;

	for(RwInt8 i = 0 ; i < WINDOWS_NUM ; ++i)
	{
		m_Window[i].GetRect(rect);

		if( m_byWinType == WT_VERTICAL )
		{
			m_Window[i].SetPositionfromParent(iPosX + iAdjust, iPosY);
			iAdjust += m_Window[i].GetWidth();
		}
		else
		{
			m_Window[i].SetPositionfromParent(iPosX, iPosY + iAdjust);		
			iAdjust += m_Window[i].GetHeight();
		}
	}
}

VOID CWindowby3::SetPositionbyParent(RwInt32 iParentPosX, RwInt32 iParentPosY)
{
	for(RwInt8 i = 0 ; i < WINDOWS_NUM ; ++i)
		m_Window[i].SetPositionbyParent(iParentPosX, iParentPosY);
}

RwInt32 CWindowby3::GetPositionX()
{
	CRectangle rect;
	m_Window[0].GetRect(rect);

	return rect.left;
}

RwInt32 CWindowby3::GetPositionY()
{
	CRectangle rect;
	m_Window[0].GetRect(rect);

	return rect.top;
}

RwInt32 CWindowby3::GetWidth()
{
	if( m_byWinType == WT_VERTICAL )
		return m_Window[0].GetWidth() + m_Window[1].GetWidth() + m_Window[2].GetWidth();
	else
		return m_Window[0].GetWidth();
}

RwInt32 CWindowby3::GetHeight()
{
	if( m_byWinType == WT_VERTICAL )
		return m_Window[0].GetHeight();
	else
		return m_Window[0].GetHeight() + m_Window[1].GetHeight() + m_Window[2].GetHeight();
}

CPos CWindowby3::GetPosfromParent()
{
	return m_Window[0].GetPosfromParent();
}

VOID CWindowby3::Show(bool bShow)
{
	for(RwInt8 i = 0 ; i < WINDOWS_NUM ; ++i)
		m_Window[i].Show(bShow);
}

VOID CWindowby3::Render(bool bTopRender /* = false */)
{
	for(RwInt8 i = 0 ; i < WINDOWS_NUM ; ++i)
		m_Window[i].Render(bTopRender);
}

RwBool CWindowby3::PtInRect(int iPosX, int iPosY)
{
	CRectangle rect_0, rect_2;

	m_Window[0].GetRect(rect_0);
	m_Window[2].GetRect(rect_2);

	if( rect_0.left <= iPosX && iPosX <= rect_2.right &&
		rect_0.top <= iPosY && iPosY <= rect_2.bottom )
		return TRUE;

	return FALSE;
}