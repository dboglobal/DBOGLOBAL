#ifndef __SERVER_CONTROLSCRIPT_NODE__
#define __SERVER_CONTROLSCRIPT_NODE__


#include "NtlLinkList.h"
#include "NtlString.h"

#include "ControlScriptGlobal.h"

class CControlScriptNodeParam_Number;
class CControlScriptNodeParam_String;


class CControlScriptManager;
class CSvrScrVariableMap;
class CControlState;
class CControlStateFactory;
class CControlScriptNodeParam;
class CControlScript;

 
class CControlScriptNode : public CNtlLinkObject
{
public:

	enum ePARAM_TYPE
	{
		PARAM_NUMBER = 0x0,
		PARAM_STRING = 0x1,
		MAX_PARAM_TYPE = 0x2,
		INVALID_PARAM_TYPE = 0xFF,
	};

	struct sPARAMETER_INFO
	{
		bool bLastParameter;
		ePARAM_TYPE eParamType;
		CNtlString strParamName;
		bool bIndispensable;
	};

	enum eNODE_TYPE
	{
		NODE_STATE = 0x0,
		NODE_ACTION = 0x1,
		NODE_CONDITION = 0x2,
		NODE_PARAM = 0x3,
		MAX_NODE_TYPE = 0x4,
	};

public:

	CControlScriptNode(int ctrlScrNodeID, const char* lpszName, bool bVariableContainer);
	virtual ~CControlScriptNode();

public:

	virtual eNODE_TYPE GetNodeType() = 0;


public:


	virtual sPARAMETER_INFO* GetParameterMap() { return NULL; }
	virtual bool CheckParameter() { return true; }
	virtual bool AttachScript(CControlState* pControlState, CControlStateFactory* pControlFactory, bool bAttachCurrent);
	virtual bool AddChild(CControlScriptNode * pNode);
	virtual bool AddParam(CControlScriptNodeParam_String* paramstring);
	virtual bool AddParam(CControlScriptNodeParam_Number* paramnumber);

	bool Create(bool bVarContainer);
	void Destroy();
	bool CheckIntegrity();

	sPARAMETER_INFO* FindParameterInfo(const char* lpszParamName);
	CControlScriptNodeParam* FindParamterNode(const char* lpszParamName);
	CControlScriptNodeParam_String* CreateControlScriptNodeParam(const char* lpszParamName, const char* lpszValue);
	CControlScriptNodeParam_Number* CreateControlScriptNodeParam(const char* lpszParamName, long double number);

	int GetID() { return m_ctrlScrNodeID; }

	const char* GetName() { return m_strName.c_str(); }

	CControlScriptManager* GetScriptManager() { return m_pScriptManager; }
	void SetScriptManager(CControlScriptManager * manager) { m_pScriptManager = manager; }

	CControlScript* GetScript() { return m_pScript; }
	void SetScript(CControlScript * controlscript) { m_pScript = controlscript; }

	CControlScriptNode* GetParent() { return m_pParent; }
	void SetParent(CControlScriptNode * controlscriptnode) { m_pParent = controlscriptnode; }

	bool IsComplete() { return m_bCompleted; }
	void SetComplete(bool b) { m_bCompleted = b; }

	void SetNodeIndex(DWORD dwNodeIndex) { m_dwNodeIndex = dwNodeIndex; }
	DWORD GetNodeIndex() { return m_dwNodeIndex; }

	void SetUnitId(const char* szUnitId){ m_strUnitId.operator=(szUnitId); }
	const char* GetUnitId() { return m_strUnitId.c_str(); }

	DWORD GetChildNodeIndex(int ctrlId);

	CControlScriptNode* GetChildNode(int ctrlId);

	CSvrScrVariableMap* GetCurrentVariableMap() { return m_pVariableMap; }
	CSvrScrVariableMap* GetValidVariableMap();

	sSSD_VARIABLE*			GetVariable(std::string& name);
	eSSD_VARIABLE_RESULT	UpdateVariable(std::string& name, sSSD_VARIABLE& sVariable);


	static const char* GetNodeTypeName(eNODE_TYPE eNodeType);
	static const char* GetParamTypeName(ePARAM_TYPE eParamType);

	void SetLoopCount(DWORD dwLoopCount) { m_dwConditionLoopCount = dwLoopCount; }
	DWORD GetLoopCount() { return m_dwConditionLoopCount; }


private:

	void Init();
	bool CheckParameterMap();


protected:

	CControlScriptManager * m_pScriptManager;
	CControlScript * m_pScript;
	CControlScriptNode * m_pParent;
	int m_ctrlScrNodeID;
	CNtlString m_strName;
	CNtlLinkList m_childList;
	CNtlLinkList m_paramList;
	bool m_bCompleted;
	int m_nScriptLine;
	DWORD m_dwNodeIndex;
	CNtlString m_strUnitId;
	DWORD m_dwConditionLoopCount;
	CSvrScrVariableMap * m_pVariableMap;
};


#endif
