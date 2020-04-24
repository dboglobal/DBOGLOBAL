// TreeObjectPane.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "shlobj.h"
#include "PropertyPane.h"
#include "PropertyListXML.h"
#include "TreeObjectPane.h"
#include "NaviMeshPane.h"

CTreeObjectPane* CTreeObjectPane::m_pInstance = NULL;
// CTreeObjectPane

IMPLEMENT_DYNCREATE(CTreeObjectPane, CXTResizeFormView)

CTreeObjectPane::CTreeObjectPane()
	: CXTResizeFormView(CTreeObjectPane::IDD)
{
    m_pInstance = this;
    m_pObject = NULL;
}

CTreeObjectPane::~CTreeObjectPane()
{
    for(UINT i = 0; i < m_vMTObject.size(); ++i)
    {
        CMTObject* pMTObject = m_vMTObject[i];
        NTL_DELETE(pMTObject);        
    }

    for(UINT i = 0; i < m_vFilePath.size(); ++i)
    {
        CHAR* szFilePath = m_vFilePath[i];
        NTL_ARRAY_DELETE(szFilePath);          
    }

    NTL_DELETE(m_pImageList)
}

void CTreeObjectPane::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE_SCRIPT, m_treeScript);
    DDX_Control(pDX, IDC_TREE_RESOURCE, m_treeRes);
}

BEGIN_MESSAGE_MAP(CTreeObjectPane, CXTResizeFormView)
    ON_NOTIFY(NM_RCLICK, IDC_TREE_RESOURCE, &CTreeObjectPane::OnNMRclickTreeResource)
    ON_COMMAND(ID_OBJECTRESOURCE_EXPORTTOSCRIPT, &CTreeObjectPane::OnObjectresourceExporttoscript)    
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_RESOURCE, &CTreeObjectPane::OnTvnSelchangedTreeResource)
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_SCRIPT, &CTreeObjectPane::OnTvnSelchangedTreeScript)
    ON_WM_SETFOCUS()
    ON_NOTIFY(NM_RCLICK, IDC_TREE_SCRIPT, &CTreeObjectPane::OnNMRclickTreeScript)
    ON_COMMAND(ID_OBJECTFOLDER_NEWFOLDER, &CTreeObjectPane::OnObjectfolderNewfolder)
    ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE_SCRIPT, &CTreeObjectPane::OnTvnEndlabeleditTreeScript)
    ON_COMMAND(ID_OBJECTFOLDER_RENAME, &CTreeObjectPane::OnObjectfolderRename)
    ON_COMMAND(ID_OBJECTFOLDER_DELETE, &CTreeObjectPane::OnObjectfolderDelete)
    ON_COMMAND(ID_OBJECTFOLDER_IMPORTSCRIPT, &CTreeObjectPane::OnObjectfolderImportscript)
    ON_COMMAND(ID_OBJECTSCRIPT_RENAME, &CTreeObjectPane::OnObjectscriptRename)
    ON_COMMAND(ID_OBJECTSCRIPT_DELETE, &CTreeObjectPane::OnObjectscriptDelete)
    ON_WM_DESTROY()
    ON_COMMAND(ID_OBJECTSCRIPT_SAVE, &CTreeObjectPane::OnObjectscriptSave)
    ON_COMMAND(ID_OBJECTSCRIPT_SAVEAS, &CTreeObjectPane::OnObjectscriptSaveas)
    ON_COMMAND(ID_OBJECTSCRIPT_SAVEALL, &CTreeObjectPane::OnObjectscriptSaveall)
    ON_COMMAND(ID_OBJECTFOLDER_SAVETREE, &CTreeObjectPane::OnObjectfolderSavetree)
    ON_NOTIFY(TVN_DELETEITEM, IDC_TREE_SCRIPT, &CTreeObjectPane::OnTvnDeleteitemTreeScript)
END_MESSAGE_MAP()


// CTreeObjectPane diagnostics

#ifdef _DEBUG
void CTreeObjectPane::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTreeObjectPane::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTreeObjectPane message handlers

