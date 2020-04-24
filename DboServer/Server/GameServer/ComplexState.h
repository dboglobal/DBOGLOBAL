#ifndef __SERVER_COMPLEXSTATE__
#define __SERVER_COMPLEXSTATE__

#include "ControlStateEx.h"

class CControlStateFactory;

class CComplexState : public CControlState
{

public:

	CComplexState(int controlStateID, const char* lpszControlStateName);
	virtual ~CComplexState();

public:

	virtual void OnContinue() {}
	virtual void OnPause()  {}

	virtual void Start();
	virtual void Pause();
	virtual void Stop();

	virtual bool AddSubControlList(CControlState * controlstate, bool bFront);
	virtual bool AddSubControlQueue(CControlState * controlstate, bool bFront);


public:

	void RemoveAllSubControl();

	void DestroyAllSubControl();

	void AllSubControlStop();

	bool MoveSubControl(CComplexState * pTo);
	bool CopySubControl(CComplexState * pTo, CControlStateFactory * pControlFactory);

	bool RemoveSubControlList(int controlStateID);
	bool RemoveSubControlQueue(int controlStateID);

	void RemoveAllSubControlList();
	void RemoveAllSubControlQueue();

	void DestroyAllSubControlList();
	void DestroyAllSubControlQueue();

	int UpdateSubControlList(DWORD dwTickTime, float fMultiple);
	int UpdateSubControlQueue(DWORD dwTickTime, float fMultiple);


public:

	CComplexState * GetCurrentAction();

public:

	virtual int OnUpdate(DWORD dwTickTime, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * objmsg);

	

protected:

	CNtlLinkList m_subControlList;
	CNtlLinkList m_subControlQueue;

};


#endif