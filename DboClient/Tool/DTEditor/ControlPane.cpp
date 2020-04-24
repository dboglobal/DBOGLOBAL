// ControlPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "ControlPane.h"
#include "MainFrm.h"

#include "DTEditorGlobal.h"
#include "PropertyPane.h"
//

// simulation
#include "precomp_ntlsimulation.h"
#include "NtlDTProp.h"
#include "NtlCameraManager.h"


#define CONTROL_PANE_INFOEDIT_ID	AFX_IDW_PANE_FIRST + 3
#define CONTROL_PANE_AXIS_COMBO_ID	AFX_IDW_PANE_FIRST + 100

LRESULT CControlPane::CControlEdit::WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
{
	switch (message)
	{
	case WM_NCPAINT:
		{
			CEdit::WindowProc(message, wParam, lParam);

			CRect rc;
			GetWindowRect(&rc);
			CWindowDC dc(this);
			rc.OffsetRect(-rc.TopLeft());
			COLORREF clr = GetXtremeColor(XPCOLOR_STATICFRAME);
			dc.Draw3dRect(rc, clr, clr);
			return TRUE;
		}
	}
	return CEdit::WindowProc(message, wParam, lParam);
}

// CControlPane 대화 상자입니다.

IMPLEMENT_DYNAMIC(CControlPane, CDialog)

CControlPane::CControlPane(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CControlPane::IDD, pParent)
, m_dwTabStyle(0)
, m_bBottom(TRUE)
, m_bHasArrows(TRUE)
, m_bHasHomeEnd(FALSE)
, m_bHScroll(TRUE)
, m_pExcelTabCtrl(NULL)

{
	m_pDTSpline		= NULL;
	m_pSplineProp	= NULL;
	UpdateTabsStyle();
}

CControlPane::~CControlPane()
{
}

void CControlPane::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BORDER_TAB, m_staticBorder);
}

void CControlPane::CalcTabRect()
{
	CXTPWindowRect rcBorder(&m_staticBorder);
	ScreenToClient(&rcBorder);
	
	// get the size of the thumb box in a horizontal scroll bar.
	int cy = ::GetSystemMetrics(SM_CXHTHUMB); // SM_CYHSCROLL
	
	CRect rtTabCtrl = rcBorder;
	rtTabCtrl.DeflateRect(2, 2);
	
	CRect rcCtrl = rcBorder;
	rcCtrl.DeflateRect(2, 1, 2, cy + 1);
	
	// Create  the flat tab control.
	m_rtTabCtrl = rtTabCtrl;
}

