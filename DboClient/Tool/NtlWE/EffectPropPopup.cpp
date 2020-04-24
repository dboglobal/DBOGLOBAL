// EffenctPropPopup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "EffectPropPopup.h"


// CEffectPropPopup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEffectPropPopup, CDialog)

CEffectPropPopup::CEffectPropPopup(CWnd* pParent /*=NULL*/)
	: CDialog(CEffectPropPopup::IDD, pParent)
	, m_strVisibleDist(_T(""))
	, m_strName(_T(""))
{

}

CEffectPropPopup::~CEffectPropPopup()
{
}

void CEffectPropPopup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_VISIBLE_DIST, m_strVisibleDist);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Control(pDX, IDC_MINIMAP_LAYER, m_cbMinimapLayer);
}


BEGIN_MESSAGE_MAP(CEffectPropPopup, CDialog)
	ON_BN_CLICKED(IDOK, &CEffectPropPopup::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEffectPropPopup::OnBnClickedCancel)
END_MESSAGE_MAP()


// CEffectPropPopup 메시지 처리기입니다.

BOOL CEffectPropPopup::OnInitDialog()
{
	CDialog::OnInitDialog();

	CNtlInstanceEffect* pEffect = dGETHDL()->GetCurEffect();

	if (pEffect)
	{
		m_strVisibleDist.Format("%.2f", pEffect->GetVisibleCullingDistance());
		m_strName.Format("%s", pEffect->GetName());

		m_cbMinimapLayer.ResetContent();
		m_cbMinimapLayer.AddString("LAYER_ONE");
		m_cbMinimapLayer.AddString("LAYER_TWO");

		m_cbMinimapLayer.SetCheck(0, pEffect->GetMinimapLayer() & NTL_PLEFLAG_MINIMAP_LAYER_ONE);
		m_cbMinimapLayer.SetCheck(1, pEffect->GetMinimapLayer() & NTL_PLEFLAG_MINIMAP_LAYER_TWO);
	}
	else
	{
		OnBnClickedCancel();
	}

	UpdateData(FALSE);

	return TRUE;
}

BOOL CEffectPropPopup::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			{
				return true;
				break;
			}
		case VK_RETURN:
			{
				OnBnClickedOk();
				return true;
				break;
			}
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CEffectPropPopup::OnBnClickedOk()
{
	UpdateData(TRUE);

	CNtlInstanceEffect* pEffect = dGETHDL()->GetCurEffect();

	if (pEffect)
	{
		RwReal fVisibleDist = static_cast<RwReal>(::atof(m_strVisibleDist.GetBuffer(0)));
		CLAMP(fVisibleDist, 0, 512);
		pEffect->SetVisibleCullingDistance(fVisibleDist);

		RwUInt32 uiMinimapLayer = 0x00000000;
		if (m_cbMinimapLayer.GetCheck(0))
		{
			uiMinimapLayer |= NTL_PLEFLAG_MINIMAP_LAYER_ONE;
		}
		if (m_cbMinimapLayer.GetCheck(1))
		{
			uiMinimapLayer |= NTL_PLEFLAG_MINIMAP_LAYER_TWO;
		}
		pEffect->SetMinimapLayer(uiMinimapLayer);
	}

	OnOK();
}

void CEffectPropPopup::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}