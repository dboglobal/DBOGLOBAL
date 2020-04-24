#include "StdAfx.h"
#include "resource.h"
#include "DockingPaneCustomTheme.h"


class CAppearanceSetPropertyPageAdobe : public CXTPTabPaintManager::CAppearanceSetPropertyPage2003
{
public:
	CAppearanceSetPropertyPageAdobe()		
	{
		m_bButtonsReverseZOrder = TRUE;	
		
	}
	void RefreshMetrics()
	{
		CAppearanceSetPropertyPage2003::RefreshMetrics();

		m_nButtonHeight = 15;
		m_rcHeaderMargin.top = 1;

		CreateAdobeButton();
	}
	
	void CreateAdobeButton()
	{
		#define FREE(hHandle) if (hHandle) { free(hHandle); hHandle = 0; }

		m_bmpAdobeTabItem.DeleteObject();

		HBITMAP hBitmap = CXTPImageManagerIcon::LoadAlphaBitmap(IDB_TABITEM);
		ASSERT(hBitmap);		

		CDC dcSrc;
		dcSrc.CreateCompatibleDC(NULL);
		
		PBYTE pBits = 0;
		PBITMAPINFO pBitmapInfo = 0;		
		
		TRY
		{
			UINT nSize;
			if (!CXTPImageManagerIcon::GetBitmapBits(dcSrc, hBitmap, pBitmapInfo, (LPVOID&)pBits, nSize))
				AfxThrowMemoryException();
			
			COLORREF clr = GetXtremeColor(COLOR_3DFACE);
			
			for (UINT i = 0; i < nSize / 2; i += 4)
			{
				if (pBits[i+0] == 0xFF && pBits[i+1] == 0xFF && pBits[i+2] == 0xFF)
				{			
					pBits[i+0] = GetBValue(clr);
					pBits[i+1] = GetGValue(clr);
					pBits[i+2] = GetRValue(clr);
				}
				
			}
			
			SetDIBits(dcSrc, hBitmap, 0, pBitmapInfo->bmiHeader.biHeight, pBits, pBitmapInfo, DIB_RGB_COLORS);
			
		}
		CATCH (CMemoryException, e)
		{
			TRACE(_T("Failed -- Memory exception thrown."));
		}
		END_CATCH
			
		FREE(pBits);
		FREE(pBitmapInfo);

		m_bmpAdobeTabItem.Attach(CXTPImageManagerIcon::PreMultiplyAlphaBitmap(hBitmap));


		DeleteObject(hBitmap);
	}
	
	void _DrawAdobeButtonPart(CDC* pDC, CRect rcDest, CRect rcSrc);
	void DrawAdobeButtonPart(CDC* pDC, CRect rcDest, CRect rcSrc, BOOL bSolidRect = FALSE);
	
	void DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem);

	virtual CRect GetHeaderMargin()
	{
		return CRect(0, m_rcHeaderMargin.top, m_rcHeaderMargin.right + m_nButtonHeight, m_rcHeaderMargin.bottom);
	}

protected:
	CBitmap m_bmpAdobeTabItem;
};


class CColorSetAdobe : public CXTPTabPaintManager::CColorSetOffice2003
{
public:
	CColorSetAdobe()
	{
		
	}
	void RefreshMetrics()
	{
		CColorSetDefault::RefreshMetrics();
		
		m_clrHeaderFace.SetStandardValue(RGB(179, 179, 179));
		m_clrNormalText = RGB(75, 75, 75);
		m_clrSelectedText = 0;
		m_csPropertyPage2003.clrFrameBorder = 0xFFFFFF;
	}

};

CDockingPaneAdobeTheme::CDockingPaneAdobeTheme()
{
	CXTPTabPaintManager* pPaintManager = GetTabPaintManager();
	pPaintManager->SetPosition(xtpTabPositionTop);
	
	m_bDrawSingleTab = TRUE;
	m_bShowCaption = FALSE;
	m_nSplitterSize = 7;
	m_dwSplitterStyle = xtpPaneSplitterGripperOfficeXP | xtpPaneSplitterSoft3D;
	
	pPaintManager->SetAppearanceSet(new CAppearanceSetPropertyPageAdobe());
	pPaintManager->SetColorSet(new CColorSetAdobe());
	pPaintManager->m_bShowIcons = FALSE;
	pPaintManager->m_rcButtonMargin.SetRect(6, 0, -5, 0);
	pPaintManager->m_clientFrame = xtpTabFrameSingleLine;

}

