// Attr_Page.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_Page.h"


// CAttr_Page 대화 상자입니다.


IMPLEMENT_DYNAMIC(CAttr_Page, COptionPage)

CAttr_Page::CAttr_Page(unsigned int uiIDD)
	: COptionPage(uiIDD)
{

}

CAttr_Page::~CAttr_Page()
{
}

CString CAttr_Page::GetAttr( CString strName )
{
	CString strRet;

	int nStart = m_strData.Find( strName );
	if ( nStart < 0 || nStart >= m_strData.GetLength() ) return CString();

	nStart = m_strData.Find( '#', nStart );
	if ( nStart < 0 || nStart >= m_strData.GetLength() ) return CString();

	strRet = m_strData.Tokenize( "#", nStart );
	strRet.Trim();

	return strRet;
}

void CAttr_Page::AddAttr( CString strData )
{
	m_strData += strData;
}

void CAttr_Page::AddAttr( CString strKey, CString strValue )
{
	m_strData += PakingPageData( strKey, strValue );
}

void CAttr_Page::AddAttr( CString strKey, int nValue )
{
	m_strData += PakingPageData( strKey, nValue );
}

void CAttr_Page::ClearAttr( void )
{
	m_strData.Empty();
}

void CAttr_Page::PakingPageData( CString& strResult, CString strKey, CString strValue )
{
	CString strData;
	strData.Format( _T("%s # %s # "), strKey, strValue );

	strResult += strData;
}

CString CAttr_Page::GetPageData( void )
{
	return CString();
}

void CAttr_Page::SetPageData( CString& strData )
{
	enum { eTOKEN_TYPE_KEY, eTOKEN_TYPE_VALUE };

	CString strKey, strValue;

	CString strToken;
	int nStart = 0;
	bool bLoop = true;

	int nTokType = eTOKEN_TYPE_KEY;

	while ( bLoop )
	{
		strToken = strData.Tokenize( _T("#"), nStart );
		if ( nStart >= strData.GetLength() ) bLoop = false;

		switch ( nTokType )
		{
		case eTOKEN_TYPE_KEY:
			{
				strKey = strToken.Trim();
				strValue.Empty();

				nTokType = eTOKEN_TYPE_VALUE;
			}
			break;

		case eTOKEN_TYPE_VALUE:
			{
				strValue = strToken.Trim();

				UnPakingPageData( strKey, strValue );

				strKey.Empty();
				strValue.Empty();

				nTokType = eTOKEN_TYPE_KEY;
			}
			break;
		}
	}

	UpdateData( false );
}

CString	CAttr_Page::PakingPageData( CString strKey, CString strValue )
{
	CString strData;
	strData.Format( _T("%s # %s # "), strKey, strValue );
	return strData;
}

CString CAttr_Page::PakingPageData( CString strKey, DWORD dwValue )
{
	CString strData;
	strData.Format( _T("%s # %d # "), strKey, dwValue );
	return strData;
}

CString CAttr_Page::PakingPageData( CString strKey, int nValue )
{
	CString strData;
	strData.Format( _T("%s # %d # "), strKey, nValue );
	return strData;
}

CString CAttr_Page::PakingPageData( CString strKey, unsigned int uiValue )
{
	CString strData;
	strData.Format( _T("%s # %d # "), strKey, uiValue );
	return strData;
}

CString CAttr_Page::PakingPageData( CString strKey, float fValue )
{
	CString strData;
	strData.Format( _T("%s # %f # "), strKey, fValue );
	return strData;
}

void CAttr_Page::UnPakingPageData( CString& strKey, CString& strValue )
{
}

void CAttr_Page::DoDataExchange(CDataExchange* pDX)
{
	COptionPage::DoDataExchange(pDX);
}

BOOL CAttr_Page::OnInitDialog()
{
	COptionPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_Page, COptionPage)
END_MESSAGE_MAP()


// CAttr_Page 메시지 처리기입니다.

void CAttr_Page::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_strData = GetPageData();

	COptionPage::OnOK();
}
