// PaletteSpawn.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteSpawn.h"
#include "NtlWorldPaletteDefine.h"
#include "PaletteSpawnMacro.h"
#include "PaletteSpawnSearch.h"
#include "SpawnManager.h"
#include "PaletteSpawnBase.h"
#include "SpawnContainer.h"
#include "SpawnTreeNode.h"
#include "NtlPLSceneManager.h"
#include "SpawnMacroManager.h"
#include "SpawnMacro.h"
#include "PaletteSpawnPath.h"
#include "PaletteSpawnMacroModify.h"

// CPaletteSpawn 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteSpawn, CDialog)

CPaletteSpawn::CPaletteSpawn(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteSpawn::IDD, pParent)
	, m_MacroIdx(_T(""))
	, m_CoolTime(_T(""))
	, m_MoveType(_T(""))
	, m_WanderRange(_T(""))
	, m_MoveRange(_T(""))
	, m_DelayTime(_T(""))
{
	m_PathDialogOpend		= FALSE;
	m_NavigatorDialogOpend	= FALSE;

	m_pPaletteSpawnBase = new CPaletteSpawnBase;
	m_pPaletteSpawnBase->Create(CPaletteSpawnBase::IDD, GetDesktopWindow());

	m_pPaletteSpawnPath = new CPaletteSpawnPath;
	m_pPaletteSpawnPath->Create(CPaletteSpawnPath::IDD, GetDesktopWindow());

	m_pPaletteSpawnNavigator = new CPaletteSpawnSearch;
	m_pPaletteSpawnNavigator->Create(CPaletteSpawnSearch::IDD, GetDesktopWindow());
}

CPaletteSpawn::~CPaletteSpawn()
{
	m_pPaletteSpawnBase->DestroyWindow();		delete m_pPaletteSpawnBase;			m_pPaletteSpawnBase = NULL;
	m_pPaletteSpawnPath->DestroyWindow();		delete m_pPaletteSpawnPath;			m_pPaletteSpawnPath = NULL;
	m_pPaletteSpawnNavigator->DestroyWindow();	delete m_pPaletteSpawnNavigator;	m_pPaletteSpawnNavigator = NULL;
}

void CPaletteSpawn::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NPC, m_NPC);
	DDX_Control(pDX, IDC_MOB, m_MOB);
	//DDX_Control(pDX, IDC_PARTY, m_PARTY);
	//DDX_Control(pDX, IDC_POINT, m_POINT);
	DDX_Control(pDX, IDC_TREE, m_SpawnTree);
	DDX_Text(pDX, IDC_MI, m_MacroIdx);
	DDX_Text(pDX, IDC_CT, m_CoolTime);
	DDX_Text(pDX, IDC_MT, m_MoveType);
	DDX_Text(pDX, IDC_WR, m_WanderRange);
	DDX_Text(pDX, IDC_MR, m_MoveRange);
	DDX_Text(pDX, IDC_DT, m_DelayTime);
	DDX_Control(pDX, IDC_MACROTYPE, m_CurMacroIdx);
	//DDX_Control(pDX, IDC_PATHLISTVIEW, m_PathListView);
	DDX_Control(pDX, IDC_CHECK_OBJECT_SNAP, m_cbObjectSnap);
	DDX_Control(pDX, IDC_CHECK_CIRCLE, m_BtnShow_Range);
	DDX_Control(pDX, IDC_CHECK_CIRCLE_SCAN, m_BtnShow_Range_Scan);
}

HTREEITEM CPaletteSpawn::GetItemByName(char* szItemName, HTREEITEM hItem)
{
	while (hItem)
	{
		CString sItemText = m_SpawnTree.GetItemText(hItem);

		if(sItemText == szItemName)
			return hItem;

		if (m_SpawnTree.ItemHasChildren(hItem))
		{
			HTREEITEM hResultItem = GetItemByName(szItemName, m_SpawnTree.GetChildItem(hItem));
			if(hResultItem != NULL)
				return hResultItem;
		} 

		hItem = m_SpawnTree.GetNextSiblingItem(hItem);
	}
	return NULL;
}

