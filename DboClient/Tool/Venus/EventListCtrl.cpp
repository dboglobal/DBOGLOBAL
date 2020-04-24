// EventListCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "VenusFrame.h"
#include "EventListCtrl.h"
#include ".\eventlistctrl.h"

#include "StringManager.h"

#include "EventLayer.h"

#include "VenusPlayerEventManager.h"

#include "NtlDebug.h"

// CEventListCtrl

IMPLEMENT_DYNAMIC(CEventListCtrl, CListCtrl)
CEventListCtrl::CEventListCtrl()
{
}

CEventListCtrl::~CEventListCtrl()
{
}


BEGIN_MESSAGE_MAP(CEventListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(POPUP_CREATE_EVENT, OnCreateEvent)
	ON_COMMAND(POPUP_DELETE_EVENT, OnDeleteEvent)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
	ON_COMMAND(POPUP_LOAD_EVENT, OnLoadEvent)
	ON_COMMAND(POPUP_SAVE_EVENT, OnSaveEvent)
	ON_COMMAND(POPUP_CLEAR_EVENT, OnClearEvent)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
END_MESSAGE_MAP()



// CEventListCtrl 메시지 처리기입니다.


int CEventListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CVenusGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	if(!m_image.Create(IDB_EFFECT_LIST_IMAGE,16,1,RGB(0, 255, 255)))
		return -1;

	SetImageList(&m_image, LVSIL_SMALL);
	CImageList *pImageList = GetImageList(LVSIL_SMALL);
	if(pImageList)
		ImageList_GetIconSize(pImageList->m_hImageList, &m_cxImage, &m_cyImage);
	else
		return -1;

	return 0;
}

void CEventListCtrl::InitializeGrid()
{
	/*
	ExtendedStyle support:
	LVS_EX_TRACKSELECT
	LVS_EX_GRIDLINES
	LVS_EX_FLATSB
	LVS_EX_CHECKBOXES
	LVS_EX_FLATSB
	all other ExStyles are not supported...buhhh and you call your self a windows-developer :(
	*/
	SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);

	LPTSTR lpszCols[] = {_T("Name"),0};

	LV_COLUMN   lvColumn;
	//initialize the columns
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 250;
	lvColumn.pszText = lpszCols[0];
	InsertColumn(0, &lvColumn);

//	lvColumn.cx = 100;
//	lvColumn.pszText = lpszCols[1];
//	InsertColumn(1, &lvColumn);
}

void CEventListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu(MF_STRING, POPUP_CREATE_EVENT, "&Create Event");
	if(GetSelectedItem() != -1)
	{
		menu.AppendMenu(MF_STRING, POPUP_DELETE_EVENT, "&Delete Event");
	}

	menu.AppendMenu(MF_SEPARATOR, 0, "");
	menu.AppendMenu(MF_STRING, POPUP_LOAD_EVENT, "&Load Event");
	if (!CVenusPlayerEventManager::GetInstance().m_svVenusPlayerEvent.empty())
	{
		menu.AppendMenu(MF_STRING, POPUP_SAVE_EVENT, "&Save Event");

		menu.AppendMenu(MF_SEPARATOR, 0, "");
		menu.AppendMenu(MF_STRING, POPUP_CLEAR_EVENT, "&Clear Event");
	}


	if(GetSelectedItem() != -1)
	{
		menu.AppendMenu(MF_SEPARATOR, 0, "");
		menu.AppendMenu(MF_STRING, ID_EDIT_CUT,		"Cut\tCtrl+X(&T)");
		menu.AppendMenu(MF_STRING, ID_EDIT_COPY,	"Copy\tCtrl+C(&C)");
	}
	if (CVenusPlayerEventManager::GetInstance().m_pCopyPlayerEvent != NULL)
	{
		menu.AppendMenu(MF_SEPARATOR, 0, "");
		menu.AppendMenu(MF_STRING, ID_EDIT_PASTE,	"Paste\tCtrl+V(&P)");
	}

	menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

