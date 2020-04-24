/*****************************************************************************
* File			: DboLogic_String.h
* Author		: Hong sungbock
* Copyright		: (주)NTL
* Date			: 2007. 1. 12
* Abstract		: 스트링 관련 로직을 모아둔다
*****************************************************************************
* Desc         : 찾으려는 스트링이 없을 시에는 반드시 알 수 있도록
*				 Assert를 걸던지 스트링이 없다는 메세지를 돌려주도록 하자
*****************************************************************************/

#include "precomp_dboclient.h"
#include "DboLogic.h"

// shared
#include "NtlCharacter.h"
#include "SkillTable.h"
#include "HTBSetTable.h"
#include "ItemRecipeTable.h"
#include "SystemEffectTable.h"
#include "ItemTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"
#include "NtlBattle.h"
#include "NtlService.h"


// simulation
#include "NtlSLApi.h"
#include "NtlSobAvatar.h"
#include "NtlSLLogic.h"
#include "NtlActionMapDef.h"


// dbo
#include "DisplayStringManager.h"
#include "DboEventGenerator.h"
#include "ChatGui.h"
#include "AlarmManager.h"
#include "SayFilter.h"



const WCHAR* Logic_GetClassName(RwUInt8 byClass)
{
	// It examines the indexes individually so that when there is a change in the index number
	// Also, to avoid affecting the order of the string table ...
	switch( byClass )
	{
	case PC_CLASS_HUMAN_FIGHTER:			return GetDisplayStringManager()->GetString( "DST_HUMAN_FIGHTER" );
	case PC_CLASS_HUMAN_MYSTIC:				return GetDisplayStringManager()->GetString( "DST_HUMAN_MYSTIC" );
	case PC_CLASS_HUMAN_ENGINEER:			return GetDisplayStringManager()->GetString( "DST_ENGINEER" );
	case PC_CLASS_NAMEK_FIGHTER:			return GetDisplayStringManager()->GetString( "DST_NAMEK_FIGHTER" );
	case PC_CLASS_NAMEK_MYSTIC:				return GetDisplayStringManager()->GetString( "DST_NAMEK_MYSTIC" );
	case PC_CLASS_MIGHTY_MAJIN:				return GetDisplayStringManager()->GetString( "DST_MIGHTY_MAJIN" );
	case PC_CLASS_WONDER_MAJIN:				return GetDisplayStringManager()->GetString( "DST_WONDER_MAJIN" );
	case PC_CLASS_STREET_FIGHTER:			return GetDisplayStringManager()->GetString( "DST_STREET_FIGHTER" );
	case PC_CLASS_SWORD_MASTER:				return GetDisplayStringManager()->GetString( "DST_SWORD_MASTER" );
	case PC_CLASS_CRANE_ROSHI:				return GetDisplayStringManager()->GetString( "DST_CRANE_ROSHI" );
	case PC_CLASS_TURTLE_ROSHI:				return GetDisplayStringManager()->GetString( "DST_TURTLE_ROSHI" );
	case PC_CLASS_GUN_MANIA:				return GetDisplayStringManager()->GetString( "DST_GUN_MANIA" );
	case PC_CLASS_MECH_MANIA:				return GetDisplayStringManager()->GetString( "DST_MECH_MANIA" );
	case PC_CLASS_DARK_WARRIOR:				return GetDisplayStringManager()->GetString( "DST_DARK_WARRIOR" );
	case PC_CLASS_SHADOW_KNIGHT:			return GetDisplayStringManager()->GetString( "DST_SHADOW_KNIGHT" );
	case PC_CLASS_POCO_SUMMONER:			return GetDisplayStringManager()->GetString( "DST_POCO_SUMMONER" );
	case PC_CLASS_DENDEN_HEALER:			return GetDisplayStringManager()->GetString( "DST_DENDEN_HEALER" );
	case PC_CLASS_GRAND_MA:					return GetDisplayStringManager()->GetString( "DST_GRAND_MA" );
	case PC_CLASS_ULTI_MA:					return GetDisplayStringManager()->GetString( "DST_ULTI_MA" );
	case PC_CLASS_PLAS_MA:					return GetDisplayStringManager()->GetString( "DST_PLAS_MA");
	case PC_CLASS_KAR_MA:					return GetDisplayStringManager()->GetString( "DST_KAR_MA" );
	}

	static WCHAR awcBuffer[] = L"Not exist class name";
	NTL_ASSERT(false, "Logic_GetClassName, Not exist class name of class index : " << byClass);

	return awcBuffer;
}

const WCHAR* Logic_GetHoipoiMixSkillName(RwUInt8 byType)
{
	switch( byType )
	{
		case eRECIPE_TYPE_NORMAL:		return GetDisplayStringManager()->GetString( "DST_ITEM_RECIPE_TYPE_FOOD_DRINK" );
		case eRECIPE_TYPE_MAINWEAPON:		return GetDisplayStringManager()->GetString( "DST_ITEM_RECIPE_TYPE_MAIN_WEAPON" );
		case eRECIPE_TYPE_CLOTH_ARMOR:			return GetDisplayStringManager()->GetString( "DST_ITEM_RECIPE_TYPE_CLOTH_ARMOR" );
		case eRECIPE_TYPE_ACCESSORY:		return GetDisplayStringManager()->GetString( "DST_ITEM_RECIPE_TYPE_ACCESSORY" );
		case eRECIPE_TYPE_TECH_ARMOR:			return GetDisplayStringManager()->GetString( "DST_ITEM_RECIPE_TYPE_TECH_ARMOR" );
		case eRECIPE_TYPE_SUBWEAPON:		return GetDisplayStringManager()->GetString( "DST_ITEM_RECIPE_TYPE_SUB_WEAPON" );
	}

	static WCHAR awcBuffer[] = L"Not exist class name";
	NTL_ASSERT(false, "Logic_GetClassName, Not exist job name of recipe type : " << byType );

	return awcBuffer;
}

const WCHAR* Logic_GetNPCJobName(RwUInt8 byJob)
{
	// It examines the indexes individually so that when there is a change in the index number
	// 또한 스트링 테이블의 순서에 영향이 없도록...
	switch( byJob )
	{
	case NPC_JOB_WEAPON_MERCHANT:			return GetDisplayStringManager()->GetString( "DST_JOB_WEAPON_MERCHANT" );
	case NPC_JOB_ARMOR_MERCHANT:			return GetDisplayStringManager()->GetString( "DST_JOB_ARMOR_MERCHANT" );
	case NPC_JOB_GOODS_MERCHANT:			return GetDisplayStringManager()->GetString( "DST_JOB_GOODS_MERCHANT" );
	case NPC_JOB_SCOUTER_MERCHANT:
	case NPC_JOB_SCOUTER_MERCHANT2:			return GetDisplayStringManager()->GetString( "DST_JOB_SCOUTER_MERCHANT" );
	case NPC_JOB_GUARD:						return GetDisplayStringManager()->GetString( "DST_JOB_GUARD" );
	case NPC_JOB_SKILL_TRAINER_HFI:			return GetDisplayStringManager()->GetString( "DST_JOB_SKILL_TRAINER_HFI" );
	case NPC_JOB_SKILL_TRAINER_HMY:			return GetDisplayStringManager()->GetString( "DST_JOB_SKILL_TRAINER_HMY" );
	case NPC_JOB_SKILL_TRAINER_HEN:			return GetDisplayStringManager()->GetString( "DST_JOB_SKILL_TRAINER_HEN" );
	case NPC_JOB_SKILL_TRAINER_NFI:			return GetDisplayStringManager()->GetString( "DST_JOB_SKILL_TRAINER_NFI" );
	case NPC_JOB_SKILL_TRAINER_NMY:			return GetDisplayStringManager()->GetString( "DST_JOB_SKILL_TRAINER_NMY" );
	case NPC_JOB_SKILL_TRAINER_MMI:			return GetDisplayStringManager()->GetString( "DST_JOB_SKILL_TRAINER_MMI" );
	case NPC_JOB_SKILL_TRAINER_MWO:			return GetDisplayStringManager()->GetString( "DST_JOB_SKILL_TRAINER_MWO" );
	case NPC_JOB_BANKER:					return GetDisplayStringManager()->GetString( "DST_JOB_BANKER" );
	case NPC_JOB_TALKER:					return GetDisplayStringManager()->GetString( "DST_JOB_TALKER" );
	case NPC_JOB_GUILD_MANAGER:				return GetDisplayStringManager()->GetString( "DST_JOB_GUILD_MANAGER" );
	case NPC_JOB_SUMMON_PET:				return GetDisplayStringManager()->GetString("DST_JOB_SUMMON_PET");
	case NPC_JOB_SPECIAL_SCOUTER_MERCHANT2:		return GetDisplayStringManager()->GetString("DST_JOB_SPECIAL_SCOUTER_MERCHANT");
	case NPC_JOB_DOGI_MERCHANT:				return GetDisplayStringManager()->GetString( "DST_JOB_DOGI_MERCHANT" );
	case NPC_JOB_SPECIAL_WEAPON_MERCHANT:	return GetDisplayStringManager()->GetString( "DST_JOB_SPECIAL_WEAPON_MERCHANT" );
	case NPC_JOB_SPECIAL_ARMOR_MERCHANT:	return GetDisplayStringManager()->GetString( "DST_JOB_SPECIAL_ARMOR_MERCHANT" );
	case NPC_JOB_SPECIAL_GOODS_MERCHANT:	return GetDisplayStringManager()->GetString( "DST_JOB_SPECIAL_GOODS_MERCHANT" );
	case NPC_JOB_SPECIAL_FOODS_MERCHANT:	return GetDisplayStringManager()->GetString( "DST_JOB_SPECIAL_FOODS_MERCHANT" );
	case NPC_JOB_SPECIAL_SCOUTER_MERCHANT:	return GetDisplayStringManager()->GetString( "DST_JOB_SPECIAL_SCOUTER_MERCHANT" );
	case NPC_JOB_GRAND_SKILL_TRAINER_HFI:	return GetDisplayStringManager()->GetString( "DST_JOB_GRAND_SKILL_TRAINER_HFI" );
	case NPC_JOB_GRAND_SKILL_TRAINER_HMY:	return GetDisplayStringManager()->GetString( "DST_JOB_GRAND_SKILL_TRAINER_HMY" );
	case NPC_JOB_GRAND_SKILL_TRAINER_HEN:	return GetDisplayStringManager()->GetString( "DST_JOB_GRAND_SKILL_TRAINER_HEN" );
	case NPC_JOB_GRAND_SKILL_TRAINER_NFI:	return GetDisplayStringManager()->GetString( "DST_JOB_GRAND_SKILL_TRAINER_NFI" );
	case NPC_JOB_GRAND_SKILL_TRAINER_NMY:	return GetDisplayStringManager()->GetString( "DST_JOB_GRAND_SKILL_TRAINER_NMY" );
	case NPC_JOB_GRAND_SKILL_TRAINER_MMI:	return GetDisplayStringManager()->GetString( "DST_JOB_GRAND_SKILL_TRAINER_MMI" );
	case NPC_JOB_GRAND_SKILL_TRAINER_MWO:	return GetDisplayStringManager()->GetString( "DST_JOB_GRAND_SKILL_TRAINER_MWO" );
	case NPC_JOB_SUB_WEAPON_MERCHANT:		return GetDisplayStringManager()->GetString( "DST_JOB_SUB_WEAPON_MERCHANT" );
	case NPC_JOB_GATE_KEEPER:				return GetDisplayStringManager()->GetString( "DST_JOB_GATE_KEEPER" );
	case NPC_JOB_VENDING_MACHINE:			return GetDisplayStringManager()->GetString( "DST_JOB_VENDING_MACHINE" );
	case NPC_JOB_TIMEMACHINE_MERCHANT:		return GetDisplayStringManager()->GetString( "DST_JOB_TIMEMACHINE_MERCHANT" );
	case NPC_JOB_PORTAL_MAN:				return GetDisplayStringManager()->GetString( "DST_JOB_POTALMAN" );
	case NPC_JOB_BUS:						return GetDisplayStringManager()->GetString( "DST_JOB_BUS" );
	case NPC_JOB_RECEPTION:					return GetDisplayStringManager()->GetString("DST_JOB_RECEPTION");			
	case NPC_JOB_BUDOHSI_MERCHANT:			
	case NPC_JOB_BUDOHSI_MERCHANT2:
	case NPC_JOB_BUDOHSI_MERCHANT3:			return GetDisplayStringManager()->GetString( "DST_JOB_BUDOHSI_MERCHANT" );
	case NPC_JOB_REFEREE:					return GetDisplayStringManager()->GetString( "DST_JOB_REFEREE" );
	case NPC_JOB_GAMBLE_MERCHANT:			
	case NPC_JOB_AIR_GAMBLE_MERCHANT:		return GetDisplayStringManager()->GetString( "DST_JOB_GAMBLE_MERCHANT" );
	case NPC_JOB_CHAMPION_MERCHANT:			return GetDisplayStringManager()->GetString( "DST_JOB_CHAMPION_MERCHANT" );
	case NPC_JOB_DOJO_MANAGER:				return GetDisplayStringManager()->GetString( "DST_JOB_DOJO_MANAGER" );
	case NPC_JOB_DOJO_MERCHANT:				return GetDisplayStringManager()->GetString( "DST_JOB_DOJO_MERCHANT" );
	case NPC_JOB_DOJO_SEAL:					return GetDisplayStringManager()->GetString( "DST_JOB_DOJO_SEAL" );
	case NPC_JOB_DOJO_BANKER:				return GetDisplayStringManager()->GetString( "DST_JOB_DOJO_BANKER" );
	case NPC_JOB_MIX_MASTER:				
	case NPC_JOB_MIX_MASTER2:				return GetDisplayStringManager()->GetString( "DST_JOB_MIX_MASTER" );
	case NPC_JOB_COMIC_NPC:				return NULL; // TLQ/TMQ NPC DONT HAVE TITLE
	case NPC_JOB_QUEST_MERCHANT:				return GetDisplayStringManager()->GetString("DST_JOB_QUEST_MERCHANT");
	case NPC_JOB_MASCOT_MERCHANT:				return GetDisplayStringManager()->GetString("DST_JOB_MASCOT_MERCHANT");
	case NPC_JOB_EVENT_NPC:				return GetDisplayStringManager()->GetString("DST_JOB_EVENT_NPC");
	case NPC_JOB_DWC_TELEPORT:				return GetDisplayStringManager()->GetString("DST_JOB_DWC_NPC");
	case NPC_JOB_MASCOT_GAMBLE_MERCHANT:				return GetDisplayStringManager()->GetString("DST_JOB_MASCOT_GAMBLE_MERCHANT");
	case NPC_JOB_BATTLE_DUNGEON_MANAGER:				return GetDisplayStringManager()->GetString("DST_JOB_BATTLE_DUNGEON_MANAGER");
	case NPC_JOB_ITEM_UPGRADE:				return GetDisplayStringManager()->GetString("DST_JOB_ITEMUPGREAD_MERCHANT");
	case NPC_JOB_VEHICLE_MERCHANT:				return GetDisplayStringManager()->GetString("DST_JOB_VEHICLE_MERCHANT");
	case NPC_JOB_ALIEN_ENGINEER:				return GetDisplayStringManager()->GetString("DST_JOB_ALIEN_ENGINEER");
	case NPC_JOB_MASCOTEX_MERCHANT:				return GetDisplayStringManager()->GetString("DST_JOB_MASCOTEX_MERCHANT");
	case NPC_JOB_MASCOT_GAMBLE_MERCHANT_2:		return GetDisplayStringManager()->GetString("DST_JOB_MASCOTEX_GAMBLE_MERCHANT");
		default: break;
	}

	DBO_WARNING_MESSAGE("Not exist NPC job name of NPC job index : " << (int)byJob); //true so it wont crash

	return NULL;
}

