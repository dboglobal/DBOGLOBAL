#include "precomp_ntlsimulation.h"
#include "NtlSobItem.h"

// shared
#include "ItemTable.h"

// presentation
#include "NtlPLPropertyContainer.h"
#include "NtlPLItemProperty.h"
#include "NtlPLResourceManager.h"
#include "NtlPLSceneManager.h"
#include "NtlPLItem.h"

// simulation
#include "NtlSobManager.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "TableContainer.h"
#include "NtlSobAttrFactory.h"
#include "NtlSobAttr.h"
#include "NtlSobItemAttr.h"
#include "NtlSobItemIcon.h"

DEFINITION_MEMORY_POOL(CNtlSobItem)

CNtlSobItem::CNtlSobItem()
{
	m_hParentSerial		= INVALID_SERIAL_ID;
	m_eItemPos			= SIP_NONE;
	m_uiSlotIdx			= -1;
	m_iChildNum			= 0;
	m_pChildSerial		= NULL;
	

	m_pIcon	= NTL_NEW CNtlSobItemIcon;
	m_pIcon->SetSobObj(this);
}

CNtlSobItem::~CNtlSobItem()
{
	// icon destroy
	if(m_pIcon)
	{
		m_pIcon->Destroy();

		NTL_DELETE( m_pIcon );
	}

}

RwBool CNtlSobItem::Create(void)
{
	NTL_FUNCTION("CNtlSobItem::Create");

	if(!CNtlSob::Create())
	{
		NTL_RETURN(FALSE);
	}

	// class name 설정.
	SetClassName(SLCLASS_NAME_SLOT_ITEM);

	m_pIcon->Create(); 
	m_pIcon->PostCreate();
	
	NTL_RETURN(TRUE);
}

void CNtlSobItem::Destroy(void)
{
	NTL_FUNCTION("CNtlSobItem::Destroy");

	// Update 가 걸려있는 경우 삭제.
	if( reinterpret_cast<CNtlSobItemAttr*>( GetSobAttr() )->GetDurationType() )
		GetNtlSobManager()->RemoveUpdateQueue( this );

	// icon destroy
	if(m_pIcon)
	{
		m_pIcon->Destroy();

		NTL_DELETE( m_pIcon );
	}

	// parent에 자신을 등록을 없애준다.
	CNtlSobItem *pParentSobItem = GetParentItem();
	if(pParentSobItem)
	{
		pParentSobItem->SetChildSerial(GetItemSlotIdx(), INVALID_SERIAL_ID);
	}

	// child item을 없애준다.
	if(m_pChildSerial)
	{
		for(RwInt32 i = 0; i < m_iChildNum; ++i)
		{
			if(IsValidChild(i))
			{
				CNtlSobItem *pChildSobItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(GetChildSerial(i)));     
				pChildSobItem->SetParentItemSerial(INVALID_SERIAL_ID);  
				CNtlSLEventGenerator::SobDelete(GetChildSerial(i));
			}
		}

		NTL_DELETE(m_pChildSerial);
	}

	m_hParentSerial = INVALID_SERIAL_ID;
	m_iChildNum = 0;

	CNtlSob::Destroy();

	NTL_RETURNVOID();
}


void CNtlSobItem::Update(RwReal fElapsed)
{
	if( m_pSobAttr )
		reinterpret_cast<CNtlSobItemAttr*>( m_pSobAttr )->Update( fElapsed );

	if( m_pIcon )
		m_pIcon->Update( fElapsed );
}

