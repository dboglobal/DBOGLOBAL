// BotPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "BotPane.h"
#include "DTEditorGlobal.h"
#include "DirectApp.h"

// shared
#include "MobTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// simulation
#include "NtlSLApi.h"




LRESULT CBotPane::CBotTree::WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
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

BEGIN_MESSAGE_MAP(CBotPane::CBotTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CBotTree::OnTvnSelchanged)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CBotPane::CBotTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HTREEITEM hItem = GetSelectedItem();

	*pResult = 0;
}

void CBotPane::CBotTree::OnDestroy()
{
	CTreeCtrl::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


// CBotPane

IMPLEMENT_DYNAMIC(CBotPane, CWnd)

CBotPane::CBotPane()
{

}

CBotPane::~CBotPane()
{
}


BEGIN_MESSAGE_MAP(CBotPane, CWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_BOT_PANE_CREATE, OnBotCreate)
	ON_UPDATE_COMMAND_UI(ID_BOT_PANE_CREATE, OnUpdateBotCreate)
END_MESSAGE_MAP()



// CBotPane 메시지 처리기입니다.



int CBotPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	XTPImageManager()->SetMaskColor(RGB(0, 0xFF, 0));
	VERIFY(m_wndToolBar.CreateToolBar(WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_BOT_PANE_TOOLBAR));


	m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR24, 0, 1);

	CBitmap bmp;
	bmp.LoadBitmap(IDB_OBJECTPANE_TOOLBARS_ICONS);

	m_ilTreeIcons.Add(&bmp, RGB(0, 255, 0));


	if (!m_wndTreeCtrl.Create(WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE|TVS_HASLINES|
		TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS, CRect(0, 0, 0, 0), this, IDC_BOT_PANE_PROP_TREECTRL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	m_wndTreeCtrl.ModifyStyleEx(0, WS_EX_STATICEDGE);

	m_wndTreeCtrl.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);

	// monster folder

	char chBuffer[1024];

	m_hMobItem				= m_wndTreeCtrl.InsertItem(_T("Monster"), 0, 0, NULL);
	SBotTreeData *pTreeData = new SBotTreeData;
	pTreeData->byType = BOT_TREE_FOLDER;

	m_listTreeData.push_back(pTreeData);
	m_wndTreeCtrl.SetItemData(m_hMobItem, (DWORD_PTR)pTreeData);

	for(int i = 0; i < 10; i++)
	{
		pTreeData			= new SBotTreeData;
		pTreeData->byType	= BOT_TREE_FOLDER;

		m_listTreeData.push_back(pTreeData);

		sprintf_s(chBuffer, "Level %d ~ %d", i*10, (i+1)*10);

		m_hMobLevelItem[i] = m_wndTreeCtrl.InsertItem(_T(chBuffer), 0, 0, m_hMobItem);
		m_wndTreeCtrl.SetItemData(m_hMobLevelItem[i], (DWORD_PTR)pTreeData);
	}

	// monster data

	CMobTable		*pMobTbl			= API_GetTableContainer()->GetMobTable();
	CTextAllTable	*pTextAllTbl		= API_GetTableContainer()->GetTextAllTable();
	CTextTable		*pTextTbl			= pTextAllTbl->GetMobTbl();

	CTable::TABLEIT itbegin		= pMobTbl->Begin();
	CTable::TABLEIT itend		= pMobTbl->End();
	CTable::TABLEIT it;

	for(it = itbegin; it != itend; it++)
	{
		sMOB_TBLDAT *pMobTblData = reinterpret_cast<sMOB_TBLDAT*>( (*it).second );
		sTEXT_TBLDAT *pTextTblData = reinterpret_cast<sTEXT_TBLDAT*>( pTextTbl->FindData(pMobTblData->Name) );
		if(pTextTblData)
		{
			::WideCharToMultiByte(GetACP(), 0, (LPCWSTR)pTextTblData->wstrText.c_str(), -1, chBuffer, 256, NULL, NULL);
			HTREEITEM hItem = m_wndTreeCtrl.InsertItem(chBuffer, 1, 1, m_hMobLevelItem[pMobTblData->byLevel/10]);

			pTreeData			= new SBotTreeData;
			pTreeData->byType	= BOT_TREE_DATA;
			pTreeData->pData	= (void*)pMobTblData;

			m_listTreeData.push_back(pTreeData);
			m_wndTreeCtrl.SetItemData(hItem, (DWORD_PTR)pTreeData);
		}
	}

	return 0;
}

void CBotPane::OnSize(UINT nType, int cx, int cy)
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

void CBotPane::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	std::list<SBotTreeData*>::iterator it;
	for(it = m_listTreeData.begin(); it != m_listTreeData.end(); it++)
	{
		delete (*it);
	}

	m_listTreeData.clear();
}


void CBotPane::OnBotCreate()
{
	HTREEITEM hSelectedItem = m_wndTreeCtrl.GetSelectedItem();
	SBotTreeData *pBotTreeData = (SBotTreeData*)m_wndTreeCtrl.GetItemData(hSelectedItem);
	if(pBotTreeData->byType == BOT_TREE_FOLDER)
		return;

	SetActiveMonsterTableData((const sMOB_TBLDAT*)pBotTreeData->pData);

	CDirectApplication *pApp = (CDirectApplication*)CNtlApplication::GetInstance();
	pApp->OnMobCreate();
}

void CBotPane::OnUpdateBotCreate(CCmdUI* pCmdUI)
{
	HTREEITEM hSelectedItem = m_wndTreeCtrl.GetSelectedItem();

	if(hSelectedItem)
	{
		SBotTreeData *pBotTreeData = (SBotTreeData*)m_wndTreeCtrl.GetItemData(hSelectedItem);
		if(pBotTreeData->byType == BOT_TREE_FOLDER)
		{
			pCmdUI->Enable(FALSE);
		}
		else
		{
			if(GetEditModeType() == EDIT_MODE_THIRD_CAMERA)
				pCmdUI->Enable(TRUE);
			else
				pCmdUI->Enable(FALSE);
		}
	}
	else
		pCmdUI->Enable(FALSE);
}