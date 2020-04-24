#ifndef __SERVER_ACTIONPATTERN__
#define __SERVER_ACTIONPATTERN__

#include "NtlActionPattern.h"


class CCharacter;
class CContainerActionPatternUnit;


class CActionPattern
{

public:

	CActionPattern();
	virtual ~CActionPattern();

public:

	bool							Create(CCharacter* pBot, CContainerActionPatternUnit* pContainerActionPatternUnit);
	void							Destroy();

	eACTIONPATTERNID				GetCurActionID();

	DWORD							GetWaitTime();

	void							Reset();

	void							ForwardActionPatternIndex();

	BYTE							GetActionPatternIndex();

	void							SendUnitIdx();

	TBLIDX							GetTableIdx();

	bool							IsActionMove();



	CContainerActionPatternUnit*	m_pContainerActionPatternUnit;

private:

	BYTE						m_actionpatternIdx;

	CCharacter*					m_pOwnerBot;

	bool						m_bIsSendSameTimePacket;

};

#endif