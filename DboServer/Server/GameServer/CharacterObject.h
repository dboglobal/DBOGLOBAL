#ifndef __INC_DBOG_CHARACTER_OBJECT_H__
#define __INC_DBOG_CHARACTER_OBJECT_H__

#include "MoveableObject.h"
#include "NtlCharacter.h"
#include "BotStateManager.h"
#include "SkillManager.h"
#include "BuffManager.h"
#include "ModelToolCharDataTable.h"
#include "NtlBattle.h"
#include "PCTable.h"
#include "CharacterAtt.h"


//====================================================================
//  CCharacterObject
//  Everything about character as example stats (life points, energy), target, modell data)
//====================================================================
class CCharacterObject : public CMoveableObject
{
public:

	enum eRESETTYPE
	{
		NONE = 0x0,
		FAINT = 0x1,
		INIT_SPAWN = 0x2,
		INVALID = 0x3,
	};

	struct sCHAR_STUN
	{
		sCHAR_STUN()
		{
			Init();
		}

		void	Init()
		{
			hCaster = INVALID_HOBJECT;
			eStunType = DBO_STUN_TYPE_INVALID;
			bCheckCaster = false;
		}

		HOBJECT hCaster;
		eDBO_STUN_TYPE eStunType;
		bool bCheckCaster;
	};

public:
	CCharacterObject(eOBJTYPE eObjType);
	virtual	~CCharacterObject();


	bool			Create(eAIR_STATE eAirState);
	void			Destroy();

	void			Init();

	virtual CStateManager*	CreateStateManager(eAIR_STATE eAirState) { return NULL; }
	virtual CSkillManager*	CreateSkillManager() { return NULL; }
	virtual CBuffManager*	CreateBuffManager() { return NULL; }
	virtual CCharacterAtt*	CreateCharAttManager() { return NULL; }

public:
	virtual void	TickProcess(DWORD dwTickDiff, float fMultiple);

	virtual void	Reset(CCharacterObject::eRESETTYPE eResetType);

	virtual bool	ChangeFightMode(bool bFightMode);
	virtual bool	ChangeAttackProgress(bool bAttackProgress);

	virtual bool	AttackProgress(DWORD dwTickDiff, float fMultiple);

	virtual bool	UpdateCurLP(int lpDiff, bool bIncrease, bool bAutoRecover/*, sLP_TRACE_DATA *rLpTraceData*/);
	virtual bool	UpdateCurEP(WORD wEpDiff, bool bIncrease, bool bAutoRecover);
	virtual bool	UpdateCurRP(WORD wRpDiff, bool bIncrease, bool bHitCharge);
	virtual bool	UpdateCurAP(int apDiff, bool bIncrease);
	virtual bool	UpdateRpBall(BYTE byDiff, bool bIncrease, bool bDropByTime);

	CStateManager*			GetStateManager() { return m_pStateManager; }
	CSkillManager*			GetSkillManager() { return m_pSkillManager; }
	CBuffManager*			GetBuffManager() { return m_pBuffManager; }
	CCharacterAtt*			GetCharAtt() { return m_pCharacterAtt; }

	sMODELTOOL_CHAR_TBLDAT*	GetAniTbldat() { return m_pAniTbldat; }

	void			OnUpdateState();
	void			OnUpdateAspectState();
	void			OnUpdateCondition();

	void			SetTblidx(TBLIDX id) { m_tblidx = id; }
	TBLIDX			GetTblidx() { return m_tblidx; }

	inline bool			IsDespawning()  { return GetStateManager()->IsCharState(CHARSTATE_DESPAWNING); }
	inline bool			IsSpawning()  { return GetStateManager()->IsCharState(CHARSTATE_SPAWNING); }
	inline bool			IsLeaving() { return GetStateManager()->IsCharState(CHARSTATE_LEAVING); }
	inline bool			IsSleeping()  { return GetStateManager()->IsCharState(CHARSTATE_SLEEPING); }
	inline bool			IsStunned()  { return GetStateManager()->IsCharState(CHARSTATE_STUNNED); }
	inline bool			IsParalyzed()  { return GetStateManager()->IsCharState(CHARSTATE_PARALYZED); }
	inline bool			IsCharging()  { return GetStateManager()->IsCharState(CHARSTATE_CHARGING); }
	inline bool			IsSitting()  { return GetStateManager()->IsCharState(CHARSTATE_SITTING); }
	inline bool			IsStanding()  { return GetStateManager()->IsCharState(CHARSTATE_STANDING); }
	inline bool			IsMoving()  { return GetStateManager()->IsCharState(CHARSTATE_MOVING); }
	inline bool			IsDestMoving()  { return GetStateManager()->IsCharState(CHARSTATE_DESTMOVE); }
	inline bool			IsFollowing()  { return GetStateManager()->IsCharState(CHARSTATE_FOLLOWING); }
	inline bool			IsCastingSkill()  { return GetStateManager()->IsCharState(CHARSTATE_CASTING); }
	inline bool			IsSkillAffecting() { return GetStateManager()->IsCharState(CHARSTATE_SKILL_AFFECTING); }
	inline bool			IsKeepingEffect() { return GetStateManager()->IsCharState(CHARSTATE_KEEPING_EFFECT); }
	inline bool			IsKnockedDown() { return GetStateManager()->IsCharState(CHARSTATE_KNOCKDOWN); }
	inline bool			IsFocusing() { return GetStateManager()->IsCharState(CHARSTATE_FOCUSING); }
	inline bool			IsUsingHtb() { return GetStateManager()->IsCharState(CHARSTATE_HTB); }
	inline bool			IsSandbag() { return GetStateManager()->IsCharState(CHARSTATE_SANDBAG); }
	inline bool			IsPrivateShop() { return GetStateManager()->IsCharState(CHARSTATE_PRIVATESHOP); }
	inline bool			IsDirectPlay() { return GetStateManager()->IsCharState(CHARSTATE_DIRECT_PLAY); }

