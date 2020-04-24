// ObjectPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "DTEditorDoc.h"
#include "ObjectPane.h"
#include "MainFrm.h"

#include "resource.h"

#include "DTEditorGlobal.h"
#include "CreatePropDlg.h"
#include "RenamePropDlg.h"

// simulation
#include "precomp_ntlsimulation.h"
#include "NtlDTProp.h"
#include "NtlSLApi.h"
//#include <map>
//#include <string>


LRESULT CObjectPane::CObjectTree::WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
{
	switch (message)
	{
	case WM_NCPAINT:
		{
			CTreeCtrl::WindowProc(message, wParam, lParam);

			CRect rc;
			GetWindowRect(&rc);
			CWindowDC dc(this);
			rc.OffsetRect(-rc.TopLeft());
			COLORREF clr = GetXtremeColor(XPCOLOR_STATICFRAME);
			dc.Draw3dRect(rc, clr, clr);
			return TRUE;
		}
	}
	return CTreeCtrl::WindowProc(message, wParam, lParam);
}

BEGIN_MESSAGE_MAP(CObjectPane::CObjectTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CObjectTree::OnTvnSelchanged)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CObjectPane::CObjectTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HTREEITEM hItem = GetSelectedItem();
	SObjTreeData *pTreeData = (SObjTreeData*)GetItemData(hItem);
	if(pTreeData)
	{

		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CPropertyPane *pPropertyPane = pMainFrame->GetPropertyPane();
		CControlPane *pControlPane = pMainFrame->GetControlPane();

		if(pTreeData->byType == OBJ_TREE_OBJECT)
		{
			CNtlDTProp *pDTProp = (CNtlDTProp*)pTreeData->pData;
			if(pDTProp)
			{
				SetActiveDTProp(pDTProp);
				pPropertyPane->ActiveSplineProp(pDTProp, pDTProp->GetData());
				pControlPane->ActiveSplineProp(pDTProp, pDTProp->GetData());
			}
		}
		else
		{
			SetActiveDTProp(NULL);

			pPropertyPane->DeActiveSplineProp();
			pControlPane->DeActiveSplineProp();
			SetActiveSelectControlPoint(-1, -1);
		}
	}
	else
	{
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CPropertyPane *pPropertyPane = pMainFrame->GetPropertyPane();
		CControlPane *pControlPane = pMainFrame->GetControlPane();

		SetActiveDTProp(NULL);

		pPropertyPane->DeActiveSplineProp();
		pControlPane->DeActiveSplineProp();
		SetActiveSelectControlPoint(-1, -1);
	}
	
	*pResult = 0;
}

void CObjectPane::CObjectTree::OnDestroy()
{
	CTreeCtrl::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}



/////////////////////////////////////////////////////////////////////////////
// CObjectPan

CObjectPane::CObjectPane()
{
}

CObjectPane::~CObjectPane()
{
}

void CObjectPane::LoadReset(void)
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CPropertyPane *pPropertyPane = pMainFrame->GetPropertyPane();
	CControlPane *pControlPane = pMainFrame->GetControlPane();

	pPropertyPane->DeActiveSplineProp();
	pControlPane->DeActiveSplineProp();
	SetActiveDTProp(NULL);
	SetActiveSelectControlPoint(-1, -1);
}

BOOL CObjectPane::IsTreeItemFolder(HTREEITEM hItem)
{
	if(hItem == NULL)
		return FALSE;

	SObjTreeData *pTreeData = (SObjTreeData*)m_wndTreeCtrl.GetItemData(hItem);
	if(pTreeData->byType == OBJ_TREE_FOLDER)
		return TRUE;

	return FALSE;
}

BOOL CObjectPane::IsTreeItemObject(HTREEITEM hItem)
{
	if(hItem == NULL)
		return FALSE;

	SObjTreeData *pTreeData = (SObjTreeData*)m_wndTreeCtrl.GetItemData(hItem);
	if(pTreeData->byType == OBJ_TREE_OBJECT)
		return TRUE;

	return FALSE;
}

