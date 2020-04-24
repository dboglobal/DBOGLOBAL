// DummyModelPane.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "DummyModelPane.h"


// CDummyModelPane

IMPLEMENT_DYNCREATE(CDummyModelPane, CXTResizeFormView)

CDummyModelPane::CDummyModelPane()
	: CXTResizeFormView(CDummyModelPane::IDD)
{
    m_pCharacter = NULL;
    m_fSensitive = 0.05f;
}

CDummyModelPane::~CDummyModelPane()
{
    if(m_pCharacter)
    {
        m_pCharacter->Destroy();
        delete m_pCharacter;
        m_pCharacter= NULL;
    }
}

void CDummyModelPane::DoDataExchange(CDataExchange* pDX)
{
    CXTResizeFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_FILENAME, m_edClumpName);
    DDX_Control(pDX, IDC_CHECK_VISIBLE, m_ckVisible);
    DDX_Control(pDX, IDC_EDIT_POSX, m_edPosX);
    DDX_Control(pDX, IDC_EDIT_POSY, m_edPosY);
    DDX_Control(pDX, IDC_SPIN1, m_spPosX);
    DDX_Control(pDX, IDC_SPIN2, m_spPosZ);
}

BEGIN_MESSAGE_MAP(CDummyModelPane, CXTResizeFormView)
    ON_EN_CHANGE(IDC_EDIT_FILENAME, &CDummyModelPane::OnEnChangeEditFilename)
    ON_BN_CLICKED(IDC_CHECK_VISIBLE, &CDummyModelPane::OnBnClickedCheckVisible)
    ON_EN_CHANGE(IDC_EDIT_POSX, &CDummyModelPane::OnEnChangeEditPosx)
    ON_EN_CHANGE(IDC_EDIT_POSY, &CDummyModelPane::OnEnChangeEditPosy)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CDummyModelPane::OnDeltaposSpin1)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CDummyModelPane::OnDeltaposSpin2)
END_MESSAGE_MAP()


// CDummyModelPane diagnostics

#ifdef _DEBUG
void CDummyModelPane::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDummyModelPane::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDummyModelPane message handlers

void CDummyModelPane::OnInitialUpdate()
{
    CXTResizeFormView::OnInitialUpdate();

    m_edClumpName.Initialize(this, BES_XT_CHOOSEFILE);
    CHAR szOpenFilter[] = "Clump File (*.dff)|*.dff||";
    m_edClumpName.SetDlgFilter(szOpenFilter);
}

void CDummyModelPane::OnEnChangeEditFilename()
{
    USES_CONVERSION;

    CString strFileName;
    m_edClumpName.GetWindowText(strFileName);

    if(m_pCharacter)
    {
        m_pCharacter->LoadClump((LPSTR)(LPCSTR)(strFileName));
    }
    else
    {
        m_pCharacter = new CMTCharacter();
        m_pCharacter->Create();
        if(m_pCharacter->LoadClump((LPSTR)(LPCSTR)(strFileName)))
        {
            RwV3d vPos;
            vPos.x = 1.5f;
            vPos.y = 0.0f;
            vPos.z = 0.0f;

            m_pCharacter->SetPosition(&vPos);
            
            CString strPosX, strPosZ;
            strPosX.Format("%.2f", vPos.x);
            strPosZ.Format("%.2f", vPos.z);

            m_edPosX.SetWindowText(strPosX);
            m_edPosY.SetWindowText(strPosZ);
        }

        m_ckVisible.EnableWindow(TRUE);
        m_edPosX.EnableWindow(TRUE);
        m_edPosY.EnableWindow(TRUE);
    }

    m_ckVisible.SetCheck(TRUE);
}

void CDummyModelPane::OnBnClickedCheckVisible()
{
    BOOL bCheck = m_ckVisible.GetCheck();
    if(m_pCharacter)
    {
        m_pCharacter->SetVisible(bCheck);
    }
}

void CDummyModelPane::OnEnChangeEditPosx()
{
    if(!m_pCharacter)
        return;

    CString strValue;
    m_edPosX.GetWindowText(strValue);
    RwReal fPosX = (RwReal)atof(strValue);

    RwV3d vPos = m_pCharacter->GetPosition();
    vPos.x = fPosX;

    m_pCharacter->SetPosition(&vPos);
}

void CDummyModelPane::OnEnChangeEditPosy()
{
    if(!m_pCharacter)
        return;

    CString strValue;
    m_edPosY.GetWindowText(strValue);
    RwReal fPosZ = (RwReal)atof(strValue);

    RwV3d vPos = m_pCharacter->GetPosition();
    vPos.z = fPosZ;

    m_pCharacter->SetPosition(&vPos);
    
}

void CDummyModelPane::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    CString strPosX;
    RwReal  fPosX;
    m_edPosX.GetWindowText(strPosX);
    fPosX = (RwReal)atof(strPosX);

    if(pNMUpDown->iDelta < 0)
    {
        fPosX += m_fSensitive;
    }
    else
    {
        fPosX -= m_fSensitive;
    }

    strPosX.Format("%.2f", fPosX);
    m_edPosX.SetWindowText(strPosX);
    
    *pResult = 0;
}

void CDummyModelPane::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    CString strPosZ;
    RwReal  fPosZ;
    m_edPosY.GetWindowText(strPosZ);
    fPosZ = (RwReal)atof(strPosZ);

    if(pNMUpDown->iDelta < 0)
    {
        fPosZ += m_fSensitive;
    }
    else
    {
        fPosZ -= m_fSensitive;
    }

    strPosZ.Format("%.2f", fPosZ);
    m_edPosY.SetWindowText(strPosZ);

    *pResult = 0;
}
