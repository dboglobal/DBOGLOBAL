// SkillPane.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "SkillPane.h"
#include "DTEditorGlobal.h"


// shared
#include "HTBSetTable.h"
#include "SkillTable.h"
#include "TableContainer.h"

// simulation
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlSLEventFunc.h"
#include "NtlFSMDef.h"
#include "NtlFSMStateBase.h"


LRESULT CSkillPane::CSkillTree::WindowProc(UINT message,WPARAM wParam,LPARAM lParam)
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

BEGIN_MESSAGE_MAP(CSkillPane::CSkillTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CSkillTree::OnTvnSelchanged)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CSkillPane::CSkillTree::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HTREEITEM hItem = GetSelectedItem();

	*pResult = 0;
}

void CSkillPane::CSkillTree::OnDestroy()
{
	CTreeCtrl::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

// CSkillPane

IMPLEMENT_DYNAMIC(CSkillPane, CWnd)

CSkillPane::CSkillPane()
{

}

CSkillPane::~CSkillPane()
{
}


BEGIN_MESSAGE_MAP(CSkillPane, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_SKILL_PANE_USE, OnSkillUse)
	ON_UPDATE_COMMAND_UI(ID_SKILL_PANE_USE, OnUpdateSkillUse)
END_MESSAGE_MAP()




void CSkillPane::InitSkill(void)
{
	std::list<SSkillTreeData*>::iterator ittree;
	for(ittree = m_listTreeData.begin(); ittree != m_listTreeData.begin(); ittree++)
	{
		delete (*ittree);
	}

	m_listTreeData.clear();

	m_wndTreeCtrl.DeleteAllItems();

	SSkillTreeData *pTreeData;

	pTreeData			= new SSkillTreeData;
	pTreeData->byType	= SKILL_TREE_FOLDER;
	pTreeData->pData	= NULL;
	
	m_hHTBItem = m_wndTreeCtrl.InsertItem(_T("HTB"), 0, 0, NULL);
	m_wndTreeCtrl.SetItemData(m_hHTBItem, (DWORD_PTR)pTreeData);
	m_listTreeData.push_back(pTreeData);

	CHTBSetTable *pHTBSetTbl	= API_GetTableContainer()->GetHTBSetTable();
	CTable::TABLEIT itbegin		= pHTBSetTbl->Begin();
	CTable::TABLEIT itend		= pHTBSetTbl->End();
	CTable::TABLEIT it;

	CString strName, strTable;
	char chBuffer[1024];

	for(it = itbegin; it != itend; it++)
	{
		sHTB_SET_TBLDAT *pHTBSetTblData = reinterpret_cast<sHTB_SET_TBLDAT*>( (*it).second );

		RwUInt8 byClass = GetAvatarClass();
		RwUInt32 uiClassFlag = 0;
		if(byClass == PC_CLASS_STREET_FIGHTER)
		{
			uiClassFlag = PC_CLASS_FLAG_STREET_FIGHTER;
		}
		else if(byClass == PC_CLASS_SWORD_MASTER)
		{
			uiClassFlag = PC_CLASS_FLAG_SWORD_MASTER;
		}
		else if(byClass == PC_CLASS_CRANE_ROSHI)
		{
			uiClassFlag = PC_CLASS_FLAG_CRANE_ROSHI;
		}
		else if(byClass == PC_CLASS_TURTLE_ROSHI)
		{
			uiClassFlag = PC_CLASS_FLAG_TURTLE_ROSHI;
		}
		else if(byClass == PC_CLASS_DARK_WARRIOR)
		{
			uiClassFlag = PC_CLASS_FLAG_DARK_WARRIOR;
		}
		else if(byClass == PC_CLASS_SHADOW_KNIGHT)
		{
			uiClassFlag = PC_CLASS_FLAG_SHADOW_KNIGHT;
		}
		else if(byClass == PC_CLASS_DENDEN_HEALER)
		{
			uiClassFlag = PC_CLASS_FLAG_DENDEN_HEALER;
		}
		else if(byClass == PC_CLASS_POCO_SUMMONER)
		{
			uiClassFlag = PC_CLASS_FLAG_POCO_SUMMONER;
		}
		else if(byClass == PC_CLASS_GRAND_MA)
		{
			uiClassFlag = PC_CLASS_FLAG_GRAND_MA;
		}
		else if(byClass == PC_CLASS_ULTI_MA)
		{
			uiClassFlag = PC_CLASS_FLAG_ULTI_MA;
		}
		else if(byClass == PC_CLASS_PLAS_MA)
		{
			uiClassFlag = PC_CLASS_FLAG_PLAS_MA;
		}
		else if(byClass == PC_CLASS_KAR_MA)
		{
			uiClassFlag = PC_CLASS_FLAG_KAR_MA;
		}


		if( pHTBSetTblData->dwPC_Class_Bit_Flag & uiClassFlag )
		{
			::WideCharToMultiByte(GetACP(), 0, pHTBSetTblData->wszNameText, -1, chBuffer, 256, NULL, NULL);
			strName = chBuffer;
			strTable.Format("( %d )", pHTBSetTblData->tblidx);
			strName += strTable;

			HTREEITEM hItem = m_wndTreeCtrl.InsertItem(strName, 1, 1, m_hHTBItem);
			m_wndTreeCtrl.SetItemData(hItem, (DWORD_PTR)pHTBSetTblData);

			pTreeData			= new SSkillTreeData;
			pTreeData->byType	= SKILL_TREE_DATA;
			pTreeData->pData	= pHTBSetTblData;

			m_wndTreeCtrl.SetItemData(hItem, (DWORD_PTR)pTreeData);
			m_listTreeData.push_back(pTreeData);
		}
	}
}


// CSkillPane 메시지 처리기입니다.



BOOL CSkillPane::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

int CSkillPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	XTPImageManager()->SetMaskColor(RGB(0, 0xFF, 0));
	VERIFY(m_wndToolBar.CreateToolBar(WS_VISIBLE|WS_CHILD|CBRS_TOOLTIPS, this));
	VERIFY(m_wndToolBar.LoadToolBar(IDR_SKILL_PANE_TOOLBAR));


	m_ilTreeIcons.Create(16, 16, ILC_MASK|ILC_COLOR24, 0, 1);

	CBitmap bmp;
	bmp.LoadBitmap(IDB_OBJECTPANE_TOOLBARS_ICONS);

	m_ilTreeIcons.Add(&bmp, RGB(0, 255, 0));


	if (!m_wndTreeCtrl.Create(WS_CLIPCHILDREN|WS_CLIPSIBLINGS|WS_VISIBLE|TVS_HASLINES|
		TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS, CRect(0, 0, 0, 0), this, IDC_SKILL_PANE_PROP_TREECTRL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	m_wndTreeCtrl.ModifyStyleEx(0, WS_EX_STATICEDGE);

	m_wndTreeCtrl.SetImageList(&m_ilTreeIcons, TVSIL_NORMAL);

	// htb를 읽어온다. 

	InitSkill();
	
	return 0;
}

void CSkillPane::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	for each( SSkillTreeData* pTreeData in m_listTreeData )
		NTL_DELETE(pTreeData);

	m_listTreeData.clear();
}

void CSkillPane::OnSize(UINT nType, int cx, int cy)
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

void CSkillPane::OnSkillUse()
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	SERIAL_HANDLE hTargetSerialId = GetSelectTarget();

	if(hTargetSerialId == INVALID_SERIAL_ID)
	{
		AfxMessageBox("Target select Please !!!");
		return;
	}

	HTREEITEM hSelectItem = m_wndTreeCtrl.GetSelectedItem();
	SSkillTreeData *pTreeData = (SSkillTreeData*)m_wndTreeCtrl.GetItemData(hSelectItem);

	sHTB_SET_TBLDAT* pHTBSkillTbl = (sHTB_SET_TBLDAT*)pTreeData->pData;

	RwReal fSkillRange = 0;
	for(RwInt32 i = 0; i < NTL_HTB_MAX_SKILL_COUNT_IN_SET; ++i)
	{
		if(pHTBSkillTbl->aHTBAction[i].skillTblidx != INVALID_TBLIDX)
		{
			CSkillTable *pSkillTbl = API_GetTableContainer()->GetSkillTable();
			sSKILL_TBLDAT *pSkillTblData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTbl->FindData(pHTBSkillTbl->aHTBAction[i].skillTblidx) );
			fSkillRange = Logic_GetSkillUseRange(pSkillTblData);
			break;
		}
	}

	SetActiveHTBTableData(pHTBSkillTbl);

	CNtlSLEventGenerator::ActionMapHTBUse(pSobAvatar->GetSerialID(), hTargetSerialId, INVALID_SERIAL_ID, fSkillRange, 0);
}

void CSkillPane::OnUpdateSkillUse(CCmdUI* pCmdUI)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if(pSobAvatar->GetCurrentState()->GetStateId() == NTL_FSMSID_HTB)
	{
		pCmdUI->Enable(FALSE);
		return;
	}

	HTREEITEM hSelectItem = m_wndTreeCtrl.GetSelectedItem();

	if(hSelectItem)
	{
		SSkillTreeData *pTreeData = (SSkillTreeData*)m_wndTreeCtrl.GetItemData(hSelectItem);
		if(pTreeData->byType == SKILL_TREE_FOLDER)
			pCmdUI->Enable(FALSE);
		else
		{
			CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
			SERIAL_HANDLE hTargetSerialId = GetSelectTarget();

			if(pSobAvatar->GetSerialID() == hTargetSerialId)
				pCmdUI->Enable(FALSE);
			else
				pCmdUI->Enable(TRUE);
		}
	}
	else
		pCmdUI->Enable(FALSE);
}