//***********************************************************************************
//
//	File		:	NtlItem.cpp
//
//	Begin		:	2007-06-28
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#include "StdAfx.h"
#include "NtlItem.h"
#include "NtlBattle.h"
#include "NtlObject.h"
#include "NtlStringHandler.h"
#include "NtlHex.h"

WORD Dbo_GetFinalOffence(WORD wBaseOffence, BYTE byGrade)
{
	if (ITEM_GRADE_LEVEL_0 == byGrade)
	{
		return wBaseOffence;
	}
	else
	{
		WORD wFinal = wBaseOffence;

		for (BYTE i = 1; i <= byGrade; i++)
		{
			float fBonus = 0;

			if (i >= 1 && i <= 5)
			{
				fBonus = (float)floor((float)wBaseOffence * 0.10f);
			}
			else if (i >= 6 && i <= 9)
			{
				fBonus = (float)floor((float)wBaseOffence * 0.15f);
			}
			else if (i >= 10 && i <= 14)
			{
				fBonus = (float)floor((float)wBaseOffence * 0.20f);
			}
			else
			{
				fBonus = (float)floor((float)wBaseOffence * 0.30f);
			}

			if (fBonus < 1.0f)
				fBonus = 1.0f;

			wFinal += (WORD)fBonus;
		}

		return wFinal;
	}
}

WORD Dbo_GetFinalDefence(WORD wBaseDefence, BYTE byGrade)
{
	if (ITEM_GRADE_LEVEL_0 == byGrade)
	{
		return wBaseDefence;
	}
	else
	{
		WORD wFinal = wBaseDefence;

		for (BYTE i = 1; i <= byGrade; i++)
		{
			float fBonus = 0;

			if (i >= 1 && i <= 4)
			{
				fBonus = (float)floor((float)wBaseDefence * 0.05f);
			}
			else if (i >= 5 && i <= 9)
			{
				fBonus = (float)floor((float)wBaseDefence * 0.10f);
			}
			else if (i >= 10 && i <= 13)
			{
				fBonus = (float)floor((float)wBaseDefence * 0.15f);
			}
			else
			{
				fBonus = (float)floor((float)wBaseDefence * 0.20f);
			}

			if (fBonus < 1.0f)
				fBonus = 1.0f;

			wFinal += (WORD)fBonus;
		}

		return wFinal;
	}
}


BYTE Dbo_GetHoipoiStoneCount( const BYTE byStoneType, const BYTE byItemType, BYTE byGrade )
{
	// 일반 호이포이 스톤
	const static BYTE byNeedCountBasicWeapon[NTL_ITEM_MAX_GRADE]	= {2,2,2,4,4,4,6,6,6,8,8,8,10,10,10};
	const static BYTE byNeedCountBasicArmor[NTL_ITEM_MAX_GRADE]		= {1,1,1,2,2,2,3,3,3,4,4,4,5, 5, 5};
	// 퓨어 호이포이 스톤
	const static BYTE byNeedCountPureWeapon[NTL_ITEM_MAX_GRADE]		= {2,2,2,4,4,4,6,6,6,8,8,8,10,10,10};
	const static BYTE byNeedCountPureArmor[NTL_ITEM_MAX_GRADE]		= {1,1,1,2,2,2,3,3,3,4,4,4,5, 5, 5};
	// 블랙 호이포이 스톤
	const static BYTE byNeedCountBlackWeapon[NTL_ITEM_MAX_GRADE]	= {2,2,2,2,2,4,4,4,4,4,6,6,6, 6, 6};
	const static BYTE byNeedCountBlackArmor[NTL_ITEM_MAX_GRADE]		= {1,1,1,1,1,2,2,2,2,2,3,3,3, 3, 3};

	// 블랙 호이포이 스톤에 대한 처리
	if ( ITEM_TYPE_BLACK_STONE == byStoneType )
	{
		--byGrade;
	}

	// 그레이드가 잘못된 경우 인밸리드.
	if ( byGrade < 0 || byGrade >= NTL_ITEM_MAX_GRADE )
		return INVALID_BYTE;

	if ( ITEM_TYPE_WEAPON_FIRST <= byItemType && ITEM_TYPE_WEAPON_LAST >= byItemType )
	{
		if ( ITEM_TYPE_STONE == byStoneType )		return byNeedCountBasicWeapon[byGrade];
		if ( ITEM_TYPE_PURE_STONE == byStoneType )	return byNeedCountPureWeapon[byGrade];
		if ( ITEM_TYPE_BLACK_STONE == byStoneType ) return byNeedCountBlackWeapon[byGrade];
	}
	else if ( ITEM_TYPE_ARMOR_FIRST <= byItemType && ITEM_TYPE_ARMOR_LAST >= byItemType )
	{
		if ( ITEM_TYPE_STONE == byStoneType )		return byNeedCountBasicArmor[byGrade];
		if ( ITEM_TYPE_PURE_STONE == byStoneType )	return byNeedCountPureArmor[byGrade];
		if ( ITEM_TYPE_BLACK_STONE == byStoneType ) return byNeedCountBlackArmor[byGrade];
	}
	
	// 여기까지 왔다면 아이템 타입이 잘못되었기 때문임.
	return INVALID_BYTE;
}