	//stats

	void			SetLevel(BYTE lv) { m_byLevel = lv; }
	BYTE			GetLevel()  { return m_byLevel; }

	bool			SetCurLP(int curLp);
	int				GetCurLP()				{ return m_curLP; }
	bool			SetCurEP(WORD wEP);
	WORD			GetCurEP()				{ return m_wCurEP; }
	bool			SetCurRP(WORD wRP);
	WORD			GetCurRP()				{ return m_wCurRP; }
	bool			SetCurAP(int charAP);
	int				GetCurAP()				{ return m_curAP; }

	bool			ConsiderLPLow(float fLowLPFactor);
	bool			ConsiderEPLow(float fLowLPFactor);

	void			SetMaxLP(int nMaxLp)	{ GetCharAtt()->SetMaxLP(nMaxLp); }
	int				GetMaxLP()				{ return GetCharAtt()->GetMaxLP(); }
	void			SetMaxEP(WORD wMaxEP)	{ GetCharAtt()->SetMaxEP(wMaxEP); }
	WORD			GetMaxEP()				{ return GetCharAtt()->GetMaxEP(); }

	inline float	GetCurLpInPercent()	{ return ((float)m_curLP / (float)GetCharAtt()->GetMaxLP()) * 100.0f; }
	inline float	GetCurEpInPercent()	{ return ((float)m_wCurEP / (float)GetCharAtt()->GetMaxEP()) * 100.0f; }
	inline float	GetCurApInPercent() { return ((float)m_curAP / (float)GetCharAtt()->GetMaxAP()) * 100.0f; }

	virtual float	GetWalkingSpeed()				{ return GetCharAtt()->GetWalkSpeed(); }
	void			SetRunSpeed(float speed)	{ GetCharAtt()->SetRunSpeed(speed); }
	virtual float	GetRunSpeed()				{ return GetCharAtt()->GetRunSpeed(); }
	void			SetFlySpeed(float speed)	{ GetCharAtt()->SetFlySpeed(speed); }
	float			GetFlySpeed()				{ return GetCharAtt()->GetRunSpeed(); }
	void			SetFlyDashSpeed(float speed){ GetCharAtt()->SetFlyDashSpeed(speed); }
	float			GetFlyDashSpeed()			{ return GetCharAtt()->GetFlyDashSpeed(); }
	void			SetFlyAccelSpeed(float speed){ GetCharAtt()->SetFlyAccelSpeed(speed); }
	float			GetFlyAccelSpeed()			{ return GetCharAtt()->GetFlyAccelSpeed(); }

	void			SetAttackSpeedRate(WORD speed){ GetCharAtt()->SetAttackSpeedRate(speed); }
	WORD			GetAttackSpeedRate()		  { return GetCharAtt()->GetAttackSpeedRate(); }

	void			SetSkillAnimationSpeedModifier(float speed){ GetCharAtt()->SetSkillAnimationSpeedModifier(speed); }
	float			GetSkillAnimationSpeedModifier()		  { return GetCharAtt()->GetSkillAnimationSpeedModifier(); }

	inline void		SetAirState(BYTE byState) { m_pStateManager->SetAirState((eAIR_STATE)byState); }
	inline BYTE		GetAirState()	{ return (BYTE)m_pStateManager->GetAirState(); }

	BYTE			GetCharStateID() 	{ return m_pStateManager->GetCharStateID(); }
	BYTE			GetAspectStateId()	{ return m_pStateManager->GetAspectStateID(); }

	QWORD			GetConditionState()	{ return m_pStateManager->GetConditionState(); }

	void			UpdateMoveSpeed(float fWalkSpeed, float fRunSpeed);
	void			UpdateAttackSpeed(WORD wAttackSpeed);

	void			BlockMovementSpeed(); //sets movement speed to 0
	void			UndoMovementBlock(); // removes movement speed block


