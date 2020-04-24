// AnimAttrView.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool.h"
#include "AnimAttrView.h"
#include "AnimToolView.h"


CAnimAttrView* CAnimAttrView::m_pInstance = NULL;
// CAnimAttrView

IMPLEMENT_DYNCREATE(CAnimAttrView, CFormView)

CAnimAttrView::CAnimAttrView()
	: CFormView(CAnimAttrView::IDD)
{
    m_pInstance = this;

    m_pCharacter            = NULL;
    m_pEventAnim            = NULL;
    m_pPropHitTime          = NULL;
    m_pHitPropRoot          = NULL;
    m_pPropTargetBehavior   = NULL;
    m_pPropAttackPower      = NULL;
    m_pPropHandType         = NULL;
    m_pPropBone             = NULL;
    m_pPropEffectType       = NULL;
    m_pPropLuaID            = NULL;
    m_pSoundPropRoot        = NULL;
    m_pPropSoundTime        = NULL;
    m_pPropSoundFile        = NULL;
}

CAnimAttrView::~CAnimAttrView()
{
}

void CAnimAttrView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_PROPERTY_FRAME, m_staticFrame);
}

BEGIN_MESSAGE_MAP(CAnimAttrView, CFormView)
    ON_WM_SIZE()   
    ON_NOTIFY(PTN_ITEMCHANGED, IDC_PROPERTY_FRAME, OnItemChanged)
END_MESSAGE_MAP()


// CAnimAttrView diagnostics

#ifdef _DEBUG
void CAnimAttrView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAnimAttrView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAnimAttrView message handlers

void CAnimAttrView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    // 프로퍼티 윈도우 초기화
    DWORD dwStyle;
    CRect rc;
    dwStyle = WS_CHILD|WS_VISIBLE|PTS_NOTIFY;

    GetClientRect(rc);

    m_HitPropertyTree.Create(dwStyle, rc, this, IDC_PROPERTY_FRAME);
    m_SoundProperyTree.Create(dwStyle, rc, this, IDC_PROPERTY_FRAME);

    InitHitPropertyItem();      // Hit Event Property 관련 초기화
    InitSoundPropertyItem();    // Sound Event Property 관련 초기화    

    SetPropertyMode(MODE_NONE);
}

void CAnimAttrView::InitHitPropertyItem()
{
    m_pHitPropRoot = m_HitPropertyTree.InsertItem(new CPropTreeItem());
    m_pHitPropRoot->SetLabelText(L"Hit Event Properties");

    // HitTime
    m_pPropHitTime = new CPropTreeItemEdit();
    m_HitPropertyTree.InsertItem(m_pPropHitTime, m_pHitPropRoot);
    m_pPropHitTime->SetLabelText(L"Hit Time");
    m_pPropHitTime->SetValueFormat(CPropTreeItemEdit::ValueFormatFloatPointer);
    m_pPropHitTime->SetInfoText(L"Set Hit Time for Hit Event");
    m_pPropHitTime->SetCtrlID(ID_PROP_ITEM_HITTIME);            

    // Target Behavior
    m_pPropTargetBehavior = new CPropTreeItemCombo();
    m_HitPropertyTree.InsertItem(m_pPropTargetBehavior, m_pHitPropRoot);
    m_pPropTargetBehavior->CreateComboBox(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST);
    m_pPropTargetBehavior->SetLabelText(L"Target Behavior");
    m_pPropTargetBehavior->SetInfoText(L"Select Behavior of Target that for Hit Event");
    m_pPropTargetBehavior->SetCtrlID(ID_PROP_ITEM_TARGETBEHAVIOR);    
    m_pPropTargetBehavior->AddString(L"None");      // Add String으로 넣어야만 제대로 작동한다.
    m_pPropTargetBehavior->AddString(L"Push");
    m_pPropTargetBehavior->AddString(L"Toss");
    m_pPropTargetBehavior->AddString(L"Fall Down");
    m_pPropTargetBehavior->AddString(L"Knock Down");
    for(int i = 0; i < 5; ++i)
    {
        m_pPropTargetBehavior->SetItemData(i, i);
    }
    m_pPropTargetBehavior->SetCurSel(0);

    // Attack Power
    m_pPropAttackPower = new CPropTreeItemCombo();
    m_HitPropertyTree.InsertItem(m_pPropAttackPower, m_pHitPropRoot);
    m_pPropAttackPower->CreateComboBoxBool();
    m_pPropAttackPower->SetLabelText(L"Power Effect");
    m_pPropAttackPower->SetInfoText(L"Set Power Effct for Hit Event");
    m_pPropAttackPower->SetCtrlID(ID_PROP_ITEM_ATTACKPOWER);
    m_pPropAttackPower->SetCurSel(0);

    // Hand Type
    m_pPropHandType = new CPropTreeItemCombo();
    m_HitPropertyTree.InsertItem(m_pPropHandType, m_pHitPropRoot);
    m_pPropHandType->CreateComboBox();
    m_pPropHandType->SetLabelText(L"Hand Type");
    m_pPropHandType->SetInfoText(L"Select Hand Type for Hit Event");
    m_pPropHandType->SetCtrlID(ID_PROP_ITEM_HANDTYPE);    
    m_pPropHandType->AddString(L"Left Hand");
    m_pPropHandType->AddString(L"Right Hand");
    for(int i = 0; i < 3; ++i)
    {
        m_pPropHandType->SetItemData(i, i);
    }
    m_pPropHandType->SetCurSel(0);
    
    // Bone
    m_pPropBone = new CPropTreeItemCombo();
    m_HitPropertyTree.InsertItem(m_pPropBone, m_pHitPropRoot);
    m_pPropBone->CreateComboBox();
    m_pPropBone->SetLabelText(L"Bone");
    m_pPropBone->SetInfoText(L"Select Bone of Model That for Hit Event");
    m_pPropBone->SetCtrlID(ID_PROP_ITEM_BONE);    

    // EffectType
    m_pPropEffectType = new CPropTreeItemCombo();
    m_HitPropertyTree.InsertItem(m_pPropEffectType, m_pHitPropRoot);
    m_pPropEffectType->CreateComboBox();
    m_pPropEffectType->SetLabelText(L"Effect Type");
    m_pPropEffectType->SetInfoText(L"Select Effect Type for Hit Event");
    m_pPropEffectType->SetCtrlID(ID_PROP_ITEM_EFFECTTYPE);    
    SetEffectTypeList();

    // Lua ID
    m_pPropLuaID = new CPropTreeItemCombo();
    m_HitPropertyTree.InsertItem(m_pPropLuaID, m_pHitPropRoot);
    m_pPropLuaID->CreateComboBox();
    m_pPropLuaID->SetLabelText(L"Lua ID");
    m_pPropLuaID->SetInfoText(L"Select Lua ID for Hit Event");
    m_pPropLuaID->SetCtrlID(ID_PROP_ITEM_LUAID);    
    SetLuaIDList();
    

    m_pHitPropRoot->Expand();
}

