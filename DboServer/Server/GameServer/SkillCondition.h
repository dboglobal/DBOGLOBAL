#ifndef __DBOG_SKILL_CONDITION__
#define __DBOG_SKILL_CONDITION__

#include "SkillBot.h"


class CNpc;
class CCharacter;
class CWorldCell;
enum eOBJTYPE;
class CSpellAreaChecker;


class CSkillCondition
{
public:

	CSkillCondition();
	virtual ~CSkillCondition();

public:

	void						Destroy();

	virtual CSkillBot*			OnUpdate(DWORD dwTickTime);

	bool						GetTarget(HOBJECT & hTarget, sSKILL_TARGET_LIST & rTargetList);

private:

	void						SkillAppointTargetSelf(sSKILL_TARGET_LIST & rTargetList);

	void						SkillAppointTargetTarget(HOBJECT & hTarget, sSKILL_TARGET_LIST & rTargetList);

	virtual void				AppointTargetSelf_ApplyTargetSelf(sSKILL_TARGET_LIST & rTargetList);

	virtual void				AppointTargetSelf_ApplyTargetEnemy(sSKILL_TARGET_LIST & rTargetList);

	virtual void				AppointTargetSelf_ApplyTargetParty(sSKILL_TARGET_LIST & rTargetList);

	virtual void				AppointTargetTarget_ApplyTargetSelf(HOBJECT & hTarget, sSKILL_TARGET_LIST & rTargetList);

	virtual void				AppointTargetTarget_ApplyTargetEnemy(HOBJECT & hTarget, sSKILL_TARGET_LIST & rTargetList);

	virtual void				AppointTargetTarget_ApplyTargetParty(HOBJECT & hTarget, sSKILL_TARGET_LIST & rTargetList);

public:

	void						GetTargetApplyRange_Cell(eOBJTYPE byObjType, CWorldCell *pCell, CSpellAreaChecker& cSpellAreaChecker, sSKILL_TARGET_LIST& rTargetList, BYTE byMaxTargetCount);

	void						GetTargetApplyRange_Cell(CWorldCell *pCell, CSpellAreaChecker& cSpellAreaChecker, sSKILL_TARGET_LIST& rTargetList, BYTE byMaxTargetCount);

	void						GetTarget_ApplyRange_PCandNPC(CCharacter *pAppointTarget, sSKILL_TARGET_LIST& rTargetList);

	void						GetTarget_ApplyRange_Bot(CCharacter *pAppointTarget, sSKILL_TARGET_LIST& rTargetList);

	void						GetTarget_ApplyRange_Party(CCharacter *pAppointTarget, sSKILL_TARGET_LIST& rTargetList, BYTE byMaxTargetCount);

	void						GetTarget_ApplyRange_Party_LPLow(CCharacter *pAppointTarget, sSKILL_TARGET_LIST& rTargetList, BYTE byMaxTargetCount);

	BYTE						GetApplyRangeType() { return m_pSkill->GetOriginalTableData()->byApply_Range; }

	BYTE						GetApplyAreaSize1() { return m_pSkill->GetOriginalTableData()->byApply_Area_Size_1; }

	BYTE						GetApplyAreaSize2() { return m_pSkill->GetOriginalTableData()->byApply_Area_Size_2; }

	BYTE						GetTargetMaxCount() { return m_pSkill->GetOriginalTableData()->byApply_Target_Max; }
	
	bool						IsApplyNotMe() { return (m_pSkill->GetOriginalTableData()->dwFunction_Bit_Flag & SKILL_FUNCTION_FLAG_APPLY_NOT_ME) != 0; }

	CNpc*						GetBot() { return m_pBot; }

	CSkillBot*					GetSkill() { return m_pSkill; }

	TBLIDX						GetSkillTblidx() { return m_Skill_Tblidx; }
	BYTE						GetSkillBasis() { return m_byUse_Skill_Basis; }
	WORD						GetSkillLP() { return m_wUse_Skill_LP; }
	WORD						GetSkillTime() { return m_wUse_Skill_Time; }

	void						SetBot(CNpc* pBot) { m_pBot = pBot; }

	void						SetSkill(CSkillBot* pSkill) { m_pSkill = pSkill; }

	void						SetSkillTblidx(TBLIDX skillTblidx) { m_Skill_Tblidx = skillTblidx; }
	void						SetSkillBasis(BYTE byUseSkillBasis) { m_byUse_Skill_Basis = byUseSkillBasis; }
	void						SetSkillLP(WORD wUseSkillLP) { m_wUse_Skill_LP = wUseSkillLP; }
	void						SetSkillTime(WORD wUseSkillTime) { m_wUse_Skill_Time = wUseSkillTime * 1000; }
	
	void						SetSkillConditionIdx(BYTE bySkillConditionIdx) { m_bySkillConditionIdx = bySkillConditionIdx; }
	BYTE						GetSkillConditionIdx() { return m_bySkillConditionIdx; }

	void						SetCanUseSkill(bool bFlag) { m_bCanUse = bFlag; }
	bool						GetCanUseSkill() { return m_bCanUse; }

protected:

	HOBJECT						IsObjectInApplyRingRange();


protected:

	CNpc *						m_pBot;

	CSkillBot *					m_pSkill;

	TBLIDX						m_Skill_Tblidx;

	BYTE						m_byUse_Skill_Basis;

	WORD						m_wUse_Skill_LP;

	WORD						m_wUse_Skill_Time;

	DWORD						m_dwTime;

	BYTE						m_bySkillConditionIdx;

	bool						m_bCanUse;

};

#endif