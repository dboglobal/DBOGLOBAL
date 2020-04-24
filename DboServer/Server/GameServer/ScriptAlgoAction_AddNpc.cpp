#include "stdafx.h"
#include "ScriptAlgoAction_AddNpc.h"
#include "WPSNodeAction_AddNpc.h"
#include "ObjectManager.h"
#include "TableContainerManager.h"
#include "NPCTable.h"
#include "Npc.h"


CWpsAlgoAction_AddNpc::CWpsAlgoAction_AddNpc(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_ADDNPC, "SCRIPTCONTROL_ACTION_ADDNPC")
{

}


CWpsAlgoAction_AddNpc::~CWpsAlgoAction_AddNpc()
{
}


bool CWpsAlgoAction_AddNpc::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_AddNpc* pAction = dynamic_cast<CWPSNodeAction_AddNpc*>(pControlScriptNode);
	if (pAction)
	{
		m_npcTblidx = pAction->m_npcTblidx;
		m_vLoc = pAction->m_vLoc;
		m_vDir = pAction->m_vDir;

		m_bUseLocVar = pAction->m_bUseLocVar;
		m_strLocVar = pAction->m_strLocVar;
		m_strDirVar = pAction->m_strDirVar;

		m_bySpawnFuncFlag = pAction->m_bySpawnFuncFlag;
		m_dwPlayScriptNumber = pAction->m_dwPlayScriptNumber;
		m_dwPlayScriptScene = pAction->m_dwPlayScriptScene;
		m_dwAISNumber = pAction->m_dwAISNumber;
		m_dwAISScene = pAction->m_dwAISScene;
		m_qwCharConditionFlag = pAction->m_qwCharConditionFlag;
		m_bStandAlone = pAction->m_bStandAlone;
		m_partyId = pAction->m_partyId;
		m_bPartyLeader = pAction->m_bPartyLeader;
		m_byImmortalMode = pAction->m_byImmortalMode;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_AddNpc", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_AddNpc::OnUpdate(DWORD dwTickDiff, float fMultiple)
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
		npcData.worldID = 1;
		npcData.worldtblidx = 1;
	}

	if (m_bUseLocVar)
	{
		sSSD_VARIABLE* pLocVar = GetVariable(m_strLocVar);
		if (!pLocVar)
		{
			ERR_LOG(LOG_SCRIPT, "Could not find m_strLocVar %s \n", m_strLocVar.c_str());
			m_status = FAILED;
			return m_status;
		}
		sSSD_VARIABLE* pDirVar = GetVariable(m_strDirVar);
		if (!pDirVar)
		{
			ERR_LOG(LOG_SCRIPT, "Could not find m_strDirVar %s \n", m_strDirVar.c_str());
			m_status = FAILED;
			return m_status;
		}

		m_vLoc = pLocVar->sVector;
		m_vDir = pDirVar->sVector;
	}


	npcData.tblidx = m_npcTblidx;
	m_vLoc.CopyTo(npcData.vCurLoc);
	m_vLoc.CopyTo(npcData.vSpawnLoc);
	m_vDir.CopyTo(npcData.vCurDir);
	m_vDir.CopyTo(npcData.vSpawnDir);
	npcData.bySpawnFuncFlag = m_bySpawnFuncFlag;

	npcData.sScriptData.playScript = (TBLIDX)m_dwPlayScriptNumber;
	npcData.sScriptData.playScriptScene = (TBLIDX)m_dwPlayScriptScene;
	npcData.sScriptData.tblidxAiScript = (TBLIDX)m_dwAISNumber;
	npcData.sScriptData.tblidxAiScriptScene = (TBLIDX)m_dwAISScene;
	npcData.qwCharConditionFlag = m_qwCharConditionFlag;
	npcData.partyID = m_partyId;
	npcData.bPartyLeader = m_bPartyLeader;
	npcData.byImmortalMode = m_byImmortalMode;
	npcData.actionpatternTblIdx = 1;

	sNPC_TBLDAT* pTbldat = (sNPC_TBLDAT*)g_pTableContainer->GetNpcTable()->FindData(m_npcTblidx);
	if (pTbldat)
	{
		CNpc* pNpc = (CNpc*)g_pObjectManager->CreateCharacter(OBJTYPE_NPC);
		if (pNpc)
		{
			if (pNpc->CreateDataAndSpawn(npcData, pTbldat))
			{
				pNpc->SetStandAlone(m_bStandAlone);
				pNpc->SetPcRelation(RELATION_TYPE_ALLIENCE);

				pNpc->AddScript(GetOwner());	//add script

				GetOwner()->AddNpc(pNpc); //if we dont do this, then many crash when removing script from all npc

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
