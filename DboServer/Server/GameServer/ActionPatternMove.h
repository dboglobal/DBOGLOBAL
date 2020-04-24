#ifndef __SERVER_ACTIONPATTERN_MOVE__
#define __SERVER_ACTIONPATTERN_MOVE__

#include "ActionPatternUnit.h"


class CActionPatternMove : public CActionPatternUnit
{

public:

	CActionPatternMove();
	virtual ~CActionPatternMove();

public:

	virtual bool				Create(CCharacter* pBot);


};

#endif