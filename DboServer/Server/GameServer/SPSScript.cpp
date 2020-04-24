#include "stdafx.h"
#include "SPSScript.h"

//lua
#include "NtlLuaState.h"

#include "ServerPlayScript.h"
#include "PlayScriptManager.h"



CNtlLuaState *g_pSPSLuaState = NULL;


CServerPlayScript* s_pCurScriptSPS = NULL;
CControlScriptNode* s_pCurNodeSPS = NULL;


CNtlLuaState* GetSPSLuaState(void)
{
	return g_pSPSLuaState;
}


void LuaScript_ErrorHandler_SP(const char *pError)
{
	if (pError == NULL)
		return;

	FILE *fp = NULL;

	if (fopen_s(&fp, "PlayScriptLog.txt", "a+") != 0)
		return;

	fprintf(fp, pError);
	fprintf(fp, "\n");

	fclose(fp);
}


void SPSLua_CreateState(void)
{
	if (g_pSPSLuaState)
	{
		delete g_pSPSLuaState;
		g_pSPSLuaState = NULL;
	}

	g_pSPSLuaState = new CNtlLuaState;
	g_pSPSLuaState->LinkErrorHandler(LuaScript_ErrorHandler_SP);


	//add functions
	g_pSPSLuaState->AddFunction("Scene", luabind_PlayScript_Scene);
	g_pSPSLuaState->AddFunction("Action", luabind_PlayScript_Action);
	g_pSPSLuaState->AddFunction("Condition", luabind_PlayScript_Condition);
	g_pSPSLuaState->AddFunction("Param", luabind_PlayScript_Param);
	g_pSPSLuaState->AddFunction("End", luabind_PlayScript_End);
}


void SPSLua_Setup(void)
{
	SPSLua_CreateState();
}


void SPSLua_Release(void)
{
	if (g_pSPSLuaState)
	{
		delete g_pSPSLuaState;
		g_pSPSLuaState = NULL;
	}
}




int luabind_PlayScript_Scene(lua_State *L)
{
	if (!g_pSPSLuaState->IsNumberArgument(1))
	{
		printf("luabind_PlayScript_Scene: number 1 check fail \n");
		return FALSE;
	}

	if (s_pCurScriptSPS == NULL)
	{
		printf("luabind_PlayScript_Scene: s_pCurScriptSPS is null \n");
		return FALSE;
	}
	if (s_pCurNodeSPS)
	{
		printf("luabind_PlayScript_Scene: Scene %u already available. [%s][%s] \n", s_pCurScriptSPS->GetScriptNodeCount(), s_pCurScriptSPS->GetScriptManager()->GetPath(), s_pCurScriptSPS->GetFileName());
		return FALSE;
	}

	DWORD dwSceneNumber = (DWORD)g_pSPSLuaState->GetNumberArgument(1, INVALID_DWORD);
	if (dwSceneNumber == INVALID_DWORD)
	{
		printf("luabind_PlayScript_Scene: Cant find scene number [%s][%s] \n", s_pCurScriptSPS->GetScriptManager()->GetPath(), s_pCurScriptSPS->GetFileName());
		return FALSE;
	}

	const char* lpszSceneName = g_pSPSLuaState->GetStringArgument(2, NULL);
	if (lpszSceneName == NULL)
	{
		printf("luabind_PlayScript_Scene: Cant find scene name [%s][%s] \n", s_pCurScriptSPS->GetScriptManager()->GetPath(), s_pCurScriptSPS->GetFileName());
		return FALSE;
	}

	CSPSNodeScene* pScene = new CSPSNodeScene(dwSceneNumber, lpszSceneName);

	if (s_pCurScriptSPS->AddScene(dwSceneNumber, pScene))
	{
		if (s_pCurScriptSPS->AddScriptNode(pScene))
		{
			s_pCurNodeSPS = pScene;
			return TRUE;
		}
	}
	else printf("luabind_AIScript_Scene: Add scene fail. Scene number [%d]. [%s][%s] \n", dwSceneNumber, s_pCurScriptSPS->GetScriptManager()->GetPath(), s_pCurScriptSPS->GetFileName());

	if (pScene)
		delete pScene;

	return FALSE;
}


