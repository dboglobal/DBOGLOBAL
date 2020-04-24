#include "stdafx.h"
#include "WpsScriptAlgoCondition_CCBD_pattern.h"
#include "WpsNodeCondition_CCBD_pattern.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"
#include "WpsAlgoStage.h"
#include "WorldPlayScriptManager.h"
#include "WorldPlayControlFactory.h"
#include "NtlCCBD.h"


CWpsScriptAlgoCondition_CCBD_pattern::CWpsScriptAlgoCondition_CCBD_pattern(CWpsAlgoObject* pObject) :
	CWpsAlgoCondition(pObject, SCRIPTCONTROL_CONDITION_CCBD_PATTERN, "SCRIPTCONTROL_CONDITION_CCBD_PATTERN")
{
	m_scriptnodeID = INVALID_DWORD;
	m_dwTimeLimit = INVALID_DWORD;
}


CWpsScriptAlgoCondition_CCBD_pattern::~CWpsScriptAlgoCondition_CCBD_pattern()
{
}


bool CWpsScriptAlgoCondition_CCBD_pattern::AttachControlScriptNode(CControlScriptNode* pControlScriptNode)
{
	CWpsNodeCondition_CCBD_pattern* pCond = dynamic_cast<CWpsNodeCondition_CCBD_pattern*>(pControlScriptNode);
	if (pCond)
	{
		m_byIndex = pCond->m_byIndex;
		m_titleIdx = pCond->m_titleIdx;
		m_subTitleIdx = pCond->m_subTitleIdx;
		m_bUseTimeLimit = pCond->m_bUseTimeLimit;

		m_scriptnodeID = pCond->GetNodeIndex();

		m_bPacketSent = false;
		m_bTimeLimitStarted = false;
		m_bStartTimeLimit = false;
		m_dwTimeLimitBeginTime = 10000;

		if (m_bUseTimeLimit)
			m_dwTimeLimit = CCBD_TIME_LIMIT_IN_MS; // 4 minutes

		CWpsAlgoStage* pStage = dynamic_cast<CWpsAlgoStage*>(m_pParent);
		if (pStage)
		{
			pStage->AddCCBDPattern(m_byIndex, this);
		}
		else printf("GetOwner()->GetController()->GetCurrentStage() == NULL \n");

		return true;
	}

	return false;
}

void CWpsScriptAlgoCondition_CCBD_pattern::OnEnter()
{
}


int CWpsScriptAlgoCondition_CCBD_pattern::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	//Dont execute the shit here because this function must be called from "CCBD exec pattern"
	return m_status;
}


