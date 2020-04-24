#include "stdafx.h"

#include "NtlWE.h"
#include "NtlWEFrm.h"
#include "NtlWEDoc.h"

#include "NtlWEControlUi.h"
#include "NtlWEController.h"
#include "NtlWEAvatarController.h"

#include "NtlWorldSectorPVS.h"

#include "CameraAttrDlg.h"

#include "UpdateEventShortKey.h"

CUpdateEventShortKey::CUpdateEventShortKey()
{
}

CUpdateEventShortKey::~CUpdateEventShortKey()
{
}

void CUpdateEventShortKey::UpdateShortKey()
{
	switch (GetSceneManager()->GetActiveWorldType())
	{
	case  AW_HEGITHFIELD:
		UpdateShortKeyOutdoor();
		break;
	case AW_RWWORLD:
		UpdateShortKeyIndoor();
		break;
	}
	UpdateShortKeyOther();
}

void CUpdateEventShortKey::UpdateShortKeyIndoor()
{
	if (dOKEY(DIK_NUMPAD1))
	{
		((CNtlWEFrm*)AfxGetMainWnd())->OnUiBlockguideline();
	}
	else if (dOKEY(DIK_NUMPAD2))
	{
		((CNtlWEFrm*)AfxGetMainWnd())->OnUiSectorguideline();
	}
	else if (dOKEY(DIK_NUMPAD3))
	{
		((CNtlWEFrm*)AfxGetMainWnd())->OnUiFieldguideline();
	}
	else if (dOKEY(DIK_NUMPAD4))
	{
		((CNtlWEFrm*)AfxGetMainWnd())->OnUiObjectwireframe();
	}
	else if (dOKEY(DIK_NUMPAD5))
	{
		((CNtlWEFrm*)AfxGetMainWnd())->OnUiObjectvisible();		
	}
	else if (dKEY(DIK_NUMPADPLUS))
	{
		CNtlPLGlobal::m_fIndoorPlaneHeight += 1.0f;
	}
	else if (dKEY(DIK_NUMPADMINUS))
	{
		CNtlPLGlobal::m_fIndoorPlaneHeight -= 1.0f;
	}

	if(dOKEY(DIK_F1))
	{
		SHELLEXECUTEINFO ExecInfo;
		ExecInfo.cbSize = sizeof(SHELLEXECUTEINFO); 
		ExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS; 
		ExecInfo.hwnd = dGETFRM()->GetSafeHwnd();
		ExecInfo.lpVerb = "open"; 
		ExecInfo.lpFile = "SABER.chm"; 
		ExecInfo.lpParameters = NULL; 

		CString strPath = dGET_WORLD_PARAM()->CurWorkingFolderName;
		strPath = strPath + "\\Tool\\SABER\\Help\\";
		ExecInfo.lpDirectory = strPath.GetBuffer(0); 
		ExecInfo.nShow = SW_MAXIMIZE; 
		ExecInfo.hInstApp = AfxGetInstanceHandle();

		ShellExecuteEx(&ExecInfo);
	}
	else if(dOKEY(DIK_F2))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPalettePerformanceOcclusion();
	}
	else if (dOKEY(DIK_F4))
	{
	}
	else if(dOKEY(DIK_F3))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPaletteSpawn();
	}
	else if(dOKEY(DIK_F5))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPaletteOrnament();
	}
	else if(dOKEY(DIK_F6))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnWorldView();
	}
	else if(dOKEY(DIK_F7))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPaletteWater();
	}
	else if(dOKEY(DIK_F8))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPaletteShadow();
	}
	else if(dOKEY(DIK_F9))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPaletteEffect();
	}
	else if(dOKEY(DIK_F10))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPaletteGameproperty();
	}
	else if(dOKEY(DIK_F11))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPaletteSE();
	}
}

