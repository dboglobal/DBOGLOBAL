#include "precomp_dboclient.h"
#include "ScouterMeasureDlgGui.h"


// core
#include "math.h"
#include "NtlDebug.h"

// sound
#include "GUISoundDefine.h"
// table
#include "ItemTable.h"
#include "MobTable.h"

// presentation
#include "NtlPLGuiManager.h"

// simulation
#include "NtlSob.h"
#include "NtlSobProxy.h"
#include "NtlSLEventFunc.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobAvatar.h"
#include "NtlSLGlobal.h"
#include "NtlSLLogic.h"
#include "NtlSobManager.h"
#include "NtlSobTriggerObject.h"
#include "NtlSobMonster.h"
#include "NtlSobMonsterAttr.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobPetAttr.h"
#include "NtlSobGroup.h"

#include "NtlStorageManager.h"
#include "NtlStorageGroupScouter.h"

// dbo
#include "DboPacketGenerator.h"
#include "GuiFunctor.h"
#include "DisplayStringManager.h"
#include "ChatGui.h"
#include "DialogManager.h"
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"



// Edge effect
#define dTARGET_EDGE_THICKNESS_ORIGINAL			1.f
#define dTARGET_EDGE_THICKNESS_EFFECT			2.5f
#define dTARGET_EDGE_THICKNESS_ORIGINAL_RED		0
#define dTARGET_EDGE_THICKNESS_ORIGINAL_GREEN	0
#define dTARGET_EDGE_THICKNESS_ORIGINAL_BLUE	0
#define dTARGET_EDGE_THICKNESS_EFFECT_RED		50
#define dTARGET_EDGE_THICKNESS_EFFECT_GREEN		50
#define dTARGET_EDGE_THICKNESS_EFFECT_BLUE		255

// time schudle
#define dTIME_DISPLAY_POWER				4.f

#define dTIME_LIMIT						15.f



CScouterMeasureDlgGui::CScouterMeasureDlgGui(const RwChar* pName)
	:CNtlPLGui(pName)
	, m_bOutofOrder(false)
{
	m_bShowCombatInfo = false;
}

CScouterMeasureDlgGui::~CScouterMeasureDlgGui()
{
	NTL_FUNCTION("CScouterMeasureDlgGui::~CScouterMeasureDlgGui");

	NTL_RETURNVOID();
}

