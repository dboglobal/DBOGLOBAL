// AttackTypePane.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "PropertyPane.h"
#include "AttackTypePane.h"


CAttackTypePane* CAttackTypePane::m_pInstance = NULL;

// CAttackTypePane

IMPLEMENT_DYNCREATE(CAttackTypePane, CFormView)

CAttackTypePane::CAttackTypePane()
	: CFormView(CAttackTypePane::IDD)
{
    m_pInstance = this;

    for(int i = 0; i < MAX_HIT_EVENT_COUNT; ++i)
    {
        m_vEventAnimHit[i] = NULL;
    }
}

CAttackTypePane::~CAttackTypePane()
{
}

void CAttackTypePane::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_RA_PHYSICAL, m_raPhysical);
    DDX_Control(pDX, IDC_RA_ENERGY, m_raEnergy); 
    DDX_Control(pDX, IDC_ED_DAMAGE00, m_edDamage[0]);
    DDX_Control(pDX, IDC_ED_DAMAGE01, m_edDamage[1]);
    DDX_Control(pDX, IDC_ED_DAMAGE02, m_edDamage[2]);
    DDX_Control(pDX, IDC_ED_DAMAGE03, m_edDamage[3]);
    DDX_Control(pDX, IDC_ED_DAMAGE04, m_edDamage[4]);
    DDX_Control(pDX, IDC_ED_DAMAGE05, m_edDamage[5]);
    DDX_Control(pDX, IDC_ED_DAMAGE06, m_edDamage[6]);
    DDX_Control(pDX, IDC_ED_DAMAGE07, m_edDamage[7]);      
    DDX_Control(pDX, IDC_ED_DAMAGE08, m_edDamage[8]);   
    DDX_Control(pDX, IDC_ED_DAMAGE09, m_edDamage[9]);   
    DDX_Control(pDX, IDC_ED_DAMAGE10, m_edDamage[10]);   
    DDX_Control(pDX, IDC_ED_DAMAGE11, m_edDamage[11]);   
    DDX_Control(pDX, IDC_ED_DAMAGE12, m_edDamage[12]);   
    DDX_Control(pDX, IDC_ED_DAMAGE13, m_edDamage[13]);   
    DDX_Control(pDX, IDC_ED_DAMAGE14, m_edDamage[14]);   
    DDX_Control(pDX, IDC_ED_DAMAGE15, m_edDamage[15]);  
}

BEGIN_MESSAGE_MAP(CAttackTypePane, CFormView)
    ON_BN_CLICKED(IDC_RA_PHYSICAL, &CAttackTypePane::OnBnClickedRaPhysical)
    ON_BN_CLICKED(IDC_RA_ENERGY, &CAttackTypePane::OnBnClickedRaEnergy)
    ON_EN_CHANGE(IDC_ED_DAMAGE00, &CAttackTypePane::OnEnChangeEdDamage00)
    ON_EN_CHANGE(IDC_ED_DAMAGE01, &CAttackTypePane::OnEnChangeEdDamage01)
    ON_EN_CHANGE(IDC_ED_DAMAGE02, &CAttackTypePane::OnEnChangeEdDamage02)
    ON_EN_CHANGE(IDC_ED_DAMAGE03, &CAttackTypePane::OnEnChangeEdDamage03)
    ON_EN_CHANGE(IDC_ED_DAMAGE04, &CAttackTypePane::OnEnChangeEdDamage04)
    ON_EN_CHANGE(IDC_ED_DAMAGE05, &CAttackTypePane::OnEnChangeEdDamage05)
    ON_EN_CHANGE(IDC_ED_DAMAGE06, &CAttackTypePane::OnEnChangeEdDamage06)
    ON_EN_CHANGE(IDC_ED_DAMAGE07, &CAttackTypePane::OnEnChangeEdDamage07)
    ON_EN_CHANGE(IDC_ED_DAMAGE08, &CAttackTypePane::OnEnChangeEdDamage08)
    ON_EN_CHANGE(IDC_ED_DAMAGE09, &CAttackTypePane::OnEnChangeEdDamage09)
    ON_EN_CHANGE(IDC_ED_DAMAGE10, &CAttackTypePane::OnEnChangeEdDamage10)
    ON_EN_CHANGE(IDC_ED_DAMAGE11, &CAttackTypePane::OnEnChangeEdDamage11)
    ON_EN_CHANGE(IDC_ED_DAMAGE12, &CAttackTypePane::OnEnChangeEdDamage12)
    ON_EN_CHANGE(IDC_ED_DAMAGE13, &CAttackTypePane::OnEnChangeEdDamage13)
    ON_EN_CHANGE(IDC_ED_DAMAGE14, &CAttackTypePane::OnEnChangeEdDamage14)
    ON_EN_CHANGE(IDC_ED_DAMAGE15, &CAttackTypePane::OnEnChangeEdDamage15)
END_MESSAGE_MAP()


// CAttackTypePane diagnostics

