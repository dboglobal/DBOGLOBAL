// ClumpPane.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "ClumpPane.h"
#include "Materialpane.h"
#include "ModelToolApplication.h"

#define BOTTOM_HEIGHT   118
#define OFFSET          3          


// CClumpPane
CClumpPane* CClumpPane::m_pInstance = NULL;

IMPLEMENT_DYNCREATE(CClumpPane, CXTResizeFormView)

CClumpPane::CClumpPane()
	: CXTResizeFormView(CClumpPane::IDD)
{
    m_pInstance = this;

    m_pMTClump = NULL;

    m_bFlagSetModel = FALSE;
    m_bToon = FALSE;

    for(int i = 0; i < CLUMP_CKBTN_CNT; ++i)
    {
        m_bFlagCkBtn[i] = FALSE;
    }
}

CClumpPane::~CClumpPane()
{
    for(UINT i = 0; i < m_vMTClump.size(); ++i)
    {
        if(m_vMTClump[i])
        {
            delete m_vMTClump[i];
            m_vMTClump[i] = NULL;
        }
    }
    m_vMTClump.clear();
}

void CClumpPane::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_CLUMP_NAME, m_editClumpName);
    DDX_Control(pDX, IDC_LIST_ATOMIC, m_listAtomic);
    DDX_Control(pDX, IDC_BT_DELETE, m_btDelete);
    DDX_Control(pDX, IDC_BT_CLUMP_SAVE, m_btSave);
    DDX_Control(pDX, IDC_BT_CLUMP_SAVE_ALL, m_btSaveSplit);
    DDX_Control(pDX, IDC_CK_2SIDE, m_ck2Side);
    DDX_Control(pDX, IDC_CK_2SIDE_EDGE, m_ck2SideEdge);
    DDX_Control(pDX, IDC_CHECK_ALPHA, m_ckAlphaTest);
    DDX_Control(pDX, IDC_CHECK_ALPHA2, m_ckAlpha);
    DDX_Control(pDX, IDC_CHECK_ALPHA3, m_ckRender2Side);
    DDX_Control(pDX, IDC_CHECK_EMBLEM_MARK, m_ckEmblemMark);
    DDX_Control(pDX, IDC_CHECK_COLLISION, m_ckCollision);    
    DDX_Control(pDX, IDC_BT_SAVECLUMP, m_btSaveClump);
    DDX_Control(pDX, IDC_CHECK_NOT_VISIBLE, m_ckNotVisible);
    DDX_Control(pDX, IDC_CHECK_DECAL_VISIBLE, m_ckDecalVisible);
    DDX_Control(pDX, IDC_CHECK_SHADOWMAP, m_ckShadowMap);
    DDX_Control(pDX, IDC_COMBO1, m_cbShadowMapResolution);
    DDX_Control(pDX, IDC_ED_FRUSTUM, m_EdFrustum);
    DDX_Control(pDX, IDC_STT_FRUSTUM, m_sttFrustum);    
    DDX_Control(pDX, IDC_CHECK_CAMERA_COLLISION, m_ckCameraCollision);    
    DDX_Control(pDX, IDC_EDIT_ENVMAP, m_edEnvMap);
    DDX_Control(pDX, IDC_CB_ENV, m_cbEnv);    
}

BEGIN_MESSAGE_MAP(CClumpPane, CXTResizeFormView)
    ON_WM_SIZE()
    ON_EN_CHANGE(IDC_EDIT_CLUMP_NAME, &CClumpPane::OnEnChangeEditClumpName)
    ON_LBN_SELCHANGE(IDC_LIST_ATOMIC, &CClumpPane::OnLbnSelchangeListAtomic)
    ON_BN_CLICKED(IDC_BT_DELETE, &CClumpPane::OnBnClickedBtDelete)
    ON_BN_CLICKED(IDC_BT_CLUMP_SAVE, &CClumpPane::OnBnClickedBtClumpSave)
    ON_BN_CLICKED(IDC_BT_CLUMP_SAVE_ALL, &CClumpPane::OnBnClickedBtClumpSaveAll)
    ON_BN_CLICKED(IDC_CK_2SIDE, &CClumpPane::OnBnClickedCk2side)
    ON_BN_CLICKED(IDC_CK_2SIDE_EDGE, &CClumpPane::OnBnClickedCk2sideEdge)
    ON_BN_CLICKED(IDC_CHECK_ALPHA, &CClumpPane::OnBnClickedCheckAlpha)
    ON_BN_CLICKED(IDC_CHECK_ALPHA2, &CClumpPane::OnBnClickedCheckAlpha2)
    ON_BN_CLICKED(IDC_CHECK_ALPHA3, &CClumpPane::OnBnClickedCheckAlpha3)
    ON_BN_CLICKED(IDC_CHECK_COLLISION, &CClumpPane::OnBnClickedCheckCollision)    
    ON_BN_CLICKED(IDC_BT_SAVECLUMP, &CClumpPane::OnBnClickedBtSaveclump)
    ON_BN_CLICKED(IDC_CHECK_NOT_VISIBLE, &CClumpPane::OnBnClickedCheckNotVisible)
	ON_BN_CLICKED(IDC_CHECK_DECAL_VISIBLE, &CClumpPane::OnBnClickedCheckDecalVisible)
	ON_BN_CLICKED(IDC_CHECK_SHADOWMAP, &CClumpPane::OnBnClickedCheckShadowmap)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CClumpPane::OnCbnSelchangeCombo1)
	ON_EN_CHANGE(IDC_ED_FRUSTUM, &CClumpPane::OnEnChangeEdFrustum)    
    ON_BN_CLICKED(IDC_CHECK_CAMERA_COLLISION, &CClumpPane::OnBnClickedCheckCameraCollision)    
    ON_EN_CHANGE(IDC_EDIT_ENVMAP, &CClumpPane::OnEnChangeEditEnvmap)
    ON_CBN_SELCHANGE(IDC_CB_ENV, &CClumpPane::OnCbnSelchangeCbEnv)
    ON_BN_CLICKED(IDC_CHECK_EMBLEM_MARK, &CClumpPane::OnBnClickedCheckEmblemMark)