CDockingPaneAdobeTheme::~CDockingPaneAdobeTheme()
{
	
}

void CAppearanceSetPropertyPageAdobe::_DrawAdobeButtonPart(CDC* pDC, CRect rcDest, CRect rcSrc)
{
	if (m_bmpAdobeTabItem.GetSafeHandle() == 0)
		return;

	CXTPImageManagerIcon::DrawAlphaBitmap(pDC, m_bmpAdobeTabItem, rcDest.TopLeft(), rcDest.Size(), rcSrc.TopLeft(), rcSrc.Size());
	
}
void CAppearanceSetPropertyPageAdobe::DrawAdobeButtonPart(CDC* pDC, CRect rcDest, CRect rcSrc, BOOL bSolidRect)
{
	if (rcDest.right <= rcDest.left)
		return;
	
	if (bSolidRect)
	{
		pDC->FillSolidRect(rcDest, GetXtremeColor(COLOR_3DFACE));
	}
	else
	{
		_DrawAdobeButtonPart(pDC, rcDest, CRect(rcSrc.left, rcSrc.top + 15, rcSrc.right, rcSrc.bottom + 15));

	}
	_DrawAdobeButtonPart(pDC, rcDest, rcSrc);
	
}

void CAppearanceSetPropertyPageAdobe::DrawSingleButton(CDC* pDC, CXTPTabManagerItem* pItem)
{
	CRect rc = pItem->GetRect();


	switch(pItem->GetTabManager()->GetPosition())
	{
	case xtpTabPositionTop:
		{
			if (pItem->IsSelected())
			{			
				int x = rc.left, y = rc.top, nWidth = rc.Width() + rc.Height() + 9;
				
				DrawAdobeButtonPart(pDC, CRect(x, y, x + 18, y + 15), CRect(0, 0, 18, 15));
				DrawAdobeButtonPart(pDC, CRect(x + 18, y, x + nWidth - 33, y + 15), CRect(18, 0, 28, 15), TRUE);
				DrawAdobeButtonPart(pDC, CRect(max(x + 18, x + nWidth - 33), y, max(x + 18, x + nWidth - 33) + 33, y + 15), CRect(28, 0, 61, 15));
			}
			else
			{
				int x = rc.left, y = rc.top + 1, nWidth = rc.Width() + rc.Height() + 9;
				
				DrawAdobeButtonPart(pDC, CRect(x, y, x + 19, y + 13), CRect(61, 0, 80, 13));
				DrawAdobeButtonPart(pDC, CRect(x + 19, y, x + nWidth - 28, y + 13), CRect(80, 0, 90, 13), TRUE);
				DrawAdobeButtonPart(pDC, CRect(max(x + 19, x + nWidth - 28), y, max(x + 19, x + nWidth - 28) + 28, y + 13), CRect(90, 0, 118, 13));
			}
		}
		break;

	}

	m_pPaintManager->DrawSingleButtonIconAndText(pDC, pItem, rc, TRUE);
}


//////////////////////////////////////////////////////////////////////////
//
CDockingPaneAdobeContextWnd::CDockingPaneAdobeContextWnd()
{
	m_bAttachContext = FALSE;
	m_bFloatContext = FALSE;
}


BEGIN_MESSAGE_MAP(CDockingPaneAdobeContextWnd, CWnd)
	//{{AFX_MSG_MAP(CXTPDockingPaneContextAlphaWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CXTPDockingPaneContextAlphaWnd message handlers

BOOL CDockingPaneAdobeContextWnd::OnEraseBkgnd(CDC* /*pDC*/)
{
	return TRUE;
}

void CDockingPaneAdobeContextWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);

	dc.FillSolidRect(rc, GetSysColor(COLOR_HIGHLIGHT));
	dc.Draw3dRect(rc, 0, 0);
	
	if (m_bAttachContext)
	{
		rc.DeflateRect(5, 5);
		dc.Draw3dRect(rc, 0, 0);
	}
}

CDockingPaneAdobeContext::CDockingPaneAdobeContext()
{
	
}

void CDockingPaneAdobeContext::InitLoop()
{
	m_rectLast.SetRectEmpty();
	m_sizeLast.cx = m_sizeLast.cy = 0;
	m_bDitherLast = FALSE;
	m_rectStickerPane.SetRectEmpty();
	m_pStickerPane = NULL;
}

