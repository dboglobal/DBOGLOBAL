// DojoPropPopup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "DojoPropPopup.h"

// CDojoPropPopup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDojoPropPopup, CDialog)

CDojoPropPopup::CDojoPropPopup(CWnd* pParent /*=NULL*/)
	: CDialog(CDojoPropPopup::IDD, pParent)
	, m_strDojoID(_T(""))
	, m_strFadeInTime(_T(""))
	, m_strFadeOutTime(_T(""))
	, m_strNameObject(_T(""))
	, m_strNameEffectUp(_T(""))
	, m_strNameEffectDown(_T(""))
{
}

CDojoPropPopup::~CDojoPropPopup()
{
}

void CDojoPropPopup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_FADE_USE, m_btnFadeInUse);
	DDX_Control(pDX, IDC_CHECK_FADE_USE2, m_btnFadeOutUse);
	DDX_Control(pDX, IDC_COMBO_LEVEL, m_cbDojoLevel);
	DDX_Text(pDX, IDC_EDIT_DOJO_ID, m_strDojoID);
	DDX_Text(pDX, IDC_EDIT_FADE_TIME, m_strFadeInTime);
	DDX_Text(pDX, IDC_EDIT_FADE_TIME2, m_strFadeOutTime);
	DDX_Text(pDX, IDC_EDIT_NAME_OBJECT, m_strNameObject);
	DDX_Text(pDX, IDC_EDIT_NAME_EFFECT_UP, m_strNameEffectUp);
	DDX_Text(pDX, IDC_EDIT_NAME_EFFECT_DOWN, m_strNameEffectDown);
}


BEGIN_MESSAGE_MAP(CDojoPropPopup, CDialog)
	ON_BN_CLICKED(IDOK, &CDojoPropPopup::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDojoPropPopup::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_OBJECT, &CDojoPropPopup::OnBnClickedButtonLoadObject)
// 	ON_BN_CLICKED(IDC_BUTTON_LOAD_EFFECT_UP, &CDojoPropPopup::OnBnClickedButtonLoadEffectUp)
// 	ON_BN_CLICKED(IDC_BUTTON_LOAD_EFFECT_DOWN, &CDojoPropPopup::OnBnClickedButtonLoadEffectDown)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CDojoPropPopup::OnBnClickedButtonReset)
// 	ON_BN_CLICKED(IDC_BUTTON_DEL_EFFECT_UP, &CDojoPropPopup::OnBnClickedButtonDelEffectUp)
// 	ON_BN_CLICKED(IDC_BUTTON_DEL_EFFECT_DOWN, &CDojoPropPopup::OnBnClickedButtonDelEffectDown)
	ON_CBN_SELCHANGE(IDC_COMBO_LEVEL, &CDojoPropPopup::OnCbnSelchangeComboLevel)
	ON_CBN_EDITCHANGE(IDC_COMBO_LEVEL, &CDojoPropPopup::OnCbnEditchangeComboLevel)
	ON_BN_CLICKED(IDC_CHECK_FADE_USE, &CDojoPropPopup::OnBnClickedCheckFadeUse)
	ON_BN_CLICKED(IDC_CHECK_FADE_USE2, &CDojoPropPopup::OnBnClickedCheckFadeUse2)
END_MESSAGE_MAP()


// CDojoPropPopup 메시지 처리기입니다.
BOOL CDojoPropPopup::OnInitDialog()
{
	CDialog::OnInitDialog();

	CNtlPLDojo* pPLDojo = dGETHDL()->GetCurDojo();

	if (pPLDojo)
	{
		GetDojoProp(pPLDojo);
	}
	else
	{
		OnBnClickedCancel();
	}

	UpdateData(FALSE);

	return TRUE;
}

BOOL CDojoPropPopup::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		UpdateData(TRUE);
		if (m_cbDojoLevel.GetCurSel() == -1)
		{
			AfxMessageBox("Invalid Level.");
			SetDojoLevel(0);
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_EDIT_NAME_OBJECT)->GetSafeHwnd())
		{
			m_astrNameObject[m_cbDojoLevel.GetCurSel()] = m_strNameObject;			
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_EDIT_NAME_EFFECT_UP)->GetSafeHwnd())
		{
			m_astrNameEffectUp[m_cbDojoLevel.GetCurSel()] = m_strNameEffectUp;
		}
		else if (pMsg->hwnd == GetDlgItem(IDC_EDIT_NAME_EFFECT_DOWN)->GetSafeHwnd())
		{
			m_astrNameEffectDown[m_cbDojoLevel.GetCurSel()] = m_strNameEffectDown;
		}
		UpdateData(FALSE);
	}
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
				return true;
				break;
			}
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CDojoPropPopup::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CNtlPLDojo* pPLDojo = dGETHDL()->GetCurDojo();

	if (pPLDojo)
	{
		SetDojoProp(pPLDojo);
		pPLDojo->RefreshResource();
	}

	OnOK();
}

void CDojoPropPopup::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

void CDojoPropPopup::OnBnClickedButtonLoadObject()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char acPathBK[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acPathBK);

	CFileDialog	fileDlg(TRUE, "Object Load", "*.*",
		OFN_HIDEREADONLY,
		"Data Files (*.*)|*.*||");

	fileDlg.m_ofn.lpstrInitialDir = ".\\";

	if (fileDlg.DoModal() == IDOK)
	{
		m_strNameObject = m_astrNameObject[m_cbDojoLevel.GetCurSel()] = fileDlg.GetFileTitle();
	}
	UpdateData(FALSE);

	::SetCurrentDirectory(acPathBK);
}

