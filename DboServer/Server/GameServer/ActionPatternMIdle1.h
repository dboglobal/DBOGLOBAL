#ifndef __SERVER_ACTIONPATTERN_MIDLE1__
#define __SERVER_ACTIONPATTERN_MIDLE1__

#include "ActionPatternUnit.h"


class CActionPatternMIdle1 : public CActionPatternUnit
{

public:

	CActionPatternMIdle1();
	virtual ~CActionPatternMIdle1();

public:

	virtual bool				Create(CCharacter* pBot);


};

#endif