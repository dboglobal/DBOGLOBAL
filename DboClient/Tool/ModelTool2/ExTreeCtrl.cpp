#include "StdAfx.h"
#include "resource.h"
#include "ExTreeCtrl.h"
#include "NtlDebug.h"

CExTreeCtrl::CExTreeCtrl(void)
{
    m_bDrag = FALSE;
}

CExTreeCtrl::~CExTreeCtrl(void)
{
}
BEGIN_MESSAGE_MAP(CExTreeCtrl, CXTTreeCtrl)
    ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CExTreeCtrl::OnTvnBegindrag)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CExTreeCtrl::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    
    // 마우스 커서를 드래깅 모드로 변경
    if(pNMTreeView->itemNew.hItem)
    {
        SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)AfxGetApp()->LoadCursor(IDC_CURSOR1));
        m_bDrag = TRUE;
        SetCapture();
    }

    *pResult = 0;
}

void CExTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    if(m_bDrag)
    {
        if(point.y <= 0 && m_hCurItem)
        {
            m_hCurItem = GetPrevItem(m_hCurItem);            
        }        
        else
        {
            m_hCurItem = HitTest(point);            
        }   

        if(m_hCurItem)
        {
            SelectDropTarget(m_hCurItem);  
            EnsureVisible(m_hCurItem);
        }
    }

    CXTTreeCtrl::OnMouseMove(nFlags, point);
}

void CExTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    //커서를 원래대로 변경
    SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)LoadCursor(NULL, IDC_ARROW));

    if(m_bDrag && m_hCurItem)
    {
        ReleaseCapture();
        
        m_bDrag = FALSE;

        int nImageIndex = 0, nSelectImageIndex = 0;
        GetItemImage(m_hCurItem, nImageIndex, nSelectImageIndex);
        if(nImageIndex == 0) // 폴더 밑에만 이동이 가능하다
        {
            OnMoveItem(GetFirstSelectedItem(), m_hCurItem);            
        }
        
        SelectDropTarget(NULL);
    }

    CXTTreeCtrl::OnLButtonUp(nFlags, point);
}

void CExTreeCtrl::OnMoveItem(HTREEITEM hItem, HTREEITEM hParentItem, BOOL bRecrusive)
{
    while(hItem)
    {
        if(hItem == hParentItem)
        {
            hItem = GetNextSelectedItem(hItem);
            continue;
        }
            
        int nImageIndex = 0, nSelectedImage = 0;
        GetItemImage(hItem, nImageIndex, nSelectedImage);
        CString strItemName = GetItemText(hItem);

        TV_INSERTSTRUCT sTvStruct;
        sTvStruct.hParent = hParentItem;
        sTvStruct.hInsertAfter = TVI_SORT;
        sTvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        sTvStruct.item.iImage = nImageIndex;
        sTvStruct.item.iSelectedImage = nSelectedImage;
        sTvStruct.item.pszText = (LPSTR)(LPCSTR)strItemName;    
        HTREEITEM hNewItem = InsertItem(&sTvStruct);        

        HTREEITEM hDeleteItem = hItem;

        if(ItemHasChildren(hItem))
        {
            OnMoveItem(GetChildItem(hItem), hNewItem, TRUE);
        }

        if(bRecrusive)
        {
            hItem = GetNextSiblingItem(hItem);
        }
        else
        {
            hItem = GetNextSelectedItem(hItem);
        }
        
        SetItemState(hDeleteItem, 0, TVIS_FOCUSED);
        DeleteItem(hDeleteItem);        

        if(!hItem && !bRecrusive)
        {
            SelectItem(hNewItem);
            break;
        }
    }
}

void CExTreeCtrl::GetAllChildNode(HTREEITEM hItem, TREEITEM_VECTOR& vTreeItem)
{
    while (hItem)
    {
        vTreeItem.push_back(hItem);

        if (ItemHasChildren(hItem))
        {
            GetAllChildNode(GetChildItem(hItem), vTreeItem);  //재귀호출
        } 

        hItem = GetNextSiblingItem(hItem);
    }    
}

