// FileNewDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "NtlWEDoc.h"
#include "FileNewDlg.h"
#include <ntlworldcommon.h>
#include "NtlPLSky.h"


// CFileNewDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFileNewDlg, CDialog)
CFileNewDlg::CFileNewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileNewDlg::IDD, pParent)
	, m_RAWFileName(_T(""))
{
}

CFileNewDlg::~CFileNewDlg()
{
}

void CFileNewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WorldHeightFieldNum, m_WorldHeightfieldNum);
	DDX_Control(pDX, IDC_WorldSectorTileSize, m_WorldSectorTileSize);
	DDX_Control(pDX, IDC_WorldSectorTileNum, m_WorldSectorTileNum);
	DDX_Control(pDX, IDC_WorldFieldSectorNum, m_WorldFieldSectorNum);
	DDX_Control(pDX, IDC_WorldTexAddr, m_WorldTexAddr);
	DDX_Text(pDX, IDC_RAW_FILE_NAME, m_RAWFileName);
	DDX_Control(pDX, IDC_SKY_TYPE, m_SkyTypeIdxCombo);
	DDX_Control(pDX, IDC_WORLD_TYPE, m_WorldTypeCombo);
	DDX_Control(pDX, IDC_WorldBlockSize, m_WorldBlockSize);
}


BEGIN_MESSAGE_MAP(CFileNewDlg, CDialog)
	ON_EN_CHANGE(IDC_WorldHeightFieldNum, &CFileNewDlg::OnEnChangeWorldHeightFieldNum)
	ON_EN_CHANGE(IDC_WorldSectorTileSize, &CFileNewDlg::OnEnChangeWorldSectorTileSize)
	ON_EN_CHANGE(IDC_WorldSectorTileNum, &CFileNewDlg::OnEnChangeWorldSectorTileNum)
	ON_EN_CHANGE(IDC_WorldFieldSectorNum, &CFileNewDlg::OnEnChangeWorldFieldSectorNum)
	ON_EN_CHANGE(IDC_WorldTexAddr, &CFileNewDlg::OnEnChangeWorldTexAddr)
	ON_EN_CHANGE(IDC_WorldBlockSize, &CFileNewDlg::OnEnChangeWorldBlockSize)
	ON_CBN_SELCHANGE(IDC_WORLD_TYPE, &CFileNewDlg::OnCbnSelchangeWorldType)
	ON_CBN_SELCHANGE(IDC_SKY_TYPE, &CFileNewDlg::OnCbnSelchangeSkyType)
	ON_BN_CLICKED(ID_LOAD_RAW, &CFileNewDlg::OnBnClickedLoadRaw)
	ON_BN_CLICKED(IDOK, &CFileNewDlg::OnBnClickedOk)
	
	
END_MESSAGE_MAP()


// CFileNewDlg 메시지 처리기입니다.

