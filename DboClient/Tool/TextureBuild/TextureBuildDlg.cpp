// TextureBuildDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "TextureBuild.h"
#include "TextureBuildDlg.h"

#include  <io.h>
#include  <stdio.h>
#include  <stdlib.h>

#include <sys/stat.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString GetLevel(unsigned int uiLevel)
{
	switch (uiLevel)
	{
	case FTD_FLAG_LV1:
		return "_lv1";
	case FTD_FLAG_LV2:
		return "_lv2";
	case FTD_FLAG_LV3:
		return "_lv3";
	}
	return "_null";
}

void GetItemsNum_All_Exclosion_Dir(HTREEITEM hItem, CTreeCtrl* pTreeCtrl, void* pUserData)
{
	unsigned int* pNum = (unsigned int*)pUserData;

	if (pTreeCtrl->GetItemData(hItem) != FTD_FLAG_DIRECTROY)
	{
		++(*pNum);
	}
}

void GetItemsNum_Lv0(HTREEITEM hItem, CTreeCtrl* pTreeCtrl, void* pUserData)
{
	unsigned int* pNum = (unsigned int*)pUserData;

	if (pTreeCtrl->GetItemData(hItem) != FTD_FLAG_DIRECTROY && pTreeCtrl->GetItemData(hItem) == FTD_FLAG_LV0)
	{
		++(*pNum);
	}
}

void GetItemsNum_Lv1(HTREEITEM hItem, CTreeCtrl* pTreeCtrl, void* pUserData)
{
	unsigned int* pNum = (unsigned int*)pUserData;

	if (pTreeCtrl->GetItemData(hItem) != FTD_FLAG_DIRECTROY && pTreeCtrl->GetItemData(hItem) == FTD_FLAG_LV1)
	{
		++(*pNum);
	}
}

void GetItemsNum_Lv2(HTREEITEM hItem, CTreeCtrl* pTreeCtrl, void* pUserData)
{
	unsigned int* pNum = (unsigned int*)pUserData;

	if (pTreeCtrl->GetItemData(hItem) != FTD_FLAG_DIRECTROY && pTreeCtrl->GetItemData(hItem) == FTD_FLAG_LV2)
	{
		++(*pNum);
	}
}

void GetItemsNum_Lv3(HTREEITEM hItem, CTreeCtrl* pTreeCtrl, void* pUserData)
{
	unsigned int* pNum = (unsigned int*)pUserData;

	if (pTreeCtrl->GetItemData(hItem) != FTD_FLAG_DIRECTROY && pTreeCtrl->GetItemData(hItem) == FTD_FLAG_LV3)
	{
		++(*pNum);
	}
}

void GetItemsNum_All(HTREEITEM hItem, CTreeCtrl* pTreeCtrl, void* pUserData)
{
	unsigned int* pNum = (unsigned int*)pUserData;

	++(*pNum);
}

void GetExportLevel(HTREEITEM hItem, CTreeCtrl* pTreeCtrl, void* pUserData)
{
	unsigned int* pLevel = (unsigned int*)pUserData;
	if (pTreeCtrl->GetItemData(hItem) != FTD_FLAG_DIRECTROY && pTreeCtrl->GetItemData(hItem) > *pLevel)
	{
		*pLevel = (unsigned int)pTreeCtrl->GetItemData(hItem);
	}
}

void ResizeLevelTexture(CString strPathOri, CString strPathNew, unsigned int uiLevel)
{
	float fDelta;
	switch (uiLevel)
	{
	case FTD_FLAG_LV1:
		fDelta = 0.5f;
		break;
	case FTD_FLAG_LV2:
		fDelta = 0.25f;
		break;
	case FTD_FLAG_LV3:
		fDelta = 0.125f;
		break;
	}

	int iStart		= 0;
	int iEnd		= 0;

	do 
	{
		iEnd = strPathNew.Find('\\', iStart);

		CString strTemp;
		if (iEnd > 0)
		{
			strTemp	= strPathNew.Mid(0, iEnd);
			iStart = iEnd + 1;
			CreateDirectory(strTemp.GetBuffer(), NULL);
		}
	} while (iEnd > 0);

	_chmod(strPathNew.GetBuffer(), _S_IREAD | _S_IWRITE);
 	if (!((CTextureBuildApp*)AfxGetApp())->GetConverter()->ImageResize(strPathOri.GetBuffer(), strPathNew.GetBuffer(), fDelta, fDelta))
	{
		CString strError = "Resize Failed : "; strError += strPathNew;
		AfxMessageBox(strError.GetBuffer());
	}
}

