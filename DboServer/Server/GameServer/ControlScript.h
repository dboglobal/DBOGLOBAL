#ifndef __SERVER_CONTROLSCRIPT__
#define __SERVER_CONTROLSCRIPT__


#include <map>


class CControlScriptManager;
class CControlScriptNode;
class CControlScriptNodeAction;
class CControlScriptNodeCondition;
class CControlStateFactory;
class CControlState;
class CNtlString;

class CControlScript
{

public:
	CControlScript(CControlScriptManager * pManager = NULL, const char* lpszFileName = NULL);
	virtual ~CControlScript();


public:
	
	virtual CControlScriptNodeAction* CreateControlScriptNodeAction(const char* lpszScriptNodeName);
	virtual CControlScriptNodeCondition* CreateControlScriptNodeCondition(const char* lpszScriptNodeName);

	virtual bool Save();
	virtual bool AttachScript(CControlState * pControlState, CControlStateFactory * pFactory);

	virtual bool Load(char * achBuffer);
	virtual bool Load(const char* lpszScriptFile);

	virtual bool CheckIntegrity();

public:

	CControlScriptManager* GetScriptManager() { return m_pScriptManager; }

	const char* GetFileName() { return m_strScriptFileName.c_str(); }

	void SetSSEPath(const char* lpszSSEPath) { m_strSSEPath.Format("%s", lpszSSEPath); }
	const char* GetSSEPath() { return m_strSSEPath.c_str(); }

	bool				AddScriptNode(CControlScriptNode * pScriptNode);
	CControlScriptNode*	GetScriptNode(DWORD dwNodeIndex);

	DWORD GetScriptNodeCount() { return (DWORD)m_mapScriptNode.size(); }


protected:

	CControlScriptManager * m_pScriptManager;

	CNtlString m_strScriptFileName;

	CNtlString m_strSSEPath;

	std::map<DWORD, CControlScriptNode *> m_mapScriptNode;

};


#endif