void CExTreeCtrl::SaveTree(const char* szFileName)
{
    if(!szFileName)
        return;
    
    CNtlMTXml xmlSave;
    xmlSave.CreateXML((LPSTR)(LPCSTR)GetItemText(GetRootItem()));

    TREEITEM_VECTOR vTreeItem;
    GetAllChildNode(GetRootItem(), vTreeItem);
    for(size_t i = 0; i < vTreeItem.size(); ++i)
    {
        HTREEITEM pItem = GetParentItem(vTreeItem[i]);
        if(!pItem)
            continue;
        int nImage = 0, nSelectedImage = 0;
        GetItemImage(vTreeItem[i], nImage, nSelectedImage);
        CString sParentName = GetItemText(pItem);
        CString sItemName = GetItemText(vTreeItem[i]);

        if(sItemName != "New Script")   // Save it only when it is not New Script.
        {
            // Add to XML.
            if(nImage == 0)    // folder
            {
                xmlSave.AddFolderNode((LPSTR)(LPCSTR)sParentName, (LPSTR)(LPCSTR)sItemName);
            }
            else  // script
            {
                xmlSave.AddScriptNode((LPSTR)(LPCSTR)sParentName, (LPSTR)(LPCSTR)sItemName);
            }        
        }
    }
    xmlSave.Save((char*)szFileName);  

}

void CExTreeCtrl::LoadTree(const char* szFileName)
{
    if(!szFileName)
        return;

    CNtlMTXml xmlLoad;
	
    xmlLoad.LoadTreeXML((char*)szFileName, &m_itemNode);

    DeleteAllItems();
    InsertItemRecursive(&m_itemNode, NULL);

    Expand(GetRootItem(), TVE_EXPAND);
}

void CExTreeCtrl::InsertItemRecursive(SItemNode* pItemNode, HTREEITEM hParentItem)
{
	// 자신을 추가한다.
	TV_INSERTSTRUCT sTvStruct;
	sTvStruct.hParent = hParentItem;
	sTvStruct.hInsertAfter = TVI_SORT;
	sTvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	sTvStruct.item.pszText = (LPSTR)(LPCSTR)pItemNode->strNodeName;

	if(pItemNode->eNodeType == NODE_SCIRPT)
	{
		// 스크립트라면 자신을 추가하고 끝낸다.
		sTvStruct.item.iImage = 2;
		sTvStruct.item.iSelectedImage = 2;
		InsertItem(&sTvStruct);
	}
	else	// 폴더라면 자식들을 재귀로 돈다.
	{
		sTvStruct.item.iImage = 0;
		sTvStruct.item.iSelectedImage = 0;
		HTREEITEM hItem = InsertItem(&sTvStruct);

		for each(SItemNode* pChildNode in pItemNode->vecChildList)
		{
			if(pChildNode)
			{
				InsertItemRecursive(pChildNode, hItem);
			}
		}
	}	
}

void CExTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // Del 키로 항목 삭제
    if(nChar == VK_DELETE)
    {
        HTREEITEM hItem = GetFirstSelectedItem();
        while(hItem)
        {
            HTREEITEM hTempItem = GetNextSelectedItem(hItem);
            DeleteItem(hItem);
            
            hItem = hTempItem;
        }
    }

    CXTTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CExTreeCtrl::ResetBoldColor() 
{
    for(UINT i = 0; i < m_vBoldColorItem.size(); ++i)
    {
        SetItemBold(m_vBoldColorItem[i], FALSE);
        SetItemColor(m_vBoldColorItem[i], RGB(0, 0, 0));
    }

    m_vBoldColorItem.clear();
}

void CExTreeCtrl::SetItemBold( HTREEITEM hItem, BOOL bBold /*= TRUE*/ ) 
{
    if(!hItem)
        return;

    if(bBold)
    {
        m_vBoldColorItem.push_back(hItem);
    }

    __super::SetItemBold(hItem, bBold);
}
void CExTreeCtrl::SetItemColor( HTREEITEM hItem, COLORREF color ) 
{
    if(!hItem)
        return;

    if(color != RGB(0, 0, 0))
    {
        m_vBoldColorItem.push_back(hItem);
    }

    __super::SetItemColor(hItem, color);
}
