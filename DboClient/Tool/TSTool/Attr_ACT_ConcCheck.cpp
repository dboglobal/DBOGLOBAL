// Attr_ACT_ConcCheck.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_ConcCheck.h"


// CAttr_ACT_ConcCheck 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_ConcCheck, CAttr_Page, 1)

CAttr_ACT_ConcCheck::CAttr_ACT_ConcCheck()
	: CAttr_Page(CAttr_ACT_ConcCheck::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_nConcCheckCnt(2)
	, m_dwResetTime(0)
	, m_dwObjIdx(0xffffffff)
	, m_dwExcTID(NTL_TS_T_ID_INVALID)
{

}

CAttr_ACT_ConcCheck::~CAttr_ACT_ConcCheck()
{
}

CString CAttr_ACT_ConcCheck::GetPageData( void )
{
	if ( !UpdateData( true ) )
	{
		return CString();
	}

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );

	strData += PakingPageData( _T("ccnt"), m_nConcCheckCnt );
	strData += PakingPageData( _T("rtime"), m_dwResetTime );
	strData += PakingPageData( _T("eobjidx"), m_dwObjIdx );
	strData += PakingPageData( _T("etsid"), m_dwExcTID );

	return strData;
}

void CAttr_ACT_ConcCheck::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("ccnt") == strKey )
	{
		m_nConcCheckCnt = atoi( strValue.GetBuffer() );
	}
	else if ( _T("rtime") == strKey )
	{
		m_dwResetTime = atoi( strValue.GetBuffer() );
	}
	else if ( _T("eobjidx") == strKey )
	{
		m_dwObjIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("etsid") == strKey )
	{
		m_dwExcTID = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_ConcCheck::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_CONC_CHECK_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_CONC_CHECK_CONCCHECK_CNT_EDITOR, m_nConcCheckCnt);
	DDV_MinMaxUInt(pDX, m_nConcCheckCnt, 1, 10);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_CONC_CHECK_CONCCHECK_RESETTIME_EDITOR, m_dwResetTime);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_CONC_CHECK_CONCCHECK_EXC_OBJIDX_EDITOR, m_dwObjIdx);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_CONC_CHECK_CONCCHECK_EXC_TID_EDITOR, m_dwExcTID);
}

BOOL CAttr_ACT_ConcCheck::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_ConcCheck, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_ACT_ConcCheck 메시지 처리기입니다.
