// VenusGridCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "VenusGridCtrl.h"
#include "ComboInListView.h"


// CVenusGridCtrl

//IMPLEMENT_DYNAMIC(CVenusGridCtrl, CSuperGridCtrl)
CVenusGridCtrl::CVenusGridCtrl()
{
}

CVenusGridCtrl::~CVenusGridCtrl()
{
}


BEGIN_MESSAGE_MAP(CVenusGridCtrl, CSuperGridCtrl)
END_MESSAGE_MAP()



// CVenusGridCtrl 메시지 처리기입니다.

//------------------------------------------------------------------
//	FuncName	: ResetTreeItemName
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CVenusGridCtrl::ResetTreeItemName(const RwChar* strName)
{
	CTreeItem* pSelectItem = GetTreeItem(GetSelectedItem());
	if (pSelectItem == NULL) return;
	CItemInfo* pInfo = GetData(pSelectItem);

	SetItemText(NodeToIndex(pSelectItem), 0, strName);
	pInfo->SetItemText(strName);

	UpdateData(pSelectItem, pInfo);

	SetRedraw(1);
	InvalidateRect(NULL);
	UpdateWindow();
}

//------------------------------------------------------------------
//	FuncName	: GetRootItem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
CSuperGridCtrl::CTreeItem* CVenusGridCtrl::GetSelectRootItem()
{
	int nItem = GetSelectedItem();
	if(nItem == -1) return NULL;

	CTreeItem* pRootItem = GetTreeItem(nItem);
	while(GetParentItem(pRootItem) != NULL)
	{
		pRootItem = GetParentItem(pRootItem);
	}
	return pRootItem;
}

//------------------------------------------------------------------
//	FuncName	: BuildTreeItem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
CSuperGridCtrl::CTreeItem* CVenusGridCtrl::BuildTreeItem(const RwChar* strName, RwInt32 nImage, DWORD_PTR data, CTreeItem* pParentItem, RwBool bUpdate /* = FALSE */, RwUInt32 state /* = 0 */)
{
	CItemInfo* lpItemInfo = new CItemInfo();
	lpItemInfo->SetImage(nImage);
	lpItemInfo->SetItemText(strName);
	lpItemInfo->SetCheck(TRUE);
	lpItemInfo->m_lParam = data;

	CTreeItem* pNewItem = (pParentItem == NULL) ? InsertRootItem(lpItemInfo) : InsertItem(pParentItem, lpItemInfo, bUpdate);

	// state = LVIS_SELECTED | LVIS_FOCUSED
	SetItemState(NodeToIndex(pNewItem), state, state);
	InvalidateRect(NULL);
	UpdateWindow();
	return pNewItem;
}

//------------------------------------------------------------------
//	FuncName	: InsertTreeItem
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
CSuperGridCtrl::CTreeItem* CVenusGridCtrl::InsertTreeItem(const RwChar* strName, RwInt32 nImage, DWORD_PTR data, CTreeItem* pParentItem, RwBool bExpand /* = TRUE */, RwUInt32 state /* = 0 */)
{
	CItemInfo* lpItemInfo = new CItemInfo();
	lpItemInfo->SetImage(nImage);
	lpItemInfo->SetItemText(strName);
	lpItemInfo->SetCheck(TRUE);
	lpItemInfo->m_lParam = data;

	CTreeItem* pNewItem = (pParentItem == NULL) ? InsertRootItem(lpItemInfo) : InsertItem(pParentItem, lpItemInfo, TRUE);

	if (pParentItem != NULL && bExpand)
	{
		Expand(pParentItem, NodeToIndex(pParentItem));
	}

	// state = LVIS_SELECTED | LVIS_FOCUSED
	SetItemState(NodeToIndex(pNewItem), state, state);
	InvalidateRect(NULL);
	UpdateWindow();
	return pNewItem;
}

