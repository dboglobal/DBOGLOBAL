// Attr_Link.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_Link.h"
#include "Shape_Link.h"


// CAttr_Link 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_Link, CAttr_Page, 1)

CAttr_Link::CAttr_Link(CWnd* pParent /*=NULL*/)
	: CAttr_Page(CAttr_Link::IDD)
	, m_uiBranchName(0xffffffff)
{

}

CAttr_Link::~CAttr_Link()
{
}

CString CAttr_Link::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("branchname"), m_uiBranchName );

	return strData;
}

void CAttr_Link::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("branchname") == strKey )
	{
		m_uiBranchName = atoi( strValue.GetBuffer() );
	}
}

void CAttr_Link::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LINK_BRANCH_NAME, m_uiBranchName);
	DDV_MinMaxUInt(pDX, m_uiBranchName, 0, 0xffffffff);
}


BEGIN_MESSAGE_MAP(CAttr_Link, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_Link 메시지 처리기입니다.