void CTreeObjectPane::OnInitialUpdate()
{
    USES_CONVERSION;

    CXTResizeFormView::OnInitialUpdate();

    SetResize(IDC_STATIC_SCRIPT, SZ_TOP_LEFT, SZ_MIDDLE_RIGHT);
    SetResize(IDC_TREE_SCRIPT, SZ_TOP_LEFT, SZ_MIDDLE_RIGHT);
    SetResize(IDC_STATIC_RESOURCE, SZ_MIDDLE_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_TREE_RESOURCE, SZ_MIDDLE_LEFT, SZ_BOTTOM_RIGHT);

    m_treeScript.EnableMultiSelect();
    m_treeRes.EnableMultiSelect();
   
    m_strObjectPath = (CModelToolApplication::GetInstance()->GetWorkDir());
    m_strObjectPath += "\\Object\\Mesh";    
    m_strScriptPath = (CModelToolApplication::GetInstance()->GetWorkDir());
    m_strScriptPath += MT_PATH_OBJECT;
    m_strTreeXMLPath = (CModelToolApplication::GetInstance()->GetWorkDir());
    m_strTreeXMLPath += "\\Tool\\ModelTool\\";

    // 이미지 리스트 설정
    m_pImageList = new CImageList();
    m_pImageList->Create(16, 16, ILC_COLOR32, 4, 4);
    m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_OPEN));
    m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_OBJECT));      // 리소스용 아이콘
    m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_OBJECT_SCRIPT)); // 오브젝트 스크립트용 아이콘
    m_treeScript.SetImageList(m_pImageList, TVSIL_NORMAL);
    m_treeRes.SetImageList(m_pImageList, TVSIL_NORMAL);
    
    //  Root 설정
    TV_INSERTSTRUCT tvStruct;
    tvStruct.hParent      = NULL;
    tvStruct.hInsertAfter = TVI_LAST;
    tvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE;
    tvStruct.item.pszText = "Object Resource";
    tvStruct.item.iImage  = 0;    
    m_itemResRoot = m_treeRes.InsertItem(&tvStruct);
    CHAR* szResPath = new CHAR[256];
    sprintf(szResPath, "%s", m_strObjectPath);
    m_treeRes.SetItemData(m_itemResRoot, (DWORD_PTR)szResPath);
    m_vFilePath.push_back(szResPath);

    tvStruct.hParent      = NULL;
    tvStruct.hInsertAfter = TVI_LAST;
    tvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE;
    tvStruct.item.pszText = "Object Script";
    tvStruct.item.iImage  = 0;    
    m_itemScriptRoot = m_treeScript.InsertItem(&tvStruct);

    // 1. Read the contents of the folder and set it in the resource tree.  
    UpdateResFolder(&m_treeRes, m_strObjectPath, m_itemResRoot, TRUE);
    m_treeRes.Expand(m_itemResRoot, TVE_EXPAND);        

    // 2. If there is any existing work (TreeObject.XML), read it and set it in the script tree.
    CString strTreeFile = m_strTreeXMLPath + SAVE_FILE_NAME;
    m_treeScript.LoadTree(strTreeFile);	
}

void CTreeObjectPane::OnSetFocus(CWnd* pOldWnd)
{
    CXTResizeFormView::OnSetFocus(pOldWnd);
    if(m_pObject)
        m_pObject->SetVisible(TRUE);

    CModelToolApplication::GetInstance()->SetAppMode(MT_MODE_OBJECT);

    //HTREEITEM hItem = m_treeScript.GetSelectedItem();
    //if(hItem)
    //{
    //    m_treeScript.SelectItem(m_treeScript.GetRootItem());
    //    m_treeScript.SelectItem(hItem);
    //}    
}

void CTreeObjectPane::SetObject(CMTObject* pObject)
{
    if(pObject == NULL)
    {
        if(m_pObject)
            m_pObject->SetVisible(FALSE);
    }
    else
    {
        pObject->SetVisible(TRUE);        
    }

    m_pObject = pObject;
    CModelToolApplication::GetInstance()->SetEditObject(pObject);
}

void CTreeObjectPane::UpdateResFolder(CTreeCtrl* pTreeCtrl, LPCTSTR pstr, HTREEITEM hItemParent, BOOL bFile)
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
            //pTreeCtrl->Expand(hItem, TVE_EXPAND);
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
                HTREEITEM hItem = pTreeCtrl->InsertItem(&tvStruct);
            }
        }
    }   

    finder.Close();
}

