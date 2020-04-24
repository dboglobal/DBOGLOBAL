#ifndef __SERVER_NEIGHBORSERVERINFOMANAGER_MANAGER__
#define __SERVER_NEIGHBORSERVERINFOMANAGER_MANAGER__


#include "NtlSingleton.h"
#include "NeighborServerInfoManager.h"
#include "EventableObject.h"


class CSubNeighborServerInfoManager : public CNeighborServerInfoManager, public EventableObject, public CNtlSingleton<CSubNeighborServerInfoManager>
{

public:

	CSubNeighborServerInfoManager();
	virtual ~CSubNeighborServerInfoManager();

private:

	EventableObjectHolder *				m_pEventHolder;

public:

	void								TickProcess(DWORD dwTickDiff);

	void								StartEvents();
	void								OnEvent_ServerAliveUpdate();

public:

	void								LoadServerList(HSESSION hSession);

};

#define GetServerInfoManager()		CSubNeighborServerInfoManager::GetInstance()
#define g_pServerInfoManager		GetServerInfoManager()

#endif