DWORD Dbo_GetRepairPay( DWORD dwCost, BYTE byDur, BYTE byCurDur)
{
	float fRate = (float)byCurDur / (float)byDur;
	if ( fRate >= 1.0f )
		return 0;

	DWORD dwRepairCost = (DWORD)((dwCost - (dwCost *  fRate)) * 0.69999999f);

	if( dwRepairCost == 0 )
	{
		dwRepairCost = 1;
	}

	return dwRepairCost;
}

DWORD Dbo_GetHoipoiMixEXP(bool bIsSuccess, BYTE byMadeLevel, BYTE byNeedMixLevel)
{
	if (bIsSuccess == false)
		return 0;

	/*
		max receive exp = 25
		lose 5 exp per byMadeLevel and byNeedMixLevel difference. Ex: 1 - 1 = 25 exp, 1 - 2 = 20 exp, 1 - 3 = 15 exp
	*/

	BYTE byDif = byMadeLevel - byNeedMixLevel;
	if (byDif >= 5)
		return 0;

	return 25 - (byDif * 5);
}

//-----------------------------------------------------------------------------------
// 아래 함수를 
// bool Dbo_SetItemData( sITEM_DATA* const pItemData_Output, sITEM_DATA* const pItemData_Input )
// 로 안한 이유: 
// sITEM_DATA의 데이타가 변경되거나 추가될 경우, 작업자에 실수를 컴파일 시점에서 잡기 위함이다. 
// 디버깅때 고생말고, 코딩때 고생하자~
// 그래서, 위에처럼 절대 수정하지 말것.
//-----------------------------------------------------------------------------------
bool Dbo_SetItemData( sITEM_DATA* const pItemData
					 , ITEMID itemId
					 , TBLIDX itemNo
					 , CHARACTERID charId
					 , BYTE byPlace
					 , BYTE byPosition
					 , BYTE byStackcount
					 , BYTE byRank
					 , BYTE byCurrentDurability
					 , bool bNeedToIdentify
					 , BYTE byGrade
					 , BYTE byBattleAttribute
					 , BYTE byRestrictType
					 , const WCHAR* const awchMaker
					 , TBLIDX* const aOptionTblidx
					 , BYTE byDurationType
					 , DBOTIME nUseStartTime
					 , DBOTIME nUseEndTime )
{
	if( NULL == pItemData )
	{
		return false;
	}

	pItemData->itemId = itemId;
	pItemData->itemNo = itemNo; 
	pItemData->charId = charId;
	pItemData->byPlace = byPlace;
	pItemData->byPosition = byPosition;
	pItemData->byStackcount = byStackcount;
	pItemData->byRank = byRank;
	pItemData->byCurrentDurability = byCurrentDurability;
	pItemData->bNeedToIdentify = bNeedToIdentify;
	pItemData->byGrade = byGrade;
	pItemData->byBattleAttribute = byBattleAttribute;
	pItemData->byRestrictState = byRestrictType;
	pItemData->byDurationType = byDurationType;
	pItemData->nUseStartTime = nUseStartTime;
	pItemData->nUseEndTime = nUseEndTime;

	if( NULL == awchMaker )
	{
		memset( pItemData->awchMaker, 0x00, sizeof(pItemData->awchMaker) );
	}
	else
	{
		memcpy( pItemData->awchMaker, awchMaker, sizeof( pItemData->awchMaker ) );
		(pItemData->awchMaker)[NTL_MAX_SIZE_CHAR_NAME] = L'\0';
	}

	if( NULL == aOptionTblidx )
	{
		memset( pItemData->sOptionSet.aOptionTblidx, INVALID_TBLIDX, sizeof( pItemData->sOptionSet.aOptionTblidx ) );
		memset( pItemData->sOptionSet.aRandomOption, INVALID_TBLIDX, sizeof( pItemData->sOptionSet.aRandomOption ) );
	}
	else
	{
		memcpy( pItemData->sOptionSet.aOptionTblidx, aOptionTblidx, sizeof( pItemData->sOptionSet.aOptionTblidx ) );
		memcpy( pItemData->sOptionSet.aRandomOption, aOptionTblidx, sizeof( pItemData->sOptionSet.aRandomOption ) );
	}

	return true;
}

