// BookmarkDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "NtlWEUtil.h"
#include "BookmarkDlg.h"
#include "NtlPLWorldEntity.h"
#include "NtlPLApi.h"


// CBookmarkDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBookmarkDlg, CDialog)

CBookmarkDlg::CBookmarkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBookmarkDlg::IDD, pParent)
	, m_strMemoIO(_T(""))
{
	m_pBookmarker	= NULL;
	m_hBitmap		= NULL;

	for(RwInt32 i = 0; i < eBL_TOT_NUM; i++)
	{
		m_AscendingSortFlags[i] = FALSE;
	}
}

CBookmarkDlg::~CBookmarkDlg()
{
	NTL_DELETE(m_pBookmarker);

	if(m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
	}
}

void CBookmarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPAWN_LIST, m_lcBookmark);
	DDX_Text(pDX, IDC_PATH_MEMO, m_strMemoIO);
	DDX_Control(pDX, IDC_PREVIEW, m_Preview);
}


BEGIN_MESSAGE_MAP(CBookmarkDlg, CDialog)
	ON_BN_CLICKED(IDC_INSERT, &CBookmarkDlg::OnBnClickedInsert)
	ON_BN_CLICKED(IDC_DELETE, &CBookmarkDlg::OnBnClickedDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_SPAWN_LIST, &CBookmarkDlg::OnLvnItemchangedBookmarkDlg)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_SPAWN_LIST, &CBookmarkDlg::OnLvnColumnclickBookmarkDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_SPAWN_LIST, &CBookmarkDlg::OnNMDblclkSpawnList)
	ON_NOTIFY(NM_CLICK, IDC_SPAWN_LIST, &CBookmarkDlg::OnNMClickSpawnList)
END_MESSAGE_MAP()


