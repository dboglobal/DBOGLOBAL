#ifndef __SERVER_ACTIONPATTERN_IDLE1__
#define __SERVER_ACTIONPATTERN_IDLE1__

#include "ActionPatternUnit.h"


class CActionPatternIdle1 : public CActionPatternUnit
{

public:

	CActionPatternIdle1();
	virtual ~CActionPatternIdle1();

public:

	virtual bool				Create(CCharacter* pBot);


};

#endif