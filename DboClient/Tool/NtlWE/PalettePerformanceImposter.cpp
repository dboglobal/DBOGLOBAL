// PalettePerformanceImposter.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PalettePerformanceImposter.h"


// CPalettePerformanceImposter 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPalettePerformanceImposter, CDialog)

CPalettePerformanceImposter::CPalettePerformanceImposter(CWnd* pParent /*=NULL*/)
	: CDialog(CPalettePerformanceImposter::IDD, pParent)
{

}

CPalettePerformanceImposter::~CPalettePerformanceImposter()
{
}

void CPalettePerformanceImposter::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPalettePerformanceImposter, CDialog)
END_MESSAGE_MAP()


// CPalettePerformanceImposter 메시지 처리기입니다.

BOOL CPalettePerformanceImposter::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CPalettePerformanceImposter::PreTranslateMessage(MSG* pMsg)
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

void CPalettePerformanceImposter::InitializeWorldData()
{

}