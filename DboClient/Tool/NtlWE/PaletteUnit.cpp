// PaletteUnit.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteUnit.h"


// CPaletteUnit 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteUnit, CDialog)
CPaletteUnit::CPaletteUnit(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteUnit::IDD, pParent)
{
}

CPaletteUnit::~CPaletteUnit()
{
}

void CPaletteUnit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CPaletteUnit::InitializeWorldData()
{
}

BEGIN_MESSAGE_MAP(CPaletteUnit, CDialog)
END_MESSAGE_MAP()


// CPaletteUnit 메시지 처리기입니다.
