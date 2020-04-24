// Attr_COND_PCCls.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TSTool.h"
#include "Attr_COND_PCCls.h"


// CAttr_COND_PCCls 대화 상자입니다.

IMPLEMENT_SERIAL(CAttr_COND_PCCls, CAttr_Page, 1)

CAttr_COND_PCCls::CAttr_COND_PCCls()
	: CAttr_Page(CAttr_COND_PCCls::IDD)
{
	m_ButtonMap.SetAt( PC_CLASS_HUMAN_FIGHTER, &m_ctrHumanFighter );
	m_ButtonMap.SetAt( PC_CLASS_HUMAN_MYSTIC, &m_ctrHumanMystic );
	m_ButtonMap.SetAt( PC_CLASS_HUMAN_ENGINEER, &m_ctrEngineer );
	m_ButtonMap.SetAt( PC_CLASS_NAMEK_FIGHTER, &m_ctrNamekFighter );
	m_ButtonMap.SetAt( PC_CLASS_NAMEK_MYSTIC, &m_ctrNamekMystic );
	m_ButtonMap.SetAt( PC_CLASS_MIGHTY_MAJIN, &m_ctrMainMighty );
	m_ButtonMap.SetAt( PC_CLASS_WONDER_MAJIN, &m_ctrMainWonder );
	m_ButtonMap.SetAt( PC_CLASS_STREET_FIGHTER, &m_ctrStreetFighter );
	m_ButtonMap.SetAt( PC_CLASS_SWORD_MASTER, &m_ctrSwordMaster );
	m_ButtonMap.SetAt( PC_CLASS_CRANE_ROSHI, &m_ctrCraneRoshi );
	m_ButtonMap.SetAt( PC_CLASS_TURTLE_ROSHI, &m_ctrTurtleRoshi );
	m_ButtonMap.SetAt( PC_CLASS_GUN_MANIA, &m_ctrGunMania );
	m_ButtonMap.SetAt( PC_CLASS_MECH_MANIA, &m_ctrMechMania );
	m_ButtonMap.SetAt( PC_CLASS_DARK_WARRIOR, &m_ctrDarkWarrior );
	m_ButtonMap.SetAt( PC_CLASS_SHADOW_KNIGHT, &m_ctrShadowWarrior );
	m_ButtonMap.SetAt( PC_CLASS_DENDEN_HEALER, &m_ctrDenDenHealer );
	m_ButtonMap.SetAt( PC_CLASS_POCO_SUMMONER, &m_ctrPocoSummoner );
	m_ButtonMap.SetAt( PC_CLASS_GRAND_MA, &m_ctrGrandMa );
	m_ButtonMap.SetAt( PC_CLASS_ULTI_MA, &m_ctrUltiMa );
	m_ButtonMap.SetAt( PC_CLASS_PLAS_MA, &m_ctrPlasMa );
	m_ButtonMap.SetAt( PC_CLASS_KAR_MA, &m_ctrKarMa );
}

CAttr_COND_PCCls::~CAttr_COND_PCCls()
{
	m_ButtonMap.RemoveAll();
}

CString CAttr_COND_PCCls::GetPageData( void )
{
	UpdateData( true );

	CString strData;

	DWORD dwCheckFlag = 0;
	for ( int i = PC_CLASS_FIRST; i <= PC_CLASS_LAST; ++i )
	{
		dwCheckFlag |= (BST_CHECKED == m_ButtonMap[i]->GetCheck()) ? (1<<i) : 0;
	}

	strData += PakingPageData( _T("clsflg"), dwCheckFlag );

	return strData;
}

void CAttr_COND_PCCls::UnPakingPageData( CString& strKey, CString& strValue )
{
	if ( _T("clsflg") == strKey )
	{
		DWORD dwCheckFlag = atoi( strValue.GetBuffer() );

		for ( int i = PC_CLASS_FIRST; i <= PC_CLASS_LAST; ++i )
		{
			if ( dwCheckFlag & (1<<i) ) m_ButtonMap[i]->SetCheck( BST_CHECKED );
		}
	}
}

