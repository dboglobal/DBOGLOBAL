// TreeItemPane.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "ModelToolApplication.h"
#include "TreeItemPane.h"
#include "PropertyPane.h"
#include "PropertyListXML.h"
#include "MTItemPool.h"


CTreeItemPane* CTreeItemPane::m_pInstance = NULL;

// CTreeItemPane

IMPLEMENT_DYNCREATE(CTreeItemPane, CXTResizeFormView)

CTreeItemPane::CTreeItemPane()
	: CXTResizeFormView(CTreeItemPane::IDD)
{
    m_pInstance = this;
    m_pImageList = NULL;
    m_pItem = NULL;
}

CTreeItemPane::~CTreeItemPane()
{
    for(UINT i = 0; i < m_vFilePath.size(); ++i)
    {
        CHAR* szFilePath = m_vFilePath[i];
        NTL_ARRAY_DELETE(szFilePath);        
    }

    NTL_DELETE(m_pImageList);
}

void CTreeItemPane::DoDataExchange(CDataExchange* pDX)
{
    CXTResizeFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE_RESOURCE, m_treeRes);
    DDX_Control(pDX, IDC_TREE_SCRIPT, m_treeScript);
}

BEGIN_MESSAGE_MAP(CTreeItemPane, CXTResizeFormView)
    ON_WM_SETFOCUS()
    ON_WM_DESTROY()
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_RESOURCE, &CTreeItemPane::OnTvnSelchangedTreeResource)
    ON_NOTIFY(NM_RCLICK, IDC_TREE_RESOURCE, &CTreeItemPane::OnNMRclickTreeResource)
    ON_COMMAND(ID_ITEMRESOURCE_EXPORTTOSCRIPT, &CTreeItemPane::OnItemresourceExporttoscript)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_SCRIPT, &CTreeItemPane::OnTvnSelchangedTreeScript)
    ON_NOTIFY(NM_RCLICK, IDC_TREE_SCRIPT, &CTreeItemPane::OnNMRclickTreeScript)
    ON_COMMAND(ID_ITEMFOLDER_NEWFOLDER, &CTreeItemPane::OnItemfolderNewfolder)
    ON_COMMAND(ID_ITEMFOLDER_DELETE, &CTreeItemPane::OnItemfolderDelete)
    ON_COMMAND(ID_ITEMFOLDER_IMPORTSCRIPT, &CTreeItemPane::OnItemfolderImportscript)
    ON_COMMAND(ID_ITEMFOLDER_RENAME, &CTreeItemPane::OnItemfolderRename)
    ON_COMMAND(ID_ITEMFOLDER_SAVETREE, &CTreeItemPane::OnItemfolderSavetree)
    ON_COMMAND(ID_ITEMSCRIPT_DELETE, &CTreeItemPane::OnItemscriptDelete)
    ON_COMMAND(ID_ITEMSCRIPT_RENAME, &CTreeItemPane::OnItemscriptRename)
    ON_COMMAND(ID_ITEMSCRIPT_SAVE, &CTreeItemPane::OnItemscriptSave)
    ON_COMMAND(ID_ITEMSCRIPT_SAVEAS, &CTreeItemPane::OnItemscriptSaveas)
    ON_COMMAND(ID_ITEMSCRIPT_SAVEALL, &CTreeItemPane::OnItemscriptSaveall)
    ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE_SCRIPT, &CTreeItemPane::OnTvnEndlabeleditTreeScript)
    ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE_SCRIPT, &CTreeItemPane::OnTvnBegindragTreeScript)
    ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREE_SCRIPT, &CTreeItemPane::OnTvnBeginlabeleditTreeScript)
    ON_NOTIFY(TVN_DELETEITEM, IDC_TREE_SCRIPT, &CTreeItemPane::OnTvnDeleteitemTreeScript)
    ON_COMMAND(ID_ITEMSCRIPT_OVERWRITEALL, &CTreeItemPane::OnItemscriptOverwriteall)
END_MESSAGE_MAP()


// CTreeItemPane diagnostics

#ifdef _DEBUG
void CTreeItemPane::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTreeItemPane::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTreeItemPane message handlers

