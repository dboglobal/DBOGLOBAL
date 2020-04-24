// DirectionPane.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "DirectionPane.h"

// simulation
#include "NtlSLCENode.h"
#include "NtlSLTBCrowdController.h"
#include "NtlSLTBCrowdManager.h"
#include "NtlSoundMoodManager.h"

// tool
#include "DTEditor.h"
#include "RenamePropDlg.h"
#include "CreatePropDlg.h"
#include "MainFrm.h"

#define MAX_DIR_PATH	2048


// CDirectionPane 메시지 처리기입니다.
LRESULT CDirectionPane::CDirectionTree::WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
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


// CDirectionPane

IMPLEMENT_DYNAMIC(CDirectionPane, CWnd)

CDirectionPane::CDirectionPane()
:m_hDirectingItem(NULL)
{

}

CDirectionPane::~CDirectionPane()
{
}


BEGIN_MESSAGE_MAP(CDirectionPane, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_BUTTON_DIRECTION_SAVE, OnSaveDirection)
	ON_COMMAND(ID_BUTTON_DIRECTION_LOAD, OnLoadDirection)
	ON_COMMAND(ID_BUTTON_DIRECTION_ADD_SET, OnAddController)
	ON_COMMAND(ID_BUTTON_DIRECTION_ADD_NODE, OnAddNode)
	ON_COMMAND(ID_BUTTON_DIRECTION_REMOVE, OnRemove)
	ON_COMMAND(ID_BUTTON_DIRECTION_RUN, OnRun)
	ON_COMMAND(ID_BUTTON_DIRECTION_STOP, OnStop)	
END_MESSAGE_MAP()



int CDirectionPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	XTPImageManager()->SetMaskColor(RGB(0, 0xFF, 0));
	VERIFY(m_wndToolBar.CreateToolBar(WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_DIRECTIONBAR));

	m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR24, 0, 1);


	CBitmap bmp;
	bmp.LoadBitmap(IDB_OBJECTPANE_TOOLBARS_ICONS);

	m_ilTreeIcons.Add(&bmp, RGB(0, 255, 0));


	if (!m_wndTreeCtrl.Create(WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE|TVS_HASLINES|
		TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS, CRect(0, 0, 0, 0), this, IDC_DIRECTION_PANE_PROP_TREECTRL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	m_wndTreeCtrl.ModifyStyleEx(0, WS_EX_STATICEDGE);
	m_wndTreeCtrl.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);


	// 툴모드 지정
	CNtlSLTBCrowdManager::ms_bDirectionToolMode = true;

	return 0;
}

void CDirectionPane::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	DestroyTreeItem();
}

void CDirectionPane::OnSize(UINT nType, int cx, int cy)
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

void CDirectionPane::OnSaveDirection()
{
	TCHAR chCurDir[MAX_DIR_PATH];
	GetCurrentDirectory(MAX_DIR_PATH, chCurDir);

	CFileDialog	SaveDlg( FALSE, _T( "dtf" ), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
								_T( "Direction Files (*.dtf)|*.dtf|All Files (*.*)|*.*||" ), NULL );

	if( SaveDlg.DoModal() == IDOK )
	{
		CString strWorkFull			= SaveDlg.GetPathName();
		CString strWorkFileName		= SaveDlg.GetFileName();
		CString strWorkFileTitle	= SaveDlg.GetFileTitle();
		CString strWorkPath			= strWorkFull;

		strWorkPath.Delete(strWorkPath.GetLength() - strWorkFileName.GetLength(), strWorkFileName.GetLength());

		CString strPropFullPath = strWorkPath + strWorkFileTitle + "." + dDIRECTION_FILE_EXT;


		// Save
		CNtlFileSerializer nsl(dDIRECTION_FILE_BUFFER, dDIRECTION_FILE_GLOW);

		int iContollerCount = 0;

		HTREEITEM hRootItem = m_wndTreeCtrl.GetRootItem();
		if( hRootItem )
		{
			++iContollerCount;

			sDirectionItem* pDirectionItem = (sDirectionItem*)m_wndTreeCtrl.GetItemData(hRootItem);
			if( pDirectionItem )
			{
				HTREEITEM hSiblingItem = m_wndTreeCtrl.GetNextSiblingItem(hRootItem);
				while( hSiblingItem )
				{
					++iContollerCount;
					hSiblingItem = m_wndTreeCtrl.GetNextSiblingItem(hSiblingItem);
				}
			}
		}

		nsl<<CNtlSLCENode::m_Version.fLastestVersion;
		nsl<<iContollerCount;


		if( hRootItem )
		{
			sDirectionItem* pDirectionItem = (sDirectionItem*)m_wndTreeCtrl.GetItemData(hRootItem);
			if( pDirectionItem )
			{
				CNtlSLTBCrowdController* pController = (CNtlSLTBCrowdController*)pDirectionItem->pData;
				pController->Save(nsl);

				HTREEITEM hSiblingItem = m_wndTreeCtrl.GetNextSiblingItem(hRootItem);
				while( hSiblingItem )
				{
					sDirectionItem* pDirectionItem = (sDirectionItem*)m_wndTreeCtrl.GetItemData(hSiblingItem);
					CNtlSLTBCrowdController* pController = (CNtlSLTBCrowdController*)pDirectionItem->pData;
					pController->Save(nsl);

					hSiblingItem = m_wndTreeCtrl.GetNextSiblingItem(hSiblingItem);
				}
			}
		}

		nsl.SaveFile((LPSTR)(LPCSTR)strPropFullPath);
	}

	SetCurrentDirectory(chCurDir);	
}

