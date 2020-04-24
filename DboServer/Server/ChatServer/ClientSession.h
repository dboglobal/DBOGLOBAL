#pragma once

#include "NtlSession.h"
#include "NtlSharedType.h"
#include "NtlResultCode.h"


class CNtlPacket;
class CPlayer;


class CClientSession : public CNtlSession
{

public:

	CClientSession()
		:CNtlSession( SESSION_CLIENT )
	{
		SetControlFlag(CONTROL_FLAG_LIMITED_RECV_SIZE);
	}

	~CClientSession() {}



public:

	virtual DWORD				GetMaxRecvPacketCount() { return 100; }

	virtual DWORD				GetMaxSendPacketCount() { return 100; }

	virtual DWORD				GetAliveCheckTime() { return 60000 * 5; } // 5 Minutes

	virtual int					OnAccept();
	virtual void				OnClose();
	virtual int					OnDispatch(CNtlPacket * pPacket);

public:

	void						OnInvalid(CNtlPacket * pPacket);

	void						RecvEnterChat(CNtlPacket * pPacket);

	/*CHAT FUNCTIONS*/
	void						RecvChatMessageSay(CNtlPacket * pPacket);
	void						RecvChatShout(CNtlPacket * pPacket);
	void						RecvChatWhisper(CNtlPacket * pPacket);
	void						RecvChatParty(CNtlPacket * pPacket);
	void						RecvChatGuild(CNtlPacket * pPacket);
	void						RecvChatTrade(CNtlPacket * pPacket);
	void						RecvChatFindParty(CNtlPacket * pPacket);

	void						RecvChatUserOnChannelUpdateNfy(CNtlPacket * pPacket);


	/*FIEND & BLACKLIST SYSTEM*/
	void						RecvAddFriendReq(CNtlPacket * pPacket);
	void						RecvDelFriendReq(CNtlPacket * pPacket);
	void						RecvMoveFriendReq(CNtlPacket * pPacket);
	void						RecvBlackListAddReq(CNtlPacket * pPacket);
	void						RecvBlackListDelReq(CNtlPacket * pPacket);

	//Rank battle
	void						RecvRankBattleRankListReq(CNtlPacket * pPacket);
	void						RecvRankBattleRankFindCharacterReq(CNtlPacket * pPacket);
	void						RecvRankBattleRankCompareDayReq(CNtlPacket * pPacket);

	//GUILD
	void						RecvDisbandGuildReq(CNtlPacket * pPacket);
	void						RecvGuildResponseInv(CNtlPacket * pPacket);
	void						RecvLeaveGuildReq(CNtlPacket * pPacket);
	void						RecvKickFromGuildReq(CNtlPacket * pPacket);

	void						RecvGuildChangeNoticeReq(CNtlPacket * pPacket);
	void						RecvNewSecondGuildMasterReq(CNtlPacket * pPacket);
	void						RecvRemoveSecondGuildMasterReq(CNtlPacket * pPacket);
	void						RecvUpdateGuildMasterReq(CNtlPacket * pPacket);

	// BUDOKAI
	void						RecvBudokaiTournamentIndividualListReq(CNtlPacket * pPacket);
	void						RecvBudokaiTournamentIndividualInfoReq(CNtlPacket * pPacket);
	void						RecvBudokaiTournamentTeamListReq(CNtlPacket * pPacket);
	void						RecvBudokaiTournamentTeamInfoReq(CNtlPacket * pPacket);

	//DOJO
	void						RecvDojoBudokaiSeedAddReq(CNtlPacket * pPacket);
	void						RecvDojoBudokaiSeedDelReq(CNtlPacket * pPacket);
	void						RecvDojoNoticeChangeReq(CNtlPacket * pPacket);

	//HLS SLOT MACHINE
	void						RecvHlsSlotMachineInfoReq(CNtlPacket * pPacket);
	void						RecvHlsSlotMachineExtractReq(CNtlPacket * pPacket);
	void						RecvHlsSlotMachineWinnerInfoReq(CNtlPacket * pPacket);

	// End Packet functions


private:
	CPlayer*					cPlayer;
	ACCOUNTID					accountId;

	DWORD						m_dwLastMessage;
	DWORD						m_dwLastGlobalMessage;

public:
	void						SetPlayer(CPlayer* pPlayer) { cPlayer = pPlayer; }
};