void CAnimAttrView::InitSoundPropertyItem()
{
    m_pSoundPropRoot = new CPropTreeItem();
    m_SoundProperyTree.InsertItem(m_pSoundPropRoot);
    m_pSoundPropRoot->SetLabelText(L"Sound Event Properties");

    // Sound Time
    m_pPropSoundTime = new CPropTreeItemEdit();
    m_SoundProperyTree.InsertItem(m_pPropSoundTime, m_pSoundPropRoot);
    m_pPropSoundTime->SetLabelText(L"Sount Event Time");
    m_pPropSoundTime->SetInfoText(L"Set Play Time for Sound Event");
    m_pPropSoundTime->SetCtrlID(ID_PROP_ITEM_SOUND_TIME);
    
    // Sound File
    m_pPropSoundFile = new CPropTreeItemCombo();
    m_SoundProperyTree.InsertItem(m_pPropSoundFile, m_pSoundPropRoot);
    m_pPropSoundFile->CreateComboBox();
    m_pPropSoundFile->SetLabelText(L"Sound File");
    m_pPropSoundFile->SetInfoText(L"Select Sound Fiel for Sound Event");
    m_pPropSoundFile->SetCtrlID(ID_PROP_ITEM_SOUND_FILE);

    m_pSoundPropRoot->Expand();
}

void CAnimAttrView::SetPropertyMode(EPropertyMode ePropertyMode)
{
    switch(ePropertyMode)
    {
    case MODE_NONE:
        m_HitPropertyTree.ShowWindow(SW_HIDE);
        m_SoundProperyTree.ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC)->ShowWindow(SW_SHOW);
        break;
    case MODE_HIT_EVENT:
        m_HitPropertyTree.ShowWindow(SW_SHOW);
        m_SoundProperyTree.ShowWindow(SW_HIDE);
        GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
        break;
    case MODE_SOUND_EVENT:
        m_HitPropertyTree.ShowWindow(SW_HIDE);
        m_SoundProperyTree.ShowWindow(SW_SHOW);
        GetDlgItem(IDC_STATIC)->ShowWindow(SW_HIDE);
        break;
    }
}