//override called when OnLButtondown
void CVenusGridCtrl::OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht)
{
	//now I am sure I added a combobox some where, so check for this control
	CTreeItem*pSelItem = GetTreeItem(ht.iItem);
	if(pSelItem!=NULL)
	{	
		CItemInfo* pInfo = GetData(pSelItem);
		CItemInfo::CONTROLTYPE ctrlType;
		if(pInfo->GetControlType(ht.iSubItem-1, ctrlType))
		{	
            if(ctrlType == CItemInfo::combobox) 
			{
					CStringList* list=NULL;
					pInfo->GetListData(ht.iSubItem-1, list);
					CComboBox * pList = ShowList(ht.iItem, ht.iSubItem, list);
			}
		}								
		/*
		else //activate default edit control
			CSuperGridCtrl::OnControlLButtonDown(nFlags, point, ht);
		*/
	}
}

#define IDC_COMBOBOXINLISTVIEW 0x1235
CComboBox* CVenusGridCtrl::ShowList(int nItem, int nCol, CStringList *lstItems)
{
	CString strFind = GetItemText(nItem, nCol);

	//basic code start
	CRect rect;
	int offset = 0;
	// Make sure that the item is visible
	if( !EnsureVisible(nItem, TRUE)) return NULL;
	GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rect);
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(rcClient);
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll(size);
		rect.left -= size.cx;
	}
	
	rect.left += offset;	
	rect.right = rect.left + GetColumnWidth(nCol);
	if(rect.right > rcClient.right) 
	   rect.right = rcClient.right;
	//basic code end

	rect.bottom += 10 * rect.Height();//dropdown area
	
	DWORD dwStyle =  WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL|CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL;
	CComboBox *pList = new CComboInListView(nItem, nCol, lstItems);
	pList->Create(dwStyle, rect, this, IDC_COMBOBOXINLISTVIEW);
	pList->ModifyStyleEx(0,WS_EX_CLIENTEDGE);//can we tell at all
	pList->SetHorizontalExtent(CalcHorzExtent(pList, lstItems));
	pList->ShowDropDown();
	pList->SelectString(-1, strFind.GetBuffer(1));
	// The returned pointer should not be saved
	return pList;
}
int CVenusGridCtrl::CalcHorzExtent(CWnd* pWnd, CStringList *pList)
{
	int nExtent=0;
	if(pWnd!=NULL)
	{
		CDC* pDC = pWnd->GetDC();
		HFONT hFont = (HFONT)pWnd->SendMessage(WM_GETFONT); //why not pWnd->GetFont();..I like the send thing alot and
		CFont *pFont = CFont::FromHandle(hFont);			//this way I get to use this function..cool :)
		if(pFont!=NULL)										//ya what ever makes me happy,.right :}
		{
			CFont* pOldFont = pDC->SelectObject(pFont);
			CSize newExtent;
			POSITION pos = pList->GetHeadPosition();
			while(pos != NULL)
			{
				CString str(pList->GetNext(pos));
				newExtent = pDC->GetTextExtent(str);
				newExtent.cx += 6;
				if (newExtent.cx > nExtent)
				{
					nExtent = newExtent.cx;
				}
			}
			pDC->SelectObject(pOldFont);
		}
		pWnd->ReleaseDC(pDC);
	}
	return nExtent;
}

//helper function to copy CItemInfo used when drag/drop you must override this this function to suit your own CItemInfo class
CItemInfo* CVenusGridCtrl::CopyData(CItemInfo* lpSrc)
{
	ASSERT(lpSrc!=NULL);
	CItemInfo* lpDest = new CItemInfo;
	//well okay I put all the copy thing in one function, located in CItemInfo class, 
	//so you should check out this function, remember to modify this function each time you add new data to CItemInfo class.
	lpDest->CopyObjects(lpSrc);
	return lpDest;
}


//override, like red!
COLORREF CVenusGridCtrl::GetCellRGB()
{
	return RGB(192,0,0);
}


//this is my override of GetIcon, override this to set what ever icon suits you
int CVenusGridCtrl::GetIcon(const CTreeItem* pItem)
{
	if(pItem!=NULL)
	{
		int n = GetData(pItem)->GetImage();
		if(n!=-1)
			return n;
		
		int iImage = 0;
		if(ItemHasChildren(pItem))
		{
			IsCollapsed(pItem) ? iImage = 1/*close icon*/:iImage = 0;/*open icon*/
		}
		else
			iImage = 2;//doc icon
		return iImage;
	}
	return 0;
}

