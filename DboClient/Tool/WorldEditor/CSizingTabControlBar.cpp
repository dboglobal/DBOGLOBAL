// SizingTabCtrlBar.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "resource.h"
#include "CSizingTabControlBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSizingTabCtrlBar

CSizingTabControlBar::CSizingTabControlBar()
{
	m_nActiveTab = 0;	
}

CSizingTabControlBar::~CSizingTabControlBar()
{
	this->m_font.DeleteObject();

	while(!m_views.IsEmpty())
	{
		TCB_ITEM *pMember=m_views.RemoveHead();
		delete pMember;
	}
}

BEGIN_MESSAGE_MAP(CSizingTabControlBar, CSizingControlBar)
	//{{AFX_MSG_MAP(CSizingTabCtrlBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDI_TABCTRLBAR, OnTabSelChange)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()		
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()	
	ON_WM_RBUTTONUP()		
	ON_WM_WINDOWPOSCHANGED()	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSizingTabControlBar::DoDataExchange(CDataExchange* pDX)
{
	CSizingControlBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SetPlayerDialog)	
	//}}AFX_DATA_MAP
}

/////////////////////////////////////////////////////////////////////////////
// CSizingTabCtrlBar 
int CSizingTabControlBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	//Create the Tab Control
	CRect rect;
	m_tabctrl.Create( WS_VISIBLE | WS_CHILD | TCS_FIXEDWIDTH , rect, this, IDI_TABCTRLBAR);	
	m_tabctrl.EnableToolTips();
	
	//m_tabctrl.SetMinTabWidth( 10 );
	
//	m_images.Create(IDB_TABIMAGE, 32, 1, RGB(0,255,0));
//	m_tabctrl.SetImageList(&m_images);

	m_font.CreateFont( 
         6, // nHeight 
         6, // nWidth 
         0, // nEscapement 
         0, // nOrientation 
         0, // nWeight 
         0, // bItalic 
         0, // bUnderline 
         0, // cStrikeOut 
         0, // nCharSet 
         OUT_DEFAULT_PRECIS, // nOutPrecision 
         0, // nClipPrecision 
         DEFAULT_QUALITY, // nQuality 
         DEFAULT_PITCH | FF_DONTCARE, // nPitchAndFamily 
         "MS Sans Serif" // lpszFacename 
    );    
    
	// set "normal" GUI-font
//	CFont *font = CFont::FromHandle((HFONT)::GetStockObject(DEFAULT_GUI_FONT));	
	m_tabctrl.SetFont(&m_font);

	CSize size( 50, 18 );
	m_tabctrl.SetItemSize( size );

	return 0;
}

void CSizingTabControlBar::OnSize(UINT nType, int cx, int cy) 
{	
	CSizingControlBar::OnSize(nType, cx, cy);
		
	int bottom = (IsHorz() || IsFloating()) ? cy - 14: cy - 18;

	m_tabctrl.MoveWindow(3, 10, cx - 7, bottom );

    for (POSITION pos=m_views.GetHeadPosition(); pos; m_views.GetNext(pos) )
    {
		 CWnd *pWnd =m_views.GetAt(pos)->pWnd;
		 pWnd->MoveWindow(4, 38, cx-9, bottom - 31);
    }

	Invalidate( TRUE );
}


int CSizingTabControlBar::AddViewes( void )
{
	return 0;
}


