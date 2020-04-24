// PostEffectDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Venus.h"
#include "PostEffectDialog.h"

#include "VenusCamera.h"

// CPostEffectDialog dialog

IMPLEMENT_DYNAMIC(CPostEffectDialog, CDialog)

CPostEffectDialog::CPostEffectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPostEffectDialog::IDD, pParent)
{

}

CPostEffectDialog::~CPostEffectDialog()
{
}

void CPostEffectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILTER_ENABLE, m_btnFilterEnable);
	DDX_Control(pDX, IDC_FILTER_SLIDER_POWER_HEIGHT, m_sliderPowerHeight);
	DDX_Control(pDX, IDC_FILTER_SLIDER_SPACE_HEIGHT, m_sliderSpaceHeight);
	DDX_Text(pDX, IDC_FILTER_EDIT_POWER_HEIGHT, m_strPowerHeight);
	DDX_Text(pDX, IDC_FILTER_EDIT_SPACE_HEIGHT, m_strSpaceHeight);
}


BEGIN_MESSAGE_MAP(CPostEffectDialog, CDialog)
	ON_BN_CLICKED(IDC_FILTER_ENABLE, &CPostEffectDialog::OnBnClickedFilterEnable)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CPostEffectDialog message handlers

// CLightDialog 메시지 처리기입니다.
CPostEffectDialog& CPostEffectDialog::GetInstance()
{
	static CPostEffectDialog dlg;
	return dlg;
}
BOOL CPostEffectDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rect;
	GetWindowRect(&rect);
	CPoint posStart;
	posStart.x = GetSystemMetrics(SM_CXSCREEN) / 2 - rect.Width() / 2;
	posStart.y = GetSystemMetrics(SM_CYSCREEN) / 2 - rect.Height() / 2;

	rect.left += posStart.x;
	rect.right += posStart.x;
	rect.top += posStart.y;
	rect.bottom += posStart.y;
	MoveWindow(rect);

    RwUInt8 byFilters = CNtlPostEffectCamera::GetPostEffectFilters();
    if(byFilters == POST_EFFECT_FILTER_HDR)
    {
	    m_btnFilterEnable.SetCheck(TRUE);
    }
    else
    {
        m_btnFilterEnable.SetCheck(FALSE);
    }


	InitSliderButton(&m_sliderPowerHeight, 0, 100, 5, RGB(255, 0, 0));
	m_sliderPowerHeight.SetPos((int)(CNtlPostEffectCamera::GetInstance()->m_fMonoPowerHeight));
	m_strPowerHeight.Format("%f", CNtlPostEffectCamera::GetInstance()->m_fMonoPowerHeight);

	InitSliderButton(&m_sliderSpaceHeight, 0, 100, 5, RGB(0, 255, 0));
	m_sliderSpaceHeight.SetPos((int)(CNtlPostEffectCamera::GetInstance()->m_vRGB2Y.y * 20.f) + 20);
	m_strSpaceHeight.Format("%f", CNtlPostEffectCamera::GetInstance()->m_vRGB2Y.y);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPostEffectDialog::InitSliderButton(CMacSliderCtrl* pSlider, int nStart, int nEnd, int nTic, COLORREF color)
{
	pSlider->SetRange(nStart, nEnd);
	pSlider->SetTicFreq(nTic);
	pSlider->SetChannelColor(color);
}

void CPostEffectDialog::OnBnClickedFilterEnable()
{
	if(m_btnFilterEnable.GetCheck())
    {
        CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_HDR);
    }
    else
    {
        CNtlPostEffectCamera::SetPostEffectFilters(POST_EFFECT_FILTER_NONE);
    }
}

void CPostEffectDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	switch(pScrollBar->GetDlgCtrlID())
	{
		case IDC_FILTER_SLIDER_POWER_HEIGHT:
			CNtlPostEffectCamera::GetInstance()->m_fMonoPowerHeight = (float)m_sliderPowerHeight.GetPos();
			m_strPowerHeight.Format("%f", CNtlPostEffectCamera::GetInstance()->m_fMonoPowerHeight);
			UpdateData(FALSE);
			break;
		case IDC_FILTER_SLIDER_SPACE_HEIGHT:
			CNtlPostEffectCamera::GetInstance()->m_vRGB2Y.y = (float)(m_sliderSpaceHeight.GetPos() - 20) / 20.f;
			m_strSpaceHeight.Format("%f", CNtlPostEffectCamera::GetInstance()->m_vRGB2Y.y);
			UpdateData(FALSE);
			break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
