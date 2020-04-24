// SplinePathListCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Venus.h"
#include "VenusFrame.h"
#include "SplinePathListCtrl.h"
#include ".\splinepathlistctrl.h"

#include "PathLayer.h"
#include "EffectEditLayer.h"

#include "NtlEffectSplinePath.h"
#include "VenusPropertyContainer.h"
#include ".\venusvisualmanager.h"

#include "CreateSplinePathDialog.h"


// CSplinePathListCtrl

IMPLEMENT_DYNAMIC(CSplinePathListCtrl, CListCtrl)
CSplinePathListCtrl::CSplinePathListCtrl()
{
}

CSplinePathListCtrl::~CSplinePathListCtrl()
{
}


BEGIN_MESSAGE_MAP(CSplinePathListCtrl, CListCtrl)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(POPUP_CREATE_PATH, OnCreatePath)
	ON_COMMAND(POPUP_DELETE_PATH, OnDeletePath)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnLvnItemchanged)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CSplinePathListCtrl 메시지 처리기입니다.


int CSplinePathListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
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


void CSplinePathListCtrl::InitializeGrid()
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

	LPTSTR lpszCols[] = {_T("Name"),_T("Test"),0};

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
void CSplinePathListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CMenu menu;
	menu.CreatePopupMenu();

	if(GetSelectedItem() == -1)
	{
		menu.AppendMenu(MF_STRING, POPUP_CREATE_PATH, "&Create Path");

		if (CVenusPropertyContainer::GetInstance().m_pCopySplinePath != NULL)
		{
			menu.AppendMenu(MF_SEPARATOR, 0, "");
			menu.AppendMenu(MF_STRING, ID_EDIT_PASTE,	"Paste\tCtrl+V(&P)");
		}

		menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
		return;
	}

	menu.AppendMenu(MF_STRING, POPUP_CREATE_PATH, "&Create Path");
	menu.AppendMenu(MF_STRING, POPUP_DELETE_PATH, "&Delete Path");

	if(GetSelectedItem() != -1)
	{
		menu.AppendMenu(MF_SEPARATOR, 0, "");
		menu.AppendMenu(MF_STRING, ID_EDIT_CUT,		"Cut\tCtrl+X(&T)");
		menu.AppendMenu(MF_STRING, ID_EDIT_COPY,	"Copy\tCtrl+C(&C)");
	}
	if (CVenusPropertyContainer::GetInstance().m_pCopySplinePath != NULL)
	{
		menu.AppendMenu(MF_SEPARATOR, 0, "");
		menu.AppendMenu(MF_STRING, ID_EDIT_PASTE,	"Paste\tCtrl+V(&P)");
	}


	menu.TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
}

void CSplinePathListCtrl::OnCreatePath()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CString strName, strSrcName;
	RwInt32 count = 0;
	do {
		strName.Format("SplinePath%02d", count ++);
	} while(CVenusPropertyContainer::GetInstance().GetProperty(PLENTITY_SPLINE, strName.GetBuffer(0)));

	CCreateSplinePathDialog dlg;
	dlg.SetSplinePath(strName.GetBuffer(0));
	if (dlg.DoModal() != IDOK)
	{
		return;
	}

	CNtlEffectSplinePath* pSplinePath = (CNtlEffectSplinePath*)CVenusPropertyContainer::GetInstance().CreateProperty(PLENTITY_SPLINE, dlg.m_strName.c_str());
	NTL_ASSERTE(pSplinePath != NULL);

	CVenusPropertyContainer::GetInstance().BuildSplinePath(pSplinePath, dlg.m_Info);

	InsertTreeItem(pSplinePath->GetName(), 0, (DWORD_PTR)pSplinePath, NULL, LVIS_SELECTED | LVIS_FOCUSED);

	CEffectEditLayer* pView = (CEffectEditLayer*)gGetEditLayer(EDIT_LAYER_EFFECT_EDIT);
	pView->ResetPropertiesOnly();
}

