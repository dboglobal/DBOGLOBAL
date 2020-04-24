#pragma once

#include "NtlSession.h"
#include "NtlSessionFactory.h"
#include "ClientSession.h"
#include "ServerPassiveSession.h"
#include "MasterServerSession.h"
#include "QueryServerSession.h"

class CGameSessionFactory : public CNtlSessionFactory
{
public:

	CNtlSession * CreateSession(SESSIONTYPE sessionType)
	{
		CNtlSession * pSession = NULL;
		switch( sessionType )
		{
			case SESSION_CLIENT: 
			{
				pSession = new CClientSession;
			}
			break;
			case SESSION_SERVER_PASSIVE: 
			{
				pSession = new CServerPassiveSession;
			}
			break;
			case SESSION_SERVER_CON_QUERY_CHAT:
			{
				pSession = new CQueryServerSession;
			}
			break;
			case SESSION_SERVER_CON_CHAT_TO_MASTER:
			{
				pSession = new CMasterServerSession;
			}
			break;

		default: break;
		}

		return pSession;
	}
};