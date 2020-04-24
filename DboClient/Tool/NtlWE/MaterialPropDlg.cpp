// MaterialPropDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"

#include "NtlWorldFieldManager.h"
#include "FieldUIManager.h"
#include "NtlPLWorldEntity.h"

#include "ProgressWnd.h"

#include "MaterialPropDlg.h"

// MaterialPropDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMaterialPropDlg, CDialog)

CMaterialPropDlg::CMaterialPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaterialPropDlg::IDD, pParent)
	, m_pNtlWorldPropMaterialController(NULL)
{
	m_pNtlWorldPropMaterialController = NTL_NEW CNtlWorldPropMaterialController;

}

CMaterialPropDlg::~CMaterialPropDlg()
{
	NTL_DELETE(m_pNtlWorldPropMaterialController);
}

void CMaterialPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMaterialPropDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CMaterialPropDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_EXPORT, &CMaterialPropDlg::OnBnClickedExport)
	ON_BN_CLICKED(IDC_DELETE, &CMaterialPropDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_ALL_DELETE, &CMaterialPropDlg::OnBnClickedAllDelete)
	ON_BN_CLICKED(IDC_ALL_UPDATE, &CMaterialPropDlg::OnBnClickedAllUpdate)
END_MESSAGE_MAP()


// MaterialPropDlg 메시지 처리기입니다.

BOOL CMaterialPropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pNtlWorldPropMaterialController->LoadScript("Tool\\Saber\\Script\\materialproperty.xml");

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CMaterialPropDlg::PreTranslateMessage(MSG* pMsg)
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

LRESULT CMaterialPropDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::WindowProc(message, wParam, lParam);
}

void CMaterialPropDlg::OnBnClickedOk()
{
	WorkProc(eWORK_UPDATE);
}

void CMaterialPropDlg::OnBnClickedExport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	WorkProc(eWORK_EXPORT);
}
void CMaterialPropDlg::OnBnClickedDelete()
{
	WorkProc(eWORK_DELETE);
}

void CMaterialPropDlg::OnBnClickedAllDelete()
{
	WorkProc(eWORK_DELETE_ALL);
}

void CMaterialPropDlg::OnBnClickedAllUpdate()
{
	WorkProc(eWORK_UPDATE_ALL);
}

void CMaterialPropDlg::WorkProc(eWORK eWork)
{
	if (eWork == eWORK_EXPORT)
	{
		CFileDialog	FileDlg(FALSE, "select save file", "Material_Property", OFN_HIDEREADONLY, "Data Files (*.bmp)|*.bmp||");
		FileDlg.m_ofn.lpstrInitialDir = "Tool\\Saber\\Texture\\Grid Map\\";

		if(FileDlg.DoModal() == IDOK)
		{
			m_pNtlWorldPropMaterialController->SaveImage(FileDlg.GetPathName().GetBuffer());
			::AfxMessageBox("Done.");
			OnOK();
		}
	}
	else if(::AfxMessageBox("Are you sure?", MB_YESNO) == IDYES)
	{
		if (eWork == eWORK_UPDATE || eWork == eWORK_DELETE)
		{
			CNtlWorldFieldManager* pFieldMgr = ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
			if(dGETHDL()->m_pFieldUIManager->IsSingleSelected())
			{
				RwV3d CurFieldMidPosInUI	= dGETHDL()->m_pFieldUIManager->GetFieldMidPos();
				RwV3d CurFieldMidPosInWorld = dGETHDL()->m_pFieldUIManager->GetWorldPT(CurFieldMidPosInUI);

				((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetNtlGameCamera()->ClampTranslation(CurFieldMidPosInWorld, FALSE);

				switch (eWork)
				{
				case eWORK_UPDATE:
					m_pNtlWorldPropMaterialController->UpdatePropertyPosInWorld(CurFieldMidPosInWorld);
					break;
				case eWORK_DELETE:
					m_pNtlWorldPropMaterialController->DeletePropertyPosInWorld(CurFieldMidPosInWorld);
					break;				
				}			
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

				CProgressWnd WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "material tile property.");
				RwInt32		 iRange = ((RwInt32)((SPos.z - dGET_WORLD_PARAM()->WorldFieldSize - EPos.z) / dGET_WORLD_PARAM()->WorldFieldSize) + 1)*
					((RwInt32)((SPos.x - dGET_WORLD_PARAM()->WorldFieldSize - EPos.x) / dGET_WORLD_PARAM()->WorldFieldSize) + 1);
				RwInt32		 iCur	= 0;

				WndProgress.GoModal();
				WndProgress.SetRange(0, iRange);	
				for(RwReal i = EPos.z; i <= SPos.z - dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldFieldSize)
				{
					for(RwReal j = EPos.x; j <= SPos.x - dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldFieldSize)
					{
						RwV3d	CurPos;
						RwChar	cText[64];
						CurPos.x = j + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);
						CurPos.z = i + (dGET_WORLD_PARAM()->WorldFieldSize / 2.0f);

						switch (eWork)
						{
						case eWORK_UPDATE:
							::sprintf_s(cText, 64, "%d / %d fields is now updating.", ++iCur, iRange);
							WndProgress.SetText(cText);
							m_pNtlWorldPropMaterialController->UpdatePropertyPosInWorld(CurPos);
							break;
						case eWORK_DELETE:
							::sprintf_s(cText, 64, "%d / %d fields is now deleting.", ++iCur, iRange);
							WndProgress.SetText(cText);
							m_pNtlWorldPropMaterialController->DeletePropertyPosInWorld(CurPos);
							break;				
						}

						WndProgress.StepIt();
						WndProgress.PeekAndPump();
					}
				}
				WndProgress.SetText("Done.");
			}
		}
		else if (eWork == eWORK_UPDATE_ALL || eWork == eWORK_DELETE_ALL)
		{
			CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
			CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
			RwInt32					iRange		= dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum;

			CProgressWnd WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "material tile property.");
			WndProgress.GoModal();
			WndProgress.SetRange(0, iRange);	
			for(int i = 0; i <= iRange ; ++i)
			{
				RwChar	cText[64];
				switch (eWork)
				{
				case eWORK_UPDATE_ALL:
					::sprintf_s(cText, 64, "%d / %d fields is now updating.", i + 1, iRange);
					WndProgress.SetText(cText);
					m_pNtlWorldPropMaterialController->UpdatePropertyPosInWorld(pFields[i].GetSPos());
					break;
				case eWORK_DELETE_ALL:
					::sprintf_s(cText, 64, "%d / %d fields is now deleting.", i + 1, iRange);
					WndProgress.SetText(cText);
					m_pNtlWorldPropMaterialController->DeletePropertyPosInWorld(pFields[i].GetSPos());
					break;				
				}

				WndProgress.StepIt();
				WndProgress.PeekAndPump();
			}
			WndProgress.SetText("Done.");
		}
		::AfxMessageBox("Done.");
		OnOK();
	}	
}
