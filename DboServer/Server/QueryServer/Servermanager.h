#pragma once

#include "NtlPacket.h"
#include "NtlSession.h"
#include "NtlSharedType.h"
#include "NtlSingleton.h"
#include "NtlSharedDef.h"


class CMasterServerSession;

class CServerManager : public CNtlSingleton<CServerManager>
{

public:
	CServerManager();
	virtual ~CServerManager();

private:

	std::map<SERVERCHANNELID, HSESSION>		mapGameServerChannelSession;

public:

	void						AddGameServer(SERVERCHANNELID byChannel, HSESSION sessionId);
	void						RemoveGameServer(SERVERCHANNELID byChannel);

	HSESSION					GetGsSession(SERVERCHANNELID channelid);

	void						Broadcast(CNtlPacket* pPacket, SERVERCHANNELID exception = INVALID_SERVERCHANNELID);

public:

	CMasterServerSession*		m_pMasterServerSession;

};

#define GetServerManager()		CServerManager::GetInstance()
#define g_SrvMgr				GetServerManager()