void CEventListCtrl::OnCreateEvent()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString strName;
	RwInt32 count = 0;
	do {
		strName.Format("Event%03d", count ++);
	} while(CVenusPlayerEventManager::GetInstance().GetPlayerEvent(strName.GetBuffer(0)));

	CVenusPlayerEvent* pPlayerEvent = (CVenusPlayerEvent*)CVenusPlayerEventManager::GetInstance().CreatePlayerEvent(strName.GetBuffer(0));
	NTL_ASSERTE(pPlayerEvent != NULL);

	InsertTreeItem(pPlayerEvent->GetName(), 0, (DWORD_PTR)pPlayerEvent, NULL, LVIS_SELECTED | LVIS_FOCUSED);
}

void CEventListCtrl::OnDeleteEvent()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (AfxMessageBox("really ?", MB_OKCANCEL) != IDOK) return;
	int nItem = GetSelectedItem();
	if(nItem == -1) return;

	CTreeItem* pSelectItem = GetTreeItem(nItem);
	if(pSelectItem == NULL) return;

	CItemInfo* pItem = GetData(pSelectItem); 
	CVenusPlayerEvent* pPlayerEvent = (CVenusPlayerEvent*)pItem->m_lParam;
	NTL_ASSERTE(pPlayerEvent);

	CString strPathName = pPlayerEvent->GetName();
	if (CVenusPlayerEventManager::GetInstance().DeletePlayerEvent(pPlayerEvent->GetName()))
	{
		DeleteItemEx(GetTreeItem(GetSelectedItem()), GetSelectedItem());
	}
}

void CEventListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nItem = GetSelectedItem();
	if(nItem == -1)
	{
		CVenusPlayerEventManager::GetInstance().SetSelectPlayerEvent(NULL);
		CEventLayer* pView = (CEventLayer*)gGetEditLayer(EDIT_LAYER_EVENT);
		pView->ResetPropertiesAvatar();
		return;
	}

	CTreeItem* pSelectItem = GetTreeItem(nItem);
	if(pSelectItem == NULL)
	{
		CVenusPlayerEventManager::GetInstance().SetSelectPlayerEvent(NULL);
		CEventLayer* pView = (CEventLayer*)gGetEditLayer(EDIT_LAYER_EVENT);
		pView->ResetPropertiesAvatar();
		return;
	}

	CItemInfo* pItem = GetData(pSelectItem); 
	CVenusPlayerEvent* pPlayerEvent = (CVenusPlayerEvent*)pItem->m_lParam;
	NTL_ASSERTE(pPlayerEvent);

	CVenusPlayerEventManager::GetInstance().SetSelectPlayerEvent(pPlayerEvent);

	CEventLayer* pView = (CEventLayer*)gGetEditLayer(EDIT_LAYER_EVENT);
	pView->ResetPropertiesAvatar();

	*pResult = 0;
}

void CEventListCtrl::OnLoadEvent()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (!CVenusPlayerEventManager::GetInstance().m_svVenusPlayerEvent.empty())
	{
		if (AfxMessageBox(CStringManager::GetInstance().GetString(STRING_SAVE), MB_OKCANCEL) == IDOK)
		{
			OnSaveEvent();
		}
		CVenusPlayerEventManager::GetInstance().Delete();
	}


	CString strFilter;
	strFilter.Format("%s File(*.%s)|*.%s\0", EVENT_FILE_EXE, EVENT_FILE_EXE, EVENT_FILE_EXE);

	CFileDialog dlg(TRUE, NULL, "Venus", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	if (dlg.DoModal() == IDOK)
	{
		SetCurrentDirectory(CVenusConfig::GetInstance().m_strMainPath.c_str());

		CString strExtName;
		strExtName.Format(".%s", EVENT_FILE_EXE);

		CString strPathName = dlg.GetPathName();
		if (strPathName.Find(strExtName) < 0)
		{
			strPathName += strExtName;
		}

		if (CVenusPlayerEventManager::GetInstance().Load(strPathName.GetBuffer(0)))
		{
		}
	}

	CEventLayer* pView = (CEventLayer*)gGetEditLayer(EDIT_LAYER_EVENT);
	pView->ResetPropertiesEnemy();

	RebuildPlayerEvent();

	SetCurrentDirectory(CVenusConfig::GetInstance().m_strMainPath.c_str());
}

