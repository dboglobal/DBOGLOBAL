#include "precomp_ntlsimulation.h"
#include "NtlInventory.h"

// shared
#include "ItemTable.h"
#include "UseItemTable.h"
#include "NtlItem.h"
#include "QuestItemTable.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGlobal.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobQuestItem.h"
#include "NtlSobQuestItemAttr.h"
#include "NtlSobManager.h"
#include "NtlSobAvatar.h"
#include "NtlSobIcon.h"
#include "NtlSLApi.h"
#include "NtlSLLogic.h"

#include "GUISoundDefine.h"

CNtlInventoryBase::CNtlInventoryBase()
{
	for(RwInt32 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
	{
		m_uiTblId[i] = INVALID_TBLIDX;
	}
}

CNtlInventoryBase::~CNtlInventoryBase()
{
}


void CNtlInventoryBase::SetEquipTableId(RwUInt8 bySlotIdx, RwUInt32 uiTblId)
{
	NTL_ASSERT(bySlotIdx >= 0 && bySlotIdx < NTL_MAX_EQUIP_ITEM_SLOT, "CNtlInventoryBase::SetEquipTableId");

	m_uiTblId[bySlotIdx] = uiTblId;
}

RwUInt32 CNtlInventoryBase::GetEquipTableId(RwUInt8 bySlotIdx)
{
	if(bySlotIdx >= NTL_MAX_EQUIP_ITEM_SLOT)
		return INVALID_TBLIDX;

	return m_uiTblId[bySlotIdx];
}

RwBool CNtlInventoryBase::IsValidEquipTable(RwUInt8 bySlotIdx)
{
	if(bySlotIdx >= NTL_MAX_EQUIP_ITEM_SLOT)
		return FALSE;

	if(m_uiTblId[bySlotIdx] == INVALID_TBLIDX)
		return FALSE;

	return TRUE;
}

void CNtlInventoryBase::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlInventoryBase::HandleEvents");

	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobPlayerCreate *pSobPlayerCreate = reinterpret_cast<SNtlEventSobPlayerCreate*>(pMsg.pData);
		sITEM_BRIEF *pItemBrief = pSobPlayerCreate->pPcBrief->sItemBrief;

		for(RwUInt8 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
		{
			if(pItemBrief[i].tblidx == INVALID_TBLIDX)
				continue;

			SetEquipTableId(i, pItemBrief[i].tblidx);
		}
	}
	
	NTL_RETURNVOID();
}


///////////////////////////////////////////////////////////////////////////////////////////////
// avatar 사용

CNtlInventory::CNtlInventory()
{
	RwInt32 i;

	for(i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
	{
		m_hEquipItem[i] = INVALID_SERIAL_ID;
	}

	for(i = 0; i < NTL_MAX_BAGSLOT_COUNT; ++i)
	{
		m_hBagItem[i] = INVALID_SERIAL_ID;
	}
	
	for(i = 0; i < ITEM_COOL_TIME_GROUP_COUNT; ++i)
	{
		m_stCoolTimeGroup[i].dwFlag = 0x01 << i;
		m_stCoolTimeGroup[i].fCoolTime = 0.0f;
		m_stCoolTimeGroup[i].fCurrentTime = 0.0f;		
	}
}

CNtlInventory::~CNtlInventory()
{
}

RwBool CNtlInventory::Create(void)
{
	return TRUE;
}

void CNtlInventory::Destroy(void)
{
	RwInt32 i = 0;
	
	for( i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
	{
		if(IsValidEquipItem(i))
		{
			CNtlSLEventGenerator::SobDelete(GetEquipItem(i));
			m_hEquipItem[i] = INVALID_SERIAL_ID;
		}
	}

	for( i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
	{
		if( IsValidBagItem( i ) )
		{
			CNtlSLEventGenerator::SobDelete( GetBagItem( i ) );
			m_hBagItem[i] = INVALID_SERIAL_ID;
		}
	}

	for( i = 0; i < ITEM_COOL_TIME_GROUP_COUNT; ++i )
	{
		m_stCoolTimeGroup[i].dwFlag = 0x01 << i;
		m_stCoolTimeGroup[i].fCoolTime = 0.0f;
		m_stCoolTimeGroup[i].fCurrentTime = 0.0f;		
	}	
}

void CNtlInventory::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlInventory::HandleEvents");

	if(pMsg.Id == g_EventSobCreate)
	{
		CreateEventHandler(pMsg);
		CNtlSLEventGenerator::SobCheckItemInOut();
	}
	else if(pMsg.Id == g_EventSobItemAdd)
	{
		AddEventHandler(pMsg);
		CNtlSLEventGenerator::SobCheckItemInOut();
	}
	else if(pMsg.Id == g_EventSobItemDelete)
	{
		DeleteEventHandler(pMsg);
		CNtlSLEventGenerator::SobCheckItemInOut();
	}
	else if(pMsg.Id == g_EventSobItemMove)
	{
		ItemMoveEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobItemStackMove)
	{
		ItemStackMoveEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobItemUpdate)
	{
		ItemUpdateEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobEquipItemDurDown)
	{
		ItemDurDownEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobItemDurUpdate)
	{
		ItemDurUpdateEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobItemUseAction)
	{
		ItemUseActionEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobItemUpgrade)
	{
		ItemUpgradeEventHandler(pMsg);
	}
	else if (pMsg.Id == g_EventSobItemChangeOptionSet)
	{
		ItemChangeOptionSetEventHandler(pMsg);
	}
	else if (pMsg.Id == g_EventSobItemChangeBattleAttribute)
	{
		ItemChangeBattleAttributeEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobEquippedItemUpgradeAll)
	{
		EquippedItemUpgradeAllEventHandler(pMsg);
	}
	else if (pMsg.Id == g_EventSobItemRestrictionUpdate)
	{
		ItemRestrictStateUpdateEventHandler(pMsg);
	}
	else if (pMsg.Id == g_EventItemSocketInsertBead)
	{
		ItemSocketInsertBeadEventHandler(pMsg);
	}
	else if (pMsg.Id == g_EventItemSocketDestroyBead)
	{
		ItemSocketDestroyBeadEventHandler(pMsg);
	}
	
	NTL_RETURNVOID();
}

void CNtlInventory::CoolTimeUpdate(RwReal fElapsed)
{
	for( RwInt32 i = 0 ; i < ITEM_COOL_TIME_GROUP_COUNT ; ++i )
	{
		if( m_stCoolTimeGroup[i].fCoolTime != 0.0f )
		{
			m_stCoolTimeGroup[i].fCurrentTime += fElapsed;

			if( m_stCoolTimeGroup[i].fCurrentTime >= m_stCoolTimeGroup[i].fCoolTime )
			{
				m_stCoolTimeGroup[i].fCoolTime = 0.0f;
				m_stCoolTimeGroup[i].fCurrentTime = 0.0f;
			}
		}
	}
}

CNtlSobItem* CNtlInventory::CreateItem(void *pParentSobItem, RwInt32 iItemPfIdx)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();
	sITEM_PROFILE* pItemProfile = &pAvatarInfo->sItemPf[iItemPfIdx];

	void *pSobObj = CNtlSLEventGenerator::SobSlotItemCreate(pParentSobItem, SLCLASS_SLOT_ITEM, pItemProfile->handle, 
						pItemProfile->tblidx, pItemProfile->sOptionSet,
						pItemProfile->byPlace, pItemProfile->byPos, pItemProfile->byStackcount, pItemProfile->byRank,
						pItemProfile->byGrade, pItemProfile->byCurDur, pItemProfile->bNeedToIdentify, 
						FALSE, pItemProfile->byBattleAttribute, pItemProfile->byRestrictState, pItemProfile->awchMaker,
						pItemProfile->byDurationType, pItemProfile->nUseStartTime, pItemProfile->nUseEndTime);   

	CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>(pSobObj); 
	pSobItem->SetOwnerID(pSobAvatar->GetSerialID()); 

	return pSobItem;
}

CNtlSobItem* CNtlInventory::CreateItem(void *pParentSobItem, SNtlEventSobItemAdd *pSobItemAdd)
{
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
	void *pSobObj = CNtlSLEventGenerator::SobSlotItemCreate(pParentSobItem, SLCLASS_SLOT_ITEM, pSobItemAdd->hItemSerialId, 
															pSobItemAdd->uiItemTblId, pSobItemAdd->sOptionSet,
															pSobItemAdd->byPlace, pSobItemAdd->bySlotIdx, pSobItemAdd->byStackCount, pSobItemAdd->byRank,
															pSobItemAdd->byGrade, pSobItemAdd->byDur, pSobItemAdd->bNeedToIdentify, 
															FALSE, pSobItemAdd->byBattleAttribute, pSobItemAdd->byRestrictState, pSobItemAdd->wszMaker,
															pSobItemAdd->byDurationType, pSobItemAdd->StartTime, pSobItemAdd->EndTime);   

	CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>(pSobObj); 
	pSobItem->SetOwnerID(pSobAvatar->GetSerialID()); 

	return pSobItem;
}

void CNtlInventory::CreateEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlInventory::CreateEventHandler");

	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

	for(int i = 0; i < pAvatarInfo->wItemCount; ++i)
	{
		RwUInt8 byPlace = pAvatarInfo->sItemPf[i].byPlace;
		if(byPlace == CONTAINER_TYPE_BAGSLOT)
		{
			CreateItem(NULL, i);

			NTL_ASSERT( pAvatarInfo->sItemPf[i].byPos >= 0 && pAvatarInfo->sItemPf[i].byPos < NTL_MAX_BAGSLOT_COUNT, 
						"CNtlInventory::HandleEvents : CONTAINER_TYPE_BAGSLOT Bag Slot idx is invalid : " << pAvatarInfo->sItemPf[i].byPos);

			m_hBagItem[pAvatarInfo->sItemPf[i].byPos] = pAvatarInfo->sItemPf[i].handle;
		}

		else if(byPlace == CONTAINER_TYPE_BAG1)
		{
			CreateItem(GetNtlSobManager()->GetSobObject(m_hBagItem[0]), i);
		}

		else if(byPlace == CONTAINER_TYPE_BAG2)
		{
			CreateItem(GetNtlSobManager()->GetSobObject(m_hBagItem[1]), i);
		}

		else if(byPlace == CONTAINER_TYPE_BAG3)
		{
			CreateItem(GetNtlSobManager()->GetSobObject(m_hBagItem[2]), i);
		}

		else if(byPlace == CONTAINER_TYPE_BAG4)
		{
			CreateItem(GetNtlSobManager()->GetSobObject(m_hBagItem[3]), i);
		}

		else if(byPlace == CONTAINER_TYPE_BAG5)
		{
			CreateItem(GetNtlSobManager()->GetSobObject(m_hBagItem[4]), i);
		}

		else if(byPlace == CONTAINER_TYPE_EQUIP)
		{
			CreateItem(NULL, i);

			NTL_ASSERT( pAvatarInfo->sItemPf[i].byPos >= 0 && pAvatarInfo->sItemPf[i].byPos < NTL_MAX_EQUIP_ITEM_SLOT, 
						"CNtlInventory::HandleEvents : CONTAINER_TYPE_EQUIP Slot idx is invalid : " << pAvatarInfo->sItemPf[i].byPos);

			m_hEquipItem[pAvatarInfo->sItemPf[i].byPos] = pAvatarInfo->sItemPf[i].handle;
		}
	}

	NTL_RETURNVOID();
}

