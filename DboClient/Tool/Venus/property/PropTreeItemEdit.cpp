// PropTreeItemEdit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "PropTreeItemEdit.h"

#include "proptree.h"


// CPropTreeItemEdit

#define _N3_MAXFLOAT	1.0e16f	// Actually this must be < sqrt(MAXFLOAT) since we store this value squared.

IMPLEMENT_DYNAMIC(CPropTreeItemEdit, CEdit)
CPropTreeItemEdit::CPropTreeItemEdit() : m_sEdit(_T("")), m_nFormat(ValueFormatText), m_bPassword(FALSE), m_fValue(0.0f)
{
}

CPropTreeItemEdit::~CPropTreeItemEdit()
{
}


BEGIN_MESSAGE_MAP(CPropTreeItemEdit, CEdit)
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	ON_CONTROL_REFLECT(EN_KILLFOCUS, OnEnKillfocus)
END_MESSAGE_MAP()



// CPropTreeItemEdit 메시지 처리기입니다.


UINT CPropTreeItemEdit::OnGetDlgCode()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return CEdit::OnGetDlgCode()|DLGC_WANTALLKEYS;
}

void CPropTreeItemEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nChar==VK_RETURN)
		CommitChanges();

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CPropTreeItemEdit::OnEnKillfocus()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CommitChanges();	
}

void CPropTreeItemEdit::DrawAttribute(CDC* pDC, const RECT& rc)
{
	ASSERT(m_pProp!=NULL);

	pDC->SelectObject(IsReadOnly() ? m_pProp->GetNormalFont() : m_pProp->GetBoldFont());
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);

	CRect r = rc;

	TCHAR ch;

	// can't use GetPasswordChar(), because window may not be created yet
	ch = (m_bPassword) ? '*' : '\0';

	if (ch)
	{
		CString s;

		s = m_sEdit;
		for (LONG i=0; i<s.GetLength();i++)
			s.SetAt(i, ch);

		pDC->DrawText(s, r, DT_SINGLELINE|DT_VCENTER);
	}
	else
	{		
		if( m_nFormat == ValueFormatFloatPointer )
		{	
			if( m_fValue == _N3_MAXFLOAT )
			{				
				CString valuestring = _T("Max");
				pDC->DrawText(valuestring, r, DT_SINGLELINE|DT_VCENTER);
			}
			else
			{					
				pDC->DrawText(m_sEdit, r, DT_SINGLELINE|DT_VCENTER);
			}
		}
		else
		{
			pDC->DrawText(m_sEdit, r, DT_SINGLELINE|DT_VCENTER);
		}
	}
}



void CPropTreeItemEdit::SetAsPassword(BOOL bPassword)
{
	m_bPassword = bPassword;
}


void CPropTreeItemEdit::SetValueFormat(ValueFormat nFormat)
{
	m_nFormat = nFormat;
}


LPARAM CPropTreeItemEdit::GetItemValue()
{
	switch (m_nFormat)
	{
		case ValueFormatNumber:
			return _ttoi(m_sEdit);

		case ValueFormatFloatPointer:
			_stscanf(m_sEdit, _T("%f"), &m_fValue);
			return (LPARAM)&m_fValue;
	}

	return (LPARAM)(LPCTSTR)m_sEdit;
}


void CPropTreeItemEdit::SetItemValue(LPARAM lParam)
{
	switch (m_nFormat)
	{
		case ValueFormatNumber:
			m_sEdit.Format(_T("%d"), lParam);
			return;

		case ValueFormatFloatPointer:
			{
				TCHAR tmp[MAX_PATH];
				m_fValue = *(float*)lParam;

				_stprintf(tmp, _T("%f"), m_fValue);
				m_sEdit = tmp;				
			}
			return;
	}

	if (lParam==0L)
	{
		TRACE0("CPropTreeItemEdit::SetItemValue - Invalid lParam value\n");
		return;
	}

	m_sEdit = (LPCTSTR)lParam;
}


void CPropTreeItemEdit::OnMove()
{
	if (IsWindow(m_hWnd))
		SetWindowPos(NULL, m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(), SWP_NOZORDER|SWP_NOACTIVATE);
}


void CPropTreeItemEdit::OnRefresh()
{
	if (IsWindow(m_hWnd))
		SetWindowText(m_sEdit);
}


void CPropTreeItemEdit::OnCommit()
{
	// hide edit control
	ShowWindow(SW_HIDE);

	// store edit text for GetItemValue
	GetWindowText(m_sEdit);
}


void CPropTreeItemEdit::OnActivate()
{
	// Check if the edit control needs creation
	if (!IsWindow(m_hWnd))
	{
		DWORD dwStyle;

		dwStyle = WS_CHILD|ES_AUTOHSCROLL;
		Create(dwStyle, m_rc, m_pProp->GetCtrlParent(), GetCtrlID());
		SendMessage(WM_SETFONT, (WPARAM)m_pProp->GetNormalFont()->m_hObject);
	}

	SetPasswordChar((TCHAR)(m_bPassword ? '*' : 0));		
	SetWindowText(m_sEdit);
	SetSel(0, -1);

	SetWindowPos(NULL, m_rc.left, m_rc.top, m_rc.Width(), m_rc.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);
	SetFocus();
}