void CNtlSobItem::CreateEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventSobItemCreate *pSobItemCreate = reinterpret_cast<SNtlEventSobItemCreate*>(pMsg.pData);
	CNtlSobItemAttr *pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( GetSobAttr() );

	// 현재 아이템이 Child Slot을 가지는 아이템이면
	if(pItemAttr->IsContainerItem())
		AllocateChild(pItemAttr->GetChildSlotSize());

	// peessi : 사용기간 제한 아이템은 Update
	if( pSobItemCreate->byDurationType )
		GetNtlSobManager()->AddUpdate( this );

	RwUInt8 byPlace = pSobItemCreate->byPlace;

	if(byPlace == CONTAINER_TYPE_BAGSLOT)
	{
		SetItemPosition(SIP_BAG);
		SetItemSlotIdx(pSobItemCreate->bySlotIdx);
	}
	else if( byPlace >= CONTAINER_TYPE_BAG_FIRST && byPlace <= CONTAINER_TYPE_BAG_LAST )
	{
		SetItemPosition(SIP_BAG_CHILD);
		SetItemSlotIdx(pSobItemCreate->bySlotIdx);
		CNtlSobItem *pParentSobItem = reinterpret_cast<CNtlSobItem*>( pSobItemCreate->pParentSobObj );
		NTL_ASSERT(pParentSobItem, "CNtlSobItem::CreateEventHandler is item bag child type");
		pParentSobItem->SetChildSerial(pSobItemCreate->bySlotIdx, GetSerialID());
		SetParentItemSerial(pParentSobItem->GetSerialID()); 
	}
	else if(byPlace == CONTAINER_TYPE_EQUIP)
	{
		SetItemPosition(SIP_EQUIP);
		SetItemSlotIdx(pSobItemCreate->bySlotIdx);
	}
	else if(byPlace == CONTAINER_TYPE_BANKSLOT)
	{
		SetItemPosition(SIP_WAREHOUSE);
		SetItemSlotIdx(pSobItemCreate->bySlotIdx);
	}
	else if(byPlace >= CONTAINER_TYPE_BANK1 && byPlace <= CONTAINER_TYPE_BANK_LAST )
	{
		SetItemPosition(SIP_WAREHOUSE_CHILD);
		SetItemSlotIdx(pSobItemCreate->bySlotIdx);
		CNtlSobItem *pParentSobItem = reinterpret_cast<CNtlSobItem*>( pSobItemCreate->pParentSobObj );
		NTL_ASSERT(pParentSobItem, "CNtlSobItem::CreateEventHandler is item bag child type");
		pParentSobItem->SetChildSerial(pSobItemCreate->bySlotIdx, GetSerialID());
		SetParentItemSerial(pParentSobItem->GetSerialID()); 
	}
	else
	{
		NTL_ASSERTFAIL("CNtlSobItem::CreateEventHandler : item place not invalid" << pSobItemCreate->byPlace);
	}
}

void CNtlSobItem::UpdateEventHandler(RWS::CMsg &pMsg)
{
	//! peessi : In case of UnIdentified item, bag does not create child space even when created.
	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( GetSobAttr() );

	if( pItemAttr->IsContainerItem() && !m_pChildSerial )
		AllocateChild( pItemAttr->GetChildSlotSize() );
}

void CNtlSobItem::ItemMoveEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlSobItem::ItemMoveEventHandler");

	SNtlEventSobItemMove *pItemMove = reinterpret_cast<SNtlEventSobItemMove*>(pMsg.pData); 
	if(pItemMove->hSrcSerial == GetSerialID())
	{
		if(pItemMove->byDestPlace == CONTAINER_TYPE_BAGSLOT)
		{
			SetItemPosition(SIP_BAG);
		}
		else if(pItemMove->byDestPlace >= CONTAINER_TYPE_BAG_FIRST && pItemMove->byDestPlace <= CONTAINER_TYPE_BAG_LAST )
		{
			SetItemPosition(SIP_BAG_CHILD);
		}
		else if(pItemMove->byDestPlace == CONTAINER_TYPE_EQUIP)
		{
			SetItemPosition(SIP_EQUIP);
		}
		else if(pItemMove->byDestPlace >= CONTAINER_TYPE_BANK1 || pItemMove->byDestPlace <= CONTAINER_TYPE_BANK_LAST)
		{
			SetItemPosition(SIP_WAREHOUSE_CHILD);
		}

		SetItemSlotIdx(pItemMove->byDestSlotIdx);
	}

	if(pItemMove->hDestSerial == GetSerialID())
	{
		if(pItemMove->bySrcPlace == CONTAINER_TYPE_BAGSLOT)
		{
			SetItemPosition(SIP_BAG);
		}
		else if(pItemMove->bySrcPlace >= CONTAINER_TYPE_BAG_FIRST && pItemMove->bySrcPlace <= CONTAINER_TYPE_BAG_LAST )				
		{
			SetItemPosition(SIP_BAG_CHILD);
		}
		else if(pItemMove->bySrcPlace == CONTAINER_TYPE_EQUIP)
		{
			SetItemPosition(SIP_EQUIP);
		}
		else if(pItemMove->byDestPlace >= CONTAINER_TYPE_BANKSLOT)
		{
			SetItemPosition(SIP_WAREHOUSE);
		}
		else if(pItemMove->byDestPlace >= CONTAINER_TYPE_BANK1 || pItemMove->byDestPlace <= CONTAINER_TYPE_BANK_LAST)
		{
			SetItemPosition(SIP_WAREHOUSE_CHILD);
		}

		SetItemSlotIdx(pItemMove->bySrcSlotIdx);
	}
	
	NTL_RETURNVOID();
}

