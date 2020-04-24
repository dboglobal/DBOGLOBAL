#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"

class CNtlPacket;

class CMasterServerSession : public CNtlSession
{
public:

	CMasterServerSession()
		:CNtlSession(SESSION_SERVER_CON_AUTH_TO_MASTER) 
	{
	}

	~CMasterServerSession() {}

public:
	int							OnConnect();
	void						OnClose();
	int							OnDispatch(CNtlPacket * pPacket);


public:

	void				OnInvalid(CNtlPacket * pPacket);

	// RECEIVE FROM MASTER SERVER
	void				RecvServerInfoAdd(CNtlPacket * pPacket);

	void				RecvGameServarFarmInfo(CNtlPacket * pPacket);
	void				RecvGameServerChannelInfo(CNtlPacket * pPacket);

	void				RecvGameServerFarmInfoChangedNfy(CNtlPacket * pPacket);
	void				RecvGameServerChannelUpdated(CNtlPacket * pPacket);

	void				RecvUserLoginRes(CNtlPacket * pPacket);
	void				RecvCharExit(CNtlPacket * pPacket);
	void				RecvUserMove(CNtlPacket * pPacket);



};