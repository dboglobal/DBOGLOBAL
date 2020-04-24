#ifndef __CHAR_BUFF_PC__
#define __CHAR_BUFF_PC__

#include "Buff.h"

class CPlayer;

class CBuffPc : public CBuff
{

public:

	CBuffPc();
	virtual ~CBuffPc();

public:

	bool						Create(CPlayer *pOwnerRef, sBUFF_INFO* pBuffInfo, eSYSTEM_EFFECT_CODE* effectCode, HOBJECT hCaster, eBUFF_TYPE buffType, BYTE byBuffGroup, BYTE* abyEffectType);

//	void						Destroy();

protected:

//	void						Init();

public:

	virtual bool				OnRegistered(bool bIsLoadedBuff, bool bNeedToDisplayMessage);

	virtual bool				OnRemoved(CBuff::BUFF_REMOVAL_REASON nReason);

private:

	CPlayer*					m_pPlayerRef;

};



#endif