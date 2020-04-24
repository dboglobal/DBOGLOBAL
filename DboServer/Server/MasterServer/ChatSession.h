#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"

class CNtlPacket;
class CMasterServer;


class CChatServerPassiveSession : public CNtlSession
{
public:

	CChatServerPassiveSession()
		:CNtlSession(SESSION_SERVER_CON_CHAT_TO_MASTER)
	{
		SetControlFlag(CONTROL_FLAG_CHECK_ALIVE);
	}

	~CChatServerPassiveSession() {}


public:

	virtual DWORD				GetAliveCheckTime() { return 60000; }

	virtual int					OnAccept();
	virtual void				OnClose();
	virtual int					OnDispatch(CNtlPacket * pPacket);

public:

	void						Tm_NfyServerBegin(CNtlPacket * pPacket, CMasterServer* app);


};