BOOL CFileNewDlg::OnInitDialog()
{
	m_IsDialogInitialize = false;

	CDialog::OnInitDialog();

	dGETDOC()->m_NtlWorldParam = *dGET_WORLD_PARAM();

	CString ResultString;
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldHeightfieldNum);
	m_WorldHeightfieldNum.SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldSectorTileSize);
	m_WorldSectorTileSize.SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldSectorTileNum);
	m_WorldSectorTileNum.SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldFieldSectorNum);
	m_WorldFieldSectorNum.SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldTexAddr);
	m_WorldTexAddr.SetWindowText(ResultString);

	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldSectorVertNum);
	((CButton*)(GetDlgItem(IDC_WorldSectorVertNum)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldSectorNum);
	((CButton*)(GetDlgItem(IDC_WorldSectorNum)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldSectorSize);
	((CButton*)(GetDlgItem(IDC_WorldSectorSize)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldSectorHalfNum);
	((CButton*)(GetDlgItem(IDC_WorldSectorHalfNum)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldSectorPolyNum);
	((CButton*)(GetDlgItem(IDC_WorldSectorPolyNum)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldSize);
	((CButton*)(GetDlgItem(IDC_WorldSize)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldSizeHalf);
	((CButton*)(GetDlgItem(IDC_WorldSizeHalf)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldValueMax);
	((CButton*)(GetDlgItem(IDC_WorldValueMax)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldValueMin);
	((CButton*)(GetDlgItem(IDC_WorldValueMin)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldFieldSize);
	((CButton*)(GetDlgItem(IDC_WorldFieldSize)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldFieldNum);
	((CButton*)(GetDlgItem(IDC_WorldFieldNum)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldFieldHalfNum);
	((CButton*)(GetDlgItem(IDC_WorldFieldHalfNum)))->SetWindowText(ResultString);

	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldBlockSize);
	((CButton*)(GetDlgItem(IDC_WorldBlockSize)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldBlockNum);
	((CButton*)(GetDlgItem(IDC_WorldBlockNum)))->SetWindowText(ResultString);

	m_IsDialogInitialize = true;

	m_WorldTypeCombo.SetCurSel(0);
	m_SkyTypeIdxCombo.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CFileNewDlg::Update()
{
	CString ResultString;

	m_WorldHeightfieldNum.GetWindowText(ResultString);
	dGETDOC()->m_NtlWorldParam.WorldHeightfieldNum	= static_cast<RwInt32>(atoi(ResultString));
	m_WorldSectorTileSize.GetWindowText(ResultString);
	dGETDOC()->m_NtlWorldParam.WorldSectorTileSize	= static_cast<RwInt32>(atoi(ResultString));
	m_WorldSectorTileNum.GetWindowText(ResultString);
	dGETDOC()->m_NtlWorldParam.WorldSectorTileNum	= static_cast<RwInt32>(atoi(ResultString));
	m_WorldFieldSectorNum.GetWindowText(ResultString);
	dGETDOC()->m_NtlWorldParam.WorldFieldSectorNum	= static_cast<RwInt32>(atoi(ResultString));
	m_WorldTexAddr.GetWindowText(ResultString);
	dGETDOC()->m_NtlWorldParam.WorldTexAddr			= static_cast<RwInt32>(atoi(ResultString));
	m_WorldBlockSize.GetWindowText(ResultString);
	dGETDOC()->m_NtlWorldParam.WorldBlockSize		= static_cast<RwInt32>(atoi(ResultString));

	if(dGETDOC()->m_NtlWorldParam.WorldHeightfieldNum <= 0 ||
	   dGETDOC()->m_NtlWorldParam.WorldSectorTileSize <= 0 ||
	   dGETDOC()->m_NtlWorldParam.WorldSectorTileNum <= 0 ||
	   dGETDOC()->m_NtlWorldParam.WorldFieldSectorNum <= 0 ||
	   dGETDOC()->m_NtlWorldParam.WorldBlockSize <= 0)
	{
		AfxMessageBox("wrong input param.");
		return;
	}

	dGETDOC()->m_NtlWorldParam.WorldSectorVertNum	= dGETDOC()->m_NtlWorldParam.WorldSectorTileNum + 1;
	dGETDOC()->m_NtlWorldParam.WorldSectorNum		= dGETDOC()->m_NtlWorldParam.WorldHeightfieldNum / dGETDOC()->m_NtlWorldParam.WorldSectorTileNum;
	dGETDOC()->m_NtlWorldParam.WorldSectorSize		= dGETDOC()->m_NtlWorldParam.WorldSectorTileNum * dGETDOC()->m_NtlWorldParam.WorldSectorTileSize;
	dGETDOC()->m_NtlWorldParam.WorldSectorHalfNum	= dGETDOC()->m_NtlWorldParam.WorldSectorNum / 2;
	dGETDOC()->m_NtlWorldParam.WorldSectorPolyNum	= dGETDOC()->m_NtlWorldParam.WorldSectorTileNum * dGETDOC()->m_NtlWorldParam.WorldSectorTileNum * 2;
	dGETDOC()->m_NtlWorldParam.WorldSize			= dGETDOC()->m_NtlWorldParam.WorldSectorNum * dGETDOC()->m_NtlWorldParam.WorldSectorSize;
	dGETDOC()->m_NtlWorldParam.WorldSizeHalf		= dGETDOC()->m_NtlWorldParam.WorldSize / 2;
	dGETDOC()->m_NtlWorldParam.WorldValueMax		= +(dGETDOC()->m_NtlWorldParam.WorldSizeHalf);
	dGETDOC()->m_NtlWorldParam.WorldValueMin		= -(dGETDOC()->m_NtlWorldParam.WorldSizeHalf);
	dGETDOC()->m_NtlWorldParam.WorldFieldSize		= dGETDOC()->m_NtlWorldParam.WorldSectorSize * dGETDOC()->m_NtlWorldParam.WorldFieldSectorNum;
	dGETDOC()->m_NtlWorldParam.WorldFieldNum		= dGETDOC()->m_NtlWorldParam.WorldSize / dGETDOC()->m_NtlWorldParam.WorldFieldSize;
	dGETDOC()->m_NtlWorldParam.WorldFieldHalfNum	= dGETDOC()->m_NtlWorldParam.WorldFieldNum / 2;
	dGETDOC()->m_NtlWorldParam.WorldBlockNum		= dGETDOC()->m_NtlWorldParam.WorldSize / dGETDOC()->m_NtlWorldParam.WorldBlockSize;
	dGETDOC()->m_NtlWorldParam.IdxSkyType			= dGETDOC()->m_NtlWorldParam.IdxSkyType;

	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldSectorVertNum);
	((CButton*)(GetDlgItem(IDC_WorldSectorVertNum)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldSectorNum);
	((CButton*)(GetDlgItem(IDC_WorldSectorNum)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldSectorSize);
	((CButton*)(GetDlgItem(IDC_WorldSectorSize)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldSectorHalfNum);
	((CButton*)(GetDlgItem(IDC_WorldSectorHalfNum)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldSectorPolyNum);
	((CButton*)(GetDlgItem(IDC_WorldSectorPolyNum)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldSize);
	((CButton*)(GetDlgItem(IDC_WorldSize)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldSizeHalf);
	((CButton*)(GetDlgItem(IDC_WorldSizeHalf)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldValueMax);
	((CButton*)(GetDlgItem(IDC_WorldValueMax)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldValueMin);
	((CButton*)(GetDlgItem(IDC_WorldValueMin)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldFieldSize);
	((CButton*)(GetDlgItem(IDC_WorldFieldSize)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldFieldNum);
	((CButton*)(GetDlgItem(IDC_WorldFieldNum)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldFieldHalfNum);
	((CButton*)(GetDlgItem(IDC_WorldFieldHalfNum)))->SetWindowText(ResultString);
	ResultString.Format("%d", dGETDOC()->m_NtlWorldParam.WorldBlockNum);
	((CButton*)(GetDlgItem(IDC_WorldBlockNum)))->SetWindowText(ResultString);

	UpdateData(FALSE);
}
void CFileNewDlg::OnEnChangeWorldHeightFieldNum()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_IsDialogInitialize)
		return;

	UpdateData();

	Update();
}

void CFileNewDlg::OnEnChangeWorldSectorTileSize()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_IsDialogInitialize)
		return;

	UpdateData();

	Update();
}

void CFileNewDlg::OnEnChangeWorldSectorTileNum()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_IsDialogInitialize)
		return;

	UpdateData();

	Update();
}

void CFileNewDlg::OnEnChangeWorldFieldSectorNum()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_IsDialogInitialize)
		return;

	UpdateData();

	Update();
}

void CFileNewDlg::OnEnChangeWorldTexAddr()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CDialog::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_IsDialogInitialize)
		return;

	UpdateData();

	Update();
}

void CFileNewDlg::OnEnChangeWorldBlockSize()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(!m_IsDialogInitialize)
		return;

	UpdateData();

	Update();
}


void CFileNewDlg::OnBnClickedLoadRaw()
{
	if (m_WorldTypeCombo.GetCurSel() == 0) // OutDoor
	{
		char dirBackup[NTL_MAX_DIR_PATH];
		::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

		static bool Floating = false;

		if(Floating)
		{
			::SetCurrentDirectory(dirBackup);
			return;
		}

		Floating = true;


		CFileDialog	FileDlg(TRUE,
			"Load *.raw files",
			"*.raw",
			OFN_HIDEREADONLY,
			"Data Files (*.raw)|*.raw||");

		FileDlg.m_ofn.lpstrInitialDir = "Tool\\SABER\\Texture\\Heightfield";

		if(FileDlg.DoModal() == IDCANCEL)
		{
			Floating = false;
			::SetCurrentDirectory(dirBackup);
			return;
		}

		Floating = false;

		::SetCurrentDirectory(dirBackup);

		// verify
		RwBool	IsProperFormat	= FALSE;
		RwInt32	RawSize			= 0;

		// set proper options properly
		HANDLE hFile = ::CreateFile(FileDlg.GetPathName().GetBuffer(0),
			GENERIC_READ,
			FILE_SHARE_READ, 
			NULL,
			OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL, 
			NULL);

		if(hFile != INVALID_HANDLE_VALUE)
		{
			DWORD	dwFileSize		= ::GetFileSize(hFile, NULL);

			switch(dwFileSize)
			{
			case (513 * 513 * 3):
				{
					IsProperFormat	= TRUE;
					RawSize			= 512;
				}
				break;

			case (1025 * 1025 * 3):
				{
					IsProperFormat	= TRUE;
					RawSize			= 1024;
				}
				break;

			case (2049 * 2049 * 3):		
				{
					IsProperFormat	= TRUE;
					RawSize			= 2048;
				}
				break;

			case (4097 * 4097 * 3):
				{
					IsProperFormat	= TRUE;
					RawSize			= 4096;
				}
				break;

			case (8193 * 8193 * 3):
				{
					IsProperFormat	= TRUE;
					RawSize			= 8192;
				}
				break;

			case (16385 * 16385 * 3):
				{
					IsProperFormat	= TRUE;
					RawSize			= 16384;
				}
				break;
			}
		}
		else
		{
			::AfxMessageBox("This raw file is something wrong.", MB_OK);
		}

		::CloseHandle(hFile);

		if(IsProperFormat)
		{
			CString Size;
			Size.Format("%d", RawSize);
			m_WorldHeightfieldNum.SetWindowText(Size);

			::strcpy_s(dGETDOC()->m_NtlWorldParam.WorldRAW, 64, FileDlg.GetFileTitle().GetBuffer(0));
			m_RAWFileName.Format("%s", dGETDOC()->m_NtlWorldParam.WorldRAW);
			UpdateData(FALSE);

			Update();
		}
		else
		{
			::strcpy_s(dGETDOC()->m_NtlWorldParam.WorldRAW, 64, "Error");
			m_RAWFileName.Format("%s", dGETDOC()->m_NtlWorldParam.WorldRAW);
			UpdateData(FALSE);
		}
	}
	else if (m_WorldTypeCombo.GetCurSel() == 1) // InDoor
	{
		char dirBackup[NTL_MAX_DIR_PATH];
		::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

		static bool Floating = false;

		if(Floating)
		{
			::SetCurrentDirectory(dirBackup);
			return;
		}

		Floating = true;

		CFileDialog	FileDlg(TRUE,
			"Load *.bsp files",
			"*.bsp",
			OFN_HIDEREADONLY,
			"Data Files (*.bsp)|*.bsp||");

		FileDlg.m_ofn.lpstrInitialDir = "Tool\\SABER\\Texture\\Heightfield";

		if(FileDlg.DoModal() == IDCANCEL)
		{
			Floating = false;
			::SetCurrentDirectory(dirBackup);
			return;
		}

		Floating = false;

		::SetCurrentDirectory(dirBackup);

		::strcpy_s(dGETDOC()->m_NtlWorldParam.WorldBSP, 64, FileDlg.GetFileTitle().GetBuffer(0));
		m_RAWFileName.Format("%s", dGETDOC()->m_NtlWorldParam.WorldBSP);
		UpdateData(FALSE);

		Update();
	}
}
void CFileNewDlg::OnCbnSelchangeSkyType()
{
	dGETDOC()->m_NtlWorldParam.IdxSkyType = static_cast<RwInt32>(m_SkyTypeIdxCombo.GetCurSel());
}

void CFileNewDlg::OnBnClickedOk()
{
	if (m_WorldTypeCombo.GetCurSel() == 0 && !m_RAWFileName.GetLength())
	{
		::AfxMessageBox("You did't select RAW file.", MB_OK);
		return;
	}
	if (m_WorldTypeCombo.GetCurSel() == 1 && !m_RAWFileName.GetLength())
	{
		::AfxMessageBox("You did't select BSP file.", MB_OK);
		return;
	}
	OnOK();
}

void CFileNewDlg::OnCbnSelchangeWorldType()
{
	switch (m_WorldTypeCombo.GetCurSel())
	{
	case 0:
		GetDlgItem(IDC_WorldFieldSectorNum)->EnableWindow(FALSE);
		GetDlgItem(IDC_WorldSectorTileNum)->EnableWindow(FALSE);
		GetDlgItem(IDC_WorldSectorTileSize)->EnableWindow(FALSE);
		GetDlgItem(IDC_WorldHeightFieldNum)->EnableWindow(FALSE);
		GetDlgItem(IDC_WorldBlockSize)->EnableWindow(FALSE);

		m_RAWFileName = "";

		dGETDOC()->m_NtlWorldParam.WorldType = 0;
		break;
	case 1:
		GetDlgItem(IDC_WorldFieldSectorNum)->EnableWindow(TRUE);
		GetDlgItem(IDC_WorldSectorTileNum)->EnableWindow(TRUE);
		GetDlgItem(IDC_WorldSectorTileSize)->EnableWindow(TRUE);
		GetDlgItem(IDC_WorldHeightFieldNum)->EnableWindow(TRUE);
		GetDlgItem(IDC_WorldBlockSize)->EnableWindow(TRUE);

		m_RAWFileName = "";

		dGETDOC()->m_NtlWorldParam.WorldType = 1;
		break;
	}

	UpdateData(FALSE);
	Update();
}