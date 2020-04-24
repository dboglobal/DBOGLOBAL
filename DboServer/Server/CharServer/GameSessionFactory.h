#pragma once

#include "NtlSessionFactory.h"
#include "ClientSession.h"
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
		
			case SESSION_SERVER_CON_CHAR_TO_MASTER: 
			{
				pSession = new CMasterServerSession;
			}
			break;

			case SESSION_SERVER_CON_QUERY_CHAR:
			{
				pSession = new CQueryServerSession;
			}
			break;
		

		default:
			break;
		}

		return pSession;
	}
};