//-----------------------------------------------------------------------------------
// 아래 함수를 
// bool Dbo_SetItemData_NeedToIdentify( sITEM_DATA* const pItemData_Output, sITEM_DATA* const pItemData_Input )
// 로 안한 이유: 
// sITEM_DATA의 데이타가 변경되거나 추가될 경우, 작업자에 실수를 컴파일 시점에서 잡기 위함이다. 
// 디버깅때 고생말고, 코딩때 고생하자~
// 그래서, 위에처럼 절대 수정하지 말것.
//-----------------------------------------------------------------------------------
bool Dbo_SetItemData_CheckNeedToIdentify( sITEM_DATA* const pItemData
					 , ITEMID itemId
					 , TBLIDX itemNo
					 , CHARACTERID charId
					 , BYTE byPlace
					 , BYTE byPosition
					 , BYTE byStackcount
					 , BYTE byRank
					 , BYTE byCurrentDurability
					 , bool bNeedToIdentify
					 , BYTE byGrade
					 , BYTE byBattleAttribute
					 , BYTE byRestrictType
					 , WCHAR* const awchMaker
					 , TBLIDX* const aOptionTblidx
					 , BYTE byDurationType
					 , DBOTIME nUseStartTime
					 , DBOTIME nUseEndTime )
{
	if( NULL == pItemData )
	{
		return false;
	}

	pItemData->itemId = itemId;
	pItemData->charId = charId;
	pItemData->byPlace = byPlace;
	pItemData->byPosition = byPosition;
	pItemData->bNeedToIdentify = bNeedToIdentify;

	if ( true == pItemData->bNeedToIdentify )
	{
		//미확인 아이템일 경우
		return true;
	}

	//확인 아이템일 경우
	pItemData->itemNo = itemNo; 
	pItemData->byStackcount = byStackcount;
	pItemData->byRank = byRank;
	pItemData->byCurrentDurability = byCurrentDurability;
	pItemData->byGrade = byGrade;
	pItemData->byBattleAttribute = byBattleAttribute;
	pItemData->byRestrictState = byRestrictType;
	pItemData->byDurationType = byDurationType;
	pItemData->nUseStartTime = nUseStartTime;
	pItemData->nUseEndTime = nUseEndTime;

	if( NULL == awchMaker )
	{
		memset( pItemData->awchMaker, 0x00, sizeof(pItemData->awchMaker) );
	}
	else
	{
		memcpy( pItemData->awchMaker, awchMaker, sizeof( pItemData->awchMaker ) );
		(pItemData->awchMaker)[NTL_MAX_SIZE_CHAR_NAME] = L'\0';
	}

	if( NULL == aOptionTblidx )
	{
		memset( pItemData->sOptionSet.aOptionTblidx, INVALID_TBLIDX, sizeof( pItemData->sOptionSet.aOptionTblidx ) );
		memset( pItemData->sOptionSet.aRandomOption, INVALID_TBLIDX, sizeof( pItemData->sOptionSet.aRandomOption ) );
	}
	else
	{
		memcpy( pItemData->sOptionSet.aOptionTblidx, aOptionTblidx, sizeof( pItemData->sOptionSet.aOptionTblidx ) );
		memcpy( pItemData->sOptionSet.aRandomOption, aOptionTblidx, sizeof( pItemData->sOptionSet.aRandomOption ) );
	}

	return true;
}


//-----------------------------------------------------------------------------------
// 아래 함수를 
// bool Dbo_SetItemProfile( sITEM_PROFILE* const pItemData_Output, sITEM_PROFILE* const pItemData_Input )
// 로 안한 이유: 
// sITEM_PROFILE의 데이타가 변경되거나 추가될 경우, 작업자에 실수를 컴파일 시점에서 잡기 위함이다. 
// 디버깅때 고생말고, 코딩때 고생하자~
// 그래서, 위에처럼 절대 수정하지 말것.
//-----------------------------------------------------------------------------------
bool Dbo_SetItemProfile( sITEM_PROFILE* const pItemProfile
						, HOBJECT handle
						, TBLIDX tblidx
						, BYTE byPlace
						, BYTE byPos
						, BYTE byStackcount
						, BYTE byRank
						, BYTE byCurDur
						, bool bNeedToIdentify
						, BYTE byGrade
						, BYTE byBattleAttribute
						, BYTE byRestrictState
						, WCHAR* const awchMaker
						, TBLIDX* const aOptionTblidx
						, BYTE byDurationType
						, DBOTIME nUseStartTime
						, DBOTIME nUseEndTime )
{

	if( NULL == pItemProfile )
	{
		return false;
	}

	pItemProfile->handle = handle;
	pItemProfile->byPlace = byPlace;
	pItemProfile->byPos = byPos;
	pItemProfile->bNeedToIdentify = bNeedToIdentify;

	if( true == bNeedToIdentify )
	{
		//미확인 아이템일 경우
		return true;
	}

	//확인 아이템일 경우
	pItemProfile->tblidx = tblidx;
	pItemProfile->byStackcount = byStackcount;
	pItemProfile->byRank = byRank;
	pItemProfile->byCurDur = byCurDur;
	pItemProfile->byGrade = byGrade;
	pItemProfile->byBattleAttribute = byBattleAttribute;
	pItemProfile->byRestrictState = byRestrictState;
	pItemProfile->byDurationType = byDurationType; 
	pItemProfile->nUseStartTime = nUseStartTime;
	pItemProfile->nUseEndTime = nUseEndTime;

	if( NULL == awchMaker )
	{
		memset( pItemProfile->awchMaker, 0x00, sizeof(pItemProfile->awchMaker) );
	}
	else
	{
		memcpy( pItemProfile->awchMaker, awchMaker, sizeof( pItemProfile->awchMaker ) );
		(pItemProfile->awchMaker)[NTL_MAX_SIZE_CHAR_NAME] = L'\0';
	}

	if( NULL == aOptionTblidx )
	{
		memset( pItemProfile->sOptionSet.aOptionTblidx, INVALID_TBLIDX, sizeof( pItemProfile->sOptionSet.aOptionTblidx ) );
		memset( pItemProfile->sOptionSet.aRandomOption, INVALID_TBLIDX, sizeof( pItemProfile->sOptionSet.aRandomOption ) );
	}
	else
	{
		memcpy( pItemProfile->sOptionSet.aOptionTblidx, aOptionTblidx, sizeof( pItemProfile->sOptionSet.aOptionTblidx ) );
		memcpy( pItemProfile->sOptionSet.aRandomOption, aOptionTblidx, sizeof( pItemProfile->sOptionSet.aRandomOption ) );
	}

	return true;
}


