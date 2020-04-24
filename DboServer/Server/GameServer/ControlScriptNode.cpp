#include "stdafx.h"
#include "ControlScriptNode.h"
#include "SvrScrVariableMap.h"
#include "ControlScriptNodeParam_Number.h"
#include "ControlScriptNodeParam_String.h"
#include "ControlStateEx.h"
#include "ControlStateFactory.h"


CControlScriptNode::CControlScriptNode(int ctrlScrNodeID, const char* lpszName, bool bVariableContainer)
{
	char* lpszStr;

	if (lpszName)
		lpszStr = (char*)lpszName;
	else
		lpszStr = "no name";

	m_ctrlScrNodeID = ctrlScrNodeID;
	m_strName.operator=(lpszStr);

	m_dwConditionLoopCount = INVALID_DWORD;

	this->Init();

	if (bVariableContainer == true)
	{
		m_pVariableMap = new CSvrScrVariableMap;

		if (!m_pVariableMap->Create())
		{
			printf("CControlScriptNode: failed to create CSvrScrVariableMap. [%s]\n", this->GetName());
		}
	}
}

CControlScriptNode::~CControlScriptNode()
{

}


bool CControlScriptNode::AttachScript(CControlState* pControlState, CControlStateFactory* pControlFactory, bool bAttachCurrent)
{
	if (bAttachCurrent)
	{
		if (GetCurrentVariableMap() && !pControlState->CreateVariableMap())
		{
			printf("CControlScriptNode::AttachScript: return false [if (GetCurrentVariableMap() && !pControlState->CreateVariableMap())] \n");
			printf("GetNodeIndex() %u pControlState GetControlStateID %u \n", GetNodeIndex(), pControlState->GetControlStateID());
			return false;
		}

		pControlState->AttachControlScriptNode(this);
	}

	if (!pControlState->DoAttachChild())
	{
		printf("CControlScriptNode::AttachScript: return true [if (!pParent->DoAttachChild())] \n");
		return true;
	}

	for (CControlScriptNode* j = (CControlScriptNode*)m_childList.GetFirst();
		j; 
		j = (CControlScriptNode *)j->GetNext())
	{
		DWORD dwConditionLoopCount = j->GetLoopCount();

		if (pControlFactory->CanCreateControlStateLoopCountCheck(j->GetNodeIndex(), dwConditionLoopCount))
		{
			CControlState* pCS = pControlFactory->CreateScriptControlState(j->GetID());
			if (!pCS)
			{
				printf("CControlScriptNode::AttachScript: CreateScriptControlState failed \n");
				return false;
			}

			pCS->SetLoopCount(dwConditionLoopCount);
		//	ERR_LOG(LOG_SCRIPT, "'%s' set '%s' as parent. Script %u\n", pCS->GetControlStateName(), pControlState->GetControlStateName(), j->GetID());
			pCS->SetParent(pControlState);
			j->AttachScript(pCS, pControlFactory, true);

			if (j->GetNodeType() == NODE_ACTION)
			{
				if (!pControlState->AddSubControlQueue(pCS, false))
				{
					ERR_LOG(LOG_SCRIPT, "AddSubControlQueue() Fail");
					SAFE_DELETE(pCS);
					return false;
				}
			}
			else if (j->GetNodeType() == NODE_CONDITION)
			{
				if (!pControlState->AddSubControlList(pCS, false))
				{
					ERR_LOG(LOG_SCRIPT, "AddSubControlList() Fail");
					SAFE_DELETE(pCS);
					return false;
				}
			}
			else
			{
				ERR_LOG(LOG_SCRIPT, "invalid nodetype [%u]", j->GetNodeType());
				return false;
			}
		}
	}

	return true;
}

bool CControlScriptNode::AddChild(CControlScriptNode * pNode)
{
	//printf("GetControlStateID %u set GetControlStateID %u as parent \n", pNode->GetNodeIndex(), GetNodeIndex());
	m_childList.PushBack(pNode);
	pNode->SetParent(this);

	return true;
}

bool CControlScriptNode::AddParam(CControlScriptNodeParam_String* paramstring)
{
	bool bResult;
	const char* lpszName = paramstring->GetName();

	if (this->FindParamterNode(lpszName))
	{
		ERR_LOG(LOG_SCRIPT,"The script node [%s] parameter [%s] is already registered. \n", this->GetName(), lpszName);
		bResult = false;
	}
	else
	{
		m_paramList.PushBack(paramstring);
		paramstring->SetParent(this);

		bResult = true;
	}

	return bResult;
}

