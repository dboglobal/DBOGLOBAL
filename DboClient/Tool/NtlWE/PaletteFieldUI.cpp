// PaletteFieldUI.cpp : implementation file
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteFieldUI.h"
#include "FieldUIManager.h"
#include "MergeUIManager.h"
#include "NtlWorldPaletteDefine.h"
#include "NtlWorldMergeManager.h"
#include "PaletteSkyFloatingEntitydlg.h"
#include "NtlWEUtil.h"

#include "NtlWEDoc.h"


// CPaletteFieldUI dialog

IMPLEMENT_DYNAMIC(CPaletteFieldUI, CDialog)

CPaletteFieldUI::CPaletteFieldUI(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteFieldUI::IDD, pParent)
	, m_MergeWorldPathName(_T(""))
	, m_MergeSpawnDstName(_T(""))
	, m_MergeSpawnSrcName(_T(""))
{
	//m_pPaletteSkyFloatingEntitydlg = CreateModalessDialogs<CPaletteSkyFloatingEntityDlg>(GetDesktopWindow());
}

CPaletteFieldUI::~CPaletteFieldUI()
{
	//CNtlWEUtil::DestroyModalessDialogs(m_pPaletteSkyFloatingEntitydlg);
}

void CPaletteFieldUI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOAD_FIELDMAP, m_LoadMinimapBtn);
	DDX_Control(pDX, IDC_FIELD_VISIBLE, m_FieldShowBtn);
	DDX_Control(pDX, IDC_MERGE_VISIBLE, m_MergeBtn);
	DDX_Control(pDX, IDC_LOAD_MERGEMAP, m_LoadMergeMinimapBtn);
	DDX_Control(pDX, IDC_MERGE_HEIGHTFIELD, m_M0);
	DDX_Control(pDX, IDC_MERGE_HEIGHTFIELD2, m_M1);
	DDX_Control(pDX, IDC_MERGE_HEIGHTFIELD3, m_M2);
	DDX_Control(pDX, IDC_MERGE_HEIGHTFIELD4, m_M3);
	DDX_Control(pDX, IDC_MERGE_HEIGHTFIELD5, m_M4);
	DDX_Control(pDX, IDC_MERGE_HEIGHTFIELD6, m_MergeAll);
	DDX_Control(pDX, IDC_MERGE_WORLD_NAME_EDIT, m_MergeWorld);
	DDX_Text(pDX, IDC_MERGE_WORLD_NAME_EDIT, m_MergeWorldPathName);
	DDX_Control(pDX, IDC_MERGE_MAPNAME, m_M5);
	DDX_Control(pDX, IDC_MERGE_BLOOM, m_M6);
	DDX_Control(pDX, IDC_SKY_PALETTE, m_SkyPaletteShow);
	DDX_Text(pDX, IDC_MERGE_SPAWN_FILENAME_DST, m_MergeSpawnDstName);
	DDX_Text(pDX, IDC_MERGE_SPAWN_FILENAME_SRC, m_MergeSpawnSrcName);
	DDX_Control(pDX, IDC_MERGE_SPAWN, m_MergeSpawn);
	DDX_Control(pDX, IDC_MERGE_SPAWN_PATH, m_MergeSpawnPath);
}

void CPaletteFieldUI::InitializeWorldData()
{

}