void* CObjectPane::GetTreeItemData(HTREEITEM hItem)
{
	if(hItem == NULL)
		return NULL;

	SObjTreeData *pTreeData = (SObjTreeData*)m_wndTreeCtrl.GetItemData(hItem);
	return pTreeData->pData;
}



BOOL CObjectPane::IsExistChildName(HTREEITEM hItem, CString& strFindName)
{
	HTREEITEM hChildItem = m_wndTreeCtrl.GetChildItem(hItem);
	if(hChildItem == NULL)
	{
		HTREEITEM hSiblingItem = m_wndTreeCtrl.GetNextSiblingItem(hItem);
		if(hSiblingItem == NULL)
			return FALSE;
		else
		{
			CString str = m_wndTreeCtrl.GetItemText(hSiblingItem);
			if(str == strFindName)
				return TRUE;

			return IsExistChildName(hSiblingItem, strFindName);
		}
	}
	else
	{
		CString str = m_wndTreeCtrl.GetItemText(hChildItem);
		if(str == strFindName)
			return TRUE;

		return IsExistChildName(hChildItem, strFindName);
	}

	return FALSE;
}

BOOL CObjectPane::IsExistName(CString& strFindName)
{
	HTREEITEM hRootItem = m_wndTreeCtrl.GetRootItem();
	if(hRootItem == NULL)
		return FALSE;

	CString str = m_wndTreeCtrl.GetItemText(hRootItem);
	if(str == strFindName)
		return TRUE;

	HTREEITEM hSiblingItem = hRootItem;

	while(hSiblingItem)
	{
		if(IsExistChildName(hSiblingItem, strFindName))
			return TRUE;

		hSiblingItem = m_wndTreeCtrl.GetNextSiblingItem(hSiblingItem);
		if(hSiblingItem)
		{
			str = m_wndTreeCtrl.GetItemText(hSiblingItem);
			if(str == strFindName)
				return TRUE;
		}
	}

	return FALSE;
}

void CObjectPane::DeleteTreeItem(HTREEITEM hItem)
{
	if(hItem == NULL)
		return;

	SObjTreeData *pTreeData = (SObjTreeData*)m_wndTreeCtrl.GetItemData(hItem);
	pTreeData->pData = NULL;
	delete pTreeData->pData;

	m_wndTreeCtrl.DeleteItem(hItem);
}

void CObjectPane::DeleteTreeItemData(HTREEITEM hItem)
{
	if(hItem == NULL)
		return;

	SObjTreeData *pTreeData = (SObjTreeData*)m_wndTreeCtrl.GetItemData(hItem);
	if(pTreeData)
	{
		pTreeData->pData = NULL;
		delete pTreeData->pData;
	}

	m_wndTreeCtrl.SetItemData(hItem, 0);
}

void CObjectPane::DeleteChildTreeItemData(HTREEITEM hItem)
{
	HTREEITEM hChildItem = m_wndTreeCtrl.GetChildItem(hItem);
	if(hChildItem == NULL)
	{
		HTREEITEM hSiblingItem = m_wndTreeCtrl.GetNextSiblingItem(hItem);
		if(hSiblingItem == NULL)
			return;
		else
		{
			DeleteTreeItemData(hSiblingItem);
			DeleteChildTreeItemData(hSiblingItem);
		}
	}
	else
	{
		DeleteTreeItemData(hChildItem);
		DeleteChildTreeItemData(hChildItem);
	}
}

void CObjectPane::DeleteAllTreeItemData(void)
{
	HTREEITEM hRootItem = m_wndTreeCtrl.GetRootItem();
	if(hRootItem == NULL)
		return;

	m_wndTreeCtrl.GetItemData(hRootItem);

	DeleteTreeItemData(hRootItem);

	HTREEITEM hSiblingItem = hRootItem;

	while(hSiblingItem)
	{
		DeleteTreeItemData(hSiblingItem);

		hSiblingItem = m_wndTreeCtrl.GetNextSiblingItem(hSiblingItem);

		if(hSiblingItem)
			DeleteTreeItemData(hSiblingItem);
	}
}

