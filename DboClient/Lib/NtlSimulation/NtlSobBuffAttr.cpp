#include "precomp_ntlsimulation.h"
#include "NtlSobBuffAttr.h"

// shared
#include "NtlObject.h"
#include "ItemTable.h"
#include "UseItemTable.h"
#include "SystemEffectTable.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// simualtion
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"

DEFINITION_MEMORY_POOL(CNtlSobBuffAttr)

void CNtlSobBuffAttr::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		CNtlEventSobBuffCreate *pBuffCreate = reinterpret_cast<CNtlEventSobBuffCreate*>(pMsg.pData);

		SetBuffIndex(pBuffCreate->byBuffIndex);
		SetBuffType(pBuffCreate->byBuffType);
		SetBuffActive(pBuffCreate->bActive);
		SetBuffParameter(pBuffCreate->aBuffParameter);

		if(pBuffCreate->byBuffType == DBO_OBJECT_SOURCE_SKILL)
		{
			CSkillTable *pSkillTable = API_GetTableContainer()->GetSkillTable();
			SetSkillTbl( reinterpret_cast<sSKILL_TBLDAT*>( pSkillTable->FindData(pBuffCreate->uiTblId) ) );

			for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_SKILL ; ++i )
			{
				sSYSTEM_EFFECT_TBLDAT* pSystemEffectTable = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>( API_GetTableContainer()->GetSystemEffectTable()->FindData( GetSkillTbl()->skill_Effect[i] ) );

				if( !pSystemEffectTable )
					continue;

				if( i == 0 )
					m_bBlessType = Dbo_IsForBlessBuff( pSystemEffectTable->byActive_Effect_Type );

				m_SkillTbl.aSkill_Effect_Value[i] = (double)pBuffCreate->aBuffParameter[i].buffParameter.fParameter;				
			}	
		}
		else if(pBuffCreate->byBuffType == DBO_OBJECT_SOURCE_ITEM)
		{
			CItemTable *pItemTable = API_GetTableContainer()->GetItemTable();
			m_pItemTbl = reinterpret_cast<sITEM_TBLDAT*>( pItemTable->FindData(pBuffCreate->uiTblId) );
			NTL_ASSERT( m_pItemTbl, "CNtlSobBuffAttr::HandleEvents => item table is null (" << pBuffCreate->uiTblId << ")" );

			sUSE_ITEM_TBLDAT* pUseItemTable = reinterpret_cast<sUSE_ITEM_TBLDAT*>( API_GetTableContainer()->GetUseItemTable()->FindData( GetItemTbl()->Use_Item_Tblidx ) );

			for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_ITEM ; ++i )
			{				
				sSYSTEM_EFFECT_TBLDAT* pSystemEffectTable = reinterpret_cast<sSYSTEM_EFFECT_TBLDAT*>(API_GetTableContainer()->GetSystemEffectTable()->FindData( pUseItemTable->aSystem_Effect[i] ) );

				if( !pSystemEffectTable )
					continue;

				m_bBlessType = Dbo_IsForBlessBuff( pSystemEffectTable->byActive_Effect_Type );				
				break;
			}	
		}
	}
}


TBLIDX CNtlSobBuffAttr::GetTblIdx(void)
{
	if(m_byType == DBO_OBJECT_SOURCE_SKILL)
	{
		return m_SkillTbl.tblidx;
	}
	else if(m_byType == DBO_OBJECT_SOURCE_ITEM)
	{
		return m_pItemTbl->tblidx;
	}
	else
	{
		return INVALID_TBLIDX;
	}
}