void CNtlSobItem::ItemStackMoveEventHandler(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlSobItem::ItemStackMoveEventHandler");

	SNtlEventSobItemStackMove* pItemStackMove = reinterpret_cast<SNtlEventSobItemStackMove*>(pMsg.pData);

	CNtlSobItemAttr* pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( GetSobAttr() );	

	if( GetSerialID() == pItemStackMove->hSrcSerial )
	{
		pItemAttr->SetStackNum( pItemStackMove->bySrcStackCount );
	}
	else if( GetSerialID() == pItemStackMove->hDestSerial )
	{
		pItemAttr->SetStackNum( pItemStackMove->byDestStackCount );
	}

	NTL_RETURNVOID();
}

void CNtlSobItem::ItemRestrictStateUpdateEventHandler(RWS::CMsg & pMsg)
{
	SNtlEventSobItemRestrictionUpdate* pSobItemUpdate = reinterpret_cast<SNtlEventSobItemRestrictionUpdate*>(pMsg.pData);

	CNtlSobItemAttr *pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(GetSobAttr());

	pSobItemAttr->SetRestrictState(pSobItemUpdate->byRestrictState);
}

void CNtlSobItem::ItemSocketInsertBeadEventHandler(RWS::CMsg & pMsg)
{
	SNtlEventItemSocketInsertBead* pSobItemUpdate = reinterpret_cast<SNtlEventItemSocketInsertBead*>(pMsg.pData);

	CNtlSobItemAttr *pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(GetSobAttr());

	pSobItemAttr->SetRestrictState(pSobItemUpdate->byRestrictState);
	pSobItemAttr->SetDurationType(pSobItemUpdate->byDurationType);
	pSobItemAttr->SetStartTime(pSobItemUpdate->nUseStartTime);
	pSobItemAttr->SetEndTime(pSobItemUpdate->nUseEndTime);

	pSobItemAttr->GetOptionSet()->SetItemOption(NTL_MAX_RANDOM_OPTION_IN_ITEM - 2, pSobItemUpdate->pRandomOption[0].wOptionIndex, pSobItemUpdate->pRandomOption[0].optionValue);
	pSobItemAttr->GetOptionSet()->SetItemOption(NTL_MAX_RANDOM_OPTION_IN_ITEM - 1, pSobItemUpdate->pRandomOption[1].wOptionIndex, pSobItemUpdate->pRandomOption[1].optionValue);
}

void CNtlSobItem::ItemSocketDestroyBeadEventHandler(RWS::CMsg & pMsg)
{
	SNtlEventItemSocketDestroyBead* pSobItemUpdate = reinterpret_cast<SNtlEventItemSocketDestroyBead*>(pMsg.pData);

	CNtlSobItemAttr *pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(GetSobAttr());

	pSobItemAttr->SetRestrictState(pSobItemUpdate->byRestrictState);
	pSobItemAttr->SetDurationType(pSobItemUpdate->byDurationType);
}

