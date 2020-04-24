// ScriptPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"

//lua
#include "NtlLuaState.h"

// simulation
#include "precomp_ntlsimulation.h"
#include "NtlSLLuaGlue.h"
#include "NtlDTCinematicManager.h"

// DTedit
#include "MainFrm.h"
#include "ScriptPane.h"
#include "DTEditorGlobal.h"
#include "DirectionPane.h"


#define SCRIPT_PANE_WND_ID	AFX_IDW_PANE_FIRST + 1
#define OUTPUT_PANE_WND_ID	AFX_IDW_PANE_FIRST + 3

#define SCRIPT_ACT_BUILD		0
#define SCRIPT_ACT_COMPILE		1

RwUInt8	g_byScriptActType	= SCRIPT_ACT_BUILD;
RwUInt8 g_byErrorCount		= 0;

void LuaErrorHandler(const char *pError)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CScriptPane *pScriptPane = pMainFrame->GetScriptPane();

	if(g_byScriptActType == SCRIPT_ACT_BUILD)
		pScriptPane->BuildErrorReport(pError);
	else
		pScriptPane->CompileErrorReport(pError);

	g_byErrorCount++;
}

void CScriptPane::Build()
{
	SetDlgItemText(OUTPUT_PANE_WND_ID, "");
	SetDlgItemText(OUTPUT_PANE_WND_ID, "   === Build Start ===  \r\n");
}

void CScriptPane::BuildSuccess()
{
	CString str;
	GetDlgItemText(OUTPUT_PANE_WND_ID, str);

	if(!GetNtlDTCinematicManager()->IsCinematicPlayPossible())
	{
		str += "\r\n";
		str += " $$$ cinematic play error : avatar positon is far $$$ \r\n";
	}

	str += "\r\n";
	str += " === Build Success ===  \r\n";

	SetDlgItemText(OUTPUT_PANE_WND_ID, str);
}

void CScriptPane::BuildErrorReport(const char *pError)
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

void CScriptPane::Compile()
{
	SetDlgItemText(OUTPUT_PANE_WND_ID, "");
	SetDlgItemText(OUTPUT_PANE_WND_ID, "   === Compile Start ===  \r\n");
}

void CScriptPane::CompileSuccess()
{
	CString str;
	GetDlgItemText(OUTPUT_PANE_WND_ID, str);

	str += "\r\n";
	str += " === Compile Success ===  \r\n";

	SetDlgItemText(OUTPUT_PANE_WND_ID, str);
}

void CScriptPane::BuildError(BYTE byErrorCount)
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

void CScriptPane::CompileErrorReport(const char *pError)
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

void CScriptPane::CompileError(BYTE byErrorCount)
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



// CScriptPane

IMPLEMENT_DYNAMIC(CScriptPane, CWnd)

LRESULT CScriptPane::CScriptEdit::WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
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
		break;
	}

	return CEdit::WindowProc(message, wParam, lParam);
}

LRESULT CScriptPane::COutputEdit::WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
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


BEGIN_MESSAGE_MAP(CScriptPane::CScriptEdit, CEdit)

	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_COPY, &CScriptEdit::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CScriptEdit::OnEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, &CScriptEdit::OnEditUndo)
	ON_COMMAND(ID_EDIT_CUT, &CScriptEdit::OnEditCut)
END_MESSAGE_MAP()


int CScriptPane::CScriptEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	m_hAccel = NULL;
	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), ATL_MAKEINTRESOURCE(IDR_MAINFRAME));

	return 0;
}

BOOL CScriptPane::CScriptEdit::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST)
	{
		if(m_hAccel)
		{
			::TranslateAccelerator(m_hWnd, m_hAccel, pMsg);
		}
	}

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CEdit::PreTranslateMessage(pMsg);
}

void CScriptPane::CScriptEdit::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	Copy();
}

void CScriptPane::CScriptEdit::OnEditPaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	Paste();
}

void CScriptPane::CScriptEdit::OnEditUndo()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	Undo();
}

void CScriptPane::CScriptEdit::OnEditCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	Cut();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////


CScriptPane::CScriptPane()
{

}

CScriptPane::~CScriptPane()
{
}


BEGIN_MESSAGE_MAP(CScriptPane, CWnd)
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_COMMAND(ID_SCRIPT_PANE_COMPILE, OnScriptPaneBuild)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_PANE_COMPILE, OnUpdateScriptPaneBuild)
	ON_COMMAND(ID_SCRIPT_PANE_COMPILE_STOP, OnScriptPaneBuildStop)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_PANE_COMPILE_STOP, OnUpdateScriptPaneBuildStop)
	ON_COMMAND(ID_SCRIPT_PANE_COPY, OnScriptPaneCopy)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_PANE_COPY, OnUpdateScriptPaneCopy)
	ON_COMMAND(ID_SCRIPT_PANE_PASTE, OnScriptPanePaste)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_PANE_PASTE, OnUpdateScriptPanePaste)
	ON_COMMAND(ID_SCRIPT_PANE_COMPILE_ALL, OnScriptPaneCompileAll)
	ON_UPDATE_COMMAND_UI(ID_SCRIPT_PANE_COMPILE_ALL, OnUpdateScriptPaneCompileAll)