void CVenusGridCtrl::OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem)
{
	//put some extra validation here 
	CString str = (CString)plvItem->pszText;
	if(!str.Compare(_T("Bugs: Impossible")))
	{
		str+=_T(" (selfconfidence allright)");//that's valid enough	:?
		CItemInfo *lp = GetData(lpItem);
		if(lp!=NULL)
		{
			if(plvItem->iSubItem==0)
				lp->SetItemText(str);
			else //subitem data 
				lp->SetSubItemText(plvItem->iSubItem-1, str);
		   UpdateData(lpItem, lp); //update internal nodes
		}
		SetItemText(plvItem->iItem, plvItem->iSubItem, str.GetBuffer(1));
	}
	else
		CSuperGridCtrl::OnUpdateListViewItem(lpItem, plvItem);
}



			
BOOL CVenusGridCtrl::OnItemLButtonDown(LVHITTESTINFO& ht)
{
	if(ht.iItem!=-1)
	{
#ifdef _WHAT_EVER_
		CTreeItem*pSelItem = GetTreeItem(ht.iItem);
		if(pSelItem!=NULL)
		{	
			CItemInfo* pInfo = GetData(pSelItem);
			if((ht.iSubItem==0) && (pInfo->GetItemText()==CString(_T("Hello World"))))
			{
				AfxMessageBox("no can do buddy");
				return 0;
			}

			for(int i=0;i<pInfo->GetItemCount(); i++)
			{
				if(i==ht.iSubItem-1)
				{
					CString str=pInfo->GetSubItem(i);
					if(str == CString(_T("Ethan Hunt"))) 
					{
						AfxMessageBox("Tom Cruise\nliked him in a few good men");
						return 0;
					}
				}
			}
		}
#endif
	}
	return 1;
}

BOOL CVenusGridCtrl::OnVkReturn()
{
	BOOL bResult=FALSE;
	int iItem = GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
	if( GetCurSubItem() != -1 && iItem != -1)
	{
		CTreeItem*pSelItem = GetTreeItem(iItem);
		if(pSelItem!=NULL)
		{	
			CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
			int iSubItem = Header_OrderToIndex(pHeader->m_hWnd, GetCurSubItem());
			CItemInfo* pInfo = GetData(pSelItem);
			CItemInfo::CONTROLTYPE ctrlType;
			if(pInfo->GetControlType(iSubItem-1, ctrlType))
			{	
				switch(ctrlType)
				{
					/*put in your own control here*/
					case CItemInfo::datecontrol:break;
					case CItemInfo::spinbutton:break;
					case CItemInfo::dropdownlistviewwhatevercontrol:break;
					case CItemInfo::combobox: 
						{

							CStringList* list=NULL;
							pInfo->GetListData(iSubItem-1, list);
							CComboBox * pList = ShowList(iItem, iSubItem, list);
							bResult=TRUE; //I'll handle it from here
						}break;
					default:break;
				}
			}
		}
	}
	return bResult;
}

void CVenusGridCtrl::HowToInsertItemsAfterTheGridHasBeenInitialized(int nIndex, const CString& str)
{
	CTreeItem *pSelItem = GetTreeItem(nIndex);
	if(pSelItem != NULL)
	{
		SetRedraw(0);
		BOOL bUpdate = FALSE;
		if(!IsCollapsed(pSelItem))
			bUpdate = TRUE;//Children are expanded, want to see update right away if not no visual update

		CItemInfo* lpRelative = new CItemInfo();
		lpRelative->SetItemText(str);
		lpRelative->AddSubItemText(_T("I am"));
		lpRelative->AddSubItemText(_T("now"));
		lpRelative->AddSubItemText(_T("going to insert"));
		lpRelative->AddSubItemText(_T("some items"));

		CTreeItem* pParent = InsertItem(pSelItem, lpRelative, bUpdate);
		for(int i=0; i < GetNumCol(); i++)
		{
			CItemInfo* lpItemInfo = new CItemInfo();
			CString strItem;
			strItem.Format(_T("Item %d"), i);
			//add items text
			lpItemInfo->SetItemText(strItem);
			//add subitem text
			for(int y=0;y < GetNumCol()-1; y++) 
			{
				CString str;
				str.Format(_T("SubItem %d of %s"), y, lpItemInfo->GetItemText());
				lpItemInfo->AddSubItemText(str);
			}
			//set combobox in last col
			lpItemInfo->SetControlType(CItemInfo::combobox, GetNumCol()-2);
			CStringList list;
			for(int x = 0; x < 3; x++)
			{
				CString str;
				str.Format("ListItem %d",x);
				list.AddTail(str);
			}
			lpItemInfo->SetListData(GetNumCol()-2, &list);

			InsertItem(pParent, lpItemInfo);
		}
		SetRedraw(1);
		InvalidateRect(NULL);
		UpdateWindow();
	}
}



