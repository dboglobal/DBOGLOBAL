// Attr_ACT_ExcCGroup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_ExcCGroup.h"


// CAttr_ACT_ExcCGroup 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_ExcCGroup, CAttr_Page, 1)

CAttr_ACT_ExcCGroup::CAttr_ACT_ExcCGroup()
	: CAttr_Page(CAttr_ACT_ExcCGroup::IDD)
	, m_nCurSelIndex( -1 )
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwCTSGID(NTL_TS_TG_ID_INVALID)
{
	m_defMapList[eEXC_C_GROUP_REG_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_EXC_C_GROUP_EXCTSGID_STATIC );
	m_defMapList[eEXC_C_GROUP_REG_TYPE_INVALID].push_back( IDC_TS_ACT_ATTR_EXC_C_GROUP_EXCTSGID_EDITOR );

	m_defMapList[eEXC_C_GROUP_REG_TYPE_REG].push_back( IDC_TS_ACT_ATTR_EXC_C_GROUP_EXCTSGID_STATIC );
	m_defMapList[eEXC_C_GROUP_REG_TYPE_REG].push_back( IDC_TS_ACT_ATTR_EXC_C_GROUP_EXCTSGID_EDITOR );
}

CAttr_ACT_ExcCGroup::~CAttr_ACT_ExcCGroup()
{
}

void CAttr_ACT_ExcCGroup::InitData( void )
{
	m_nCurSelIndex = -1;

	m_dwCTSGID = NTL_TS_TG_ID_INVALID;

	UpdateData( FALSE );
}

CString CAttr_ACT_ExcCGroup::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("rtype"), (int)m_ctrRegType.GetItemData( m_ctrRegType.GetCurSel() ) );
	strData += PakingPageData( _T("gid"), m_dwCTSGID );

	return strData;
}

void CAttr_ACT_ExcCGroup::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("rtype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrRegType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrRegType.GetItemData( i ) == nValue )
			{
				m_ctrRegType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("gid") == strKey )
	{
		m_dwCTSGID = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_ExcCGroup::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_EXC_C_GROUP_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);

	DDX_Control(pDX, IDC_TS_ACT_ATTR_EXC_C_GROUP_REGTYPE_COMBO, m_ctrRegType);

	DDX_Text(pDX, IDC_TS_ACT_ATTR_EXC_C_GROUP_EXCTSGID_EDITOR, m_dwCTSGID);

	if ( m_ctrRegType.GetItemData( m_ctrRegType.GetCurSel() ) == eEXC_C_GROUP_REG_TYPE_REG )
	{
		DDV_MinMaxUInt(pDX, m_dwCTSGID, NTL_TS_EXCEPT_CLIENT_GROUP_ID_BEGIN, NTL_TS_EXCEPT_CLIENT_GROUP_ID_END);
	}
}

BOOL CAttr_ACT_ExcCGroup::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrRegType.SetItemData( m_ctrRegType.AddString( _T("해제") ), eEXC_C_GROUP_REG_TYPE_UNREG );
	int nIdx = m_ctrRegType.AddString( _T("등록") );
	m_ctrRegType.SetItemData( nIdx, eEXC_C_GROUP_REG_TYPE_REG );
	m_ctrRegType.SetCurSel( nIdx );

	vecdef_ControlList::iterator it = m_defMapList[eEXC_C_GROUP_REG_TYPE_INVALID].begin();
	for ( ; it != m_defMapList[eEXC_C_GROUP_REG_TYPE_INVALID].end(); ++it )
	{
		GetDlgItem( *it )->ShowWindow( SW_HIDE );
	}

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	OnCbnSelchangeTsActAttrExcCGroupRegtypeCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_ExcCGroup, CAttr_Page)
	ON_CBN_SELCHANGE(IDC_TS_ACT_ATTR_EXC_C_GROUP_REGTYPE_COMBO, &CAttr_ACT_ExcCGroup::OnCbnSelchangeTsActAttrExcCGroupRegtypeCombo)
END_MESSAGE_MAP()


// CAttr_ACT_ExcCGroup 메시지 처리기입니다.

void CAttr_ACT_ExcCGroup::OnCbnSelchangeTsActAttrExcCGroupRegtypeCombo()
{
	if ( -1 != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrRegType.GetItemData( m_nCurSelIndex );

		if ( m_defMapList.end() != m_defMapList.find( dwData ) )
		{
			vecdef_ControlList::iterator it = m_defMapList[dwData].begin();
			for ( ; it != m_defMapList[dwData].end(); ++it )
			{
				GetDlgItem( *it )->ShowWindow( SW_HIDE );
			}
		}

		InitData();
	}

	m_nCurSelIndex = m_ctrRegType.GetCurSel();
	if ( CB_ERR != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrRegType.GetItemData( m_nCurSelIndex );

		if ( m_defMapList.end() != m_defMapList.find( dwData ) )
		{
			vecdef_ControlList::iterator it = m_defMapList[dwData].begin();
			for ( ; it != m_defMapList[dwData].end(); ++it )
			{
				GetDlgItem( *it )->ShowWindow( SW_SHOW );
			}
		}
		else
		{
			m_nCurSelIndex = -1;
		}
	}
	else
	{
		m_nCurSelIndex = -1;
	}
}