const WCHAR* Logic_GetPlayerRaceName(RwUInt8 byRace)
{
	switch( byRace )
	{
	case RACE_HUMAN:					return GetDisplayStringManager()->GetString( "DST_HUMAN" );
	case RACE_NAMEK:					return GetDisplayStringManager()->GetString( "DST_NAMEK" );
	case RACE_MAJIN:					return GetDisplayStringManager()->GetString( "DST_MAJIN" );	
	}

	NTL_ASSERT(false, "Logic_GetPlayerRaceName, Invalid Player race index : " << byRace);
	return NULL;
}

const WCHAR* Logic_GetNPCRaceName(RwUInt8 byRace)
{
	return Logic_GetMobRaceName(byRace);
}

const WCHAR* Logic_GetMobRaceName(RwUInt8 byRace)
{
	// 인덱스를 일일이 검사하여 인덱스의 숫자에 변경이 있을 시 Assert를 발생시켜 바로 알 수 있도록 한다
	// 또한 스트링 테이블의 순서에 영향이 없도록...
	switch( byRace )
	{
	case MOB_TYPE_ANIMAL:					return GetDisplayStringManager()->GetString( "DST_MOB_TYPE_ANIMAL" );
	case MOB_TYPE_HUMAN_ANIMAL:				return GetDisplayStringManager()->GetString( "DST_MOB_TYPE_HUMAN_ANIMAL" );
	case MOB_TYPE_DINOSAUR:					return GetDisplayStringManager()->GetString( "DST_MOB_TYPE_DINOSAUR" );
	case MOB_TYPE_ALIEN:					return GetDisplayStringManager()->GetString( "DST_MOB_TYPE_ALIEN" );
	case MOB_TYPE_ANDROID:					return GetDisplayStringManager()->GetString( "DST_MOB_TYPE_ANDROID" );
	case MOB_TYPE_ROBOT:					return GetDisplayStringManager()->GetString( "DST_MOB_TYPE_ROBOT" );
	case MOB_TYPE_DRAGON:					return GetDisplayStringManager()->GetString( "DST_MOB_TYPE_DRAGON" );
	case MOB_TYPE_DEVIL:					return GetDisplayStringManager()->GetString( "DST_MOB_TYPE_DEVIL" );
	case MOB_TYPE_UNDEAD:					return GetDisplayStringManager()->GetString( "DST_MOB_TYPE_UNDEAD" );
	case MOB_TYPE_PLANT:					return GetDisplayStringManager()->GetString( "DST_MOB_TYPE_PLANT" );
	case MOB_TYPE_INSECT:					return GetDisplayStringManager()->GetString( "DST_MOB_TYPE_INSECT" );
	case MOB_TYPE_HUMAN:					return GetDisplayStringManager()->GetString("DST_MOB_TYPE_HUMAN");
	case MOB_TYPE_NAMEC:					return GetDisplayStringManager()->GetString("DST_MOB_TYPE_NAMEC");
	case MOB_TYPE_MAJIN:					return GetDisplayStringManager()->GetString("DST_MOB_TYPE_MAJIN");
	case MOB_TYPE_BUILDING:					return GetDisplayStringManager()->GetString("DST_MOB_TYPE_BUILDING");
	case MOB_TYPE_ITEM_BOX:					return GetDisplayStringManager()->GetString("DST_MOB_TYPE_ITEM_BOX");
	}

	static WCHAR awcBuffer[] = L"Not exist race name";
	NTL_ASSERT(false, "Logic_GetMobRaceName, Not exist race name of race index : " << byRace);

	return awcBuffer;
}

const WCHAR* Logic_GetSkillName(RwUInt32 tblidx)
{
	static WCHAR wBuf[256];

	CSkillTable* pSkillTable = API_GetTableContainer()->GetSkillTable();
	CTextTable* pSkillTextTable = API_GetTableContainer()->GetTextAllTable()->GetSkillTbl();

	sSKILL_TBLDAT* pSkillData = reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData( tblidx ) );
	sTEXT_TBLDAT* pTextData = NULL;
	if( pSkillData )
	{
		pTextData = reinterpret_cast<sTEXT_TBLDAT*>( pSkillTextTable->FindData( pSkillData->Skill_Name ) );
		if( pTextData )
		{
			return pTextData->wstrText.c_str();
		}
		else
			swprintf_s( wBuf, 256, L"Invalid Name Text Index : %u", pSkillData->Skill_Name );
	}
	else
		swprintf_s( wBuf, 256, L"Invalid Skill Table Index : %u", tblidx );
	
	return wBuf;
}	

const WCHAR* Logic_GetHTBName(RwUInt32 tblidx)
{
	static WCHAR wBuf[256];

	CHTBSetTable* pHTBTable = API_GetTableContainer()->GetHTBSetTable();
	CTextTable* pHTBTextTable = API_GetTableContainer()->GetTextAllTable()->GetHTBSetTbl();

	sHTB_SET_TBLDAT* pHTBData = reinterpret_cast<sHTB_SET_TBLDAT*>( pHTBTable->FindData( tblidx ) );
	sTEXT_TBLDAT* pTextData = NULL;
	if( pHTBData )
	{
		pTextData = reinterpret_cast<sTEXT_TBLDAT*>( pHTBTextTable->FindData( pHTBData->HTB_Skill_Name ) );
		if( pTextData )
		{
			return pTextData->wstrText.c_str();
		}
		else
			swprintf_s( wBuf, 256, L"Invalid Name Text Index : %u", pHTBData->HTB_Skill_Name );
	}
	else
		swprintf_s( wBuf, 256, L"Invalid Skill Table Index : %u", tblidx );

	return wBuf;
}

const WCHAR* Logic_GetItemName(RwUInt32 tblidx)
{
	static WCHAR wBuf[256];

	CItemTable* pItemTable = API_GetTableContainer()->GetItemTable();
	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	sITEM_TBLDAT* pItemData = reinterpret_cast<sITEM_TBLDAT*>( pItemTable->FindData( tblidx ) );
	sTEXT_TBLDAT* pTextData = NULL;
	if( pItemData )
	{
		pTextData = reinterpret_cast<sTEXT_TBLDAT*>( pItemTextTable->FindData( pItemData->Name ) );
		if( pTextData )
		{
			return pTextData->wstrText.c_str();
		}
		else
			swprintf_s( wBuf, 256, L"Invalid Name Text Index : %u", pItemData->Name );
	}
	else
		swprintf_s( wBuf, 256, L"Invalid Item Table Index : %u", tblidx );

	return wBuf;
}

