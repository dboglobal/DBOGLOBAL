// GamePropVisibleDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "GamePropVisibleDlg.h"


// CGamePropVisibleDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CGamePropVisibleDlg, CDialog)

CGamePropVisibleDlg::CGamePropVisibleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGamePropVisibleDlg::IDD, pParent)
	,m_uiFlags(0x00000000)
{
}

CGamePropVisibleDlg::~CGamePropVisibleDlg()
{
}

void CGamePropVisibleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGamePropVisibleDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CGamePropVisibleDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGamePropVisibleDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECK_PROP1, &CGamePropVisibleDlg::OnBnClickedCheckProp1)
	ON_BN_CLICKED(IDC_CHECK_PROP2, &CGamePropVisibleDlg::OnBnClickedCheckProp2)
	ON_BN_CLICKED(IDC_CHECK_PROP3, &CGamePropVisibleDlg::OnBnClickedCheckProp3)
	ON_BN_CLICKED(IDC_CHECK_PROP4, &CGamePropVisibleDlg::OnBnClickedCheckProp4)
	ON_BN_CLICKED(IDC_CHECK_PROP5, &CGamePropVisibleDlg::OnBnClickedCheckProp5)
	ON_BN_CLICKED(IDC_CHECK_PROP6, &CGamePropVisibleDlg::OnBnClickedCheckProp6)
	ON_BN_CLICKED(IDC_CHECK_PROP7, &CGamePropVisibleDlg::OnBnClickedCheckProp7)
	ON_BN_CLICKED(IDC_CHECK_PROP8, &CGamePropVisibleDlg::OnBnClickedCheckProp8)
	ON_BN_CLICKED(IDC_CHECK_PROP9, &CGamePropVisibleDlg::OnBnClickedCheckProp9)
	ON_BN_CLICKED(IDC_CHECK_PROP10, &CGamePropVisibleDlg::OnBnClickedCheckProp10)
	ON_BN_CLICKED(IDC_CHECK_PROP11, &CGamePropVisibleDlg::OnBnClickedCheckProp11)
	ON_BN_CLICKED(IDC_CHECK_PROP12, &CGamePropVisibleDlg::OnBnClickedCheckProp12)
	ON_BN_CLICKED(IDC_CHECK_PROP13, &CGamePropVisibleDlg::OnBnClickedCheckProp13)
	ON_BN_CLICKED(IDC_CHECK_PROP14, &CGamePropVisibleDlg::OnBnClickedCheckProp14)
	ON_BN_CLICKED(IDC_CHECK_PROP15, &CGamePropVisibleDlg::OnBnClickedCheckProp15)
	ON_BN_CLICKED(IDC_CHECK_PROP16, &CGamePropVisibleDlg::OnBnClickedCheckProp16)
END_MESSAGE_MAP()


// CGamePropVisibleDlg 메시지 처리기입니다.

LRESULT CGamePropVisibleDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			SetFlags(0x00000000);
			UpdateUI();
			break;
		}

	case WM_SHOWWINDOW:
		{
			SetFlags(0x00000000);
			UpdateUI();
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CGamePropVisibleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CNtlWorldPropToolData* pWorldPropToolData = &CNtlWEApplication::GetInstance()->m_WorldPropToolData;

	((CButton*)(GetDlgItem(IDC_CHECK_PROP1)))->SetWindowText(pWorldPropToolData->GetPropName(0));
	((CButton*)(GetDlgItem(IDC_CHECK_PROP2)))->SetWindowText(pWorldPropToolData->GetPropName(1));
	((CButton*)(GetDlgItem(IDC_CHECK_PROP3)))->SetWindowText(pWorldPropToolData->GetPropName(2));
	((CButton*)(GetDlgItem(IDC_CHECK_PROP4)))->SetWindowText(pWorldPropToolData->GetPropName(3));
	((CButton*)(GetDlgItem(IDC_CHECK_PROP5)))->SetWindowText(pWorldPropToolData->GetPropName(4));
	((CButton*)(GetDlgItem(IDC_CHECK_PROP6)))->SetWindowText(pWorldPropToolData->GetPropName(5));
	((CButton*)(GetDlgItem(IDC_CHECK_PROP7)))->SetWindowText(pWorldPropToolData->GetPropName(6));
	((CButton*)(GetDlgItem(IDC_CHECK_PROP8)))->SetWindowText(pWorldPropToolData->GetPropName(7));
	((CButton*)(GetDlgItem(IDC_CHECK_PROP9)))->SetWindowText(pWorldPropToolData->GetPropName(8));
	((CButton*)(GetDlgItem(IDC_CHECK_PROP10)))->SetWindowText(pWorldPropToolData->GetPropName(9));
	((CButton*)(GetDlgItem(IDC_CHECK_PROP11)))->SetWindowText(pWorldPropToolData->GetPropName(10));
	((CButton*)(GetDlgItem(IDC_CHECK_PROP12)))->SetWindowText(pWorldPropToolData->GetPropName(11));
	((CButton*)(GetDlgItem(IDC_CHECK_PROP13)))->SetWindowText(pWorldPropToolData->GetPropName(12));
	((CButton*)(GetDlgItem(IDC_CHECK_PROP14)))->SetWindowText(pWorldPropToolData->GetPropName(13));
	((CButton*)(GetDlgItem(IDC_CHECK_PROP15)))->SetWindowText(pWorldPropToolData->GetPropName(14));
	((CButton*)(GetDlgItem(IDC_CHECK_PROP16)))->SetWindowText(pWorldPropToolData->GetPropName(15));

	UpdateData(FALSE);

	return TRUE;
}

void CGamePropVisibleDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}

void CGamePropVisibleDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

void CGamePropVisibleDlg::OnBnClickedCheckProp1()
{
	UpdateFlags();
}

void CGamePropVisibleDlg::OnBnClickedCheckProp2()
{
	UpdateFlags();
}

void CGamePropVisibleDlg::OnBnClickedCheckProp3()
{
	UpdateFlags();
}

void CGamePropVisibleDlg::OnBnClickedCheckProp4()
{
	UpdateFlags();
}

void CGamePropVisibleDlg::OnBnClickedCheckProp5()
{
	UpdateFlags();
}

void CGamePropVisibleDlg::OnBnClickedCheckProp6()
{
	UpdateFlags();
}

void CGamePropVisibleDlg::OnBnClickedCheckProp7()
{
	UpdateFlags();
}

void CGamePropVisibleDlg::OnBnClickedCheckProp8()
{
	UpdateFlags();
}

void CGamePropVisibleDlg::OnBnClickedCheckProp9()
{
	UpdateFlags();
}

void CGamePropVisibleDlg::OnBnClickedCheckProp10()
{
	UpdateFlags();
}

void CGamePropVisibleDlg::OnBnClickedCheckProp11()
{
	UpdateFlags();
}

void CGamePropVisibleDlg::OnBnClickedCheckProp12()
{
	UpdateFlags();
}

void CGamePropVisibleDlg::OnBnClickedCheckProp13()
{
	UpdateFlags();
}

void CGamePropVisibleDlg::OnBnClickedCheckProp14()
{
	UpdateFlags();
}

void CGamePropVisibleDlg::OnBnClickedCheckProp15()
{
	UpdateFlags();
}

void CGamePropVisibleDlg::OnBnClickedCheckProp16()
{
	UpdateFlags();
}

void CGamePropVisibleDlg::UpdateFlags()
{
	UpdateData(TRUE);

	m_uiFlags = 0x00000000;
	m_uiFlags |= (((CButton*)(GetDlgItem(IDC_CHECK_PROP1)))->GetCheck() ? 0x00000001 : 0x00000000);
	m_uiFlags |= (((CButton*)(GetDlgItem(IDC_CHECK_PROP2)))->GetCheck() ? 0x00000002 : 0x00000000);
	m_uiFlags |= (((CButton*)(GetDlgItem(IDC_CHECK_PROP3)))->GetCheck() ? 0x00000004 : 0x00000000);
	m_uiFlags |= (((CButton*)(GetDlgItem(IDC_CHECK_PROP4)))->GetCheck() ? 0x00000008 : 0x00000000);
	m_uiFlags |= (((CButton*)(GetDlgItem(IDC_CHECK_PROP5)))->GetCheck() ? 0x00000010 : 0x00000000);
	m_uiFlags |= (((CButton*)(GetDlgItem(IDC_CHECK_PROP6)))->GetCheck() ? 0x00000020 : 0x00000000);
	m_uiFlags |= (((CButton*)(GetDlgItem(IDC_CHECK_PROP7)))->GetCheck() ? 0x00000040 : 0x00000000);
	m_uiFlags |= (((CButton*)(GetDlgItem(IDC_CHECK_PROP8)))->GetCheck() ? 0x00000080 : 0x00000000);
	m_uiFlags |= (((CButton*)(GetDlgItem(IDC_CHECK_PROP9)))->GetCheck() ? 0x00000100 : 0x00000000);
	m_uiFlags |= (((CButton*)(GetDlgItem(IDC_CHECK_PROP10)))->GetCheck() ? 0x00000200 : 0x00000000);
	m_uiFlags |= (((CButton*)(GetDlgItem(IDC_CHECK_PROP11)))->GetCheck() ? 0x00000400 : 0x00000000);
	m_uiFlags |= (((CButton*)(GetDlgItem(IDC_CHECK_PROP12)))->GetCheck() ? 0x00000800 : 0x00000000);
	m_uiFlags |= (((CButton*)(GetDlgItem(IDC_CHECK_PROP13)))->GetCheck() ? 0x00001000 : 0x00000000);
	m_uiFlags |= (((CButton*)(GetDlgItem(IDC_CHECK_PROP14)))->GetCheck() ? 0x00002000 : 0x00000000);
	m_uiFlags |= (((CButton*)(GetDlgItem(IDC_CHECK_PROP15)))->GetCheck() ? 0x00004000 : 0x00000000);
	m_uiFlags |= (((CButton*)(GetDlgItem(IDC_CHECK_PROP16)))->GetCheck() ? 0x00008000 : 0x00000000);
}

