#ifndef __CITEMPET__
#define __CITEMPET__

#include "MascotGradeTable.h"
#include "MascotStatusTable.h"
#include "EventableObject.h"

class CPlayer;

class CItemPet : public EventableObject
{

public:
	CItemPet();
	virtual ~CItemPet();

private:

	void				Initialize();
	void				Destroy();

private:

	sMASCOT_STATUS_TBLDAT*		pet_tbldat;
	sMASCOT_GRADE_TBLDAT*		mascotGradeTbldat;

	CPlayer*					m_pOwner;

	sMASCOT_DATA_EX				m_mascotData;

public:

	void						TickProcess(DWORD dwTickDiff);

	void						Summon();
	void						UnSummon();

	void						SetSkillCooldown(BYTE bySkillindex, DWORD dwTime);
	void						ResetSkillCooldown(BYTE bySkillindex);
	bool						IsSkillOnCooldown(BYTE bySkillindex);

	void						OnEvent_VpRegen();

public:

	sMASCOT_STATUS_TBLDAT*		GetTbldat() { return pet_tbldat; }
	sMASCOT_GRADE_TBLDAT*		GetGradeTbldat() { return mascotGradeTbldat; }

	inline TBLIDX				GetTblidx() { return pet_tbldat->tblidx; }

	void						SetPetData(sMASCOT_STATUS_TBLDAT* pettable, CPlayer* owner, sMASCOT_DATA_EX* mascotData);
	CPlayer*					GetOwner() { return m_pOwner; }
	BYTE						GetSourceType() const { return m_bySourceType; }
	TBLIDX						GetNpcTblidx() const { return m_npcTblidx; }

	void						UpdateExp(DWORD dwPlusExp);

	sMASCOT_DATA_EX*			GetMascotData() { return &m_mascotData; }

	void						LearnRandomSkill(BYTE bySlot);
	void						UpdateSkill(BYTE bySlot, BYTE byItemPlace, BYTE byItemPos);
	void						UpgradeSkill(BYTE bySlot, BYTE byItemPlace, BYTE byItemPos);
	TBLIDX						GetDashEpReduceSkill();

	DWORD						GetExp() { return m_mascotData.dwCurExp; }
	DWORD						GetNeedExp() { return mascotGradeTbldat->dwNeedExp; }
	bool						CanLevelUp() { return pet_tbldat->nextMascotTblidx != INVALID_TBLIDX; }

	void						LevelUp();

	void						SetCurVP(DWORD dwVP) { m_mascotData.dwCurVP = dwVP; }
	DWORD						GetCurVP() { return m_mascotData.dwCurVP; }
	DWORD						GetMaxVP() { return m_mascotData.dwMaxVP; }
	bool						IsVpFull() { return m_mascotData.dwCurVP == m_mascotData.dwMaxVP; }
	WORD						UpdateVP(DWORD dwVP);

	bool						ConsiderVPLow(float fLowVPFactor);

	inline void					SetCanLoot(bool bFlag) { m_bCanLoot = bFlag; }
	inline bool					CanLoot() { return m_bCanLoot; }

	inline void					SetCanRemoteSell(bool bFlag) { m_bCanRemoteSell = bFlag; }
	inline bool					CanRemoteSell() { return m_bCanRemoteSell; }

private:

	BYTE						m_bySourceType;
	TBLIDX						m_npcTblidx;

	DWORD						m_dwSkillCooldown[DBO_MASCOT_MAX_SKILL_COUNT];

	bool						m_bCanLoot;
	bool						m_bCanRemoteSell;
};



#endif