void CDirectionPane::OnLoadDirection()
{
	TCHAR chCurDir[MAX_DIR_PATH];
	GetCurrentDirectory(MAX_DIR_PATH, chCurDir);

	CFileDialog	LoadDlg( TRUE, _T( "dtf" ), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
							   _T( "Direct Project Files (*.dtf)|*.dtf|All Files (*.*)|*.*||" ), NULL );

	if( LoadDlg.DoModal() == IDOK )
	{
		CString strFull			= LoadDlg.GetPathName();
		CString strFileName		= LoadDlg.GetFileName();
		CString strFileTitle	= LoadDlg.GetFileTitle();
		CString strPath			= strFull;

		strPath.Delete(strPath.GetLength() - strFileName.GetLength(), strFileName.GetLength());

		CString strPropFullPath = strPath + strFileTitle + "." + dDIRECTION_FILE_EXT;

		// Load
		CNtlFileSerializer nsl(dDIRECTION_FILE_BUFFER, dDIRECTION_FILE_GLOW);
		if( nsl.LoadFile((LPSTR)(LPCSTR)strPropFullPath) )
		{
			// 기존의 트리 아이템을 지운다
			DestroyTreeItem();

			int iControllerCount;

			nsl>>CNtlSLCENode::m_Version.fCurrentWorkVerstion;
			nsl>>iControllerCount;

			for( int i = 0 ; i < iControllerCount ; ++i )
			{
				// 컨트롤러
				CNtlSLTBCrowdController* pController = NTL_NEW CNtlSLTBCrowdController();
				pController->Load(nsl);

				// Direction Item
				sDirectionItem* pNewDirectionItem		= new sDirectionItem;
				pNewDirectionItem->byItemType			= LIST_TYPE_CONTROLLER;
				pNewDirectionItem->byNodeType			= CNtlSLCENode::INVALID_E_CE;
				pNewDirectionItem->pData				= pController;

				// Tree list data
				HTREEITEM hTreeControllerItem = m_wndTreeCtrl.InsertItem((LPCSTR)(LPSTR)pController->GetName().c_str(), 1, 1, NULL);
				m_wndTreeCtrl.SetItemData(hTreeControllerItem, (DWORD_PTR)pNewDirectionItem);
				m_listDirectionItem.push_back(pNewDirectionItem);

				// crowd manager
				GetNtlSLCrowdManager()->AddController(pController);

				// 각 노드 추가
				LIST_CENODE& m_listNode = pController->GetNodeList();
				for each( CNtlSLCENode* pNode in m_listNode )
				{
					// Direction Item
					sDirectionItem* pNewDirectionNodeItem = NTL_NEW sDirectionItem;
					pNewDirectionNodeItem->byItemType			= LIST_TYPE_NODE;
					pNewDirectionNodeItem->byNodeType			= pNode->GetType();
					pNewDirectionNodeItem->pData				= pNode;

					// Tree list data
					HTREEITEM hTreeNodeItem = m_wndTreeCtrl.InsertItem((LPCSTR)(LPSTR)pNode->GetName().c_str(), 1, 1, hTreeControllerItem);
					m_wndTreeCtrl.SetItemData(hTreeNodeItem, (DWORD_PTR)pNewDirectionNodeItem);
					m_listDirectionItem.push_back(pNewDirectionNodeItem);
				}
			}
		}		
	}

	SetCurrentDirectory(chCurDir);
}