HTREEITEM CObjectPane::FindItemMapping(const CHAR *pKey)
{
	MapItemMapping::iterator it = m_mapItemMapping.find(pKey);
	if(it == m_mapItemMapping.end())
		return NULL;

	return (*it).second;
}

CNtlDTProp* CObjectPane::CreateSplineProp(int nShape)
{
	// test 용...
	SSplineProp sProp;
	sProp.eShape				= (EDTSplineShape)nShape;
	sProp.iCtrlNum				= 20;
	sProp.iCurveSubDivision		= 3;
	sProp.fRadius				= 1.0f;
	sProp.fTime					= 6.0f;
	sProp.iRotNum				= 3;
	sProp.fDist					= 2.0f;
	
	return API_GetDTContainter()->CreateSplineProp(sProp);
}

void CObjectPane::LoadItem(CNtlSerializer& s)
{
	BYTE byType;
	INT nParentNameSize;
	CHAR chParentBuffer[128], chBuffer[128];

	while(1)
	{
		s >> byType;

		switch(byType)
		{
		case OBJECT_PANE_SERIALIZE_PARENTRNAME:
			{
				s >> nParentNameSize;
				if(nParentNameSize > 0)
				{
					s.Out(chParentBuffer, nParentNameSize);
					chParentBuffer[nParentNameSize] = 0;
				}
			}
			break;
		case OBJECT_PANE_SERIALIZE_FOLDERNAME:
			{
				INT nSize;
				s >> nSize;
				s.Out(chBuffer, nSize);
				chBuffer[nSize] = 0;

				HTREEITEM hParentItem = NULL;
				if(nParentNameSize > 0)
					hParentItem = FindItemMapping(chParentBuffer);

				HTREEITEM hItem = m_wndTreeCtrl.InsertItem(chBuffer, 0, 0, hParentItem);

				SObjTreeData *pTreeData = new SObjTreeData;
				pTreeData->byType = OBJ_TREE_FOLDER;
				pTreeData->pData = NULL;

				m_wndTreeCtrl.SetItemData(hItem, (DWORD_PTR)pTreeData);

				m_mapItemMapping[chBuffer] = hItem;
			}
			break;
		case OBJECT_PANE_SERIALIZE_OBJECTNAME:
			{
				INT nSize;
				s >> nSize;
				s.Out(chBuffer, nSize);
				chBuffer[nSize] = 0;

				HTREEITEM hParentItem = NULL;
				if(nParentNameSize > 0)
					hParentItem = FindItemMapping(chParentBuffer);

				HTREEITEM hItem = m_wndTreeCtrl.InsertItem(chBuffer, 1, 1, hParentItem);

				CNtlDTProp *pDTProp = API_GetDTContainter()->Find(chBuffer);
				NTL_ASSERT(pDTProp, "CObjectPane::LoadItem");

				SObjTreeData *pTreeData = new SObjTreeData;
				pTreeData->byType = OBJ_TREE_OBJECT;
				pTreeData->pData = pDTProp;

				m_wndTreeCtrl.SetItemData(hItem, (DWORD_PTR)pTreeData);

				m_mapItemMapping[chBuffer] = hItem;
			}
			break;
		case OBJECT_PANE_SERIALIZE_END:
			return;
			break;
		}
	}
}

void CObjectPane::SaveItem(CNtlSerializer& s, HTREEITEM hItem, HTREEITEM hParentItem)
{
	// parent name을 저장한다.

	s << (BYTE)OBJECT_PANE_SERIALIZE_PARENTRNAME;
	if(hParentItem == NULL)
	{
		s << (INT)0;
	}
	else
	{
		CString strParent = m_wndTreeCtrl.GetItemText(hParentItem);
		s << (INT)strParent.GetLength();
		s.In(strParent.GetString(), strParent.GetLength());
	}

	// tree item을 저장한다.
	SObjTreeData *pData = (SObjTreeData*)m_wndTreeCtrl.GetItemData(hItem);
	if(pData->byType ==	OBJ_TREE_FOLDER)
	{
		s << (BYTE)OBJECT_PANE_SERIALIZE_FOLDERNAME;
		CString strFolder = m_wndTreeCtrl.GetItemText(hItem);
		s << (INT)strFolder.GetLength();
		s.In(strFolder.GetString(), strFolder.GetLength());
	}
	else
	{
		s << (BYTE)OBJECT_PANE_SERIALIZE_OBJECTNAME;
		CString strObj = m_wndTreeCtrl.GetItemText(hItem);
		s << (INT)strObj.GetLength();
		s.In(strObj.GetString(), strObj.GetLength());
	}
}