void CAnimAttrView::SetInit(CMTCharacter* pCharacter, SEventAnim* pEventAnim)
{
    USES_CONVERSION;

    if(!pCharacter || !pEventAnim)
    {
        SetPropertyMode(MODE_NONE);
        return;
    }

    if(m_pCharacter != pCharacter)
    {
        m_pCharacter = pCharacter;
        SetBoneList();
    }
    
    m_pEventAnim = pEventAnim;

    // 받은 데이터를 윈도우에 표시한다.
    if(pEventAnim->eEventID == EVENT_ANIM_HIT)
    {
        SetPropertyMode(MODE_HIT_EVENT);        
        SEventAnimHit* pEventAnimHit = (SEventAnimHit*)pEventAnim;

        m_pPropHitTime->SetItemValue((LPARAM)&pEventAnimHit->fTime);
        m_pPropTargetBehavior->SetCurSel(pEventAnimHit->eTargetBehavior);
        if(pEventAnimHit->bPowerEffect)
        {
            m_pPropAttackPower->SetCurSel(1);
        }
        else
        {
            m_pPropAttackPower->SetCurSel(0);
        }
        m_pPropHandType->SetCurSel(pEventAnimHit->eHandType);

        // 받아온 데이터에 맞게 세팅한다.
        m_pPropBone->SetCurSelText(pEventAnimHit->chBoneName);
        
        std::string* sEffectName = CNtlPLCharacterParser::GetInstance().GetProjEffectTypeTable()->GetString(pEventAnimHit->uiProjectileEffectType);
        if(sEffectName)
        {
            m_pPropEffectType->SetCurSelText(sEffectName->c_str());
        }
        else
        {
            m_pPropEffectType->SetCurSel(0);
        }
        
        std::string* pLuaName = CNtlPLCharacterParser::GetInstance().GetEffectLuaIDTable()->GetString(pEventAnimHit->uiProjectileEffectType);
        if(pLuaName)
        {
            m_pPropLuaID->SetCurSelText(pLuaName->c_str());
        }
        else
        {
            m_pPropLuaID->SetCurSel(0);
        }
        
    }
    else if(pEventAnim->eEventID == EVENT_ANIM_SOUND)
    {
        SetPropertyMode(MODE_SOUND_EVENT);
    }    

    m_HitPropertyTree.Invalidate();
}

void CAnimAttrView::SetBoneList()
{
    USES_CONVERSION;

    if(!m_pCharacter)
        return;

    m_pPropBone->ResetContent();
    

    for(RwInt32 i = 1; i < m_pCharacter->GetBoneNum(); ++i)
    {
        m_pPropBone->AddString(A2W(m_pCharacter->GetBoneName(i)));
        m_pPropBone->SetItemData(i - 1, i - 1);
    }

    m_pPropBone->SetCurSel(0);    
}

void CAnimAttrView::SetLuaIDList()
{
    USES_CONVERSION;

    m_pPropLuaID->ResetContent();

    // 파서로부터 데이터를 가져와서 설정한다.
    int nCount = 0;
    KEY_STRING_MAP* pMap = CNtlPLCharacterParser::GetInstance().GetEffectLuaIDTable()->GetMapKeyToString();
    if(pMap)
    {
        KEY_STRING_MAP_ITER it;
        for(it = pMap->begin(); it != pMap->end(); ++it)
        {
            std::string sLuaName = it->second;
            m_pPropLuaID->AddString(A2W(sLuaName.c_str()));
            m_pPropLuaID->SetItemData(nCount, nCount);
            ++nCount;
        }
    }

    m_pPropLuaID->SetCurSel(0);
}

void CAnimAttrView::SetEffectTypeList()
{
    USES_CONVERSION;

    m_pPropEffectType->ResetContent();

    // 파서로부터 데이터를 가져와서 설정한다.
    int nCount = 0;
    KEY_STRING_MAP* pMap = CNtlPLCharacterParser::GetInstance().GetProjEffectTypeTable()->GetMapKeyToString();
    if(pMap)
    {
        KEY_STRING_MAP_ITER it = pMap->begin();
        for(; it != pMap->end(); ++it)
        {
            std::string sEffectName = it->second;
            m_pPropEffectType->AddString(A2W(sEffectName.c_str()));
            m_pPropEffectType->SetItemData(nCount, nCount);
            ++nCount;
        }        
    }

    m_pPropEffectType->SetCurSel(0);
}

void CAnimAttrView::SetSoundFileList()
{

}
void CAnimAttrView::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);

    SetScrollSizes(MM_TEXT, CSize(cx, cy));

    CRect rc;
    GetClientRect(&rc);
    
    m_HitPropertyTree.OnSize(nType, rc.Width(), rc.Height());
    m_SoundProperyTree.OnSize(nType, rc.Width(), rc.Height());
}

