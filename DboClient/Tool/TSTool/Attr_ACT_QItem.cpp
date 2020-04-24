// Attr_ACT_QItem.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_QItem.h"


// CAttr_ACT_QItem 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_QItem, CAttr_Page, 1)

CAttr_ACT_QItem::CAttr_ACT_QItem()
	: CAttr_Page(CAttr_ACT_QItem::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
	, m_dwItemIdx0(0xffffffff)
	, m_nItemCnt0(255)
	, m_fItemProb0(1.f)
	, m_dwItemIdx1(0xffffffff)
	, m_nItemCnt1(255)
	, m_fItemProb1(1.f)
	, m_dwItemIdx2(0xffffffff)
	, m_nItemCnt2(255)
	, m_fItemProb2(1.f)
{

}

CAttr_ACT_QItem::~CAttr_ACT_QItem()
{
}

CString CAttr_ACT_QItem::GetPageData( void )
{
	UpdateData( TRUE );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );

	if ( m_ctrCreateBtn.GetCheck() == BST_CHECKED )			strData += PakingPageData( _T("type"), eQITEM_TYPE_CREATE );
	else if ( m_ctrDeleteBtn.GetCheck() == BST_CHECKED )	strData += PakingPageData( _T("type"), eQITEM_TYPE_DELETE );

	if ( 0xffffffff != m_dwItemIdx0 )
	{
		strData += PakingPageData( _T("iidx0"), m_dwItemIdx0 );
		strData += PakingPageData( _T("icnt0"), m_nItemCnt0 );
		strData += PakingPageData( _T("iprob0"), m_fItemProb0 );
	}
	if ( 0xffffffff != m_dwItemIdx1 )
	{
		strData += PakingPageData( _T("iidx1"), m_dwItemIdx1 );
		strData += PakingPageData( _T("icnt1"), m_nItemCnt1 );
		strData += PakingPageData( _T("iprob1"), m_fItemProb1 );
	}
	if ( 0xffffffff != m_dwItemIdx2 )
	{
		strData += PakingPageData( _T("iidx2"), m_dwItemIdx2 );
		strData += PakingPageData( _T("icnt2"), m_nItemCnt2 );
		strData += PakingPageData( _T("iprob2"), m_fItemProb2 );
	}

	return strData;
}

void CAttr_ACT_QItem::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}
	else if ( _T("type") == strKey )
	{
		eQITEM_TYPE eType = (eQITEM_TYPE)atoi( strValue.GetBuffer() );

		if ( eQITEM_TYPE_CREATE == eType )
		{
			m_ctrCreateBtn.SetCheck( BST_CHECKED );
			m_ctrDeleteBtn.SetCheck( BST_UNCHECKED );
		}
		else if ( eQITEM_TYPE_DELETE == eType )
		{
			m_ctrCreateBtn.SetCheck( BST_UNCHECKED );
			m_ctrDeleteBtn.SetCheck( BST_CHECKED );
		}
	}
	else if ( _T("iidx0") == strKey )
	{
		m_dwItemIdx0 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("icnt0") == strKey )
	{
		m_nItemCnt0 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("iprob0") == strKey )
	{
		m_fItemProb0 = atof( strValue.GetBuffer() );
	}
	else if ( _T("iidx1") == strKey )
	{
		m_dwItemIdx1 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("icnt1") == strKey )
	{
		m_nItemCnt1 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("iprob1") == strKey )
	{
		m_fItemProb1 = atof( strValue.GetBuffer() );
	}
	else if ( _T("iidx2") == strKey )
	{
		m_dwItemIdx2 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("icnt2") == strKey )
	{
		m_nItemCnt2 = atoi( strValue.GetBuffer() );
	}
	else if ( _T("iprob2") == strKey )
	{
		m_fItemProb2 = atof( strValue.GetBuffer() );
	}
}