bool CControlScriptNode::AddParam(CControlScriptNodeParam_Number* paramnumber)
{
	bool bResult = false;
	const char* lpszName = paramnumber->GetName();

	if (this->FindParamterNode(lpszName))
	{
		printf("The script node [%s] parameter [%s] is already registered. \n", this->GetName(), lpszName);
		bResult = false;
	}
	else
	{
		m_paramList.PushBack(paramnumber);
		paramnumber->SetParent(this);

		bResult = true;
	}

	return bResult;
}


bool CControlScriptNode::Create(bool bVarContainer)
{
	if (bVarContainer)
	{
		CSvrScrVariableMap* pVariableMap = new CSvrScrVariableMap;
		if (pVariableMap->Create())
		{
			m_pVariableMap = pVariableMap;
			return true;
		}
		else
		{
			printf("CControlScriptNode::Create: m_pVariableMap->Create() FAILED ! \n");
			return false;
		}
	}

	return true;
}

void CControlScriptNode::Destroy()
{
	SAFE_DELETE(m_pVariableMap);

	CNtlLinkObject* pLinkObject = NULL;

	for (pLinkObject = m_paramList.GetFirst();
		pLinkObject;
		pLinkObject = m_paramList.GetFirst())
	{
		m_paramList.Remove(pLinkObject);
		SAFE_DELETE(pLinkObject);
	}


	for (pLinkObject = m_childList.GetFirst();
		pLinkObject;
		pLinkObject = m_childList.GetFirst())
	{
		m_childList.Remove(pLinkObject);

		if (pLinkObject)
		{
			SAFE_DELETE(pLinkObject);
		}
	}
}


bool CControlScriptNode::CheckIntegrity()
{
	if (CheckParameterMap())
	{
		if (CheckParameter())
		{
			for (CControlScriptNode* j = (CControlScriptNode *)m_childList.GetFirst();
			j;
			j = (CControlScriptNode *)j->GetNext())
			{
				if (!j->CheckIntegrity())
					return false;
			}

			return true;
		}
	}

	return false;
}


CControlScriptNode::sPARAMETER_INFO* CControlScriptNode::FindParameterInfo(const char* lpszParamName)
{
	CControlScriptNodeParam* j = FindParamterNode(lpszParamName);
	if (j)
	{
		sPARAMETER_INFO* info = j->GetParameterMap();
		
		for (int i = 0;; i++)
		{
			if (!strcmp(info[i].strParamName.c_str(), lpszParamName))
				return &info[i];

			if (info[i].bLastParameter)
				break;
		}
	}

	return NULL;
}


CControlScriptNodeParam* CControlScriptNode::FindParamterNode(const char* lpszParamName)
{
	CControlScriptNode* j = (CControlScriptNode*)m_paramList.GetFirst();
	while (j)
	{
		if (!strcmp(j->GetName(), lpszParamName))
			return (CControlScriptNodeParam*)j;

		j = (CControlScriptNode*)j->GetNext();
	}

	return NULL;
}


CControlScriptNodeParam_String* CControlScriptNode::CreateControlScriptNodeParam(const char* lpszParamName, const char* lpszValue)
{
	CControlScriptNodeParam_String* pParamString = NULL;

	if (!FindParameterInfo(lpszParamName))
	{
		pParamString = new CControlScriptNodeParam_String(lpszParamName, lpszValue);
		if (!pParamString)
		{
			printf("CControlScriptNode::CreateControlScriptNodeParam(Number): Script nodes [%s] parameter [%s] to the object creation failed \n", this->GetName(), lpszParamName);
			pParamString = NULL;
		}
	}
	else
	{
		printf("Script nodes [%s] couldnt find parameter [%s] \n", this->GetName(), lpszParamName);
		pParamString = NULL;
	}

	return pParamString;
}

CControlScriptNodeParam_Number* CControlScriptNode::CreateControlScriptNodeParam(const char* lpszParamName, long double number)
{
	CControlScriptNodeParam_Number* pParamNumber;

	if (!FindParameterInfo(lpszParamName))
	{
		pParamNumber = new CControlScriptNodeParam_Number(lpszParamName, number);
		if (!pParamNumber)
		{
			printf("CControlScriptNode::CreateControlScriptNodeParam(Number): Script nodes [%s] parameter [%s] to the object creation failed \n", this->GetName(), lpszParamName);
			pParamNumber = NULL;
		}
	}
	else
	{
		printf("Script nodes[%s] does not allow the parameter [%s] \n", this->GetName(), lpszParamName);
		pParamNumber = NULL;
	}

	return pParamNumber;
}


DWORD CControlScriptNode::GetChildNodeIndex(int ctrlId)
{
	for (CControlScriptNode* j = (CControlScriptNode *)m_childList.GetFirst();
		j;
		j = (CControlScriptNode *)j->GetNext())
	{
		if (j->GetID() == ctrlId)
			return j->GetNodeIndex();
	}

	return INVALID_DWORD;
}

