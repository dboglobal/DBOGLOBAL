// AnimToolView.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool.h"
#include "AnimToolView.h"
#include "AnimAttrView.h"
#include "RightToolView.h"

// CAnimToolView
CAnimToolView* CAnimToolView::m_pInstance = NULL;

IMPLEMENT_DYNCREATE(CAnimToolView, CFormView)

CAnimToolView::CAnimToolView()
	: CFormView(CAnimToolView::IDD)
    , m_sAnimTime(_T("0.0/0.0"))
{
    m_bUpdate = FALSE;
    m_pCharacter = NULL;
    m_pInstance = this;
    m_pAnimData = NULL;
}

CAnimToolView::~CAnimToolView()
{
}

void CAnimToolView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT1, m_sAnimTime);
    DDX_Control(pDX, IDC_SLIDER1, m_slideAnimTime);
    DDX_Control(pDX, IDC_BUTTON1, m_btAnimPlay);
    DDX_Control(pDX, IDC_BUTTON2, m_btAnimStop);
    DDX_Control(pDX, IDC_EDIT2, m_edAnimSpeed);
    DDX_Control(pDX, IDC_SPIN_ANIM_SPEED, m_spinAnimSpeed);
}

BEGIN_MESSAGE_MAP(CAnimToolView, CFormView)
    ON_BN_CLICKED(IDC_BUTTON2, &CAnimToolView::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_BUTTON1, &CAnimToolView::OnBnClickedButton1)    
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_ANIM_SPEED, &CAnimToolView::OnDeltaposSpinAnimSpeed)
    ON_EN_CHANGE(IDC_EDIT2, &CAnimToolView::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CAnimToolView diagnostics

#ifdef _DEBUG
void CAnimToolView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAnimToolView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

CAnimToolView* CAnimToolView::GetInstance()
{
    return m_pInstance;
}

void CAnimToolView::SetInit(CMTCharacter* pCharacter,STypeAnimData* pAnimData)
{
    if(pCharacter == NULL || pAnimData == NULL)
    {
        m_pCharacter = NULL;
        SetEnable(FALSE);
    }
    else
    {
        m_pCharacter = pCharacter;   
        m_pAnimData = pAnimData;  
        m_pCharacter->SetAnimUpdate(TRUE);        
        CString strAnimSpeed;
        strAnimSpeed.Format(L"%.1f", m_pCharacter->GetAnimSpeed());
        m_edAnimSpeed.SetWindowText(strAnimSpeed);

        InitMarker();
    }
}

void CAnimToolView::SetEnable(BOOL bEnable)
{
    m_bUpdate = bEnable;
    if(bEnable)
    {
        m_btAnimPlay.EnableWindow(FALSE);
        m_btAnimStop.EnableWindow(TRUE);
    }
    else
    {
        m_btAnimPlay.EnableWindow(FALSE);
        m_btAnimStop.EnableWindow(FALSE);
    }

    m_slideAnimTime.EnableWindow(bEnable);
    m_spinAnimSpeed.EnableWindow(bEnable);
    m_edAnimSpeed.EnableWindow(bEnable);

}

void CAnimToolView::SetAnimTime(RwReal fCurrentTime, RwReal fTotalTime)
{
    m_sAnimTime.Format(L"%.2f/%.2f", fCurrentTime, fTotalTime);

    int nMax = (int)(fTotalTime * 100.0f);
    int nValue = (int)(fCurrentTime * 100.0f);
    m_slideAnimTime.SetRangeMax(nMax);
    m_slideAnimTime.SetPos(nValue);

    UpdateData(FALSE);
}

void CAnimToolView::InitMarker()
{
    if(!m_pAnimData)
        return;

    CAnimAttrView::GetInstance()->SetInit(NULL, NULL);

    m_slideAnimTime.ClearMarker();
    m_pCharacter->Update(0.1f);
    SetAnimTime(m_pCharacter->GetBaseCurrentAnimTime(), m_pCharacter->GetBaseDurationAnimTime());        

    for(UINT i = 0; i < m_pAnimData->vecAnimEvent.size(); ++i)
    {
         SEventAnim* pAnim = m_pAnimData->vecAnimEvent[i];
         if(pAnim->fTime > 0.0f && pAnim->eEventID == EVENT_ANIM_HIT)
         {
             int nPos = TimeToPos(pAnim->fTime);
             m_slideAnimTime.AddMarker(nPos, pAnim);
         }
    } 
}

void CAnimToolView::Update()  
{
    if(m_pCharacter && m_pCharacter->GetAnimUpdate())
    {
        SetAnimTime(m_pCharacter->GetBaseCurrentAnimTime(), m_pCharacter->GetBaseDurationAnimTime());        
    }
}


void CAnimToolView::OnBnClickedButton2()
{
    if(!m_pCharacter)
        return;

    m_btAnimPlay.EnableWindow(TRUE);
    m_btAnimStop.EnableWindow(FALSE);

    m_pCharacter->SetAnimUpdate(FALSE);   
}

void CAnimToolView::OnBnClickedButton1()
{
    if(!m_pCharacter)
        return;

    m_btAnimPlay.EnableWindow(FALSE);
    m_btAnimStop.EnableWindow(TRUE);

    m_pCharacter->SetAnimUpdate(TRUE);   
}

void CAnimToolView::ChangeAnimPlay()
{
    if(!m_pCharacter)
        return;

    RwBool bUpdate = m_pCharacter->GetAnimUpdate();
    if(bUpdate)
    {
        OnBnClickedButton2();
    }
    else
    {
        OnBnClickedButton1();
    }
}


BOOL CAnimToolView::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
    switch(message)
    {
    case EXSM_POS_CHANGED:
        OnSlidePosChanged();    
        break;
    case EXSM_MARKER_ADD:
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
    }

    return CFormView::OnWndMsg(message, wParam, lParam, pResult);
}