void CAttr_ACT_QItem::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_QITEM_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_QITEM_CREATE_CHECK, m_ctrCreateBtn);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_QITEM_DELETE_CHECK, m_ctrDeleteBtn);

	DDX_Text(pDX, IDC_TS_ACT_ATTR_QITEM_0_ITEMIDX_EDITOR, m_dwItemIdx0);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_QITEM_0_ITEMCNT_EDITOR, m_nItemCnt0);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_QITEM_0_ITEMPROB_EDITOR, m_fItemProb0);
	DDV_MinMaxFloat( pDX, m_fItemProb0, 0.f, 1.f);

	DDX_Text(pDX, IDC_TS_ACT_ATTR_QITEM_1_ITEMIDX_EDITOR, m_dwItemIdx1);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_QITEM_1_ITEMCNT_EDITOR, m_nItemCnt1);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_QITEM_1_ITEMPROB_EDITOR, m_fItemProb1);
	DDV_MinMaxFloat( pDX, m_fItemProb1, 0.f, 1.f);

	DDX_Text(pDX, IDC_TS_ACT_ATTR_QITEM_2_ITEMIDX_EDITOR, m_dwItemIdx2);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_QITEM_2_ITEMCNT_EDITOR, m_nItemCnt2);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_QITEM_2_ITEMPROB_EDITOR, m_fItemProb2);
	DDV_MinMaxFloat( pDX, m_fItemProb2, 0.f, 1.f);

	DDX_Control(pDX, IDC_TS_ACT_ATTR_QITEM_DELETE_ALL_BTN, m_ctrDelAllBtn);
}

BOOL CAttr_ACT_QItem::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrCreateBtn.SetCheck( BST_CHECKED );
	m_ctrDeleteBtn.SetCheck( BST_UNCHECKED );

	m_ctrDelAllBtn.ShowWindow( SW_HIDE );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_QItem, CAttr_Page)
	ON_BN_CLICKED(IDC_TS_ACT_ATTR_QITEM_CREATE_CHECK, &CAttr_ACT_QItem::OnBnClickedTsActAttrQitemCreateCheck)
	ON_BN_CLICKED(IDC_TS_ACT_ATTR_QITEM_DELETE_CHECK, &CAttr_ACT_QItem::OnBnClickedTsActAttrQitemDeleteCheck)
	ON_BN_CLICKED(IDC_TS_ACT_ATTR_QITEM_DELETE_ALL_BTN, &CAttr_ACT_QItem::OnBnClickedTsActAttrQitemDeleteAllBtn)
END_MESSAGE_MAP()


// CAttr_ACT_QItem 메시지 처리기입니다.

void CAttr_ACT_QItem::OnBnClickedTsActAttrQitemCreateCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( BST_CHECKED == m_ctrCreateBtn.GetCheck() )
	{
		m_ctrDelAllBtn.ShowWindow( SW_HIDE );

		m_ctrDeleteBtn.SetCheck( BST_UNCHECKED );
	}
	else
	{
		m_ctrDelAllBtn.ShowWindow( SW_SHOW );

		m_ctrDeleteBtn.SetCheck( BST_CHECKED );
	}
}

void CAttr_ACT_QItem::OnBnClickedTsActAttrQitemDeleteCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if ( BST_CHECKED == m_ctrDeleteBtn.GetCheck() )
	{
		m_ctrDelAllBtn.ShowWindow( SW_SHOW );

		m_ctrCreateBtn.SetCheck( BST_UNCHECKED );
	}
	else
	{
		m_ctrDelAllBtn.ShowWindow( SW_HIDE );

		m_ctrCreateBtn.SetCheck( BST_CHECKED );
	}
}

void CAttr_ACT_QItem::OnBnClickedTsActAttrQitemDeleteAllBtn()
{
	m_nItemCnt0 = 255;
	m_fItemProb0 = 1.f;

	m_nItemCnt1 = 255;
	m_fItemProb1 = 1.f;

	m_nItemCnt2 = 255;
	m_fItemProb2 = 1.f;

	UpdateData( FALSE );
}