#ifdef _DEBUG
void CAttackTypePane::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAttackTypePane::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAttackTypePane message handlers
void CAttackTypePane::SetAnimEventData(STypeAnimData* pAnimData)
{
    if(!pAnimData)
    {
        SetEnable(FALSE);                
    }
    else
    {
        m_raEnergy.EnableWindow(TRUE);
        m_raPhysical.EnableWindow(TRUE);

        pAnimData->AnimEventSort();

        for(int i = 0; i < MAX_HIT_EVENT_COUNT; ++i)
        {
            m_vEventAnimHit[i] = NULL;       
            m_edDamage[i].SetWindowText("");
            m_edDamage[i].EnableWindow(FALSE);            
        }

        int nCount = 0;
        for(UINT i = 0; i < pAnimData->vecAnimEvent.size(); ++i)
        {
            if(pAnimData->vecAnimEvent[i]->eEventID == EVENT_ANIM_HIT && pAnimData->vecAnimEvent[i]->fTime > 0.0f)
            {
                m_vEventAnimHit[nCount] = (SEventAnimHit*)pAnimData->vecAnimEvent[i];
                if(m_vEventAnimHit[nCount])
                {
                    CString sDamage;
                    sDamage.Format("%d", m_vEventAnimHit[nCount]->uiDamage);
                    m_edDamage[nCount].SetWindowText(sDamage);
                    m_edDamage[nCount].EnableWindow(TRUE);

                    if(m_vEventAnimHit[nCount]->eAttackType == ATTACK_TYPE_PHYSICAL)
                    {
                        m_raEnergy.SetCheck(FALSE);
                        m_raPhysical.SetCheck(TRUE);
                        
                    }
                    else
                    {
                        m_raEnergy.SetCheck(TRUE);
                        m_raPhysical.SetCheck(FALSE);                        
                    }
                }

                ++nCount;
            }        
        }
        
    }
}

void CAttackTypePane::SetEnable(BOOL bEnable)
{
    m_raEnergy.EnableWindow(bEnable);
    m_raPhysical.EnableWindow(bEnable);
    for(int i = 0; i < MAX_HIT_EVENT_COUNT; ++i)
    {
        m_vEventAnimHit[i] = NULL;      
        m_edDamage[i].SetWindowText("");
        m_edDamage[i].EnableWindow(bEnable);
    }
}
void CAttackTypePane::OnBnClickedRaPhysical()
{
    if(m_vEventAnimHit[0])
    {
        if(m_vEventAnimHit[0]->eAttackType == ATTACK_TYPE_PHYSICAL)
            return;
    }

    for(int i = 0; i < MAX_HIT_EVENT_COUNT; ++i)
    {
        if(m_vEventAnimHit[i])
        {
            m_vEventAnimHit[i]->eAttackType = ATTACK_TYPE_PHYSICAL;
        }
        
    }

    GetSafeInstance(CPropertyPane)->SetAttackType(ATTACK_TYPE_PHYSICAL);

    CModelToolApplication::GetInstance()->SetDataChanged();
}

void CAttackTypePane::OnBnClickedRaEnergy()
{
    if(m_vEventAnimHit[0])
    {
        if(m_vEventAnimHit[0]->eAttackType == ATTACK_TYPE_ENERGY)
            return;
    }

    for(int i = 0; i < MAX_HIT_EVENT_COUNT; ++i)
    {
        if(m_vEventAnimHit[i])
        {
            m_vEventAnimHit[i]->eAttackType = ATTACK_TYPE_ENERGY;
        }        
    }

    GetSafeInstance(CPropertyPane)->SetAttackType(ATTACK_TYPE_ENERGY);

    CModelToolApplication::GetInstance()->SetDataChanged();
}

void CAttackTypePane::ChangeDamage(int nIndex)
{
    CString sDamage;

    if(!m_vEventAnimHit[nIndex])
        return;

    // 총합이 100이 넘으면 에러를 출력하고 원래값으로 되돌린다.
    int nTotalDamage = 0;
    for(int i = 0; i < MAX_HIT_EVENT_COUNT; ++i)
    {
        if(m_edDamage[i])
        {
            m_edDamage[i].GetWindowText(sDamage);
            nTotalDamage += atoi(sDamage);
        }
    }

    if(nTotalDamage <= 100)
    {
        m_edDamage[nIndex].GetWindowText(sDamage);        
        RwUInt32 uiDamage = (RwUInt32)(atoi(sDamage));        
        if(m_vEventAnimHit[nIndex]->uiDamage != uiDamage)
        {
            m_vEventAnimHit[nIndex]->uiDamage = uiDamage;
            CModelToolApplication::GetInstance()->SetDataChanged();
        }
    }   
    else    // 100보다 크면
    {
        //sDamage.Format(L"%d", m_vEventAnimHit[nIndex]->uiDamage);
        //m_edDamage[nIndex].SetWindowText(sDamage);
        MessageBox("Total Damage is Over then 100");
    }
}

void CAttackTypePane::OnEnChangeEdDamage00()
{
    ChangeDamage(0);
}

void CAttackTypePane::OnEnChangeEdDamage01()
{
    ChangeDamage(1);
}

void CAttackTypePane::OnEnChangeEdDamage02()
{
    ChangeDamage(2);
}

void CAttackTypePane::OnEnChangeEdDamage03()
{
    ChangeDamage(3);
}

void CAttackTypePane::OnEnChangeEdDamage04()
{
    ChangeDamage(4);
}

void CAttackTypePane::OnEnChangeEdDamage05()
{
    ChangeDamage(5);
}

void CAttackTypePane::OnEnChangeEdDamage06()
{
    ChangeDamage(6);
}

void CAttackTypePane::OnEnChangeEdDamage07()
{
    ChangeDamage(7);
}

void CAttackTypePane::OnEnChangeEdDamage08()
{
    ChangeDamage(8);
}

void CAttackTypePane::OnEnChangeEdDamage09()
{
    ChangeDamage(9);
}

void CAttackTypePane::OnEnChangeEdDamage10()
{
    ChangeDamage(10);
}

void CAttackTypePane::OnEnChangeEdDamage11()
{
    ChangeDamage(11);
}

void CAttackTypePane::OnEnChangeEdDamage12()
{
    ChangeDamage(12);
}

void CAttackTypePane::OnEnChangeEdDamage13()
{
    ChangeDamage(13);
}

void CAttackTypePane::OnEnChangeEdDamage14()
{
    ChangeDamage(14);
}

void CAttackTypePane::OnEnChangeEdDamage15()
{
    ChangeDamage(15);
}
