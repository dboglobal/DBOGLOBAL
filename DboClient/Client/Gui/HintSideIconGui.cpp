#include "precomp_dboclient.h"

// core
#include "NtlDebug.h"
#include "CEventHandler.h"

// shared
#include "NPCTable.h"

// Sound
#include "GUISoundDefine.h"

// presentation
#include "NtlPLGui.h"
#include "NtlPLGuiManager.h"

// framework
#include "NtlApplication.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobNpc.h"
#include "NtlSobNpcAttr.h"
#include "NtlSobAvatar.h"
#include "NtlSobManager.h"
#include "NtlWorldConcept.h"


// table
#include "HelpTable.h"
#include "ItemTable.h"
#include "SkillTable.h"
#include "GuideHintTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// dbo
#include "DboGlobal.h"
#include "DboEvent.h"
#include "DboEventGenerator.h"
#include "DialogManager.h"
#include "DisplayStringManager.h"
#include "SideIconGui.h"
#include "HintSideIconGui.h"

#include "HintCommon.h"

// 문자열 알림이 출력되는 시간
#define dSTRING_HINT_TIME_NEWMAIL 5.0f

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class : CHelpHintConditionCheck
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief 생성자
*/
CHelpHintConditionCheck ::CHelpHintConditionCheck()
:m_bRegNewHint(FALSE)
,m_uiHelpHint(0)
{
}

/**
* \brief 소멸자
*/
CHelpHintConditionCheck ::~CHelpHintConditionCheck()
{
}

