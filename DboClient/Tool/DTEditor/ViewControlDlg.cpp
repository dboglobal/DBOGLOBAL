// ViewControlDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DTEditor.h"
#include "MainFrm.h"
#include "ViewControlDlg.h"
#include "DTEditorGlobal.h"
#include "DirectApp.h"

// shared
#include "TableContainer.h"
#include "WorldTable.h"
#include "NtlCharacter.h"

// core
#include "NtlMath.h"

// simulation
#include "precomp_ntlsimulation.h"
#include "NtlSLApi.h"
#include "NtlSobAvatar.h"
#include "NtlSLLogic.h"


// CViewControlDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CViewControlDlg, CDialog)

CViewControlDlg::CViewControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewControlDlg::IDD, pParent)
	, m_fTeleportX(0)
	, m_fTeleportZ(0)
	, m_fAvatarSpeed(1)
{

}

CViewControlDlg::~CViewControlDlg()
{
}

void CViewControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_WORLD_NAME, m_comboWorld);
	DDX_Text(pDX, IDC_EDIT_TELEPORT_X, m_fTeleportX);
	DDX_Text(pDX, IDC_EDIT_TELEPORT_Z, m_fTeleportZ);
	DDX_Text(pDX, IDC_EDIT_AVATAR_SPEED, m_fAvatarSpeed);
	DDX_Text(pDX, IDC_EDIT_WORLD_MIN_X, m_fWorldMinX);
	DDX_Text(pDX, IDC_EDIT_WORLD_MIN_Z, m_fWorldMinZ);
	DDX_Text(pDX, IDC_EDIT_WORLD_MAX_X, m_fWorldMaxX);
	DDX_Text(pDX, IDC_EDIT_WORLD_MAX_Z, m_fWorldMaxZ);

	DDV_MinMaxFloat(pDX, m_fTeleportX, -10000.0, 10000.0);
	DDV_MinMaxFloat(pDX, m_fTeleportZ, -10000.0, 10000.0);
	DDV_MinMaxFloat(pDX, m_fAvatarSpeed, 1, 100);
	DDX_Control(pDX, IDC_COMBO_AVATAR_RACE, m_comboRace);
	DDX_Control(pDX, IDC_COMBO_AVATAR_CLASS, m_comboClass);
	DDX_Control(pDX, IDC_COMBO_AVATAR_GENDER, m_comboGender);
}


