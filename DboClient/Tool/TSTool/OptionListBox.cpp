// OptionListBox.cpp : implementation file
//

#include "stdafx.h"
#include "OptionListBox.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class COptionListBoxItem : public CObject {
	DECLARE_DYNAMIC(COptionListBoxItem);
public:
	COptionListBoxItem() {
		isGroup = FALSE;
		m_Page = NULL;
		m_Group = NULL;
		m_ImgIdx = -1;
	}

	BOOL		isGroup;
	COptionPage	*m_Page;
	COptionPage	*m_Group;
	int			m_ImgIdx;
};

IMPLEMENT_DYNAMIC(COptionListBoxItem, CObject);


/////////////////////////////////////////////////////////////////////////////
// COptionListBox

COptionListBox::COptionListBox()
{
	m_ImageList.Create(16, 16, ILC_COLOR | ILC_MASK, 0, 1);
}

COptionListBox::~COptionListBox()
{
}


BEGIN_MESSAGE_MAP(COptionListBox, CListBox)
	//{{AFX_MSG_MAP(COptionListBox)
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelectionChange)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionListBox message handlers
void COptionListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	if(lpMeasureItemStruct->itemHeight < 18) {
		lpMeasureItemStruct->itemHeight = 18;
	}
}

void COptionListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	COptionListBoxItem *item;

	item = (COptionListBoxItem *)lpDrawItemStruct->itemData;
	ASSERT_KINDOF(COptionListBoxItem, item);

	CDC* pDC    = CDC::FromHandle(lpDrawItemStruct->hDC);

	if ((int)lpDrawItemStruct->itemID < 0)
	{
		// If there are no elements in the List Box 
		// based on whether the list box has Focus or not 
		// draw the Focus Rect or Erase it,
		if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && 
			(lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		}
		else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&	
			!(lpDrawItemStruct->itemState & ODS_FOCUS)) 
		{
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem); 
		}
		return;
	}

    CRect  rcItem(lpDrawItemStruct->rcItem); // To draw the focus rect.
    CRect  rClient(rcItem); // Rect to highlight the Item
    CRect  rText(rcItem); // Rect To display the Text
    CPoint Pt( rcItem.left , rcItem.top ); // Point To draw the Image

	if(-1 != item->m_ImgIdx) {
		// if the page has an icon, adjust for it
		rText.left += 18;
	}

	if(FALSE == item->isGroup) {
		Pt.x += 16;
		rcItem.left += 16;
		rClient.left += 16;
		rText.left += 16;
	}

	rText.top += 2;

	COLORREF crText;
	CString strText;

	// Image information in the item data.
	int iImg = (int)lpDrawItemStruct->itemData;

	// If item selected, draw the highlight rectangle.
	// Or if item deselected, draw the rectangle using the window color.
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
		 (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		CBrush br(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->FillRect(&rClient, &br);
	}
	else if (!(lpDrawItemStruct->itemState & ODS_SELECTED) && 
		(lpDrawItemStruct->itemAction & ODA_SELECT)) 
	{
		CBrush br(::GetSysColor(COLOR_WINDOW));
		pDC->FillRect(&rClient, &br);
	}

	// If the item has focus, draw the focus rect.
	// If the item does not have focus, erase the focus rect.
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && 
		(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		pDC->DrawFocusRect(&rcItem); 
	}
	else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&	
		!(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		pDC->DrawFocusRect(&rcItem); 
	}

	// To draw the Text set the background mode to Transparent.
	int iBkMode = pDC->SetBkMode(TRANSPARENT);

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		crText = pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	else if (lpDrawItemStruct->itemState & ODS_DISABLED)
		crText = pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
	else
		crText = pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

	// Get the item text.
	GetText(lpDrawItemStruct->itemID, strText);

	// Setup the text format.
	UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	if (GetStyle() & LBS_USETABSTOPS)
		nFormat |= DT_EXPANDTABS;
	

	// if the ImageList is Existing and there is an associated Image
	// for the Item, draw the Image.
	if(-1 != item->m_ImgIdx) {
		m_ImageList.Draw(pDC, item->m_ImgIdx, Pt, ILD_TRANSPARENT);
	}

	//Draw the Text
	pDC->DrawText(strText, -1, &rText, nFormat | DT_CALCRECT);
	pDC->DrawText(strText, -1, &rText, nFormat);

	pDC->SetTextColor(crText); 
	pDC->SetBkMode(iBkMode);
}


BOOL COptionListBox::Create(COptionSheet *pParentWnd, UINT nID)
{
	ASSERT(pParentWnd != NULL);

	return CListBox::Create(LBS_OWNERDRAWFIXED |
							WS_BORDER |
							WS_TABSTOP |
							WS_CHILD |
							WS_VSCROLL |
							LBS_HASSTRINGS |
							LBS_NOTIFY,
							CRect(0,0,0,0),
							pParentWnd,
							nID);
}


