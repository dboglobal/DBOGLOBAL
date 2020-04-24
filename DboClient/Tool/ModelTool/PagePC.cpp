// PagePC.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool.h"
#include "ModelToolApplication.h"
#include "NtlPLCharacterParser.h"
#include "PagePC.h"
#include "BottomToolView.h"
#include "RightToolView.h"
#include "AnimToolView.h"

#define PAGE_HEIGHT_BOTTOM_CTRL  120

// CPagePC dialog
CPagePC* CPagePC::m_pInstance = NULL;

IMPLEMENT_DYNAMIC(CPagePC, CPropertyPage)

CPagePC::CPagePC()
	: CPropertyPage(CPagePC::IDD)
{
    m_pImageList         = NULL;
    m_pCharacterProperty = NULL;
    m_vCharacter.clear();
}

CPagePC::CPagePC(int nEnum)
    : CPropertyPage(nEnum)
{
    m_pImageList         = NULL;
    m_pCharacterProperty = NULL;
}

CPagePC::~CPagePC()
{
    for(size_t i = 0; i < m_vCharacter.size(); ++i)
    {
        CMTCharacter* pCharacter = m_vCharacter[i];
        if(pCharacter)
        {
            pCharacter->Destroy();
            delete pCharacter;
            pCharacter = NULL;
        }
    }
    m_vCharacter.clear();

    if(m_pImageList)
    { 
        delete m_pImageList;
        m_pImageList = NULL;
    }
}

void CPagePC::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE_PC, m_treePC);
    DDX_Control(pDX, IDC_EDIT_SCRIPTFILE, m_edScriptFileName);
    DDX_Control(pDX, IDC_EDIT_CLUMPFILE, m_edClumpFileName);    
    DDX_Control(pDX, IDC_BT_SET_CLUMP, m_btLoadClump);
    DDX_Control(pDX, IDC_BUTTON1, m_btSaveScript);
    DDX_Control(pDX, IDC_BT_SAVE_ALL, m_btSaveAll);
}


BEGIN_MESSAGE_MAP(CPagePC, CPropertyPage)
    ON_WM_CONTEXTMENU()    
    ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_PC, &CPagePC::OnTvnSelchangedTreePc)
    ON_BN_CLICKED(IDC_BT_SET_CLUMP, &CPagePC::OnBnClickedBtSetClump)
    ON_BN_CLICKED(IDC_BUTTON1, &CPagePC::OnBnClickedButton1)
    ON_COMMAND(ID_MENU_LOAD_SCRIPT, &CPagePC::OnMenuLoadScript)    
    ON_WM_DESTROY()    
    ON_BN_CLICKED(IDC_BT_SAVE_ALL, &CPagePC::OnBnClickedBtSaveAll)    
    ON_WM_SIZE()
END_MESSAGE_MAP()


// CPagePC message handlers

