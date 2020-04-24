#include "precomp_ntlsimulation.h"
#include "NtlSobQuestItemIcon.h"

// shared
#include "QuestItemTable.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSobQuestItem.h"
#include "NtlSobQuestItemAttr.h"
#include "NtlSLGlobal.h"
#include "TableContainer.h"
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"
#include "NtlSobManager.h"
#include "NtlSLLogic.h"

DEFINITION_MEMORY_POOL(CNtlSobQuestItemIcon)

CNtlSobQuestItemIcon::CNtlSobQuestItemIcon() 
{
	SetClassID(SLCLASS_QUESTITEM_ICON);
	SetClassName(SLCLASS_NAME_QUESTITEM_ICON);
}

CNtlSobQuestItemIcon::~CNtlSobQuestItemIcon() 
{
	
}

void CNtlSobQuestItemIcon::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		//SNtlEventSobQuestItemCreate *pSobQuestItemCreate = reinterpret_cast<SNtlEventSobQuestItemCreate*>(pMsg.pData);
		CNtlSobQuestItem *pSobQuestItem = reinterpret_cast<CNtlSobQuestItem*>(m_pSobObj);
		CNtlSobQuestItemAttr *pSobQuestItemAttr = reinterpret_cast<CNtlSobQuestItemAttr*>(pSobQuestItem->GetSobAttr());
		sQUESTITEM_TBLDAT* pQuestItemTbl = pSobQuestItemAttr->GetQuestItemTbl();

		CreateImage(pQuestItemTbl->szIconName);
	}

	/*
	else if(pMsg.Id == g_EventSobItemUpdate)
	{
		SNtlEventSobItemUpdate* pSobItemUpdate = reinterpret_cast<SNtlEventSobItemUpdate*>(pMsg.pData);
		
		CNtlSobItem *pSobItem = reinterpret_cast<CNtlSobItem*>(m_pSobObj);
		CNtlSobItemAttr *pSobItemAttr = reinterpret_cast<CNtlSobItemAttr*>(pSobItem->GetSobAttr());
		sITEM_TBLDAT* pItemTbl = pSobItemAttr->GetItemTbl();
		
		// 이전엔 ItemUnIdentified였다가 풀렸을때.
		if( pSobItemAttr->IsNeedToIdentify() && !pSobItemUpdate->bNeedToIdentify )
		{
			CreateImage(pItemTbl->strIcon_Name.c_str());
		}
	}
	*/
}


RwBool CNtlSobQuestItemIcon::IsDragAndDropPossible(void) 
{
	return TRUE; 
}

RwBool CNtlSobQuestItemIcon::IsUsePossible(void)
{
	return FALSE;
}

RwBool CNtlSobQuestItemIcon::Use(SERIAL_HANDLE hTargetSerialId, RwUInt32 param1/* = 0xFF*/, RwUInt32 param2/* = INVALID_SERIAL_ID*/, bool pStatus/* = false*/)
{
	return FALSE;
}