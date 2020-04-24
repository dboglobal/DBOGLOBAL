#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"

//#include "NtlPacketEncoder_RandKeyNew.h"

class CNtlPacket;
class CAuthServer;

class CClientSession : public CNtlSession
{
public:

	CClientSession(bool bAliveCheck = true, bool bOpcodeCheck = false)
		:CNtlSession( SESSION_CLIENT )
	{

		if( bAliveCheck )
			SetControlFlag( CONTROL_FLAG_CHECK_ALIVE );
		if( bOpcodeCheck )
			SetControlFlag( CONTROL_FLAG_CHECK_OPCODE );

//		SetControlFlag(CONTROL_FLAG_LIMITED_RECV_SIZE);

//		SetPacketEncoder( &m_packetEncoder );
	}


	~CClientSession();

public:

	virtual DWORD						GetMaxRecvPacketCount() { return 10; }

	virtual DWORD						GetMaxSendPacketCount() { return 10; }

	virtual DWORD						GetAliveCheckTime() { return 60000 * 3; } // 3 Minutes

	virtual int							OnAccept();
	virtual void						OnClose();
	virtual int							OnDispatch(CNtlPacket * pPacket);

public:

	void						SendCharLogInReq(CNtlPacket * pPacket, CAuthServer * app);
	void						SendLoginDcReq(CNtlPacket * pPacket, CAuthServer * app);

private:
//	CNtlPacketEncoder_RandKeyNew	m_packetEncoder;

	ACCOUNTID		AccountID;

	BYTE						m_byLoginTrys;

};