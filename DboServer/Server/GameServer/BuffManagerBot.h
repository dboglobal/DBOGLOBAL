#ifndef __DBOG_BUFFMANAGER_BOT_H__
#define __DBOG_BUFFMANAGER_BOT_H__


#include "BuffManager.h"

class CNpc;

class CBuffManagerBot : public CBuffManager
{
public:

	CBuffManagerBot();
	virtual ~CBuffManagerBot();

public:

	bool							Create(CNpc* pOwnerRef);

public:

	virtual bool					RegisterBuff(DWORD& rdwKeepTime, eSYSTEM_EFFECT_CODE* effectCode, sDBO_BUFF_PARAMETER* paBuffParameter, HOBJECT hCaster, eBUFF_TYPE buffType, sSKILL_TBLDAT* pSkillTbldat, BYTE* prBuffIndex = NULL);

private:

	CNpc*							m_pBotRef;

};

#endif