// ProjectMainMenu.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "TSDoc.h"
#include "TSTrigger.h"
#include "TSProjectMainMenuForm.h"
#include "TSGroupIDInputDlg.h"


// CTSProjectMainMenuForm

CTSProjectMainMenuForm* g_pProjMainMenuForm = NULL;

IMPLEMENT_DYNCREATE(CTSProjectMainMenuForm, CFormView)

CTSProjectMainMenuForm::CTSProjectMainMenuForm()
	: CFormView(CTSProjectMainMenuForm::IDD)
{
	g_pProjMainMenuForm = this;
}

CTSProjectMainMenuForm::~CTSProjectMainMenuForm()
{
	g_pProjMainMenuForm = NULL;
}

void CTSProjectMainMenuForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROJECT_MAIN_MENU, m_ctrMainMenu);
	DDX_Control(pDX, IDC_PROJECT_TRIGGERLIST, m_ctrTriggerList);
	DDX_Control(pDX, IDC_MAIN_MENU_TEXT, m_ctrMainMenuText);
	DDX_Control(pDX, IDC_TRIGGER_LIST_TEXT, m_ctrTriggerListText);
	DDX_Control(pDX, IDC_MAIN_GROUP_CREATE, m_ctrMainGroupBtn);
	DDX_Control(pDX, IDC_EXCEPTION_TLIMIT_GROUP_CREATE, m_ctrGExpTLimitCreateBtn);
	DDX_Control(pDX, IDC_EXCEPTION_SERVER_GROUP_CREATE, m_ctrGExpServerCreateBtn);
	DDX_Control(pDX, IDC_EXCEPTION_CLIENT_GROUP_CREATE, m_ctrGExpClientCreateBtn);
	DDX_Control(pDX, IDC_EXCEPTION_GIVEUP_GROUP_CREATE, m_ctrGExpGiveUpCreateBtn);
	DDX_Control(pDX, IDC_EXCEPTION_GROUP_DELETE, m_ctrGExpDeleteBtn);
	DDX_Control(pDX, IDC_UPDATE_TRIGGER_LIST, m_ctrUpdateTriggerList);
}

BEGIN_MESSAGE_MAP(CTSProjectMainMenuForm, CFormView)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EXCEPTION_TLIMIT_GROUP_CREATE, &CTSProjectMainMenuForm::OnBnClickedExceptionTLimitGroupCreate)
	ON_BN_CLICKED(IDC_EXCEPTION_SERVER_GROUP_CREATE, &CTSProjectMainMenuForm::OnBnClickedExceptionServerGroupCreate)
	ON_BN_CLICKED(IDC_EXCEPTION_CLIENT_GROUP_CREATE, &CTSProjectMainMenuForm::OnBnClickedExceptionClientGroupCreate)
	ON_BN_CLICKED(IDC_EXCEPTION_GIVEUP_GROUP_CREATE, &CTSProjectMainMenuForm::OnBnClickedExceptionGiveUpGroupCreate)
	ON_BN_CLICKED(IDC_EXCEPTION_GROUP_DELETE, &CTSProjectMainMenuForm::OnBnClickedExceptionGroupDelete)
	ON_BN_CLICKED(IDC_UPDATE_TRIGGER_LIST, &CTSProjectMainMenuForm::OnBnClickedUpdateTriggerList)
	ON_BN_CLICKED(IDC_MAIN_GROUP_CREATE, &CTSProjectMainMenuForm::OnBnClickedMainGroupCreate)
END_MESSAGE_MAP()


// CTSProjectMainMenuForm 진단입니다.