void CObjectPane::SaveChildItem(CNtlSerializer& s, HTREEITEM hItem)
{
	HTREEITEM hNextItem = m_wndTreeCtrl.GetChildItem(hItem);
	if(hNextItem == NULL)
		return;

	while(hNextItem)
	{
		s << (BYTE)OBJECT_PANE_SERIALIZE_TREEITEM;
		SaveItem(s, hNextItem, m_wndTreeCtrl.GetParentItem(hNextItem));
		
		hNextItem = m_wndTreeCtrl.GetNextSiblingItem(hNextItem);
	};


	/*
	HTREEITEM hChildItem = m_wndTreeCtrl.GetChildItem(hItem);
	if(hChildItem == NULL)
	{
		HTREEITEM hSiblingItem = m_wndTreeCtrl.GetNextSiblingItem(hItem);
		if(hSiblingItem == NULL)
			return;
		else
		{
			s << (BYTE)OBJECT_PANE_SERIALIZE_TREEITEM;
			SaveItem(s, hSiblingItem, m_wndTreeCtrl.GetParentItem(hItem));

			SaveChildItem(s, hSiblingItem);
		}
	}
	else
	{
		s << (BYTE)OBJECT_PANE_SERIALIZE_TREEITEM;
		SaveItem(s, hChildItem, hItem);

		SaveChildItem(s, hChildItem);
	}
	*/
}

RwBool CObjectPane::Load(CNtlSerializer& s)
{
	m_mapItemMapping.clear();
	DeleteAllTreeItemData();
	m_wndTreeCtrl.DeleteAllItems();

	BYTE		byType;

	while(1)
	{
		if(s.IsEmpty())
		{
			LoadReset();
			return TRUE;
		}

		s >> byType;
		switch(byType)
		{
		case OBJECT_PANE_SERIALIZE_TREEITEM:
			LoadItem(s);
			break;
		case OBJECT_PANE_SERIALIZE_END:
			LoadReset();
			return TRUE;
			break;
		}
	}

	LoadReset();

	return TRUE;
}

RwBool CObjectPane::Save(CNtlSerializer& s)
{
	HTREEITEM hRootItem = m_wndTreeCtrl.GetRootItem();
	if(hRootItem == NULL)
	{
		s << (BYTE)OBJECT_PANE_SERIALIZE_END;
		return TRUE;
	}

	s << (BYTE)OBJECT_PANE_SERIALIZE_TREEITEM;
	SaveItem(s, hRootItem, NULL);

	HTREEITEM hSiblingItem = hRootItem;

	while(hSiblingItem)
	{
		SaveChildItem(s, hSiblingItem);

		hSiblingItem = m_wndTreeCtrl.GetNextSiblingItem(hSiblingItem);

		if(hSiblingItem)
			SaveItem(s, hSiblingItem, NULL);
	}

	s << (BYTE)OBJECT_PANE_SERIALIZE_END;

	return TRUE;
}


