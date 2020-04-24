// MTTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool.h"
#include "MTTreeCtrl.h"
#include "PagePC.h"
#include "NtlMTXml.h"

// CMTTreeCtrl

IMPLEMENT_DYNAMIC(CMTTreeCtrl, CTreeCtrl)


CMTTreeCtrl::CMTTreeCtrl()
{
    m_pImageList = NULL;
    m_hSelectedItem = NULL;
    m_bDrag = FALSE;
    m_pParentView = NULL;
}

CMTTreeCtrl::~CMTTreeCtrl()
{
    if(m_pImageList)
    {
        delete m_pImageList;
        m_pImageList = NULL;
    }
}


BEGIN_MESSAGE_MAP(CMTTreeCtrl, CTreeCtrl)
    ON_WM_CONTEXTMENU()    
    ON_COMMAND(ID_MENU_ADD_FOLDER, &CMTTreeCtrl::OnMenuAddFolder)    
    ON_COMMAND(ID_MENU_ADD_SCRIPT, &CMTTreeCtrl::OnMenuAddScript)
    //ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, &CMTTreeCtrl::OnTvnItemexpanding)    
    ON_COMMAND(ID_MENU_NEW_SCRIPT, &CMTTreeCtrl::OnMenuNewScript)    
    ON_COMMAND(ID_MENU_LOAD_SCRIPT, &CMTTreeCtrl::OnMenuLoadScript)
    ON_COMMAND(ID_MENU_DELETE_FOLDER, &CMTTreeCtrl::OnMenuDeleteFolder)
    ON_COMMAND(ID_MENU_DELETE_SCRIPT, &CMTTreeCtrl::OnMenuDeleteScript)    
    ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CMTTreeCtrl::OnTvnBegindrag)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
    ON_COMMAND(ID_MENU_RENAME_FOLDER, &CMTTreeCtrl::OnMenuRenameFolder)
    ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CMTTreeCtrl::OnTvnEndlabeledit)
    ON_COMMAND(ID_MENU_SCRIPT_SAVE, &CMTTreeCtrl::OnMenuScriptSave)
    ON_UPDATE_COMMAND_UI(ID_MENU_SCRIPT_SAVE, &CMTTreeCtrl::OnUpdateMenuScriptSave)
    ON_COMMAND(ID_MENU_SCRIPT_SAVEAS, &CMTTreeCtrl::OnMenuScriptSaveas)
    ON_COMMAND(ID_MENU_SCRIPT_RENAME, &CMTTreeCtrl::OnMenuScriptRename)
    ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, &CMTTreeCtrl::OnTvnBeginlabeledit)
    ON_WM_SIZE()
END_MESSAGE_MAP()



// CMTTreeCtrl message handlers
void CMTTreeCtrl::SetRoot(WCHAR* szRootName)
{
    // 이미지 리스트 설정
    m_pImageList = new CImageList();
    m_pImageList->Create(16, 16, ILC_COLOR32, 4, 4);
    m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON7));
    m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON8));
    m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON6));    
    m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON9));    
    SetImageList(m_pImageList, TVSIL_NORMAL);

    // 트리 컨트롤 초기화
    TV_INSERTSTRUCT tvStruct;
    tvStruct.hParent      = NULL;
    tvStruct.hInsertAfter = TVI_LAST;
    tvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvStruct.item.pszText = szRootName;
    tvStruct.item.iImage  = 0;
    tvStruct.item.iSelectedImage = 1;
    m_hSelectedItem = InsertItem(&tvStruct);

}

void CMTTreeCtrl::SetParent(CPagePC* pLeftView)
{
    m_pParentView = pLeftView;
}


void CMTTreeCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
{
    m_hSelectedItem = GetSelectedItem();

    CPoint tempPoint = point;
    ScreenToClient(&tempPoint);
    HTREEITEM hItem     = HitTest(tempPoint);
    if(hItem)
    {
        SelectItem(hItem);
        m_hSelectedItem = hItem;
    }
    if(m_hSelectedItem)
    {
        int nImage = 0;
        int nSelectedImage = 0;
        GetItemImage(m_hSelectedItem, nImage, nSelectedImage);
        
        CMenu  menuPopup;         
        CMenu* subMenu = NULL;
        menuPopup.LoadMenu(IDR_MENU1);

        if(nImage == 0)         // 폴더
        {
            // 메뉴를 연다.
            subMenu = menuPopup.GetSubMenu(0);
            subMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);        
            
        }
        else if(nImage == 3)    // 스크립트 
        {
            subMenu = menuPopup.GetSubMenu(1);            
            subMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);        
        }       
    }    
}

