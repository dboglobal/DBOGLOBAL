#include "precomp_ntlsimulation.h"
#include "NtlSobTriggerObjectAttr.h"
#include "NtlDebug.h"

// shared
#include "ObjectTable.h"
#include "TextAllTable.h"
#include "TableContainer.h"

// simualtion
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"

#include "NtlSLLogic.h"
#include "NtlSLApi.h" 


DEFINITION_MEMORY_POOL(CNtlSobTriggerObjectAttr)

CNtlSobTriggerObjectAttr::CNtlSobTriggerObjectAttr() 
{
	m_pTriggerObjTbl = NULL;
}


void CNtlSobTriggerObjectAttr::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobTriggerObjectCreate *pTriggerObjectCreate = reinterpret_cast<SNtlEventSobTriggerObjectCreate*>(pMsg.pData);
		RwUInt32 uiWorldTblId = Logic_GetActiveWorldTableId();
		CObjectTable *pObjectTbl = API_GetTableContainer()->GetObjectTable(uiWorldTblId);
		NTL_ASSERT(pObjectTbl, "CNtlSobTriggerObjectAttr::HandleEvents => acitve world object table is null ( world table id - " << uiWorldTblId << " )");

		sOBJECT_TBLDAT *pObjectTblData = reinterpret_cast<sOBJECT_TBLDAT*>( pObjectTbl->FindData(pTriggerObjectCreate->uiTriggerObjectTblid) );
		NTL_ASSERT(pObjectTblData, "CNtlSobTriggerObjectAttr::HandleEvents => object table is null ( trigger object id - " << pTriggerObjectCreate->uiTriggerObjectTblid << " )");

		SetTriggerObjectTbl(pObjectTblData);

		CTextTable *pTextTable = API_GetTableContainer()->GetTextAllTable()->GetObjectTbl();
		sTEXT_TBLDAT* pTextTblData = reinterpret_cast<sTEXT_TBLDAT*>( pTextTable->FindData( m_pTriggerObjTbl->dwName ) );

		if(pTextTblData)
		{
			if(pTextTblData->wstrText.size() > 0)
			{
				SetName(pTextTblData->wstrText.c_str());
			}
			else
			{
				SetNameCreate(FALSE);
			}
		}
		else
		{
			SetNameCreate(FALSE);
		}

		SetNameColor(NTL_OBJ_NAME_COLOR);
	}
}

void CNtlSobTriggerObjectAttr::SetTriggerObjectTbl(const sOBJECT_TBLDAT *pTriggerObjTbl)
{
	m_pTriggerObjTbl = const_cast<sOBJECT_TBLDAT*>(pTriggerObjTbl);
	
	SetRadius(pTriggerObjTbl->fRadius);
}


RwBool CNtlSobTriggerObjectAttr::IsHaveOperating(void)
{
	if(m_pTriggerObjTbl == NULL)
		return FALSE;

	if(m_pTriggerObjTbl->objectDirectionIndex == INVALID_DWORD)
		return FALSE;

	return TRUE;
}

