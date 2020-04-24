#ifndef __SERVER_FACTORY_ACTIONPATTERN_UNIT__
#define __SERVER_FACTORY_ACTIONPATTERN_UNIT__

#include "NtlActionPattern.h"
#include "ActionPatternMove.h"
#include "ActionPatternIdle1.h"
#include "ActionPatternIdle2.h"
#include "ActionPatternMIdle1.h"
#include "ActionPatternMIdle2.h"


class CFactoryActionPatternUnit
{

public:

	CFactoryActionPatternUnit();
	virtual ~CFactoryActionPatternUnit();


	static CActionPatternUnit*		CreateActionPattern(eACTIONPATTERNID eActionPatternID)
	{
		CActionPatternUnit* pUnit = NULL;

		switch (eActionPatternID)
		{
		case BOTAP_MOVE:
		{
			pUnit = new CActionPatternMove();
		}
		break;
		case BOTAP_IDLE1:
		{
			pUnit = new CActionPatternIdle1();
		}
		break;
		case BOTAP_IDLE2:
		{
			pUnit = new CActionPatternIdle2();
		}
		break;
		case BOTAP_MIDLE1:
		{
			pUnit = new CActionPatternMIdle1();
		}
		break;
		case BOTAP_MIDLE2:
		{
			pUnit = new CActionPatternMIdle2();
		}
		break;

		default: ERR_LOG(LOG_BOTAI, "fail : Over switch : eActionPatternID(%d)", eActionPatternID);
		}

		return pUnit;
	}


};

#endif