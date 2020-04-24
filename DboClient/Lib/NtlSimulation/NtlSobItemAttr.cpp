#include "precomp_ntlsimulation.h"
#include "NtlSobItemAttr.h"

// shared
#include "ItemTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLApi.h"


#define ITEM_TIMELIMIT_NOTIFY_BEGINTIME		3600.0f	//	1시간.
#define ITEM_TIMELIMIT_NOTIFY_INTERVALTIME  600.0f	//	10분.

DEFINITION_MEMORY_POOL(CNtlSobItemAttr)

RwBool CNtlSobItemAttr::IsContainerItem(void)
{
	if(m_pItemTbl)
	{
		if( m_pItemTbl->byItem_Type == ITEM_TYPE_BAG ||
		    m_pItemTbl->byItem_Type == ITEM_TYPE_SCOUTER ||
		    m_pItemTbl->byItem_Type == ITEM_TYPE_WAREHOUSE ||
		    m_pItemTbl->byItem_Type == ITEM_TYPE_NETPYSTORE )			
				return TRUE;

	}

	return FALSE;
}

RwInt32 CNtlSobItemAttr::GetChildSlotSize(void)
{
	NTL_FUNCTION("CNtlSobItemAttr::GetBagSlotSize");

	if(m_pItemTbl)
		NTL_RETURN(m_pItemTbl->byBag_Size);

	NTL_RETURN( 0 );
}

RwBool CNtlSobItemAttr::IsBagItem(void)
{
	if(m_pItemTbl)
	{
		if(m_pItemTbl->byItem_Type == ITEM_TYPE_BAG)
			return TRUE;
	}

	return FALSE;
}

RwBool CNtlSobItemAttr::IsEquipItem(void)
{
	if(m_pItemTbl)
	{
		if(EQUIP_TYPE_FIRST <= m_pItemTbl->byEquip_Type && EQUIP_TYPE_LAST >= m_pItemTbl->byEquip_Type)
			return TRUE;
		else
			return FALSE;
	}

	return FALSE;
}

RwBool CNtlSobItemAttr::IsScouterItem(void)
{
	if(m_pItemTbl)
	{
		if(m_pItemTbl->byItem_Type == ITEM_TYPE_SCOUTER)
			return TRUE;
	}

	return FALSE;
}

RwUInt32 CNtlSobItemAttr::GetTblIdx(void) const
{
	if(m_pItemTbl)
		return m_pItemTbl->tblidx;
	
	return INVALID_TBLIDX;
}

