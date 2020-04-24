#include "stdafx.h"
#include "AIScript.h"

//lua
#include "NtlLuaState.h"

#include "ServerAIScript.h"

#include "AISNodeState.h"
#include "AIScriptManager.h"



CNtlLuaState *g_pAISLuaState = NULL;

CServerAIScript* s_pCurScript = NULL;
CControlScriptNode* s_pCurNode = NULL;



CNtlLuaState* GetAISLuaState(void)
{
	return g_pAISLuaState;
}


void LuaScript_ErrorHandler_AI(const char *pError)
{
	if (pError == NULL)
		return;

	FILE *fp = NULL;

	if (fopen_s(&fp, "AiScriptLog.txt", "a+") != 0)
		return;

	fprintf(fp, pError);
	fprintf(fp, "\n");

	fclose(fp);
}


void AISLua_CreateState(void)
{
	if (g_pAISLuaState)
	{
		delete g_pAISLuaState;
		g_pAISLuaState = NULL;
	}

	g_pAISLuaState = new CNtlLuaState;
	g_pAISLuaState->LinkErrorHandler(LuaScript_ErrorHandler_AI);


	//add functions
	g_pAISLuaState->AddFunction("Scene", luabind_AIScript_Scene);
	g_pAISLuaState->AddFunction("State", luabind_AIScript_State);
	g_pAISLuaState->AddFunction("Action", luabind_AIScript_Action);
	g_pAISLuaState->AddFunction("Condition", luabind_AIScript_Condition);
	g_pAISLuaState->AddFunction("Param", luabind_AIScript_Param);
	g_pAISLuaState->AddFunction("End", luabind_AIScript_End);
}


void AISLua_Setup(void)
{
	AISLua_CreateState();
}


void AISLua_Release(void)
{
	if (g_pAISLuaState)
	{
		delete g_pAISLuaState;
		g_pAISLuaState = NULL;
	}
}


int luabind_AIScript_Scene(lua_State *L)
{
	if (!g_pAISLuaState->IsNumberArgument(1))
	{
		printf("luabind_AIScript_Scene: number 1 check fail \n");
		return FALSE;
	}
	if (!g_pAISLuaState->IsStringArgument(2))
	{
		printf("luabind_AIScript_Scene: string 2 check fail \n");
		return FALSE;
	}
	if (s_pCurScript == NULL)
	{
		printf("luabind_AIScript_Scene: s_pCurScript is null \n");
		return FALSE;
	}
	if (s_pCurNode)
	{
		printf("luabind_AIScript_Scene: Scene already available. [%s][%s] \n", s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName());
		return FALSE;
	}
	
	DWORD dwSceneNumber = (DWORD)g_pAISLuaState->GetNumberArgument(1, INVALID_DWORD);
	if (dwSceneNumber == INVALID_DWORD)
	{
		printf("luabind_AIScript_Scene: Cant find scene number [%s][%s] \n", s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName());
		return FALSE;
	}

	const char* lpszSceneName = g_pAISLuaState->GetStringArgument(2, NULL);
	if (lpszSceneName == NULL)
	{
		printf("luabind_AIScript_Scene: Cant find scene name [%s][%s] \n", s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName());
		return FALSE;
	}

	CAISNodeScene* pScene = new CAISNodeScene(dwSceneNumber, lpszSceneName);

	if (s_pCurScript->AddScene(dwSceneNumber, pScene))
	{
		if (s_pCurScript->AddScriptNode(pScene))
		{
			s_pCurNode = pScene;
			return TRUE;
		}
	}
	else printf("luabind_AIScript_Scene: Add scene fail. Scene number [%d]. [%s][%s] \n", dwSceneNumber, s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName());

	if (pScene)
		delete pScene;

	return FALSE;
}


