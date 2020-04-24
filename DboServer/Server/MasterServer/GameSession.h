#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"
#include "NtlCSArchitecture.h"

class CNtlPacket;
class CMasterServer;


class CGameServerPassiveSession : public CNtlSession
{
public:

	CGameServerPassiveSession()
		:CNtlSession(SESSION_SERVER_CON_GAME_TO_MASTER),
		serverFarmID(INVALID_SERVERFARMID),
		serverChannelID(INVALID_SERVERCHANNELID)
	{
		SetControlFlag( CONTROL_FLAG_CHECK_ALIVE );
	}

	~CGameServerPassiveSession() {}

public:

	virtual DWORD				GetAliveCheckTime() { return 60000; }

	virtual int					OnAccept();
	virtual void				OnClose();
	virtual int					OnDispatch(CNtlPacket * pPacket);


	void						Gm_NfyServerBegin(CNtlPacket * pPacket, CMasterServer* app);
	void						Gm_PlayerEnter(CNtlPacket * pPacket, CMasterServer* app);
	void						Gm_PlayerLeave(CNtlPacket * pPacket, CMasterServer* app);
	void						Gm_PlayerMove(CNtlPacket * pPacket, CMasterServer* app);
	void						Gm_PlayerSwitchChannel(CNtlPacket * pPacket, CMasterServer* app);
	void						RecvCharServerTeleportReq(CNtlPacket * pPacket, CMasterServer* app);

	void						Gm_RecvServerShutdownNfy(CNtlPacket * pPacket, CMasterServer* app);

	void						RecvDragonballScrambleSeasonStateNfy(CNtlPacket * pPacket, CMasterServer* app);

private:

	SERVERFARMID				serverFarmID;
	SERVERCHANNELID				serverChannelID;

public:

	SERVERFARMID				GetFarmID() { return serverFarmID; }
	SERVERCHANNELID				GetChannelID() { return serverChannelID; }

};