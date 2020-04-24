#include "stdafx.h"
#include "AISNodeAction_AddMob.h"



CAISNodeAction_AddMob::CAISNodeAction_AddMob(const char* lpszName)
:CControlScriptNodeAction(AIS_NODE_ACTION_ADD_MOB, lpszName)
{
	Init();
}

CAISNodeAction_AddMob::~CAISNodeAction_AddMob()
{

}

CControlScriptNode::sPARAMETER_INFO* CAISNodeAction_AddMob::GetParameterMap()
{
	return NULL;
}


bool CAISNodeAction_AddMob::AddParam(CControlScriptNodeParam_Number* pNode)
{
	return CControlScriptNode::AddParam(pNode);
}

bool CAISNodeAction_AddMob::AddParam(CControlScriptNodeParam_String* pNode)
{
	return CControlScriptNode::AddParam(pNode);
}


bool CAISNodeAction_AddMob::CheckParameter()
{
	return true;
}

void CAISNodeAction_AddMob::Init()
{
	m_mobTblidx = INVALID_TBLIDX;
	m_byPartyNumber = INVALID_BYTE;
	m_eAddMobType = INVALID_ADDMOBTYPE;
}




