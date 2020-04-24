// MiniMapPropDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "ProgressWnd.h"

#include "NtlWE.h"
#include "NtlWEFrm.h"

#include "NtlWorldInterface.h"

#include "NtlWeControlUi.h"
#include "NtlWeAvatarController.h"

#include "MiniMapManager.h"
#include "FieldUIManager.h"
#include "NtlWorldFieldManager.h"

#include "NtlPLWorldEntity.h"

#include "WorldViewDlg.h"

#include "CameraAttrDlg.h"
#include "MiniMapPropDlg.h"

// CMiniMapPropDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMiniMapPropDlg, CDialog)

CMiniMapPropDlg::CMiniMapPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMiniMapPropDlg::IDD, pParent)
{

}

CMiniMapPropDlg::~CMiniMapPropDlg()
{
}

void CMiniMapPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CREATE_ALL, m_CreateAll);
	DDX_Control(pDX, IDC_TEXSIZE, m_TexSizeCombo);
	DDX_Control(pDX, IDC_MINIMAP_INFO, m_MMPropCombo);
}


BEGIN_MESSAGE_MAP(CMiniMapPropDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_TEXSIZE, &CMiniMapPropDlg::OnCbnSelchangeTexsize)
	ON_BN_CLICKED(IDC_CREATE_ALL, &CMiniMapPropDlg::OnBnClickedCreateAll)
	ON_BN_CLICKED(IDOK, &CMiniMapPropDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMiniMapPropDlg 메시지 처리기입니다.

BOOL CMiniMapPropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMiniMapManager::GetInstance()->GetTexSize() = 512;
	m_TexSizeCombo.SetCurSel(4);
	m_CreateAll.SetCheck(FALSE);
	m_MMPropCombo.EnableWindow(FALSE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CMiniMapPropDlg::PreTranslateMessage(MSG* pMsg)
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

LRESULT CMiniMapPropDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::WindowProc(message, wParam, lParam);
}

void CMiniMapPropDlg::OnCbnSelchangeTexsize()
{
	CMiniMapManager::GetInstance()->GetTexSize() = 32 << m_TexSizeCombo.GetCurSel();

	// meter per pixel
	if(CMiniMapManager::GetInstance()->GetTexSize() == 256)
	{
		m_MMPropCombo.SetCurSel(0);
		m_MMPropCombo.EnableWindow(TRUE);
	}
	else
	{
		m_MMPropCombo.SetCurSel(-1);
		m_MMPropCombo.EnableWindow(FALSE);
	}
}

void CMiniMapPropDlg::OnBnClickedCreateAll()
{
	CMiniMapManager::GetInstance()->GetAllFields() = static_cast<RwBool>(m_CreateAll.GetCheck());
}

void CMiniMapPropDlg::OnBnClickedOk()
{
	// Minimap Pass Object List Refresh
	RwChar acBkPath[NTL_MAX_DIR_PATH];
	GetCurrentDirectory(NTL_MAX_DIR_PATH, acBkPath);
	SetCurrentDirectory(dGET_WORLD_PARAM()->CurWorkingFolderName);

	RwChar acTemp[NTL_MAX_DIR_PATH];
	sprintf_s(acTemp, NTL_MAX_DIR_PATH, "tool\\saber\\script\\minimappass.txt");
	CNtlPLGlobal::LoadMinimapPassObjectList(acTemp);

	SetCurrentDirectory(acBkPath);

	// Minimap
	RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
	RwV3d					OldPos		= *RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));
	CProgressWnd			WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "Create MiniMap");
	RwChar					StrProgress[128];
	
	CNtlPLGlobal::m_IsMinimapRendering = TRUE;

	CMiniMapManager::GetInstance()->SetMMProp(NULL);
	switch(m_MMPropCombo.GetCurSel())
	{
	case 1:
		{
			CMiniMapManager::GetInstance()->SetMMProp(dMM_PROP_MOB);
		}
		break;
	case 2:
		{
			CMiniMapManager::GetInstance()->SetMMProp((dMM_PROP_NPC | dMM_PROP_TRIGGER));
		}
		break;
	case  3:
		{
			CMiniMapManager::GetInstance()->SetMMProp(dMM_PROP_TRIGGER);
		}
		break;		
	}

	// setup progress window
	WndProgress.GoModal();

	// backup old param
	RwInt32 iOldCamMode			= dGETFRM()->m_pCameraPortal->GetCurCamMode();
	RwV2d	vOldFieldView		= *RwCameraGetViewWindow(CNtlPLGlobal::m_RwCamera);
	RwBool	bOldFog				= dGETAPP().IsWorking(eAO_FOG);
	RwBool	bOldPlantSet		= CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_PLNAT);
	RwBool	bOldVisibleDist		= CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_VISIBILTITY_DIST);
	RwBool	bOldFogActivation	= CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_FOG_ACTIVATION);
	RwBool	bOldFieldGuide		= dGETAPP().IsWorking(eAO_FIELD_GUIDE_LINE);
	RwBool	bOldPVSMode			= CNtlPLGlobal::m_UseTerrainPVSMode;


	// camera
	if (iOldCamMode != 2)
	{
		CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetInstance()->GetNtlTopCamera());
		dGETFRM()->m_pCameraPortal->SetCurCamMode(2);
	}

	if (bOldPVSMode)
	{
		GetSceneManager()->SetWorldPVSActivation(FALSE);
	}

	// fog	
	if (bOldFog)
	{
		dGETAPP().Switch(eAO_FOG);
	}
	// field guide line
	if (bOldFieldGuide)
	{
		dGETAPP().Switch(eAO_FIELD_GUIDE_LINE);
	}
	// plant set
	if (bOldPlantSet)
	{
		CNtlWorldObjVisionAnalyzer::GetInstance()->SetCheck(eNWOVA_PLNAT, FALSE);
	}
	// visible dist
	if (bOldVisibleDist)
	{
		CNtlWorldObjVisionAnalyzer::GetInstance()->SetCheck(eNWOVA_VISIBILTITY_DIST, FALSE);
	}
	// fog cull
	if (bOldFogActivation)
	{
		CNtlWorldObjVisionAnalyzer::GetInstance()->SetCheck(eNWOVA_FOG_ACTIVATION, FALSE);
	}

	// set minimap view window
	RwV2d					vFieldView;
	vFieldView.x = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
	vFieldView.y = static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
	RwCameraSetViewWindow(CNtlPLGlobal::m_RwCamera, &vFieldView);

	// create minimap
	if(!m_CreateAll.GetCheck())
	{
		if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
		{
			if(IsAField)
			{
				RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
				RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);
				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);

				WndProgress.SetRange(0, 1);
				::sprintf_s(StrProgress, 128, "%d's Field is now progressing.", pFieldMgr->GetFieldIdx(CurFieldMidPosInWorld));
				WndProgress.SetText(StrProgress);
				WndProgress.StepIt();
				WndProgress.PeekAndPump();

				CMiniMapManager::GetInstance()->Create(pFieldMgr->GetFieldIdx(CurFieldMidPosInWorld));
			}
			else
			{
				RECT RTInUI = dGETHDL()->m_pFieldUIManager->GetFieldDragInfo();
				RwV3d STmp, ETmp;
				STmp.x = static_cast<RwReal>(RTInUI.left);
				STmp.z = static_cast<RwReal>(RTInUI.top);
				ETmp.x = static_cast<RwReal>(RTInUI.right);
				ETmp.z = static_cast<RwReal>(RTInUI.bottom);
				RwV3d SPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(STmp);
				RwV3d EPos = dGETHDL()->m_pFieldUIManager->GetWorldPT(ETmp);
				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(SPos, FALSE);
				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(EPos, FALSE);

				RwInt32 NumFields = 0;
				for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
					{
						NumFields++;
					}
				}

				WndProgress.SetRange(0, NumFields);

				for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
					{
						RwV3d CurPos;
						CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
						CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);

						::sprintf_s(StrProgress, 128, "%d's Field is now progressing.", pFieldMgr->GetFieldIdx(CurPos));
						WndProgress.SetText(StrProgress);
						WndProgress.StepIt();
						WndProgress.PeekAndPump();

						CMiniMapManager::GetInstance()->Create(pFieldMgr->GetFieldIdx(CurPos));
					}
				}
			}
		}
		else if (GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
		{
			if (dGETFRM()->m_pWorldViewDlg->GetBlockMode() == EBLOCK_MODE_FIELD && !dGETFRM()->m_pWorldViewDlg->IsSelectEmpty())
			{
				RwV3d vSup, vInf;
				dGETFRM()->m_pWorldViewDlg->GetSelectBlockPos(&vSup, &vInf);

				RwInt32 iXNum, iYNum;
				dGETFRM()->m_pWorldViewDlg->GetSelectBlockNum(&iXNum, &iYNum);

				WndProgress.SetRange(0, iXNum * iYNum);

				vInf.x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldSize / 2);
				vInf.z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldSize / 2);
				vSup.x -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldSize / 2);
				vSup.z -= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldSize / 2);

				for (RwReal z = vInf.z; z <= vSup.z; z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldSize))
				{
					for (RwReal x = vInf.x; x <= vSup.x; x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldFieldSize))
					{
						RwV3d CurPos;
						CurPos.x = x;
						CurPos.z = z;

						::sprintf_s(StrProgress, 128, "%d's Field is now progressing.", pFieldMgr->GetFieldIdx(CurPos));
						WndProgress.SetText(StrProgress);
						WndProgress.StepIt();
						WndProgress.PeekAndPump();

						CMiniMapManager::GetInstance()->Create(pFieldMgr->GetFieldIdx(CurPos));
					}
				}

			}
		}		
	}
	else
	{
		RwInt32 NumAllFields = dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum;

		WndProgress.SetRange(0, NumAllFields);

		for(RwInt32 i = 0; i < NumAllFields; ++i)
		{
			::sprintf_s(StrProgress, 128, "%d's Field is now progressing.", pFieldMgr->GetFieldIdx(pFields[i].GetSPos()));
			WndProgress.SetText(StrProgress);
			WndProgress.StepIt();
			WndProgress.PeekAndPump();

			CMiniMapManager::GetInstance()->Create(pFieldMgr->GetFieldIdx(pFields[i].GetSPos()));
		}
	}

	// backup old view
	RwCameraSetViewWindow(CNtlPLGlobal::m_RwCamera, &vOldFieldView);

	// camera
	if (iOldCamMode != 2)
	{
		switch (iOldCamMode)
		{
		case 0:
			{
				CNtlWeControlUi::GetInstance().SetActiveWeController(dGETAPP().GetInstance()->GetNtlGameCamera());
				dGETFRM()->m_pCameraPortal->SetCurCamMode(0);
				break;
			}
		case 1:
			{
				RwFrame*	pFrame	= RwCameraGetFrame(CNtlPLGlobal::m_RwCamera);
				RwMatrix*	pMatrix = RwFrameGetMatrix(pFrame);

				if(GetSceneManager()->GetAvailablePos(pMatrix->pos))
				{
					CNtlWeControlUi::GetInstance().SetActiveWeController(&CNtlWeAvatarController::GetInstance());
					dGETFRM()->m_pCameraPortal->SetCurCamMode(1);
				}
				break;
			}
		}
	}
	// fog	
	if (bOldFog)
	{
		dGETAPP().Switch(eAO_FOG);
	}
	// field guide line
	if (bOldFieldGuide)
	{
		dGETAPP().Switch(eAO_FIELD_GUIDE_LINE);
	}
	// plant set
	if (bOldPlantSet)
	{
		CNtlWorldObjVisionAnalyzer::GetInstance()->SetCheck(eNWOVA_PLNAT, TRUE);
	}
	// visible dist
	if (bOldVisibleDist)
	{
		CNtlWorldObjVisionAnalyzer::GetInstance()->SetCheck(eNWOVA_VISIBILTITY_DIST, TRUE);
	}
	// fog cull
	if (bOldFogActivation)
	{
		CNtlWorldObjVisionAnalyzer::GetInstance()->SetCheck(eNWOVA_FOG_ACTIVATION, TRUE);
	}	

	if (bOldPVSMode)
	{
		GetSceneManager()->SetWorldPVSActivation(TRUE);
	}

	// portal 2 OldPos
	((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlPLWorldEntity()->SetPortalPosition(OldPos);
	CNtlWeControlUi::GetInstance().Update();

	WndProgress.SetText("Done.");
	Sleep(100);

	OnOK();

	CNtlPLGlobal::m_IsMinimapRendering = FALSE;
}
