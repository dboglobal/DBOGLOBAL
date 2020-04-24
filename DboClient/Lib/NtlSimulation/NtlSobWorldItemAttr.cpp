#include "precomp_ntlsimulation.h"
#include "NtlSobWorldItemAttr.h"

// shared
#include "ItemTable.h"
#include "NtlObject.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSLApi.h"

DEFINITION_MEMORY_POOL( CNtlSobWorldItemAttr )

CNtlSobWorldItemAttr::~CNtlSobWorldItemAttr()
{
	NTL_DELETE(m_pItemOptionSet);
}

void CNtlSobWorldItemAttr::HandleEvents( RWS::CMsg &msg )
{
	if( msg.Id == g_EventSobCreate )
	{
		SNtlEventSobWorldItemCreate* pSobWorldItemCreate = reinterpret_cast<SNtlEventSobWorldItemCreate*>( msg.pData );
		
		if( pSobWorldItemCreate->eObjType == OBJTYPE_DROPITEM )
		{
			m_eType = DROPTYPE_ITEM;
			
			sITEM_STATE* pItemState = reinterpret_cast<sITEM_STATE*>( pSobWorldItemCreate->pState );
			m_bUnidentified = pItemState->bNeedToIdentify;

			sITEM_BRIEF* pItemBrief = reinterpret_cast<sITEM_BRIEF*>( pSobWorldItemCreate->pBrief );
			CItemTable *pItemTbl = API_GetTableContainer()->GetItemTable();

			m_uiGrade = pItemBrief->byGrade;
			m_uiRank = pItemBrief->byRank;
			m_ucBattleAttribute = pItemBrief->byBattleAttribute;			
			m_pItemTbl = reinterpret_cast<sITEM_TBLDAT*>( pItemTbl->FindData( pItemBrief->tblidx ) );

			m_pItemOptionSet = NTL_NEW sITEM_OPTION_SET;
			m_pItemOptionSet->Init();
			memcpy(m_pItemOptionSet, pSobWorldItemCreate->pItemOptionSet, sizeof(sITEM_OPTION_SET));

			NTL_ASSERT( m_pItemTbl, "CNtlSobWorldItemAttr::HandleEvents is table id invalid(" <<  pItemBrief->tblidx << ")" );
		}
		else if( pSobWorldItemCreate->eObjType == OBJTYPE_DROPMONEY )
		{
			m_eType = DROPTYPE_MONEY;

			sMONEY_BRIEF* pMoneyBrief = reinterpret_cast<sMONEY_BRIEF*>( pSobWorldItemCreate->pBrief );
			m_uiMoneyAmount = pMoneyBrief->dwZenny;
		}
	}
}

RwBool CNtlSobWorldItemAttr::IsDragonBall(VOID)
{
	if( m_eType == DROPTYPE_ITEM )
	{
		return m_pItemTbl->byItem_Type == ITEM_TYPE_DRAGONBALL ? TRUE : FALSE;
	}

	return FALSE;	
}