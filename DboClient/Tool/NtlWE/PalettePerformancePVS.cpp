// PalettePerformancePVS.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PalettePerformancePVS.h"
#include "NtlWorldSectorPVS.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLSceneManager.h"
#include "NtlPLWorldEntity.h"
#include "ProgressWnd.h"
#include "ntlplapi.h"
#include "NtlWeControlUi.h"
#include "PerformanceChecker.h"


// CPalettePerformancePVS 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPalettePerformancePVS, CDialog)

CPalettePerformancePVS::CPalettePerformancePVS(CWnd* pParent /*=NULL*/)
	: CDialog(CPalettePerformancePVS::IDD, pParent)
	, m_PVSBMPName(_T("null"))
{

}

CPalettePerformancePVS::~CPalettePerformancePVS()
{
}

void CPalettePerformancePVS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_PVS_BMP_FILE_NAME, m_PVSBMPName);
}


BEGIN_MESSAGE_MAP(CPalettePerformancePVS, CDialog)
	ON_BN_CLICKED(IDC_LOS_FULL_BUILD, &CPalettePerformancePVS::OnBnClickedLosFullBuild)
	ON_BN_CLICKED(IDC_LOS_FULL_DESTROY, &CPalettePerformancePVS::OnBnClickedLosFullDestroy)
	ON_BN_CLICKED(ID_LOAD_PVS_BMP, &CPalettePerformancePVS::OnBnClickedLoadPvsBmp)
	ON_BN_CLICKED(IDC_REPORT, &CPalettePerformancePVS::OnBnClickedReport)
END_MESSAGE_MAP()


// CPalettePerformancePVS 메시지 처리기입니다.

BOOL CPalettePerformancePVS::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CPalettePerformancePVS::PreTranslateMessage(MSG* pMsg)
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

