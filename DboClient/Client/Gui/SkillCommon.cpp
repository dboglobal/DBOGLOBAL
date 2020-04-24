#include "precomp_dboclient.h"
#include "SkillCommon.h"

// simulation
#include "InputActionMap.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSobAvatarAttr.h"
#include "NtlSLGlobal.h"
#include "NtlSkillContainer.h"
#include "NtlSobSkill.h"
#include "NtlSobSkillAttr.h"
#include "NtlSobSkillIcon.h"
#include "NtlSobHTBSkill.h"
#include "NtlSobHTBSkillAttr.h"
#include "NtlSobHTBSkillIcon.h"
#include "NtlSobActionSkill.h"
#include "NtlSobActionSkillAttr.h"
#include "NtlSobActionSkillIcon.h"
#include "NtlSobNpc.h"
#include "NtlSobNpcAttr.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"
#include "NtlCameraController.h"
#include "NtlWorldConceptNPCCommu.h"
#include "NtlSLLogic.h"

// Table
#include "SkillTable.h"
#include "HTBSetTable.h"
#include "NPCTable.h"
#include "PCTable.h"
#include "MerchantTable.h"
#include "ActionTable.h"
#include "ChatCommandTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"


/// woosungs_test 20090730
namespace SkillCommonLogic
{

	RwBool IsLearnable( CSkillGuiItem::TYPE eType, sTBLDAT* pItemTblData )
	{
		if( pItemTblData )
		{
			switch( eType )
			{
			case CSkillGuiItem::SKILL:
				if( SRQ_FLAG_CAN_LEARN_BY_SKILLMASTER == ExmineRQBySkill( pItemTblData, FALSE ) ) 
					return TRUE;
				break;
			case CSkillGuiItem::HTB:
				if( SRQ_FLAG_CAN_LEARN_BY_SKILLMASTER == ExmineRQByHTB( pItemTblData ) ) 
					return TRUE;
				break;
			}
		}
		return FALSE;
	}

	RwUInt32 ExmineRQBySkill( sTBLDAT* pItemTblData, RwBool bExceptSelfLearn )	
	{
		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );
		CNtlSkillContainer* pSkillContainer = pAvatar->GetSkillContainer();

		sSKILL_TBLDAT* pItemData = reinterpret_cast<sSKILL_TBLDAT*>( pItemTblData );

		RwUInt32 eResult = SRQ_FLAG_INITIAL;

		if( !( pAvatarAttr->GetRaceFlag() & pItemData->dwPC_Class_Bit_Flag ) )
			return SRQ_FLAG_NOT_LEARN_CLASS;

		if( TRUE == bExceptSelfLearn && pItemData->bSelfTrain )
			return SRQ_FLAG_CAN_LEARN_BY_SELF;

		if( pItemData->byPC_Class_Change != INVALID_BYTE )
		{
			if( Logic_IsFirstClass( pAvatar ) )
			{
				if( !pAvatarAttr->m_bCanChangeClass )
					eResult |= SRQ_FLAG_NOT_LEARN_CHANGECLASSRIGHT;
				// else 밑으로 내려가사 사용가능 검사.
			}
			else 
			{
				if( pAvatarAttr->GetPcTbl()->byClass != pItemData->byPC_Class_Change )
					return SRQ_FLAG_NOT_LEARN_CLASS;
				else
					return SRQ_FLAG_LEARNED;
			}
		}

		RwBool bRequireSkillTest1 = FALSE;
		RwBool bRequireSkillTest2 = FALSE;

		if( pItemData->uiRequire_Skill_Tblidx_Min_1 == INVALID_TBLIDX ||
			pItemData->uiRequire_Skill_Tblidx_Max_1 == INVALID_TBLIDX ) 						
			bRequireSkillTest1 = FALSE;
		else
			bRequireSkillTest1 = TRUE;

