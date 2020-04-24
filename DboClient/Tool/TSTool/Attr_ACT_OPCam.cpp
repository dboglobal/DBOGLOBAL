// Attr_ACT_OPCam.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_OPCam.h"


// CAttr_ACT_OPCam 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_OPCam, CAttr_Page, 1)

CAttr_ACT_OPCam::CAttr_ACT_OPCam()
	: CAttr_Page(CAttr_ACT_OPCam::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_strFuncName(_T(""))
{

}

CAttr_ACT_OPCam::~CAttr_ACT_OPCam()
{
}

CString CAttr_ACT_OPCam::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("fname"), m_strFuncName );

	return strData;
}

void CAttr_ACT_OPCam::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("fname") == strKey )
	{
		m_strFuncName = strValue.GetBuffer();
	}
}

void CAttr_ACT_OPCam::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_OPCAM_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_OPCAM_SCRIPT_INDEX_EDITOR, m_strFuncName);
}


BEGIN_MESSAGE_MAP(CAttr_ACT_OPCam, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_ACT_OPCam 메시지 처리기입니다.