END_MESSAGE_MAP()


// CScriptPane 메시지 처리기입니다.

BOOL CScriptPane::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CScriptPane::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CSize sz(0);

	if (m_wndToolBar.GetSafeHwnd())
	{
		sz = m_wndToolBar.CalcDockingLayout(cx, /*LM_HIDEWRAP|*/ LM_HORZDOCK|LM_HORZ | LM_COMMIT);

		m_wndToolBar.MoveWindow(0, 0, cx, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
	}

	if (m_wndEdit.GetSafeHwnd())
	{
		m_wndEdit.MoveWindow(5, sz.cy, cx/2-5, cy - sz.cy-5);
	}

	if (m_wndOutEdit.GetSafeHwnd())
	{
		m_wndOutEdit.MoveWindow(cx/2+5, sz.cy, cx-10, cy - sz.cy-5);
	}
}

int CScriptPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	XTPImageManager()->SetMaskColor(RGB(0, 0xFF, 0));
	VERIFY(m_wndToolBar.CreateToolBar(WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_SCRIPT_PANE_TOOLBAR));
	m_wndToolBar.GetControl(0)->SetStyle(xtpButtonIconAndCaption);
	m_wndToolBar.GetControl(1)->SetStyle(xtpButtonIconAndCaption);
	m_wndToolBar.GetControl(2)->SetStyle(xtpButtonIconAndCaption);
	m_wndToolBar.GetControl(3)->SetStyle(xtpButtonIconAndCaption);
	m_wndToolBar.GetControl(4)->SetStyle(xtpButtonIconAndCaption);

	m_wndEdit.Create(WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, CRect(0, 0,200, 120), this, SCRIPT_PANE_WND_ID);
	m_wndEdit.ModifyStyleEx(0, WS_EX_STATICEDGE);

	m_wndOutEdit.Create(WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE | ES_READONLY | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE, CRect(0, 0,200, 120), this, OUTPUT_PANE_WND_ID);
	m_wndOutEdit.ModifyStyleEx(0, WS_EX_STATICEDGE);

	CNtlLuaState *pLuaState = GetLuaState();
	pLuaState->LinkErrorHandler(LuaErrorHandler);

	return 0;
}

void CScriptPane::OnScriptPaneBuild()
{
	CString str;
	GetDlgItemText(SCRIPT_PANE_WND_ID, str);

	if(str.IsEmpty())
	{
		AfxMessageBox("script command is empty !!!");
		return;
	}

	g_byScriptActType = SCRIPT_ACT_BUILD;

	CNtlLuaState *pLuaState = GetLuaState();
	
	Build();

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if( pMainFrame->GetDirectionPane()->GetSafeHwnd() )
		pMainFrame->GetDirectionPane()->OnStop();

	// lua run
	pLuaState->RunString(str.GetBuffer(str.GetLength()));
	
	if(g_byErrorCount > 0)
		BuildError(g_byErrorCount);
	else
		BuildSuccess();

	g_byErrorCount = 0;
}

void CScriptPane::OnUpdateScriptPaneBuild(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CScriptPane::OnScriptPaneBuildStop()
{
	
}

void CScriptPane::OnUpdateScriptPaneBuildStop(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CScriptPane::OnScriptPaneCopy()
{
	m_wndEdit.Copy();
}

void CScriptPane::OnUpdateScriptPaneCopy(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CScriptPane::OnScriptPanePaste()
{
	m_wndEdit.Paste();
}

void CScriptPane::OnUpdateScriptPanePaste(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}


void CScriptPane::OnScriptPaneCompileAll()
{
	g_byScriptActType = SCRIPT_ACT_COMPILE;

	CString str;
	GetDlgItemText(SCRIPT_PANE_WND_ID, str);
	
	Compile();

	// lua run
	SLLua_CreateState();

	CNtlLuaState *pLuaState = GetLuaState();
	pLuaState->LinkErrorHandler(LuaErrorHandler);

	SLLua_Compile();

	if(g_byErrorCount > 0)
		CompileError(g_byErrorCount);
	else
		CompileSuccess();

	g_byErrorCount = 0;
}

void CScriptPane::OnUpdateScriptPaneCompileAll(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}




