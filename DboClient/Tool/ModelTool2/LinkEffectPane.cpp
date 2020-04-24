// LinkEffectPane.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "NtlPLPropertyContainer.h"
#include "PropertyPane.h"
#include "NtlPLLinkEffect.h"
#include "LinkEffectPane.h"


// CLinkEffectPane

IMPLEMENT_DYNCREATE(CLinkEffectPane, CXTResizeFormView)

CLinkEffectPane* CLinkEffectPane::m_pInstance = NULL;

CLinkEffectPane::CLinkEffectPane()
	: CXTResizeFormView(CLinkEffectPane::IDD)
{
    m_pCharacter = NULL;
    m_pItem      = NULL;
    m_pObject    = NULL;
    m_pClump     = NULL;
    m_ePrevMode  = MT_MODE_NONE;

    m_pInstance = this;
}

CLinkEffectPane::~CLinkEffectPane()
{


}

void CLinkEffectPane::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CB_EFFECT, m_cbEffect);
    DDX_Control(pDX, IDC_LIST_EFFECT, m_lbEffect);
}

BEGIN_MESSAGE_MAP(CLinkEffectPane, CXTResizeFormView)
    ON_CBN_SELCHANGE(IDC_CB_EFFECT, &CLinkEffectPane::OnCbnSelchangeCbEffect)
    ON_LBN_SELCHANGE(IDC_LIST_EFFECT, &CLinkEffectPane::OnLbnSelchangeListEffect)
    ON_WM_KEYDOWN()    
    ON_WM_DESTROY()
END_MESSAGE_MAP()


// CLinkEffectPane diagnostics

#ifdef _DEBUG
void CLinkEffectPane::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLinkEffectPane::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLinkEffectPane message handlers

