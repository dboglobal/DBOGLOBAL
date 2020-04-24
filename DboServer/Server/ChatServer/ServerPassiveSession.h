#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"

class CNtlPacket;
class CChatServer;

////////////////////////////////////////////////////////////////////////////////////////////////
//// RECEIVE PACKET FROM GAME SERVER & CHAT SERVER
////////////////////////////////////////////////////////////////////////////////////////////////

class CServerPassiveSession :  public CNtlSession
{
public:

	CServerPassiveSession()
		:CNtlSession(SESSION_SERVER_PASSIVE)
	{

	}

	~CServerPassiveSession() {}


public:

	virtual int					OnAccept();
	virtual void				OnClose();
	virtual int					OnDispatch(CNtlPacket * pPacket);

	void						OnInvalid(CNtlPacket * pPacket);

	//GAME SERVER

	void						RecGameInfo(CNtlPacket * pPacket);
	void						RecWorldCreateNfy(CNtlPacket * pPacket);
	void						RecWorldDestroyNfy(CNtlPacket * pPacket);
	void						RecUserEnterGame(CNtlPacket * pPacket);
	void						RecUserLeaveGame(CNtlPacket * pPacket);
	void						RecvUserAuthKeyCreatedNfy(CNtlPacket * pPacket);

	void						RecUpdatePcPos(CNtlPacket * pPacket);
	void						RecUpdatePcClass(CNtlPacket * pPacket);
	void						RecUpdatePcLevel(CNtlPacket * pPacket);
	void						RecUpdatePcReputation(CNtlPacket * pPacket);
	void						RecUpdatePcPunish(CNtlPacket * pPacket);
	void						RecvConnectChannelChangeReq(CNtlPacket * pPacket);
	void						RecvDojoScrambleReset(CNtlPacket * pPacket);
	void						RecvDojoScramblePointNfy(CNtlPacket * pPacket);
	void						RecvDojoScrambleShieftSealStateNfy(CNtlPacket * pPacket);

	void						RecvChangeNameNfy(CNtlPacket * pPacket);


	//PARTY
	void						RecvPartyCreated(CNtlPacket * pPacket);
	void						RecvPartyDisbanded(CNtlPacket * pPacket);
	void						RecvPartyMemberJoined(CNtlPacket * pPacket);
	void						RecvPartyMemberLeft(CNtlPacket * pPacket);
	void						RecvPartyLeaderChanged(CNtlPacket * pPacket);

	//GUILD
	void						RecPcCreateGuild(CNtlPacket * pPacket);
	void						RecPcInviteGuild(CNtlPacket * pPacket);

	void						RecvSystemDisplayText(CNtlPacket * pPacket);

	void						RecGuildGiveZenny(CNtlPacket * pPacket);
	void						RecGuildFunctionAddReq(CNtlPacket * pPacket);
	void						RecvGuildBankStartReq(CNtlPacket * pPacket);
	void						RecvGuildBankEndNfy(CNtlPacket * pPacket);
	void						RecGuildCreateMarkReq(CNtlPacket * pPacket);
	void						RecGuildChangeMarkReq(CNtlPacket * pPacket);
	void						RecGuildChangeNameReq(CNtlPacket * pPacket);

	// BUDOKAI
	void						RecvBudokaiChannelSateInfoNfy(CNtlPacket * pPacket);
	void						RecvBudokaiChannelUpdateState(CNtlPacket * pPacket);
	void						RecvBudokaiChannelUpdateMatchState(CNtlPacket * pPacket);
	void						RecvBudokaiTournamentIndividualListRes(CNtlPacket * pPacket);
	void						RecvBudokaiTournamentIndividualInfoRes(CNtlPacket * pPacket);
	void						RecvBudokaiTournamentTeamListRes(CNtlPacket * pPacket);
	void						RecvBudokaiTournamentTeamInfoRes(CNtlPacket * pPacket);
	void						RecvBudokaiMinorMatchIndividualListNfy(CNtlPacket * pPacket);
	void						RecvBudokaiMinorMatchTeamListNfy(CNtlPacket * pPacket);
	void						RecvBudokaiMinorMatchTeleportInfoReq(CNtlPacket * pPacket);
	void						RecvBudokaiMinorMatchTeleportInfoRes(CNtlPacket * pPacket);
	void						RecvBudokaiProgressMessageNfy(CNtlPacket * pPacket);

	void						RecvCharReadyForCommunityServerNfy(CNtlPacket * pPacket);

	void						RecAuctionHouseSellReq(CNtlPacket * pPacket);
	void						RecAuctionHouseSellCancelReq(CNtlPacket * pPacket);
	void						RecAuctionHousePriceInfoReq(CNtlPacket * pPacket);
	void						RecAuctionHouseBuyReq(CNtlPacket * pPacket);
	void						RecAuctionHouseListDataReq(CNtlPacket * pPacket);
	void						RecAuctionHousePeriodEndText(CNtlPacket * pPacket);

	void						RecvCharServerTeleport(CNtlPacket * pPacket);

	//DOJO
	void						RecvDojoCreateReq(CNtlPacket * pPacket);
	void						RecvDojoDeleteReq(CNtlPacket * pPacket);
	void						RecvDojoFunctionAddReq(CNtlPacket * pPacket);
	void						RecvDojoScrambleReq(CNtlPacket * pPacket);
	void						RecvDojoScrambleResponse(CNtlPacket * pPacket);
	void						RecvDojoScrambleStateChange(CNtlPacket * pPacket);
	void						RecvDojoScrambleRewardReq(CNtlPacket * pPacket);
	void						RecvDojoNpcInfoReq(CNtlPacket * pPacket);
	void						RecvDojoSendTeleportProposal(CNtlPacket * pPacket);

	void						RecvBroadcastingSystemNfy(CNtlPacket * pPacket);

	void						RecvWaguCoinIncreaseNfy(CNtlPacket * pPacket);
	void						RecvEventCoinIncreaseNfy(CNtlPacket * pPacket);



private:

	CNtlNetQueue				_recvQueue;

	SERVERCHANNELID				m_channelId;

public:

	void						SetChannelID(SERVERCHANNELID id) { m_channelId = id; }

	SERVERCHANNELID				GetChannelID() { return m_channelId; }

};