RwBool	Logic_GetSystemEffectText( RwUInt32 uiSystemEffectIdx, RwReal fValue, std::wstring& wstrText, RwUInt8 byPercentType, sSKILL_TBLDAT* pSkillData /* = NULL */, RwUInt32* pColor /* = NULL  */, RwBool* pBuffShow /* = NULL */ )
{
	if( uiSystemEffectIdx == INVALID_TBLIDX )
		return FALSE;

	CSystemEffectTable* pSystemEffectTable = API_GetTableContainer()->GetSystemEffectTable();
	sSYSTEM_EFFECT_TBLDAT* pSystemEffectData = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( pSystemEffectTable->FindData( uiSystemEffectIdx ) );		

	if( !pSystemEffectData ) 
	{
		if( Logic_IsUIDevInfoVisible() )
		{
			WCHAR strError[256];
			swprintf_s( strError, 256, L" (%u) SystemEffectError", uiSystemEffectIdx );
			wstrText = strError;		
		}					

		return FALSE;
	}

	// Buff의 경우 Direct계열은 출력안함
	if( pBuffShow )
	{
		if( pSystemEffectData->byActive_Effect_Type == SKILL_ACTIVE_TYPE_DD ||
			pSystemEffectData->byActive_Effect_Type == SKILL_ACTIVE_TYPE_DH ||
			pSystemEffectData->byActive_Effect_Type == SKILL_ACTIVE_TYPE_DB ||
			pSystemEffectData->byActive_Effect_Type == SKILL_ACTIVE_TYPE_DC )
		{
			*pBuffShow = FALSE;
			return TRUE;
		}

		*pBuffShow = TRUE;
	}

	CTextTable* pSystemEffectText = API_GetTableContainer()->GetTextAllTable()->GetSystemEffectTbl();
	wstrText = pSystemEffectText->GetText( pSystemEffectData->Effect_Info_Text );

	WCHAR buf[256];

	// Excpetion : 스킬 타입 표기 예외
	if( pSkillData )
	{
		if( pSystemEffectData->effectCode == ACTIVE_DIRECT_DAMAGE ||
			pSystemEffectData->effectCode == ACTIVE_BACKSTAB ||
			pSystemEffectData->effectCode == ACTIVE_WARP_BACK_ATTACK ||
			pSystemEffectData->effectCode == ACTIVE_THROUGH_ATTACK )
		{
			switch( pSkillData->bySkill_Type )
			{
				case NTL_SKILL_TYPE_PHYSICAL: 
					wstrText = GetDisplayStringManager()->GetString( "DST_SYSTEMEFFECT_PHYSICAL" ) + wstrText;
					if( pColor ) 
						*pColor = INFOCOLOR_6;
				break;
				case NTL_SKILL_TYPE_ENERGY:
					wstrText = GetDisplayStringManager()->GetString( "DST_SYSTEMEFFECT_ENERGY" ) + wstrText;
					if( pColor ) 
						*pColor = INFOCOLOR_3;
				break;
				case NTL_SKILL_TYPE_STATE:
					wstrText = GetDisplayStringManager()->GetString( "DST_SYSTEMEFFECT_STATE" ) + wstrText;
					if( pColor ) 
						*pColor = INFOCOLOR_7;
				break;
			}	
		}
		else if (pSystemEffectData->effectCode == ACTIVE_TELEPORT_BIND)
		{
			/*v16 = (_DWORD *)sub_461CB0();
			v17 = sub_461E90(v16);
			v13 = v17;
			if (v17)
			{
				if (*(_DWORD *)(v17 + 2664) != -1)
				{
					v18 = *(_DWORD *)(v17 + 2660);
					v19 = sub_46BA10();
					v20 = sub_637810(v19, (char)v9, v18);
					if (v20)
					{
						v13 = (*(int(__thiscall **)(int, _DWORD))(*(_DWORD *)v20 + 12))(v20, *(_DWORD *)(v13 + 2664));
						if (v13)
						{
							v54 = 0;
							v57 = 7;
							v56 = 0;
							v55 = 0;
							v60 = 3;
							v21 = sub_46BA10();
							if (sub_620B70(
								*(void **)(*(_DWORD *)(v21 + 456) + 44),
								(unsigned int)v9,
								*(_DWORD *)(v13 + 848),
								(int)&v54))
							{
								sub_404FD0(a3, v13, &v54, 0, 0xFFFFFFFF);
							}
							v60 = -1;
							sub_404E10(&v54);
						}
					}
				}
			}*/
		}
	}
	WCHAR* szFormat = L"%d";
	RwInt32 nInsertPosition = wstrText.find( szFormat );
	RwBool bINTValue = TRUE;

	if( (RwUInt32)nInsertPosition == std::wstring::npos )
	{
		szFormat = L"%g";
		nInsertPosition = wstrText.find( szFormat );
		bINTValue = FALSE;
	}	

	// Exception : 단위 관련 표기 예외
	if( byPercentType && pSystemEffectData->byActive_Effect_Type != SKILL_ACTIVE_TYPE_DOT)
	{
		if( nInsertPosition >= 0 )
			wstrText.insert( nInsertPosition + 2, L"%%" );		

		if( bINTValue )
			swprintf_s( buf, 256, wstrText.c_str(), (RwInt32)fValue );
		else
			swprintf_s( buf, 256, wstrText.c_str(), fValue );
	}
	else
	{
		if( pSystemEffectData->effectCode == PASSIVE_SKILL_CASTING_TIME_DOWN ||
			pSystemEffectData->effectCode == ACTIVE_PARALYZE_KEEPTIME_DOWN ||
			pSystemEffectData->effectCode == ACTIVE_TERROR_KEEPTIME_DOWN ||
			pSystemEffectData->effectCode == ACTIVE_CONFUSE_KEEPTIME_DOWN ||
			pSystemEffectData->effectCode == ACTIVE_STONE_KEEPTIME_DOWN ||
			pSystemEffectData->effectCode == ACTIVE_CANDY_KEEPTIME_DOWN ||
			pSystemEffectData->effectCode == ACTIVE_BLEEDING_KEEPTIME_DOWN ||
			pSystemEffectData->effectCode == ACTIVE_POISON_KEEPTIME_DOWN ||
			pSystemEffectData->effectCode == ACTIVE_STOMACHACHE_KEEPTIME_DOWN )
		{
			if( nInsertPosition >= 0 )
				wstrText.replace( nInsertPosition, 2, L"%s" );

			swprintf_s( buf, 256, wstrText.c_str(), Logic_GetTimeString( fValue ) );			
		}	
		else if( pSystemEffectData->effectCode == PASSIVE_APPLY_AREA_SIZE_UP || 
				 pSystemEffectData->effectCode == PASSIVE_USE_RANGE_UP ||
				 pSystemEffectData->effectCode == ACTIVE_ATTACK_RANGE_UP ||
				 pSystemEffectData->effectCode == ACTIVE_ATTACK_RANGE_DOWN )
		{
			if( nInsertPosition >= 0 )
				wstrText.insert( nInsertPosition + 2, GetDisplayStringManager()->GetString( "DST_SYSTEMEFFECT_METER" ) );		

			if( bINTValue )
				swprintf_s( buf, 256, wstrText.c_str(), (RwInt32)fValue );			
			else
				swprintf_s( buf, 256, wstrText.c_str(), fValue );			
		}
		else if( pSystemEffectData->effectCode == PASSIVE_SKILL_KEEP_TIME_UP ||
			     pSystemEffectData->effectCode == PASSIVE_SKILL_COOL_TIME_DOWN
			)
		{
			if( pSkillData && pSkillData->Root_Skill != INVALID_TBLIDX )
			{
				if( nInsertPosition >= 0 )
					wstrText.replace( nInsertPosition, 2, L"%s" );

				swprintf_s( buf, 256, wstrText.c_str(), Logic_GetTimeString( fValue ) );			
			}
			else
			{
				if( nInsertPosition >= 0 )
					wstrText.insert( nInsertPosition + 2, L"%%" );		

				if( bINTValue )
					swprintf_s( buf, 256, wstrText.c_str(), (RwInt32)fValue );
				else
					swprintf_s( buf, 256, wstrText.c_str(), fValue );
			}
		}	
		else if( pSystemEffectData->effectCode == PASSIVE_REQUIRE_EP_DOWN )
		{
			if( pSkillData && pSkillData->Root_Skill != INVALID_TBLIDX )
			{
				if( bINTValue )
					swprintf_s( buf, 256, wstrText.c_str(), (RwInt32)fValue );
				else
					swprintf_s( buf, 256, wstrText.c_str(), fValue );
			}
			else
			{
				if( nInsertPosition >= 0 )
					wstrText.insert( nInsertPosition + 2, L"%%" );		

				if( bINTValue )
					swprintf_s( buf, 256, wstrText.c_str(), (RwInt32)fValue );
				else
					swprintf_s( buf, 256, wstrText.c_str(), fValue );
			}
		}
		else
		{
			if( bINTValue )
				swprintf_s( buf, 256, wstrText.c_str(), (RwInt32)fValue );
			else
				swprintf_s( buf, 256, wstrText.c_str(), fValue );
		}
	}

	wstrText = buf;	

	return TRUE;
}


void Logic_DialogModeMessage(eDialogMode eMode)
{
	switch(eMode)
	{
	case DIALOGMODE_ITEM_REPAIR:
		{
			GetAlarmManager()->AlarmMessage("DST_DIALOG_MODE_ITEM_REPAIR");
			break;
		}
	case DIALOGMODE_ITEM_IDENTIFICATION:
	case DIALOGMODE_NPCSHOP_ITEM_IDENTIFICATION:
		{
			GetAlarmManager()->AlarmMessage("DST_DIALOG_MODE_ITEM_IDENTIFICATION");
			break;
		}
	case DIALOGMODE_NARRATION:
		{
			GetAlarmManager()->AlarmMessage("DST_DIALOG_MODE_NARRATION");
			break;
		}
	case DIALOGMODE_CHATTING_RESIZE_HORI:
		{
			GetAlarmManager()->AlarmMessage("DST_DIALOG_MODE_CHAT_RESIZING");
			break;
		}
	case DIALOGMODE_CHATTING_RESIZE_RIGHTUP:
		{
			GetAlarmManager()->AlarmMessage("DST_DIALOG_MODE_CHAT_RESIZING");
			break;
		}
	case DIALOGMODE_CHATTING_RESIZE_RIGHTDOWN:
		{
			GetAlarmManager()->AlarmMessage("DST_DIALOG_MODE_CHAT_RESIZING");
			break;
		}
	case DIALOGMODE_CHATTING_RESIZE_VERT:
		{
			GetAlarmManager()->AlarmMessage("DST_DIALOG_MODE_CHAT_RESIZING");
			break;
		}
	default:
		{
			NTL_ASSERT(false, "Wrong dialog mode : " << eMode);
		}
	}
}