void CMTTreeCtrl::OnMenuAddFolder()
{
    // 새 폴더를 추가한다.

    m_hSelectedItem = GetSelectedItem();

    if(!m_hSelectedItem)
        return;

    TV_INSERTSTRUCT tvStruct;
    tvStruct.hParent      = m_hSelectedItem;    
    tvStruct.hInsertAfter = TVI_LAST;
    tvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvStruct.item.pszText = L"새폴더";
    tvStruct.item.iImage  = 0;
    tvStruct.item.iSelectedImage = 1;
    HTREEITEM hAddItem = InsertItem(&tvStruct);

    Select(hAddItem, TVGN_CARET);
    EditLabel(hAddItem);
}


void CMTTreeCtrl::OnMenuAddScript()
{
    m_hSelectedItem = GetSelectedItem();

    if(!m_hSelectedItem)
        return;

    TV_INSERTSTRUCT tvStruct;
    tvStruct.hParent      = m_hSelectedItem;
    tvStruct.hInsertAfter = TVI_LAST;
    tvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvStruct.item.pszText = L"새스크립트";
    tvStruct.item.iImage  = 2;    
    tvStruct.item.iSelectedImage = 2;
    HTREEITEM hAddItem = InsertItem(&tvStruct);
    
    Select(hAddItem, TVGN_CARET);
    EditLabel(hAddItem);
}

void CMTTreeCtrl::OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    m_hSelectedItem = GetSelectedItem();
    HTREEITEM hItem = GetChildItem(m_hSelectedItem);
    if(hItem)
    {
        int nImage          = 0;
        int nSelectedImage  = 0;
        GetItemImage(hItem, nImage, nSelectedImage);
        if(nImage == 3)
        {
            *pResult = 1;  
            return;
        }        
    }
    
    *pResult = 0;
}

CString CMTTreeCtrl::SelectScriptName()
{
    m_hSelectedItem = GetSelectedItem();
    if(!m_hSelectedItem)
        return L"";

    int nImage = 0;
    int nSelectedImage = 0;
    GetItemImage(m_hSelectedItem, nImage, nSelectedImage);

    if(nImage != 3)     // 스크립트 파일 로드일때만 표시
        return L"";
    
    return GetItemText(m_hSelectedItem);    
}

void CMTTreeCtrl::SetScriptName(CString sScriptName)
{
    m_hSelectedItem = GetSelectedItem();
    if(!m_hSelectedItem)
        return;
    
    SetItemText(m_hSelectedItem, sScriptName);
}

CString CMTTreeCtrl::SelectClumpName()
{
    m_hSelectedItem = GetSelectedItem();
    if(!m_hSelectedItem)
        return L"";

    int nImage = 0;
    int nSelectedImage = 0;
    GetItemImage(m_hSelectedItem, nImage, nSelectedImage);

    if(nImage != 2)     // 스크립트 파일 로드일때만 표시
        return L"";

    HTREEITEM hItem = GetChildItem(m_hSelectedItem);
    while(hItem)
    {
        CString strFileName = GetItemText(hItem);
        if(strFileName.MakeUpper().Find(L".DFF") > -1)
        {
            return strFileName;
        }
        else
        {
            hItem = GetNextSiblingItem(hItem);
        }
    }

    return L"";
}

void CMTTreeCtrl::OnMenuNewScript()
{
    // 새로운 스크립트를 폴더에 추가한다.
    m_hSelectedItem = GetSelectedItem();

    // 스크립트 항목 추가
    TV_INSERTSTRUCT tvStruct;
    tvStruct.hParent      = m_hSelectedItem;
    tvStruct.hInsertAfter = TVI_LAST;
    tvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvStruct.item.pszText = L"New Script";
    tvStruct.item.iImage  = 3;    
    tvStruct.item.iSelectedImage = 3;
    HTREEITEM hAddItem = InsertItem(&tvStruct);
   
    SelectItem(hAddItem);

    if(m_pParentView)
    {
        m_pParentView->OnMenuNewScript();
    }
}


