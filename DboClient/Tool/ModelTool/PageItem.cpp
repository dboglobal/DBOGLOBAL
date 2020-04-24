// PageItem.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool.h"
#include "PageItem.h"


// CPageItem dialog

IMPLEMENT_DYNAMIC(CPageItem, CPropertyPage)

CPageItem::CPageItem()
	: CPropertyPage(CPageItem::IDD)
{

}

CPageItem::~CPageItem()
{
}

void CPageItem::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPageItem, CPropertyPage)
END_MESSAGE_MAP()


// CPageItem message handlers