void CNtlInventory::AddEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlInventory::AddEventHandler");

	SNtlEventSobItemAdd *pSobItemAdd = reinterpret_cast<SNtlEventSobItemAdd*>( pMsg.pData );

	RwUInt8 byPlace = pSobItemAdd->byPlace;
	if(byPlace == CONTAINER_TYPE_BAGSLOT)
	{
		CreateItem(NULL, pSobItemAdd);

		NTL_ASSERT( pSobItemAdd->bySlotIdx >= 0 && pSobItemAdd->bySlotIdx < NTL_MAX_BAGSLOT_COUNT, 
			"CNtlInventory::HandleEvents : CONTAINER_TYPE_BAGSLOT Bag Slot idx is invalid : " << pSobItemAdd->bySlotIdx);

		m_hBagItem[pSobItemAdd->bySlotIdx] = pSobItemAdd->hItemSerialId;
	}

	else if(byPlace == CONTAINER_TYPE_BAG1)
	{
		CreateItem(GetNtlSobManager()->GetSobObject(m_hBagItem[0]), pSobItemAdd);
	}

	else if(byPlace == CONTAINER_TYPE_BAG2)
	{
		CreateItem(GetNtlSobManager()->GetSobObject(m_hBagItem[1]), pSobItemAdd);
	}

	else if(byPlace == CONTAINER_TYPE_BAG3)
	{
		CreateItem(GetNtlSobManager()->GetSobObject(m_hBagItem[2]), pSobItemAdd);
	}

	else if(byPlace == CONTAINER_TYPE_BAG4)
	{
		CreateItem(GetNtlSobManager()->GetSobObject(m_hBagItem[3]), pSobItemAdd);
	}

	else if(byPlace == CONTAINER_TYPE_BAG5)
	{
		CreateItem(GetNtlSobManager()->GetSobObject(m_hBagItem[4]), pSobItemAdd);
	}

	else if(byPlace == CONTAINER_TYPE_EQUIP)
	{
		CreateItem(NULL, pSobItemAdd);

		NTL_ASSERT( pSobItemAdd->bySlotIdx >= 0 && pSobItemAdd->bySlotIdx < NTL_MAX_EQUIP_ITEM_SLOT, 
			"CNtlInventory::HandleEvents : CONTAINER_TYPE_EQUIP Slot idx is invalid : " << pSobItemAdd->bySlotIdx);

		SetEquipItem(pSobItemAdd->bySlotIdx, pSobItemAdd->hItemSerialId);
	}
	

	NTL_RETURNVOID();
}

