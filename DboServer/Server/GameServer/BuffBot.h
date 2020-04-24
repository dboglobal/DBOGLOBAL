#ifndef __CHAR_BUFF_BOT__
#define __CHAR_BUFF_BOT__

#include "Buff.h"

class CNpc;

class CBuffBot : public CBuff
{

public:

	CBuffBot();
	virtual ~CBuffBot();

public:

	bool						Create(CNpc *pOwnerRef, sBUFF_INFO* pBuffInfo, eSYSTEM_EFFECT_CODE* effectCode, HOBJECT hCaster, eBUFF_TYPE buffType, BYTE byBuffGroup, BYTE* abyEffectType);

private:

	CNpc*						m_pBotRef;

};



#endif