BOOL CControlPane::CreateExcelTab()
{
	BOOL bResize = FALSE;

	CXTPEmptyRect rTab;
	RemoveResize(IDC_EXCEL_TABCTRL);

	m_pExcelTabCtrl = new CXTExcelTabCtrl();

	// Create  the flat tab control.
	if (!m_pExcelTabCtrl->Create(WS_CHILD|WS_VISIBLE|m_dwTabStyle,
		m_rtTabCtrl, this, IDC_EXCEL_TABCTRL))
	{
		TRACE0( "Failed to create flattab control\n" );
		return FALSE;
	}

	// set the tab control to have a static edge border.
	m_pExcelTabCtrl->ModifyStyleEx(NULL, WS_EX_STATICEDGE);

	if (!CreateControlPointSheet()) return FALSE;
	
	// Insert tabs into the flat tab control.
	m_pExcelTabCtrl->InsertItem(0, _T("Time control"),    m_pCtrlPointSheet);
	
	m_pExcelTabCtrl->SetTabBackColor(0, RGB(0x80,0x00,0x00));
	m_pExcelTabCtrl->SetTabTextColor(0, RGB(0xff,0xff,0xff));
	m_pExcelTabCtrl->SetSelTabBackColor(0, RGB(0xff,0x00,0x00));
	m_pExcelTabCtrl->SetSelTabTextColor(0, RGB(0xff,0xff,0xff));

	m_pExcelTabCtrl->SetWindowPos(&wndTop, 0, 0, 0, 0,
		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_FRAMECHANGED);

	// enable resizing for the flat tab control.
	SetResize(IDC_EXCEL_TABCTRL, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	if (bResize)
	{
		CXTResize::Size();
	}
	
	m_pExcelTabCtrl->SetCurSel(0);

	SetRedraw(TRUE);
	RedrawWindow(0,0, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);

	return TRUE;
}

void CControlPane::UpdateTabsStyle()
{
	
	DWORD dwNewStyle = 0;

	if (m_bHasHomeEnd)
	{
		dwNewStyle |= FTS_XT_HASHOMEEND;
		m_bHasArrows = TRUE;
	}

	if (m_bBottom)
		dwNewStyle |= FTS_XT_BOTTOM;

	if (m_bHasArrows)
		dwNewStyle |= FTS_XT_HASARROWS;

	if (m_bHScroll)
		dwNewStyle |= FTS_XT_HSCROLL;

	dwNewStyle |= FTS_XT_TOOLTIPS;

	if (dwNewStyle != m_dwTabStyle)
	{
		m_dwTabStyle = dwNewStyle;
	}
	
}

BOOL CControlPane::CreateControlPointSheet()
{
	// views must be created dynamically because they are destroyed during
	// window destruction
	CCreateContext cc;
	::ZeroMemory(&cc, sizeof(cc));
	cc.m_pCurrentDoc = NULL;

	m_pCtrlPointSheet = (CControlPointView*)RUNTIME_CLASS(CControlPointView)->CreateObject();
	if (!m_pCtrlPointSheet  || !((CWnd *)m_pCtrlPointSheet)->Create(
		NULL, NULL, WS_CHILD, CXTPEmptyRect(), m_pExcelTabCtrl, IDC_CONTROL_POINT_SHEET, &cc))
	{
		TRACE0( "Failed to create control point sheet.\n" );
		return FALSE;
	}

	m_pCtrlPointSheet->m_pEditDlg	= &m_EditDlg;
	m_pCtrlPointSheet->m_pAttrtDlg	= &m_AttrDlg;

	return TRUE;
}

void CControlPane::Render(float fElapsed)
{
	m_PointRotateDlg.Render(fElapsed);
}


void CControlPane::ActiveSplineProp(CNtlDTProp *pDTProp, void *pPropData)
{
	m_pDTSpline		= (CNtlDTSpline*)pDTProp;
	m_pSplineProp	= (SSplineProp*)pPropData;

	m_GraphDlg.ActiveSplineProp(pDTProp, pPropData);
	m_pCtrlPointSheet->ActiveSplineProp(pDTProp, pPropData);
	m_EditDlg.OnPropUpdate();
	m_AttrDlg.OnPropUpdate();
}

void CControlPane::DeActiveSplineProp(void)
{
	m_GraphDlg.DeActiveSplineProp();
	m_pCtrlPointSheet->DeActiveSplineProp();
	m_EditDlg.OnPropUpdate();
	m_AttrDlg.OnPropUpdate();
}

void CControlPane::DeleteSplineProp(CNtlDTProp *pDTProp)
{
	if(m_pDTSpline == pDTProp)
	{
		m_GraphDlg.DeleteSplineProp(pDTProp);
		m_pCtrlPointSheet->DeleteSplineProp(pDTProp);

		m_pDTSpline = NULL;
		m_pSplineProp = NULL;
	}

	m_EditDlg.OnPropUpdate();
	m_AttrDlg.OnPropUpdate();
}

void CControlPane::RefreshSplineProp(void)
{
	m_EditDlg.OnPropUpdate();
	m_AttrDlg.OnPropUpdate();
}

void CControlPane::RebuildProp()
{
	m_GraphDlg.RebuildProp();
	m_pCtrlPointSheet->RebuildProp();

	m_EditDlg.OnPropUpdate();
	m_AttrDlg.OnPropUpdate();
}

void CControlPane::SplinePropUpdate(void)
{
	m_EditDlg.OnPropUpdate();
	m_AttrDlg.OnPropUpdate();
}


void CControlPane::PointClick(int nSx, int nEx)
{
	m_pCtrlPointSheet->PointClick(nSx, nEx);
	m_EditDlg.OnPropSelectUpdate(nSx, nEx);
	m_AttrDlg.OnPropSelectUpdate(nSx, nEx);
}

void CControlPane::SetInfoText(CString& strInfo)
{
	SetDlgItemText(CONTROL_PANE_INFOEDIT_ID, strInfo.GetBuffer());
}

void CControlPane::ShowHideControlView(void)
{
	m_GraphDlg.ShowWindow(FALSE);
	m_ShapeDlg.ShowWindow(FALSE);
	m_EditDlg.ShowWindow(FALSE);
	m_AttrDlg.ShowWindow(FALSE);
}

BEGIN_MESSAGE_MAP(CControlPane, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_COMMAND(ID_CONTROL_PANE_TIME_CHANGE, OnControlPaneTimeChange)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_PANE_TIME_CHANGE, OnUpdateControlPaneTimeChange)
	ON_COMMAND(ID_CONTROL_PANE_TIME_GRAPH, OnControlPaneTimeGraph)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_PANE_TIME_GRAPH, OnUpdateControlPaneTimeGraph)
	ON_COMMAND(ID_CONTROL_PANE_SHAPE, OnControlPaneShape)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_PANE_SHAPE, OnUpdateControlPaneShape)
	ON_COMMAND(ID_CONTROL_PANE_POINTEDIT, OnControlPanePointEdit)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_PANE_POINTEDIT, OnUpdateControlPanePointEdit)
	ON_COMMAND(ID_CONTROL_PANE_POINTROTATEEDIT, OnControlPanePointRotateEdit)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_PANE_POINTROTATEEDIT, OnUpdateControlPanePointRotateEdit)
	ON_COMMAND(ID_CONTROL_PANE_POINTATTREDIT, OnControlPanePointAttributeEdit)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_PANE_POINTATTREDIT, OnUpdateControlPanePointAttributeEdit)
	ON_COMMAND(ID_CONTROL_PANE_RESET_TRANS, OnControlPaneResetTrans)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_PANE_RESET_TRANS, OnUpdateControlPaneResetTrans)
	ON_COMMAND(ID_CONTROL_PANE_RESET_CAMERA, OnControlPaneResetCamera)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_PANE_RESET_CAMERA, OnUpdateControlPaneResetCamera)
	ON_COMMAND(ID_CONTROL_PANE_REMOVE_POINT, OnControlPaneRemovePoint)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_PANE_REMOVE_POINT, OnUpdateControlPaneRemovePoint)
	ON_COMMAND(ID_CONTROL_PANE_ADD_POINT, OnControlPaneAddPoint)
	ON_UPDATE_COMMAND_UI(ID_CONTROL_PANE_ADD_POINT, OnUpdateControlPaneAddPoint)

	