void CUpdateEventShortKey::UpdateShortKeyOutdoor()
{
	if (dOKEY(DIK_NUMPAD1))
	{
		((CNtlWEFrm*)AfxGetMainWnd())->OnObjectDisplaywireframe();
	}
	else if (dOKEY(DIK_NUMPAD2))
	{
		((CNtlWEFrm*)AfxGetMainWnd())->OnUiSectorguideline();
	}
	else if (dOKEY(DIK_NUMPAD3))
	{
		((CNtlWEFrm*)AfxGetMainWnd())->OnUiFieldguideline();
	}
	else if (dOKEY(DIK_NUMPAD4))
	{
		((CNtlWEFrm*)AfxGetMainWnd())->OnUiObjectwireframe();
	}
	else if (dOKEY(DIK_NUMPAD5))
	{
		((CNtlWEFrm*)AfxGetMainWnd())->OnUiObjectvisible();		
	}
	// Test
	else if (dOKEY(DIK_NUMPAD7))
	{
		if (--CNtlPLGlobal::m_iCurrentPVSLayer < -1)
		{
			CNtlPLGlobal::m_iCurrentPVSLayer = -1;			
		}
	}
	else if (dOKEY(DIK_NUMPAD8))
	{
		CNtlPLGlobal::m_iCurrentPVSLayer = -1;			
	}
	else if (dOKEY(DIK_NUMPAD9))
	{
		if (++CNtlPLGlobal::m_iCurrentPVSLayer >= dPVS_LAYER_CNT)
		{
			CNtlPLGlobal::m_iCurrentPVSLayer = dPVS_LAYER_CNT - 1;			
		}
	}

	if(dOKEY(DIK_F1))
	{
		SHELLEXECUTEINFO ExecInfo;
		ExecInfo.cbSize = sizeof(SHELLEXECUTEINFO); 
		ExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS; 
		ExecInfo.hwnd = dGETFRM()->GetSafeHwnd();
		ExecInfo.lpVerb = "open"; 
		ExecInfo.lpFile = "SABER.chm"; 
		ExecInfo.lpParameters = NULL; 

		CString strPath = dGET_WORLD_PARAM()->CurWorkingFolderName;
		strPath = strPath + "\\Tool\\SABER\\Help\\";
		ExecInfo.lpDirectory = strPath.GetBuffer(0); 
		ExecInfo.nShow = SW_MAXIMIZE; 
		ExecInfo.hInstApp = AfxGetInstanceHandle();

		ShellExecuteEx(&ExecInfo);
	}
	else if(dOKEY(DIK_F2))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPalettePerformanceSectorCull();
	}
	else if(dOKEY(DIK_F3))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPaletteSpawn();
	}
	else if(dOKEY(DIK_F4))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPaletteTerrain();
	}
	else if(dOKEY(DIK_F5))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPaletteOrnament();
	}
	else if(dOKEY(DIK_F6))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPaletteFieldmerge();
	}
	else if(dOKEY(DIK_F7))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPaletteWater();
	}
	else if(dOKEY(DIK_F8))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPaletteShadow();
	}
	else if(dOKEY(DIK_F9))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPaletteEffect();
	}
	else if(dOKEY(DIK_F10))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPaletteNormalmapproperty();
	}
	else if(dOKEY(DIK_F11))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		dGETFRM()->OnPaletteSE();
	}
}

void CUpdateEventShortKey::UpdateShortKeyOther()
{	
	if(dOKEY(DIK_H))
	{
		dGET_WORLD_PARAM()->FlgHeightValueShow = !dGET_WORLD_PARAM()->FlgHeightValueShow;
	}
	else if(dOKEY(DIK_NUMPADENTER))
	{
		dGET_WORLD_PARAM()->FlgObjInfoShow = !dGET_WORLD_PARAM()->FlgObjInfoShow;
		dGETHDL()->m_pFieldName->Show(dGET_WORLD_PARAM()->FlgObjInfoShow);
	}
	else if(dKEY(DIK_EQUALS))
	{
		int FlgClr = (dGET_WORLD_PARAM()->ClrDayAndNight >> 16) - 1;
		if(FlgClr >= 0)
			dGET_WORLD_PARAM()->ClrDayAndNight -= 0x00010101;
		else
			dGET_WORLD_PARAM()->ClrDayAndNight = 0x00000000;
	}
	else if(dKEY(DIK_MINUS))
	{
		int FlgClr = (dGET_WORLD_PARAM()->ClrDayAndNight >> 16) + 1;
		if(FlgClr < 255)
			dGET_WORLD_PARAM()->ClrDayAndNight += 0x00010101;
		else
			dGET_WORLD_PARAM()->ClrDayAndNight = 0x00ffffff;
	}
	else if(dOKEY(DIK_1))
	{
		CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetInstance()->GetNtlGameCamera());
		dGETFRM()->m_pCameraPortal->SetCurCamMode(0);
	}
	else if(dOKEY(DIK_2))
	{
		RwFrame*	pFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
		RwMatrix*	pMatrix = RwFrameGetMatrix(pFrame);

		if(GetSceneManager()->GetAvailablePos(pMatrix->pos))
		{
			CNtlWeControlUi::GetInstance().SetActiveWeController(&CNtlWeAvatarController::GetInstance());
			dGETFRM()->m_pCameraPortal->SetCurCamMode(1);
		}
		else
		{
			AfxMessageBox("You can't get third person view here.");
		}
	}
	else if(dOKEY(DIK_3))
	{
		CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetInstance()->GetNtlTopCamera());
		dGETFRM()->m_pCameraPortal->SetCurCamMode(2);
	}
#ifdef _NTL_PROFILE_ENABLE
	else if (dOKEY(DIK_TAB))
	{
		static RwBool bShowProfiler = FALSE;

		bShowProfiler = !bShowProfiler;
		CNtlPLEventGenerator::ShowProfilerGui(bShowProfiler);
	}
#endif
 	else if (dKEY(DIK_LCONTROL) && dKEY(DIK_LALT) && dKEY(DIK_BACKSPACE))
 	{
 		dGETFRM()->OnSaberTest();
 	}
// 	else if(dOKEY(DIK_DIVIDE))
// 	{
// 		dGET_WORLD_PARAM()->LODEnable = !dGET_WORLD_PARAM()->LODEnable;
// 	}
}