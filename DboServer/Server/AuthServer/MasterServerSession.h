#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"

class CNtlPacket;
class CAuthServer;

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


	// RECEIVE FROM MASTER SERVER
	void						RecvServersInfoAdd(CNtlPacket * pPacket, CAuthServer* app);
	void						RecvPlayerOnlineCheck(CNtlPacket * pPacket, CAuthServer* app);
	void						RecvServerInfoChangedNfy(CNtlPacket * pPacket, CAuthServer* app);

private:

};