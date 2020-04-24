#include "stdafx.h"
#include "SPSNodeAction_PathMove.h"
#include "ControlScriptNodeParam_Number.h"


CSPSNodeAction_PathMove::CSPSNodeAction_PathMove(const char* lpszName)
:CControlScriptNodeAction(SPS_NODE_ACTION_PATH_MOVE, lpszName)
{
	m_pathTblidx = INVALID_TBLIDX;
	m_bRunMode = false;
	m_fPointRadius = 0;
}

CSPSNodeAction_PathMove::~CSPSNodeAction_PathMove()
{

}

bool CSPSNodeAction_PathMove::AddParam(CControlScriptNodeParam_Number* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "tblidx") == 0)
	{
		m_pathTblidx = (TBLIDX)floor(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "run mode") == 0)
	{
		m_bRunMode = pNode->GetValue() != 0.0f;
		return CControlScriptNode::AddParam(pNode);
	}

	if (_stricmp(name, "radius") == 0)
	{
		m_fPointRadius = float(pNode->GetValue());
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CSPSNodeAction_PathMove: [%s] not exist \n", name);

	return false;
}

