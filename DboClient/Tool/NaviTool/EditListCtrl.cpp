#include "stdafx.h"
#include "EditListCtrl.h"
#include "Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditListCtrl

CEditListCtrl::CEditListCtrl()
{
}

CEditListCtrl::~CEditListCtrl()
{
}

BEGIN_MESSAGE_MAP(CEditListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CEditListCtrl)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(LVN_ENDLABELEDIT,IDC_LIST_EXPORT,OnEndLabelEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditListCtrl message handlers
void CEditListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{ 
	int index;
	CListCtrl::OnLButtonDown(nFlags, point);
	int colnum;
	int nItem, nSubItem; 
	CRect rcClient; 
	GetSubItemFromPt(point, nItem, nSubItem, rcClient); 

	if((index=HitTestEx(point,&colnum)) != -1 )
	{
		EditSubLabel(index,nSubItem);
	}
} 

void CEditListCtrl::GetSubItemFromPt(CPoint point, int &nItem, int &nSubItem, CRect &rect) 
{ 
	LVHITTESTINFO lvhti; 

	lvhti.pt = point; 
	SubItemHitTest(&lvhti); 
	nItem = lvhti.iItem; 
	nSubItem = lvhti.iSubItem; 
	GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rect); 
} 
int CEditListCtrl::HitTestEx(CPoint &point, int *col) const
{
	int colnum = 0;
	int row = HitTest( point, NULL );
	if( col ) *col = 0;
	if( row != -1 )
	{
		return row;
	}

	if( (GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT)
		return row;

	row = GetTopIndex();
	int bottom = row + GetCountPerPage();
	if( bottom > GetItemCount() )
		bottom = GetItemCount();

	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();

	for( ;row <=bottom;row++)
	{
		CRect rect;
		GetItemRect( row, &rect, LVIR_BOUNDS );
		if( rect.PtInRect(point) )
		{
			for( colnum = 0; colnum < nColumnCount; colnum++ )
			{
				int colwidth = GetColumnWidth(colnum);
				if( point.x >= rect.left  && point.x <= (rect.left + colwidth ) )
				{
					if( col ) *col = colnum;
					return row;
				}
				rect.left += colwidth;
			}
		}
	}
	return -1;
}

CEdit* CEditListCtrl::EditSubLabel( int nItem, int nCol )
{
	if( !EnsureVisible( nItem, TRUE ) ) 
		return NULL;
	CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
	int nColumnCount = pHeader->GetItemCount();
	if( nCol >= nColumnCount || GetColumnWidth(nCol) < 5 )
		return NULL;

	int offset = 0;
	for(int i=0;i<nCol; i++)
		offset += GetColumnWidth(i);
	CRect rect;
	GetItemRect(nItem,&rect,LVIR_BOUNDS);
	CRect rcClient;
	GetClientRect(&rcClient);
	if(offset+rect.left<0 || offset+rect.left>rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll( size );
		rect.left -= size.cx;
	}

	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn( nCol, &lvcol );
	DWORD dwStyle ;
	if((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT)
		dwStyle = ES_LEFT;
	else if((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT)
		dwStyle = ES_RIGHT;
	else
		dwStyle = ES_CENTER;

	rect.left += offset+4;
	rect.right = rect.left + GetColumnWidth(nCol)-3;
	if(rect.right>rcClient.right)
		rect.right = rcClient.right;
	dwStyle |=WS_CHILD|WS_VISIBLE|ES_AUTOHSCROLL;
	CEdit *pEdit = new CInPlaceEdit(nItem, nCol, GetItemText(nItem,nCol));
	pEdit->Create(dwStyle,rect,this,1);

	return pEdit;
}
void CEditListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( GetFocus() != this ) 
		SetFocus();
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}
void CEditListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( GetFocus() != this ) 
		SetFocus();
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}
void CEditListCtrl::OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult)
{
	LV_DISPINFO *plvDispInfo = (LV_DISPINFO *)pNMHDR;
	LV_ITEM *plvItem = &plvDispInfo->item;
	if (plvItem->pszText != NULL)
	{
		SetItemText(plvItem->iItem, plvItem->iSubItem, plvItem->pszText);
	}
	*pResult = FALSE;
}