END_MESSAGE_MAP()


// CControlPane 메시지 처리기입니다.

int CControlPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTResizeDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	

	return 0;
}

BOOL CControlPane::OnInitDialog()
{
	CXTResizeDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	XTPImageManager()->SetMaskColor(RGB(0, 0xFF, 0));
	VERIFY(m_wndToolBar.CreateToolBar(WS_VISIBLE | WS_CHILD | CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_CONTROL_PANE_TOOLBAR));
	m_wndToolBar.GetControl(0)->SetStyle(xtpButtonIconAndCaption);
	m_wndToolBar.GetControl(1)->SetStyle(xtpButtonIconAndCaption);
	m_wndToolBar.GetControl(2)->SetStyle(xtpButtonIconAndCaption);
	m_wndToolBar.GetControl(3)->SetStyle(xtpButtonIconAndCaption);
	m_wndToolBar.GetControl(4)->SetStyle(xtpButtonIconAndCaption);
	m_wndToolBar.GetControl(5)->SetStyle(xtpButtonIconAndCaption);
	m_wndToolBar.GetControl(6)->SetStyle(xtpButtonIconAndCaption);
	m_wndToolBar.GetControl(7)->SetStyle(xtpButtonIconAndCaption);
	m_wndToolBar.GetControl(8)->SetStyle(xtpButtonIconAndCaption);
	m_wndToolBar.GetControl(9)->SetStyle(xtpButtonIconAndCaption);


	CFont* pFont = XTPPaintManager()->GetRegularFont();
	LOGFONT lf = {sizeof(LOGFONT)};
	pFont->GetLogFont(&lf);
	m_fntRegular.CreateFontIndirect(&lf);

	m_wndEdit.Create(WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | ES_READONLY, CRect(0, 0,200, 120), this, CONTROL_PANE_INFOEDIT_ID);
	m_wndEdit.ModifyStyleEx(0, WS_EX_STATICEDGE);
	m_wndEdit.SetFont(&m_fntRegular);

		

	SetResize(IDC_BORDER_TAB, SZ_TOP_LEFT,     SZ_BOTTOM_RIGHT);

	// Create  the flat tab control.
	CalcTabRect();
	CreateExcelTab();

	m_GraphDlg.Create(IDD_SEQUENCE_GRAPH, NULL);
	m_GraphDlg.ShowWindow(SW_HIDE);

	m_ShapeDlg.Create(IDD_POINTSHAPE, NULL);
	m_ShapeDlg.ShowWindow(SW_HIDE);

	m_EditDlg.Create(IDD_POINTEDIT, NULL);
	m_EditDlg.ShowWindow(SW_HIDE);

	m_AttrDlg.Create(IDD_POINTATTRIBUTE, NULL);
	m_AttrDlg.m_pEditDlg = &m_EditDlg;
	m_AttrDlg.ShowWindow(SW_HIDE);

	m_PointRotateDlg.Create(IDD_POINTROTATEEDIT, NULL);
	m_PointRotateDlg.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CControlPane::OnSize(UINT nType, int cx, int cy)
{
	CXTResizeDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CSize sz(0);
	if (m_wndToolBar.GetSafeHwnd())
	{
		sz = m_wndToolBar.CalcDockingLayout(cx-100, /*LM_HIDEWRAP|*/ LM_HORZDOCK|LM_HORZ | LM_COMMIT);

		m_wndToolBar.MoveWindow(0, 0, cx-200, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
	}

	if (m_wndEdit.GetSafeHwnd())
	{
		m_wndEdit.MoveWindow(cx - 195 , 2, 190, sz.cy - 7);
	}

	if(m_staticBorder.GetSafeHwnd())
	{
		m_staticBorder.MoveWindow(0, sz.cy, cx, cy - sz.cy);
		
		CalcTabRect();

		if(m_pExcelTabCtrl && m_pExcelTabCtrl->GetSafeHwnd())
			m_pExcelTabCtrl->MoveWindow(m_rtTabCtrl.left, m_rtTabCtrl.top, m_rtTabCtrl.Width(), m_rtTabCtrl.Height());
	}
}

void CControlPane::OnDestroy()
{
	CXTResizeDialog::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if (::IsWindow(m_pCtrlPointSheet->GetSafeHwnd()))
		m_pCtrlPointSheet->DestroyWindow();
	
	SAFE_DELETE(m_pExcelTabCtrl);
}

void CControlPane::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		/*
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
		*/
	}
	else
	{
		CXTResizeDialog::OnPaint();
	}

	/*
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CXTResizeDialog::OnPaint()을(를) 호출하지 마십시오.
	*/
}