#ifdef _DEBUG
void CTSProjectMainMenuForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTSProjectMainMenuForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTSProjectMainMenuForm 메시지 처리기입니다.
void CTSProjectMainMenuForm::OnSize(UINT nType, int cx, int cy)
{
	CSize size( cx, cy );
	SetScaleToFitSize( size );

	int nHeight = 0;
	CRect rect( 0, 0, 0, 0 );

	if ( m_ctrMainMenuText.GetSafeHwnd() )
	{
		nHeight = 5;

		m_ctrMainMenuText.GetClientRect( rect );
		rect.SetRect( rect.left + (cx-rect.Width())/2, nHeight, rect.left + (cx-rect.Width())/2 + rect.Width(), nHeight + 20 );
		m_ctrMainMenuText.MoveWindow( rect );

		nHeight += 20;
	}

	if ( m_ctrMainMenu.GetSafeHwnd() )
	{
		nHeight += 5;

		rect.SetRect( 0, nHeight, cx, nHeight + 150 );
		m_ctrMainMenu.MoveWindow( rect );

		nHeight += 150;
	}

	if ( m_ctrMainGroupBtn.GetSafeHwnd() )
	{
		nHeight += 5;

		rect.SetRect( 0, nHeight, cx, nHeight + 20 );
		m_ctrMainGroupBtn.MoveWindow( rect );

		nHeight += 20;
	}

	if ( m_ctrGExpTLimitCreateBtn.GetSafeHwnd() )
	{
		nHeight += 5;

		rect.SetRect( 0, nHeight, cx, nHeight + 20 );
		m_ctrGExpTLimitCreateBtn.MoveWindow( rect );

		nHeight += 20;
	}

	if ( m_ctrGExpServerCreateBtn.GetSafeHwnd() )
	{
		nHeight += 2;

		rect.SetRect( 0, nHeight, cx, nHeight + 20 );
		m_ctrGExpServerCreateBtn.MoveWindow( rect );

		nHeight += 20;
	}

	if ( m_ctrGExpClientCreateBtn.GetSafeHwnd() )
	{
		nHeight += 2;

		rect.SetRect( 0, nHeight, cx, nHeight + 20 );
		m_ctrGExpClientCreateBtn.MoveWindow( rect );

		nHeight += 20;
	}

	if ( m_ctrGExpGiveUpCreateBtn.GetSafeHwnd() )
	{
		nHeight += 2;

		rect.SetRect( 0, nHeight, cx, nHeight + 20 );
		m_ctrGExpGiveUpCreateBtn.MoveWindow( rect );

		nHeight += 20;
	}

	if ( m_ctrGExpDeleteBtn.GetSafeHwnd() )
	{
		nHeight += 5;

		rect.SetRect( 0, nHeight, cx, nHeight + 20 );
		m_ctrGExpDeleteBtn.MoveWindow( rect );

		nHeight += 20;
	}

	if ( m_ctrTriggerListText.GetSafeHwnd() )
	{
		nHeight += 5;

		m_ctrTriggerListText.GetClientRect( rect );
		rect.SetRect( rect.left + (cx-rect.Width())/2, nHeight, rect.left + (cx-rect.Width())/2 + rect.Width(), nHeight + 20 );
		m_ctrTriggerListText.MoveWindow( rect );

		nHeight += 20;
	}

	if ( m_ctrTriggerList.GetSafeHwnd() )
	{
		nHeight += 5;

		rect.SetRect( 0, nHeight, cx, cy - 25 );
		m_ctrTriggerList.MoveWindow( rect );

		nHeight = cy - 25;
	}

	if ( m_ctrUpdateTriggerList.GetSafeHwnd() )
	{
		nHeight += 5;

		rect.SetRect( 0, nHeight, cx, cy );
		m_ctrUpdateTriggerList.MoveWindow( rect );
	}
}

void CTSProjectMainMenuForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_ctrTriggerList.Init();
}

void CTSProjectMainMenuForm::OnBnClickedMainGroupCreate()
{
	if ( !CTSDoc::GetInstance()->IsProjectLoaded() ) return;

	CTSDoc::GetInstance()->GetProjectObject()->GetTrigger().AddMainGroup();
	CTSDoc::GetInstance()->SetModifiedFlag();
}

void CTSProjectMainMenuForm::OnBnClickedExceptionTLimitGroupCreate()
{
	if ( !CTSDoc::GetInstance()->IsProjectLoaded() ) return;

	CTSGroupIDInputDlg clDlg;
	clDlg.m_dwMin = NTL_TS_EXCEPT_TLIMT_GROUP_ID_BEGIN;
	clDlg.m_dwMax = NTL_TS_EXCEPT_TLIMT_GROUP_ID_END;
	clDlg.m_ctrGroupID = clDlg.m_dwMin;

	if ( IDOK == clDlg.DoModal() )
	{
		NTL_TS_TG_ID tgID = (NTL_TS_TG_ID)clDlg.m_ctrGroupID;
		if ( CTSDoc::GetInstance()->GetProjectObject()->GetTrigger().GetExceptionGroup( tgID ) )
		{
			AfxMessageBox( _T("해당 아이디가 이미 존재합니다") );
			return;
		}

		CTSDoc::GetInstance()->GetProjectObject()->GetTrigger().AddExceptionGroup( tgID );
		CTSDoc::GetInstance()->SetModifiedFlag();
	}
}