void CVenusGridCtrl::HowToLoopThroughAllItems_if_we_wanted_to_print_them_or_what_ever(CDC *pDC)
{
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int cy = tm.tmHeight + tm.tmExternalLeading;
	int nLineY=0;
	nLineY+=cy;

	pDC->TextOut(10, nLineY, _T("GIVE ME A BREAK YOU KNOW PRINT PREVIEW IS NOT THE ISSUE HERE"));
	nLineY+=cy;
	nLineY+=cy;
	int nIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); //print from current node
	if(nIndex!=-1)
	{
		//GetHeadPosition
		POSITION pos = GetRootHeadPosition();
		while(pos != NULL)
		{
			CTreeItem *pParent =(CTreeItem*)GetNextRoot(pos); 
			CTreeItem *pItem = pParent;
			CItemInfo* lp = GetData(pParent);

			CString strData = lp->GetItemText();
			strData+='\t';
			nLineY+=cy;
			for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
			{
				CString str = lp->GetSubItem(nCol);
				strData+=str;
				strData+='\t';
			}	
			pDC->TabbedTextOut(10,nLineY,strData,strData.GetLength(), 0, NULL, 0);
			nLineY+=cy;
			//GetNext ....loop through all children 
			for(;;)
			{
				CTreeItem *pCur = GetNext(pParent,pItem, TRUE, FALSE/*regardless of the item are hidden or not*/);	  

				if(!IsChildOf(pParent, pCur))
					break;
				else
				if(pCur==pItem)
					break;
				
				CItemInfo* lp = GetData(pCur);
				CString strData = lp->GetItemText();
				strData+='\t';
				for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
				{
					CString str = lp->GetSubItem(nCol);
					strData+=str;
					strData+='\t';
				}	
				pDC->TabbedTextOut(10,nLineY,strData,strData.GetLength(), 0, NULL, 0);
				nLineY+=cy;
				pItem=pCur;
			}
		}
	 }
}



void CVenusGridCtrl::HowToLoopThroughAllItems_that_has_a_checkmark_and_print_them_or_what_ever(CDC *pDC)
{
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int cy = tm.tmHeight + tm.tmExternalLeading;
	int nLineY=0;
	nLineY+=cy;

	pDC->TextOut(10,nLineY,_T("GIVE ME A BREAK YOU KNOW PRINT PREVIEW IS NOT THE ISSUE HERE"));
	nLineY+=cy;
	nLineY+=cy;
	if(!GetItemCount())
		return;

	int nIndex=0;//has to be the root 
	if(nIndex!=-1)
	{
		//do a GetHeadPosition
		POSITION pos = GetRootHeadPosition();
		while(pos != NULL)
		{
			CTreeItem *pParent = (CTreeItem*)GetNextRoot(pos); 
			CTreeItem *pItem = pParent;
			CItemInfo* lp = GetData(pParent);
			if(lp->GetCheck())//very hard :)=
			{
				CString strData=lp->GetItemText();
				strData+='\t';
				nLineY+=cy;
				for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
	
				{
					CString str = lp->GetSubItem(nCol);
					strData+=str;
					strData+='\t';
				}	
				pDC->TabbedTextOut(10,nLineY,strData,strData.GetLength(), 0, NULL, 0);
				nLineY+=cy;
			}
			//GetNext ....loop through all children 
			for(;;)
			{
				CTreeItem *pCur = GetNext(pParent, pItem, TRUE, FALSE/*regardless of the item are hidden or not*/);	  
				if(!IsChildOf(pParent, pCur))
					break;
				else
				if(pCur==pItem)
					break;
				

				CItemInfo* lp = GetData(pCur);
				if(lp->GetCheck())
				{
					CString strData = lp->GetItemText();
					strData+='\t';
					for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
					{
						CString str = lp->GetSubItem(nCol);
						strData+=str;
						strData+='\t';
					}	
					pDC->TabbedTextOut(10,nLineY,strData,strData.GetLength(), 0, NULL, 0);
					nLineY+=cy;
				}
				pItem=pCur;
			}
		}//while
	 }//if
}


