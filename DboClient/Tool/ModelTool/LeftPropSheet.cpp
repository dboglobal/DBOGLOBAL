// LeftPropSheet.cpp : implementation file
//

#include "stdafx.h"
#include "ModelTool.h"
#include "LeftPropSheet.h"


// CLeftPropSheet

IMPLEMENT_DYNAMIC(CLeftPropSheet, CPropertySheet)

CLeftPropSheet::CLeftPropSheet()
: CPropertySheet()
{

}
CLeftPropSheet::CLeftPropSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CLeftPropSheet::CLeftPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

}

CLeftPropSheet::~CLeftPropSheet()
{
}


BEGIN_MESSAGE_MAP(CLeftPropSheet, CPropertySheet)
END_MESSAGE_MAP()


// CLeftPropSheet message handlers
