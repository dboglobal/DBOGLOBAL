// BottomToolView.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool.h"
#include "BottomToolView.h"
#include "ModelToolApplication.h"
#include "NtlPLEntityRenderHelpers.h"


// CBottomToolView

CBottomToolView* CBottomToolView::m_Instance = NULL;

IMPLEMENT_DYNCREATE(CBottomToolView, CFormView)

CBottomToolView::CBottomToolView()
	: CFormView(CBottomToolView::IDD)
    , m_sBoneName(_T(""))    
    , m_fBoneLength(0)
    , m_fBoneWidth(0)
    , m_fBoneMove(0)
    , m_fSensitive(0)
{
    m_Instance = this;
    m_nSelectBoneIndex = 0;
    m_pProperty = NULL;
    m_pCharacter = NULL;
    m_eBoneEditMode = BONE_EDIT_NONE;
}

CBottomToolView::~CBottomToolView()
{
}

void CBottomToolView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CK_BONE_EDIT, m_ckEnableBoneEdit);
    DDX_Text(pDX, IDC_EDIT13, m_sBoneName);    
    DDX_Control(pDX, IDC_ED_BONE_LENGTH, m_edBoneLength);
    DDX_Control(pDX, IDC_ED_BONE_WIDTH, m_edBoneWidth);
    DDX_Control(pDX, IDC_BONE_MOVE, m_edBoneMove);
    DDX_Control(pDX, IDC_ET_SENSE, m_edBoneSensitive);
    DDX_Control(pDX, IDC_SPIN1, m_spBoneLength);
    DDX_Control(pDX, IDC_SPIN2, m_spBoneWidth);
    DDX_Control(pDX, IDC_SPIN3, m_spBoneMove);
    DDX_Control(pDX, IDC_SPIN4, m_spBoneSensitive);
    DDX_Control(pDX, IDC_CK_BONE_EDIT2, m_ckEnableBaseScale);
    DDX_Control(pDX, IDC_ED_BONE_LENGTH2, m_edBoneBaseScale);
    DDX_Control(pDX, IDC_SPIN5, m_spBaseBoneScale);
}

BEGIN_MESSAGE_MAP(CBottomToolView, CFormView)
    ON_BN_CLICKED(IDC_CK_BONE_EDIT, &CBottomToolView::OnBnClickedCkBoneEdit)
    ON_BN_CLICKED(IDC_BT_BONE_PREV, &CBottomToolView::OnBnClickedBtBonePrev)
    ON_BN_CLICKED(IDC_BT_BONE_NEXT, &CBottomToolView::OnBnClickedBtBoneNext)
    ON_WM_KEYDOWN()
    ON_EN_CHANGE(IDC_ED_BONE_LENGTH, &CBottomToolView::OnEnChangeEdBoneLength)
    ON_EN_CHANGE(IDC_ED_BONE_WIDTH, &CBottomToolView::OnEnChangeEdBoneWidth)
    ON_EN_CHANGE(IDC_BONE_MOVE, &CBottomToolView::OnEnChangeBoneMove)
    ON_EN_CHANGE(IDC_ET_SENSE, &CBottomToolView::OnEnChangeEtSense)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CBottomToolView::OnDeltaposSpin1)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CBottomToolView::OnDeltaposSpin2)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CBottomToolView::OnDeltaposSpin3)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CBottomToolView::OnDeltaposSpin4)
    ON_BN_CLICKED(IDC_CK_BONE_EDIT2, &CBottomToolView::OnBnClickedCkBoneEdit2)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CBottomToolView::OnDeltaposSpin5)
END_MESSAGE_MAP()


// CBottomToolView diagnostics

#ifdef _DEBUG
void CBottomToolView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CBottomToolView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

void CBottomToolView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();

    // 툴팁 적용
    m_ToolTip.Create(this);
    m_ckEnableBoneEdit.EnableToolTips(TRUE);
    m_ckEnableBaseScale.EnableToolTips(TRUE);
    m_spBoneLength.EnableToolTips(TRUE);
    m_spBoneWidth.EnableToolTips(TRUE);
    m_spBoneMove.EnableToolTips(TRUE);
    m_spBaseBoneScale.EnableToolTips(TRUE);
    m_spBoneSensitive.EnableToolTips(TRUE);
    m_ToolTip.AddTool(&m_ckEnableBoneEdit, L"Set On/Off of Bone Scale");
    m_ToolTip.AddTool(&m_ckEnableBaseScale, L"Set On/Off of Base Scale");
    m_ToolTip.AddTool(&m_spBoneLength, L"Control Length of Selected Bone");
    m_ToolTip.AddTool(&m_spBoneWidth, L"Control Width of Selected Bone");
    m_ToolTip.AddTool(&m_spBoneMove, L"Control Offset of Land to Model");
    m_ToolTip.AddTool(&m_spBaseBoneScale, L"Control Base Scale of Model");
    m_ToolTip.AddTool(&m_spBoneSensitive, L"Control Sensitive for Control");
}

