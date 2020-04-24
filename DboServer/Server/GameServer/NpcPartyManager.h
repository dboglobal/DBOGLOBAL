#ifndef __DBOG_NPC_PARTY_MANAGER_H__
#define __DBOG_NPC_PARTY_MANAGER_H__


#include <vector>
#include "NtlSharedDef.h"

class CNpcParty;
class CNpc;

class CNpcPartyManager
{

public:
	CNpcPartyManager();
	virtual ~CNpcPartyManager();

public:

	void		Destroy();

protected:

	void		Init();

public:

	void		TickProcess(DWORD dwTickDiff, float fMultiple);

//	RemoveParty;

	CNpcParty*	FindParty(PARTYID partyId);
	CNpcParty*	FindParty(PARTYID partyId, NPC_PARTYPTR partyPtr);

//	GetPartyCount;

	CNpcParty*	CreateParty(PARTYID partyId);

	void		DestroyParty(CNpcParty *pParty);

	bool		JoinParty(CNpc *pBot, PARTYID partyId, bool bLeader);

	void		LeaveParty(CNpc *pBot);

private:

	std::map<PARTYID, NPC_PARTYPTR> m_mapNpcPartyId;
	std::vector<CNpcParty*>			m_vecNpcPartyPtr;
	std::list<NPC_PARTYPTR>			m_lstNpcPartyPtrPool;

};


#endif