BOOL COptionListBox::AddItem(COptionPage *pPage, COptionPage *pGroup, int nIndex, BOOL bGroup)
{
	int idx;
	CString caption;

	ASSERT(NULL != pPage);
	ASSERT_KINDOF(COptionPage, pPage);

	pPage->GetCaption(caption);
	idx = InsertString(nIndex, caption);
	if(idx >= 0) {
		UINT id;
		COptionListBoxItem *item = new COptionListBoxItem();

		id = pPage->GetIconID();

		item->isGroup = bGroup;
		item->m_Page = pPage;
		item->m_Group = pGroup;

		if(0 != id) {
			HICON icon;

			icon = LoadIcon(AfxGetInstanceHandle(), (LPCSTR)id);
			if(NULL != icon) {
				id = m_ImageList.Add(icon);
				item->m_ImgIdx = id;
			}
		}

		SetItemData(idx, (DWORD)item);

		return TRUE;
	} 

	return FALSE;

}


COptionPage* COptionListBox::GetItemPage(int nIndex)
{
	COptionListBoxItem *item = (COptionListBoxItem*)GetItemData(nIndex);
	ASSERT(NULL != item);
	ASSERT_KINDOF(COptionListBoxItem, item);
	ASSERT_KINDOF(COptionPage, item->m_Page);

	return item->m_Page;
}


COptionPage* COptionListBox::GetItemGroup(int nIndex)
{
	COptionListBoxItem *item = (COptionListBoxItem*)GetItemData(nIndex);
	ASSERT(NULL != item);
	ASSERT_KINDOF(COptionListBoxItem, item);
	ASSERT_KINDOF(COptionPage, item->m_Page);

	return item->m_Group;
}



BOOL COptionListBox::AddGroup(COptionPage *pGroupPage)
{
	return AddItem(pGroupPage, NULL, -1, TRUE);
}


BOOL COptionListBox::AddPage(COptionPage *page, COptionPage *pGroupPage)
{
	ASSERT(NULL != page);

	int i;
	int count;
	CString caption;

	if(NULL != pGroupPage) {
		// Find the group
		count = GetCount();
		if(LB_ERR == count) {
			return FALSE;
		}
		ASSERT(count > 0);
		
		for(i = 0; i < count; i++) {

			COptionPage *itemPage = GetItemPage(i);

			if(pGroupPage == itemPage) {
				//	We found a group page, Skip past the last page associated with this
				//	group so the page may be inserted there.
				if(++i < count) {
					while(i < count) {
						if(pGroupPage != GetItemGroup(i)) {
							break;
						}
						i++;
					}
				}
				return AddItem(page, pGroupPage, i, FALSE);
			}
		}
	} else {
		return AddItem(page);
	}

	return FALSE;
}

BOOL COptionListBox::SetSelection(COptionPage *page)
{
	int count;
	int i;

	if(NULL == page) {
		SetCurSel(-1);
	}

	count = GetCount();
	if(LB_ERR == count) {
		return FALSE;
	}

	for(i = 0; i < count; i++) {
		COptionPage *itemPage = GetItemPage(i);

		if(page == itemPage) {
			SetCurSel(i);
			return TRUE;
		}
	}

	return FALSE;
}


COptionPage *COptionListBox::GetSelection()
{
	int idx = GetCurSel();
	if(LB_ERR == idx) {
		return NULL;
	}

	COptionListBoxItem *item = (COptionListBoxItem*)GetItemData(idx);
	ASSERT_KINDOF(COptionListBoxItem, item);
	ASSERT_KINDOF(COptionPage, item->m_Page);

	return item->m_Page;
}


CWnd *COptionListBox::GetControlWnd()
{
	ASSERT_VALID(this);
	return this;
}


void COptionListBox::OnSelectionChange() 
{
	NotifyParentOfSelChange();
}


void COptionListBox::DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct) 
{
	// TODO: Add your specialized code here and/or call the base class
	COptionListBoxItem *item = (COptionListBoxItem *)GetItemData(lpDeleteItemStruct->itemID);

	if(NULL != item) {
		if(-1 != item->m_ImgIdx) {
			m_ImageList.Remove(item->m_ImgIdx);
		}

		delete item;
	}

	CListBox::DeleteItem(lpDeleteItemStruct);
}


void COptionListBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int idx;
	int curIdx;
	BOOL bResult;

	
	curIdx = GetCurSel();

	idx = ItemFromPoint(point, bResult);
	if(FALSE == bResult) {
		CRect rect;
		GetItemRect(idx, rect);
		if(	point.x >= rect.left && point.x < rect.right &&
			point.y >= rect.top && point.y < rect.bottom) {

			SetCurSel(idx);
			UpdateWindow();

			bResult = NotifyParentOfSelChanging();
			if(FALSE == bResult) {
				/*
				::PlaySound(MAKEINTRESOURCE(SND_ALIAS_SYSTEMDEFAULT),
							NULL,
							SND_ALIAS_ID | SND_ASYNC);
				*/
				MessageBox("Hello");
				SetCurSel(curIdx);

				return;
			}
			SetCapture();
		}
	}
	
	//CListBox::OnLButtonDown(nFlags, point);
}

void COptionListBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(this == GetCapture()) {
		ReleaseCapture();
	}
	
	CListBox::OnLButtonUp(nFlags, point);
}

void COptionListBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	OnLButtonDown(nFlags, point);
	//CListBox::OnLButtonDblClk(nFlags, point);
}

void COptionListBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	//CListBox::OnMouseMove(nFlags, point);
}


void COptionListBox::OnDestroy() 
{
	CListBox::OnDestroy();
	
	// TODO: Add your message handler code here
	while(0 != GetCount()) {
		int count = GetCount();

		for(int i = 0; i < count; i++) {
			DeleteString(0);
		}
	}
	
}