void CControlPane::OnControlPaneTimeChange()
{
	m_pCtrlPointSheet->ChangeControlPointTime();
	m_GraphDlg.ChangeControlPointTime();
}


void CControlPane::OnUpdateControlPaneTimeChange(CCmdUI* pCmdUI)
{
	/*
	if(m_pCtrlPointSheet)
	{
		if(m_pCtrlPointSheet->PossibleControlPointTime())
			pCmdUI->Enable(TRUE);
		else
			pCmdUI->Enable(FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
	*/

	pCmdUI->Enable(FALSE);
}


void CControlPane::OnControlPaneTimeGraph()
{
	m_GraphDlg.ShowWindow(SW_SHOW);
}

void CControlPane::OnUpdateControlPaneTimeGraph(CCmdUI* pCmdUI)
{
}


void CControlPane::OnControlPaneShape()
{
	m_ShapeDlg.ShowWindow(SW_SHOW);
}

void CControlPane::OnUpdateControlPaneShape(CCmdUI* pCmdUI)
{
}

void CControlPane::OnControlPanePointEdit()
{
	m_EditDlg.ShowWindow(SW_SHOW);
}

void CControlPane::OnUpdateControlPanePointEdit(CCmdUI* pCmdUI)
{
}

void CControlPane::OnControlPanePointRotateEdit()
{
	m_PointRotateDlg.CreateResouce();
	m_PointRotateDlg.ShowWindow(SW_SHOW);
}

