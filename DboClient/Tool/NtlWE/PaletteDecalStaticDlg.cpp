// PaletteDecalStaticDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"

#include "NtlWorldPaletteDefine.h"

#include "PaletteDecalStaticDlg.h"


// PaletteDecalStaticDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteDecalStaticDlg, CDialog)

CPaletteDecalStaticDlg::CPaletteDecalStaticDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteDecalStaticDlg::IDD, pParent)
{

}

CPaletteDecalStaticDlg::~CPaletteDecalStaticDlg()
{
}

void CPaletteDecalStaticDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPaletteDecalStaticDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CPaletteDecalStaticDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// PaletteDecalStaticDlg 메시지 처리기입니다.

BOOL CPaletteDecalStaticDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a controldf
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

LRESULT CPaletteDecalStaticDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			dGET_CUR_PALETTE() = ePM_DECAL_STATIC;
			dGETAPP().GetInstance()->m_pEventHandler->DeleteAllPaletteDummies();

			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CPaletteDecalStaticDlg::PreTranslateMessage(MSG* pMsg)
{
	// windows messages
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

void CPaletteDecalStaticDlg::InitializeWorldData()
{
}

void CPaletteDecalStaticDlg::OnBnClickedOk()
{
 	if (dGET_BRUSH_ENTITY())
 	{
 		GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
 		dGET_BRUSH_ENTITY() = NULL;
 	}
 	dGET_BRUSH_ENTITY()  = GetSceneManager()->CreateEntity(PLENTITY_DECAL_STATIC, NULL, NULL);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnOK();
}