BEGIN_MESSAGE_MAP(CPaletteFieldUI, CDialog)
	ON_BN_CLICKED(IDC_LOAD_FIELDMAP, &CPaletteFieldUI::OnBnClickedLoadFieldmap)
	ON_BN_CLICKED(IDC_LOAD_MERGEMAP, &CPaletteFieldUI::OnBnClickedLoadMergemap)
	ON_BN_CLICKED(IDC_FIELD_VISIBLE, &CPaletteFieldUI::OnBnClickedFieldVisible)
	ON_BN_CLICKED(IDC_MERGE_VISIBLE, &CPaletteFieldUI::OnBnClickedMergeVisible)
	ON_BN_CLICKED(IDC_LINK_WORLD, &CPaletteFieldUI::OnBnClickedLinkWorld)
	ON_BN_CLICKED(IDC_MERGE_HEIGHTFIELD6, &CPaletteFieldUI::OnBnClickedMergeHeightfield6)
	ON_BN_CLICKED(IDC_MERGE_HEIGHTFIELD, &CPaletteFieldUI::OnBnClickedMergeHeightfield)
	ON_BN_CLICKED(IDC_MERGE_HEIGHTFIELD2, &CPaletteFieldUI::OnBnClickedMergeHeightfield2)
	ON_BN_CLICKED(IDC_MERGE_HEIGHTFIELD3, &CPaletteFieldUI::OnBnClickedMergeHeightfield3)
	ON_BN_CLICKED(IDC_MERGE_HEIGHTFIELD4, &CPaletteFieldUI::OnBnClickedMergeHeightfield4)
	ON_BN_CLICKED(IDC_MERGE_HEIGHTFIELD5, &CPaletteFieldUI::OnBnClickedMergeHeightfield5)
	ON_BN_CLICKED(IDC_MERGE_MAPNAME, &CPaletteFieldUI::OnBnClickedMergeMapname)
	ON_BN_CLICKED(IDC_MERGE_BLOOM, &CPaletteFieldUI::OnBnClickedMergeBloom)
	ON_BN_CLICKED(IDC_SKY_PALETTE, &CPaletteFieldUI::OnBnClickedSkyPalette)
	ON_BN_CLICKED(IDC_BTN_MERGE_SPAWN_DST, &CPaletteFieldUI::OnBnClickedBtnMergeSpawnDst)
	ON_BN_CLICKED(IDC_BTN_MERGE_SPAWN_SRC, &CPaletteFieldUI::OnBnClickedBtnMergeSpawnSrc)
END_MESSAGE_MAP()


// CPaletteFieldUI message handlers

BOOL CPaletteFieldUI::PreTranslateMessage(MSG* pMsg)
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

void CPaletteFieldUI::OnBnClickedLoadFieldmap()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.dds; *.bmp; *.tga; *.jpg",
						OFN_HIDEREADONLY,
						"Data Files (*.dds;*.bmp;*.tga;*.jpg)|*.dds;*.bmp;*.tga;*.jpg||");

	FileDlg.m_ofn.lpstrInitialDir = "tool\\saber\\texture\\Grid Map";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = FALSE;
		return;
	}
	
	dGETHDL()->m_pFieldUIManager->LoadFieldTexture(FileDlg.GetFileTitle().GetBuffer());

	::SetCurrentDirectory(dirBackup);

	m_LoadMinimapBtn.SetWindowText(FileDlg.GetFileTitle().GetBuffer());

	UpdateData(FALSE);

	Floating = false;
}

void CPaletteFieldUI::OnBnClickedLoadMergemap()
{
	static bool Floating = false;

	if(Floating)
		return;

	Floating = true;

	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE,
						"register detail texture layer",
						"*.dds; *.bmp; *.tga; *.jpg",
						OFN_HIDEREADONLY,
						"Data Files (*.dds;*.bmp;*.tga;*.jpg)|*.dds;*.bmp;*.tga;*.jpg||");

	FileDlg.m_ofn.lpstrInitialDir = "tool\\saber\\texture\\Grid Map";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = FALSE;
		return;
	}

	dGETHDL()->m_pMergeUIManager->LoadFieldTexture(FileDlg.GetFileTitle().GetBuffer());

	::SetCurrentDirectory(dirBackup);

	m_LoadMergeMinimapBtn.SetWindowText(FileDlg.GetFileTitle().GetBuffer());

	UpdateData(FALSE);

	Floating = false;
}

void CPaletteFieldUI::OnBnClickedFieldVisible()
{
	if(m_FieldShowBtn.GetCheck())
	{
		dGETHDL()->m_pFieldUIManager->Switch(TRUE);
	}
	else
	{
		dGETHDL()->m_pFieldUIManager->Switch(FALSE);
	}
}

void CPaletteFieldUI::OnBnClickedMergeVisible()
{
	if(m_MergeBtn.GetCheck())
	{
		dGETHDL()->m_pMergeUIManager->Switch(TRUE);
	}
	else
	{
		dGETHDL()->m_pMergeUIManager->Switch(FALSE);
	}
}

