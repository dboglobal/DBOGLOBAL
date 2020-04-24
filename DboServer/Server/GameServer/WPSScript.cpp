#include "stdafx.h"
#include "WPSScript.h"

//lua
#include "NtlLuaState.h"

#include "ServerWorldPlayScript.h"
#include "WorldPlayScriptManager.h"

#include "WPSNodeGameStage.h"
#include "WPSNodeGameFailed.h"



CNtlLuaState *g_pWPSLuaState = NULL;


CServerWorldPlayScript* s_pCurScriptWPS = NULL;
CControlScriptNode* s_pCurNodeWPS = NULL;


CNtlLuaState* GetWPSLuaState(void)
{
	return g_pWPSLuaState;
}


void LuaScript_ErrorHandler_WPS(const char *pError)
{
	if (pError == NULL)
		return;

	FILE *fp = NULL;

	if (fopen_s(&fp, "WorldPlayScriptLog.txt", "a+") != 0)
		return;

	fprintf(fp, pError);
	fprintf(fp, "\n");

	fclose(fp);
}


void WPSLua_CreateState(void)
{
	if (g_pWPSLuaState)
	{
		delete g_pWPSLuaState;
		g_pWPSLuaState = NULL;
	}

	g_pWPSLuaState = new CNtlLuaState;
	g_pWPSLuaState->LinkErrorHandler(LuaScript_ErrorHandler_WPS);


	//add functions
	g_pWPSLuaState->AddFunction("GameStage", luabind_WorldPlayScript_GameStage);
	g_pWPSLuaState->AddFunction("GameFailed", luabind_WorldPlayScript_GameFailed);
	g_pWPSLuaState->AddFunction("Action", luabind_WorldPlayScript_Action);
	g_pWPSLuaState->AddFunction("Condition", luabind_WorldPlayScript_Condition);
	g_pWPSLuaState->AddFunction("Param", luabind_WorldPlayScript_Param);
	g_pWPSLuaState->AddFunction("End", luabind_WorldPlayScript_End);
}


void WPSLua_Setup(void)
{
	WPSLua_CreateState();
}


void WPSLua_Release(void)
{
	if (g_pWPSLuaState)
	{
		delete g_pWPSLuaState;
		g_pWPSLuaState = NULL;
	}
}




int luabind_WorldPlayScript_GameStage(lua_State *L)
{
	if (s_pCurScriptWPS == NULL)
	{
		printf("luabind_WorldPlayScript_GameStage: s_pCurScriptWPS is null \n");
		return FALSE;
	}
	if (s_pCurNodeWPS)
	{
		printf("luabind_WorldPlayScript_GameStage: node is not finished. NodeCount %u [%s][%s] \n", s_pCurScriptWPS->GetScriptNodeCount(), s_pCurScriptWPS->GetScriptManager()->GetPath(), s_pCurScriptWPS->GetFileName());
		return FALSE;
	}
	if (!g_pWPSLuaState->IsNumberArgument(1))
	{
		printf("luabind_WorldPlayScript_GameStage: number 1 check fail \n");
		return FALSE;
	}

	BYTE byStageNumber = (BYTE)g_pWPSLuaState->GetNumberArgument(1, INVALID_BYTE);
	if (byStageNumber == INVALID_BYTE)
	{
		printf("luabind_WorldPlayScript_GameStage: Cant find stage number [%s][%s] \n", s_pCurScriptWPS->GetScriptManager()->GetPath(), s_pCurScriptWPS->GetFileName());
		return FALSE;
	}

	CWPSNodeGameStage* pStage = new CWPSNodeGameStage(byStageNumber);

	if (s_pCurScriptWPS->AddStage(pStage->GetStageNumber(), pStage))
	{
		if (s_pCurScriptWPS->AddScriptNode(pStage))
		{
			pStage->Create();

			s_pCurNodeWPS = pStage;
			return TRUE;
		}
	}
	else printf("luabind_WorldPlayScript_GameStage: Add stage fail. [%s][%s] \n", s_pCurScriptWPS->GetScriptManager()->GetPath(), s_pCurScriptWPS->GetFileName());

	if (pStage)
		delete pStage;

	return FALSE;
}



