// PaletteSpawnMacroModify.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteSpawnMacroModify.h"
#include "SpawnMacro.h"
#include "SpawnManager.h"


// CPaletteSpawnMacroModify 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteSpawnMacroModify, CDialog)

CPaletteSpawnMacroModify::CPaletteSpawnMacroModify(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteSpawnMacroModify::IDD, pParent)
	, m_StrCoolTime(_T("1"))
	, m_StrWanderRange(_T("1.0"))
	, m_StrMoveRange(_T("1.0"))
	, m_StrDelayTime(_T("1.0"))
{

}

CPaletteSpawnMacroModify::~CPaletteSpawnMacroModify()
{
}

void CPaletteSpawnMacroModify::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COOLTIME, m_StrCoolTime);
	DDX_Text(pDX, IDC_WANDER_RANGE, m_StrWanderRange);
	DDX_Text(pDX, IDC_MOVE_RANGE, m_StrMoveRange);
	DDX_Text(pDX, IDC_DELAY_TIME, m_StrDelayTime);
	DDX_Control(pDX, IDC_MOVETYPE, m_MoveTypeCombo);
}


BEGIN_MESSAGE_MAP(CPaletteSpawnMacroModify, CDialog)
	ON_BN_CLICKED(IDOK, &CPaletteSpawnMacroModify::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPaletteSpawnMacroModify::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_MOVETYPE, &CPaletteSpawnMacroModify::OnCbnSelchangeMovetype)
END_MESSAGE_MAP()


// CPaletteSpawnMacroModify 메시지 처리기입니다.

BOOL CPaletteSpawnMacroModify::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,	SWP_NOMOVE | SWP_NOSIZE);

	m_MoveTypeCombo.SetCurSel(0);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CPaletteSpawnMacroModify::PreTranslateMessage(MSG* pMsg)
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
	default:
		break;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CPaletteSpawnMacroModify::OnBnClickedOk()
{
	UpdateData(TRUE);
	
	// 현재의 속성을 선택되어 있는 모든 스폰오브젝트 매크로에 적용한다.
	CSpawnMacro CurSpawnMacro;
	CurSpawnMacro.m_CoolTime	= atoi(m_StrCoolTime.GetBuffer(0));
	CurSpawnMacro.m_MoveType	= m_MoveTypeCombo.GetCurSel();
	CurSpawnMacro.m_WanderRange = static_cast<RwReal>(atof(m_StrWanderRange.GetBuffer(0)));
	CurSpawnMacro.m_MoveRange	= static_cast<RwReal>(atof(m_StrMoveRange.GetBuffer(0)));
	CurSpawnMacro.m_DelayTime	= static_cast<RwReal>(atof(m_StrDelayTime.GetBuffer(0)));

	if(!GetSpawnInstance()->SetSpawnMacroToCurSpawns(CurSpawnMacro))
	{
		AfxMessageBox("plz select spawns first");
	}

	OnOK();
}

void CPaletteSpawnMacroModify::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnCancel();
}

void CPaletteSpawnMacroModify::OnCbnSelchangeMovetype()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
