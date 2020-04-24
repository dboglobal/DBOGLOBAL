// PointShapeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "PointShapeDlg.h"


// PointShapeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPointShapeDlg, CDialog)

CPointShapeDlg::CPointShapeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPointShapeDlg::IDD, pParent)
{

}

CPointShapeDlg::~CPointShapeDlg()
{
}

void CPointShapeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPointShapeDlg, CDialog)
END_MESSAGE_MAP()


// CPointShapeDlg 메시지 처리기입니다.
