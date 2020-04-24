// Attr_ACT_SkipCont.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_SkipCont.h"


// CAttr_ACT_SkipCont 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_SkipCont, CAttr_Page, 1)

CAttr_ACT_SkipCont::CAttr_ACT_SkipCont()
	: CAttr_Page(CAttr_ACT_SkipCont::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_tID(NTL_TS_T_ID_INVALID)
	, m_tcID(NTL_TS_TC_ID_INVALID)
{
}

CAttr_ACT_SkipCont::~CAttr_ACT_SkipCont()
{
}

CString CAttr_ACT_SkipCont::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("stid"), m_tID );
	strData += PakingPageData( _T("stcid"), m_tcID );

	return strData;
}

void CAttr_ACT_SkipCont::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("stid") == strKey )
	{
		m_tID = (NTL_TS_T_ID)atoi( strValue.GetBuffer() );
	}
	else if ( _T("stcid") == strKey )
	{
		m_tcID = (NTL_TS_TC_ID)atof( strValue.GetBuffer() );
	}
}

void CAttr_ACT_SkipCont::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_SKIP_CONT_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);

	DDX_Text(pDX, IDD_TS_ACT_SKIP_CONT_QUESTID_EDITOR, m_tID);
	DDV_MinMaxUInt(pDX, m_tID, 0, NTL_TS_T_ID_INVALID);

	DDX_Text(pDX, IDD_TS_ACT_SKIP_CONT_CONTID_EDITOR, m_tcID);
	DDV_MinMaxUInt(pDX, m_tcID, 0, NTL_TS_TC_ID_INVALID);
}

BOOL CAttr_ACT_SkipCont::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_SkipCont, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_ACT_SkipCont 메시지 처리기입니다.
