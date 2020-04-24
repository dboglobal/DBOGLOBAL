// ListEditCtrl.cpp : implementation file
//

// The code contained in this file is based on the original
// CInPlaceEdit from http://www.codeguru.com/listview/edit_subitems.shtml

#include "stdafx.h"
#include "ListEditCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListEditCtrl
BEGIN_MESSAGE_MAP(CListEditCtrl, CEdit)
	//{{AFX_MSG_MAP(CListEditCtrl)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_CHAR()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListEditCtrl message handlers

CListEditCtrl::CListEditCtrl(int iItem, int iSubItem, CString sInitText):m_strInitText(sInitText)
{	
	m_iItem = iItem;
	m_iSubItem = iSubItem;
	m_bVK_ESCAPE = 0;
}

int CListEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)		
		return -1;	

	CFont* font = GetParent()->GetFont();
	SetFont(font);
	SetWindowText(m_strInitText);
	SetFocus();	
	SetSel(0, 0);
	return 0;
}

BOOL CListEditCtrl::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )	
	{		
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_DELETE || pMsg->wParam == VK_ESCAPE || GetKeyState( VK_CONTROL))
		{			
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return 1;
		}	
	}
	return CEdit::PreTranslateMessage(pMsg);
}


void CListEditCtrl::OnKillFocus(CWnd* pNewWnd)
{	
	CEdit::OnKillFocus(pNewWnd);

	CString str;	GetWindowText(str);

	// Send Notification to parent of ListView ctrl	
	LV_DISPINFO lvDispInfo;
	lvDispInfo.hdr.hwndFrom = GetParent()->m_hWnd;
	lvDispInfo.hdr.idFrom = GetDlgCtrlID();	
	lvDispInfo.hdr.code = LVN_ENDLABELEDIT;
	lvDispInfo.item.mask = LVIF_TEXT;	
	lvDispInfo.item.iItem = m_iItem;
	lvDispInfo.item.iSubItem = m_iSubItem;
	lvDispInfo.item.pszText = m_bVK_ESCAPE ? NULL : LPTSTR((LPCTSTR)str);
	lvDispInfo.item.cchTextMax = str.GetLength();
	GetParent()->GetParent()->SendMessage( WM_NOTIFY, GetParent()->GetDlgCtrlID(),(LPARAM)&lvDispInfo);
	DestroyWindow();
}


void CListEditCtrl::OnNcDestroy()
{
	CEdit::OnNcDestroy();	
	delete this;
}


void CListEditCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( nChar == VK_ESCAPE || nChar == VK_RETURN)	
	{		
		if( nChar == VK_ESCAPE)
			m_bVK_ESCAPE = 1;		
		GetParent()->SetFocus();
		return;	
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);	
	// Resize edit control if needed
	// Get text extent	
	CString str;	
	GetWindowText( str );	
	CWindowDC dc(this);
	CFont *pFont = GetParent()->GetFont();
	CFont *pFontDC = dc.SelectObject(pFont);
	CSize size = dc.GetTextExtent(str);	
	dc.SelectObject(pFontDC);
	size.cx += 5; // add some extra buffer	
	// Get client rect
	CRect rect, rcParent;	
	GetClientRect(&rect);
	GetParent()->GetClientRect(&rcParent);
	// Transform rect to parent coordinates	
	ClientToScreen(&rect);
	GetParent()->ScreenToClient(&rect);
	// Check whether control needs to be resized
	// and whether there is space to grow	
	if(size.cx > rect.Width())	
	{
		if(size.cx + rect.left < rcParent.right)
			rect.right = rect.left + size.cx;		
		else			
			rect.right = rcParent.right;
		MoveWindow(&rect);	
	}
}

