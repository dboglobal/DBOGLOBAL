// SaberView3.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "SaberView3.h"


// CSaberView3 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSaberView3, CDialog)

CSaberView3::CSaberView3(CWnd* pParent /*=NULL*/)
	: CDialog(CSaberView3::IDD, pParent)
{

}

CSaberView3::~CSaberView3()
{
}

void CSaberView3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSaberView3, CDialog)
END_MESSAGE_MAP()


// CSaberView3 메시지 처리기입니다.
