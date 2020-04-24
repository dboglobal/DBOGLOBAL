#include "precomp_ntlsimulation.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLLogic.h"
#include "NtlSLGlobal.h"
#include "NtlSLEventFunc.h"
#include "NtlSobAvatar.h"
#include "NtlSobItem.h"
#include "NtlSobItemAttr.h"
#include "NtlSobManager.h"

#include "ItemTable.h"
#include "NtlSLPrivateShop.h"

CNtlPrivateShop::CNtlPrivateShop()
{
}

CNtlPrivateShop::~CNtlPrivateShop()
{
}

RwBool CNtlPrivateShop::Create()
{
	NTL_FUNCTION("CNtlPrivateShop::Create");

	m_mapEventFunc[g_EventPrivateShopState.Get_pEventId()]			= &CNtlPrivateShop::HandleEvents_PrivateShopState;
	m_mapEventFunc[g_EventPrivateShopStateVisitor.Get_pEventId()]	= &CNtlPrivateShop::HandleEvents_PrivateShopStateVisitor;
	m_mapEventFunc[g_EventPrivateShopItem.Get_pEventId()]			= &CNtlPrivateShop::HandleEvents_PrivateShopItem;
	m_mapEventFunc[g_EventPrivateShopItemDataInfo.Get_pEventId()]	= &CNtlPrivateShop::HandleEvents_PrivateShopItemDataInfo;
	m_mapEventFunc[g_EventPrivateShopItemSelect.Get_pEventId()]		= &CNtlPrivateShop::HandleEvents_PrivateShopItemSelect;
	m_mapEventFunc[g_EventPrivateShopItemBuying.Get_pEventId()]		= &CNtlPrivateShop::HandleEvents_PrivateShopItemBuying;
	m_mapEventFunc[g_EventPrivateShopItemState.Get_pEventId()]		= &CNtlPrivateShop::HandleEvents_PrivateShopItemState;

	NTL_RETURN(TRUE);
}

void CNtlPrivateShop::Destroy()
{
	NTL_FUNCTION("CNtlPrivateShop::Destroy");

	m_mapEventFunc.clear();

	NTL_RETURNVOID();
}

void CNtlPrivateShop::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlPrivateShop::HandleEvents");

	MapFuncEventHandler::iterator it = m_mapEventFunc.find( pMsg.Id );
	if( it != m_mapEventFunc.end() )
	{
		(this->*(*it).second)(pMsg);
	}

	NTL_RETURNVOID();
}

void CNtlPrivateShop::HandleEvents_PrivateShopState(RWS::CMsg &pMsg)
{
	SNtlEventPrivateShopState* pShopState = reinterpret_cast<SNtlEventPrivateShopState*>(pMsg.pData);
	if (pShopState->IsAvatarPrivateShop)
	{
		switch (pShopState->uiEventType)
		{
		case PRIVATESHOP_EVENT_CREATE: // 상점 시작
			SetPrivateShopState((ePRIVATESHOP_STATE)pShopState->uiPrivateShopState, pShopState->pPrivateShopData);
			ClearPrivateShopItem(true);
			break;
		case PRIVATESHOP_EVENT_EXIT: // 상점 끝
			SetPrivateShopState((ePRIVATESHOP_STATE)pShopState->uiPrivateShopState, pShopState->pPrivateShopData);
			ClearPrivateShopItem(true);
			break;
		case PRIVATESHOP_EVENT_OPEN: // 판매 개시
			SetPrivateShopState((ePRIVATESHOP_STATE)pShopState->uiPrivateShopState, pShopState->pPrivateShopData);
			break;
		case PRIVATESHOP_EVENT_CLOSE: // 판매 중지
			SetPrivateShopState((ePRIVATESHOP_STATE)pShopState->uiPrivateShopState, pShopState->pPrivateShopData);
			break;
		}
	}
}