bool Dbo_CheckClass( const BYTE byClass, const DWORD dwNeedClassBitFlag )
{
	if(dwNeedClassBitFlag == 127) // 127 = for all
		return true;

	bool bIsSecClass = false;

	if(byClass >= PC_CLASS_2_FIRST)
		bIsSecClass = true;

	BYTE byFirstClass = 0;

	//get first class
	if(bIsSecClass == true)
	{
		if(byClass == PC_CLASS_STREET_FIGHTER || byClass == PC_CLASS_SWORD_MASTER)
			byFirstClass = PC_CLASS_HUMAN_FIGHTER;

		else if(byClass == PC_CLASS_CRANE_ROSHI || byClass == PC_CLASS_TURTLE_ROSHI)
			byFirstClass = PC_CLASS_HUMAN_MYSTIC;

		else if (byClass == PC_CLASS_GUN_MANIA || byClass == PC_CLASS_MECH_MANIA)
			byFirstClass = PC_CLASS_HUMAN_ENGINEER;

		else if(byClass == PC_CLASS_DARK_WARRIOR || byClass == PC_CLASS_SHADOW_KNIGHT)
			byFirstClass = PC_CLASS_NAMEK_FIGHTER;

		else if(byClass == PC_CLASS_DENDEN_HEALER || byClass == PC_CLASS_POCO_SUMMONER)
			byFirstClass = PC_CLASS_NAMEK_MYSTIC;

		else if(byClass == PC_CLASS_ULTI_MA || byClass == PC_CLASS_GRAND_MA)
			byFirstClass = PC_CLASS_MIGHTY_MAJIN;
		
		else if(byClass == PC_CLASS_PLAS_MA || byClass == PC_CLASS_KAR_MA)
			byFirstClass = PC_CLASS_WONDER_MAJIN;
	}


	if(dwNeedClassBitFlag & MAKE_BIT_FLAG(byClass))
		return true;

	//check first class if char is second class
	if(bIsSecClass == true)
	{
		if(dwNeedClassBitFlag & MAKE_BIT_FLAG(byFirstClass))
			return true;
	}

	return false;
}

DWORD Dbo_GetAuctionHouseSellFee(DWORD dwCost)
{
	DWORD fee = 1;

	if (dwCost == 0)
		return fee;

	fee = (dwCost / 100);

	if (fee == 0) fee = 1;

	return fee;
}


DWORD Dbo_GetItemRestrictFlagType(BYTE byItemRestrictType, BYTE byItemRestrictState)
{
	const int dwArrRestrictFlag[ITEM_RESTRICT_TYPE_MAX][ITEM_RESTRICT_STATE_TYPE_MAX] =
	{
		{ 0, 123 },
		{ 123, 123 },
		{ 115, 115 },
		{ 0, 123 },
		{ 0, 123 },
		{ 127, 127 },
		{ 510, 510 },
		{ 127, 127 },
		{ 0, 0 },
		{ 0, 0 }
	};

	if (byItemRestrictState <= ITEM_RESTRICT_STATE_TYPE_LIMIT || byItemRestrictState >= ITEM_RESTRICT_STATE_TYPE_SEAL)
	{
		if (byItemRestrictState < ITEM_RESTRICT_STATE_TYPE_SEAL || byItemRestrictState >= ITEM_RESTRICT_STATE_TYPE_INSERT_BEAD)
		{
			if (byItemRestrictState >= ITEM_RESTRICT_STATE_TYPE_INSERT_BEAD)
			{
				byItemRestrictType = byItemRestrictState - ITEM_RESTRICT_STATE_TYPE_INSERT_BEAD;
				byItemRestrictState = 1;
			}
		}
		else
		{
			byItemRestrictType = 0;
			byItemRestrictState = 0;
		}
	}
	else
	{
		byItemRestrictType = byItemRestrictState - 1;
		byItemRestrictState = 1;
	}

	if (byItemRestrictType < 10)
	{
		if (byItemRestrictState < 2)
			return dwArrRestrictFlag[byItemRestrictType][byItemRestrictState];
	}
	

	return INVALID_DWORD;
}

