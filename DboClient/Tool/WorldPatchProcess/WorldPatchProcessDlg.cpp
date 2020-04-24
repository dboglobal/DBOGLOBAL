#include "stdafx.h"
#include "WorldPatchProcess.h"
#include "WorldPatchProcessDlg.h"

#include "process.h"

#include <rwcore.h>
#include <rpworld.h>
#include <rtintsec.h> 
#include <rpcollis.h>
#include <rtquat.h> 
#include <rtanim.h>
#include <rphanim.h>
#include <rptoon.h>
#include <rpskin.h>
#include <rpusrdat.h>
#include <rpid.h>
#include <rpmatfx.h>
#include <rpUVAnim.h>
#include <rppvs.h>
#include <rpltmap.h>
#include <rprandom.h>
#include <rtcharse.h>

#include "NtlWorldCommon.h"

#include "NtlWorldFieldManager.h"
#include "NtlWorldFieldManager4RwWorld.h"
#include "NtlWorldLTManager.h"

#include "NtlPLWorldEntity.h"

#include "NtlWppApplication.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWorldPatchProcessDlg::CWorldPatchProcessDlg(CWnd* pParent /*=NULL*/)
:CDialog(CWorldPatchProcessDlg::IDD, pParent)
,m_pNtlWPPApplication(NULL)
,m_strWorldProject(_T(""))
,m_strWorldFields(_T(""))
,m_hThread(0)
,m_uiThreadID(0)
,m_bThreading(FALSE)
,m_strTime(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWorldPatchProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WORLD_PROJECT, m_listWorldProject);
	DDX_Text(pDX, IDC_EDIT_WORLD_PROJECT, m_strWorldProject);
	DDX_Text(pDX, IDC_EDIT_FIELDS, m_strWorldFields);
	DDX_Control(pDX, IDC_PROGRESS_WORLD_PROJECT, m_progressWorldProject);
	DDX_Control(pDX, IDC_PROGRESS_FILEDS, m_progressWorldFields);
	DDX_Text(pDX, IDC_EDIT_TIME, m_strTime);
}

BEGIN_MESSAGE_MAP(CWorldPatchProcessDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_QUERYDRAGICON()
	
	ON_BN_CLICKED(IDOK, &CWorldPatchProcessDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CWorldPatchProcessDlg::OnBnClickedCancel)
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CWorldPatchProcessDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CWorldPatchProcessDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CWorldPatchProcessDlg::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_PATCH, &CWorldPatchProcessDlg::OnBnClickedButtonPatch)
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CWorldPatchProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	m_pNtlWPPApplication = NTL_NEW CNtlWPPApplication;
	m_pNtlWPPApplication->Create(GetSafeHwnd());

	SetWindowText(CString("Saber Patcher - Convert Version(v") + CString(dNTL_WORLD_VERSION_OLD) + CString(" -> v") + CString(dNTL_WORLD_VERSION) + CString(")"));

	m_listWorldProject.InsertColumn(0, "", LVCFMT_LEFT, 0);
	m_listWorldProject.InsertColumn(LIST_SUBITEM_STATE, "State", LVCFMT_CENTER, 80);
	m_listWorldProject.InsertColumn(LIST_SUBITEM_WORLD_VERSION, "World Version", LVCFMT_CENTER, 80);
	m_listWorldProject.InsertColumn(LIST_SUBITEM_WORLD_NAME, "World Project Name", LVCFMT_CENTER, 150);
	m_listWorldProject.InsertColumn(LIST_SUBITEM_WORLD_FULLPATH, "World Project Folder", LVCFMT_LEFT, 300);
	m_listWorldProject.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	InitControlUI();

// 	m_strTime.Format("00:00:00/00:00:00");
// 	m_strWorldProject.Format("World(0/0)");
// 	m_strWorldFields.Format("Field(0/0)");

	DragAcceptFiles(TRUE);

	UpdateData(FALSE);

	return TRUE;
}

void CWorldPatchProcessDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CWorldPatchProcessDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWorldPatchProcessDlg::OnDestroy()
{
	CDialog::OnDestroy();

	KillTimer(TIMER_PATCH);

	if (m_hThread)
	{
		TerminateThread(m_hThread, 0);
		DeleteThread();
	}	

	if (m_pNtlWPPApplication)
	{
		m_pNtlWPPApplication->Destroy();
		NTL_DELETE(m_pNtlWPPApplication);
		m_pNtlWPPApplication = NULL;
	}
}

void CWorldPatchProcessDlg::OnBnClickedOk()
{
	OnOK();
}

void CWorldPatchProcessDlg::OnBnClickedCancel()
{
	OnCancel();
}

void CWorldPatchProcessDlg::OnDropFiles(HDROP hDropInfo)
{
	UINT uiDragCount = DragQueryFile(hDropInfo,0xFFFFFFFF,NULL,0);
	char acPath[_MAX_PATH];

	std::vector<std::string> vecPath;

	for (UINT uiDrag = 0; uiDrag < uiDragCount; ++uiDrag)
	{
		DragQueryFile(hDropInfo, uiDrag, acPath, _MAX_PATH);		

		AddWorldProject(acPath);		
	}

	CDialog::OnDropFiles(hDropInfo);
}

void CWorldPatchProcessDlg::OnBnClickedButtonClear()
{
	m_listWorldProject.DeleteAllItems();
}

void CWorldPatchProcessDlg::OnBnClickedButtonAdd()
{
	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	// set dir. info
	char pszPathname[NTL_MAX_DIR_PATH];
	ITEMIDLIST *pidlBrowse;

	BROWSEINFO BrInfo;
	BrInfo.hwndOwner = GetSafeHwnd()/*GetDesktopWindow()*/;
	BrInfo.pidlRoot = NULL;
	memset(&BrInfo, 0, sizeof(BrInfo));
	BrInfo.pszDisplayName = pszPathname;
	BrInfo.lpszTitle = "find a project folder";
	pidlBrowse = ::SHBrowseForFolder(&BrInfo);

	CString strResult = "";
	if(pidlBrowse != NULL)
	{
		SHGetPathFromIDList(pidlBrowse, pszPathname);
		AddWorldProject(pszPathname);
	}

	SetCurrentDirectory(dirBackup);
}

void CWorldPatchProcessDlg::OnBnClickedButtonDel()
{
	UINT	uiItemCount	= m_listWorldProject.GetSelectedCount();
	INT		iSymbol		= -1;

	std::vector<INT> vecSelectIndices;
	for (UINT uiItem = 0; uiItem < uiItemCount; ++uiItem)
	{
		iSymbol = m_listWorldProject.GetNextItem(iSymbol, LVNI_SELECTED);
		if (iSymbol != -1)
		{
			vecSelectIndices.push_back(iSymbol);
		}
	}

	for (std::vector<INT>::reverse_iterator rit = vecSelectIndices.rbegin(); rit != vecSelectIndices.rend(); ++rit)
	{
		m_listWorldProject.DeleteItem(*rit);
	}
}

void CWorldPatchProcessDlg::OnBnClickedButtonPatch()
{
	if(AfxMessageBox("Are you sure?", MB_YESNO) == IDYES)
	{
		InitControlUI();

		SetTimer(TIMER_PATCH, 100, NULL);
		CreateThread();
		SetEnableAll(FALSE);
	}
}