void CTreeItemPane::OnInitialUpdate()
{
    USES_CONVERSION;

    CXTResizeFormView::OnInitialUpdate();

    SetResize(IDC_STATIC_SCRIPT, SZ_TOP_LEFT, SZ_MIDDLE_RIGHT);
    SetResize(IDC_TREE_SCRIPT, SZ_TOP_LEFT, SZ_MIDDLE_RIGHT);
    SetResize(IDC_STATIC_RESOURCE, SZ_MIDDLE_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_TREE_RESOURCE, SZ_MIDDLE_LEFT, SZ_BOTTOM_RIGHT);
    
    m_treeScript.EnableMultiSelect();
    m_treeRes.EnableMultiSelect();

    m_strResPath = (CModelToolApplication::GetInstance()->GetWorkDir());
    m_strResPath += "\\Item";
    m_strScriptPath = (CModelToolApplication::GetInstance()->GetWorkDir());
    m_strScriptPath += MT_PATH_ITEM;
    m_strTreeXMLPath = (CModelToolApplication::GetInstance()->GetWorkDir());
    m_strTreeXMLPath += "\\Tool\\ModelTool\\";

    // 이미지 리스트 설정
    m_pImageList = new CImageList();
    m_pImageList->Create(16, 16, ILC_COLOR32, 4, 4);
    m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_OPEN));
    m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_ITEM));      // 리소스용 아이콘
    m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_OBJECT_SCRIPT)); // 스크립트용 아이콘
    m_treeScript.SetImageList(m_pImageList, TVSIL_NORMAL);
    m_treeRes.SetImageList(m_pImageList, TVSIL_NORMAL);

    //  Root 설정
    TV_INSERTSTRUCT tvStruct;
    tvStruct.hParent      = NULL;
    tvStruct.hInsertAfter = TVI_LAST;
    tvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE;
    tvStruct.item.pszText = "Item Resource";
    tvStruct.item.iImage  = 0;    
    m_itemResRoot = m_treeRes.InsertItem(&tvStruct);
    char* szResPath = new char[256];
	sprintf(szResPath, "%s", m_strResPath);
    m_treeRes.SetItemData(m_itemResRoot, (DWORD_PTR)szResPath);
    m_vFilePath.push_back(szResPath);

    tvStruct.hParent      = NULL;
    tvStruct.hInsertAfter = TVI_LAST;
    tvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE;
    tvStruct.item.pszText = "Item Script";
    tvStruct.item.iImage  = 0;    
    m_itemScriptRoot = m_treeScript.InsertItem(&tvStruct);

    // 1. 폴더의 내용을 읽어와서 리소스 트리에 설정한다.    
    UpdateResFolder(&m_treeRes, m_strResPath, m_itemResRoot, TRUE);
    m_treeRes.Expand(m_itemResRoot, TVE_EXPAND);    
    m_treeScript.Expand(m_itemScriptRoot, TVE_EXPAND);

    // 2. 기존에 작업한 내용(TreeObject.XML)이 있으면 읽어와서 스크립트 트리에 설정한다.
    CString strTreeFile = m_strTreeXMLPath + TREE_ITEM_FILE_NAME;
    m_treeScript.LoadTree(strTreeFile);
}

void CTreeItemPane::SetItem( CMTItem* pItem ) 
{
    if(pItem == NULL)
    {
        if(m_pItem)
            m_pItem->SetVisible(FALSE);
    }
    else
    {
        pItem->SetVisible(TRUE);
    }

    m_pItem = pItem;    
    CModelToolApplication::GetInstance()->SetEditItem(pItem);
}

void CTreeItemPane::OnSetFocus(CWnd* pOldWnd)
{
    CXTResizeFormView::OnSetFocus(pOldWnd);
    if(m_pItem)
        m_pItem->SetVisible(TRUE);

    GetSafeInstance(CModelToolApplication)->SetAppMode(MT_MODE_ITEM);

    //HTREEITEM hItem = m_treeScript.GetSelectedItem();
    //if(hItem)
    //{
    //    m_treeScript.SelectItem(m_treeScript.GetRootItem());
    //    m_treeScript.SelectItem(hItem);
    //}
}