void CBottomToolView::SetInit(CMTCharacter* pCharacter)
{
    USES_CONVERSION;

    if(pCharacter == NULL)
    {
        // Bone Edit기능을 Disable 시킨다
        SetBoneEditEnable(FALSE);        
    }
    else
    {        
        m_pCharacter = pCharacter;

        // Property 설정
        m_pProperty = pCharacter->GetProperty();

        // UI 초기화 작업
        OnSelectBone(1);  
        m_fSensitive = 0.1f;    
        CString sBaseScale;
        sBaseScale.Format(L"%.2f", m_pCharacter->GetBaseScale());
        m_edBoneBaseScale.SetWindowText(sBaseScale);
        SetBoneEditEnable(FALSE); // Anim이 설정되기 전까지는 FALSE로 둔다.
    }
}

void CBottomToolView::SetBoneEditEnable(BOOL bEnable)
{
    if(!m_pCharacter || !m_pProperty || bEnable == FALSE)
    {
        m_ckEnableBoneEdit.SetCheck(FALSE);
        m_ckEnableBaseScale.SetCheck(FALSE);
        OnBnClickedCkBoneEdit();
        OnBnClickedCkBoneEdit2();
        m_ckEnableBoneEdit.EnableWindow(FALSE);
        m_ckEnableBaseScale.EnableWindow(FALSE);
    }
    else
    {
        m_ckEnableBoneEdit.EnableWindow(TRUE);
        m_ckEnableBaseScale.EnableWindow(TRUE);
        RwBool bBoneScale = m_pProperty->GetBoneScaleData()->bBoneScale;
        m_ckEnableBoneEdit.SetCheck(bBoneScale);
        RwBool bBaseScale = m_pProperty->GetBoneScaleData()->bBaseScale;
        m_ckEnableBaseScale.SetCheck(bBaseScale);
        OnBnClickedCkBoneEdit();
        OnBnClickedCkBoneEdit2();
    }
}

void CBottomToolView::OnSelectBone(int nBoneIndex)
{
    USES_CONVERSION;

    if(nBoneIndex <= 0 || !m_pCharacter)
        return;

    m_nSelectBoneIndex = nBoneIndex;
    m_pCharacter->SetSelectBone(nBoneIndex);
    
    // Bone 정보를 표시한다.
    m_sBoneName = A2W(m_pCharacter->GetBoneName(nBoneIndex));
    STypeBoneData* pBoneData = m_pProperty->GetBoneScaleData();

	//Data가 변경 되어서 수정 합니다 (2006.4.27 HoHoDong)
	m_fBoneLength = pBoneData->BoneScale[nBoneIndex].fLength;
	m_fBoneWidth  = pBoneData->BoneScale[nBoneIndex].fWidth;    

    //m_fBoneLength = pBoneData[nBoneIndex].fLength;
    //m_fBoneWidth  = pBoneData[nBoneIndex].fWidth;    

    WCHAR buf[12] = {0,};
    swprintf(buf, L"%.1f", m_fBoneLength);    
    m_edBoneLength.SetWindowText(buf);

    swprintf(buf, L"%.1f", m_fBoneWidth);
    m_edBoneWidth.SetWindowText(buf);

    swprintf(buf, L"%.1f", pBoneData->vPosOffset.y);
    m_edBoneMove.SetWindowText(buf);

    swprintf(buf, L"%.1f", m_fSensitive);
    m_edBoneSensitive.SetWindowText(buf);    

    UpdateData(FALSE);

}