void CTreeObjectPane::OnNMRclickTreeScript(NMHDR *pNMHDR, LRESULT *pResult)
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
            subMenu = menuPopup.GetSubMenu(4);            
            subMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);        
        }               
        else    // 스크립트
        {
            subMenu = menuPopup.GetSubMenu(5);            
            subMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);        
        }
    }

    *pResult = 0;
}

void CTreeObjectPane::OnNMRclickTreeResource(NMHDR *pNMHDR, LRESULT *pResult)
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
            subMenu = menuPopup.GetSubMenu(3);            
            subMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);        
        }               
    }  
    
    *pResult = 0;
}

void CTreeObjectPane::OnTvnSelchangedTreeScript(NMHDR *pNMHDR, LRESULT *pResult)
{
    USES_CONVERSION;

    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
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
        CMTObject* pObject = (CMTObject*)m_treeScript.GetItemData(hItem);
        if(m_pObject && m_pObject != pObject)
        {
            m_pObject->SetVisible(FALSE);
            m_pObject = pObject;                    
        }

        pObject->SetVisible(TRUE);      
        CModelToolApplication::GetInstance()->SetEditObject(pObject);        
    }
    else
    {
        CString strFilePath = m_strScriptPath;
        strFilePath += m_treeScript.GetItemText(hItem);
        strFilePath += ".XML";
        
        ::SetCurrentDirectoryA(CModelToolApplication::GetInstance()->GetWorkDir());
 
        CMTObject* pObject = new CMTObject();
        if(pObject->GetProperty()->LoadScript((strFilePath)))
        {
            //UVAnim 데이터가 있으면 Clump를 Load하기 전에 적용한다.
            if(pObject->GetProperty()->m_strUVAnimFileName.size() > 0)
            {
                pObject->SetUVAnim(pObject->GetProperty()->m_strUVAnimFileName.c_str());
            }

           CString strFileName = (pObject->GetProperty()->GetDffName());
           CString strFilePath = FindFileFromPath(strFileName, m_strObjectPath);
           if(strFilePath != "")
           {
               if(pObject->LoadClump((LPSTR)(LPCSTR)(strFilePath)))                                             
               {
                   if(m_pObject)                   
                       m_pObject->SetVisible(FALSE);

                   ::SetCurrentDirectoryA(CModelToolApplication::GetInstance()->GetWorkDir());

                   // Anim이 있으면 적용한다.
                   if(pObject->GetProperty()->m_strAnimFileName.size())
                   {
                       pObject->SetAnimation(pObject->GetProperty()->m_strAnimFileName.c_str());
                   }

                   m_pObject = pObject;
                   m_treeScript.SetItemData(hItem, (DWORD_PTR)pObject);
                   CModelToolApplication::GetInstance()->SetEditObject(pObject);                   
                   m_vMTObject.push_back(pObject);
                   return;
               }
           }           
        }
        
        // Fail
        pObject->Destroy();
        delete pObject;

        MessageBox("Object Script Load Fail", "Error", MB_ICONERROR);       

    }

    *pResult = 0;
}


void CTreeObjectPane::OnTvnSelchangedTreeResource(NMHDR *pNMHDR, LRESULT *pResult)
{
    USES_CONVERSION;

    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
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
        CMTObject* pObject = (CMTObject*)m_treeRes.GetItemData(hItem);
        if(pObject && m_pObject != pObject)
        {
            m_pObject = pObject;         
            CModelToolApplication::GetInstance()->SetEditObject(pObject);
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

           CMTObject* pObject = new CMTObject();
           if(pObject->LoadClump((LPSTR)(LPCSTR)(strObjectPath)))
           {
               m_treeRes.SetItemData(hItem, (DWORD_PTR)pObject);
               m_pObject = pObject;
               CModelToolApplication::GetInstance()->SetEditObject(pObject);
               m_vMTObject.push_back(pObject);
           }
           else
           {
               pObject->Destroy();
               delete pObject;
               pObject = NULL;
           }
           
       }
    }    
    *pResult = 0;
}

