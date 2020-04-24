// Attr_ACT_Item.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_Item.h"


// CAttr_ACT_Item 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_Item, CAttr_Page, 1)

CAttr_ACT_Item::CAttr_ACT_Item()
	: CAttr_Page(CAttr_ACT_Item::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwItemIdx0(0xffffffff)
	, m_dwItemIdx1(0xffffffff)
	, m_dwItemIdx2(0xffffffff)
{

}

CAttr_ACT_Item::~CAttr_ACT_Item()
{
}

CString CAttr_ACT_Item::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );

	if ( m_ctrCreateBtn.GetCheck() == BST_CHECKED )			strData += PakingPageData( _T("type"), eTSITEM_TYPE_CREATE );
	else if ( m_ctrDeleteBtn.GetCheck() == BST_CHECKED )	strData += PakingPageData( _T("type"), eTSITEM_TYPE_DELETE );

	if ( 0xffffffff != m_dwItemIdx0 )
	{
		strData += PakingPageData( _T("stype0"), (int)m_ctrSType0.GetItemData( m_ctrSType0.GetCurSel() ) );
		strData += PakingPageData( _T("iidx0"), m_dwItemIdx0 );
	}
	if ( 0xffffffff != m_dwItemIdx1 )
	{
		strData += PakingPageData( _T("stype1"), (int)m_ctrSType1.GetItemData( m_ctrSType1.GetCurSel() ) );
		strData += PakingPageData( _T("iidx1"), m_dwItemIdx1 );
	}
	if ( 0xffffffff != m_dwItemIdx2 )
	{
		strData += PakingPageData( _T("stype2"), (int)m_ctrSType2.GetItemData( m_ctrSType2.GetCurSel() ) );
		strData += PakingPageData( _T("iidx2"), m_dwItemIdx2 );
	}

	return strData;
}

void CAttr_ACT_Item::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("type") == strKey )
	{
		eTSITEM_TYPE eType = (eTSITEM_TYPE)atoi( strValue.GetBuffer() );

		if ( eTSITEM_TYPE_CREATE == eType )
		{
			m_ctrCreateBtn.SetCheck( BST_CHECKED );
			m_ctrDeleteBtn.SetCheck( BST_UNCHECKED );
		}
		else if ( eTSITEM_TYPE_DELETE == eType )
		{
			m_ctrCreateBtn.SetCheck( BST_UNCHECKED );
			m_ctrDeleteBtn.SetCheck( BST_CHECKED );
		}
	}
	else if ( _T("stype0") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrSType0.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrSType0.GetItemData( i ) == nValue )
			{
				m_ctrSType0.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("iidx0") == strKey )
	{
		m_dwItemIdx0 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("stype1") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrSType1.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrSType1.GetItemData( i ) == nValue )
			{
				m_ctrSType1.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("iidx1") == strKey )
	{
		m_dwItemIdx1 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("stype2") == strKey )
	{
		int nValue = atoi( strValue.GetBuffer() );

		int nCnt = m_ctrSType2.GetCount();
		for ( int i = 0; i < nCnt; ++i )
		{
			if ( m_ctrSType2.GetItemData( i ) == nValue )
			{
				m_ctrSType2.SetCurSel( i );
				break;
			}
		}
	}
	else if ( _T("iidx2") == strKey )
	{
		m_dwItemIdx2 = atoi( strValue.GetBuffer() );
	}
}

void CAttr_ACT_Item::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_ITEM_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_ITEM_CREATE_CHECK, m_ctrCreateBtn);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_ITEM_DELETE_CHECK, m_ctrDeleteBtn);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_ITEM_0_ITEMSLOTTYPE_COMBO, m_ctrSType0);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_ITEM_0_ITEMIDX_EDITOR, m_dwItemIdx0);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_ITEM_1_ITEMSLOTTYPE_COMBO, m_ctrSType1);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_ITEM_1_ITEMIDX_EDITOR, m_dwItemIdx1);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_ITEM_2_ITEMSLOTTYPE_COMBO, m_ctrSType2);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_ITEM_2_ITEMIDX_EDITOR, m_dwItemIdx2);
}

BOOL CAttr_ACT_Item::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrCreateBtn.SetCheck( BST_CHECKED );
	m_ctrDeleteBtn.SetCheck( BST_UNCHECKED );

	int nIdx;

	m_ctrSType0.SetItemData( m_ctrSType0.AddString( _T("Equip") ), eTSITEM_SLOT_TYPE_EQUIP );
	m_ctrSType0.SetItemData( m_ctrSType0.AddString( _T("Parts") ), eTSITEM_SLOT_TYPE_PARTS );
	nIdx = m_ctrSType0.AddString( _T("Bag") ); m_ctrSType0.SetItemData( nIdx, eTSITEM_SLOT_TYPE_BAG );
	m_ctrSType0.SetCurSel( nIdx );

	m_ctrSType1.SetItemData( m_ctrSType1.AddString( _T("Equip") ), eTSITEM_SLOT_TYPE_EQUIP );
	m_ctrSType1.SetItemData( m_ctrSType1.AddString( _T("Parts") ), eTSITEM_SLOT_TYPE_PARTS );
	nIdx = m_ctrSType1.AddString( _T("Bag") ); m_ctrSType1.SetItemData( nIdx, eTSITEM_SLOT_TYPE_BAG );
	m_ctrSType1.SetCurSel( nIdx );

	m_ctrSType2.SetItemData( m_ctrSType2.AddString( _T("Equip") ), eTSITEM_SLOT_TYPE_EQUIP );
	m_ctrSType2.SetItemData( m_ctrSType2.AddString( _T("Parts") ), eTSITEM_SLOT_TYPE_PARTS );
	nIdx = m_ctrSType2.AddString( _T("Bag") ); m_ctrSType2.SetItemData( nIdx, eTSITEM_SLOT_TYPE_BAG );
	m_ctrSType2.SetCurSel( nIdx );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BEGIN_MESSAGE_MAP(CAttr_ACT_Item, CAttr_Page)
	ON_BN_CLICKED(IDC_TS_ACT_ATTR_ITEM_CREATE_CHECK, &CAttr_ACT_Item::OnBnClickedTsActAttrItemCreateCheck)
	ON_BN_CLICKED(IDC_TS_ACT_ATTR_ITEM_DELETE_CHECK, &CAttr_ACT_Item::OnBnClickedTsActAttrItemDeleteCheck)
END_MESSAGE_MAP()


// CAttr_ACT_Item 메시지 처리기입니다.

void CAttr_ACT_Item::OnBnClickedTsActAttrItemCreateCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( BST_CHECKED == m_ctrCreateBtn.GetCheck() )
	{
		m_ctrDeleteBtn.SetCheck( BST_UNCHECKED );
	}
	else
	{
		m_ctrDeleteBtn.SetCheck( BST_CHECKED );
	}
}

void CAttr_ACT_Item::OnBnClickedTsActAttrItemDeleteCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( BST_CHECKED == m_ctrDeleteBtn.GetCheck() )
	{
		m_ctrCreateBtn.SetCheck( BST_UNCHECKED );
	}
	else
	{
		m_ctrCreateBtn.SetCheck( BST_CHECKED );
	}
}
