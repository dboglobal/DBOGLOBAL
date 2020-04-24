#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"

class CNtlPacket;


class CMasterServerSession : public CNtlSession
{
public:

	CMasterServerSession()
		:CNtlSession(SESSION_SERVER_CON_GAME_TO_MASTER) 
	{
	}

	~CMasterServerSession() {}

public:

	virtual int							OnConnect();
	virtual void						OnClose();
	virtual int							OnDispatch(CNtlPacket * pPacket);


public:

	void						OnInvalid(CNtlPacket * pPacket);

	void						RecvPingRes(CNtlPacket * pPacket);

	void						RecvServerInfoAdd(CNtlPacket * pPacket);

	void						RecvGameServerChannelInfo(CNtlPacket * pPacket);

	void						RecvCharServerUpdate(CNtlPacket * pPacket);
	void						RecvGameServerUpdate(CNtlPacket * pPacket);

	void						RecvUserLogin(CNtlPacket * pPacket);
	void						RecvUserMove(CNtlPacket * pPacket);
	void						RecvPlayerSwitchChannel(CNtlPacket * pPacket);
	void						RecvCharServerTeleportRes(CNtlPacket * pPacket);
};
