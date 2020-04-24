// PeHeightPopup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PeHeightPopup.h"


// CPeHeightPopup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPeHeightPopup, CDialog)

CPeHeightPopup::CPeHeightPopup(CWnd* pParent /*=NULL*/)
	: CDialog(CPeHeightPopup::IDD, pParent)
	, m_strValue(_T(""))
{

}

CPeHeightPopup::~CPeHeightPopup()
{
}

void CPeHeightPopup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PE_HEIGHT, m_strValue);
}


BEGIN_MESSAGE_MAP(CPeHeightPopup, CDialog)
	ON_BN_CLICKED(IDOK, &CPeHeightPopup::OnBnClickedOk)
END_MESSAGE_MAP()


// CPeHeightPopup 메시지 처리기입니다.

BOOL CPeHeightPopup::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CPeHeightPopup::PreTranslateMessage(MSG* pMsg)
{
	// prevent keys; escape, return
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

LRESULT CPeHeightPopup::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::WindowProc(message, wParam, lParam);
}
void CPeHeightPopup::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

 	m_fValue = static_cast<RwReal>(::atof(m_strValue.GetBuffer(0)));

	OnOK();
}