void CNtlPrivateShop::HandleEvents_PrivateShopStateVisitor(RWS::CMsg &pMsg)
{
	SNtlEventPrivateShopStateVisitor* pShopStateVisitor = reinterpret_cast<SNtlEventPrivateShopStateVisitor*>(pMsg.pData);

	switch(pShopStateVisitor->uiEventType)
	{
	case PRIVATESHOP_EVENT_ENTER:
		m_ePrivateShopState = PRIVATESHOP_STATE_NONE;
		break;
	case PRIVATESHOP_EVENT_LEAVE:
		m_ePrivateShopState = PRIVATESHOP_STATE_NONE;
		break;
	}

}

void CNtlPrivateShop::HandleEvents_PrivateShopItem(RWS::CMsg &pMsg)
{
	SNtlEventPrivateShopItem* pShopItem = reinterpret_cast<SNtlEventPrivateShopItem*>(pMsg.pData);
	switch (pShopItem->uiEventType)
	{
	case PRIVATESHOP_EVENT_ITEM_INSERT:
		{
			if (pShopItem->sPrivateShopItemData.bIsSaveItem)
			{
				sPRIVATESHOP_ITEM_DATA& sPrivateShopItemData = pShopItem->sPrivateShopItemData;
				RwUInt32				uiPlace				 = sPrivateShopItemData.sItem.byPlace - CONTAINER_TYPE_BAG1;
				if (uiPlace >= 0 && uiPlace <= 5)
				{
					SERIAL_HANDLE		hBag		= GetNtlSLGlobal()->GetSobAvatar()->GetInventory()->GetBagItem(uiPlace);
					CNtlSobItem*		pBag		= reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hBag));
					CNtlSobItem*		pItem		= pBag->GetChildItem(sPrivateShopItemData.sItem.byPosition);

					CNtlSLEventGenerator::SobItemDelete(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(),
						pItem->GetSerialID(),
						sPrivateShopItemData.sItem.byPlace,
						sPrivateShopItemData.sItem.byPosition);
				}
			}
			SetPrivateShopItemData(pShopItem->sPrivateShopItemData.byPrivateShopInventorySlotPos, &pShopItem->sPrivateShopItemData);
			break;
		}
	case PRIVATESHOP_EVENT_ITEM_UPDATE:
		{
			m_aPrivateShopItemData[pShopItem->sPrivateShopItemData.byPrivateShopInventorySlotPos].dwZenny = pShopItem->sPrivateShopItemData.dwZenny;
			break;
		}
	case PRIVATESHOP_EVENT_ITEM_DELETE:
		{				
			if (pShopItem->sPrivateShopItemData.bIsSaveItem)
			{
				sPRIVATESHOP_ITEM_DATA&	sPrivateShopItemData = pShopItem->sPrivateShopItemData;
				sITEM_TBLDAT*			pTblData			 = Logic_GetItemDataFromTable(sPrivateShopItemData.sItem.itemNo);
				CNtlSLEventGenerator::SobItemAdd(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(),
					sPrivateShopItemData.hItem,
					sPrivateShopItemData.sItem.itemNo, 
					&sPrivateShopItemData.sItem.sOptionSet,
					sPrivateShopItemData.sItem.byPlace,
					sPrivateShopItemData.sItem.byPosition,
					sPrivateShopItemData.sItem.byStackcount,
					sPrivateShopItemData.sItem.byRank,
					sPrivateShopItemData.sItem.byGrade,
					sPrivateShopItemData.sItem.byCurrentDurability,
					pTblData->byDurability,
					sPrivateShopItemData.sItem.bNeedToIdentify,
					sPrivateShopItemData.sItem.byBattleAttribute,
					sPrivateShopItemData.sItem.byRestrictState,
					sPrivateShopItemData.sItem.awchMaker,
					sPrivateShopItemData.sItem.byDurationType,
					sPrivateShopItemData.sItem.nUseStartTime,
					sPrivateShopItemData.sItem.nUseEndTime);		
			}
			SetPrivateShopItemData(pShopItem->sPrivateShopItemData.byPrivateShopInventorySlotPos, NULL);
			break;
		}
	}
}