BOOL CPagePC::OnInitDialog()
{
    USES_CONVERSION;

    CPropertyPage::OnInitDialog();

    m_treePC.SetRoot(L"PC");
    m_treePC.SetParent(this);
    m_btSaveScript.EnableWindow(FALSE);

    m_pInstance = this;
    
    CString sWorkFolder = A2W(CModelToolApplication::GetInstance()->GetWorkDir());
    sWorkFolder += L"\\TreePC.xml";
    m_treePC.LoadTree((LPWSTR)(LPCWSTR)sWorkFolder);

    // Save할 폴더이름을 설정한다.
    m_sSaveFolderName = A2W(CModelToolApplication::GetInstance()->GetWorkDir());
    m_sSaveFolderName += L"\\Character\\";

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CPagePC::OnContextMenu(CWnd* pWnd, CPoint point)
{
    m_treePC.OnContextMenu(pWnd, point);
}

void CPagePC::OnTvnSelchangedTreePc(NMHDR *pNMHDR, LRESULT *pResult)
{
    USES_CONVERSION;

    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    
    // 목록 변환시 기존의 내용을 저장한다.
    HTREEITEM hItem =  pNMTreeView->itemOld.hItem;
    int nImage = 0, nSelectedImage = 0;
    m_treePC.GetItemImage(hItem, nImage, nSelectedImage);

    if(nImage == 3)
    {
        //CString oldScriptName = m_treePC.GetItemText(hItem);
        //if(oldScriptName != L"")
        //{
        //    OnSaveScript(oldScriptName, FALSE);
        //}

        // 기존의 캐릭터를 화면에서 제거한다.
        CMTCharacter* pCharacter = (CMTCharacter*)m_treePC.GetItemData(hItem);
        if(pCharacter)
        {
            pCharacter->SetVisible(FALSE);
        }
    }
    
    // 스크립트 노드가 선택되었을때 파일내용을 표시한다.
    CString sScriptFileName = m_treePC.SelectScriptName();
    if(sScriptFileName != L"")
    {
        m_edScriptFileName.SetWindowText(sScriptFileName);
        m_edClumpFileName.SetWindowText(L"");

        if(!m_treePC.GetItemData(m_treePC.GetSelectedItem()) && sScriptFileName == L"New Script")
        {
            // 초기화            
            CBottomToolView::GetInstance()->SetInit(NULL);
            CRightToolView::GetInstance()->SetInit(NULL);
            CAnimToolView::GetInstance()->SetInit(NULL, NULL); 
            m_edScriptFileName.SetWindowText(L"New Script");
            m_edClumpFileName.SetWindowText(L"");            
        }
        else
        {
            OnDisplayScriptInfo(sScriptFileName);                        
        }

        m_btLoadClump.EnableWindow(TRUE);        
    }
    else
    {
       m_edScriptFileName.SetWindowText(L"");
       m_edClumpFileName.SetWindowText(L"");       
       m_btLoadClump.EnableWindow(FALSE);               
    }

    *pResult = 0;
}

BOOL CPagePC::OnDisplayScriptInfo(CString strFileName)
{
    USES_CONVERSION;

    // 기존에 트리에 설정된 캐릭터가 있는지 확인한다
    CMTCharacter* pCharacter = NULL;
    pCharacter = (CMTCharacter*)m_treePC.GetItemData(m_treePC.GetSelectedItem());
    if(pCharacter)
    {
        m_pCharacterProperty = pCharacter->GetProperty();
    }
    else
    {

        std::string strFilePath = CModelToolApplication::GetInstance()->GetWorkDir();
        strFilePath += "\\Character\\";
        strFilePath += W2A(strFileName);

        // 기존에 설정된 캐릭터 인스턴가 없으면 추가하여 준다.
        pCharacter = new CMTCharacter;
        pCharacter->Create();
        m_treePC.SetItemData(m_treePC.GetSelectedItem(), (DWORD_PTR)pCharacter);   
        m_vCharacter.push_back(pCharacter);
        m_pCharacterProperty = pCharacter->GetProperty();
        RwBool bReturn = m_pCharacterProperty->LoadScript(strFilePath);
        if(bReturn)
        {   
            std::string sClumpFilePath = CModelToolApplication::GetInstance()->GetWorkDir();
            sClumpFilePath += "\\";
            //sClumpFilePath += m_pCharacterProperty->GetMeshPath();
            sClumpFilePath += m_pCharacterProperty->GetBaseMeshFileName();
            RwBool bReturn = pCharacter->LoadClump((RwChar*)sClumpFilePath.c_str());
            if(!bReturn)
            {
                MessageBox(L"Clump File Open Fail");
                return bReturn;
            }                   
        }        
    }

    pCharacter->SetVisible(TRUE);
    pCharacter->GetProperty()->GetBoneScaleData()->bBoneScale = TRUE;

    CModelToolApplication::GetInstance()->SetEditChar(pCharacter);

    CString strMeshName = A2W(m_pCharacterProperty->GetBaseMeshFileName().c_str());
    m_edClumpFileName.SetWindowText(strMeshName);


    // 하단뷰와 오른쪽뷰에 내용을 설정한다.
    CBottomToolView::GetInstance()->SetInit(pCharacter);
    CRightToolView::GetInstance()->SetInit(pCharacter);


    // 오른쪽 UI 변경부분 추가
    CRightToolView::GetInstance()->SetEnable(CRightToolView::ENABLE_ANIMSET);
    m_btSaveScript.EnableWindow(TRUE);
    m_btSaveAll.EnableWindow(TRUE);

    return TRUE;
}


void CPagePC::OnBnClickedBtSetClump()
{
    // Clump 파일을 로드한다.
    USES_CONVERSION;

    WCHAR szOpenFilter[] = L"Clump File (*.dff)|*.dff||";
    CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szOpenFilter);    
    if(fileDlg.DoModal() == IDOK)
    {
        CString sLoadFilePath = L".";
        sLoadFilePath += fileDlg.GetPathName();	
        CString sLoadFileName = fileDlg.GetFileName();

        CMTCharacter* pCharacter = NULL;
        pCharacter = (CMTCharacter*)m_treePC.GetItemData(m_treePC.GetSelectedItem());
        if(!pCharacter)
        {
            // 없으면 추가해준다.
            pCharacter = new CMTCharacter;
            pCharacter->Create();
            m_treePC.SetItemData(m_treePC.GetSelectedItem(), (DWORD_PTR)pCharacter);
            m_vCharacter.push_back(pCharacter);
        }

        RwBool retBool = pCharacter->LoadClump(W2A(sLoadFilePath));
        if(retBool == (RwBool)TRUE)
        {
            m_edClumpFileName.SetWindowText(sLoadFileName);

            //  정보를 구조체에 설정한다.
            CMTCharacter* pCharacter = (CMTCharacter*)m_treePC.GetItemData(m_treePC.GetSelectedItem());
            if(pCharacter)
            {
                CModelToolApplication::GetInstance()->SetEditChar(pCharacter);                
                m_pCharacterProperty = pCharacter->GetProperty();                
                CString sWorkPath = A2W(CModelToolApplication::GetInstance()->GetWorkDir());
                
                sWorkPath = sWorkPath.MakeUpper();                
                sLoadFilePath = sLoadFilePath.MakeUpper();
                sLoadFileName = sLoadFileName.MakeUpper();

                sLoadFilePath.Replace(sWorkPath, L"");
                sLoadFilePath.Replace(sLoadFileName, L"");

                //m_pCharacterProperty->SetMeshPath(W2A(sLoadFilePath));
                sLoadFileName = sLoadFilePath + sLoadFileName;
                m_pCharacterProperty->SetBaseMeshFileName(W2A(sLoadFileName));

                // 하단뷰와 오른쪽뷰에 내용을 설정한다.
                CBottomToolView::GetInstance()->SetInit(pCharacter);
                CRightToolView::GetInstance()->SetInit(pCharacter);

                CRightToolView::GetInstance()->SetEnable(CRightToolView::ENABLE_ANIMSET);
                m_btSaveScript.EnableWindow(TRUE);
            }
        }
        else
        {   
            m_edClumpFileName.SetWindowText(L"");
            MessageBox(L"Clump File Load Fail");
        }
    }	
}

