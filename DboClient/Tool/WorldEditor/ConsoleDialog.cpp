// ConsoleDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "WorldEditor.h"

#include "MainFrm.h"

#include "ConsoleDialog.h"


// CConsoleDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CConsoleDialog, CDialog)
CConsoleDialog::CConsoleDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CConsoleDialog::IDD, pParent)
{
	m_EditCommand.SetParent( this );
}

CConsoleDialog::~CConsoleDialog()
{
}

void CConsoleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_INPUT_COMMAND, m_EditCommand);
}


BEGIN_MESSAGE_MAP(CConsoleDialog, CDialog)
END_MESSAGE_MAP()


// CConsoleDialog 메시지 처리기입니다.

BOOL CConsoleDialog::OnInitDialog()
{
	RECT		Rect;
	static const int iHeight = 80;

//	int		iScreenSizeX = GetSystemMetrics( SM_CXSCREEN );
	int		iScreenSizeY = GetSystemMetrics( SM_CYSCREEN );

	CMainFrame::GetActiveWindow()->GetWindowRect( &Rect );

	if( iScreenSizeY >= Rect.bottom + iHeight )
	{
		MoveWindow( Rect.left, Rect.bottom, Rect.right - Rect.left, iHeight );
	}
	else
	{
		MoveWindow( Rect.left, iScreenSizeY - iHeight, Rect.right - Rect.left, iHeight );
	}

	return CDialog::OnInitDialog();
}