BOOL CWorldPatchProcessDlg::AddWorldProject(char* pcPath)
{
	CFileFind	fileFind;
	BOOL		bWorking	= fileFind.FindFile(pcPath);
	BOOL		bDirectory	= FALSE;
	while (bWorking)
	{
		bWorking = fileFind.FindNextFile();

		if (fileFind.IsDots())
		{
		}
		else if (fileFind.IsDirectory())
		{
			bDirectory = TRUE;
		}
		else
		{
		}
	}

	BOOL bResult = FALSE;
	if (bDirectory)
	{
		RwBool bIsExist = FALSE;
		for (int i = 0; i < m_listWorldProject.GetItemCount(); ++i)
		{
			if (!strcmp(m_listWorldProject.GetItemText(i, LIST_SUBITEM_WORLD_FULLPATH), pcPath))
			{
				bIsExist = TRUE;
			}
		}		

		LVITEM			lvi;
		sNtlWorldParam	NtlWorldParam;
		if (m_pNtlWPPApplication->TestWorldProject(pcPath, &NtlWorldParam) && !bIsExist)
		{
			lvi.mask		= LVIF_TEXT;
			lvi.iItem		= m_listWorldProject.GetItemCount();
			lvi.iSubItem	= LIST_SUBITEM_EMPTY;
			lvi.iImage		= 0;
			lvi.pszText		= "";

			m_listWorldProject.SetItemData(m_listWorldProject.InsertItem(&lvi), WORLD_PROJECT_STATE_RAEDY);

			lvi.iSubItem	= LIST_SUBITEM_STATE;
			lvi.iImage		= 0;
			lvi.pszText		= "ready";				

			m_listWorldProject.SetItem(&lvi);
			
			bResult			= TRUE;
		}
		else
		{
			lvi.mask		= LVIF_TEXT;
			lvi.iItem		= m_listWorldProject.GetItemCount();
			lvi.iSubItem	= LIST_SUBITEM_EMPTY;
			lvi.iImage		= 0;
			lvi.pszText		= "";				

			m_listWorldProject.SetItemData(m_listWorldProject.InsertItem(&lvi), WORLD_PROJECT_STATE_ERROR);

			lvi.iSubItem	= LIST_SUBITEM_STATE;
			lvi.iImage		= 0;
			lvi.pszText		= "error";

			m_listWorldProject.SetItem(&lvi);
		}
		
		CString WorldProjectName = pcPath;
		WorldProjectName.Delete(0, WorldProjectName.ReverseFind('\\') + 1);

		// WorldVersion
		lvi.iSubItem	= LIST_SUBITEM_WORLD_VERSION;
		lvi.iImage		= 0;
		lvi.pszText		= NtlWorldParam.WorldVer;				

		m_listWorldProject.SetItem(&lvi);

		// WorldProjectName
		lvi.iSubItem	= LIST_SUBITEM_WORLD_NAME;
		lvi.iImage		= 0;
		lvi.pszText		= WorldProjectName.GetBuffer();				

		m_listWorldProject.SetItem(&lvi);

		// WorldProjectFullPath
		lvi.iSubItem	= LIST_SUBITEM_WORLD_FULLPATH;
		lvi.iImage		= 0;
		lvi.pszText		= pcPath;				

		m_listWorldProject.SetItem(&lvi);

		m_listWorldProject.EnsureVisible( m_listWorldProject.GetItemCount() - 1, FALSE);
	}
	return bResult;
}

void CWorldPatchProcessDlg::ProgressWorldProject(int iPos, int iRange)
{
	m_progressWorldProject.SetRange(0, iRange);
	m_progressWorldProject.SetPos(iPos);
}

void CWorldPatchProcessDlg::ProgressWorldFields(int iPos, int iRange)
{
	m_progressWorldFields.SetRange(0, iRange);
	m_progressWorldFields.SetPos(iPos);
}

void CWorldPatchProcessDlg::CreateThread()
{
	if (m_hThread)
		return;

	m_hThread		= (HANDLE)_beginthreadex(NULL, 0, &PatchThread, this, 0, &m_uiThreadID);
	m_bThreading	= TRUE;
}

void CWorldPatchProcessDlg::DeleteThread()
{
	if (m_hThread)
	{
		WaitForSingleObject(m_hThread, INFINITE);

		// Thread 핸들을 닫는다
		CloseHandle(m_hThread);

		m_hThread		= 0;
		m_uiThreadID	= 0;
	}
}

