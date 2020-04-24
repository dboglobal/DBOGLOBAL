#include "stdafx.h"
#include "ScriptAlgoBase.h"
#include "ScriptAlgoObjectManager.h"
#include "CPlayer.h"


CWpsAlgoObject::CWpsAlgoObject(TBLIDX tblidx, sWORLDPLAY_TBLDAT* pWorldPlayTbldat, DWORD dwHandle) :
	CScriptAlgoObject(tblidx, dwHandle, SSD_SCRIPT_TYPE_WPS)
{
	m_pWorldPlayTbldat = pWorldPlayTbldat;

	Init();
}

CWpsAlgoObject::~CWpsAlgoObject()
{
}


void CWpsAlgoObject::Init()
{
	m_byCCBDStage = 1;
	m_bCCBDTimeLimit = false;
	m_bCCBDFail = false;
	m_byCCBDStartStage = m_byCCBDStage;
}


void CWpsAlgoObject::Start()
{
	m_scriptAlgoController.ChangeControlStage_Stage(0, SSD_SCRIPT_TYPE_WPS);
	
	CScriptAlgoObject::Start();
}

void CWpsAlgoObject::Stop(bool bEnterFailStage/* = false*/, bool bRemoveFromWorld/* = true*/)
{
	if (bEnterFailStage)
	{
		GetController()->ChangeControlStage_Failed();
		return;
	}

	CScriptAlgoObject::Stop(bEnterFailStage, bRemoveFromWorld);
}


int CWpsAlgoObject::OnUpdate(DWORD dwTickDiff)
{
	int nStatus = CScriptAlgoObject::OnUpdate(dwTickDiff);

	if (nStatus == CComplexState::FAILED || nStatus == CComplexState::SYSTEMERROR)
	{
	//	printf("CWpsAlgoObject::OnUpdate STOP! reason SYSTEMERROR\n");
		GetController()->ChangeControlStage_Failed();
	}

	return nStatus;
}


bool CWpsAlgoObject::NotifyPlayerLeave(HOBJECT hPlayer, bool bRemoveScriptFromPlayer/* = true*/)
{
	if (CScriptAlgoObject::NotifyPlayerLeave(hPlayer, bRemoveScriptFromPlayer) == false)
		return false;

	if (m_setPlayers.size() == 0 
		&& (GetExecuterType() == WORLDPLAY_EXECUTER_TS
		|| GetExecuterType() == WORLDPLAY_EXECUTER_ITEM_USE_BY_LOCATION)) //if no players anymore connected then stop the script. Does not USE for scripts hosted on world.
	{
	//	printf("stop script \n");
		Stop(true);
	}

	return true;
}