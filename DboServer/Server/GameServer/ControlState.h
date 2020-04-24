#ifndef __SERVER_CONTROLSTATE__
#define __SERVER_CONTROLSTATE__


#include "NtlLinkList.h"
#include "ControlScriptGlobal.h"


class CSvrScrVariableMap; 

class CControlScriptNode;

class CObjectMsg;

class CControlState : public CNtlLinkObject
{

public:
	CControlState(int controlStateID, const char* lpszControlStateName);
	virtual ~CControlState();

	enum eSTATUS
	{
		ACTIVATED = 0,
		INACTIVATED = 1,
		COMPLETED = 2,
		FAILED = 3,
		CHANGED = 4,
		BLOCKED = 0x5,
		SYSTEMERROR = 0x6,
		PAUSED = 0x7,
		CLEARED = 0x8,
		STATUS_COUNT,
	};

public:
	
	virtual void	OnEnter() { }
	virtual void	OnExit() { }
	virtual int		OnUpdate(DWORD dwTickTime, float fMultiple) { return m_status; }
	virtual int		OnObjectMsg(CObjectMsg * msg) { return m_status; }
	virtual bool	AddSubControlList(CControlState * state, bool bFront) = 0;
	virtual bool	AddSubControlQueue(CControlState * state, bool bFront) = 0;
	virtual bool	AttachControlScriptNode(CControlScriptNode * scriptnode)  { return true; }


public:

	CControlState* GetParent() { return m_pParent; }
	void SetParent(CControlState * controlstate) { m_pParent = controlstate; }

	void SetStatus(int status) { m_status = status; }
	int GetStatus() { return m_status; }
	bool IsStatus(eSTATUS status) const { return (status == m_status ? true : false); }

	int GetControlStateID() { return m_controlStateID; }

	char const * GetControlStateName() { return m_lpszControlStateName; }

	void SetLoopCount(DWORD dw) { m_dwConditionLoopCount = dw; }
	DWORD GetLoopCount() { return m_dwConditionLoopCount; }

	void SetDoAttachChild(bool b) { m_bDoAttachChild = b; }
	bool DoAttachChild() { return m_bDoAttachChild; }


public:

	virtual void Start();
	virtual void Stop();
	virtual void CopyTo(CControlState * destState);


public:

	bool					CreateVariableMap();

	CSvrScrVariableMap*		GetCurrentVariableMap() { return m_pVariableMap; }
	CSvrScrVariableMap*		GetValidVariableMap();

	sSSD_VARIABLE*			GetVariable(std::string& name, eSSD_VARIABLE_TYPE eVarType);
	sSSD_VARIABLE*			GetVariable(std::string& name);
	eSSD_VARIABLE_RESULT	UpdateVariable(std::string& name, sSSD_VARIABLE& sVariable);



protected:

	CControlState*			m_pParent;

	int						m_status;

	int						m_controlStateID;

	const char*				m_lpszControlStateName;

	DWORD					m_dwConditionLoopCount;

	CSvrScrVariableMap*		m_pVariableMap;

	bool					m_bDoAttachChild;

};


#endif