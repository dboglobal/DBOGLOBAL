// Attr_EVT_ColRgn.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_EVT_ColRgn.h"

// CAttr_EVT_ColRgn 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_EVT_ColRgn, CAttr_Page, 1)

CAttr_EVT_ColRgn::CAttr_EVT_ColRgn(CWnd* pParent /*=NULL*/)
	: CAttr_Page(CAttr_EVT_ColRgn::IDD)
	, m_nCurSelIndex( -1 )
	, m_dwWorldIdx(0xffffffff)
	, m_fRadiusX(FLT_MAX)
	, m_fRadiusZ(FLT_MAX)
	, m_fRadiusR(FLT_MAX)
	, m_fRectX0(FLT_MAX)
	, m_fRectZ0(FLT_MAX)
	, m_fRectX1(FLT_MAX)
	, m_fRectZ1(FLT_MAX)
	, m_fRectX2(FLT_MAX)
	, m_fRectZ2(FLT_MAX)
	, m_fRectX3(FLT_MAX)
	, m_fRectZ3(FLT_MAX)
{
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RADIUS_X_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RADIUS_X );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RADIUS_Z_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RADIUS_Z );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RADIUS_R_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RADIUS_R );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X0_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X0 );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z0_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z0 );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X1_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X1 );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z1_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z1 );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X2_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X2 );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z2_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z2 );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X3_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X3 );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z3_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z3 );

	m_defMapList[eEVENT_COL_RGN_TYPE_RADIUS].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RADIUS_X_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_RADIUS].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RADIUS_X );
	m_defMapList[eEVENT_COL_RGN_TYPE_RADIUS].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RADIUS_Z_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_RADIUS].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RADIUS_Z );
	m_defMapList[eEVENT_COL_RGN_TYPE_RADIUS].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RADIUS_R_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_RADIUS].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RADIUS_R );

	m_defMapList[eEVENT_COL_RGN_TYPE_RECTANGLE].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X0_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_RECTANGLE].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X0 );
	m_defMapList[eEVENT_COL_RGN_TYPE_RECTANGLE].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z0_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_RECTANGLE].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z0 );
	m_defMapList[eEVENT_COL_RGN_TYPE_RECTANGLE].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X1_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_RECTANGLE].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X1 );
	m_defMapList[eEVENT_COL_RGN_TYPE_RECTANGLE].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z1_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_RECTANGLE].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z1 );
	m_defMapList[eEVENT_COL_RGN_TYPE_RECTANGLE].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X2_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_RECTANGLE].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X2 );
	m_defMapList[eEVENT_COL_RGN_TYPE_RECTANGLE].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z2_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_RECTANGLE].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z2 );
	m_defMapList[eEVENT_COL_RGN_TYPE_RECTANGLE].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X3_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_RECTANGLE].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X3 );
	m_defMapList[eEVENT_COL_RGN_TYPE_RECTANGLE].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z3_STATIC );
	m_defMapList[eEVENT_COL_RGN_TYPE_RECTANGLE].push_back( IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z3 );
}

CAttr_EVT_ColRgn::~CAttr_EVT_ColRgn()
{
}

void CAttr_EVT_ColRgn::InitData( void )
{
	m_nCurSelIndex = -1;

	UpdateData( FALSE );
}

CString CAttr_EVT_ColRgn::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("widx"), m_dwWorldIdx );

	DWORD dwCCTData = eCOL_RESULT_TYPE_NOT_COLLISION;
	dwCCTData |= (m_ctrEnter.GetCheck() == BST_CHECKED) ? eCOL_RESULT_TYPE_ENTER : 0;
	dwCCTData |= (m_ctrLeave.GetCheck() == BST_CHECKED) ? eCOL_RESULT_TYPE_LEAVE : 0;
	dwCCTData |= (m_ctrThrough.GetCheck() == BST_CHECKED) ? eCOL_RESULT_TYPE_THROUGH : 0;
	strData += PakingPageData( _T("cct"), dwCCTData );

	DWORD dwCRType = m_ctrCRType.GetItemData( m_ctrCRType.GetCurSel() );
	strData += PakingPageData( _T("crt"), dwCRType );

	switch ( dwCRType )
	{
	case eEVENT_COL_RGN_TYPE_RADIUS:
		{
			strData += PakingPageData( _T("x"), m_fRadiusX );
			strData += PakingPageData( _T("z"), m_fRadiusZ );
			strData += PakingPageData( _T("r"), m_fRadiusR );
		}
		break;

	case eEVENT_COL_RGN_TYPE_RECTANGLE:
		{
			strData += PakingPageData( _T("x0"), m_fRectX0 );
			strData += PakingPageData( _T("z0"), m_fRectZ0 );
			strData += PakingPageData( _T("x1"), m_fRectX1 );
			strData += PakingPageData( _T("z1"), m_fRectZ1 );
			strData += PakingPageData( _T("x2"), m_fRectX2 );
			strData += PakingPageData( _T("z2"), m_fRectZ2 );
			strData += PakingPageData( _T("x3"), m_fRectX3 );
			strData += PakingPageData( _T("z3"), m_fRectZ3 );
		}
		break;
	}

	return strData;
}