void CNtlPrivateShop::HandleEvents_PrivateShopItemDataInfo(RWS::CMsg &pMsg)
{
	SNtlEventPrivateShopItemDataInfo* pShopItemDataInfo = reinterpret_cast<SNtlEventPrivateShopItemDataInfo*>(pMsg.pData);
	if (pShopItemDataInfo->hOwnerSerial == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID())
	{
		SetPrivateShopItemData(pShopItemDataInfo->sPrivateShopItemData.sItem.byPosition, &pShopItemDataInfo->sPrivateShopItemData);
	}
}

void CNtlPrivateShop::HandleEvents_PrivateShopItemSelect(RWS::CMsg &pMsg)
{
	SNtlEventPrivateShopItemSelect* pShopItemSelect = reinterpret_cast<SNtlEventPrivateShopItemSelect*>(pMsg.pData);

	if (m_ePrivateShopState != PRIVATESHOP_STATE_NONE)
	{
		m_aPrivateShopItemData[pShopItemSelect->uiPrivateShopPos].byItemState = pShopItemSelect->uiItemState;
	}		
}

void CNtlPrivateShop::HandleEvents_PrivateShopItemState(RWS::CMsg &pMsg)
{
	SNtlEventPrivateShopItemState* pShopItemSelect = reinterpret_cast<SNtlEventPrivateShopItemState*>(pMsg.pData);
	if (m_ePrivateShopState != PRIVATESHOP_STATE_NONE)
	{
		for (int i = 0; i < pShopItemSelect->iStateCount; ++i)
		{
			m_aPrivateShopItemData[i].byItemState = pShopItemSelect->auiItemState[i];
		}
	}	
}

void CNtlPrivateShop::HandleEvents_PrivateShopItemBuying(RWS::CMsg &pMsg)
{
	SNtlEventPrivateShopItemBuying* pShopItemBuying = reinterpret_cast<SNtlEventPrivateShopItemBuying*>(pMsg.pData);

	if (pShopItemBuying->bIsBuyAvatar)
	{
		for (int i = 0; i < pShopItemBuying->uiBuyCount; ++i)
		{
			sPRIVATESHOP_ITEM_DATA& sPrivateShopItemData = pShopItemBuying->asPrivateShopItemData[i];
			if (sPrivateShopItemData.hItem != INVALID_SERIAL_ID &&
				sPrivateShopItemData.byPrivateShopInventorySlotPos != PRIVATESHOP_ITEMPOS_INVALID)
			{
				sITEM_TBLDAT* pTblData = Logic_GetItemDataFromTable(sPrivateShopItemData.sItem.itemNo);
				CNtlSLEventGenerator::SobItemAdd(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(),
					sPrivateShopItemData.hItem,
					sPrivateShopItemData.sItem.itemNo, 
					&sPrivateShopItemData.sItem.sOptionSet,
					sPrivateShopItemData.sItem.byPlace,
					sPrivateShopItemData.sItem.byPosition,
					sPrivateShopItemData.sItem.byStackcount,
					sPrivateShopItemData.sItem.byRank,
					sPrivateShopItemData.sItem.byGrade,
					sPrivateShopItemData.sItem.byCurrentDurability,
					pTblData->byDurability,
					sPrivateShopItemData.sItem.bNeedToIdentify,
					sPrivateShopItemData.sItem.byBattleAttribute,
					sPrivateShopItemData.sItem.byRestrictState,
					sPrivateShopItemData.sItem.awchMaker,
					sPrivateShopItemData.sItem.byDurationType,
					sPrivateShopItemData.sItem.nUseStartTime,
					sPrivateShopItemData.sItem.nUseEndTime);		
			}
		}
	}
	else if (pShopItemBuying->hOwnerSerial == GetNtlSLGlobal()->GetSobAvatar()->GetSerialID())
	{
		for (int i = 0; i < pShopItemBuying->uiBuyCount; ++i)
		{
			if (pShopItemBuying->asPrivateShopItemData[i].byPrivateShopInventorySlotPos != PRIVATESHOP_ITEMPOS_INVALID)
			{
				sPRIVATESHOP_ITEM_DATA& sPrivateShopItemData = m_aPrivateShopItemData[pShopItemBuying->asPrivateShopItemData[i].byPrivateShopInventorySlotPos];

				if (sPrivateShopItemData.hItem != INVALID_SERIAL_ID &&
					sPrivateShopItemData.byPrivateShopInventorySlotPos != PRIVATESHOP_ITEMPOS_INVALID)					
				{
					if (!sPrivateShopItemData.bIsSaveItem)
					{
						RwUInt32 uiPlace = sPrivateShopItemData.sItem.byPlace - CONTAINER_TYPE_BAG1;
						if (uiPlace >= 0 && uiPlace <= 5)
						{
							SERIAL_HANDLE	hBag	= GetNtlSLGlobal()->GetSobAvatar()->GetInventory()->GetBagItem(uiPlace);
							CNtlSobItem*	pBag	= reinterpret_cast<CNtlSobItem*>(GetNtlSobManager()->GetSobObject(hBag));
							CNtlSobItem*	pItem	= pBag->GetChildItem(sPrivateShopItemData.sItem.byPosition);

							CNtlSLEventGenerator::SobItemDelete(GetNtlSLGlobal()->GetSobAvatar()->GetSerialID(),
								pItem->GetSerialID(),
								sPrivateShopItemData.sItem.byPlace,
								sPrivateShopItemData.sItem.byPosition);
						}
					}
				}
				SetPrivateShopItemData(pShopItemBuying->asPrivateShopItemData[i].byPrivateShopInventorySlotPos, NULL);
			}
		}
	}
}