VOID CPaletteSpawn::CreateTree(eSPAWN_TYPE _eSpawnType)
{
	m_SpawnTree.DeleteAllItems();

	CSpawnContainer*						pSpawnContainer = GetSpawnInstance()->GetSpawnContainer();
	CSpawnContainer::SPAWN_TREE_NODE_ITER	The				= pSpawnContainer->STN_BEGIN();
	RwChar									SortCodeIdx[10];
	HTREEITEM								hTreeItem;

	while(The != pSpawnContainer->STN_END())
	{
		// model index folder
		_itoa_s(The->first, SortCodeIdx, 10);

		TV_INSERTSTRUCT sTvStruct;
		sTvStruct.hParent				= NULL;
		sTvStruct.hInsertAfter			= TVI_LAST;
		sTvStruct.item.mask				= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		sTvStruct.item.iImage			= 0;
		sTvStruct.item.iSelectedImage	= 0;
		sTvStruct.item.pszText			= SortCodeIdx;
		hTreeItem = m_SpawnTree.InsertItem(&sTvStruct);

		CSpawnTreeNode* pSpawnTreeNode = The->second;
		for(RwUInt32 i = 0; i < pSpawnTreeNode->m_vecSpawnTreeNode.size(); ++i)
		{
			sTvStruct.hParent				= hTreeItem;
			sTvStruct.hInsertAfter			= TVI_LAST;
			sTvStruct.item.mask				= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			sTvStruct.item.iImage			= 0;
			sTvStruct.item.iSelectedImage	= 0;
			sTvStruct.item.pszText			= const_cast<LPSTR>(pSpawnTreeNode->m_vecSpawnTreeNode[i].m_Name.c_str());
			m_SpawnTree.InsertItem(&sTvStruct);
		}

		++The;
	}

	UpdateData(FALSE);
}

VOID CPaletteSpawn::InitializeWorldData()
{
	GetSpawnInstance()->Create(eSPAWN_MOB);
	CreateTree(eSPAWN_MOB);
	Refresh();

	m_BtnShow_Range.SetCheck(GetSpawnInstance()->IsVisibleCircle());
	m_BtnShow_Range_Scan.SetCheck(GetSpawnInstance()->IsVisibleCircleScan());

	dGET_SPAWN_MODE() = eSEM_IDLE;
}


BEGIN_MESSAGE_MAP(CPaletteSpawn, CDialog)
	ON_BN_CLICKED(IDC_MACRO_SETUP, &CPaletteSpawn::OnBnClickedMacroSetup)
	ON_BN_CLICKED(IDC_SPAWN_PALETTE_SEARCH, &CPaletteSpawn::OnBnClickedSpawnPaletteSearch)
	ON_BN_CLICKED(IDC_NPC, &CPaletteSpawn::OnBnClickedNpc)
	ON_BN_CLICKED(IDC_MOB, &CPaletteSpawn::OnBnClickedMob)
	//ON_BN_CLICKED(IDC_POINT, &CPaletteSpawn::OnBnClickedPoint)
	//ON_BN_CLICKED(IDC_PARTY, &CPaletteSpawn::OnBnClickedParty)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CPaletteSpawn::OnTvnSelchangedTree)
	ON_CBN_SELCHANGE(IDC_MACROTYPE, &CPaletteSpawn::OnCbnSelchangeMacrotype)
	ON_BN_CLICKED(IDC_SPAWN_SAVE, &CPaletteSpawn::OnBnClickedSpawnSave)
	ON_BN_CLICKED(IDC_SPAWN_LOAD, &CPaletteSpawn::OnBnClickedSpawnLoad)
	ON_BN_CLICKED(IDC_SPAWN_EXPORT, &CPaletteSpawn::OnBnClickedSpawnExport)
	//ON_BN_CLICKED(IDC_PATHLISTVIEW, &CPaletteSpawn::OnBnClickedPathlistview)
	ON_BN_CLICKED(IDC_PATH_OPEN, &CPaletteSpawn::OnBnClickedPathOpen)
	ON_BN_CLICKED(IDC_SPAWN_MACRO_MODIFY, &CPaletteSpawn::OnBnClickedSpawnMacroModify)
	ON_BN_CLICKED(IDC_CHECK_OBJECT_SNAP, &CPaletteSpawn::OnBnClickedCheckObjectSnap)
	ON_BN_CLICKED(IDC_CHECK_CIRCLE, &CPaletteSpawn::OnBnClickedCheckCircle)
	ON_BN_CLICKED(IDC_CHECK_CIRCLE_SCAN, &CPaletteSpawn::OnBnClickedCheckCircleScan)
