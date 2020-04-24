// EnvMapPane.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "ClumpPane.h"
#include "NormEnvMap.h"
#include "ModelToolApplication.h"
#include "EnvMapPane.h"


// CEnvMapPane

IMPLEMENT_DYNCREATE(CEnvMapPane, CFormView)

CEnvMapPane::CEnvMapPane()
	: CFormView(CEnvMapPane::IDD)
{
    m_pEnvMapTexture = NULL;
}

CEnvMapPane::~CEnvMapPane()
{
    if(m_pEnvMapTexture)
    {
        RwTextureDestroy(m_pEnvMapTexture);
        m_pEnvMapTexture = NULL;
    }
}

void CEnvMapPane::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CK_NORM_MAP, m_ckNormalMap);
    DDX_Control(pDX, IDC_CK_ENV_MAP, m_ckEnvMap);
    DDX_Control(pDX, IDC_ED_ENV_MAP, m_edEnvBrowse);
    DDX_Control(pDX, IDC_SL_SHINE, m_slShininess);
}

BEGIN_MESSAGE_MAP(CEnvMapPane, CFormView)
    ON_BN_CLICKED(IDC_CK_ENV_MAP, &CEnvMapPane::OnBnClickedCkEnvMap)
    ON_BN_CLICKED(IDC_CK_NORM_MAP, &CEnvMapPane::OnBnClickedCkNormMap)
    ON_EN_CHANGE(IDC_ED_ENV_MAP, &CEnvMapPane::OnEnChangeEdEnvMap)    
    ON_NOTIFY(NM_CUSTOMDRAW, IDC_SL_SHINE, &CEnvMapPane::OnNMCustomdrawSlShine)
    ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SL_SHINE, &CEnvMapPane::OnNMReleasedcaptureSlShine)
    ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CEnvMapPane diagnostics

#ifdef _DEBUG
void CEnvMapPane::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEnvMapPane::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEnvMapPane message handlers

void CEnvMapPane::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    m_edEnvBrowse.Initialize(this, BES_XT_CHOOSEFILE);    
    m_edEnvBrowse.SetDlgFilter(FILTER_TEXTURE);

    m_slShininess.SetRange(0, 100);
    m_slShininess.SetPos(70);
}

void CEnvMapPane::OnBnClickedCkNormMap()
{
    CMTClump* pMTClump = CClumpPane::GetInstance()->GetEditClump();
    if(!pMTClump)
        return;

    BOOL bCheck = m_ckNormalMap.GetCheck();
    MATERIAL_TABLE_ITER it = pMTClump->m_mapMaterialtable.begin();
    for(; it != pMTClump->m_mapMaterialtable.end(); ++it)
    {
        RpMaterial* pMaterial = it->second;
        if(pMaterial)
        {
            CNormEnvMap::SetNormMap(pMaterial, bCheck);
        }        
    }

    if(!bCheck && !m_ckEnvMap.GetCheck())
    {
        CNormEnvMap::SetEnableNormMap(pMTClump->GetClump(), bCheck);
    }
    else
    {
        CNormEnvMap::SetEnableNormMap(pMTClump->GetClump(), bCheck);
    }
    
}

void CEnvMapPane::OnBnClickedCkEnvMap()
{
    CMTClump* pMTClump = CClumpPane::GetInstance()->GetEditClump();
    if(!pMTClump)
        return;

    BOOL bCheck = m_ckEnvMap.GetCheck();

    m_edEnvBrowse.EnableWindow(bCheck);
    m_slShininess.EnableWindow(bCheck);    

    MATERIAL_TABLE_ITER it = pMTClump->m_mapMaterialtable.begin();
    for(; it != pMTClump->m_mapMaterialtable.end(); ++it)
    {
        RpMaterial* pMaterial = it->second;
        if(pMaterial)
        {
            if(bCheck && m_pEnvMapTexture)
            {
                CNormEnvMap::SetEnvMap(pMaterial, m_pEnvMapTexture);
            }
            else
            {
                CNormEnvMap::SetEnvMap(pMaterial, NULL);
            }            
        }        
    }

    if(!bCheck && !m_ckEnvMap.GetCheck())
    {
        CNormEnvMap::SetEnableNormMap(pMTClump->GetClump(), bCheck);
    }
    else
    {
        CNormEnvMap::SetEnableNormMap(pMTClump->GetClump(), bCheck);
    }
}

void CEnvMapPane::OnEnChangeEdEnvMap()
{
    USES_CONVERSION;

    CMTClump* pMTClump = CClumpPane::GetInstance()->GetEditClump();
    if(!pMTClump)
        return;

    if(m_pEnvMapTexture)
    {
        RwTextureDestroy(m_pEnvMapTexture);
        m_pEnvMapTexture = NULL;
    }

    CString strFilePath;
    m_edEnvBrowse.GetWindowText(strFilePath);
    if(strFilePath != "")
    {
        m_pEnvMapTexture = CNtlPLResourceManager::GetInstance()->LoadTexture((strFilePath), CModelToolApplication::GetInstance()->GetTexturePath());
        if(!m_pEnvMapTexture)
            return;

        OnBnClickedCkEnvMap();
    }    
}

void CEnvMapPane::OnNMCustomdrawSlShine(NMHDR *pNMHDR, LRESULT *pResult)
{
    //LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
    
    *pResult = 0;
}

void CEnvMapPane::OnNMReleasedcaptureSlShine(NMHDR *pNMHDR, LRESULT *pResult)
{

    *pResult = 0;
}

void CEnvMapPane::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    if(!m_pEnvMapTexture)
        return;

    CMTClump* pMTClump = CClumpPane::GetInstance()->GetEditClump();
    if(!pMTClump)
        return;

    float fShininess = (float)m_slShininess.GetPos() / (float)m_slShininess.GetRangeMax();

    MATERIAL_TABLE_ITER it = pMTClump->m_mapMaterialtable.begin();
    for(; it != pMTClump->m_mapMaterialtable.end(); ++it)
    {
        RpMaterial* pMaterial = it->second;
        if(pMaterial)
        {
            CNormEnvMap::SetEnvMapShininess(pMaterial, fShininess);            
        }        
    }

    CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}
