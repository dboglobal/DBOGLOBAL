// Attr_ACT_Drop.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_Drop.h"
#include "NtlSharedType.h"
#include "NtlDirection.h"


// CAttr_ACT_Drop 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_Drop, CAttr_Page, 1)

CAttr_ACT_Drop::CAttr_ACT_Drop()
	: CAttr_Page(CAttr_ACT_Drop::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwProbTblIdx( 0xffffffff )
{

}

CAttr_ACT_Drop::~CAttr_ACT_Drop()
{
}

CString CAttr_ACT_Drop::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );
	strData += PakingPageData( _T("ptype"), (int)m_ctrPermissionTypeCtrl.GetItemData( m_ctrPermissionTypeCtrl.GetCurSel() ) );
	strData += PakingPageData( _T("tblidx"), m_dwProbTblIdx );

	return strData;
}

void CAttr_ACT_Drop::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("ptype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrPermissionTypeCtrl.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrPermissionTypeCtrl.GetItemData( i ) == nValue )
			{
				m_ctrPermissionTypeCtrl.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("tblidx") == strKey )
	{
		m_dwProbTblIdx = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_Drop::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_DROP_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_DROP_PERMISSION_TYPE_COMBO, m_ctrPermissionTypeCtrl);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_DROP_PROBTBLIDX_EDITOR, m_dwProbTblIdx);
}

BOOL CAttr_ACT_Drop::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrPermissionTypeCtrl.SetItemData( m_ctrPermissionTypeCtrl.AddString( _T("개인") ), eQUEST_DROP_PERMISSION_TYPE_PERSONAL );
	int nIdx = m_ctrPermissionTypeCtrl.AddString( _T("파티") );
	m_ctrPermissionTypeCtrl.SetItemData( nIdx, eQUEST_DROP_PERMISSION_TYPE_PARTY );
	m_ctrPermissionTypeCtrl.SetCurSel( nIdx );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_Drop, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_ACT_Drop 메시지 처리기입니다.
