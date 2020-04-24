#include "precomp_ntlsimulation.h"
#include "NtlGuildWarehouse.h"

// core
#include "NtlDebug.h"

// shared
#include "ItemTable.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobAvatar.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"

CNtlGuildWarehouse::CNtlGuildWarehouse()
{
	Clear();
}

CNtlGuildWarehouse::~CNtlGuildWarehouse()
{

}

RwBool CNtlGuildWarehouse::Create()
{
	NTL_FUNCTION("CNtlGuildWarehouse::Create");

	NTL_RETURN(TRUE);
}

void CNtlGuildWarehouse::Destroy(void)
{
	NTL_FUNCTION("CNtlGuildWarehouse::Destroy");

	Clear();

	NTL_RETURNVOID();
}

void CNtlGuildWarehouse::Clear()
{
	m_hNPC = INVALID_SERIAL_ID;
	memset(aWarehouseSlot, 0, sizeof(aWarehouseSlot));
	m_uiZenny = 0;

	for(RwUInt8 i = 0 ; i < NTL_MAX_GUILD_BANK_COUNT ; ++i)
	{
		for(RwUInt8 k = 0 ; k < NTL_MAX_GUILD_ITEM_SLOT ; ++k)
		{
			DeleteGuildWarehouseItem(i, k);
		}
	}
}

CNtlSobItem* CNtlGuildWarehouse::CreateSobItem(CNtlSobItem* pParentItem, SERIAL_HANDLE hOwner,
											   sGuildWarehouseSlot* pItemData, RwUInt8 byPlace, RwUInt8 byPos)
{
	// peessi ItemAttr : You can not keep a time limit item in the current warehouse, but you can move the item with ownership.
	void* pSobObj = CNtlSLEventGenerator::SobSlotItemCreate(pParentItem, SLCLASS_SLOT_ITEM,
		pItemData->hHandle, pItemData->pITEM_TBLDAT->tblidx,
		pItemData->sOptionSet,
		byPlace, byPos,
		pItemData->byStackcount, pItemData->byRank,
		pItemData->byGrade, pItemData->byCurDur,
		pItemData->bNeedToIdentify, pItemData->bNeedToIdentify,
		pItemData->byBattleAttribute, pItemData->byRestrictState, NULL, eDURATIONTYPE_NORMAL, 0, 0);

	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>(pSobObj); 
	pSobItem->SetOwnerID(hOwner); 

	return pSobItem;
}

void CNtlGuildWarehouse::CreateGuildWarehouseItem(RwUInt8 byIndex, RwUInt8 byPos, SERIAL_HANDLE hSrcItem)
{
	CNtlSobItem* pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hSrcItem) );
	if( !pSobItem )
	{
		DBO_FAIL("Not exist sob item of handle : " << hSrcItem);
		return;
	}
	
	CNtlSobItemAttr* pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
	sITEM_TBLDAT* pITEM_TBLDAT = pSobItemAttr->GetItemTbl();
	if( !pITEM_TBLDAT )
	{
		DBO_FAIL("Not exist item table : " << hSrcItem);
		return;
	}

	if( FALSE == Logic_IsUsableIndex(byIndex, NTL_MAX_GUILD_BANK_COUNT) )
	{
		DBO_FAIL("Invalid index : " << byIndex);
		return;
	}

	if( FALSE == Logic_IsUsableIndex(byPos, NTL_MAX_GUILD_ITEM_SLOT) )
	{
		DBO_FAIL("Invalid index : " << byPos);
		return;
	}

	aWarehouseSlot[byIndex][byPos].hHandle				= hSrcItem;
	aWarehouseSlot[byIndex][byPos].pITEM_TBLDAT			= pITEM_TBLDAT;
	aWarehouseSlot[byIndex][byPos].byServerPlace		= (RwUInt8)(byIndex + CONTAINER_TYPE_GUILD_FIRST);
	aWarehouseSlot[byIndex][byPos].byPos				= byPos;
	aWarehouseSlot[byIndex][byPos].byStackcount			= pSobItemAttr->GetStackNum();
	aWarehouseSlot[byIndex][byPos].byRank				= pSobItemAttr->GetRank();
	aWarehouseSlot[byIndex][byPos].byCurDur				= pSobItemAttr->GetDur();
	aWarehouseSlot[byIndex][byPos].bNeedToIdentify		= pSobItemAttr->IsNeedToIdentify();
	aWarehouseSlot[byIndex][byPos].byGrade				= pSobItemAttr->GetGrade();
	aWarehouseSlot[byIndex][byPos].byBattleAttribute	= pSobItemAttr->GetBattleAttribute();
	aWarehouseSlot[byIndex][byPos].byRestrictState		= pSobItemAttr->GetRestrictState();

	memcpy(&aWarehouseSlot[byIndex][byPos].sOptionSet, pSobItemAttr->GetOptionSet(), sizeof(sITEM_OPTION_SET));
}