int luabind_PlayScript_Action(lua_State *L)
{
	if (!g_pSPSLuaState->IsStringArgument(1))
	{
		printf("luabind_PlayScript_Action: string 1 check fail \n");
		return FALSE;
	}
	if (s_pCurScriptSPS == NULL)
	{
		printf("luabind_PlayScript_Action: s_pCurScript is null \n");
		return FALSE;
	}
	if (s_pCurNodeSPS == NULL)
	{
		printf("luabind_PlayScript_Action: s_pCurNode is null \n");
		return FALSE;
	}

	const char* lpszAction = g_pSPSLuaState->GetStringArgument(1, NULL);
	if (lpszAction == NULL)
	{
		printf("luabind_PlayScript_Action: Cant find state name [%s][%s] \n", s_pCurScriptSPS->GetScriptManager()->GetPath(), s_pCurScriptSPS->GetFileName());
		return FALSE;
	}

	CControlScriptNode* pScriptNode = (CControlScriptNode*)g_pPlayScriptManager->GetControlScriptNodeFactory()->CreateControlNodeAction(lpszAction);
	if (pScriptNode)
	{
		if (s_pCurNodeSPS->AddChild(pScriptNode))
		{
			if (s_pCurScriptSPS->AddScriptNode(pScriptNode))
			{
				s_pCurNodeSPS = pScriptNode;
				return TRUE;
			}
		}
	}
	else printf("luabind_PlayScript_Action: CreateControlNodeAction failed [%s][%s] \n", s_pCurScriptSPS->GetScriptManager()->GetPath(), s_pCurScriptSPS->GetFileName());

	if (pScriptNode)
		delete pScriptNode;

	return FALSE;
}

int luabind_PlayScript_Condition(lua_State *L)
{
	if (!g_pSPSLuaState->IsStringArgument(1))
	{
		printf("luabind_PlayScript_Condition: string 1 check fail \n");
		return FALSE;
	}
	if (s_pCurScriptSPS == NULL)
	{
		printf("luabind_PlayScript_Condition: s_pCurScript is null \n");
		return FALSE;
	}
	if (s_pCurNodeSPS == NULL)
	{
		printf("luabind_PlayScript_Condition: s_pCurNode is null \n");
		return FALSE;
	}

	const char* lpszCondition = g_pSPSLuaState->GetStringArgument(1, NULL);
	if (lpszCondition == NULL)
	{
		printf("luabind_PlayScript_Condition: Cant read condition [%s][%s] \n", s_pCurScriptSPS->GetScriptManager()->GetPath(), s_pCurScriptSPS->GetFileName());
		return FALSE;
	}

	CControlScriptNode* pScriptNode = (CControlScriptNode*)g_pPlayScriptManager->GetControlScriptNodeFactory()->CreateControlNodeCondition(lpszCondition);
	if (pScriptNode)
	{
		if (s_pCurNodeSPS->AddChild(pScriptNode))
		{
			if (s_pCurScriptSPS->AddScriptNode(pScriptNode))
			{
				s_pCurNodeSPS = pScriptNode;
				return TRUE;
			}
		}
	}
	else printf("luabind_PlayScript_Condition: CreateControlNodeCondition failed [%s][%s] \n", s_pCurScriptSPS->GetScriptManager()->GetPath(), s_pCurScriptSPS->GetFileName());

	if (pScriptNode)
		delete pScriptNode;

	return FALSE;
}

