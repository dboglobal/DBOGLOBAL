#include <Windows.h>
#include <oleauto.h>
#include "NtlLuaState.h"

extern "C" {
#include "lua/src/lua.h"
#include "lua/src/luac.h"
#include "lua/src/lualib.h"
#include "lua/src/lauxlib.h"
}

#define NTL_MAX_LUA_ERR		2048

CNtlLuaState::CNtlLuaState()
{
	m_pScriptContext = luaL_newstate();
//	luaopen_base(m_pScriptContext);
//	luaopen_io(m_pScriptContext);
//	luaopen_string(m_pScriptContext);
//	luaopen_math(m_pScriptContext);
//	luaopen_debug(m_pScriptContext);
//	luaopen_table(m_pScriptContext);
//	luaopen_loadlib(m_pScriptContext);
	luaL_openlibs(m_pScriptContext);
}

CNtlLuaState::~CNtlLuaState()
{
	if(m_pScriptContext)
		lua_close(m_pScriptContext);
}


void CNtlLuaState::CallErrorHandler(const char *pError)
{
	ListErrorHandler::iterator it;
	for(it = m_listErrorHandler.begin(); it != m_listErrorHandler.end(); ++it)
	{
		(*it)(pError);
	}
}

void CNtlLuaState::CallSuccessHandler(const char *pSuccess)
{
	ListSuccessHandler::iterator it;
	for(it = m_listSuccessHandler.begin(); it != m_listSuccessHandler.end(); ++it)
	{
		(*it)(pSuccess);
	}
}

bool CNtlLuaState::RunScript(const char *pFileName)
{
	if(luaL_loadfile(m_pScriptContext, pFileName) != 0)
	{
		CallErrorHandler(luaL_checkstring(m_pScriptContext, -1));
		return false;
	}

	if(lua_pcall(m_pScriptContext, 0, LUA_MULTRET, 0) != 0)
	{
		CallErrorHandler(luaL_checkstring(m_pScriptContext, -1));
		return false;
	}

	char chSuccess[NTL_MAX_LUA_ERR];
	sprintf_s(chSuccess, NTL_MAX_LUA_ERR, "RunScript(lua_pcall) Success - Script Name : %s\n", pFileName);
	CallSuccessHandler(chSuccess);

	return true;
}

bool CNtlLuaState::RunString(const char *pCmd)
{
	if(luaL_loadbuffer(m_pScriptContext, pCmd, strlen(pCmd), NULL) != 0)
	{
		char chError[ NTL_MAX_LUA_ERR ] = { '\0', };
		sprintf_s(chError, NTL_MAX_LUA_ERR, "RunString(luaL_loadbuffer) Error - Error Message:%s", luaL_checkstring(m_pScriptContext, -1));

		CallErrorHandler(chError);

		return false;
	}

	if(lua_pcall(m_pScriptContext, 0, LUA_MULTRET, 0) != 0)
	{
		char chError[ NTL_MAX_LUA_ERR ] = { '\0', };
		sprintf_s(chError, NTL_MAX_LUA_ERR, "RunString(lua_pcall) Error - Error Message:%s", luaL_checkstring(m_pScriptContext, -1));

		CallErrorHandler(chError);

		return false;
	}

	char chSuccess[NTL_MAX_LUA_ERR] = { '\0', };
	sprintf_s(chSuccess, NTL_MAX_LUA_ERR, "RunString(lua_pcall) Success\n");
	CallSuccessHandler(chSuccess);

	return true;
}

bool CNtlLuaState::AddFunction(const char *pFuncName, LuaFunctionType pFunc)
{
	lua_register(m_pScriptContext, pFuncName, pFunc);
	return true;
}

const char* CNtlLuaState::GetStringArgument(int num, const char *pDefault)
{
	return luaL_optstring(m_pScriptContext, num, pDefault);

}

double CNtlLuaState::GetNumberArgument(int num, double dDefault)
{
	return luaL_optnumber(m_pScriptContext, num, dDefault);
}

bool CNtlLuaState::IsStringArgument(int num)
{
	if ( !lua_isstring( m_pScriptContext, num) )
	{
		return false;
	}

	return true;
}

bool CNtlLuaState::IsNumberArgument(int num)
{
	if ( !lua_isnumber( m_pScriptContext, num) )
	{
		return false;
	}

	return true;
}

void CNtlLuaState::PushNumber(int iValue)
{
	lua_pushnumber(m_pScriptContext, iValue);
}

void CNtlLuaState::PushNumber(unsigned int uiValue)
{
	lua_pushnumber(m_pScriptContext, uiValue);
}

void CNtlLuaState::PushNumber(float fValue)
{
	lua_pushnumber(m_pScriptContext, fValue);
}

void CNtlLuaState::SetNumberVariable(const char *pVariable, int iValue)
{
	lua_pushnumber ( m_pScriptContext, iValue );
	lua_setglobal ( m_pScriptContext, pVariable );
}

void CNtlLuaState::SetNumberVariable(const char *pVariable, unsigned int uiValue)
{
	lua_pushnumber ( m_pScriptContext, uiValue );
	lua_setglobal ( m_pScriptContext, pVariable );
}

void CNtlLuaState::SetNumberVariable(const char *pVariable, float fValue)
{
	lua_pushnumber ( m_pScriptContext, fValue );
	lua_setglobal ( m_pScriptContext, pVariable );
}


void CNtlLuaState::LinkErrorHandler(fpFuncErrorHandler fpFunc)
{
	m_listErrorHandler.push_back(fpFunc);
}

void CNtlLuaState::LinkSuccessHandler(fpFuncSuccessHandler fpFunc)
{
	m_listSuccessHandler.push_back(fpFunc);
}

int CNtlLuaState::CompileLua(const char* szFilePath, const char* szOutputPath /* = NULL */)
{
	if (!szFilePath)
		return false;

	return false;

	//// File Path
	//char szDrive[128] = { 0, };
	//char szdir[128] = { 0, };
	//char szfName[128] = { 0, };
	//char szExt[128] = { 0, };
	//_splitpath_s(szFilePath, szDrive, szdir, szfName, szExt);

	//char* szArgv[4] = { 0, };
	//szArgv[0] = new char[128];
	//szArgv[1] = new char[128];
	//szArgv[2] = new char[128];
	//szArgv[3] = new char[128];

	//sprintf(szArgv[0], "dummy");
	//sprintf(szArgv[1], "-o");
	//if (!szOutputPath)
	//{
	//	sprintf(szArgv[2], "%s%s%s.out", szDrive, szdir, szfName);
	//}
	//else
	//{
	//	sprintf(szArgv[2], "%s\\%s.out", szOutputPath, szfName);
	//}

	//sprintf(szArgv[3], "%s", szFilePath);

	//int nReturn = ::ComplieLua(4, szArgv);

	//delete szArgv[0];
	//delete szArgv[1];
	//delete szArgv[2];
	//delete szArgv[3];

	//return nReturn;
}