void CDirectionPane::OnAddController()
{
	CRenamePropDlg renameDlg;

	renameDlg.SetName( _T("NewController") );

	if( renameDlg.DoModal() != IDOK )
		return;

	CString strName = renameDlg.GetName();

	if( IsExistSameName_inSibling(NULL, (LPSTR)(LPCSTR)strName) )
	{
		AfxMessageBox("Same name exist in controller");
		return;
	}

	// 컨트롤러
	CNtlSLTBCrowdController* pNewDirectionController = NTL_NEW CNtlSLTBCrowdController;
	pNewDirectionController->Create();
	pNewDirectionController->SetName((LPSTR)(LPCSTR)strName);
	
	// Direction Item
	sDirectionItem* pNewDirectionItem = new sDirectionItem;
	pNewDirectionItem->byItemType			= LIST_TYPE_CONTROLLER;
	pNewDirectionItem->byNodeType			= CNtlSLCENode::INVALID_E_CE;
	pNewDirectionItem->pData				= pNewDirectionController;

	// Tree list data
	HTREEITEM hTreeItem = m_wndTreeCtrl.InsertItem(strName, 1, 1, NULL);
	m_wndTreeCtrl.SetItemData(hTreeItem, (DWORD_PTR)pNewDirectionItem);
	m_listDirectionItem.push_back(pNewDirectionItem);

	// Simulation crowd manager
	GetNtlSLCrowdManager()->AddController(pNewDirectionController);
}

void CDirectionPane::OnAddNode()
{
	HTREEITEM hSelectedItem = m_wndTreeCtrl.GetSelectedItem();

	if( !hSelectedItem )
	{
		AfxMessageBox("You must select the set or node");
		return;
	}


	HTREEITEM hParent_of_NewItem;	
	sDirectionItem* pDirectionItem = reinterpret_cast<sDirectionItem*>( m_wndTreeCtrl.GetItemData(hSelectedItem) );

	// 노드의 부모를 찾는다
	if( pDirectionItem->byItemType != LIST_TYPE_CONTROLLER )
	{
		hParent_of_NewItem = m_wndTreeCtrl.GetParentItem(hSelectedItem);
		if( !hParent_of_NewItem )
		{
			AfxMessageBox("error : The node have not parent");
			return;
		}
	}
	else
		hParent_of_NewItem = hSelectedItem;
	

	static int iEffectCount = 0;
	static int iSoundCount = 0;
	static int iBallonCount = 0;
	static int iRefereeCount = 0;	
	char acBuffer[128] = "";
	CCreatePropDlg dlg;

	dlg.SetCreateType(CREATE_DIRECTION_NODE);

	if( dlg.DoModal() != IDOK )
		return;

	CString strNodeName;
	int iCount;
	sDirectionItem* pParentDirectionItem = (sDirectionItem*)m_wndTreeCtrl.GetItemData(hParent_of_NewItem);
	CNtlSLTBCrowdController* pController = (CNtlSLTBCrowdController*)pParentDirectionItem->pData;

	switch( dlg.GetSelectedIndex() )
	{
	case 0:		strNodeName.LoadString(IDS_STRING_EFFECT);		iCount = iEffectCount;	break;
	case 1:		strNodeName.LoadString(IDS_STRING_SOUND);		iCount = iSoundCount;	break;
	case 2:		strNodeName.LoadString(IDS_STRING_REFREE);		iCount = iRefereeCount;	break;
	default:	NTL_ASSERT(false, "Invalid direction node type");	return;
	}		

	// 기본 이름을 설정한다
	while( true )
	{
		sprintf_s(acBuffer, 128, "%s%d", (LPSTR)(LPCSTR)strNodeName, iCount);

		if( !GetDirectionItem(LIST_TYPE_NODE, acBuffer) )
			break;

		++iCount;
	}

	CNtlSLCENode* pNewDirectionNode;
	switch( dlg.GetSelectedIndex() )
	{
	case CNtlSLCENode::E_CE_EFFECT:
		{
			CNtlSLCENodeEffect* pNodeEffect = NTL_NEW CNtlSLCENodeEffect;
			pNodeEffect->SetEffectName("EffectName");
			pNewDirectionNode	= pNodeEffect;
			iEffectCount		= iCount;
			break;
		}
	case CNtlSLCENode::E_CE_SOUND:
		{
			CNtlSLCENodeSound* pNodeSound = NTL_NEW CNtlSLCENodeSound;
			pNewDirectionNode	= pNodeSound;
			iSoundCount			= iCount;
			break;
		}
	case CNtlSLCENode::E_CE_JUDGE:
		{
			CNtlSLCENodeReferee* pNodeReferee = NTL_NEW CNtlSLCENodeReferee;
			pNewDirectionNode	= pNodeReferee;
			iRefereeCount		= iCount;
			break;
		}
	default:
		{
			NTL_ASSERT(false, "CDirectionPane::OnAddNode, Invalid node type");
			return;
		}
	}

	// 노드
	pNewDirectionNode->SetName(acBuffer);	

	// Direction Item
	sDirectionItem* pNewDirectionItem = NTL_NEW sDirectionItem;
	pNewDirectionItem->byItemType			= LIST_TYPE_NODE;
	pNewDirectionItem->byNodeType			= dlg.GetSelectedIndex();
	pNewDirectionItem->pData				= pNewDirectionNode;

	// Tree list data
	HTREEITEM hTreeItem = m_wndTreeCtrl.InsertItem(acBuffer, 1, 1, hParent_of_NewItem);
	m_wndTreeCtrl.SetItemData(hTreeItem, (DWORD_PTR)pNewDirectionItem);
	m_listDirectionItem.push_back(pNewDirectionItem);

	// Add node to controller
	pController->AddNode(pNewDirectionNode);
	pNewDirectionNode->SetPosition( pController->GetPosition() );
}