void CNtlInventory::DeleteEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlInventory::DeleteEventHandler");

	SNtlEventSobItemDelete *pSobItemDelete = reinterpret_cast<SNtlEventSobItemDelete*>( pMsg.pData );

	if( pSobItemDelete->byPlace >= CONTAINER_TYPE_BAG_FIRST && pSobItemDelete->byPlace <= CONTAINER_TYPE_BAG_LAST )
	{
		CNtlSLEventGenerator::SobDelete( pSobItemDelete->hItemSerialId );
	}
	else if( pSobItemDelete->byPlace == CONTAINER_TYPE_EQUIP )
	{
		CNtlSLEventGenerator::SobDelete( pSobItemDelete->hItemSerialId );
		SetEquipItem( pSobItemDelete->bySlotIdx, INVALID_SERIAL_ID );
	}
	else if( pSobItemDelete->byPlace == CONTAINER_TYPE_BAGSLOT )
	{
		CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pSobItemDelete->hItemSerialId ) );
		NTL_ASSERT( pItem->EmptyChild(), "CNtlInventory::DeleteEventHandler, Bag Must be Empty!!" );
		
		CNtlSLEventGenerator::SobDelete( pSobItemDelete->hItemSerialId );
		SetBagItem( pSobItemDelete->bySlotIdx, INVALID_SERIAL_ID );
	}
}

void CNtlInventory::ItemMoveEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlInventory::ItemMoveEventHandler");

	SNtlEventSobItemMove *pItemMove = reinterpret_cast<SNtlEventSobItemMove*>(pMsg.pData); 

	// source item handle event
	CNtlSobItem *pSrcSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(pItemMove->hSrcSerial) );
	NTL_ASSERT(pSrcSobItem, "CNtlInventory::ItemMoveEventHandler : source item instance is null !!!");
	
	if(pItemMove->bySrcPlace == CONTAINER_TYPE_BAGSLOT)
	{
		if( pItemMove->byDestPlace == CONTAINER_TYPE_BAGSLOT)
		{
			SetBagItem(pItemMove->byDestSlotIdx, pItemMove->hSrcSerial);
			SetBagItem(pItemMove->bySrcSlotIdx, pItemMove->hDestSerial);
		}

		else if(pItemMove->byDestPlace >= CONTAINER_TYPE_BAG_FIRST && pItemMove->byDestPlace <= CONTAINER_TYPE_BAG_LAST)
		{
			RwUInt8 byDestParentSlotIdx = (RwUInt8)(pItemMove->byDestPlace - CONTAINER_TYPE_BAG1);

			CNtlSobItem *pDestParentBagSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( GetBagItem(byDestParentSlotIdx) ) );
			NTL_ASSERT(pDestParentBagSobItem, "CNtlInventory::ItemMoveEventHandler : dest parent item instance is null !!!");

			// source 이동.
			pSrcSobItem->SetParentItemSerial(GetBagItem(byDestParentSlotIdx));
			pDestParentBagSobItem->SetChildSerial(pItemMove->byDestSlotIdx, pItemMove->hSrcSerial);
			// dest 이동.
			SetBagItem(pItemMove->bySrcSlotIdx, pItemMove->hDestSerial);
		}

		else if(pItemMove->byDestPlace == CONTAINER_TYPE_EQUIP)
		{
			NTL_ASSERTFAIL("CNtlInventory::ItemMoveEventHandler : source item type is CONTAINER_TYPE_BAGSLOT");
		}
	}

	else if(pItemMove->bySrcPlace >= CONTAINER_TYPE_BAG_FIRST && pItemMove->bySrcPlace <= CONTAINER_TYPE_BAG_LAST)
	{
		RwUInt8 bySrcParentSlotIdx = (RwUInt8)(pItemMove->bySrcPlace - CONTAINER_TYPE_BAG1);
		CNtlSobItem *pSrcParentBagSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( GetBagItem(bySrcParentSlotIdx) ) );
		NTL_ASSERT(pSrcParentBagSobItem, "CNtlInventory::ItemMoveEventHandler : source parent item instance is null !!!");

		if(pItemMove->byDestPlace == CONTAINER_TYPE_BAGSLOT)
		{
			// source 이동.
			pSrcSobItem->SetParentItemSerial(INVALID_SERIAL_ID);
			pSrcParentBagSobItem->SetChildSerial(pItemMove->bySrcSlotIdx, pItemMove->hDestSerial);
			// dest 이동.
			SetBagItem(pItemMove->byDestSlotIdx, pItemMove->hSrcSerial);
		}

		else if(pItemMove->byDestPlace >= CONTAINER_TYPE_BAG_FIRST && pItemMove->byDestPlace <= CONTAINER_TYPE_BAG_LAST)
		{
			RwUInt8 byDestParentSlotIdx = (RwUInt8)(pItemMove->byDestPlace - CONTAINER_TYPE_BAG1);

			// source 이동.
			pSrcSobItem->SetParentItemSerial(GetBagItem(byDestParentSlotIdx));
			pSrcParentBagSobItem->SetChildSerial(pItemMove->bySrcSlotIdx, pItemMove->hDestSerial);

			// dest 이동
			if(pItemMove->hDestSerial != INVALID_SERIAL_ID)
			{
				CNtlSobItem *pDestSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pItemMove->hDestSerial ) );
				NTL_ASSERT( pDestSobItem, "CNtlInventory::ItemMoveEventHandler : Dest item instance is null !!!" );
				pDestSobItem->SetParentItemSerial(GetBagItem(bySrcParentSlotIdx));
			}

			CNtlSobItem *pDestParentBagSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( GetBagItem(byDestParentSlotIdx) ) );
			NTL_ASSERT(pDestParentBagSobItem, "CNtlInventory::ItemMoveEventHandler : dest parent item instance is null !!!");
			pDestParentBagSobItem->SetChildSerial(pItemMove->byDestSlotIdx, pItemMove->hSrcSerial);
		}

		else if(pItemMove->byDestPlace == CONTAINER_TYPE_EQUIP)
		{
			// source 이동.
			pSrcSobItem->SetParentItemSerial(INVALID_SERIAL_ID);
			pSrcParentBagSobItem->SetChildSerial(pItemMove->bySrcSlotIdx, pItemMove->hDestSerial);

			// dest 이동
			SetEquipItem(pItemMove->byDestSlotIdx, pItemMove->hSrcSerial);
			if( pItemMove->hDestSerial != INVALID_SERIAL_ID )
			{
				CNtlSobItem* pDestItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pItemMove->hDestSerial ) );
				NTL_ASSERT( pDestItem, "CNtlInventory::ItemMoveEventHandler : Dest item instance is null !!!" );
				pDestItem->SetParentItemSerial( pSrcParentBagSobItem->GetSerialID() );
			}

			Logic_PlayGUISound(GSD_SYSTEM_ITEM_EQUIP);
		}
	}

	else if (pItemMove->bySrcPlace == CONTAINER_TYPE_EQUIP)
	{
		if(pItemMove->byDestPlace == CONTAINER_TYPE_BAGSLOT)
		{
			NTL_ASSERTFAIL("CNtlInventory::ItemMoveEventHandler : source item type equip to bag slot");
		}

		else if (pItemMove->byDestPlace >= CONTAINER_TYPE_BAG_FIRST && pItemMove->byDestPlace <= CONTAINER_TYPE_BAG_LAST)
		{
			CNtlSobItem *pSrcSobItemSlot = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( GetEquipItem(pItemMove->bySrcSlotIdx) ) );
			NTL_ASSERT(pSrcSobItemSlot, "CNtlInventory::ItemMoveEventHandler : source item instance(CONTAINER_TYPE_EQUIP) is null !!!");

			RwUInt8 byDestParentSlotIdx = (RwUInt8)(pItemMove->byDestPlace - CONTAINER_TYPE_BAG1);
			CNtlSobItem *pDestsParentBagSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( GetBagItem(byDestParentSlotIdx) ) );
			NTL_ASSERT(pDestsParentBagSobItem, "CNtlInventory::ItemMoveEventHandler : dest bag item instance is null !!!");

			// source 이동.
			pSrcSobItemSlot->SetParentItemSerial(GetBagItem(byDestParentSlotIdx));
			SetEquipItem(pItemMove->bySrcSlotIdx, pItemMove->hDestSerial);

			// dest 이동.
			pDestsParentBagSobItem->SetChildSerial(pItemMove->byDestSlotIdx, pItemMove->hSrcSerial);

			Logic_PlayGUISound(GSD_SYSTEM_ITEM_TAKEOFF);
		}

		else if(pItemMove->byDestPlace == CONTAINER_TYPE_EQUIP)
		{
			// source 이동.
			SetEquipItem(pItemMove->bySrcSlotIdx, pItemMove->hDestSerial);

			// dest 이동.
			SetEquipItem(pItemMove->byDestSlotIdx, pItemMove->hSrcSerial);

			Logic_PlayGUISound(GSD_SYSTEM_ITEM_EQUIP);
		}
	}
	
	// source item handle event
	pSrcSobItem->HandleEvents(pMsg); 

	// dest item handle event
	if(pItemMove->hDestSerial != INVALID_SERIAL_ID)
	{
		CNtlSobItem *pDestSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(pItemMove->hDestSerial) );
		NTL_ASSERT(pDestSobItem, "CNtlInventory::ItemMoveEventHandler : dest item instance is null !!!");
		pDestSobItem->HandleEvents(pMsg); 
	}

	NTL_RETURNVOID();
}

