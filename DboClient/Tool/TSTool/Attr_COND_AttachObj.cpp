// Attr_COND_AttachObj.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_AttachObj.h"


// CAttr_COND_AttachObj 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_AttachObj, CAttr_Page, 1)

CAttr_COND_AttachObj::CAttr_COND_AttachObj(CWnd* pParent /*=NULL*/)
	: CAttr_Page(CAttr_COND_AttachObj::IDD)
	, m_dwWorldIdx(0xffffffff)
{

}

CAttr_COND_AttachObj::~CAttr_COND_AttachObj()
{
}

CString CAttr_COND_AttachObj::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("widx"), m_dwWorldIdx );
	strData += PakingPageData( _T("objidx"), m_strObjectIdx );

	return strData;
}

void CAttr_COND_AttachObj::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("widx") == strKey )
	{
		m_dwWorldIdx = atoi( strValue.GetBuffer() );
	}

	if ( _T("objidx") == strKey )
	{
		m_strObjectIdx = strValue;
	}
}

void CAttr_COND_AttachObj::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_COND_ATTR_ATTACH_OBJ_OBJIDX_EDITOR, m_strObjectIdx);
	DDX_Text(pDX, IDC_TS_COND_ATTR_ATTACH_OBJ_WORLDIDX_EDITOR, m_dwWorldIdx);
}


BEGIN_MESSAGE_MAP(CAttr_COND_AttachObj, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_AttachObj 메시지 처리기입니다.