BEGIN_MESSAGE_MAP(CViewControlDlg, CDialog)
	ON_EN_CHANGE(IDC_EDIT_TELEPORT_X, &CViewControlDlg::OnEnChangeEditTeleportX)
	ON_BN_CLICKED(IDOK, &CViewControlDlg::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_AVATAR_RACE, &CViewControlDlg::OnCbnSelchangeComboAvatarRace)
	ON_BN_CLICKED(IDCANCEL, &CViewControlDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CViewControlDlg 메시지 처리기입니다.

BOOL CViewControlDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// world table 등록.
	CWorldTable *pWorldTbl = API_GetTableContainer()->GetWorldTable();

	CTable::TABLEIT it;
	CTable::TABLEIT itbegin = pWorldTbl->Begin();
	CTable::TABLEIT itend = pWorldTbl->End();


	std::map<std::string, BYTE> mapWorld;
	std::map<std::string, BYTE>::iterator itSearch;

	sWORLD_TBLDAT *pWorldTblData = GetActiveWorldTableData();
	if( !pWorldTblData )
	{
		SetActiveWorldTableData( (sWORLD_TBLDAT*)itbegin->second );
		pWorldTblData = GetActiveWorldTableData();
	}


	for(it = itbegin; it != itend; it++)
	{
		sWORLD_TBLDAT *pWorldTblData = reinterpret_cast<sWORLD_TBLDAT*>((*it).second);

		std::string strName = pWorldTblData->szName;
		itSearch = mapWorld.find(strName);
		if(itSearch == mapWorld.end())
		{
			int nIndex = m_comboWorld.AddString(_T(pWorldTblData->szName));
			m_comboWorld.SetItemData(nIndex, (DWORD_PTR)pWorldTblData);

			mapWorld[strName] = 0;
		}
	}

	// current world name.
	SetDlgItemText( IDC_EDIT_WORLD_NAME, _T(pWorldTblData->szName) );

	int nComboNum = m_comboWorld.GetCount();
	for(int i = 0; i < nComboNum; i++)
	{
		if(pWorldTblData == (const sWORLD_TBLDAT*)m_comboWorld.GetItemData(i))
		{
			m_comboWorld.SetCurSel(i);
			break;
		}
	}

	m_fWorldMinX = pWorldTblData->vEnd.x;
	m_fWorldMinZ = pWorldTblData->vEnd.z;

	m_fWorldMaxX = pWorldTblData->vStart.x;
	m_fWorldMaxZ = pWorldTblData->vStart.z;

	if(m_eType != EWORLD_CREATE)
	{
		CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

		// avatar race/class/gender
		InitRace();

		// class
		InitClass(GetAvatarRace(), GetAvatarClass());

		// gender
		InitGender(GetAvatarRace(), GetAvatarGender());

		// teleport position

		RwV3d vPos = pSobAvatar->GetPosition();

		m_fTeleportX = vPos.x;
		m_fTeleportZ = vPos.z;

		// avatar speed
		m_fAvatarSpeed = Logic_GetFrontRunSpeed(pSobAvatar);
	}

	// control enable
	GetDlgItem(IDC_COMBO_WORLD_NAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_AVATAR_RACE)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_AVATAR_CLASS)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_AVATAR_GENDER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TELEPORT_X)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TELEPORT_Z)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_AVATAR_SPEED)->EnableWindow(FALSE);

	if(m_eType == EWORLD_CREATE)
	{
		GetDlgItem(IDC_COMBO_WORLD_NAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TELEPORT_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TELEPORT_Z)->EnableWindow(TRUE);
	}
	else if(m_eType == EWORLD_CHANGE)
	{
		GetDlgItem(IDC_COMBO_WORLD_NAME)->EnableWindow(TRUE);
	}
	else if(m_eType == EAVATAR_RACE)
	{
		GetDlgItem(IDC_COMBO_AVATAR_RACE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_AVATAR_CLASS)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_AVATAR_GENDER)->EnableWindow(TRUE);
	}
	else if(m_eType == EAVATAR_TELEPORT)
	{
		GetDlgItem(IDC_EDIT_TELEPORT_X)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TELEPORT_Z)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_AVATAR_SPEED)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CViewControlDlg::OnEnChangeEditTeleportX()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialog::OnInitDialog() 함수를 재지정하고  마스크에 OR 연산하여 설정된
	// ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출해야만
	// 해당 알림 메시지를 보냅니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CViewControlDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	switch(m_eType)
	{
	case EWORLD_CREATE:
		{
			int nIndex = m_comboWorld.GetCurSel();
			sWORLD_TBLDAT *pSelWorldTable = (sWORLD_TBLDAT*)m_comboWorld.GetItemData(nIndex);
			CDirectApplication *pApp = (CDirectApplication*)CNtlApplication::GetInstance();

			SetActiveWorldTableData(pSelWorldTable);

			pApp->m_vAvatarInitPos.x = m_fTeleportX;
			pApp->m_vAvatarInitPos.y = 400.f;
			pApp->m_vAvatarInitPos.z = m_fTeleportZ;

			break;
		}
	case EWORLD_CHANGE:
		{
			const sWORLD_TBLDAT *pActiveWorldTable = GetActiveWorldTableData();
			int nIndex = m_comboWorld.GetCurSel();
			sWORLD_TBLDAT *pSelWorldTable = (sWORLD_TBLDAT*)m_comboWorld.GetItemData(nIndex);

			if(pActiveWorldTable != pSelWorldTable)
			{
				SetActiveWorldTableData(pSelWorldTable);

				CDirectApplication *pApp = (CDirectApplication*)CNtlApplication::GetInstance();
				pApp->OnWorldChange();
			}
			break;
		}
	case EAVATAR_RACE:
		{
			int nIndex;
			RwUInt8 byRace, byClass, byGender;

			nIndex = m_comboRace.GetCurSel();
			byRace = (RwUInt8)m_comboRace.GetItemData(nIndex);

			nIndex = m_comboClass.GetCurSel();
			byClass = (RwUInt8)m_comboClass.GetItemData(nIndex);

			nIndex = m_comboGender.GetCurSel();
			byGender = (RwUInt8)m_comboGender.GetItemData(nIndex);

			SetAvatarRace(byRace); 
			SetAvatarClass(byClass);
			SetAvatarGender(byGender);

			CDirectApplication *pApp = (CDirectApplication*)CNtlApplication::GetInstance();
			pApp->OnAvatarChange();

			CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
			CSkillPane *pSkillPane = pMainFrame->GetSkillPane();
			if(pSkillPane->GetSafeHwnd())
				pSkillPane->InitSkill();

			break;
		}
	case EAVATAR_TELEPORT:
		{
			const sWORLD_TBLDAT *pActiveWorldTable = GetActiveWorldTableData();

			if(m_fTeleportX <= pActiveWorldTable->vEnd.x || m_fTeleportZ <= pActiveWorldTable->vEnd.z ||
				m_fTeleportX >= pActiveWorldTable->vStart.x || m_fTeleportZ >= pActiveWorldTable->vStart.z)
			{
				AfxMessageBox("Not Invalid Position !!!");
			}
			else
			{
				CDirectApplication *pApp = (CDirectApplication*)CNtlApplication::GetInstance();
				pApp->OnTeleport(m_fTeleportX, m_fTeleportZ);
			}
			break;
		}
	case EAVATAR_SPEED:
		{
			CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
			Logic_SetSpeed(pSobAvatar, m_fAvatarSpeed, m_fAvatarSpeed);
			break;
		}
	default:
		{
			NTL_ASSERT(false, "CViewControlDlg::OnBnClickedOk, Invalid view contolDog type");
		}
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	OnOK();
}