void CTreeItemPane::UpdateResFolder( CTreeCtrl* pTreeCtrl, LPCTSTR pstr, HTREEITEM hItemParent, BOOL bFile ) 
{
    USES_CONVERSION;

    // 재귀로 돌면서 리소스 폴더의 내용을 읽어와서 리소스 트리에 설정한다. 
    CFileFind finder;
    CString strWildcard(pstr);   
    strWildcard += _T("\\*.*");   // start working for files       

    BOOL bWorking = finder.FindFile(strWildcard);   

    while (bWorking)   
    {    
        bWorking = finder.FindNextFile();      

        // skip . and .. files; otherwise, we'd      
        // recur infinitely!      
        if (finder.IsDots())         
            continue;      

        // if it's a directory, recursively search it      
        if (finder.IsDirectory())      
        {        
            CString str = finder.GetFilePath(); 
            CString strPathName = finder.GetFileName();

            TV_INSERTSTRUCT tvStruct;
            tvStruct.hParent      = hItemParent;
            tvStruct.hInsertAfter = TVI_SORT;
            tvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE;
            tvStruct.item.pszText = (LPSTR)(LPCSTR)strPathName;
            tvStruct.item.iImage  = 0;                
            HTREEITEM hItem = pTreeCtrl->InsertItem(&tvStruct);

            // 아이템 데이터로 경로를 설정한다.            
            CString strFilePath = finder.GetFilePath();              
            CHAR* szFilePath = new CHAR[256];
			sprintf(szFilePath, "%s", strFilePath);
            pTreeCtrl->SetItemData(hItem, (DWORD_PTR)szFilePath);            
            m_vFilePath.push_back(szFilePath);

            UpdateResFolder(pTreeCtrl, str, hItem, bFile);                
        }  
        else if(bFile) // file
        {
            CString strFileName = finder.GetFileName();
            strFileName.MakeUpper();
            if(strFileName.Find("DFF") != -1)
            {
                TV_INSERTSTRUCT tvStruct;
                tvStruct.hParent      = hItemParent;
                tvStruct.hInsertAfter = TVI_SORT;
                tvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
                tvStruct.item.pszText = (LPSTR)(LPCSTR)strFileName;
                tvStruct.item.iImage  = 1;    
                tvStruct.item.iSelectedImage = 1;
                pTreeCtrl->InsertItem(&tvStruct);
            }
        }
    }   

    finder.Close();
}



void CTreeItemPane::OnTvnSelchangedTreeResource(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    USES_CONVERSION;

    HTREEITEM hItem = m_treeRes.GetSelectedItem();
    if(!hItem)
        return;


    // 선택된 아이템은 Bold로 나타내기 위해서 설정    
    m_treeRes.SetItemBold(pNMTreeView->itemOld.hItem, FALSE);
    m_treeRes.SetItemBold(hItem, TRUE);


    int nImage, nSelectedImage;
    m_treeRes.GetItemImage(hItem, nImage, nSelectedImage);

    if(nImage == 0) // 폴더
        return;

    // 기존에 Object 데이터가 있으면 화면에 보여주고, 없으면 로드한다.
    if(m_treeRes.GetItemData(hItem))
    {
        CMTItem* pItem = (CMTItem*)m_treeRes.GetItemData(hItem);
        if(pItem && m_pItem != pItem)
        {
            m_pItem = pItem;
            CModelToolApplication::GetInstance()->SetEditItem(m_pItem);
            pItem->SetVisible(TRUE);
        }
    }   
    else
    {
        HTREEITEM hItemFolder = m_treeRes.GetParentItem(hItem);
        if(hItemFolder && m_treeRes.GetItemData(hItemFolder))
        {
            CString strObjectPath = (CHAR*)m_treeRes.GetItemData(hItemFolder);           
            strObjectPath += "\\";            
            strObjectPath += m_treeRes.GetItemText(hItem);

            CString strKeyName = m_treeRes.GetItemText(hItem);
            strKeyName.Replace(".DFF", "");

            CMTItem* pItem = NULL;

            // ItemPool에 이미 생성된 Item이 있는지 체크한다.            
            pItem = CMTItemPool::GetInstance()->GetItem((LPSTR)(LPCSTR)(strKeyName));
            if(!pItem)
            {
                pItem = new CMTItem();
                if(!pItem->Create((strObjectPath)))
                {
                    NTL_DELETE(pItem);
                    MessageBox("Item Clump Load Fail", "Error", MB_ICONERROR);
                    *pResult = 0;
                    delete pItem;
                    return;
                } 

                pItem->GetProperty()->SetName((strKeyName));                
                GetSafeInstance(CMTItemPool)->AddItem(pItem);
            }

            m_treeRes.SetItemData(hItem, (DWORD_PTR)pItem);            
            m_pItem = pItem;
            CModelToolApplication::GetInstance()->SetEditItem(m_pItem);            
            pItem->SetVisible(TRUE);
        }
    }    
    
    *pResult = 0;
}


void CTreeItemPane::OnNMRclickTreeResource(NMHDR *pNMHDR, LRESULT *pResult)
{
    CPoint point;
    ::GetCursorPos(&point);

    CPoint tempPoint = point;
    m_treeRes.ScreenToClient(&tempPoint);
    HTREEITEM hItem     = m_treeRes.HitTest(tempPoint);
    if(hItem)
    {
        m_treeRes.SelectItem(hItem);

        int nImage = 0;
        int nSelectedImage = 0;
        m_treeRes.GetItemImage(hItem, nImage, nSelectedImage);

        CMenu  menuPopup;         
        CMenu* subMenu = NULL;
        menuPopup.LoadMenu(IDR_MENU_POPUP);

        if(nImage == 1)    // 리소스
        {
            subMenu = menuPopup.GetSubMenu(6);            
            subMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);        
        }               
    }  

    *pResult = 0;
}