std::string Logic_GetWarnningWorldConceptActionMessageIndex(RwUInt32 uiGrade)
{
	if( uiGrade == WORLD_CONCEPT_FIRST_GRADE )
	{
		CNtlWorldConceptController *pController = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_FIRST_GRADE);
		if(pController == NULL)
			return "";

		switch(pController->GetConceptType())
		{
			case WORLD_PLAY_TUTORIAL:
			{
				return "DST_WORLD_CONCEPT_TUTORIAL";
			}
			case WORLD_PLAY_TIME_MACHINE:
			{
				return "DST_WORLD_CONCEPT_TMQ";
			}
			case WORLD_PLAY_RANK_BATTLE:
			{
				return "DST_WORLD_CONCEPT_RANK_BATTLE";
			}
			case WORLD_PLAY_T_BUDOKAI:
			{
				return "DST_WORLD_CONCEPT_BUDOKAI";
			}
			case WORLD_PLAY_PARTY_DUNGEON:
			{
				return "DST_WORLD_CONCEPT_PARTY_DUNGEON";
			}
			case WORLD_PLAY_TLQ:
			{
				return "DST_WORLD_CONCEPT_TLQ";
			}
			case WORLD_PLAY_DOJO_SCRAMBLE:
			{
				return "DST_WORLD_CONCEPT_DOJO_SCRAMBLE";
			}
			case WORLD_PLAY_CCBD:
			{
				return "DST_WORLD_CONCEPT_CCBD";
			}
			case WORLD_PLAY_DWC:
			{
				return "DST_WORLD_CONCPET_DWC";
			}
			default:
			{
				DBO_FAIL("Invalid world concept");
			}
		}
	}
	else if( uiGrade == WORLD_CONCEPT_SECOND_GRADE )
	{
		CNtlWorldConceptController *pController = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_SECOND_GRADE);
		if(pController == NULL)
			return "";

		switch(pController->GetConceptType())
		{
			case WORLD_PLAY_FREEPVP:
			{
				return "DST_WORLD_CONCEPT_FREEBATTLE";
			}
			case WORLD_PLAY_NPC_COMMU:
			{
				return "DST_WORLD_CONCEPT_NPC_COMMUNICATION";
			}
			case WORLD_PLAY_TRADE:
			{
				return "DST_WORLD_CONCEPT_TRADE";
			}
			case WORLD_PLAY_DRAGONBALL_COLLECT:
			{
				return "DST_WORLD_CONCEPT_PLAY_DRAGONBALL";
			}
			case WORLD_PLAY_PRIVATESHOP:
			{
				return "DST_WORLD_CONCEPT_PLAY_PRIVATESHOP";
			}
			case WORLD_PLAY_PRIVATESHOP_VISITOR:
			{
				return "DST_WORLD_CONCEPT_PLAY_PRIVATESHOP_VISITOR";
			}
			case WORLD_PLAY_REMOTE_BEHAVIOUR:
			{
				return "DST_WORLD_CONCPET_REMOTE_BEHAVIOR";
			}
			case WORLD_PLAY_MASCOTEX_SKILL:
			{
				return "DST_WORLD_CONCPET_MASCOTEX_SKILL";
			}
			default:
			{
				DBO_FAIL("Invalid world concept");
			}
		}
	}
	else if (uiGrade == WORLD_CONCEPT_THIRD_GRADE)
	{
		CNtlWorldConceptController *pController = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_THIRD_GRADE);
		if (pController == NULL)
			return "";

		switch (pController->GetConceptType())
		{
			case WORLD_PLAY_DIRECT:
			{
				return "DST_WORLD_CONCEPT_PLAY_DIRECT";
			}
			default:
			{
				DBO_FAIL("Invalid world concept");
			}
		}
	}
	else if (uiGrade == WORLD_CONCEPT_FOURTH_GRADE)
	{
		CNtlWorldConceptController *pController = GetNtlWorldConcept()->FindGradeExtController(WORLD_CONCEPT_FOURTH_GRADE);
		if (pController == NULL)
			return "";

		switch (pController->GetConceptType())
		{
			case WORLD_PLAY_DRAGONBALL_SCRAMBLE:
			{
				return "DST_WORLD_CONCEPT_DRAGONBALL_SCRAMBLE";
			}
			default:
			{
				DBO_FAIL("Invalid world concept");
			}
		}
	}

	return "";
}

void Logic_WarnningWorldConceptActionMessage(RwUInt32 uiGrade)
{
	std::string string = Logic_GetWarnningWorldConceptActionMessageIndex(uiGrade);

	if(string.length() > 1)
		GetAlarmManager()->AlarmMessage(string);
}

// 테이블 인덱스로 지역의 이름을 가져온다.
// return FALSE면 테이블에 없음
RwBool Logic_GetAreaNameFromTblIDX( TBLIDX tblIdx, std::wstring* pstrString ) 
{
	CMapNameTextTable* pTable = API_GetTableContainer()->GetTextAllTable()->GetMapNameTbl();
	if(!pTable->GetAreaName(tblIdx, pstrString))
	{
		if(!pTable->GetSectorName(tblIdx, pstrString))
		{
			if(!pTable->GetZoneName(tblIdx, pstrString))
			{
				// 텍스트 테이블이 존재하지 않는다
				static WCHAR awcBuffer[] = L"Not exist terrain name";
				*pstrString = awcBuffer;
				return FALSE;
			}
		}
	}

	return TRUE;
}

const WCHAR* Logic_GetBattleAttributeName(RwUInt8 byAttr)
{
	switch( byAttr )
	{
	case BATTLE_ATTRIBUTE_NONE:				return GetDisplayStringManager()->GetString( "DST_ITEM_ATTR_NONE" );
	case BATTLE_ATTRIBUTE_HONEST:			return GetDisplayStringManager()->GetString( "DST_ITEM_ATTR_HONEST" );
	case BATTLE_ATTRIBUTE_STRANGE:			return GetDisplayStringManager()->GetString( "DST_ITEM_ATTR_STRANGE" );
	case BATTLE_ATTRIBUTE_WILD:				return GetDisplayStringManager()->GetString( "DST_ITEM_ATTR_WILD" );
	case BATTLE_ATTRIBUTE_ELEGANCE:			return GetDisplayStringManager()->GetString( "DST_ITEM_ATTR_ELEGANCE" );
	case BATTLE_ATTRIBUTE_FUNNY:			return GetDisplayStringManager()->GetString( "DST_ITEM_ATTR_FUNNY" );
	case BATTLE_ATTRIBUTE_UNKNOWN:			return L"";
	}

	static WCHAR awcBuffer[] = L"Not exist attribute name";
	NTL_ASSERT(false, "Logic_GetBattleAttributeName, Not exist attribute name of attribute index : " << byAttr);

	return awcBuffer;
}

void Logic_timeToDetailTime(time_t tTime, sDetailTime& sTime)
{
	tm curTime;

	localtime_s(&curTime, &tTime);

	sTime.uiYear		= curTime.tm_year + 1900;
	sTime.byMonth		= (RwUInt8)(curTime.tm_mon + 1);
	sTime.byDay			= (RwUInt8)curTime.tm_mday;
	sTime.byHour		= (RwUInt8)curTime.tm_hour;
	sTime.byMinute		= (RwUInt8)curTime.tm_min;
	sTime.bySecond		= (RwUInt8)curTime.tm_sec;


	switch(curTime.tm_wday)
	{
	case 0:			sTime.strDay = GetDisplayStringManager()->GetString("DST_SUNDAY");		break;
	case 1:			sTime.strDay = GetDisplayStringManager()->GetString("DST_MONDAY");		break;
	case 2:			sTime.strDay = GetDisplayStringManager()->GetString("DST_TUESDAY");		break;
	case 3:			sTime.strDay = GetDisplayStringManager()->GetString("DST_WEDNESDAY");		break;
	case 4:			sTime.strDay = GetDisplayStringManager()->GetString("DST_THURSDAY");		break;
	case 5:			sTime.strDay = GetDisplayStringManager()->GetString("DST_FRIDAY");		break;
	case 6:			sTime.strDay = GetDisplayStringManager()->GetString("DST_SATURDAY");		break;	
	default:		NTL_ASSERT(false, "Logic_timeToDetailTime, localtime_s function return wrong value : " << curTime.tm_wday);
	}


	sTime.byDay_of_Year	= (RwUInt8)(curTime.tm_yday + 1);
}

WCHAR* Logic_GetTimeString(RwReal fSecond)
{
	static WCHAR awcBuffer[256] = L"";
	RwUInt32 uiDay, uiHour, uiMinute, uiSecond;
	RwReal fLeftSecond = fSecond;

	uiDay = uiHour = uiMinute = uiSecond = 0;
	memset(awcBuffer, 0, 64);


	// 시간 계산
	uiDay = (RwUInt32)(fLeftSecond / 86400.f);

	fLeftSecond -= (RwReal)uiDay * 86400.f;
	uiHour = (RwUInt32)(fLeftSecond / 3600.f);

	fLeftSecond -= (RwReal)uiHour * 3600.f;
	uiMinute = (RwUInt32)(fLeftSecond / 60.f);

	fLeftSecond -= (RwReal)uiMinute * 60.f;
	uiSecond = (RwUInt32)fLeftSecond;
	

	// 시간 스트링 조합
	if( uiDay > 0 )
	{
		const WCHAR* pwcText = GetDisplayStringManager()->GetString("DST_TIME_DAY");
		swprintf_s(awcBuffer, 64, L"%u%s", uiDay, pwcText);
	}

	if( uiHour > 0 )
	{
		const WCHAR* pwcText = GetDisplayStringManager()->GetString("DST_TIME_AN_HOUR");
		RwUInt32 uiBufferStart = wcslen(awcBuffer);

		// 이미 스트링이 있다. 빈칸을 확보하자
		if( uiBufferStart > 0 )
		{
			swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L" ");
			++uiBufferStart;
		}

		swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L"%u%s", uiHour, pwcText);
	}

	if( uiMinute > 0 )
	{
		const WCHAR* pwcText	= GetDisplayStringManager()->GetString("DST_TIME_MINUTE");
		RwUInt32 uiBufferStart = wcslen(awcBuffer);

		// 이미 스트링이 있다. 빈칸을 확보하자
		if( uiBufferStart > 0 )
		{
			swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L" ");
			++uiBufferStart;
		}

		swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L"%u%s", uiMinute, pwcText);
	}

	if( uiSecond > 0 ||
		( !uiDay && !uiHour && !uiMinute ) )
	{
		const WCHAR* pwcText	= GetDisplayStringManager()->GetString("DST_TIME_SECOND");
		RwUInt32 uiBufferStart = wcslen(awcBuffer);

		// 이미 스트링이 있다. 빈칸을 확보하자
		if( uiBufferStart > 0 )
		{
			swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L" ");
			++uiBufferStart;
		}

		swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L"%u%s", uiSecond, pwcText);
	}
	
	return awcBuffer;
}

WCHAR * Logic_GetTimeStringWithMillisecond(RwReal fSecond)
{
	static WCHAR awcBuffer[256] = L"";
	RwUInt32 uiDay, uiHour, uiMinute;
	float fSecondWithMs = 0.f;
	RwReal fLeftSecond = fSecond;

	uiDay = uiHour = uiMinute = 0;
	memset(awcBuffer, 0, 64);


	// 시간 계산
	uiDay = (RwUInt32)(fLeftSecond / 86400.f);

	fLeftSecond -= (RwReal)uiDay * 86400.f;
	uiHour = (RwUInt32)(fLeftSecond / 3600.f);

	fLeftSecond -= (RwReal)uiHour * 3600.f;
	uiMinute = (RwUInt32)(fLeftSecond / 60.f);

	fLeftSecond -= (RwReal)uiMinute * 60.f;
	fSecondWithMs = fLeftSecond;


	// 시간 스트링 조합
	if (uiDay > 0)
	{
		const WCHAR* pwcText = GetDisplayStringManager()->GetString("DST_TIME_DAY");
		swprintf_s(awcBuffer, 64, L"%u%s", uiDay, pwcText);
	}

	if (uiHour > 0)
	{
		const WCHAR* pwcText = GetDisplayStringManager()->GetString("DST_TIME_AN_HOUR");
		RwUInt32 uiBufferStart = wcslen(awcBuffer);

		// 이미 스트링이 있다. 빈칸을 확보하자
		if (uiBufferStart > 0)
		{
			swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L" ");
			++uiBufferStart;
		}

		swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L"%u%s", uiHour, pwcText);
	}

	if (uiMinute > 0)
	{
		const WCHAR* pwcText = GetDisplayStringManager()->GetString("DST_TIME_MINUTE");
		RwUInt32 uiBufferStart = wcslen(awcBuffer);

		// 이미 스트링이 있다. 빈칸을 확보하자
		if (uiBufferStart > 0)
		{
			swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L" ");
			++uiBufferStart;
		}

		swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L"%u%s", uiMinute, pwcText);
	}

	if (fSecondWithMs > 0.0f ||
		(!uiDay && !uiHour && !uiMinute))
	{
		const WCHAR* pwcText = GetDisplayStringManager()->GetString("DST_TIME_SECOND");
		RwUInt32 uiBufferStart = wcslen(awcBuffer);

		// 이미 스트링이 있다. 빈칸을 확보하자
		if (uiBufferStart > 0)
		{
			swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L" ");
			++uiBufferStart;
		}

		swprintf_s(awcBuffer + uiBufferStart, 64 - uiBufferStart, L"%.1f%s", fSecondWithMs, pwcText);
	}

	return awcBuffer;
}

