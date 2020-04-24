#include "precomp_ntlsimulation.h"
#include "NtlSobQuestItemAttr.h"

// shared
#include "QuestItemTable.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLGlobal.h"
#include "NtlSLEvent.h"
#include "NtlSLApi.h"

DEFINITION_MEMORY_POOL(CNtlSobQuestItemAttr)

void CNtlSobQuestItemAttr::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobQuestItemCreate *pSobQuestItemCreate = reinterpret_cast<SNtlEventSobQuestItemCreate*>(pMsg.pData);
		CQuestItemTable *pQuestItemTbl = API_GetTableContainer()->GetQuestItemTable();
				
		m_pQuestItemTbl = reinterpret_cast<sQUESTITEM_TBLDAT*>( pQuestItemTbl->FindData(pSobQuestItemCreate->uiQuestItemTblId) );
		NTL_ASSERT( m_pQuestItemTbl, "CNtlSobQuestItemAttr::HandleEvents is table id invalid(" <<  pSobQuestItemCreate->uiQuestItemTblId << ")" );
		m_byStackNum = pSobQuestItemCreate->byStackCount;
	}
	else if(pMsg.Id == g_EventSobQuestItemUpdate)
	{
		SNtlEventSobQuestItemUpdate *pSobQuestItemUpdate = reinterpret_cast<SNtlEventSobQuestItemUpdate*>(pMsg.pData);

		m_byStackNum = pSobQuestItemUpdate->byCount;
	}
}