void CDockingPaneAdobeContext::CancelLoop()
{
	ReleaseCapture();

	m_wndContext.DestroyWindow();
}




void CDockingPaneAdobeContext::DrawFocusRect(BOOL /*bRemoveRect*/)
{
	CRect rect = (m_pContainer == 0)? m_rectDragFrame: m_rectContainer;
	
	m_rectLast = rect;
	
	if (!m_bFloatable && m_pContainer == 0)
	{
		rect.SetRectEmpty();
	}

	if (m_bAttach != m_bAttachLast)
	{
		m_bAttachLast = m_bAttach;
		m_wndContext.DestroyWindow();
	}

	m_wndContext.m_bAttachContext = m_bAttach;
	m_wndContext.m_bFloatContext = (m_pContainer == 0);
	
	if (m_wndContext.GetSafeHwnd() == 0 || CXTPWindowRect(&m_wndContext).Size() != rect.Size() )
	{
		m_wndContext.DestroyWindow();

		m_wndContext.CreateEx(WS_EX_TOPMOST|WS_EX_TOOLWINDOW, AfxRegisterWndClass(NULL, ::LoadCursor(NULL, IDC_ARROW)),
			0, WS_POPUP, CRect(0, 0, 0, 0), m_pManager->GetSite(), 0);

		if (m_bAttach)
		{
			CRgn rgn;
			rgn.CreateRectRgn(0, 0, rect.Width(), rect.Height());
			
			CRgn rgnInside;
			rgnInside.CreateRectRgn(6, 6, rect.Width() - 6, rect.Height() - 6);

			rgn.CombineRgn(&rgn, &rgnInside, RGN_DIFF );
			
			m_wndContext.SetWindowRgn(rgn, FALSE);
			rgn.Detach();
		}
	}
	
	m_wndContext.SetWindowPos(0, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER|SWP_NOACTIVATE|SWP_SHOWWINDOW);
}



void CDockingPaneAdobeContext::FindContainer(CPoint pt)
{
	CXTPDockingPaneInfoList* pList = &m_pManager->GetPaneList();
	POSITION pos = pList->GetHeadPosition();
	while (pos)
	{
		CXTPDockingPaneBase* pPane = pList->GetNext(pos)->GetContainer();
		if (pPane == NULL)
			continue;

		ASSERT(pPane->GetType() == xtpPaneTypeTabbedContainer);

		if (pPane->GetContainer() == 0 || pPane->GetContainer()->GetType() != xtpPaneTypeSplitterContainer)
			continue;

		if (! (::GetWindowLong(((CXTPDockingPaneTabbedContainer*)pPane)->GetSafeHwnd(), GWL_STYLE) & WS_VISIBLE))
			continue;

		CRect rcClient = m_rectContainer = pPane->GetPaneWindowRect();

		if (m_pPane->ContainPane(pPane))
			continue;

		CRect rcAttach(rcClient);
		rcAttach.DeflateRect(6, 6);

		if (rcAttach.PtInRect(pt))
		{
			if (IsAllowAttachTo(pPane))
			{
				m_pContainer = pPane;
				m_bAttach = TRUE;
				return;
			}
		}

		CRect rcDock(rcClient);
		rcDock.InflateRect(7, 7);

		if (CanDock(rcDock, pt, pPane))
		{
			switch (m_containDirection)
			{
				case xtpPaneDockLeft: m_rectContainer.SetRect(rcDock.left, rcDock.top + 7, rcDock.left + 7, rcDock.bottom - 7); break;
				case xtpPaneDockRight: m_rectContainer.SetRect(rcDock.right - 7, rcDock.top + 7, rcDock.right, rcDock.bottom - 7); break;
				case xtpPaneDockTop: m_rectContainer.SetRect(rcDock.left + 7, rcDock.top, rcDock.right - 7, rcDock.top + 7); break;
				case xtpPaneDockBottom: m_rectContainer.SetRect(rcDock.left + 7, rcDock.bottom - 7, rcDock.right - 7, rcDock.bottom); break;
			}
			return;
		}
	}

	
	CXTPDockingPaneBase* pPane = m_pManager->GetClientPane();
	m_rectContainer = pPane->GetPaneWindowRect();
	if (CanDock(pPane->GetPaneWindowRect(), pt, pPane))
		return;
	
	pPane = m_pManager->GetTopPane();
	m_rectContainer = pPane->GetPaneWindowRect();
	if (CanDock(pPane->GetPaneWindowRect(), pt, pPane, FALSE))
		return;
}