void CViewControlDlg::InitRace(void)
{
	int nIndex;

	// race
	nIndex = m_comboRace.AddString(_T("HUMAN"));
	m_comboRace.SetItemData(nIndex, (DWORD_PTR)RACE_HUMAN);

	nIndex = m_comboRace.AddString(_T("NAMEK"));
	m_comboRace.SetItemData(nIndex, (DWORD_PTR)RACE_NAMEK);

	nIndex = m_comboRace.AddString(_T("MAJIN"));
	m_comboRace.SetItemData(nIndex, (DWORD_PTR)RACE_MAJIN);

	RwUInt8 byRace = GetAvatarRace();
	if(byRace == (RwUInt8)RACE_HUMAN)
		m_comboRace.SetCurSel(0);
	else if(byRace == (RwUInt8)RACE_NAMEK)
		m_comboRace.SetCurSel(1);
	else if(byRace == (RwUInt8)RACE_MAJIN)
		m_comboRace.SetCurSel(2);
}


void CViewControlDlg::InitClass(RwUInt8 byRace, RwUInt8 bySelClass)
{
	m_comboClass.ResetContent();

	int nIndex;

	if(byRace == RACE_HUMAN)
	{
		nIndex = m_comboClass.AddString(_T("STREET_FIGHTER"));
		m_comboClass.SetItemData(nIndex, (DWORD_PTR)PC_CLASS_STREET_FIGHTER);

		nIndex = m_comboClass.AddString(_T("SWORD_MASTER"));
		m_comboClass.SetItemData(nIndex, (DWORD_PTR)PC_CLASS_SWORD_MASTER);

		nIndex = m_comboClass.AddString(_T("CRANE_ROSHI"));
		m_comboClass.SetItemData(nIndex, (DWORD_PTR)PC_CLASS_CRANE_ROSHI);

		nIndex = m_comboClass.AddString(_T("TURTLE_ROSHI"));
		m_comboClass.SetItemData(nIndex, (DWORD_PTR)PC_CLASS_TURTLE_ROSHI);
	}

//	nIndex = m_comboClass.AddString(_T("GUN_MANIA"));
//	m_comboClass.SetItemData(nIndex, (DWORD_PTR)PC_CLASS_GUN_MANIA);

//	nIndex = m_comboClass.AddString(_T("MECH_MANIA"));
//	m_comboClass.SetItemData(nIndex, (DWORD_PTR)PC_CLASS_MECH_MANIA);

	else if(byRace == RACE_NAMEK)
	{
		nIndex = m_comboClass.AddString(_T("DARK_WARRIOR"));
		m_comboClass.SetItemData(nIndex, (DWORD_PTR)PC_CLASS_DARK_WARRIOR);

		nIndex = m_comboClass.AddString(_T("SHADOW_KNIGHT"));
		m_comboClass.SetItemData(nIndex, (DWORD_PTR)PC_CLASS_SHADOW_KNIGHT);

		nIndex = m_comboClass.AddString(_T("POCO_SUMMONER"));
		m_comboClass.SetItemData(nIndex, (DWORD_PTR)PC_CLASS_POCO_SUMMONER);

		nIndex = m_comboClass.AddString(_T("DENDEN_HEALER"));
		m_comboClass.SetItemData(nIndex, (DWORD_PTR)PC_CLASS_DENDEN_HEALER);
	}

	else if(byRace == RACE_MAJIN)
	{
		nIndex = m_comboClass.AddString(_T("GRAND_MA"));
		m_comboClass.SetItemData(nIndex, (DWORD_PTR)PC_CLASS_GRAND_MA);

		nIndex = m_comboClass.AddString(_T("ULTI_MA"));
		m_comboClass.SetItemData(nIndex, (DWORD_PTR)PC_CLASS_ULTI_MA);

		nIndex = m_comboClass.AddString(_T("PLAS_MA"));
		m_comboClass.SetItemData(nIndex, (DWORD_PTR)PC_CLASS_PLAS_MA);

		nIndex = m_comboClass.AddString(_T("KAR_MA"));
		m_comboClass.SetItemData(nIndex, (DWORD_PTR)PC_CLASS_KAR_MA);
	}

	if(bySelClass == 0xff)
		m_comboClass.SetCurSel(0);
	else
	{
		int nNum = m_comboClass.GetCount();
		for(int i = 0; i < nNum; i++)
		{
			if(bySelClass == (RwUInt8)m_comboClass.GetItemData(i))
			{
				m_comboClass.SetCurSel(i);
				break;
			}
		}
	}
}