void CPalettePerformancePVS::InitializeWorldData()
{

}
void CPalettePerformancePVS::OnBnClickedLosFullBuild()
{
	CNtlWorldFieldManager* pFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();

	if(::AfxMessageBox("Are you sure?", MB_YESNO) == IDYES)	
	{
// 		std::vector<RwReal> vecHegiht;
// 		vecHegiht.push_back(10.0f + 20.0f + 100.0f);
// 		vecHegiht.push_back(10.0f + 20.0f + 60.0f);
// 		vecHegiht.push_back(10.0f + 20.0f + 30.0f);
// 		vecHegiht.push_back(10.0f + 20.0f);
// 		vecHegiht.push_back(10.0f);
// 		
// 		for (int iLayer = 0; iLayer < (int)vecHegiht.size(); ++iLayer)
		{				
			CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
			CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
			RwInt32					Counter		= 0;
			CProgressWnd			WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "PVS Building.");
			RwChar					Text[40];
			vector<RwInt32>			vecPVSAreaSectorCnt;
			RwInt32					TotPVSAreaCnt;

			// get the total number & sectors
			if(!CNtlWorldSectorPVS::GetPVSAreaSectorArray(vecPVSAreaSectorCnt))
			{
				DBO_TRACE(FALSE, "CPalettePerformancePVS::OnBnClickedLosFullBuild, get data faield.");
				return;
			}

			TotPVSAreaCnt = static_cast<RwInt32>(vecPVSAreaSectorCnt.size());

			// looping for building
			Counter = TotPVSAreaCnt;
			WndProgress.GoModal();
			WndProgress.SetRange(0, Counter);

			RwV3d	TempPos;
			RwInt32	FieldSectorCnt = 0;

			CPerformanceChecker Performace;
			for(RwInt32 i = 0; i < TotPVSAreaCnt; ++i)
			{
				// progress
				::sprintf_s(Text, 40, "%d's sector is now calculating...", vecPVSAreaSectorCnt[i]);
				WndProgress.SetText(Text);
				WndProgress.StepIt();
				WndProgress.PeekAndPump();

				if(!(FieldSectorCnt % 4))
				{
					// move the area for a bit more fast loading
					CNtlMath::MathRwV3dAssign(&TempPos, pFieldMgr->m_pSectors[vecPVSAreaSectorCnt[i]].DatumPoint.x, pFieldMgr->m_pSectors[vecPVSAreaSectorCnt[i]].DatumPoint.y, pFieldMgr->m_pSectors[vecPVSAreaSectorCnt[i]].DatumPoint.z);	
					pFieldMgr->SetAnotherField(TRUE);
					((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlPLWorldEntity()->SetPortalPosition(TempPos);
					CNtlWeControlUi::GetInstance().Update();
					((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.OnIdle();
				}

				// calculate PVS
				Performace.Run();
				pFieldMgr->m_pSectors[vecPVSAreaSectorCnt[i]].CreateSectorPVS(vecPVSAreaSectorCnt[i]/*, vecHegiht.at(iLayer)*/);
				Performace.Stop();
				Performace.Print("Sector PVS Test");

				++FieldSectorCnt;
			}

			vecPVSAreaSectorCnt.clear();

			// refresh PVS at current position; could work for the next frame
			pFieldMgr->RefreshPVS();

			WndProgress.SetText("recreating PVS file...");

			_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);

			FILE *pFile = NULL;

			//char acFilename[256];
			//sprintf_s(acFilename, 256, "-------.%d", (RwInt32)vecHegiht.at(iLayer));
			//fopen_s(&pFile, acFilename, "wb");
		
			fopen_s(&pFile, "-------", "wb");
			DBO_ASSERT(pFile, "file open failed. (" << "-------" << ")");

			BYTE PVSFlag;
			for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum; ++i)
			{
				PVSFlag = pFieldMgr->m_pSectors[i].m_pNtlWorldSectorPVS->GetEnable();
				fwrite(&PVSFlag, sizeof(BYTE), 1, pFile);

				if(PVSFlag)
				{
					BYTE* pTmp = pFieldMgr->m_pSectors[i].m_pNtlWorldSectorPVS->GetVisibilityNeighbor();
					fwrite(pTmp, (sizeof(BYTE) * dPVS_TOT_CELL_CNT * dPVS_LAYER_CNT), 1, pFile);
				}
			}

			fclose(pFile);

			_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

			WndProgress.SetText("Done.");
			Sleep(1000);
		}
	}
}

void CPalettePerformancePVS::OnBnClickedLosFullDestroy()
{
	CNtlWorldFieldManager* pFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();

	if(::AfxMessageBox("Are you sure?", MB_YESNO) == IDYES)	
	{
		CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
		CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
		RwInt32					Counter		= 0;
		CProgressWnd			WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "PVS Building.");
		RwChar					Text[40];
		vector<RwInt32>			vecPVSAreaSectorCnt;
		RwInt32					TotPVSAreaCnt;

		// get the total number & sectors
		if(!CNtlWorldSectorPVS::GetPVSAreaSectorArray(vecPVSAreaSectorCnt))
		{
			DBO_TRACE(FALSE, "CPalettePerformancePVS::OnBnClickedLosFullDestroy, get data faield.");
			return;
		}

		TotPVSAreaCnt = static_cast<RwInt32>(vecPVSAreaSectorCnt.size());

		// looping for destroying
		Counter = TotPVSAreaCnt;
		WndProgress.GoModal();
		WndProgress.SetRange(0, Counter);

		RwInt32	FieldSectorCnt = 0;

		for(RwInt32 i = 0; i < TotPVSAreaCnt; ++i)
		{
			// progress
			::sprintf_s(Text, 40, "%d's sector is now destroying...", vecPVSAreaSectorCnt[i]);
			WndProgress.SetText(Text);
			WndProgress.StepIt();
			WndProgress.PeekAndPump();

			// destroy PVS
			pFieldMgr->m_pSectors[vecPVSAreaSectorCnt[i]].DestroySectorPVS(vecPVSAreaSectorCnt[i]);

			++FieldSectorCnt;
		}

		vecPVSAreaSectorCnt.clear();

		// refresh PVS at current position; could work for the next frame
		pFieldMgr->RefreshPVS();

		WndProgress.SetText("recreating PVS file...");

		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);

		FILE *pFile = NULL;
		fopen_s(&pFile, "-------", "wb");

		DBO_ASSERT(pFile, "file open failed. (" << "-------" << ")");

		BYTE PVSFlag;
		for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldSectorNum * dGET_WORLD_PARAM()->WorldSectorNum; ++i)
		{
			PVSFlag = pFieldMgr->m_pSectors[i].m_pNtlWorldSectorPVS->GetEnable();
			fwrite(&PVSFlag, sizeof(BYTE), 1, pFile);

			if(PVSFlag)
			{
				BYTE* pTmp = pFieldMgr->m_pSectors[i].m_pNtlWorldSectorPVS->GetVisibilityNeighbor();
				fwrite(pTmp, (sizeof(BYTE) * dPVS_TOT_CELL_CNT * dPVS_LAYER_CNT), 1, pFile);
			}
		}

		fclose(pFile);

		_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

		WndProgress.SetText("Done.");
		Sleep(1000);
	}
}