void CNtlInventory::ItemStackMoveEventHandler(RWS::CMsg &msg)
{
	NTL_FUNCTION( "CNtlInventory::ItemStackMoveEventHandler" );

	SNtlEventSobItemStackMove *pItemStackMove = reinterpret_cast<SNtlEventSobItemStackMove*>(msg.pData); 

	CNtlSobItem* pSrcItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pItemStackMove->hSrcSerial ) );
	CNtlSobItem* pDestItem= reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pItemStackMove->hDestSerial ) );

	SERIAL_HANDLE hDestBag = m_hBagItem[pItemStackMove->byDestPlace - CONTAINER_TYPE_BAG1];
	CNtlSobItem* pDestBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hDestBag ) );
	NTL_ASSERT( pDestBag, "CNtlInventory::ItemStackMoveEventHandler : Dest Bag is Null!!" );
	
	// When divided into two
	if( pDestBag->GetChildSerial( pItemStackMove->byDestSlotIdx ) == INVALID_SERIAL_ID )
	{
		NTL_ASSERT( pSrcItem, "CNtlInventory::ItemStackMoveEventHandler : SrcItem is Null!!" );
		CNtlSobItemAttr* pSrcItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSrcItem->GetSobAttr() );
		sITEM_TBLDAT* pItemData = pSrcItemAttr->GetItemTbl();
		NTL_ASSERT( pItemData, "CNtlInventory::ItemStackMoveEventHandler : SrcItem Table is NULL!" );

		// Item Create
		CNtlSLEventGenerator::SobItemAdd( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(),
										  pItemStackMove->hDestSerial, pItemData->tblidx, pSrcItemAttr->GetOptionSet(),
										  pItemStackMove->byDestPlace, pItemStackMove->byDestSlotIdx, pItemStackMove->byDestStackCount,
										  pSrcItemAttr->GetRank(), pSrcItemAttr->GetGrade(), pSrcItemAttr->GetDur(), pSrcItemAttr->GetMaxDur(), FALSE, pSrcItemAttr->GetBattleAttribute(),
										  pSrcItemAttr->GetRestrictState(), pSrcItemAttr->GetMaker(), pSrcItemAttr->GetDurationType(), pSrcItemAttr->GetStartTime(), pSrcItemAttr->GetEndTime() );										  
		pSrcItem->HandleEvents( msg );		
	}

	// Items to be added are added together only when they are in same time, same owner.

	// when combined into one
	else if( pItemStackMove->bySrcStackCount == 0 )
	{
		NTL_ASSERT( pDestItem, "CNtlInventory::ItemStackMoveEventHandler : DestItem is Null!!" );
		// Item Delete
		CNtlSLEventGenerator::SobItemDelete( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(),
											 pItemStackMove->hSrcSerial, pItemStackMove->bySrcPlace, pItemStackMove->bySrcSlotIdx );
		pDestItem->HandleEvents( msg );
	}

	// 둘다 존재할 때
	else
	{
		NTL_ASSERT( pSrcItem, "CNtlInventory::ItemStackMoveEventHandler : SrcItem is Null!!" );
		NTL_ASSERT( pDestItem, "CNtlInventory::ItemStackMoveEventHandler : DestItem is Null!!" );

		pSrcItem->HandleEvents( msg );
		pDestItem->HandleEvents( msg );
	}
	
	NTL_RETURNVOID();
}

void CNtlInventory::ItemUpdateEventHandler(RWS::CMsg &msg)
{
	NTL_FUNCTION( "CNtlInventory::ItemUpdateEventHandler" );

	SNtlEventSobItemUpdate *pItemUpdate = reinterpret_cast<SNtlEventSobItemUpdate*>(msg.pData);
	
	CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pItemUpdate->hItemSerialId ) );
	
	pItem->HandleEvents(msg);
	NTL_RETURNVOID();
}

void CNtlInventory::ItemDurDownEventHandler(RWS::CMsg &msg)
{
	NTL_FUNCTION( "CNtlInventory::ItemDurDownEventHandler" );
	RwUInt8* pDurData = (RwUInt8*)msg.pData;

	for( RwInt32 i = 0 ; i < NTL_MAX_EQUIP_ITEM_SLOT ; ++i )
	{
		SERIAL_HANDLE hSerial = GetEquipItem( i );
		if( hSerial == INVALID_SERIAL_ID || pDurData[i] == NTL_DUR_UNKNOWN )
			continue;

		CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( hSerial ) );
		NTL_ASSERT( pItem, "CNtlInventory::ItemDurDownEventHandler, pItem Must be present" );
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
		
		pItemAttr->SetDur( pDurData[i] );
	}

	NTL_RETURNVOID();
}

