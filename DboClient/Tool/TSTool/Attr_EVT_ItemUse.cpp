// Attr_EVT_ItemUse.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_EVT_ItemUse.h"


// CAttr_EVT_ItemUse 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_EVT_ItemUse, CAttr_Page, 1)

CAttr_EVT_ItemUse::CAttr_EVT_ItemUse(CWnd* pParent /*=NULL*/)
	: CAttr_Page(CAttr_EVT_ItemUse::IDD)
	, m_dwItemIdx(0xffffffff)
	, m_dwItemTargetIdx(0xffffffff)
{

}

CAttr_EVT_ItemUse::~CAttr_EVT_ItemUse()
{
}

CString CAttr_EVT_ItemUse::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("idx"), m_dwItemIdx );
	strData += PakingPageData( _T("ttype"), (int)m_ctrItemTargetTypeCombo.GetItemData( m_ctrItemTargetTypeCombo.GetCurSel() ) );
	strData += PakingPageData( _T("tidx"), m_dwItemTargetIdx );

	return strData;
}

void CAttr_EVT_ItemUse::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("idx") == strKey )
	{
		m_dwItemIdx = atoi( strValue.GetBuffer() );
	}
	else if ( _T("ttype") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrItemTargetTypeCombo.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrItemTargetTypeCombo.GetItemData( i ) == nValue )
			{
				m_ctrItemTargetTypeCombo.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("tidx") == strKey )
	{
		m_dwItemTargetIdx = atoi( strValue.GetBuffer() );
	}
}

void CAttr_EVT_ItemUse::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_EVT_ATTR_ITEM_USE_EDITOR, m_dwItemIdx);
	DDX_Control(pDX, IDC_TS_EVT_ATTR_ITEMUSE_ITEMTARGETTYPE_COMBO, m_ctrItemTargetTypeCombo );
	DDX_Text(pDX, IDC_TS_EVT_ATTR_ITEMUSE_ITEMTARGETIDX_EDITOR, m_dwItemTargetIdx);
}

BOOL CAttr_EVT_ItemUse::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrItemTargetTypeCombo.SetItemData( m_ctrItemTargetTypeCombo.AddString( _T("Mob") ), eEVENT_ITEM_TARGET_TYPE_MOB );
	m_ctrItemTargetTypeCombo.SetItemData( m_ctrItemTargetTypeCombo.AddString( _T("NPC") ), eEVENT_ITEM_TARGET_TYPE_NPC );
	m_ctrItemTargetTypeCombo.SetItemData( m_ctrItemTargetTypeCombo.AddString( _T("Avatar") ), eEVENT_ITEM_TARGET_TYPE_AVATAR );
	int nIdx = m_ctrItemTargetTypeCombo.AddString( _T("All") );
	m_ctrItemTargetTypeCombo.SetItemData( nIdx, eEVENT_ITEM_TARGET_TYPE_ALL );
	m_ctrItemTargetTypeCombo.SetCurSel( nIdx );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_EVT_ItemUse, CAttr_Page)
END_MESSAGE_MAP()


// CAttr_EVT_ItemUse 메시지 처리기입니다.
