#include "stdafx.h"
#include "TqsAlgoObject.h"
#include "ControlScriptGlobal.h"


CTqsAlgoObject::CTqsAlgoObject(TBLIDX tblidx, sTIMEQUEST_TBLDAT* pTimeQuestTbldat, DWORD dwHandle) :
	CScriptAlgoObject(tblidx, dwHandle, SSD_SCRIPT_TYPE_TQS)
{
	m_pTimeQuestTbldat = pTimeQuestTbldat;

	Init();
}

CTqsAlgoObject::~CTqsAlgoObject()
{
}


void CTqsAlgoObject::Init()
{
	m_pTimeQuest = NULL;
}


void CTqsAlgoObject::Start()
{
	m_scriptAlgoController.ChangeControlStage_Begin();

	CScriptAlgoObject::Start();
}


void CTqsAlgoObject::Stop(bool bEnterFailStage/* = false*/, bool bRemoveFromWorld/* = true*/)
{
	CScriptAlgoObject::Stop(bEnterFailStage, bRemoveFromWorld);
}



