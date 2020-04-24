// BoneEditPane.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool2.h"
#include "BoneEditPane.h"

CBoneEditPane* CBoneEditPane::m_pInstance = NULL;

// CBoneEditPane

IMPLEMENT_DYNCREATE(CBoneEditPane, CFormView)

CBoneEditPane::CBoneEditPane()
	: CFormView(CBoneEditPane::IDD)
{
    m_pInstance = this;
    m_pCharacter = NULL;
    m_pProperty = NULL;
    m_nSelectBoneIndex = 0;
    m_fSensitive = 0.1f;   
}

CBoneEditPane::~CBoneEditPane()
{
}

void CBoneEditPane::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CK_BONE_EDIT, m_ckEnableBoneEdit);    
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
    DDX_Control(pDX, IDC_CB_BONE, m_cbBoneName);
}

BEGIN_MESSAGE_MAP(CBoneEditPane, CFormView)
    ON_BN_CLICKED(IDC_CK_BONE_EDIT, &CBoneEditPane::OnBnClickedCkBoneEdit)    
    ON_WM_KEYDOWN()
    ON_EN_CHANGE(IDC_ED_BONE_LENGTH, &CBoneEditPane::OnEnChangeEdBoneLength)
    ON_EN_CHANGE(IDC_ED_BONE_WIDTH, &CBoneEditPane::OnEnChangeEdBoneWidth)
    ON_EN_CHANGE(IDC_BONE_MOVE, &CBoneEditPane::OnEnChangeBoneMove)
    ON_EN_CHANGE(IDC_ET_SENSE, &CBoneEditPane::OnEnChangeEtSense)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CBoneEditPane::OnDeltaposSpin1)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CBoneEditPane::OnDeltaposSpin2)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CBoneEditPane::OnDeltaposSpin3)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CBoneEditPane::OnDeltaposSpin4)
    ON_BN_CLICKED(IDC_CK_BONE_EDIT2, &CBoneEditPane::OnBnClickedCkBoneEdit2)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN5, &CBoneEditPane::OnDeltaposSpin5)
    ON_EN_CHANGE(IDC_ED_BONE_LENGTH2, &CBoneEditPane::OnEnChangeEdBoneLength2)
    ON_CBN_SELCHANGE(IDC_CB_BONE, &CBoneEditPane::OnCbnSelchangeCbBone)
END_MESSAGE_MAP()


// CBoneEditPane diagnostics

#ifdef _DEBUG
void CBoneEditPane::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CBoneEditPane::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBoneEditPane message handlers

void CBoneEditPane::OnInitialUpdate()
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
    m_ToolTip.AddTool(&m_ckEnableBoneEdit, "Set On/Off of Bone Scale");
    m_ToolTip.AddTool(&m_ckEnableBaseScale, "Set On/Off of Base Scale");
    m_ToolTip.AddTool(&m_spBoneLength, "Control Length of Selected Bone");
    m_ToolTip.AddTool(&m_spBoneWidth, "Control Width of Selected Bone");
    m_ToolTip.AddTool(&m_spBoneMove, "Control Offset of Land to Model");
    m_ToolTip.AddTool(&m_spBaseBoneScale, "Control Base Scale of Model");
    m_ToolTip.AddTool(&m_spBoneSensitive, "Control Sensitive for Control");    
}

void CBoneEditPane::SetModel(CMTCharacter* pCharacter)
{
    USES_CONVERSION;

    if(!pCharacter)
    {
        m_pCharacter = NULL;
        m_pProperty = NULL;
        
        SetEnable(FALSE);
    }
    else
    {
        m_pCharacter = pCharacter;
        m_pProperty  = pCharacter->GetProperty();

        // UI 초기화 작업        
        m_cbBoneName.ResetContent();        
        for(int i = 0; i < m_pCharacter->GetBoneNum(); ++i)
        {
            CString strBoneName = (m_pCharacter->GetBoneName(i));
            if(strBoneName != "")
            {
                m_cbBoneName.AddString(strBoneName);
            }            
        }
        m_cbBoneName.SetCurSel(0);

        m_fSensitive = 0.1f;    
        CString sBaseScale;
        sBaseScale.Format("%.2f", m_pCharacter->GetBaseScale());
        m_edBoneBaseScale.SetWindowText(sBaseScale);

        // 애니메이션이 설정될때까지는 비활성화로 둔다.
        SetEnable(FALSE);
    }
}

