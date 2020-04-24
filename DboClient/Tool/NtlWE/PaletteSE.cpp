// SEPropDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteSE.h"
#include "NtlWorldPaletteDefine.h"
#include "NtlPLObject.h"


// CPaletteSE 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteSE, CDialog)

CPaletteSE::CPaletteSE(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteSE::IDD, pParent)
{
}

CPaletteSE::~CPaletteSE()
{
}

void CPaletteSE::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SE_TREE, m_SETree);
}


BEGIN_MESSAGE_MAP(CPaletteSE, CDialog)
	ON_BN_CLICKED(IDOK, &CPaletteSE::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPaletteSE::OnBnClickedCancel)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SE_TREE, &CPaletteSE::OnTvnSelchangedSeTree)
END_MESSAGE_MAP()


// CPaletteSE 메시지 처리기입니다.

void CPaletteSE::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}

void CPaletteSE::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

BOOL CPaletteSE::OnInitDialog()
{
	CDialog::OnInitDialog();

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

HTREEITEM CPaletteSE::GetItemByName(char* szItemName, HTREEITEM hItem)
{
	while (hItem)
	{
		CString sItemText = m_SETree.GetItemText(hItem);

		if(sItemText == szItemName)
			return hItem;

		if (m_SETree.ItemHasChildren(hItem))
		{
			HTREEITEM hResultItem = GetItemByName(szItemName, m_SETree.GetChildItem(hItem));
			if(hResultItem != NULL)
				return hResultItem;
		} 

		hItem = m_SETree.GetNextSiblingItem(hItem);
	}
	return NULL;
}

void CPaletteSE::CreateLeafNode(const char *pName, HTREEITEM hTreeItem)
{
	TVINSERTSTRUCT TI;
	TI.hParent				= hTreeItem;
	TI.hInsertAfter			= TVI_LAST;
	TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TI.item.iImage			= 0;
	TI.item.iSelectedImage	= 1;
	TI.item.pszText			= const_cast<LPSTR>(pName);
	m_SETree.InsertItem(&TI);
}

void CPaletteSE::InitializeWorldData()
{
	m_SETree.DeleteAllItems();

	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CString Result	= dGET_WORLD_PARAM()->CurWorkingFolderName;
	CString Temp0	= "/script/environment.tbl";
	Result			= Result + Temp0;

	char Return[NTL_MAX_DIR_PATH] = {0,};

	::GetPrivateProfileString("category info", "num", "error", Return, NTL_MAX_DIR_PATH, Result);
	RwInt32 NumNode = atoi(Return);
	if(!NumNode)
	{
		DBO_TRACE(FALSE, "CPaletteSE::InitializeWorldData, Numnode is zero");
		return;
	}
	else
	{
		// create root tree item
		TVINSERTSTRUCT TI;
		TI.hParent				= TVI_ROOT;
		TI.hInsertAfter			= TVI_LAST;
		TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		TI.item.iImage			= 0;
		TI.item.iSelectedImage	= 1;
		TI.item.pszText			= "SE objects";
		m_SETree.SetItemData(m_SETree.InsertItem(&TI), 0);

		// create sub tree & element items
		CString strCategoryName;
		for(RwInt32 i = 0; i < NumNode; ++i)
		{	
			// create sub tree
			strCategoryName.Format("category%d", i);
			::GetPrivateProfileString(strCategoryName.GetBuffer(0), "name", "error", Return, NTL_MAX_DIR_PATH, Result);

			TI.hParent				= m_SETree.GetRootItem();
			TI.hInsertAfter			= TVI_LAST;
			TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			TI.item.iImage			= 0;
			TI.item.iSelectedImage	= 1;
			TI.item.pszText			= Return;
			HTREEITEM Category		= m_SETree.InsertItem(&TI);

			m_SETree.SetItemData(Category, 0);
			
			// create element 
			RwInt32 NumElement;
			::GetPrivateProfileString(strCategoryName.GetBuffer(0), "num", "error", Return, NTL_MAX_DIR_PATH, Result);
			NumElement = atoi(Return);

			for(RwInt32 j = 0; j < NumElement; ++j)
			{
				CString StrIdx;
				StrIdx.Format("%d", j);
				::GetPrivateProfileString(strCategoryName.GetBuffer(0), StrIdx.GetBuffer(0), "error", Return, NTL_MAX_DIR_PATH, Result);

				TI.hParent				= Category;
				TI.hInsertAfter			= TVI_LAST;
				TI.item.mask			= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
				TI.item.iImage			= 0;
				TI.item.iSelectedImage	= 1;
				TI.item.pszText			= Return;
				m_SETree.SetItemData(m_SETree.InsertItem(&TI), 1);
			}
		}
	}

	::SetCurrentDirectory(dirBackup);

	UpdateData(FALSE);
}

BOOL CPaletteSE::PreTranslateMessage(MSG* pMsg)
{
	// prevent keys; escape, return
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			{
				return true;
				break;
			}
		case VK_RETURN:
			{
				return true;
				break;
			}
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CPaletteSE::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			dGET_CUR_PALETTE() = ePM_DEFAULT;
			dGETHDL()->DeleteAllPaletteDummies();
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CPaletteSE::OnTvnSelchangedSeTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView	= reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hTreeItem			= pNMTreeView->itemNew.hItem;
	CString StrObjName			= m_SETree.GetItemText(hTreeItem);

	RwV3d Pos = {-999.0f, -999.0f, -999.0f};
	SPLEntityCreateParam PLEntityCreateParam;
	PLEntityCreateParam.pPos = &Pos;

	if(m_SETree.GetRootItem() == hTreeItem)
		return;

	if(m_SETree.ItemHasChildren(hTreeItem))
		return;

	// update display brush entity
	if(dGET_BRUSH_ENTITY())
	{
		GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
		dGET_BRUSH_ENTITY() = NULL;
	}

	dGET_BRUSH_ENTITY() = static_cast<CNtlPLEntity*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, "TEXT_SOUND", &PLEntityCreateParam));
	DBO_ASSERT(dGET_BRUSH_ENTITY(), "Entity create failed.");

	(static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY()))->SetFadeEnable(FALSE);

	RwV3d vDefP	= {0.0f, 0.0f, 0.0f};
	RwV3d vDefS	= {1.0f, 1.0f, 1.0f};

	RwV3d vDist;
	vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
	vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
	vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;

	static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetScale(&vDefS);
	static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetRotate(0.0f, 0.0f, 0.0f);
	static_cast<CNtlPLObject*>(dGET_BRUSH_ENTITY())->SetPosition(&vDist);

	dGET_CUR_PALETTE() = ePM_SE;

	*pResult = 0;
}

RwBool CPaletteSE::GetCurBGMFileName(RwChar _FileName[32])
{
	HTREEITEM hTreeItem = m_SETree.GetSelectedItem();

	if(!hTreeItem || !m_SETree.GetItemData(hTreeItem))
	{
		return FALSE;
	}
	else
	{
		strcpy_s(_FileName, 32, m_SETree.GetItemText(hTreeItem).GetBuffer(0));
		return TRUE;
	}
}

RwBool CPaletteSE::GetCurCategoryName(RwChar _CategoryName[32])
{
	HTREEITEM hTreeItem = m_SETree.GetSelectedItem();

	if(!hTreeItem || !m_SETree.GetItemData(hTreeItem))
	{
		return FALSE;
	}
	else
	{
		hTreeItem = m_SETree.GetParentItem(hTreeItem);
		strcpy_s(_CategoryName, 32, m_SETree.GetItemText(hTreeItem).GetBuffer(0));
		return TRUE;
	}

	
}
