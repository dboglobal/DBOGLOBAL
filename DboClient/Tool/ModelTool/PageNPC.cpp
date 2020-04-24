// PageNPC.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool.h"
#include "PageNPC.h"


// CPageNPC dialog

IMPLEMENT_DYNAMIC(CPageNPC, CPropertyPage)

CPageNPC::CPageNPC()
	: CPropertyPage(CPageNPC::IDD)
{

}

CPageNPC::~CPageNPC()
{
}

void CPageNPC::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageNPC, CPropertyPage)
END_MESSAGE_MAP()


// CPageNPC message handlers