RwBool CScouterMeasureDlgGui::Create()
{
	NTL_FUNCTION("CScouterMeasureDlgGui::Create");

	if (!CNtlPLGui::Create("", "gui\\ScouterMeasureDlg.srf", "gui\\ScouterMeasureDlg.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(CNtlPLGuiManager::GetInstance()->GetGuiManager());

	m_pThis = (gui::CDialog*)GetComponent("dlgMain");
	m_pThis->SetPriority(0);


	m_pFlaMeasureBg = (gui::CFlash*)GetComponent("flaMeasureBg");
	m_pFlaMeasureBg->PlayMovie(FALSE);

	m_pFlashDirect = (gui::CFlash*)GetComponent("flaMeasureDirect");
	m_pFlashDirect->PlayMovie(FALSE);

	m_pPn_AttackRate = (gui::CPanel*)GetComponent("Pn_AttackRate");
	m_pPn_DodgeRate = (gui::CPanel*)GetComponent("Pn_DodgeRate");

	m_pdlgPhysicalEnergy = (gui::CDialog*)GetComponent("dlgPhysicalEnergy");

	m_pPn_OffenceBg = (gui::CPanel*)GetComponent("Pn_OffenceBg");
	m_pPn_DefenceBg = (gui::CPanel*)GetComponent("Pn_DefenceBg");
	m_pPn_AttackEnergyName = (gui::CPanel*)GetComponent("Pn_AttackEnergyName");

	m_pPn_EnergyOffece = (gui::CPanel*)GetComponent("Pn_EnergyOffece");
	m_pPgb_PhysicalOffence = (gui::CProgressBar*)GetComponent("Pgb_PhysicalOffence");

	m_pPn_EnergyDefence = (gui::CPanel*)GetComponent("Pn_EnergyDefence");
	m_pPgb_PhysicalDefence = (gui::CProgressBar*)GetComponent("Pgb_PhysicalDefence");

	m_pStb_PhysicalOffence = (gui::CStaticBox*)GetComponent("Stb_PhysicalOffence");
	m_pStb_EnergyOffece = (gui::CStaticBox*)GetComponent("Stb_EnergyOffece");
	m_pStb_PhysicalDefence = (gui::CStaticBox*)GetComponent("Stb_PhysicalDefence");
	m_pStb_EnergyDefence = (gui::CStaticBox*)GetComponent("Stb_EnergyDefence");

	CRectangle rect;

	// Number indicating red power
	m_Number[NUMBERCOLOR_RED].Create();
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureRed_0"));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureRed_1"));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureRed_2"));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureRed_3"));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureRed_4"));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureRed_5"));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureRed_6"));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureRed_7"));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureRed_8"));
	m_Number[NUMBERCOLOR_RED].SetSurface(NC_9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureRed_9"));
	m_Number[NUMBERCOLOR_RED].SetNumGap(-5);

	// Number indicating yellow power
	m_Number[NUMBERCOLOR_YELLOW].Create();
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureYellow_0"));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureYellow_1"));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureYellow_2"));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureYellow_3"));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureYellow_4"));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureYellow_5"));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureYellow_6"));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureYellow_7"));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureYellow_8"));
	m_Number[NUMBERCOLOR_YELLOW].SetSurface(NC_9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureYellow_9"));
	m_Number[NUMBERCOLOR_YELLOW].SetNumGap(-5);

	// Number indicating white power
	m_Number[NUMBERCOLOR_WHITE].Create();
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureWhite_0"));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureWhite_1"));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureWhite_2"));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureWhite_3"));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureWhite_4"));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureWhite_5"));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureWhite_6"));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureWhite_7"));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureWhite_8"));
	m_Number[NUMBERCOLOR_WHITE].SetSurface(NC_9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterMeasureWhite_9"));
	m_Number[NUMBERCOLOR_WHITE].SetNumGap(-5);

	// Distance
	m_NumberDistance.Create();
	m_NumberDistance.SetSurface(NC_0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterUnitNum_0"));
	m_NumberDistance.SetSurface(NC_1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterUnitNum_1"));
	m_NumberDistance.SetSurface(NC_2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterUnitNum_2"));
	m_NumberDistance.SetSurface(NC_3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterUnitNum_3"));
	m_NumberDistance.SetSurface(NC_4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterUnitNum_4"));
	m_NumberDistance.SetSurface(NC_5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterUnitNum_5"));
	m_NumberDistance.SetSurface(NC_6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterUnitNum_6"));
	m_NumberDistance.SetSurface(NC_7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterUnitNum_7"));
	m_NumberDistance.SetSurface(NC_8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterUnitNum_8"));
	m_NumberDistance.SetSurface(NC_9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterUnitNum_9"));
	m_NumberDistance.SetNumGap(-4);
	m_NumberDistance.SetRate(.8f);

	m_srfDistanceMark.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterDistanceMark"));
	m_srfDistanceUnit.SetSurface(GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterDistanceUnit"));

	m_numberHitRate.Create();
	m_numberHitRate.SetSurface(NC_0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_0"));
	m_numberHitRate.SetSurface(NC_1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_1"));
	m_numberHitRate.SetSurface(NC_2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_2"));
	m_numberHitRate.SetSurface(NC_3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_3"));
	m_numberHitRate.SetSurface(NC_4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_4"));
	m_numberHitRate.SetSurface(NC_5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_5"));
	m_numberHitRate.SetSurface(NC_6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_6"));
	m_numberHitRate.SetSurface(NC_7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_7"));
	m_numberHitRate.SetSurface(NC_8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_8"));
	m_numberHitRate.SetSurface(NC_9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_9"));
	m_numberHitRate.SetNumGap(-4);
	m_numberHitRate.SetRate(.9f);

	m_numberDodge.Create();
	m_numberDodge.SetSurface(NC_0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_0"));
	m_numberDodge.SetSurface(NC_1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_1"));
	m_numberDodge.SetSurface(NC_2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_2"));
	m_numberDodge.SetSurface(NC_3, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_3"));
	m_numberDodge.SetSurface(NC_4, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_4"));
	m_numberDodge.SetSurface(NC_5, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_5"));
	m_numberDodge.SetSurface(NC_6, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_6"));
	m_numberDodge.SetSurface(NC_7, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_7"));
	m_numberDodge.SetSurface(NC_8, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_8"));
	m_numberDodge.SetSurface(NC_9, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("CommonNumber3.srf", "srfScouterEtcNum_9"));
	m_numberDodge.SetNumGap(-4);
	m_numberDodge.SetRate(.9f);

	GetNtlGuiManager()->AddUpdateFunc(this);

	// Sig
	m_slotPaint = m_pThis->SigPaint().Connect(this, &CScouterMeasureDlgGui::OnPaint);

	LinkMsg(g_EventScouter);
	LinkMsg(g_EventCharObjDelete);
	LinkMsg(g_EventSobTargetSelectRelease);

	Show(false);

	NTL_RETURN(TRUE);
}

VOID CScouterMeasureDlgGui::Destroy()
{
	NTL_FUNCTION("CScouterMeasureDlgGui::Destroy");

	GetNtlGuiManager()->RemoveUpdateFunc(this);

	Clear();

	UnLinkMsg(g_EventScouter);
	UnLinkMsg(g_EventCharObjDelete);
	UnLinkMsg(g_EventSobTargetSelectRelease);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

VOID CScouterMeasureDlgGui::Clear()
{
	MeasurePower_Destroy();
}

VOID CScouterMeasureDlgGui::StartMeasure()
{
	CNtlInventory* pInventory = GetNtlSLGlobal()->GetSobAvatar()->GetInventory();
	RwUInt32 uiSerial = pInventory->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);

	if (uiSerial == INVALID_SERIAL_ID)
		return;

	CNtlSobItem* pScouterItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(uiSerial));
	CNtlSobItemAttr* pScouterItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pScouterItem->GetSobAttr());

	if (pScouterItemAttr->GetDur() == 0)
		return;

	if (MeasurePower_Init() == FALSE)
		return;

	GetDialogManager()->OpenDialog(DIALOG_SCOUTER_MEASURE);
}

VOID CScouterMeasureDlgGui::Update(RwReal fElapsed)
{
	if (IsShow() == FALSE)
		return;

	m_pFlaMeasureBg->Update(fElapsed);
	m_pFlashDirect->Update(fElapsed);

	m_scouterData.fTimer += fElapsed;

	if (dTIME_LIMIT <= m_scouterData.fTimer)
	{
		EndDisplayImmediately();
		return;
	}

	if (m_bOutofOrder)
	{
		if (m_scouterData.fTimer >= m_scouterData.fTimeRecievePower)
		{
			EndDisplayImmediately();
			return;
		}
		else
		{
			return;
		}
	}


	if (m_scouterData.bShowScouterBack == FALSE)
		m_scouterData.fTimeDisplayPower += fElapsed;
}

RwBool CScouterMeasureDlgGui::MeasurePower_Init()
{
	SERIAL_HANDLE hTarget = Logic_GetAvatarTargetHandle();
	if (hTarget == INVALID_SERIAL_ID)
	{
		// No target
		GetAlarmManager()->AlarmMessage("DST_TARGET_NONE");
		return FALSE;
	}

	// It is already being measured.
	if(m_scouterData.hTarget == hTarget)
		return FALSE;

	CNtlSob* pSob = GetNtlSobManager()->GetSobObject(hTarget);
	NTL_ASSERT(pSob, "CScouterMeasureDlgGui::MeasurePower_Init, Not exist target of handle " << hTarget);
	if (pSob->GetClassID() == SLCLASS_AVATAR || pSob->GetClassID() == SLCLASS_PLAYER ||
		pSob->GetClassID() == SLCLASS_NPC || pSob->GetClassID() == SLCLASS_MONSTER ||
		pSob->GetClassID() == SLCLASS_PET)
	{
		m_scouterData.hTarget = hTarget;
		m_scouterData.pSobTarget = pSob;

		// 
		m_pFlaMeasureBg->RestartMovie();

		// 
		m_pFlashDirect->RestartMovie();

		Logic_PlayGUISound(GSD_SYSTEM_SCOUTER_SCAN);

		MeasurePower_AskPowertoServer();

		return TRUE;
	}

	return FALSE;
}

VOID CScouterMeasureDlgGui::MeasurePower_Destroy()
{
	// Hide the edge
	TargetEdgeEffect(FALSE);

	m_scouterData.hTarget = INVALID_SERIAL_ID;
	m_scouterData.pSobTarget = NULL;
	m_scouterData.dwDisplayFlag = 0;
	m_scouterData.fTimer = 0.f;

	m_scouterData.bShowScouterBack = true;
	m_scouterData.uiOffencePower = 0;
	m_scouterData.uiMoreMeasureCount = 0;
	m_scouterData.fTimeRecievePower = 0.f;
	m_scouterData.fTimeAdjustPower = 0.f;
	m_scouterData.fTimeDisplayPower = 0.f;
	m_scouterData.iAdjustCount = 0;
	m_scouterData.bAskPower = false;
}

VOID CScouterMeasureDlgGui::MeasurePower_Display(RwUInt32 uiPower, int nPhysOffence, int nPhysDefence, int nEnergyOffence, int nEnergyDefence, int nHitRate, int nDodge)
{
	m_scouterData.uiMoreMeasureCount += 1;
	m_scouterData.fTimeRecievePower = m_scouterData.fTimer;
	m_scouterData.uiOffencePower = uiPower;
	m_scouterData.iAdjustCount = 0;
	m_scouterData.bAskPower = false;

	m_bShowCombatInfo = Logic_ExistScouterEffect(ACTIVE_SCOUTER_CHECK_ABILITY_ATTACK);

	if (m_bShowCombatInfo)
	{
		if (nPhysOffence <= 0) 
			nPhysOffence = 1;
		if (nPhysDefence <= 0) 
			nPhysDefence = 1;
		if (nEnergyOffence <= 0) 
			nEnergyOffence = 1;
		if (nEnergyDefence <= 0) 
			nEnergyDefence = 1;

		float fTotalOffence = float(nPhysOffence + nEnergyOffence);
		float fTotalDefence = float(nPhysDefence + nEnergyDefence);

		float fPhysicalOffencePercent = ((float)nPhysOffence / fTotalOffence) * 100.f;
		float fPhysicalDefencePercent = ((float)nPhysDefence / fTotalDefence) * 100.f;

		float fEnergyOffencePercent = ((float)nEnergyOffence / fTotalOffence) * 100.f;
		float fEnergyDefencePercent = ((float)nEnergyDefence / fTotalDefence) * 100.f;

		m_pPgb_PhysicalOffence->SetPos((int)fEnergyOffencePercent);
		m_pPgb_PhysicalDefence->SetPos((int)fEnergyDefencePercent);

		m_pStb_PhysicalOffence->Format("%d%%", (int)(fPhysicalOffencePercent + 0.5f));
		m_pStb_PhysicalOffence->Show();

		m_pStb_EnergyOffece->Format("%d%%", (int)(fEnergyOffencePercent + 0.5f));
		m_pStb_EnergyOffece->Show();

		m_pStb_PhysicalDefence->Format("%d%%", (int)(fPhysicalDefencePercent + 0.5f));
		m_pStb_PhysicalDefence->Show();

		m_pStb_EnergyDefence->Format("%d%%", (int)(fEnergyDefencePercent + 0.5f));
		m_pStb_EnergyDefence->Show();

		m_pdlgPhysicalEnergy->Show();

		m_pPn_AttackRate->Show();
		m_pPn_DodgeRate->Show();

		m_numberHitRate.SetNumber(nHitRate);
		m_numberDodge.SetNumber(nDodge);
	}
	else
	{
		m_pStb_PhysicalOffence->Show(false);
		m_pStb_EnergyOffece->Show(false);
		m_pStb_PhysicalDefence->Show(false);
		m_pStb_EnergyDefence->Show(false);

		m_pdlgPhysicalEnergy->Show(false);
		m_pPn_AttackRate->Show(false);
		m_pPn_DodgeRate->Show(false);
	}



	CNtlSob* pSobObj = GetNtlSobManager()->GetSobObject(m_scouterData.hTarget);
	if (pSobObj->GetClassID() == SLCLASS_MONSTER)
	{
		CNtlSobMonsterAttr* pMonsterAttr = reinterpret_cast<CNtlSobMonsterAttr*>(pSobObj->GetSobAttr());
		sMOB_TBLDAT* pMobData = pMonsterAttr->GetMobTbl();
		RwBool bHaveScanData = FALSE;

		CNtlStorageGroupScouter* pGroupScouter = (CNtlStorageGroupScouter*)GetNtlStorageManager()->GetStorageGroup(eNTL_STORAGE_GROUP_SCOUTER);

		if (pGroupScouter->IsUseMonsterScouter(pMobData->dwMobGroup) || pMobData->bShow_Name)
			bHaveScanData = TRUE;


		pGroupScouter->UseMonsterScouter(pMobData->dwMobGroup, m_scouterData.uiOffencePower);
	//	CDboEventGenerator::TargetStatusRefresh();

		if (bHaveScanData == FALSE)
		{
			CNtlSobGroup::MapObject::iterator it;
			CNtlSobGroup* pSobGroup = GetNtlSobManager()->GetSobGroup(SLCLASS_MONSTER);

			CNtlSobMonsterAttr* pTargetMonsterAttr = NULL;
			for (it = pSobGroup->GetObjects().begin(); it != pSobGroup->GetObjects().end(); ++it)
			{
				CNtlSobMonster* pMonster = reinterpret_cast<CNtlSobMonster*>(it->second);
				pTargetMonsterAttr = reinterpret_cast<CNtlSobMonsterAttr*>(pMonster->GetSobAttr());
				sMOB_TBLDAT* pMOB_TBLDAT = pTargetMonsterAttr->GetMobTbl();

				if (pMobData->dwMobGroup == pMOB_TBLDAT->dwMobGroup)
					Logic_SetHeadNameColor(it->second);
			}
		}
	}
}

VOID CScouterMeasureDlgGui::MeasurePower_AskPowertoServer()
{
	// Make a renewal request to the server.
	GetDboGlobal()->GetGamePacketGenerator()->SendScouterMeasure(m_scouterData.hTarget);
	m_scouterData.bAskPower = true;
}

VOID CScouterMeasureDlgGui::EndDisplayImmediately()
{
	GetDialogManager()->CloseDialog(DIALOG_SCOUTER_MEASURE);
}

VOID CScouterMeasureDlgGui::TargetEdgeEffect(bool bActive)
{
	if (GetNtlSobManager()->GetSobObject(m_scouterData.hTarget) == NULL)
		return;

	CNtlSobProxy* pSobProxy = m_scouterData.pSobTarget->GetSobProxy();
	NTL_ASSERT(pSobProxy, "CScouterMeasureDlgGui::TargetEdgeEffect, Not exist sob proxy of handle : " << m_scouterData.pSobTarget->GetSerialID());

	if (bActive)
	{
		pSobProxy->SetInkColor(dTARGET_EDGE_THICKNESS_EFFECT_RED,
			dTARGET_EDGE_THICKNESS_EFFECT_GREEN,
			dTARGET_EDGE_THICKNESS_EFFECT_BLUE);
		pSobProxy->SetInkThicknessWeight(dTARGET_EDGE_THICKNESS_EFFECT);
	}
	else
	{
		pSobProxy->SetInkColor(dTARGET_EDGE_THICKNESS_ORIGINAL_RED,
			dTARGET_EDGE_THICKNESS_ORIGINAL_GREEN,
			dTARGET_EDGE_THICKNESS_ORIGINAL_BLUE);
		pSobProxy->SetInkThicknessWeight(dTARGET_EDGE_THICKNESS_ORIGINAL);
	}
}

VOID CScouterMeasureDlgGui::OnPaint()
{
	if (m_scouterData.hTarget == INVALID_SERIAL_ID)
		return;

	CPos focusPos;
	RwV3d v3Pos;
	RwReal fTargetHeight;
	CRectangle rect;
	RwUInt8 byNumberColor;

	CNtlSobAvatarAttr* pAttr = reinterpret_cast<CNtlSobAvatarAttr*>(GetNtlSLGlobal()->GetSobAvatar()->GetSobAttr());

	RwReal fPowerRate = (RwReal)m_scouterData.uiOffencePower / (RwReal)pAttr->GetAP();

	v3Pos = m_scouterData.pSobTarget->GetPosition();
	fTargetHeight = m_scouterData.pSobTarget->GetSobProxy()->GetPLEntityHeight();

	if (fPowerRate >= 1.6f)
	{
		byNumberColor = NUMBERCOLOR_RED;
	}
	else if (fPowerRate >= 1.3f)
	{
		byNumberColor = NUMBERCOLOR_YELLOW;
	}
	else
	{
		byNumberColor = NUMBERCOLOR_WHITE;
	}

	if (GetGuiFuntor()->Calc3DPosTo2D(v3Pos, fTargetHeight / 2, NULL, &focusPos))
	{
		rect.left = focusPos.x - 800 / 2;
		rect.top = focusPos.y - 800 / 2;
		rect.right = focusPos.x + 800 / 2;
		rect.bottom = focusPos.y + 800 / 2;

		m_pFlaMeasureBg->Show(true);
		m_pFlaMeasureBg->SetPosition(rect);
		m_pFlashDirect->Show(true);
		m_pFlashDirect->SetPosition(rect);

		RwInt32 iPosX, iPosY;
		CRectangle rtRect;
		RwV3d v3Distance;

		// adjust
		iPosX = focusPos.x - 85;
		iPosY = focusPos.y - 80;

		// Power Level
		m_Number[byNumberColor].SetNumber(m_scouterData.uiOffencePower);
		m_Number[byNumberColor].SetPosition(iPosX, iPosY);
		m_Number[byNumberColor].Render();

		// 'M'
		rtRect = m_Number[byNumberColor].GetScreenRect();
		iPosX = rtRect.right;
		iPosY = rtRect.bottom + 5;
		m_srfDistanceUnit.SetPosition(iPosX, iPosY);
		m_srfDistanceUnit.Render();

		// Distance Number
		m_srfDistanceUnit.GetRect(rtRect);
		v3Distance = m_scouterData.pSobTarget->GetPosition() - GetNtlSLGlobal()->GetSobAvatar()->GetPosition();

		m_NumberDistance.SetNumber((RwUInt32)RwV3dLength(&v3Distance));
		m_NumberDistance.SetPosition(rtRect.left - 5, iPosY);
		m_NumberDistance.Render();

		// Distance Mark
		rtRect = m_NumberDistance.GetScreenRect();
		m_srfDistanceMark.SetPosition(rtRect.left - m_srfDistanceMark.GetWidth() - 5, iPosY);
		m_srfDistanceMark.Render();

		// stats
		if (m_bShowCombatInfo)
		{
			rtRect = m_Number[byNumberColor].GetScreenRect();

			// Hit Rate Number
			m_numberHitRate.SetPosition(rtRect.right + 20, iPosY + 30);
			m_numberHitRate.Render();
			// Hit Rate Text
			m_pPn_AttackRate->SetPosition(m_numberHitRate.GetScreenRect().left - m_pPn_AttackRate->GetWidth() - 10, iPosY + 29);

			// Dodge Number
			m_numberDodge.SetPosition(rtRect.right + 20, iPosY + 48);
			m_numberDodge.Render();
			// Dodge Text
			m_pPn_DodgeRate->SetPosition(m_numberDodge.GetScreenRect().left - m_pPn_DodgeRate->GetWidth() - 10, iPosY + 47);

			// Combat Info
			iPosX = ((rect.left + rect.right) / 2) - (m_pdlgPhysicalEnergy->GetWidth() / 2);

			m_pdlgPhysicalEnergy->SetPosition(iPosX, focusPos.y + 70);
			m_pdlgPhysicalEnergy->Show();

			m_pPn_AttackRate->Show();
			m_pPn_DodgeRate->Show();
		}
	}
	else
	{
		m_pFlaMeasureBg->Show(false);
		m_pFlashDirect->Show(false);
		m_pdlgPhysicalEnergy->Show(false);
		m_pPn_AttackRate->Show(false);
		m_pPn_DodgeRate->Show(false);
	}
}

RwInt32 CScouterMeasureDlgGui::SwitchDialog(bool bOpen)
{
	m_bOutofOrder = false;

	if (bOpen)
	{
		Show(true);
	}
	else
	{
		Clear();
		Show(false);
	}

	return TRUE;
}

VOID CScouterMeasureDlgGui::HandleEvents(RWS::CMsg& msg)
{
	NTL_FUNCTION("CScouterMeasureDlgGui::HandleEvents");


	if (msg.Id == g_EventScouter)
	{
		SDboEventScouter* pEvent = (SDboEventScouter*)msg.pData;

		switch (pEvent->iType)
		{
			case SCOUTER_EVENT_STATUS:
			{
				if (pEvent->iUserData2 == false && m_scouterData.hTarget != INVALID_SERIAL_ID)
				{
					EndDisplayImmediately();
				}
			}
			break;
			case SCOUTER_EVENT_MEASURE_POWER_START:
			{
				if (m_scouterData.hTarget != INVALID_SERIAL_ID)
					EndDisplayImmediately();

				StartMeasure();
			}
			break;
			case SCOUTER_EVENT_MEASURE_MEASURE_RESULT:
			{
					if (m_scouterData.hTarget != pEvent->uiUserData)
						return;

					if (m_scouterData.uiMoreMeasureCount == 0)
					{
						// Use the scouter to measure your combat power for the tutorial.
						SERIAL_HANDLE hHandle = GetNtlSLGlobal()->GetSobAvatar()->GetInventory()->GetEquipItem(EQUIP_SLOT_TYPE_SCOUTER);
						sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromSob(hHandle);
						RwUInt32 uiClassID = m_scouterData.pSobTarget->GetClassID();
						RwUInt32 uiTargetTableIndex = 0xffffffff;

						switch (uiClassID)
						{
						case SLCLASS_AVATAR:
						{
							NTL_RETURNVOID();
						}
						break;
						case SLCLASS_PLAYER:
						{
							CNtlSobPlayerAttr* pPlayer = reinterpret_cast<CNtlSobPlayerAttr*>(m_scouterData.pSobTarget->GetSobAttr());
							uiTargetTableIndex = pPlayer->GetPcTbl()->tblidx;
						}
						break;
						case SLCLASS_NPC:
						{
							CNtlSobNpcAttr* pNPC = reinterpret_cast<CNtlSobNpcAttr*>(m_scouterData.pSobTarget->GetSobAttr());
							uiTargetTableIndex = pNPC->GetNpcTbl()->tblidx;
						}
						break;
						case SLCLASS_MONSTER:
						{
							CNtlSobMonsterAttr* pMonster = reinterpret_cast<CNtlSobMonsterAttr*>(m_scouterData.pSobTarget->GetSobAttr());
							uiTargetTableIndex = pMonster->GetMobTbl()->dwMobGroup;
						}
						break;
						case SLCLASS_PET:
						{
							CNtlSobPetAttr* pPet = reinterpret_cast<CNtlSobPetAttr*>(m_scouterData.pSobTarget->GetSobAttr());
							uiTargetTableIndex = pPet->GetNpcTbl()->tblidx;
						}
						break;
						default:
						{
							NTL_ASSERT(false, "CScouterMeasureDlgGui::HandleEvents, g_EventScouter, SCOUTER_EVENT_MEASURE_MEASURE_RESULT, Can not measure target of class ID : " << uiClassID);
						}
						break;
						}

						CNtlSLEventGenerator::TSScoutUse(pITEM_TBLDAT->tblidx, uiClassID, uiTargetTableIndex);
						Logic_SendTutorialCondition(ETL_CONDITION_TYPE_SCOUTER_POWERLEVEL);

						MeasurePower_Display(pEvent->iUserData2, pEvent->iUserData3, pEvent->iUserData4, pEvent->iUserData5, pEvent->iUserData6, pEvent->iUserData7, pEvent->iUserData8);

						TargetEdgeEffect(true);
					}
					else
					{
						MeasurePower_Display(pEvent->iUserData2, pEvent->iUserData3, pEvent->iUserData4, pEvent->iUserData5, pEvent->iUserData6, pEvent->iUserData7, pEvent->iUserData8);
					}
			}
			break;
			case SCOUTER_EVENT_MEASURE_ERROR:
			{
				if (m_scouterData.hTarget == pEvent->uiUserData)
				{
					EndDisplayImmediately();
				}
			}
			break;
			case SCOUTER_EVENT_EXIT_AFTER_OUT_OF_ORDER:
			{
				EndDisplayImmediately();
			}
			break;
			case SCOUTER_EVENT_EXIT:
			{
				EndDisplayImmediately();
			}
			break;

			default: break;
		}
	}
	else if (msg.Id == g_EventCharObjDelete)
	{
		RwUInt32* pDeleteSerial = reinterpret_cast<RwUInt32*>(msg.pData);
		if (m_scouterData.hTarget == *pDeleteSerial)
		{
			EndDisplayImmediately();
		}
	}
	else if (msg.Id == g_EventSobTargetSelectRelease)
	{
		SNtlEventSobTargetSelectRelease* pEvent = (SNtlEventSobTargetSelectRelease*)msg.pData;

		if (m_scouterData.hTarget == pEvent->hSerialId)
		{
			EndDisplayImmediately();
		}
	}

	NTL_RETURNVOID();
}