BOOL CSizingTabControlBar::AddView(LPCTSTR lpszLabel, CRuntimeClass *pViewClass, CCreateContext *pContext)
{	

#ifdef _DEBUG
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));
#endif

	CCreateContext context;
	if (pContext == NULL)
	{
		// if no context specified, generate one from the currently selected
		//  client if possible
/*		CView* pOldView = NULL;
		if (pOldView != NULL && pOldView->IsKindOf(RUNTIME_CLASS(CView)))
		{
			// set info about last pane
			ASSERT(context.m_pCurrentFrame == NULL);
			context.m_pLastView = pOldView;
			context.m_pCurrentDoc = pOldView->GetDocument();
			if (context.m_pCurrentDoc != NULL)
				context.m_pNewDocTemplate =
				context.m_pCurrentDoc->GetDocTemplate();
		}
*/

//		CFrameWnd *pFrameWnd = (CFrameWnd*)AfxGetMainWnd();
		CDocument* pDoc = ((CFrameWnd*)AfxGetMainWnd())->GetActiveDocument();
		context.m_pCurrentDoc = pDoc;
				
		if (context.m_pCurrentDoc != NULL)
		{
//			POSITION viewPos = pDoc->GetFirstViewPosition();
//			CXCViewerView *pView = (CXCViewerView *)pDoc->GetNextView(viewPos);			
//			pMainFrame->SetActiveView (pView);
		
	        context.m_pNewDocTemplate = context.m_pCurrentDoc->GetDocTemplate();
		}

		pContext = &context;

	}
	
	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
			AfxThrowMemoryException();
	}
	CATCH_ALL(e)
	{
		TRACE0("Out of memory creating a view.\n");
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL
		
    ASSERT_KINDOF(CWnd, pWnd);
	ASSERT(pWnd->m_hWnd == NULL);       // not yet created
	
	DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	CRect rect;
	// Create with the right size and position
	if (!pWnd->Create(NULL, NULL, dwStyle, rect, this, 0, pContext))
	{
		TRACE0("Warning: couldn't create client pane for view.\n");
		// pWnd will be cleaned up by PostNcDestroy
		return FALSE;
	}

	//pWnd->SetWindowPos(&CWnd::wndTop, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOMOVE | SWP_NOREDRAW);

	m_pActiveView = (CView*) pWnd;

	TCB_ITEM *pMember=new TCB_ITEM;
	pMember->pWnd=pWnd;
	strcpy(pMember->szLabel, lpszLabel);
	m_views.AddTail(pMember);

	int nViews = static_cast<int>(m_views.GetCount());
	if (nViews!=1)
	{
		//pWnd->EnableWindow(FALSE);
		//pWnd->ShowWindow(SW_HIDE);
        pWnd->SetWindowPos(&m_tabctrl, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW );
	}
	else
	{
		//((CFrameWnd *)GetParent())->SetActiveView((CView *)m_pActiveView);
		if( pContext->m_pNewDocTemplate )
			if(pContext->m_pNewDocTemplate->IsKindOf(RUNTIME_CLASS(CMultiDocTemplate)))
					  ((CFrameWnd *)GetParent())->SetActiveView((CView *)m_pActiveView);
	}

	TC_ITEM tci;
	tci.mask = TCIF_TEXT; /* TCIF_IMAGE;*/
	tci.pszText = (LPTSTR)(LPCTSTR)lpszLabel;
	tci.iImage = nViews-1;
	
	m_tabctrl.InsertItem(nViews, &tci);

	return TRUE;
}

void CSizingTabControlBar::RemoveView(int nView)
{
	ASSERT_VALID(this);
	ASSERT(nView >= 0);

	// remove the page from internal list
	m_views.RemoveAt(m_views.FindIndex(nView));
}

void CSizingTabControlBar::OnTabSelChange(NMHDR* pNMHDR, LRESULT* pResult)
{
	SetActiveView(m_tabctrl.GetCurSel());	
}

void CSizingTabControlBar::SetActiveView(int nNewTab)
{
	ASSERT_VALID(this);
	ASSERT(nNewTab >= 0);

	//CXCViewerView* pView = ((CMainFrame*)AfxGetMainWnd())->pMainView;

	if (nNewTab!=-1 && nNewTab!=m_nActiveTab)
	{
        TCB_ITEM *newMember=m_views.GetAt(m_views.FindIndex(nNewTab));
        TCB_ITEM *oldMember=NULL;
		
        if (m_nActiveTab!=-1)
        {
            oldMember=m_views.GetAt(m_views.FindIndex(m_nActiveTab));
            //oldMember->pWnd->EnableWindow(FALSE);
			oldMember->pWnd->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_HIDEWINDOW | SWP_NOZORDER);           
            //oldMember->pWnd->ShowWindow(SW_HIDE);
        }
        //newMember->pWnd->EnableWindow(TRUE);
        //newMember->pWnd->ShowWindow(SW_SHOW);
        //newMember->pWnd->SetFocus();
        newMember->pWnd->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW );

        m_pActiveView = (CView *)newMember->pWnd;
		((CFrameWnd *)GetParent())->SetActiveView(m_pActiveView);

        m_nActiveTab = nNewTab;
		// select the tab (if tab programmatically changed)
		m_tabctrl.SetCurSel(m_nActiveTab);
    }
	else
	{
//		if( pView )
		{
			// Redraw view
		}
		return;
	}
	
	// Tab change process...