void CNtlInventory::ItemDurUpdateEventHandler(RWS::CMsg &msg)
{
	NTL_FUNCTION( "CNtlInventory::ItemDurUpdateEventHandler" );

	SNtlEventSobItemDurUpdate *pItemUpdate = reinterpret_cast<SNtlEventSobItemDurUpdate*>(msg.pData);

	if( pItemUpdate->hSerialId != INVALID_SERIAL_ID || pItemUpdate->byDur != NTL_DUR_UNKNOWN )
	{
		CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( pItemUpdate->hSerialId ) );
		NTL_ASSERT( pItem, "CNtlInventory::ItemDurUpdateEventHandler, pItem Must be present" );
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );

		pItemAttr->SetDur( pItemUpdate->byDur );
	}

	NTL_RETURNVOID();
}

void CNtlInventory::ItemRestrictStateUpdateEventHandler(RWS::CMsg & msg)
{
	NTL_FUNCTION("CNtlInventory::ItemRestrictStateUpdateEventHandler");

	SNtlEventSobItemRestrictionUpdate *pItemUpdate = reinterpret_cast<SNtlEventSobItemRestrictionUpdate*>(msg.pData);

	CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(GetBagItem(pItemUpdate->byPlace - CONTAINER_TYPE_BAG1))); // convert item place to bag slot
	NTL_ASSERT(pBagItem, "Bag is must present");


	CNtlSobItem* pItem = pBagItem->GetChildItem(pItemUpdate->byPos);
	NTL_ASSERT(pItem, "Item must present");

	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());
	if (pItemAttr->GetRestrictState() == pItemUpdate->byRestrictState)
		return;

	pItem->HandleEvents(msg);

	NTL_RETURNVOID();
}

void CNtlInventory::ItemSocketInsertBeadEventHandler(RWS::CMsg & msg)
{
	NTL_FUNCTION("CNtlInventory::ItemSocketInsertBeadEventHandler");

	SNtlEventItemSocketInsertBead *pItemUpdate = reinterpret_cast<SNtlEventItemSocketInsertBead*>(msg.pData);

	if (pItemUpdate->bSuccess)
	{
		CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(GetBagItem(pItemUpdate->byItemPlace - CONTAINER_TYPE_BAG1))); // convert item place to bag slot
		NTL_ASSERT(pBagItem, "Bag is must present");
		CNtlSobItem* pItem = pBagItem->GetChildItem(pItemUpdate->byItemPos);
		NTL_ASSERT(pItem, "Item must present");

		pItem->HandleEvents(msg);
	}

	if (pItemUpdate->byBeadRemainStack == 0) // if no left, return to avoid crash.
		return;

	CNtlSobItem* pBagItemBead = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(GetBagItem(pItemUpdate->byBeadPlace - CONTAINER_TYPE_BAG1))); // convert item place to bag slot
	NTL_ASSERT(pBagItemBead, "pBagItemBead is must present");
	CNtlSobItem* pItemBead = pBagItemBead->GetChildItem(pItemUpdate->byBeadPos);
	NTL_ASSERT(pItemBead, "pItemBead must present");

	Logic_SetItemStack(pItemBead, pItemUpdate->byBeadRemainStack);

	NTL_RETURNVOID();
}

void CNtlInventory::ItemSocketDestroyBeadEventHandler(RWS::CMsg & msg)
{
	NTL_FUNCTION("CNtlInventory::ItemSocketDestroyBeadEventHandler");

	SNtlEventItemSocketDestroyBead *pItemUpdate = reinterpret_cast<SNtlEventItemSocketDestroyBead*>(msg.pData);

	CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(GetBagItem(pItemUpdate->byPlace - CONTAINER_TYPE_BAG1))); // convert item place to bag slot
	NTL_ASSERT(pBagItem, "Bag is must present");


	CNtlSobItem* pItem = pBagItem->GetChildItem(pItemUpdate->byPos);
	NTL_ASSERT(pItem, "Item must present");

	pItem->HandleEvents(msg);

	NTL_RETURNVOID();
}

void CNtlInventory::ItemUseActionEventHandler(RWS::CMsg &msg)
{
	NTL_FUNCTION( "CNtlInventory::ItemUseActionEventHandler" );

	SNtlEventSobItemUseAction *pItemUseAction = reinterpret_cast<SNtlEventSobItemUseAction*>(msg.pData);

	CUseItemTable* pUseItemTbl = API_GetTableContainer()->GetUseItemTable();
	sUSE_ITEM_TBLDAT *pUseItemTblData = reinterpret_cast<sUSE_ITEM_TBLDAT*>( pUseItemTbl->FindData(pItemUseAction->uiItemTableIdx) );
	NTL_ASSERT(pUseItemTblData, "CNtlInventory::ItemUseActionEventHandler => use item table data not found" << "(" << pItemUseAction->uiItemTableIdx << ")");

	SetItemCooltimeInfo( pUseItemTblData->dwCool_Time_Bit_Flag, (float)pUseItemTblData->dwCool_Time);

	for( RwInt32 i = 0 ; i < NTL_MAX_BAGSLOT_COUNT ; ++i )
	{
		if( GetBagItem( i ) == INVALID_SERIAL_ID )
			continue;

		CNtlSobItem* pBagItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( GetBagItem( i ) ) );
		NTL_ASSERT( pBagItem, "Bag is must present" );		

		for( RwInt32 j = 0 ; j < pBagItem->GetChildNum() ; ++j )
		{
			CNtlSobItem* pItem = pBagItem->GetChildItem( j );
			if( pItem == NULL )
				continue;

			CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
			if( pItemAttr->IsNeedToIdentify() )
				continue;

			sITEM_TBLDAT* pItemData = pItemAttr->GetItemTbl();
			NTL_ASSERT( pItemData, "CNtlInventory::ItemUseActionEventHandler : pItemData Muse be Present! " );

			if( pItemData->Use_Item_Tblidx == INVALID_TBLIDX )
				continue;

			sUSE_ITEM_TBLDAT* pItemUseData = reinterpret_cast<sUSE_ITEM_TBLDAT*>( pUseItemTbl->FindData( pItemData->Use_Item_Tblidx ) );

			stCOOLTIMEDATA* pCooltimeData = GetFirstItemCooltimeInfo( pItemUseData->dwCool_Time_Bit_Flag );
			
			if( pCooltimeData )
			{
				if( pCooltimeData->fCoolTime != 0.0f )
				{
					CNtlSobIcon* pSobIcon = pItem->GetIcon();
					pSobIcon->SetIconState( CNtlSobIcon::ICON_STATE_COOLING, pCooltimeData->fCoolTime, pCooltimeData->fCurrentTime );				
				}
			}
		}
	}

	NTL_RETURNVOID();
}

