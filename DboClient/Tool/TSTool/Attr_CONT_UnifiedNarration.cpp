// Attr_CONT_UnifiedNarration.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_CONT_UnifiedNarration.h"


// CAttr_CONT_UnifiedNarration 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_CONT_UnifiedNarration, CAttr_Page, 1)

CAttr_CONT_UnifiedNarration::CAttr_CONT_UnifiedNarration()
	: CAttr_Page(CAttr_CONT_UnifiedNarration::IDD)
	, m_tcID(NTL_TS_TC_ID_INVALID)
	, m_dwTblIdx(0xffffffff)
{
}

CAttr_CONT_UnifiedNarration::~CAttr_CONT_UnifiedNarration()
{
}

CString CAttr_CONT_UnifiedNarration::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("cid"), m_tcID );
	strData += PakingPageData( _T("idx"), m_dwTblIdx );

	return strData;
}

void CAttr_CONT_UnifiedNarration::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("cid") == strKey )
	{
		m_tcID = atoi( strValue.GetBuffer() );
	}
	if ( _T("idx") == strKey )
	{
		m_dwTblIdx = atoi( strValue.GetBuffer() );
	}
}

void CAttr_CONT_UnifiedNarration::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_UNIFIED_NARRATION_ID_EDITOR, m_tcID);
	DDV_MinMaxUInt(pDX, m_tcID, 0, NTL_TS_TC_ID_INVALID);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_UNIFIED_NARRATION_TBLIDX_EDITOR, m_dwTblIdx);
}

BOOL CAttr_CONT_UnifiedNarration::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_CONT_UnifiedNarration, CAttr_Page)
END_MESSAGE_MAP()
