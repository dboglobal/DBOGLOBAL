#include "stdafx.h"
#include "PartyHelpData.h"


CPartyHelpData::CPartyHelpData()
{
	hTarget = INVALID_HOBJECT;
	dwAggroPoint = 0;
}

CPartyHelpData::~CPartyHelpData()
{
}



bool CPartyHelpData::IsSaveCallObj(HOBJECT hCallObj)
{
	CALLOBJ_MAP::iterator it = mapCallObj.find(hCallObj);
	if (it != mapCallObj.end())
	{
		return true;
	}

	return false;
}


void CPartyHelpData::AddCallObj(HOBJECT hCallObj)
{
	if (!IsSaveCallObj(hCallObj))
	{
		mapCallObj.insert(CALLOBJ_VAL(hCallObj, hCallObj));
	}
}


void CPartyHelpData::DeleteCallObj(HOBJECT hCallObj)
{
	mapCallObj.erase(hCallObj);
}