float Dbo_GetRandomOptionSystemEffectRate(BYTE byRvType, WORD wMaxValue, BYTE byValue, float fAttack_Physical_Revision, float fAttack_Energy_Revision, float fDefence_Physical_Revision, float fDefence_Energy_Revision)
{
	float fAdjRate = 1.0f;
	float fRetRate = 0.0f;

	switch (byRvType)
	{
	case 1: fAdjRate = fAttack_Physical_Revision;
	case 2: fAdjRate = fAttack_Energy_Revision;
	case 3: fAdjRate = fDefence_Physical_Revision;
	case 4: fAdjRate = fDefence_Energy_Revision;
	default: printf("Dbo_GetRandomOptionSystemEffectRate: byRvType %u not found \n", byRvType); break;
	}

	if (fAdjRate == INVALID_FLOAT)
		fAdjRate = 1.0f;

	fRetRate = ((float)(byValue * wMaxValue) * fAdjRate) / 100.0f;
	if (fRetRate < 1.0f)
		fRetRate = 1.0f;

	return fRetRate;
}

bool IsBagContainer(BYTE byContainr)
{
	return byContainr == CONTAINER_TYPE_BAGSLOT;
}

bool IsInvenContainer(BYTE byContainr)
{
	return byContainr >= CONTAINER_TYPE_BAG1 && byContainr <= CONTAINER_TYPE_BAG5;
}

bool IsEquipContainer(BYTE byContainr)
{
	return byContainr == CONTAINER_TYPE_EQUIP;
}

bool IsBankContainer(BYTE byContainr)
{
	return byContainr >= CONTAINER_TYPE_BANKSLOT && byContainr <= CONTAINER_TYPE_BANK4;
}

bool IsGuildContainer(BYTE byContainr)
{
	return byContainr >= CONTAINER_TYPE_GUILD1 && byContainr <= CONTAINER_TYPE_GUILD3;
}

BYTE GetBagSlotPositionByContainerType(eCONTAINER_TYPE container)
{
	switch (container)
	{
		case CONTAINER_TYPE_BAG1: return BAGSLOT_POSITION_BAGSLOT_POSITION_0; break;
		case CONTAINER_TYPE_BAG2: return BAGSLOT_POSITION_BAGSLOT_POSITION_1; break;
		case CONTAINER_TYPE_BAG3: return BAGSLOT_POSITION_BAGSLOT_POSITION_2; break;
		case CONTAINER_TYPE_BAG4: return BAGSLOT_POSITION_BAGSLOT_POSITION_3; break;
		case CONTAINER_TYPE_BAG5: return BAGSLOT_POSITION_BAGSLOT_POSITION_4; break;

		default: return BAGSLOT_POSITION_NONE; break;
	}

	//return BAGSLOT_POSITION_NONE; // Unreachable ~Mateo
}

BYTE GetContainerTypeByBagSlotPosition(eBAGSLOT_POSITION eBagSlotPosition)
{
	switch (eBagSlotPosition)
	{
	case BAGSLOT_POSITION_BAGSLOT_POSITION_0: return CONTAINER_TYPE_BAG1; break;
	case BAGSLOT_POSITION_BAGSLOT_POSITION_1: return CONTAINER_TYPE_BAG2; break;
	case BAGSLOT_POSITION_BAGSLOT_POSITION_2: return CONTAINER_TYPE_BAG3; break;
	case BAGSLOT_POSITION_BAGSLOT_POSITION_3: return CONTAINER_TYPE_BAG4; break;
	case BAGSLOT_POSITION_BAGSLOT_POSITION_4: return CONTAINER_TYPE_BAG5; break;
	default: return CONTAINER_TYPE_NONE; break;
	}

	//return CONTAINER_TYPE_NONE; // Unreachable ~Mateo
}

float GetDisassembleHigherRankRate(BYTE byItemRank, BYTE byItemNeedMinLevel, BYTE byHigherRank, BYTE byItemNeedMaxLevel)
{
	return ((float)((float)byItemRank * (float)byItemNeedMinLevel / (float)byHigherRank * (float)byItemNeedMaxLevel) * 100.0f);
}

bool Dbo_NeedToSaveItemCoolTime(BYTE byItemCoolTimeGroup)
{
	bool bResult;

	switch (byItemCoolTimeGroup)
	{
	case ITEM_COOL_TIME_GROUP_DH_LP:
	case ITEM_COOL_TIME_GROUP_DH_EP:
	case ITEM_COOL_TIME_GROUP_HOT_LP:
	case ITEM_COOL_TIME_GROUP_HOT_EP:
	case ITEM_COOL_TIME_GROUP_DH_RESCUE:
	case ITEM_COOL_TIME_GROUP_DH_POISON:
	case ITEM_COOL_TIME_GROUP_DH_STOMACHACHE:
	case ITEM_COOL_TIME_GROUP_TELEPORT:
	case ITEM_COOL_TIME_GROUP_CB_OFFENCE:
	case ITEM_COOL_TIME_GROUP_CB_DEFENCE:
	case ITEM_COOL_TIME_GROUP_HOT_FOOD:
	case ITEM_COOL_TIME_GROUP_HOT_DRINK:
	case ITEM_COOL_TIME_GROUR_MASCOT_FOOT:
		bResult = false;
		break;

	case ITEM_COOL_TIME_GROUP_GIFT_BOX: bResult = true; break;

	case ITEM_COOL_TIME_GROUP_TMQ_TIME_PLUS:
	case ITEM_COOL_TIME_GROUP_HLS_SKILL:
	case ITEM_COOL_TIME_GROUP_EVENT_COIN_PLUS:
		bResult = false;
		break;

	case ITEM_COOL_TIME_GROUP_LINGER_LP:
	case ITEM_COOL_TIME_GROUP_LINGER_EP:
		bResult = true;
		break;
		
		default: bResult = false; break;
	}

	return bResult;
}