	DWORD				GetExp() const		{ return m_dwEXP; }
	void				SetExp(DWORD exp)	{ m_dwEXP = exp; }

	inline DWORD		GetZeni() 		{ return m_dwZeni; }
	inline void			SetZeni(DWORD zeni)	{ m_dwZeni = zeni; }

	float				GetScale() { return m_fScale; }
	virtual float		GetObjectRadius() { return m_fObjectRadius; }

	bool				ChangeScale(float fScale);

	sCHAR_TBLDAT*		GetTbldat() { return m_pTbldat; }

	bool				SetRPBall(BYTE byNewRPBall);
	BYTE				GetCurRPBall() { return m_byCurRPBall; }
	void				SetMaxRPBall(BYTE ball) { m_byMaxRPBall = ball; }
	BYTE				GetMaxRPBall() { return m_byMaxRPBall; }

	virtual void		StartIncreaseRP() {}
	virtual void		StopIncreaseRP() {}

	void				SetImmortalMode(BYTE byImmortalMode) { m_byImmortalMode = byImmortalMode; }
	BYTE				GetImmortalMode() { return m_byImmortalMode; }

	virtual bool		IsValidTarget(DWORD dwNoCheckStateFlag);	//check if Im a valid target
	virtual bool		IsAttackable(CCharacterObject* pTarget);	//check if target is attack able

	virtual bool		IsAttackTarget(CCharacterObject* pTarget) { return false; }
	virtual bool		CanApplyAttackAction(DWORD dwAttackActionFlag) { return true; } //eATTACK_ACTION_FLAG

	float				GetAttackRange();

	virtual bool		SetTargetHandle(HOBJECT hHandle);
	HOBJECT				GetTargetHandle() { return m_hTarget; }
	virtual bool		ChangeTarget(HOBJECT hTarget);

	void				ResetAlreadyFainted() { m_bAlreadyFainted = false; }
	inline bool			IsAlreadyFainted() { return m_bAlreadyFainted; }
	virtual bool		IsJustFainting(bool bNeedToCheckLp);
	bool				IsFainting() { return m_pStateManager->GetCharStateID() == CHARSTATE_FAINTING; }
	virtual bool		Faint(CCharacterObject* pkKiller, eFAINT_REASON byReason = FAINT_REASON_HIT);

	bool				GetFightMode() { return m_bFightMode; }
	bool				SetFightMode(bool bFlag);
	bool				GetAttackProgress() { return m_bAttackProgress; }
	bool				SetAttackProgress(bool bFlag);

	bool				NeedToIgnoreDistance() { return m_bNeedToIgnoreDistance; }
	void				SetNeedToIgnoreDistance(bool bFlag) { m_bNeedToIgnoreDistance = bFlag; }

	virtual BYTE		GetMaxChainAttack() { return NTL_BATTLE_CHAIN_ATTACK_START; }

	void				SetLastAttackTime(DWORD dwTime) { m_dwNextAttackTime = dwTime; }

	DWORD				AcquireLpEpEventID();
	DWORD				GetLpEpEventID() { return m_dwLpEpEventID; }
	void				SetLpEpEventID(DWORD dwID) { m_dwLpEpEventID = dwID; }

	sCHAR_STUN &		GetCharStun() { return m_sCharStun; }

protected:

	bool				RefreshObjectRadius();

	virtual void		OnTargetChanged(HOBJECT hOldTarget) = 0;

protected:

	CStateManager*		m_pStateManager;
	CSkillManager*		m_pSkillManager;
	CBuffManager*		m_pBuffManager;
	CCharacterAtt*		m_pCharacterAtt;

	sMODELTOOL_CHAR_TBLDAT* m_pAniTbldat;

protected:

	float	m_fObjectRadius;

	TBLIDX	m_tblidx;			// TABLE TBLIDX

	BYTE	m_byCurRPBall;
	BYTE	m_byMaxRPBall;

	BYTE	m_byLevel;
	DWORD	m_dwEXP;
	DWORD	m_dwZeni;

	int		m_curLP;
	WORD	m_wCurEP;
	WORD	m_wCurRP;
	int		m_curAP;

	DWORD	m_dwLpEpEventID;

	HOBJECT	m_hTarget;

	bool	m_bAlreadyFainted;

	bool	m_bAttackProgress;
	bool	m_bFightMode;

	bool	m_bNeedToIgnoreDistance;

	float	m_fWalkSpeedBeforeBlock;
	float	m_fRunSpeedBeforeBlock;

protected:

	DWORD			m_dwNextAttackTime;
	DWORD			m_dwSkillBlockTime; //time until when skill usage is blocked (GetTickCount() + skill animation time)
	BYTE			m_byChainSequence;

	float			m_fScale;

	BYTE			m_byImmortalMode;
	sCHAR_STUN		m_sCharStun;

	sCHAR_TBLDAT*	m_pTbldat;

};

#endif
