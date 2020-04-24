#include "stdafx.h"
#include "TQSScript.h"

//lua
#include "NtlLuaState.h"

#include "ServerTimeQuestScript.h"
#include "TimeQuestScriptManager.h"

#include "TQSNodeGameBegin.h"
#include "TQSNodeGameEnd.h"



CNtlLuaState *g_pTQSLuaState = NULL;


CServerTimeQuestScript* s_pCurScriptTQS = NULL;
CControlScriptNode* s_pCurNodeTQS = NULL;


CNtlLuaState* GetTQSLuaState(void)
{
	return g_pTQSLuaState;
}


void LuaScript_ErrorHandler_TQS(const char *pError)
{
	if (pError == NULL)
		return;

	FILE *fp = NULL;

	if (fopen_s(&fp, "TimeQuestScriptLog.txt", "a+") != 0)
		return;

	fprintf(fp, pError);
	fprintf(fp, "\n");

	fclose(fp);
}


void TQSLua_CreateState(void)
{
	if (g_pTQSLuaState)
	{
		delete g_pTQSLuaState;
		g_pTQSLuaState = NULL;
	}

	g_pTQSLuaState = new CNtlLuaState;
	g_pTQSLuaState->LinkErrorHandler(LuaScript_ErrorHandler_TQS);


	//add functions
	g_pTQSLuaState->AddFunction("GameBegin", luabind_TimeQuestScript_GameBegin);
	g_pTQSLuaState->AddFunction("GameEnd", luabind_TimeQuestScript_GameEnd);
	g_pTQSLuaState->AddFunction("GameStage", luabind_TimeQuestScript_GameStage);
	g_pTQSLuaState->AddFunction("Action", luabind_TimeQuestScript_Action);
	g_pTQSLuaState->AddFunction("Condition", luabind_TimeQuestScript_Condition);
	g_pTQSLuaState->AddFunction("Param", luabind_TimeQuestScript_Param);
	g_pTQSLuaState->AddFunction("End", luabind_TimeQuestScript_End);
}


void TQSLua_Setup(void)
{
	TQSLua_CreateState();
}


void TQSLua_Release(void)
{
	if (g_pTQSLuaState)
	{
		delete g_pTQSLuaState;
		g_pTQSLuaState = NULL;
	}
}


int luabind_TimeQuestScript_GameBegin(lua_State *L)
{
	if (s_pCurScriptTQS == NULL)
	{
		printf("luabind_TimeQuestScript_GameBegin: s_pCurScriptTQS is null \n");
		return FALSE;
	}
	if (s_pCurNodeTQS)
	{
		printf("luabind_TimeQuestScript_GameBegin: node is not finished. NodeCount %u [%s][%s] \n", s_pCurScriptTQS->GetScriptNodeCount(), s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName());
		return FALSE;
	}

	CTQSNodeGameBegin* pStage = new CTQSNodeGameBegin;

	if (s_pCurScriptTQS->AddStage(0, pStage))
	{
		if (s_pCurScriptTQS->AddScriptNode(pStage))
		{
			pStage->Create();

			s_pCurNodeTQS = pStage;
			return TRUE;
		}
	}
	else printf("luabind_TimeQuestScript_GameBegin: Add gamebegin fail. [%s][%s] \n", s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName());

	if (pStage)
		delete pStage;

	return FALSE;
}


int luabind_TimeQuestScript_GameEnd(lua_State *L)
{
	//Game end = last stage

	if (s_pCurScriptTQS == NULL)
	{
		printf("luabind_TimeQuestScript_GameEnd: s_pCurScript is null \n");
		return FALSE;
	}
	if (s_pCurNodeTQS)
	{
		printf("luabind_TimeQuestScript_GameEnd: s_pCurNode is not null. Previous node not end \n");
		return FALSE;
	}


	CTQSNodeGameEnd* pStage = new CTQSNodeGameEnd;

	if (s_pCurScriptTQS->AddStage(INVALID_BYTE, pStage))
	{
		if (s_pCurScriptTQS->AddScriptNode(pStage))
		{
			pStage->Create();

			s_pCurNodeTQS = pStage;
			return TRUE;
		}
	}
	else printf("luabind_TimeQuestScript_GameBegin: Add gameend fail. [%s][%s] \n", s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName());

	if (pStage)
		delete pStage;

	return FALSE;
}


