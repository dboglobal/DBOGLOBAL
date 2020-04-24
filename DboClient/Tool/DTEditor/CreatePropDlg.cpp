// CreatePropDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"

// simulation
#include "NtlSLTBCrowdManager.h"

// DTEdit
#include "CreatePropDlg.h"


// CCreatePropDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCreatePropDlg, CDialog)

CCreatePropDlg::CCreatePropDlg(CWnd* pParent /*=NULL*/)
: CDialog(CCreatePropDlg::IDD, pParent)
,m_nCreateType(INVALID_CREATE_TYPE)
,m_nIndex(0)
{
}

CCreatePropDlg::~CCreatePropDlg()
{
}

void CCreatePropDlg::SetCreateType(eCreateType byType)
{
	m_nCreateType = byType;	
}

int CCreatePropDlg::GetSelectedIndex(void) const
{
	return m_nIndex;
}

CString CCreatePropDlg::GetSelectedString()
{
	return m_strString;
}

void CCreatePropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD_PROP_LIST, m_PropList);
}


BEGIN_MESSAGE_MAP(CCreatePropDlg, CDialog)
	ON_WM_SHOWWINDOW()
	ON_LBN_SELCHANGE(IDC_ADD_PROP_LIST, &CCreatePropDlg::OnLbnSelchangeAddPropList)
END_MESSAGE_MAP()


// CCreatePropDlg 메시지 처리기입니다.

BOOL CCreatePropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	switch( m_nCreateType )
	{
	case CREATE_SPLINE:
		{
			m_PropList.AddString( _T("Line") );
			m_PropList.AddString( _T("Circle") );
			m_PropList.AddString( _T("Hurricane") );

			SetWindowText(_T("Add Property") );
			break;
		}
	case CREATE_DIRECTION_NODE:
		{
			CString strName;

			strName.LoadString(IDS_STRING_EFFECT);
			m_PropList.AddString( strName );

			strName.LoadString(IDS_STRING_SOUND);
			m_PropList.AddString( strName );

			strName.LoadString(IDS_STRING_REFREE);
			m_PropList.AddString( strName );

			SetWindowText(_T("DT Node Name") );
			break;
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CCreatePropDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	if(bShow)
	{
		m_nIndex = 0;
		m_PropList.SetCurSel(0);
	}
	else
	{
		m_nCreateType = INVALID_CREATE_TYPE;
	}
}

void CCreatePropDlg::OnLbnSelchangeAddPropList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_nIndex = m_PropList.GetCurSel();	

	m_PropList.GetText(m_nIndex, m_strString);
}
