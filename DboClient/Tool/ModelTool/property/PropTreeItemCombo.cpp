// PropTreeItemCombo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "PropTreeItemCombo.h"

#include "PropTree.h"
#include "Resource.h"


// CPropTreeItemCombo
#define DROPDOWN_HEIGHT			100

IMPLEMENT_DYNAMIC(CPropTreeItemCombo, CComboBox)
CPropTreeItemCombo::CPropTreeItemCombo() :
	m_lComboData(0),
	m_nDropHeight(DROPDOWN_HEIGHT)
{
	m_ControlType = _XPTI_ControlType_Combo;
}

CPropTreeItemCombo::~CPropTreeItemCombo()
{
}


BEGIN_MESSAGE_MAP(CPropTreeItemCombo, CComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnCbnSelchange)
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnCbnKillfocus)
END_MESSAGE_MAP()



// CPropTreeItemCombo 메시지 처리기입니다.


void CPropTreeItemCombo::OnCbnSelchange()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CommitChanges();
}

void CPropTreeItemCombo::OnCbnKillfocus()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CommitChanges();
}

void CPropTreeItemCombo::DrawAttribute(CDC* pDC, const RECT& rc)
{
	ASSERT(m_pProp!=NULL);

	// verify the window has been created
	if (!IsWindow(m_hWnd))
	{
		TRACE0("CPropTreeItemCombo::DrawAttribute() - The window has not been created\n");
		return;
	}

	pDC->SelectObject(IsReadOnly() ? m_pProp->GetNormalFont() : m_pProp->GetBoldFont());
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);

	CRect r = rc;
	CString s;
	LONG idx;

	if ((idx = GetCurSel())!=CB_ERR)
		GetLBText(idx, s);
	else
		s = _T("");

	pDC->DrawText(s, r, DT_SINGLELINE|DT_VCENTER);
}


LPARAM CPropTreeItemCombo::GetItemValue()
{
	return m_lComboData;
}


void CPropTreeItemCombo::SetItemValue(LPARAM lParam)
{
	m_lComboData = lParam;
	OnRefresh();
}


void CPropTreeItemCombo::OnMove()
{
	if (IsWindow(m_hWnd) && IsWindowVisible())
		SetWindowPos(NULL, m_rc.left, m_rc.top, m_rc.Width() + 1, m_rc.Height(), SWP_NOZORDER|SWP_SHOWWINDOW);
}


void CPropTreeItemCombo::OnRefresh()
{
	LONG idx = FindCBData(m_lComboData);

	if (idx!=CB_ERR)
		SetCurSel(idx);
}


void CPropTreeItemCombo::OnCommit()
{
	LONG idx;
	
	// store combo box item data
	if ((idx = GetCurSel())==CB_ERR)
		m_lComboData = 0;
	else
		m_lComboData = (LPARAM)GetItemData(idx);

	ShowWindow(SW_HIDE);
}


void CPropTreeItemCombo::OnActivate()
{
	// activate the combo box
	SetWindowPos(NULL, m_rc.left, m_rc.top, m_rc.Width() + 1, m_rc.Height() + m_nDropHeight, SWP_NOZORDER|SWP_SHOWWINDOW);
	SetFocus();

	if (GetCount())
		ShowDropDown(TRUE);
}


BOOL CPropTreeItemCombo::CreateComboBox(DWORD dwStyle)
{
	ASSERT(m_pProp!=NULL);

	if (IsWindow(m_hWnd))
		DestroyWindow();

	// force as not visible child window
	dwStyle = (WS_CHILD|WS_VSCROLL|dwStyle) & ~WS_VISIBLE & ~CBS_SORT;

	if (!Create(dwStyle, CRect(0,0,0,0), m_pProp->GetCtrlParent(), GetCtrlID()))
	{
		TRACE0("CPropTreeItemCombo::CreateComboBox() - failed to create combo box\n");
		return FALSE;
	}

	SendMessage(WM_SETFONT, (WPARAM)m_pProp->GetNormalFont()->m_hObject);

	return TRUE;
}


BOOL CPropTreeItemCombo::CreateComboBoxBool()
{
	ASSERT(m_pProp!=NULL);

	if (IsWindow(m_hWnd))
		DestroyWindow();

	// force as a non-visible child window
	DWORD dwStyle = WS_CHILD|WS_VSCROLL|CBS_SORT|CBS_DROPDOWNLIST;

	if (!Create(dwStyle, CRect(0,0,0,0), m_pProp->GetCtrlParent(), GetCtrlID()))
	{
		TRACE0("CPropTreeItemCombo::CreateComboBoxBool() - failed to create combo box\n");
		return FALSE;
	}

	SendMessage(WM_SETFONT, (WPARAM)m_pProp->GetNormalFont()->m_hObject);

	// file the combo box
	LONG idx;
	CString s;

    s = _T("True");
	idx = AddString(s);
	SetItemData(idx, TRUE);

    s = _T("False");
	idx = AddString(s);
	SetItemData(idx, FALSE);

	return TRUE;
}


LONG CPropTreeItemCombo::FindCBData(LPARAM lParam)
{
	LONG idx;
	
	for (idx = 0; idx < GetCount(); idx++)
	{
		if (idx==(DWORD)lParam)
//		if (GetItemData(idx)==(DWORD)lParam)
			return idx;
	}

	return CB_ERR;
}

void CPropTreeItemCombo::SetDropDownHeight(LONG nDropHeight)
{
	m_nDropHeight = nDropHeight;
}


LONG CPropTreeItemCombo::GetDropDownHeight()
{
	return m_nDropHeight;
}

void CPropTreeItemCombo::SetCurSelText(const char* szText)
{
    USES_CONVERSION;

    if(!szText)
        return;

    BOOL bExist = FALSE;
    int nCurSel = GetCurSel();
    CString strTarget = A2W(szText);

    // TODO : 함수 완성
    for(int i = 0; i < GetCount(); ++i)
    {
        CString strText;
        SetCurSel(i);   
        GetWindowText(strText);
        if(strText.MakeUpper() == strTarget.MakeUpper())
        {
            bExist = TRUE;
            break;
        }        
    }

    if(bExist == FALSE)
    {
        SetCurSel(0);
    }
}