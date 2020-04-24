// AnimAssetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CharacterEditor.h"
#include "AnimationTreeDialog.h"
#include ".\AnimationTreeDialog.h"
#include "ToolData.h"

// CAnimAssetDlg 대화 상자입니다.
IMPLEMENT_DYNAMIC(CAnimationTreeDialog, CDialog)
CAnimationTreeDialog::CAnimationTreeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimationTreeDialog::IDD, pParent)
{

}

CAnimationTreeDialog::~CAnimationTreeDialog()
{
}

void CAnimationTreeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE2, m_AnimAssetTreeCtrl);
}


BEGIN_MESSAGE_MAP(CAnimationTreeDialog, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE2, OnTvnSelchangedTree2)
//	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE2, OnTvnSelchangingTree2)
ON_WM_DESTROY()
END_MESSAGE_MAP()


// CAnimAssetDlg 메시지 처리기입니다.

BOOL CAnimationTreeDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	theApp.m_pAnimationTreeDialog = this;

	
	//theDbo.
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAnimationTreeDialog::UpdateTree(CDboCharacterApplication *pDboApp)
{
	CNtlPLActionName *pActionName         = pDboApp->GetActionName();
	CNtlPLActionName *pConditionName      = pDboApp->GetConditionName();
	CNtlPLAnimationTable *pAnimationTable = pDboApp->GetAnimaitonTable();

	m_AnimAssetTreeCtrl.DeleteAllItems();
	TV_INSERTSTRUCT	tv;

	for(int m = ACTION_DEFAULT; m < ACTION_END; m++)
	{
		if(pAnimationTable->CheckData((EAnimAction)m))
		{
			memset (&tv,0,sizeof(tv));
			tv.hParent = NULL;
			tv.hInsertAfter = NULL;
			tv.item.mask = TVIF_STATE | TVIF_TEXT;// | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			tv.item.pszText = (LPSTR)pActionName->GetString(m)->c_str();
			tv.item.cchTextMax = 255;
			HTREEITEM hDirParent = m_AnimAssetTreeCtrl.InsertItem(&tv);

			
			for(int n = COND_DEFAULT; n < COND_END; n++)
			{
				if(pAnimationTable->CheckData((EAnimCondition)n, (EAnimAction)m))
				{
					memset (&tv,0,sizeof(tv));
					tv.hParent = hDirParent;
					tv.hInsertAfter = NULL;
					tv.item.mask = TVIF_STATE | TVIF_TEXT;// | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
					tv.item.pszText = (LPSTR)pConditionName->GetString(n)->c_str();
					tv.item.cchTextMax = 255;
					HTREEITEM hItem = m_AnimAssetTreeCtrl.InsertItem(&tv);
					
					SAnimData *pAnimData = pAnimationTable->GetData( (EAnimCondition)n, (EAnimAction)m );
					SToolAnimData *pToolAnimData = new SToolAnimData;
					pToolAnimData->pAnimData = pAnimData;
					pToolAnimData->uiAnimKey = CREATE_KEY(m, n);

					m_AnimAssetTreeCtrl.SetItemData(hItem, (DWORD_PTR)pToolAnimData);
				}
			}
		}
	}
}
void CAnimationTreeDialog::OnTvnSelchangedTree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	SToolAnimData *pToolAnimData = (SToolAnimData *)m_AnimAssetTreeCtrl.GetItemData(pNMTreeView->itemNew.hItem);
	if(pToolAnimData != NULL)
	{
		theApp.m_pAnimationSetDialog->SetAnimDataInfoChange(pToolAnimData);
	}
	*pResult = 0;
}

void CAnimationTreeDialog::OnDestroy()
{
	CDialog::OnDestroy();
	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	DeleteRecursiveToolAnimData(m_AnimAssetTreeCtrl.GetRootItem());
}

void CAnimationTreeDialog::DeleteToolAnimData(HTREEITEM hItem)
{
	if(hItem == NULL)
		return;

	SToolAnimData *pToolAnimData = (SToolAnimData *)m_AnimAssetTreeCtrl.GetItemData(hItem);
	if(pToolAnimData == NULL)
		return;
	
	delete pToolAnimData;
	
	m_AnimAssetTreeCtrl.SetItemData(hItem, NULL);
}

void CAnimationTreeDialog::DeleteRecursiveToolAnimData(HTREEITEM hItem)
{
	if(hItem == NULL)
		return;

	DeleteToolAnimData(hItem);

	HTREEITEM hChild = m_AnimAssetTreeCtrl.GetChildItem(hItem);

	if(hChild)
		DeleteRecursiveToolAnimData(hChild);

	HTREEITEM hSibling = m_AnimAssetTreeCtrl.GetNextSiblingItem(hItem);
	if(hSibling)
		DeleteRecursiveToolAnimData(hSibling);
}
