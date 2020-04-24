// Attr_ACT_Dir.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_Dir.h"
#include "NtlSharedType.h"
#include "NtlDirection.h"


// CAttr_ACT_Dir 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_Dir, CAttr_Page, 1)

CAttr_ACT_Dir::CAttr_ACT_Dir()
	: CAttr_Page(CAttr_ACT_Dir::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwRange(0)
	, m_TblIdx(0xffffffff)
{

}

CAttr_ACT_Dir::~CAttr_ACT_Dir()
{
}

CString CAttr_ACT_Dir::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("rng"), m_dwRange );
	strData += PakingPageData( _T("type"), (int)m_ctrType.GetItemData( m_ctrType.GetCurSel() ) );
	strData += PakingPageData( _T("idx"), m_TblIdx );

	return strData;
}

void CAttr_ACT_Dir::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("rng") == strKey )
	{
		m_dwRange = atoi( strValue.GetBuffer() );
	}
	else if ( _T("type") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrType.GetItemData( i ) == nValue )
			{
				m_ctrType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("idx") == strKey )
	{
		m_TblIdx = atof( strValue.GetBuffer() );
	}
}

void CAttr_ACT_Dir::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_DIR_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);

	DDX_Text(pDX, IDC_TS_ACT_ATTR_DIR_RANGE_EDITOR, m_dwRange);

	DDX_Control(pDX, IDC_TS_ACT_ATTR_DIR_DIRTYPE_COMBO, m_ctrType);

	DDX_Text(pDX, IDC_TS_ACT_ATTR_DIR_DIRTBLIDX_EDITOR, m_TblIdx);
}

BOOL CAttr_ACT_Dir::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrType.SetItemData( m_ctrType.AddString( _T("Flash") ), DIRECTION_FLASH );
	m_ctrType.SetItemData( m_ctrType.AddString( _T("Cinematic") ), DIRECTION_CINEMATIC );
	m_ctrType.SetItemData( m_ctrType.AddString( _T("Play") ), DIRECTION_PLAY );
	int nIdx = m_ctrType.AddString( _T("Invalid") );
	m_ctrType.SetItemData( nIdx, INVALID_DIRECTION_TYPE );
	m_ctrType.SetCurSel( nIdx );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_Dir, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_ACT_Dir 메시지 처리기입니다.
