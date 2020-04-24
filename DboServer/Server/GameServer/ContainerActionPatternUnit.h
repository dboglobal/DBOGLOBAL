#ifndef __SERVER_CONTAINER_ACTIONPATTERN_UNIT__
#define __SERVER_CONTAINER_ACTIONPATTERN_UNIT__


#include "ActionPatternUnit.h"
#include <vector>

class CContainerActionPatternUnit
{
	typedef std::vector<CActionPatternUnit*>	ACTIONPATTERN_VECTOR;
	typedef ACTIONPATTERN_VECTOR::iterator		ACTIONPATTERN_ITER;
	typedef ACTIONPATTERN_VECTOR::value_type	ACTIONPATTERN_VALUE;

public:

	CContainerActionPatternUnit();
	virtual ~CContainerActionPatternUnit();

public:

	bool					Create(TBLIDX actionpatternTblID, ACTIONPATTERN_FIELDSIZE nActionPatternFieldSize);
	void					Destroy();

	bool					AddActionPattern(CActionPatternUnit* pActionUnit);

	CActionPatternUnit*		GetActionPattern(ACTIONPATTERN_FIELD apField);

	bool					DeleteActionPattern(ACTIONPATTERN_FIELD apField);

	void					AllDeleteActionPattern();

	int						GetCount();

	TBLIDX					GetTblIdx();

	bool					m_bIsDestroy;

private:

	ACTIONPATTERN_VECTOR	m_vecActionPattern;

	TBLIDX					m_actionpatternTblIdx;

};

#endif