void CTreeItemPane::OnItemresourceExporttoscript()
{
    USES_CONVERSION;

    HTREEITEM hItem = m_treeRes.GetFirstSelectedItem();
    while(hItem != NULL)
    {
        //리소스의 이름을 확장자만 바꿔(DFF->XML) 스크립트 이름으로 저장한다.
        CString strResName = m_treeRes.GetItemText(hItem);    
        strResName.MakeUpper();

        CString strKeyName = strResName;
        strKeyName.Replace(".DFF", "");               ///< 확장자를 뺀이름이 keyName이 된다.

        CString strScriptPath = m_strScriptPath + strResName;        
        strScriptPath.Replace(".DFF", ".XML");

        CMTItem* pItem = NULL;

        if(!m_treeRes.GetItemData(hItem))
        {
            // 데이터가 없으면 생성하여 준다.                                    
            pItem = CMTItemPool::GetInstance()->GetItem((LPSTR)(LPCSTR)(strKeyName));
            if(!pItem)
            {
                pItem = new CMTItem();            
                pItem->GetProperty()->SetName((strKeyName));                
                GetSafeInstance(CMTItemPool)->AddItem(pItem);
            }
        }
        else
        {
            pItem = (CMTItem*)m_treeRes.GetItemData(hItem);    
            if(!pItem->GetProperty())
                continue;
        }

        // 기존의 파일이 존재하는지 확인한다.
        HTREEITEM hExistItem = m_treeScript.FindItem(strKeyName,TRUE, TRUE, TRUE);
        if(hExistItem)
        {
            m_treeScript.SelectItem(hExistItem);
            MessageBox("Same KeyName File is Exist", NULL, MB_ICONERROR);            
            return;
        }

        // Mesh Path를 찾는다. (폴더 아이템의 Data에 포함되어 있다)
        CString strMeshPath;
        HTREEITEM hItemParent = m_treeRes.GetParentItem(hItem);
        if(hItemParent)
        {
            strMeshPath = (WCHAR*)m_treeRes.GetItemData(hItemParent);
            strMeshPath.Replace((CModelToolApplication::GetInstance()->GetWorkDir()), ".");
            strMeshPath += "\\";
        }

        // property를 설정한다.
        pItem->GetProperty()->SetName((strKeyName));
        pItem->GetProperty()->SetMeshPath((LPCSTR)(strMeshPath));        
        pItem->GetProperty()->SetMeshFileName((LPCSTR)(strResName));
        

        // 스크립트를 저장하고, 스크립트 트리의 루트폴더에 추가한다.
        if(pItem->GetProperty()->SaveScript((LPCSTR)(strScriptPath)))
        {
            TV_INSERTSTRUCT tvStruct;
            tvStruct.hParent      = NULL;
            tvStruct.hInsertAfter = TVI_SORT;
            tvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
            tvStruct.item.pszText = (LPSTR)(LPCSTR)strKeyName;
            tvStruct.item.iImage  = 2;    
            tvStruct.item.iSelectedImage = 2;
            HTREEITEM hInsertItem = m_treeScript.InsertItem(&tvStruct);
            m_treeScript.SetItemData(hInsertItem, (DWORD_PTR)pItem);
            m_treeScript.SelectItem(hInsertItem);            
        }
        else
        {
            MessageBox("Export Script Process Fail", "Error", MB_ICONERROR);
        }

        hItem = m_treeRes.GetNextSelectedItem(hItem);
    }     
}

