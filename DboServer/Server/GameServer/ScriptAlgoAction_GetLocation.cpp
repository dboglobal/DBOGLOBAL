#include "stdafx.h"
#include "ScriptAlgoAction_GetLocation.h"
#include "WPSNodeAction_GetLocation.h"
#include "Monster.h"


CWpsAlgoAction_GetLocation::CWpsAlgoAction_GetLocation(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_GET_LOCATION, "SCRIPTCONTROL_ACTION_GET_LOCATION")
{

}


CWpsAlgoAction_GetLocation::~CWpsAlgoAction_GetLocation()
{
}


bool CWpsAlgoAction_GetLocation::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWPSNodeAction_GetLocation* pAction = dynamic_cast<CWPSNodeAction_GetLocation*>(pControlScriptNode);
	if (pAction)
	{
		m_eTargetType = pAction->m_eTargetType;
		m_dwTargetIndex = pAction->m_dwTargetIndex;

		m_strLoc = pAction->m_strLoc;
		m_strDir = pAction->m_strDir;

		m_cDefaultLoc = pAction->m_cDefaultLoc;
		m_cDefaultDir = pAction->m_cDefaultDir;

		return true;
	}

	ERR_LOG(LOG_BOTAI, "fail : Can't dynamic_cast from CControlScriptNode[%X] to CWPSNodeAction_GetLocation", pControlScriptNode);
	return false;
}

int CWpsAlgoAction_GetLocation::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	sSSD_VARIABLE* pVarLoc = GetVariable(m_strLoc);
	sSSD_VARIABLE* pVarDir = GetVariable(m_strDir);

	pVarLoc->operator=(m_cDefaultLoc);
	pVarDir->operator=(m_cDefaultDir);

	if (m_eTargetType == OBJTYPE_NPC)
	{
		CNpc* pNpc = GetOwner()->FindNpc(m_dwTargetIndex);

		if (pNpc == NULL && GetOwner()->GetWorld())
		{
			pNpc = GetOwner()->GetWorld()->FindNpc(m_dwTargetIndex);
		}

		if (pNpc == NULL || !pNpc->IsInitialized()) //did we already remove the npc? 
		{
			m_status = COMPLETED;
			return m_status;
		}

		pVarLoc->operator=(pNpc->GetCurLoc());
		pVarDir->operator=(pNpc->GetCurDir());
	}
	else
	{
		DWORD dwCount = GetOwner()->GetSpawnGroupCount(m_dwTargetIndex);
		HOBJECT hMonster = GetOwner()->GetSpawnGroupFirst(m_dwTargetIndex);

		for (DWORD i = 0; i < dwCount; i++)
		{
			CMonster* pMob = g_pObjectManager->GetMob(hMonster);
			if (pMob && pMob->IsInitialized())
			{
				pVarLoc->operator=(pMob->GetCurLoc());
				pVarDir->operator=(pMob->GetCurDir());

				break;
			}

			hMonster = GetOwner()->GetSpawnGroupNext(m_dwTargetIndex);
		}
	}

	//printf("getlocation: %f %f %f \n", pVarLoc->sVector.x, pVarLoc->sVector.y, pVarLoc->sVector.z);

	m_status = COMPLETED;
	return m_status;
}
