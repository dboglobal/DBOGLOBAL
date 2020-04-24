#ifndef __SERVER_NEIGHBORSERVERINFOMANAGER_MANAGER__
#define __SERVER_NEIGHBORSERVERINFOMANAGER_MANAGER__


#include "NtlSingleton.h"
#include "NtlCSArchitecture.h"


class CNeighborServerInfoManager : public CNtlSingleton<CNeighborServerInfoManager>
{

public:

	CNeighborServerInfoManager();
	virtual ~CNeighborServerInfoManager();

private:

//	list<sDBO_SERVER_INFO *> m_listCharacterServerInfo;

};

#define GetServerInfoManager()		CNeighborServerInfoManager::GetInstance()
#define g_pServerInfoManager		GetServerInfoManager()

#endif