void CDirectionPane::OnRemove()
{
	HTREEITEM hSelectedItem = m_wndTreeCtrl.GetSelectedItem();

	if(!hSelectedItem)
	{
		AfxMessageBox("You must select the set or node");
		return;
	}

	sDirectionItem* pDelDirectionItem = reinterpret_cast<sDirectionItem*>( m_wndTreeCtrl.GetItemData(hSelectedItem) );

	switch( pDelDirectionItem->byItemType )
	{
	case LIST_TYPE_CONTROLLER:
		{
			if( m_wndTreeCtrl.GetChildItem(hSelectedItem) )
			{
				AfxMessageBox("Can not remove, because have child node");
				return;
			}

			ITER_DIRECTION_ITEM it = m_listDirectionItem.begin();
			for( ; it != m_listDirectionItem.end() ; ++it )
			{
				if( pDelDirectionItem == (*it) )
				{
					m_listDirectionItem.erase(it);
					break;
				}
			}

			m_wndTreeCtrl.DeleteItem(hSelectedItem);

			CNtlSLTBCrowdController* pController = (CNtlSLTBCrowdController*)pDelDirectionItem->pData;
			GetNtlSLCrowdManager()->RemoveController(pController->GetName());

			break;
		}
	case LIST_TYPE_NODE:
		{
			HTREEITEM hParentItem = m_wndTreeCtrl.GetParentItem(hSelectedItem);
			NTL_ASSERT(hParentItem, "CDirectionPane::OnRemove, The parent of node not exist");
			
			sDirectionItem* pParentDirectionItem = (sDirectionItem*)m_wndTreeCtrl.GetItemData(hParentItem);
			CNtlSLTBCrowdController* pController = (CNtlSLTBCrowdController*)pParentDirectionItem->pData;
			CNtlSLCENode* pNode = (CNtlSLCENode*)pDelDirectionItem->pData;

			pController->RemoveNode(pNode);

			ITER_DIRECTION_ITEM it = m_listDirectionItem.begin();
			for( ; it != m_listDirectionItem.end() ; ++it )
			{
				if( pDelDirectionItem == (*it) )
				{
					m_listDirectionItem.erase(it);
					break;
				}
			}

			m_wndTreeCtrl.DeleteItem(hSelectedItem);

			NTL_DELETE(pDelDirectionItem);

			break;
		}
	default:
		{
			NTL_ASSERT(false, "CDirectionPane::OnRemove, Invalid direction item type");
			return;
		}
	}


	if( hSelectedItem == m_hDirectingItem )
		m_hDirectingItem = NULL;
}