CControlScriptNode* CControlScriptNode::GetChildNode(int ctrlId)
{
	for (CControlScriptNode* j = (CControlScriptNode *)m_childList.GetFirst();
		j;
		j = (CControlScriptNode *)j->GetNext())
	{
		if (j->GetID() == ctrlId)
			return j;
	}

	return NULL;
}


CSvrScrVariableMap* CControlScriptNode::GetValidVariableMap()
{
	for (CControlScriptNode* j = this; j; j = j->GetParent())
	{
		if (j->GetCurrentVariableMap())
			return j->GetCurrentVariableMap();
	}

	return NULL;
}


sSSD_VARIABLE* CControlScriptNode::GetVariable(std::string& name)
{
	for (CControlScriptNode* j = this; j; j = j->GetParent())
	{
		if (j->GetCurrentVariableMap())
		{
			sSSD_VARIABLE* sVar = j->GetCurrentVariableMap()->GetVariable(name);
			if (sVar)
				return sVar;
		}
	}

	return NULL;
}


eSSD_VARIABLE_RESULT CControlScriptNode::UpdateVariable(std::string& name, sSSD_VARIABLE& sVariable)
{
	//printf("this->GetParent()->GetNodeIndex() %u \n", this->GetParent()->GetNodeIndex()); //for a weird reason this is node index 0.. should be 1 when the cur node is 2

	for (CControlScriptNode* j = this; j; j = j->GetParent())
	{
	//	printf("j->GetNodexIndex() %u j->GetName() %s \n", j->GetNodeIndex(), j->GetName()); //find out why node 2 dont have node 1 as parent..
		if (j->GetCurrentVariableMap())
		{
			
			eSSD_VARIABLE_RESULT eRes = j->GetCurrentVariableMap()->Update(name, sVariable);

			if (eRes != SSD_VARIABLE_RESULT_VARIABLE_NOT_EXIST)
			{
				return eRes;
			}
		}
	}

	return SSD_VARIABLE_RESULT_VARIABLE_NOT_EXIST;
}


const char * s_node_type_name[4] =
{
	"NODE_STATE",
	"NODE_ACTION",
	"NODE_CONDITION",
	"NODE_PARAM",
};
const char* CControlScriptNode::GetNodeTypeName(eNODE_TYPE eNodeType)
{
	const char* result;

	if (eNodeType < MAX_NODE_TYPE)
		result = s_node_type_name[eNodeType];
	else
		result = "Not Defined Script Node Type";

	return result;
}

const char * s_param_type_name[2] =
{
	"PARAM_NUMBER",
	"PARAM_STRING",
};
const char* CControlScriptNode::GetParamTypeName(ePARAM_TYPE eParamType)
{
	const char* result;

	if (eParamType < MAX_PARAM_TYPE)
		result = s_param_type_name[eParamType];
	else
		result = "Not Defined Script Parameter Type";

	return result;
}


void CControlScriptNode::Init()
{
	this->m_pScriptManager = NULL;
	this->m_pScript = NULL;
	this->m_pParent = NULL;
	this->m_bCompleted = false;
	this->m_dwNodeIndex = INVALID_DWORD;
	this->m_pVariableMap = NULL;
}


bool CControlScriptNode::CheckParameterMap()
{
	//CURRENTLY DISABLED BCAUSE NOT ALL NODE HAVE FUNCTION GetParameterMap TODO !!!
	return true;

	/*for (CControlScriptNode* j = (CControlScriptNode *)m_paramList.GetFirst();
		j;
		j = (CControlScriptNode *)j->GetNext())
	{
		CControlScriptNodeParam* pParamNode = FindParamterNode(j->GetName());
		if (pParamNode)
		{
			printf("CControlScriptNode::CheckParameterMap() %s \n", j->GetName());
			if (pParamNode->GetParameterMap()->eParamType != j->GetParameterMap()->eParamType)
			{
				ERR_LOG(LOG_SCRIPT, "Script node [%s] type [%s] XXXXX [%s] XXXXX [%s]", this->GetName(), j->GetName(), pParamNode->GetParamTypeName(pParamNode->GetParameterMap()->eParamType), j->GetParamTypeName(j->GetParameterMap()->eParamType));
				return false;
			}
		}
		else
		{
			ERR_LOG(LOG_SCRIPT, "Parameter in the script node [%s] [%s] is necessary.", this->GetName(), j->GetName());
			return false;
		}
	}



	for (CControlScriptNode* k = (CControlScriptNode *)m_paramList.GetFirst();
		;
		k = (CControlScriptNode *)k->GetNext())
	{
		if (!k)
			return true;

		if (!FindParameterInfo(k->GetName()))
			break;
	}

	return false;*/
}
