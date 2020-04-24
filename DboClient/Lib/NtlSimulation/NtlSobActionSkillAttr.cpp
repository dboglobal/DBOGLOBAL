#include "precomp_ntlsimulation.h"
#include "NtlSobActionSkillAttr.h"

// shared
#include "ActionTable.h"
#include "TableContainer.h"

// core
#include "NtlDebug.h"

// simualtion
#include "NtlSLEvent.h"
#include "NtlSLGlobal.h"
#include "NtlSLApi.h"

DEFINITION_MEMORY_POOL(CNtlSobActionSkillAttr)

void CNtlSobActionSkillAttr::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventSobCreate)
	{
		SNtlEventSobActionSkillCreate *pActionSkillCreate = reinterpret_cast<SNtlEventSobActionSkillCreate*>(pMsg.pData);

		CActionTable *pActionSkillTable = API_GetTableContainer()->GetActionTable();
		m_pActionSkillTbl = reinterpret_cast<sACTION_TBLDAT*>( pActionSkillTable->FindData(pActionSkillCreate->uiActionSkillTblId) );
		NTL_ASSERT( m_pActionSkillTbl, "ActionSkill table is null (" << pActionSkillCreate->uiActionSkillTblId << ")" );		
	}
}