//HOWTO: Search nodeptr that have a specific item and subitems also shows you how to select the node and delete it
void CVenusGridCtrl::HowToSearch_I_am_using_hardcoded_values_here_cause_I_am_tired_now(void)
{

	//one Item and two Subitems
	CTreeItem *pNode =	Search(__T("Hello World"),_T("Happy"),_T("Programming"),NULL);
	
	if(pNode!=NULL)
	{
		CItemInfo *pInfo = GetData(pNode);
		AfxMessageBox(_T("Found Item ") + pInfo->GetItemText());
	}
	else AfxMessageBox(_T("not found"));
	//one Item and one Subitem
	CTreeItem *pNode1 = Search(_T("Mission: Impossible"),_T("Allan Nielsen"),NULL);
	if(pNode1!=NULL)
	{
		CItemInfo *pInfo = GetData(pNode1);
		AfxMessageBox(_T("Found Item ") + pInfo->GetItemText());

	}
	else AfxMessageBox(_T("not found"));
	
	//one Item and one Subitem
	CTreeItem *pNode2 = Search(_T("Training Agent"),_T("Mr. Bean"),NULL);
	if(pNode2!=NULL)
	{
		CItemInfo *pInfo = GetData(pNode2);
		AfxMessageBox(_T("Found Item") + pInfo->GetItemText());
	}
	else AfxMessageBox(_T("not found"));

	CTreeItem *pNode3 = Search(_T("BC"),NULL);
	if(pNode3!=NULL)
	{
		CItemInfo *pInfo = GetData(pNode3);
		AfxMessageBox(_T("Found Item ") + pInfo->GetItemText());
		int nIndex = SelectNode(pNode3);
		if(nIndex!=-1)
		if(AfxMessageBox(_T("Do you want to delete it"),MB_OKCANCEL)==IDOK)
			DeleteItemEx(pNode3, nIndex);
	}
	else AfxMessageBox(_T("not found"));

	CTreeItem *pItem = NULL;

	POSITION pos = GetRootHeadPosition();
	while(pos != NULL)
	{
		CTreeItem * pRoot = (CTreeItem*)GetNextRoot(pos); 
		pItem = SearchEx(pRoot, "AB");
		if(pItem!=NULL)
		{
			CItemInfo *pInfo = GetData(pItem);
			AfxMessageBox(_T("Found Item ") + pInfo->GetItemText());
			int nIndex = SelectNode(pItem);
			if(nIndex!=-1)
			if(AfxMessageBox(_T("Do you want to delete it"),MB_OKCANCEL)==IDOK)
				DeleteItemEx(pItem, nIndex);
		}
	}
		
	if( pItem == NULL )
		AfxMessageBox(_T("not found"));


}

BOOL CVenusGridCtrl::OnDeleteItem(CTreeItem* pItem, int nIndex)
{
	return 1;
}



BOOL CVenusGridCtrl::OnItemExpanding(CTreeItem *pItem, int iItem)
{
	return 1;
}


BOOL CVenusGridCtrl::OnItemExpanded(CTreeItem* pItem, int iItem)
{
	return 1;
}


BOOL CVenusGridCtrl::OnCollapsing(CTreeItem *pItem)
{
	return 1;
}



BOOL CVenusGridCtrl::OnItemCollapsed(CTreeItem *pItem)
{
	return 1;
}