int luabind_TimeQuestScript_GameStage(lua_State *L)
{
	if (s_pCurScriptTQS == NULL)
	{
		printf("luabind_TimeQuestScript_GameStage: s_pCurScript is null \n");
		return FALSE;
	}
	if (s_pCurNodeTQS)
	{
		printf("luabind_TimeQuestScript_GameStage: Stage %u already available. [%s][%s] \n", s_pCurScriptTQS->GetScriptNodeCount(), s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName());
		return FALSE;
	}

	DWORD dwStageNumber = (DWORD)g_pTQSLuaState->GetNumberArgument(1, INVALID_DWORD);
	if (dwStageNumber == INVALID_DWORD)
	{
		printf("luabind_TimeQuestScript_GameStage: Cant find stage number [%s][%s] \n", s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName());
		return FALSE;
	}

	CTQSNodeGameStage* pStage = new CTQSNodeGameStage(dwStageNumber);

	if (s_pCurScriptTQS->AddStage(dwStageNumber, pStage))
	{
		if (s_pCurScriptTQS->AddScriptNode(pStage))
		{
			pStage->Create();

			s_pCurNodeTQS = pStage;
			return TRUE;
		}
	}
	else printf("luabind_TimeQuestScript_GameStage: Add gameend fail. [%s][%s] \n", s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName());

	if (pStage)
		delete pStage;

	return FALSE;
}


int luabind_TimeQuestScript_Action(lua_State *L)
{
	if (!g_pTQSLuaState->IsStringArgument(1))
	{
		printf("luabind_TimeQuestScript_Action: string 1 check fail \n");
		return FALSE;
	}
	if (s_pCurScriptTQS == NULL)
	{
		printf("luabind_TimeQuestScript_Action: s_pCurScriptTQS is null \n");
		return FALSE;
	}
	if (s_pCurNodeTQS == NULL)
	{
		printf("luabind_TimeQuestScript_Action: s_pCurNodeTQS is null \n");
		return FALSE;
	}

	const char* lpszAction = g_pTQSLuaState->GetStringArgument(1, NULL);
	if (lpszAction == NULL)
	{
		printf("luabind_TimeQuestScript_Action: Cant find state name [%s][%s] \n", s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName());
		return FALSE;
	}

	CControlScriptNode* pScriptNode = (CControlScriptNode*)g_pTimeQuestScriptManager->GetControlScriptNodeFactory()->CreateControlNodeAction(lpszAction);
	if (pScriptNode)
	{
		if (s_pCurNodeTQS->AddChild(pScriptNode))
		{
			if (s_pCurScriptTQS->AddScriptNode(pScriptNode))
			{
				s_pCurNodeTQS = pScriptNode;
				return TRUE;
			}
		}
	}
	else printf("luabind_TimeQuestScript_Action: CreateControlNodeAction failed [%s][%s] \n", s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName());

	if (pScriptNode)
		delete pScriptNode;

	return FALSE;
}

int luabind_TimeQuestScript_Condition(lua_State *L)
{
	if (!g_pTQSLuaState->IsStringArgument(1))
	{
		printf("luabind_TimeQuestScript_Condition: string 1 check fail \n");
		return FALSE;
	}
	if (s_pCurScriptTQS == NULL)
	{
		printf("luabind_TimeQuestScript_Condition: s_pCurScript is null \n");
		return FALSE;
	}
	if (s_pCurNodeTQS == NULL)
	{
		printf("luabind_TimeQuestScript_Condition: s_pCurNode is null \n");
		return FALSE;
	}

	const char* lpszCondition = g_pTQSLuaState->GetStringArgument(1, NULL);
	if (lpszCondition == NULL)
	{
		printf("luabind_TimeQuestScript_Condition: Cant read condition [%s][%s] \n", s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName());
		return FALSE;
	}

	CControlScriptNode* pScriptNode = (CControlScriptNode*)g_pTimeQuestScriptManager->GetControlScriptNodeFactory()->CreateControlNodeCondition(lpszCondition);
	if (pScriptNode)
	{
		if (s_pCurNodeTQS->AddChild(pScriptNode))
		{
			if (s_pCurScriptTQS->AddScriptNode(pScriptNode))
			{
				s_pCurNodeTQS = pScriptNode;
				return TRUE;
			}
		}
	}
	else printf("luabind_TimeQuestScript_Condition: CreateControlNodeCondition failed [%s][%s] \n", s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName());

	if (pScriptNode)
		delete pScriptNode;

	return FALSE;
}