void CAnimToolView::OnSlidePosChanged()
{
    // 슬라이더의 값이 변경되었을때.
    int nPos = m_slideAnimTime.GetPos();
    float fTime = (float)nPos / 100.0f;
    m_pCharacter->SetBaseCurrentAnimTime(fTime);
    
    m_sAnimTime.Format(L"%.2f/%.2f", fTime, m_pCharacter->GetBaseDurationAnimTime());
    UpdateData(FALSE);
}

void CAnimToolView::OnSlideMarkerSelect()
{
    // NOTE: AddMarker떄도 이 메소드가 호출된다.

    SSliderMarker* pSliderMarker = m_slideAnimTime.GetSelectMarker();
    if(pSliderMarker && m_pCharacter && m_pAnimData)
    {
        // 데이터가 없는 경우 생성하여 프로퍼티에 추가한다.
        if(pSliderMarker->m_pAnimEvent == NULL)
        {
            SEventAnimHit* pEventHit = new SEventAnimHit();
            pEventHit->fTime = PosToTime(pSliderMarker->m_nPoint);
            m_pAnimData->vecAnimEvent.push_back(pEventHit);
            pSliderMarker->m_pAnimEvent = pEventHit;
            CRightToolView::GetInstance()->SetAnimEventData(m_pAnimData);
        }

        CAnimAttrView::GetInstance()->SetInit(m_pCharacter, pSliderMarker->m_pAnimEvent);
    }
}

void CAnimToolView::OnSlideMarkerMove()
{
    // 마커가 움직일때 오른쪽 AttrView의 정보도 갱신하여 준다.
    SSliderMarker* pSliderMarker = m_slideAnimTime.GetSelectMarker();
    if(pSliderMarker && m_pCharacter && m_pAnimData)
    {
        pSliderMarker->m_pAnimEvent->fTime = PosToTime(pSliderMarker->m_nPoint);
        //CRightToolView::GetInstance()->SetAnimEventData(m_pAnimData);
        CAnimAttrView::GetInstance()->SetInit(m_pCharacter, pSliderMarker->m_pAnimEvent);
    }
}

void CAnimToolView::OnSlideMarkerChanged()
{
    // 이동이 완료되었을때 호출한다.
    CRightToolView::GetInstance()->SetAnimEventData(m_pAnimData);
}

void CAnimToolView::OnSlideMarkerRemove(SSliderMarker* pRemovedMarker)
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
    
    CRightToolView::GetInstance()->SetAnimEventData(m_pAnimData);
    CAnimAttrView::GetInstance()->SetInit(NULL, NULL);
}

void CAnimToolView::SetMarkerPos(SEventAnim* pEventAnim)
{
    if(!pEventAnim)
        return;
    
    m_slideAnimTime.SetSelectMarkerPos(TimeToPos(pEventAnim->fTime));
}

RwReal CAnimToolView::PosToTime(int nPos)
{
    if(!m_pCharacter)
        return 0.0f;

    return ((float)nPos / (float)m_slideAnimTime.GetRangeMax()) * m_pCharacter->GetBaseDurationAnimTime();
}

int CAnimToolView::TimeToPos(RwReal fTime)
{
    return (int)((fTime * 100.0f) + 0.5f);   
}


void CAnimToolView::OnDeltaposSpinAnimSpeed(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

    if(!m_pCharacter)
        return;

    CString strAnimSpeed;
    m_edAnimSpeed.GetWindowText(strAnimSpeed);
    RwReal fAnimSpeed = (RwReal)_wtof(strAnimSpeed);
    float fOffset = 0.1f;

    if(pNMUpDown->iDelta < 0)
    {
        fAnimSpeed += fOffset;            
    }
    else
    {
        fAnimSpeed -= fOffset;
    }
    
    m_pCharacter->SetAnimSpeed(fAnimSpeed);
    strAnimSpeed.Format(L"%.1f", fAnimSpeed);
    m_edAnimSpeed.SetWindowText(strAnimSpeed);

    *pResult = 0;
}

void CAnimToolView::OnEnChangeEdit2()
{
    if(!m_pCharacter)
        return;

    CString strAnimSpeed;
    m_edAnimSpeed.GetWindowText(strAnimSpeed);
    RwReal fAnimSpeed = (RwReal)_wtof(strAnimSpeed);
    m_pCharacter->SetAnimSpeed(fAnimSpeed);    
}