BOOL CPaletteFieldUI::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_FieldShowBtn.SetCheck(TRUE);
	m_MergeBtn.SetCheck(FALSE);

	m_MergeWorldPathName.Format("null");
	m_MergeSpawnSrcName.Format("null");
	m_MergeSpawnDstName.Format("null");

	m_SkyPaletteShow.SetCheck(FALSE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CPaletteFieldUI::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		{
			dGET_CUR_PALETTE() = ePM_DEFAULT;
			dGETHDL()->m_PlanetIOHandler.DestroyAll();
			((CNtlWEFrm*)AfxGetMainWnd())->m_pSkyFloatingDlg->Reset();
			((CNtlWEFrm*)AfxGetMainWnd())->m_pSkyFloatingDlg->ShowWindow(SW_HIDE);

			break;
		}

	case WM_SHOWWINDOW:
		{
			if(m_SkyPaletteShow.GetCheck())
			{
				((CNtlWEFrm*)AfxGetMainWnd())->m_pSkyFloatingDlg->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
				((CNtlWEFrm*)AfxGetMainWnd())->m_pSkyFloatingDlg->ShowWindow(SW_SHOW);
			}

			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CPaletteFieldUI::OnBnClickedLinkWorld()
{
	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	// set dir. info
	char pszPathname[NTL_MAX_DIR_PATH];
	ITEMIDLIST *pidlBrowse;

	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "find a merge project folder";
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	CString strResult = "";
	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);

		// check this folder'got already ntl files
		char CurDir[NTL_MAX_DIR_PATH] = {0,};
		GetCurrentDirectory(NTL_MAX_DIR_PATH, CurDir);
		SetCurrentDirectory(pszPathname);
		if((_access("#######.gwp", 0)) == -1)
		{
			::AfxMessageBox("this folder hasn't got ntl world files, please try another one", MB_OK);
			return;
		}
		else
		{
			if(CNtlWorldMergeManager::GetInstance()->Load(pszPathname))
			{
				m_MergeWorldPathName.Format("%s", pszPathname);
			}
			else
			{
				m_MergeWorldPathName.Format("null");
			}

			UpdateData(FALSE);
		}

		SetCurrentDirectory(CurDir);
	}
	else
		return;

	::SetCurrentDirectory(dirBackup);
}

RwBool CPaletteFieldUI::GetWorldMergeStateNothing()
{
	if(	!m_M0.GetCheck() &&
		!m_M1.GetCheck() &&
		!m_M2.GetCheck() &&
		!m_M3.GetCheck() &&
		!m_M4.GetCheck() &&
		!m_M5.GetCheck() &&
		!m_M6.GetCheck())
	{
		return TRUE;
	}
	return FALSE;
}

RwBool CPaletteFieldUI::GetSpawnMergeStateNothing()
{
	if( !m_MergeSpawn.GetCheck() && !m_MergeSpawnPath.GetCheck())
	{
		return TRUE;
	}
	return FALSE;
}

void CPaletteFieldUI::OnBnClickedMergeHeightfield6()
{
	if(m_MergeAll.GetCheck())
	{
		m_M0.SetCheck(TRUE);
		m_M1.SetCheck(TRUE);
		m_M2.SetCheck(TRUE);
		m_M3.SetCheck(TRUE);
		m_M4.SetCheck(TRUE);
		m_M5.SetCheck(TRUE);
		m_M6.SetCheck(TRUE);

		m_M0.EnableWindow(FALSE);
		m_M1.EnableWindow(FALSE);
		m_M2.EnableWindow(FALSE);
		m_M3.EnableWindow(FALSE);
		m_M4.EnableWindow(FALSE);
		m_M5.EnableWindow(FALSE);
		m_M6.EnableWindow(FALSE);
	}
	else
	{
		m_M0.EnableWindow(TRUE);
		m_M1.EnableWindow(TRUE);
		m_M2.EnableWindow(TRUE);
		m_M3.EnableWindow(TRUE);
		m_M4.EnableWindow(TRUE);
		m_M5.EnableWindow(TRUE);
		m_M6.EnableWindow(TRUE);

		m_M0.SetCheck(FALSE);
		m_M1.SetCheck(FALSE);
		m_M2.SetCheck(FALSE);
		m_M3.SetCheck(FALSE);
		m_M4.SetCheck(FALSE);
		m_M5.SetCheck(FALSE);
		m_M6.SetCheck(FALSE);
	}

	UpdateData(FALSE);
}

VOID CPaletteFieldUI::UpdateMergeAll()
{
	if(	m_M0.GetCheck() &&
		m_M1.GetCheck() &&
		m_M2.GetCheck() &&
		m_M3.GetCheck() &&
		m_M4.GetCheck() &&
		m_M5.GetCheck() &&
		m_M6.GetCheck())
	{
		m_MergeAll.SetCheck(TRUE);
		UpdateData(FALSE);

		OnBnClickedMergeHeightfield6();
	}
}

void CPaletteFieldUI::OnBnClickedMergeHeightfield()
{
	UpdateMergeAll();
}

void CPaletteFieldUI::OnBnClickedMergeHeightfield2()
{
	UpdateMergeAll();
}

