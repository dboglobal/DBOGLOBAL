// NaviMeshPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "NaviMeshPane.h"
#include "MainFrm.h"


CNaviMeshPane* CNaviMeshPane::m_pInstance = NULL;
// CNaviMeshPane

IMPLEMENT_DYNCREATE(CNaviMeshPane, CXTResizeFormView)

CNaviMeshPane::CNaviMeshPane()
	: CXTResizeFormView(CNaviMeshPane::IDD)
{
    m_pInstance = this;
    m_pObject = NULL;
}

CNaviMeshPane::~CNaviMeshPane()
{
    RemoveAllClump();
}

void CNaviMeshPane::DoDataExchange(CDataExchange* pDX)
{
    CXTResizeFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_NAVI, m_listNavi);
    DDX_Control(pDX, IDC_LIST_OBS, m_listObs);
}

BEGIN_MESSAGE_MAP(CNaviMeshPane, CXTResizeFormView)
END_MESSAGE_MAP()


// CNaviMeshPane 진단입니다.

#ifdef _DEBUG
void CNaviMeshPane::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNaviMeshPane::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNaviMeshPane 메시지 처리기입니다.

void CNaviMeshPane::OnInitialUpdate()
{
    CXTResizeFormView::OnInitialUpdate();

    SetResize(IDC_ST_NAVI, SZ_TOP_LEFT, SZ_TOP_LEFT);
    SetResize(IDC_LIST_NAVI, SZ_TOP_LEFT, SZ_MIDDLE_RIGHT);
    SetResize(IDC_ST_OBS, SZ_MIDDLE_LEFT, SZ_MIDDLE_LEFT);
    SetResize(IDC_LIST_OBS, SZ_MIDDLE_LEFT, SZ_BOTTOM_RIGHT);

    if(CModelToolApplication::GetInstance()->GetAppMode() == MT_MODE_OBJECT)
    {
        SetObject(CModelToolApplication::GetInstance()->GetEditObject());
    }
}

void CNaviMeshPane::SetObject(CMTObject* pObject)
{
    if(m_pObject == pObject)
        return;

    USES_CONVERSION;

    m_listNavi.ResetContent();
    m_listObs.ResetContent();

    CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

    if(!pObject)
    {
        pFrame->SetVisibleNaviPane(FALSE);
        return;
    }

    pFrame->SetVisibleNaviPane(TRUE);
    m_pObject = pObject;
    RemoveAllClump();

    CString strName = (pObject->GetClumpInfo()->GetClumpName());
    strName = GetFileNameFromPath(strName);
    strName.Replace(".DFF", "");
    strName.Replace(".dff", "");

    CString strNaviMesh = (CModelToolApplication::GetInstance()->GetWorkDir());
    strNaviMesh += MT_PATH_NAVI_MESH;
    strNaviMesh += strName + "_PE_Nav_";

    CString strObsMesh = (CModelToolApplication::GetInstance()->GetWorkDir());
    strObsMesh += MT_PATH_NAVI_MESH;
    strObsMesh += strName + "_PE_Obs_";
    
    CFileFind finder;
    CString tempNavi, tempObs;
    int nCount = 0;
    while(TRUE)
    {
        RwBool bNone = TRUE;

        tempNavi.Format("%s%02d.dff", strNaviMesh, nCount);
        tempObs.Format("%s%02d.dff", strObsMesh, nCount);

        if(finder.FindFile(tempNavi))
        {
            m_listNavi.AddString(GetFileNameFromPath(tempNavi));
            CreateClump(tempNavi);

            bNone = FALSE;
        }

        if(finder.FindFile(tempObs))
        {
            m_listObs.AddString(GetFileNameFromPath(tempObs));
            CreateClump(tempObs);

            bNone = FALSE;
        }

        ++nCount;

        if(bNone) break;
    }
}

void CNaviMeshPane::Render() 
{
    USES_CONVERSION;

    for each(CMTClump* pClump in m_vecClump)
    {
        pClump->SetVisible(FALSE);
    }

    // Navigation Mesh
    int nCnt = m_listNavi.GetSelCount();
    int* arrIndex = new int[nCnt];
    m_listNavi.GetSelItems(nCnt, arrIndex);
    CString strMeshName;

    for(int i = 0; i < nCnt; ++i)
    {
        m_listNavi.GetText(arrIndex[i], strMeshName);    

        for each(CMTClump* pClump in m_vecClump)
        {
            if(strstr(pClump->GetClumpName(), (strMeshName)))
            {
                pClump->SetVisible(TRUE);
            }            
        }
    }

    delete[] arrIndex;

    // Obstacle Mesh
    nCnt = m_listObs.GetSelCount();
    arrIndex = new int[nCnt];
    m_listObs.GetSelItems(nCnt, arrIndex);    

    for(int i = 0; i < nCnt; ++i)
    {
        m_listObs.GetText(arrIndex[i], strMeshName);    

        for each(CMTClump* pClump in m_vecClump)
        {
            if(strstr(pClump->GetClumpName(), (strMeshName)))
            {
                pClump->SetVisible(TRUE);
            }            
        }
    }

    delete[] arrIndex;
}

void CNaviMeshPane::CreateClump( CString& strFileName ) 
{
    USES_CONVERSION;

    CMTClump* pMTClump = new CMTClump();
    if(!pMTClump->Load((LPSTR)(LPCSTR)(strFileName)))
    {
        delete pMTClump;
        return;
    }

    pMTClump->SetVisible(FALSE);
    m_vecClump.push_back(pMTClump);
}

void CNaviMeshPane::RemoveAllClump() 
{
    for each(CMTClump* pClump in m_vecClump)
    {
        pClump->Destroy();
        delete pClump;
    }

    m_vecClump.clear();
}