void CAttr_EVT_ColRgn::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("widx") == strKey )
	{
		m_dwWorldIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("cct") == strKey )
	{
		DWORD dwCCTData = atoi( strValue.GetBuffer() );

		m_ctrEnter.SetCheck( BST_UNCHECKED );
		m_ctrLeave.SetCheck( BST_UNCHECKED );
		m_ctrThrough.SetCheck( BST_UNCHECKED );

		if ( dwCCTData & eCOL_RESULT_TYPE_ENTER )
		{
			m_ctrEnter.SetCheck( BST_CHECKED );
		}
		if ( dwCCTData & eCOL_RESULT_TYPE_LEAVE )
		{
			m_ctrLeave.SetCheck( BST_CHECKED );
		}
		if ( dwCCTData & eCOL_RESULT_TYPE_THROUGH )
		{
			m_ctrThrough.SetCheck( BST_CHECKED );
		}
	}
	else if ( _T("crt") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrCRType.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrCRType.GetItemData( i ) == nValue )
			{
				m_ctrCRType.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("x") == strKey )
	{
		m_fRadiusX = atof( strValue.GetBuffer() );
	}
	else if ( _T("z") == strKey )
	{
		m_fRadiusZ = atof( strValue.GetBuffer() );
	}
	else if ( _T("r") == strKey )
	{
		m_fRadiusR = atof( strValue.GetBuffer() );
	}
	else if ( _T("x0") == strKey )
	{
		m_fRectX0 = atof( strValue.GetBuffer() );
	}
	else if ( _T("z0") == strKey )
	{
		m_fRectZ0 = atof( strValue.GetBuffer() );
	}
	else if ( _T("x1") == strKey )
	{
		m_fRectX1 = atof( strValue.GetBuffer() );
	}
	else if ( _T("z1") == strKey )
	{
		m_fRectZ1 = atof( strValue.GetBuffer() );
	}
	else if ( _T("x2") == strKey )
	{
		m_fRectX2 = atof( strValue.GetBuffer() );
	}
	else if ( _T("z2") == strKey )
	{
		m_fRectZ2 = atof( strValue.GetBuffer() );
	}
	else if ( _T("x3") == strKey )
	{
		m_fRectX3 = atof( strValue.GetBuffer() );
	}
	else if ( _T("z3") == strKey )
	{
		m_fRectZ3 = atof( strValue.GetBuffer() );
	}
}

void CAttr_EVT_ColRgn::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_COL_RGN_WORLDIDX_EDITOR, m_dwWorldIdx);
	DDX_Control(pDX, IDC_TS_EVT_ATTR_COL_RGN_CCT_ENTER_CHECK, m_ctrEnter);
	DDX_Control(pDX, IDC_TS_EVT_ATTR_COL_RGN_CCT_LEAVE_CHECK, m_ctrLeave);
	DDX_Control(pDX, IDC_TS_EVT_ATTR_COL_RGN_CCT_THROUGH_CHECK, m_ctrThrough);
	DDX_Control(pDX, IDC_TS_EVT_ATTR_COL_RGN_CRT_COMBO, m_ctrCRType);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_COL_RGN_CRD_RADIUS_X, m_fRadiusX);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_COL_RGN_CRD_RADIUS_Z, m_fRadiusZ);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_COL_RGN_CRD_RADIUS_R, m_fRadiusR);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X0, m_fRectX0);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z0, m_fRectZ0);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X1, m_fRectX1);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z1, m_fRectZ1);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X2, m_fRectX2);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z2, m_fRectZ2);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_X3, m_fRectX3);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_COL_RGN_CRD_RECT_Z3, m_fRectZ3);
}


BOOL CAttr_EVT_ColRgn::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrEnter.SetCheck( BST_UNCHECKED );
	m_ctrLeave.SetCheck( BST_UNCHECKED );
	m_ctrThrough.SetCheck( BST_UNCHECKED );

	m_ctrCRType.SetItemData( m_ctrCRType.AddString( _T("사각 영역 데이터") ), eEVENT_COL_RGN_TYPE_RECTANGLE );
	int nIdx = m_ctrCRType.AddString( _T("반경 데이터") );
	m_ctrCRType.SetItemData( nIdx , eEVENT_COL_RGN_TYPE_RADIUS );
	m_ctrCRType.SetCurSel( nIdx );

	vecdef_ControlList::iterator it = m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].begin();
	for ( ; it != m_defMapList[eEVENT_COL_RGN_TYPE_INVALID].end(); ++it )
	{
		GetDlgItem( *it )->ShowWindow( SW_HIDE );
	}

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	OnCbnSelchangeTsEvtAttrColRgnCrtCombo();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BEGIN_MESSAGE_MAP(CAttr_EVT_ColRgn, CAttr_Page)
	ON_CBN_SELCHANGE(IDC_TS_EVT_ATTR_COL_RGN_CRT_COMBO, &CAttr_EVT_ColRgn::OnCbnSelchangeTsEvtAttrColRgnCrtCombo)
END_MESSAGE_MAP()


// CAttr_EVT_ColRgn 메시지 처리기입니다.

void CAttr_EVT_ColRgn::OnCbnSelchangeTsEvtAttrColRgnCrtCombo()
{
	UpdateData( TRUE );

	if ( -1 != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrCRType.GetItemData( m_nCurSelIndex );

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

	m_nCurSelIndex = m_ctrCRType.GetCurSel();
	if ( CB_ERR != m_nCurSelIndex )
	{
		DWORD dwData = (DWORD)m_ctrCRType.GetItemData( m_nCurSelIndex );

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