void CTreeItemPane::OnTvnSelchangedTreeScript(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    USES_CONVERSION;
    
    HTREEITEM hItem = m_treeScript.GetSelectedItem();
    if(!hItem)
        return;

    // 선택된 아이템은 Bold로 나타내기 위해서 설정    
    m_treeScript.SetItemBold(pNMTreeView->itemOld.hItem, FALSE);
    m_treeScript.SetItemBold(hItem, TRUE);

    int nImage, nSelectedImage;
    m_treeScript.GetItemImage(hItem, nImage, nSelectedImage);

    if(nImage == 0)     // 폴더
        return;

    // 기존에 Object 데이터가 있으면 화면에 보여주고, 없으면 로드한다.
    if(m_treeScript.GetItemData(hItem))
    {
        CMTItem* pItem = (CMTItem*)m_treeScript.GetItemData(hItem);
        if(m_pItem == pItem)
            return;            

        m_pItem = pItem;    
        CModelToolApplication::GetInstance()->SetEditItem(pItem);        

        // Matrix를 초기화 하고, 프로퍼티에 설정된 Offset값으로 변경하여 준다.
        RwMatrix matIdentity;
        RwMatrixSetIdentity(&matIdentity);
        pItem->SetMatrix(matIdentity);
        RwV3d vItemPos = pItem->GetSubWeaponOffset();
        pItem->SetPosition(&vItemPos);
    }
    else
    {
        CString strFilePath = m_strScriptPath;
        strFilePath += m_treeScript.GetItemText(hItem);
        strFilePath += ".XML";
        
		// We originally created an Item Pool to write something like Item Pane
		// I have a bug in the top code change
		// Finally, create and use a separate object. -_-; (by agebreak)
		// Check if it exists in ItemPool.
        //CMTItem* pItem = CMTItemPool::GetInstance()->GetItem(W2A(m_treeScript.GetItemText(hItem)));
        //if(pItem)
        //{
        //    if(pItem->GetProperty()->LoadScript(W2A(strFilePath)))
        //    {
        //        RwMatrix matIdentity;
        //        RwMatrixSetIdentity(&matIdentity);
        //        pItem->SetMatrix(matIdentity);
        //        
        //        m_treeScript.SetItemData(hItem, (DWORD_PTR)pItem);
        //        CModelToolApplication::GetInstance()->SetEditItem(pItem);
        //        
        //        m_pItem = pItem;
        //        
        //        return;
        //    }                 
        //}
        //else
        //{
        CMTItem* pItem = new CMTItem();
        if(pItem->GetProperty()->LoadScript((LPCSTR)(strFilePath)))
        {
            CString strFileName = (pItem->GetProperty()->GetMeshFileName().c_str());
            CString strFilePath = FindFileFromPath(strFileName, m_strResPath);
            if(strFilePath != "" && pItem->Create((strFilePath)))
            {
                m_pItem = pItem;                        
                m_treeScript.SetItemData(hItem, (DWORD_PTR)pItem);
                CModelToolApplication::GetInstance()->SetEditItem(pItem);                        
                GetSafeInstance(CMTItemPool)->AddItem(pItem);
                return;
            }           
        }            
        //}

        // Fail
        pItem->Destroy();
        delete pItem;
        MessageBox("Object Script Load Fail", "Error", MB_ICONERROR);       
    }
    
    *pResult = 0;
}

void CTreeItemPane::OnDestroy()
{
    USES_CONVERSION;

    CString strSaveFileName = m_strTreeXMLPath + TREE_ITEM_FILE_NAME;
    m_treeScript.SaveTree(strSaveFileName);

    // 프로퍼티 리스트를 저장한다.
    OnSavePropertyList();


    CXTResizeFormView::OnDestroy();
}

void CTreeItemPane::OnNMRclickTreeScript(NMHDR *pNMHDR, LRESULT *pResult)
{
    CPoint point;
    ::GetCursorPos(&point);

    CPoint tempPoint = point;
    m_treeScript.ScreenToClient(&tempPoint);
    HTREEITEM hItem     = m_treeScript.HitTest(tempPoint);
    if(hItem)
    {
        m_treeScript.SelectItem(hItem);

        int nImage, nSelectedImage;
        m_treeScript.GetItemImage(hItem, nImage, nSelectedImage);

        CMenu  menuPopup;         
        CMenu* subMenu = NULL;
        menuPopup.LoadMenu(IDR_MENU_POPUP);

        if(nImage == 0)    // 폴더
        {
            subMenu = menuPopup.GetSubMenu(7);            
            subMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);        
        }               
        else    // 스크립트
        {
            subMenu = menuPopup.GetSubMenu(8);            
            subMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);        
        }
    }
    
    *pResult = 0;
}

void CTreeItemPane::OnItemfolderNewfolder()
{
    HTREEITEM hItem = m_treeScript.GetSelectedItem();       
    if(hItem)
    {
        m_treeScript.SelectAll(FALSE);

        TV_INSERTSTRUCT tvStruct;
        tvStruct.hParent      = hItem;
        tvStruct.hInsertAfter = TVI_LAST;
        tvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        tvStruct.item.pszText = "New Folder";
        tvStruct.item.iImage  = 0;    
        tvStruct.item.iSelectedImage = 0;
        HTREEITEM hItem = m_treeScript.InsertItem(&tvStruct);
        m_treeScript.SelectItem(hItem);
        m_treeScript.EditLabel(hItem);
    }
}