void ExportLevelTexture(HTREEITEM hItem, CTreeCtrl* pTreeCtrl, void* pUserData)
{
	CProgressCtrl*	pProgressCtrl	= ((CTextureBuildDlg*)(AfxGetApp()->m_pMainWnd))->GetProgressCtrl();
	int				iPos			= pProgressCtrl->GetPos(); pProgressCtrl->SetPos(++iPos);

	unsigned int*	pLevel = (unsigned int*)pUserData;
	unsigned int	uiLevel = (unsigned int)(pTreeCtrl->GetItemData(hItem));

	if (*pLevel > FTD_FLAG_LV3 || uiLevel == FTD_FLAG_DIRECTROY)
	{
		return;
	}

	for (unsigned int ui = FTD_FLAG_LV1; ui <= *pLevel; ++ui)
	{
		CString strLev = GetLevel(ui);
		
		CString strPathOri = ((CFileTreeCtrl*)pTreeCtrl)->GetPathByItem(hItem);
		CString strPathNew = strPathOri; strPathNew.Insert(strPathNew.Find('\\', 0), strLev);
		if (uiLevel >= ui)
		{
			ResizeLevelTexture(strPathOri, strPathNew, ui);
		}
// 		else
// 		{
// 			ResizeLevelTexture(strPathOri, strPathNew, uiLevel);
// 		}
	}
}

void SetLevelByFile(HTREEITEM hItem, CTreeCtrl* pTreeCtrl, void* pUserData)
{
	CFileTreeCtrl* pFileTreeCtrl = (CFileTreeCtrl*)pTreeCtrl;

	if (pFileTreeCtrl->GetItemData(hItem) == FTD_FLAG_DIRECTROY)
	{
		return;
	}

	CString strPathOri = pFileTreeCtrl->GetPathByItem(hItem);
	for (unsigned int ui = FTD_FLAG_LV1; ui <= FTD_FLAG_LV3; ++ui)
	{
		CString strLev = GetLevel(ui);

		CString strPathNew = strPathOri;
		strPathNew.Insert(strPathNew.Find('\\', 0), strLev);

		if (_access(strPathNew.GetBuffer(), 0) != -1)
		{
			pFileTreeCtrl->SetItemData(hItem, ui);
			pFileTreeCtrl->SetItemImage(hItem, ui, ui);
		}
	}
}

CTextureBuildDlg::CTextureBuildDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTextureBuildDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTextureBuildDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_FILE, m_treeFile);
	DDX_Control(pDX, IDC_PROGRESS1, m_proAllItems);
}

BEGIN_MESSAGE_MAP(CTextureBuildDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CTextureBuildDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTextureBuildDlg::OnBnClickedCancel)

	ON_NOTIFY(NM_CLICK, IDC_TREE_FILE, &CTextureBuildDlg::OnNMClickTreeFile)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_FILE, &CTextureBuildDlg::OnNMRclickTreeFile)

	ON_COMMAND(ID_0_SETUPLEVEL0, &CTextureBuildDlg::On0Setuplevel0)
	ON_COMMAND(ID_0_SETUPLEVEL1, &CTextureBuildDlg::On0Setuplevel1)
	ON_COMMAND(ID_0_SETUPLEVEL2, &CTextureBuildDlg::On0Setuplevel2)
	ON_COMMAND(ID_0_SETUPLEVEL3, &CTextureBuildDlg::On0Setuplevel3)
	ON_COMMAND(ID_0_EXPORT, &CTextureBuildDlg::On0Export)
	ON_COMMAND(ID_0_TEXTURENUM, &CTextureBuildDlg::On0Texturenum)
END_MESSAGE_MAP()


// CTextureBuildDlg 메시지 처리기

