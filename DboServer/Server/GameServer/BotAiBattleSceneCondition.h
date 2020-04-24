#ifndef __AI_DBOG_BATTLESCENECONDITION_H__
#define __AI_DBOG_BATTLESCENECONDITION_H__


#include "BotAiCondition.h"


class CBotAiBattleSceneCondition : public CBotAiCondition
{

public:

	CBotAiBattleSceneCondition(CNpc* pBot, eBOTCONTROLID controlID, const char *lpszControlName);
	virtual	~CBotAiBattleSceneCondition();

	enum eSTATUS_BATTLESCENE
	{
		BSCONDITION_TRUE = 0x9,
		BSCONDITION_FALSE,
		STATUS_BATTLESCENE_COUNT,
	};


public:

	virtual int		UpdateSubControlList(DWORD dwTickDiff, float fMultiple);

public:

	bool			m_bOperatorAnd;

};

#endif