void CBottomToolView::OnBnClickedCkBoneEdit()
{
    // Bone을 Edit할지 여부를 선택한다.
    // Bone을 Edit할때만 Bone을 렌더링한다.
    BOOL bEnable = m_ckEnableBoneEdit.GetCheck();    

    GetDlgItem(IDC_BT_BONE_PREV)->EnableWindow(bEnable);
    GetDlgItem(IDC_BT_BONE_NEXT)->EnableWindow(bEnable);    
    GetDlgItem(IDC_ET_SENSE)->EnableWindow(bEnable);
    m_edBoneLength.EnableWindow(bEnable);
    m_edBoneWidth.EnableWindow(bEnable);
    m_edBoneMove.EnableWindow(bEnable);
    m_edBoneSensitive.EnableWindow(bEnable);
    m_spBoneLength.EnableWindow(bEnable);
    m_spBoneWidth.EnableWindow(bEnable);
    m_spBoneMove.EnableWindow(bEnable);
    m_spBoneSensitive.EnableWindow(bEnable);

    // 캐릭터에도 설정한다.        
    if(m_pCharacter && m_pProperty)
    {
        m_pCharacter->SetRenderBone(bEnable); 
        m_pProperty->GetBoneScaleData()->bBoneScale = bEnable;
    }  

    
}

void CBottomToolView::OnBnClickedCkBoneEdit2()
{
    // Base Scale 설정을 On/Off 한다.
    BOOL bEnable = m_ckEnableBaseScale.GetCheck();
    
    m_edBoneBaseScale.EnableWindow(bEnable);
    m_spBaseBoneScale.EnableWindow(bEnable);

    if(m_pProperty)
    {
        m_pProperty->GetBoneScaleData()->bBaseScale = bEnable;
        
        // 캐릭터 클래스 자체에서 못해주기 때문에 여기서 해준다.
        
        RwV3d vZeroPos;
        ZeroMemory(&vZeroPos, sizeof(vZeroPos));

        m_pCharacter->SetPosition(&vZeroPos);
        
    }
}


void CBottomToolView::OnBnClickedBtBonePrev()
{
    if(!m_pCharacter)
        return;

    if(m_nSelectBoneIndex == 1)
    {
        m_nSelectBoneIndex = m_pCharacter->GetBoneNum() - 1;
    }
    else
    {
        --m_nSelectBoneIndex;        
    }

    OnSelectBone(m_nSelectBoneIndex);
}

void CBottomToolView::OnBnClickedBtBoneNext()
{
    if(!m_pCharacter)
        return;

    ++m_nSelectBoneIndex;

    if(m_nSelectBoneIndex == m_pCharacter->GetBoneNum())
    {
        m_nSelectBoneIndex = 1;
    }

    OnSelectBone(m_nSelectBoneIndex);
}


void CBottomToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // 키보드 입력으로 Bone을 변경한다.
    if(m_ckEnableBoneEdit.GetCheck() == FALSE)
        return;

    if(m_pProperty)
    {
        switch(nChar)
        {
        case VK_INSERT:
			OnChangeBoneLength(TRUE);
            break;
        case VK_DELETE:
            OnChangeBoneLength(FALSE);
            break;        
        case VK_HOME:
            OnChangeBoneWidth(TRUE);
            break;
        case VK_END:
            OnChangeBoneWidth(FALSE);
            break;
        case VK_PRIOR:
            OnChangeBoneMove(TRUE);
            break;
        case VK_NEXT:
            OnChangeBoneMove(FALSE);
            break;
        }
    }

    CFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}

LRESULT CBottomToolView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    return CFormView::WindowProc(message, wParam, lParam);
}

BOOL CBottomToolView::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_MOUSEMOVE)
    {
        // 툴팁을 적용한다.
        m_ToolTip.RelayEvent(pMsg);
    }
    WindowProc(pMsg->message, pMsg->wParam, pMsg->lParam);
    return CFormView::PreTranslateMessage(pMsg);
}

void CBottomToolView::OnEnChangeEdBoneLength()
{
    STypeBoneData* pBoneData = m_pProperty->GetBoneScaleData();
    if(pBoneData)
    {
        CString sBoneLength;
        m_edBoneLength.GetWindowText(sBoneLength);
		//BoneData가 수정되어서 수정함(2006.4.26 HongHoDong)
        pBoneData->BoneScale[m_nSelectBoneIndex].fLength = (RwReal)_wtof(sBoneLength);
    }
}

void CBottomToolView::OnEnChangeEdBoneWidth()
{
    STypeBoneData* pBoneData = m_pProperty->GetBoneScaleData();
    if(pBoneData)
    {
        CString sBoneWidth;
        m_edBoneWidth.GetWindowText(sBoneWidth);
		//BoneData가 수정되어서 수정함(2006.4.26 HongHoDong)
        pBoneData->BoneScale[m_nSelectBoneIndex].fWidth= (RwReal)_wtof(sBoneWidth);
    }
    
}

