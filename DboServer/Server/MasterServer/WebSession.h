#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"

class CNtlPacket;


class CWebSession : public CNtlSession
{
public:

	CWebSession()
		:CNtlSession(SESSION_SERVER_CON_WEB)
	{
		SetControlFlag(CONTROL_FLAG_CHECK_ALIVE);
	}

	~CWebSession() {}

public:

	virtual DWORD				GetMaxRecvPacketCount() { return 10; }

	virtual DWORD				GetMaxSendPacketCount() { return 10; }

	virtual DWORD				GetAliveCheckTime() { return 5000; }

	virtual int					OnAccept();
	virtual void				OnClose();
	virtual int					OnDispatch(CNtlPacket * pPacket);


};