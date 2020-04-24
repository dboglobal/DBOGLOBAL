#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"
#include "NtlCharacter.h"


class CNtlPacket;
class CCharServer;
class CPlayer;

class CClientSession : public CNtlSession
{
public:

	CClientSession()
		:CNtlSession( SESSION_CLIENT )
	{
		SetControlFlag( CONTROL_FLAG_CHECK_ALIVE );

		SetControlFlag(CONTROL_FLAG_LIMITED_RECV_SIZE);
	}

	~CClientSession();

public:

	virtual DWORD						GetMaxRecvPacketCount() { return 100; }

	virtual DWORD						GetMaxSendPacketCount() { return 100; }

	virtual int							OnAccept();
	virtual void						OnClose();
	virtual int							OnDispatch(CNtlPacket * pPacket);

	// Packet functions

	void						OnInvalid(CNtlPacket * pPacket);

	void						SendCharServerReq(CNtlPacket * pPacket);
	void						SendCharLoadReq(CNtlPacket * pPacket);
	void						SendCharCreateReq(CNtlPacket * pPacket);

	void						SendCharDeleteReq(CNtlPacket * pPacket);
	void						SendCancelCharDeleteReq(CNtlPacket * pPacket);

	void						SendCharExitReq(CNtlPacket * pPacket);
	void						SendCharWaitCheckReq(CNtlPacket * pPacket);
	void						SendCharRenameReq(CNtlPacket * pPacket);
	void						SendCharSelectReq(CNtlPacket * pPacket);
	void						SendCancelWaitReq(CNtlPacket * pPacket);

	void						LoadOneServerList(CNtlPacket * pPacket);
	void						LoadServerChannelInfo();
	void						LoadServerFarmInfo();
	void						LoadAllServerFarmInfo();

	void						SendServerListRes(CNtlPacket * pPacket);


public:

	void						SetPlayer(CPlayer* pPlayer) { m_pPlayer = pPlayer; }
	CPlayer*					GetPlayer() { return m_pPlayer; }

private:

	CPlayer*					m_pPlayer;

	eNTL_USER_CONNECTION_STATE	eUserState;
};