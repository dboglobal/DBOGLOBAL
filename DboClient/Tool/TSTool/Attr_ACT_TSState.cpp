// Attr_ACT_Cin.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_ACT_TSState.h"


// CAttr_ACT_TSState 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_ACT_TSState, CAttr_Page, 1)

CAttr_ACT_TSState::CAttr_ACT_TSState()
	: CAttr_Page(CAttr_ACT_TSState::IDD)
	, m_taID(NTL_TS_TA_ID_INVALID)
{

}

CAttr_ACT_TSState::~CAttr_ACT_TSState()
{
}

CString CAttr_ACT_TSState::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("taid"), m_taID );

	if ( m_ctrTypeAddBtn.GetCheck() == BST_CHECKED ) strData += PakingPageData( _T("type"), eTSSTATE_TYPE_ADD );
	else strData += PakingPageData( _T("type"), eTSSTATE_TYPE_REMOVE );

	DWORD dwState = 0;
	dwState |= (m_ctrTSFailed.GetCheck() == BST_CHECKED) ? eTS_SVR_STATE_FAILED : 0;
	strData += PakingPageData( _T("state"), dwState );

	return strData;
}

void CAttr_ACT_TSState::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("taid") == strKey )
	{
		m_taID = atoi( strValue.GetBuffer() );
	}

	if ( _T("type") == strKey )
	{
		DWORD dwType = atoi( strValue.GetBuffer() );

		if ( eTSSTATE_TYPE_ADD == dwType )
		{
			m_ctrTypeAddBtn.SetCheck( BST_CHECKED );
			m_ctrTypeRemoveBtn.SetCheck( BST_UNCHECKED );
		}
		else
		{
			m_ctrTypeAddBtn.SetCheck( BST_UNCHECKED );
			m_ctrTypeRemoveBtn.SetCheck( BST_CHECKED );
		}
	}

	if ( _T("state") == strKey )
	{
		DWORD dwState = atoi( strValue.GetBuffer() );

		if ( dwState & eTS_SVR_STATE_FAILED ) m_ctrTSFailed.SetCheck( BST_CHECKED );
	}
}

void CAttr_ACT_TSState::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TS_ACT_ATTR_TSSTATE_ID_EDITOR, m_taID);
	DDV_MinMaxUInt(pDX, m_taID, 0, NTL_TS_TA_ID_INVALID);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_TSSTATE_TYPE_ADD_CHECK, m_ctrTypeAddBtn);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_TSSTATE_TYPE_REMOVE_CHECK, m_ctrTypeRemoveBtn);
	DDX_Control(pDX, IDC_TS_ACT_ATTR_TSSTATE_FAILED_CHECK, m_ctrTSFailed);
}

BOOL CAttr_ACT_TSState::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrTypeAddBtn.SetCheck( BST_CHECKED );
	m_ctrTypeRemoveBtn.SetCheck( BST_UNCHECKED );

	m_ctrTSFailed.SetCheck( BST_UNCHECKED );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BEGIN_MESSAGE_MAP(CAttr_ACT_TSState, CAttr_Page)
	ON_BN_CLICKED(IDC_TS_ACT_ATTR_TSSTATE_TYPE_ADD_CHECK, &CAttr_ACT_TSState::OnBnClickedTsActAttrTsstateTypeAddCheck)
	ON_BN_CLICKED(IDC_TS_ACT_ATTR_TSSTATE_TYPE_REMOVE_CHECK, &CAttr_ACT_TSState::OnBnClickedTsActAttrTsstateTypeRemoveCheck)
END_MESSAGE_MAP()


// CAttr_ACT_TSState 메시지 처리기입니다.

void CAttr_ACT_TSState::OnBnClickedTsActAttrTsstateTypeAddCheck()
{
	if ( m_ctrTypeAddBtn.GetCheck() == BST_CHECKED )
	{
		m_ctrTypeRemoveBtn.SetCheck( BST_UNCHECKED );
	}
}

void CAttr_ACT_TSState::OnBnClickedTsActAttrTsstateTypeRemoveCheck()
{
	if ( m_ctrTypeRemoveBtn.GetCheck() == BST_CHECKED )
	{
		m_ctrTypeAddBtn.SetCheck( BST_UNCHECKED );
	}
}
