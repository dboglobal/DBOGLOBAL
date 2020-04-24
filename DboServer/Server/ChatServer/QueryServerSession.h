#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"

class CNtlPacket;
class CChatServer;

class CQueryServerSession : public CNtlSession
{
public:

	CQueryServerSession()
		:CNtlSession(SESSION_SERVER_CON_QUERY_CHAT)
	{
	}

	~CQueryServerSession() {}

public:

	virtual int							OnConnect();
	virtual void						OnClose();
	virtual int							OnDispatch(CNtlPacket * pPacket);

public:

	virtual DWORD						GetMaxRecvPacketCount() { return DEF_PACKET_MAX_COUNT * 10; }

private:

	void								RecvGuildCreateRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvGuildData(CNtlPacket * pPacket, CChatServer* app);
	void								RecvGuildMemberData(CNtlPacket * pPacket, CChatServer* app);
	void								RecvGuildDisbandRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvGuildInviteRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvGuildLeaveRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvGuildKickOutRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvGuildAppointSecondMasterRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvGuildDismissSecondMasterRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvGuildChangeGuildMasterRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvGuildFunctionAddRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvGuildGiveZeniRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvGuildChangeNoticeRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvGuildCreateMarkRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvGuildChangeMarkRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvGuildChangeNameRes(CNtlPacket * pPacket, CChatServer* app);

	void								RecvDojoDataRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvDojoCreateRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvDojoUpdateRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvDojoDeleteRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvDojoFunctionAddRes(CNtlPacket * pPacket, CChatServer* app);

	void								RecvDojoBudokaiSeedAddRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvDojoBudokaiSeedDelRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvDojoScrambleRewardRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvDojoChangeNoticeRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvAuctionHouseSellRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvAuctionHouseSellCancelRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvAuctionHouseBuyRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvAuctionHouseServerDataRes(CNtlPacket * pPacket, CChatServer* app);
	void								RecvAuctionHousePeriodEndRes(CNtlPacket * pPacket, CChatServer* app);

	void								RecvHlsSlotMachineExtractRes(CNtlPacket * pPacket, CChatServer* app);
};