void CControlPane::OnUpdateControlPanePointRotateEdit(CCmdUI* pCmdUI)
{
}

void CControlPane::OnControlPanePointAttributeEdit()
{
	m_AttrDlg.ShowWindow(SW_SHOW);
}

void CControlPane::OnUpdateControlPanePointAttributeEdit(CCmdUI* pCmdUI)
{
}


void CControlPane::OnControlPaneResetTrans()
{
	if(m_pDTSpline)
	{
		m_pDTSpline->ResetTransform();
	}
}

void CControlPane::OnUpdateControlPaneResetTrans(CCmdUI* pCmdUI)
{
}


void CControlPane::OnControlPaneResetCamera()
{
	GetNtlGameCameraManager()->ResetAttribute();
}

void CControlPane::OnUpdateControlPaneResetCamera(CCmdUI* pCmdUI)
{
	if(GetEditModeType() == EDIT_MODE_FREE_CAMERA)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CControlPane::OnControlPaneRemovePoint()
{
	if(AfxMessageBox("Remove control point ?", MB_YESNO) == IDYES)
	{
		if(m_pDTSpline)
		{
			int nSelSx, nSelEx;
			GetActiveSelectControlPoint(nSelSx, nSelEx);
			
			m_pDTSpline->RemoveControlPoint(nSelSx);
			m_pCtrlPointSheet->RebuildProp();
			m_GraphDlg.RebuildProp();
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			CPropertyPane *pPropertyPane = pMainFrame->GetPropertyPane();
			pPropertyPane->RefreshSplineProp();
		}
	}
}

void CControlPane::OnUpdateControlPaneRemovePoint(CCmdUI* pCmdUI)
{
	if(m_pDTSpline)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

void CControlPane::OnControlPaneAddPoint()
{
	if(AfxMessageBox("Add control point ?", MB_YESNO) == IDYES)
	{
		if(m_pDTSpline)
		{
			int nSelSx, nSelEx;
			GetActiveSelectControlPoint(nSelSx, nSelEx);
			const SSplineProp *pSplineProp = m_pDTSpline->GetSplineProp();

			if(nSelSx < 0)
				nSelSx = pSplineProp->iCtrlNum;
			
			RwBool bUseWorldPos = m_pDTSpline->IsUseWorldPosition();

			RwV3d vPointPos;

			if(bUseWorldPos)
			{
				RwV3dAssign(&vPointPos, GetNtlGameCameraManager()->GetCameraPos());
			}
			else
			{
				RwV3d vPos = GetSplineStandardPosition();
				RwV3dSubMacro(&vPointPos, GetNtlGameCameraManager()->GetCameraPos(), &vPos);
			}
			
			m_pDTSpline->InsertControlPoint(nSelSx, &vPointPos, GetNtlGameCameraManager()->GetCameraDir());
			m_pCtrlPointSheet->RebuildProp();
			m_GraphDlg.RebuildProp();
			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			CPropertyPane *pPropertyPane = pMainFrame->GetPropertyPane();
			pPropertyPane->RefreshSplineProp();
		}
	}
}

void CControlPane::OnUpdateControlPaneAddPoint(CCmdUI* pCmdUI)
{
	if(m_pDTSpline)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

