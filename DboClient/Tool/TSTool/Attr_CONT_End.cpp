// Attr_CONT_End.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_CONT_End.h"


// CAttr_CONT_End 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_CONT_End, CAttr_Page, 1)

CAttr_CONT_End::CAttr_CONT_End()
	: CAttr_Page(CAttr_CONT_End::IDD)
	, m_tcID(NTL_TS_TC_ID_INVALID)
{
}

CAttr_CONT_End::~CAttr_CONT_End()
{
}

CString CAttr_CONT_End::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("cid"), m_tcID );
	strData += PakingPageData( _T("type"), (int)m_ctlEndType.GetItemData( m_ctlEndType.GetCurSel() ) );

	return strData;
}

void CAttr_CONT_End::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("cid") == strKey )
	{
		m_tcID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("type") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctlEndType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctlEndType.GetItemData( i ) == nValue )
			{
				m_ctlEndType.SetCurSel( i );
				break;
			}
		}
	}
}

void CAttr_CONT_End::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_CONT_ATTR_END_ID_EDITOR, m_tcID);
	DDV_MinMaxUInt(pDX, m_tcID, 0, NTL_TS_TC_ID_INVALID);
	DDX_Control(pDX, IDC_TS_CONT_ATTR_END_ENDTYPE_COMBO, m_ctlEndType);
}

BOOL CAttr_CONT_End::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctlEndType.SetItemData( m_ctlEndType.AddString( _T("Complete") ), eEND_TYPE_COMPLETE );
	int nSel = m_ctlEndType.AddString( _T("Not progress") );
	m_ctlEndType.SetItemData( nSel, eEND_TYPE_NOT_PROGRESS );
	m_ctlEndType.SetCurSel( nSel );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_CONT_End, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_CONT_End 메시지 처리기입니다.