void CTreeObjectPane::OnObjectresourceExporttoscript()
{
    USES_CONVERSION;

    HTREEITEM hItem = m_treeRes.GetFirstSelectedItem();
    while(hItem != NULL)
    {
        CMTObject* pObject = NULL;

        if(!m_treeRes.GetItemData(hItem))
        {
            // 데이터가 없으면 생성하여 준다.
            pObject = new CMTObject();            
            m_vMTObject.push_back(pObject);
        }
        else
        {
            pObject = (CMTObject*)m_treeRes.GetItemData(hItem);    
            if(!pObject->GetProperty())
                continue;
        }

        //리소스의 이름을 확장자만 바꿔(DFF->XML) 스크립트 이름으로 저장한다.
        CString strResName = m_treeRes.GetItemText(hItem);    
        strResName.MakeUpper();

        CString strKeyName = strResName;
        strKeyName.Replace(".DFF", "");               ///< 확장자를 뺀이름이 keyName이 된다.

        CString strScriptPath = m_strScriptPath + strResName;
        strScriptPath.Replace(".DFF", ".XML");

        // 기존의 파일이 존재하는지 확인한다.
        HTREEITEM hExistItem = m_treeScript.FindItem(strKeyName,TRUE, TRUE, TRUE);
        if(hExistItem)
        {
            m_treeScript.SelectItem(hExistItem);
            MessageBox("Same KeyName File is Exist", NULL, MB_ICONERROR);            
            return;
        }

        // property를 설정한다.
        pObject->GetProperty()->SetName((strKeyName));
        pObject->GetProperty()->SetDffName((strResName));
        pObject->GetProperty()->m_strMeshPath = (OBJECT_MESH_PATH);
        pObject->GetProperty()->m_strTexPath = (OBJECT_TEXTURE_PATH);

        // 스크립트를 저장하고, 스크립트 트리의 루트폴더에 추가한다.
        if(pObject->GetProperty()->SaveScript((LPSTR)(LPCSTR)(strScriptPath)))
        {
            TV_INSERTSTRUCT tvStruct;
            //tvStruct.hParent      = m_itemScriptRoot;
            tvStruct.hParent      = NULL;
            tvStruct.hInsertAfter = TVI_SORT;
            tvStruct.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
            tvStruct.item.pszText = (LPSTR)(LPCSTR)strKeyName;
            tvStruct.item.iImage  = 2;    
            tvStruct.item.iSelectedImage = 2;
            HTREEITEM hInsertItem = m_treeScript.InsertItem(&tvStruct);
            m_treeScript.SelectItem(hInsertItem);
        }
        else
        {
            MessageBox("Export Script Process Fail", "Error", MB_ICONERROR);
        }

        hItem = m_treeRes.GetNextSelectedItem(hItem);
    }    
}


void CTreeObjectPane::OnObjectfolderNewfolder()
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