END_MESSAGE_MAP()


// CClumpPane diagnostics

#ifdef _DEBUG
void CClumpPane::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CClumpPane::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CClumpPane message handlers
void CClumpPane::OnInitialUpdate()
{
    CXTResizeFormView::OnInitialUpdate();

    m_editClumpName.Initialize(this, BES_XT_CHOOSEFILE);    
    m_editClumpName.SetDlgFilter("Clump File (*.dff)|*.dff||");

    m_edEnvMap.Initialize(this, BES_XT_CHOOSEFILE);
    m_edEnvMap.SetDlgFilter("Texture File (*.dds) | *.dds||");

    SetResize(IDC_EDIT_CLUMP_NAME, SZ_TOP_LEFT, SZ_TOP_RIGHT);
    SetResize(IDC_FRAME_ATOMIC, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_LIST_ATOMIC, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_BT_DELETE, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_BT_SAVECLUMP, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_BT_CLUMP_SAVE, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_BT_CLUMP_SAVE_ALL, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_STATIC_TOON, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_CK_2SIDE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
    SetResize(IDC_CK_2SIDE_EDGE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
    SetResize(IDC_STATIC_RENDER, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_CHECK_ALPHA, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
    SetResize(IDC_CHECK_ALPHA2, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
    SetResize(IDC_CHECK_ALPHA3, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
    SetResize(IDC_CHECK_EMBLEM_MARK, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_SHADOWMAP, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STT_FRUSTUM, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_ED_FRUSTUM, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);  
    SetResize(IDC_ST_ENV_ATOMIC, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
    SetResize(IDC_ST_ENV_TEXTURE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
    SetResize(IDC_CB_ENV, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_EDIT_ENVMAP, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);    
	SetResize(IDC_COMBO1, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_CHECK_COLLISION, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_STATIC_COLLISION, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_CHECK_NOT_VISIBLE, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_CHECK_DECAL_VISIBLE, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);    
    SetResize(IDC_CHECK_CAMERA_COLLISION, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);

	m_cbShadowMapResolution.AddString("64");
	m_cbShadowMapResolution.AddString("128");
	m_cbShadowMapResolution.AddString("256");
	m_cbShadowMapResolution.AddString("512");	
	m_cbShadowMapResolution.SetItemData(0, 64);
	m_cbShadowMapResolution.SetItemData(1, 128);
	m_cbShadowMapResolution.SetItemData(2, 256);
	m_cbShadowMapResolution.SetItemData(3, 512);	
    
    //CXTPPaintManager::SetTheme(xtpThemeOffice2003);
    //XTThemeManager()->SetTheme(xtThemeOffice2003);
}

void CClumpPane::SetClump(CMTClump* pMTClump)
{
    USES_CONVERSION;
    
    if(m_pMTClump == pMTClump)
        return;

    m_pMTClump = pMTClump;

    if(!pMTClump)
    {
        m_listAtomic.ResetContent();
        m_editClumpName.SetWindowText("");
        SetEnable(FALSE);                
    }
    else
    {
        m_bFlagSetModel = TRUE;        
        m_editClumpName.SetWindowText((m_pMTClump->GetClumpName()));
        DisplayAtomics();
        SetEnable(TRUE);
    }
}

void CClumpPane::SetEnable(BOOL bEnable)
{
    m_listAtomic.EnableWindow(bEnable);
    m_btSaveSplit.EnableWindow(bEnable);
    m_btSaveClump.EnableWindow(bEnable);
 

    // Atomic을 선택하기전까지 모든 버튼은 Disable 시켜둔다.
    m_btDelete.EnableWindow(bEnable);
    m_btSave.EnableWindow(bEnable);    

    SetEnableAllButtons(FALSE);
}

void CClumpPane::SetEnableAllButtons(BOOL bEnable)
{
    m_ck2Side.EnableWindow(bEnable);    
    m_ckAlphaTest.EnableWindow(bEnable);
    m_ckAlpha.EnableWindow(bEnable);
    m_ckRender2Side.EnableWindow(bEnable);
    m_ckEmblemMark.EnableWindow(bEnable);
    m_ckCollision.EnableWindow(bEnable);  
    m_ckNotVisible.EnableWindow(bEnable);
	m_ckDecalVisible.EnableWindow(bEnable);    
	m_ckShadowMap.EnableWindow(bEnable);	
    m_ckCameraCollision.EnableWindow(bEnable);
	m_cbShadowMapResolution.EnableWindow(FALSE);		
	m_sttFrustum.EnableWindow(bEnable);
	m_EdFrustum.EnableWindow(bEnable);    
}

RwBool CClumpPane::OnLoadClump(RwChar* szClumpFileName)
{
    USES_CONVERSION;

    if(!szClumpFileName)
        return FALSE;

    CMTClump* pMTClump = new CMTClump();
    if(!pMTClump->Load(szClumpFileName))
    {
        delete pMTClump;        
        MessageBox("Clump Load Fail!");                
        CModelToolApplication::GetInstance()->SetEditChar(NULL);
        return FALSE;
    }

    if(m_pMTClump)
    {
        m_pMTClump->SetVisible(FALSE);
    }

    m_pMTClump = pMTClump;
    m_vMTClump.push_back(pMTClump);   

    CModelToolApplication::GetInstance()->SetAppMode(MT_MODE_CLUMP);
    CModelToolApplication::GetInstance()->SetEditChar(NULL);
    GetSafeInstance(CMaterialPane)->SetClump(m_pMTClump);

    DisplayAtomics();
    SetEnable(TRUE);

    return TRUE;
}

void CClumpPane::DisplayAtomics()
{
    USES_CONVERSION;

    if(!m_pMTClump)
        return;

    m_listAtomic.ResetContent();
    m_pMTClump->ClearBB();

    m_cbEnv.ResetContent();
    m_cbEnv.AddString("");

    std::vector<char*> vAtomicNames = m_pMTClump->GetAtomicNames();
    for(UINT i = 0; i < vAtomicNames.size(); ++i)
    {
        m_listAtomic.AddString((vAtomicNames[i]));
        m_cbEnv.AddString((vAtomicNames[i]));
    }    

    
	SetEnableAllButtons(FALSE);

    // 환경맵 설정
    SetEnvMap();    
}

void CClumpPane::OnEnChangeEditClumpName()
{
    USES_CONVERSION;

    // 다른클래스에서 텍스트를 바꾼 경우에는 모델을 로드하지 않는다.
    if(m_bFlagSetModel)
    {
        m_bFlagSetModel = FALSE;        
    }
    else
    {
        CString strClumpFileName;
        m_editClumpName.GetWindowText(strClumpFileName);
        
        if(strClumpFileName != "")
        {
            OnLoadClump((LPSTR)(LPCSTR)(strClumpFileName));
        }        
    }
}

void CClumpPane::OnLbnSelchangeListAtomic()
{
    // ListBox의 Atomic을 선택햇을때 바운딩박스를 그려준다.
    USES_CONVERSION;

    if(!m_pMTClump)
        return;

    m_pMTClump->ClearBB();

    // 처음엔 모든 체크 버튼을 UnCheck한다
    for(int i = 0; i < CLUMP_CKBTN_CNT; ++i)
    {
        m_bFlagCkBtn[i] = FALSE;
    }
    m_ck2Side.SetCheck(BST_UNCHECKED);
    m_ck2SideEdge.SetCheck(BST_UNCHECKED);
    m_ckAlphaTest.SetCheck(BST_UNCHECKED);
    m_ckAlpha.SetCheck(BST_UNCHECKED);
    m_ckRender2Side.SetCheck(BST_UNCHECKED);
    m_ckEmblemMark.SetCheck(BST_UNCHECKED);
    m_ckCollision.SetCheck(BST_UNCHECKED);    
	m_ckDecalVisible.SetCheck(BST_UNCHECKED);
	m_ckShadowMap.SetCheck(BST_UNCHECKED);    
    m_ckCameraCollision.SetCheck(BST_UNCHECKED);    

    int nCount = m_listAtomic.GetSelCount();
    if(nCount > 0)
    {
        
        m_btDelete.EnableWindow(TRUE);
        m_btSave.EnableWindow(TRUE);        

        if(m_bToon)
        {
            m_ck2Side.EnableWindow(TRUE);
            m_ck2SideEdge.EnableWindow(TRUE);
        }
        
        m_ckAlphaTest.EnableWindow(TRUE);
        m_ckAlpha.EnableWindow(TRUE);
        m_ckRender2Side.EnableWindow(TRUE);
        m_ckEmblemMark.EnableWindow(TRUE);
        m_ckCollision.EnableWindow(TRUE);             
        m_ckCameraCollision.EnableWindow(TRUE);        
        m_cbEnv.EnableWindow(TRUE);
        m_edEnvMap.EnableWindow(TRUE);

        int* arrIndex = new int[nCount];
        m_listAtomic.GetSelItems(nCount, arrIndex);
        for(int i = 0; i < nCount; i++)
        {
            CHAR szAtomicName[128] = {0,};			

            m_listAtomic.GetText(arrIndex[i], szAtomicName);
            m_pMTClump->CreateBB((RwChar*)(szAtomicName));            
            OnSetCheckButtons(szAtomicName);
        }	

        delete[] arrIndex;
        
    }   
    else
    {
        m_btDelete.EnableWindow(FALSE);
        m_btSave.EnableWindow(FALSE);        
        SetEnableAllButtons(FALSE);
    }
}

void CClumpPane::OnSetCheckButtons(CHAR* szAtomicName)
{
    USES_CONVERSION;

    if(!szAtomicName || !m_pMTClump)
        return;

    RpAtomic* pAtomic = m_pMTClump->GetAtomicByName((szAtomicName));
    if(!pAtomic)
        return;

    // Toon 이 적용되어있는지 Check 한다.
    RpToonGeo *g = NULL;
    g = RpToonGeometryGetToonGeo( RpAtomicGetGeometry(pAtomic) );
    if(g == NULL)
    {
        m_ck2Side.EnableWindow(FALSE);
        m_ck2SideEdge.EnableWindow(FALSE);
    }
    else
    {
        m_ck2Side.EnableWindow(TRUE);
        m_ck2SideEdge.EnableWindow(TRUE);
    }

    RwInt32 flag = RpNtlAtomicGetFlag(pAtomic);
    int nCk2Side = m_ck2Side.GetCheck();
    int nCk2SideEdge = m_ck2SideEdge.GetCheck();
    int nCkAlphaTest = m_ckAlphaTest.GetCheck();
    int nCkAlpha = m_ckAlpha.GetCheck();
    int nCkRender2Side = m_ckRender2Side.GetCheck();
    int nCkEmblemMark = m_ckEmblemMark.GetCheck();
    int nCkCollision = m_ckCollision.GetCheck();        
    //int nCkCameraCollision = m_ckCameraCollision.GetCheck();

    if(flag & NTL_TOON_NOT_EDGE)
    {
        if(m_bFlagCkBtn[0] == false)
            m_ck2Side.SetCheck(BST_CHECKED);        
    }
    else if(nCk2Side == BST_CHECKED)
    {
        m_ck2Side.SetCheck(BST_UNCHECKED);
        m_bFlagCkBtn[0] = TRUE;
    }

    if(flag & NTL_TOON_DETAIL_EDGE)
    {
        if(m_bFlagCkBtn[1] == false)
            m_ck2SideEdge.SetCheck(BST_CHECKED);        
    }
    else if(nCk2SideEdge == BST_CHECKED)
    {
        m_ck2SideEdge.SetCheck(BST_UNCHECKED);
        m_bFlagCkBtn[1] = TRUE;     
    }

    if(flag & NTL_ALPHATEST)
    {
        if(m_bFlagCkBtn[2] == false)
            m_ckAlphaTest.SetCheck(BST_CHECKED);
    }
    else if(nCkAlphaTest == BST_CHECKED)
    {
        m_ckAlphaTest.SetCheck(BST_UNCHECKED);
        m_bFlagCkBtn[2] = TRUE;
    }

    if(flag & NTL_ALPHA)
    {
        if(m_bFlagCkBtn[3] == false)
            m_ckAlpha.SetCheck(BST_CHECKED);
    }
    else if(nCkAlpha == BST_CHECKED)
    {
        m_ckAlpha.SetCheck(BST_UNCHECKED);
        m_bFlagCkBtn[3] = TRUE;
    }

    if(flag & NTL_TWOSIDE)
    {
        if(m_bFlagCkBtn[4] == false)
            m_ckRender2Side.SetCheck(BST_CHECKED);
    }
    else if(nCkRender2Side == BST_CHECKED)
    {
        m_ckRender2Side.SetCheck(BST_UNCHECKED);
        m_bFlagCkBtn[4] = TRUE;
    }

    if(flag & NTL_COLLISION)
    {
        if(m_bFlagCkBtn[5] == false)
            m_ckCollision.SetCheck(BST_CHECKED);
    }
    else if(nCkCollision == BST_CHECKED)
    {
        m_ckCollision.SetCheck(BST_UNCHECKED);
        m_bFlagCkBtn[5] = TRUE;
    }

    if(flag & NTL_CAMERA_COLLISION)
    {
        if(m_bFlagCkBtn[6] == false)            
            m_ckCameraCollision.SetCheck(BST_CHECKED);
    }
    else if(nCkCollision == BST_CHECKED)
    {
        m_ckCameraCollision.SetCheck(BST_UNCHECKED);
        m_bFlagCkBtn[6] = TRUE;
    }

    if(flag & NTL_EMBLEM_MARK)
    {
        if(m_bFlagCkBtn[7] == false)
            m_ckEmblemMark.SetCheck(BST_CHECKED);
    }
    else if(nCkEmblemMark == BST_CHECKED)
    {
        m_ckEmblemMark.SetCheck(BST_UNCHECKED);
        m_bFlagCkBtn[7] = TRUE;
    }

    if(flag & NTL_NOT_VISIBLE)
    {
        m_ckNotVisible.SetCheck(TRUE);

        SetEnableAllButtons(FALSE);
        m_ckNotVisible.EnableWindow(TRUE);
    }
    else
    {
        m_ckNotVisible.SetCheck(FALSE);

        SetEnableAllButtons(TRUE);
    }

	if(flag & NTL_DECAL_VISIBLE)
	{
		m_ckDecalVisible.SetCheck(TRUE);
	}
	else
	{
		m_ckDecalVisible.SetCheck(FALSE);
	}

	if(flag & NTL_SHADOW_MAP)
	{
		m_ckShadowMap.SetCheck(TRUE);
		m_cbShadowMapResolution.EnableWindow(TRUE);
		SetShadowMapSize(szAtomicName);
	}
	else
	{
		m_ckShadowMap.SetCheck(FALSE);		
		m_cbShadowMapResolution.EnableWindow(FALSE);
		m_cbShadowMapResolution.SelectString(0, "0");
	}

	// Frustum Scale
	RwReal fFrustumScale = RpNtlAtomicGetUserDatReal(pAtomic);
	CString strFrustumScale;
	strFrustumScale.Format("%.3f", fFrustumScale);
	m_EdFrustum.SetWindowText(strFrustumScale);
    
}

void CClumpPane::OnBnClickedBtDelete()
{
    if(!m_pMTClump)
        return;

    USES_CONVERSION;

    
    // 선택되지 않은 Atomic을 골라서 삭제한다.
    std::vector<CString> vDeleteItems;
    int nCount = m_listAtomic.GetSelCount();
    int nTotalCount = m_listAtomic.GetCount();
    if(nCount > 0)
    {
        for(int i = 0; i < nTotalCount; ++i)
        {
            if(m_listAtomic.GetSel(i) <= 0)
            {
                CHAR szAtomicName[128] = {0,};
                m_listAtomic.GetText(i, szAtomicName);
                RwBool retBool = m_pMTClump->RemoveAtomic((szAtomicName));
                if(retBool == FALSE)
                {
                    MessageBox("Atomic Remove Error!", szAtomicName);
                }
                else
                {
                    vDeleteItems.push_back(szAtomicName);                    
                }
            }                        
        }

        for(UINT i = 0; i < vDeleteItems.size(); ++i)
        {
            int nIndex = m_listAtomic.FindString(0, vDeleteItems[i]);
            m_listAtomic.DeleteString(nIndex);
        }

        //m_pCharacter->UpdateClumpInfo();
    }    
}

void CClumpPane::OnBnClickedBtClumpSave()
{
    USES_CONVERSION;

    if(!m_pMTClump)
        return;

    SaveEnvMap();

    // 선택한 Atomic들만 Save한다.

    CString strClumpName = (m_pMTClump->GetClumpName());

    CHAR szOpenFilter[] = "Clump File (*.dff)|*.dff||";
    CFileDialog fileDlg(FALSE, "dff", strClumpName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szOpenFilter);

    if(fileDlg.DoModal() == IDOK)
    {
        CString strFileName = fileDlg.GetPathName();	        

        OnBnClickedBtDelete();

        RwBool retBool = m_pMTClump->Save((LPSTR)(LPCSTR)(strFileName));
        if(retBool == (RwBool)FALSE)
        {
            MessageBox("Clump File Save Fail");
        }

        if(CModelToolApplication::GetInstance()->GetAppMode() == MT_MODE_CLUMP)
        {
            // 파츠를 Save후 원래 모델을 다시 로드한다.
            m_pMTClump->Load((LPSTR)(LPCSTR)(strClumpName));   
            DisplayAtomics();
        }        
    }  
}

void CClumpPane::OnBnClickedBtClumpSaveAll()
{
    USES_CONVERSION;
    // 모든 파츠를 저장한다.
    if(!m_pMTClump)
        return;

    SaveEnvMap();

    CString strFileName = (m_pMTClump->GetClumpName());
    strFileName = strFileName.MakeUpper();
    strFileName.Replace(".DFF", "_");

    for(int i = 0; i < m_listAtomic.GetCount(); ++i)
    {
        m_listAtomic.SetSel(i);
        CString strSaveFileName = strFileName;
        CString strAtomicName;
        m_listAtomic.GetText(i, strAtomicName);
        strSaveFileName += strAtomicName;
        strSaveFileName += ".DFF";
        
        OnBnClickedBtDelete();
        m_pMTClump->Save((LPSTR)(LPCSTR)(strSaveFileName));

        m_pMTClump->Load((RwChar*)m_pMTClump->GetClumpName());
        m_listAtomic.SetSel(-1, FALSE);
        
    }

    MessageBox("All Split Parts Save Success");
}

void CClumpPane::SetChangeAtomicFlag( ENtlAtomicFlag eFlag, RwBool bEnable ) 
{
	USES_CONVERSION;

	if(!m_pMTClump)
		return;

	int nCount = m_listAtomic.GetSelCount();
	if(nCount > 0)
	{
		int* arrIndex = new int[nCount];
		m_listAtomic.GetSelItems(nCount, arrIndex);
		for(int i = 0; i < nCount; i++)
		{
			CHAR szAtomicName[128] = {0,};			
			m_listAtomic.GetText(arrIndex[i], szAtomicName); 			
			m_pMTClump->SetAtomicFlag(eFlag, (szAtomicName), bEnable);                

			// Flag에 따른 추가 작업
			switch(eFlag)
			{
			case NTL_COLLISION:			// Collision Tree를 빌드한다.
				m_pMTClump->CollisionDataBuild((szAtomicName));
				break;
			case NTL_SHADOW_MAP:		// 설정된 멀티텍스쳐의 크기를 설정한다.
				if(bEnable)
				{
					SetShadowMapSize(szAtomicName);
				}
				else
				{
					m_cbShadowMapResolution.SelectString(-1, "0");
				}
			}
		}	

		delete[] arrIndex;
	}  
}

void CClumpPane::OnBnClickedCk2side()
{
    USES_CONVERSION;
    if(!m_pMTClump)
        return;

    int nCount = m_listAtomic.GetSelCount();
    if(nCount > 0)
    {
        int* arrIndex = new int[nCount];
        m_listAtomic.GetSelItems(nCount, arrIndex);
        for(int i = 0; i < nCount; ++i)
        {
            CString strAtomicName;
            m_listAtomic.GetText(arrIndex[i], strAtomicName);
            if(m_ck2Side.GetCheck())
            {
                m_pMTClump->SetAtomicFlag(NTL_TOON_NOT_EDGE, (LPSTR)(LPCSTR)(strAtomicName), TRUE);
                m_ck2SideEdge.SetCheck(BST_UNCHECKED);                
                m_pMTClump->SetAtomicFlag(NTL_TOON_DETAIL_EDGE, (LPSTR)(LPCSTR)(strAtomicName), FALSE);
            }
            else
            {
                m_pMTClump->SetAtomicFlag(NTL_TOON_NOT_EDGE, (LPSTR)(LPCSTR)(strAtomicName), FALSE);
            }

			RpAtomic* pAtomic = m_pMTClump->GetAtomicByName((LPSTR)(LPCSTR)(strAtomicName));
			if(pAtomic)
			{
				Helper_SetToonAtomic(pAtomic, m_pMTClump->GetToonData());
			}
        }

        delete[] arrIndex;
    }    
}

void CClumpPane::OnBnClickedCk2sideEdge()
{
    USES_CONVERSION;

    if(!m_pMTClump)
        return;

    int nCount = m_listAtomic.GetSelCount();
    if(nCount > 0)
    {
        int* arrIndex = new int[nCount];
        m_listAtomic.GetSelItems(nCount, arrIndex);
        for(int i = 0; i < nCount; ++i)
        {
            CString strAtomicName;
            m_listAtomic.GetText(arrIndex[i], strAtomicName);
            if(m_ck2SideEdge.GetCheck())
            {
                m_pMTClump->SetAtomicFlag(NTL_TOON_DETAIL_EDGE, (LPSTR)(LPCSTR)(strAtomicName), TRUE);
                m_ck2Side.SetCheck(BST_UNCHECKED);
                m_pMTClump->SetAtomicFlag(NTL_TOON_NOT_EDGE, (LPSTR)(LPCSTR)(strAtomicName), FALSE);
            }
            else
            {
                m_pMTClump->SetAtomicFlag(NTL_TOON_DETAIL_EDGE, (LPSTR)(LPCSTR)(strAtomicName), FALSE);
            }

			RpAtomic* pAtomic = m_pMTClump->GetAtomicByName((LPSTR)(LPCSTR)(strAtomicName));
			if(pAtomic)
			{
				Helper_SetToonAtomic(pAtomic, m_pMTClump->GetToonData());
			}
        }

        delete[] arrIndex;
    }    
}

void CClumpPane::OnBnClickedCheckAlpha()
{
	SetChangeAtomicFlag(NTL_ALPHATEST, m_ckAlphaTest.GetCheck());
}

void CClumpPane::OnBnClickedCheckAlpha2()
{
	SetChangeAtomicFlag(NTL_ALPHA, m_ckAlpha.GetCheck());
}

void CClumpPane::OnBnClickedCheckAlpha3()
{
    // 2Side 설정 (Toon 말고 일반)    
	SetChangeAtomicFlag(NTL_TWOSIDE, m_ckRender2Side.GetCheck());    
}

void CClumpPane::OnBnClickedCheckEmblemMark()
{
    SetChangeAtomicFlag(NTL_EMBLEM_MARK, m_ckEmblemMark.GetCheck());
}

/**
 * Collision 설정 
 */
void CClumpPane::OnBnClickedCheckCollision()
{
	SetChangeAtomicFlag(NTL_COLLISION, m_ckCollision.GetCheck());	
}

// Not Visible 버튼을 체크했을때
void CClumpPane::OnBnClickedCheckNotVisible()
{
    SetChangeAtomicFlag(NTL_NOT_VISIBLE, m_ckNotVisible.GetCheck());
}

void CClumpPane::OnBnClickedCheckDecalVisible()
{
	SetChangeAtomicFlag(NTL_DECAL_VISIBLE, m_ckDecalVisible.GetCheck());
}

void CClumpPane::OnBnClickedCheckShadowmap()
{
	SetChangeAtomicFlag(NTL_SHADOW_MAP, m_ckShadowMap.GetCheck());
	m_cbShadowMapResolution.EnableWindow(m_ckShadowMap.GetCheck());
}


// Camera Collision 체크 박스가 클릭 되었을때
void CClumpPane::OnBnClickedCheckCameraCollision()
{
    SetChangeAtomicFlag(NTL_CAMERA_COLLISION, m_ckCameraCollision.GetCheck());
}

void CClumpPane::OnCbnSelchangeCombo1()
{
	USES_CONVERSION;
	
	if(!m_pMTClump)
		return;

	int nCurSel = m_cbShadowMapResolution.GetCurSel();
	RwUInt16 uiSize = (RwUInt16)m_cbShadowMapResolution.GetItemData(nCurSel);	
	if(uiSize == 0)
		return;

	int nCount = m_listAtomic.GetSelCount();
	if(nCount > 0)
	{
		int* arrIndex = new int[nCount];
		m_listAtomic.GetSelItems(nCount, arrIndex);
		for(int i = 0; i < nCount; i++)
		{
			CHAR szAtomicName[128] = {0,};			
			m_listAtomic.GetText(arrIndex[i], szAtomicName); 						
			RpNtlAtomicSetUserDatU16(m_pMTClump->GetAtomicByName((szAtomicName)), uiSize);
		}	

		delete[] arrIndex;
	}  
}


void CClumpPane::Render() 
{
    if(!m_pMTClump)
        return;

    // 선택된 Atomic들의 BB를 Render한다.
    RenderAtomicBB();

}

void CClumpPane::RenderAtomicBB() 
{
    if(!m_pMTClump)
        return;
    
    m_pMTClump->RenderAtomicBB();
}

void CClumpPane::DisplayInfo( RtCharset* pCharSet ) 
{
    if(!m_pMTClump || !pCharSet)
        return;

    m_pMTClump->DisplayInfo(pCharSet);
}

void CClumpPane::OnBnClickedBtSaveclump()
{
    USES_CONVERSION;

    if(!m_pMTClump)
        return;

    SaveEnvMap();

    CString strMsg;
    if(m_pMTClump->Save((RwChar*)m_pMTClump->GetClumpName()))
    {
        strMsg.Format("[%s]\n Clump Save Complete", (m_pMTClump->GetClumpName()));
        MessageBox(strMsg, NULL, MB_ICONINFORMATION);
    }
    else
    {
        strMsg.Format("[%s]\n Clump Save Fail", (m_pMTClump->GetClumpName()));
        MessageBox(strMsg, NULL, MB_ICONERROR);
    }    
}

void CClumpPane::SetShadowMapSize( CHAR* szAtomicName ) 
{
	USES_CONVERSION;

	RwUInt32 uiSize = RpNtlAtomicGetUserDatU16(m_pMTClump->GetAtomicByName((szAtomicName)));
	if(uiSize <= 0)
	{
		// 값이 설정되어 있지 않으면 디폴트로 256을 설정한다.
		uiSize = 256;
		RpNtlAtomicSetUserDatU16(m_pMTClump->GetAtomicByName((szAtomicName)), (RwUInt16)uiSize);
	}

	CString strSize;
	strSize.Format("%d", uiSize);					
	m_cbShadowMapResolution.SelectString(-1, strSize);
}

void CClumpPane::OnEnChangeEdFrustum()
{
	USES_CONVERSION;

	if(!m_pMTClump)
		return;

	CString strFrustumScale;
	m_EdFrustum.GetWindowText(strFrustumScale);
	RwReal fFrustumScale = (RwReal)atof(strFrustumScale);	
	int nCount = m_listAtomic.GetSelCount();
	if(nCount > 0)
	{
		int* arrIndex = new int[nCount];
		m_listAtomic.GetSelItems(nCount, arrIndex);
		for(int i = 0; i < nCount; i++)
		{
			CHAR szAtomicName[128] = {0,};			
			m_listAtomic.GetText(arrIndex[i], szAtomicName); 						
			RpNtlAtomicSetUserDatReal(m_pMTClump->GetAtomicByName((szAtomicName)), fFrustumScale);			
		}	
		delete[] arrIndex;
	}  
		
}

void CClumpPane::OnEnChangeEditEnvmap()
{
    
}

void CClumpPane::OnCbnSelchangeCbEnv()
{

}

void CClumpPane::SetEnvMap()
{
    USES_CONVERSION;

    RpAtomic* pAtomic = Helper_GetEnvMapAtomic(m_pMTClump->GetClump());
    if(!pAtomic)
    {
        m_edEnvMap.SetWindowText("");
        return;
    }

    RpUserDataArray *pUserData = RpGeometryGetUserDataArray(RpAtomicGetGeometry(pAtomic), 0);
    char *chBuffer = RpUserDataArrayGetString(pUserData, 0);    
    m_cbEnv.SelectString(-1, (chBuffer));
    m_cbEnv.EnableWindow(TRUE);

    RwUInt16 uiTexName = RpNtlAtomicGetEnvTexName(pAtomic);
    CString strTexName;
    strTexName.Format("%d", uiTexName);
    m_edEnvMap.SetWindowText(strTexName);    
    m_edEnvMap.EnableWindow(TRUE);

    CMTCharacter* pCharacter = CModelToolApplication::GetInstance()->GetEditChar();
    if(pCharacter)
    {
        ::SetCurrentDirectoryA(CModelToolApplication::GetInstance()->GetWorkDir());
        RwChar* szEnvMap = uiTexName > 0 ? (LPSTR)(LPCSTR)(strTexName) : NULL;
        pCharacter->SetEnvMap(szEnvMap);        
    }        
}

void CClumpPane::SaveEnvMap() 
{
    USES_CONVERSION;

    CString strEnvAtomicName;
    m_cbEnv.GetWindowText(strEnvAtomicName);

    CString strPath, strAtomicName, strEnvTex;
    m_edEnvMap.GetWindowText(strPath);
    m_cbEnv.GetWindowText(strAtomicName);    
    if(strAtomicName.GetLength() <= 0)
        return;

    if(strPath.Find(".dds") > 0)
    {
        CHAR szDrive[256] = {0,};
        CHAR szDir[256] = {0,};
        CHAR szFileName[256] = {0,};
        CHAR szExt[256] = {0,};

        SPLITPATH_S(strPath, szDrive, szDir, szFileName, szExt);
        strEnvTex = szFileName;
    }
    else
    {
        strEnvTex = strPath;
    }
    
    RwUInt16 uiTexture = (RwUInt16)atoi(strEnvTex);
    if(uiTexture > 0)
    {
        // 기존에 설정된게 있으면 지워야 한다.
        RpAtomic* pAtomic = Helper_GetEnvMapAtomic(m_pMTClump->GetClump());
        if(pAtomic)
            m_pMTClump->SetAtomicFlag(NTL_ENVMAP_TEX, pAtomic, FALSE);
        
        RpAtomic* pAtomicNew = m_pMTClump->GetAtomicByName((LPSTR)(LPCSTR)(strAtomicName));
        m_pMTClump->SetAtomicFlag(NTL_ENVMAP_TEX, pAtomicNew, TRUE);
        RpNtlAtomicSetEnvTexName(pAtomicNew, uiTexture);        
    }  

    SetEnvMap();
}


