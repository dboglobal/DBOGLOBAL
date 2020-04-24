#ifndef __SERVER_ACTIONPATTERN_MIDLE2__
#define __SERVER_ACTIONPATTERN_MIDLE2__

#include "ActionPatternUnit.h"


class CActionPatternMIdle2 : public CActionPatternUnit
{

public:

	CActionPatternMIdle2();
	virtual ~CActionPatternMIdle2();

public:

	virtual bool				Create(CCharacter* pBot);


};

#endif