// PaletteSpawnBase.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "PaletteSpawnBase.h"


// CPaletteSpawnBase 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPaletteSpawnBase, CDialog)

CPaletteSpawnBase::CPaletteSpawnBase(CWnd* pParent /*=NULL*/)
	: CDialog(CPaletteSpawnBase::IDD, pParent)
	, m_CurSpawnPos(_T("(0.00, 0.00, 0.00)"))
	, m_CurSpawnDir(_T("(0.00, 0.00, 0.00)"))
	, m_CurSpawnMacro(_T("(0, 0, 0, 0, 0, 0)"))
	, m_CurSpawnInfo(_T("(0, 0, 0, 0, 0, 0, 0)"))
{

}

CPaletteSpawnBase::~CPaletteSpawnBase()
{
}

void CPaletteSpawnBase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_POS, m_CurSpawnPos);
	DDX_Text(pDX, IDC_DIR, m_CurSpawnDir);
	DDX_Text(pDX, IDC_MACRO, m_CurSpawnMacro);
	DDX_Text(pDX, IDC_INFO, m_CurSpawnInfo);
}


BEGIN_MESSAGE_MAP(CPaletteSpawnBase, CDialog)
END_MESSAGE_MAP()


// CPaletteSpawnBase 메시지 처리기입니다.

LRESULT CPaletteSpawnBase::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CPaletteSpawnBase::PreTranslateMessage(MSG* pMsg)
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

BOOL CPaletteSpawnBase::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

VOID CPaletteSpawnBase::Refresh(RwV3d& _SpawnPos, RwV3d& _SpawnDir, CSpawnMacro& _SpawnMacro, CSpawn* pSpawn)
{
	m_CurSpawnPos.Format("%.2f, %.2f, %.2f", _SpawnPos.x, _SpawnPos.y, _SpawnPos.z);

	m_CurSpawnDir.Format("%.2f, %.2f, %.2f", _SpawnDir.x, _SpawnDir.y, _SpawnDir.z);

	m_CurSpawnMacro.Format("CT(%d), MT(%d), WR(%.2f), MR(%.2f), DT(%.2f)",	_SpawnMacro.m_CoolTime,
																			_SpawnMacro.m_MoveType,
																			_SpawnMacro.m_WanderRange,
																			_SpawnMacro.m_MoveRange,
																			_SpawnMacro.m_DelayTime);

	m_CurSpawnInfo.Format("NC(%d) PI(%d) SGI(%d) PSI(%d) PSSI(%d) ASI(%d) ASSI(%d)", pSpawn->GetNestMemberCnt(),
																					pSpawn->GetPathIndex(),
																					pSpawn->GetGroupIdx(),
																					pSpawn->GetPlayScriptIdx(),
																					pSpawn->GetPlayScriptSceneIdx(),
																					pSpawn->GetAIScriptIdx(),
																					pSpawn->GetAIScriptSceneIdx());

	UpdateData(FALSE);
}