/*	if( m_nActiveTab == 0 )
	{
		if( g_TrackEditView )
		{
			g_TrackEditView->SetCurrentActionHelperParameter();
			g_TrackEditView->SetPropertiesWindow();
		}
	}
	
	if( g_pResourceView ) g_pResourceView->RedrawWindow();	
	if( g_pLeftView ) g_pLeftView->Draw3DScene();
	if( g_pRightView ) g_pRightView->Draw3DScene();
*/
}

void CSizingTabControlBar::SetActiveView(CRuntimeClass *pViewClass)
{
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	int nNewTab = 0;
	for (POSITION pos=m_views.GetHeadPosition(); pos; m_views.GetNext(pos))
	{
		TCB_ITEM *pMember=m_views.GetAt(pos);
		if (pMember->pWnd->IsKindOf(pViewClass))
		{
			//first hide old first view
            m_pActiveView->EnableWindow(FALSE);
            m_pActiveView->ShowWindow(SW_HIDE);
			
			// set new active view
			m_pActiveView = (CView*)pMember->pWnd;
			// enable, show, set focus to new view
			m_pActiveView->EnableWindow(TRUE);
			m_pActiveView->ShowWindow(SW_SHOW);
			m_pActiveView->SetFocus();
			
			((CFrameWnd *)GetParent())->SetActiveView(m_pActiveView);

	        m_nActiveTab = nNewTab;
			// select the tab
			m_tabctrl.SetCurSel(m_nActiveTab);

			break;
		}
		nNewTab++;
    }
}

CView* CSizingTabControlBar::GetActiveView()
{
	return m_pActiveView;
}

CView* CSizingTabControlBar::GetView(int nView)
{
	ASSERT_VALID(this);
	ASSERT(nView >= 0);

	if (nView!=-1)
	{
        TCB_ITEM *pMember=m_views.GetAt(m_views.FindIndex(nView));
		return (CView*)pMember->pWnd;
	}
	else
		return NULL;
}

CView* CSizingTabControlBar::GetView(CRuntimeClass *pViewClass)
{
	ASSERT_VALID(this);
	ASSERT(pViewClass != NULL);
	ASSERT(pViewClass->IsDerivedFrom(RUNTIME_CLASS(CWnd)));
	ASSERT(AfxIsValidAddress(pViewClass, sizeof(CRuntimeClass), FALSE));

	for (POSITION pos=m_views.GetHeadPosition(); pos; m_views.GetNext(pos))
	{
		TCB_ITEM *pMember=m_views.GetAt(pos);
		if (pMember->pWnd->IsKindOf(pViewClass))
		{
			return (CView*)pMember->pWnd;
		}
    }

	return NULL;
}


BOOL CSizingTabControlBar::OnEraseBkgnd(CDC* pDC)
{
	return CWnd::OnEraseBkgnd(pDC);
}

void CSizingTabControlBar::OnPaint()
{	
	CSizingControlBar::OnPaint();
}

void CSizingTabControlBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	CSizingControlBar::OnMouseMove(nFlags, point);	 
}

void CSizingTabControlBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CSizingControlBar::OnLButtonDblClk(nFlags, point);
}

void CSizingTabControlBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	 CSizingControlBar::OnLButtonDown(nFlags, point);
}

void CSizingTabControlBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	 CSizingControlBar::OnLButtonUp(nFlags, point);
}

void CSizingTabControlBar::OnRButtonDown(UINT nFlags, CPoint point)
{
	 CSizingControlBar::OnRButtonDown(nFlags, point);
}

void CSizingTabControlBar::OnRButtonUp(UINT nFlags, CPoint point)
{
	 CSizingControlBar::OnRButtonUp(nFlags, point);
}

void CSizingTabControlBar::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CSizingControlBar::OnWindowPosChanged(lpwndpos);
}