void CObjectPane::ImportItem(CNtlSerializer& s, CNtlDTContainer *pDTContainer)
{
	BYTE byType;
	INT nParentNameSize;
	CHAR chParentBuffer[128], chBuffer[128];

	while(1)
	{
		s >> byType;

		switch(byType)
		{
		case OBJECT_PANE_SERIALIZE_PARENTRNAME:
			{
				s >> nParentNameSize;
				if(nParentNameSize > 0)
				{
					s.Out(chParentBuffer, nParentNameSize);
					chParentBuffer[nParentNameSize] = 0;
				}
			}
			break;
		case OBJECT_PANE_SERIALIZE_FOLDERNAME:
			{
				INT nSize;
				s >> nSize;
				s.Out(chBuffer, nSize);
				chBuffer[nSize] = 0;

				HTREEITEM hParentItem = NULL;
				if(nParentNameSize > 0)
					hParentItem = FindItemMapping(chParentBuffer);

				CString strName = chBuffer;
				if(IsExistName(strName))
					strName += "_New";
								
				HTREEITEM hItem = m_wndTreeCtrl.InsertItem(strName.GetBuffer(), 0, 0, hParentItem);

				SObjTreeData *pTreeData = new SObjTreeData;
				pTreeData->byType = OBJ_TREE_FOLDER;
				pTreeData->pData = NULL;

				m_wndTreeCtrl.SetItemData(hItem, (DWORD_PTR)pTreeData);

				m_mapItemMapping[chBuffer] = hItem;

				return;
			}
			break;
		case OBJECT_PANE_SERIALIZE_OBJECTNAME:
			{
				INT nSize;
				s >> nSize;
				s.Out(chBuffer, nSize);
				chBuffer[nSize] = 0;

				HTREEITEM hParentItem = NULL;
				if(nParentNameSize > 0)
					hParentItem = FindItemMapping(chParentBuffer);

				CNtlDTProp *pDTProp = pDTContainer->Find(chBuffer);
				NTL_ASSERT(pDTProp, "CObjectPane::LoadItem");

				CString strName = chBuffer;
				if(IsExistName(strName))
				{
					while(1)
					{
						strName += "_New";
						if(!IsExistName(strName))
							break;
					}

					pDTContainer->RenameProp(pDTProp, strName.GetBuffer());
				}

				HTREEITEM hItem = m_wndTreeCtrl.InsertItem(strName.GetBuffer(), 1, 1, hParentItem);

				SObjTreeData *pTreeData = new SObjTreeData;
				pTreeData->byType = OBJ_TREE_OBJECT;
				pTreeData->pData = pDTProp;

				m_wndTreeCtrl.SetItemData(hItem, (DWORD_PTR)pTreeData);

				m_mapItemMapping[chBuffer] = hItem;

				return;
			}
			break;
		}
	}
}

RwBool CObjectPane::Import(CNtlSerializer& s, CNtlDTContainer *pDTContainer)
{
	m_mapItemMapping.clear();
	BYTE		byType;

	while(1)
	{
		if(s.IsEmpty())
			return TRUE;

		s >> byType;
		switch(byType)
		{
		case OBJECT_PANE_SERIALIZE_TREEITEM:
			ImportItem(s, pDTContainer);
			break;
		case OBJECT_PANE_SERIALIZE_END:
			return TRUE;
			break;
		}
	}

	return TRUE;
}


BEGIN_MESSAGE_MAP(CObjectPane, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_OBJ_PANE_ADD, OnObjPaneAdd)
	ON_UPDATE_COMMAND_UI(ID_OBJ_PANE_ADD, OnUpdateObjPaneAdd)
	ON_COMMAND(ID_OBJ_PANE_DELETE, OnObjPaneDelete)
	ON_UPDATE_COMMAND_UI(ID_OBJ_PANE_DELETE, OnUpdateObjPaneDelete)
	ON_COMMAND(ID_OBJ_PANE_FOLDER_ADD, OnObjPaneFolderAdd)
	ON_UPDATE_COMMAND_UI(ID_OBJ_PANE_FOLDER_ADD, OnUpdateObjPaneFolderAdd)
	ON_COMMAND(ID_OBJ_PANE_REBUILD, OnObjPaneRebuild)
	ON_UPDATE_COMMAND_UI(ID_OBJ_PANE_REBUILD, OnUpdateObjPaneRebuild)
	ON_COMMAND(ID_OBJ_PANE_RENAME, OnObjPaneRename)
	ON_UPDATE_COMMAND_UI(ID_OBJ_PANE_RENAME, OnUpdateObjPaneRename)
	ON_COMMAND(ID_OBJ_PANE_IMPORT_FILE, OnObjPaneImportFile)
	ON_UPDATE_COMMAND_UI(ID_OBJ_PANE_IMPORT_FILE, OnUpdateObjPaneImportFile)
	

	
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CObjectPane 메시지 처리기입니다.



int CObjectPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	XTPImageManager()->SetMaskColor(RGB(0, 0xFF, 0));
	VERIFY(m_wndToolBar.CreateToolBar(WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_OBJ_PANE_TOOLBAR));

	m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR24, 0, 1);

	CBitmap bmp;
	bmp.LoadBitmap(IDB_OBJECTPANE_TOOLBARS_ICONS);

	m_ilTreeIcons.Add(&bmp, RGB(0, 255, 0));


	if (!m_wndTreeCtrl.Create(WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE|TVS_HASLINES|
		TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS, CRect(0, 0, 0, 0), this, IDC_OBJ_PANE_PROP_TREECTRL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	m_wndTreeCtrl.ModifyStyleEx(0, WS_EX_STATICEDGE);

	m_wndTreeCtrl.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);


//	Refresh();

	return 0;
}

void CObjectPane::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CSize sz(0);
	if (m_wndToolBar.GetSafeHwnd())
	{
		sz = m_wndToolBar.CalcDockingLayout(cx, /*LM_HIDEWRAP|*/ LM_HORZDOCK|LM_HORZ | LM_COMMIT);

		m_wndToolBar.MoveWindow(0, 0, cx, sz.cy);
		m_wndToolBar.Invalidate(FALSE);
	}
	if (m_wndTreeCtrl.GetSafeHwnd())
	{
		m_wndTreeCtrl.MoveWindow(0, sz.cy, cx, cy - sz.cy);
	}
}

void CObjectPane::OnObjPaneAdd()
{
	HTREEITEM hSelectedItem = m_wndTreeCtrl.GetSelectedItem();
	if(hSelectedItem)
	{
		HTREEITEM hParentItem = m_wndTreeCtrl.GetParentItem(hSelectedItem);
		if(!IsTreeItemFolder(hSelectedItem) && hParentItem != NULL)
		{
			hSelectedItem = hParentItem;
			if(!IsTreeItemFolder(hParentItem))
			{
				AfxMessageBox("Select item is not folder... Spline add failed !!!" );
				return;
			}
		}
	}

	CCreatePropDlg dlg;
	dlg.SetCreateType(CREATE_SPLINE);

	if(dlg.DoModal() == IDOK)
	{
		CNtlDTProp *pDTProp = CreateSplineProp(dlg.GetSelectedIndex());
		SetActiveDTProp(pDTProp);

		HTREEITEM hItem = m_wndTreeCtrl.InsertItem(pDTProp->GetName(), 1, 1, hSelectedItem);

		SObjTreeData *pTreeData = new SObjTreeData;
		pTreeData->byType = OBJ_TREE_OBJECT;
		pTreeData->pData = pDTProp;

		m_wndTreeCtrl.SetItemData(hItem,(DWORD_PTR)pTreeData);
		m_wndTreeCtrl.SelectItem(hItem);
	}
}

void CObjectPane::OnUpdateObjPaneAdd(CCmdUI* pCmdUI)
{
}


void CObjectPane::OnObjPaneDelete()
{
	HTREEITEM hSelectedItem = m_wndTreeCtrl.GetSelectedItem();
	if(hSelectedItem == NULL)
	{
		AfxMessageBox("Select item is not exist... Delete  failed !!!" );
		return;
	}

	if(IsTreeItemFolder(hSelectedItem))
	{
		//if(AfxMessageBox("Selete item is folder... Do you want all item into folder delete ?", MB_YESNO) == IDYES)
		
		HTREEITEM hChildItem = m_wndTreeCtrl.GetChildItem(hSelectedItem);
		if(hChildItem != NULL)
		{
			AfxMessageBox("Folder is item not empty... Can not delete !!!");
			return;
		}

		DeleteTreeItem(hSelectedItem);
	}
	else
	{
		if(AfxMessageBox("Do you want spline item delete ?", MB_YESNO ) == IDYES)
		{
			CNtlDTProp *pDTProp = (CNtlDTProp*)GetTreeItemData(hSelectedItem);
			API_GetDTContainter()->DeleteProp(pDTProp);

			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			CPropertyPane *pPropertyPane = pMainFrame->GetPropertyPane();
			CControlPane *pControlPane = pMainFrame->GetControlPane();

			SetActiveDTProp(NULL);

			pPropertyPane->DeleteSplineProp(pDTProp);
			pControlPane->DeleteSplineProp(pDTProp);

			DeleteTreeItem(hSelectedItem);
		}
	}
}