void CAttr_COND_PCCls::DoDataExchange(CDataExchange* pDX)
{
	CAttr_Page::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_ALL_CHECK, m_ctrAll);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_HUMAN_FIGHTER_CHECK, m_ctrHumanFighter);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_HUMAN_MYSTIC_CHECK, m_ctrHumanMystic);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_ENGINEER_CHECK, m_ctrEngineer);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_NAMEK_FIGHTER_CHECK, m_ctrNamekFighter);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_NAMEK_MYSTIC_CHECK, m_ctrNamekMystic);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_MIGHTY_MAIN_CHECK, m_ctrMainMighty);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_WONDER_MAIN_CHECK, m_ctrMainWonder);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_STREET_FIGHTER_CHECK, m_ctrStreetFighter);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_SWORD_MASTER_CHECK, m_ctrSwordMaster);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_CRANE_ROSHI_CHECK, m_ctrCraneRoshi);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_TURTLE_ROSHI_CHECK, m_ctrTurtleRoshi);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_GUN_MANIA_CHECK, m_ctrGunMania);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_MECH_MANIA_CHECK, m_ctrMechMania);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_DARK_WARRIOR_CHECK, m_ctrDarkWarrior);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_SHADOW_WARRIOR_CHECK, m_ctrShadowWarrior);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_DENDEN_HEALER_CHECK, m_ctrDenDenHealer);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_POCO_SUMMONER_CHECK, m_ctrPocoSummoner);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_GRAND_MA_CHECK, m_ctrGrandMa);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_ULTI_MA_CHECK, m_ctrUltiMa);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_PLAS_MA_CHECK, m_ctrPlasMa);
	DDX_Control(pDX, IDC_TS_COND_ATTR_PCCLASS_KAR_MA_CHECK, m_ctrKarMa);
}


BEGIN_MESSAGE_MAP(CAttr_COND_PCCls, CAttr_Page)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_ALL_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassAllCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_HUMAN_FIGHTER_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassHumanFighterCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_HUMAN_MYSTIC_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassHumanMysticCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_ENGINEER_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassEngineerCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_NAMEK_FIGHTER_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassNamekFighterCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_NAMEK_MYSTIC_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassNamekMysticCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_MIGHTY_MAIN_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassMightyMainCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_WONDER_MAIN_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassWonderMainCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_STREET_FIGHTER_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassStreetFighterCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_SWORD_MASTER_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassSwordMasterCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_CRANE_ROSHI_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassCraneRoshiCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_TURTLE_ROSHI_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassTurtleRoshiCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_GUN_MANIA_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassGunManiaCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_MECH_MANIA_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassMechManiaCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_DARK_WARRIOR_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassDarkWarriorCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_SHADOW_WARRIOR_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassShadowWarriorCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_DENDEN_HEALER_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassDendenHealerCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_POCO_SUMMONER_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassPocoSummonerCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_GRAND_MA_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassGrandMaCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_ULTI_MA_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassUltiMaCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_PLAS_MA_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassPlasMaCheck)
	ON_BN_CLICKED(IDC_TS_COND_ATTR_PCCLASS_KAR_MA_CHECK, &CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassKarMaCheck)
END_MESSAGE_MAP()


// CAttr_COND_PCCls 메시지 처리기입니다.

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassAllCheck()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nRet = m_ctrAll.GetCheck();
	for ( int i = PC_CLASS_FIRST; i <= PC_CLASS_LAST; ++i )
	{
		m_ButtonMap[i]->SetCheck( nRet );
	}
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassHumanFighterCheck()
{
	if ( m_ctrHumanFighter.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassHumanMysticCheck()
{
	if ( m_ctrHumanMystic.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassEngineerCheck()
{
	if ( m_ctrEngineer.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassNamekFighterCheck()
{
	if ( m_ctrNamekFighter.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassNamekMysticCheck()
{
	if ( m_ctrNamekMystic.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassMightyMainCheck()
{
	if ( m_ctrMainMighty.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassWonderMainCheck()
{
	if ( m_ctrMainWonder.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassStreetFighterCheck()
{
	if ( m_ctrStreetFighter.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassSwordMasterCheck()
{
	if ( m_ctrSwordMaster.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassCraneRoshiCheck()
{
	if ( m_ctrCraneRoshi.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassTurtleRoshiCheck()
{
	if ( m_ctrTurtleRoshi.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassGunManiaCheck()
{
	if ( m_ctrGunMania.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassMechManiaCheck()
{
	if ( m_ctrMechMania.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassDarkWarriorCheck()
{
	if ( m_ctrDarkWarrior.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassShadowWarriorCheck()
{
	if ( m_ctrShadowWarrior.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassDendenHealerCheck()
{
	if ( m_ctrDenDenHealer.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassPocoSummonerCheck()
{
	if ( m_ctrPocoSummoner.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassGrandMaCheck()
{
	if ( m_ctrGrandMa.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassUltiMaCheck()
{
	if ( m_ctrUltiMa.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassPlasMaCheck()
{
	if ( m_ctrPlasMa.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}

void CAttr_COND_PCCls::OnBnClickedTsCondAttrPcclassKarMaCheck()
{
	if ( m_ctrKarMa.GetCheck() == BST_UNCHECKED ) m_ctrAll.SetCheck( BST_UNCHECKED );
}