WCHAR * Logic_GetTimeRemainString(DBOTIME timeRemain)
{
	static WCHAR awcBuffer[256] = L"";

	memset(awcBuffer, 0, 64);

	const WCHAR* pStringDay = GetDisplayStringManager()->GetString("DST_TIME_DAY");
	RwUInt32 uiDay = (RwUInt32)(timeRemain / 86400);

	DBOTIME timeExceptDay = timeRemain - (uiDay * 86400);
	RwUInt32 uiHour = (RwUInt32)(timeExceptDay / 3600);

	if (uiDay > 0)
	{
		if (uiHour > 0)
		{
			const WCHAR* pStringHour = GetDisplayStringManager()->GetString("DST_TIME_AN_HOUR");
			swprintf_s(awcBuffer, 256, L"%u%s %u%s", uiDay, pStringDay, uiHour, pStringHour);
		}
		else
			swprintf_s(awcBuffer, 256, L"%u%s", uiDay, pStringDay);
	}
	else
	{
		if (uiHour > 0)
		{
			const WCHAR* pStringHour = GetDisplayStringManager()->GetString("DST_TIME_AN_HOUR");
			swprintf_s(awcBuffer, 256, L"%u%s", uiHour, pStringHour);
		}
		else
			swprintf_s(awcBuffer, 256, L"%s", GetDisplayStringManager()->GetString("DST_COMMERCIAL_USE_TERM_LIMETED_ITEM_INFO_BELOW_HOUR"));
	}

	return awcBuffer;
}

WCHAR * Logic_GetTimeTotalString(DWORD timeSeconds)
{
	static WCHAR awcBuffer[256] = L"";

	memset(awcBuffer, 0, 64);

	const WCHAR* pStringDay = GetDisplayStringManager()->GetString("DST_TIME_DAY");
	RwUInt32 uiDay = (RwUInt32)(timeSeconds / 86400);

	DBOTIME timeExceptDay = timeSeconds - (uiDay * 86400);
	RwUInt32 uiHour = (RwUInt32)(timeExceptDay / 3600);

	if (uiDay > 0)
	{
		if (uiHour > 0)
		{
			const WCHAR* pStringHour = GetDisplayStringManager()->GetString("DST_TIME_AN_HOUR");
			swprintf_s(awcBuffer, 256, L"%u%s %u%s", uiDay, pStringDay, uiHour, pStringHour);
		}
		else
			swprintf_s(awcBuffer, 256, L"%u%s", uiDay, pStringDay);
	}
	else
	{
		if (uiHour > 0)
		{
			const WCHAR* pStringHour = GetDisplayStringManager()->GetString("DST_TIME_AN_HOUR");
			swprintf_s(awcBuffer, 256, L"%u%s", uiHour, pStringHour);
		}
	}

	return awcBuffer;
}

WCHAR* Logic_GetCoolTimeString(RwReal fSecond)
{
	static WCHAR awcBuffer[64] = L"";	

	if( fSecond <= 60.f )
	{
		const WCHAR* pwcText	= GetDisplayStringManager()->GetString("DST_TIME_SECOND");

		swprintf_s(awcBuffer, 64, L"%u%s", (RwUInt32)fSecond, pwcText);
	}
	else if( fSecond < 3600.f )
	{
		const WCHAR* pwcText	= GetDisplayStringManager()->GetString("DST_TIME_MINUTE");
		RwUInt32 uiMinute		= (RwUInt32)(fSecond / 60.f) + 1;

		swprintf_s(awcBuffer, 64, L"%u%s", uiMinute, pwcText);
	}
	else if( fSecond <= 86400.f )
	{
		const WCHAR* pwcText	= GetDisplayStringManager()->GetString("DST_TIME_AN_HOUR");
		RwUInt32 uiHour		= (RwUInt32)(fSecond / 3600.f);

		RwReal fLeftSecond = fSecond - ((RwReal)uiHour * 3600.f);
		RwUInt32 uiMinute = (RwUInt32)(fLeftSecond / 60.f);

		if( uiMinute > 0 )
		{
			const WCHAR* pwcText2 = GetDisplayStringManager()->GetString("DST_TIME_MINUTE");
			swprintf_s(awcBuffer, 64, L"%u%s %d%s", uiHour, pwcText, uiMinute, pwcText2);
		}
		else
			swprintf_s(awcBuffer, 64, L"%u%s", uiHour, pwcText);
	}
	else
	{
		const WCHAR* pwcText	= GetDisplayStringManager()->GetString("DST_TIME_DAY");
		RwUInt32 uiDay		= (RwUInt32)(fSecond / 86400.f);

		RwReal fLeftSecond = fSecond - ((RwReal)uiDay * 86400.f);
		RwUInt32 uiHour = (RwUInt32)(fLeftSecond / 3600.f);

		if( uiHour > 0 )
		{
			const WCHAR* pwcText2 = GetDisplayStringManager()->GetString("DST_TIME_AN_HOUR");
			swprintf_s(awcBuffer, 64, L"%u%s %u%s", uiDay, pwcText, uiHour, pwcText2);
		}
		else
			swprintf_s(awcBuffer, 64, L"%u%s", uiDay, pwcText);
	}

	return awcBuffer;
}

/**
* \brief 천하제일 무도회의 매치 스테이트를 스트링으로 반환한다.
*/
const WCHAR* Logic_GetStringTBMatchType( RwUInt8 byTBMatchType )
{
	if( byTBMatchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL )
	{
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_SOLO" );
	}
	else if( byTBMatchType == BUDOKAI_MATCH_TYPE_TEAM )
	{
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_NOTICE_PARTY" );
	}
	
	static WCHAR awcBuffer[] = L"Invalid Budokai Match Type(INDI/TEAM)";
	return awcBuffer;
}

const WCHAR* Logic_GetStringTBMatchState( RwUInt8 byTBMatchState )
{
	switch( byTBMatchState )
	{
	case BUDOKAI_MATCHSTATE_REGISTER:
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_REGISTER_INFO_TITLE" ); 				// 등록 기간
	case BUDOKAI_MATCHSTATE_WAIT_MINOR_MATCH:		// 예선전 대기
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_MINORMATCH_INFO_TITLE_WAIT" );
	case BUDOKAI_MATCHSTATE_MINOR_MATCH:				// 예선전 
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_MINORMATCH_INFO_TITLE_BATTLE" );
	case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_32:
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_MAJORMATCH_INFO_TITLE_32_WAIT" );
	case BUDOKAI_MATCHSTATE_MAJORMATCH_32:
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_MAJORMATCH_INFO_TITLE_32" );
	case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_16:
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_MAJORMATCH_INFO_TITLE_16_WAIT" );
	case BUDOKAI_MATCHSTATE_MAJORMATCH_16:
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_MAJORMATCH_INFO_TITLE_16" );
	case BUDOKAI_MATCHSTATE_WAIT_MAJORMATCH_08:
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_MAJORMATCH_INFO_TITLE_8_WAIT" );
	case BUDOKAI_MATCHSTATE_MAJORMATCH_08:
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_MAJORMATCH_INFO_TITLE_8" );
	case BUDOKAI_MATCHSTATE_WAIT_SEMIFINAL_MATCH:	// 준결승 대기
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_MAJORMATCH_INFO_TITLE_SEMIFINAL_WAIT" );
	case BUDOKAI_MATCHSTATE_SEMIFINAL_MATCH:
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_MAJORMATCH_INFO_TITLE_SEMIFINAL" );
	case BUDOKAI_MATCHSTATE_FINAL_MATCH:
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_MAJORMATCH_INFO_TITLE_FINAL" );
	case BUDOKAI_MATCHSTATE_MATCH_END:
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_MAJORMATCH_INFO_TITLE_END" );
	default:
		break;
	}

	static WCHAR awcBuffer[] = L"Invalid Budokai Match State(REGISTER ~ END)";
	return awcBuffer;
}

/**
* \brief 현재 아바타의 참가 상태를 스트링으로 가져온다. ( 예선 진출자, 16강 진출자, 탈락자 등등 )
*/
const WCHAR* Logic_GetAvatarTBJoinState()
{
	CNtlSobAvatar* pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();

	sBUDOKAI_JOIN_INFO* pJoinInfo;

	// NULL 일 때는 아바타 생성 전에 정보가 들어왔을 경우
	// NULL이 아닐 때는 정상적으로 데이터가 셋팅 되었을 경우
	if( pSobAvatar == NULL )
		pJoinInfo = &GetNtlSLGlobal()->GetTBudokaiStateInfo()->sJoinInfo;
	else
		pJoinInfo = pSobAvatar->GetTenkaichiBudokai()->GetBudokaiJoinInfo();	
	
	if( pJoinInfo->byJoinState == INVALID_BUDOKAI_JOIN_STATE )
	{
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_NONE" );
	}
	else
	{
		if( pJoinInfo->byJoinState == BUDOKAI_JOIN_STATE_DROPOUT )
		{
			switch( pJoinInfo->byJoinResult )
			{
			case BUDOKAI_JOIN_RESULT_REGISTER:
			case BUDOKAI_JOIN_RESULT_MINORMATCH:
				return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_MINOR_DROPOUT" );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_32:
				return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_32_DROPOUT" );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_16:
				return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_16_DROPOUT" );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_8:
				return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_8_DROPOUT" );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_4:
				return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_PRIZE_WINNER" );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_2:
				return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_SEMIFINAL_WINNER" );
				break;
			case BUDOKAI_JOIN_RESULT_WINNER:
				return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_FINAL_WINNER" );
				break;
			default:
				break;
			}	
		}
		else if( pJoinInfo->byJoinState == BUDOKAI_JOIN_STATE_PLAY )
		{
			switch( pJoinInfo->byJoinResult )
			{
			case BUDOKAI_JOIN_RESULT_REGISTER:
				return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_MINOR_APPLICANT" );
				break;
			case BUDOKAI_JOIN_RESULT_MINORMATCH:
				return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_MINOR_ENTRY" );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_32:
				return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_32_ENTRY" );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_16:
				return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_16_ENTRY" );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_8:
				return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_8_ENTRY" );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_4:
				return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_SEMIFINAL_ENTRY" );
				break;
			case BUDOKAI_JOIN_RESULT_ENTER_2:
				return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_FINAL_ENTRY" );
				break;
			case BUDOKAI_JOIN_RESULT_WINNER:
				return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_FINAL_WINNER" );
				break;
			default:
				break;
			}
		}
	}

	return GetDisplayStringManager()->GetString( "DST_BUDOKAI_PC_STATE_NONE" );
}