void CPaletteFieldUI::OnBnClickedMergeHeightfield3()
{
	UpdateMergeAll();
}

void CPaletteFieldUI::OnBnClickedMergeHeightfield4()
{
	UpdateMergeAll();
}

void CPaletteFieldUI::OnBnClickedMergeHeightfield5()
{
	UpdateMergeAll();
}

void CPaletteFieldUI::OnBnClickedMergeMapname()
{
	UpdateMergeAll();
}

void CPaletteFieldUI::OnBnClickedMergeBloom()
{
	UpdateMergeAll();
}

VOID CPaletteFieldUI::SetCheckSkyPaletteShow(RwBool _ShowFlag)
{
	m_SkyPaletteShow.SetCheck(_ShowFlag);
	UpdateData(FALSE);
}

void CPaletteFieldUI::OnBnClickedSkyPalette()
{
	if(m_SkyPaletteShow.GetCheck())
	{
		dGETDOC()->SetFieldPropMode(EFIELDPROP_MODE_OUTDOOR);
		((CNtlWEFrm*)AfxGetMainWnd())->m_pSkyFloatingDlg->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		((CNtlWEFrm*)AfxGetMainWnd())->m_pSkyFloatingDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		((CNtlWEFrm*)AfxGetMainWnd())->m_pSkyFloatingDlg->ShowWindow(SW_HIDE);
	}
}

VOID CPaletteFieldUI::Hide()
{
	m_SkyPaletteShow.SetCheck(FALSE);

	UpdateData(FALSE);
}

// void CPaletteFieldUI::OnBnClickedBtnMergeSpawnDst()
// {
// 	char acPathBK[NTL_MAX_DIR_PATH];
// 	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acPathBK);
// 
// 	CFileDialog	fileDlg(TRUE, "Dest Spawn File Load", "*.spn",
// 		OFN_HIDEREADONLY,
// 		"Data Files (*.spn)|*.spn||");
// 
// 	fileDlg.m_ofn.lpstrInitialDir = "tool\\saber\\spawn";
// 
// 	if (fileDlg.DoModal() == IDOK)
// 	{
// 		m_MergeSpawnDstName = fileDlg.GetPathName();
// 		CNtlWorldMergeManager::GetInstance()->LoadSpawnDst(m_MergeSpawnDstName.GetBuffer());
// 	}
// 	UpdateData(FALSE);
// 
// 	::SetCurrentDirectory(acPathBK);
// }
// 
// void CPaletteFieldUI::OnBnClickedBtnMergeSpawnSrc()
// {
// 	char acPathBK[NTL_MAX_DIR_PATH];
// 	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acPathBK);
// 
// 	CFileDialog	fileDlg(TRUE, "Src Spawn File Load", "*.spn",
// 		OFN_HIDEREADONLY,
// 		"Data Files (*.spn)|*.spn||");
// 
// 	fileDlg.m_ofn.lpstrInitialDir = "tool\\saber\\spawn";
// 
// 	if (fileDlg.DoModal() == IDOK)
// 	{
// 		m_MergeSpawnSrcName = fileDlg.GetPathName();
// 		CNtlWorldMergeManager::GetInstance()->LoadSpawnSrc(m_MergeSpawnSrcName.GetBuffer());
// 	}
// 	UpdateData(FALSE);
// 
// 	::SetCurrentDirectory(acPathBK);
// }

void CPaletteFieldUI::OnBnClickedBtnMergeSpawnDst()
{
	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	// set dir. info
	char pszPathname[NTL_MAX_DIR_PATH];
	ITEMIDLIST *pidlBrowse;

	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "find a spawn dst merge folder";
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		m_MergeSpawnDstName = pszPathname;

		SHGetPathFromIDList(pidlBrowse, pszPathname);

		m_MergeSpawnDstFullName = pszPathname;		
	}
	UpdateData(FALSE);
	::SetCurrentDirectory(dirBackup);
}

void CPaletteFieldUI::OnBnClickedBtnMergeSpawnSrc()
{
	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	// set dir. info
	char pszPathname[NTL_MAX_DIR_PATH];
	ITEMIDLIST *pidlBrowse;

	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd();
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "find a spawn src merge folder";
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	if(pidlBrowse != NULL)
	{
		m_MergeSpawnSrcName = pszPathname;

		SHGetPathFromIDList(pidlBrowse, pszPathname);

		m_MergeSpawnSrcFullName = pszPathname;
	}
	UpdateData(FALSE);
	::SetCurrentDirectory(dirBackup);
}
