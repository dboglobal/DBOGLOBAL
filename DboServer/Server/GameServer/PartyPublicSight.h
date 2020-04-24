#ifndef __INC_DBOG_NPC_PARTY_PUBLICSIGHT_H__
#define __INC_DBOG_NPC_PARTY_PUBLICSIGHT_H__


class CPartyHelpData;
#include "NtlSharedDef.h"

class CPartyPublicSight
{

public:

	typedef std::map<HOBJECT, CPartyHelpData*> PARTYHELPDATA_MAP;
	typedef PARTYHELPDATA_MAP::value_type PARTYHELPDATA_VALUE;

public:

	CPartyPublicSight();
	virtual ~CPartyPublicSight();

public:

	void			AddHelpList(HOBJECT hCallObj, HOBJECT hTarget, DWORD dwAggroPoint);
	void			DeleteHelpList(HOBJECT hCallObj);
	void			DeleteHelpList(HOBJECT hTarget, HOBJECT hCallObj);
	void			AllDeleteHelpList();
	bool			IsHelpTarget(HOBJECT hTarget);

	bool			IsEmpty() { return m_mapPartyHelpList.size() == 0; }

private:

	PARTYHELPDATA_MAP	m_mapPartyHelpList;
	bool	m_bIsDestroy;

};

#endif