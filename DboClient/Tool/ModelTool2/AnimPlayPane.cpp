// AnimPlayPane.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "AttackTypePane.h"
#include "PropertyPane.h"
#include "NtlSoundManager.h"
#include "AnimPlayPane.h"

CAnimPlayPane* CAnimPlayPane::m_pIntance = NULL;

// CAnimPlayPane

IMPLEMENT_DYNCREATE(CAnimPlayPane, CXTResizeFormView)

CAnimPlayPane::CAnimPlayPane()
	: CXTResizeFormView(CAnimPlayPane::IDD)
{
    m_pIntance = this;

    m_pCharacter = NULL;
    m_pAnimData = NULL;
    m_pEventHit = NULL;
    m_pEventSound = NULL;
    m_pEventClipBoard = NULL;
	m_pObject = NULL;
	m_eAnimPlayPaneState = ANIMPLAYPANE_CHARACTER;
}

CAnimPlayPane::~CAnimPlayPane()
{
}

void CAnimPlayPane::DoDataExchange(CDataExchange* pDX)
{
    CXTResizeFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_SLIDER, m_slideAnimTime);
    DDX_Control(pDX, IDC_BT_ANIM_PLAY, m_btAnimPlay);
    DDX_Control(pDX, IDC_BT_ANIM_STOP, m_btAnimStop);
    DDX_Control(pDX, IDC_ED_SPEED, m_edAnimSpeed);
    DDX_Control(pDX, IDC_SPIN_ANIM_SPEED, m_spinAnimSpeed);
    DDX_Control(pDX, IDC_ED_TIME, m_edAnimTime);
    DDX_Control(pDX, IDC_CB_EVENT, m_cbEvent);
}

BEGIN_MESSAGE_MAP(CAnimPlayPane, CXTResizeFormView)
    ON_BN_CLICKED(IDC_BT_ANIM_PLAY, &CAnimPlayPane::OnBnClickedBtAnimPlay)
    ON_BN_CLICKED(IDC_BT_ANIM_STOP, &CAnimPlayPane::OnBnClickedBtAnimStop)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ANIM_SPEED, &CAnimPlayPane::OnDeltaposSpinAnimSpeed)
    ON_EN_CHANGE(IDC_ED_SPEED, &CAnimPlayPane::OnEnChangeEdSpeed)
    ON_CBN_SELCHANGE(IDC_CB_EVENT, &CAnimPlayPane::OnCbnSelchangeCbEvent)
END_MESSAGE_MAP()


// CAnimPlayPane diagnostics