void CMTTreeCtrl::OnMenuLoadScript()
{
    if(m_pParentView)
        m_pParentView->OnMenuLoadScript();    
}

void CMTTreeCtrl::AddItem(const RwChar* sScriptName)
{
    USES_CONVERSION;

    if(!sScriptName)
        return;

    m_hSelectedItem = GetSelectedItem();

    // 항목 추가
    TV_INSERTSTRUCT sTvStruct;
    sTvStruct.hParent = m_hSelectedItem;
    sTvStruct.hInsertAfter = TVI_LAST;
    sTvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    sTvStruct.item.iImage = 3;
    sTvStruct.item.iSelectedImage = 3;
    sTvStruct.item.pszText = A2W(sScriptName);
    HTREEITEM hItem = InsertItem(&sTvStruct);
    SelectItem(hItem);
}

void CMTTreeCtrl::OnMenuDeleteFolder()
{
    if(MessageBox(L"정말로 삭제하시겠습니까?", NULL, MB_OKCANCEL) == IDOK)
    {
        m_hSelectedItem = GetSelectedItem();
        DeleteItem(m_hSelectedItem);
    }
}

void CMTTreeCtrl::OnMenuDeleteScript()
{
    if(MessageBox(L"정말로 삭제하시겠습니까?", NULL, MB_OKCANCEL) == IDOK)
    {
        m_hSelectedItem = GetSelectedItem();
        if(m_pParentView)
            m_pParentView->OnMenuDeleteScript(m_hSelectedItem);
        DeleteItem(m_hSelectedItem);        
    }    
}

void CMTTreeCtrl::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    
    // 마우스 커서를 드래깅 모드로 변경
    SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)AfxGetApp()->LoadCursor(IDC_CURSOR1));
    m_bDrag = TRUE;
    SelectItem(pNMTreeView->itemNew.hItem);
    m_hDragItem = pNMTreeView->itemNew.hItem;

    *pResult = 0;
}

void CMTTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    if(m_bDrag)
    {
        m_hCurItem = HitTest(point);
        SelectItem(m_hCurItem);
    }

    CTreeCtrl::OnMouseMove(nFlags, point);
}

void CMTTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    if(m_bDrag)
    {
        //커서를 원래대로 변경
        SetClassLong(this->m_hWnd, GCL_HCURSOR, (long)LoadCursor(NULL, IDC_ARROW));
        m_bDrag = FALSE;
        EndDrag(point);
    }

    CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CMTTreeCtrl::EndDrag(CPoint point)
{
    // 드래그가 끝난 아이템이 폴더일때만 이동한다.
    int nImageIndex = 0, nSelectImageIndex = 0;
    GetItemImage(m_hCurItem, nImageIndex, nSelectImageIndex);
    if(nImageIndex != 0)
    {
        SelectItem(m_hDragItem);
        return;
    }

    CString strItemName = GetItemText(m_hDragItem);
    // 항목 추가
    TV_INSERTSTRUCT sTvStruct;
    sTvStruct.hParent = m_hCurItem;
    sTvStruct.hInsertAfter = TVI_LAST;
    sTvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    sTvStruct.item.iImage = 3;
    sTvStruct.item.iSelectedImage = 3;
    sTvStruct.item.pszText = (LPWSTR)(LPCWSTR)strItemName;    
    HTREEITEM hItem = InsertItem(&sTvStruct);

    DeleteItem(m_hDragItem);

    SelectItem(hItem);        
}

void CMTTreeCtrl::OnMenuRenameFolder()
{
    m_hSelectedItem = GetSelectedItem();
    EditLabel(m_hSelectedItem);
}

void CMTTreeCtrl::SaveTree(WCHAR* szRootName, WCHAR* szFileName)
{
    if(!szFileName)
        return;
        
    CNtlMTXml xmlSave;
    xmlSave.CreateXML(szRootName);
    
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

        if(sItemName != "New Script")   // New Script가 아닐때만 저장한다.
        {
            // XML에 추가한다.
            if(nImage == 0)    // 폴더
            {
                xmlSave.AddFolderNode((LPWSTR)(LPCWSTR)sParentName, (LPWSTR)(LPCWSTR)sItemName);
            }
            else if(nImage == 3)  // 스크립트
            {
                xmlSave.AddScriptNode((LPWSTR)(LPCWSTR)sParentName, (LPWSTR)(LPCWSTR)sItemName);
            }        
        }
    }
    xmlSave.Save(szFileName);    
}