int luabind_AIScript_State(lua_State *L)
{
	if (!g_pAISLuaState->IsStringArgument(1))
	{
		printf("luabind_AIScript_State: string 1 check fail \n");
		return FALSE;
	}
	if (s_pCurScript == NULL)
	{
		printf("luabind_AIScript_State: s_pCurScript is null \n");
		return FALSE;
	}
	if (s_pCurNode == NULL)
	{
		printf("luabind_AIScript_State: s_pCurNode is null \n");
		return FALSE;
	}

	const char* lpszStateName = g_pAISLuaState->GetStringArgument(1, NULL);
	if (lpszStateName == NULL)
	{
		printf("luabind_AIScript_State: Cant find state name [%s][%s] \n", s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName());
		return FALSE;
	}

	CAISNodeState* pState = (CAISNodeState*)g_pAiScriptManager->GetControlScriptNodeFactory()->CreateControlNodeAction(lpszStateName);
	if (pState)
	{
		if (dynamic_cast<CAISNodeScene*>(s_pCurNode)->AddState(pState))
		{
			if (s_pCurNode->AddChild(pState))
			{
				if (s_pCurScript->AddScriptNode(pState))
				{
					s_pCurNode = (CControlScriptNode *)pState;
					return TRUE;
				}
			}
		}
	}
	else printf("luabind_AIScript_State: CreateControlNodeAction failed [%s][%s] \n", s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName());


	if (pState)
		delete pState;

	return FALSE;
}


int luabind_AIScript_Action(lua_State *L)
{
	if (!g_pAISLuaState->IsStringArgument(1))
	{
		printf("luabind_AIScript_Action: string 1 check fail \n");
		return FALSE;
	}
	if (s_pCurScript == NULL)
	{
		printf("luabind_AIScript_Action: s_pCurScript is null \n");
		return FALSE;
	}
	if (s_pCurNode == NULL)
	{
		printf("luabind_AIScript_Action: s_pCurNode is null \n");
		return FALSE;
	}

	const char* lpszAction = g_pAISLuaState->GetStringArgument(1, NULL);
	if (lpszAction == NULL)
	{
		printf("luabind_AIScript_Action: Cant find state name [%s][%s] \n", s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName());
		return FALSE;
	}

	CControlScriptNode* pScriptNode = (CControlScriptNode*)g_pAiScriptManager->GetControlScriptNodeFactory()->CreateControlNodeAction(lpszAction);
	if (pScriptNode)
	{
		if (s_pCurNode->AddChild(pScriptNode))
		{
			if (s_pCurScript->AddScriptNode(pScriptNode))
			{
				s_pCurNode = pScriptNode;
				return TRUE;
			}
		}
	}
	else printf("luabind_AIScript_Action: CreateControlNodeAction failed [%s][%s] \n", s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName());

	if (pScriptNode)
		delete pScriptNode;

	return FALSE;
}


int luabind_AIScript_Condition(lua_State *L)
{
	if (!g_pAISLuaState->IsStringArgument(1))
	{
		printf("luabind_AIScript_Condition: string 1 check fail \n");
		return FALSE;
	}
	if (s_pCurScript == NULL)
	{
		printf("luabind_AIScript_Condition: s_pCurScript is null \n");
		return FALSE;
	}
	if (s_pCurNode == NULL)
	{
		printf("luabind_AIScript_Condition: s_pCurNode is null \n");
		return FALSE;
	}

	const char* lpszCondition = g_pAISLuaState->GetStringArgument(1, NULL);
	if (lpszCondition == NULL)
	{
		printf("luabind_AIScript_Condition: Cant read condition [%s][%s] \n", s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName());
		return FALSE;
	}

	CControlScriptNode* pScriptNode = (CControlScriptNode*)g_pAiScriptManager->GetControlScriptNodeFactory()->CreateControlNodeCondition(lpszCondition);
	if (pScriptNode)
	{
		DWORD dwConditionLoopCount = INVALID_DWORD;
		const char* lpszConditionLoopCount = g_pAISLuaState->GetStringArgument(2, NULL);

		if (lpszConditionLoopCount)
			dwConditionLoopCount = std::atoi(lpszConditionLoopCount);

		
		pScriptNode->SetLoopCount(dwConditionLoopCount);

		if (s_pCurNode->AddChild(pScriptNode))
		{
			if (s_pCurScript->AddScriptNode(pScriptNode))
			{
				s_pCurNode = pScriptNode;
				return TRUE;
			}
		}
	}
	else printf("luabind_AIScript_Condition: CreateControlNodeCondition failed [%s][%s] \n", s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName());

	if (pScriptNode)
		delete pScriptNode;

	return FALSE;
}