void CTreeItemPane::OnItemfolderDelete()
{
    HTREEITEM hItem = m_treeScript.GetSelectedItem();
    if(hItem)
    {
        CString strMsg;
        strMsg.Format("Sure Delete This Folder? [%s]", m_treeScript.GetItemText(hItem));
        if(MessageBox(strMsg, NULL, MB_YESNO) == IDYES)
        {
            TREEITEM_VECTOR vItems;
            m_treeScript.GetAllChildNode(hItem, vItems);
            for(UINT i = 0; i < vItems.size(); ++i)
            {
                CString strItemName = m_treeScript.GetItemText(vItems[i]);
                CString strXmlFileName = m_strScriptPath;
                strXmlFileName += strItemName;
                strXmlFileName += ".XML";
                CFile::Remove(strXmlFileName); 
            }
            m_treeScript.DeleteItem(hItem);
        }        
    }
}

void CTreeItemPane::OnItemfolderImportscript()
{
    CHAR szFilter[] = "Item Script File (*.XML)|*.XML||";
    CString strPath = m_strScriptPath + "*.XML";
    CFileDialog dlg(TRUE, NULL, strPath, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
    if(dlg.DoModal() == IDOK)
    {
        CString sLoadFileName = dlg.GetFileName();
        sLoadFileName.MakeUpper();
        sLoadFileName.Replace(".XML", "");

        // 트리에 항목을 추가한다. (파일명만 추가하면 된다)

        TV_INSERTSTRUCT sTvStruct;
        sTvStruct.hParent = m_treeScript.GetSelectedItem();
        sTvStruct.hInsertAfter = TVI_SORT;
        sTvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        sTvStruct.item.iImage = 2;
        sTvStruct.item.iSelectedImage = 2;
        sTvStruct.item.pszText = (LPSTR)(LPCSTR)sLoadFileName;
        HTREEITEM hItem = m_treeScript.InsertItem(&sTvStruct);
        m_treeScript.SelectItem(hItem);
    }
}

void CTreeItemPane::OnItemfolderRename()
{
    HTREEITEM hItem = m_treeScript.GetSelectedItem();
    if(hItem)
    {
        m_treeScript.EditLabel(hItem);
    }
}

void CTreeItemPane::OnItemfolderSavetree()
{
    // Save the current tree structure.
    CString strSaveFileName = m_strTreeXMLPath + TREE_ITEM_FILE_NAME;
    m_treeScript.SaveTree(strSaveFileName);

    // PropertyList is also stored.
    OnSavePropertyList();

    MessageBox("Tree Save Complete");
}

void CTreeItemPane::OnItemscriptDelete()
{
    HTREEITEM hItem = m_treeScript.GetFirstSelectedItem();

    TREEITEM_VECTOR vItems;
    CString strMsg = "Sure Delete This Script?\n\n";
    while(hItem)
    {
        vItems.push_back(hItem);

        strMsg += m_treeScript.GetItemText(hItem) + "\n";
        hItem = m_treeScript.GetNextSelectedItem(hItem);
    }    

    if(MessageBox(strMsg, NULL, MB_YESNO) == IDYES)
    {
        for(UINT i = 0; i < vItems.size(); ++i)
        {
            CString strItemName = m_treeScript.GetItemText(vItems[i]);
            CString strXmlFileName = m_strScriptPath;
            strXmlFileName += strItemName;
            strXmlFileName += ".XML";
            CFile::Remove(strXmlFileName); 
            m_treeScript.DeleteItem(vItems[i]);
        }
    }
}

void CTreeItemPane::OnItemscriptRename()
{
    HTREEITEM hItem = m_treeScript.GetSelectedItem();
    if(hItem)
    {
        m_strPrevFileName = m_treeScript.GetItemText(hItem);        
        m_strPrevFileName += ".XML";

        m_treeScript.EditLabel(hItem);
    }  
}

void CTreeItemPane::OnItemscriptSave()
{
    HTREEITEM hItem = m_treeScript.GetFirstSelectedItem();
    while(hItem)
    {
        int nImage, nSelectedImage;
        m_treeScript.GetItemImage(hItem, nImage, nSelectedImage);
        if(nImage != 0)
        {
            RwBool bRet = OnSaveScript(hItem);   
            if(!bRet)
            {
                CString strMsg;
                strMsg.Format("[%s] Script Save Fail", m_treeScript.GetItemText(hItem));
                MessageBox(strMsg, NULL, MB_ICONERROR);
            }
        }

        hItem = m_treeScript.GetNextSelectedItem(hItem);
    }
}

void CTreeItemPane::OnItemscriptSaveas()
{
    USES_CONVERSION;

    HTREEITEM hItem = m_treeScript.GetSelectedItem();
    if(!hItem)
        return;

    CMTItem* pItem = (CMTItem*)m_treeScript.GetItemData(hItem);
    if(!pItem)
        return;

    CString strItemName = m_treeScript.GetItemText(hItem);
    strItemName.Replace("*", "");

    CHAR szFilter[] = "Item Script File (*.XML)|*.XML||";
    CString strPath = m_strScriptPath + strItemName + ".XML";
    CFileDialog dlg(FALSE, "XML", strPath, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

    if(dlg.DoModal() == IDOK)
    {
        //  파일을 저장한다.
        CString strSaveFileName = dlg.GetFileName();
        CString strSaveFilePath = dlg.GetPathName();

        strSaveFileName.MakeUpper();
        strSaveFileName.Replace(".XML", "");

        CString strTempKeyName = (pItem->GetProperty()->GetName());
        pItem->GetProperty()->SetName((strSaveFileName));
        if(pItem->GetProperty()->SaveScript((LPCSTR)(strSaveFilePath)))
        {
            // 트리에 항목을 추가한다. (파일명만 추가하면 된다)
            TV_INSERTSTRUCT sTvStruct;
            sTvStruct.hParent = m_treeScript.GetParentItem(hItem);
            sTvStruct.hInsertAfter = TVI_SORT;
            sTvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
            sTvStruct.item.iImage = 2;
            sTvStruct.item.iSelectedImage = 2;
            sTvStruct.item.pszText = (LPSTR)(LPCSTR)strSaveFileName;
            HTREEITEM hItem = m_treeScript.InsertItem(&sTvStruct);
            m_treeScript.SelectItem(hItem);            
        }

        pItem->GetProperty()->SetName((strTempKeyName));
    }
}

void CTreeItemPane::OnItemscriptSaveall()
{
    TREEITEM_VECTOR vItems;
    m_treeScript.GetAllChildNode(m_treeScript.GetRootItem(), vItems);

    for(UINT i = 0; i < vItems.size(); ++i)
    {
        int nImage, nSelectedImage;
        m_treeScript.GetItemImage(vItems[i], nImage, nSelectedImage);

        if(nImage != 0) // 스크립트만 저장한다.
        {
            OnSaveScript(vItems[i]);
        }
    }  
}

void CTreeItemPane::OnItemscriptOverwriteall()
{
    USES_CONVERSION;

    TREEITEM_VECTOR vItems;
    m_treeScript.GetAllChildNode(m_treeScript.GetRootItem(), vItems);
    
    for each(HTREEITEM hItem in vItems)    
    {
        int nImage, nSelectedImage;
        m_treeScript.GetItemImage(hItem, nImage, nSelectedImage);

        if(nImage != 0) // 스크립트만 저장한다.
        {
            CMTItem* pItem = (CMTItem*)m_treeScript.GetItemData(hItem);            
            if(pItem)
            {
                OnSaveScript(hItem);
            }
            else    // 생선된 객체가 없으면 생성을 한후에 저장을한다.
            {
                CString strFilePath = m_strScriptPath;
                strFilePath += m_treeScript.GetItemText(hItem);
                strFilePath += ".XML";    

                CNtlPLItemProperty* pProperty = NTL_NEW CNtlPLItemProperty();
                if(pProperty->LoadScript((LPCSTR)(strFilePath)))
                {                    
                    // MSXML에서 300개 이상정도 되는 파일들을 new->로드->delete하면 뻑이 난다. -_-; 
                    //pItem->GetProperty()->SaveScript(W2A(strFilePath));                    
                }                                 
                NTL_DELETE(pProperty);
            }
        }            
    }    
}

RwBool CTreeItemPane::OnSaveScript( HTREEITEM hItem ) 
{
    USES_CONVERSION;

    if(!hItem)
        return FALSE;

    CMTItem* pItem = (CMTItem*)m_treeScript.GetItemData(hItem);
    if(pItem)
    {
        CString scriptName = m_treeScript.GetItemText(hItem);
        scriptName.Replace("*", "");
        CString strFileName = m_strScriptPath + scriptName;
        strFileName += ".XML";

        pItem->GetProperty()->SetName((scriptName));

        if(pItem->GetProperty()->SaveScript((LPCSTR)(strFileName)))
        {
            // * 표시가 있으면 없애준다.            
            m_treeScript.SetItemText(hItem, scriptName);
            return TRUE;
        }
    }

    return FALSE;
}

void CTreeItemPane::OnTvnEndlabeleditTreeScript(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);

    USES_CONVERSION;

    CString strText = pTVDispInfo->item.pszText;
    if(strText != "")
    {
        m_treeScript.SetItemText(pTVDispInfo->item.hItem, strText);

        HTREEITEM hItem = m_treeScript.GetSelectedItem();
        int nImage, nSelectedImage;
        m_treeScript.GetItemImage(hItem, nImage, nSelectedImage);

        if(nImage == 2) // 스크립트라면 파일명도 변경하여 준다.
        {
            CMTItem* pItem = (CMTItem*)m_treeScript.GetItemData(hItem);
            if(pItem)
            {
                CString strOrgFileName = m_strScriptPath + m_strPrevFileName;
                CString strNewFileName = m_strScriptPath + strText + ".XML";                
                int nModify = strOrgFileName.Remove('*');
                strNewFileName.Remove('*');
                
                CFile::Rename(strOrgFileName, strNewFileName);

                pItem->GetProperty()->SetName((strText));                                
                CModelToolApplication::GetInstance()->SetEditItem(pItem);

                // 원래 "*" 붙어있는것은 이름 변경후에 다시 "*"를 붙여준다.
                if(nModify > 0)
                {
                    OnDataChanged();
                }
            }
        }
    }
    
    *pResult = 0;
}

void CTreeItemPane::OnTvnBegindragTreeScript(NMHDR *pNMHDR, LRESULT *pResult)
{
    //LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);    
    *pResult = 0;
}

void CTreeItemPane::OnTvnBeginlabeleditTreeScript(NMHDR *pNMHDR, LRESULT *pResult)
{
    //LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
    *pResult = 0;
}

void CTreeItemPane::OnDataChanged() 
{
    // 데이터가 변경되면 스크립트명앞에 '*'를 붙여줘서 표시한다.
    HTREEITEM hSelectedItem = m_treeScript.GetSelectedItem();
    if(hSelectedItem)
    {
        CString strItemName = m_treeScript.GetItemText(hSelectedItem);
        if(strItemName.Find("*") < 0)
        {
            strItemName.Insert(0, "*");
            m_treeScript.SetItemText(hSelectedItem, strItemName);
        }        
    }

}
void CTreeItemPane::OnShowSaveForChanges() 
{
    TREEITEM_VECTOR vTreeItem;
    TREEITEM_VECTOR vChangeItem;
    m_treeScript.GetAllChildNode(m_treeScript.GetRootItem(), vTreeItem);

    // *표시가 있는 아이템들을 리스트에 담는다.
    for(UINT i = 0; i < vTreeItem.size(); ++i)
    {
        HTREEITEM hItem = vTreeItem[i];
        if(m_treeScript.GetItemData(hItem))
        {
            CString strScriptName = m_treeScript.GetItemText(hItem);
            if(strScriptName.Find("*") >= 0)
            {
                strScriptName.Replace("*", "");
                m_treeScript.SetItemText(hItem, strScriptName);
                vChangeItem.push_back(hItem);
            }
        }
    }


    if(vChangeItem.size())
    {
        CString strTitle, strScriptList, strMsg;
        strTitle.Format("< Item > Save Changes to following Items?\n\n");
        for(UINT i = 0; i < vChangeItem.size(); ++i)
        {
            HTREEITEM hItem = vChangeItem[i];
            strScriptList += m_treeScript.GetItemText(hItem);
            strScriptList += "\n";
        }

        strScriptList += "\n";
        strMsg = strTitle + strScriptList;
        if(MessageBox(strMsg, "Item", MB_YESNO) == IDYES)
        {
            for(UINT i = 0; i < vChangeItem.size(); ++i)
            {
                CString strScriptName = m_treeScript.GetItemText(vChangeItem[i]);
                OnSaveScript(vChangeItem[i]);
            }
        }
    } 

}
void CTreeItemPane::OnSavePropertyList() 
{
    CString strPropertyListFileName = m_strScriptPath + ITEM_PROPERTY_LIST_FILE_NAME;

    TREEITEM_VECTOR vItem;
    m_treeScript.GetAllChildNode(m_treeScript.GetRootItem(), vItem);
    CPropertyListXML xmlPropertyList;
    xmlPropertyList.Save(strPropertyListFileName, CPropertyListXML::PROPERTY_ITEM, &m_treeScript, vItem);
}

void CTreeItemPane::OnTvnDeleteitemTreeScript(NMHDR *pNMHDR, LRESULT *pResult)
{
    //LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);    
    *pResult = 0;
}