void CNtlGuildWarehouse::DeleteGuildWarehouseItem(RwUInt8 byIndex, RwUInt8 byPos)
{
	if( FALSE == Logic_IsUsableIndex(byIndex, NTL_MAX_GUILD_BANK_COUNT) )
	{
		DBO_FAIL("Invalid index : " << byIndex);
		return;
	}

	if( FALSE == Logic_IsUsableIndex(byPos, NTL_MAX_GUILD_ITEM_SLOT) )
	{
		DBO_FAIL("Invalid index : " << byPos);
		return;
	}

	memset(&aWarehouseSlot[byIndex][byPos], 0, sizeof(sGuildWarehouseSlot));

	aWarehouseSlot[byIndex][byPos].hHandle			= INVALID_SERIAL_ID;
	aWarehouseSlot[byIndex][byPos].byServerPlace	= (RwUInt8)(CONTAINER_TYPE_GUILD_FIRST + byIndex);
	aWarehouseSlot[byIndex][byPos].byPos			= byPos;
}

void CNtlGuildWarehouse::SetGuildWarehouseItem(SERIAL_HANDLE hHandle, RwUInt8 byIndex, RwUInt8 byPos, sGuildWarehouseSlot& rItemData)
{
	if( FALSE == Logic_IsUsableIndex(byIndex, NTL_MAX_GUILD_BANK_COUNT) )
	{
		DBO_FAIL("Invalid index : " << byIndex);
		return;
	}

	if( FALSE == Logic_IsUsableIndex(byPos, NTL_MAX_GUILD_ITEM_SLOT) )
	{
		DBO_FAIL("Invalid index : " << byPos);
		return;
	}

	aWarehouseSlot[byIndex][byPos]					= rItemData;
	aWarehouseSlot[byIndex][byPos].hHandle			= hHandle;
	aWarehouseSlot[byIndex][byPos].byServerPlace	= (RwUInt8)(CONTAINER_TYPE_GUILD_FIRST + byIndex);
	aWarehouseSlot[byIndex][byPos].byPos			= byPos;
}

RwUInt32 CNtlGuildWarehouse::GetZenny()
{
	return m_uiZenny;
}

SERIAL_HANDLE  CNtlGuildWarehouse::GetNPCHandle()
{
	return m_hNPC;
}

sGuildWarehouseSlot* CNtlGuildWarehouse::GetItem(RwUInt8 byIndex, RwUInt8 byPos)
{
	if( FALSE == Logic_IsUsableIndex(byIndex, NTL_MAX_GUILD_BANK_COUNT) )
	{
		DBO_FAIL("Invalid index : " << byIndex);
		return NULL;
	}

	if( FALSE == Logic_IsUsableIndex(byPos, NTL_MAX_GUILD_ITEM_SLOT) )
	{
		DBO_FAIL("Invalid index : " << byPos);
		return NULL;
	}

	return &aWarehouseSlot[byIndex][byPos];
}

sGuildWarehouseSlot* CNtlGuildWarehouse::GetItem(SERIAL_HANDLE hItem)
{
	for(RwUInt8 i = 0 ; i < NTL_MAX_GUILD_BANK_COUNT ; ++i)
	{
		for(RwUInt8 k = 0 ; k < NTL_MAX_GUILD_ITEM_SLOT ; ++k )
		{
			if( hItem == aWarehouseSlot[i][k].hHandle )
				return &aWarehouseSlot[i][k];
		}
	}

	return NULL;
}

RwBool CNtlGuildWarehouse::CanUseGuileWarehouse()
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();
	if( !pAvatar )
	{
		DBO_FAIL("Not exist avatar instance");
		return FALSE;
	}

	CNtlGuild* pGuild = pAvatar->GetGuild();

	if( pGuild->IsGuildMaster( pAvatar->GetCharID() ) || pGuild->IsSecondGuildMaster( pAvatar->GetCharID() )  )
	{
		if( pGuild->IsExistFunction(DBO_GUILD_FUNCTION_WAREHOUSE) )
			return TRUE;
	}

	return FALSE;
}