void CBoneEditPane::SetEnable(BOOL bEnable)
{
    if(m_pCharacter && m_pProperty)
    {
        RwBool bBoneScale = m_pProperty->GetBoneScaleData()->bBoneScale;
        m_ckEnableBoneEdit.SetCheck(bBoneScale);
        RwBool bBaseScale = m_pProperty->GetBoneScaleData()->bBaseScale;
        m_ckEnableBaseScale.SetCheck(bBaseScale);
        OnBnClickedCkBoneEdit();
        OnBnClickedCkBoneEdit2();
    }

    if(bEnable)
    {
        m_ckEnableBoneEdit.EnableWindow(TRUE);
        m_ckEnableBaseScale.EnableWindow(TRUE);        
    }
    else
    {
        m_ckEnableBoneEdit.EnableWindow(FALSE);
        m_ckEnableBaseScale.EnableWindow(FALSE);
    }
}

void CBoneEditPane::OnCbnSelchangeCbBone()
{
    m_nSelectBoneIndex = m_cbBoneName.GetCurSel();
    if(m_nSelectBoneIndex > m_cbBoneName.GetCount())
        return;

    if(!m_pCharacter->SetSelectBone(m_nSelectBoneIndex))
        return;

    STypeBoneData* pBoneData = m_pProperty->GetBoneScaleData();

    //Data가 변경 되어서 수정 합니다 (2006.4.27 HoHoDong)
    m_fBoneLength = pBoneData->BoneScale[m_nSelectBoneIndex].fLength;
    m_fBoneWidth  = pBoneData->BoneScale[m_nSelectBoneIndex].fWidth;    

    char buf[12] = {0,};
	sprintf(buf, "%.2f", m_fBoneLength);
    m_edBoneLength.SetWindowText(buf);

	sprintf(buf, "%.2f", m_fBoneWidth);
    m_edBoneWidth.SetWindowText(buf);

	sprintf(buf, "%.2f", pBoneData->vPosOffset.y);
    m_edBoneMove.SetWindowText(buf);
}

void CBoneEditPane::OnBnClickedCkBoneEdit()
{
    // Bone을 Edit할지 여부를 선택한다.
    // Bone을 Edit할때만 Bone을 렌더링한다.
    BOOL bEnable = m_ckEnableBoneEdit.GetCheck();    

    m_cbBoneName.EnableWindow(bEnable);
    //GetDlgItem(IDC_ET_SENSE)->EnableWindow(bEnable);
    m_edBoneLength.EnableWindow(bEnable);
    m_edBoneWidth.EnableWindow(bEnable);
    m_edBoneMove.EnableWindow(bEnable);
    //m_edBoneSensitive.EnableWindow(bEnable);
    m_spBoneLength.EnableWindow(bEnable);
    m_spBoneWidth.EnableWindow(bEnable);
    m_spBoneMove.EnableWindow(bEnable);
    //m_spBoneSensitive.EnableWindow(bEnable);

    // 캐릭터에도 설정한다.        
    if(m_pCharacter && m_pProperty)
    {
        m_pCharacter->SetRenderBone(bEnable); 
        m_pProperty->GetBoneScaleData()->bBoneScale = bEnable;
    }  
}

void CBoneEditPane::OnBnClickedCkBoneEdit2()
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

void CBoneEditPane::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    // 키보드 입력으로 Bone을 변경한다.
    if(m_ckEnableBoneEdit.GetCheck() == FALSE || !m_pCharacter || !m_pProperty)
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


BOOL CBoneEditPane::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message == WM_MOUSEMOVE)
    {
        // 툴팁을 적용한다.
        m_ToolTip.RelayEvent(pMsg);
    }
    WindowProc(pMsg->message, pMsg->wParam, pMsg->lParam);
    return CFormView::PreTranslateMessage(pMsg);
}

void CBoneEditPane::OnEnChangeEdBoneLength()
{
    STypeBoneData* pBoneData = m_pProperty->GetBoneScaleData();
    if(pBoneData)
    {
        CString sBoneLength;
        m_edBoneLength.GetWindowText(sBoneLength);
        //BoneData가 수정되어서 수정함(2006.4.26 HongHoDong)
        pBoneData->BoneScale[m_nSelectBoneIndex].fLength = (RwReal)atof(sBoneLength);
    }
}

