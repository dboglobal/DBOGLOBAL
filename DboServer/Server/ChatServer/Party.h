#ifndef __INC_DBOG_PARTY_H__
#define __INC_DBOG_PARTY_H__

#include "NtlParty.h"
#include "NtlSingleton.h"


class CNtlPacket;
class CPlayer;

class CParty
{

public:

	CParty(PARTYID hPartyID, CPlayer* pLeader);
	virtual ~CParty();


private:

	CHARACTERID					m_hLeader;
	PARTYID						m_hPartyID;
	SERVERCHANNELID				m_serverChannelId;
	std::vector<CPlayer*>		m_vecMember;

public:

	SERVERCHANNELID				GetChannelID() { return m_serverChannelId; }

	void						AddMember(CHARACTERID hMember);

	void						RemoveMember(CHARACTERID hMember);

	void						ChangeLeader(CHARACTERID hNewLeader);

	void						Broadcast(CNtlPacket* pPacket, CHARACTERID hException);

	BYTE						GetMemberCount() { return (BYTE)m_vecMember.size(); }

	void						FillMembersCharID(CHARACTERID* pCharID, BYTE& rCount, CHARACTERID charException);

};


// PARTY MANAGER
class CPartyManager : public CNtlSingleton<CPartyManager>
{

public:

	CPartyManager();
	virtual ~CPartyManager();

private:

	PARTYID								m_PartyHandler;

	std::map<PARTYID, CParty*>			m_map_Party;	//Each Channel can only create max 1000 parties. Channel 0 = 0-999, Channel 1 = 1000-1999, Channel 2 = 2000-2999... etc..


public:

	void								CreateParty(PARTYID partyId, CHARACTERID hLeader);

	CParty*								GetParty(PARTYID partyid);

	void								DisbandParty(PARTYID partyId);

	void								AddMember(PARTYID partyId, CHARACTERID hMember);

	void								RemoveMember(PARTYID partyId, CHARACTERID hMember);

	void								ChangePartyLeader(PARTYID partyId, CHARACTERID newLeaderCharId);

	void								Broadcast(PARTYID partyId, CNtlPacket* pPacket, CHARACTERID hException = INVALID_CHARACTERID);


public:

	void								RemoveAllParties(SERVERCHANNELID channelid);

};

#define GetPartyManager()				CPartyManager::GetInstance()
#define g_pPartyManager					GetPartyManager()


#endif