VOID CNtlGuildWarehouse::HandleEvents(RWS::CMsg &pMsg)
{	
	NTL_FUNCTION("CNtlGuildWarehouse::HandleEvents");

	if( pMsg.Id == g_EventGuildWarehouse )
	{
		SNtlGuildWarehouse* pEvent = reinterpret_cast<SNtlGuildWarehouse*>(pMsg.pData);

		switch(pEvent->byMessage)
		{
		case GUILD_WAREHOUSE_EVENT_START:
			{
				m_hNPC = pEvent->uiParam;
				CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_START);
				break;
			}
		case GUILD_WAREHOUSE_EVENT_END:
			{
				CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_END);
				Clear();
				break;
			}
		case GUILD_WAREHOUSE_EVENT_SET_ZENNY:
			{
				m_uiZenny = pEvent->uiParam;
				CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ZENNY, m_uiZenny);
				break;
			}
		case GUILD_WAREHOUSE_EVENT_ADD_ZENNY:
			{
				m_uiZenny += pEvent->uiParam;
				CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ZENNY, m_uiZenny);
				break;
			}
		case GUILD_WAREHOUSE_EVENT_SUB_ZENNY:
			{
				m_uiZenny -= pEvent->uiParam;
				CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ZENNY, m_uiZenny);
				break;
			}
		}
	}
	else if( pMsg.Id == g_EventGuildWarehouseItemInfo )
	{
		SNtlGuildWarehouseItemInfo* pEvent = reinterpret_cast<SNtlGuildWarehouseItemInfo*>(pMsg.pData);		
		sITEM_PROFILE* pITEM_PROFILE = reinterpret_cast<sITEM_PROFILE*>(pEvent->pData);
		RwUInt8 byIndex = (RwUInt8)(pEvent->byPlace - CONTAINER_TYPE_GUILD_FIRST);

		if( FALSE == Logic_IsUsableIndex(byIndex, NTL_MAX_GUILD_BANK_COUNT) )
		{
			DBO_FAIL("Invalid index : " << byIndex);
			NTL_RETURNVOID();
		}		

		for( RwUInt8 i = 0 ; i < pEvent->byItemCount ; ++i )
		{
			sITEM_TBLDAT* pITEM_TBLDAT = Logic_GetItemDataFromTable(pITEM_PROFILE->tblidx);
			if( !pITEM_TBLDAT )
			{
				DBO_FAIL("g_EventGuildWarehouseItemInfo, Not exist item table of index : " << pITEM_PROFILE->tblidx);
				continue;
			}			

			if( FALSE == Logic_IsUsableIndex(pITEM_PROFILE->byPos, NTL_MAX_GUILD_ITEM_SLOT) )
			{
				DBO_FAIL("Invalid index : " << pITEM_PROFILE->byPos);
				continue;
			}

			aWarehouseSlot[byIndex][pITEM_PROFILE->byPos].hHandle			= pITEM_PROFILE->handle;
			aWarehouseSlot[byIndex][pITEM_PROFILE->byPos].pITEM_TBLDAT		= pITEM_TBLDAT;
			aWarehouseSlot[byIndex][pITEM_PROFILE->byPos].byServerPlace		= pITEM_PROFILE->byPlace;
			aWarehouseSlot[byIndex][pITEM_PROFILE->byPos].byPos				= pITEM_PROFILE->byPos;
			aWarehouseSlot[byIndex][pITEM_PROFILE->byPos].byStackcount		= pITEM_PROFILE->byStackcount;
			aWarehouseSlot[byIndex][pITEM_PROFILE->byPos].byRank			= pITEM_PROFILE->byRank;
			aWarehouseSlot[byIndex][pITEM_PROFILE->byPos].byCurDur			= pITEM_PROFILE->byCurDur;
			aWarehouseSlot[byIndex][pITEM_PROFILE->byPos].bNeedToIdentify	= pITEM_PROFILE->bNeedToIdentify;
			aWarehouseSlot[byIndex][pITEM_PROFILE->byPos].byGrade			= pITEM_PROFILE->byGrade;
			aWarehouseSlot[byIndex][pITEM_PROFILE->byPos].byBattleAttribute	= pITEM_PROFILE->byBattleAttribute;


			memcpy(&aWarehouseSlot[byIndex][pITEM_PROFILE->byPos].sOptionSet, &pITEM_PROFILE->sOptionSet, sizeof(sITEM_OPTION_SET));

			++pITEM_PROFILE;
		}
	}
	else if( pMsg.Id == g_EventGuildWarehouseItemMove )
	{
		ItemMoveEventHandler(pMsg);		
	}
	else if( pMsg.Id == g_EventGuildWarehouseItemStackMove )
	{
		ItemStackMoveEventHandler(pMsg);		
	}

	NTL_RETURNVOID();
}

