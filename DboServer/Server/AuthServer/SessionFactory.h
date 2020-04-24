#pragma once

#include "NtlSessionFactory.h"
#include "ClientSession.h"
#include "MasterServerSession.h"

class CAuthSessionFactory : public CNtlSessionFactory
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

			case SESSION_SERVER_CON_AUTH_TO_MASTER: 
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