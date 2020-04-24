#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"

class CNtlPacket;
class CChatServer;

class CMasterServerSession : public CNtlSession
{
public:

	CMasterServerSession()
		:CNtlSession(SESSION_SERVER_CON_CHAT_TO_MASTER)
	{
	}

	~CMasterServerSession() {}

public:
	int							OnConnect();
	void						OnClose();
	int							OnDispatch(CNtlPacket * pPacket);



private:

};