#pragma once

#include "NtlSessionFactory.h"
#include "ClientSession.h"
#include "ChatServerSession.h"
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
			case SESSION_SERVER_CON_CHAT:
			{
				pSession = new CChatServerSession;
			}
			break;
			case SESSION_SERVER_CON_QUERY_GAME:
			{
				pSession = new CQueryServerSession;
			}
			break;
			case SESSION_SERVER_CON_GAME_TO_MASTER:
			{
				pSession = new CMasterServerSession;
			}
			break;

			default:
				break;
		}

		return pSession;
	}
};