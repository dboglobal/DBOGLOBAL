#ifndef __INC_DBOG_NPC_PARTY_HELPDATA_H__
#define __INC_DBOG_NPC_PARTY_HELPDATA_H__

#include "NtlSharedDef.h"

class CPartyHelpData
{

public:

	typedef std::map<HOBJECT, HOBJECT> CALLOBJ_MAP;
	typedef CALLOBJ_MAP::value_type CALLOBJ_VAL;

public:

	CPartyHelpData();
	virtual ~CPartyHelpData();

public:

	bool		IsSaveCallObj(HOBJECT hCallObj);
	bool		IsEmpty() { return mapCallObj.size() == 0; }
	void		AddCallObj(HOBJECT hCallObj);
	void		DeleteCallObj(HOBJECT hCallObj);

	HOBJECT		hTarget;
	DWORD		dwAggroPoint;

private:

	CALLOBJ_MAP	mapCallObj;

};

#endif