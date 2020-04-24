// Attr_COND_SSM.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_SSM.h"


// CAttr_COND_SSM 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_SSM, CAttr_Page, 1)

CAttr_COND_SSM::CAttr_COND_SSM()
	: CAttr_Page(CAttr_COND_SSM::IDD)
	, m_dwValue(0xffffffff)
{

}

CAttr_COND_SSM::~CAttr_COND_SSM()
{
}

CString CAttr_COND_SSM::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("op"), (int)m_ctrOPType.GetItemData( m_ctrOPType.GetCurSel() ) );
	strData += PakingPageData( _T("ssmid"), (int)m_ctrSSMId.GetItemData( m_ctrSSMId.GetCurSel() ) );
	strData += PakingPageData( _T("var"), m_dwValue );

	return strData;
}

void CAttr_COND_SSM::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("op") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrOPType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrOPType.GetItemData( i ) == nValue )
			{
				m_ctrOPType.SetCurSel( i );
				break;
			}
		}
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
	else if ( _T("var") == strKey )
	{
		m_dwValue = atoi( strValue.GetBuffer() );
	}
}

void CAttr_COND_SSM::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TS_COND_ATTR_SSM_OPTYPE_COMBO, m_ctrOPType);
	DDX_Control(pDX, IDC_TS_COND_ATTR_SSM_SLOTID_COMBO, m_ctrSSMId);
	DDX_Text(pDX, IDC_TS_COND_ATTR_SSM_VARIABLE_EDITOR, m_dwValue);
}

BOOL CAttr_COND_SSM::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrOPType.SetItemData( m_ctrOPType.AddString( _T("Equal") ), eSSM_OP_TYPE_EQUAL );
	m_ctrOPType.SetItemData( m_ctrOPType.AddString( _T("Not equal") ), eSSM_OP_TYPE_NOT_EQUAL );
	m_ctrOPType.SetItemData( m_ctrOPType.AddString( _T("Less") ), eSSM_OP_TYPE_LESS );
	m_ctrOPType.SetItemData( m_ctrOPType.AddString( _T("Less equal") ), eSSM_OP_TYPE_LESS_EQUAL );
	m_ctrOPType.SetItemData( m_ctrOPType.AddString( _T("More") ), eSSM_OP_TYPE_MORE );
	m_ctrOPType.SetItemData( m_ctrOPType.AddString( _T("More equal") ), eSSM_OP_TYPE_MORE_EQUAL );
	int nIdx = m_ctrOPType.AddString( _T("Invalid") );
	m_ctrOPType.SetItemData(  nIdx, eSSM_ID_INVALID );
	m_ctrOPType.SetCurSel( nIdx );

	m_ctrSSMId.SetItemData( m_ctrSSMId.AddString( _T("Slot 0") ), eSSM_ID_0 );
	m_ctrSSMId.SetItemData( m_ctrSSMId.AddString( _T("Slot 1") ), eSSM_ID_1 );
	m_ctrSSMId.SetItemData( m_ctrSSMId.AddString( _T("Slot 2") ), eSSM_ID_2 );
	m_ctrSSMId.SetItemData( m_ctrSSMId.AddString( _T("Slot 3") ), eSSM_ID_3 );
	m_ctrSSMId.SetItemData( m_ctrSSMId.AddString( _T("Slot 4") ), eSSM_ID_4 );
	m_ctrSSMId.SetItemData( m_ctrSSMId.AddString( _T("Slot 5") ), eSSM_ID_5 );
	m_ctrSSMId.SetItemData( m_ctrSSMId.AddString( _T("Slot 6") ), eSSM_ID_6 );
	m_ctrSSMId.SetItemData( m_ctrSSMId.AddString( _T("Slot 7") ), eSSM_ID_7 );
	nIdx = m_ctrSSMId.AddString( _T("Invalid") );
	m_ctrSSMId.SetItemData(  nIdx, eSSM_ID_INVALID );
	m_ctrSSMId.SetCurSel( nIdx );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_COND_SSM, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_COND_SSM 메시지 처리기입니다.
