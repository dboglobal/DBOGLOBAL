#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"

class CNtlPacket;
class CMasterServer;


class CAuthServerPassiveSession : public CNtlSession
{
public:

	CAuthServerPassiveSession()
		:CNtlSession(SESSION_SERVER_CON_AUTH_TO_MASTER),
		serverIndex(INVALID_SERVERINDEX)
	{
		SetControlFlag(CONTROL_FLAG_CHECK_ALIVE);
	}

	~CAuthServerPassiveSession() {}

public:

	virtual DWORD				GetAliveCheckTime() { return 60000; }


	virtual int					OnAccept();
	virtual void				OnClose();
	virtual int					OnDispatch(CNtlPacket * pPacket);




	void						Am_NfyServerBegin(CNtlPacket * pPacket, CMasterServer* app);
	void						Am_CheckPlayerOnline(CNtlPacket * pPacket, CMasterServer* app);




private:

	SERVERINDEX		serverIndex;

public:

	SERVERINDEX		GetServerIndex() { return serverIndex; }

};