#ifdef _DEBUG
void CAnimPlayPane::AssertValid() const
{
	CXTResizeFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAnimPlayPane::Dump(CDumpContext& dc) const
{
	CXTResizeFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAnimPlayPane message handlers

void CAnimPlayPane::OnInitialUpdate()
{
    CXTResizeFormView::OnInitialUpdate();

    SetResize(IDC_BT_ANIM_PLAY, SZ_TOP_LEFT, SZ_TOP_LEFT);
    SetResize(IDC_BT_ANIM_STOP, SZ_TOP_LEFT, SZ_TOP_LEFT);
    SetResize(IDC_SLIDER, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_CB_EVENT, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
    SetResize(IDC_ED_TIME, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
    SetResize(IDC_ED_SPEED, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
    SetResize(IDC_SPIN_ANIM_SPEED, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
    SetResize(IDC_STATIC_X, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
}

void CAnimPlayPane::SetAnimData(CMTCharacter* pCharacter, STypeAnimData* pAnimData)
{
    if(!pCharacter || !pAnimData)
    {
        m_pCharacter = NULL;
        m_pAnimData  = NULL;
        m_slideAnimTime.SetPos(0);
        m_slideAnimTime.ClearMarker();
        m_edAnimSpeed.SetWindowText("");
        m_edAnimTime.SetWindowText("");

        SetEnable(FALSE);
        GetSafeInstance(CPropertyPane)->SetEvent(NULL, NULL);        
    }
    else
    {
		m_eAnimPlayPaneState = ANIMPLAYPANE_CHARACTER;
		UpdateEventComboBox();

        m_pCharacter = pCharacter;
		m_pObject = NULL;
        m_pAnimData = pAnimData;

        CString strAnimSpeed;
        strAnimSpeed.Format("%.1f", m_pCharacter->GetAnimSpeed());
        m_edAnimSpeed.SetWindowText(strAnimSpeed);

        InitMarker();
        SetEnable(TRUE);

        if(m_pCharacter->GetAnimUpdate())
        {
            OnBnClickedBtAnimPlay();        
        }
        else
        {
            OnBnClickedBtAnimStop();
        }
    }
}

void CAnimPlayPane::SetTriggerObjectAnimData( CMTObject* pObject, STypeAnimData* pAnimData ) 
{
	if(!pObject || !pAnimData)
		return;

	m_eAnimPlayPaneState = ANIMPLAYPANE_OBJECT;
	UpdateEventComboBox();

	m_pCharacter = NULL;
	m_pObject = pObject;
	m_pAnimData = pAnimData;

	m_edAnimSpeed.SetWindowText("1.0");

	InitMarker();
	SetEnable(TRUE);

	if(pObject->GetPlayAnim())
	{
		OnBnClickedBtAnimPlay();
	}
	else
	{
		OnBnClickedBtAnimStop();
	}
}


void CAnimPlayPane::SetEnable(BOOL bEnable)
{
    if(bEnable)
    {
		RwBool bAnimUpdate = FALSE;
		
		switch(m_eAnimPlayPaneState)
		{
		case ANIMPLAYPANE_CHARACTER:
			bAnimUpdate = m_pCharacter->GetAnimUpdate();
			break;
		case ANIMPLAYPANE_OBJECT:
			bAnimUpdate = m_pObject->GetPlayAnim();
			break;
		}

        if(bAnimUpdate)
        {
            m_btAnimPlay.EnableWindow(FALSE);
            m_btAnimStop.EnableWindow(TRUE);
        }
        else
        {
            m_btAnimPlay.EnableWindow(TRUE);
            m_btAnimStop.EnableWindow(FALSE);
        }        
    }
    else
    {
        m_btAnimPlay.EnableWindow(FALSE);
        m_btAnimStop.EnableWindow(FALSE);
    }

    m_cbEvent.EnableWindow(bEnable);
    m_slideAnimTime.EnableWindow(bEnable);
    m_spinAnimSpeed.EnableWindow(bEnable);
    m_edAnimSpeed.EnableWindow(bEnable);
}

void CAnimPlayPane::SetAnimTime(RwReal fCurrentTime, RwReal fTotalTime)
{
    CString strAnimTime;
    strAnimTime.Format("%.2f/%.2f", fCurrentTime, fTotalTime);
    m_edAnimTime.SetWindowText(strAnimTime);

    int nMax = (int)(fTotalTime * 100.0f);
    int nValue = (int)(fCurrentTime * 100.0f);
    m_slideAnimTime.SetRangeMax(nMax);
    m_slideAnimTime.SetPos(nValue);
}

void CAnimPlayPane::InitMarker()
{
    if(!m_pAnimData)
        return;

    GetSafeInstance(CPropertyPane)->SetEvent(NULL, NULL);    

    m_slideAnimTime.ClearMarker();

	switch(m_eAnimPlayPaneState)
	{
	case ANIMPLAYPANE_CHARACTER:
		m_pCharacter->Update(0.1f);
		SetAnimTime(m_pCharacter->GetBaseCurrentAnimTime(), m_pCharacter->GetBaseDurationAnimTime());        
        m_pAnimData->fPlayTime = m_pCharacter->GetBaseDurationAnimTime();   // 애니메이션 리소스가 바뀌는 경우가 있다. 그럴때를 대비해서 Play시에 시간을 업데이트한다.
		break;
	case ANIMPLAYPANE_OBJECT:
		//m_pObject->Update(0.1f);
		SetAnimTime(m_pObject->GetBaseCurrentAnimTime(), m_pObject->GetBaseDurationAnimTime());
        m_pAnimData->fPlayTime = m_pObject->GetBaseDurationAnimTime();
		break;
	}

    for(UINT i = 0; i < m_pAnimData->vecAnimEvent.size(); ++i)
    {
        SEventAnim* pAnim = m_pAnimData->vecAnimEvent[i];
        if(pAnim->fTime > 0.0f)
        {
            int nPos = TimeToPos(pAnim->fTime);
            m_slideAnimTime.AddMarker(nPos, pAnim);
        }
    } 
}

void CAnimPlayPane::Update()
{
	switch(m_eAnimPlayPaneState)
	{
	case ANIMPLAYPANE_CHARACTER:
		if(m_pCharacter && m_pCharacter->GetAnimUpdate())
		{
			SetAnimTime(m_pCharacter->GetBaseCurrentAnimTime(), m_pCharacter->GetBaseDurationAnimTime());        
		}
		break;
	case ANIMPLAYPANE_OBJECT:
		if(m_pObject && m_pObject->GetPlayAnim())
		{
			SetAnimTime(m_pObject->GetBaseCurrentAnimTime(), m_pObject->GetBaseDurationAnimTime());
		}
		break;    
	}
}

void CAnimPlayPane::OnBnClickedBtAnimPlay()
{
    if(!m_pCharacter && !m_pObject)
        return;

    m_btAnimPlay.EnableWindow(FALSE);
    m_btAnimStop.EnableWindow(TRUE);

	switch(m_eAnimPlayPaneState)
	{
	case ANIMPLAYPANE_CHARACTER:
		m_pCharacter->SetAnimUpdate(TRUE);  
		break;
	case ANIMPLAYPANE_OBJECT:
		m_pObject->SetAnimUpdate(TRUE);
		break;
	}
}

void CAnimPlayPane::OnBnClickedBtAnimStop()
{
    if(!m_pCharacter && !m_pObject)
        return;

    m_btAnimPlay.EnableWindow(TRUE);
    m_btAnimStop.EnableWindow(FALSE);

	switch(m_eAnimPlayPaneState)
	{
	case ANIMPLAYPANE_CHARACTER:
		m_pCharacter->SetAnimUpdate(FALSE);
		break;
	case ANIMPLAYPANE_OBJECT:
		m_pObject->SetAnimUpdate(FALSE);
        break;    
	}
}

BOOL CAnimPlayPane::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    switch(message)
    {
    case EXSM_POS_CHANGED:
        OnSlidePosChanged();    
        break;
    case EXSM_MARKER_ADD:
        OnSlideMarkerAdd((SSliderMarker*)lParam);
        break;
    case EXSM_MARKER_SELECT:
        OnSlideMarkerSelect();
        break;
    case EXSM_MARKER_REMOVE:
        OnSlideMarkerRemove((SSliderMarker*)lParam);
        break;
    case EXSM_MARKER_POS_CHANGED:
        OnSlideMarkerMove();
        break;
    case EXSM_MARKER_POS_CHANGE_END:
        OnSlideMarkerChanged();
        break;    
    case EXSM_MARKER_COPY:
        OnSlideMarkerCopy();
        break;
    case EXSM_MARKER_PASTE:
        OnSlideMarkerPaste();
        break;
    }

    return CXTResizeFormView::OnWndMsg(message, wParam, lParam, pResult);
}

void CAnimPlayPane::OnSlidePosChanged()
{
    // 슬라이더의 값이 변경되었을때.
    int nPos = m_slideAnimTime.GetPos();
    float fTime = (float)nPos / 100.0f;
	CString sAnimTime;

	switch(m_eAnimPlayPaneState)
	{
	case ANIMPLAYPANE_CHARACTER:
		m_pCharacter->SetBaseCurrentAnimTime(fTime);
	    sAnimTime.Format("%.2f/%.2f", fTime, m_pCharacter->GetBaseDurationAnimTime());
		break;
	case ANIMPLAYPANE_OBJECT:
		m_pObject->SetBaseCurrentAnimTime(fTime);
		sAnimTime.Format("%.2f/%.2f", fTime, m_pObject->GetBaseDurationAnimTime());
		break;
	}

    m_edAnimTime.SetWindowText(sAnimTime);    
}

#define CreateAnimEvent(className) \
        {\
            className *pEvent = new className(); \
            pEvent->fTime = PosToTime(pSliderMarker->m_nPoint);\
            m_pAnimData->vecAnimEvent.push_back(pEvent);\
            pSliderMarker->m_pAnimEvent = pEvent;\
        }\

void CAnimPlayPane::OnSlideMarkerAdd(SSliderMarker* pSliderMarker)
{
    if(!pSliderMarker || !m_pAnimData)
        return;

    // 데이터가 없는 경우 생성하여 프로퍼티에 추가한다.
    if(pSliderMarker->m_pAnimEvent == NULL)
    {
        switch(m_slideAnimTime.GetEventMode())
        {
        case EVENT_ANIM_HIT:
            CreateAnimEvent(SEventAnimHit);
            GetSafeInstance(CAttackTypePane)->SetAnimEventData(m_pAnimData);
            break;
        case EVENT_ANIM_VISUAL_EFFECT:
            CreateAnimEvent(SEventVisualEffect);            
            break;
        case EVENT_ANIM_VISUAL_SOUND:
            CreateAnimEvent(SEventSound);            
            break;
        case EVENT_ANIM_FOOT_STEP:            
            CreateAnimEvent(SEventFootStep);
            break;
        case EVENT_ANIM_WEIGHT_TIME:            
            CreateAnimEvent(SEventWeightTime);
            break;
        case EVENT_ANIM_TRACE_EFFECT:            
            CreateAnimEvent(SEventTrace);
            break;
        case EVENT_ANIM_SUB_WEAPON:            
            CreateAnimEvent(SEventSubWeapon);
            break;
        case EVENT_ANIM_POST_EFFECT:            
            CreateAnimEvent(SEventPostEffect);
			break;
		case EVENT_ANIM_SUMMON_PET:			
            CreateAnimEvent(SEventSummonPet);
			break;
        case EVENT_ANIM_TMQ:            
            CreateAnimEvent(SEventAnimCinematic);
            break;
        case EVENT_ANIM_ALPHA:            
            CreateAnimEvent(SEventAlpha);
            break;
        case EVENT_ANIM_EXPLOSION:            
            CreateAnimEvent(SEventExplosion);
            break;
        case EVENT_ANIM_DIRECT:
            CreateAnimEvent(SEventDirect);
            break;
        case EVENT_ANIM_COLOR_CHANGE:
            CreateAnimEvent(SEventColorChange);
            break;
        case EVENT_ANIM_STRETCH:
            CreateAnimEvent(SEventStretch);
            break;		
        case EVENT_ANIM_TRIGGER:
            CreateAnimEvent(SEventTrigger);
            break;
        case EVENT_ANIM_SKILL_CANCEL:
            if(CheckSkillCancel(PosToTime(pSliderMarker->m_nPoint)))
            {
                CreateAnimEvent(SEventSkillCancel);
            }            
            else
            {
                MessageBox("Skill Cancel Event Must Add After All Hit Event!");                
            }
            break;
        }

        CModelToolApplication::GetInstance()->SetDataChanged();
    }
}

void CAnimPlayPane::OnSlideMarkerSelect()
{
    // NOTE: AddMarker떄도 이 메소드가 호출된다.

    SSliderMarker* pSliderMarker = m_slideAnimTime.GetSelectMarker();
    if(pSliderMarker && m_pAnimData)
    {
		switch(m_eAnimPlayPaneState)
		{
		case ANIMPLAYPANE_CHARACTER:
			GetSafeInstance(CPropertyPane)->SetEvent(pSliderMarker->m_pAnimEvent, m_pCharacter->GetClump(),TRUE);        
			break;
		case ANIMPLAYPANE_OBJECT:
			GetSafeInstance(CPropertyPane)->SetEvent(pSliderMarker->m_pAnimEvent, m_pObject->GetClump(), TRUE);
			break;
		}
    }
}

void CAnimPlayPane::OnSlideMarkerMove()
{
    // 마커가 움직일때 오른쪽 AttrView의 정보도 갱신하여 준다.
    SSliderMarker* pSliderMarker = m_slideAnimTime.GetSelectMarker();
    if(pSliderMarker && m_pAnimData)
    {
		pSliderMarker->m_pAnimEvent->fTime = PosToTime(pSliderMarker->m_nPoint);        			
		GetSafeInstance(CPropertyPane)->Update();			
    }
}

void CAnimPlayPane::OnSlideMarkerChanged()
{
    // 이동이 완료되었을때 호출된다.
    if(m_slideAnimTime.GetEventMode() == EVENT_ANIM_HIT)
    {
        GetSafeInstance(CAttackTypePane)->SetAnimEventData(m_pAnimData);
    }    
}

void CAnimPlayPane::OnSlideMarkerRemove(SSliderMarker* pRemovedMarker)
{
    if(pRemovedMarker && m_pAnimData)
    {
        std::vector<SEventAnim*>::iterator it;
        for(it = m_pAnimData->vecAnimEvent.begin(); it != m_pAnimData->vecAnimEvent.end(); ++it)
        {
            SEventAnim* pEventAnim = *it;
            if(pEventAnim && (pEventAnim == pRemovedMarker->m_pAnimEvent))
            {
                // vector에서 삭제한다.
                m_pAnimData->vecAnimEvent.erase(it);
                break;
            }
        }
    }

    if(m_slideAnimTime.GetEventMode() == EVENT_ANIM_HIT)
    {
        GetSafeInstance(CAttackTypePane)->SetAnimEventData(m_pAnimData);
    }
    
    GetSafeInstance(CPropertyPane)->SetEvent(NULL, NULL);    
}

void CAnimPlayPane::OnSlideMarkerCopy()
{
    // Hit Event Copy
    SSliderMarker* pSlideMarker = m_slideAnimTime.GetSelectMarker();
    if(pSlideMarker)
    {
        m_pEventClipBoard = pSlideMarker->m_pAnimEvent;
    }
}

void CAnimPlayPane::OnSlideMarkerPaste()
{
    if(!m_pEventClipBoard)
        return;

    SSliderMarker* pSlideMarker = m_slideAnimTime.GetSelectMarker();
    if(pSlideMarker)
    {
        RwReal fOriginTime = pSlideMarker->m_pAnimEvent->fTime;

        if(pSlideMarker->m_pAnimEvent->eEventID == m_pEventClipBoard->eEventID)
        {
            memcpy(pSlideMarker->m_pAnimEvent, m_pEventClipBoard, GetAnimEventSize(pSlideMarker->m_pAnimEvent->eEventID));
        }

        pSlideMarker->m_pAnimEvent->fTime = fOriginTime;

        OnSlideMarkerSelect();
    }
}

void CAnimPlayPane::SetMarkerPos(SEventAnim* pEventAnim)
{   
    if(!pEventAnim)
        return;

    m_slideAnimTime.SetSelectMarkerPos(TimeToPos(pEventAnim->fTime));
}

RwReal CAnimPlayPane::PosToTime(int nPos)
{
	RwReal fTotalTime = 1.0f;
	
	switch(m_eAnimPlayPaneState)
	{
	case ANIMPLAYPANE_CHARACTER:
		fTotalTime = m_pCharacter->GetBaseDurationAnimTime();
		break;
	case ANIMPLAYPANE_OBJECT:
		fTotalTime = m_pObject->GetBaseDurationAnimTime();
		break;
	}

    return ((float)nPos / (float)m_slideAnimTime.GetRangeMax()) * fTotalTime;
}

int CAnimPlayPane::TimeToPos(RwReal fTime)
{
    return (int)((fTime * 100.0f) + 0.5f);   
}

void CAnimPlayPane::OnDeltaposSpinAnimSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    if(!m_pCharacter)
        return;

    CString strAnimSpeed;
    m_edAnimSpeed.GetWindowText(strAnimSpeed);
    RwReal fAnimSpeed = (RwReal)atof(strAnimSpeed);
    float fOffset = 0.1f;

    if(pNMUpDown->iDelta < 0)
    {
        fAnimSpeed += fOffset;            
    }
    else
    {
        fAnimSpeed -= fOffset;
    }

    if(fAnimSpeed < 0.0f)
        fAnimSpeed = 0.0f;

    m_pCharacter->SetAnimSpeed(fAnimSpeed);    
    strAnimSpeed.Format("%.1f", fAnimSpeed);
    m_edAnimSpeed.SetWindowText(strAnimSpeed);

    *pResult = 0;
}

void CAnimPlayPane::OnEnChangeEdSpeed()
{
    if(!m_pCharacter)
        return;

    CString strAnimSpeed;
    m_edAnimSpeed.GetWindowText(strAnimSpeed);
    RwReal fAnimSpeed = (RwReal)atof(strAnimSpeed);
    m_pCharacter->SetAnimSpeed(fAnimSpeed);    

}

void CAnimPlayPane::OnCbnSelchangeCbEvent()
{
    EAnimEventType eEventType = (EAnimEventType)m_cbEvent.GetItemData(m_cbEvent.GetCurSel());
    m_slideAnimTime.SetEventMode(eEventType);

    // AttackType Pane은 Hit시에만 표시한다.
    if(eEventType == EVENT_ANIM_HIT)
    {
        GetSafeInstance(CAttackTypePane)->SetAnimEventData(m_pAnimData);
    }
    else
    {
        GetSafeInstance(CAttackTypePane)->SetAnimEventData(NULL);
    }    
}

void CAnimPlayPane::ChangeAnimPlay()
{
    if(!m_pAnimData)
        return;

	RwBool bAnimPlay = FALSE;

	switch(m_eAnimPlayPaneState)
	{
	case ANIMPLAYPANE_CHARACTER:
		bAnimPlay = m_pCharacter->GetAnimUpdate();
		break;
	case ANIMPLAYPANE_OBJECT:
		bAnimPlay = m_pObject->GetPlayAnim();
		break;
	}

    if(bAnimPlay)
    {
        OnBnClickedBtAnimStop();
    }
    else
    {
        OnBnClickedBtAnimPlay();
    }
}

void CAnimPlayPane::UpdateEventComboBox() 
{
	int nSel = m_cbEvent.GetCurSel();
	m_cbEvent.ResetContent();

	switch(m_eAnimPlayPaneState)
	{
	case ANIMPLAYPANE_CHARACTER:
		m_cbEvent.AddString("Hit");
		m_cbEvent.AddString("Effect");
		m_cbEvent.AddString("Sound");
		m_cbEvent.AddString("FootStep");    
		m_cbEvent.AddString("WeightTime");
		m_cbEvent.AddString("TraceEvent");
		m_cbEvent.AddString("SubWeapon");
		m_cbEvent.AddString("PostEffect");
		m_cbEvent.AddString("SummonPet");        
        m_cbEvent.AddString("Alpha Fade");
        m_cbEvent.AddString("Direct Event");
        m_cbEvent.AddString("Color Change");
        m_cbEvent.AddString("Stretch Bone");
        m_cbEvent.AddString("Trigger");
        m_cbEvent.AddString("Skill Cancel");

		m_cbEvent.SetItemData(0, EVENT_ANIM_HIT);
		m_cbEvent.SetItemData(1, EVENT_ANIM_VISUAL_EFFECT);
		m_cbEvent.SetItemData(2, EVENT_ANIM_VISUAL_SOUND);
		m_cbEvent.SetItemData(3, EVENT_ANIM_FOOT_STEP);   
		m_cbEvent.SetItemData(4, EVENT_ANIM_WEIGHT_TIME);
		m_cbEvent.SetItemData(5, EVENT_ANIM_TRACE_EFFECT);
		m_cbEvent.SetItemData(6, EVENT_ANIM_SUB_WEAPON);
		m_cbEvent.SetItemData(7, EVENT_ANIM_POST_EFFECT);
		m_cbEvent.SetItemData(8, EVENT_ANIM_SUMMON_PET);        
        m_cbEvent.SetItemData(9, EVENT_ANIM_ALPHA);
        m_cbEvent.SetItemData(10, EVENT_ANIM_DIRECT);
        m_cbEvent.SetItemData(11, EVENT_ANIM_COLOR_CHANGE);
        m_cbEvent.SetItemData(12, EVENT_ANIM_STRETCH);
        m_cbEvent.SetItemData(13, EVENT_ANIM_TRIGGER);
        m_cbEvent.SetItemData(14, EVENT_ANIM_SKILL_CANCEL);
		break;
	case ANIMPLAYPANE_OBJECT:
		m_cbEvent.AddString("Effect");
		m_cbEvent.AddString("Sound");
        m_cbEvent.AddString("TMQ Event");
        m_cbEvent.AddString("Alpha Fade");
        m_cbEvent.AddString("Explosion Camera");

		m_cbEvent.SetItemData(0, EVENT_ANIM_VISUAL_EFFECT);
		m_cbEvent.SetItemData(1, EVENT_ANIM_VISUAL_SOUND);
        m_cbEvent.SetItemData(2, EVENT_ANIM_TMQ);
        m_cbEvent.SetItemData(3, EVENT_ANIM_ALPHA);
        m_cbEvent.SetItemData(4, EVENT_ANIM_EXPLOSION);
		if(nSel > 4)
			nSel = 0;
		break;
	}

	m_cbEvent.SetCurSel(nSel);
	m_slideAnimTime.SetEventMode((EAnimEventType)m_cbEvent.GetItemData(nSel));
}

/**
 * 스킬 캔슬 이벤트 이후에는 Hit Event가 있으면 안된다. 이것을 체크한다.
 * \param fTime 스킬 캔슬 이벤트 타임 
 */
RwBool CAnimPlayPane::CheckSkillCancel( RwReal fTime ) 
{
    if(!m_pAnimData)
        return FALSE;
    
    for each(SEventAnim* pAnimEvent in m_pAnimData->vecAnimEvent)
    {
        if(pAnimEvent->eEventID == EVENT_ANIM_HIT && pAnimEvent->fTime > fTime)
            return FALSE;
    }

    return TRUE;
}