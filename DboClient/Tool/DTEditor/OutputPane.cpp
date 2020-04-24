// OutputPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "OutputPane.h"


#define OUTPUT_PANE_WND_ID	AFX_IDW_PANE_FIRST + 2

// COutputPane

IMPLEMENT_DYNAMIC(COutputPane, CWnd)

LRESULT COutputPane::COutputEdit::WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
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

COutputPane::COutputPane()
{

}

COutputPane::~COutputPane()
{
}

/*
void COutputPane::Build()
{
	SetDlgItemText(OUTPUT_PANE_WND_ID, "");
	SetDlgItemText(OUTPUT_PANE_WND_ID, "   === Build Start ===  \r\n");
}

void COutputPane::BuildSuccess()
{
	CString str;
	GetDlgItemText(OUTPUT_PANE_WND_ID, str);

	str += "\r\n";
	str += " === Build Success ===  \r\n";

	SetDlgItemText(OUTPUT_PANE_WND_ID, str);
}

void COutputPane::BuildErrorReport(const char *pError)
{
	CString str;
	GetDlgItemText(OUTPUT_PANE_WND_ID, str);

	str += "\r\n";
	str += pError;
	str += "\r\n";
	str += "\r\n";
	str += " === Build Fail !!! ===  \r\n";

	SetDlgItemText(OUTPUT_PANE_WND_ID, str);
}

void COutputPane::Compile()
{
	SetDlgItemText(OUTPUT_PANE_WND_ID, "");
	SetDlgItemText(OUTPUT_PANE_WND_ID, "   === Compile Start ===  \r\n");
}

void COutputPane::CompileSuccess()
{
	CString str;
	GetDlgItemText(OUTPUT_PANE_WND_ID, str);

	str += "\r\n";
	str += " === Compile Success ===  \r\n";

	SetDlgItemText(OUTPUT_PANE_WND_ID, str);
}

void COutputPane::BuildError(BYTE byErrorCount)
{
	CString str;
	GetDlgItemText(OUTPUT_PANE_WND_ID, str);

	CString strCount;
	strCount.Format("%d", byErrorCount);

	str += "\r\n";
	str += " === Build Error : ";
	str += strCount;
	str += " ==  \r\n";
		
	SetDlgItemText(OUTPUT_PANE_WND_ID, str);
}

void COutputPane::CompileErrorReport(const char *pError)
{
	CString str;
	GetDlgItemText(OUTPUT_PANE_WND_ID, str);

	str += "\r\n";
	str += "Error : ";
	str += pError;
	str += "\r\n";
	str += "\r\n";

	SetDlgItemText(OUTPUT_PANE_WND_ID, str);
}

void COutputPane::CompileError(BYTE byErrorCount)
{
	CString str;
	GetDlgItemText(OUTPUT_PANE_WND_ID, str);

	CString strCount;
	strCount.Format("%d", byErrorCount);

	str += "\r\n";
	str += " === Compile Error : ";
	str += strCount;
	str += " ==  \r\n";
		
	SetDlgItemText(OUTPUT_PANE_WND_ID, str);
}

*/

BEGIN_MESSAGE_MAP(COutputPane, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// COutputPane 메시지 처리기입니다.


int COutputPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	m_wndEdit.Create(WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | ES_READONLY | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, CRect(0, 0,200, 120), this, OUTPUT_PANE_WND_ID);
	m_wndEdit.ModifyStyleEx(0, WS_EX_STATICEDGE);
	
	return 0;
}

void COutputPane::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CSize sz(0);

	if (m_wndEdit.GetSafeHwnd())
	{
		m_wndEdit.MoveWindow(5, sz.cy+5, cx-10, cy - sz.cy-10);
	}
}
