#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"
#include "NtlPacketQC.h"


class CNtlPacket;

class CQueryServerSession : public CNtlSession
{
public:

	CQueryServerSession()
		:CNtlSession(SESSION_SERVER_CON_QUERY_CHAR)
	{
	}

	~CQueryServerSession() {}

public:

	int							OnConnect();
	void						OnClose();
	int							OnDispatch(CNtlPacket * pPacket);

public:

	void						OnInvalid(CNtlPacket * pPacket);

	void						RecvCreateCharRes(CNtlPacket * pPacket);
	void						RecvCharacterLoadRes(CNtlPacket * pPacket);

};