void CNtlGuildWarehouse::CreateEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlGuildWarehouse::::CreateEventHandler");

	NTL_RETURNVOID();
}

void CNtlGuildWarehouse::DeleteEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlGuildWarehouse::::DeleteEventHandler");

	NTL_RETURNVOID();
}

void CNtlGuildWarehouse::ItemMoveEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlGuildWarehouse::::ItemMoveEventHandler");

	SNtlGuildWarehouseItemMove* pEvent = reinterpret_cast<SNtlGuildWarehouseItemMove*>(pMsg.pData);	
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	// 길드 창고에서 아이템을.....
	if( CONTAINER_TYPE_GUILD_FIRST <= pEvent->bySrcPlace && pEvent->bySrcPlace <= CONTAINER_TYPE_GUILD_LAST )
	{
		RwUInt8 bySrcIndex = (RwUInt8)(pEvent->bySrcPlace - CONTAINER_TYPE_GUILD_FIRST);

		if( CONTAINER_TYPE_GUILD_FIRST <= pEvent->byDestPlace && pEvent->byDestPlace <= CONTAINER_TYPE_GUILD_LAST )
		{			
			RwUInt8 byDestIndex = (RwUInt8)(pEvent->byDestPlace - CONTAINER_TYPE_GUILD_FIRST);

			sGuildWarehouseSlot sTemp = aWarehouseSlot[bySrcIndex][pEvent->bySrcPos];
			SetGuildWarehouseItem(pEvent->hDestItem, bySrcIndex, pEvent->bySrcPos, aWarehouseSlot[byDestIndex][pEvent->byDestPos]);
			SetGuildWarehouseItem(pEvent->hSrcItem, byDestIndex, pEvent->byDestPos, sTemp);

			CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, bySrcIndex, pEvent->bySrcPos);
			CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, byDestIndex, pEvent->byDestPos);
		}
		else if( CONTAINER_TYPE_BAGSLOT == pEvent->byDestPlace )
		{
			CNtlInventory* pInventory = pAvatar->GetInventory();
			sGuildWarehouseSlot tempSrcSlotData = aWarehouseSlot[bySrcIndex][pEvent->bySrcPos];

			// src place
			if( pEvent->hDestItem != INVALID_SERIAL_ID )
				CreateGuildWarehouseItem(bySrcIndex, pEvent->bySrcPos, pEvent->hDestItem);				
			else
				DeleteGuildWarehouseItem(bySrcIndex, pEvent->bySrcPos);

			// dest place
			CNtlSLEventGenerator::SobDelete( pEvent->hDestItem );			

			CreateSobItem(NULL, pAvatar->GetSerialID(), &tempSrcSlotData,
				CONTAINER_TYPE_BAGSLOT, pEvent->byDestPos);

			pInventory->SetBagItem(pEvent->byDestPos, pEvent->hSrcItem);

			CNtlSLEventGenerator::SobUpdate(pAvatar->GetSerialID(), EVENT_AIUT_ITEM, pAvatar->GetSerialID());
			CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, bySrcIndex, pEvent->bySrcPos);
		}
		else if( CONTAINER_TYPE_BAG_FIRST <= pEvent->byDestPlace && pEvent->byDestPlace <= CONTAINER_TYPE_BAG_LAST )
		{
			CNtlInventory*	pInventory				= pAvatar->GetInventory();
			SERIAL_HANDLE	hDestParent				= pInventory->GetBagItem(pEvent->byDestPlace - CONTAINER_TYPE_BAG_FIRST);
			CNtlSobItem*	pDestParentBagSobItem	= reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject( hDestParent ) );
			
			if( !pDestParentBagSobItem )
			{
				DBO_FAIL("CONTAINER_TYPE_BAG_FIRST <= pEvent->byDestPlace dest parent item instance is null !!!. dest item handle is " << hDestParent);
				NTL_RETURNVOID();
			}
				
			if( aWarehouseSlot[bySrcIndex][pEvent->bySrcPos].hHandle == INVALID_SERIAL_ID )
			{
				DBO_FAIL("Not exist item of Pos : " << pEvent->bySrcPos << " in Place : " << bySrcIndex);
				NTL_RETURNVOID();
			}

			sGuildWarehouseSlot tempSrcSlotData = aWarehouseSlot[bySrcIndex][pEvent->bySrcPos];

			// src place
			if( pEvent->hDestItem != INVALID_SERIAL_ID )
				CreateGuildWarehouseItem(bySrcIndex, pEvent->bySrcPos, pEvent->hDestItem);				
			else
				DeleteGuildWarehouseItem(bySrcIndex, pEvent->bySrcPos);

			// dest place
			CNtlSLEventGenerator::SobDelete( pEvent->hDestItem );

			CreateSobItem(pDestParentBagSobItem, pAvatar->GetSerialID(), &tempSrcSlotData,
				pEvent->byDestPlace, pEvent->byDestPos);

			CNtlSLEventGenerator::SobUpdate(pAvatar->GetSerialID(), EVENT_AIUT_ITEM, pAvatar->GetSerialID());
			CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, bySrcIndex, pEvent->bySrcPos);
		}
		else if( CONTAINER_TYPE_EQUIP == pEvent->byDestPlace )
		{			
			sGuildWarehouseSlot tempSrcSlotData = aWarehouseSlot[bySrcIndex][pEvent->bySrcPos];

			// src place
			if( pEvent->hDestItem != INVALID_SERIAL_ID )
				CreateGuildWarehouseItem(bySrcIndex, pEvent->bySrcPos, pEvent->hDestItem);
			else
				DeleteGuildWarehouseItem(bySrcIndex, pEvent->bySrcPos);

			// dest place
			CNtlSLEventGenerator::SobDelete( pEvent->hDestItem );

			CNtlInventory* pInventory = pAvatar->GetInventory();			

			CreateSobItem(NULL, pAvatar->GetSerialID(), &tempSrcSlotData,
				CONTAINER_TYPE_EQUIP, pEvent->byDestPos);

			pInventory->SetEquipItem(pEvent->byDestPos, tempSrcSlotData.hHandle);

			CNtlSLEventGenerator::SobUpdate(pAvatar->GetSerialID(), EVENT_AIUT_ITEM, pAvatar->GetSerialID());
			CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, bySrcIndex, pEvent->bySrcPos);
		}
	}
	// Items from the guild warehouse elsewhere ....
	else if( CONTAINER_TYPE_GUILD_FIRST <= pEvent->byDestPlace && pEvent->byDestPlace <= CONTAINER_TYPE_GUILD_LAST )
	{
		RwUInt8 byDestIndex = (RwUInt8)(pEvent->byDestPlace - CONTAINER_TYPE_GUILD_FIRST);

		// dest place
		sGuildWarehouseSlot tempDestSlotData = aWarehouseSlot[byDestIndex][pEvent->byDestPos];
		CreateGuildWarehouseItem(byDestIndex, pEvent->byDestPos, pEvent->hSrcItem);		
		CNtlSLEventGenerator::SobDelete( pEvent->hSrcItem );

		if( pEvent->bySrcPlace == CONTAINER_TYPE_BAGSLOT )
		{
			CNtlInventory* pInventory = pAvatar->GetInventory();			

			if( pEvent->hDestItem != INVALID_SERIAL_ID )
			{
				CNtlSobItem* pDestSobItem = CreateSobItem(NULL, pAvatar->GetSerialID(), &tempDestSlotData, pEvent->bySrcPlace, pEvent->bySrcPos);
				if( !pDestSobItem )
				{
					DBO_FAIL("pEvent->bySrcPlace == CONTAINER_TYPE_BAGSLOT, Can not create sob item of handle : " << aWarehouseSlot[byDestIndex][pEvent->byDestPos].hHandle);
					NTL_RETURNVOID();
				}				
			}

			pInventory->SetBagItem(pEvent->bySrcPos, pEvent->hDestItem);
		}
		else if( CONTAINER_TYPE_BAG_FIRST <= pEvent->bySrcPlace && pEvent->bySrcPlace <= CONTAINER_TYPE_BAG_LAST )
		{
			CNtlInventory* pInventory = pAvatar->GetInventory();
			RwInt8 byBagIndex = (RwInt8)(pEvent->bySrcPlace - CONTAINER_TYPE_BAG_FIRST);
			CNtlSobItem* pParentItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pInventory->GetBagItem(byBagIndex)) );

			if( !pParentItem )
			{
				DBO_FAIL("Not exist sob item of handle : " << pInventory->GetBagItem(byBagIndex));
				NTL_RETURNVOID();
			}			

			pParentItem->SetChildSerial( pEvent->bySrcPos, pEvent->hDestItem );

			if( pEvent->hDestItem != INVALID_SERIAL_ID )
			{
				CNtlSobItem* pDestSobItem = CreateSobItem(pParentItem, pAvatar->GetSerialID(), &tempDestSlotData, pEvent->bySrcPlace, pEvent->bySrcPos);
				if( !pDestSobItem )
				{
					DBO_FAIL("CONTAINER_TYPE_BAG_FIRST <= pEvent->bySrcPlac, Can not create sob item of handle : " << aWarehouseSlot[byDestIndex][pEvent->byDestPos].hHandle);
					NTL_RETURNVOID();
				}				
			}
		}
		else if( CONTAINER_TYPE_EQUIP == pEvent->bySrcPlace )
		{
			CNtlInventory* pInventory = pAvatar->GetInventory();			

			if( pEvent->hDestItem != INVALID_SERIAL_ID )
			{
				CNtlSobItem* pDestSobItem = CreateSobItem(NULL, pAvatar->GetSerialID(), &tempDestSlotData, pEvent->bySrcPlace, pEvent->bySrcPos);
				if( !pDestSobItem )
				{
					DBO_FAIL("CONTAINER_TYPE_EQUIP == pEvent->bySrcPlace, Can not create sob item of handle : " << aWarehouseSlot[byDestIndex][pEvent->byDestPos].hHandle);
					NTL_RETURNVOID();
				}
			}

			pInventory->SetEquipItem(pEvent->bySrcPos, pEvent->hDestItem);
		}	

		CNtlSLEventGenerator::SobUpdate(pAvatar->GetSerialID(), EVENT_AIUT_ITEM, pAvatar->GetSerialID());
		CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, byDestIndex, pEvent->byDestPos);
	}

	NTL_RETURNVOID();
}

