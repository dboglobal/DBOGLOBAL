// PageGround.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool.h"
#include "PageGround.h"


// CPageGround dialog

IMPLEMENT_DYNAMIC(CPageGround, CPropertyPage)

CPageGround::CPageGround()
	: CPropertyPage(CPageGround::IDD)
{

}

CPageGround::~CPageGround()
{
}

void CPageGround::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageGround, CPropertyPage)
END_MESSAGE_MAP()


// CPageGround message handlers