void CNtlSobItemAttr::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobItemCreate *pSobItemCreate = reinterpret_cast<SNtlEventSobItemCreate*>(pMsg.pData);
		CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();

		if( !pSobItemCreate->bNeedToIdentify )
		{
			m_pItemTbl = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData(pSobItemCreate->uiItemTblId) );
			NTL_ASSERT(pItemTbl, "CNtlSobItemAttr::HandleEvents is table id invalid(" <<  pSobItemCreate->uiItemTblId << ")" );
            if(!m_pItemTbl)
                return;

			m_byStackNum = pSobItemCreate->byStackCount;
			m_byRank = pSobItemCreate->byRank;
			m_byGrade = pSobItemCreate->byGrade;
			m_byDur = pSobItemCreate->byDur;
			m_byMaxDur = m_pItemTbl->byDurability;
			memcpy(&m_sOptionSet, &pSobItemCreate->sOptionSet, sizeof(sITEM_OPTION_SET));
			m_ucBattleAttribute = (RwUInt8)pSobItemCreate->byBattleAttribute;
			m_ucRestrictState = pSobItemCreate->byRestrictState;
			m_ucDurationType = pSobItemCreate->byDurationType;
			if( m_ucDurationType )
			{
				m_StartTime = pSobItemCreate->StartTime;
				m_EndTime = pSobItemCreate->EndTime;			
				m_RemainTime = m_EndTime - GetNtlSLGlobal()->GetServerTime();
				if( m_RemainTime < 0 )
				{
					m_RemainTime = 0;
					m_fRemainTimeBelowPeriod = 0.0f;
					m_fNotifyTime = 0.0f;
				}
			}			

			if( pSobItemCreate->wszMaker )
				m_wstrMaker = pSobItemCreate->wszMaker;
			
			NTL_ASSERT(m_byMaxDur, " CNtlSobItemAttr::HandleEvents => max dur value is 0 ( " << m_pItemTbl->tblidx << ")" );
		}
		m_bNeedToIdentify = pSobItemCreate->bNeedToIdentify;
	}
	else if( pMsg.Id == g_EventSobItemUpdate)
	{
		SNtlEventSobItemUpdate *pSobItemUpdate = reinterpret_cast<SNtlEventSobItemUpdate*>(pMsg.pData);
		CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();

		if( !pSobItemUpdate->bNeedToIdentify )
		{
			m_pItemTbl = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData(pSobItemUpdate->uiItemTblId) );
			NTL_ASSERT( m_pItemTbl, "CNtlSobItemAttr::HandleEvents is table id invalid(" <<  pSobItemUpdate->uiItemTblId << ")" );
			m_byStackNum = pSobItemUpdate->byStackCount;
			m_byRank = pSobItemUpdate->byRank;
			m_byGrade = pSobItemUpdate->byGrade;
			m_byDur = pSobItemUpdate->byDur;
			m_byMaxDur = m_pItemTbl->byDurability;
			memcpy(&m_sOptionSet, &pSobItemUpdate->sOptionSet, sizeof(sITEM_OPTION_SET));
			m_ucBattleAttribute = pSobItemUpdate->byBattleAttribute;
			m_ucRestrictState = pSobItemUpdate->byRestrictState;
			m_ucDurationType = pSobItemUpdate->byDurationType;
			m_StartTime = pSobItemUpdate->StartTime;
			m_EndTime = pSobItemUpdate->EndTime;
			if( m_ucDurationType )
			{
				m_StartTime = pSobItemUpdate->StartTime;
				m_EndTime = pSobItemUpdate->EndTime;
				m_RemainTime = m_EndTime - GetNtlSLGlobal()->GetServerTime();
				if( m_RemainTime < 0 )
				{
					m_RemainTime = 0;
					m_fRemainTimeBelowPeriod = 0.0f;
					m_fNotifyTime = 0.0f;
				}
			}

			if( pSobItemUpdate->wszMaker )
				m_wstrMaker = pSobItemUpdate->wszMaker;

			NTL_ASSERT(m_byMaxDur, " CNtlSobItemAttr::HandleEvents => max dur value is 0 ( " << m_pItemTbl->tblidx << ")" );
		}
		m_bNeedToIdentify = pSobItemUpdate->bNeedToIdentify;
	}
}

void CNtlSobItemAttr::Update(RwReal fElapsedTime)
{
	if( m_ucDurationType )
	{
		if( m_RemainTime > 0 )
		{
			m_fRemainTimeBelowPeriod += fElapsedTime;
			if( m_fRemainTimeBelowPeriod > 1.0f )
			{
				RwInt32 nRemainTimeAbovePeriod = (RwInt32)m_fRemainTimeBelowPeriod;
				m_RemainTime -= nRemainTimeAbovePeriod;
				m_fRemainTimeBelowPeriod -= nRemainTimeAbovePeriod;
			}			

			// 사용기간 만료
			if( m_RemainTime < 0 )
			{
				m_RemainTime = 0;
				m_fRemainTimeBelowPeriod = 0.0f;
				m_fNotifyTime = 0.0f;

				// 패킷에서 처리.
				//CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
				//CNtlSLEventGenerator::FormatSysMsg( INVALID_SERIAL_ID, DST_COMMERCIAL_MB_UPDATE_TERM_LIMETED_ITEM_MSG, FALSE, pItemTextTable->GetText( m_pItemTbl->Name ).c_str() );
			}
			// 사용기간 만료 알림
			else if( m_RemainTime < ITEM_TIMELIMIT_NOTIFY_BEGINTIME )
			{
				if( m_fNotifyTime > 0.0f )
				{
					m_fNotifyTime -= fElapsedTime;
				}
				else
				{
					CTextTable* pItemTextTable = API_GetTableContainer()->GetTextAllTable()->GetItemTbl();
					CNtlSLEventGenerator::FormatSysMsg( INVALID_SERIAL_ID, "DST_COMMERCIAL_TERM_LIMITED_ITEM_WARNING_MSG", FALSE, pItemTextTable->GetText( m_pItemTbl->Name ).c_str() );
					
					m_fNotifyTime = ITEM_TIMELIMIT_NOTIFY_INTERVALTIME;					
				}
			}			
		}
	}
}