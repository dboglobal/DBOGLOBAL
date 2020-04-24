// UvAniListDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "UvAniListDialog.h"
#include ".\uvanilistdialog.h"

#include "rwcore.h"

#include "NtlEffectSystemFreeList.h"


// CUvAniListDialog 대화 상자입니다.

IMPLEMENT_DYNCREATE(CUvAniListDialog, CDialog)
CUvAniListDialog::CUvAniListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CUvAniListDialog::IDD, pParent)
{
}

CUvAniListDialog::~CUvAniListDialog()
{
}

void CUvAniListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_UVANIMATION_LIST, m_UvAnimationList);
}


BEGIN_MESSAGE_MAP(CUvAniListDialog, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CUvAniListDialog 메시지 처리기입니다.

BOOL CUvAniListDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_UvAnimationList.InitTreeCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CUvAniListDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CUvAniListDialog::PreTranslateMessage(MSG* pMsg)
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
void CUvAniListDialog::SerializeList()
{
	CNtlResourceFileManager::svdef_String::iterator it;
	for (it = CNtlEffectSystemFreeList::m_UvAnimationFileManager.m_svExt.begin(); it != CNtlEffectSystemFreeList::m_UvAnimationFileManager.m_svExt.end(); ++ it)
	{
		m_UvAnimationList.AddExt((*it).c_str());
	}
	m_UvAnimationList.SerializeList(CNtlEffectSystemFreeList::m_UvAnimationFileManager.m_strPath.c_str());
}

void CUvAniListDialog::OnRefresh()
{
    m_UvAnimationList.DeleteAllItems();
    SerializeList();
}

