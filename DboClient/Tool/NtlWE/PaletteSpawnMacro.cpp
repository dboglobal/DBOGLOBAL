// PaletteSpawnMacro.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteSpawnMacro.h"
#include "SpawnManager.h"
#include "SpawnMacro.h"


// CPaletteSpawnMacro 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteSpawnMacro, CDialog)

CPaletteSpawnMacro::CPaletteSpawnMacro(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteSpawnMacro::IDD, pParent)
	, m_CoolTime(_T("0"))
	, m_WanderRange(_T("0.00"))
	, m_MoveRange(_T("0.00"))
	, m_DelayTime(_T("0.00"))
{

}

CPaletteSpawnMacro::~CPaletteSpawnMacro()
{
}

void CPaletteSpawnMacro::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COOLTIME, m_CoolTime);
	DDX_Control(pDX, IDC_MOVETYPE, m_MoveType);
	DDX_Text(pDX, IDC_WANDER_RANGE, m_WanderRange);
	DDX_Text(pDX, IDC_MOVE_RANGE, m_MoveRange);
	DDX_Text(pDX, IDC_DELAY_TIME, m_DelayTime);
	DDX_Control(pDX, IDC_SPAWN_MACRO, m_SpawnMacro);
}


BEGIN_MESSAGE_MAP(CPaletteSpawnMacro, CDialog)
	ON_BN_CLICKED(IDC_CREATE, &CPaletteSpawnMacro::OnBnClickedCreate)
	ON_BN_CLICKED(IDC_DELETE, &CPaletteSpawnMacro::OnBnClickedDelete)
	ON_LBN_SELCHANGE(IDC_SPAWN_MACRO, &CPaletteSpawnMacro::OnLbnSelchangeSpawnMacro)
END_MESSAGE_MAP()


// CPaletteSpawnMacro 메시지 처리기입니다.

void CPaletteSpawnMacro::OnBnClickedCreate()
{
	UpdateData(TRUE);

	CSpawnMacro Macro;
	Macro.SetProp(	static_cast<RwInt32>(atoi(m_CoolTime.GetBuffer(0))),
					static_cast<RwInt32>(m_MoveType.GetCurSel()),
					static_cast<RwReal>(atof(m_WanderRange.GetBuffer(0))),
					static_cast<RwReal>(atof(m_MoveRange.GetBuffer(0))),
					static_cast<RwReal>(atof(m_DelayTime.GetBuffer(0))));

	GetSpawnInstance()->InsertMacro(Macro);
	RefreshSMIListCtrl();
}

void CPaletteSpawnMacro::OnBnClickedDelete()
{
	if(!m_SpawnMacro.GetCurSel())
	{
		AfxMessageBox("Default Spawn Macro Indicator can't be deleted.", MB_OK);
		return;
	}

	GetSpawnInstance()->DeleteMacro(m_SpawnMacro.GetCurSel());
	RefreshSMIListCtrl();
}

BOOL CPaletteSpawnMacro::PreTranslateMessage(MSG* pMsg)
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

VOID CPaletteSpawnMacro::RefreshSMIListCtrl()
{
	m_SpawnMacro.ResetContent();

	CSpawnMacroManager::SPAWN_MACRO_INDICATOR_ITER The = GetSpawnInstance()->SMI_BEGIN();
	while(The != GetSpawnInstance()->SMI_END())
	{
		RwInt32 Idx		= (*The).m_MacroIdx;
		CString IdxStr;

		IdxStr.Format("SPAWN MACRO INDICATOR NUM %d", Idx);
		m_SpawnMacro.AddString(IdxStr);

		++The;
	}

/*
	RwInt32 CurCnt = GetSpawnInstance()->SMI_SIZE() - 1;
	while(CurCnt >= 0)
	{
		RwInt32 Idx	= GetSpawnInstance()->SMI_ELEMENT(CurCnt).m_MacroIdx;
		CString IdxStr;

		IdxStr.Format("SPAWN MACRO INDICATOR NUM %d", Idx);
		m_SpawnMacro.AddString(IdxStr);

		--CurCnt;
	}*/

	m_SpawnMacro.SetCurSel(GetSpawnInstance()->GetCurMacroIdx());

	CSpawnMacro SM = GetSpawnInstance()->GetCurSpawnMacro();
	m_CoolTime.Format("%d", SM.m_CoolTime);
	m_MoveType.SetCurSel(SM.m_MoveType);
	m_WanderRange.Format("%.2f", SM.m_WanderRange);
	m_MoveRange.Format("%.2f", SM.m_MoveRange);
	m_DelayTime.Format("%.2f", SM.m_DelayTime);

	UpdateData(FALSE);
}

BOOL CPaletteSpawnMacro::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_MoveType.SetCurSel(0);

	RefreshSMIListCtrl();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

LRESULT CPaletteSpawnMacro::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_SHOWWINDOW:
		{
			SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,	SWP_NOMOVE | SWP_NOSIZE);

			break;
		}
	case WM_CLOSE:
		{
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CPaletteSpawnMacro::OnLbnSelchangeSpawnMacro()
{
	GetSpawnInstance()->GetCurMacroIdx() = m_SpawnMacro.GetCurSel();
	RefreshSMIListCtrl();
}
