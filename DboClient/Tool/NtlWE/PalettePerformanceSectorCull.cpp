// PalettePerformanceSectorCull.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PalettePerformanceSectorCull.h"
#include "NtlPLObject.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"
#include "ISectorMethodContainer.h"
#include "SectorMethodImp.h"
#include "FieldMethodImp.h"
#include "ProgressWnd.h"


IMPLEMENT_DYNAMIC(CPalettePerformanceSectorCull, CDialog)


CPalettePerformanceSectorCull::CPalettePerformanceSectorCull(CWnd* pParent /*=NULL*/)
	: CDialog(CPalettePerformanceSectorCull::IDD, pParent)
{

}

CPalettePerformanceSectorCull::~CPalettePerformanceSectorCull()
{
}

void CPalettePerformanceSectorCull::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SECTORCULL, m_SectorCull);
	DDX_Control(pDX, IDC_SSM, m_SSMCombo);
	DDX_Control(pDX, IDC_FIELD_CHECK, m_SSMFieldCheck);
}


BEGIN_MESSAGE_MAP(CPalettePerformanceSectorCull, CDialog)
	ON_BN_CLICKED(IDC_SECTORCULL, &CPalettePerformanceSectorCull::OnBnClickedSectorcull)
	ON_BN_CLICKED(IDC_ANALYZE, &CPalettePerformanceSectorCull::OnBnClickedAnalyze)
END_MESSAGE_MAP()


// CPalettePerformanceSectorCull 메시지 처리기입니다.

BOOL CPalettePerformanceSectorCull::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_SSMCombo.ResetContent();
	m_SSMCombo.AddString("TEX_BLEND_DENSITY < 1%");
	m_SSMCombo.AddString("TEX_BLEND_DENSITY > 99%");
	m_SSMCombo.AddString("GET_RULEOUT_DOODAD(UPPER,LOWER)");
	m_SSMCombo.AddString("GET_RULEOUT_WATER(ALPHA,LOWER)");
	m_SSMCombo.AddString("GET_TILE_TRANSPARENCY");
	m_SSMCombo.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CPalettePerformanceSectorCull::PreTranslateMessage(MSG* pMsg)
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

void CPalettePerformanceSectorCull::InitializeWorldData()
{

}

void CPalettePerformanceSectorCull::OnBnClickedSectorcull()
{
	if(m_SectorCull.GetCheck())
	{
		dGETHDL()->OnSectorCullProc(0);
	}
	else
	{
		dGETHDL()->OnSectorCullProc(1);
	}
}

VOID CPalettePerformanceSectorCull::Refresh()
{
	CNtlWorldFieldManager* pMgr = dGETMGR();

	m_SectorCull.SetCheck(FALSE);

	if(dGETHDL()->m_vecSelectedSectorCullObj.size() == 1)
	{
		CNtlPLObject*	pNtlPLObj	= static_cast<CNtlPLObject*>(dGETHDL()->m_vecSelectedSectorCullObj[0]);
		RwV3d			Pos			= pNtlPLObj->GetPosition();
		RwInt32			SectorIdx	= pMgr->GetSectorIdx(Pos);

		if(SectorIdx == -1)
		{
			DBO_TRACE(FALSE, "CPalettePerformanceSectorCull::Refresh, invalid sector index.");
			return;
		}

		if(pMgr->m_pSectors[SectorIdx].m_IsCulled)
		{
			m_SectorCull.SetCheck(TRUE);
		}
	}

	UpdateData(FALSE);
}

void CPalettePerformanceSectorCull::OnBnClickedAnalyze()
{
	ISectorMethodContainer*	_pSMC;
	CNtlWorldFieldManager*	_pMgr	= dGETMGR();
	CNtlWorldField*			_pField	= const_cast<CNtlWorldField*>(_pMgr->GetFields());
	DWORD					SSFlag	= 0;
	CFileMem				_CurFileMem;

	// set a style
	if(m_SSMCombo.GetCheck(0))
	{
		SSFlag |= dSSM_FLAG_SPLAT_TEX_MIN_BLEND;
	}

	if(m_SSMCombo.GetCheck(1))
	{
		SSFlag |= dSSM_FLAG_SPLAT_TEX_MAX_BLEND;
	}

	if(m_SSMCombo.GetCheck(2))
	{
		SSFlag |= dSSM_FLAG_GET_RULE_OUT_DOODAD;
	}

	if(m_SSMCombo.GetCheck(3))
	{
		SSFlag |= dSSM_FLAG_GET_RULE_OUT_WATER;
	}

	if(m_SSMCombo.GetCheck(4))
	{
		SSFlag |= dSSM_FLAG_GET_TILE_TRANSPARENCY;
	}
	
	// create a method container
	if(m_SSMFieldCheck.GetCheck())
	{
		_pSMC = NTL_NEW CFieldMethodImp(SSFlag);
	}
	else
	{
		_pSMC = NTL_NEW CSectorMethodImp(SSFlag);
	}
	
	CProgressWnd WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "sector method generator");
	WndProgress.GoModal();
	WndProgress.SetRange(0, 5);

	WndProgress.SetText("sector method working : Analyze");
	WndProgress.StepIt();
	WndProgress.PeekAndPump();
	_pSMC->Analyze();

	WndProgress.SetText("sector method working : Execute");
	WndProgress.StepIt();
	WndProgress.PeekAndPump();
	_pSMC->Execute();
	

	WndProgress.SetText("sector method working : Report");
	WndProgress.StepIt();
	WndProgress.PeekAndPump();
	_pSMC->Report();
	
	WndProgress.StepIt();
	WndProgress.PeekAndPump();
	WndProgress.SetText("sector method working : Refresh");
	for(RwInt32 i = 0; i < 36; ++i)
	{
		_pMgr->RefreshField(_pMgr->m_Fields6x6[1][i]);
	}
	
	WndProgress.StepIt();
	WndProgress.PeekAndPump();
	WndProgress.SetText("sector method working : Done");

	NTL_DELETE(_pSMC);

	::MessageBox(NULL, "Done", "SAVER", MB_OK | MB_TOPMOST);
}