int luabind_WorldPlayScript_GameFailed(lua_State *L)
{
	if (s_pCurScriptWPS == NULL)
	{
		printf("luabind_WorldPlayScript_GameFailed: s_pCurScriptWPS is null \n");
		return FALSE;
	}
	if (s_pCurNodeWPS)
	{
		printf("luabind_WorldPlayScript_GameFailed: node is not finished. NodeCount %u [%s][%s] \n", s_pCurScriptWPS->GetScriptNodeCount(), s_pCurScriptWPS->GetScriptManager()->GetPath(), s_pCurScriptWPS->GetFileName());
		return FALSE;
	}


	CWPSNodeGameFailed* pStage = new CWPSNodeGameFailed;
	if (s_pCurScriptWPS->AddStage(INVALID_BYTE, pStage))
	{
		if (s_pCurScriptWPS->AddScriptNode(pStage))
		{
			pStage->Create();

			s_pCurNodeWPS = pStage;
			return TRUE;
		}
	}
	else printf("luabind_WorldPlayScript_GameStage: Add stage fail. [%s][%s] \n", s_pCurScriptWPS->GetScriptManager()->GetPath(), s_pCurScriptWPS->GetFileName());

	if (pStage)
		delete pStage;

	return FALSE;
}

int luabind_WorldPlayScript_Action(lua_State *L)
{
	if (!g_pWPSLuaState->IsStringArgument(1))
	{
		printf("luabind_WorldPlayScript_Action: string 1 check fail \n");
		return FALSE;
	}
	if (s_pCurScriptWPS == NULL)
	{
		printf("luabind_WorldPlayScript_Action: s_pCurScriptWPS is null \n");
		return FALSE;
	}
	if (s_pCurNodeWPS == NULL)
	{
		printf("luabind_WorldPlayScript_Action: s_pCurNodeWPS is null \n");
		return FALSE;
	}

	const char* lpszAction = g_pWPSLuaState->GetStringArgument(1, NULL);
	if (lpszAction == NULL)
	{
		printf("luabind_WorldPlayScript_Action: Cant find state name [%s][%s] \n", s_pCurScriptWPS->GetScriptManager()->GetPath(), s_pCurScriptWPS->GetFileName());
		return FALSE;
	}

	CControlScriptNodeAction* pScriptNode = g_pWorldPlayScriptManager->GetControlScriptNodeFactory()->CreateControlNodeAction(lpszAction);
	if (pScriptNode)
	{
		if (s_pCurNodeWPS->AddChild(pScriptNode))
		{
			if (s_pCurScriptWPS->AddScriptNode(pScriptNode))
			{	
				s_pCurNodeWPS = pScriptNode;
				return TRUE;
			}
		}
	}
	else printf("luabind_WorldPlayScript_Action: CreateControlNodeAction failed [%s][%s] \n", s_pCurScriptWPS->GetScriptManager()->GetPath(), s_pCurScriptWPS->GetFileName());

	SAFE_DELETE(pScriptNode);

	return FALSE;
}

int luabind_WorldPlayScript_Condition(lua_State *L)
{
	if (!g_pWPSLuaState->IsStringArgument(1))
	{
		printf("luabind_WorldPlayScript_Condition: string 1 check fail \n");
		return FALSE;
	}
	if (s_pCurScriptWPS == NULL)
	{
		printf("luabind_WorldPlayScript_Condition: s_pCurScript is null \n");
		return FALSE;
	}
	if (s_pCurNodeWPS == NULL)
	{
		printf("luabind_WorldPlayScript_Condition: s_pCurNode is null \n");
		return FALSE;
	}

	const char* lpszCondition = g_pWPSLuaState->GetStringArgument(1, NULL);
	if (lpszCondition == NULL)
	{
		printf("luabind_WorldPlayScript_Condition: Cant read condition [%s][%s] \n", s_pCurScriptWPS->GetScriptManager()->GetPath(), s_pCurScriptWPS->GetFileName());
		return FALSE;
	}

	CControlScriptNodeCondition* pScriptNode = g_pWorldPlayScriptManager->GetControlScriptNodeFactory()->CreateControlNodeCondition(lpszCondition);
	if (pScriptNode)
	{
		if (s_pCurNodeWPS->AddChild(pScriptNode))
		{
			if (s_pCurScriptWPS->AddScriptNode(pScriptNode))
			{
				s_pCurNodeWPS = pScriptNode;
				return TRUE;
			}
		}
	}
	else printf("luabind_WorldPlayScript_Condition: CreateControlNodeCondition failed [%s][%s] \n", s_pCurScriptWPS->GetScriptManager()->GetPath(), s_pCurScriptWPS->GetFileName());

	if (pScriptNode)
		delete pScriptNode;

	return FALSE;
}

