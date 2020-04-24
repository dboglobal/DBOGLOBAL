#ifndef __DBOG_BUFFMANAGER_PC_H__
#define __DBOG_BUFFMANAGER_PC_H__


#include "BuffManager.h"

class CPlayer;
class QueryBuffer;

class CBuffManagerPc : public CBuffManager
{
public:

	CBuffManagerPc();
	virtual ~CBuffManagerPc();

public:

	bool							Create(CPlayer* pOwnerRef);

public:

	bool							RegisterLoadBuff(sBUFF_INFO & rBuffInfo, BYTE byBuffGroup);

	virtual bool					RegisterSubBuff(sBUFF_INFO* pBuffInfo, eSYSTEM_EFFECT_CODE* effectCode, HOBJECT hCaster, BYTE byBuffGroup, WORD & rwResultCode, BYTE* abyEffectType);

	virtual bool					RegisterBuff(DWORD& rdwKeepTime, eSYSTEM_EFFECT_CODE* effectCode, sDBO_BUFF_PARAMETER* paBuffParameter, HOBJECT hCaster, eBUFF_TYPE buffType, sSKILL_TBLDAT* pSkillTbldat, BYTE* prBuffIndex = NULL);

	virtual void					_SaveBuffCooldowns();

public:

	void							CheckSubBuffs(BYTE byWorldRuleType);

	void							RemoveBlessBuffsFromCaster(HOBJECT hCaster);


private:

	CPlayer*						m_pPlayerRef;

};

#endif