BYTE Dbo_GetItemCategoryByItemType(BYTE byItemType)
{
	switch (byItemType)
	{
		case ITEM_TYPE_GLOVE:
		case ITEM_TYPE_STAFF:
		case ITEM_TYPE_GUN:
		case ITEM_TYPE_DUAL_GUN:
			return DBO_ITEM_CATEGORY_MAIN_WEAPON; break;

		case ITEM_TYPE_CLAW:
		case ITEM_TYPE_AXE:
		case ITEM_TYPE_SCROLL:
		case ITEM_TYPE_GEM:
		case ITEM_TYPE_STICK:
		case ITEM_TYPE_SWORD:
		case ITEM_TYPE_FAN:
		case ITEM_TYPE_WAND:
		case ITEM_TYPE_BAZOOKA:
		case ITEM_TYPE_BACK_PACK:
		case ITEM_TYPE_INSTRUMENT:
		case ITEM_TYPE_CLUB:
		case ITEM_TYPE_DRUM:
		case ITEM_TYPE_MASK:
			return DBO_ITEM_CATEGORY_SUB_WEAPON; break;

		case ITEM_TYPE_JACKET: return DBO_ITEM_CATEGORY_JACKET; break;
		case ITEM_TYPE_PANTS: return DBO_ITEM_CATEGORY_PANTS; break;
		case ITEM_TYPE_BOOTS: return DBO_ITEM_CATEGORY_BOOTS; break;
		case ITEM_TYPE_NECKLACE: return DBO_ITEM_CATEGORY_NECKLACE; break;
		case ITEM_TYPE_EARRING: return DBO_ITEM_CATEGORY_EARRING; break;
		case ITEM_TYPE_RING: return DBO_ITEM_CATEGORY_RING; break;

		case ITEM_TYPE_COSTUME_SET: return DBO_ITEM_CATEGORY_DOGI; break;
		case ITEM_TYPE_COSTUME_HAIR_STYLE: return DBO_ITEM_CATEGORY_HAIR; break;
		case ITEM_TYPE_COSTUME_MASK: return DBO_ITEM_CATEGORY_MASK; break;
		case ITEM_TYPE_COSTUME_HAIR_ACCESSORY: return DBO_ITEM_CATEGORY_RICHNESS; break;
		case ITEM_TYPE_COSTUME_BACK_ACCESSORY: return DBO_ITEM_CATEGORY_BACKPACK; break;

		default: return DBO_ITEM_CATEGORY_ETC; break;
	}
}

float Dbo_GetItemWorth(BYTE byItemRank, BYTE byItemLevel)
{
	float fItemWorthLevel = (float)byItemLevel;
	
	if (byItemLevel < 56)
		fItemWorthLevel = (float)byItemLevel + 5.0f;
	else
		fItemWorthLevel = (float)byItemLevel + 10.0f;
	

	switch (byItemRank)
	{
		case ITEM_RANK_NOTHING: return 0.0f; break;
		case ITEM_RANK_NORMAL:
		{
			return (float)pow(pow((fItemWorthLevel * 1.0f) * 100.0f, 1.5f), 0.66666669f);
		}
		break;
		case ITEM_RANK_SUPERIOR:
		{
			return (float)pow(pow((fItemWorthLevel * 1.1f) * 100.0f, 1.5f), 0.66666669f);
		}
		break;
		case ITEM_RANK_EXCELLENT:
		{ 
			return (float)pow(pow((fItemWorthLevel * 2.3f) * 100.0f, 1.5f), 0.66666669f);
		}
		break;
		case ITEM_RANK_RARE:
		{ 
			return (float)pow(pow((fItemWorthLevel * 2.8f) * 100.0f, 1.5f), 0.66666669f);
		}
		break;
		case ITEM_RANK_LEGENDARY:
		{ 
			return (float)pow(pow((fItemWorthLevel * 3.4f) * 100.0f, 1.5f), 0.66666669f);
		}
		break;

		default: printf("Dbo_GetItemWorth: \"byItemRank\" has a wrong value., byItemRank = %u \n", byItemRank); break;
	}

	return 0.0f;
}