void CSplinePathListCtrl::OnDeletePath()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (AfxMessageBox("really ?", MB_OKCANCEL) != IDOK) return;

	int nItem = GetSelectedItem();
	if(nItem == -1) return;

	CTreeItem* pSelectItem = GetTreeItem(nItem);
	if(pSelectItem == NULL) return;

	CItemInfo* pItem = GetData(pSelectItem); 
	CNtlEffectSplinePath* pSplinePath = (CNtlEffectSplinePath*)pItem->m_lParam;
	NTL_ASSERTE(pSplinePath);

	CString strPathName = pSplinePath->GetName();
	if (CVenusPropertyContainer::GetInstance().DeleteProperty(PLENTITY_SPLINE, pSplinePath->GetName()))
	{
		CVenusVisualManager::GetInstance().ResetInstanceEffectFromSplinePath(strPathName.GetBuffer(0));
		DeleteItemEx(GetTreeItem(GetSelectedItem()), GetSelectedItem());
	}

	CEffectEditLayer* pView = (CEffectEditLayer*)gGetEditLayer(EDIT_LAYER_EFFECT_EDIT);
	pView->ResetPropertiesOnly();
}

void CSplinePathListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nItem = GetSelectedItem();
	if(nItem == -1)
	{
		CVenusPropertyContainer::GetInstance().SetSelectSplinePath(NULL);
		return;
	}

	CTreeItem* pSelectItem = GetTreeItem(nItem);
	if(pSelectItem == NULL)
	{
		CVenusPropertyContainer::GetInstance().SetSelectSplinePath(NULL);
		return;
	}

	CItemInfo* pItem = GetData(pSelectItem); 
	CNtlEffectSplinePath* pSplinePath = (CNtlEffectSplinePath*)pItem->m_lParam;
	NTL_ASSERTE(pSplinePath);

	CVenusPropertyContainer::GetInstance().SetSelectSplinePath(pSplinePath);

	CPathLayer* pView = (CPathLayer*)gGetEditLayer(EDIT_LAYER_PATH);
	pView->ResetProperties();

	*pResult = 0;
}

//------------------------------------------------------------------
//	FuncName	: RebuildSplinePath
//	Desc		: 
//	Parameter	: 
//	Return		: 
//------------------------------------------------------------------
void CSplinePathListCtrl::RebuildSplinePath()
{
	DeleteAllItems();
	DeleteAll();

    // Merge의 버그로 생긴 00이 들어간 Spline들을 리스트와 맵에서 제거해준다.
	CVenusPropertyContainer::svdef_Property::iterator it;
    it = CVenusPropertyContainer::GetInstance().m_svSplinePathProperty.begin();

    while(it != CVenusPropertyContainer::GetInstance().m_svSplinePathProperty.end())
    {
        CNtlEffectSplinePath* pEffectSplinePath = (CNtlEffectSplinePath*)(*it);

        CString strPathName = pEffectSplinePath->GetName();
        if(strPathName.Find("00") > 0)
        {
            // 리스트에서 제거한다.
            it = CVenusPropertyContainer::GetInstance().m_svSplinePathProperty.erase(it);            

            // Map에서 제거한다.
            CNtlPLPropertyContainer::MapProp& PropertyList = CVenusPropertyContainer::GetInstance().Gets(PLENTITY_SPLINE);            
            if (!PropertyList.empty())
            {
                CNtlPLPropertyContainer::MapProp::iterator it2 = PropertyList.find(pEffectSplinePath->GetName());
                if (it2 != PropertyList.end())
                {
                    NTL_DELETE((*it2).second);
                    PropertyList.erase(it2);                    
                }
            }
        }
        else
        {
            InsertTreeItem(pEffectSplinePath->GetName(), 0, (DWORD_PTR)pEffectSplinePath, NULL, LVIS_SELECTED | LVIS_FOCUSED);
            ++it;
        }        
    }
}

void CSplinePathListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nItem = GetSelectedItem();
	if(nItem == -1)
	{
		CVenusPropertyContainer::GetInstance().SetSelectSplinePath(NULL);
		return;
	}

	CTreeItem* pSelectItem = GetTreeItem(nItem);
	if(pSelectItem == NULL)
	{
		CVenusPropertyContainer::GetInstance().SetSelectSplinePath(NULL);
		return;
	}
	CItemInfo* pItem = GetData(pSelectItem); 
	CNtlEffectSplinePath* pSplinePath = (CNtlEffectSplinePath*)pItem->m_lParam;
	NTL_ASSERTE(pSplinePath);

	CCreateSplinePathDialog dlg;
	dlg.SetSplinePath(pSplinePath);
	if (dlg.DoModal() != IDOK) return;

	if (_stricmp(pSplinePath->GetName(), dlg.m_strName.c_str()) != 0)
	{
		CVenusPropertyContainer::GetInstance().ChangePropertyName(PLENTITY_SPLINE, pSplinePath->GetName(), dlg.m_strName.c_str());
		ResetTreeItemName(pSplinePath->GetName());
	}

	CVenusPropertyContainer::GetInstance().BuildSplinePath(pSplinePath, dlg.m_Info);

	CPathLayer* pView = (CPathLayer*)gGetEditLayer(EDIT_LAYER_PATH);
	pView->ResetProperties();

	*pResult = 0;
}

void CSplinePathListCtrl::OnEditCopy()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (CVenusPropertyContainer::GetInstance().m_pEffectSplinePath)
		CVenusPropertyContainer::GetInstance().SetCopySplinePath(TRUE, CVenusPropertyContainer::GetInstance().m_pEffectSplinePath);
}

void CSplinePathListCtrl::OnEditCut()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (CVenusPropertyContainer::GetInstance().m_pEffectSplinePath)
		CVenusPropertyContainer::GetInstance().SetCopySplinePath(FALSE, CVenusPropertyContainer::GetInstance().m_pEffectSplinePath);
}

void CSplinePathListCtrl::OnEditPaste()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	NTL_ASSERTE(CVenusPropertyContainer::GetInstance().m_pCopySplinePath != NULL);

	CString strName;
	RwInt32 count = 0;
	do {
		strName.Format("SplinePath%02d", count ++);
	} while(CVenusPropertyContainer::GetInstance().GetProperty(PLENTITY_SPLINE, strName.GetBuffer(0)));

	CNtlEffectSplinePath* pSplinePath = (CNtlEffectSplinePath*)CVenusPropertyContainer::GetInstance().CreateProperty(PLENTITY_SPLINE, strName.GetBuffer(0));
	NTL_ASSERTE(pSplinePath != NULL);

	CVenusPropertyContainer::GetInstance().BuildSplinePath(pSplinePath, CVenusPropertyContainer::GetInstance().m_pCopySplinePath->m_Info, CVenusPropertyContainer::GetInstance().m_pCopySplinePath);

	InsertTreeItem(pSplinePath->GetName(), 0, (DWORD_PTR)pSplinePath, NULL, LVIS_SELECTED | LVIS_FOCUSED);

	CEffectEditLayer* pView = (CEffectEditLayer*)gGetEditLayer(EDIT_LAYER_EFFECT_EDIT);
	pView->ResetPropertiesOnly();
}

void CSplinePathListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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
				if (CVenusPropertyContainer::GetInstance().m_pCopySplinePath != NULL)
				{
					OnEditPaste();
				}
			}
		}
		break;
	case VK_INSERT:
		{
			OnCreatePath();
		}
		break;
	case VK_DELETE:
		{
			if(GetSelectedItem() != -1)
			{
				OnDeletePath();
			}
		}
		break;
	}

	CVenusGridCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}