void CDirectionPane::OnRun()
{
	HTREEITEM hSelectedItem = m_wndTreeCtrl.GetSelectedItem();

	if(!hSelectedItem)
	{
		AfxMessageBox("You must select the controller or node");
		return;
	}

	// 기존에 연출중인 것은 종료
	StopDirectoin(m_hDirectingItem);
	m_hDirectingItem = hSelectedItem;


	sDirectionItem* pDirectionItem = (sDirectionItem*)m_wndTreeCtrl.GetItemData(m_hDirectingItem);

	switch(pDirectionItem->byItemType)
	{
	case LIST_TYPE_CONTROLLER:
		{
			CNtlSLTBCrowdController* pController = (CNtlSLTBCrowdController*)pDirectionItem->pData;
			GetNtlSLCrowdManager()->AddPlayController(pController);
			break;
		}
	case LIST_TYPE_NODE:
		{
			CNtlSLCENode* pNode = (CNtlSLCENode*)pDirectionItem->pData;
			pNode->Create();
			break;
		}
	default:
		{
			NTL_ASSERT(false, "CDirectionPane::IsExistSameName_inSibling, Invalid direction node type");
			break;
		}
	}
}

void CDirectionPane::OnStop()
{
	HTREEITEM hSelectedItem = m_wndTreeCtrl.GetSelectedItem();
	if( !hSelectedItem )
		return;

	StopDirectoin(hSelectedItem);
}

BOOL CDirectionPane::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	NMHDR* pNMHDR = (NMHDR*)lParam;

	if( pNMHDR->idFrom == IDC_DIRECTION_PANE_PROP_TREECTRL )
	{
		HTREEITEM hSelectedItem = m_wndTreeCtrl.GetSelectedItem();
		if(hSelectedItem)
		{
			switch( pNMHDR->code )
			{
			case NM_DBLCLK:
				{
					sDirectionItem* pDirectionItem = reinterpret_cast<sDirectionItem*>( m_wndTreeCtrl.GetItemData(hSelectedItem) );
					NTL_ASSERT(pDirectionItem, "CDirectionPane::OnNotify, invalid sDirectionItem pointer");

					// 이름 변경
					switch( pDirectionItem->byItemType )
					{
					case LIST_TYPE_CONTROLLER:
						{
							CRenamePropDlg renameDlg;
							CNtlSLTBCrowdController* pController = (CNtlSLTBCrowdController*)pDirectionItem->pData;

							renameDlg.SetName( (LPCSTR)(LPSTR)pController->GetName().c_str() );

							if( renameDlg.DoModal() == IDOK )
							{
								CString strName = renameDlg.GetName();

								if( IsExistSameName_inSibling(NULL, (LPSTR)(LPCSTR)strName) )
								{
									AfxMessageBox("Same name exist in controller");
									return true;
								}
								
								pController->SetName((LPSTR)(LPCSTR)strName);
								m_wndTreeCtrl.SetItemText(hSelectedItem, strName);
							}							
							break;
						}
					case LIST_TYPE_NODE:
						{							
							CRenamePropDlg dlg;
							CNtlSLCENode* pSelectedNode = (CNtlSLCENode*)pDirectionItem->pData;

							dlg.SetName( (LPCSTR)(LPSTR)pSelectedNode->GetName().c_str() );

							if( dlg.DoModal() == IDOK )
							{
								CString strName = dlg.GetName();

								if( IsExistSameName_inSibling(m_wndTreeCtrl.GetParentItem(hSelectedItem), (LPSTR)(LPCSTR)strName) )
								{
									AfxMessageBox("Same name exist in controller");
									return true;
								}

								pSelectedNode->SetName((LPSTR)(LPCSTR)strName);
								m_wndTreeCtrl.SetItemText(hSelectedItem, strName);
							}
							break;
						}
					default:
						{
							NTL_ASSERT(false, "CDirectionPane::OnNotify, Invalid direction item type");
							return CWnd::OnNotify(wParam, lParam, pResult);
						}
					}

					break;
				}
			case TVN_SELCHANGED:
				{
					sDirectionItem* pDirectionItem = reinterpret_cast<sDirectionItem*>( m_wndTreeCtrl.GetItemData(hSelectedItem) );
					NTL_ASSERT(pDirectionItem, "CDirectionPane::OnNotify, invalid sDirectionItem pointer");

					// Property Pane 변경
					switch( pDirectionItem->byItemType )
					{
					case LIST_TYPE_CONTROLLER:
						{
							((CMainFrame*)AfxGetMainWnd())->SetDirectionProperty((CNtlSLTBCrowdController*)pDirectionItem->pData);
							break;
						}
					case LIST_TYPE_NODE:
						{
							((CMainFrame*)AfxGetMainWnd())->SetDirectionProperty(pDirectionItem->byNodeType, (CNtlSLCENode*)pDirectionItem->pData);
							break;
						}
					default:
						{
							NTL_ASSERT(false, "CDirectionPane::OnNotify, Invalid direction item type");
							break;
						}
					}

					break;
				}
			}
		}
	}

	return CWnd::OnNotify(wParam, lParam, pResult);
}

