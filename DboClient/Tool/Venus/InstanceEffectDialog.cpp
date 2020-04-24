// InstanceEffectDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "InstanceEffectDialog.h"
#include ".\instanceeffectdialog.h"


#include ".\venusvisualmanager.h"

#include "NtlDebug.h"
#include "NtlInstanceEffect.h"

// CInstanceEffectDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(CInstanceEffectDialog, CDialog)
CInstanceEffectDialog::CInstanceEffectDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInstanceEffectDialog::IDD, pParent)
{
}

CInstanceEffectDialog::~CInstanceEffectDialog()
{
}

void CInstanceEffectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EFFECT_LIST, m_EffectList);
}


BEGIN_MESSAGE_MAP(CInstanceEffectDialog, CDialog)
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(IDC_EFFECT_LIST, OnLbnSelchangeEffectList)
END_MESSAGE_MAP()


// CInstanceEffectDialog 메시지 처리기입니다.
CInstanceEffectDialog& CInstanceEffectDialog::GetInstance()
{
	static CInstanceEffectDialog dlg;
	return dlg;
}

BOOL CInstanceEffectDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRect rect;
	GetWindowRect(&rect);
	CPoint posStart;
	posStart.x = GetSystemMetrics(SM_CXSCREEN) / 2 - rect.Width() / 2;
	posStart.y = GetSystemMetrics(SM_CYSCREEN) / 2 - rect.Height() / 2;

	rect.left += posStart.x;
	rect.right += posStart.x;
	rect.top += posStart.y;
	rect.bottom += posStart.y;
	MoveWindow(rect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CInstanceEffectDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CInstanceEffectDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) 
	{
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CInstanceEffectDialog::BuildInstanceEffect()
{
	m_EffectList.ResetContent();
}


void CInstanceEffectDialog::OnLbnSelchangeEffectList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CVenusVisualManager::GetInstance().m_svSelectInstanceEffect.clear();

	int nSelectCount = m_EffectList.GetSelCount();
	if (nSelectCount <= 0) 
	{
		return;
	}

	int* pListIndex = NTL_NEW int[nSelectCount];
	m_EffectList.GetSelItems(nSelectCount, pListIndex);
	for (int i = 0; i < nSelectCount; ++ i)
	{
//		CNtlInstanceEffect* pInstanceEffect = (CNtlInstanceEffect*)m_EffectList.GetItemData(pListIndex[i]);
//		CVenusVisualManager::GetInstance().m_slSelectInstanceEffect.push_back(pInstanceEffect->GetEffectHandle());
	}

	NTL_ARRAY_DELETE(pListIndex);
}
