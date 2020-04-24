// PaletteRegion.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteRegion.h"


// CPaletteRegion 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteRegion, CDialog)
CPaletteRegion::CPaletteRegion(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteRegion::IDD, pParent)
{
}

CPaletteRegion::~CPaletteRegion()
{
}

void CPaletteRegion::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CPaletteRegion::InitializeWorldData()
{
}

BEGIN_MESSAGE_MAP(CPaletteRegion, CDialog)
END_MESSAGE_MAP()


// CPaletteRegion 메시지 처리기입니다.
