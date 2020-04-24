// VisibilityDistPropDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "VisibilityDistPropDlg.h"
#include "FieldUIManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldMergeManager.h"
#include "progresswnd.h"
#include "NtlWEFrm.h"


// CVisibilityDistPropDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CVisibilityDistPropDlg, CDialog)

CVisibilityDistPropDlg::CVisibilityDistPropDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVisibilityDistPropDlg::IDD, pParent)
	, m_RangeMin(_T("400.00"))
	, m_RangeMax(_T("500.00"))
	, m_Dist(_T("512.00"))
{

}

CVisibilityDistPropDlg::~CVisibilityDistPropDlg()
{
}

void CVisibilityDistPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RANGE_MIN, m_RangeMin);
	DDX_Text(pDX, IDC_RANGE_MAX, m_RangeMax);
	DDX_Text(pDX, IDC_DIST, m_Dist);
	DDX_Control(pDX, IDC_APPLY_ALL, m_AllFields);
}


BEGIN_MESSAGE_MAP(CVisibilityDistPropDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CVisibilityDistPropDlg::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CVisibilityDistPropDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{		
		UpdateData(TRUE);
		UpdateData(FALSE);
	}

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

BOOL CVisibilityDistPropDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CVisibilityDistPropDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	RwBool					IsAField	= dGETHDL()->m_pFieldUIManager->IsSingleSelected();
	CNtlWorldFieldManager*	pFieldMgr	= ((CNtlWEApp*)(AfxGetApp()))->m_NtlWEApplication.GetInstance()->GetNtlPLWorldEntity()->GetWorldFieldMgr();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
	RwReal					Min			= static_cast<RwReal>(atof(m_RangeMin.GetBuffer(0)));
	RwReal					Max			= static_cast<RwReal>(atof(m_RangeMax.GetBuffer(0)));
	RwReal					Dist		= static_cast<RwReal>(atof(m_Dist.GetBuffer(0)));
	RwBool					All			= static_cast<RwBool>(m_AllFields.GetCheck());
	CProgressWnd			WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "Visibility Distance");
	RwChar					StrProgress[128];
	RwInt32					NumFields;

	WndProgress.GoModal();

	if(!m_AllFields.GetCheck())
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

			CNtlWorldMergeManager::GetInstance()->OnObjVisibilityDistChange(pFieldMgr->GetFieldIdx(CurFieldMidPosInWorld), Dist, Min, Max);
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

			NumFields = 0;
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

					CNtlWorldMergeManager::GetInstance()->OnObjVisibilityDistChange(pFieldMgr->GetFieldIdx(CurPos), Dist, Min, Max);
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

			CNtlWorldMergeManager::GetInstance()->OnObjVisibilityDistChange(pFieldMgr->GetFieldIdx(pFields[i].GetSPos()), Dist, Min, Max);
		}
	}

	WndProgress.SetText("Done.");
	Sleep(100);

	OnOK();
}