void CNtlPrivateShop::SetPrivateShopState(ePRIVATESHOP_STATE ePrivateShopState, sPRIVATESHOP_SHOP_DATA* pPrivateShopData)
{
	m_ePrivateShopState = ePrivateShopState;
	if (pPrivateShopData)
	{
		memcpy(&m_PrivateShopData, pPrivateShopData, sizeof(sPRIVATESHOP_SHOP_DATA));
	}
}

void CNtlPrivateShop::SetPrivateShopItemData(RwUInt8 byPos, sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData)
{
	if (pPrivateShopItemData)
	{		
		m_aPrivateShopItemData[byPos] = *pPrivateShopItemData;
	}
	else
	{
		m_aPrivateShopItemData[byPos] = sPRIVATESHOP_ITEM_DATA();
	}
}

RwUInt8 CNtlPrivateShop::GetPrivateShopEmptyPos()
{
	for (RwUInt8 i = 0; i < NTL_MAX_PRIVATESHOP_INVENTORY; ++i)
	{
		if (m_aPrivateShopItemData[i].byPrivateShopInventorySlotPos == PRIVATESHOP_ITEMPOS_INVALID) 
		{
			return i;
		}
	}
	return PRIVATESHOP_ITEMPOS_INVALID;
}

void CNtlPrivateShop::ClearPrivateShopItem(RwBool bCashOnly)
{
	for (RwUInt8 i = 0; i < NTL_MAX_PRIVATESHOP_INVENTORY; ++i)
	{
		if (bCashOnly)
		{
			if (m_aPrivateShopItemData[i].bIsSaveItem)
			{
				continue;
			}
		}

		SetPrivateShopItemData(i, NULL);
	}
}

sPRIVATESHOP_ITEM_DATA*	CNtlPrivateShop::GetPrivateShopItemData(BYTE byPos)
{
	return &m_aPrivateShopItemData[byPos];
}