void CMTTreeCtrl::LoadTree(WCHAR* szFileName)
{
    if(!szFileName)
        return;

    

    CNtlMTXml xmlLoad;
    ITEM_VECTOR vItem = xmlLoad.LoadTreeXML(szFileName);
    if(vItem.size() == 0)
    {
        return;
    }

    DeleteAllItems();
    for(size_t i = 0; i < vItem.size(); ++i)
    {
        TV_INSERTSTRUCT sTvStruct;
        sTvStruct.hParent = GetItemByName(vItem[i].szParenetName);
        sTvStruct.hInsertAfter = TVI_LAST;
        sTvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        if(vItem[i].nImageKind == 0)
        {
            sTvStruct.item.iImage = 0;
            sTvStruct.item.iSelectedImage = 1;
        }
        else
        {
            sTvStruct.item.iImage = 3;
            sTvStruct.item.iSelectedImage = 3;
        }
        
        sTvStruct.item.pszText = vItem[i].szNodeName;

        HTREEITEM hItem = InsertItem(&sTvStruct);
    }

    Expand(GetRootItem(), TVE_EXPAND);
}

HTREEITEM CMTTreeCtrl::GetItemByName(WCHAR* szItemName)
{
    // 모든 트리의 아이템을 검색해서 이름을 찾아낸다.
    TREEITEM_VECTOR vTreeItem;
    GetAllChildNode(GetRootItem(), vTreeItem);
    for(size_t i = 0; i < vTreeItem.size(); ++i)
    {
        CString sItemText = GetItemText(vTreeItem[i]);
        if(sItemText == szItemName)
        {
            return vTreeItem[i];
        }
    }

    return NULL;
}

void CMTTreeCtrl::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
    m_hSelectedItem = GetSelectedItem();
    CString sITemName = pTVDispInfo->item.pszText;
    if(sITemName != "")
    {
        SetItemText(m_hSelectedItem, pTVDispInfo->item.pszText);

        // 스크립트 라면 파일 이름도 변경한다.
        int nImage = 0, nSelectImage = 0;
        GetItemImage(m_hSelectedItem, nImage, nSelectImage);
        if(nImage == 3 && m_pParentView && m_sPrevScriptName != "")
        {
            m_pParentView->OnMenuScriptRename(m_sPrevScriptName, sITemName);
        }
    }    

    

    *pResult = 0;
}

void CMTTreeCtrl::GetAllChildNode(HTREEITEM hItem, TREEITEM_VECTOR& vTreeItem)
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

void CMTTreeCtrl::OnMenuScriptSave()
{
    if(m_pParentView)
        m_pParentView->OnMenuScriptSave();
}

void CMTTreeCtrl::OnUpdateMenuScriptSave(CCmdUI *pCmdUI)
{
    
}

void CMTTreeCtrl::OnMenuScriptSaveas()
{
    if(m_pParentView)
        m_pParentView->OnMenuScriptSaveas();
}

void CMTTreeCtrl::OnMenuScriptRename()
{
    m_sPrevScriptName = GetItemText(GetSelectedItem());
    EditLabel(GetSelectedItem());
}

void CMTTreeCtrl::OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
    m_sPrevScriptName = pTVDispInfo->item.pszText;
    *pResult = 0;
}

BOOL CMTTreeCtrl::PreTranslateMessage(MSG* pMsg)
{
    if(GetEditControl() && pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
    {
        // 이름 변경시 엔터를 입력하면 적용이 되게 한다.
        CString sNewName;
        GetEditControl()->GetWindowText(sNewName);
        SetItemText(GetSelectedItem(), sNewName);        
        SelectItem(GetSelectedItem());
        ::TranslateMessage(pMsg);
        ::DispatchMessage(pMsg);
        return true;
    }

    return CTreeCtrl::PreTranslateMessage(pMsg);
}

void CMTTreeCtrl::OnSize(UINT nType, int cx, int cy)
{
    CTreeCtrl::OnSize(nType, cx, cy);
    
}
