// SpawnMergeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "NtlWE.h"
#include "NtlWEFrm.h"
#include "NtlWEDoc.h"
#include "NtlWEView.h"

#include "ProgressWnd.h"

#include "PaletteFieldUI.h"

#include "FieldUIManager.h"
#include "MergeUIManager.h"

#include "NtlWorldFieldManager.h"

#include "NtlPLWorldEntity.h"

#include "SpawnMergeDlg.h"

// CSpawnMergeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSpawnMergeDlg, CDialog)

CSpawnMergeDlg::CSpawnMergeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpawnMergeDlg::IDD, pParent)
{

}

CSpawnMergeDlg::~CSpawnMergeDlg()
{
	m_SpawnMergeContainerNPCDst.Free();
	m_SpawnMergeContainerMOBDst.Free();
	m_PathListMergeContainerDst.Free();

	m_SpawnMergeContainerNPCSrc.Free();
	m_SpawnMergeContainerMOBSrc.Free();
	m_PathListMergeContainerSrc.Free();
}

void CSpawnMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PATH_LIST_DST, m_ListCtrlDst);
	DDX_Control(pDX, IDC_LIST_PATH_LIST_SRC, m_ListCtrlSrc);
}


BEGIN_MESSAGE_MAP(CSpawnMergeDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CSpawnMergeDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CSpawnMergeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSpawnMergeDlg 메시지 처리기입니다.

BOOL CSpawnMergeDlg::PreTranslateMessage(MSG* pMsg)
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

BOOL CSpawnMergeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (!TestSpawnMergeReady())
	{
		OnBnClickedCancel();
	}

	if (m_bMergeSpawnPath)
	{
		UpdatePathListCtrl(&m_PathListMergeContainerDst, &m_ListCtrlDst, FALSE);
		UpdatePathListCtrl(&m_PathListMergeContainerSrc, &m_ListCtrlSrc, TRUE);
	}
	else
	{
		OnBnClickedOk();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


BOOL CSpawnMergeDlg::TestSpawnMergeReady()
{
	m_bMergeSpawn		= dGETFRM()->m_pFieldUI->m_MergeSpawn.GetCheck();
	m_bMergeSpawnPath	= dGETFRM()->m_pFieldUI->m_MergeSpawnPath.GetCheck();

	if (!m_bMergeSpawn && !m_bMergeSpawnPath)
	{
		AfxMessageBox("not merge check");
		return FALSE;
	}

	if (m_bMergeSpawn)
	{
		if (!GetMergeArea(&m_bboxMergeDst, &m_bboxMergeSrc))
		{
			AfxMessageBox("merge area error.");
			return FALSE;
		}
	}

	if (!(TestSpawnMergeFileExist(dGETFRM()->m_pFieldUI->m_MergeSpawnDstFullName, m_strNPCSpawnFileNameDst, m_strMOBSpawnFileNameDst, m_strPath4ToolFileNameDst) && 
		TestSpawnMergeFileExist(dGETFRM()->m_pFieldUI->m_MergeSpawnSrcFullName, m_strNPCSpawnFileNameSrc, m_strMOBSpawnFileNameSrc, m_strPath4ToolFileNameSrc)))
	{
		return FALSE;
	}

	char acDirOld[NTL_MAX_DIR_PATH];
	GetCurrentDirectory(NTL_MAX_DIR_PATH, acDirOld);

	RwBool bLoadSucceeded = TRUE;
		
	SetCurrentDirectory(dGETFRM()->m_pFieldUI->m_MergeSpawnDstFullName.GetBuffer());
	bLoadSucceeded &= m_SpawnMergeContainerNPCDst.Load(m_strNPCSpawnFileNameDst.GetBuffer());
	bLoadSucceeded &= m_SpawnMergeContainerMOBDst.Load(m_strMOBSpawnFileNameDst.GetBuffer());
	bLoadSucceeded &= m_PathListMergeContainerDst.Load(m_strPath4ToolFileNameDst.GetBuffer());

	SetCurrentDirectory(dGETFRM()->m_pFieldUI->m_MergeSpawnSrcFullName.GetBuffer());
	bLoadSucceeded &= m_SpawnMergeContainerNPCSrc.Load(m_strNPCSpawnFileNameSrc.GetBuffer());
	bLoadSucceeded &= m_SpawnMergeContainerMOBSrc.Load(m_strMOBSpawnFileNameSrc.GetBuffer());
	bLoadSucceeded &= m_PathListMergeContainerSrc.Load(m_strPath4ToolFileNameSrc.GetBuffer());

	GetCurrentDirectory(NTL_MAX_DIR_PATH, acDirOld);

	if (!bLoadSucceeded)
	{
		AfxMessageBox("spawn merge file load failed.");
		return FALSE;
	}

	if (m_SpawnMergeContainerNPCSrc.GetSpawnType() != eSPAWN_NPC ||
		m_SpawnMergeContainerNPCDst.GetSpawnType() != eSPAWN_NPC ||
		m_SpawnMergeContainerMOBSrc.GetSpawnType() != eSPAWN_MOB ||
		m_SpawnMergeContainerMOBDst.GetSpawnType() != eSPAWN_MOB)
	{
		AfxMessageBox("spawn type error");
		return FALSE;
	}

	return TRUE;
}

BOOL CSpawnMergeDlg::TestSpawnMergeFileExist(CString& strFolderName, CString& strNPCSpawnFileName, CString& strMOBSpawnFileName, CString& strPath4ToolFileName)
{
	char acDirOld[NTL_MAX_DIR_PATH];
	GetCurrentDirectory(NTL_MAX_DIR_PATH, acDirOld);
	SetCurrentDirectory(strFolderName.GetBuffer());

	CFileFind	findFile;
	RwBool		bWorking;
	RwBool		bExistNPCSpawn = FALSE;		
	RwBool		bExistMOBSpawn = FALSE;		
	RwBool		bExistPath4Tool= FALSE;		

	bWorking = findFile.FindFile("NPC_*.spn");
	while (bWorking)
	{
		bWorking = findFile.FindNextFile();

		if (findFile.IsDots() || findFile.IsDirectory())
		{
			continue;
		}

		bExistNPCSpawn = TRUE;

		strNPCSpawnFileName = findFile.GetFileName();
		break;
	}

	bWorking = findFile.FindFile("MOB_*.spn");
	while (bWorking)
	{
		bWorking = findFile.FindNextFile();

		if (findFile.IsDots() || findFile.IsDirectory())
		{
			continue;
		}

		bExistMOBSpawn = TRUE;

		strMOBSpawnFileName = findFile.GetFileName();
		break;
	}

	

	bWorking = findFile.FindFile("*.pth4tool");
	while (bWorking)
	{
		bWorking = findFile.FindNextFile();

		if (findFile.IsDots() || findFile.IsDirectory())
		{
			continue;
		}

		bExistPath4Tool = TRUE;

		strPath4ToolFileName = findFile.GetFileName();
		break;
	}

	if (!bExistNPCSpawn)
	{
		AfxMessageBox("not exist npc spawn file.");
	}

	if (!bExistMOBSpawn)
	{
		AfxMessageBox("not exist mob spawn file.");
	}

	if (!bExistPath4Tool)
	{
		AfxMessageBox("not exist path file.");
	}	

	SetCurrentDirectory(acDirOld);
	return bExistNPCSpawn && bExistMOBSpawn && bExistPath4Tool;
}

void CSpawnMergeDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

void CSpawnMergeDlg::OnBnClickedOk()
{
	UpdateData(FALSE);

	char acDirOld[NTL_MAX_DIR_PATH];
	GetCurrentDirectory(NTL_MAX_DIR_PATH, acDirOld);

	SetCurrentDirectory(dGETFRM()->m_pFieldUI->m_MergeSpawnDstFullName.GetBuffer());
	
	FILE* pLogFile = NULL;
	if (fopen_s(&pLogFile, "MergeSpawnLog.txt", "a+") != 0)
	{
		AfxMessageBox("log file open failed.");
		SetCurrentDirectory(acDirOld);
		return;
	}
	SetCurrentDirectory(acDirOld);

	fprintf_s(pLogFile, "[spawn merge start]\n");

	CProgressWnd WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "Spawn Merge");

	WndProgress.GoModal();
	WndProgress.SetRange(0, 11);
	
	WndProgress.SetText("Merge Ready...");
	WndProgress.StepIt();
	WndProgress.PeekAndPump();

	std::vector<RwInt32> vecPathIdxList;
	if (m_bMergeSpawnPath)
	{
		RwInt32 iItemCnt = m_ListCtrlSrc.GetItemCount();
		for (RwInt32 i = 0; i < iItemCnt; ++i)
		{
			if (m_ListCtrlSrc.GetCheck(i))
			{
				RwInt32 iIdx = m_ListCtrlSrc.GetItemData(i);
				vecPathIdxList.push_back(iIdx);
			}
		}
	}

	WndProgress.SetText("Merge Path Unlinked NPC Spawn...");
	WndProgress.StepIt();
	WndProgress.PeekAndPump();

	if (m_bMergeSpawn)
	{
		fprintf_s(pLogFile, "[spawn merge] path unlinked npc spawn merge start\n");
		m_SpawnMergeContainerNPCDst.MergePathUnlinkedSpawn(&m_bboxMergeDst, &m_bboxMergeSrc, &m_SpawnMergeContainerNPCSrc, pLogFile);
		fprintf_s(pLogFile, "[spawn merge] path unlinked npc spawn merge end\n");
	}

	WndProgress.SetText("Merge Path Unlinked MOB Spawn...");
	WndProgress.StepIt();
	WndProgress.PeekAndPump();

	if (m_bMergeSpawn)
	{
		fprintf_s(pLogFile, "[spawn merge] path unlinked mob spawn merge start\n");
		m_SpawnMergeContainerMOBDst.MergePathUnlinkedSpawn(&m_bboxMergeDst, &m_bboxMergeSrc, &m_SpawnMergeContainerMOBSrc, pLogFile);
		fprintf_s(pLogFile, "[spawn merge] path unlinked mob spawn merge end\n");
	}

	WndProgress.SetText("Update Merge Path Index List...");
	WndProgress.StepIt();
	WndProgress.PeekAndPump();

	std::vector<RwInt32> vecPathMergeInsertList = vecPathIdxList;
	if (m_bMergeSpawnPath)
	{
		fprintf_s(pLogFile, "[spawn merge] path delete start\n");
		while (TRUE)
		{
			if (m_SpawnMergeContainerNPCSrc.GetPathIdxListFromPathLinkedSpawnParty(&vecPathMergeInsertList, pLogFile) &&
				m_SpawnMergeContainerMOBSrc.GetPathIdxListFromPathLinkedSpawnParty(&vecPathMergeInsertList, pLogFile))
			{
				break;
			}
		}
		fprintf_s(pLogFile, "[spawn merge] path delete end\n");
	}

	std::vector<RwInt32> vecPathMergeDeleteList = vecPathMergeInsertList;
	if (m_bMergeSpawnPath)
	{
		fprintf_s(pLogFile, "[spawn merge] path insert start\n");
		while (TRUE)
		{
			if (m_SpawnMergeContainerNPCDst.GetPathIdxListFromPathLinkedSpawnParty(&vecPathMergeDeleteList, pLogFile) &&
				m_SpawnMergeContainerMOBDst.GetPathIdxListFromPathLinkedSpawnParty(&vecPathMergeDeleteList, pLogFile))
			{
				break;
			}
		}
		fprintf_s(pLogFile, "[spawn merge] path insert end\n");
	}

	WndProgress.SetText("Merge Path Linked NPC Spawn...");
	WndProgress.StepIt();
	WndProgress.PeekAndPump();

	if (m_bMergeSpawnPath)
	{
		fprintf_s(pLogFile, "[spawn merge] path linked npc spawn merge start\n");
		m_SpawnMergeContainerNPCDst.MergePathLinkedSpawn(&vecPathMergeDeleteList, &vecPathMergeInsertList, &m_SpawnMergeContainerNPCSrc, pLogFile);
		fprintf_s(pLogFile, "[spawn merge] path linked npc spawn merge end\n");
	}

	WndProgress.SetText("Merge Path Linked MOB Spawn...");
	WndProgress.StepIt();
	WndProgress.PeekAndPump();

	if (m_bMergeSpawnPath)
	{
		fprintf_s(pLogFile, "[spawn merge] path linked mob spawn merge start\n");
		m_SpawnMergeContainerMOBDst.MergePathLinkedSpawn(&vecPathMergeDeleteList, &vecPathMergeInsertList, &m_SpawnMergeContainerMOBSrc, pLogFile);
		fprintf_s(pLogFile, "[spawn merge] path linked mob spawn merge end\n");
	}

	WndProgress.SetText("Merge Path List...");
	WndProgress.StepIt();
	WndProgress.PeekAndPump();

	if (m_bMergeSpawnPath)
	{
		m_PathListMergeContainerDst.MergePathList(&vecPathMergeDeleteList, &vecPathMergeInsertList, &m_PathListMergeContainerSrc);
	}

	RwBool bLoadSucceeded = TRUE;

	SetCurrentDirectory(dGETFRM()->m_pFieldUI->m_MergeSpawnDstFullName.GetBuffer());
	
	WndProgress.SetText("Save NPC Spawn...");
	WndProgress.StepIt();
	WndProgress.PeekAndPump();

	bLoadSucceeded &= m_SpawnMergeContainerNPCDst.Save(m_strNPCSpawnFileNameDst.GetBuffer());

	WndProgress.SetText("Save MOB Spawn...");
	WndProgress.StepIt();
	WndProgress.PeekAndPump();

	bLoadSucceeded &= m_SpawnMergeContainerMOBDst.Save(m_strMOBSpawnFileNameDst.GetBuffer());

	WndProgress.SetText("Save Path List...");
	WndProgress.StepIt();
	WndProgress.PeekAndPump();

	if (m_bMergeSpawnPath)
	{
		bLoadSucceeded &= m_PathListMergeContainerDst.Save(m_strPath4ToolFileNameDst.GetBuffer());
	}

	GetCurrentDirectory(NTL_MAX_DIR_PATH, acDirOld);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	WndProgress.SetText("Done...");
	WndProgress.StepIt();
	WndProgress.PeekAndPump();

	fprintf_s(pLogFile, "[spawn merge end]\n");

	fclose(pLogFile);

	OnOK();
}

void CSpawnMergeDlg::UpdatePathListCtrl(CNtlPathListMergeContainer* pPathListMergeContainer, CListCtrl* pListCtrl, RwBool bUseCheckBox)
{
	pListCtrl->DeleteAllItems();
	pListCtrl->InsertColumn(0, "Idx", LVCFMT_CENTER, 40);
	pListCtrl->InsertColumn(1, "Memo", LVCFMT_LEFT, 160);
	pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
	
	if (!bUseCheckBox)
	{
		pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() & ~LVS_EX_CHECKBOXES);
	}
	
	UpdateData(FALSE);

	RwInt32 iItem = 0;
	CNtlPathListMergeContainer::PATH_LIST_ITER it = pPathListMergeContainer->GetPathListMap()->begin();
	while (it != pPathListMergeContainer->GetPathListMap()->end())
	{
		RwInt32 iIdx = it->first;
		char	acTemp[32];

		LVITEM	lvi;
		lvi.mask		= LVIF_TEXT;
		lvi.iItem		= iItem++;
		lvi.iImage		= 0;
		lvi.iSubItem	= 0;
		lvi.pszText		= itoa(iIdx, acTemp, 10);

		iItem = pListCtrl->InsertItem(&lvi);

		lvi.iItem		= iItem;
		lvi.iSubItem	= 1;
		lvi.pszText		= const_cast<RwChar*>(it->second->GetMemo());
		pListCtrl->SetItem(&lvi);

		pListCtrl->SetItemData(iItem, iIdx);

		++it;
	}
}