void CTreeObjectPane::OnTvnEndlabeleditTreeScript(NMHDR *pNMHDR, LRESULT *pResult)
{
    USES_CONVERSION;

    LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
    
    CString strText = pTVDispInfo->item.pszText;
    if(strText != "")
    {
        m_treeScript.SetItemText(pTVDispInfo->item.hItem, strText);

        HTREEITEM hItem = m_treeScript.GetSelectedItem();
        int nImage, nSelectedImage;
        m_treeScript.GetItemImage(hItem, nImage, nSelectedImage);

        if(nImage == 2) // 스크립트라면 파일명도 변경하여 준다.
        {
            CMTObject* pObject = (CMTObject*)m_treeScript.GetItemData(hItem);
            if(pObject)
            {
                CString strOrgFileName = m_strScriptPath + m_strPrevFileName;
                CString strNewFileName = m_strScriptPath + strText + ".XML"; 
                int nModify = strOrgFileName.Remove('*');
                strNewFileName.Remove('*');

                CFile::Rename(strOrgFileName, strNewFileName);

                pObject->GetProperty()->SetName((strText));                                
                CModelToolApplication::GetInstance()->SetEditObject(pObject);

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

void CTreeObjectPane::OnObjectfolderRename()
{
    HTREEITEM hItem = m_treeScript.GetSelectedItem();
    if(hItem)
    {
        m_treeScript.EditLabel(hItem);
    }
}

void CTreeObjectPane::OnObjectfolderDelete()
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
                int nImage, nSelectedImage;
                m_treeScript.GetItemImage(vItems[i], nImage, nSelectedImage);
                if(nImage == 0)    // 폴더
                    continue;

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

void CTreeObjectPane::OnObjectfolderImportscript()
{
    CHAR szFilter[] = "Object Script File (*.XML)|*.XML||";
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


void CTreeObjectPane::OnObjectscriptRename()
{
    HTREEITEM hItem = m_treeScript.GetSelectedItem();
    if(hItem)
    {
        m_strPrevFileName = m_treeScript.GetItemText(hItem);        
        m_strPrevFileName += ".XML";

        m_treeScript.EditLabel(hItem);
    }    
}

void CTreeObjectPane::OnObjectscriptDelete()
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
            m_treeScript.DeleteItem(vItems[i]);
            CFile::Remove(strXmlFileName); 
            
        }
    }
}

void CTreeObjectPane::OnDestroy()
{
    USES_CONVERSION;

    CString strSaveFileName = m_strTreeXMLPath + SAVE_FILE_NAME;
    m_treeScript.SaveTree(strSaveFileName);

    // 프로퍼티 리스트를 저장한다.
    OnSavePropertyList();


    CXTResizeFormView::OnDestroy();
}

void CTreeObjectPane::OnObjectscriptSave()
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

void CTreeObjectPane::OnObjectscriptSaveas()
{
    USES_CONVERSION;

    HTREEITEM hItem = m_treeScript.GetSelectedItem();
    if(!hItem)
        return;

    CMTObject* pObject = (CMTObject*)m_treeScript.GetItemData(hItem);
    if(!pObject)
        return;

    CString strItemName = m_treeScript.GetItemText(hItem);
    strItemName.Replace("*", "");

    CHAR szFilter[] = "Object Script File (*.XML)|*.XML||";
    CString strPath = m_strScriptPath + strItemName + ".XML";
    CFileDialog dlg(FALSE, "XML", strPath, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);
    
    if(dlg.DoModal() == IDOK)
    {
        //  파일을 저장한다.
        CString strSaveFileName = dlg.GetFileName();
        CString strSaveFilePath = dlg.GetPathName();
        
        strSaveFileName.MakeUpper();
        strSaveFileName.Replace(".XML", "");
        
        CString strTempKeyName = (pObject->GetProperty()->GetName());
        pObject->GetProperty()->SetName((strSaveFileName));
        if(pObject->GetProperty()->SaveScript((LPSTR)(LPCSTR)(strSaveFilePath)))
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

        pObject->GetProperty()->SetName((strTempKeyName));
    }
}

void CTreeObjectPane::OnObjectscriptSaveall()
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

RwBool CTreeObjectPane::OnSaveScript(HTREEITEM hItem)
{
    USES_CONVERSION;

    if(!hItem)
        return FALSE;

    CMTObject* pObject = (CMTObject*)m_treeScript.GetItemData(hItem);
    if(pObject)
    {
        CString scriptName = m_treeScript.GetItemText(hItem);
        scriptName.Replace("*", "");
        CString strFileName = m_strScriptPath + scriptName;
        strFileName += ".XML";

        pObject->GetProperty()->SetName((scriptName));
        
        if(pObject->GetProperty()->SaveScript((LPSTR)(LPCSTR)(strFileName)))
        {
            // * 표시가 있으면 없애준다.
            m_treeScript.SetItemText(hItem, scriptName);           
            return TRUE;
        }
    }

    return FALSE;
}

void CTreeObjectPane::OnObjectfolderSavetree()
{
    // 현재 트리구조를 저장한다.
    CString strSaveFileName = m_strTreeXMLPath + SAVE_FILE_NAME;
    m_treeScript.SaveTree(strSaveFileName);

    // 프로퍼티 리스트도 저장한다.
    OnSavePropertyList();
    
    MessageBox("Tree Save Complete");
}

void CTreeObjectPane::OnSavePropertyList() 
{
    CString strPropertyListFileName = m_strScriptPath + OBJECT_PROPERTY_LIST_FILE_NAME;

    TREEITEM_VECTOR vItem;
    m_treeScript.GetAllChildNode(m_treeScript.GetRootItem(), vItem);
    CPropertyListXML xmlPropertyList;
    xmlPropertyList.Save(strPropertyListFileName, CPropertyListXML::PROPERTY_OBJECT, &m_treeScript, vItem);
}

void CTreeObjectPane::OnDataChanged() 
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
void CTreeObjectPane::OnTvnDeleteitemTreeScript(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR); 
    *pResult = 0;
}

void CTreeObjectPane::OnShowSaveForChanges() 
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
        strTitle.Format("< Object > Save Changes to following Items?\n\n");
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