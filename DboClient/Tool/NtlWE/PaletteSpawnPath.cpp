// PaletteSpawnPath.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteSpawnPath.h"
#include "PaletteSpawn.h"
#include "SpawnManager.h"
#include "PathList.h"
#include "PathPoint.h"
#include "NtlWEFrm.h"
#include "CameraAttrDlg.h"
#include "NtlWeControlUi.h"

using std::vector;
#include <vector>

struct SSortData
{
	CListCtrl*	pListCtrl;
	LPARAM		lParam;
};

static int CALLBACK ComparePathlist(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	SSortData* pSortData = (SSortData*)lParamSort;

	CString    strItem1 = pSortData->pListCtrl->GetItemText(lParam1, pSortData->lParam);
	CString    strItem2 = pSortData->pListCtrl->GetItemText(lParam2, pSortData->lParam);

	switch(pSortData->lParam)
	{
	case 0:
		if (::atoi(strItem1.GetBuffer(0)) > ::atoi(strItem2.GetBuffer(0)))
		{
			return TRUE;
		}
		return FALSE;		
	case 1:
		if (::atof(strItem1.GetBuffer(0)) > ::atof(strItem2.GetBuffer(0)))
		{
			return TRUE;
		}
		return FALSE;		
	}

	return strcmp(strItem1, strItem2);
}

// CPaletteSpawnPath 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteSpawnPath, CDialog)

CPaletteSpawnPath::CPaletteSpawnPath(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteSpawnPath::IDD, pParent)
	, m_StrPathIndex(_T("0"))
	, m_StrPathMemo(_T("@"))
	, m_StrPathRadius(_T("0.0"))
{
}

CPaletteSpawnPath::~CPaletteSpawnPath()	
{
}

void CPaletteSpawnPath::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PATH_INDEX, m_StrPathIndex);
	DDX_Text(pDX, IDC_PATH_MEMO, m_StrPathMemo);
	DDX_Control(pDX, IDC_PATHLIST, m_LcPathList);
	DDX_Control(pDX, IDC_PATHPOINTLIST, m_LcPointList);
	DDX_Text(pDX, IDC_PATH_RADIUS, m_StrPathRadius);
}


BEGIN_MESSAGE_MAP(CPaletteSpawnPath, CDialog)
	ON_BN_CLICKED(IDC_SAVE, &CPaletteSpawnPath::OnBnClickedSave)
	ON_BN_CLICKED(IDC_LOAD, &CPaletteSpawnPath::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_EXPORT, &CPaletteSpawnPath::OnBnClickedExport)
	ON_BN_CLICKED(IDC_PORTAL, &CPaletteSpawnPath::OnBnClickedPortal)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PATHLIST, &CPaletteSpawnPath::OnLvnItemchangedPathlist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PATHPOINTLIST, &CPaletteSpawnPath::OnLvnItemchangedPathpointlist)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_PATHLIST, &CPaletteSpawnPath::OnLvnColumnclickPathlist)
END_MESSAGE_MAP()