void CGamePropVisibleDlg::UpdateUI()
{
	((CButton*)(GetDlgItem(IDC_CHECK_PROP1)))->SetCheck(m_uiFlags & 0x00000001); 
	((CButton*)(GetDlgItem(IDC_CHECK_PROP2)))->SetCheck(m_uiFlags & 0x00000002); 
	((CButton*)(GetDlgItem(IDC_CHECK_PROP3)))->SetCheck(m_uiFlags & 0x00000004); 
	((CButton*)(GetDlgItem(IDC_CHECK_PROP4)))->SetCheck(m_uiFlags & 0x00000008); 
	((CButton*)(GetDlgItem(IDC_CHECK_PROP5)))->SetCheck(m_uiFlags & 0x00000010); 
	((CButton*)(GetDlgItem(IDC_CHECK_PROP6)))->SetCheck(m_uiFlags & 0x00000020); 
	((CButton*)(GetDlgItem(IDC_CHECK_PROP7)))->SetCheck(m_uiFlags & 0x00000040); 
	((CButton*)(GetDlgItem(IDC_CHECK_PROP8)))->SetCheck(m_uiFlags & 0x00000080); 
	((CButton*)(GetDlgItem(IDC_CHECK_PROP9)))->SetCheck(m_uiFlags & 0x00000100); 
	((CButton*)(GetDlgItem(IDC_CHECK_PROP10)))->SetCheck(m_uiFlags & 0x00000200);
	((CButton*)(GetDlgItem(IDC_CHECK_PROP11)))->SetCheck(m_uiFlags & 0x00000400);
	((CButton*)(GetDlgItem(IDC_CHECK_PROP12)))->SetCheck(m_uiFlags & 0x00000800);
	((CButton*)(GetDlgItem(IDC_CHECK_PROP13)))->SetCheck(m_uiFlags & 0x00001000);
	((CButton*)(GetDlgItem(IDC_CHECK_PROP14)))->SetCheck(m_uiFlags & 0x00002000);
	((CButton*)(GetDlgItem(IDC_CHECK_PROP15)))->SetCheck(m_uiFlags & 0x00004000);
	((CButton*)(GetDlgItem(IDC_CHECK_PROP16)))->SetCheck(m_uiFlags & 0x00008000);

	UpdateData(FALSE);	
}

RwBool CGamePropVisibleDlg::GetGamePropColor(RwUInt32 uiFlags, RwRGBA* pRGBA)
{
	if (uiFlags == 0x00000000)
	{
		return FALSE;
	}

	RwInt32	uiCnt = -1;
	for (int i = 0; i < 16; ++i)				
	{
		if ((0x00000001 << i) & uiFlags)
		{
			if (uiCnt == -1)
			{
				uiCnt = i;
			}
			else
			{
				uiCnt = -1;
				break;
			}
		}
	}

	CNtlWorldPropToolData* pWorldPropToolData = &CNtlWEApplication::GetInstance()->m_WorldPropToolData;
	if (uiCnt == -1)
	{
		*pRGBA = *pWorldPropToolData->GetPropColorIntersection();
	}
	else
	{	
		*pRGBA = *pWorldPropToolData->GetPropColor(uiCnt);
	}



	return TRUE;

}