void CDojoPropPopup::OnBnClickedButtonLoadEffectUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char acPathBK[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acPathBK);

	CFileDialog	fileDlg(TRUE, "Object Load", "*.*",
		OFN_HIDEREADONLY,
		"Data Files (*.*)|*.*||");

	fileDlg.m_ofn.lpstrInitialDir = ".\\";

	if (fileDlg.DoModal() == IDOK)
	{
		m_strNameEffectUp = m_astrNameEffectUp[m_cbDojoLevel.GetCurSel()] = fileDlg.GetFileTitle();
	}
	UpdateData(FALSE);

	::SetCurrentDirectory(acPathBK);
}

void CDojoPropPopup::OnBnClickedButtonLoadEffectDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char acPathBK[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acPathBK);

	CFileDialog	fileDlg(TRUE, "Object Load", "*.*",
		OFN_HIDEREADONLY,
		"Data Files (*.*)|*.*||");

	fileDlg.m_ofn.lpstrInitialDir = ".\\";

	if (fileDlg.DoModal() == IDOK)
	{
		m_strNameEffectDown = m_astrNameEffectDown[m_cbDojoLevel.GetCurSel()] = fileDlg.GetFileTitle();
	}
	UpdateData(FALSE);

	::SetCurrentDirectory(acPathBK);
}

void CDojoPropPopup::OnBnClickedButtonReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CNtlPLDojo* pPLDojo = dGETHDL()->GetCurDojo();

	if (pPLDojo)
	{
		GetDojoProp(pPLDojo);
	}
}

void CDojoPropPopup::OnBnClickedButtonDelEffectUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_strNameEffectUp = m_astrNameEffectUp[m_cbDojoLevel.GetCurSel()] = "";
}

void CDojoPropPopup::OnBnClickedButtonDelEffectDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_strNameEffectDown = m_astrNameEffectDown[m_cbDojoLevel.GetCurSel()] = "";
}

void CDojoPropPopup::GetDojoProp(CNtlPLDojo *pPLDojo)
{
	m_strDojoID.Format("%d", pPLDojo->GetDojoID());

	m_strFadeInTime.Format("%.2f", pPLDojo->GetFadeInTime());
	m_btnFadeInUse.SetCheck((pPLDojo->GetFadeInTime() > 0.0f));
	GetDlgItem(IDC_EDIT_FADE_TIME)->EnableWindow(m_btnFadeInUse.GetCheck());

	m_strFadeOutTime.Format("%.2f", pPLDojo->GetFadeOutTime());
	m_btnFadeOutUse.SetCheck((pPLDojo->GetFadeOutTime() > 0.0f));
	GetDlgItem(IDC_EDIT_FADE_TIME2)->EnableWindow(m_btnFadeOutUse.GetCheck());

	for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
	{
		m_astrNameObject[i]		= pPLDojo->GetResObjectName(i);
		m_astrNameEffectUp[i]	= pPLDojo->GetResEffectNameUp(i);
		m_astrNameEffectDown[i]	= pPLDojo->GetResEffectNameDown(i);
	}

	m_cbDojoLevel.SetCurSel(0);

	m_strNameObject		= m_astrNameObject[0];
	m_strNameEffectUp	= m_astrNameEffectUp[0];
	m_strNameEffectDown	= m_astrNameEffectDown[0];

	UpdateData(FALSE);
}

void CDojoPropPopup::SetDojoProp(CNtlPLDojo *pPLDojo)
{
	UpdateData(TRUE);

	pPLDojo->SetDojoID(::atoi(m_strDojoID.GetBuffer()));

	if (m_btnFadeInUse.GetCheck())
	{
		pPLDojo->SetFadeInTime(static_cast<RwReal>(::atof(m_strFadeInTime.GetBuffer())));
	}
	else 
	{
		pPLDojo->SetFadeInTime(0.0f);
	}

	if (m_btnFadeOutUse.GetCheck())
	{
		pPLDojo->SetFadeOutTime(static_cast<RwReal>(::atof(m_strFadeOutTime.GetBuffer())));
	}
	else 
	{
		pPLDojo->SetFadeOutTime(0.0f);
	}
	
	for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
	{
		pPLDojo->SetResObjectName(i, m_astrNameObject[i].GetBuffer());
		pPLDojo->SetResEffectNameUp(i, m_astrNameEffectUp[i].GetBuffer());
		pPLDojo->SetResEffectNameDown(i, m_astrNameEffectDown[i].GetBuffer());
	}	
}
void CDojoPropPopup::OnCbnSelchangeComboLevel()
{
	SetDojoLevel(m_cbDojoLevel.GetCurSel());
}

void CDojoPropPopup::OnCbnEditchangeComboLevel()
{
	SetDojoLevel(0);
}

void CDojoPropPopup::SetDojoLevel(RwInt32 iLevel)
{
	m_strNameObject		= m_astrNameObject[iLevel];
	m_strNameEffectUp	= m_astrNameEffectUp[iLevel];
	m_strNameEffectDown	= m_astrNameEffectDown[iLevel];

	UpdateData(FALSE);
}
void CDojoPropPopup::OnBnClickedCheckFadeUse()
{
	m_strFadeInTime = "0.0f";

	GetDlgItem(IDC_EDIT_FADE_TIME)->EnableWindow(FALSE);

	UpdateData(FALSE);
}

void CDojoPropPopup::OnBnClickedCheckFadeUse2()
{
	m_strFadeOutTime = "0.0f";

	GetDlgItem(IDC_EDIT_FADE_TIME2)->EnableWindow(FALSE);

	UpdateData(FALSE);
}