END_MESSAGE_MAP()


// CPaletteSpawn 메시지 처리기입니다.

BOOL CPaletteSpawn::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_MOB.SetCheck(TRUE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

VOID CPaletteSpawn::Refresh()
{
	CSpawnMacro SpawnMacro = GetSpawnInstance()->GetCurSpawnMacro();

	m_MacroIdx.Format("%d", GetSpawnInstance()->GetCurMacroIndicatorIdx());
	m_CoolTime.Format("%d", SpawnMacro.m_CoolTime);
	m_MoveType.Format("%d", SpawnMacro.m_MoveType);
	m_WanderRange.Format("%.2f", SpawnMacro.m_WanderRange);
	m_MoveRange.Format("%.2f", SpawnMacro.m_MoveRange);
	m_DelayTime.Format("%.2f", SpawnMacro.m_DelayTime);

	m_CurMacroIdx.ResetContent();

	CSpawnMacroManager::SPAWN_MACRO_INDICATOR_ITER The = GetSpawnInstance()->SMI_BEGIN();
	while(The != GetSpawnInstance()->SMI_END())
	{
		RwInt32 Idx		= (*The).m_MacroIdx;
		CString IdxStr;

		IdxStr.Format("%d", Idx);
		m_CurMacroIdx.AddString(IdxStr);

		++The;
	}

	m_CurMacroIdx.SetCurSel(GetSpawnInstance()->GetCurMacroIdx());
/*

	RwInt32 CurCnt = GetSpawnInstance()->SMI_SIZE() - 1;
	while(CurCnt >= 0)
	{
		RwInt32 Idx	= GetSpawnInstance()->SMI_ELEMENT(CurCnt).m_MacroIdx;
		CString IdxStr;

		IdxStr.Format("%d", Idx);
		m_CurMacroIdx.AddString(IdxStr);

		--CurCnt;
	}
	m_CurMacroIdx.SetCurSel(GetSpawnInstance()->GetCurMacroIdx());*/

	UpdateData(FALSE);
}

BOOL CPaletteSpawn::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		/*
		if(pMsg->hwnd == GetDlgItem(IDC_R_TAP_VALUE_EFFECT)->m_hWnd)
		{
			UpdateData(TRUE);
		}
		*/
	}

	// windows messages
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_ESCAPE:
			{
				return true;
			}
		case VK_RETURN:
			{
				return true;
			}
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CPaletteSpawn::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SHOWWINDOW:
		{
			if(wParam == 0)
			{
				m_pPaletteSpawnBase->ShowWindow(SW_HIDE);
			}
			else if(wParam == 1)
			{
				dGETFRM()->m_pSpawnPalette->m_PathDialogOpend = FALSE;
				GetSpawnInstance()->SetPathModeActive(FALSE);

				dGETFRM()->m_pSpawnPalette->m_NavigatorDialogOpend = FALSE;
				GetSpawnInstance()->SetNavigatorModeActive(FALSE);

				m_pPaletteSpawnBase->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				m_pPaletteSpawnBase->ShowWindow(SW_SHOW);
			}

			break;
		}
	case WM_CLOSE:
		{
			dGET_CUR_PALETTE() = ePM_DEFAULT;
			dGETHDL()->DeleteAllPaletteDummies();
			
			m_pPaletteSpawnBase->ShowWindow(SW_HIDE);
			m_pPaletteSpawnPath->ShowWindow(SW_HIDE);
			m_pPaletteSpawnNavigator->ShowWindow(SW_HIDE);
			m_PathDialogOpend		= FALSE;
			m_NavigatorDialogOpend	= FALSE;
			
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CPaletteSpawn::OnBnClickedMacroSetup()
{
	static RwBool OneAtATimeFlag = FALSE;

	if(!OneAtATimeFlag)
	{
		OneAtATimeFlag = TRUE;

		CPaletteSpawnMacro PaletteSpawnMacro;
		if(PaletteSpawnMacro.DoModal() == IDOK)
		{
			Refresh();
		}

		OneAtATimeFlag = FALSE;
	}
}

void CPaletteSpawn::OnBnClickedNpc()
{
	if(GetSpawnInstance()->GetSpawnType() == eSPAWN_NPC)
	{
		return;
	}

	if(::AfxMessageBox("Would you like to save MOB Spawn?", MB_YESNO) == IDYES)
	{
		// save
		GetSpawnInstance()->Save();
	}

	GetSpawnInstance()->Create(eSPAWN_NPC);
	CreateTree(eSPAWN_NPC);
}

void CPaletteSpawn::OnBnClickedMob()
{
	if(GetSpawnInstance()->GetSpawnType() == eSPAWN_MOB)
	{
		return;
	}

	if(::AfxMessageBox("Would you like to save NPC Spawn?", MB_YESNO) == IDYES)
	{
		// save
		GetSpawnInstance()->Save();
	}

	GetSpawnInstance()->Create(eSPAWN_MOB);
	CreateTree(eSPAWN_MOB);
}
/*
void CPaletteSpawn::OnBnClickedPoint()
{
	if(!((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.IsWorking())
	{
		return;
	}

	if(m_POINT.GetCheck())
	{
		GetSpawnInstance()->GetSpawnMode() = ePOINT_SPAWN;
		m_pPaletteSpawnPoint->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		m_pPaletteSpawnPoint->ShowWindow(SW_SHOW);
		m_pPaletteSpawnParty->ShowWindow(SW_HIDE);
	}
	else
	{
		GetSpawnInstance()->GetSpawnMode() = ePARTY_SPAWN;
		m_pPaletteSpawnPoint->ShowWindow(SW_HIDE);
		m_pPaletteSpawnParty->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		m_pPaletteSpawnParty->ShowWindow(SW_SHOW);
	}
}

void CPaletteSpawn::OnBnClickedParty()
{
	if(!((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.IsWorking())
	{
		return;
	}

	if(m_PARTY.GetCheck())
	{
		GetSpawnInstance()->GetSpawnMode() = ePARTY_SPAWN;
		m_pPaletteSpawnPoint->ShowWindow(SW_HIDE);
		m_pPaletteSpawnParty->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		m_pPaletteSpawnParty->ShowWindow(SW_SHOW);
	}
	else
	{
		GetSpawnInstance()->GetSpawnMode() = ePOINT_SPAWN;
		m_pPaletteSpawnPoint->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		m_pPaletteSpawnPoint->ShowWindow(SW_SHOW);
		m_pPaletteSpawnParty->ShowWindow(SW_HIDE);
	}
}
*/
void CPaletteSpawn::OnTvnSelchangedTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW	pNMTreeView	= reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM		hTreeItem	= pNMTreeView->itemNew.hItem;
	CString			StrItmName	= m_SpawnTree.GetItemText(hTreeItem);
	CString			StrObjName	= m_SpawnTree.GetItemText(hTreeItem);
	CString			StrLvlName	= StrObjName;
	CString			StrGUID		= StrObjName;
	string			StrTreeName	= StrObjName.GetBuffer(0);
	string			MdlName;
	BYTE			Lvl;
	RwUInt32		MdlGUID;

	// trim
	StrObjName.Delete(StrObjName.Find('('), StrObjName.GetLength());
	StrLvlName.Delete(0, StrLvlName.Find('(') + 1);
	StrLvlName.Delete(StrLvlName.Find("레"), StrLvlName.GetLength());

	StrGUID.Delete(0, StrGUID.Find(',') + 2);
	StrGUID.Delete(StrGUID.Find("'"), StrGUID.GetLength());
	Lvl		= static_cast<BYTE>(atoi(StrLvlName.GetBuffer(0)));
	MdlGUID	= static_cast<RwUInt32>(atoi(StrGUID.GetBuffer(0)));

	RwV3d Pos = {-999.0f, -999.0f, -999.0f};
	SPLCharacterCreateParam PLEntityCreateParam;
	PLEntityCreateParam.pPos = &Pos;
	PLEntityCreateParam.bPcFlag = FALSE;

	if(m_SpawnTree.ItemHasChildren(hTreeItem))
		return;

	GetSpawnInstance()->GetMdlNameOfTreeItem(StrItmName.GetBuffer(0), Lvl, MdlName, MdlGUID);

	// update display brush entity
	if(dGET_BRUSH_ENTITY())
	{
		GetSceneManager()->DeleteEntity(dGET_BRUSH_ENTITY());
		dGET_BRUSH_ENTITY() = NULL;
	}

	dGET_BRUSH_ENTITY() = static_cast<CNtlPLEntity*>(GetSceneManager()->CreateEntity(PLENTITY_CHARACTER, MdlName.c_str(), &PLEntityCreateParam));
	DBO_ASSERT(dGET_BRUSH_ENTITY(), "Entity create failed.");

	string Mdl_ObjName = StrTreeName;
	string Mdl_MdlName = MdlName;
	GetSpawnInstance()->SetCurMdlInfo(Mdl_ObjName, Lvl, Mdl_MdlName);

	RwV3d vDefP	= {0.0f, 0.0f, 0.0f};
	RwV3d vDefS	= {1.0f, 1.0f, 1.0f};

	RwV3d vDist;
	vDist.x = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.x;
	vDist.y = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.y;
	vDist.z = dGET_COLLISION_INFO()->RayIntersectionPt4Terrain.z;

	dGET_BRUSH_ENTITY()->SetScale(1.0f);
	dGET_BRUSH_ENTITY()->SetRotate(0.0f, 0.0f, 0.0f);
	dGET_BRUSH_ENTITY()->SetPosition(&vDist);

	dGET_CUR_PALETTE()	= ePM_SPAWN;
	dGET_SPAWN_MODE()	= eSEM_PLACE;

	// set current macro info.
	CSpawnTreeElement*	pSpawnTreeElement	= GetSpawnInstance()->GetTreeElement(Lvl, Mdl_MdlName, Mdl_ObjName);
	RwInt32				MatchedMacroIdx		= GetSpawnInstance()->GetMacroIdx(pSpawnTreeElement->m_LastMacroIdx);

	if(MatchedMacroIdx != -999)
	{
		GetSpawnInstance()->GetCurMacroIdx() = MatchedMacroIdx;
	}
	else
	{
		AfxMessageBox("The last macro index was deleted.");
		pSpawnTreeElement->m_LastMacroIdx = 0;
		GetSpawnInstance()->GetCurMacroIdx() = 0;
	}

	Refresh();

	*pResult = 0;
}

