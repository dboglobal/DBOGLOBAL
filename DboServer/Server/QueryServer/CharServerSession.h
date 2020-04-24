#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"

class CNtlPacket;
class CQueryServer;

class CCharServerSession : public CNtlSession
{
public:

	CCharServerSession()
		:CNtlSession(SESSION_SERVER_CON_QUERY_CHAR)
	{
	}

	~CCharServerSession() {}

public:
	virtual int							OnAccept();
	virtual void						OnClose();
	virtual int							OnDispatch(CNtlPacket * pPacket);

public:
	
	void						RecvCharacterInfoReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvCreateCharReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvCharacterDelReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvCharacterLoadReq(CNtlPacket * pPacket, CQueryServer* app);

};