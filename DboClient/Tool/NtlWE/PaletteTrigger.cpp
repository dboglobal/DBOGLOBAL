// PaletteTrigger.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteTrigger.h"


// CPaletteTrigger 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteTrigger, CDialog)
CPaletteTrigger::CPaletteTrigger(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteTrigger::IDD, pParent)
{
}

CPaletteTrigger::~CPaletteTrigger()
{
}

void CPaletteTrigger::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CPaletteTrigger::InitializeWorldData()
{
}

BEGIN_MESSAGE_MAP(CPaletteTrigger, CDialog)
END_MESSAGE_MAP()


// CPaletteTrigger 메시지 처리기입니다.