		if( pItemData->uiRequire_Skill_Tblidx_Min_2 == INVALID_TBLIDX ||
			pItemData->uiRequire_Skill_Tblidx_Max_2 == INVALID_TBLIDX ) 						
			bRequireSkillTest2 = FALSE;
		else
			bRequireSkillTest2 = TRUE;

		if( pAvatarAttr->GetLevel() < pItemData->byRequire_Train_Level )
		{
			if( bRequireSkillTest1 || bRequireSkillTest2 )
				eResult |= SRQ_FLAG_NOT_LEARN_LEVEL;
			else
				return SRQ_FLAG_NOT_LEARN_LEVEL;
		}		

		RwBool bRequreSkillPresent1 = FALSE; 
		RwBool bRequreSkillPresent2 = FALSE;

		for( RwInt32 i = 0 ; i < NTL_MAX_PC_HAVE_SKILL ; ++i )
		{
			CNtlSobSkill* pSkill = pSkillContainer->GetSkill( i );
			if( !pSkill )
				continue;

			CNtlSobSkillAttr* pSkillAttr = reinterpret_cast<CNtlSobSkillAttr*>( pSkill->GetSobAttr() );
			sSKILL_TBLDAT* pContainerData = pSkillAttr->GetSkillTbl();

			if( pContainerData->tblidx / 10 == pItemData->tblidx / 10 )
			{
				return SRQ_FLAG_LEARNED;
			}
			else 
			{
				if( bRequireSkillTest1 &&
					pItemData->uiRequire_Skill_Tblidx_Min_1 <= pContainerData->tblidx && 
					pItemData->uiRequire_Skill_Tblidx_Max_1 >= pContainerData->tblidx )			
				{
					bRequreSkillPresent1 = TRUE;
				}		

				if( bRequireSkillTest2 &&
					pItemData->uiRequire_Skill_Tblidx_Min_2 <= pContainerData->tblidx && 
					pItemData->uiRequire_Skill_Tblidx_Max_2 >= pContainerData->tblidx )			
				{
					bRequreSkillPresent2 = TRUE;
				}		
			}
		}

		if( ( bRequireSkillTest1 && !bRequreSkillPresent1 ) || ( bRequireSkillTest2 && !bRequreSkillPresent2 ) )
			eResult |= SRQ_FLAG_NOT_LEARN_SKILL;

		if( eResult == SRQ_FLAG_INITIAL )
			return SRQ_FLAG_CAN_LEARN_BY_SKILLMASTER;
		else
			return (eSKILLRQSTATE)eResult;
	}

	RwUInt32 ExmineRQByHTB( sTBLDAT* pItemTblData )		
	{
		//SRQ_FLAG_NOT_LEARN_LEVEL = 0x02,					// 레벨이 낮아 스킬을 배울 수 없다.
		//SRQ_FLAG_CAN_LEARN_BY_SKILLMASTER = 0x04,			// 스킬마스터에게 스킬을 배울 수 있다.
		//SRQ_FLAG_LEARNED	= 0x0100,						// 이미 배운 스킬이다. 	

		CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
		CNtlSobAvatarAttr* pAvatarAttr = reinterpret_cast<CNtlSobAvatarAttr*>( pAvatar->GetSobAttr() );
		CNtlSkillContainer* pSkillContainer = pAvatar->GetSkillContainer();

		sHTB_SET_TBLDAT* pItemData = reinterpret_cast<sHTB_SET_TBLDAT*>( pItemTblData );

		if( !( pAvatarAttr->GetRaceFlag() & pItemData->dwPC_Class_Bit_Flag ) )
			return SRQ_FLAG_NOT_LEARN_CLASS;

		if( pAvatarAttr->GetLevel() < pItemData->byRequire_Train_Level )
			return SRQ_FLAG_NOT_LEARN_LEVEL;
		else 
		{
			if( pSkillContainer->GetHTBSkillFromTableId( pItemData->tblidx ) )
				return SRQ_FLAG_LEARNED;
		}

		return SRQ_FLAG_CAN_LEARN_BY_SKILLMASTER;
	}

}