void CNtlGuildWarehouse::ItemStackMoveEventHandler(RWS::CMsg &msg)
{
	NTL_FUNCTION("CNtlGuildWarehouse::::ItemStackMoveEventHandler");

	RwBool bDivideTwo = FALSE;
	SNtlGuildWarehouseItemStackMove* pEvent = reinterpret_cast<SNtlGuildWarehouseItemStackMove*>(msg.pData);
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	if( CONTAINER_TYPE_BAG_FIRST <= pEvent->byDestPlace && pEvent->byDestPlace <= CONTAINER_TYPE_BAG_LAST )
	{
		CNtlInventory*	pInventory	= pAvatar->GetInventory();
		RwUInt8			byBagIndex	= (RwUInt8)(pEvent->byDestPlace - CONTAINER_TYPE_BAG_FIRST);
		CNtlSobItem*	pDestBag	= reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pInventory->GetBagItem(byBagIndex) ) );

		if( !pDestBag )
		{
			DBO_FAIL("Not exist bag of index : " << byBagIndex);
			NTL_RETURNVOID();
		}		

		if( pDestBag->GetChildSerial( pEvent->byDestPos ) == INVALID_SERIAL_ID )
			bDivideTwo = TRUE;
	}
	else if( CONTAINER_TYPE_GUILD_FIRST <= pEvent->byDestPlace && pEvent->byDestPlace <= CONTAINER_TYPE_GUILD_LAST )
	{
		if( aWarehouseSlot[pEvent->byDestPlace - CONTAINER_TYPE_GUILD_FIRST][pEvent->byDestPos].hHandle == INVALID_SERIAL_ID )
			bDivideTwo = TRUE;
	}

	// When divided in two
	if( bDivideTwo )
	{
		// Divides an item in a guild warehouse by two
		if( CONTAINER_TYPE_GUILD_FIRST <= pEvent->bySrcPlace && pEvent->bySrcPlace <= CONTAINER_TYPE_GUILD_LAST )
		{
			// src place
			RwUInt8 bySrcIndex = (RwUInt8)(pEvent->bySrcPlace - CONTAINER_TYPE_GUILD_FIRST);
			aWarehouseSlot[bySrcIndex][pEvent->bySrcPos].byStackcount = pEvent->bySrcStackCount;
			CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, bySrcIndex, pEvent->bySrcPos);

			// dest place
			if( CONTAINER_TYPE_GUILD_FIRST <= pEvent->byDestPlace && pEvent->byDestPlace <= CONTAINER_TYPE_GUILD_LAST )
			{
				RwUInt8 byDestIndex = (RwUInt8)(pEvent->byDestPlace - CONTAINER_TYPE_GUILD_FIRST);

				SetGuildWarehouseItem(pEvent->hDestItem, byDestIndex, pEvent->byDestPos, aWarehouseSlot[bySrcIndex][pEvent->bySrcPos]);
				aWarehouseSlot[byDestIndex][pEvent->byDestPos].byStackcount = pEvent->byDestStackCount;

				CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, byDestIndex, pEvent->byDestPos);
			}
			else
			{
				sGuildWarehouseSlot& rDestItem = aWarehouseSlot[bySrcIndex][pEvent->bySrcPos];

				// Create Item
				// peessiItemAttr : You can't keep timed items in the current warehouse, but you can move your owned items.
				CNtlSLEventGenerator::SobItemAdd( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(),
					pEvent->hDestItem, rDestItem.pITEM_TBLDAT->tblidx, &rDestItem.sOptionSet,
					pEvent->byDestPlace, pEvent->byDestPos, pEvent->byDestStackCount,
					rDestItem.byRank, rDestItem.byGrade, rDestItem.byCurDur, rDestItem.pITEM_TBLDAT->byDurability,
					rDestItem.bNeedToIdentify, rDestItem.byBattleAttribute, rDestItem.byRestrictState, NULL, eDURATIONTYPE_NORMAL, 0, 0 );
					
			}			
		}
		// Divide the Sob object into two
		else if( CONTAINER_TYPE_GUILD_FIRST <= pEvent->byDestPlace && pEvent->byDestPlace  <= CONTAINER_TYPE_GUILD_LAST )
		{
			CNtlSobItem* pSrcSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(pEvent->hSrcItem) );
			if( !pSrcSobItem )
			{
				DBO_FAIL("CONTAINER_TYPE_GUILD_FIRST <= pEvent->byDestPlace, Not exist sob item of handle : " << pEvent->hSrcItem);
				NTL_RETURNVOID();
			}
			
			CNtlSobItemAttr* pSrcSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSrcSobItem->GetSobAttr() );
			pSrcSobItemAttr->SetStackNum(pEvent->bySrcStackCount);

			RwUInt8 byDestIndex = (RwUInt8)(pEvent->byDestPlace - CONTAINER_TYPE_GUILD_FIRST);
			CreateGuildWarehouseItem(byDestIndex, pEvent->byDestPos, pEvent->hSrcItem);
			aWarehouseSlot[byDestIndex][pEvent->byDestPos].byStackcount = pEvent->byDestStackCount;
			aWarehouseSlot[byDestIndex][pEvent->byDestPos].hHandle = pEvent->hDestItem;

			CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, byDestIndex, pEvent->byDestPos);
			CNtlSLEventGenerator::SobUpdate(pAvatar->GetSerialID(), EVENT_AIUT_ITEM, pAvatar->GetSerialID());
		}

	}
	// 하나로 합쳐질 때
	else if( pEvent->bySrcStackCount == 0 )
	{
		if( CONTAINER_TYPE_GUILD_FIRST <= pEvent->bySrcPlace && pEvent->bySrcPlace <= CONTAINER_TYPE_GUILD_LAST &&
			CONTAINER_TYPE_GUILD_FIRST <= pEvent->byDestPlace && pEvent->byDestPlace  <= CONTAINER_TYPE_GUILD_LAST )
		{
			RwUInt8 bySrcIndex	= (RwUInt8)(pEvent->bySrcPlace - CONTAINER_TYPE_GUILD_FIRST);
			RwUInt8 byDestIndex	= (RwUInt8)(pEvent->byDestPlace - CONTAINER_TYPE_GUILD_FIRST);

			// src place
			DeleteGuildWarehouseItem(bySrcIndex, pEvent->bySrcPos);

			// dest place
			aWarehouseSlot[byDestIndex][pEvent->byDestPos].byStackcount = pEvent->byDestStackCount;

			CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, bySrcIndex, pEvent->bySrcPos);
			CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, byDestIndex, pEvent->byDestPos);
		}
		else if( CONTAINER_TYPE_GUILD_FIRST <= pEvent->bySrcPlace && pEvent->bySrcPlace <= CONTAINER_TYPE_GUILD_LAST )
		{
			CNtlSobItem* pDestSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(pEvent->hDestItem) );
			if( !pDestSobItem )
			{
				DBO_FAIL("CONTAINER_TYPE_GUILD_FIRST <= pEvent->bySrcPlace, Not exist sob item of handle : " << pEvent->hDestItem);
				NTL_RETURNVOID();
			}
			
			CNtlSobItemAttr* pDestSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pDestSobItem->GetSobAttr() );
			RwUInt8 bySrcIndex = (RwUInt8)(pEvent->bySrcPlace - CONTAINER_TYPE_GUILD_FIRST);

			// src place
			DeleteGuildWarehouseItem(bySrcIndex, pEvent->bySrcPos);

			// dest place
			pDestSobItemAttr->SetStackNum(pEvent->byDestStackCount);

			CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, bySrcIndex, pEvent->bySrcPos);
			CNtlSLEventGenerator::SobUpdate(pAvatar->GetSerialID(), EVENT_AIUT_ITEM, pAvatar->GetSerialID());
		}
		else if( CONTAINER_TYPE_GUILD_FIRST <= pEvent->byDestPlace && pEvent->byDestPlace  <= CONTAINER_TYPE_GUILD_LAST )
		{
			RwUInt8 byDestIndex = (RwUInt8)(pEvent->byDestPlace - CONTAINER_TYPE_GUILD_FIRST);

			// src place
			CNtlSLEventGenerator::SobItemDelete( pAvatar->GetSerialID(), pEvent->hSrcItem,
				pEvent->bySrcPlace, pEvent->bySrcPos );

			// dest place
			aWarehouseSlot[byDestIndex][pEvent->byDestPos].byStackcount = pEvent->byDestStackCount;

			CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, byDestIndex, pEvent->byDestPos);
			CNtlSLEventGenerator::SobUpdate(pAvatar->GetSerialID(), EVENT_AIUT_ITEM, pAvatar->GetSerialID());
		}
	}

	// 둘 다 존재할 때
	else
	{
		if( CONTAINER_TYPE_GUILD_FIRST <= pEvent->bySrcPlace && pEvent->bySrcPlace <= CONTAINER_TYPE_GUILD_LAST &&
			CONTAINER_TYPE_GUILD_FIRST <= pEvent->byDestPlace && pEvent->byDestPlace  <= CONTAINER_TYPE_GUILD_LAST )
		{
			RwUInt8 bySrcIndex	= (RwUInt8)(pEvent->bySrcPlace - CONTAINER_TYPE_GUILD_FIRST);
			RwUInt8 byDestIndex	= (RwUInt8)(pEvent->byDestPlace - CONTAINER_TYPE_GUILD_FIRST);

			// src place
			aWarehouseSlot[bySrcIndex][pEvent->bySrcPos].byStackcount = pEvent->bySrcStackCount;

			// dest place
			aWarehouseSlot[byDestIndex][pEvent->byDestPos].byStackcount = pEvent->byDestStackCount;

			CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, bySrcIndex, pEvent->bySrcPos);
			CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, byDestIndex, pEvent->byDestPos);
		}
		else if( CONTAINER_TYPE_GUILD_FIRST <= pEvent->bySrcPlace && pEvent->bySrcPlace <= CONTAINER_TYPE_GUILD_LAST )
		{
			CNtlSobItem* pDestSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(pEvent->hDestItem) );
			if( !pDestSobItem )
			{
				DBO_FAIL("CONTAINER_TYPE_GUILD_FIRST <= pEvent->bySrcPlace, Not exist sob item of handle : " << pEvent->hDestItem);
				NTL_RETURNVOID();
			}
			
			CNtlSobItemAttr* pDestSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pDestSobItem->GetSobAttr() );
			RwUInt8 bySrcIndex = (RwUInt8)(pEvent->bySrcPlace - CONTAINER_TYPE_GUILD_FIRST);

			// src place
			aWarehouseSlot[bySrcIndex][pEvent->bySrcPos].byStackcount = pEvent->bySrcStackCount;

			// dest place
			pDestSobItemAttr->SetStackNum(pEvent->byDestStackCount);			

			CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, bySrcIndex, pEvent->bySrcPos);
			CNtlSLEventGenerator::SobUpdate(pAvatar->GetSerialID(), EVENT_AIUT_ITEM, pAvatar->GetSerialID());
		}
		else if( CONTAINER_TYPE_GUILD_FIRST <= pEvent->byDestPlace && pEvent->byDestPlace  <= CONTAINER_TYPE_GUILD_LAST )
		{
			CNtlSobItem* pSrcSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(pEvent->hSrcItem) );
			if( !pSrcSobItem )
			{
				DBO_FAIL("CONTAINER_TYPE_GUILD_FIRST <= pEvent->byDestPlace, Not exist sob item of handle : " << pEvent->hSrcItem);
				NTL_RETURNVOID();
			}
			
			CNtlSobItemAttr* pSrcSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSrcSobItem->GetSobAttr() );
			RwUInt8 byDestIndex = (RwUInt8)(pEvent->byDestPlace - CONTAINER_TYPE_GUILD_FIRST);

			// src place
			pSrcSobItemAttr->SetStackNum(pEvent->bySrcStackCount);

			// dest place
			aWarehouseSlot[byDestIndex][pEvent->byDestPos].byStackcount = pEvent->byDestStackCount;

			CNtlSLEventGenerator::GuildWarehouseNotify(GUILD_WAREHOUSE_EVENT_SET_ITEM, byDestIndex, pEvent->byDestPos);
			CNtlSLEventGenerator::SobUpdate(pAvatar->GetSerialID(), EVENT_AIUT_ITEM, pAvatar->GetSerialID());
		}
	}

	NTL_RETURNVOID();
}