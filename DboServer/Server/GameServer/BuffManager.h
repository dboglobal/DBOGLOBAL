#ifndef __DBOG_BUFFMANAGER_H__
#define __DBOG_BUFFMANAGER_H__


#include "Buff.h"
#include "NtlLinkArray.h"
#include "NtlLinkList.h"

struct sSKILL_TBLDAT;
struct sUSE_ITEM_TBLDAT;
struct sITEM_TBLDAT;
class CSkill;
struct sAVATAR_ATTRIBUTE;
class QueryBuffer;
class CCharacter;
class CCharacterAtt;


class CBuffManager
{

public:

	CBuffManager();
	virtual ~CBuffManager();

public:

	bool							Create(CCharacter* pOwnerRef);

	void							Destroy();

protected:

	void							Init();

public:

	virtual void					TickProcess(DWORD dwTickDiff);

public:

	void							LeaveGame();

	virtual bool					RegisterBuff(DWORD& rdwKeepTime, eSYSTEM_EFFECT_CODE* effectCode, sDBO_BUFF_PARAMETER * paBuffParameter, HOBJECT hCaster, eBUFF_TYPE buffType, sSKILL_TBLDAT* pSkillTbldat, BYTE* prBuffIndex = NULL);

	virtual bool					RegisterSubBuff(sBUFF_INFO* pBuffInfo, eSYSTEM_EFFECT_CODE* effectCode, HOBJECT hCaster, BYTE byBuffGroup, WORD & rwResultCode, BYTE* abyEffectType);

	void							EnableDisableBuff(BYTE byBuffIndex, eBUFF_TYPE buffType); //disables buff if enabled / enable buff if disabled

	void							RemoveBuff(BYTE byBuffIndex, eBUFF_TYPE buffType, CBuff::BUFF_REMOVAL_REASON nReason);
	void							RemoveBuffTS(BYTE byBuffIndex, eBUFF_TYPE buffType, CBuff::BUFF_REMOVAL_REASON nReason);

	void							EndBuff(TBLIDX sourceTblidx, CBuff::BUFF_REMOVAL_REASON nReason);
	void							EndBuffTS(TBLIDX sourceTblidx, CBuff::BUFF_REMOVAL_REASON nReason);

	void							EndBuff(eSYSTEM_EFFECT_CODE effectcode);
	void							EndBuffs(eSYSTEM_EFFECT_CODE effectcode);

	void							EndBuff(BYTE byBuffIndex, CBuff::BUFF_REMOVAL_REASON nReason);
	void							EndBuffTS(BYTE byBuffIndex, eBUFF_TYPE buffType);

	void							EndBlessBuff(eSYSTEM_EFFECT_CODE effectcode);
	void							EndBlessBuffTS(eSYSTEM_EFFECT_CODE effectcode);
	void							EndSubBuff(eSYSTEM_EFFECT_CODE effectcode, eSYSTEM_EFFECT_CODE effectcode2);
	void							RemoveBlessBuff(TBLIDX skillTblidx);
	void							RemoveRandomBlessBuff();

	void							EndCurseBuff(eSYSTEM_EFFECT_CODE effectcode);
	void							EndCurseBuffTS(eSYSTEM_EFFECT_CODE effectcode);

	bool							CheckAndApplyOtherStun(BYTE byEndState, WORD wEffectCodeEnd);
	void							OnLoginApplyBuffs();

	bool							LoadBuff(CBuff* pBuff);

	bool							AddBuff(CBuff* pBuff, bool bNeedToDisplayMessage);

	CBuff *							FindBuff(BYTE byBuffIndex);
	CBuff *							FindBuff(BYTE byBuffGroup, BYTE bySourceType);
	CBuff *							FindBuff(TBLIDX sourceTblidx, BYTE bySourceType);
	CBuff *							FindBuff(eSYSTEM_EFFECT_CODE effectcode, eSYSTEM_EFFECT_CODE effectcode2 = INVALID_SYSTEM_EFFECT_CODE);
	CBuff *							FindAnyBuff(eSYSTEM_EFFECT_CODE effectcode); //find any buf with the effect code

	CBuff *							FindSubBuff(BYTE byBuffGroup, BYTE bySourceType);

	CBuff *							FindCurse(eSYSTEM_EFFECT_CODE effectcode, DWORD dwLeastDuration);

	bool							HasAirSkillBuff();

	void							RemoveAllBuff();
	void							RemoveAllBuffExceptApplySelf();

	void							RemoveAllCurseBuff();

	void							RemoveAllStun();

	bool							RemoveOldestCurseBuff();
	bool							RemoveRandomCurseBuff(bool bDontRemoveBodyCurse = false);

	int								GetNumberOfBuff() { return m_listBuffOrderedByRegisteredTime.GetCount(); }

	int								GetSubBuffCount() { return (int)m_subBuffList.GetSize(); }

	void							CopyBuffAttributesTo(CCharacterAtt* pCharAtt);

	void							SendBuffInfo(CCharacter* pPlayer); //send owner buff infos to pPlayer

protected:

	bool							AddBuff_Internal(CBuff* pBuff, CNtlLinkArray_Static<CBuff*> & rlistBuff, int nMaxBuffCount, bool bIsLoadedBuff, bool bNeedToDisplayMessage);

	unsigned int					RemoveBuff_Internal(CBuff::BUFF_REMOVAL_REASON nReason, CNtlLinkArray_Static<CBuff *> *plistBuff, unsigned int iterBuff);

protected:


	CCharacter *					m_pOwnerRef;

	CBuff *							m_apBuff[DBO_MAX_BUFF_CHARACTER_HAS];

	CNtlLinkArray_Static<CBuff *>	m_blessBuffList;

	CNtlLinkArray_Static<CBuff *>	m_curseBuffList;

	CNtlLinkArray_Static<CBuff *>	m_subBuffList;

	CNtlLinkList					m_listBuffOrderedByRegisteredTime;
};


#endif