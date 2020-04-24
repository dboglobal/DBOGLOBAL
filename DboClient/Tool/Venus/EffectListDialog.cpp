// EffectListDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "EffectListDialog.h"
#include ".\effectlistdialog.h"


// CEffectListDialog 대화 상자입니다.

IMPLEMENT_DYNCREATE(CEffectListDialog, CDialog)
CEffectListDialog::CEffectListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CEffectListDialog::IDD, pParent)
{
	m_Initialized = FALSE;
}

CEffectListDialog::~CEffectListDialog()
{
}

void CEffectListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESOURCE_EFFECT_LIST, m_EffectFrame);
}


BEGIN_MESSAGE_MAP(CEffectListDialog, CDialog)
	ON_WM_SIZE()
	ON_WM_CREATE()    
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CEffectListDialog 메시지 처리기입니다.

BOOL CEffectListDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Initialized = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEffectListDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_Initialized)
	{
		CRect rc;
		m_EffectFrame.GetWindowRect(rc);
		ScreenToClient(rc);
		m_EffectFrame.ShowWindow(SW_HIDE);
		if(m_List.GetSafeHwnd())
				m_List.MoveWindow(rc);
//		m_List.SetWindowPos(0,-1,-1,cx,cy,SWP_SHOWWINDOW);
	}
}

BOOL CEffectListDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == VK_RETURN) return FALSE;	
		if(pMsg->wParam == VK_ESCAPE) return FALSE;	
	}
	return CDialog::PreTranslateMessage(pMsg);
}


int CEffectListDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_List.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, CRect(0,0,0,0),this,0x1001);	
	m_List.InitializeGrid();
//	m_List.SetExtendedStyle(0);
	m_List.ModifyStyleEx(0,WS_EX_CLIENTEDGE);

	return 0;
}


void CEffectListDialog::ResetTreeItemName(const RwChar* strName)
{
	m_List.ResetTreeItemName(strName);
}


//------------------------------------------------------------------
//	FuncName	: RebuildResourceEffect
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEffectListDialog::RebuildResourceEffect()
{
	m_List.RebuildResourceEffect();
}

// 내부의 리스트 컨트롤을 제외한 틀을 클릭했을때 호출된다.
// 리스트 컨트롤에서 선택한 이펙트를 해제한다.
void CEffectListDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
    int nSelect = m_List.GetSelectedItem();
    m_List.SetItemState(nSelect, LVIS_FOCUSED, LVIS_SELECTED);

    CDialog::OnLButtonUp(nFlags, point);
}

RwBool CEffectListDialog::SaveXMLTree( RwChar* szFileName ) 
{
    return m_List.SaveXMLTree(szFileName);
}