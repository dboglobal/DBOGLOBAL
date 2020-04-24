#include "stdafx.h"
#include "AISNodeState.h"



CAISNodeState::CAISNodeState(int nStateId, const char* lpszStateName)
: m_nStateId(nStateId), CControlScriptNodeAction(AIS_NODE_STATE, lpszStateName)
{
}

CAISNodeState::~CAISNodeState()
{

}