int CWpsScriptAlgoCondition_CCBD_pattern::Update(DWORD dwTickDiff, float fMultiple)
{
	//executed inside "CCBD exec pattern"

	if (m_bStartTimeLimit == false && m_bUseTimeLimit)
	{
		m_dwTimeLimitBeginTime = UnsignedSafeDecrease<DWORD>(m_dwTimeLimitBeginTime, dwTickDiff);
		if (m_dwTimeLimitBeginTime == 0)
			m_bStartTimeLimit = true;
	}

	if(m_bTimeLimitStarted)
		m_dwTimeLimit = UnsignedSafeDecrease<DWORD>(m_dwTimeLimit, dwTickDiff);

	if (m_bPacketSent == false)
	{
		m_bPacketSent = true;
	//	NTL_PRINT(PRINT_APP, "CWpsScriptAlgoCondition_CCBD_pattern::Update() %u, m_titleIdx %u, m_byIndex %u\n", GetOwner()->GetCCBDStage(), m_titleIdx, m_byIndex);
		CNtlPacket packet(sizeof(sGU_BATTLE_DUNGEON_STATE_UPATE_NFY));
		sGU_BATTLE_DUNGEON_STATE_UPATE_NFY* res = (sGU_BATTLE_DUNGEON_STATE_UPATE_NFY *)packet.GetPacketData();
		res->wOpCode = GU_BATTLE_DUNGEON_STATE_UPATE_NFY;
		res->subTitleTblidx = m_subTitleIdx;
		res->titleTblidx = m_titleIdx;
		res->byStage = GetOwner()->GetCCBDStage();

		if(m_bStartTimeLimit)
			res->dwLimitTime = m_dwTimeLimit;

		GetOwner()->GetWorld()->Broadcast(&packet);
	}

	if (m_bStartTimeLimit && m_bTimeLimitStarted == false)
	{
		CNtlPacket packet2(sizeof(sGU_BATTLE_DUNGEON_LIMIT_TIME_START_NFY));
		sGU_BATTLE_DUNGEON_LIMIT_TIME_START_NFY* res2 = (sGU_BATTLE_DUNGEON_LIMIT_TIME_START_NFY *)packet2.GetPacketData();
		res2->wOpCode = GU_BATTLE_DUNGEON_LIMIT_TIME_START_NFY;
		res2->dwLimitTime = m_dwTimeLimit / 1000; //show in seconds
		GetOwner()->GetWorld()->Broadcast(&packet2);

		m_bTimeLimitStarted = true;
		GetOwner()->SetCCBDTimeLimit(true);
	}

	int status = UpdateSubControlList(dwTickDiff, fMultiple); //Update CONDITIONS. returns completed if EMPTY

	if (status == COMPLETED)
	{
		m_status = UpdateSubControlQueue(dwTickDiff, fMultiple); //update actions

		if (m_status == COMPLETED) //if finish pattern then re-attach
		{
			CControlScriptNode* pPlayScript = g_pWorldPlayScriptManager->GetWorldPlayScriptNode(GetOwner()->GetScriptID(), m_scriptnodeID);
			if (pPlayScript == NULL)
			{
				ERR_LOG(LOG_BOTAI, "can not find 'CWpsScriptAlgoCondition_CCBD_pattern'. script[%u] node index[%u]", GetOwner()->GetScriptID(), m_scriptnodeID);
				m_status = FAILED;
				return m_status;
			}

			if (pPlayScript->GetID() != WPS_NODE_CONDITION_CCBD_PATTERN)
			{
				ERR_LOG(LOG_BOTAI, "CControlScriptNode is not 'WPS_NODE_CONDITION_CCBD_PATTERN'. script[%u] node index[%u]", GetOwner()->GetScriptID(), m_scriptnodeID);
				m_status = FAILED;
				return m_status;
			}

			//	ERR_LOG(LOG_GENERAL, "Script %u RESTART WHILE m_scriptnodeID %u", GetOwner()->GetScriptID(), m_scriptnodeID);
		//	printf("Script %u RESET 'CCBD PATTERN' m_scriptnodeID %u\n", GetOwner()->GetScriptID(), m_scriptnodeID);

			CWpsScriptAlgoCondition_CCBD_pattern* pNewPattern = new CWpsScriptAlgoCondition_CCBD_pattern(GetOwner());
			pNewPattern->SetParent(GetOwner()->GetController()->GetCurrentStage());

			CWorldPlayControlFactory factory(GetOwner());
			if (!pPlayScript->AttachScript(pNewPattern, &factory, true))
			{
				ERR_LOG(LOG_BOTAI, "could not attach script. script[%u] node index[%u]", GetOwner()->GetScriptID(), m_scriptnodeID);
				m_status = FAILED;
			}
		}
		else if (GetOwner()->GetCCBDTimeLimit() && m_status == ACTIVATED)
		{
			if (m_dwTimeLimit == 0) // CCBD failed.
			{
				CNtlPacket packet3(sizeof(sGU_BATTLE_DUNGEON_FAIL_NFY));
				sGU_BATTLE_DUNGEON_FAIL_NFY* res3 = (sGU_BATTLE_DUNGEON_FAIL_NFY *)packet3.GetPacketData();
				res3->wOpCode = GU_BATTLE_DUNGEON_FAIL_NFY;
				GetOwner()->GetWorld()->Broadcast(&packet3);

				GetOwner()->SetCCBDFail(true); //block further ccbd process & tp out in 10 seconds
			}
		}
	}

	return m_status;
}