int luabind_TimeQuestScript_Param(lua_State *L)
{
	if (s_pCurScriptTQS == NULL)
	{
		printf("luabind_TimeQuestScript_Param: s_pCurScriptTQS is null \n");
		return FALSE;
	}
	if (s_pCurNodeTQS == NULL)
	{
		printf("luabind_TimeQuestScript_Param: s_pCurNodeTQS is null \n");
		return FALSE;
	}

	const char* lpszParamName = g_pTQSLuaState->GetStringArgument(1, NULL);
	if (lpszParamName == NULL)
	{
		printf("luabind_TimeQuestScript_Param: Cant read param [%s][%s] \n", s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName());
		return FALSE;
	}

	if (g_pTQSLuaState->IsNumberArgument(2))
	{
		long double ldParamValue = g_pTQSLuaState->GetNumberArgument(2);

		CControlScriptNodeParam_Number* pParamNumber = s_pCurNodeTQS->CreateControlScriptNodeParam(lpszParamName, ldParamValue);
		if (!pParamNumber)
		{
			printf("%s\\%s[%d] : The script node [%s] parameter [%s] is a STRING type. \n", s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName(), s_pCurScriptTQS->GetScriptNodeCount() - 1, s_pCurNodeTQS->GetName(), lpszParamName);
			return FALSE;
		}

		if (s_pCurNodeTQS->AddParam(pParamNumber))
		{
			return TRUE;
		}
		else
		{
			printf("%s\\%s[%d] : false == s_pCurNodeTQS->AddParam( %f )\n", s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName(), s_pCurScriptTQS->GetScriptNodeCount() - 1, ldParamValue);
			return FALSE;
		}
	}
	else if (g_pTQSLuaState->IsStringArgument(2))
	{
		const char* lpszValue = g_pTQSLuaState->GetStringArgument(2);

		CControlScriptNodeParam_String* pParamString = s_pCurNodeTQS->CreateControlScriptNodeParam(lpszParamName, lpszValue);
		if (!pParamString)
		{
			printf("%s\\%s[%d] : The script node [%s] parameter [%s] is not available \n", s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName(), s_pCurScriptTQS->GetScriptNodeCount() - 1, s_pCurNodeTQS->GetName(), lpszParamName);
			return FALSE;
		}

		if (s_pCurNodeTQS->AddParam(pParamString))
		{
			return TRUE;
		}
		else
		{
			printf("%s\\%s[%d] : false == s_pCurNodeTQS->AddParam( %s )\n", s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName(), s_pCurScriptTQS->GetScriptNodeCount() - 1, lpszValue);
			return FALSE;
		}
	}
	else
	{
		printf("%s\\%s[%d] : The script node [%s] parameter [%s] is not valid. No number nor string \n", s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName(), s_pCurScriptTQS->GetScriptNodeCount() - 1, s_pCurNodeTQS->GetName(), lpszParamName);
	}

	return FALSE;
}

int luabind_TimeQuestScript_End(lua_State *L)
{
	if (s_pCurScriptTQS == NULL)
	{
		printf("luabind_TimeQuestScript_End: s_pCurScript is null \n");
		return 0xFFFFFFFF;
	}
	if (s_pCurNodeTQS == NULL)
	{
		printf("luabind_TimeQuestScript_End: s_pCurNode is null \n");
		return 0xFFFFFFFF;
	}


	if (s_pCurNodeTQS->IsComplete())
	{
		printf("luabind_TimeQuestScript_End: true == s_pCurNodeTQS->IsComplete() [%s][%s] \n", s_pCurScriptTQS->GetScriptManager()->GetPath(), s_pCurScriptTQS->GetFileName());
		return FALSE;
	}

	CControlScriptNode::eNODE_TYPE eNodeType = s_pCurNodeTQS->GetNodeType();
	if (eNodeType == CControlScriptNode::NODE_STATE)
	{
		s_pCurNodeTQS->SetComplete(true);
		s_pCurNodeTQS = NULL;
		return 0;
	}

	if (eNodeType <= CControlScriptNode::NODE_STATE || eNodeType > CControlScriptNode::NODE_CONDITION)
	{
		printf("luabind_TimeQuestScript_End: eNodeType > CControlScriptNode::NODE_CONDITION \n");
		return 0;
	}

	if (s_pCurNodeTQS->GetParent())
	{
		s_pCurNodeTQS->SetComplete(true);
		s_pCurNodeTQS = s_pCurNodeTQS->GetParent();
		return 0;
	}
	else
	{
		printf("luabind_TimeQuestScript_End: s_pCurNodeSPS->GetParent() == NULL \n");
		return 0xFFFFFFFF;
	}

	return 0xFFFFFFFF;
}