BOOL CPagePC::OnSetActive()
{
    // 활성화 될때 RightView에 자신의 종류를 알려줘야 한다.
    // RightView는 공통으로 사용되고, LeftView의 종류에 따라서
    // 애니메이션 종류가 달라져야 하기 때문
    CRightToolView::GetInstance()->SetActiveLeftView(CRightToolView::LEFT_PAGE_PC);

    // 선택된 아이템을 다시한번 선택해 준다. (오른쪽 툴뷰의 활성화를 위해)
    HTREEITEM hSelectedItem = m_treePC.GetSelectedItem();
    if(hSelectedItem)
    {
        m_treePC.SelectItem(m_treePC.GetRootItem());
        m_treePC.SelectItem(hSelectedItem);
    }
    
    return CPropertyPage::OnSetActive();
}


BOOL CPagePC::OnKillActive()
{
    CRightToolView::GetInstance()->SetEnable(CRightToolView::DISABLE_ALL);

    // 현재 화면에 있는 모델을 화면에서 지운다.
    CMTCharacter* pCharacter = (CMTCharacter*)m_treePC.GetItemData(m_treePC.GetSelectedItem());
    if(pCharacter)
    {
        pCharacter->SetVisible(FALSE);
    }

    return CPropertyPage::OnKillActive();
}