int luabind_AIScript_Param(lua_State *L)
{
	if (s_pCurScript == NULL)
	{
		printf("luabind_AIScript_Param: s_pCurScript is null \n");
		return FALSE;
	}
	if (s_pCurNode == NULL)
	{
		printf("luabind_AIScript_Param: s_pCurNode is null \n");
		return FALSE;
	}

	const char* lpszParamName = g_pAISLuaState->GetStringArgument(1, NULL);
	if (lpszParamName == NULL)
	{
		printf("luabind_AIScript_Param: Cant read param [%s][%s] \n", s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName());
		return FALSE;
	}

	if (g_pAISLuaState->IsNumberArgument(2))
	{
		long double ldParamValue = g_pAISLuaState->GetNumberArgument(2);

		CControlScriptNodeParam_Number* pParamNumber = s_pCurNode->CreateControlScriptNodeParam(lpszParamName, ldParamValue);
		if (!pParamNumber)
		{
			printf("%s\\%s[%d] : The script node [%s] parameter [%s] is a STRING type. \n", s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName(), s_pCurScript->GetScriptNodeCount()-1, s_pCurNode->GetName(), lpszParamName);
			return FALSE;
		}

		if (s_pCurNode->AddParam(pParamNumber))
		{
			return TRUE;
		}
		else
		{
			printf("%s\\%s[%d] : false == s_pCurNode->AddParam( %f )\n", s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName(), s_pCurScript->GetScriptNodeCount() - 1, ldParamValue);
			return FALSE;
		}
	}
	else if (g_pAISLuaState->IsStringArgument(2))
	{
		const char* lpszValue = g_pAISLuaState->GetStringArgument(2);

		CControlScriptNodeParam_String* pParamString = s_pCurNode->CreateControlScriptNodeParam(lpszParamName, lpszValue);
		if (!pParamString)
		{
			printf("%s\\%s[%d] : The script node [%s] parameter [%s] is not available \n", s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName(), s_pCurScript->GetScriptNodeCount()-1, s_pCurNode->GetName(), lpszParamName);
			return FALSE;
		}

		if (s_pCurNode->AddParam(pParamString))
		{
			return TRUE;
		}
		else
		{
			printf("%s\\%s[%d] : false == s_pCurNode->AddParam( %s )\n", s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName(), s_pCurScript->GetScriptNodeCount() - 1, lpszValue);
			return FALSE;
		}
	}
	else
	{
		printf("%s\\%s[%d] : The script node [%s] parameter [%s] is not valid. No number nor string \n", s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName(), s_pCurScript->GetScriptNodeCount() - 1, s_pCurNode->GetName(), lpszParamName);
	}

	return FALSE;
}


int luabind_AIScript_End(lua_State *L)
{
	if (s_pCurScript == NULL)
	{
		printf("luabind_AIScript_End: s_pCurScript is null \n");
		return FALSE;
	}
	if (s_pCurNode == NULL)
	{
		printf("luabind_AIScript_End: s_pCurNode is null \n");
		return FALSE;
	}
	

	if (s_pCurNode->IsComplete())
	{
		printf("luabind_AIScript_End: true == s_pCurNode->IsComplete() [%s][%s] \n", s_pCurScript->GetScriptManager()->GetPath(), s_pCurScript->GetFileName());
		return FALSE;
	}

	CControlScriptNode::eNODE_TYPE eNodeType = s_pCurNode->GetNodeType();
	if (eNodeType == CControlScriptNode::NODE_STATE)
	{
		s_pCurNode->SetComplete(true);
		s_pCurNode = NULL;
		return 0;
	}
	else if (eNodeType > CControlScriptNode::NODE_CONDITION)
	{
		printf("luabind_AIScript_End: eNodeType > CControlScriptNode::NODE_CONDITION \n");
		return 0xFFFFFFFF;
	}

	if (s_pCurNode->GetParent())
	{
		s_pCurNode->SetComplete(true);
		s_pCurNode = s_pCurNode->GetParent();
		return 0;
	}
	else
	{
		printf("luabind_AIScript_End: s_pCurNodeSPS->GetParent() == NULL \n");
		return 0xFFFFFFFF;
	}

	return 0xFFFFFFFF;
}