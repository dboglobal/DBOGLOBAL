// Attr_Trigger.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "TSDoc.h"
#include "Attr_Trigger.h"
#include "TSProject.h"
#include "TextIdxChanger.h"
#include "RwdTblIdxChanger.h"


// CAttr_Trigger 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_Trigger, CAttr_Page, 1)

CAttr_Trigger::CAttr_Trigger()
	: CAttr_Page(CAttr_Trigger::IDD)
	, m_tID(NTL_TS_T_ID_INVALID)
	, m_uiTitle(0xffffffff)
{
}

CAttr_Trigger::~CAttr_Trigger()
{
}

CString CAttr_Trigger::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	strData += PakingPageData( _T("tid"), m_tID );
	strData += PakingPageData( _T("rq"), m_ctrRepeat.GetCheck() == BST_CHECKED ? 1 : 0 );
	strData += PakingPageData( _T("sq"), m_ctrCanShare.GetCheck() == BST_CHECKED ? 1 : 0 );
	strData += PakingPageData( _T("sm"), m_ctrOutStateMsg.GetCheck() == BST_CHECKED ? 1 : 0 );
	strData += PakingPageData( _T("title"), m_uiTitle );

	return strData;
}

void CAttr_Trigger::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("tid") == strKey )
	{
		if ( strValue.IsEmpty() )
		{
			m_tID = NTL_TS_T_ID_INVALID;
		}
		else
		{
			m_tID = atoi( strValue.GetBuffer() );
		}
	}
	else if ( _T("rq") == strKey )
	{
		if ( strValue.IsEmpty() )
		{
			m_ctrRepeat.SetCheck( BST_UNCHECKED );
		}
		else
		{
			if ( atoi( strValue.GetBuffer() ) == 1 )
			{
				m_ctrRepeat.SetCheck( BST_CHECKED );
			}
			else
			{
				m_ctrRepeat.SetCheck( BST_UNCHECKED );
			}
		}
	}
	else if ( _T("sq") == strKey )
	{
		if ( strValue.IsEmpty() )
		{
			m_ctrCanShare.SetCheck( BST_CHECKED );
		}
		else
		{
			if ( atoi( strValue.GetBuffer() ) == 1 )
			{
				m_ctrCanShare.SetCheck( BST_CHECKED );
			}
			else
			{
				m_ctrCanShare.SetCheck( BST_UNCHECKED );
			}
		}
	}
	else if ( _T("sm") == strKey )
	{
		if ( strValue.IsEmpty() )
		{
			m_ctrOutStateMsg.SetCheck( BST_CHECKED );
		}
		else
		{
			if ( atoi( strValue.GetBuffer() ) == 1 )
			{
				m_ctrOutStateMsg.SetCheck( BST_CHECKED );
			}
			else
			{
				m_ctrOutStateMsg.SetCheck( BST_UNCHECKED );
			}
		}
	}
	else if ( _T("title") == strKey )
	{
		if ( strValue.IsEmpty() )
		{
			m_uiTitle = 0xffffffff;
		}
		else
		{
			m_uiTitle = atoi( strValue.GetBuffer() );
		}
	}
}

void CAttr_Trigger::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_TS_TRIGGER_ATTR_ID_EDITOR, m_tID);
	DDV_MinMaxUInt(pDX, m_tID, 0, 65535);
	DDX_Text(pDX, IDC_TS_TRIGGER_ATTR_TITLE_EDITOR, m_uiTitle);
	DDV_MinMaxUInt(pDX, m_uiTitle, 0, 0xffffffff);
	DDX_Control(pDX, IDC_TS_TRIGGER_ATTR_REPEAT_QUEST_CHECK, m_ctrRepeat);
	DDX_Control(pDX, IDC_TS_TRIGGER_ATTR_SHARE_QUEST_CHECK, m_ctrCanShare);
	DDX_Control(pDX, IDC_TS_TRIGGER_ATTR_OUT_STATE_MSG_CHECK, m_ctrOutStateMsg);
}

BOOL CAttr_Trigger::OnInitDialog()
{
	CAttr_Page::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ctrRepeat.SetCheck( BST_UNCHECKED );
	m_ctrCanShare.SetCheck( BST_CHECKED );
	m_ctrOutStateMsg.SetCheck( BST_CHECKED );

	if ( m_strData.GetLength() > 0 ) SetPageData( m_strData );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BEGIN_MESSAGE_MAP(CAttr_Trigger, CAttr_Page)
	ON_BN_CLICKED(IDC_TS_TRIGGER_ATTR_DEF_INDEX_BTN, &CAttr_Trigger::OnBnClickedTsTriggerAttrDefIndexBtn)
	ON_BN_CLICKED(IDC_TS_TRIGGER_ATTR_RWD_APPLY_BTN, &CAttr_Trigger::OnBnClickedTsTriggerAttrRwdApplyBtn)
END_MESSAGE_MAP()


// CAttr_Trigger 메시지 처리기입니다.

void CAttr_Trigger::OnBnClickedTsTriggerAttrDefIndexBtn()
{
	UpdateData( true );

	if ( NTL_TS_T_ID_INVALID == m_tID )
	{
		AfxMessageBox( "Trigger id is invalid" );
	}
	else
	{
		CTSProject* pProj = CTSDoc::GetInstance()->GetProjectObject();

		if ( pProj )
		{
			CTextIdxChanger clChange( &pProj->GetTrigger() );

			clChange.Run();

			CTSDoc::GetInstance()->DoFileSave();

			CString strTitle = pProj->GetTrigger().GetAttr( _T("title") );
			m_uiTitle = (strTitle.IsEmpty() ? 0xffffffff : atoi( strTitle.GetBuffer() ));

			UpdateData( false );
		}
	}
}

void CAttr_Trigger::OnBnClickedTsTriggerAttrRwdApplyBtn()
{
	UpdateData( true );

	if ( NTL_TS_T_ID_INVALID == m_tID )
	{
		AfxMessageBox( "Trigger id is invalid" );
	}
	else
	{
		CTSProject* pProj = CTSDoc::GetInstance()->GetProjectObject();

		if ( pProj )
		{
			CRwdTblIdxChanger clChange( &pProj->GetTrigger() );

			clChange.Run();

			CTSDoc::GetInstance()->DoFileSave();
		}
	}
}
