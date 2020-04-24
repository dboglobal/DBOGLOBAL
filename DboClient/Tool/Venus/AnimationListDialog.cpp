// AnimationListDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "AnimationListDialog.h"
#include ".\animationlistdialog.h"

#include "rwcore.h"

#include "NtlResourceEffect.h"

#include "NtlEffectSystemFreeList.h"


// CAnimationListDialog 대화 상자입니다.

IMPLEMENT_DYNCREATE(CAnimationListDialog, CDialog)
CAnimationListDialog::CAnimationListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimationListDialog::IDD, pParent)
{
}

CAnimationListDialog::~CAnimationListDialog()
{
}

void CAnimationListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANIMATION_LIST, m_AnimationList);
}


BEGIN_MESSAGE_MAP(CAnimationListDialog, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CAnimationListDialog 메시지 처리기입니다.

BOOL CAnimationListDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_AnimationList.InitTreeCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAnimationListDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


BOOL CAnimationListDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) 
	{
		if(pMsg->wParam == VK_RETURN) return FALSE;	
		if(pMsg->wParam == VK_ESCAPE) return FALSE;	
        if(pMsg->wParam == VK_F5)
        {
            OnRefresh();
        }
	}
	return CDialog::PreTranslateMessage(pMsg);
}

//------------------------------------------------------------------
//	FuncName	: SerializeList
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CAnimationListDialog::SerializeList()
{
	CNtlResourceFileManager::svdef_String::iterator it;
	for (it = CNtlEffectSystemFreeList::m_AnimationFileManager.m_svExt.begin(); it != CNtlEffectSystemFreeList::m_AnimationFileManager.m_svExt.end(); ++ it)
	{
		m_AnimationList.AddExt((*it).c_str());
	}
	m_AnimationList.SerializeList(CNtlEffectSystemFreeList::m_AnimationFileManager.m_strPath.c_str());
}

void CAnimationListDialog::OnRefresh()
{
    m_AnimationList.DeleteAllItems();
    SerializeList();
}