void CBoneEditPane::OnEnChangeEdBoneWidth()
{
    STypeBoneData* pBoneData = m_pProperty->GetBoneScaleData();
    if(pBoneData)
    {
        CString sBoneWidth;
        m_edBoneWidth.GetWindowText(sBoneWidth);
        //BoneData가 수정되어서 수정함(2006.4.26 HongHoDong)
        pBoneData->BoneScale[m_nSelectBoneIndex].fWidth= (RwReal)atof(sBoneWidth);
    }

}

void CBoneEditPane::OnEnChangeBoneMove()
{
    STypeBoneData* pBoneData = m_pProperty->GetBoneScaleData();
    if(pBoneData)
    {
        CString sBoneMove;
        m_edBoneMove.GetWindowText(sBoneMove);        
        RwV3d vPosOffset = pBoneData->vPosOffset;
        vPosOffset.y = (RwReal)atof(sBoneMove);
        m_pCharacter->SetPosOffset(&vPosOffset);
    }    
}

void CBoneEditPane::OnEnChangeEtSense()
{
    CString sBoneSensitive;
    m_edBoneSensitive.GetWindowText(sBoneSensitive);
    m_fSensitive = (float)atof(sBoneSensitive);
}

void CBoneEditPane::OnChangeBoneLength(BOOL bPlus)
{
    char buf[12] = {0,};
    STypeBoneData* pBoneData = m_pProperty->GetBoneScaleData();

    if(bPlus)
    {
        pBoneData->BoneScale[m_nSelectBoneIndex].fLength += m_fSensitive;
    }
    else
    {
        pBoneData->BoneScale[m_nSelectBoneIndex].fLength -= m_fSensitive;
    }

	sprintf(buf, "%.2f", pBoneData->BoneScale[m_nSelectBoneIndex].fLength);
    m_edBoneLength.SetWindowText(buf);          
}

void CBoneEditPane::OnChangeBoneWidth(BOOL bPlus)
{
    char buf[12] = {0,};
    STypeBoneData* pBoneData = m_pProperty->GetBoneScaleData();

    if(bPlus)
    {
        pBoneData->BoneScale[m_nSelectBoneIndex].fWidth += m_fSensitive;
    }
    else
    {
        pBoneData->BoneScale[m_nSelectBoneIndex].fWidth -= m_fSensitive;
    }

    sprintf(buf, "%.2f", pBoneData->BoneScale[m_nSelectBoneIndex].fWidth);
	m_edBoneWidth.SetWindowText(buf);
}

void CBoneEditPane::OnChangeBoneMove(BOOL bPlus)
{
    char buf[12] = {0,};
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

	sprintf(buf, "%.2f", vPos.y);
    m_edBoneMove.SetWindowText(buf);
}

void CBoneEditPane::OnChangeSensitive(BOOL bPlus)
{
    char buf[12] = {0,};
    if(bPlus)
    {
        m_fSensitive += 0.01f;
    }
    else
    {
        m_fSensitive -= 0.01f;
    }
	sprintf(buf, "%.2f", m_fSensitive);
    m_edBoneSensitive.SetWindowText(buf);
}

void CBoneEditPane::OnChangeBaseScale(BOOL bPlus)
{
    if(!m_pCharacter)
        return;

    char buf[12] = {0,};
    RwReal fBaseScale = m_pCharacter->GetBaseScale();
    if(bPlus)
    {
        fBaseScale += m_fSensitive;
    }
    else
    {
        fBaseScale -= m_fSensitive;
    }
    //m_pCharacter->SetBaseScale(fBaseScale);
	sprintf(buf, "%.2f", fBaseScale);
    m_edBoneBaseScale.SetWindowText(buf);
}

void CBoneEditPane::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
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


void CBoneEditPane::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
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

void CBoneEditPane::OnDeltaposSpin3(NMHDR *pNMHDR, LRESULT *pResult)
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

void CBoneEditPane::OnDeltaposSpin4(NMHDR *pNMHDR, LRESULT *pResult)
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


void CBoneEditPane::OnDeltaposSpin5(NMHDR *pNMHDR, LRESULT *pResult)
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

void CBoneEditPane::OnEnChangeEdBoneLength2()
{
    if(!m_pCharacter)
        return;

    CString strValue;
    m_edBoneBaseScale.GetWindowText(strValue);

    float fBaseScale = (float)atof(strValue);
    m_pCharacter->SetBaseScale(fBaseScale);    
}

