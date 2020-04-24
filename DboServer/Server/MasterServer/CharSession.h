#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"

class CNtlPacket;
class CMasterServer;


class CCharServerPassiveSession : public CNtlSession
{
public:

	CCharServerPassiveSession()
		:CNtlSession(SESSION_SERVER_CON_CHAR_TO_MASTER),
		serverIndex(INVALID_SERVERINDEX)
	{
		SetControlFlag( CONTROL_FLAG_CHECK_ALIVE );
	}

	~CCharServerPassiveSession() {}


public:

	virtual DWORD				GetAliveCheckTime() { return 60000; }

	virtual int					OnAccept();
	virtual void				OnClose();
	virtual int					OnDispatch(CNtlPacket * pPacket);


	void						Cm_NfyServerBegin(CNtlPacket * pPacket, CMasterServer* app);
	void						Cm_UserLogin(CNtlPacket * pPacket, CMasterServer* app);
	void						Cm_UserLeave(CNtlPacket * pPacket, CMasterServer* app);
	void						Cm_CharExit(CNtlPacket * pPacket, CMasterServer* app);
	void						Cm_UserMove(CNtlPacket * pPacket, CMasterServer* app);


private:

	SERVERINDEX		serverIndex;

public:

	SERVERINDEX		GetServerIndex() { return serverIndex; }

};