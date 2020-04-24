#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"

class CNtlPacket;
class CQueryServer;

class CChatServerSession : public CNtlSession
{
public:

	CChatServerSession()
		:CNtlSession(SESSION_SERVER_CON_QUERY_CHAT)
	{
	}

	~CChatServerSession() {}

public:
	virtual int							OnAccept();
	virtual void						OnClose();
	virtual int							OnDispatch(CNtlPacket * pPacket);


public:

	void						RecvGuildCreateReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvGuildDataReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvGuildDisbandReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvGuildInviteReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvGuildLeaveReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvGuildKickOutReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvGuildAppointSecondMasterReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvGuildDismissSecondMasterReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvGuildChangeGuildMasterReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvGuildFunctionAddReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvGuildGiveZeniReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvGuildChangeNoticeReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvGuildCreateMarkReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvGuildChangeMarkReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvGuildChangeNameRes(CNtlPacket * pPacket, CQueryServer* app);

	void						RecvDojoDataReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvDojoCreateReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvDojoUpdateReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvDojoDeleteReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvDojoFunctionAddReq(CNtlPacket * pPacket, CQueryServer* app);
	
	void						RecvDojoBudokaiSeedAddReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvDojoBudokaiSeedDelReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvDojoScrambleRewardReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvDojoChangeNoticeReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvAuctionHouseSellReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvAuctionHouseSellCancelReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvAuctionHouseBuyReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvAuctionHouseServerDataReq(CNtlPacket * pPacket, CQueryServer* app);
	void						RecvAuctionHousePeriodEndReq(CNtlPacket * pPacket, CQueryServer* app);

	void						RecvMutePlayerNfy(CNtlPacket * pPacket, CQueryServer* app);

	void						RecvHlsSlotMachineExtractReq(CNtlPacket * pPacket, CQueryServer* app);
};