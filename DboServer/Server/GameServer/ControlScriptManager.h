#ifndef __SERVER_CONTROLSCRIPT_MANAGER__
#define __SERVER_CONTROLSCRIPT_MANAGER__

class CControlScript;
class CControlScriptNodeFactory;

class CControlState;
class CControlStateFactory;


#include "NtlString.h"
#include <map>

#include "ControlScript.h"


class CControlScriptManager
{

public:
	CControlScriptManager();
	virtual ~CControlScriptManager();

	enum eFILE_TYPE
	{
		FILE_SCRIPT,
		FILE_PACK,
		FILE_ENCRYPT,
		FILE_ALL,
		INVALID_FILE_TYPE,
	};

public:

	virtual bool						Create(const char* lpszScriptPath, eFILE_TYPE eLoadFileType, const char* lpszScriptFileExt, const char* lpszScriptPackFileExt, const char* lpszScriptEncryptFileExt) = 0;

	virtual void						SetLogFunction(){}

	virtual CControlScript*				CreateControlScript(const char* lpszScriptFileName) = 0;

	virtual CControlScriptNodeFactory*	GetControlScriptNodeFactory() = 0;


public:

	void			Destroy(){}

	bool			Load(const char* lpszScriptPath, eFILE_TYPE eLoadFileType);

	void			Unload();

	bool			Reload();

	bool			Save(char const* lpszScriptPath) { return true; }

	bool			AttachScript(CControlState* pControlState, CControlStateFactory* pFactory, DWORD dwScriptNumber);

protected:

	bool			LoadScript(const char* lpszScriptPath, const char* lpszScriptFile, const char* lpszSSEPath);

public:

	const char*		GetPath() { return m_strPath.c_str(); }

	CControlScript* FindScript(DWORD dwScriptNumber);

protected:

	bool			LoadScriptFile(const char* lpszScriptPath, const char* lpszFileName, const char* lpszSSEPath);
	bool			LoadScriptPackFile(char const* lpszScriptPath, char const* lpszFileName) { return false; }
	bool			LoadScriptEncryptFile(char const* lpszScriptPath, char const* lpszFileName) { return false; }
	bool			CheckScript();

	bool			LoadFile(const char* lpszScriptPath, const char* lpszSearchFileName, const char* lpszSSEPath);

private:

	typedef std::map<DWORD, CControlScript*> SCRIPTLIST;
	typedef SCRIPTLIST::value_type SCRIPT_VAL;

	SCRIPTLIST		m_scriptList;

protected:

	CNtlString		m_strPath;
	CNtlString		m_strScriptFileExt;
	CNtlString		m_strScriptPackFileExt;
	CNtlString		m_strScriptEncryptFileExt;

};


#endif
