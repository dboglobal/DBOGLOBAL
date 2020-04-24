// ItemViewPane.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "TreeItemPane.h"
#include "ItemViewPane.h"
#include "MTItemPool.h"

// CItemViewPane

IMPLEMENT_DYNCREATE(CItemViewPane, CXTResizeFormView)

CItemViewPane* CItemViewPane::m_pInstance = NULL;

CItemViewPane::CItemViewPane()
	: CXTResizeFormView(CItemViewPane::IDD)
{
    m_pImageList = NULL;
    m_pCharacter = NULL;

    m_colorSelect = RGB(0, 0, 255);
    m_colorNon = RGB(0, 0, 0);

    m_pInstance = this;
}

CItemViewPane::~CItemViewPane()
{
    NTL_DELETE(m_pImageList);
}

void CItemViewPane::DoDataExchange(CDataExchange* pDX)
{
    CXTResizeFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE_ITEM, m_treeItem);
    DDX_Control(pDX, IDC_CHECK_ITEM, m_ckItemView);
}

BEGIN_MESSAGE_MAP(CItemViewPane, CXTResizeFormView)
    ON_WM_SHOWWINDOW()
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_ITEM, OnTvnSelchangedTreeItem)
    ON_BN_CLICKED(IDC_CHECK_ITEM, &CItemViewPane::OnBnClickedCheckItem)
END_MESSAGE_MAP()


// CItemViewPane diagnostics

#ifdef _DEBUG
void CItemViewPane::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CItemViewPane::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CItemViewPane message handlers

void CItemViewPane::OnInitialUpdate()
{
    CXTResizeFormView::OnInitialUpdate();

    SetResize(IDC_TREE_ITEM, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_CHECK_ITEM, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);

    // 이미지 리스트 설정
    m_pImageList = new CImageList();
    m_pImageList->Create(16, 16, ILC_COLOR32, 4, 4);
    m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_OPEN));
    m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_ITEM));      // 리소스용 아이콘
    m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_OBJECT_SCRIPT)); // 스크립트용 아이콘
    m_treeItem.SetImageList(m_pImageList, TVSIL_NORMAL);    

    m_ckItemView.SetCheck(TRUE);
}

void CItemViewPane::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CXTResizeFormView::OnShowWindow(bShow, nStatus);

    if(!bShow)
        return;

    // 윈도우가 나타날때 내용을 Update한다.
    UpdateTree();

    // 현재 Edit중인 캐릭터를 설정한다.
    SetCharacter(CModelToolApplication::GetInstance()->GetEditChar());
}

void CItemViewPane::UpdateTree() 
{
    USES_CONVERSION;

    CString strXmlFile = (CModelToolApplication::GetInstance()->GetWorkDir());
    strXmlFile += MT_PATH_TOOL;
    strXmlFile += MT_TREE_ITEM;
    m_treeItem.LoadTree(strXmlFile);
}

void CItemViewPane::SetCharacter( CMTCharacter* pCharacter ) 
{
    if(m_pCharacter == pCharacter)
        return;

    m_pCharacter = pCharacter;

    if(pCharacter)
    {
        m_treeItem.ResetBoldColor();
        m_treeItem.EnableWindow(TRUE);
    }
    else
    {
        m_treeItem.EnableWindow(FALSE);
    }
}

