#ifndef __SERVER_CONTROLSTATE_COMPOSITE__
#define __SERVER_CONTROLSTATE_COMPOSITE__

#include "ControlStateFactory.h"

class CControlAction;


class CControlStateComposite : public CControlState
{

public:

	CControlStateComposite(int controlStateID, const char* lpszControlStateName);
	virtual ~CControlStateComposite();

public:

	virtual int OnUpdate(DWORD dwTickTime, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * objmsg);
	virtual bool AddSubControlList(CControlState * pControlState, bool bFront);
	virtual bool AddSubControlQueue(CControlState * pControlState, bool bFront);

public:

	void RemoveAllSubControl();
	void DestroyAllSubControl();

public:

	void AllSubControlStop();
	bool MoveSubControl(CControlStateComposite* pTo);
	bool CopySubControl(CControlStateComposite * pTo, CControlStateFactory* pControlFactory);

	bool RemoveSubControlList(int controlStateID);
	bool RemoveSubControlQueue(int controlStateID);

	void RemoveAllSubControlList();
	void RemoveAllSubControlQueue();
	void DestroyAllSubControlList();
	void DestroyAllSubControlQueue();

	int UpdateSubControlList(DWORD dwTickTime, float fMultiple);
	int UpdateSubControlQueue(DWORD dwTickTime, float fMultiple);

	CControlAction* GetCurrentAction();
	CControlAction* GetNextAction();


protected:

	CNtlLinkList			m_subControlList;
	CNtlLinkList			m_subControlQueue;

};


#endif