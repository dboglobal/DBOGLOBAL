#include "stdafx.h"
#include "TqsAlgoAction_AddMob.h"
#include "TQSNodeAction_AddMob.h"
#include "ObjectManager.h"
#include "TableContainerManager.h"
#include "MobTable.h"
#include "Monster.h"


CTqsAlgoAction_AddMob::CTqsAlgoAction_AddMob(CTqsAlgoObject* pObject) :
	CTqsAlgoAction_Base(pObject, TQS_ALGOCONTROLID_ACTION_ADDMOB, "TQS_ALGOCONTROLID_ACTION_ADDMOB")
{
}


CTqsAlgoAction_AddMob::~CTqsAlgoAction_AddMob()
{
}


bool CTqsAlgoAction_AddMob::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CTQSNodeAction_AddMob* pAction = dynamic_cast<CTQSNodeAction_AddMob*>(pControlScriptNode);
	if (pAction)
	{
		m_mobTblidx = pAction->m_mobTblidx;
		m_vLoc = pAction->m_vLoc;
		m_vDir = pAction->m_vDir;

		m_bRespawn = pAction->m_bRespawn;

		m_dwPlayScriptNumber = pAction->m_dwPlayScriptNumber;
		m_dwPlayScriptScene = pAction->m_dwPlayScriptScene;
		m_bySpawnFuncFlag = pAction->m_bySpawnFuncFlag;
		m_partyId = pAction->m_partyId;
		m_bPartyLeader = pAction->m_bPartyLeader;

		return true;
	}

	ERR_LOG(LOG_SCRIPT, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CTQSNodeAction_AddMob", pControlScriptNode);
	return false;
}

int CTqsAlgoAction_AddMob::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	sMOB_DATA data;
	InitMobData(data);

	data.spawnGroupId = INVALID_SPAWNGROUPID;

	if (GetOwner()->GetWorld())
	{
		data.worldID = GetOwner()->GetWorld()->GetID();
		data.worldtblidx = GetOwner()->GetWorld()->GetIdx();
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "fail : CTqsAlgoAction_AddMob does not have WORLD! ");
		return SYSTEMERROR;
	}

	data.tblidx = m_mobTblidx;

	m_vLoc.CopyTo(data.vCurLoc);
	m_vLoc.CopyTo(data.vSpawnLoc);
	m_vDir.CopyTo(data.vCurDir);
	m_vDir.CopyTo(data.vSpawnDir);
	data.wSpawnTime = 1000;
	data.bySpawnFuncFlag = m_bySpawnFuncFlag;
	data.sScriptData.playScript = (TBLIDX)m_dwPlayScriptNumber;
	data.sScriptData.playScriptScene = (TBLIDX)m_dwPlayScriptScene;
	data.partyID = m_partyId;
	data.bPartyLeader = m_bPartyLeader;
	data.actionpatternTblIdx = 1;
	data.byMoveType = SPAWN_MOVE_WANDER;


	sMOB_TBLDAT* pTbldat = (sMOB_TBLDAT*)g_pTableContainer->GetMobTable()->FindData(m_mobTblidx);
	if (pTbldat)
	{
		CMonster* pMob = (CMonster*)g_pObjectManager->CreateCharacter(OBJTYPE_MOB);
		if (pMob)
		{
			if (pMob->CreateDataAndSpawn(data, pTbldat))
			{
				pMob->AddScript(GetOwner());	//add script
				pMob->SetStandAlone(false);

				m_status = COMPLETED;
			}
			else m_status = FAILED;
		}
		else m_status = FAILED;
	}
	else
	{
		ERR_LOG(LOG_SCRIPT, "Could not find MOB-TBLDAT. Tblidx %u", m_mobTblidx);
		m_status = SYSTEMERROR;
	}

	return m_status;
}
