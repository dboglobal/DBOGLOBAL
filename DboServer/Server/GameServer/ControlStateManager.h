#ifndef __SERVER_CONTROLSTATE_MANAGER__
#define __SERVER_CONTROLSTATE_MANAGER__

#include <map>
#include "ComplexState.h"

class CNpc;

class CControlStateManager
{

public:

	CControlStateManager();
	virtual ~CControlStateManager();


public:

	bool Create(CNpc * pBotNpc);
	void Destroy();

	bool AddState(DWORD aisnodeID, DWORD dwValue);
	bool UpdateState(DWORD aisnodeID, DWORD dwValue);

	DWORD GetState(DWORD aisnodeID);

	void DeleteState(DWORD aisnodeID);
	bool AllDeleteState();

	bool CopySubControl(DWORD nodeID, CComplexState* pFrom, CComplexState* pTo, CControlStateFactory* factory);


protected:

	std::map<DWORD, DWORD> m_mapState;

private:

	CNpc * m_pBotNpc;

};


#endif