#include "stdafx.h"
#include "WPSNodeAction_Switch.h"
#include "ControlScriptNodeParam_String.h"
//#include "ControlStateFactory.h"
//#include "SvrScrVariableMap.h"

CWPSNodeAction_Switch::CWPSNodeAction_Switch(const char* lpszName)
:CControlScriptNodeAction(WPS_NODE_ACTION_SWITCH, lpszName)
{
}

CWPSNodeAction_Switch::~CWPSNodeAction_Switch()
{

}

CWPSNodeAction_Switch::sPARAMETER_INFO* CWPSNodeAction_Switch::GetParameterMap()
{
	return NULL;
}


bool CWPSNodeAction_Switch::AddParam(CControlScriptNodeParam_String* pNode)
{
	const char* name = pNode->GetName();

	if (_stricmp(name, "compare variable") == 0)
	{
		m_strCompareVariable = pNode->GetValue();
		return CControlScriptNode::AddParam(pNode);
	}

	printf("CWPSNodeAction_Switch: [%s] not exist \n", name);
	return false;
}

//bool CWPSNodeAction_Switch::AttachScript(CControlState * pControlState, CControlStateFactory * pControlFactory, bool bAttachCurrent)
//{
//	printf("CWPSNodeAction_Switch::AttachScript \n");
//
//	if (bAttachCurrent)
//	{
//		if (GetCurrentVariableMap() && !pControlState->CreateVariableMap())
//		{
//			printf("CControlScriptNode::AttachScript: return false [if (GetCurrentVariableMap() && !pControlState->CreateVariableMap())] \n");
//			printf("GetNodeIndex() %u pControlState GetControlStateID %u \n", GetNodeIndex(), pControlState->GetControlStateID());
//			return false;
//		}
//
//		pControlState->AttachControlScriptNode(this);
//	}
//
//	if (!pControlState->DoAttachChild())
//	{
//		printf("CControlScriptNode::AttachScript: return true [if (!pParent->DoAttachChild())] \n");
//		return true;
//	}
//
//	CSvrScrVariableMap* pValidVariableMap = GetValidVariableMap();
//	if (!pValidVariableMap)
//	{
//		printf("NULL == pVariableMap \n");
//		return false;
//	}
//
//	int nVal = pValidVariableMap->GetVariable(m_strCompareVariable)->nInteger;
//
//	printf("switch case %u \n", nVal);
//
//
//	for (CControlScriptNode* j = (CControlScriptNode*)m_childList.GetFirst(); //child are only "case"
//		j;
//		j = (CControlScriptNode *)j->GetNext())
//	{
//		DWORD dwConditionLoopCount = j->GetLoopCount();
//
//		if (pControlFactory->CanCreateControlStateLoopCountCheck(j->GetNodeIndex(), dwConditionLoopCount))
//		{
//			CControlState* pCS = pControlFactory->CreateScriptControlState(j->GetID());
//			if (!pCS)
//			{
//				printf("CControlScriptNode::AttachScript: CreateScriptControlState failed \n");
//				return false;
//			}
//
//			pCS->SetLoopCount(dwConditionLoopCount);
//			printf("GetControlStateID %u set GetControlStateID %u as parent \n", pCS->GetControlStateID(), pControlState->GetControlStateID());
//			pCS->SetParent(pControlState);
//			j->AttachScript(pCS, pControlFactory, true);
//
//			if (j->GetNodeType() == NODE_ACTION)
//			{
//				if (!pControlState->AddSubControlQueue(pCS, false))
//				{
//					ERR_LOG(LOG_SCRIPT, "AddSubControlQueue() Fail");
//					SAFE_DELETE(pCS);
//					return false;
//				}
//			}
//			else
//			{
//				ERR_LOG(LOG_SCRIPT, "invalid nodetype [%u]", j->GetNodeType());
//				return false;
//			}
//		}
//	}
//
//	return true;
//}
//
//void CWPSNodeAction_Switch::AttachCase()
//{
//}