void CTSProjectMainMenuForm::OnBnClickedExceptionServerGroupCreate()
{
	if ( !CTSDoc::GetInstance()->IsProjectLoaded() ) return;

	CTSGroupIDInputDlg clDlg;
	clDlg.m_dwMin = NTL_TS_EXCEPT_SERVER_GROUP_ID_BEGIN;
	clDlg.m_dwMax = NTL_TS_EXCEPT_SERVER_GROUP_ID_END;
	clDlg.m_ctrGroupID = clDlg.m_dwMin;

	if ( IDOK == clDlg.DoModal() )
	{
		NTL_TS_TG_ID tgID = (NTL_TS_TG_ID)clDlg.m_ctrGroupID;
		if ( CTSDoc::GetInstance()->GetProjectObject()->GetTrigger().GetExceptionGroup( tgID ) )
		{
			AfxMessageBox( _T("해당 아이디가 이미 존재합니다") );
			return;
		}

		CTSDoc::GetInstance()->GetProjectObject()->GetTrigger().AddExceptionGroup( tgID );
		CTSDoc::GetInstance()->SetModifiedFlag();
	}
}


void CTSProjectMainMenuForm::OnBnClickedExceptionClientGroupCreate()
{
	if ( !CTSDoc::GetInstance()->IsProjectLoaded() ) return;

	CTSGroupIDInputDlg clDlg;
	clDlg.m_dwMin = NTL_TS_EXCEPT_CLIENT_GROUP_ID_BEGIN;
	clDlg.m_dwMax = NTL_TS_EXCEPT_CLIENT_GROUP_ID_END;
	clDlg.m_ctrGroupID = clDlg.m_dwMin;

	if ( IDOK == clDlg.DoModal() )
	{
		NTL_TS_TG_ID tgID = (NTL_TS_TG_ID)clDlg.m_ctrGroupID;
		if ( CTSDoc::GetInstance()->GetProjectObject()->GetTrigger().GetExceptionGroup( tgID ) )
		{
			AfxMessageBox( _T("해당 아이디가 이미 존재합니다") );
			return;
		}

		CTSDoc::GetInstance()->GetProjectObject()->GetTrigger().AddExceptionGroup( tgID );
		CTSDoc::GetInstance()->SetModifiedFlag();
	}
}

void CTSProjectMainMenuForm::OnBnClickedExceptionGiveUpGroupCreate()
{
	if ( !CTSDoc::GetInstance()->IsProjectLoaded() ) return;

	CTSGroupIDInputDlg clDlg;
	clDlg.m_dwMin = NTL_TS_EXCEPT_GIVEUP_ID;
	clDlg.m_dwMax = NTL_TS_EXCEPT_GIVEUP_ID;
	clDlg.m_ctrGroupID = clDlg.m_dwMin;

	if ( IDOK == clDlg.DoModal() )
	{
		NTL_TS_TG_ID tgID = (NTL_TS_TG_ID)clDlg.m_ctrGroupID;
		if ( CTSDoc::GetInstance()->GetProjectObject()->GetTrigger().GetExceptionGroup( tgID ) )
		{
			AfxMessageBox( _T("해당 아이디가 이미 존재합니다") );
			return;
		}

		CTSDoc::GetInstance()->GetProjectObject()->GetTrigger().AddExceptionGroup( tgID );
		CTSDoc::GetInstance()->SetModifiedFlag();
	}
}

void CTSProjectMainMenuForm::OnBnClickedExceptionGroupDelete()
{
	if ( !CTSDoc::GetInstance()->IsProjectLoaded() ) return;

	HTREEITEM hItem = m_ctrMainMenu.GetSelectedItem();
	if ( NULL == hItem )
	{
		AfxMessageBox( _T("삭제하고자 하는 그룹을 선택해주세요") );
		return;
	}

	NTL_TS_TG_ID tgID = (NTL_TS_TG_ID)m_ctrMainMenu.GetItemData( hItem );

	if ( 0 == tgID )
	{
		AfxMessageBox( _T("메인 그룹은 삭제할 수 없습니다") );
		return;
	}

	if ( IDOK == AfxMessageBox( _T("정말 해당 그룹을 지우시겠습니까?"), MB_OKCANCEL ) )
	{
		CTSDoc::GetInstance()->GetProjectObject()->GetTrigger().DelExceptionGroup( tgID );
		CTSDoc::GetInstance()->SetModifiedFlag();
	}
}

void CTSProjectMainMenuForm::OnBnClickedUpdateTriggerList()
{
	if ( !CTSDoc::GetInstance()->IsProjectLoaded() ) return;

	CTSDoc::GetInstance()->OnListReload();
}
