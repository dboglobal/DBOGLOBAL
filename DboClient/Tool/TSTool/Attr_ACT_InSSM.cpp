// Attr_ACT_InSSM.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_InSSM.h"


// CAttr_ACT_InSSM 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_InSSM, CAttr_Page, 1)

CAttr_ACT_InSSM::CAttr_ACT_InSSM()
	: CAttr_Page(CAttr_ACT_InSSM::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwValue(0xffffffff)
{

}

CAttr_ACT_InSSM::~CAttr_ACT_InSSM()
{
}

CString CAttr_ACT_InSSM::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("ssmid"), (int)m_ctrSSMId.GetItemData( m_ctrSSMId.GetCurSel() ) );
	strData += PakingPageData( _T("val"), m_dwValue );

	return strData;
}

void CAttr_ACT_InSSM::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("ssmid") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrSSMId.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrSSMId.GetItemData( i ) == nValue )
			{
				m_ctrSSMId.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("val") == strKey )
	{
		m_dwValue = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_InSSM::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_INSSM_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_INSSM_SSMID_COMBO, m_ctrSSMId);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_INSSM_VARIABLE_EDITOR, m_dwValue);
}

BOOL CAttr_ACT_InSSM::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrSSMId.SetItemData( m_ctrSSMId.AddString( _T("Slot 0") ), eSSM_ID_0 );
	m_ctrSSMId.SetItemData( m_ctrSSMId.AddString( _T("Slot 1") ), eSSM_ID_1 );
	m_ctrSSMId.SetItemData( m_ctrSSMId.AddString( _T("Slot 2") ), eSSM_ID_2 );
	m_ctrSSMId.SetItemData( m_ctrSSMId.AddString( _T("Slot 3") ), eSSM_ID_3 );
	m_ctrSSMId.SetItemData( m_ctrSSMId.AddString( _T("Slot 4") ), eSSM_ID_4 );
	m_ctrSSMId.SetItemData( m_ctrSSMId.AddString( _T("Slot 5") ), eSSM_ID_5 );
	m_ctrSSMId.SetItemData( m_ctrSSMId.AddString( _T("Slot 6") ), eSSM_ID_6 );
	m_ctrSSMId.SetItemData( m_ctrSSMId.AddString( _T("Slot 7") ), eSSM_ID_7 );
	int nIdx = m_ctrSSMId.AddString( _T("Invalid") );
	m_ctrSSMId.SetItemData( nIdx, eSSM_ID_INVALID );
	m_ctrSSMId.SetCurSel( nIdx );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BEGIN_MESSAGE_MAP(CAttr_ACT_InSSM, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_ACT_InSSM 메시지 처리기입니다.
