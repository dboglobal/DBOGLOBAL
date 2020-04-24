#ifndef __SERVER_ACTIONPATTERN_IDLE2__
#define __SERVER_ACTIONPATTERN_IDLE2__

#include "ActionPatternUnit.h"


class CActionPatternIdle2 : public CActionPatternUnit
{

public:

	CActionPatternIdle2();
	virtual ~CActionPatternIdle2();

public:

	virtual bool				Create(CCharacter* pBot);


};

#endif