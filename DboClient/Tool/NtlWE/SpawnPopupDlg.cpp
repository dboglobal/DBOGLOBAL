// SpawnPopupDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "NtlWE.h"
#include "SpawnPopupDlg.h"

#include "NtlPLWorldEntity.h"

#include "NtlWorldFieldManager.h"

#include "SpawnManager.h"


// CSpawnPopupDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSpawnPopupDlg, CDialog)

CSpawnPopupDlg::CSpawnPopupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpawnPopupDlg::IDD, pParent)
	, m_strFieldNumber(_T(""))
	, m_strNestCount(_T(""))
	, m_strCoolTime(_T(""))
	, m_strWanderRange(_T(""))
	, m_strMoveRange(_T(""))
	, m_strDelayTime(_T(""))
	, m_strPathIndex(_T(""))
	, m_strSpawnGroupIdx(_T(""))
	, m_strPlayScriptIdx(_T(""))
	, m_strPlayScriptSceneIdx(_T(""))
	, m_strAIScriptIdx(_T(""))
	, m_strAIScriptSceneIdx(_T(""))
{

}

CSpawnPopupDlg::~CSpawnPopupDlg()
{
}

void CSpawnPopupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FIELD_NUMBER, m_strFieldNumber);
	DDX_Text(pDX, IDC_INDEX, m_strIndex);
	DDX_Text(pDX, IDC_NEST_COUNT, m_strNestCount);
	DDX_Text(pDX, IDC_COOL_TIME, m_strCoolTime);
	DDX_Text(pDX, IDC_WANDER_RANGE, m_strWanderRange);
	DDX_Text(pDX, IDC_MOVE_RANGE, m_strMoveRange);
	DDX_Text(pDX, IDC_DELAY_TIME, m_strDelayTime);
	DDX_Text(pDX, IDC_PATH_INDEX, m_strPathIndex);
	DDX_Text(pDX, IDC_SPAWN_GROUP_IDX, m_strSpawnGroupIdx);
	DDX_Text(pDX, IDC_PLAYSCRIPT_IDX, m_strPlayScriptIdx);
	DDX_Text(pDX, IDC_PLAYSCRIPT_SCENE_IDX, m_strPlayScriptSceneIdx);
	DDX_Control(pDX, IDC_COMBO1, m_cbMoveType);
	DDX_Text(pDX, IDC_AISCRIPT_IDX, m_strAIScriptIdx);
	DDX_Text(pDX, IDC_AISCRIPT_SCENE_IDX, m_strAIScriptSceneIdx);
}