int luabind_WorldPlayScript_Param(lua_State *L)
{
	if (s_pCurScriptWPS == NULL)
	{
		printf("luabind_WorldPlayScript_Param: s_pCurScriptWPS is null \n");
		return FALSE;
	}
	if (s_pCurNodeWPS == NULL)
	{
		printf("luabind_WorldPlayScript_Param: s_pCurNodeWPS is null \n");
		return FALSE;
	}

	const char* lpszParamName = g_pWPSLuaState->GetStringArgument(1, NULL);
	if (lpszParamName == NULL)
	{
		printf("luabind_WorldPlayScript_Param: Cant read param [%s][%s] \n", s_pCurScriptWPS->GetScriptManager()->GetPath(), s_pCurScriptWPS->GetFileName());
		return FALSE;
	}

	if (g_pWPSLuaState->IsNumberArgument(2))
	{
		long double ldParamValue = g_pWPSLuaState->GetNumberArgument(2);

		CControlScriptNodeParam_Number* pParamNumber = s_pCurNodeWPS->CreateControlScriptNodeParam(lpszParamName, ldParamValue);
		if (!pParamNumber)
		{
			printf("%s\\%s[%d] : The script node [%s] parameter [%s] is a STRING type. \n", s_pCurScriptWPS->GetScriptManager()->GetPath(), s_pCurScriptWPS->GetFileName(), s_pCurScriptWPS->GetScriptNodeCount() - 1, s_pCurNodeWPS->GetName(), lpszParamName);
			return FALSE;
		}

		if (s_pCurNodeWPS->AddParam(pParamNumber))
		{
			return TRUE;
		}
		else
		{
			printf("%s\\%s[%d] : false == s_pCurNodeWPS->AddParam( %f )\n", s_pCurScriptWPS->GetScriptManager()->GetPath(), s_pCurScriptWPS->GetFileName(), s_pCurScriptWPS->GetScriptNodeCount() - 1, ldParamValue);
			return FALSE;
		}
	}
	else if (g_pWPSLuaState->IsStringArgument(2))
	{
		const char* lpszValue = g_pWPSLuaState->GetStringArgument(2);

		CControlScriptNodeParam_String* pParamString = s_pCurNodeWPS->CreateControlScriptNodeParam(lpszParamName, lpszValue);
		if (!pParamString)
		{
			printf("%s\\%s[%d] : The script node [%s] parameter [%s] is not available \n", s_pCurScriptWPS->GetScriptManager()->GetPath(), s_pCurScriptWPS->GetFileName(), s_pCurScriptWPS->GetScriptNodeCount() - 1, s_pCurNodeWPS->GetName(), lpszParamName);
			return FALSE;
		}

		if (s_pCurNodeWPS->AddParam(pParamString))
		{
			return TRUE;
		}
		else
		{
			printf("%s\\%s[%d] : false == s_pCurNodeWPS->AddParam( %s )\n", s_pCurScriptWPS->GetScriptManager()->GetPath(), s_pCurScriptWPS->GetFileName(), s_pCurScriptWPS->GetScriptNodeCount() - 1, lpszValue);
			return FALSE;
		}
	}
	else
	{
		printf("%s\\%s[%d] : The script node [%s] parameter [%s] is not valid. No number nor string \n", s_pCurScriptWPS->GetScriptManager()->GetPath(), s_pCurScriptWPS->GetFileName(), s_pCurScriptWPS->GetScriptNodeCount() - 1, s_pCurNodeWPS->GetName(), lpszParamName);
	}

	return FALSE;
}

int luabind_WorldPlayScript_End(lua_State *L)
{
	if (s_pCurScriptWPS == NULL)
	{
		printf("luabind_WorldPlayScript_End: s_pCurScript is null \n");
		return FALSE;
	}
	if (s_pCurNodeWPS == NULL)
	{
		printf("luabind_WorldPlayScript_End: s_pCurNode is null \n");
		return FALSE;
	}


	if (s_pCurNodeWPS->IsComplete())
	{
		printf("luabind_WorldPlayScript_End: true == s_pCurNodeWPS->IsComplete() [%s][%s] \n", s_pCurScriptWPS->GetScriptManager()->GetPath(), s_pCurScriptWPS->GetFileName());
		return FALSE;
	}

	CControlScriptNode::eNODE_TYPE eNodeType = s_pCurNodeWPS->GetNodeType();
	if (eNodeType == CControlScriptNode::NODE_STATE)
	{
		s_pCurNodeWPS->SetComplete(true);
		s_pCurNodeWPS = NULL;
		return 0;
	}

	if (eNodeType <= CControlScriptNode::NODE_STATE || eNodeType > CControlScriptNode::NODE_CONDITION)
	{
		printf("luabind_WorldPlayScript_End: eNodeType > CControlScriptNode::NODE_CONDITION \n");
		return 0;
	}

	if (s_pCurNodeWPS->GetParent())
	{
		s_pCurNodeWPS->SetComplete(true);
		s_pCurNodeWPS = s_pCurNodeWPS->GetParent();
		return 0;
	}
	else
	{
		printf("luabind_WorldPlayScript_End: s_pCurNodeSPS->GetParent() == NULL \n");
		return 0xFFFFFFFF;
	}

	return 0xFFFFFFFF;
}