void CNtlInventory::ItemUpgradeEventHandler(RWS::CMsg &msg)
{
	NTL_FUNCTION( "CNtlInventory::ItemUpgradeEventHandler" );

	SNtlEventSobItemUpgrade* pItemUpgrade = reinterpret_cast<SNtlEventSobItemUpgrade*>( msg.pData );

	if( pItemUpgrade->byStonePlace >= CONTAINER_TYPE_BAG_FIRST && pItemUpgrade->byStonePlace <= CONTAINER_TYPE_BAG_LAST )
	{
		CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagItem[pItemUpgrade->byStonePlace-CONTAINER_TYPE_BAG1] ) );
		NTL_ASSERT(pBag, "Bag not found");
		CNtlSobItem* pStoneItem = pBag->GetChildItem( pItemUpgrade->byStonePos );
		CNtlSobItemAttr* pStoneItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pStoneItem->GetSobAttr() );

		RwUInt8 nStackCount = pItemUpgrade->byStoneStack;
		
		if( nStackCount <= 0 )
		{
			CNtlSLEventGenerator::SobItemDelete( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), pStoneItem->GetSerialID(),
												 pItemUpgrade->byStonePlace, pItemUpgrade->byStonePos );
		}
		else
		{
			pStoneItemAttr->SetStackNum( nStackCount );
		}
	}
	else
		NTL_ASSERTFAIL( "CNtlInventory::ItemUpgradeEventHandler : Upgrade stone place is Wrong" );

	if (pItemUpgrade->byCorePlace != CONTAINER_TYPE_NONE)
	{
		if (pItemUpgrade->byCorePlace >= CONTAINER_TYPE_BAG_FIRST && pItemUpgrade->byCorePlace <= CONTAINER_TYPE_BAG_LAST)
		{
			CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(m_hBagItem[pItemUpgrade->byCorePlace - CONTAINER_TYPE_BAG1]));
			NTL_ASSERT(pBag, "Bag not found");
			CNtlSobItem* pStoneItem = pBag->GetChildItem(pItemUpgrade->byCorePos);
			CNtlSobItemAttr* pStoneItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pStoneItem->GetSobAttr());

			if (pItemUpgrade->byCoreStack <= 0)
			{
				CNtlSLEventGenerator::SobItemDelete(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), pStoneItem->GetSerialID(),
					pItemUpgrade->byCorePlace, pItemUpgrade->byCorePos);
			}
			else
			{
				pStoneItemAttr->SetStackNum(pItemUpgrade->byCoreStack);
			}
		}
		else
			NTL_ASSERTFAIL("CNtlInventory::ItemUpgradeEventHandler : Upgrade core stone place is Wrong");
	}

	if( pItemUpgrade->byItemPlace >= CONTAINER_TYPE_BAG_FIRST && pItemUpgrade->byItemPlace <= CONTAINER_TYPE_BAG_LAST )
	{
		CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( m_hBagItem[pItemUpgrade->byItemPlace-CONTAINER_TYPE_BAG1] ) );
		NTL_ASSERT(pBag, "Bag not found");
		CNtlSobItem* pItem = pBag->GetChildItem( pItemUpgrade->byItemPos );
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );

		if( pItemUpgrade->bItemDestroy )
		{
			CNtlSLEventGenerator::SobItemDelete( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), pItem->GetSerialID(),
												 pItemUpgrade->byItemPlace, pItemUpgrade->byItemPos );
		}
		else
		{
			pItemAttr->SetGrade( pItemUpgrade->byItemGrade );	
		}		
	}
	else
		NTL_ASSERTFAIL( "CNtlInventory::ItemUpgradeEventHandler : Upgrade item place is Wrong" );
	

	NTL_RETURNVOID();
}

void CNtlInventory::ItemChangeOptionSetEventHandler(RWS::CMsg & msg)
{
	NTL_FUNCTION("CNtlInventory::ItemChangeOptionSetEventHandler");

	SNtlEventSobItemChangeOptionSet* pItemOption = reinterpret_cast<SNtlEventSobItemChangeOptionSet*>(msg.pData);

	if (pItemOption->byKitPlace >= CONTAINER_TYPE_BAG_FIRST && pItemOption->byKitPlace <= CONTAINER_TYPE_BAG_LAST)
	{
		CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(m_hBagItem[pItemOption->byKitPlace - CONTAINER_TYPE_BAG1]));
		NTL_ASSERT(pBag, "Bag not found");
		CNtlSobItem* pStoneItem = pBag->GetChildItem(pItemOption->byKitPos);
		NTL_ASSERT(pBag, "Item not found");
		CNtlSobItemAttr* pStoneItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pStoneItem->GetSobAttr());

		if (pItemOption->byKitRemainStack <= 0)
		{
			CNtlSLEventGenerator::SobItemDelete(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), pStoneItem->GetSerialID(),
				pItemOption->byKitPlace, pItemOption->byKitPos);
		}
		else
		{
			pStoneItemAttr->SetStackNum(pItemOption->byKitRemainStack);
		}
	}
	else
		NTL_ASSERTFAIL("CNtlInventory::ItemChangeOptionSetEventHandler : Kit place is Wrong");

	if (pItemOption->byItemPlace >= CONTAINER_TYPE_BAG_FIRST && pItemOption->byItemPlace <= CONTAINER_TYPE_BAG_LAST)
	{
		CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(m_hBagItem[pItemOption->byItemPlace - CONTAINER_TYPE_BAG1]));
		NTL_ASSERT(pBag, "Bag not found");
		CNtlSobItem* pItem = pBag->GetChildItem(pItemOption->byItemPos);
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());

		pItemAttr->SetOptionSet((sITEM_OPTION_SET*)pItemOption->pOptionSet);
	}
	else
		NTL_ASSERTFAIL("CNtlInventory::ItemChangeOptionSetEventHandler : Item place is Wrong");


	NTL_RETURNVOID();
}

void CNtlInventory::ItemChangeBattleAttributeEventHandler(RWS::CMsg & msg)
{
	NTL_FUNCTION("CNtlInventory::ItemChangeBattleAttributeEventHandler");

	SNtlEventSobItemChangeBattleAttribute* pMsgData = reinterpret_cast<SNtlEventSobItemChangeBattleAttribute*>(msg.pData);

	if (pMsgData->byItemPlace >= CONTAINER_TYPE_BAG_FIRST && pMsgData->byItemPlace <= CONTAINER_TYPE_BAG_LAST)
	{
		CNtlSobItem* pBag = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(m_hBagItem[pMsgData->byItemPlace - CONTAINER_TYPE_BAG1]));
		NTL_ASSERT(pBag, "Bag not found");
		CNtlSobItem* pItem = pBag->GetChildItem(pMsgData->byItemPos);
		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pItem->GetSobAttr());

		pItemAttr->SetBattleAttribute(pMsgData->byBattleAttribute);
	}
	else
		NTL_ASSERTFAIL("CNtlInventory::ItemChangeBattleAttributeEventHandler : Upgrade place is Wrong");


	NTL_RETURNVOID();
}