// CPaletteSpawnPath 메시지 처리기입니다.
BOOL CPaletteSpawnPath::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_LcPathList.DeleteAllItems();
	m_LcPointList.DeleteAllItems();

	m_LcPathList.InsertColumn(0, "Index", LVCFMT_CENTER, 40);
	m_LcPathList.InsertColumn(1, "Radius", LVCFMT_LEFT, 40);
	m_LcPathList.InsertColumn(2, "Memo", LVCFMT_LEFT, 40);
	m_LcPathList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_LcPointList.InsertColumn(0, "Number", LVCFMT_CENTER, 40);
	m_LcPointList.InsertColumn(1, "X", LVCFMT_CENTER, 40);
	m_LcPointList.InsertColumn(2, "Y", LVCFMT_CENTER, 40);
	m_LcPointList.InsertColumn(3, "Z", LVCFMT_CENTER, 40);
	m_LcPointList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPaletteSpawnPath::CreatePathList(CPathHandler* _PathHandler, RwInt32 SelIdx)
{
	m_LcPathList.DeleteAllItems();

	LVITEM	lvi;
	CString	Index;
	RwInt32 CurFieldIdx = m_LcPathList.GetItemCount();
	RwInt32 Count = 0;

	CPathHandler::tVECTOR_PL_ITER The = _PathHandler->GetBEGIN();
	while(The != _PathHandler->GetEND())
	{
		Index.Format("%d", (*The)->GetUID());
		lvi.mask		= LVIF_TEXT;
		lvi.iItem		= Count;
		lvi.iSubItem	= 0;
		lvi.iImage		= 0;
		lvi.pszText		= (LPSTR)(Index.GetBuffer(0));	
		m_LcPathList.SetItemData(m_LcPathList.InsertItem(&lvi), Count);

		Index.Format("%.1f", (*The)->GetRadius());
		lvi.mask		= LVIF_TEXT;
		lvi.iSubItem	= 1;
		lvi.pszText		= (LPSTR)(Index.GetBuffer(0));
		m_LcPathList.SetItem(&lvi);

		lvi.mask		= LVIF_TEXT;
		lvi.iSubItem	= 2;
		lvi.pszText		= (LPSTR)((*The)->GetMemo());
		m_LcPathList.SetItem(&lvi);

		++The;
		++Count;
	}

	if(SelIdx != -999)
	{
		CurFieldIdx = SelIdx - 1;

		m_LcPathList.SetColumnWidth(0, LVSCW_AUTOSIZE);
		m_LcPathList.SetColumnWidth(1, LVSCW_AUTOSIZE);

		m_LcPathList.SetSelectionMark(CurFieldIdx);
		m_LcPathList.SetItemState(CurFieldIdx, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_LcPathList.SetFocus();

		dGETFRM()->SetFocus();

		m_LcPathList.EnsureVisible(CurFieldIdx, FALSE);
	}

	RefreshPathList();
}

void CPaletteSpawnPath::SelectPathList(RwInt32 iIndex)
{
	for (int i = 0; i < m_LcPathList.GetItemCount(); ++i)
	{
		if (atoi(m_LcPathList.GetItemText(i, 0)) == iIndex)
		{
			m_LcPathList.SetSelectionMark(i);
			m_LcPathList.SetItemState(i, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
			m_LcPathList.SetFocus();

			dGETFRM()->SetFocus();

			m_LcPathList.EnsureVisible(i, FALSE);
		}
	}
}

void CPaletteSpawnPath::CreatePathPoint(CPathList* _pPathList, RwInt32 SelIdx)
{
	m_LcPointList.DeleteAllItems();

	if(!_pPathList)
	{
		UpdateData(FALSE);
		return;
	}

	LVITEM	lvi;
	CString Number;
	CString	PosX, PosY, PosZ;
	RwInt32 Count = 0;

	CPathList::tLIST_PP_ITER The = _pPathList->GetBEGIN();
	while(The != _pPathList->GetEND())
	{
		Number.Format("%d", Count + 1);
		PosX.Format("%.2f", (*The)->GetPosition().x);
		PosY.Format("%.2f", (*The)->GetPosition().y);
		PosZ.Format("%.2f", (*The)->GetPosition().z);

		lvi.mask		= LVIF_TEXT;
		lvi.iItem		= Count;
		lvi.iSubItem	= 0;
		lvi.iImage		= 0;		
		lvi.pszText		= (LPSTR)(Number.GetBuffer(0));
		m_LcPointList.InsertItem(&lvi);

		lvi.mask		= LVIF_TEXT;
		lvi.iSubItem	= 1;
		lvi.pszText		= (LPSTR)(PosX.GetBuffer(0));
		m_LcPointList.SetItem(&lvi);

		lvi.mask		= LVIF_TEXT;
		lvi.iSubItem	= 2;
		lvi.pszText		= (LPSTR)(PosY.GetBuffer(0));
		m_LcPointList.SetItem(&lvi);

		lvi.mask		= LVIF_TEXT;
		lvi.iSubItem	= 3;
		lvi.pszText		= (LPSTR)(PosZ.GetBuffer(0));
		m_LcPointList.SetItem(&lvi);

		++The;
		++Count;
	}

	if(SelIdx == -999)
	{
		m_LcPointList.SetColumnWidth(0, LVSCW_AUTOSIZE);
		m_LcPointList.SetColumnWidth(1, LVSCW_AUTOSIZE);
		m_LcPointList.SetColumnWidth(2, LVSCW_AUTOSIZE);
		m_LcPointList.SetColumnWidth(3, LVSCW_AUTOSIZE);

		m_LcPointList.SetSelectionMark(SelIdx - 1);
		m_LcPointList.SetItemState(SelIdx - 1, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
		m_LcPointList.SetFocus();

		dGETFRM()->SetFocus();

		m_LcPointList.EnsureVisible(SelIdx - 1, FALSE);
	}
	
	RefreshPathPoint();
}

LRESULT CPaletteSpawnPath::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SHOWWINDOW:
		{
			break;
		}
	case WM_CLOSE:
		{
			dGETFRM()->m_pSpawnPalette->ShowWindow(SW_SHOW);
			dGETFRM()->m_pSpawnPalette->m_PathDialogOpend = FALSE;
			dGETFRM()->m_pSpawnPalette->ShowWindow(SW_SHOW);
			dGETFRM()->m_pSpawnPalette->m_NavigatorDialogOpend = FALSE;
			GetSpawnInstance()->SetPathModeActive(FALSE);
			GetSpawnInstance()->SetNavigatorModeActive(FALSE);
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CPaletteSpawnPath::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{
		switch(pMsg->wParam)
		{
		case VK_DELETE:
			{
				CWnd *pWnd			= this->GetFocus(); 
				CWnd *pPathListWnd	= GetDlgItem(IDC_PATHLIST);
				CWnd *pPathPointWnd	= GetDlgItem(IDC_PATHPOINTLIST);

				if(pWnd == pPathListWnd)
				{
					GetSpawnInstance()->GetPathHandler()->OnDeletePathList();
					ClearEditBox();
				}
				else if(pWnd == pPathPointWnd)
				{
					RwUInt32				SelCount		= m_LcPointList.GetSelectedCount();
					RwInt32					FirstItemSymbol	= -1;
					std::vector<RwInt32>	SelIndices;

					if(SelCount > 0)
					{
						for(RwUInt32 i = 0; i < SelCount; i++)
						{
							FirstItemSymbol = m_LcPointList.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
							if(FirstItemSymbol != -1)
							{
								SelIndices.push_back(FirstItemSymbol);

							}
						}
					
						GetSpawnInstance()->GetPathHandler()->OnDeletePathPoints(SelIndices);
					}

					SelIndices.clear();
					ClearEditBox();
				}

				return true;
			}
		case VK_ESCAPE:
			{
				return true;
			}
		case VK_RETURN:
			{
				CWnd *pWnd			= this->GetFocus();
				CWnd *pStrRadiusWnd	= GetDlgItem(IDC_PATH_RADIUS);
				CWnd *pStrMemoWnd	= GetDlgItem(IDC_PATH_MEMO);
				
				if (pWnd == pStrRadiusWnd)
				{
					GetSpawnInstance()->GetPathHandler()->OnChangedRadius();
				}
				else if (pWnd == pStrMemoWnd)
				{
					GetSpawnInstance()->GetPathHandler()->OnChangedMemo();
				}
				return true;
			}
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

VOID CPaletteSpawnPath::RefreshPathList()
{
	SSortData sSortData;
	sSortData.pListCtrl = &m_LcPathList;
	sSortData.lParam	= 0;
	m_LcPathList.SortItems(ComparePathlist, (LPARAM)(&sSortData));
	UpdateData(FALSE);
}

VOID CPaletteSpawnPath::RefreshPathPoint()
{
	UpdateData(FALSE);
}

VOID CPaletteSpawnPath::ClearEditBox()
{
	m_StrPathIndex	= _T("0");
	m_StrPathMemo	= _T("@");
	m_StrPathRadius	= _T("0.0");
	UpdateData(FALSE);
}

void CPaletteSpawnPath::OnBnClickedSave()
{
	GetSpawnInstance()->GetPathHandler()->OnSave();
}

void CPaletteSpawnPath::OnBnClickedLoad()
{
	ClearEditBox();
	GetSpawnInstance()->GetPathHandler()->OnLoad();
}

void CPaletteSpawnPath::OnBnClickedExport()
{
	GetSpawnInstance()->GetPathHandler()->OnExport();
}

void CPaletteSpawnPath::OnBnClickedPortal()
{
	RwUInt32 SelCount = m_LcPointList.GetSelectedCount();

	if(SelCount > 1)
	{
		AfxMessageBox("can't portal to multi path points please choose one path point.");
		return;
	}

	if(!SelCount)
	{
		AfxMessageBox("please choose a path point.");
		return;
	}

	RwInt32 SelIndex = m_LcPointList.GetSelectionMark();
	GetSpawnInstance()->GetPathHandler()->OnPortalTo(SelIndex);

	// force the camera to be free
	CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetNtlGameCamera());
	dGETFRM()->m_pCameraPortal->SetCurCamMode(0);
}

RwInt32 CPaletteSpawnPath::GetCurPathListIndex()
{ 
	RwInt32 SelIndex = m_LcPathList.GetSelectionMark();
	
	if(SelIndex >= 0)
	{ 
		return SelIndex;
	}

	return -1;
}

void CPaletteSpawnPath::OnLvnItemchangedPathlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if(pNMLV && (pNMLV->uNewState == (UINT)(LVIS_FOCUSED | LVIS_SELECTED)))
	{
		//RwInt32 SelIndex = m_LcPathList.GetSelectionMark();
		RwInt32 SelIndex = pNMLV->lParam;
		if(SelIndex >= 0)
		{
			if(GetSpawnInstance()->GetPathHandler()->GetCurPathList())
			{
				GetSpawnInstance()->GetPathHandler()->OnCancelCurPathList();
			}

			GetSpawnInstance()->GetPathHandler()->SetCurSelPathList(SelIndex);

			CPathHandler::tVECTOR_PL_ITER The = GetSpawnInstance()->GetPathHandler()->GetBEGIN();
			for(RwInt32 i = 0; i < SelIndex; ++i)
			{
				++The;
			}

			m_StrPathIndex.Format("%d", (*The)->GetUID());
			m_StrPathRadius.Format("%.1f", (*The)->GetRadius());
			m_StrPathMemo = (*The)->GetMemo();
			CreatePathPoint(*The, (*The)->GetPathPointSize());

			GetSpawnInstance()->GetPathHandler()->OnClearCurVertex();
		}
	}

	*pResult = 0;
}
void CPaletteSpawnPath::OnLvnItemchangedPathpointlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(pNMLV && (pNMLV->uNewState == (UINT)(LVIS_FOCUSED | LVIS_SELECTED)))
	{
		//RwInt32 SelIndex = m_LcPathList.GetSelectionMark();
		RwInt32 SelIndex = pNMLV->iItem;
		if(SelIndex >= 0)
		{
			GetSpawnInstance()->GetPathHandler()->OnClearCurVertex();
		}
	}

	*pResult = 0;
}

void CPaletteSpawnPath::OnLvnColumnclickPathlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	for (int i = 0; i < m_LcPathList.GetItemCount(); ++i)
	{
		m_LcPathList.SetItemData(i, i);
	}

	SSortData sSortData;
	sSortData.pListCtrl = &m_LcPathList;
	sSortData.lParam	= pNMLV->iSubItem;
	m_LcPathList.SortItems(ComparePathlist, (LPARAM)(&sSortData));
	
	*pResult = 0;
}