void CPagePC::OnBnClickedButton1()
{
    OnSaveScript(m_treePC.SelectScriptName(), TRUE);
}

void CPagePC::OnSaveScript(const WCHAR* szScriptName, BOOL bVisible)
{
    USES_CONVERSION;

    CMTCharacter* pCharacter = (CMTCharacter*)m_treePC.GetItemData(m_treePC.GetSelectedItem());
    if(!pCharacter)
    {
        MessageBox(L"Not Exist Character");
        return;
    }

    CString sCurrentScriptFileName = szScriptName;
    if(sCurrentScriptFileName != L"New Script")
    {
        std::string sSaveFileName = W2A(m_sSaveFolderName);        
        sSaveFileName += W2A(sCurrentScriptFileName);
        sCurrentScriptFileName.Replace(L".XML", L"");
        pCharacter->GetProperty()->SetName(W2A(sCurrentScriptFileName));        
        RwBool bReturn = pCharacter->GetProperty()->SaveScript(sSaveFileName.c_str());
        if(bReturn)
        {
            if(bVisible)
            {
                MessageBox(L"Script File Save Success");
            }
        }
        else
        {
            MessageBox(L"Script File Save Fail");
        }                
    }
    else
    {
        if(bVisible)
        {
            CString sSavePath = m_sSaveFolderName;
            CString sMeshName;
            m_edClumpFileName.GetWindowText(sMeshName);
            sMeshName = sMeshName.MakeUpper();
            sMeshName.Replace(L".DFF", L".XML");
            sSavePath += sMeshName;
            WCHAR szSaveFilder[] = L"Character Script File (*.XML)|*.XML||";
            CFileDialog fileDlg(FALSE, L"XML", sSavePath, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szSaveFilder);
            if(fileDlg.DoModal() == IDOK)
            {
                CString sSaveFilePath = fileDlg.GetPathName();
                CString sSaveFileName = fileDlg.GetFileName();

                sSaveFileName.Replace(L".XML", L""); // 프로퍼티에 네임에서는 확장자를 제외한다.
                pCharacter->GetProperty()->SetName(W2A(sSaveFileName));                
                RwBool bReturn = pCharacter->GetProperty()->SaveScript(W2A(sSaveFilePath));
                if(bReturn)
                {
                    sSaveFileName += L".XML";   // 다시 확장자를 붙여준다.
                    m_edScriptFileName.SetWindowText(sSaveFileName);   
                    m_treePC.SetScriptName(sSaveFileName);
                }
                else
                {
                    MessageBox(L"Script File Save Fail");
                }
            }
        }
        else
        {
            pCharacter->GetProperty()->SaveScript(W2A(sCurrentScriptFileName));
        }
    }    
}

void CPagePC::OnBnClickedBtSaveAll()
{
    USES_CONVERSION;

    // 모든 스크립트를 저장한다.
    TREEITEM_VECTOR vAllITem;
    m_treePC.GetAllChildNode(m_treePC.GetRootItem(), vAllITem);
    for(size_t i = 0; i < vAllITem.size(); ++i)
    {
        if(vAllITem[i] && m_treePC.GetItemData(vAllITem[i]))
        {
            CString sScriptName = m_treePC.GetItemText(vAllITem[i]);
            if(sScriptName != "")
            {
                std::string sSaveFileName = W2A(m_sSaveFolderName);
                sSaveFileName += W2A(sScriptName);
                CMTCharacter* pCharacter = (CMTCharacter*)m_treePC.GetItemData(vAllITem[i]);
                if(pCharacter)
                {
                    pCharacter->GetProperty()->SaveScript(sSaveFileName.c_str());
                }

            }
        }                
    }

    MessageBox(L"Save All Complete");
}

