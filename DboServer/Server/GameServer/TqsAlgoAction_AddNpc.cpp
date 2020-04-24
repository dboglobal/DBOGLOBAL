#include "stdafx.h"
#include "TqsAlgoAction_AddNpc.h"
#include "TQSNodeAction_AddNpc.h"
#include "ObjectManager.h"
#include "TableContainerManager.h"
#include "NPCTable.h"
#include "Npc.h"


CTqsAlgoAction_AddNpc::CTqsAlgoAction_AddNpc(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_ADDNPC, "TQS_ALGOCONTROLID_ACTION_ADDNPC")
{
}


CTqsAlgoAction_AddNpc::~CTqsAlgoAction_AddNpc()
{
}


bool CTqsAlgoAction_AddNpc::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_AddNpc* pAction = dynamic_cast<CTQSNodeAction_AddNpc*>(pControlScriptNode);
	if (pAction)
	{
		m_npcTblidx = pAction->m_npcTblidx;
		m_vLoc = pAction->m_vLoc;
		m_vDir = pAction->m_vDir;

		m_bySpawnFuncFlag = pAction->m_bySpawnFuncFlag;
		m_dwPlayScriptNumber = pAction->m_dwPlayScriptNumber;
		m_dwPlayScriptScene = pAction->m_dwPlayScriptScene;

		m_qwCharConditionFlag = pAction->m_qwCharConditionFlag;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_AddNpc", pControlScriptNode);
	return false;
}

int CTqsAlgoAction_AddNpc::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	sNPC_DATA npcData;
	InitNpcData(npcData);


	if (GetOwner()->GetWorld())
	{
		npcData.worldID = GetOwner()->GetWorld()->GetID();
		npcData.worldtblidx = GetOwner()->GetWorld()->GetIdx();
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "fail : CTqsAlgoAction_AddMob does not have WORLD! ");
		return SYSTEMERROR;
	}


	npcData.tblidx = m_npcTblidx;
	m_vLoc.CopyTo(npcData.vCurLoc);
	m_vLoc.CopyTo(npcData.vSpawnLoc);
	m_vDir.CopyTo(npcData.vCurDir);
	m_vDir.CopyTo(npcData.vSpawnDir);
	npcData.bySpawnFuncFlag = m_bySpawnFuncFlag;

	npcData.sScriptData.playScript = (TBLIDX)m_dwPlayScriptNumber;
	npcData.sScriptData.playScriptScene = (TBLIDX)m_dwPlayScriptScene;
	npcData.qwCharConditionFlag = m_qwCharConditionFlag;
	npcData.actionpatternTblIdx = 1;

	sNPC_TBLDAT* pTbldat = (sNPC_TBLDAT*)g_pTableContainer->GetNpcTable()->FindData(m_npcTblidx);
	if (pTbldat)
	{
		CNpc* pNpc = (CNpc*)g_pObjectManager->CreateCharacter(OBJTYPE_NPC);
		if (pNpc)
		{
			if (pNpc->CreateDataAndSpawn(npcData, pTbldat))
			{
				pNpc->SetStandAlone(false);
				pNpc->SetPcRelation(RELATION_TYPE_ALLIENCE);
				pNpc->SetTMQ(GetOwner()->GetTimeQuest());
				pNpc->AddScript(GetOwner());	//add script

				GetOwner()->AddNpc(pNpc);

				m_status = COMPLETED;
			}
			else m_status = FAILED;
		}
		else m_status = FAILED;
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "Could not find NPC-TBLDAT. Tblidx %u", m_npcTblidx);
		m_status = FAILED;
	}

	return m_status;
}
