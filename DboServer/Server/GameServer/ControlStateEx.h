#ifndef __SERVER_CONTROLSTATE_EX__
#define __SERVER_CONTROLSTATE_EX__


#include "ControlState.h"
#include "ComplexState.h"
#include "ControlStateComposite.h"


class CObjectMsg;

class CControlStateEx : public CControlState
{

public:
	CControlStateEx(int controlStateID, const char* lpszControlStateName);
	virtual ~CControlStateEx();

public:

	virtual int OnUpdate(DWORD dwTickTime, float fMultiple);
	virtual int OnObjectMsg(CObjectMsg * pObjectMsg);
	virtual bool AddSubControlList(CControlState * state, bool bFront);
	virtual bool AddSubControlQueue(CControlState * state, bool bFront);

public:

	void				RemoveAllSubControl();

	CControlStateEx*	GetSubControlQueue(int id);

	void				RemoveAllSubControlList();
	void				RemoveAllSubControlQueue();

	int					UpdateSubControlList(DWORD dwTickTime, float fMultiple);
	int					UpdateSubControlQueue(DWORD dwTickTime, float fMultiple);

	void				ClearRemovableList();
	void				ClearRemovableQueue();

	bool				IsRemovable() { return m_bRemovable; }
	bool				IsRemoving() { return m_bRemoving; }

	void				SetRemoving(bool b) { m_bRemoving = b; }
	bool				IsEmpty() { return !m_subControlList.GetCount() && !m_subControlQueue.GetCount(); }

private:

	CNtlLinkList m_subControlList;
	CNtlLinkList m_subControlQueue;
	bool m_bRemovable;
	bool m_bRemoving;

};

#endif