void CNtlInventory::EquippedItemUpgradeAllEventHandler(RWS::CMsg &msg)
{
	NTL_FUNCTION( "CNtlInventory::EquippedItemUpgradeAllEventHandler" );

	SNtlEventSobEquippedItemUpgradeAll* pEquippedItemUpgradeAll = reinterpret_cast<SNtlEventSobEquippedItemUpgradeAll*>( msg.pData );

	for( RwUInt8 i = 0 ; i < pEquippedItemUpgradeAll->byCount ; ++i )
	{
		if( pEquippedItemUpgradeAll->abyUpgradeValue[i] == INVALID_BYTE )
			continue;

		CNtlSobItem* pItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( GetEquipItem( i ) ) );
		if( !pItem )
			continue;

		CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pItem->GetSobAttr() );
		pItemAttr->SetGrade( pEquippedItemUpgradeAll->abyUpgradeValue[i] );

		CNtlSLEventGenerator::SobEquipModelChange( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(), i, pItemAttr->GetTblIdx(), pItemAttr->GetGrade(), pItemAttr->GetBattleAttribute());
	}
}

void CNtlInventory::SetEquipItem(RwInt32 iIdx, SERIAL_HANDLE hSerial)
{
	NTL_FUNCTION("CNtlInventory::SetEquipItem");

	NTL_ASSERT(IsVaildEquipItemIndex(iIdx), "Invalid equip item index");

	m_hEquipItem[iIdx] = hSerial;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 

	if(hSerial == INVALID_SERIAL_ID)
		CNtlSLEventGenerator::SobEquipModelChange(pSobAvatar->GetSerialID(), (RwUInt8)iIdx, INVALID_TBLIDX, 0, BATTLE_ATTRIBUTE_NONE);
	else
	{
		CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject(hSerial) );
		CNtlSobItemAttr *pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>( pSobItem->GetSobAttr() );
		
		CNtlSLEventGenerator::SobEquipModelChange(pSobAvatar->GetSerialID(), (RwUInt8)iIdx, pSobItemAttr->GetTblIdx(), pSobItemAttr->GetGrade(), pSobItemAttr->GetBattleAttribute());
	}

	NTL_RETURNVOID();
}

SERIAL_HANDLE CNtlInventory::GetEquipItem(RwInt32 iIdx)
{
	if(!IsVaildEquipItemIndex(iIdx))
		return INVALID_SERIAL_ID;

	return m_hEquipItem[iIdx];
}

RwInt32 CNtlInventory::FindEquipSlot(SERIAL_HANDLE hSerial)
{
	for(RwInt32 i = 0; i < NTL_MAX_EQUIP_ITEM_SLOT; ++i)
	{
		if(m_hEquipItem[i] == hSerial)
			return i;
	}
	
	return -1;
}

RwBool CNtlInventory::IsVaildEquipItemIndex(RwInt32 iIdx)
{
	if(iIdx < 0 || iIdx >= NTL_MAX_EQUIP_ITEM_SLOT)
		return FALSE;

	return TRUE;
}

RwBool CNtlInventory::IsValidEquipItem(RwInt32 iIdx)
{
	if(!IsVaildEquipItemIndex(iIdx))
		return FALSE;

	if(m_hEquipItem[iIdx] == INVALID_SERIAL_ID)
		return FALSE;

	return TRUE;
}

void CNtlInventory::SetBagItem(RwInt32 iIdx, SERIAL_HANDLE hSerial)
{
	NTL_ASSERT(IsValidBagItemIndex(iIdx), "Invalid bag item index");

	m_hBagItem[iIdx] = hSerial;
}

SERIAL_HANDLE CNtlInventory::GetBagItem(RwInt32 iIdx)
{
	if(!IsValidBagItemIndex(iIdx))
		return INVALID_SERIAL_ID;

	return m_hBagItem[iIdx];
}

RwInt32 CNtlInventory::FindBagSlot(SERIAL_HANDLE hSerial)
{
	for(RwInt32 i = 0; i < NTL_MAX_BAGSLOT_COUNT; ++i)
	{
		if(m_hBagItem[i] == hSerial)
			return i;
	}
	
	return -1;
}

RwBool CNtlInventory::IsValidBagItemIndex(RwInt32 iIdx)
{
	if(iIdx < 0 || iIdx >= NTL_MAX_BAGSLOT_COUNT)
		return FALSE;

	return TRUE;
}

RwBool CNtlInventory::IsValidBagItem(RwInt32 iIdx)
{
	if(!IsValidBagItemIndex(iIdx))
		return FALSE;

	if(m_hBagItem[iIdx] == INVALID_SERIAL_ID)
		return FALSE;

	return TRUE;
}

void CNtlInventory::SetScouterChildItem(RwInt32 iIdx, SERIAL_HANDLE hSerial)
{
	CNtlSobItem* pScouterItem = reinterpret_cast<CNtlSobItem*>( GetNtlSobManager()->GetSobObject( GetScouterItem() ) );
	
	pScouterItem->SetChildSerial( iIdx, hSerial );
}

SERIAL_HANDLE CNtlInventory::GetScouterItem(VOID)
{
	SERIAL_HANDLE hScouter = m_hEquipItem[EQUIP_SLOT_TYPE_SCOUTER-EQUIP_SLOT_TYPE_FIRST];
	NTL_ASSERT( hScouter != INVALID_SERIAL_ID, "CNtlInventory::GetScouterItem : Scouter is not present" );

	return hScouter;
}

void CNtlInventory::InvalidItem(RwUInt32 uiItemSerial)
{
	/*
	CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(uiItemSerial, TRUE)); 
	if(pSobItem == NULL)
		return;
	
	if(pSobItem->GetItemPosition() == CNtlSobItem::SIP_EQUIP)
	{
		CNtlPLItem *pPLItem = pSobItem->GetPLItem();
		if(pPLItem)
			m_pCharacter->SetChangeDefaultItem(pPLItem);

		pSobItem->DestroyItemModel(); 

		SetEquipItem(pSobItem->GetItemSlotIdx(), INVALID_SERIAL_ID);
	}
	else if(pSobItem->GetItemPosition() == CNtlSobItem::SIP_BAG)
	{
		SetBagItem(pSobItem->GetItemSlotIdx(), INVALID_SERIAL_ID);
	}
	else
	{
		CNtlSobItem *pSobParentItem = pSobItem->GetParentItem();
		pSobParentItem->SetChildSerial(pSobItem->GetItemSlotIdx(), INVALID_SERIAL_ID);
	}
	*/
}

void CNtlInventory::SetItemCooltimeInfo( RwUInt32 uiFlag, RwReal fCoolTime )
{
	for( RwInt32 i = 0 ; i < ITEM_COOL_TIME_GROUP_COUNT ; ++i )
	{
		if( uiFlag & m_stCoolTimeGroup[i].dwFlag )
		{
			m_stCoolTimeGroup[i].fCoolTime = fCoolTime;
			m_stCoolTimeGroup[i].fCurrentTime = 0.0f;
		}
	}
}

