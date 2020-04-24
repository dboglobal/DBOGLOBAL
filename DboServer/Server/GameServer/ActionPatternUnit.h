#ifndef __SERVER_ACTIONPATTERN_UNIT__
#define __SERVER_ACTIONPATTERN_UNIT__

#include "NtlActionPattern.h"

class CCharacter;

class CActionPatternUnit
{

public:

	CActionPatternUnit(eACTIONPATTERNID eActionPatternID);
	virtual ~CActionPatternUnit();

public:

	virtual bool					Create(CCharacter* pBot) = 0;

	eACTIONPATTERNID				GetActionPatternID() { return this->m_eActionPatternID; }

	DWORD							GetAniTime() { return this->m_dwAniPlayTime; }


protected:

	eACTIONPATTERNID				m_eActionPatternID;

	DWORD							m_dwAniPlayTime;

};

#endif