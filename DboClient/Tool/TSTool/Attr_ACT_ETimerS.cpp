// Attr_ACT_ETimerS.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_ETimerS.h"


// CAttr_ACT_ETimerS 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_ETimerS, CAttr_Page, 1)

CAttr_ACT_ETimerS::CAttr_ACT_ETimerS()
	: CAttr_Page(CAttr_ACT_ETimerS::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwWaitTime(0xffffffff)
	, m_tgID(1)
{

}

CAttr_ACT_ETimerS::~CAttr_ACT_ETimerS()
{
}

CString CAttr_ACT_ETimerS::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("sort"), (int)m_ctrTimerSort.GetItemData( m_ctrTimerSort.GetCurSel() ) );
	strData += PakingPageData( _T("time"), m_dwWaitTime * 1000 );
	strData += PakingPageData( _T("tgid"), m_tgID );

	return strData;
}

void CAttr_ACT_ETimerS::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("sort") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrTimerSort.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrTimerSort.GetItemData( i ) == nValue )
			{
				m_ctrTimerSort.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("time") == strKey )
	{
		m_dwWaitTime = atoi( strValue.GetBuffer() ) / 1000;
	}
	else if ( _T("tgid") == strKey )
	{
		m_tgID = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_ETimerS::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_EXCEPTTIMER_S_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_EXCEPTTIMER_TIMERSORT_COMBO, m_ctrTimerSort);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_EXCEPTTIMER_WTIME_EDITOR, m_dwWaitTime);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_EXCEPTTIMER_GROUPID_EDITOR, m_tgID);

	if ( m_ctrTimerSort.GetSafeHwnd() )
	{
		DWORD_PTR dwData = m_ctrTimerSort.GetItemData( m_ctrTimerSort.GetCurSel() );
		switch ( dwData )
		{
		case eEXCEPT_TIMER_SORT_LIMIT_TIMER:
			{
				DDV_MinMaxUInt(pDX, m_tgID, NTL_TS_EXCEPT_TLIMT_GROUP_ID_BEGIN, NTL_TS_EXCEPT_TLIMT_GROUP_ID_END );
			}
			break;
		case eEXCEPT_TIMER_SORT_SERVER_TIMER:
			{
				DDV_MinMaxUInt(pDX, m_tgID, NTL_TS_EXCEPT_SERVER_GROUP_ID_BEGIN, NTL_TS_EXCEPT_SERVER_GROUP_ID_END );
			}
			break;
		case eEXCEPT_TIMER_SORT_CLIENT_TIMER:
			{
				DDV_MinMaxUInt(pDX, m_tgID, NTL_TS_EXCEPT_CLIENT_GROUP_ID_BEGIN, NTL_TS_EXCEPT_CLIENT_GROUP_ID_END );
			}
			break;
		default:
			{
				DDV_MinMaxUInt(pDX, m_tgID, 0, NTL_TS_TG_ID_INVALID);
			}
			break;
		}
	}
	else
	{
		DDV_MinMaxUInt(pDX, m_tgID, 0, NTL_TS_TG_ID_INVALID);
	}
}

BOOL CAttr_ACT_ETimerS::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_ctrTimerSort.SetItemData( m_ctrTimerSort.AddString( _T("Server timer") ), eEXCEPT_TIMER_SORT_SERVER_TIMER );
	m_ctrTimerSort.SetItemData( m_ctrTimerSort.AddString( _T("Client timer") ), eEXCEPT_TIMER_SORT_CLIENT_TIMER );
	int nIdx = m_ctrTimerSort.AddString( _T("Time limit timer") );
	m_ctrTimerSort.SetItemData( nIdx, eEXCEPT_TIMER_SORT_LIMIT_TIMER );
	m_ctrTimerSort.SetCurSel( nIdx );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_ETimerS, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_ACT_ETimerS 메시지 처리기입니다.