void CNtlInventory::UnsetItemCooltimeInfo( RwUInt32 uiFlag )
{
	for( RwUInt32 i = 0 ; i < ITEM_COOL_TIME_GROUP_COUNT ; ++i )
	{
		if( uiFlag & m_stCoolTimeGroup[i].dwFlag )
		{
			m_stCoolTimeGroup[i].fCoolTime = 0.0f;
			m_stCoolTimeGroup[i].fCurrentTime = 0.0f;
		}
	}
}

stCOOLTIMEDATA* CNtlInventory::GetFirstItemCooltimeInfo( RwUInt32 uiFlag )
{
	for( RwUInt32 i = 0 ; i < ITEM_COOL_TIME_GROUP_COUNT ; ++i )
	{
		if( uiFlag & m_stCoolTimeGroup[i].dwFlag )
			return &m_stCoolTimeGroup[i];
			
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// avatar quest inventroy 


CNtlQuestInventory::CNtlQuestInventory()
{
	for(RwInt32 i = 0; i < MAX_QUEST_INVENTORY_SLOT; ++i)
	{
		m_hSlotItem[i] = INVALID_SERIAL_ID;
	}
}

CNtlQuestInventory::~CNtlQuestInventory()
{
}

RwBool CNtlQuestInventory::Create(void)
{
	return TRUE;
}

void CNtlQuestInventory::Destroy(void)
{
}

void CNtlQuestInventory::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		CreateEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobQuestItemAdd)
	{
		AddEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobQuestItemDelete)
	{
		DeleteEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobQuestItemUpdate)
	{
		UpdateEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventSobQuestItemMove)
	{
		MoveEventHandler(pMsg);
	}
}

void CNtlQuestInventory::CreateEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlQuestInventory::CreateEventHandler");

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
	SAvatarInfo *pAvatarInfo = GetNtlSLGlobal()->GetAvatarInfo();

	for(RwInt32 i = 0; i < pAvatarInfo->sQuestInventoryInfo.byQuestItemCount; ++i)
	{
		sQUESTITEM_DATA *pQuestItemData = &pAvatarInfo->sQuestInventoryInfo.aQuestItemData[i];
		void *pSobObj = CNtlSLEventGenerator::SobQuestItemCreate(SLCLASS_SLOT_QUESTITEM, INVALID_SERIAL_ID, pQuestItemData->tblidx,
																pQuestItemData->byPos, pQuestItemData->byCurStackCount);
	
		CNtlSobQuestItem *pSobQuestItem = reinterpret_cast<CNtlSobQuestItem*>(pSobObj); 
		pSobQuestItem->SetOwnerID(pSobAvatar->GetSerialID()); 

		m_hSlotItem[pQuestItemData->byPos] = pSobQuestItem->GetSerialID();
	}

	NTL_RETURNVOID();

}

void CNtlQuestInventory::AddEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlQuestInventory::AddEventHandler");

	SNtlEventSobQuestItemAdd *pSobQuestItemAdd = reinterpret_cast<SNtlEventSobQuestItemAdd*>( pMsg.pData );

	void *pSobObj = CNtlSLEventGenerator::SobQuestItemCreate(SLCLASS_SLOT_QUESTITEM, INVALID_SERIAL_ID, pSobQuestItemAdd->uiQuestItemTblId,
															pSobQuestItemAdd->bySlotIdx, pSobQuestItemAdd->byStackCount);

	
	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar(); 
	CNtlSobQuestItem *pSobQuestItem = reinterpret_cast<CNtlSobQuestItem*>(pSobObj); 
	pSobQuestItem->SetOwnerID(pSobAvatar->GetSerialID()); 

	m_hSlotItem[pSobQuestItemAdd->bySlotIdx] = pSobQuestItem->GetSerialID();
	
	NTL_RETURNVOID();
}

void CNtlQuestInventory::DeleteEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobQuestItemDelete *pSobQuestItemDelete = reinterpret_cast<SNtlEventSobQuestItemDelete*>( pMsg.pData );

	CNtlSLEventGenerator::SobDelete( m_hSlotItem[pSobQuestItemDelete->bySlotIdx] );
	m_hSlotItem[pSobQuestItemDelete->bySlotIdx] = INVALID_SERIAL_ID;
}

void CNtlQuestInventory::UpdateEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobQuestItemUpdate *pSobQuestItemUpdate = reinterpret_cast<SNtlEventSobQuestItemUpdate*>( pMsg.pData );

	CNtlSobQuestItem* pItem = GetQuestItemFromIdx( pSobQuestItemUpdate->bySlotIdx );
	pItem->HandleEvents( pMsg );
}

void CNtlQuestInventory::MoveEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobQuestItemMove *pSobQuestItemMove = reinterpret_cast<SNtlEventSobQuestItemMove*>( pMsg.pData );

	SERIAL_HANDLE hSrcSerial = m_hSlotItem[pSobQuestItemMove->bySrcSlotIdx];
	SERIAL_HANDLE hDestSerial = m_hSlotItem[pSobQuestItemMove->byDestSlotIdx];
	
	m_hSlotItem[pSobQuestItemMove->bySrcSlotIdx] = hDestSerial;
	m_hSlotItem[pSobQuestItemMove->byDestSlotIdx] = hSrcSerial;
}

SERIAL_HANDLE CNtlQuestInventory::GetQuestItemSerial( RwUInt8 bySlotIdx )
{
	if( bySlotIdx < 0 || bySlotIdx >= MAX_QUEST_INVENTORY_SLOT )
		return INVALID_SERIAL_ID;

	return m_hSlotItem[bySlotIdx];
}

CNtlSobQuestItem* CNtlQuestInventory::GetQuestItemFromIdx(RwUInt8 bySlotIdx)
{
	if( bySlotIdx < 0 || bySlotIdx >= MAX_QUEST_INVENTORY_SLOT )
		return NULL;

	if(m_hSlotItem[bySlotIdx] == INVALID_SERIAL_ID)
		return NULL;

	return reinterpret_cast<CNtlSobQuestItem*>( GetNtlSobManager()->GetSobObject(m_hSlotItem[bySlotIdx]) );
}

CNtlSobQuestItem* CNtlQuestInventory::GetQuestItemFromTable(RwUInt32 uiQuestItemTblIdx)
{
	for(RwInt32 i= 0; i < MAX_QUEST_INVENTORY_SLOT; ++i)
	{
		if(m_hSlotItem[i] == INVALID_SERIAL_ID)
			continue;

		CNtlSobQuestItem *pSobQuestItem = reinterpret_cast<CNtlSobQuestItem*>( GetNtlSobManager()->GetSobObject(m_hSlotItem[i]) );
		CNtlSobQuestItemAttr *pSobQuestItemAttr = reinterpret_cast<CNtlSobQuestItemAttr*>( pSobQuestItem->GetSobAttr() );
		sQUESTITEM_TBLDAT *pQuestItemTbl = pSobQuestItemAttr->GetQuestItemTbl();

		if(pQuestItemTbl->tblidx != uiQuestItemTblIdx)
			continue;

		return pSobQuestItem;
	}

	return NULL;
}