void CPalettePerformancePVS::OnBnClickedLoadPvsBmp()
{
	static bool Floating = false;

	if(Floating)
	{
		return;
	}

	Floating = true;

	char dirBackup[NTL_MAX_DIR_PATH];
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, dirBackup);

	CFileDialog	FileDlg(TRUE, "register PVS BMP layer", "*.bmp", OFN_HIDEREADONLY, "PVS Layer File (*.bmp)|*.bmp||");

	FileDlg.m_ofn.lpstrInitialDir = "tool\\saber\\texture\\PVS";

	if(FileDlg.DoModal() == IDCANCEL)
	{
		Floating = false;
		return;
	}

	::SetCurrentDirectory(dirBackup);

	RwImageSetPath("tool/SABER/Texture/PVS/");
	RwImage* pImg = RwImageRead(FileDlg.GetFileName().GetBuffer(0));
	if(!pImg || pImg->width != dGET_WORLD_PARAM()->WorldFieldNum)
	{
		RwChar Output[NTL_MAX_DIR_PATH];
		sprintf(Output, "the filter is only used for %d fields world", pImg->width);
		AfxMessageBox(Output);
		RwImageDestroy(pImg);
		Floating = false;
		return;
	}

	// set filter
	CNtlWorldSectorPVS::SetPVSAreaFilter(pImg);

	m_PVSBMPName = FileDlg.GetFileName().GetBuffer(0);
	UpdateData(FALSE);

	Floating = false;
}

void CPalettePerformancePVS::OnBnClickedReport()
{
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
	::_chdir("Tool");
	::_chdir("Saber");
	::_chdir("Report");
	::_chdir("PVS");

	RwImage* pImage = RwImageCreate(dGET_WORLD_PARAM()->WorldFieldNum, dGET_WORLD_PARAM()->WorldFieldNum, 32);
	if(!pImage)
	{
		DBO_TRACE(FALSE, "CPalettePerformancePVS::OnBnClickedReport, image create failed.");
		return;
	}

	FILE* pFile = NULL;
	if(fopen_s(&pFile, "PVS.rep", "wt"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "PVS.rep" << ")");
		return;
	}

	RwImageAllocatePixels(pImage);

	CNtlWorldFieldManager*	pFMgr	= dGETMGR();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pFMgr->GetFields());
	DWORD*					pBits	= (DWORD*)RwImageGetPixels(pImage);
	DWORD*					pDest;
	RwInt32					CurFieldIdx;
	RwV3d					FieldDatumMidPos;
	RwBool					FieldPVSEnable;
	RwInt32					CurSectorIdx;

	for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum; ++i)
	{
		CurFieldIdx			= dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum - 1 - i;
		FieldDatumMidPos.x	= pFields[CurFieldIdx].GetSPos().x + dGET_WORLD_PARAM()->WorldSectorSize;
		FieldDatumMidPos.z	= pFields[CurFieldIdx].GetSPos().z + dGET_WORLD_PARAM()->WorldSectorSize;

		CurSectorIdx		= pFMgr->GetSectorIdx(FieldDatumMidPos);
		FieldPVSEnable		= pFMgr->m_pSectors[CurSectorIdx].m_pNtlWorldSectorPVS->GetEnable();
		pDest				= pBits + i;

		if(FieldPVSEnable)
		{
			(*pDest) = (DWORD)(255 << 24) | (DWORD)(0 << 16) | (DWORD)(0 << 8) | (DWORD)(0 << 0);
		}
		else
		{
			(*pDest) = (DWORD)(255 << 24) | (DWORD)(255 << 16) | (DWORD)(255 << 8) | (DWORD)(255 << 0);
		}

		if(FieldPVSEnable)
		{
			fprintf(pFile, "===> [ %d ] 필드에 PVS데이타가 존재합니다.\n", CurFieldIdx);
		}
		else
		{
			fprintf(pFile, "===> [ %d ] 필드에 PVS데이타가 존재하지 않습니다.\n", CurFieldIdx);
		}
	}

	RwImageWrite(pImage, "PVS.bmp");
	RwImageDestroy(pImage);
	fclose(pFile);

	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
}
