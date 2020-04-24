#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"

class CNtlPacket;
class CPlayer;

class CChatServerSession : public CNtlSession
{
public:

	CChatServerSession()
		:CNtlSession(SESSION_SERVER_CON_CHAT) 
	{
	}

	~CChatServerSession() {}

public:

	virtual int							OnConnect();
	virtual void						OnClose();
	virtual int							OnDispatch(CNtlPacket * pPacket);

	//receive packet from chat server
	void						OnInvalid(CNtlPacket * pPacket);

	void						RecUserEnterGameAck(CNtlPacket * pPacket);

	void						RecvUserAuthKeyGeneratedAck(CNtlPacket * pPacket);

	void						RecGuildCreateRes(CNtlPacket * pPacket);
	void						RecGuildCreateNfy(CNtlPacket * pPacket);
	void						RecGuildInviteRes(CNtlPacket * pPacket);
	void						RecGuildDestroyedRes(CNtlPacket * pPacket);
	void						RecvGuildInfoRefreshedNfy(CNtlPacket * pPacket);
	void						RecGuildIdChangedRes(CNtlPacket * pPacket);
	void						RecGuildGiveZenny(CNtlPacket * pPacket);
	void						RecGuildFunctionAddRes(CNtlPacket * pPacket);

	void						RecvGuildBankStartRes(CNtlPacket * pPacket);
	void						RecGuildCreateMarkRes(CNtlPacket * pPacket);
	void						RecGuildChangeMarkRes(CNtlPacket * pPacket);
	void						RecGuildChangeNameRes(CNtlPacket * pPacket);
	void						RecvGuildBankEndNfy(CNtlPacket * pPacket);

	// BUDOKAI
	void						RecvBudokaiChannelStateInfoNfy(CNtlPacket * pPacket);
	void						RecvBudokaiChannelUpdateState(CNtlPacket * pPacket);
	void						RecvBudokaiChannelUpdateMatchState(CNtlPacket * pPacket);
	void						RecvBudokaiTournamentIndividualListReq(CNtlPacket * pPacket);
	void						RecvBudokaiTournamentIndividualInfoReq(CNtlPacket * pPacket);
	void						RecvBudokaiTournamentTeamListReq(CNtlPacket * pPacket);
	void						RecvBudokaiTournamentTeamInfoReq(CNtlPacket * pPacket);
	void						RecvBudokaiMinorMatchIndividualListNfy(CNtlPacket * pPacket);
	void						RecvBudokaiMinorMatchTeamListNfy(CNtlPacket * pPacket);
	void						RecvBudokaiMinorMatchTeleportInfoReq(CNtlPacket * pPacket);
	void						RecvBudokaiMinorMatchTeleportInfoRes(CNtlPacket * pPacket);
	void						RecvBudokaiProgressMessageNfy(CNtlPacket * pPacket);


	//AUCTION HOUSE
	void						RecAuctionHouseSellRes(CNtlPacket * pPacket);
	void						RecAuctionHouseSellCancelRes(CNtlPacket * pPacket);
	void						RecAuctionHousePriceInfoRes(CNtlPacket * pPacket);
	void						RecAuctionHouseBuyRes(CNtlPacket * pPacket);
	void						RecAuctionHouseListData(CNtlPacket * pPacket);
	void						RecAuctionHouseListDataRes(CNtlPacket * pPacket);
	void						RecAuctionHousePeriodEndRes(CNtlPacket * pPacket);

	void						RecvWaguCoinDecreaseNfy(CNtlPacket * pPacket);
	void						RecvWaguWaguMachineUpdateCashitemInfo(CNtlPacket * pPacket);

	void						RecvDojoCreateRes(CNtlPacket * pPacket);
	void						RecvCharServerTeleport(CNtlPacket * pPacket);
	void						RecvDojoCreatedNfy(CNtlPacket * pPacket);
	void						RecvDojoDeleteRes(CNtlPacket * pPacket);
	void						RecvDojoFunctionAddRes(CNtlPacket * pPacket);
	void						RecvDojoUpdateRes(CNtlPacket * pPacket);
	void						RecvDojoSendAttGuildId(CNtlPacket * pPacket);
	void						RecvDojoNpcInfoRes(CNtlPacket * pPacket);
	void						RecvDojoSendTeleportProposalNfy(CNtlPacket * pPacket);
	void						RecvDojoUserCountChange(CNtlPacket * pPacket);
	void						RecvDojoDestroyedNfy(CNtlPacket * pPacket);

	void						RecvDojoScrambleRes(CNtlPacket * pPacket);
	void						RecvDojoScrambleResponse(CNtlPacket * pPacket);

	void						RecvDojoScrambleStateChange(CNtlPacket * pPacket);

	void						RecvConnectChannelChangeRes(CNtlPacket * pPacket);

	//Send packet to chat server
	void						SendUpdatePcLevel(CPlayer* pPlayer);



};