void CBottomToolView::OnEnChangeBoneMove()
{
    STypeBoneData* pBoneData = m_pProperty->GetBoneScaleData();
    if(pBoneData)
    {
        CString sBoneMove;
        m_edBoneMove.GetWindowText(sBoneMove);        
        RwV3d vPosOffset = pBoneData->vPosOffset;
        vPosOffset.y = (RwReal)_wtof(sBoneMove);        
        m_pCharacter->SetPosOffset(&vPosOffset);
    }    
}

void CBottomToolView::OnEnChangeEtSense()
{
    CString sBoneSensitive;
    m_edBoneSensitive.GetWindowText(sBoneSensitive);
    m_fSensitive = (float)_wtof(sBoneSensitive);
}

void CBottomToolView::OnChangeBoneLength(BOOL bPlus)
{
    WCHAR buf[12] = {0,};
    STypeBoneData* pBoneData = m_pProperty->GetBoneScaleData();

    if(bPlus)
    {
        pBoneData->BoneScale[m_nSelectBoneIndex].fLength += m_fSensitive;
    }
    else
    {
        pBoneData->BoneScale[m_nSelectBoneIndex].fLength -= m_fSensitive;
    }
    
    swprintf(buf, L"%.1f", pBoneData->BoneScale[m_nSelectBoneIndex].fLength);
    m_edBoneLength.SetWindowText(buf);          
}

void CBottomToolView::OnChangeBoneWidth(BOOL bPlus)
{
    WCHAR buf[12] = {0,};
    STypeBoneData* pBoneData = m_pProperty->GetBoneScaleData();

    if(bPlus)
    {
        pBoneData->BoneScale[m_nSelectBoneIndex].fWidth += m_fSensitive;
    }
    else
    {
        pBoneData->BoneScale[m_nSelectBoneIndex].fWidth -= m_fSensitive;
    }
    
    swprintf(buf, L"%.1f", pBoneData->BoneScale[m_nSelectBoneIndex].fWidth);
    m_edBoneWidth.SetWindowText(buf);            
}

void CBottomToolView::OnChangeBoneMove(BOOL bPlus)
{
    WCHAR buf[12] = {0,};
    STypeBoneData* pBoneData = m_pProperty->GetBoneScaleData();

	RwV3d vPos = pBoneData->vPosOffset;

    if(bPlus)
    {
        vPos.y += m_fSensitive;
        
    }
    else
    {
        vPos.y -= m_fSensitive;
    }

    m_pCharacter->SetPosOffset(&vPos);
    
    swprintf(buf, L"%.1f", vPos.y);
    m_edBoneMove.SetWindowText(buf);
}

void CBottomToolView::OnChangeSensitive(BOOL bPlus)
{
    WCHAR buf[12] = {0,};
    if(bPlus)
    {
        m_fSensitive += 0.1f;
    }
    else
    {
        m_fSensitive -= 0.1f;
    }
    swprintf(buf, L"%.1f", m_fSensitive);
    m_edBoneSensitive.SetWindowText(buf);
}

void CBottomToolView::OnChangeBaseScale(BOOL bPlus)
{
    if(!m_pCharacter)
        return;

    WCHAR buf[12] = {0,};
    RwReal fBaseScale = m_pCharacter->GetBaseScale();
    if(bPlus)
    {
        fBaseScale += m_fSensitive;
    }
    else
    {
        fBaseScale -= m_fSensitive;
    }
    m_pCharacter->SetBaseScale(fBaseScale);
    swprintf(buf, L"%.1f", fBaseScale);
    m_edBoneBaseScale.SetWindowText(buf);
}

void CBottomToolView::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    if(pNMUpDown->iDelta < 0)
    {
        OnChangeBoneLength(TRUE);
    }
    else
    {
        OnChangeBoneLength(FALSE);
    }
    *pResult = 0;
}


void CBottomToolView::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    if(pNMUpDown->iDelta < 0)
    {
        OnChangeBoneWidth(TRUE);
    }
    else
    {
        OnChangeBoneWidth(FALSE);
    }
    *pResult = 0;
}

void CBottomToolView::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    if(pNMUpDown->iDelta < 0)
    {
        OnChangeBoneMove(TRUE);
    }
    else
    {
        OnChangeBoneMove(FALSE);
    }

    *pResult = 0;
}

void CBottomToolView::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    if(pNMUpDown->iDelta < 0)
    {
        OnChangeSensitive(TRUE);
    }
    else
    {
        OnChangeSensitive(FALSE);
    }    

    *pResult = 0;
}


void CBottomToolView::OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    if(pNMUpDown->iDelta < 0)
    {
        OnChangeBaseScale(TRUE);
    }
    else
    {
        OnChangeBaseScale(FALSE);
    }

    *pResult = 0;
}
