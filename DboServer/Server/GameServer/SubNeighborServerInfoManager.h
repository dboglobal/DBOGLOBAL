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


public:

	void							SendMasterServerAlive();



};

#define GetServerInfoManager()		CSubNeighborServerInfoManager::GetInstance()
#define g_pServerInfoManager		GetServerInfoManager()

#endif