BEGIN_MESSAGE_MAP(CSpawnPopupDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSpawnPopupDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSpawnPopupDlg 메시지 처리기입니다.

BOOL CSpawnPopupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (GetSpawnInstance()->GetOneSpawnSelected())
	{
		const std::vector<CSpawn*>*	pvecSpawnList	= GetSpawnInstance()->GetSpawnSelected();
		CSpawn*						pSpawn			= pvecSpawnList->front();

		m_strFieldNumber.Format("%d", dGETMGR()->GetFieldIdx(pSpawn->m_T));
		m_strIndex.Format("%u",pSpawn->m_MdlGUID);
		m_strNestCount.Format("%d", pSpawn->GetNestMemberCnt());
		m_strCoolTime.Format("%d", pSpawn->GetSpawnMacro().m_CoolTime);
		m_cbMoveType.SetCurSel(pSpawn->GetSpawnMacro().m_MoveType);
		m_strWanderRange.Format("%.2f", pSpawn->GetSpawnMacro().m_WanderRange);
		m_strMoveRange.Format("%.2f", pSpawn->GetSpawnMacro().m_MoveRange);
		m_strDelayTime.Format("%.2f", pSpawn->GetSpawnMacro().m_DelayTime);
		m_strPathIndex.Format("%d", pSpawn->GetPathIndex());
		m_strSpawnGroupIdx.Format("%u", pSpawn->GetGroupIdx());
		m_strPlayScriptIdx.Format("%u", pSpawn->GetPlayScriptIdx());
		m_strPlayScriptSceneIdx.Format("%u", pSpawn->GetPlayScriptSceneIdx());			
		m_strAIScriptIdx.Format("%u", pSpawn->GetAIScriptIdx());
		m_strAIScriptSceneIdx.Format("%u", pSpawn->GetAIScriptSceneIdx());
	}
	else if (GetSpawnInstance()->GetSpawnSelectedAtLeastOnce())
	{
		m_strFieldNumber.Format("@");
		m_strIndex.Format("@");
		m_strNestCount.Format("@");
		m_strCoolTime.Format("@");
		m_cbMoveType.SetCurSel(-1);
		m_strWanderRange.Format("@");
		m_strMoveRange.Format("@");
		m_strDelayTime.Format("@");
		m_strPathIndex.Format("@");
		m_strSpawnGroupIdx.Format("@");
		m_strPlayScriptIdx.Format("@");
		m_strPlayScriptSceneIdx.Format("@");
		m_strAIScriptIdx.Format("@");
		m_strAIScriptSceneIdx.Format("@");
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CSpawnPopupDlg::PreTranslateMessage(MSG* pMsg)
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

LRESULT CSpawnPopupDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CLOSE:
		{
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CSpawnPopupDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	
	const std::vector<CSpawn*>*	pvecSpawnList	= GetSpawnInstance()->GetSpawnSelected();

	for (int i = 0; i < (int)pvecSpawnList->size(); ++i)
	{
		CSpawn*	pSpawn	= pvecSpawnList->at(i);
		if (m_strIndex != "@")
		{
			GetSpawnInstance()->ChangeSpawnModel(pSpawn, atoi(m_strIndex));
		}
		if (m_strNestCount != "@")
		{
			GetSpawnInstance()->ChangeSpawnNestCount(pSpawn, atoi(m_strNestCount));
		}
		if (m_strCoolTime != "@")
		{
			GetSpawnInstance()->ChangeSpawnCoolTime(pSpawn, atoi(m_strCoolTime));
		}
		if (m_cbMoveType.GetCurSel() != -1)
		{
			GetSpawnInstance()->ChangeSpawnMoveType(pSpawn, m_cbMoveType.GetCurSel());
		}
		if (m_strWanderRange != "@")
		{
			GetSpawnInstance()->ChangeSpawnWanderRange(pSpawn, static_cast<RwReal>(atof(m_strWanderRange)));
		}
		if (m_strMoveRange != "@")
		{
			GetSpawnInstance()->ChangeSpawnMoveRange(pSpawn,  static_cast<RwReal>(atof(m_strMoveRange)));
		}
		if (m_strDelayTime != "@")
		{
			GetSpawnInstance()->ChangeSpawnDelayTime(pSpawn,  static_cast<RwReal>(atof(m_strDelayTime)));
		}
		if (m_strPathIndex != "@")
		{
			GetSpawnInstance()->ChangeSpawnPathIndex(pSpawn, atoi(m_strPathIndex));
		}
		if (m_strSpawnGroupIdx != "@")
		{
			GetSpawnInstance()->ChangeSpawnGroupIndex(pSpawn, static_cast<DWORD>(atoi(m_strSpawnGroupIdx)));
		}
		if (m_strPlayScriptIdx != "@")
		{
			GetSpawnInstance()->ChangeSpawnPlayScriptIndex(pSpawn, static_cast<DWORD>(atoi(m_strPlayScriptIdx)));
		}
		if (m_strPlayScriptSceneIdx != "@")
		{
			GetSpawnInstance()->ChangeSpawnPlayScriptSceneIndex(pSpawn,  static_cast<DWORD>(atoi(m_strPlayScriptSceneIdx)));
		}
		if (m_strAIScriptIdx != "@")
		{
			GetSpawnInstance()->ChangeSpawnAIScriptIndex(pSpawn, static_cast<DWORD>(atoi(m_strAIScriptIdx)));
		}
		if (m_strAIScriptSceneIdx != "@")
		{
			GetSpawnInstance()->ChangeSpawnAIScriptSceneIndex(pSpawn,  static_cast<DWORD>(atoi(m_strAIScriptSceneIdx)));
		}
	}

	OnOK();
}
