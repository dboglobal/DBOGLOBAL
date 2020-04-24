// Attr_ACT_TMQInfoShow.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_TMQInfoShow.h"
#include "NtlTimeQuest.h"


// CAttr_ACT_TMQInfoShow 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_TMQInfoShow, CAttr_Page, 1)

CAttr_ACT_TMQInfoShow::CAttr_ACT_TMQInfoShow()
	: CAttr_Page(CAttr_ACT_TMQInfoShow::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
{

}

CAttr_ACT_TMQInfoShow::~CAttr_ACT_TMQInfoShow()
{
}

CString CAttr_ACT_TMQInfoShow::GetPageData( void )
{
	UpdateData( TRUE );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("itype"), (int)m_ctrInfoType.GetItemData( m_ctrInfoType.GetCurSel() ) );

	return strData;
}

void CAttr_ACT_TMQInfoShow::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("itype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrInfoType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrInfoType.GetItemData( i ) == nValue )
			{
				m_ctrInfoType.SetCurSel( i );
				break;
			}
		}
	}
}

void CAttr_ACT_TMQInfoShow::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_TS_ACT_ATTR_TMQINFOSHOW_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);

	DDX_Control(pDX, IDC_TS_ACT_ATTR_TMQINFOSHOW_INFOTYPE_COMBO, m_ctrInfoType);
}

BOOL CAttr_ACT_TMQInfoShow::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrInfoType.SetItemData( m_ctrInfoType.AddString( _T("Scenario") ), TIMEQUEST_INFORMATION_TYPE_SCENARIO );
	m_ctrInfoType.SetItemData( m_ctrInfoType.AddString( _T("Stage") ), TIMEQUEST_INFORMATION_TYPE_STAGE );
	int nIdx = m_ctrInfoType.AddString( _T("Invalid") );
	m_ctrInfoType.SetItemData( nIdx, INVALID_TIMEQUEST_INFORMATION_TYPE );
	m_ctrInfoType.SetCurSel( nIdx );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_TMQInfoShow, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_ACT_TMQInfoShow 메시지 처리기입니다.
