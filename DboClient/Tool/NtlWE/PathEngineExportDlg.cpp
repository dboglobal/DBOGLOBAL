#include "stdafx.h"

#include "NtlWE.h"

#include "NtlWorldPlugin.h"
#include "NtlWorldCommon.h"
#include "NtlWorldInterface.h"
#include "NtlWorldMaterialPlugin.h"

#include "NtlWorldFieldManager.h"

#include "NtlPLRenderState.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLHeatHaze.h"

#include "NtlPostEffectCamera.h"

#include "NtlWeControlUi.h"

#include "ProgressWnd.h"

#include "NtlWorldMergeManager.h"

#include "PathEngineExportDlg.h"

BOOL OpenFolderBrowser(const char* pcWindowTitle, char* pSelectFolder)
{
	ITEMIDLIST*	pidlBrowse;

	// set dir. info
	BROWSEINFO BrInfo;
	memset(&BrInfo, 0, sizeof(BrInfo));

	BrInfo.hwndOwner		= NULL;
	BrInfo.pidlRoot			= NULL;
	BrInfo.pszDisplayName	= pSelectFolder;
	BrInfo.lpszTitle		= pcWindowTitle;
	BrInfo.ulFlags			= BIF_NEWDIALOGSTYLE;
	pidlBrowse				= ::SHBrowseForFolder(&BrInfo);

	if (pidlBrowse)
	{
		SHGetPathFromIDList(pidlBrowse, pSelectFolder);

		return TRUE;
	}
	return FALSE;
}


IMPLEMENT_DYNAMIC(CPathEngineExportDlg, CDialog)

CPathEngineExportDlg::CPathEngineExportDlg(CWnd* pParent /*=NULL*/)
: CDialog(CPathEngineExportDlg::IDD, pParent)
, m_strWorldPath(_T(""))
, m_strExportPath(_T(""))
, m_strWorldName(_T(""))
, m_strCrossFieldCntOfGroup(_T(""))
, m_strOverlapSize(_T(""))
, m_strWorldID(_T(""))
{
}

CPathEngineExportDlg::~CPathEngineExportDlg()
{
}

void CPathEngineExportDlg::InitializeWorldData()
{
}

BOOL CPathEngineExportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CPathEngineExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WORLD_PATH, m_strWorldPath);
	DDX_Text(pDX, IDC_EDIT_EXPORT_PATH, m_strExportPath);
	DDX_Text(pDX, IDC_EDIT_WORLD_NAME, m_strWorldName);
	DDX_Text(pDX, IDC_EDIT_FIELD_CNT, m_strCrossFieldCntOfGroup);
	DDX_Text(pDX, IDC_EDIT_OVERLAP_SIZE, m_strOverlapSize);
	DDX_Text(pDX, IDC_EDIT_AGENCY, m_strAgency);
	DDX_Text(pDX, IDC_EDIT_WORLD_ID, m_strWorldID);
}

BOOL CPathEngineExportDlg::PreTranslateMessage(MSG* pMsg)
{
	// prevent keys; escape, return
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
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


BEGIN_MESSAGE_MAP(CPathEngineExportDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_WORLD_PATH, &CPathEngineExportDlg::OnBnClickedBtnWorldPath)
	ON_BN_CLICKED(IDC_BTN_EXPORT_PATH, &CPathEngineExportDlg::OnBnClickedBtnExportPath)
	ON_BN_CLICKED(IDC_BTN_PATH_ENGINE_DATA_EXPORT, &CPathEngineExportDlg::OnBnClickedBtnPathEngineDataExport)
END_MESSAGE_MAP()


// CSaberTestDlg 메시지 처리기입니다.

void CPathEngineExportDlg::OnBnClickedBtnWorldPath()
{
	UpdateData(TRUE);

	char acTemp[NTL_MAX_DIR_PATH];
	if (OpenFolderBrowser("Select World Project Folder", acTemp))
	{
		char			acOldPath[NTL_MAX_DIR_PATH];
		FILE*			pFile;

		::GetCurrentDirectory(NTL_MAX_DIR_PATH, acOldPath);
		::SetCurrentDirectory(acTemp);

		m_strWorldPath = "";
		if(fopen_s(&pFile, "#######.gwp", "rb"))
		{
			::SetCurrentDirectory(acOldPath);
			AfxMessageBox("wroong worldpath!");
		}
		else
		{
			LoadFileWorldState(&m_NtlWorldParam, pFile);
			fclose(pFile);
			::SetCurrentDirectory(acOldPath);

			m_strWorldPath = acTemp;

			GetDlgItem(IDC_EDIT_FIELD_CNT)->EnableWindow(m_NtlWorldParam.WorldType == 0);
			GetDlgItem(IDC_EDIT_OVERLAP_SIZE)->EnableWindow(m_NtlWorldParam.WorldType == 0);			
		}
	}
	UpdateData(FALSE);
}

void CPathEngineExportDlg::OnBnClickedBtnExportPath()
{
	UpdateData(TRUE);

	char acTemp[NTL_MAX_DIR_PATH];
	if (OpenFolderBrowser("Select Export Folder", acTemp))
	{
		m_strExportPath = acTemp;
	}
	else
	{
		m_strExportPath = "";
	}

	UpdateData(FALSE);
}

void CPathEngineExportDlg::OnBnClickedBtnPathEngineDataExport()
{
	UpdateData(TRUE);

	char			acOldPath[NTL_MAX_DIR_PATH];
	FILE*			pFile;

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acOldPath);
	::SetCurrentDirectory(m_strWorldPath.GetBuffer());

	sNtlWorldParam sWorldParam;
	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		::SetCurrentDirectory(acOldPath);
		AfxMessageBox("wroong worldpath!");
	}
	else
	{
		LoadFileWorldState(&sWorldParam, pFile);
		fclose(pFile);
		::SetCurrentDirectory(acOldPath);
	}

	if (sWorldParam.WorldType == 0)
	{
		CNtlWorldMergeManager::GetInstance()->OnExportPathEngineOutDoor(
			m_strWorldPath.GetBuffer(),
			::atoi(m_strWorldID.GetBuffer()),
			(m_strExportPath + CString("\\") + m_strWorldID).GetBuffer(),
			m_strWorldName.GetBuffer(),
			m_strAgency.GetBuffer(),
			::atoi(m_strCrossFieldCntOfGroup.GetBuffer()),
			::atoi(m_strOverlapSize.GetBuffer())
			);
	}
	else
	{
		CNtlWorldMergeManager::GetInstance()->OnExportPathEngineInDoor(
			m_strWorldPath.GetBuffer(),
			::atoi(m_strWorldID.GetBuffer()),
			(m_strExportPath + CString("\\") + m_strWorldID).GetBuffer(),
			m_strWorldName.GetBuffer(),
			m_strAgency.GetBuffer()
			);
	}
}