unsigned int CWorldPatchProcessDlg::PatchThread(void* pParam)
{
	((CWorldPatchProcessDlg*)pParam)->Patch();
	((CWorldPatchProcessDlg*)pParam)->m_bThreading = FALSE;
	
	return 0;
}

// void CWorldPatchProcessDlg::Patch()
// {
// 	m_pNtlWPPApplication->InitPatchLog();
// 	m_pNtlWPPApplication->PrintPatchLog("<------ Patch Start ----->\n");
// 
// 	CPerformanceChecker performanceChecker;
// 
// 	int					iTotalFieldNum = 0;
// 	std::vector<int>	vecWorldProjectIndices;
// 	
// 	for (int i = 0; i < m_listWorldProject.GetItemCount(); ++i)
// 	{
// 		if ((BOOL)(m_listWorldProject.GetItemData(i)) == WORLD_PROJECT_STATE_RAEDY)
// 		{
// 			std::string strWorldProject = m_listWorldProject.GetItemText(i, LIST_SUBITEM_WORLD_FULLPATH).GetBuffer();			
// 			int			iFieldNum		= m_pNtlWPPApplication->GetWorldProjectFieldNum(strWorldProject.c_str());
// 			
// 			iTotalFieldNum += iFieldNum * iFieldNum;
// 			vecWorldProjectIndices.push_back(i);
// 		}
// 	}
// 
// 	double	fStartTime		= performanceChecker.GetTime();
// 	int		iTotalFieldCnt	= iTotalFieldNum;
// 	for (int i = 0; i < (int)vecWorldProjectIndices.size(); ++i)
// 	{
// 		int iWorldProjectIndex = vecWorldProjectIndices.at(i);
// 		if (m_listWorldProject.GetItemData(iWorldProjectIndex) == WORLD_PROJECT_STATE_RAEDY)
// 		{
// 			std::string strWorldProject = m_listWorldProject.GetItemText(iWorldProjectIndex, LIST_SUBITEM_WORLD_FULLPATH).GetBuffer();			
// 			m_pNtlWPPApplication->PrintPatchLog("[Patch World Start] %s\n", strWorldProject.c_str());
// 			if (m_pNtlWPPApplication->CreateWorldProject(strWorldProject.c_str(), dNTL_WORLD_VERSION_OLD, dNTL_WORLD_VERSION))
// 			{
// 				ProgressWorldProject(i + 1, (int)vecWorldProjectIndices.size());
// 				m_strWorldProject.Format("World(%d/%d) : %s", i + 1, (int)vecWorldProjectIndices.size(), strWorldProject.c_str());
// 
// 				CNtlWorldFieldManager*	pNtlWorldFieldManager	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
// 				int						iFieldNum				= dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum;
// 
// 				for(int iFieldIdx = 0; iFieldIdx < iFieldNum; ++iFieldIdx)
// 				{
// 					ProgressWorldFields(iFieldIdx + 1, iFieldNum);
// 					m_strWorldFields.Format("Field(%d/%d)", iFieldIdx + 1, iFieldNum);				
// 
// 					performanceChecker.Run();
// 					pNtlWorldFieldManager->SaveField(iFieldIdx);
// 					performanceChecker.Stop();
// 					--iTotalFieldCnt;
// 
// 					double fProcessTime = (performanceChecker.GetTime() - fStartTime);
// 					//double fTotalTime	= ((fProcessTime) + ((iTotalFieldCnt) * performanceChecker.GetMaxTime()));
// 					double fTotalTime	= ((fProcessTime) + ((fProcessTime / (iFieldIdx + 1)) * iTotalFieldCnt));
// 
// 					fProcessTime		= fProcessTime / 1000.0f;
// 					fTotalTime			= fTotalTime / 1000.0f;
// 
// 					RwUInt32 uiHourP	= (RwUInt32)(fProcessTime / 3600.0f);
// 					RwUInt32 uiMinuteP	= (RwUInt32)((fProcessTime - ((double)uiHourP * 3600.0f)) / 60.0f);
// 					RwUInt32 uiSecondP	= (RwUInt32)(fProcessTime - ((double)uiHourP * 3600.0f) - ((double)uiMinuteP * 60.0f));
// 					RwUInt32 uiHourT	= (RwUInt32)(fTotalTime / 3600.0f);
// 					RwUInt32 uiMinuteT	= (RwUInt32)((fTotalTime - ((double)uiHourT * 3600.0f)) / 60.0f);
// 					RwUInt32 uiSecondT	= (RwUInt32)(fTotalTime - ((double)uiHourT * 3600.0f) - ((double)uiMinuteT * 60.0f));
// 
// 					m_strTime.Format("%.2fsec/%.2fsec", fProcessTime, fTotalTime);
// 					m_strTime.Format("%.2u:%.2u:%.2u/%.2u:%.2u:%.2u", uiHourP, uiMinuteP, uiSecondP, uiHourT, uiMinuteT, uiSecondT);
// 					m_pNtlWPPApplication->PrintPatchLog("[Patch Field Succeeded] %d/%d\n", iFieldIdx, iFieldNum - 1);
// 
// 					Sleep(0);
// 				}
// 
// 				if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
// 				{
// 					((CNtlWorldFieldManager4RWWorld*)(pNtlWorldFieldManager))->SaveProperty();
// 					CNtlWorldLTManager::GetInstance()->SaveBSP();
// 				}
// 
// 				strcpy_s(dGET_WORLD_PARAM()->WorldVer, 10, dGET_WORLD_PARAM()->WorldSaveVer);
// 				SaveCurWorldState();
// 
// 				m_listWorldProject.SetItemData(iWorldProjectIndex, WORLD_PROJECT_STATE_SUCCEEDED);
// 				m_listWorldProject.SetItemText(iWorldProjectIndex, LIST_SUBITEM_STATE, "succeeded");
// 				m_listWorldProject.SetItemText(iWorldProjectIndex, LIST_SUBITEM_WORLD_VERSION, dGET_WORLD_PARAM()->WorldVer);
// 
// 				m_pNtlWPPApplication->PrintPatchLog("[Patch World Succeeded] %s\n", strWorldProject.c_str());
// 			}
// 			else
// 			{
// 				m_listWorldProject.SetItemData(iWorldProjectIndex, WORLD_PROJECT_STATE_FAILED);
// 				m_listWorldProject.SetItemText(iWorldProjectIndex, LIST_SUBITEM_STATE, "failed");
// 
// 				m_pNtlWPPApplication->PrintPatchLog("[Patch World Failed] %s\n", strWorldProject.c_str());
// 			}			
// 			m_pNtlWPPApplication->PrintPatchLog("[Patch World End] %s\n", strWorldProject.c_str());
// 			m_pNtlWPPApplication->DestroyWorldProject();			
// 		}
// 	}
// 
// 	m_pNtlWPPApplication->PrintPatchLog("<------ Patch End ------>\n");
// }

void CWorldPatchProcessDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_PATCH)
	{
		UpdateControlUI();
		if (!m_bThreading)
		{
			KillTimer(TIMER_PATCH);
			DeleteThread();
			SetEnableAll(TRUE);

			AfxMessageBox("Succeeded!");
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void CWorldPatchProcessDlg::SetEnableAll(BOOL bEnable)
{
	GetDlgItem(IDC_LIST_WORLD_PROJECT)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_CLEAR)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_ADD)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_DEL)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_PATCH)->EnableWindow(bEnable);
}

void CWorldPatchProcessDlg::InitControlUI()
{
	m_strWroldProjectCurrent	= "";
	m_fTimeStartPatch			= -1.0f;
	m_fTimeStartCurrent			= -1.0f;

	m_iWorldProjectCnt			= 0;	
	m_iWorldProjectNum			= 0;

	m_iFieldCntTotal			= 0;
	m_iFieldNumTotal			= 0;
	m_iFieldCntCurrent			= 0;
	m_iFieldNumCurrent			= 0;

	UpdateControlUI();
}

void CWorldPatchProcessDlg::UpdateControlUI()
{
	m_strWorldProject.Format("World(%d/%d)", m_iWorldProjectCnt, m_iWorldProjectNum);
	if (m_strWroldProjectCurrent.length() > 0)
	{
		m_strWorldProject += " : ";
		m_strWorldProject += m_strWroldProjectCurrent.c_str();
	}

	m_strWorldFields.Format("Field(%d/%d)", m_iFieldCntCurrent, m_iFieldNumCurrent);

	// progress bar
	ProgressWorldProject(m_iFieldCntTotal, m_iFieldNumTotal);
	ProgressWorldFields(m_iFieldCntCurrent, m_iFieldNumCurrent);

	// time
	double fCurrentTime = m_PerformanceChecker.GetTime();

	// fields
	double fTimeField, fTimeFieldN, fTimeTotal, fTimeTotalN;
	fTimeField = fTimeFieldN = fTimeTotal = fTimeTotalN = 0.0f;

	if (m_fTimeStartCurrent > 0.0f)
	{
		fTimeField	= fCurrentTime - m_fTimeStartCurrent;	
		fTimeFieldN	= ((fTimeField) + ((fTimeField / (m_iFieldCntCurrent + 1)) * (m_iFieldNumCurrent - m_iFieldCntCurrent)));
	}

	if (m_fTimeStartPatch > 0.0f)
	{
		fTimeTotal	= fCurrentTime - m_fTimeStartPatch;	
		fTimeTotalN	= ((fTimeTotal) + ((fTimeTotal / (m_iFieldCntTotal + 1)) * (m_iFieldNumTotal - m_iFieldCntTotal)));
	}
	
	CString strField, strFieldN, strTotal, strTotalN;

	GetTimeString(strField, fTimeField / 1000.0f);
	GetTimeString(strFieldN, fTimeFieldN / 1000.0f);
	GetTimeString(strTotal, fTimeTotal / 1000.0f);
	GetTimeString(strTotalN, fTimeTotalN / 1000.0f);

	m_strTime.Format("Current(%s/%s) : Total(%s/%s)", strField, strFieldN, strTotal, strTotalN);

	UpdateData(FALSE);
}

