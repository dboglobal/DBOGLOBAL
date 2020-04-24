#ifndef __SERVER_CONTAINER_ACTIONPATTERN_SET__
#define __SERVER_CONTAINER_ACTIONPATTERN_SET__


#include "ContainerActionPatternUnit.h"
#include "Npc.h"


class CContainerActionPatternSet
{
	struct sPATTERN_DATA
	{
		TBLIDX		mobTblidx;
		TBLIDX		actionePatternTblidx;
		CContainerActionPatternUnit* pPatternUnit;
	};

	typedef std::map<TBLIDX, CContainerActionPatternUnit*>	PATTERN_MAP;
	typedef PATTERN_MAP::value_type							PATTERN_VAL;

	typedef std::multimap<TBLIDX, sPATTERN_DATA>			ACTIONPATTERN_MAP;
	typedef ACTIONPATTERN_MAP::value_type					ACTIONPATTERN_VAL;

public:

	CContainerActionPatternSet();
	virtual ~CContainerActionPatternSet();

public:

	bool							Create();
	void							Destroy();

	bool							Add(CCharacter* pBot, TBLIDX actionPatternTblidx);

	void							Clear();

	CContainerActionPatternUnit*	Find(TBLIDX mobTblidx, TBLIDX actionPatternTblidx);

private:

	bool							AddPattern(CCharacter* pBot);

	CContainerActionPatternUnit*	FindPattern(TBLIDX tblidxBot);

public:

	bool							DeletePattern(TBLIDX tblidxBot);

	void							AllDeletePattern();


private:
	bool					m_bIsDestroy;

	PATTERN_MAP				m_mapPattern;

	ACTIONPATTERN_MAP		m_mapActionPattern;

};

#endif