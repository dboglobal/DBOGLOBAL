#include "precomp_ntlsimulation.h"
#include "NtlSobHTBSkillAttr.h"

// shared
#include "HTBSetTable.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// simualtion
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"


DEFINITION_MEMORY_POOL(CNtlSobHTBSkillAttr)

void CNtlSobHTBSkillAttr::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobHTBSkillCreate *pHTBSkillCreate = reinterpret_cast<SNtlEventSobHTBSkillCreate*>(pMsg.pData);

		CHTBSetTable *pHTBSkillTable = API_GetTableContainer()->GetHTBSetTable();
		m_pHTBSkillTbl = reinterpret_cast<sHTB_SET_TBLDAT*>( pHTBSkillTable->FindData(pHTBSkillCreate->uiHTBSkillTblId) );
		NTL_ASSERT( m_pHTBSkillTbl, "HTBSkill table is null (" << pHTBSkillCreate->uiHTBSkillTblId << ")" );		
	}
}