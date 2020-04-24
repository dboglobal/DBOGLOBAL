// Attr_EVT_ItemUpgrade.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_EVT_ItemUpgrade.h"


// CAttr_EVT_ItemUpgrade 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_EVT_ItemUpgrade, CAttr_Page, 1)

CAttr_EVT_ItemUpgrade::CAttr_EVT_ItemUpgrade(CWnd* pParent /*=NULL*/)
	: CAttr_Page(CAttr_EVT_ItemUpgrade::IDD)
{

}

CAttr_EVT_ItemUpgrade::~CAttr_EVT_ItemUpgrade()
{
}

CString CAttr_EVT_ItemUpgrade::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	return strData;
}

void CAttr_EVT_ItemUpgrade::UnPakingPageData( CString& strKey, CString& strValue )
{
}

void CAttr_EVT_ItemUpgrade::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
}

BOOL CAttr_EVT_ItemUpgrade::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_EVT_ItemUpgrade, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_EVT_ItemUpgrade 메시지 처리기입니다.