//this is just one way to search items...strItem must match and then all subitems must be
//a match before returning the node
//the search function here search all nodes regardless if collapsed or expanded
CVenusGridCtrl::CTreeItem* CVenusGridCtrl::Search(CString strItem,...)
{
	if(!GetItemCount())
		return NULL;

	va_list args;
	va_start(args, strItem);
	int nCount=0;
	
	for(;;)
	{
		LPCTSTR lpsz = va_arg(args, LPCTSTR);
		if(lpsz==NULL)
			break;
	   nCount++;
	}


	POSITION pos = GetRootHeadPosition();
	while(pos != NULL)
	{
		CTreeItem *pParent = (CTreeItem*)GetNextRoot(pos); 
		CTreeItem *pItem = pParent;
		CItemInfo* lp = GetData(pParent);
		CString strData = lp->GetItemText();
		if(strData==strItem)//must be a match before going further...suit you self
		{
			va_start(args, strItem);
			int nResult=0;
			for(int i=0; i<nCount;i++)
			{
				LPCTSTR lpsz = va_arg(args, LPCTSTR);   
				for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
				{
					CString str = lp->GetSubItem(nCol);
					if(!str.CompareNoCase(lpsz))
					{
						nResult++;
						break;
					}
				}	
			}
			va_end(args);
			if(nCount==nResult)//all args was found return node
				return pParent;
		}

		//GetNext ....loop through all children 
		for(;;)
		{
			CTreeItem *pCur = GetNext(pParent, pItem, TRUE, FALSE/*regardless of the item are hidden or not, set to TRUE if only visible items must be included in the search*/);	  
			if(!IsChildOf(pParent, pCur))
				break;
			else
			if(pCur==pItem)
				break;
			CItemInfo* lp = GetData(pCur);
			CString strData = lp->GetItemText();
			if(strData==strItem)//must be a match before going further
			{
				va_start(args, strItem);
				int nResult=0;
				for(int i=0; i<nCount;i++)
				{
					LPCTSTR lpsz = va_arg(args, LPCTSTR);   
					for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
					{
						CString str = lp->GetSubItem(nCol);
						if(!str.CompareNoCase(lpsz))
						{
							nResult++;
							break;
						}
					}	
				}
				va_end(args);
				if(nCount==nResult)//all args was found return node
					return pCur;
			}
			pItem=pCur;//next;
		}
	}	
	return NULL;
}



void CVenusGridCtrl::SortData()
{
	int nIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
	if(nIndex==-1)
		return;

	CTreeItem *pItem = reinterpret_cast<CTreeItem*>(GetItemData(nIndex));

	if(AfxMessageBox("Sort all children(Yes)\nor just sort rootitems(No)",MB_YESNO)==IDYES)
		Sort(pItem, TRUE);			
	else
		Sort(pItem, FALSE);			
	//do a simple refresh thing
	if(ItemHasChildren(pItem))
	{
		SetRedraw(0);
		Collapse(pItem);
		Expand(pItem, nIndex);
		SetRedraw(1);
	}
}



//another search thing
CVenusGridCtrl::CTreeItem* CVenusGridCtrl::SearchEx(CTreeItem *pStartPosition, CString strItem)
{
	CItemInfo* lp = GetData(pStartPosition);
	//if(lp->GetCheck()) another condition here maybe
	CString strData = lp->GetItemText();
	if(strData==strItem)
	{
		return pStartPosition;
	}

	const int nChildren = NumChildren(pStartPosition);
	if (nChildren > 0)
	{
		POSITION pos = GetHeadPosition(pStartPosition);
		while (pos)
		{
			CTreeItem *pChild = GetNextChild(pStartPosition, pos);
			CItemInfo* lp = GetData(pChild);
			CString strData = lp->GetItemText();
			if(strData==strItem)
			{
				return pChild;
			}
		}
	}

	POSITION pos = GetHeadPosition(pStartPosition);
	while (pos)
	{
		CTreeItem *pChild = GetNextChild(pStartPosition, pos);
		CItemInfo* lp = GetData(pChild);
		CString strData = lp->GetItemText();
		if(strData==strItem)
		{
			return pChild;
		}

		pChild = SearchEx(pChild, strItem);
		if(pChild!=NULL)
			return pChild;
	}
	return NULL;
}
