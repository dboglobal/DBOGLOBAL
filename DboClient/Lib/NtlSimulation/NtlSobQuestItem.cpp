#include "precomp_ntlsimulation.h"
#include "NtlSobQuestItem.h"

// shared
#include "QuestItemTable.h"

// simulation
#include "NtlSobManager.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "TableContainer.h"
#include "NtlSobAttrFactory.h"
#include "NtlSobAttr.h"
#include "NtlSobQuestItemAttr.h"
#include "NtlSobQuestItemIcon.h"

DEFINITION_MEMORY_POOL(CNtlSobQuestItem)

CNtlSobQuestItem::CNtlSobQuestItem()
{
	m_uiSlotIdx			= -1;

	m_pIcon	= NTL_NEW CNtlSobQuestItemIcon;
	m_pIcon->SetSobObj(this);
}

CNtlSobQuestItem::~CNtlSobQuestItem()
{
	// icon destroy
	if(m_pIcon)
	{
		m_pIcon->Destroy();

		NTL_DELETE( m_pIcon );
	}
}

RwBool CNtlSobQuestItem::Create(void)
{
	NTL_FUNCTION("CNtlSobQuestItem::Create");

	if(!CNtlSob::Create())
	{
		NTL_RETURN(FALSE);
	}

	// class name 설정.
	SetClassName(SLCLASS_NAME_SLOT_QUESTITEM);

	m_pIcon->Create(); 
	m_pIcon->PostCreate();
	
	NTL_RETURN(TRUE);
}

void CNtlSobQuestItem::Destroy(void)
{
	NTL_FUNCTION("CNtlSobQuestItem::Destroy");

	// icon destroy
	if(m_pIcon)
	{
		m_pIcon->Destroy();

		NTL_DELETE( m_pIcon );
	}

	CNtlSob::Destroy();

	NTL_RETURNVOID();
}


void CNtlSobQuestItem::Update(RwReal fElapsed)
{
	if( m_pIcon )
		m_pIcon->Update(fElapsed);
}

void CNtlSobQuestItem::CreateEventHandler(RWS::CMsg &pMsg)
{
	/*
	SNtlEventSobItemCreate *pSobItemCreate = reinterpret_cast<SNtlEventSobItemCreate*>(pMsg.pData);
	CNtlSobQuestItemAttr *pItemAttr = reinterpret_cast<CNtlSobQuestItemAttr*>( GetSobAttr() );

	// 현재 아이템이 Child Slot을 가지는 아이템이면
	if(pItemAttr->IsContainerItem())
		AllocateChild(pItemAttr->GetChildSlotSize());

	RwUInt8 byPlace = pSobItemCreate->byPlace;

	if(byPlace == CONTAINER_TYPE_BAGSLOT)
	{
		SetItemPosition(SIP_BAG);
		SetItemSlotIdx(pSobItemCreate->bySlotIdx);
	}
	else if(byPlace == CONTAINER_TYPE_BAG1 || byPlace == CONTAINER_TYPE_BAG2 ||
			byPlace == CONTAINER_TYPE_BAG3 || byPlace == CONTAINER_TYPE_BAG4 ||
			byPlace == CONTAINER_TYPE_BAG5)
	{
		SetItemPosition(SIP_BAG_CHILD);
		SetItemSlotIdx(pSobItemCreate->bySlotIdx);
		CNtlSobQuestItem *pParentSobItem = reinterpret_cast<CNtlSobQuestItem*>( pSobItemCreate->pParentSobObj );
		NTL_ASSERT(pParentSobItem, "CNtlSobQuestItem::CreateEventHandler is item bag child type");
		pParentSobItem->SetChildSerial(pSobItemCreate->bySlotIdx, GetSerialID());
		SetParentItemSerial(pParentSobItem->GetSerialID()); 
	}
	else if(byPlace == CONTAINER_TYPE_EQUIP)
	{
		SetItemPosition(SIP_EQUIP);
		SetItemSlotIdx(pSobItemCreate->bySlotIdx);
	}
	else if(byPlace == CONTAINER_TYPE_SCOUT)
	{
		SetItemPosition(SIP_SCOUT_CHILD);
		SetItemSlotIdx(pSobItemCreate->bySlotIdx);
		CNtlSobQuestItem *pParentSobItem = reinterpret_cast<CNtlSobQuestItem*>( pSobItemCreate->pParentSobObj );
		NTL_ASSERT(pParentSobItem, "CNtlSobQuestItem::CreateEventHandler is item scouter child type");
		pParentSobItem->SetChildSerial(pSobItemCreate->bySlotIdx, GetSerialID());
		SetParentItemSerial(pParentSobItem->GetSerialID()); 
	}
	else
	{
		NTL_ASSERTFAIL("CNtlSobQuestItem::CreateEventHandler : item place not invalid" << pSobItemCreate->byPlace);
	}
	*/
}


void CNtlSobQuestItem::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlSobQuestItem::HandleEvents");
	
	if(pMsg.Id == g_EventSobCreate)
	{
		// item attribute events.
		GetSobAttr()->HandleEvents(pMsg);

		// item create event handler.
		CreateEventHandler(pMsg);

		// item icon events.
		m_pIcon->HandleEvents(pMsg); 
	}
	else if(pMsg.Id == g_EventSobQuestItemUpdate)
	{
		// item attribute events.
		GetSobAttr()->HandleEvents(pMsg);
	}
	
	NTL_RETURNVOID();
}

RwBool CNtlSobQuestItem::IsDragAndDropPossible(void)
{
	return TRUE;
}

RwBool CNtlSobQuestItem::IsUsePossible(void)
{
	return FALSE;
}

CNtlSobIcon* CNtlSobQuestItem::GetIcon(void) const 
{
	return m_pIcon;
}