BYTE GetRequiredSealItemNum(BYTE byRank, BYTE byGrade)
{
	BYTE byItemNum = byGrade;

	if (byRank > ITEM_RANK_SUPERIOR)
	{
		if (byRank == ITEM_RANK_EXCELLENT)
			byItemNum += 1;
		else if (byRank == ITEM_RANK_RARE)
			byItemNum += 2;
		else if (byRank == ITEM_RANK_LEGENDARY)
			byItemNum += 3;
	}

	return byItemNum;
}

DWORD Dbo_GetChargeItemBattleAttributeChange(BYTE byRank, BYTE byItemLv)
{
	DWORD dwFinalCharge = 15000;
	int nStepCount = (byItemLv - 1) / 10;

	if (nStepCount)
	{
		for (int nStep = 1; nStep < nStepCount; nStep++)
			dwFinalCharge *= 2;
		if (byRank > 3)
			dwFinalCharge *= 2;
	}

	return dwFinalCharge;
}

float Dbo_GetChangeItemBattleAttributeSuccessRate(BYTE byAttributePosNum)
{
	if (byAttributePosNum == INVALID_BYTE)
		return 20.0f;

	return 50.0f;
}

void Dbo_GetChangeItemBattleAttributeEachRate(BYTE byAdditionalAttribute, int *panEachRate)
{
	*panEachRate = 0;

	if (byAdditionalAttribute == BATTLE_ATTRIBUTE_UNKNOWN)
	{
		for (int j = BATTLE_ATTRIBUTE_HONEST; j < BATTLE_ATTRIBUTE_COUNT; j++)
			panEachRate[j] = 20;
	}
	else
	{
		for (int k = BATTLE_ATTRIBUTE_HONEST; k < BATTLE_ATTRIBUTE_COUNT; k++)
		{
			if (byAdditionalAttribute == k)
				panEachRate[k] = 24;
			else
				panEachRate[k] = 19;
		}
	}
}

bool IsSealItem(BYTE byRestrictState)
{
	return byRestrictState == ITEM_RESTRICT_STATE_TYPE_SEAL;
}

bool IsSealing(BYTE byRestrictType, BYTE byRestrictState, BYTE byEquipType)
{
	//printf("byRestrictType %u byRestrictState %u, byEquipType %u\n", byRestrictType, byRestrictState, byEquipType);
	if (byEquipType != EQUIP_TYPE_QUEST && byEquipType != EQUIP_TYPE_UNKNOWN && byEquipType <= EQUIP_TYPE_COSTUME)
	{
		if (byRestrictType == ITEM_RESTRICT_TYPE_EQUIP)
			return byRestrictState == ITEM_RESTRICT_STATE_TYPE_LIMIT || byRestrictState == ITEM_RESTRICT_STATE_TYPE_EQUIP;
	}

	return false;
}


bool CanChangeOption(BYTE byEquip_Type, BYTE byItemRank, BYTE byRestrictState, BYTE byNeed_Min_Level)
{
	if (IsSealItem(byRestrictState))
	{
		return false;
	}
	else if (byItemRank < ITEM_RANK_SUPERIOR || byItemRank > ITEM_RANK_LEGENDARY)
	{
		return false;
	}
	else if (byEquip_Type != EQUIP_TYPE_MAIN_WEAPON && byEquip_Type != EQUIP_TYPE_SUB_WEAPON && byEquip_Type != EQUIP_TYPE_ARMOR && byEquip_Type != EQUIP_TYPE_ACCESSORY)
	{
		return false;
	}
	else
	{
		float fRemainingWorth = Dbo_GetItemWorth(byItemRank, byNeed_Min_Level);

		if (fRemainingWorth == 0.0f)
			return false;
	}

	return true;
}

bool GetInsertBeadByDurationType(BYTE byItem_Type)
{
	return byItem_Type == ITEM_TYPE_BEAD;
}

BYTE GetDefaultRestrictState(BYTE byRestrictType, BYTE byItemType, bool bActivate)
{
	//printf("byItemType %u, byRestrictType %u\n", byItemType, byRestrictType);

	if (bActivate == false)
		ITEM_RESTRICT_STATE_TYPE_NONE;
	
	if (byItemType == ITEM_TYPE_DRAGONBALL)				//bind dragon balls (also blank) to character
		return ITEM_RESTRICT_STATE_TYPE_CHARACTER_GET;

	switch (byRestrictType)
	{
		case ITEM_RESTRICT_TYPE_CHARACTER_GET:
		{
			return ITEM_RESTRICT_STATE_TYPE_CHARACTER_GET;
		}
		break;
		case ITEM_RESTRICT_TYPE_ACCOUNT_GET: return ITEM_RESTRICT_STATE_TYPE_ACCOUNT_GET;
		case ITEM_RESTRICT_TYPE_EQUIP: return ITEM_RESTRICT_STATE_TYPE_NONE;
		case ITEM_RESTRICT_TYPE_USE: return ITEM_RESTRICT_STATE_TYPE_NONE;
		case ITEM_RESTRICT_TYPE_DRAGONBALL: return ITEM_RESTRICT_STATE_TYPE_DRAGONBALL;
		case ITEM_RESTRICT_TYPE_BATTLE_DRAGONBALL: return ITEM_RESTRICT_STATE_TYPE_BATTLE_DRAGONBALL;
		case ITEM_RESTRICT_TYPE_QUEST: return ITEM_RESTRICT_STATE_TYPE_QUEST;
		case ITEM_RESTRICT_TYPE_FOREVER_EQUIP: return ITEM_RESTRICT_STATE_TYPE_EQUIP;
		case ITEM_RESTRICT_TYPE_FOREVER_CHARACTER_GET: return ITEM_RESTRICT_STATE_TYPE_CHARACTER_GET;

		default: return ITEM_RESTRICT_STATE_TYPE_NONE;
	}
}

