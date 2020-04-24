// VehicleViewPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "VehicleViewPane.h"

#define VEHICLE_ATTACH_BONE_NAME "Bike_Dummy"

// CVehicleViewPane

IMPLEMENT_DYNCREATE(CVehicleViewPane, CXTResizeFormView)

CVehicleViewPane* CVehicleViewPane::m_pInstance = NULL;

CVehicleViewPane::CVehicleViewPane()
	: CXTResizeFormView(CVehicleViewPane::IDD)
{
    m_pInstance = this;

    m_pImageList = NULL;
    m_pCharacter = NULL;
    m_pVehicle   = NULL;
}

CVehicleViewPane::~CVehicleViewPane()
{
    NTL_DELETE(m_pImageList);
}

void CVehicleViewPane::DoDataExchange(CDataExchange* pDX)
{
    CXTResizeFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE_VEHICLE, m_treeVehicle);    
}

BEGIN_MESSAGE_MAP(CVehicleViewPane, CXTResizeFormView)
    ON_WM_SHOWWINDOW()
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_VEHICLE, &CVehicleViewPane::OnTvnSelchangedTreeVehicle)
END_MESSAGE_MAP()


// CVehicleViewPane 진단입니다.

#ifdef _DEBUG
void CVehicleViewPane::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CVehicleViewPane::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CVehicleViewPane 메시지 처리기입니다.

void CVehicleViewPane::OnInitialUpdate()
{
    CXTResizeFormView::OnInitialUpdate();

    SetResize(IDC_TREE_VEHICLE, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);    

    // 이미지 리스트 설정
    m_pImageList = new CImageList();
    m_pImageList->Create(16, 16, ILC_COLOR32, 4, 4);
    m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_OPEN));
    m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_ITEM));      // 리소스용 아이콘
    m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_OBJECT_SCRIPT)); // 스크립트용 아이콘
    m_treeVehicle.SetImageList(m_pImageList, TVSIL_NORMAL);    
}

void CVehicleViewPane::OnShowWindow(BOOL bShow, UINT nStatus)
{
    CXTResizeFormView::OnShowWindow(bShow, nStatus);

    if(!bShow)
        return;

    // 윈도우가 나타날때 내용을 갱신한다.
    UpdateTree();

    if(CModelToolApplication::GetInstance()->GetAppMode() == MT_MODE_VEHICLE)
    {
        m_treeVehicle.EnableWindow(TRUE);
        SetVehicle(CModelToolApplication::GetInstance()->GetEditChar());
    }
    else
    {
        m_treeVehicle.EnableWindow(FALSE);
    }
}

void CVehicleViewPane::UpdateTree() 
{
    USES_CONVERSION;

    CString strXmlFile = (CModelToolApplication::GetInstance()->GetWorkDir());
    strXmlFile += MT_PATH_TOOL;
    strXmlFile += MT_TREE_PC;
    m_treeVehicle.LoadTree(strXmlFile);
}

void CVehicleViewPane::OnTvnSelchangedTreeVehicle(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    
    USES_CONVERSION;

    HTREEITEM hItem = m_treeVehicle.GetSelectedItem();
    if(!hItem)
        return;

    int nImage, nSelectImage;
    m_treeVehicle.GetItemImage(hItem, nImage, nSelectImage);
    if(nImage == 0)
        return;

    CString strXMLFilePath = (CModelToolApplication::GetInstance()->GetWorkDir());
    strXMLFilePath += MT_PATH_CHARACTER;
    strXMLFilePath += m_treeVehicle.GetItemText(hItem);    

    if(m_treeVehicle.GetItemData(hItem))
    {
        AttachVehicle(hItem);
    }
    else
    {
        CMTCharacter* pChar = new CMTCharacter();
        pChar->Create();
        if(pChar->GetProperty()->LoadScript((LPSTR)(LPCSTR)(strXMLFilePath)))
        {
            std::string strClumpFileName = CModelToolApplication::GetInstance()->GetWorkDir();
            strClumpFileName += pChar->GetProperty()->GetBaseMeshFilePath().c_str();
            strClumpFileName += pChar->GetProperty()->GetBaseMeshFileName().c_str();
            if(pChar->LoadClump((RwChar*)strClumpFileName.c_str()))
            {
                m_treeVehicle.SetItemData(hItem, (DWORD_PTR)pChar);
                GetSceneManager()->AddPLEntity(pChar);
                AttachVehicle(hItem);
                return;
            }
        } 

        NTL_DELETE(pChar);
    }

    *pResult = 0;
}

void CVehicleViewPane::AttachVehicle( HTREEITEM hItem ) 
{
    CMTCharacter* pChar = (CMTCharacter*)m_treeVehicle.GetItemData(hItem);
    if(m_pCharacter == pChar)
        return;
    
    if(m_pCharacter)
    {
        m_pCharacter->SetVisible(FALSE);
    }

    m_pCharacter = pChar;

    if(m_pCharacter && m_pVehicle)
    {
        m_pCharacter->SetVisible(TRUE);
        m_pCharacter->SetPosition(&ZeroAxis);
        m_pVehicle->AttachBone(m_pCharacter, VEHICLE_ATTACH_BONE_NAME);   
    }    
}

void CVehicleViewPane::SetVehicle( CMTCharacter* pVehicle ) 
{
    if(m_pVehicle == pVehicle)
        return;

    m_treeVehicle.EnableWindow(pVehicle != NULL);    

    if(m_pVehicle && m_pCharacter)
    {
        m_pVehicle->Detach(m_pCharacter);
    }

    m_pVehicle = pVehicle;

    if(!IsWindowVisible())
        return;

    if(m_pVehicle && m_pCharacter)
    {
        m_pVehicle->AttachBone(m_pCharacter, VEHICLE_ATTACH_BONE_NAME);
        m_pCharacter->SetVisible(TRUE);
    }
}

void CVehicleViewPane::SetVehicleAnimation( RwUInt32 uiAnimID ) 
{
    if(!m_pVehicle || !m_pCharacter)
        return;

    RwUInt32 uiCharAnimID = 0;

    if(strstr(m_pVehicle->GetClumpName(), "SRP1"))
    {
        uiCharAnimID = uiAnimID + (VEHICLE_SRP1_ANIMATION_START - VEHICLE_ANIMATION_START);    
    }
    else if(strstr(m_pVehicle->GetClumpName(), "SRP2"))
    {
        uiCharAnimID = uiAnimID + (VEHICLE_SRP2_ANIMATION_START - VEHICLE_ANIMATION_START);    
    }

    if(uiCharAnimID != 0)
    {
        m_pCharacter->SetAnim(uiCharAnimID);
    }    
}