void CPaletteSpawn::OnCbnSelchangeMacrotype()
{
	GetSpawnInstance()->GetCurMacroIdx() = m_CurMacroIdx.GetCurSel();
	Refresh();
}

void CPaletteSpawn::OnBnClickedSpawnSave()
{
	GetSpawnInstance()->Save();
}

void CPaletteSpawn::OnBnClickedSpawnLoad()
{
	GetSpawnInstance()->Load();
}

void CPaletteSpawn::OnBnClickedSpawnExport()
{
	GetSpawnInstance()->Export();
}

// void CPaletteSpawn::OnBnClickedPathlistview()
// {
// 	if(m_PathListView.GetCheck())
// 	{
// 
// 	}
// 	else
// 	{
// 
// 	}
// }

void CPaletteSpawn::OnBnClickedSpawnPaletteSearch()
{
	if(!m_NavigatorDialogOpend)
	{
		GetSpawnInstance()->PopAllSpawnCaptured();
		GetSpawnInstance()->SetNavigatorModeActive();
		m_pPaletteSpawnNavigator->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		m_pPaletteSpawnNavigator->ShowWindow(SW_SHOW);
		m_NavigatorDialogOpend = TRUE;

		this->ShowWindow(SW_HIDE);
	}
}

void CPaletteSpawn::OnBnClickedPathOpen()
{
	if(!m_PathDialogOpend)
	{
		GetSpawnInstance()->PopAllSpawnCaptured();
		GetSpawnInstance()->SetPathModeActive();
		m_pPaletteSpawnPath->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		m_pPaletteSpawnPath->ShowWindow(SW_SHOW);
		m_PathDialogOpend	= TRUE;

		this->ShowWindow(SW_HIDE);
	}
}

void CPaletteSpawn::OnBnClickedSpawnMacroModify()
{
	CPaletteSpawnMacroModify PSMMDlg;
	PSMMDlg.DoModal();
}

void CPaletteSpawn::OnBnClickedCheckObjectSnap()
{
	GetSpawnInstance()->SetObjectSnap(m_cbObjectSnap.GetCheck());
}

void CPaletteSpawn::OnBnClickedCheckCircle()
{
	GetSpawnInstance()->SetVisibleCircle(m_BtnShow_Range.GetCheck());
	
}

void CPaletteSpawn::OnBnClickedCheckCircleScan()
{
	GetSpawnInstance()->SetVisibleCircleScan(m_BtnShow_Range_Scan.GetCheck());
}
