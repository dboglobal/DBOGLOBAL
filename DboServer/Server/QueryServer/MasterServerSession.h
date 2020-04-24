#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"

class CNtlPacket;
class CQueryServer;

class CMasterServerSession : public CNtlSession
{
public:

	CMasterServerSession()
		:CNtlSession(SESSION_SERVER_CON_QUERY_TO_MASTER)
	{
	}

	~CMasterServerSession() {}

public:
	int							OnConnect();
	void						OnClose();
	int							OnDispatch(CNtlPacket * pPacket);



private:

};