sDirectionItem* CDirectionPane::GetDirectionItem(BYTE byItemType, char* pcName)
{
	for each( sDirectionItem* pDirectionItem in m_listDirectionItem )
	{
		if( pDirectionItem->byItemType != byItemType )
			continue;

		switch( pDirectionItem->byItemType )
		{
		case LIST_TYPE_CONTROLLER:
			{
				CNtlSLTBCrowdController* pController = (CNtlSLTBCrowdController*)pDirectionItem->pData;
				if( strcmp(pcName, pController->GetName().c_str() ) == 0 )
					return pDirectionItem;

				break;
			}
		case LIST_TYPE_NODE:
			{
				CNtlSLCENode* pNode = (CNtlSLCENode*)pDirectionItem->pData;
				if( strcmp(pcName, pNode->GetName().c_str() ) == 0 )
					return pDirectionItem;

				break;
			}
		default:
			{
				NTL_ASSERT(false, "CDirectionPane::GetDirectionItem, Invalid direction item type : " << pDirectionItem->byItemType);
				break;
			}
		}
	}

	return NULL;
}

void CDirectionPane::StopDirectoin(HTREEITEM hItem)
{
	if( !hItem )
		return;

	sDirectionItem* pDirectionItem = (sDirectionItem*)m_wndTreeCtrl.GetItemData(hItem);
	StopDirectoin(pDirectionItem);
}

void CDirectionPane::StopDirectoin(sDirectionItem* pDirectionItem)
{
	if( !pDirectionItem )
		return;

	switch( pDirectionItem->byItemType )
	{
	case LIST_TYPE_CONTROLLER:
		{
			CNtlSLTBCrowdController* pController = (CNtlSLTBCrowdController*)pDirectionItem->pData;
			GetNtlSLCrowdManager()->RemovePlayController( pController->GetName() );
			break;
		}
	case LIST_TYPE_NODE:
		{
			CNtlSLCENode* pNode = (CNtlSLCENode*)pDirectionItem->pData;
			pNode->Destory();
			break;
		}
	default:
		{
			NTL_ASSERT(false, "CDirectionPane::StopDirectoin, Invalid direction item type");
			return;
		}
	}

	m_hDirectingItem = NULL;
}

void CDirectionPane::Update(float fElpased)
{
	GetNtlSLCrowdManager()->Update(fElpased);
	GetNtlSoundMoodManager()->Update(fElpased);
}

void CDirectionPane::DestroyTreeItem()
{
	for each( sDirectionItem* pDirectionItem in m_listDirectionItem )
	{
		if( pDirectionItem->byItemType == LIST_TYPE_CONTROLLER )
		{			
			CNtlSLTBCrowdController* pController = (CNtlSLTBCrowdController*)pDirectionItem->pData;
			GetNtlSLCrowdManager()->RemoveController( pController->GetName() );
		}
	}

	m_listDirectionItem.clear();
	m_wndTreeCtrl.DeleteAllItems();
}

bool CDirectionPane::IsExistSameName_inSibling(HTREEITEM hParentItem, const char* pcName)
{
	HTREEITEM hItem = NULL;

	if( hParentItem )
		hItem = m_wndTreeCtrl.GetChildItem(hParentItem);
	else
		hItem = m_wndTreeCtrl.GetRootItem();

	while( hItem )
	{
		sDirectionItem* pDirectionItem = (sDirectionItem*)m_wndTreeCtrl.GetItemData(hItem);

		switch(pDirectionItem->byItemType)
		{
		case LIST_TYPE_CONTROLLER:
			{
				CNtlSLTBCrowdController* pController = (CNtlSLTBCrowdController*)pDirectionItem->pData;
				if( strcmp(pController->GetName().c_str(), pcName) == 0 )
					return true;

				break;
			}
		case LIST_TYPE_NODE:
			{
				CNtlSLCENode* pNode = (CNtlSLCENode*)pDirectionItem->pData;
				if( strcmp(pNode->GetName().c_str(), pcName) == 0 )
					return true;

				break;
			}
		default:
			{
				NTL_ASSERT(false, "CDirectionPane::IsExistSameName_inSibling, Invalid direction node type");
				break;
			}
		}

		hItem = m_wndTreeCtrl.GetNextSiblingItem(hItem);
	}

	return false;
}