BOOL CTextureBuildDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	BOOL		bResult		= FALSE;		
	char		acTemp[MAX_PATH];
	BROWSEINFO	browseInfo;
	ZeroMemory(&browseInfo, sizeof(BROWSEINFO));

	browseInfo.hwndOwner		= GetSafeHwnd();
	browseInfo.pidlRoot			= NULL;
	browseInfo.pszDisplayName	= acTemp;
	browseInfo.lpszTitle		= "select texture directory";
	browseInfo.ulFlags			= BIF_NEWDIALOGSTYLE;
	
	ITEMIDLIST* pItemIdList = ::SHBrowseForFolder(&browseInfo);
	if (pItemIdList)
	{
		GetCurrentDirectory(MAX_PATH, CGlobalVariable::acProgPath);
		SHGetPathFromIDList(pItemIdList, CGlobalVariable::acRootPath);
		SetCurrentDirectory(CGlobalVariable::acRootPath);

		CString strTemp = CGlobalVariable::acProgPath; strTemp += "\\TextureBuild.ini";
		::GetPrivateProfileString("TextureBuild", "Directory", "", acTemp, MAX_PATH, strTemp.GetBuffer());
		strTemp = acTemp;

		m_treeFile.Init();
		if (strTemp.GetLength())
		{
			int		iStart	= 0;
			int		iEnd	= 0;
			do 
			{
				iEnd = strTemp.Find(',', iStart);

				CString strTokken;
				if (iEnd > 0)
				{
					strTokken	= strTemp.Mid(iStart, iEnd - iStart);
					iStart		= iEnd + 1;
				}
				else
				{
					strTokken	= strTemp.Mid(iStart);
				}

				HTREEITEM hItem = m_treeFile.InsertDirectory(strTokken.GetBuffer());
				if (!hItem)
				{
					break;
				}
				m_treeFile.SetItemLevel(hItem, FTD_FLAG_LV0);
				m_treeFile.AllItems(hItem, SetLevelByFile, NULL);
			} while (iEnd > 0);

			if (iEnd < 0)
			{				
				return TRUE;
			}
		}
	}

	AfxMessageBox("select textrue directory");			
	OnCancel();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTextureBuildDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTextureBuildDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTextureBuildDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//OnOK();
}

void CTextureBuildDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}
void CTextureBuildDlg::OnNMClickTreeFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

void CTextureBuildDlg::OnNMRclickTreeFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	CMenu muTemp, *pContextMenu;
	CPoint point;
	UINT uFlags;
	GetCursorPos(&point);
	m_treeFile.ScreenToClient(&point);

	HTREEITEM hItem = m_treeFile.HitTest(point, &uFlags);

	if((hItem != NULL) && (uFlags & TVHT_ONITEM))
	{
		m_treeFile.SelectItem(hItem);

		muTemp.LoadMenu(IDR_POPUP_MENU);
		pContextMenu = muTemp.GetSubMenu(0);

		m_treeFile.ClientToScreen(&point);

		pContextMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x ,point.y, this);
	}
}

void CTextureBuildDlg::On0Setuplevel0()
{
	m_treeFile.SetItemLevel(m_treeFile.GetSelectedItem(), FTD_FLAG_LV0);
}

void CTextureBuildDlg::On0Setuplevel1()
{
	m_treeFile.SetItemLevel(m_treeFile.GetSelectedItem(), FTD_FLAG_LV1);
}

void CTextureBuildDlg::On0Setuplevel2()
{
	m_treeFile.SetItemLevel(m_treeFile.GetSelectedItem(), FTD_FLAG_LV2);
}

void CTextureBuildDlg::On0Setuplevel3()
{
	m_treeFile.SetItemLevel(m_treeFile.GetSelectedItem(), FTD_FLAG_LV3);
}

void CTextureBuildDlg::On0Export()
{
	unsigned int uiLevel	= FTD_FLAG_LV0;
	unsigned int uiNum		= 0;
	
	m_treeFile.AllItems(m_treeFile.GetSelectedItem(), GetExportLevel, &uiLevel);
	
	m_treeFile.AllItems(m_treeFile.GetSelectedItem(), GetItemsNum_All, &uiNum);

	m_proAllItems.SetRange(0, uiNum); m_proAllItems.SetPos(0);

	m_treeFile.AllItems(m_treeFile.GetSelectedItem(), ExportLevelTexture, &uiLevel);

	AfxMessageBox("Complete.");
}

void CTextureBuildDlg::On0Texturenum()
{
	unsigned int uiAll = 0;
	m_treeFile.AllItems(m_treeFile.GetSelectedItem(), GetItemsNum_All_Exclosion_Dir, &uiAll);

	unsigned int uiLv0 = 0;
	m_treeFile.AllItems(m_treeFile.GetSelectedItem(), GetItemsNum_Lv0, &uiLv0);

	unsigned int uiLv1 = 0;
	m_treeFile.AllItems(m_treeFile.GetSelectedItem(), GetItemsNum_Lv1, &uiLv1);

	unsigned int uiLv2 = 0;
	m_treeFile.AllItems(m_treeFile.GetSelectedItem(), GetItemsNum_Lv2, &uiLv2);

	unsigned int uiLv3 = 0;
	m_treeFile.AllItems(m_treeFile.GetSelectedItem(), GetItemsNum_Lv3, &uiLv3);

	char acTemp[1024];
	sprintf_s(acTemp, 1024, "All(%u), Lv0(%u), Lv1(%u), Lv2(%u), Lv3(%u)", uiAll, uiLv0, uiLv1, uiLv2, uiLv3);
	AfxMessageBox(acTemp);
}