void CItemViewPane::OnTvnSelchangedTreeItem(NMHDR *pNMHDR, LRESULT *pResult)
{
    if(m_ckItemView.GetCheck() == FALSE)
        return;

    USES_CONVERSION;

    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    HTREEITEM hItem = m_treeItem.GetSelectedItem();
    if(!hItem)
        return;

    int nImage, nSelectImage;
    m_treeItem.GetItemImage(hItem, nImage, nSelectImage);
    if(nImage == 0)
        return;

    CString strXMLFilePath = (CModelToolApplication::GetInstance()->GetWorkDir());
    strXMLFilePath += MT_PATH_ITEM;
    strXMLFilePath += m_treeItem.GetItemText(hItem);
    strXMLFilePath += ".XML";
        
    if(m_treeItem.GetItemData(hItem))
    {
        // 만약 Item Pane에 수정한 값이 있다면 적용한다.        
        CMTItem* pItem  = (CMTItem*)m_treeItem.GetItemData(hItem);
        pItem->GetProperty()->LoadScript((LPCSTR)(strXMLFilePath));
        EquipItem(hItem);
    }
    else
    {
        // 원래는 Item Pane과 같이 쓰기 위해서 Item Pool을 만들어서 
        // 공용으로 썻는데.. 상위 코드의 변경으로 버그가 생겨서
        // 결국 따로 객체를 생성해서 사용한다. -_-; (by agebreak)
        //CMTItem* pItem = NULL;
        //pItem = CMTItemPool::GetInstance()->GetItem(W2A(m_treeItem.GetItemText(hItem)));
        //if(pItem)
        //{
        //    m_treeItem.SetItemData(hItem, (DWORD_PTR)pItem);            
        //    EquipItem(hItem);
        //    return;
        //}
        //else
        //{
    
            CMTItem* pItem = new CMTItem();
            if(pItem->GetProperty()->LoadScript((LPCSTR)(strXMLFilePath)))
            {
                pItem->SetProperty(pItem->GetProperty());

                ::SetCurrentDirectoryA(CModelToolApplication::GetInstance()->GetWorkDir());
                CString strClumpFilePath = (pItem->GetProperty()->GetMeshPath().c_str());
                strClumpFilePath += (pItem->GetProperty()->GetMeshFileName().c_str());
                if(pItem->Create((strClumpFilePath), TRUE))
                {
                    m_treeItem.SetItemData(hItem, (DWORD_PTR)pItem);                    
                    GetSceneManager()->AddPLEntity(pItem);
                    EquipItem(hItem);
                    return;
                }
            }
        //}
        
        MessageBox("Item Script Load Fail!", "Item View", MB_ICONERROR);
        NTL_DELETE(pItem);
    }
    
    *pResult = 0;
}

void CItemViewPane::EquipItem(HTREEITEM hItem)
{
    USES_CONVERSION;

    if(!m_pCharacter || !hItem)
        return;

    CMTItem* pItem = (CMTItem*)m_treeItem.GetItemData(hItem);
    if(!pItem)
        return;

    pItem->SetAnimation(ITEM_IDLE);    
    pItem->SetAnimUpdate(TRUE);

    m_treeItem.SetItemBold(hItem);
    m_treeItem.SetItemColor(hItem, m_colorSelect);

    CMTItem* pRemoveItem = (CMTItem*)m_pCharacter->SetChangeEquipItemForTool(pItem);

    // 테스트
    //pItem->SetUpgradeEffect(1);

    if(pRemoveItem && pRemoveItem != pItem)
    {
        pRemoveItem->SetVisible(FALSE);
        CString strRemoveItemName = (pRemoveItem->GetProperty()->GetName());
        HTREEITEM hRemoveItem = m_treeItem.FindItem(strRemoveItemName, FALSE, TRUE, TRUE);
        if(hRemoveItem)
        {
            m_treeItem.SetItemBold(hRemoveItem, FALSE);
            m_treeItem.SetItemColor(hRemoveItem, m_colorNon);
        }
    }
}
void CItemViewPane::OnBnClickedCheckItem()
{
    if(!m_pCharacter)
        return;

    BOOL bCheck = m_ckItemView.GetCheck();
    if(bCheck)
    {
        m_treeItem.EnableWindow(TRUE);

        // 기존에 붙어있던 Item들을 부착한다.
        for(UINT i = 0; i < m_vAttachItem.size(); ++i)
        {
            m_pCharacter->SetChangeEquipItem(m_vAttachItem[i]);
        }
    }
    else
    {
        m_treeItem.EnableWindow(FALSE);

        // Attach된 모든 Item을 제거한다
        m_vAttachItem.clear();
        while(m_pCharacter->GetEquipItem()->GetEquipItemList()->size())
        {
            CNtlPLItem* pItem = *(m_pCharacter->GetEquipItem()->GetEquipItemList()->begin());
            if(pItem)
            {
                m_vAttachItem.push_back(pItem);
                m_pCharacter->SetRemoveEquipItem(pItem);
            }
        }

        // 사실상 Clump를 다시 Loading 한다.        
        m_pCharacter->LoadClump(m_pCharacter->GetClumpName());
        RwUInt32 uiAnimKey = m_pCharacter->GetCurrentAnimKey();
        if(uiAnimKey)
        {
            m_pCharacter->SetAnim(m_pCharacter->GetCurrentAnimKey());
        }
        
    }
}