void CPagePC::OnMenuLoadScript()
{
    USES_CONVERSION;
    // 스크립트 파일을 로드 한다. (스크립트 파일은 .\Character\ 폴더에 있어야만 한다.)
    WCHAR sLoadFilter[] = L"Character Script File (*.xml)|*.xml||";
    CString workDir = A2W(CModelToolApplication::GetInstance()->GetWorkDir());
    workDir += L"\\Character\\*.xml";    
    CFileDialog fileDlg(TRUE, NULL, workDir, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sLoadFilter);
    if(fileDlg.DoModal() == IDOK)
    {
        CString sLoadFileName = fileDlg.GetFileName();
        //OnDisplayScriptInfo(sLoadFileName);        

        // 트리에 항목을 추가한다.
        m_treePC.AddItem(W2A(sLoadFileName));
    }
}

void CPagePC::OnMenuNewScript()
{
    // 새로운 스크립트를 생성시, DFF 파일도 같이 선택한다.
    OnBnClickedBtSetClump();
}

void CPagePC::OnDestroy()
{
    // 창이 없어지기 전에 XML로 내용을 저장한다.
    USES_CONVERSION;

    CString sWorkFolder = A2W(CModelToolApplication::GetInstance()->GetWorkDir());
    sWorkFolder += L"\\TreePC.xml";
    m_treePC.SaveTree(L"PC", (LPWSTR)(LPCWSTR)sWorkFolder);

    CPropertyPage::OnDestroy();    
}

void CPagePC::OnMenuScriptSave()
{
    // 현재 선택된 스크립트를 저장한다.
    // NewScript라면 새로 저장을 하고, 기존 스크립트라면 덮어 쓴다.
    CString sScriptName = m_treePC.GetItemText(m_treePC.GetSelectedItem());
    OnSaveScript(sScriptName, TRUE); 
}

void CPagePC::OnMenuScriptSaveas()
{
    // 새로운 이름으로 스크립트를 저장한다.
    OnSaveScript(L"New Script", TRUE);
}

void CPagePC::OnMenuScriptRename(CString sOrgScriptName, CString sNewScriptName)
{
    USES_CONVERSION;

    // 스크립트 파일명을 변경한다.    
    // 새 스크립트인 경우에는 그파일이름으로 저장한다.
    if(sOrgScriptName == L"New Script")
    {
        OnSaveScript(sNewScriptName, FALSE);
    }
    else
    {
        CMTCharacter* pCharacter = (CMTCharacter*)m_treePC.GetItemData(m_treePC.GetSelectedItem());
        if(pCharacter && pCharacter->GetProperty())
        {
            sNewScriptName.Replace(L".XML", L"");
            pCharacter->GetProperty()->SetName(W2A(sNewScriptName));
        }
        sOrgScriptName = m_sSaveFolderName + sOrgScriptName;
        sNewScriptName = m_sSaveFolderName + sNewScriptName;
        CFile::Rename(sOrgScriptName, sNewScriptName);   
    }    
}

void CPagePC::OnMenuDeleteScript(HTREEITEM hItem)
{
    if(!hItem)
        return;

    CMTCharacter* pCharacter = (CMTCharacter*)m_treePC.GetItemData(hItem);
    if(pCharacter)
    {
        pCharacter->SetVisible(FALSE);
    }
}
void CPagePC::OnSize(UINT nType, int cx, int cy)
{
    CPropertyPage::OnSize(nType, cx, cy);

    //if(m_treePC.GetSafeHwnd())
    //    m_treePC.MoveWindow(0, 0, cx, cy - PAGE_HEIGHT_BOTTOM_CTRL);
}