/**
* \brief 천하제일 무도회 64 비트 정수 시간에 관련된 스트링 ( 단위에 따라서 다르게 리턴된다. )
*/
const WCHAR* Logic_GetTBRemainTimeString( BUDOKAITIME tmTime )
{
	static WCHAR awcBuffer[256] = L"";
	
	RwReal fDay = (RwReal)tmTime / 86400.f;
	if( fDay >= 1.f )
	{
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_TIME_DAY" ), (RwInt32)fDay );
		
		return awcBuffer;
	}

	RwReal fHour = (RwReal)tmTime / 3600.f;
	if( fHour >= 1.f )
	{
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_TIME_HOUR" ), (RwInt32)fHour );

		return awcBuffer;
	}

	RwReal fMinute = (RwReal)tmTime / 60.f;
	if( fMinute >= 1.f )
	{
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_TIME_MINUTE" ), (RwInt32)fMinute );

		return awcBuffer;
	}

	swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_TIME_SECOND" ), tmTime );
	
	return  awcBuffer;
}

const WCHAR* Logic_GetTBDateToDateString( tm tmAtTime, tm tmToTime )
{
	WCHAR awcBuffer[256];
	WCHAR awcBuffer2[256];

	static WCHAR awcBufferResult[512];

	// 도장 추천 기간
	// 예선전 기간
	swprintf_s( awcBuffer, 128, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME"),
		tmAtTime.tm_mon+1, tmAtTime.tm_mday, 
		(tmAtTime.tm_hour >= 12 ? GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_PM" ) : GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_AM" ) ),
		(tmAtTime.tm_hour > 12 ? (tmAtTime.tm_hour) - 12 : tmAtTime.tm_hour ),
		tmAtTime.tm_min );

	swprintf_s( awcBuffer2, 128, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME"),
		tmToTime.tm_mon+1, tmToTime.tm_mday, 
		(tmToTime.tm_hour >= 12 ? GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_PM" ) : GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_AM" ) ),
		(tmToTime.tm_hour > 12 ? (tmToTime.tm_hour) - 12 : tmToTime.tm_hour ),
		tmToTime.tm_min );

	swprintf_s( awcBufferResult, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_FULL" ),
		awcBuffer, awcBuffer2 );
	
	return awcBufferResult;
}

const WCHAR* Logic_GetTBTypeString( RwUInt8 byMainState )
{
	static WCHAR awcBuffer[16] = L"";

	if( GetBudokaiType(byMainState) == BUDOKAI_TYPE_JUNIOR )
	{
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_TYPE_CHILD_TITLE" );
	}
	else
	{
		return GetDisplayStringManager()->GetString( "DST_BUDOKAI_TYPE_ADULT_TITLE" );
	}

	return awcBuffer;
}

