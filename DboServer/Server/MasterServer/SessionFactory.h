#pragma once

#include "NtlSessionFactory.h"
#include "AuthSession.h"
#include "CharSession.h"
#include "ChatSession.h"
#include "GameSession.h"
#include "WebSession.h"


class CMasterSessionFactory : public CNtlSessionFactory
{
public:

	CNtlSession * CreateSession(SESSIONTYPE sessionType)
	{
		CNtlSession * pSession = NULL;
		switch( sessionType )
		{
			case SESSION_SERVER_CON_AUTH_TO_MASTER:	{	pSession = new CAuthServerPassiveSession;	}break;
			case SESSION_SERVER_CON_CHAR_TO_MASTER:	{	pSession = new CCharServerPassiveSession;	}break;
			case SESSION_SERVER_CON_CHAT_TO_MASTER:	{	pSession = new CChatServerPassiveSession;	}break;
			case SESSION_SERVER_CON_GAME_TO_MASTER:	{	pSession = new CGameServerPassiveSession;	}break;
			case SESSION_SERVER_CON_WEB:			{	pSession = new CWebSession;	}break;

		default:
			break;
		}

		return pSession;
	}
};