/**
* \brief Create
*/
RwBool CHelpHintConditionCheck::Create()
{
	NTL_FUNCTION("CHelpHintConditionCheck::Create");

	// 조건 검사를 할 Help Data를 Map에 보관한다. (byConditionCheck를 index로, data는 tblidx=> 호출위한)
	CHelpTable* pHelpTable = API_GetTableContainer()->GetHelpTable();
	for (CTable::TABLEIT it = pHelpTable->Begin(); it != pHelpTable->End(); ++it)
	{
		sHELP_TBLDAT* pHelpTableData = (sHELP_TBLDAT*)it->second;
		if (pHelpTableData->byConditionCheck != INVALID_BYTE)
		{
			m_mapHelpHint[pHelpTableData->byConditionCheck].push_back(pHelpTableData->tblidx);
		}
	}

	LinkMsg(g_EventHelpHint, 0);

	LinkMsg(g_EventEndterWorld, 0);
	LinkMsg(g_EventSobInfoUpdate, 0);
	//LinkMsg(g_EventSobCreate, 0);
	LinkMsg(g_EventSobTargetSelect, 0);
	LinkMsg(g_EventSobSkillAdd, 0);
	LinkMsg(g_EventSobHTBSkillAdd, 0);
	LinkMsg(g_EventSobHit, 0);
	LinkMsg(g_EventTMQCleintState, 0);
	LinkMsg(g_EventTSItemGet, 0);
	LinkMsg(g_EventOpenBagGui, 0);
	LinkMsg(g_EventOpenScouterBackgroundGui, 0);
	LinkMsg(g_EventSobEquipItemDurDown, 0);
	LinkMsg(g_EventMobDie, 0);

	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
void CHelpHintConditionCheck::Destroy()
{
	NTL_FUNCTION("CHelpHintConditionCheck::Destroy");

	UnLinkMsg(g_EventHelpHint);

	UnLinkMsg(g_EventEndterWorld);
	UnLinkMsg(g_EventSobInfoUpdate);
	//UnLinkMsg(g_EventSobCreate);
	UnLinkMsg(g_EventSobTargetSelect);
	UnLinkMsg(g_EventSobSkillAdd);
	UnLinkMsg(g_EventSobHTBSkillAdd);
	UnLinkMsg(g_EventSobHit);
	UnLinkMsg(g_EventTMQCleintState);
	UnLinkMsg(g_EventTSItemGet);
	UnLinkMsg(g_EventOpenBagGui);
	UnLinkMsg(g_EventOpenScouterBackgroundGui);
	UnLinkMsg(g_EventSobEquipItemDurDown);
	UnLinkMsg(g_EventMobDie);

	NTL_RETURNVOID();
}

/**
* \brief HandleEvents
* \param msg	Event Message 구조체
*/
void CHelpHintConditionCheck::HandleEvents(RWS::CMsg& msg)
{
	NTL_FUNCTION("CHelpHintConditionCheck::HandleEvents");

	// Tutorial 힌트가 업데이트 되었을 때
	if (msg.Id == g_EventHelpHint)
	{
		SNtlEventHelpHint* pData	= reinterpret_cast<SNtlEventHelpHint*>(msg.pData);
		m_uiHelpHint				= pData->uiHelpHint;
	}														
	else if (msg.Id == g_EventEndterWorld)
	{
		RwBool bBool = TRUE;

		bBool = IsHelpHint_EndterWorldNPCDialog(msg);				/// woosungs_test 20090804

		if (bBool)
		{
			UnLinkMsg(g_EventEndterWorld);
		}
	}
	else if (msg.Id == g_EventSobInfoUpdate)
	{
		RwBool bBool = TRUE;

		bBool = IsHelpHint_First_Faint(msg);
		bBool &= IsHelpHint_Level(msg, LEVEL_2, 2);
		bBool &= IsHelpHint_Level(msg, LEVEL_3, 3);
		bBool &= IsHelpHint_Level(msg, LEVEL_4, 4);
		bBool &= IsHelpHint_Level(msg, LEVEL_5, 5);
		bBool &= IsHelpHint_Level(msg, LEVEL_6, 6);
		bBool &= IsHelpHint_Level(msg, LEVEL_7, 7);
		bBool &= IsHelpHint_Level(msg, LEVEL_8, 8);
		bBool &= IsHelpHint_Level(msg, LEVEL_9, 9);
		bBool &= IsHelpHint_Level(msg, LEVEL_10, 10);
		bBool &= IsHelpHint_Level(msg, LEVEL_15, 15);
		bBool &= IsHelpHint_Level(msg, LEVEL_20, 20);
		bBool &= IsHelpHint_Level(msg, LEVEL_25, 25);
		bBool &= IsHelpHint_Level(msg, LEVEL_30, 30);

		if (bBool)
		{
			UnLinkMsg(g_EventSobInfoUpdate);
		}
	}
	else if (msg.Id == g_EventSobTargetSelect)
	{
		RwBool bBool = TRUE;

		bBool = IsHelpHint_First_Click_NPC(msg, FIRST_CLICK_SHOPNPC, NPC_FUNC_FLAG_MERCHANT);
		bBool &= IsHelpHint_First_Click_NPC(msg, FIRST_CLICK_WARHOUSENPC, NPC_FUNC_FLAG_BANKER);
		bBool &= IsHelpHint_First_Click_Mob(msg);

		if (bBool)
		{
			UnLinkMsg(g_EventSobTargetSelect);
		}
	}
	else if (msg.Id == g_EventSobSkillAdd)
	{
		RwBool bBool = TRUE;

		bBool = IsHelpHint_Learn_Skill(msg, FIRST_LEARN_CHARGE, 320321) ||
			IsHelpHint_Learn_Skill(msg, FIRST_LEARN_CHARGE, 420421) ||
			IsHelpHint_Learn_Skill(msg, FIRST_LEARN_CHARGE, 20221) ||
			IsHelpHint_Learn_Skill(msg, FIRST_LEARN_CHARGE, 120501) ||
			IsHelpHint_Learn_Skill(msg, FIRST_LEARN_CHARGE, 520161) ||
			IsHelpHint_Learn_Skill(msg, FIRST_LEARN_CHARGE, 620161);

		bBool &= IsHelpHint_Learn_Skill(msg, FIRST_LEARN_DASH, 320311) ||
			IsHelpHint_Learn_Skill(msg, FIRST_LEARN_DASH, 420411) ||
			IsHelpHint_Learn_Skill(msg, FIRST_LEARN_DASH, 20211) ||
			IsHelpHint_Learn_Skill(msg, FIRST_LEARN_DASH, 120311) ||
			IsHelpHint_Learn_Skill(msg, FIRST_LEARN_DASH, 520321) ||
			IsHelpHint_Learn_Skill(msg, FIRST_LEARN_DASH, 620151);

		if (bBool)
		{
			UnLinkMsg(g_EventSobSkillAdd);
		}
	}
	else if (msg.Id == g_EventSobHTBSkillAdd)
	{
		RwBool bBool = TRUE;

		bBool = IsHelpHint_Learn_HTBSkill(msg);

		if (bBool)
		{
			UnLinkMsg(g_EventSobSkillAdd);
		}
	}
	else if (msg.Id == g_EventSobHit)
	{
		RwBool bBool = TRUE;

		bBool = IsHelpHint_First_Battle_Finish(msg);

		if (bBool)
		{
			UnLinkMsg(g_EventSobHit);
		}
	}
	//else if (msg.Id == g_EventSobCreate)
	//{
	//	RwBool bBool = TRUE;

	//	bBool = IsHelpHint_First_Drop_Item(msg);
	//	bBool &= IsHelpHint_First_Get_Pet(msg);

	//	if (bBool)
	//	{
	//		UnLinkMsg(g_EventSobCreate);
	//	}
	//}
	else if (msg.Id == g_EventTMQCleintState)
	{
		RwBool bBool = TRUE;

		bBool = IsHelpHint_First_Into_Tmq(msg);
		
		if( bBool )
			UnLinkMsg(g_EventTMQCleintState);
	}   ///////////////  /// woosungs_test 20090804
	else if (msg.Id == g_EventTSItemGet)					/// item 획득  최초 item 루팅, 최초 장비item 루팅
	{
		RwBool bBool = TRUE;

		bBool = IsHelpHint_First_TSItemGet(msg);
		bBool &= IsHelpHint_First_TSItemGetEquip(msg);
		bBool &= IsHelpHint_First_TSItemGetHoipoiStone(msg);

		if( bBool )
			UnLinkMsg(g_EventTSItemGet);
	}
	else if (msg.Id == g_EventOpenBagGui)					/// 최초 inventory open
	{
		RwBool bBool = TRUE;

		bBool = IsHelpHint_First_OpenBagGui(msg);
		
		if( bBool )
			UnLinkMsg(g_EventOpenBagGui);
	}
	else if (msg.Id == g_EventOpenScouterBackgroundGui)		/// 최초 scouter use
	{
		RwBool bBool = TRUE;

		bBool = IsHelpHint_First_OpenScouterBackgroundGui(msg);
		
		if( bBool )
			UnLinkMsg(g_EventOpenScouterBackgroundGui);
	}
	else if (msg.Id == g_EventSobEquipItemDurDown)			/// 최초 장비 내구도 다운
	{
		RwBool bBool = TRUE;

		bBool = IsHelpHint_First_SobEquipItemDurDown(msg);
		
		if( bBool )
			UnLinkMsg(g_EventSobEquipItemDurDown);
	}
	else if (msg.Id == g_EventMobDie)						/// 최초 Mob 사냥
	{
		RwBool bBool = TRUE;

		bBool = IsHelpHint_First_AvatarKillMob(msg);
		
		if( bBool )
			UnLinkMsg(g_EventMobDie);
	}

	SendPacketHintReq();

	NTL_RETURNVOID();
}

void CHelpHintConditionCheck::SendPacketHintReq()
{
	if (m_bRegNewHint)
	{
		GetDboGlobal()->GetGamePacketGenerator()->SendTutorialHintUpdateReq(m_uiHelpHint);
		m_bRegNewHint = FALSE;
	}
}

RwBool CHelpHintConditionCheck::RegHelpHint(const BYTE& byID)
{
	if (byID == INVALID_BYTE)
	{
		return FALSE;
	}

	if( GetNtlWorldConcept()->IsActivePlayConcept( WORLD_PLAY_TUTORIAL ) )
		return FALSE;

	HELPMAP::iterator it = m_mapHelpHint.find(byID);
	if (it == m_mapHelpHint.end())
	{
		return TRUE;													/// Help_Data에 존재하지않는 조건..... 무조건 TRUE반환
	}


	m_uiHelpHint |= 0x1 << byID;
	m_bRegNewHint = TRUE;												/// SendPacketHintReq() 예약
	//GetDboGlobal()->GetGamePacketGenerator()->SendTutorialHintUpdateReq(m_uiHelpHint);
	//같은 상황에서 합쳐서 Request 위해 HandleEvents로 옮김

	TBLIDXVEC& vecTable = it->second;
	for (int i = 0; i < (int)vecTable.size(); ++i)
	{
		CNtlSLEventGenerator::RegSideHintIcon(HINTICON_HELP, vecTable.at(i));
	}
	return TRUE;
}

RwBool CHelpHintConditionCheck::IsRegedHelpHint(const BYTE& byID)
{
	DWORD dwCheck = 0x1 << byID;
	if ((m_uiHelpHint & dwCheck) == dwCheck)	
	{
		return TRUE;
	}
	return FALSE;
}

RwBool CHelpHintConditionCheck::IsHelpHint_First_Battle_Finish(RWS::CMsg& msg)
{
	if (IsRegedHelpHint(FIRST_BATTLE_FINISH))
	{
		return TRUE;
	}

	//if (msg.Id == g_EventSobHit)
	//{
		SNtlEventSobHit*	pData			= reinterpret_cast<SNtlEventSobHit*>(msg.pData);
		CNtlSob*			pSobAttacker	= GetNtlSobManager()->GetSobObject(pData->sHitStuff.hAttackerSerialId);
		CNtlSob*			pSobDefender	= GetNtlSobManager()->GetSobObject(pData->sHitStuff.hDefenderSerialId);

		if (pSobAttacker == GetNtlSLGlobal()->GetSobAvatar() && pSobDefender)
		{
			RwUInt32 uiLp = Logic_GetLp(pSobDefender);
			if (uiLp <= 0)
			{
				return RegHelpHint(FIRST_BATTLE_FINISH);
			}
		}
	//}

	return FALSE;
}

RwBool CHelpHintConditionCheck::IsHelpHint_Level(RWS::CMsg& msg, const eHELPHINT_CONDITION_CHECK& eHelpHint, const RwUInt8& uiLevel)
{
	if (IsRegedHelpHint((BYTE)eHelpHint))
	{
		return TRUE;
	}

	//if (msg.Id == g_EventSobInfoUpdate)
	//{
		SNtlEventSobInfoUpdate* pData	= reinterpret_cast<SNtlEventSobInfoUpdate*>(msg.pData); 
		CNtlSob*				pSob	= GetNtlSobManager()->GetSobObject(pData->hSerialId);

		if (pSob == GetNtlSLGlobal()->GetSobAvatar())
		{
			if(pData->uiUpdateType & EVENT_AIUT_ATTR_LEVELUP)
			{
				CNtlSobAvatar*	pSobAvatar	= GetNtlSLGlobal()->GetSobAvatar();
				if (Logic_GetLevel(pSobAvatar) >= uiLevel)
				{
					return RegHelpHint((BYTE)eHelpHint);
				}
			}
		}
	//}
	return FALSE;
}

RwBool CHelpHintConditionCheck::IsHelpHint_First_Faint(RWS::CMsg& msg)
{
	if (IsRegedHelpHint(FIRST_FAINT))
	{
		return TRUE;
	}

	//if (msg.Id == g_EventSobInfoUpdate)
	//{
		SNtlEventSobInfoUpdate* pData	= reinterpret_cast<SNtlEventSobInfoUpdate*>(msg.pData); 
		CNtlSob*				pSob	= GetNtlSobManager()->GetSobObject(pData->hSerialId);

		if (pSob == GetNtlSLGlobal()->GetSobAvatar())
		{
			if(pData->uiUpdateType & EVENT_AIUT_DIE)
			{
				return RegHelpHint(FIRST_FAINT);
			}
		}
	//}

	return FALSE;
}

RwBool	CHelpHintConditionCheck::IsHelpHint_First_Click_Mob(RWS::CMsg& msg)
{
	if (IsRegedHelpHint(FIRST_CLICK_MOB))
	{
		return TRUE;
	}

	//if (msg.Id == g_EventSobTargetSelect)
	//{
		SNtlEventSobTargetSelect*	pData	= reinterpret_cast<SNtlEventSobTargetSelect*>(msg.pData);
		CNtlSob*					pSob	= reinterpret_cast<CNtlSobNpc*>(GetNtlSobManager()->GetSobObject(pData->hSerialId));

		if (pSob && pSob->GetClassID() == SLCLASS_MONSTER)
		{
			return RegHelpHint(FIRST_CLICK_MOB);
		}
	//}

	return FALSE;
}

RwBool CHelpHintConditionCheck::IsHelpHint_First_Click_NPC(RWS::CMsg& msg, const eHELPHINT_CONDITION_CHECK& eHelpHint, const RwUInt32& uiNPC)
{
	if (IsRegedHelpHint((BYTE)eHelpHint))
	{
		return TRUE;
	}

	//if (msg.Id == g_EventSobTargetSelect)
	//{
		SNtlEventSobTargetSelect*	pData	= reinterpret_cast<SNtlEventSobTargetSelect*>(msg.pData);
		CNtlSob*					pSob	= reinterpret_cast<CNtlSobNpc*>(GetNtlSobManager()->GetSobObject(pData->hSerialId));

		if (pSob && pSob->GetClassID() == SLCLASS_NPC)
		{
			sNPC_TBLDAT* pTableData	= Logic_GetNPCDataFromSob(pData->hSerialId);
			if (pTableData->dwFunc_Bit_Flag & uiNPC)
			{
				return RegHelpHint((BYTE)eHelpHint);
			}
		}
	//}

	return FALSE;
}

RwBool CHelpHintConditionCheck::IsHelpHint_Learn_HTBSkill(RWS::CMsg& msg)
{
	if (IsRegedHelpHint(FIRST_LEARN_HTB))
	{
		return TRUE;
	}

	//if (msg.Id == g_EventSobHTBSkillAdd)
	//{
		return RegHelpHint(FIRST_LEARN_HTB);
	//}

	//return FALSE;
}

RwBool CHelpHintConditionCheck::IsHelpHint_Learn_Skill(RWS::CMsg& msg, const eHELPHINT_CONDITION_CHECK& eHelpHint, const RwUInt32& uiSkill)
{
	if (IsRegedHelpHint((BYTE)eHelpHint))
	{
		return TRUE;
	}

	//if (msg.Id == g_EventSobSkillAdd)
	//{
		SNtlEventSobSkillAdd*	pData = reinterpret_cast<SNtlEventSobSkillAdd*>(msg.pData); 

		if (pData->uiSkillTblId == uiSkill)
		{
			return RegHelpHint((BYTE)eHelpHint);
		}
	//}


	return FALSE;
}
/// woosungs_test 20090804
RwBool CHelpHintConditionCheck::IsHelpHint_First_TSItemGet(RWS::CMsg& msg)
{
	if( IsRegedHelpHint(FIRST_LOOT_ITEM) )
		return TRUE;

	return RegHelpHint(FIRST_LOOT_ITEM);
}

RwBool CHelpHintConditionCheck::IsHelpHint_EndterWorldNPCDialog(RWS::CMsg& msg)
{
	if( IsRegedHelpHint(FIRST_IN_MAINWORLD) )
		return TRUE;

	return RegHelpHint(FIRST_IN_MAINWORLD);
}

RwBool CHelpHintConditionCheck::IsHelpHint_First_TSItemGetEquip(RWS::CMsg& msg)
{
	if( IsRegedHelpHint(FIRST_LOOT_EQUIP) )
		return TRUE;

	SNtlEventTSItemGet* pEventData = (SNtlEventTSItemGet*) msg.pData;
	sITEM_TBLDAT* pITEM_TBLDAT = (sITEM_TBLDAT*)API_GetTableContainer()->GetItemTable()->FindData(pEventData->uiItemIdx);	
																	//Logic_GetItemDataFromTable	
	if(pITEM_TBLDAT)
	{
		if(EQUIP_TYPE_UNKNOWN != pITEM_TBLDAT->byEquip_Type)	/// equip 일때만 체크
		{
			return RegHelpHint(FIRST_LOOT_EQUIP);
		}
	}

	return FALSE;
}

RwBool CHelpHintConditionCheck::IsHelpHint_First_AvatarKillMob(RWS::CMsg& msg)
{
	if( IsRegedHelpHint(FIRST_KILL_MOB) )
		return TRUE;

	SNtlEventMobDie* pEventMobDie = (SNtlEventMobDie*) msg.pData;
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if(pSobAvatar && pSobAvatar->GetSerialID() == pEventMobDie->m_hSerialID)
	{
		return RegHelpHint(FIRST_KILL_MOB);
	}

	return FALSE;
}

RwBool CHelpHintConditionCheck::IsHelpHint_First_SobEquipItemDurDown(RWS::CMsg& msg)
{
	if( IsRegedHelpHint(FIRST_DOWN_DURABILITY) )
		return TRUE;

	return RegHelpHint(FIRST_DOWN_DURABILITY);
}

RwBool CHelpHintConditionCheck::IsHelpHint_First_TSItemGetHoipoiStone(RWS::CMsg& msg)
{
	if( IsRegedHelpHint(FIRST_LOOT_HOIPOISTONE) )
		return TRUE;

	SNtlEventTSItemGet* pEventData = (SNtlEventTSItemGet*) msg.pData;

	if(HOIPOISTONE_ITEM_INDEX == pEventData->uiItemIdx)
	{
		return RegHelpHint(FIRST_LOOT_HOIPOISTONE);
	}

	return FALSE;
}

RwBool CHelpHintConditionCheck::IsHelpHint_First_OpenBagGui(RWS::CMsg& msg)
{
	if( IsRegedHelpHint(FIRST_OPEN_CAPSULEKIT) )
		return TRUE;

	return RegHelpHint(FIRST_OPEN_CAPSULEKIT);
}

RwBool CHelpHintConditionCheck::IsHelpHint_First_OpenScouterBackgroundGui(RWS::CMsg& msg)
{
	if( IsRegedHelpHint(FIRST_USE_SCOUTER) )
		return TRUE;

	return RegHelpHint(FIRST_USE_SCOUTER);
}

/**
* \brief TMQ에 처음 진입한 것을 체크
*/
RwBool CHelpHintConditionCheck::IsHelpHint_First_Into_Tmq( RWS::CMsg& msg ) 
{
	if( IsRegedHelpHint(FIRST_INTO_TMQ) )
		return TRUE;

	SNtlEventTMQServerState* pEvent = reinterpret_cast<SNtlEventTMQServerState*>(msg.pData);

	// TMQ 시작의 State
	if( pEvent->uiState == WORLD_STATE_TMQ_BEGIN )
	{
		return RegHelpHint(FIRST_INTO_TMQ);
	}

	return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class : CHintSideIconGui
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief 생성자
*/
CHintSideIconGui::CHintSideIconGui(const RwChar* pName) 
: CSideIconBase(pName)
, m_pIconButton(NULL)
{
}

/**
* \brief 소멸자
*/
CHintSideIconGui::~CHintSideIconGui()
{
}

/**
* \brief Create
*/
RwBool CHintSideIconGui::Create()
{
	NTL_FUNCTION("CHintSideIconGui::Create");

	if (!CNtlPLGui::Create("", "gui\\HintSideIcon.srf", "gui\\HintSideIcon.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis			= (gui::CDialog*)GetComponent("dlgMain");
	m_pIconButton	= (gui::CButton*)GetComponent("btnIcon");	// 포포 아이콘

	m_slotIconButtonClicked	= m_pIconButton->SigClicked().Connect(this, &CHintSideIconGui::OnIconButtonClicked);
	m_slotPaint = m_pIconButton->SigPaint().Connect( this, &CHintSideIconGui::OnPaint );
	m_slotMove = m_pThis->SigMove().Connect( this, &CHintSideIconGui::OnMove );

	// Help Hint 조건 검사 생성
	m_helpHintConditionCheck.Create();

	//srfHintPulse
	gui::CSurface surface = GetNtlGuiManager()->GetSurfaceManager()->GetSurface( "HintSideIcon.srf", "srfHintPulse" );
	m_feEffect.SetSurface( surface );
	m_feEffect.SetTime( 0.0f, 0.5f );
	m_feEffect.SetAlpha( 0, 255 );


	LinkMsg(g_EventRegSideHintIcon, 0);
	LinkMsg(g_EventPopoNotify, 0);
	LinkMsg(g_EventMailSystemNfy, 0 );
	LinkMsg(g_EventHintViewClosed, 0 );
	LinkMsg(g_EventChangeClassAuthorityChangedNfy, 0);

	Show(false);
	NTL_RETURN(TRUE);
}

/**
* \brief Destroy
*/
void CHintSideIconGui::Destroy()
{
	NTL_FUNCTION("CHintSideIconGui::Destroy");
	
	m_helpHintConditionCheck.Destroy();

	while( m_vecHelpHint.empty() == FALSE )
	{
		CHint* pHint = m_vecHelpHint.back();
		NTL_DELETE( pHint );
		m_vecHelpHint.pop_back();
	}

	UnLinkMsg(g_EventRegSideHintIcon);
	UnLinkMsg(g_EventPopoNotify);
	UnLinkMsg(g_EventMailSystemNfy);
	UnLinkMsg(g_EventHintViewClosed);
	UnLinkMsg(g_EventChangeClassAuthorityChangedNfy);

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

void CHintSideIconGui::Update( RwReal fElapsed )
{
	m_feEffect.Update( fElapsed );
}

/**
* \brief HandleEvents
*/
void CHintSideIconGui::HandleEvents(RWS::CMsg& msg)
{
	NTL_FUNCTION("CHintSideIconGui::HandleEnvets");

	// Hint를 등록 하고 포포 아이콘을 보여준다.
	// ShowHint(); 내부에서 힌트의 갯수를 업데이트 한다.
	if (msg.Id == g_EventRegSideHintIcon)
	{
		SNtlEventRegSideHintIcon* pHintData = reinterpret_cast<SNtlEventRegSideHintIcon*>(msg.pData);

		CHelpHint* pHelpHint = NTL_NEW CHelpHint( pHintData->uiEventType, 
			pHintData->uiTableIndex, 
			CHint::eHINT_TYPE_HELP );
		m_vecHelpHint.push_back( reinterpret_cast<CHint*>( pHelpHint ) );
		
		ShowHint();

		// HintType이 HINTICON_HELP 라면 링크된 도움말을 열어준다.
		if(pHintData->uiEventType == HINTICON_HELP )
		{
			CDboEventGenerator::OpenHelpWindow(pHintData->uiTableIndex);
		}
		/*else if (pHintData->uiEventType == HINTICON_GUIDE)
		{
			pHelpHint->SetType( CHint::eHINT_TYPE_GUIDE );
			CGuideHintTable*	pGuide		= API_GetTableContainer()->GetGuideHintTable();
			sGUIDE_HINT_TBLDAT* pGuideData	= (sGUIDE_HINT_TBLDAT*)pGuide->FindData(pHelpHint->GetTableIndex());
			if (pGuideData && pGuideData->bAutoShow)
			{
				ShowHintView();
			}
		}*/

		Logic_PlayGUISound(GSD_SYSTEM_WINDOW_ALREAT);
	}
	else if( msg.Id == g_EventPopoNotify )
	{
		SNtlEventPopoNotify* pData = reinterpret_cast<SNtlEventPopoNotify*>(msg.pData);

		CStringHint* pStringHint = NTL_NEW CStringHint( pData->pwcTitle,
			pData->pwcBuffer, pData->bImmediate, 
			pData->fLifeTime, 
			CHint::eHINT_TYPE_STRING );
		m_vecHelpHint.push_back( reinterpret_cast<CHint*>( pStringHint ) );

		ShowHint();
		
		if( pData->bImmediate )
		{
			ShowHintView();
		}

		Logic_PlayGUISound(GSD_SYSTEM_WINDOW_ALREAT);
	}
	// Newmail Check
	else if( msg.Id == g_EventMailSystemNfy)
	{
		SNtlEventMailSystemNfy* pEvent = reinterpret_cast<SNtlEventMailSystemNfy*>( msg.pData );

		// Return if not new
		if( pEvent->uiNotifyType != CNtlMailSystem::MAILSYSTEM_NOTIFY_TYPE_NEWMAIL )
			return;

		CStringHint* pStringHint = NTL_NEW CStringHint( GetDisplayStringManager()->GetString( "DST_SIDEICON_HINT_NEWMAIL_TITLE" ),
			GetDisplayStringManager()->GetString( "DST_SIDEICON_HINT_NEWMAIL_TEXT" ), TRUE,
			dSTRING_HINT_TIME_NEWMAIL,
			CHint::eHINT_TYPE_STRING );
		m_vecHelpHint.push_back( reinterpret_cast<CHint*>( pStringHint ) );

		ShowHint();
		ShowHintView();

		Logic_PlayGUISound(GSD_SYSTEM_WINDOW_ALREAT);
	}
	else if( msg.Id == g_EventChangeClassAuthorityChangedNfy )
	{
		SNtlEventChangeClassAuthorityChanged* pData = reinterpret_cast<SNtlEventChangeClassAuthorityChanged*>(msg.pData);

		// 전직 가능 처리
		if( pData->bCanChangeClass )
		{
			CStringHint* pStringHint = NTL_NEW CStringHint( GetDisplayStringManager()->GetString( "DST_SIDEICON_HINT_CLASS_CHANGE_TITLE" ),
				GetDisplayStringManager()->GetString( "DST_SIDEICON_HINT_CLASS_CHANGE_TEXT" ), TRUE,
				dSTRING_HINT_TIME_NEWMAIL,
				CHint::eHINT_TYPE_STRING );
			m_vecHelpHint.push_back( reinterpret_cast<CHint*>( pStringHint ) );

			ShowHint();
			ShowHintView();
		}
	}
	else if( msg.Id == g_EventHintViewClosed )
	{
		// 지우고 출력한다.
		if( !m_vecHelpHint.empty() )
		{
			CHint* pHint = m_vecHelpHint.back();

			NTL_DELETE( pHint );
			m_vecHelpHint.pop_back();

			if( UpdateHint() )
			{
				ShowHintView();
			}
			else
			{
				HideHint();
			}
		}
	}

	NTL_RETURNVOID();
}

/**
* \brief Update Hint
* 현재 남아있는 힌트들의 갯수를 체크하여 출력한다.
* \return 힌트가 있는지 없는지의 여부( TRUE : 힌트 남아있음, FALSE : 힌트 없음 )
*/
RwBool CHintSideIconGui::UpdateHint()
{
	if (m_vecHelpHint.empty())
	{
		m_pIconButton->ClearText();
		return FALSE;
	}

	wchar_t strText[256];
	swprintf_s(strText, 256, L"%d", (int)m_vecHelpHint.size());
	m_pIconButton->SetText(strText);
	return TRUE;
}

/**
* \brief Show PoPo Icon
*/
void CHintSideIconGui::ShowHint()
{
	if (UpdateHint())
	{
		GetNtlGuiManager()->AddUpdateFunc( this );
		m_feEffect.StartProc( TRUE );
		Show(true);
	}
}

/**
* \brief Hide Popo icon when hint is empty.
*/
void CHintSideIconGui::HideHint()
{
	if (!UpdateHint())
	{
		GetNtlGuiManager()->RemoveUpdateFunc( this );
		m_feEffect.EndProc();
		Show(false);
	}
}

/**
* \brief 사이드뷰가 닫길때 호출
*/
void CHintSideIconGui::OnSideViewClosed()
{
	HideHint();
}

/**
* \brief 포포 아이콘을 클릭하였을 때 호출
*/
void CHintSideIconGui::OnIconButtonClicked(gui::CComponent* pComponent)
{
	eSideViewType eType = CSideIconGui::GetInstance()->GetPresentView();
	if( eType == SIDEVIEW_HINT_HELP ||
		eType == SIDEVIEW_HINT_GUIDE ||
		eType == SIDEVIEW_HINT_STRING )
	{
		CHelpHintSideViewGui* pView = (CHelpHintSideViewGui*)CSideIconGui::GetInstance()->GetSideView( eType );
		pView->OnSideViewClose();
	}
	else
	{
		ShowHintView();
	}
}

/**
* \brief 힌트의 사이드 뷰를 보여준다.
*/
void CHintSideIconGui::ShowHintView()
{
	// 힌트가 있을때만 보여준다.
	if (!m_vecHelpHint.empty())
	{
		CHint* pHint = m_vecHelpHint.back();

		switch( pHint->GetType() )
		{
		case CHint::eHINT_TYPE_HELP:
			{
				CHelpHint* pHelpHint = reinterpret_cast<CHelpHint*>( pHint );				
				sHELP_TBLDAT*	pHelpData	= (sHELP_TBLDAT*)API_GetTableContainer()->GetHelpTable()->FindData(pHelpHint->GetTableIndex());
				if (pHelpData)
				{
					CSideIconGui::GetInstance()->OpenSideView(this, SIDEVIEW_HINT_HELP, pHelpData);
				}
			}
			break;
		case CHint::eHINT_TYPE_GUIDE:
			{
				CHelpHint* pHelpHint = reinterpret_cast<CHelpHint*>( pHint );
				CGuideHintTable*	pGuide		= API_GetTableContainer()->GetGuideHintTable();
				sGUIDE_HINT_TBLDAT* pGuideData	= (sGUIDE_HINT_TBLDAT*)pGuide->FindData( pHelpHint->GetTableIndex() );
				if (pGuideData)
				{
					CSideIconGui::GetInstance()->OpenSideView(this, SIDEVIEW_HINT_GUIDE, pGuideData);
				}
			}
			break;
		case CHint::eHINT_TYPE_STRING:
			{
				CStringHint* pStringHint = reinterpret_cast<CStringHint*>( pHint );
				CSideIconGui::GetInstance()->OpenSideView( this, SIDEVIEW_HINT_STRING, pStringHint );
			}
			break;
		default:
			break;
		}
	}
}

void CHintSideIconGui::OnPaint()
{
	m_feEffect.Render();
}

void CHintSideIconGui::OnMove( RwInt32 nOldX, RwInt32 nOldY )
{
	CRectangle rect = m_pThis->GetScreenRect();

	RwInt32 nCenterX, nCenterY;
	gui::CSurface surface = m_feEffect.GetSurface();
	surface.m_Original.rtRect.GetCenterPosition( &nCenterX, &nCenterY );
	m_feEffect.SetCenterPos( rect.left + nCenterX, rect.top + nCenterY );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class : CHelpHintSideViewGui
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
* \brief 생성자
*/
CHelpHintSideViewGui::CHelpHintSideViewGui(const RwChar* pName) 
: CSideViewBase(pName)
, m_pHelpHintTitle(NULL)
, m_pHelpHintText(NULL),
m_pCloseButton(NULL)
, m_pOpenButton(NULL)
, m_pHelpHintData(NULL)
{
}

/**
* \brief 소멸자
*/
CHelpHintSideViewGui::~CHelpHintSideViewGui()
{
}

RwBool CHelpHintSideViewGui::Create()
{
	NTL_FUNCTION("CHelpHintSideViewGui::Create");

	if (!CNtlPLGui::Create("", "gui\\HelpHintSideView.srf", "gui\\HelpHintSideView.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis				= (gui::CDialog*)GetComponent("dlgMain");
	m_pHelpHintTitle	= (gui::CStaticBox*)GetComponent("stbTitle");
	m_pHelpHintText		= (gui::CStaticBox*)GetComponent("stbText");
	m_pOpenButton		= (gui::CButton*)GetComponent("btnOpen");
	m_pCloseButton		= (gui::CButton*)GetComponent("btnClose");


	m_BackPanel.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackPanel.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HelpHintSideView.srf", "srfDialogBackUp"));
	m_BackPanel.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HelpHintSideView.srf", "srfDialogBackCenter"));
	m_BackPanel.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("HelpHintSideView.srf", "srfDialogBackDown"));
	m_BackPanel.SetSize(43, 43);
	m_BackPanel.SetPositionfromParent(0, 0);

	m_slotMove					= m_pThis->SigMove().Connect(this, &CHelpHintSideViewGui::OnMove);
	m_slotPaint					= m_pThis->SigPaint().Connect(this, &CHelpHintSideViewGui::OnPaint);
	m_slotOpenButtonClicked		= m_pOpenButton->SigClicked().Connect(this, &CHelpHintSideViewGui::OnOpenButtonClicked);
	m_slotCloseButtonClicked	= m_pCloseButton->SigClicked().Connect(this, &CHelpHintSideViewGui::OnCloseButtonClicked);

	m_pOpenButton->SetText(GetDisplayStringManager()->GetString("DST_SIDEICON_HINT_HELP_MORE"));

	Show(false);
	NTL_RETURN(TRUE);
}

void CHelpHintSideViewGui::Destroy()
{
	NTL_FUNCTION("CHelpHintSideViewGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

void CHelpHintSideViewGui::HandleEvents(RWS::CMsg& msg)
{
	NTL_FUNCTION("CHelpHintSideViewGui::HandleEvents");

	NTL_RETURNVOID();	
}

void CHelpHintSideViewGui::OnMove(RwInt32 iOldX, RwInt32 iOldY)
{
	m_BackPanel.SetRect(m_pThis->GetScreenRect());
}

void CHelpHintSideViewGui::OnPaint()
{
	m_BackPanel.Render();
}

void CHelpHintSideViewGui::OnOpenButtonClicked(gui::CComponent* pComponent)
{
	CDboEventGenerator::OpenHelpWindow(m_pHelpHintData->tblidx);
	Show(false);
}

void CHelpHintSideViewGui::OnCloseButtonClicked(gui::CComponent* pComponent)
{	
	OnSideViewClose();

	CDboEventGenerator::HintSideViewClosed();
}

void CHelpHintSideViewGui::OnSideViewLocate(const CRectangle& rectSideIcon)
{
	int	iHeight = m_pHelpHintText->GetHeight() + dHINT_HELP_DEFAULT_HEIGHT;

	m_pThis->SetHeight(iHeight);
	m_pThis->SetPosition(rectSideIcon.left - m_pThis->GetWidth() + rectSideIcon.GetWidth(), rectSideIcon.top - iHeight);

	iHeight	-= dHINT_HELP_OPEN_POS_BY_BOTTOM;
	m_pOpenButton->SetPosition(m_pOpenButton->GetPosition().left, iHeight);
}

void CHelpHintSideViewGui::OnPressESC()
{
}

void CHelpHintSideViewGui::OnSideViewOpen(const void* pData)
{
	ShowHelpHint((sHELP_TBLDAT*)pData);
}

void CHelpHintSideViewGui::OnSideViewClose()
{
	Show(false);
}

void CHelpHintSideViewGui::ShowHelpHint(sHELP_TBLDAT* pTableData)
{
	m_pHelpHintData = pTableData;
	if (pTableData)
	{
		CTextTable*	pTable = API_GetTableContainer()->GetTextAllTable()->GetHelpTbl();
		CTextTable* pPopoTable = API_GetTableContainer()->GetTextAllTable()->GetHelpPopoTbl();

		m_pHelpHintTitle->SetText(pTable->GetText(pTableData->dwHelpTitle).c_str());
		m_pHelpHintText->SetText(pPopoTable->GetText(pTableData->dwPopoHint).c_str());

		Show(true);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class : CGuideHintSideViewGui
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGuideHintSideViewGui::CGuideHintSideViewGui(const RwChar* pName) : CSideViewBase(pName), m_pGuideHintData(NULL), m_pCloseButton(NULL),
m_pHtmlBox(NULL), m_pFlash(NULL)
{
}

CGuideHintSideViewGui::~CGuideHintSideViewGui()
{
}

RwBool CGuideHintSideViewGui::Create()
{
	NTL_FUNCTION("CGuideHintSideViewGui::Create");

	if (!CNtlPLGui::Create("", "gui\\GuideHintSideView.srf", "gui\\GuideHintSideView.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis				= (gui::CDialog*)GetComponent("dlgMain");
	m_pCloseButton		= (gui::CButton*)GetComponent("btnClose");
	m_pHtmlBox			= (gui::CHtmlBox*)GetComponent("hbxHtml");
	m_pFlash			= (gui::CFlash*)GetComponent("flaFlash");

	m_slotCloseButtonClicked	= m_pCloseButton->SigClicked().Connect(this, &CGuideHintSideViewGui::OnCloseButtonClicked);

	Show(false);
	NTL_RETURN(TRUE);
}


void CGuideHintSideViewGui::Destroy()
{
	NTL_FUNCTION("CGuideHintSideViewGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

void CGuideHintSideViewGui::Update(RwReal fElapsed)
{
	m_pFlash->Update(fElapsed);
}

void CGuideHintSideViewGui::HandleEvents(RWS::CMsg& msg)
{
	NTL_FUNCTION("CGuideHintSideViewGui::HandleEvents");

	NTL_RETURNVOID();	
}

void CGuideHintSideViewGui::OnCloseButtonClicked(gui::CComponent* pComponent)
{
	OnSideViewClose();

	CDboEventGenerator::HintSideViewClosed();
}

void CGuideHintSideViewGui::OnSideViewLocate(const CRectangle& rectSideIcon)
{
	if (!m_pGuideHintData)
	{
		Show(false);
		return;
	}

	RwUInt32 uiLeft = 0;
	RwUInt32 uiTop = 0;

	switch (m_pGuideHintData->byWidthPosition)
	{
	case GUIDE_LEFT:
		{
			uiLeft = 0;
			break;
		}
	case GUIDE_CENTER:
		{
			uiLeft = (GetDboGlobal()->GetScreenWidth() - m_pThis->GetWidth()) / 2;
			break;
		}
	case GUIDE_RIGHT:
		{
			uiLeft = GetDboGlobal()->GetScreenWidth() - m_pThis->GetWidth();
			break;
		}
	}

	switch (m_pGuideHintData->byHeightPosition)
	{
	case GUIDE_TOP:
		{
			uiTop = 0;
			break;
		}
	case GUIDE_MIDDLE:
		{
			uiTop = (GetDboGlobal()->GetScreenHeight() - m_pThis->GetHeight()) / 2;
			break;
		}
	case GUIDE_BOTTOM:
		{
			uiTop = GetDboGlobal()->GetScreenHeight() - m_pThis->GetHeight();
			break;
		}
	}

	m_pThis->SetPosition(uiLeft, uiTop);
}

void CGuideHintSideViewGui::OnPressESC()
{
	OnSideViewClose();
}

void CGuideHintSideViewGui::OnSideViewOpen(const void* pData)
{
	sGUIDE_HINT_TBLDAT* pTableData = (sGUIDE_HINT_TBLDAT*)pData;
	
	switch (pTableData->byType)
	{
	case GUIDE_FLASH:
		{
			OpenFlashBox(pTableData);
			break;
		}
	case GUIDE_HTML:
		{
			OpenHtmlBox(pTableData);
			break;
		}
	}
}

void CGuideHintSideViewGui::OnSideViewClose()
{
	Show(false);
	GetNtlGuiManager()->RemoveUpdateFunc( this );
}

void CGuideHintSideViewGui::SetDialog(const eGUIDE_TYPE& eGuideType)
{
	switch (eGuideType)
	{
	case GUIDE_FLASH:
		{
			m_pHtmlBox->Show( false );
			m_pFlash->Show( true );
			break;
		}
	case GUIDE_HTML:
		{
			m_pHtmlBox->Show( true );
			m_pFlash->Show( false );
			break;
		}
	}
}

void CGuideHintSideViewGui::OpenHtmlBox(sGUIDE_HINT_TBLDAT* pTableData)
{
	SetDialog(GUIDE_HTML);
	m_pGuideHintData = pTableData;

	m_pThis->SetPosition(CRectangle(0, 0, pTableData->wWidthSize, pTableData->wHeightSize));
	m_pHtmlBox->SetPosition(CRectangle(5, 5, pTableData->wWidthSize - 10, pTableData->wHeightSize - 20 - 5));
	m_pCloseButton->SetPosition(pTableData->wWidthSize - 12 - 10, pTableData->wHeightSize - 12 - 10);

	CHAR	acBuffer[1024];	
#ifdef dHINTSIDEICON_DEBUG
	sprintf_s(acBuffer, 1024, "%s%s", dHINT_HTML_DIRECTORY_GUIDE, "test.htm");
#else
	sprintf_s(acBuffer, 1024, "%s%s", dHINT_HTML_DIRECTORY_GUIDE, pTableData->szResource);
#endif

	m_pHtmlBox->Clear();
	m_pHtmlBox->SetHtmlFromFile(acBuffer);
	Show(true);	
}

void CGuideHintSideViewGui::OpenFlashBox(sGUIDE_HINT_TBLDAT* pTableData)
{
	SetDialog(GUIDE_FLASH);
	m_pGuideHintData = pTableData;

	CRectangle rectPosition(0, 0, pTableData->wWidthSize, pTableData->wHeightSize);

	m_pThis->SetPosition(rectPosition);

	m_pFlash->Load( pTableData->szResource ); // 로딩 부분은 이벤트 받는 곳으로 옮겨야 함. by Peessi.
	m_pFlash->SetPosition(rectPosition);
	m_pFlash->PlayMovie( TRUE );
	m_pFlash->RestartMovie();
	
	GetNtlGuiManager()->AddUpdateFunc( this );
	
	Show(true);
}

////////////////////////////////////////////////////////////////////////////
// CStringHintSideViewGui
////////////////////////////////////////////////////////////////////////////

CStringHintSideViewGui::CStringHintSideViewGui( const RwChar* pName ) 
: CSideViewBase(pName)
,m_pStringHintTitle(NULL)
,m_pStringHintText(NULL)
,m_pCloseButton(NULL)
,m_fLifeTime(0.0f)
,m_fCurrentTime(0.0f)
,m_bNotifyShow(FALSE)
{

}

CStringHintSideViewGui::~CStringHintSideViewGui() 
{

}

RwBool CStringHintSideViewGui::Create() 
{
	NTL_FUNCTION("CStringHintSideViewGui::Create");

	if (!CNtlPLGui::Create("", "gui\\StringHintSideView.srf", "gui\\StringHintSideView.frm"))
		NTL_RETURN(FALSE);

	CNtlPLGui::CreateComponents(GetNtlGuiManager()->GetGuiManager());

	m_pThis				= (gui::CDialog*)GetComponent("dlgMain");
	m_pStringHintTitle	= (gui::CStaticBox*)GetComponent("stbTitle");
	m_pStringHintText		= (gui::CStaticBox*)GetComponent("stbText");
	m_pCloseButton		= (gui::CButton*)GetComponent("btnClose");
 

	m_BackPanel.SetType(CWindowby3::WT_HORIZONTAL);
	m_BackPanel.SetSurface(0, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("StringHintSideView.srf", "srfDialogBackUp"));
	m_BackPanel.SetSurface(1, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("StringHintSideView.srf", "srfDialogBackCenter"));
	m_BackPanel.SetSurface(2, GetNtlGuiManager()->GetSurfaceManager()->GetSurface("StringHintSideView.srf", "srfDialogBackDown"));
	m_BackPanel.SetSize(200, 124);
	m_BackPanel.SetPositionfromParent(0, 0);

	m_slotMove					= m_pThis->SigMove().Connect(this, &CStringHintSideViewGui::OnMove);
	m_slotPaint					= m_pThis->SigPaint().Connect(this, &CStringHintSideViewGui::OnPaint);
	m_slotCloseButtonClicked	= m_pCloseButton->SigClicked().Connect(this, &CStringHintSideViewGui::OnCloseButtonClicked);
	
	Show(false);

	NTL_RETURN(TRUE);
}

void CStringHintSideViewGui::Destroy() 
{
	NTL_FUNCTION("CStringHintSideViewGui::Destroy");

	CNtlPLGui::DestroyComponents();
	CNtlPLGui::Destroy();

	NTL_RETURNVOID();
}

void CStringHintSideViewGui::HandleEvents( RWS::CMsg& msg ) 
{
	NTL_FUNCTION("CStringHintSideViewGui::HandleEvents");

	NTL_RETURNVOID();
}

void CStringHintSideViewGui::Update( RwReal fElapsed ) 
{
	if( m_bNotifyShow )
	{
		m_fCurrentTime += fElapsed;

		if( m_fCurrentTime > m_fLifeTime )
		{
			// 닫아준다.
			OnSideViewClose();

			CDboEventGenerator::HintSideViewClosed();
		}
	}
}

void CStringHintSideViewGui::OnPaint() 
{
	m_BackPanel.Render();
}

void CStringHintSideViewGui::OnMove( RwInt32 iOldX, RwInt32 iOldY ) 
{
	m_BackPanel.SetRect(m_pThis->GetScreenRect());
}

void CStringHintSideViewGui::OnCloseButtonClicked( gui::CComponent* pComponent ) 
{
	OnSideViewClose();

	CDboEventGenerator::HintSideViewClosed();
}

void CStringHintSideViewGui::OnPressESC() 
{
	OnSideViewClose();

	CDboEventGenerator::HintSideViewClosed();
}

void CStringHintSideViewGui::OnSideViewOpen( const void* pData )
{
	CStringHint* pStringHint = (CStringHint*)pData;

	ShowStringHint( pStringHint->GetTitle(), pStringHint->GetBuffer() );

	m_fLifeTime = pStringHint->GetLifeTime();
	
	if( pStringHint->GetImmdiate() && pStringHint->GetLifeTime() > 0.0f )
		StartStringHint();
}

void CStringHintSideViewGui::OnSideViewClose() 
{
	Show(false);
	EndStringHint();
}

void CStringHintSideViewGui::OnSideViewLocate( const CRectangle& rectSideIcon ) 
{
	int	iHeight = m_pStringHintText->GetHeight() + dHINT_HELP_DEFAULT_HEIGHT;

	m_pThis->SetHeight(iHeight);
	m_pThis->SetPosition(rectSideIcon.left - m_pThis->GetWidth() + rectSideIcon.GetWidth(), rectSideIcon.top - iHeight);

	iHeight	-= dHINT_HELP_OPEN_POS_BY_BOTTOM;
}

void CStringHintSideViewGui::StartStringHint() 
{
	m_bNotifyShow = TRUE;
	m_fCurrentTime = 0.0f;
	GetNtlGuiManager()->AddUpdateFunc( this );
}

void CStringHintSideViewGui::EndStringHint() 
{
	m_bNotifyShow = FALSE;
	m_fCurrentTime = 0.0f;
	GetNtlGuiManager()->RemoveUpdateFunc( this );
	Show(false);
}

void CStringHintSideViewGui::ShowStringHint( const WCHAR* pwcTitle, const WCHAR* pwcText ) 
{
	m_pStringHintTitle->SetText( pwcTitle );
	m_pStringHintText->SetText( pwcText );

	Show(true);
}