void CLinkEffectPane::OnInitialUpdate()
{
    CXTResizeFormView::OnInitialUpdate();

    SetResize(IDC_LIST_EFFECT, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
    SetResize(IDC_CB_EFFECT, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
}

void CLinkEffectPane::InitEffectList() 
{
    if(CModelToolApplication::GetInstance()->IsNotChangeMode() && m_cbEffect.GetCount())    
        return;

    // OnInitialUpdate시에 부르면 Map에 아직 내용이 없다. 초기화 후에 불러야 한다.
    USES_CONVERSION;

    m_cbEffect.ResetContent();

    // 파서로부터 데이터를 가져와서 설정한다.
    CNtlPLPropertyContainer::MapProp mapEffectProperty = CNtlPLPropertyContainer::GetInstance()->Gets(PLENTITY_EFFECT);
    CNtlPLPropertyContainer::MapProp::iterator it = mapEffectProperty.begin();
    for(; it != mapEffectProperty.end(); ++it)
    {
        std::string strEffectName = it->first;
        CString sEffectName = (strEffectName.c_str());

        if(CModelToolApplication::GetInstance()->GetAppMode() == MT_MODE_OBJECT)
        {
            if(MTLogic_IsObjectEffect((RwChar*)strEffectName.c_str()))
            {
                m_cbEffect.AddString(sEffectName);
            }
        }
        else
        {
            m_cbEffect.AddString(sEffectName);
        }
    }

    m_ePrevMode = CModelToolApplication::GetInstance()->GetAppMode();
}

void CLinkEffectPane::SetEnable( BOOL bEnable ) 
{
    m_lbEffect.EnableWindow(bEnable);

    m_cbEffect.EnableWindow(bEnable);

    if(bEnable)
    {
        InitEffectList();
    }    
}

void CLinkEffectPane::SetCharacter( CMTCharacter* pCharacter ) 
{
    USES_CONVERSION;

    if(m_pCharacter == pCharacter)
        return;

    m_pCharacter = pCharacter;
    m_pItem = NULL;
    m_pObject = NULL;
    m_pClump = NULL;
    ClearListItem();

    if(!pCharacter)
    {
        SetEnable(FALSE);
    }
    else
    {
        m_pClump = pCharacter->GetClump();

        // ListBox 내용을 Update한다.
        for(UINT i = 0; i < pCharacter->GetProperty()->m_vLinkEffect.size(); ++i)
        {
            SEventLinkEffect* pEventEffect = pCharacter->GetProperty()->m_vLinkEffect[i];
            if(pEventEffect)
            {
                int nIndex = m_lbEffect.AddString((pEventEffect->chEffectName));
                SLinkEffectItemData* pItemData = new SLinkEffectItemData();
                VecLinkEffect* vEffectInstance = pCharacter->GetVecLinkEffect();
                pItemData->pEffectInstance = (*vEffectInstance)[i];
                pItemData->pEventLinkEffect = pEventEffect;

                m_lbEffect.SetItemData(nIndex, (DWORD_PTR)pItemData);
            }
        }
        SetEnable(TRUE);
    }
}

void CLinkEffectPane::SetItem( CMTItem* pItem ) 
{
    USES_CONVERSION;

    if(m_pItem == pItem)
        return;

    m_pItem = pItem;
    m_pCharacter = NULL;
    m_pObject = NULL;
    m_pClump = NULL;
    ClearListItem();

    if(!pItem)
    {
        SetEnable(FALSE);
    }
    else
    {
        m_pClump = pItem->GetClumpInfo()->GetClump();

        // ListBox 내용을 Update한다.
        for(UINT i = 0; i < pItem->GetProperty()->m_vLinkEffect.size(); ++i)
        {
            SEventLinkEffect* pEventEffect = pItem->GetProperty()->m_vLinkEffect[i];
            if(pEventEffect)
            {
                int nIndex = m_lbEffect.AddString((pEventEffect->chEffectName));
                SLinkEffectItemData* pItemData = new SLinkEffectItemData();
                VecLinkEffect* vEffectInstance = pItem->GetVecLinkEffect();
                pItemData->pEffectInstance = (*vEffectInstance)[i];
                pItemData->pEventLinkEffect = pEventEffect;

                m_lbEffect.SetItemData(nIndex, (DWORD_PTR)pItemData);
            }
        }

        SetEnable(TRUE);
    }
}

void CLinkEffectPane::SetObject( CMTObject* pObject ) 
{
    USES_CONVERSION;

    if(m_pObject == pObject)
        return;

    m_pObject = pObject;
    m_pCharacter = NULL;
    m_pItem = NULL;
    m_pClump = NULL;
    ClearListItem();

    if(!pObject)
    {
        SetEnable(FALSE);
    }
    else
    {
        m_pClump = pObject->GetClump();
        
        // ListBox 내용을 Update한다.
        for(UINT i = 0; i < pObject->GetProperty()->m_vLinkEffect.size(); ++i)
        {
            SEventLinkEffect* pEventEffect = pObject->GetProperty()->m_vLinkEffect[i];
            if(pEventEffect)
            {
                int nIndex = m_lbEffect.AddString((pEventEffect->chEffectName));
                SLinkEffectItemData* pItemData = new SLinkEffectItemData();
                VecLinkEffect* vEffectInstance = pObject->GetVecLinkEffect();
                pItemData->pEffectInstance = (*vEffectInstance)[i];
                pItemData->pEventLinkEffect = pEventEffect;

                m_lbEffect.SetItemData(nIndex, (DWORD_PTR)pItemData);
            }
        }
        SetEnable(TRUE);
    }
}


void CLinkEffectPane::OnCbnSelchangeCbEffect()
{
    int nSelIndex = m_cbEffect.GetCurSel();
    CString strEffect;
    m_cbEffect.GetLBText(nSelIndex, strEffect);

    AddLinkEffect(strEffect);    
}

/**
 * Link Effect를 추가한다.
 * \param streffectName 추가할 Effect의 이름
 * return 추가된 Link Effect의 포인터, 실패하면 NULL을 반환한다.
 */
SEventLinkEffect* CLinkEffectPane::AddLinkEffect( CString streffectName ) 
{
    if(!m_pCharacter && !m_pObject && !m_pItem)
        return NULL;
    
    USES_CONVERSION;

    SEventLinkEffect* pEventLinkEffect = new SEventLinkEffect();
    sprintf_s(pEventLinkEffect->chEffectName, "%s", (streffectName));

    CNtlInstanceEffect* pEffect = AttachEffect(pEventLinkEffect);
    if(!pEffect)
    {
        NTL_DELETE(pEventLinkEffect);
        return pEventLinkEffect;
    }

    // 리스트 박스에 추가한다.
    SLinkEffectItemData* pItemData = new SLinkEffectItemData();
    pItemData->pEffectInstance = pEffect;
    pItemData->pEventLinkEffect = pEventLinkEffect;

    int nIndex = m_lbEffect.AddString(streffectName);
    m_lbEffect.SetItemData(nIndex, (DWORD_PTR)pItemData);
    m_lbEffect.SetCurSel(nIndex);

    // 프로퍼티에 추가한다.
    if(m_pCharacter)
    {
        m_pCharacter->GetProperty()->m_vLinkEffect.push_back(pEventLinkEffect);
    }
    else if(m_pObject)
    {
        m_pObject->GetProperty()->m_vLinkEffect.push_back(pEventLinkEffect);
    }
    else if(m_pItem)
    {
        m_pItem->GetProperty()->m_vLinkEffect.push_back(pEventLinkEffect);
    }

    OnLbnSelchangeListEffect();

    return pEventLinkEffect;
}

void CLinkEffectPane::OnLbnSelchangeListEffect()
{
    if(!m_pClump)
        return;

    // 리스트 박스에서 항목을 선택하면, Property Pane에 데이터를 설정한다.
    int nIndex = m_lbEffect.GetCurSel();
    if(nIndex < 0)
        return;

    SLinkEffectItemData* pLinkEffectItemData = (SLinkEffectItemData*)m_lbEffect.GetItemData(nIndex);
    if(!pLinkEffectItemData)
        return;

    GetSafeInstance(CPropertyPane)->SetEvent(pLinkEffectItemData->pEventLinkEffect, m_pClump);
}

/**
 * LinkEffect를 생성하여 부착한다.
 * \param pEventLinkEffect 생성할 Effect의 프로퍼티
 * return 생성된 Effect객체. 실패시에는 NULL을 반환
 */
CNtlInstanceEffect* CLinkEffectPane::AttachEffect(SEventLinkEffect* pEventLinkEffect)
{
    USES_CONVERSION;

    if(!pEventLinkEffect)
        return NULL;

    CNtlInstanceEffect* pEffectInstance = NULL;

    if(m_pCharacter)
    {
        pEffectInstance = m_pCharacter->AttachLinkEffect(pEventLinkEffect);
    }
    else if(m_pObject)
    {
        pEffectInstance = m_pObject->AttachLinkEffect(pEventLinkEffect);
    }
    else if(m_pItem)
    {
        pEffectInstance = m_pItem->AttachLinkEffect(pEventLinkEffect);
    }
    else
    {
        return FALSE;
    }

    if(!pEffectInstance)
    {
        CString strMsg;
        strMsg.Format("[%s] Link Effect Can't Use AutoDelete Effect", (pEventLinkEffect->chEffectName));
        MessageBox(strMsg,NULL, MB_ICONERROR);
    }    
    
    return pEffectInstance;
}

void CLinkEffectPane::DeleteEffect(SLinkEffectItemData* pLinkEffectItemData)
{
    if(!pLinkEffectItemData)
        return;
    
    if(m_pCharacter)
    {
        std::vector<SEventLinkEffect*>::iterator it = m_pCharacter->GetProperty()->m_vLinkEffect.begin();
        for(;it != m_pCharacter->GetProperty()->m_vLinkEffect.end(); ++it)
        {
            SEventLinkEffect* pEffect = *it;
            if(pEffect == pLinkEffectItemData->pEventLinkEffect)
            {
                m_pCharacter->GetProperty()->m_vLinkEffect.erase(it);
                break;
            }
        }

        if(pLinkEffectItemData->pEffectInstance)
            m_pCharacter->DetachLinkEffect(pLinkEffectItemData->pEffectInstance);            
    }
    else if(m_pItem)
    {
        std::vector<SEventLinkEffect*>::iterator it = m_pItem->GetProperty()->m_vLinkEffect.begin();
        for(;it != m_pItem->GetProperty()->m_vLinkEffect.end(); ++it)
        {
            SEventLinkEffect* pEffect = *it;
            if(pEffect == pLinkEffectItemData->pEventLinkEffect)
            {
                m_pItem->GetProperty()->m_vLinkEffect.erase(it);
                break;
            }
        }
        
        if(pLinkEffectItemData->pEventLinkEffect)
            m_pItem->DetachLinkEffect(pLinkEffectItemData->pEffectInstance);            
    }
    else if(m_pObject)
    {
        std::vector<SEventLinkEffect*>::iterator it = m_pObject->GetProperty()->m_vLinkEffect.begin();
        for(;it != m_pObject->GetProperty()->m_vLinkEffect.end(); ++it)
        {
            SEventLinkEffect* pEffect = *it;
            if(pEffect == pLinkEffectItemData->pEventLinkEffect)
            {
                m_pObject->GetProperty()->m_vLinkEffect.erase(it);
                break;
            }
        }

        if(pLinkEffectItemData->pEffectInstance)
            m_pObject->DetachLinkEffect(pLinkEffectItemData->pEffectInstance);            
    }

    NTL_DELETE(pLinkEffectItemData->pEventLinkEffect);
}

BOOL CLinkEffectPane::PreTranslateMessage(MSG* pMsg)
{
    if(GetFocus() == &m_lbEffect && pMsg->message == WM_KEYDOWN)
    {
       if(pMsg->wParam == VK_DELETE)
        {
            int nIndex = m_lbEffect.GetCurSel();
            if(nIndex >= 0)
            {
                SLinkEffectItemData* pEffect = (SLinkEffectItemData*)m_lbEffect.GetItemData(nIndex);
                DeleteEffect(pEffect);
                NTL_DELETE(pEffect);

                m_lbEffect.DeleteString(nIndex);
            }
            
        }
    }
    
    return CXTResizeFormView::PreTranslateMessage(pMsg);
}

RwBool CLinkEffectPane::UpdateEffect(SEventLinkEffect* pEventLinkEffect)
{
    if(!pEventLinkEffect)
        return FALSE;

    int nIndex = m_lbEffect.GetCurSel();
    SLinkEffectItemData* pItemData = (SLinkEffectItemData*)m_lbEffect.GetItemData(nIndex);
    if(pItemData && pItemData->pEventLinkEffect == pEventLinkEffect)
    {
        CNtlInstanceEffect* pNewEffect = NULL;
        if(m_pCharacter)
        {
            m_pCharacter->DetachLinkEffect(pItemData->pEffectInstance);
            pNewEffect = m_pCharacter->AttachLinkEffect(pEventLinkEffect);
        }
        else if(m_pObject)
        {
            m_pObject->DetachLinkEffect(pItemData->pEffectInstance);
            pNewEffect = m_pObject->AttachLinkEffect(pEventLinkEffect);
        }
        else if(m_pItem)
        {
            m_pItem->DetachLinkEffect(pItemData->pEffectInstance);
            pNewEffect = m_pItem->AttachLinkEffect(pEventLinkEffect);
        }
        else
        {
            return FALSE;
        }
        
        pItemData->pEffectInstance = pNewEffect;
        return TRUE;        
    }

    return FALSE;
}

void CLinkEffectPane::ClearListItem()
{
    for(int i = 0; i < m_lbEffect.GetCount(); ++i)
    {
        SLinkEffectItemData* pItemData = (SLinkEffectItemData*)m_lbEffect.GetItemData(i);
        if(pItemData)
        {
            NTL_DELETE(pItemData);
        }
    }

    m_lbEffect.ResetContent();
}
void CLinkEffectPane::OnDestroy()
{
    ClearListItem();

    CXTResizeFormView::OnDestroy();
}
