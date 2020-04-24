// SaberView2.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "SaberView2.h"


// CSaberView2 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSaberView2, CDialog)

CSaberView2::CSaberView2(CWnd* pParent /*=NULL*/)
	: CDialog(CSaberView2::IDD, pParent)
{

}

CSaberView2::~CSaberView2()
{
}

void CSaberView2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSaberView2, CDialog)
END_MESSAGE_MAP()


// CSaberView2 메시지 처리기입니다.