BOOL CSpawnMergeDlg::GetMergeArea(RwBBox* pbboxMergeDst, RwBBox* pbboxMergeSrc)
{
	if (!dGETHDL()->m_pFieldUIManager->IsSelected() && !dGETHDL()->m_pMergeUIManager->IsSelected())
	{
		return FALSE;
	}

	std::vector<RwInt32> vecMergeFieldsDst;
	std::vector<RwInt32> vecMergeFieldsSrc;
	if(dGETHDL()->m_pFieldUIManager->IsSingleSelected() && dGETHDL()->m_pMergeUIManager->IsSingleSelected())
	{
		// current world
		RwV3d CurFieldMidPosInUI4CurWorld		= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
		RwV3d CurFieldMidPosInWorld4CurWorld	= dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI4CurWorld);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld4CurWorld, FALSE);

		// merge world
		RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pMergeUIManager->GetFieldMidPos();
		RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pMergeUIManager->GetWorldPT(CurFieldMidPosInUI);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);

		vecMergeFieldsDst.push_back(dGETMGR()->GetFieldIdx(CurFieldMidPosInWorld4CurWorld));
		vecMergeFieldsSrc.push_back(dGETMGR()->GetFieldIdx(CurFieldMidPosInWorld));
	}
	else
	{
		RECT	RTInUI4CurWorld		= dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
		RECT	RTInUI				= dGETHDL()->m_pMergeUIManager->GetFieldDragInfo();
		RwInt32 GridSize			= dGETHDL()->m_pFieldUIManager->GetFieldGridSize();

		RwInt32 WidthGridNum			= abs(RTInUI.left - RTInUI.right) / GridSize;
		RwInt32 HeightColGridNum		= abs(RTInUI.top - RTInUI.bottom) / GridSize;
		RwInt32 WidthGridNum4World		= abs(RTInUI4CurWorld.left - RTInUI4CurWorld.right) / GridSize;
		RwInt32 HeightColGridNum4World	= abs(RTInUI4CurWorld.top - RTInUI4CurWorld.bottom) / GridSize;

		// current world
		RwV3d STmp4CurWorld, ETmp4CurWorld;
		STmp4CurWorld.x = static_cast<RwReal>(RTInUI4CurWorld.left);
		STmp4CurWorld.z = static_cast<RwReal>(RTInUI4CurWorld.top);
		ETmp4CurWorld.x = static_cast<RwReal>(RTInUI4CurWorld.right);
		ETmp4CurWorld.z = static_cast<RwReal>(RTInUI4CurWorld.bottom);
		RwV3d SPos4CurWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp4CurWorld);
		RwV3d EPos4CurWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp4CurWorld);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos4CurWorld, FALSE);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos4CurWorld, FALSE);

		// merge world
		RwV3d STmp, ETmp;
		STmp.x = static_cast<RwReal>(RTInUI.left - 512);
		STmp.z = static_cast<RwReal>(RTInUI.top);
		ETmp.x = static_cast<RwReal>(RTInUI.right - 512);
		ETmp.z = static_cast<RwReal>(RTInUI.bottom);
		RwV3d SPos = dGETHDL()->m_pMergeUIManager->GetWorldPT(STmp);
		RwV3d EPos = dGETHDL()->m_pMergeUIManager->GetWorldPT(ETmp);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
		((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

		for(RwReal i = EPos4CurWorld.z; i <= SPos4CurWorld.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
		{
			for(RwReal j = EPos4CurWorld.x; j <= SPos4CurWorld.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				RwV3d CurPos;
				CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
				CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);						
				vecMergeFieldsDst.push_back(dGETMGR()->GetFieldIdx(CurPos));
			}
		}

		for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
		{
			for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
			{
				RwV3d CurPos;
				CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
				CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);						
				vecMergeFieldsSrc.push_back(dGETMGR()->GetFieldIdx(CurPos));
			}
		}
	}

	RwInt32 iIdxMin, iIdxMax;
	iIdxMin = dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum; iIdxMax = 0;
	for (int i = 0; i < (int)vecMergeFieldsSrc.size(); ++i)
	{
		if (iIdxMin > vecMergeFieldsSrc.at(i))
		{
			iIdxMin = vecMergeFieldsSrc.at(i);
		}
		if (iIdxMax < vecMergeFieldsSrc.at(i))
		{
			iIdxMax = vecMergeFieldsSrc.at(i);
		}
	}

	pbboxMergeSrc->inf.x = static_cast<RwReal>(iIdxMin % dGET_WORLD_PARAM()->WorldFieldNum) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf;
	pbboxMergeSrc->inf.z = static_cast<RwReal>(iIdxMin / dGET_WORLD_PARAM()->WorldFieldNum) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf;
	pbboxMergeSrc->sup.x = static_cast<RwReal>(iIdxMax % dGET_WORLD_PARAM()->WorldFieldNum + 1) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf;
	pbboxMergeSrc->sup.z = static_cast<RwReal>(iIdxMax / dGET_WORLD_PARAM()->WorldFieldNum + 1) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf;

	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(pbboxMergeSrc->inf, FALSE);
	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(pbboxMergeSrc->sup, FALSE);

	iIdxMin = dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum; iIdxMax = 0;
	for (int i = 0; i < (int)vecMergeFieldsDst.size(); ++i)
	{
		if (iIdxMin > vecMergeFieldsDst.at(i))
		{
			iIdxMin = vecMergeFieldsDst.at(i);
		}
		if (iIdxMax < vecMergeFieldsDst.at(i))
		{
			iIdxMax = vecMergeFieldsDst.at(i);
		}
	}

	pbboxMergeDst->inf.x = static_cast<RwReal>(iIdxMin % dGET_WORLD_PARAM()->WorldFieldNum) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf;
	pbboxMergeDst->inf.z = static_cast<RwReal>(iIdxMin / dGET_WORLD_PARAM()->WorldFieldNum) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf;
	pbboxMergeDst->sup.x = static_cast<RwReal>(iIdxMax % dGET_WORLD_PARAM()->WorldFieldNum + 1) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf;
	pbboxMergeDst->sup.z = static_cast<RwReal>(iIdxMax / dGET_WORLD_PARAM()->WorldFieldNum + 1) * dGET_WORLD_PARAM()->WorldFieldSize - dGET_WORLD_PARAM()->WorldSizeHalf;

	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(pbboxMergeDst->inf, FALSE);
	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(pbboxMergeDst->sup, FALSE);

	if (pbboxMergeDst->inf.x == pbboxMergeSrc->inf.x &&
		pbboxMergeDst->inf.z == pbboxMergeSrc->inf.z &&
		pbboxMergeDst->sup.x == pbboxMergeSrc->sup.x &&
		pbboxMergeDst->sup.z == pbboxMergeSrc->sup.z)
	{
		return TRUE;
	}
	return FALSE;
}
