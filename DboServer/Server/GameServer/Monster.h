#ifndef __CMONSTER__
#define __CMONSTER__

#include "Npc.h"

class CMonster : public CNpc
{

public:
	CMonster();
	virtual ~CMonster();

private:

	void			Initialize();
	void			Destroy();

protected:

	virtual void		CopyToObjectInfo(sOBJECT_INFO *pObjectInfo, CHARACTERID playerCharID);

public:

	bool				CreateDataAndSpawn(WORLDID worldId, sMOB_TBLDAT* mobTbldat, sSPAWN_TBLDAT* spawnTbldat, bool bSpawnOnServerStart = false, BYTE bySpawnFuncFlag = SPAWN_FUNC_FLAG_RESPAWN);

	bool				CreateDataAndSpawn(sMOB_DATA& sData, sMOB_TBLDAT* mobTbldat, BYTE byActualLevel = INVALID_BYTE, BYTE byEffectLevel = INVALID_BYTE);

	virtual bool		DoTransformation(TBLIDX tblidx, ACTIONPATTERNTBLIDX actionPatternTblidx, TBLIDX tblidxOnlyOneSkillUse);

	virtual void		Spawn(bool bSpawnOnServerStart);

	virtual void		TickProcess(DWORD dwTickDiff, float fMultiple);

public:

	sMOB_TBLDAT*		GetTbldat() { return reinterpret_cast<sMOB_TBLDAT*>(m_pTbldat); }

	BYTE				GetMobRank()  { return GetTbldat()->byGrade; }
	BYTE				GetMobType()  { return GetTbldat()->byMob_Type; }

	float				GetZeniDropRate()  { return GetTbldat()->fDrop_Zenny_Rate; }
	DWORD				GetZeniDrop()  { return GetTbldat()->dwDrop_Zenny; }

	WORD				GetScanRange()  { return GetTbldat()->wScan_Range; }

	SPAWNGROUPID		GetSpawnGroupID() { return m_SpawnGroupID; }

	virtual bool		HasFunction(DWORD dwFuncFlag) { return false; }

	virtual BYTE		GetMaxChainAttack() { return GetTbldat()->byAttack_Animation_Quantity + 1; }

	virtual HOBJECT		ConsiderScanTarget(WORD wRange);

	virtual bool		Faint(CCharacterObject* pkKiller, eFAINT_REASON byReason = FAINT_REASON_HIT);

	CPlayer*			DistributeExp(); //return players which receive exp from obj kill
	void				CreateKillReward(bool bItemDrop);

	HOBJECT				ConsiderScanTargetRingRange(WORD wRingRange);

	bool				HasDragonball() { return m_bHasDragonball; }

	void				ResetDragonball();

	CHARACTERID			GetKillerCharID() { return m_hKillerCharID; }

	void				SetFaintBuff(TBLIDX faintBuffIndex, BYTE byFaintBuffApplyType, float fFaintBuffRange);
	void				SetDropItemProbabilityTblidx(TBLIDX dropItem_ProbabilityTblidx);

	void				FaintBuffReward(CPlayer* pPlayer);

	bool				IsSightAngle(CCharacter* pTarget);

private:

	SPAWNGROUPID		m_SpawnGroupID;

	BYTE				m_byBallType;
	bool				m_bHasDragonball;
	CHARACTERID			m_hKillerCharID;


	TBLIDX				m_faintBuffIndex;
	BYTE				m_byFaintBuffApplyType;
	float				m_fFaintBuffRange;

	TBLIDX				m_dropItem_ProbabilityTblidx;

	BYTE				m_byKillerLevel;

};


#endif