void CAnimAttrView::OnItemChanged(NMHDR* pNotifyStruct, LRESULT* plResult)
{
    // 프로퍼티 컨트롤의 이벤트 처리 함수
    LPNMPROPTREE pNMPropTree = (LPNMPROPTREE)pNotifyStruct;

    if (pNMPropTree->pItem)
    {
        //ApplyProperties(pNMPropTree->pItem);
        if(pNMPropTree->pItem == m_pPropHitTime)
        {
            OnChangeHitTime();
        }
        else if(pNMPropTree->pItem == m_pPropTargetBehavior)
        {
            OnChangeBehavior();
        }
        else if(pNMPropTree->pItem == m_pPropAttackPower)
        {
            OnChangePowerEffect();
        }
        else if(pNMPropTree->pItem == m_pPropHandType)
        {
            OnChangeHandType();
        }
        else if(pNMPropTree->pItem == m_pPropBone)
        {
            OnChangeBone();
        }
        else if(pNMPropTree->pItem == m_pPropEffectType)
        {
            OnChangeEffectType();
        }
        else if(pNMPropTree->pItem == m_pPropLuaID)
        {
            OnChangeLuaID();
        }
        else if(pNMPropTree->pItem == m_pPropSoundFile)
        {
            OnChangeSoundFile();
        }
        else if(pNMPropTree->pItem == m_pPropSoundTime)
        {   
            OnChangeSoundTime();
        }
    }
    *plResult = 0;
}

void CAnimAttrView::OnChangeHitTime()
{
    if(!m_pEventAnim)
        return;

    CString sTime;
    m_pPropHitTime->GetWindowText(sTime);    
    m_pEventAnim->fTime = (RwReal)_wtof(sTime);    

    // AnimToolView의 마커위치도 변경하여 준다.
    CAnimToolView::GetInstance()->SetMarkerPos(m_pEventAnim);

}

void CAnimAttrView::OnChangeBehavior()
{
    if(!m_pEventAnim)
        return;

    SEventAnimHit* pEventAnimHit = (SEventAnimHit*)m_pEventAnim;

    int nIndex = m_pPropTargetBehavior->GetCurSel();
    pEventAnimHit->eTargetBehavior = (ETargetBehavior)nIndex;
}

void CAnimAttrView::OnChangePowerEffect()
{
    if(!m_pEventAnim)
        return;

    SEventAnimHit* pEventAnimHit = (SEventAnimHit*)m_pEventAnim;

    int nIndex = m_pPropAttackPower->GetCurSel();
    if(nIndex == 0)
    {
        pEventAnimHit->bPowerEffect = FALSE;
    }
    else
    {
        pEventAnimHit->bPowerEffect = TRUE;
    }
}

void CAnimAttrView::OnChangeHandType()
{
    if(!m_pEventAnim)
        return;

    SEventAnimHit* pEventAnimHit = (SEventAnimHit*)m_pEventAnim;

    int nIndex = m_pPropHandType->GetCurSel();
    if(nIndex == 0)
    {
        pEventAnimHit->eHandType = HAND_TYPE_LEFT;
    }
    else if(nIndex == 1)
    {
        pEventAnimHit->eHandType = HAND_TYPE_RIGHT;
    }    
}

void CAnimAttrView::OnChangeBone()
{
    USES_CONVERSION;

    if(!m_pEventAnim)
        return;

    SEventAnimHit* pEventAnimHit = (SEventAnimHit*)m_pEventAnim;

    CString sBoneName;
    m_pPropBone->GetWindowText(sBoneName);
    sprintf(pEventAnimHit->chBoneName, "%s", W2A(sBoneName));    
}

void CAnimAttrView::OnChangeEffectType()
{
    USES_CONVERSION;

    if(!m_pEventAnim)
        return;

    SEventAnimHit* pEventAnimHit = (SEventAnimHit*)m_pEventAnim;

    CString sEffectName;
    m_pPropEffectType->GetWindowText(sEffectName);
    std::string strEffectName = W2A(sEffectName);
    pEventAnimHit->uiProjectileEffectType = CNtlPLCharacterParser::GetInstance().GetProjEffectTypeTable()->GetKey(strEffectName);
   
}

void CAnimAttrView::OnChangeLuaID()
{
    USES_CONVERSION;

    if(!m_pEventAnim)
        return;

    SEventAnimHit* pEventAnimHit = (SEventAnimHit*)m_pEventAnim;
    CString sLuaName;
    m_pPropLuaID->GetWindowText(sLuaName);
    std::string strLuaName = W2A(sLuaName);

	//by HongHoDong LuaID에 넣어야 합니다.(2006. 5. 18)
    //pEventAnimHit->uiProjectileEffectType = CNtlPLCharacterParser::GetInstance().GetEffectLuaIDTable()->GetKey(strLuaName);
	pEventAnimHit->uiEffectLuaID = CNtlPLCharacterParser::GetInstance().GetEffectLuaIDTable()->GetKey(strLuaName);

}

void CAnimAttrView::OnChangeSoundTime()
{

}

void CAnimAttrView::OnChangeSoundFile()
{

}