void CViewControlDlg::InitGender(RwUInt8 byRace, RwUInt8 bySelGender)
{
	m_comboGender.ResetContent();

	int nIndex;

	if(byRace == RACE_HUMAN)
	{
		nIndex = m_comboGender.AddString(_T("GENDER_MALE"));
		m_comboGender.SetItemData(nIndex, (DWORD_PTR)GENDER_MALE);

		nIndex = m_comboGender.AddString(_T("FEMALE"));
		m_comboGender.SetItemData(nIndex, (DWORD_PTR)GENDER_FEMALE);
	}
	else if(byRace == RACE_NAMEK)
	{
		nIndex = m_comboGender.AddString(_T("ONE_SEX"));
		m_comboGender.SetItemData(nIndex, (DWORD_PTR)GENDER_ONE_SEX);
	}
	else if(byRace == RACE_MAJIN)
	{
		nIndex = m_comboGender.AddString(_T("GENDER_MALE"));
		m_comboGender.SetItemData(nIndex, (DWORD_PTR)GENDER_MALE);

		nIndex = m_comboGender.AddString(_T("FEMALE"));
		m_comboGender.SetItemData(nIndex, (DWORD_PTR)GENDER_FEMALE);
	}

	if(bySelGender == 0xff)
		m_comboGender.SetCurSel(0);
	else
	{
		int nNum = m_comboGender.GetCount();
		for(int i = 0; i < nNum; i++)
		{
			if(bySelGender == (RwUInt8)m_comboGender.GetItemData(i))
			{
				m_comboGender.SetCurSel(i);
				break;
			}
		}
	}
}

void CViewControlDlg::OnCbnSelchangeComboAvatarRace()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int nIndex = m_comboRace.GetCurSel();
	RwUInt8 byRace = (RwUInt8)m_comboRace.GetItemData(nIndex);

	InitClass(byRace, 0xff);
	InitGender(byRace, 0xff);
}

void CViewControlDlg::OnBnClickedCancel()
{
	if( m_eType == EWORLD_CREATE )
	{
		AfxMessageBox(_T("Oops! Don't do that!"));
		return;
	}

	OnCancel();
}