// CBookmarkDlg 메시지 처리기입니다.
BOOL CBookmarkDlg::PreTranslateMessage(MSG* pMsg)
{
	/*
	UpdateData(TRUE);
	
		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_PORTALX)->m_hWnd && pMsg->wParam	== VK_RETURN)
		{
	
		}
	
		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_PORTALY)->m_hWnd && pMsg->wParam	== VK_RETURN)
		{
	
		}
	
		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_PORTALZ)->m_hWnd && pMsg->wParam	== VK_RETURN)
		{
	
		}
	
		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_CAM_FOV)->m_hWnd && pMsg->wParam	== VK_RETURN)
		{
			UpdateData(TRUE);
	
			CNtlWeControlUi::GetInstance().SetFov(static_cast<RwReal>(::atof(m_FOV.GetBuffer(0))));
		}
	
		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_CAM_DIST)->m_hWnd && pMsg->wParam == VK_RETURN)
		{
			UpdateData(TRUE);
	
			float Temp = static_cast<float>(::atof(m_DistFromAvatar));
			CLAMP(Temp, 0.1f, (dGET_WORLD_PARAM()->WorldSectorSize / 2.0f));
			m_DistFromAvatar.Format("%.2f", Temp);
	
			CNtlWeControlUi::GetInstance().SetZoomDist(static_cast<RwReal>(::atof(m_DistFromAvatar.GetBuffer(0))));
		}
	
		if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_CAM_VELOCITY)->m_hWnd && pMsg->wParam == VK_RETURN)
		{
			UpdateData(TRUE);
	
			float Temp = static_cast<float>(::atof(m_CamVelocity));
			CLAMP(Temp, 0.1f, 200.0f);
	
			CNtlWeControlUi::GetInstance().SetSpeed(Temp);
			dGET_WORLD_PARAM()->WorldCamSpeedPerSec = Temp;
		}
	
		UpdateData(FALSE);*/
	
	if(pMsg->message == WM_KEYDOWN && pMsg->hwnd == GetDlgItem(IDC_PATH_MEMO)->m_hWnd && pMsg->wParam == VK_RETURN)
	{
		if(m_lcBookmark.GetSelectedCount() != 1)
		{
			AfxMessageBox("You should select only one record.");
			return true;
		}

		UpdateData(TRUE);

		int			FirstItemSymbol = m_lcBookmark.GetNextItem(-1, LVNI_SELECTED);
		CString		StrIndex		= m_lcBookmark.GetItemText(FirstItemSymbol, eBL_INDEX);

		m_pBookmarker->GetBM(atoi((LPSTR)(StrIndex.GetBuffer(0))))->m_Memo = m_strMemoIO;

		OnUpateList();
	}

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

BOOL CBookmarkDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,	SWP_NOMOVE | SWP_NOSIZE);

	if(!m_pBookmarker)
		m_pBookmarker = NTL_NEW CBookmarker;

	m_lcBookmark.DeleteAllItems();
	m_lcBookmark.InsertColumn(eBL_INDEX, "Index", LVCFMT_CENTER, 40);
	m_lcBookmark.InsertColumn(eBL_USER, "User", LVCFMT_CENTER, 80);
	m_lcBookmark.InsertColumn(eBL_POS, "Pos : X, Y, Z", LVCFMT_CENTER, 80);
	m_lcBookmark.InsertColumn(eBL_DIR, "Dir : X, Y, Z", LVCFMT_CENTER, 80);
	m_lcBookmark.InsertColumn(eBL_DATE, "Date", LVCFMT_CENTER, 80);
	m_lcBookmark.InsertColumn(eBL_MEMO, "Memo", LVCFMT_LEFT, 256);
	m_lcBookmark.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_strMemoIO = _T("");

	m_Preview.ModifyStyle(0, WS_CHILD|WS_VISIBLE | SS_BITMAP | SS_CENTERIMAGE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CBookmarkDlg::OnBnClickedInsert()
{
	UpdateData(TRUE);

	CBookmark bm;

	bm.m_Idx	= m_lcBookmark.GetItemCount();
	bm.m_User	= m_pBookmarker->GetEndUserName();

	RwV3d CurPos = *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	bm.m_Pos = CurPos;

	RwV3d CurFront = *RwMatrixGetAt(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwV3dNormalize(&CurFront, &CurFront);
	bm.m_Dir = CurFront;

	RwV3d CurRight = *RwMatrixGetRight(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwV3dNormalize(&CurRight, &CurRight);
	bm.m_Right = CurRight;

	RwV3d CurUp = *RwMatrixGetUp(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	RwV3dNormalize(&CurUp, &CurUp);
	bm.m_Up = CurUp;


	struct tm*	tm_ptr;
	time_t		the_time;
	RwChar		Tmp[_MAX_PATH] = {0,};
	time(&the_time);
	tm_ptr = localtime(&the_time);
	strftime(Tmp, _MAX_PATH, "%Y%m%d_%a_%H_%M_%S_%p", tm_ptr);

	CString strPreviewExt = ".bmp";

	bm.m_Date		= Tmp;
	bm.m_Memo		= m_strMemoIO;
	bm.m_Preview	= Tmp + strPreviewExt;

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	_chdir("tool");
	_chdir("saber");
	_chdir("bookmark");
	_chdir("images");

	// screen capture
	RwCamera *pCamera = CNtlPLGlobal::m_RwCamera;
	if(pCamera && strlen(bm.m_Preview) > 0)
	{
		RwRaster *pRaster = RwCameraGetRaster(pCamera);		
		API_PL_RasterSaveImage(pRaster, bm.m_Preview, 320, 240);
	}

	m_pBookmarker->AddMark(bm);

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	OnUpateList();
}

void CBookmarkDlg::OnBnClickedDelete()
{
	RwUInt32	SelCount		= m_lcBookmark.GetSelectedCount();
	RwInt32		FirstItemSymbol	= -1;

	if(SelCount > 0)
	{
		for(RwUInt32 i = 0; i < SelCount; i++)
		{
			FirstItemSymbol = m_lcBookmark.GetNextItem(FirstItemSymbol, LVNI_SELECTED);
			if(FirstItemSymbol != -1)
			{
				CString Index = m_lcBookmark.GetItemText(FirstItemSymbol, 0);

				CBookmark* pBM = m_pBookmarker->GetBM(atoi((LPSTR)(Index.GetBuffer(0))));
				if(!pBM->m_User.Compare(m_pBookmarker->GetEndUserName()))
				{
					_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
					_chdir("tool");
					_chdir("saber");
					_chdir("bookmark");
					_chdir("images");

					NtlFileAttrReadOnlyRelease(pBM->m_Preview);
					::DeleteFile(pBM->m_Preview);

					_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
				}

				m_pBookmarker->RemoveMark(atoi((LPSTR)(Index.GetBuffer(0))));
			}
		}

		m_pBookmarker->RegenerateIndices();
	}

	OnUpateList();
}

VOID CBookmarkDlg::InsertNode(RwInt32 _Index, CString& _User, RwV3d& _Pos, RwV3d& _Dir, CString& _Date, CString& _Memo)
{
	CString strRecord[eBL_TOT_NUM];
	strRecord[eBL_INDEX].Format("%d", _Index);
	strRecord[eBL_USER] = _User;
	strRecord[eBL_POS].Format("%.2f, %.2f, %.2f", _Pos.x, _Pos.y, _Pos.z);
	strRecord[eBL_DIR].Format("%.2f, %.2f, %.2f", _Dir.x, _Dir.y, _Dir.z);
	strRecord[eBL_DATE] = _Date;
	strRecord[eBL_MEMO] = _Memo;

	LVITEM lvi;
	lvi.mask	= LVIF_TEXT;
	lvi.iItem	= m_lcBookmark.GetItemCount();
	lvi.iImage	= 0;

	for(int i = 0; i < eBL_TOT_NUM; ++i)
	{		
		lvi.iSubItem	= i;
		lvi.pszText		= (LPSTR)(strRecord[i].GetBuffer(0));

		if(i == eBL_INDEX)
			m_lcBookmark.InsertItem(&lvi);
		else
			m_lcBookmark.SetItem(&lvi);
	}

	for(int i = 0; i < eBL_TOT_NUM; ++i)
		m_lcBookmark.SetColumnWidth(i, LVSCW_AUTOSIZE);
}

VOID CBookmarkDlg::OnUpateList()
{
	m_lcBookmark.DeleteAllItems();

	CBookmarker::tVECTOR_BM_ITER The = m_pBookmarker->GetBmBIter();
	while(The != m_pBookmarker->GetBmEIter())
	{
		CBookmark CurBm = *The;
		InsertNode(CurBm.m_Idx, CurBm.m_User, CurBm.m_Pos, CurBm.m_Dir, CurBm.m_Date, CurBm.m_Memo);

		++The;
	}

	UpdateData(FALSE);
}

// int CALLBACK StringCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
// {
// 	CSortData*	pLc1	= (CSortData*)lParam1;
// 	CSortData*	pLc2	= (CSortData*)lParam2;
// 	RwInt32		nReturn	= pLc1->m_strText.Compare(pLc2->m_strText);
// 
// 	if(lParamSort)
// 	{
// 		return nReturn;
// 	}
// 	else
// 	{
// 		return -nReturn;
// 	}
// }
// 
// int CALLBACK NumberCompare(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
// {
// 	CSortData*	pLc1	= (CSortData*)lParam1;
// 	CSortData*	pLc2	= (CSortData*)lParam2;
// 	RwInt32		LC1Val	= static_cast<RwInt32>(atof(pLc1->m_strText.GetBuffer(0)));
// 	RwInt32		LC2Val	= static_cast<RwInt32>(atof(pLc2->m_strText.GetBuffer(0)));
// 	RwInt32		nReturn	= LC1Val < LC2Val ? -1 : 1;
// 
// 	if(lParamSort)
// 	{
// 		return nReturn;
// 	}
// 	else
// 	{
// 		return -nReturn;
// 	}
// }

RwBool CBookmarkDlg::OnSortData(RwInt32 _Index)
{
	CListCtrl*	pLc			= (CListCtrl*)GetDlgItem(IDC_SPAWN_LIST);
	RwInt32		totItems	= pLc->GetItemCount();

	// Callback Item Setting
	for(RwInt32 i = 0; i < totItems; i++)
	{
		DWORD	dw	= pLc->GetItemData(i);
		CString txt = pLc->GetItemText(i, _Index);

		pLc->SetItemData(i, (DWORD)(new CSortData(txt, dw)));
	}

	//if(_Index == eBL_USER || _Index == eBL_DATE)
	pLc->SortItems(StringCompare, m_AscendingSortFlags[_Index]);

	for(RwInt32 i = 0; i < totItems; ++i)
	{
		CSortData* pItem = (CSortData*)(pLc->GetItemData(i));
		ASSERT(pItem);

		pLc->SetItemData(i, pItem->m_dw);

		delete pItem;
	}

	return TRUE;
}

void CBookmarkDlg::OnLvnItemchangedBookmarkDlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
 	if(pNMLV && (pNMLV->uNewState == (UINT)(LVIS_FOCUSED | LVIS_SELECTED)))
 	{
		RwUInt32 SelCount = m_lcBookmark.GetSelectedCount();
		if(SelCount == 1)
		{
			int FirstItemSymbol = m_lcBookmark.GetNextItem(-1, LVNI_SELECTED);
			m_strMemoIO = m_lcBookmark.GetItemText(FirstItemSymbol, eBL_MEMO);

			UpdateData(FALSE);
		}
 	}

	*pResult = 0;
}

void CBookmarkDlg::OnLvnColumnclickBookmarkDlg(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
 	if(!OnSortData(pNMLV->iSubItem))
 		return;
 
 	m_AscendingSortFlags[pNMLV->iSubItem] = !m_AscendingSortFlags[pNMLV->iSubItem];

	*pResult = 0;
}

void CBookmarkDlg::OnNMClickSpawnList(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(m_lcBookmark.GetSelectedCount() != 1)
	{
		return;
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	_chdir("tool");
	_chdir("saber");
	_chdir("bookmark");
	_chdir("images");

	int			FirstItemSymbol = m_lcBookmark.GetNextItem(-1, LVNI_SELECTED);
	CString		StrIndex		= m_lcBookmark.GetItemText(FirstItemSymbol, eBL_INDEX);
	CBookmark*	pBM				= m_pBookmarker->GetBM(atoi((LPSTR)(StrIndex.GetBuffer(0))));
	CString		strPreview		= pBM->m_Preview;

	if(m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
	}

	m_hBitmap = (HBITMAP)::LoadImage(NULL, strPreview.GetBuffer(0), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if(m_hBitmap)
	{
		m_Preview.SetBitmap(m_hBitmap);
	}
	else
	{
		DBO_TRACE(FALSE, "CBookmarkDlg::OnNMClickSpawnList, texture load failed.");
	}		

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	*pResult = 0;
}

void CBookmarkDlg::OnNMDblclkSpawnList(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(m_lcBookmark.GetSelectedCount() != 1)
	{
		AfxMessageBox("You should select only one record.");
		return;
	}

	int			FirstItemSymbol = m_lcBookmark.GetNextItem(-1, LVNI_SELECTED);
	CString		StrIndex		= m_lcBookmark.GetItemText(FirstItemSymbol, eBL_INDEX);
	CBookmark*	pBM				= m_pBookmarker->GetBM(atoi((LPSTR)(StrIndex.GetBuffer(0))));

	if(pBM)
	{
		RwV3d Tmp	= pBM->m_Pos;
		RwV3d Dir	= pBM->m_Dir;
		RwV3d Up	= pBM->m_Up;
		RwV3d Right	= pBM->m_Right;

		RwMatrix matEffect;
		RwMatrixSetIdentity(&matEffect);

		*RwMatrixGetAt(&matEffect) = Dir;
		*RwMatrixGetUp(&matEffect) = Up;
		*RwMatrixGetRight(&matEffect) = Right;
		*RwMatrixGetPos(&matEffect) = Tmp;
		RwMatrixUpdate(&matEffect);
		RwMatrixCopy(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)), &matEffect);
		RwFrameUpdateObjects(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera));

		GetSceneManager()->GetWorld()->SetPortalPosition(Tmp, FALSE);
	}
	else
	{
		AfxMessageBox("You should select the records.");
		return;
	}

	*pResult = 0;
}

VOID CBookmarkDlg::Save()
{
	m_pBookmarker->Save();
}

VOID CBookmarkDlg::RefreshWindow()
{
	OnUpateList();
}