void CNtlSobItem::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlSobItem::HandleEvents");
	
	if(pMsg.Id == g_EventSobCreate)
	{
		// item attribute events.
		GetSobAttr()->HandleEvents(pMsg);

		// item create event handler.
		CreateEventHandler(pMsg);

		// item icon events.
		m_pIcon->HandleEvents(pMsg); 
	}
	else if(pMsg.Id == g_EventSobItemMove || pMsg.Id == g_EventSobWarehouseItemMove)
	{
		ItemMoveEventHandler(pMsg);

		m_pIcon->HandleEvents(pMsg);
	}
	else if(pMsg.Id == g_EventSobItemStackMove || pMsg.Id == g_EventSobWarehouseItemStackMove)
	{
		ItemStackMoveEventHandler(pMsg);

		m_pIcon->HandleEvents(pMsg);
	}
	else if(pMsg.Id == g_EventSobItemUpdate)
	{
		// Order is important. On the Icon, check the previous Attr state.
		m_pIcon->HandleEvents(pMsg);
		
		GetSobAttr()->HandleEvents(pMsg);
		// Order is important. In the case of an unidentified item in the Attr, after inputting data, it is processed by UpdateEventHandler.
		UpdateEventHandler(pMsg);
	}
	else if (pMsg.Id == g_EventSobItemRestrictionUpdate)
	{
		// Order is important. On the Icon, check the previous Attr state.
		m_pIcon->HandleEvents(pMsg);

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


void CNtlSobItem::AllocateChild(RwInt32 iChildNum)
{
	NTL_FUNCTION("CNtlSobItem::AllocateChild");

	NTL_ASSERT(m_pChildSerial == NULL, "Child Item pointer not Null");

	m_iChildNum = iChildNum;
	m_pChildSerial = NTL_NEW RwUInt32 [iChildNum];

	for(RwInt32 i = 0; i < m_iChildNum; ++i)
	{
		m_pChildSerial[i] = INVALID_SERIAL_ID;
	}

	NTL_RETURNVOID();
}

CNtlSobItem* CNtlSobItem::GetParentItem(void) const
{
	if(m_hParentSerial == INVALID_SERIAL_ID)
		return NULL;

	CNtlSobItem *pSobParentItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(m_hParentSerial));     

	return pSobParentItem; 
}


CNtlSobItem::SLOT_ITEM_POSITION CNtlSobItem::GetParentItemPosition(void) const
{
	if(m_hParentSerial == INVALID_SERIAL_ID)
		return SIP_NONE;

	CNtlSobItem *pSobParentItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(m_hParentSerial));     

	return pSobParentItem->GetItemPosition(); 
}

RwUInt32 CNtlSobItem::GetParentItemSlotIdx(void) const
{
	if(m_hParentSerial == INVALID_SERIAL_ID)
		return SIP_NONE;

	CNtlSobItem *pSobParentItem = reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(m_hParentSerial));     

	return pSobParentItem->GetItemSlotIdx(); 
}

CNtlSobItem* CNtlSobItem::GetChildItem(RwInt32 iIdx)
{
	if(!IsValidChild(iIdx))
		return NULL;

	RwUInt32 uiSerial = GetChildSerial(iIdx);
	return reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(uiSerial)); 
}

RwInt32 CNtlSobItem::FindEmptyChildSlot(void)
{
	if(m_pChildSerial == NULL)
		return -1;

	for(RwInt32 i = 0; i < m_iChildNum; i++)
	{
		if(m_pChildSerial[i] == INVALID_SERIAL_ID)
			return i;
	}

	return -1;
}


RwBool CNtlSobItem::IsDragAndDropPossible(void)
{
	return TRUE;
}

RwBool CNtlSobItem::IsUsePossible(void)
{
	return m_pIcon->IsUsePossible();
}

RwBool CNtlSobItem::IsStackable(void)
{
	return m_pIcon->IsStackable();
}

RwBool CNtlSobItem::IsEquipItem(void)
{
	CNtlSobItemAttr *pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( GetSobAttr() );
	return pItemAttr->IsEquipItem();
}

RwBool CNtlSobItem::IsBagItem(void)
{
	CNtlSobItemAttr *pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( GetSobAttr() );
	return pItemAttr->IsBagItem();
}

RwBool CNtlSobItem::IsScouterItem(void)
{
	CNtlSobItemAttr *pItemAttr = reinterpret_cast<CNtlSobItemAttr*>( GetSobAttr() );
	return pItemAttr->IsScouterItem();
}

CNtlSobIcon* CNtlSobItem::GetIcon(void) const 
{
	return m_pIcon;
}


