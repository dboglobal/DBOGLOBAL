#include "stdafx.h"
#include "ScriptAlgoAction_WpsFail.h"

CWpsAlgoAction_WpsFail::CWpsAlgoAction_WpsFail(CWpsAlgoObject* pObject) :
CScriptAlgoAction(pObject, SCRIPTCONTROL_ACTION_WPS_FAIL, "SCRIPTCONTROL_ACTION_WPS_FAIL")
{

}


CWpsAlgoAction_WpsFail::~CWpsAlgoAction_WpsFail()
{
}


int CWpsAlgoAction_WpsFail::OnUpdate(DWORD dwTickDiff, float fMultiple)
{
	GetOwner()->GetController()->ChangeControlStage_Failed();
	m_status = CHANGED;
	return CHANGED;
}
