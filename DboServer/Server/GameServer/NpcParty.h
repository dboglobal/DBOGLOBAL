#ifndef __INC_DBOG_NPC_PARTY_H__
#define __INC_DBOG_NPC_PARTY_H__

#include "PartyPublicSight.h"

class CNpc;

class CNpcParty
{

public:

	struct sMEMBER_INFO
	{
		HOBJECT handle;
		bool bLeader;
	};

	typedef std::map<HOBJECT, sMEMBER_INFO> MEMBER_MAP;
	typedef MEMBER_MAP::value_type MEMBER_VAL;

	typedef std::map<HOBJECT, WORD> SHARETARGET_MAP;

public:
	CNpcParty();
	virtual ~CNpcParty();

public:

	bool			Create(PARTYID partyId, NPC_PARTYPTR partyPtr);

	PARTYID			GetPartyId() { return m_partyId; }
	NPC_PARTYPTR	GetPartyPtr() { return m_partyPtr; }

	size_t			GetMemberCount() { return m_mapMember.size(); }

	MEMBER_MAP::iterator Begin() { return m_mapMember.begin(); }
	MEMBER_MAP::iterator End() { return m_mapMember.end(); }

	bool			AddMember(CNpc* pBot, bool bLeader);

	void			RemoveMemberWithHandle(HOBJECT hMember);

	bool			IsEmpty() { return m_mapMember.size() == 0; }

	HOBJECT			GetLeaderHandle() { return m_hLeader; }

	CNpc*			GetLeader();

	CNpc*			GetMember_LowLP();

	bool			IsOutOfNest();

	void			AddHelpList(HOBJECT hCallObj, HOBJECT hTarget, DWORD dwAggroPoint);
	void			DeleteHelpList(HOBJECT hCallObj);
	void			DeleteHelpList(HOBJECT hTarget, HOBJECT hCallObj);
	bool			IsHelpTarget(HOBJECT hTarget);

	void			AddShareTarget(HOBJECT hTarget);
	void			RemoveShareTarget(HOBJECT hTarget);
	HOBJECT			OnShareTargetList(CNpc* pBot);



private:

	PARTYID m_partyId;
	NPC_PARTYPTR m_partyPtr;
	HOBJECT m_hLeader;
	MEMBER_MAP	m_mapMember;
	CPartyPublicSight m_cPartyPublicSight;
	SHARETARGET_MAP m_mapShareTarget;

};

#endif