BYTE GetItemTypeGroup(BYTE byItemType)
{
	BYTE byItemTypeGroup = ITEM_TYPE_GROUP_ETC;

	if (byItemType >= ITEM_TYPE_WEAPON_FIRST && byItemType <= ITEM_TYPE_WEAPON_LAST)
		byItemTypeGroup = ITEM_TYPE_GROUP_WEAPON;
	else if (byItemType >= ITEM_TYPE_ARMOR_FIRST && byItemType <= ITEM_TYPE_ARMOR_LAST)
		byItemTypeGroup = ITEM_TYPE_GROUP_ARMOR;

	return byItemTypeGroup;
}

bool IsValidStateToUseItem(TBLIDX itemIdx, WORD wNeed_State_Bit_Flag, BYTE byCurStateId, BYTE byCurAspectStateId, QWORD qwCurCharCondition, bool bIsSwimmingState, bool bIsJumpingState, BYTE byAirState)
{
	if (wNeed_State_Bit_Flag == INVALID_WORD)
		return true;

	static bool bInitItemStateCheck = false;
	static QWORD qwInvalidItemStateCondition = 0;

	if (!bInitItemStateCheck)
	{
		//cant use item when char has one of these conditions
		BIT_FLAG_SET(qwInvalidItemStateCondition, MAKE_BIT_FLAG64(CHARCOND_DIRECT_PLAY));
		BIT_FLAG_SET(qwInvalidItemStateCondition, MAKE_BIT_FLAG64(CHARCOND_CONFUSED));
		BIT_FLAG_SET(qwInvalidItemStateCondition, MAKE_BIT_FLAG64(CHARCOND_TERROR));
		BIT_FLAG_SET(qwInvalidItemStateCondition, MAKE_BIT_FLAG64(CHARCOND_FAKE_DEATH));
		BIT_FLAG_SET(qwInvalidItemStateCondition, MAKE_BIT_FLAG64(CHARCOND_RABIES));
		BIT_FLAG_SET(qwInvalidItemStateCondition, MAKE_BIT_FLAG64(CHARCOND_DRUNK));

		bInitItemStateCheck = true;
	}

	switch (wNeed_State_Bit_Flag)
	{
		case 1:  //only use-able when NOT transformed (items like super saiyan pot, vehicle have this)
		{
			if (BIT_FLAG_TEST(qwInvalidItemStateCondition, qwCurCharCondition) == true) //check if forbidden char conditions are set
				return false;

			if (byCurStateId == CHARSTATE_KEEPING_EFFECT)
				return false;

			if (byCurAspectStateId != ASPECTSTATE_INVALID) //check if transformed ( If we dont have this check, then we have issues using vehicle while transformed )
				return false;

			if (byAirState == AIR_STATE_ON)
				return false;

			if (bIsSwimmingState || bIsJumpingState)
				return false;
		}
		break;

		case 3: //only use-able while standing (items like food have this)
		{
			if (byCurStateId != CHARSTATE_STANDING && byCurStateId != CHARSTATE_SITTING)
				return false;

			if (byAirState == AIR_STATE_ON)
				return false;

			if (bIsSwimmingState || bIsJumpingState)
				return false;
		}
		break;

		case 15: //used by items like pet feed, bitter tea(dodge pot) exp bonus, lp/ep recover
		{
			if (itemIdx == 200309) //bitter tea
			{
				if (BIT_FLAG_TEST(qwInvalidItemStateCondition, qwCurCharCondition) == true) //check if forbidden char conditions are set
					return false;

				if (byAirState == AIR_STATE_ON)
					return false;

				if (byCurStateId == CHARSTATE_STUNNED || byCurStateId == CHARSTATE_SLEEPING || byCurStateId == CHARSTATE_PARALYZED)
					return false;
			}
			else if (itemIdx == 200310) // turtle book
			{
				if (BIT_FLAG_TEST(qwInvalidItemStateCondition, qwCurCharCondition) == true) //check if forbidden char conditions are set
					return false;

				if (byAirState == AIR_STATE_ON)
					return false;

				if (byCurStateId == CHARSTATE_STUNNED || byCurStateId == CHARSTATE_SLEEPING || byCurStateId == CHARSTATE_PARALYZED)
					return false;
			}
		}
		break;

		case 31: //used by flying scroll
		{

		}
		break;

		default: printf("IsValidStateToUseItem: %u wNeed_State_Bit_Flag not used \n", wNeed_State_Bit_Flag); break;
	}
	
	return true;
}