const WCHAR* Logic_GetTBDateString( tm tmTime )
{
	static WCHAR awcBuffer[128];

	swprintf_s( awcBuffer, 128, GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME"),
		tmTime.tm_mon+1, tmTime.tm_mday, 
		(tmTime.tm_hour >= 12 ? GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_PM" ) : GetDisplayStringManager()->GetString( "DST_BUDOKAI_NEWS_NOTICE_GUIDE_TIME_AM" ) ),
		(tmTime.tm_hour > 12 ? (tmTime.tm_hour) - 12 : tmTime.tm_hour ),
		tmTime.tm_min );

	return awcBuffer;
}

const WCHAR* Logic_GetMailRemainTimeString( DBOTIME tmTime )
{
	static WCHAR awcBuffer[256] = L"";

	DBOTIME serverTime = GetNtlSLGlobal()->GetServerTime();

	tm tmServerTime;

	errno_t errid = _localtime64_s( &tmServerTime, (__time64_t*)&serverTime );
	if( 0 != errid )
	{
		DBO_FAIL( "Logic_GetMailRemainTimeString - "<< serverTime );
		return awcBuffer;
	}

	DBOTIME remainTime = tmTime - serverTime; 
	if( remainTime <= 0 )
	{
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_TIME_MINUTE" ), 0. );
		return awcBuffer;
	}

	tm tmRemainTime;
	
	errid = _localtime64_s( &tmRemainTime, (__time64_t*)&remainTime );
	if( 0 != errid )
	{
		DBO_FAIL( "Logic_GetMailRemainTimeString - "<< remainTime );
		return awcBuffer;
	}

	if( tmRemainTime.tm_yday > 0 )
	{
		// 남은 일수
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_TIME_DAY" ), tmRemainTime.tm_yday );

		return awcBuffer;
	}

	if( tmRemainTime.tm_hour > 0 )
	{
		// 남은 시간
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_TIME_HOUR" ), tmRemainTime.tm_hour );

		return awcBuffer;
	}

	if( tmRemainTime.tm_min > 0 )
	{
		// 남은 분
		swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_TIME_MINUTE" ), tmRemainTime.tm_min );

		return awcBuffer;
	}

	swprintf_s( awcBuffer, 256, GetDisplayStringManager()->GetString( "DST_BUDOKAI_TIME_MINUTE" ), 0. );

	return  awcBuffer;
}

void Logic_NameNotify(const WCHAR* pwcText)
{
	if( !pwcText )
		return;

	if( wcsstr( pwcText, Logic_GetAvatarName() ) )
		CDboEventGenerator::NameNotify((WCHAR*)pwcText);
}

bool Logic_IsUsableCharacterName(const WCHAR* pwcText)
{
	if (!pwcText)
	{
		return false;
	}

	RwUInt32 uiTextLength = wcslen(pwcText);
	if (0 == uiTextLength)
	{
		return false;
	}

	if (uiTextLength < NTL_MIN_SIZE_CHAR_NAME)
	{
		return false;
	}

	if (uiTextLength > NTL_MAX_SIZE_CHAR_NAME)
	{
		return false;
	}

	if (GetChattingFilter()->IsAnyInvalidText(pwcText))
	{
		return false;
	}

	return true;
}

RwBool Logic_IsUsableCharacterName_with_WarningMessage(const WCHAR* pwcText)
{
	if( !pwcText )
	{
		GetAlarmManager()->AlarmMessage("DST_LOBBY_EMPTYNAME");
		return FALSE;
	}

	RwUInt32 uiTextLength = wcslen(pwcText);
	if( 0 == uiTextLength )
	{
		GetAlarmManager()->AlarmMessage("DST_LOBBY_EMPTYNAME");
		return FALSE;
	}

	if (uiTextLength < NTL_MIN_SIZE_CHAR_NAME)
	{
		GetAlarmManager()->AlarmMessage("CHARACTER_TOO_SHORT_NAME");
		return FALSE;
	}

	if (uiTextLength > NTL_MAX_SIZE_CHAR_NAME)
	{
		GetAlarmManager()->AlarmMessage("CHARACTER_TOO_LONG_NAME");
		return FALSE;
	}

	if( GetChattingFilter()->IsAnyInvalidText(pwcText) )
	{
		GetAlarmManager()->AlarmMessage( "DST_LOBBY_CAN_NOT_USE_THE_WORD" );
		return FALSE;
	}

	return TRUE;
}

const WCHAR* Logic_GetHoipoiMixRecipeName(RwUInt32 tblIdx)
{
	static WCHAR wBuf[256];

	CItemRecipeTable* pRecipeItemTable = API_GetTableContainer()->GetItemRecipeTable();
	CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();

	sITEM_RECIPE_TBLDAT* pRecipeItemData = reinterpret_cast<sITEM_RECIPE_TBLDAT*>( pRecipeItemTable->FindData( tblIdx ) );
	sTEXT_TBLDAT* pTextData = NULL;
	if( pRecipeItemData )
	{
		pTextData = reinterpret_cast<sTEXT_TBLDAT*>( pItemTextTable->FindData( pRecipeItemData->dwName ) );
		if( pTextData )
		{
			return pTextData->wstrText.c_str();
		}
		else
			swprintf_s( wBuf, 256, L"Invalid Name Text Index : %u", pRecipeItemData->dwName );
	}
	else
		swprintf_s( wBuf, 256, L"Invalid Item Table Index : %u", tblIdx );

	return wBuf;
}

const WCHAR* Logic_GetTeleportTypeName(RwUInt32 uiTeleportType)
{
	switch( uiTeleportType )
	{
		case TELEPORT_TYPE_GAME_IN:			return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_GAME_IN" );
		case TELEPORT_TYPE_GAME_OUT:		return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_GAME_OUT" );
		case TELEPORT_TYPE_SKILL:			return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_SKILL" );
		case TELEPORT_TYPE_NPC_PORTAL:		return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_NPC_PORTAL" );
		case TELEPORT_TYPE_POPOSTONE:		return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_POPOSTONE" );
		case TELEPORT_TYPE_WORLD_MOVE:		return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_WORLD_MOVE" );
		case TELEPORT_TYPE_TMQ_WORLD_MOVE:	return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_TMQ_WORLD_MOVE" );
		case TELEPORT_TYPE_TMQ_PORTAL:		return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_TMQ_PORTAL" );
		case TELEPORT_TYPE_RANKBATTLE:		return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_RANKBATTLE" );
		case TELEPORT_TYPE_TIMEQUEST:		return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_TIMEQUEST" );
		case TELEPORT_TYPE_TUTORIAL:		return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_TUTORIAL" );
		case TELEPORT_TYPE_BUDOKAI:			return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_BUDOKAI" );
		case TELEPORT_TYPE_MATCH:			return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_MATCH" );
		case TELEPORT_TYPE_MINORMATCH:		return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_MINORMATCH" );
		case TELEPORT_TYPE_MAJORMATCH:		return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_MAJORMATCH" );
		case TELEPORT_TYPE_FINALMATCH:		return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_FINALMATCH" );
		case TELEPORT_TYPE_COMMAND:			return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_COMMAND" );
		case TELEPORT_TYPE_GM:				return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_GM" );
		case TELEPORT_TYPE_DUNGEON:			return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_DUNGEON" );
		case TELEPORT_TYPE_DOJO:			return GetDisplayStringManager()->GetString( "DST_TELEPORT_TYPE_DOJO" );
		case TELEPORT_TYPE_SCS:			return GetDisplayStringManager()->GetString("DST_TELEPORT_TYPE_WORLD_MOVE");
		case TELEPORT_TYPE_QUICK_TELEPORT:			return GetDisplayStringManager()->GetString("DST_TELEPORT_TYPE_WORLD_MOVE");
		case TELEPORT_TYPE_PARTY_POPO:			return GetDisplayStringManager()->GetString("DST_TELEPORT_TYPE_WORLD_MOVE");
		case TELEPORT_TYPE_DWC:			return GetDisplayStringManager()->GetString("DST_TELEPORT_TYPE_WORLD_MOVE");
		default:
			break;
	}

	static WCHAR awcBuffer[32];
	swprintf_s( awcBuffer, 32, L"%u", uiTeleportType );
	
	return awcBuffer;
}

const WCHAR * Logic_GetPartyItemLootingMethod(BYTE byLootingMethod)
{
	switch (byLootingMethod)
	{
		case 0: return GetDisplayStringManager()->GetString("DST_PARTYGUI_ITEM_LOOTING_PICKUP_PERSON");
		case 1: return GetDisplayStringManager()->GetString("DST_PARTYGUI_ITEM_LOOTING_IN_ORDER");
		case 2: return GetDisplayStringManager()->GetString("DST_PARTYGUI_ITEM_LOOTING_IN_GRADE");
		case 3: return GetDisplayStringManager()->GetString("DST_PARTYGUI_ITEM_LOOTING_BY_LEADER");
		case 4: return GetDisplayStringManager()->GetString("DST_PARTYGUI_ITEM_LOOTING_BY_ZENNY");
		case 5: return GetDisplayStringManager()->GetString("DST_PARTYGUI_ITEM_LOOTING_BY_DICE");

		default: break;
	}

	static WCHAR awcBuffer[32];
	swprintf_s(awcBuffer, 32, L"%u", byLootingMethod);

	return L"";
}

const WCHAR * Logic_GetPartyZeniLootingMethod(BYTE byLootingMethod)
{
	switch (byLootingMethod)
	{
	case 0: return GetDisplayStringManager()->GetString("DST_PARTYGUI_ZENNY_LOOTING_PICKUP_PERSON");
	case 1: return GetDisplayStringManager()->GetString("DST_PARTYGUI_ZENNY_LOOTING_EQUAL");

	default: break;
	}

	static WCHAR awcBuffer[32];
	swprintf_s(awcBuffer, 32, L"%u", byLootingMethod);

	return L"";
}

const WCHAR * Logic_GetGuildFunctionString(BYTE byFunction)
{
	switch (byFunction)
	{
		case DBO_GUILD_FUNCTION_MAX_MEMBER_30: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_20");
		case DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_1: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_MAX_SECOND_MASTER_1");
		case DBO_GUILD_FUNCTION_NOTICE_BOARD: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_NOTICE");
		case DBO_GUILD_FUNCTION_POST: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_POST");
		case DBO_GUILD_FUNCTION_WAREHOUSE: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_WAREHOUSE");

		case DBO_GUILD_FUNCTION_MAX_MEMBER_50: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_30");
		case DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_2: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_MAX_SECOND_MASTER_2");

			case DBO_GUILD_FUNCTION_MAX_MEMBER_70: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_40");

			case DBO_GUILD_FUNCTION_MAX_MEMBER_90: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_50");
			case DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_3: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_MAX_SECOND_MASTER_3");
			case DBO_GUILD_FUNCTION_CAN_SET_EMBLEM: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_SET_EMBLEM");

			case DBO_GUILD_FUNCTION_MAX_MEMBER_110: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_60");

			case DBO_GUILD_FUNCTION_MAX_MEMBER_130: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_70");
			case DBO_GUILD_FUNCTION_MAX_SECOND_MASTER_4: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_MAX_SECOND_MASTER_4");
			case DBO_GUILD_FUNCTION_CAN_HAVE_DOJO: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_HAVE_DOJO");
			case DBO_GUILD_FUNCTION_CAN_USE_DOJO_TELEPORT: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_USE_DOJO_TELEPORT");

			case DBO_GUILD_FUNCTION_MAX_MEMBER_150: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_80");
	//		case DBO_GUILD_FUNCTION_DOJO_UPGRADE_1: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_DOJO_UPGRADE_1");
			case DBO_GUILD_FUNCTION_CAN_MAKE_DOGI: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_MAKE_DOGI");
			case DBO_GUILD_FUNCTION_CAN_CHANGE_EMBLEM: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_CHANGE_EMBLEM");
			case DBO_GUILD_FUNCTION_DOJO_UPGRADE_2: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_DOJO_UPGRADE_2");
			case DBO_GUILD_FUNCTION_TENKAICHI_SEED: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_TENKAICHI_SEED");

			case DBO_GUILD_FUNCTION_MAX_MEMBER_170: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_90");
			case DBO_GUILD_FUNCTION_DOJO_UPGRADE_3: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_DOJO_UPGRADE_3");
			case DBO_GUILD_FUNCTION_DOJO_QUEST_1: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_DOJO_QUEST_1");
			case DBO_GUILD_FUNCTION_DOJO_MIX_RATE_UP_1: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_DOJO_MIX_RATE_UP_1");

			case DBO_GUILD_FUNCTION_MAX_MEMBER_190: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_MAX_MEMBER_100");
			case DBO_GUILD_FUNCTION_DOJO_UPGRADE_4: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_DOJO_UPGRADE_4");
			case DBO_GUILD_FUNCTION_DOJO_UPGRADE_5: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_DOJO_UPGRADE_5");
			case DBO_GUILD_FUNCTION_DOJO_QUEST_2: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_DOJO_QUEST_2");
			case DBO_GUILD_FUNCTION_DOJO_VEHICLE: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_DOJO_VEHICLE");
			case DBO_GUILD_FUNCTION_DOJO_UPGRADE_6: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_DOJO_UPGRADE_6");
			case DBO_GUILD_FUNCTION_CAN_HAVE_BOOK: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_CAN_HAVE_BOOK");
			case DBO_GUILD_FUNCTION_DOJO_UPGRADE_7: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_DOJO_UPGRADE_7");
			case DBO_GUILD_FUNCTION_DOJO_MIX_RATE_UP_2: return GetDisplayStringManager()->GetString("DST_MULTIDIALOG_GUILDFUNCTION_DOJO_MIX_RATE_UP_2");
	}

	DBO_WARNING_MESSAGE("Not defined guild function : " << byFunction);

	static WCHAR awcBuffer[32];
	swprintf_s(awcBuffer, 32, L"%u", byFunction);

	return L"";
}

const WCHAR * GetActionDisplayStringNum(RwUInt32 actionId)
{
	switch (actionId)
	{
		case ACTION_AVATAR_FORWARD: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_AVATAR_FORWARD");
		case ACTION_AVATAR_BACKWARD: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_AVATAR_BACKWARD");
		case ACTION_AVATAR_LEFTTURN: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_AVATAR_LEFTTURN");
		case ACTION_AVATAR_RIGHTTURN: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_AVATAR_RIGHTTURN");
		case ACTION_AVATAR_SITDOWN: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_AVATAR_SITDOWN");
		case ACTION_AVATAR_JUMP: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_AVATAR_JUMP");
		case ACTION_AVATAR_AUTORUN: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_AVATAR_AUTORUN");
		case ACTION_AVATAR_SCOUTER: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_AVATAR_SCOUTER");
		case ACTION_AVATAR_CHARGE: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_AVATAR_CHARGE");
		case ACTION_AVATAR_LOOTING: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_AVATAR_LOOTING");
		case ACTION_AVATAR_BLOCKING: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_AVATAR_BLOCKING");
		case ACTION_AVATAR_LEFTSIDE: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_AVATAR_LEFTSIDE");
		case ACTION_AVATAR_RIGHTSIDE: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_AVATAR_RIGHTSIDE");
		case ACTION_AVATAR_FREEDOWN: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_AVATAR_FREEDOWN");
		case ACTION_TARGET_SELF: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_SELF");
		case ACTION_TARGET_1STPARTY: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_1STPARTY");
		case ACTION_TARGET_2NDPARTY: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_2NDPARTY");
		case ACTION_TARGET_3RDPARTY: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_3RDPARTY");
		case ACTION_TARGET_4THPARTY: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_4THPARTY");
		case ACTION_TARGET_AUTO: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_AUTO");
		case ACTION_TARGET_AUTOATK: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_AUTOATK");
		case ACTION_TARGET_AUTOFOLLOW: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_AUTOFOLLOW");
		case ACTION_TARGET_MARKING_1: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_MARKING1");
		case ACTION_TARGET_MARKING_2: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_MARKING2");
		case ACTION_TARGET_MARKING_3: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_MARKING3");
		case ACTION_TARGET_MARKING_4: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_MARKING4");
		case ACTION_TARGET_MARKING_5: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_MARKING5");
		case ACTION_TARGET_SELECTING_1: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_SELECTING1");
		case ACTION_TARGET_SELECTING_2: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_SELECTING2");
		case ACTION_TARGET_SELECTING_3: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_SELECTING3");
		case ACTION_TARGET_SELECTING_4: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_SELECTING4");
		case ACTION_TARGET_SELECTING_5: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_SELECTING5");
		case ACTION_TARGET_AUTOTARGET_PARTY: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_TARGET_AUTOPARTY");
		case ACTION_QUICK_1: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK1");
		case ACTION_QUICK_2: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK2");
		case ACTION_QUICK_3: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK3");
		case ACTION_QUICK_4: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK4");
		case ACTION_QUICK_5: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK5");
		case ACTION_QUICK_6: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK6");
		case ACTION_QUICK_7: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK7");
		case ACTION_QUICK_8: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK8");
		case ACTION_QUICK_9: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK9");
		case ACTION_QUICK_0: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK0");
		case ACTION_QUICK_MIN: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICKMIN");
		case ACTION_QUICK_PLUS: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICKPLUS");
		case ACTION_QUICK_PREV: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_PREV");
		case ACTION_QUICK_NEXT: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_NEXT");
		case ACTION_QUICK_2NDCAP: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_2NDCAP");
		case ACTION_QUICK_3RDCAP: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_3RDCAP");
		case ACTION_QUICK_4THCAP: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_4THCAP");
		case ACTION_QUICK_5THCAP: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_5THCAP");
		case ACTION_WINDOW_PROFILE: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_WINDOW_PROFILE");
		case ACTION_WINDOW_SKILL: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_WINDOW_SKILL");
		case ACTION_WINDOW_QUEST: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_WINDOW_QUEST");
		case ACTION_WINDOW_MINIMAP: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_WINDOW_MINIMAP");
		case ACTION_WINDOW_WORLDMAP: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_WINDOW_WORLDMAP");
		case ACTION_WINDOW_MAINCAP: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_WINDOW_MAINCAP");
		case ACTION_WINDOW_PARTY: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_WINDOW_PARTY");
		case ACTION_WINDOW_GUILD: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_WINDOW_GUILD");
		case ACTION_WINDOW_FRIEND: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_WINDOW_FRIEND");
		case ACTION_WINDOW_OPTION: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_WINDOW_OPTION");
		case ACTION_WINDOW_HELP: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_WINDOW_HELP");
		case ACTION_WINDOW_MAIN: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_WINDOW_MAIN");
		case ACTION_WINDOW_RANKBOARD: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_WINDOW_RANKBOARD");
		case ACTION_WINDOW_HOIPOIMIX: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_WINDOW_HOIPOIMIX");
		case ACTION_WINDOW_MASCOT: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_WINDOW_MASCOT");
		case ACTION_WINDOW_DWC: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_WINDOW_DWC");
		case ACTION_QUICK_1_EX: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK1_EX");
		case ACTION_QUICK_2_EX: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK2_EX");
		case ACTION_QUICK_3_EX: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK3_EX");
		case ACTION_QUICK_4_EX: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK4_EX");
		case ACTION_QUICK_5_EX: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK5_EX");
		case ACTION_QUICK_6_EX: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK6_EX");
		case ACTION_QUICK_7_EX: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK7_EX");
		case ACTION_QUICK_8_EX: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK8_EX");
		case ACTION_QUICK_9_EX: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK9_EX");
		case ACTION_QUICK_0_EX: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK0_EX");
		case ACTION_QUICK_MIN_EX: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICKMIN_EX");
		case ACTION_QUICK_PLUS_EX: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICKPLUS_EX");
		case ACTION_QUICK_1_EX2: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK1_EX2");
		case ACTION_QUICK_2_EX2: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK2_EX2");
		case ACTION_QUICK_3_EX2: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK3_EX2");
		case ACTION_QUICK_4_EX2: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK4_EX2");
		case ACTION_QUICK_5_EX2: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK5_EX2");
		case ACTION_QUICK_6_EX2: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK6_EX2");
		case ACTION_QUICK_7_EX2: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK7_EX2");
		case ACTION_QUICK_8_EX2: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK8_EX2");
		case ACTION_QUICK_9_EX2: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK9_EX2");
		case ACTION_QUICK_0_EX2: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICK0_EX2");
		case ACTION_QUICK_MIN_EX2: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICKMIN_EX2");
		case ACTION_QUICK_PLUS_EX2: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_QUICKPLUS_EX2");
		case ACTION_QUICK_SUMMON: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_SUMMON_1");
		case ACTION_QUICK_SUMMON2: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_QUICKSLOT_SUMMON_2");
		case ACTION_MINIMAP_ZOOMOUT: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_MINIMAP_ZOOMOUT");
		case ACTION_MINIMAP_ZOOMIN: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_MINIMAP_ZOOMIN");
		case ACTION_CHAT_PGUP: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_CHAT_PGUP");
		case ACTION_CHAT_PGDN: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_CHAT_PGDN");
		case ACTION_GLOBAL_CANCLE: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_GLOBAL_CANCLE");
		case ACTION_GLOBAL_SNAPSHOT: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_GLOBAL_SNAPSHOT");
		case ACTION_GLOBAL_CHAT: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_GLOBAL_CHAT");
		case ACTION_GLOBAL_RENDERGUI: return GetDisplayStringManager()->GetString("DST_OPTION_CONTROL_ACTION_GLOBAL_RENDERGUI");

		default: break;
	}

	DBO_WARNING_MESSAGE("Not defined action string ID : " << actionId);

	static WCHAR awcBuffer[32];
	swprintf_s(awcBuffer, 32, L"%u", actionId);

	return awcBuffer;
}

const WCHAR * Logic_GetItemRestrictionString(BYTE byRestrictState)
{
	switch (byRestrictState)
	{
		case ITEM_RESTRICT_STATE_TYPE_LIMIT:
		{
			return GetDisplayStringManager()->GetString("DST_REVERTED_GENERAL");
		}
		break;
		case ITEM_RESTRICT_STATE_TYPE_CHARACTER_GET:
		case ITEM_RESTRICT_STATE_TYPE_SEAL:
		case ITEM_RESTRICT_STATE_TYPE_INSERT_BEAD:
		{
			return GetDisplayStringManager()->GetString("DST_REVERTED_GENERAL");
		}
		break;
		case ITEM_RESTRICT_STATE_TYPE_ACCOUNT_GET:
		{
			return GetDisplayStringManager()->GetString("DST_REVERTED_ACCOUNT");
		}
		break;
		case ITEM_RESTRICT_STATE_TYPE_EQUIP:
		{
			return GetDisplayStringManager()->GetString("DST_REVERTED_EQUIP");
		}
		break;
		case ITEM_RESTRICT_STATE_TYPE_USE:
		{
			return GetDisplayStringManager()->GetString("DST_REVERTED_USE");
		}
		break;
		case ITEM_RESTRICT_STATE_TYPE_DRAGONBALL:
		{
			return GetDisplayStringManager()->GetString("DST_REVERTED_DRAGONBALL");
		}
		break;
		case ITEM_RESTRICT_STATE_TYPE_BATTLE_DRAGONBALL:
		{
			return GetDisplayStringManager()->GetString("DST_REVERTED_BATTLE_DRAGONBALL");
		}
		break;
		case ITEM_RESTRICT_STATE_TYPE_QUEST:
		{
			return GetDisplayStringManager()->GetString("DST_REVERTED_QUEST");
		}
		break;

		default: break;
	}

	DBO_WARNING_MESSAGE("Not defined restriction ID : " << (int)byRestrictState);

	static WCHAR awcBuffer[32];
	swprintf_s(awcBuffer, 32, L"%u", byRestrictState);

	return awcBuffer;

}

const WCHAR * Logic_GetItemRestrictionTypeString(BYTE byRestrictType)
{
	switch (byRestrictType)
	{
	case ITEM_RESTRICT_TYPE_CHARACTER_GET:
	case ITEM_RESTRICT_TYPE_FOREVER_CHARACTER_GET:
	{
		return GetDisplayStringManager()->GetString("DST_REVERTED_GENERAL");
	}
	break;
	case ITEM_RESTRICT_TYPE_ACCOUNT_GET:
	{
		return GetDisplayStringManager()->GetString("DST_REVERTED_ACCOUNT");
	}
	break;
	case ITEM_RESTRICT_TYPE_EQUIP:
	case ITEM_RESTRICT_TYPE_FOREVER_EQUIP:
	{
		return GetDisplayStringManager()->GetString("DST_REVERTED_EQUIP");
	}
	break;
	case ITEM_RESTRICT_TYPE_USE:
	{
		return GetDisplayStringManager()->GetString("DST_REVERTED_USE");
	}
	break;
	case ITEM_RESTRICT_TYPE_DRAGONBALL:
	{
		return GetDisplayStringManager()->GetString("DST_REVERTED_DRAGONBALL");
	}
	break;
	case ITEM_RESTRICT_TYPE_BATTLE_DRAGONBALL:
	{
		return GetDisplayStringManager()->GetString("DST_REVERTED_BATTLE_DRAGONBALL");
	}
	break;
	case ITEM_RESTRICT_TYPE_QUEST:
	{
		return GetDisplayStringManager()->GetString("DST_REVERTED_QUEST");
	}
	break;

	default: break;
	}

	static WCHAR awcBuffer[32];
	swprintf_s(awcBuffer, 32, L"%u", byRestrictType);

	return awcBuffer;

}

const char * Logic_GetItemTypeName(BYTE byItemType)
{
	switch (byItemType)
	{
		case ITEM_TYPE_GLOVE: return "DST_GLOVE";
		case ITEM_TYPE_STAFF: return "DST_STAFF";
		case ITEM_TYPE_GUN: return "DST_GUN";
		case ITEM_TYPE_DUAL_GUN: return "DST_DUALDST_GUN";
		case ITEM_TYPE_CLAW: return "DST_CLAW";
		case ITEM_TYPE_AXE: return "DST_AXE";
		case ITEM_TYPE_SCROLL: return "DST_SCROLL";
		case ITEM_TYPE_GEM: return "DST_GEM";
		case ITEM_TYPE_STICK: return "DST_STICK";
		case ITEM_TYPE_SWORD: return "DST_SWORD";
		case ITEM_TYPE_FAN: return "DST_FAN";
		case ITEM_TYPE_WAND: return "DST_WAND";
		case ITEM_TYPE_BAZOOKA: return "DST_BAZOOKA";
		case ITEM_TYPE_BACK_PACK: return "DST_BACKDST_PACK";
		case ITEM_TYPE_INSTRUMENT: return "DST_INSTRUMENT";
		case ITEM_TYPE_CLUB: return "DST_CLUB";
		case ITEM_TYPE_DRUM: return "DST_DRUM";
		case ITEM_TYPE_MASK: return "DST_MASK";
		
		default: DBO_WARNING_MESSAGE("Not defined item type string : " << (int)byItemType); break;
	}

	return "";
}

const WCHAR* Logic_GetItemWorldRuleRestrictionString(BYTE byWorldRule)
{
	switch (byWorldRule)
	{
		case GAMERULE_NORMAL: return GetDisplayStringManager()->GetString("DST_RESTRICTION_NORMAL"); break;
		case GAMERULE_RANKBATTLE: return GetDisplayStringManager()->GetString("DST_RESTRICTION_RANKBATTLE"); break;
		case GAMERULE_DOJO: return GetDisplayStringManager()->GetString("DST_RESTRICTION_DOJO"); break;
		case GAMERULE_RAID: return GetDisplayStringManager()->GetString("DST_RESTRICTION_RAID"); break;
		case GAMERULE_HUNT: return GetDisplayStringManager()->GetString("DST_RESTRICTION_HUNT"); break;
		case GAMERULE_TIMEQUEST: return GetDisplayStringManager()->GetString("DST_RESTRICTION_TIMEQUEST"); break;
		case GAMERULE_TUTORIAL: return GetDisplayStringManager()->GetString("DST_RESTRICTION_TUTORIAL"); break;
		case GAMERULE_TLQ: return GetDisplayStringManager()->GetString("DST_RESTRICTION_TLQ"); break;
		case GAMERULE_MUDOSA:
		case GAMERULE_MINORMATCH:
		case GAMERULE_MAJORMATCH:
		case GAMERULE_FINALMATCH:
		case GAMERULE_TEINKAICHIBUDOKAI: return GetDisplayStringManager()->GetString("DST_RESTRICTION_TEINKAICHIBUDOKAI"); break;
		case GAMERULE_CCBATTLEDUNGEON: return GetDisplayStringManager()->GetString("DST_RESTRICTION_CCBD"); break;
		case GAMERULE_SKD: return L"DST_RESTRICTION_SKD"; break;

		default:  DBO_WARNING_MESSAGE("Unknown world rule : " << (int)byWorldRule); break;
	}

	static WCHAR awcBuffer[32];
	swprintf_s(awcBuffer, 32, L"%u", byWorldRule);

	return awcBuffer;
}


RwChar* Logic_GetFormatString(const RwChar* pcFormat, ...)
{
	static RwChar acBuffer[1024];

	va_list args;
	va_start(args, pcFormat);

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	vsprintf_s(acBuffer, 1024, pcFormat, args);
#else
	vsprintf(acBuffer, pcFormat, args);
#endif
	va_end( args );

	return acBuffer;
}

WCHAR* Logic_GetFormatWideString(const WCHAR* pwcFormat, ...)
{
	static WCHAR awcBuffer[1024];

	va_list args;
	va_start(args, pwcFormat);

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	vswprintf_s(awcBuffer, 1024, pwcFormat, args);
#else
	vswprintf(awcBuffer, pwcFormat, args);
#endif
	va_end( args );

	return awcBuffer;
}

WCHAR* Logic_NumToWchar(int nNumber)
{
	static WCHAR awcBuffer[32];

	swprintf_s(awcBuffer, 32, L"%u", nNumber);

	return awcBuffer;
}

WCHAR * Logic_NumToWchar(float fNumber)
{
	static WCHAR awcBuffer[32];

	swprintf_s(awcBuffer, 32, L"%.2f", fNumber);

	return awcBuffer;
}

WCHAR * Logic_NumToWchar(DWORD dwNumber)
{
	static WCHAR awcBuffer[32];

	swprintf_s(awcBuffer, 32, L"%u", dwNumber);

	return awcBuffer;
}

WCHAR * Logic_NumToWchar(RwUInt32 dwNumber)
{
	static WCHAR awcBuffer[32];

	swprintf_s(awcBuffer, 32, L"%u", dwNumber);

	return awcBuffer;
}

WCHAR * Logic_FormatZeni(DWORD dwZeni)
{
	if (dwZeni < 1000)
		return Logic_NumToWchar(dwZeni);


	static WCHAR awcZeni[128];
	static WCHAR awcFinal[128];

	memset(awcFinal, 0, sizeof(awcFinal)); // set null otherwise old zeni is mixed with new value

	_itow_s(dwZeni, awcZeni, 128, 10); // convert zeni dword to wchar

	int Length = wcslen(awcZeni); // zeni length
	int CommaOffset = Length % 3; // Get the comma offset
	int n = 0;

	for (int i = 0; i < Length; i++) // Loop each character
	{
		if (i && i % 3 == CommaOffset) // If our Index%3 == CommaOffset and this isn't first character, add a comma
		{
			awcFinal[n++] = L','; // Add the comma
		}

		awcFinal[n++] = awcZeni[i]; // Add the original character
	}

	return awcFinal;
}