void CEventListCtrl::OnSaveEvent()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString strFileName = "Venus";

	CString strFilter;
	strFilter.Format("%s File(*.%s)|*.%s\0", EVENT_FILE_EXE, EVENT_FILE_EXE, EVENT_FILE_EXE);

	CFileDialog dlg(FALSE, NULL, strFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strFilter);
	if (dlg.DoModal() == IDOK)
	{
		SetCurrentDirectory(CVenusConfig::GetInstance().m_strMainPath.c_str());

		CString strExtName;
		strExtName.Format(".%s", EVENT_FILE_EXE);

		CString strPathName = dlg.GetPathName();
		if (strPathName.Find(strExtName) < 0)
		{
			strPathName += strExtName;
		}

		if (CVenusPlayerEventManager::GetInstance().Save(strPathName.GetBuffer(0)))
		{
		}
	}
	SetCurrentDirectory(CVenusConfig::GetInstance().m_strMainPath.c_str());
}

//------------------------------------------------------------------
//	FuncName	: RebuildPlayerEvent
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CEventListCtrl::RebuildPlayerEvent()
{
	DeleteAllItems();
	DeleteAll();

	SetRedraw(1);

	CVenusPlayerEventManager::svdef_VenusPlayerEvent::iterator it;
	for (it = CVenusPlayerEventManager::GetInstance().m_svVenusPlayerEvent.begin(); it != CVenusPlayerEventManager::GetInstance().m_svVenusPlayerEvent.end(); ++ it)
	{
		CVenusPlayerEvent* pPlayerEvent = (*it);
		InsertTreeItem(pPlayerEvent->GetName(), 0, (DWORD_PTR)pPlayerEvent, NULL, LVIS_SELECTED | LVIS_FOCUSED);
	}

	SetRedraw(1);
}

void CEventListCtrl::OnClearEvent()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (AfxMessageBox("Really ?", MB_OKCANCEL) == IDOK)
	{
		CVenusPlayerEventManager::GetInstance().Delete();

		CEventLayer* pView = (CEventLayer*)gGetEditLayer(EDIT_LAYER_EVENT);
		pView->ResetPropertiesEnemy();


		DeleteAllItems();
		DeleteAll();


		Invalidate(FALSE);
	}
}

void CEventListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nChar)
	{
	case VK_CONTROL:
		{
		}
		break;
	case 'C':
		{
			if (nFlags & MK_CONTROL)
				OnEditCopy();
		}
		break;
	case 'T':
		{
			if (nFlags & MK_CONTROL)
				OnEditCut();
		}
		break;
	case 'V':
		{
			if (nFlags & MK_CONTROL)
			{
				if (CVenusPlayerEventManager::GetInstance().m_pCopyPlayerEvent != NULL)
				{
					OnEditPaste();
				}
			}
		}
		break;
	case VK_INSERT:
		{
			OnCreateEvent();
		}
		break;
	case VK_DELETE:
		{
			if(GetSelectedItem() != -1)
			{
				OnDeleteEvent();
			}
		}
		break;
	}

	CVenusGridCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEventListCtrl::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (CVenusPlayerEventManager::GetInstance().m_pSelectPlayerEvent)
		CVenusPlayerEventManager::GetInstance().SetCopyPlayerEvent(TRUE, CVenusPlayerEventManager::GetInstance().m_pSelectPlayerEvent);
}

void CEventListCtrl::OnEditCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (CVenusPlayerEventManager::GetInstance().m_pSelectPlayerEvent)
		CVenusPlayerEventManager::GetInstance().SetCopyPlayerEvent(FALSE, CVenusPlayerEventManager::GetInstance().m_pSelectPlayerEvent);
}

void CEventListCtrl::OnEditPaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	NTL_ASSERTE(CVenusPlayerEventManager::GetInstance().m_pCopyPlayerEvent != NULL);

	CString strName;
	RwInt32 count = 0;
	do {
		strName.Format("Event%03d", count ++);
	} while(CVenusPlayerEventManager::GetInstance().GetPlayerEvent(strName.GetBuffer(0)));

	CVenusPlayerEvent* pPlayerEvent = (CVenusPlayerEvent*)CVenusPlayerEventManager::GetInstance().CreatePlayerEvent(strName.GetBuffer(0), CVenusPlayerEventManager::GetInstance().m_pCopyPlayerEvent);
	NTL_ASSERTE(pPlayerEvent != NULL);

	InsertTreeItem(pPlayerEvent->GetName(), 0, (DWORD_PTR)pPlayerEvent, NULL, LVIS_SELECTED | LVIS_FOCUSED);
}
