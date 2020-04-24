#pragma once

#include "NtlSessionFactory.h"
#include "MasterServerSession.h"
#include "CharServerSession.h"
#include "ChatServerSession.h"
#include "GameServerSession.h"

class CSessionFactory : public CNtlSessionFactory
{
public:

	CNtlSession * CreateSession(SESSIONTYPE sessionType)
	{
		CNtlSession * pSession = NULL;
		switch( sessionType )
		{
			case SESSION_SERVER_CON_QUERY_TO_MASTER:
			{
				pSession = new CMasterServerSession;
			}break;
			case SESSION_SERVER_CON_QUERY_CHAR:
			{
				pSession = new CCharServerSession;
			}break;
			case SESSION_SERVER_CON_QUERY_CHAT:
			{
				pSession = new CChatServerSession;
			}break;
			case SESSION_SERVER_CON_QUERY_GAME:
			{
				pSession = new CGameServerSession;
			}break;

		default:
			break;
		}

		return pSession;
	}
};