void CObjectPane::OnUpdateObjPaneDelete(CCmdUI* pCmdUI)
{
	HTREEITEM hItem = m_wndTreeCtrl.GetSelectedItem();
	if(hItem == NULL)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}


void CObjectPane::OnObjPaneFolderAdd()
{
	static INT nFolderId = 0;
	nFolderId++;

	CString strFolder;
	strFolder.Format("Folder%d", nFolderId);

	HTREEITEM hItem = m_wndTreeCtrl.InsertItem(strFolder, 0, 0, NULL);

	SObjTreeData *pTreeData = new SObjTreeData;
	pTreeData->byType = OBJ_TREE_FOLDER;
	pTreeData->pData = NULL;

	m_wndTreeCtrl.SetItemData(hItem,(DWORD_PTR)pTreeData);
	m_wndTreeCtrl.SelectItem(hItem);
}

void CObjectPane::OnUpdateObjPaneFolderAdd(CCmdUI* pCmdUI)
{
	
}


void CObjectPane::OnObjPaneRebuild()
{
	HTREEITEM hItem = m_wndTreeCtrl.GetSelectedItem();
	if(hItem == NULL)
	{
		AfxMessageBox("Rebuild proprety is not selected !!!");
		return;
	}

	if(IsTreeItemFolder(hItem))
		return;

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CPropertyPane *pPropertyPane = pMainFrame->GetPropertyPane();
	pPropertyPane->RebuildProp();

	void *pPropData = pPropertyPane->GetActivePropData();

	CNtlDTProp *pDTProp = (CNtlDTProp*)GetTreeItemData(hItem);
	pDTProp->SetData(pPropData);
	pDTProp->Build();

	CControlPane *pControlPane = pMainFrame->GetControlPane();
	pControlPane->RebuildProp();
}

void CObjectPane::OnUpdateObjPaneRebuild(CCmdUI* pCmdUI)
{
	HTREEITEM hItem = m_wndTreeCtrl.GetSelectedItem();
	if(hItem == NULL)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CObjectPane::OnObjPaneRename()
{
	HTREEITEM hSelectedItem = m_wndTreeCtrl.GetSelectedItem();
	if(hSelectedItem == NULL)
		return;

	CString str = m_wndTreeCtrl.GetItemText(hSelectedItem);

	CRenamePropDlg dlg;
	dlg.SetName(str);
	if(dlg.DoModal() == IDOK)
	{
		str = dlg.GetName();
		if(IsExistName(str))
		{
			AfxMessageBox("Name is exist !!!");
			return;
		}

		m_wndTreeCtrl.SetItemText(hSelectedItem, str);
		if(IsTreeItemObject(hSelectedItem))
		{
			CNtlDTProp *pDTProp = (CNtlDTProp*)GetTreeItemData(hSelectedItem);
			API_GetDTContainter()->RenameProp(pDTProp, str.GetString());
		}
	}
}

void CObjectPane::OnUpdateObjPaneRename(CCmdUI* pCmdUI)
{
	HTREEITEM hItem = m_wndTreeCtrl.GetSelectedItem();
	if(hItem == NULL)
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(TRUE);
}

void CObjectPane::OnObjPaneImportFile()
{
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CDTEditorDoc *pDoc = (CDTEditorDoc*)pMainFrame->GetActiveDocument();
	pDoc->OnImportFile();
}

void CObjectPane::OnUpdateObjPaneImportFile(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CObjectPane::OnDestroy()
{
	DeleteAllTreeItemData();

	CWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

