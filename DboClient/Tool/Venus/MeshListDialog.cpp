// MeshListDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "MeshListDialog.h"
#include ".\meshlistdialog.h"

#include "rwcore.h"

#include "VenusFramework.h"
#include "NtlEffectSystemFreeList.h"

// CMeshListDialog 대화 상자입니다.

IMPLEMENT_DYNCREATE(CMeshListDialog, CDialog)
CMeshListDialog::CMeshListDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMeshListDialog::IDD, pParent)
{
	m_Initialized = FALSE;
}

CMeshListDialog::~CMeshListDialog()
{
}

void CMeshListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESH_LIST, m_MeshList);
}


BEGIN_MESSAGE_MAP(CMeshListDialog, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_MESH_LIST, OnTvnSelchangedMeshList)
END_MESSAGE_MAP()


// CMeshListDialog 메시지 처리기입니다.

BOOL CMeshListDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Initialized = TRUE;

	m_MeshList.InitTreeCtrl();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CMeshListDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_Initialized)
	{
	}
}


BOOL CMeshListDialog::PreTranslateMessage(MSG* pMsg)
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
void CMeshListDialog::SerializeList()
{
	CNtlResourceFileManager::svdef_String::iterator it;
	for (it = CNtlEffectSystemFreeList::m_ClumpFileManager.m_svExt.begin(); it != CNtlEffectSystemFreeList::m_ClumpFileManager.m_svExt.end(); ++ it)
	{
		m_MeshList.AddExt((*it).c_str());
	}
	m_MeshList.SerializeList(CNtlEffectSystemFreeList::m_ClumpFileManager.m_strPath.c_str());
}


void CMeshListDialog::OnTvnSelchangedMeshList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM hItem = m_MeshList.GetSelectedItem();
	if(hItem == NULL)
	{
		CVenusFramework::GetInstance().CreateEffectClump(NULL);
		return;
	}

	CString strFileName = m_MeshList.GetItemText(hItem);
	const RwChar* strPathName = CNtlEffectSystemFreeList::m_ClumpFileManager.GetPathName(strFileName.GetBuffer(0));
	CVenusFramework::GetInstance().CreateEffectClump(strPathName);
	*pResult = 0;
}

void CMeshListDialog::OnRefresh()
{
    m_MeshList.DeleteAllItems();
    SerializeList();
}