void CWorldPatchProcessDlg::GetTimeString(CString& str, double fTime)
{
	RwUInt32 uiHour		= (RwUInt32)(fTime / 3600.0f);
	RwUInt32 uiMinute	= (RwUInt32)((fTime - ((double)uiHour * 3600.0f)) / 60.0f);
	RwUInt32 uiSecond	= (RwUInt32)(fTime - ((double)uiHour * 3600.0f) - ((double)uiMinute * 60.0f));

	str.Format("%.2u:%.2u:%.2u", uiHour, uiMinute, uiSecond);
}

void CWorldPatchProcessDlg::Patch()
{
	m_pNtlWPPApplication->InitPatchLog();
	m_pNtlWPPApplication->PrintPatchLog("<------ Patch Start ----->\n");

	int					iTotalFieldNum = 0;
	std::vector<int>	vecWorldProjectIndices;

	for (int i = 0; i < m_listWorldProject.GetItemCount(); ++i)
	{
		if ((BOOL)(m_listWorldProject.GetItemData(i)) == WORLD_PROJECT_STATE_RAEDY)
		{
			std::string strWorldProject = m_listWorldProject.GetItemText(i, LIST_SUBITEM_WORLD_FULLPATH).GetBuffer();			
			int			iFieldNum		= m_pNtlWPPApplication->GetWorldProjectFieldNum(strWorldProject.c_str());

			iTotalFieldNum += iFieldNum * iFieldNum;
			vecWorldProjectIndices.push_back(i);
		}
	}
	
	m_iWorldProjectCnt	= 0;
	m_iWorldProjectNum	= (int)vecWorldProjectIndices.size();	
	m_iFieldCntTotal	= 0;
	m_iFieldNumTotal	= iTotalFieldNum;
	m_fTimeStartPatch	= m_PerformanceChecker.GetTime();

	for (int i = 0; i < (int)vecWorldProjectIndices.size(); ++i)
	{
		int iWorldProjectIndex = vecWorldProjectIndices.at(i);
		if (m_listWorldProject.GetItemData(iWorldProjectIndex) == WORLD_PROJECT_STATE_RAEDY)
		{
			m_strWroldProjectCurrent = m_listWorldProject.GetItemText(iWorldProjectIndex, LIST_SUBITEM_WORLD_FULLPATH).GetBuffer();			

			m_listWorldProject.EnsureVisible(iWorldProjectIndex, FALSE);
			m_listWorldProject.SetItemText(iWorldProjectIndex, LIST_SUBITEM_STATE, "patching");			
			m_pNtlWPPApplication->PrintPatchLog("[Patch World Start] %s\n",  m_strWroldProjectCurrent.c_str());			
			if (m_pNtlWPPApplication->CreateWorldProject( m_strWroldProjectCurrent.c_str(), dNTL_WORLD_VERSION_OLD, dNTL_WORLD_VERSION))
			{				
				CNtlWorldFieldManager*	pNtlWorldFieldManager	= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
				int						iFieldNum				= dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum;

				m_iFieldCntCurrent	= 0;
				m_iFieldNumCurrent	= iFieldNum;
				m_fTimeStartCurrent	= m_PerformanceChecker.GetTime();

				for(int iFieldIdx = 0; iFieldIdx < iFieldNum; ++iFieldIdx)
				{					
					pNtlWorldFieldManager->SaveField(iFieldIdx);				

					++m_iFieldCntTotal;
					++m_iFieldCntCurrent;

					Sleep(0);

					m_pNtlWPPApplication->PrintPatchLog("[Patch Field Succeeded] %d/%d\n", iFieldIdx, iFieldNum - 1);
				}

				if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
				{
					((CNtlWorldFieldManager4RWWorld*)(pNtlWorldFieldManager))->SaveProperty();
					CNtlWorldLTManager::GetInstance()->SaveBSP();
				}

				strcpy_s(dGET_WORLD_PARAM()->WorldVer, 10, dGET_WORLD_PARAM()->WorldSaveVer);
				SaveCurWorldState();

				m_listWorldProject.SetItemData(iWorldProjectIndex, WORLD_PROJECT_STATE_SUCCEEDED);
				m_listWorldProject.SetItemText(iWorldProjectIndex, LIST_SUBITEM_STATE, "succeeded");
				m_listWorldProject.SetItemText(iWorldProjectIndex, LIST_SUBITEM_WORLD_VERSION, dGET_WORLD_PARAM()->WorldVer);

				m_pNtlWPPApplication->PrintPatchLog("[Patch World Succeeded] %s\n",  m_strWroldProjectCurrent.c_str());
			}
			else
			{
				m_listWorldProject.SetItemData(iWorldProjectIndex, WORLD_PROJECT_STATE_FAILED);
				m_listWorldProject.SetItemText(iWorldProjectIndex, LIST_SUBITEM_STATE, "failed");

				m_pNtlWPPApplication->PrintPatchLog("[Patch World Failed] %s\n",  m_strWroldProjectCurrent.c_str());
			}			
			m_pNtlWPPApplication->PrintPatchLog("[Patch World End] %s\n",  m_strWroldProjectCurrent.c_str());
			m_pNtlWPPApplication->DestroyWorldProject();			
		}

		++m_iWorldProjectCnt;
	}

	m_pNtlWPPApplication->PrintPatchLog("<------ Patch End ------>\n");
}