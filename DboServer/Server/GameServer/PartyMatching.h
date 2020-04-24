#ifndef __INC_DBOG_PARTY_MATCHING_H__
#define __INC_DBOG_PARTY_MATCHING_H__

#include "NtlParty.h"
#include "NtlSingleton.h"

class CPlayer;
class CParty;


// PARTY MANAGER
class CPartyMatching : public CNtlSingleton<CPartyMatching>
{

public:
	CPartyMatching();
	virtual ~CPartyMatching();

private:

	struct sPARTY_MARCHING
	{
		CParty*		pkParty;
		BYTE		byDifficulty;
		BYTE		byDungeonType;
		BYTE		byRegionType;

		sPARTY_MARCHING(CParty* party, BYTE diffi, BYTE dungeontype, BYTE regiontype)
			: pkParty(party), byDifficulty(diffi), byDungeonType(dungeontype), byRegionType(regiontype)
		{
		}

	};

	std::map<PARTYID, sPARTY_MARCHING*> m_map_PartyMatching;


public:

	void				LoadPartyMatchingList(CPlayer* player, BYTE byPage, BYTE byDungeonType, BYTE byRegion);
	
	void				Register(CPlayer* player, BYTE byDifficulty, BYTE byDungeonType, BYTE byRegion, HOBJECT hItem, TBLIDX rankBattleWorldId);
	
	void				Unregister(CPlayer* player, PARTYID partyid = INVALID_PARTYID);

	void				GetPartyInfo(CPlayer* player, PARTYID partyid, BYTE byDungeonType, BYTE byRegion);

	void				Join(CPlayer* player, PARTYID partyid, BYTE byDungeonType, BYTE byRegion);
};

#define GetPartyMatching()		CPartyMatching::GetInstance()
#define g_pPartyMatching		GetPartyMatching()


#endif