int luabind_PlayScript_Param(lua_State *L)
{
	if (s_pCurScriptSPS == NULL)
	{
		printf("luabind_PlayScript_Param: s_pCurScript is null \n");
		return FALSE;
	}
	if (s_pCurNodeSPS == NULL)
	{
		printf("luabind_PlayScript_Param: s_pCurNode is null \n");
		return FALSE;
	}

	const char* lpszParamName = g_pSPSLuaState->GetStringArgument(1, NULL);
	if (lpszParamName == NULL)
	{
		printf("luabind_PlayScript_Param: Cant read param [%s][%s] \n", s_pCurScriptSPS->GetScriptManager()->GetPath(), s_pCurScriptSPS->GetFileName());
		return FALSE;
	}

	if (g_pSPSLuaState->IsNumberArgument(2))
	{
		long double ldParamValue = g_pSPSLuaState->GetNumberArgument(2);

		CControlScriptNodeParam_Number* pParamNumber = s_pCurNodeSPS->CreateControlScriptNodeParam(lpszParamName, ldParamValue);
		if (!pParamNumber)
		{
			printf("%s\\%s[%d] : The script node [%s] parameter [%s] is a STRING type. \n", s_pCurScriptSPS->GetScriptManager()->GetPath(), s_pCurScriptSPS->GetFileName(), s_pCurScriptSPS->GetScriptNodeCount() - 1, s_pCurNodeSPS->GetName(), lpszParamName);
			return FALSE;
		}

		if (s_pCurNodeSPS->AddParam(pParamNumber))
		{
			return TRUE;
		}
		else
		{
			printf("%s\\%s[%d] : false == s_pCurNodeSPS->AddParam( %f )\n", s_pCurScriptSPS->GetScriptManager()->GetPath(), s_pCurScriptSPS->GetFileName(), s_pCurScriptSPS->GetScriptNodeCount() - 1, ldParamValue);
			return FALSE;
		}
	}
	else if (g_pSPSLuaState->IsStringArgument(2))
	{
		const char* lpszValue = g_pSPSLuaState->GetStringArgument(2);

		CControlScriptNodeParam_String* pParamString = s_pCurNodeSPS->CreateControlScriptNodeParam(lpszParamName, lpszValue);
		if (!pParamString)
		{
			printf("%s\\%s[%d] : The script node [%s] parameter [%s] is not available \n", s_pCurScriptSPS->GetScriptManager()->GetPath(), s_pCurScriptSPS->GetFileName(), s_pCurScriptSPS->GetScriptNodeCount() - 1, s_pCurNodeSPS->GetName(), lpszParamName);
			return FALSE;
		}

		if (s_pCurNodeSPS->AddParam(pParamString))
		{
			return TRUE;
		}
		else
		{
			printf("%s\\%s[%d] : false == s_pCurNodeSPS->AddParam( %s )\n", s_pCurScriptSPS->GetScriptManager()->GetPath(), s_pCurScriptSPS->GetFileName(), s_pCurScriptSPS->GetScriptNodeCount() - 1, lpszValue);
			return FALSE;
		}
	}
	else
	{
		printf("%s\\%s[%d] : The script node [%s] parameter [%s] is not valid. No number nor string \n", s_pCurScriptSPS->GetScriptManager()->GetPath(), s_pCurScriptSPS->GetFileName(), s_pCurScriptSPS->GetScriptNodeCount() - 1, s_pCurNodeSPS->GetName(), lpszParamName);
	}

	return FALSE;
}

int luabind_PlayScript_End(lua_State *L)
{
	if (s_pCurScriptSPS == NULL)
	{
		printf("luabind_PlayScript_End: s_pCurScript is null \n");
		return 0xFFFFFFFF;
	}
	if (s_pCurNodeSPS == NULL)
	{
		printf("luabind_PlayScript_End: s_pCurNode is null \n");
		return 0xFFFFFFFF;
	}


	if (s_pCurNodeSPS->IsComplete())
	{
		printf("luabind_PlayScript_End: true == s_pCurNodeSPS->IsComplete() [%s][%s] \n", s_pCurScriptSPS->GetScriptManager()->GetPath(), s_pCurScriptSPS->GetFileName());
		return 0xFFFFFFFF;
	}


	CControlScriptNode::eNODE_TYPE eNodeType = s_pCurNodeSPS->GetNodeType();
	if (eNodeType == CControlScriptNode::NODE_STATE)
	{
		s_pCurNodeSPS->SetComplete(true);
		s_pCurNodeSPS = NULL;
		return 0;
	}
	else if (eNodeType > CControlScriptNode::NODE_CONDITION)
	{
		printf("luabind_PlayScript_End: eNodeType > CControlScriptNode::NODE_CONDITION \n");
		return 0xFFFFFFFF;
	}

	if (s_pCurNodeSPS->GetParent()) 
	{
		s_pCurNodeSPS->SetComplete(true);
		s_pCurNodeSPS = s_pCurNodeSPS->GetParent();
		return 0;
	}
	else
	{
		printf("luabind_PlayScript_End: s_pCurNodeSPS->GetParent() == NULL \n");
		return 0xFFFFFFFF;
	}

	return 0xFFFFFFFF;
}