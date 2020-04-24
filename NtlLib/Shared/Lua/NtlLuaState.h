/*****************************************************************************
 *
 * File			: NtlLuaState.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2006. 2. 20	
 * Abstract		: lua state class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_LUA_STATE_H__
#define __NTL_LUA_STATE_H__

#include <list>

struct lua_State;

#define LuaGlue extern "C" int
extern "C" {
typedef int (*LuaFunctionType)(struct lua_State *pLuaState);
};

typedef void(*fpFuncErrorHandler)(const char *pError);
typedef void(*fpFuncSuccessHandler)(const char *pError);

class CNtlLuaState
{
private:
	
	typedef std::list<fpFuncErrorHandler> ListErrorHandler;
	typedef std::list<fpFuncSuccessHandler> ListSuccessHandler;

	lua_State			*m_pScriptContext;
	ListErrorHandler	m_listErrorHandler;
	ListSuccessHandler	m_listSuccessHandler;

private:

	void CallErrorHandler(const char *pError);
	void CallSuccessHandler(const char *pSuccess);

public:

	CNtlLuaState();
	~CNtlLuaState();

	bool RunScript(const char *pFileName);
	bool RunString(const char *pCmd);

	bool AddFunction(const char *pFuncName, LuaFunctionType pFunc);
	const char* GetStringArgument(int num, const char *pDefault = NULL);
	double GetNumberArgument(int num, double dDefault=0.0);
	bool IsStringArgument(int num);
	bool IsNumberArgument(int num);

	void PushNumber(int iValue);
	void PushNumber(unsigned int uiValue);
	void PushNumber(float fValue);

	void SetNumberVariable(const char *pVariable, int iValue);
	void SetNumberVariable(const char *pVariable, unsigned int uiValue);
	void SetNumberVariable(const char *pVariable, float fValue);
	
	void LinkErrorHandler(fpFuncErrorHandler fpFunc);
	void LinkSuccessHandler(fpFuncSuccessHandler fpFunc);

	static int CompileLua(const char* szFilePath, const char* szOutputPath = NULL);
};

#endif