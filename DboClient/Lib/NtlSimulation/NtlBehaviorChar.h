/*****************************************************************************
 *
 * File			: NtlBehaviorChar.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 2. 9	
 * Abstract		: character behavior class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_BEHAVIOR_CHAR_H__
#define __NTL_BEHAVIOR_CHAR_H__

#include "NtlBehaviorBase.h"
#include "NtlBehaviorData.h"
#include "NtlAnimEventData.h"

class CNtlSobProxySystemEffect;
class CNtlPLCharacter;
class CNtlSobVehicle;

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBehaviorCharSpawn : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharSpawn, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharSpawn)

private:

	enum ESpawnType
	{
		SPAWN_NORMAL,
		SPAWN_SPECIAL,
		SPAWN_TMQ_IN
	};

	RwUInt8	m_byType;
	RwBool	m_bEnterShow;    
	RwBool	m_bSpawnAnimExist;
	RwReal	m_fTime;    
    RwReal	m_fSpawnDelayTime;

	CNtlSobProxySystemEffect	*m_pVSEAlpha;

	void	SetSpawnType(void);
	
	void	SetNormalSpawnAnim(RwReal fSpawnTime);
	void	SetNormalSpawnVisualEffect(void);

	void	SetSpecialSpawnVisualEffect(void);

	void	EnterNormalSpawn(void);
	void	EnterSpecialSpawn(void);
	void	EnterTMQInSpawn(void);

	void	ExitNormalSpawn(void);
	void	ExitTMQInSpawn(void);

	void	UpdateNormalSpawn(RwReal fElapsed);
	void	UpdateTMQSpawn(RwReal fElapsed);
	
public:

	CNtlBehaviorCharSpawn(); 
	~CNtlBehaviorCharSpawn();

	virtual void		Enter(void);
	virtual void		Exit(void);
	virtual void		Update(RwReal fElapsed);
	virtual RwUInt32	HandleEvents(RWS::CMsg &pMsg);
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBehaviorSmoothHeading : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorSmoothHeading, 1)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorSmoothHeading)

private:

	RwReal m_fDeltaAngleY;
	RwReal m_fUpdateAngleY;

	bool	m_bSetAngleX;
	RwReal m_fDeltaAngleX;
	RwReal m_fUpdateAngleX;

	void SetAnim(RwReal fDeltaAngleY, RwReal fDeltaAngleX);

public:

	CNtlBehaviorSmoothHeading();
	~CNtlBehaviorSmoothHeading();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual void UpdateData(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 일반적인 공격.

class CNtlBehaviorCharPushFollow;

class CNtlBehaviorCharNormalAttack : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharNormalAttack, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharNormalAttack)

protected:

#define NTL_MAX_HITGROUP	NTL_MAX_NUMBER_OF_SKILL_TARGET
#define NTL_MAX_HITNUM		16
	
	RwV3d			m_vPushDir;
	RwInt32			m_iHitGroupNum;
	RwInt32			m_iHitNum;
	RwUInt8			m_iHitIdx;
	SHitStuff		*m_ppHit[NTL_MAX_HITGROUP][NTL_MAX_HITNUM];
	RwReal			m_fHitTime[NTL_MAX_HITNUM];
	RwReal			m_fAnimTime;					/** attack animation 전체 시간 */
	RwReal			m_fAnimSpeed;					/** attack animation speed */

	CNtlBehaviorCharPushFollow *m_pPushFollow;


protected:

	void				Init(void);
	void				Delete(void);
		
	void				SetTransform(const SAttackStuff *pAttackStuff);
	void				SetAnim(const SAttackStuff *pAttackStuff);
	void				SetAnimNoAttackStuff(void);
	void				CalcHitStuff(const SAttackStuff *pAttackStuff, RwInt8 byHitGroupIdx);
	RwUInt8				CalcPowerHitNum(void);
	SAttackDamageStuff	CalcDamage(SAttackDamageStuff sTotDamageStuff, RwUInt8 byTotHitNum, RwUInt8 byPowerHitNum, RwBool bPowerHit);

	void				CreatePushFollow(RwBool bOneStepFollow);
	void				DeletePushFollow(void);

	void				EventProcPhysicHit(SHitStuff *pHitStuff, RwUInt8 byHitIdx);
	void				EventProcProjectileHit(SHitStuff *pHitStuff, RwUInt8 byHitIdx);
	void				EventProcHit(RwBool bOneStepFollow);
	void				EventProcSkillAffectingCancel(void);

public:

	CNtlBehaviorCharNormalAttack();
	~CNtlBehaviorCharNormalAttack();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void UpdateData(void);
};





///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 일반적인 공격.

class CNtlBehaviorCharPushFollow : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharPushFollow, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharPushFollow)

protected:

	RwReal m_fPushSpeed;
	RwReal m_fCurrDist;
	RwV3d m_vPushDir;

public:

	CNtlBehaviorCharPushFollow();
	~CNtlBehaviorCharPushFollow();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	
public:

	void SetPushSpeed(RwReal fPushSpeed);
	void SetPushDir(const RwV3d *pPushDir);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 

class CNtlBehaviorCharPush : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharPush, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharPush)

public:
    CNtlBehaviorCharPush();
    ~CNtlBehaviorCharPush();

    virtual void Enter(void);
    virtual void Update(RwReal fElapsed);
    virtual void Exit(void);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
    virtual void UpdateData(void);

private:
    void CreateTimeSequence(void);          ///< push sequence node 생성 함수.
    void DeleteTimeSequence(void);          ///< push sequence node 삭제 함수. 
    RwReal GetMaxTime(void) const;          ///< 객체의 life time.
    RwReal GetMaxDist(void) const;          ///< 움직임의 최대 거리.
    void   UpdatePosition(RwReal fElapsed);                ///< 위치를 업데이트한다.

private:
	RwV3d m_vPushDir;		/** push 방향 */
	RwReal m_fPrevDist;     /** 시간에 따른 distance를 사용하므로, 이전의 dist를 보관한다. */
	RwReal m_fTime;			/** push 누적 time */
	RwReal m_fDistScale;    /** time sequence는 시간과 거리가 고정, push 거리가 고정이 아니므로 */
	RwV3d  m_vDestPos;		/** push로 인하여 움직일 최종 위치 */

	CNtlDistTimeSequence *m_pSeq;	/** push sequence node */
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 

class CNtlBehaviorCharSliding : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharSliding, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharSliding)

private:

	enum 
	{
		SS_SLIDING_LADING,
		SS_SLIDING_STOP
	};
	
	RwUInt32 m_uiSlidingState;	/** sliding state */
	RwV3d m_vPushDir;			/** sliding 방향 */
	RwReal m_fPrevDist;		    /** 시간에 따른 distance를 사용하므로, 이전의 dist를 보관한다. */
	RwReal m_fTime;				/** sliding 누적 time */
	RwReal m_fDistScale;		/** time sequence는 시간과 거리가 고정, sliding 거리가 고정이 아니므로 */
	RwV3d  m_vDestPos;			/** sliding로 인하여 움직일 최종 위치 */

	CNtlDistTimeSequence *m_pSeq;	/** sliding sequence node */

private:

	/**
	* sliding sequence node 생성 함수.
	*/
	void CreateTimeSequence(void);

	/**
	* sliding sequence node 삭제 함수. 
	*/
	void DeleteTimeSequence(void);

	/**
	* 객체의 life time.
	*/
	RwReal GetMaxTime(void) const;

	/**
	* 움직임의 최대 거리.
	*/
	RwReal GetMaxDist(void) const;

	/**
	* update sliding landing.
	*/
	void UpdateSlidingLanding(RwReal fElapsed);

	/**
	* update sliding stop.
	*/
	void UpdateSlidingStop(RwReal fElapsed);

	/**
	* sliding 좌표 계산.
	*/
	void UpdatePosition(RwReal fTime);

	/**
	* sliding 상태 바꾸는 함수.
	*/
	void ChangeSlidingState(RwUInt32 uiState);

public:

	/**
	* 생성자
	*/
	CNtlBehaviorCharSliding();

	/**
	* 소멸자
	*/
	~CNtlBehaviorCharSliding();

	virtual void Enter(void);
	virtual void Update(RwReal fElapsed);
	virtual void Exit(void);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

	/**
	* 객체는 CNtlBeCharData 객체 정보를 참조하여 사용하기 때문에, sliding 객체 update시 사용.
	* 현재 sliding behavior가 동작중인 상태에서, 다른 sliding 명령을 받았을 때. 정보 update.
	*/
	virtual void UpdateData(void);

public:

	void SetSlidingDir(const RwV3d *pSlidingDir);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 

class CNtlBehaviorCharShock : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharShock, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharShock)

private:

	RwV3d m_vPushDir;		/** shock 방향 */
	RwV3d m_vDestPos;		/** shock로 인하여 움직일 최종 위치 */
	RwReal m_fTime;			/** shock 누적 time */
	RwReal m_fDistScale;    /** time sequence는 시간과 거리가 고정, shock 거리가 고정이 아니므로 */
		
	CNtlDistTimeSequence *m_pSeq;

private:

	/**
	* hit shock sequence node 생성 함수.
	*/
	void CreateTimeSequence(void);

	/**
	* hit shock sequence node 삭제 함수. 
	*/
	void DeleteTimeSequence(void);

	/**
	* hit shock 객체 data reset 함수.
	*/
	void ResetTimeSequence(void);

	/**
	* time sequence node update 함수.
	* param1 : 바로 이전의 누적 시간.
	* param2 : 현재의 누적 시간.
	*/
	void UpdateTimeSequence(RwReal fOldTime, RwReal fCurrTime);

	/**
	* 객체의 life time.
	*/
	RwReal GetMaxTime(void) const;

	/**
	* 움직임의 최대 거리.
	*/
	RwReal GetMaxDist(void) const;

public:

	/**
	* 생성자.
	*/
	CNtlBehaviorCharShock();

	/**
	* 소멸자.
	*/
	~CNtlBehaviorCharShock();

	virtual void Enter(void);
	virtual void Update(RwReal fElapsed);
	virtual void Exit(void);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

	/**
	* 객체는 CNtlBeCharData 객체 정보를 참조하여 사용하기 때문에, shock 객체 update시 사용.
	* 현재 shock behavior가 동작중인 상태에서, 다른 shock 명령을 받았을 때. 정보 update.
	*/
	virtual void UpdateData(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//
class CNtlBehaviorCharKnockDown : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharKnockDown, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharKnockDown)

public:
	enum 
	{
		KD_FALLUP,
		KD_LANDING,
		KD_FALLDOWN,
		KD_KNOCK,
		KD_KNOCK_EXIT
	};

public:
    CNtlBehaviorCharKnockDown();
    ~CNtlBehaviorCharKnockDown();

    virtual void Enter(void);
    virtual void Update(RwReal fElapsed);
    virtual void Exit(void);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

    /**
    * 객체는 CNtlBeCharData 객체 정보를 참조하여 사용하기 때문에, knock down 객체 update시 사용.
    * 현재 knock down behavior가 동작중인 상태에서, 다른 knock down 명령을 받았을 때. 정보 update.
    */
    virtual void UpdateData(void);
    void ChangeKnockDownState(RwUInt32 uiState);    ///< knockdown bahavior 의 state를 전이시키는 함수.
    void SetKnockDownDir(const RwV3d *pKnockDownDir);   ///< knock down direction.

private:
	void CreateFallupTimeSequence(void);    ///< knockdown fall up sequence node 생성 함수.
	void CreateLandingTimeSequence(void);   ///< knockdown landing sequence node 생성 함수.
	void DeleteTimeSequence(void);          ///< knockdown sequence node 삭제 함수. 
	void UpdateFallUp(RwReal fElapsed);     ///< knockdown fall up update 함수.
	void UpdateLanding(RwReal fElapsed);    ///< knockdown landing update 함수.
	void UpdateFallDown(RwReal fElapsed);   ///< knockdown fall down update 함수.
	void UpdateKnock(RwReal fElapsed);      ///< knockdown의 쓰러져 있을 때 update 함수.
	void CalcLanding(RwReal fElapsed);      ///< landing 상태 계산.
    RwV3d UpdatePosition(RwReal fElapsed, RwV3d& vPos, RwReal fSpeed);   ///< y값을 제외한 위치 계산 (충돌 처리)

protected:
    RwUInt32 m_uiKnockDownState;		/** knock down state */
    RwBool m_bStanding;					/** knock down standing */
    RwBool m_bFainting;					/** knock down fainting */

    RwReal m_fTime;						/** known down 누적 time */
    RwV3d m_vKnockDownDir;				/** knock down 방향 */
    RwV2d m_vPrevDist;					/** 시간에 따른 distance를 사용하므로, 이전의 dist를 보관한다. */
    RwV2d m_vFallDelta;					/** 떨어지는 delta dist */
    RwReal m_fDistScale;				/** time sequence는 시간과 거리가 고정, push 거리가 고정이 아니므로 */

    CNtl2DTimeSequence *m_pSeq;				/** knock down fall up sequence node */
    CNtlDistTimeSequence *m_pLandingSeq;	/** knock donw landing sequence node */
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBehaviorCharHurt : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharHurt, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharHurt)

private:

	void SetAnim(SHitStuff *pHitStuff);
	void SetTrans(SHitStuff *pHitStuff);

public:

	CNtlBehaviorCharHurt();
	~CNtlBehaviorCharHurt();

	virtual void Enter(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void UpdateData(void);
};


///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBehaviorCharHurtTossUp : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharHurtTossUp, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharHurtTossUp)

protected:

	enum
	{
		TOSS_UP_UP,
		TOSS_UP_IDLE,
		TOSS_UP_DOWN,
	};

	RwUInt8 m_byTossUpState;
	CNtlDistTimeSequence *m_pSeq;		// time sequence
	RwReal m_fTime;
	RwReal m_fPrevDist;

private:

	/**
	* animation setting
	*/
	void SetAnim(void);

	/**
	* push sequence node 생성 함수.
	*/
	void CreateTimeSequence(void);

	/**
	* push sequence node 삭제 함수. 
	*/
	void DeleteTimeSequence(void);

	/**
	* toss update up
	*/
	void UpdateUp(RwReal fElapsed);

	/**
	* toss update idle
	*/
	void UpdateIdle(RwReal fElapsed);

	/**
	* toss update down
	*/
	void UpdateDown(RwReal fElapsed);

public:

	CNtlBehaviorCharHurtTossUp();
	~CNtlBehaviorCharHurtTossUp();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void UpdateData(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBehaviorCharHurtTossDown : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharHurtTossDown, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharHurtTossDown)

protected:

	enum
	{
		TOSS_DOWN_DOWN,
		TOSS_DOWN_IDLE,
	};

	RwUInt8 m_byTossDownState;
	
private:

	/**
	* animation setting
	*/
	void SetAnim(void);

	/**
	* toss update up
	*/
	void UpdateDown(RwReal fElapsed);

	/**
	* toss update idle
	*/
	void UpdateIdle(RwReal fElapsed);

public:

	CNtlBehaviorCharHurtTossDown();
	~CNtlBehaviorCharHurtTossDown();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void UpdateData(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 수직 상승.(target을 따라간다)

class CNtlBehaviorCharHomingUp : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharHomingUp, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharHomingUp)

private:

	RwUInt8 m_bySpeedSearch;
	RwV3d m_vOldTargetPos;

private:

	/**
	* animation setting
	*/
	void SetAnim(void);


public:

	CNtlBehaviorCharHomingUp();
	~CNtlBehaviorCharHomingUp();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void UpdateData(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 수직 하강.

class CNtlBehaviorCharHomingDown : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharHomingDown, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharHomingDown)

private:

	CNtlDistTimeSequence	*m_pSeq;		// time sequence
	RwReal					m_fTime;
	RwReal					m_fPrevDist;

	RwBool					m_bLandingEnd;
	RwBool					m_bLandingAnimPlay;

	RwBool					m_bHomingDown;

private:

	/**
	* animation setting
	*/
	void SetAnim(void);


	/**
	* push sequence node 생성 함수.
	*/
	void CreateTimeSequence(void);

	/**
	* push sequence node 삭제 함수. 
	*/
	void DeleteTimeSequence(void);

	void UpdateHomingDown(RwReal fElapsed);

	void UpdateIdleDown(RwReal fElapsed);

public:

	CNtlBehaviorCharHomingDown();
	~CNtlBehaviorCharHomingDown();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void UpdateData(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// 수직 상승(target을 따라가지 않고) 일정한 높이를 jumping 한다.

class CNtlBehaviorCharJumpingUp : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharJumpingUp, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharJumpingUp)

private:

	CNtlDistTimeSequence *m_pSeq;		// time sequence
	RwReal m_fTime;
	RwReal m_fPrevDist;

private:

	/**
	* animation setting
	*/
	void SetAnim(void);

	/**
	* push sequence node 생성 함수.
	*/
	void CreateTimeSequence(void);

	/**
	* push sequence node 삭제 함수. 
	*/
	void DeleteTimeSequence(void);


public:

	CNtlBehaviorCharJumpingUp();
	~CNtlBehaviorCharJumpingUp();

	virtual void Enter(void);
	virtual void Exit(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
	virtual void UpdateData(void);
};
//////////////////////////////////////////////////////////////////////////

/**
 * \ingroup NtlSimulation
 * \brief 나메크의 마신완 스킬. 타겟을 끌어당긴다.
 *
 * \date 2008-10-17
 * \author agebreak
 */
class CNtlBehaviorCharStretchAttack : public CNtlBehaviorBase
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorCharStretchAttack, NTL_DEFAULT_MEMORY_POOL)
    DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharStretchAttack)

public:
    CNtlBehaviorCharStretchAttack();
    ~CNtlBehaviorCharStretchAttack();

    virtual void Enter(void);
    virtual void Exit(void);
    virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);
    virtual void UpdateData(void);

protected:
    RwReal  GetTargetDistance(CNtlSobActor* pTargetActor);              ///< 당겨오는 타겟과의 거리를 계산한다 (Radius로 비교)

protected:
    CNtlPLCharacter*    m_pCharacter;           ///< 사용자의 엔티티 객체    
    RwUInt32            m_uiTargetId;           ///< 타겟 시리얼 ID
    RwV3d               m_vTargetPos;           ///< 처음 이벤트가 발생했을때의 타겟 위치
    RwBool              m_bPulling;             ///< 당기는 상태인지 뻗는상태인지 플래그 (FALSE 뻗는상태, TRUE 당겨오는 상태)    
    SEventStretch*      m_pStretchEvent;         ///< 이벤트 구조체
    RwReal              m_fStretchPullingSpeed;  ///< 현재 풀링 스피드
    RwMatrix            m_matDir;                ///< 어깨 방향 매트릭스

    RwBool              m_bOrgBoneScale;        ///< 원래의 본 스케일 플래그
    RwReal              m_fOrgBoneLength[3];    ///< 원래의 본 길이
    RwReal              m_fOrgBoneWidth[3];     ///< 원래의 본 두께
};
//////////////////////////////////////////////////////////////////////////
/**
 * \ingroup NtlSimulation
 * \brief 상대의 등뒤로 워프해서 공격하는 스킬 (섬광참)
 *
 * \date 2008-11-20
 * \author agebreak
 */
class CNtlBehaviorCharActiveWarpBackAttack : public CNtlBehaviorBase
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorCharActiveWarpBackAttack, NTL_DEFAULT_MEMORY_POOL)
    DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharActiveWarpBackAttack)

public:
    CNtlBehaviorCharActiveWarpBackAttack();
    ~CNtlBehaviorCharActiveWarpBackAttack();

    virtual void Enter(void);
    virtual void Exit(void);
    virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);    

	void	SetFinalLoc(RwV3d& loc);

protected:

    void    OnWarpBack();                       ///< 상대방의 뒤로 워프한다.

protected:

	RwV3d		m_vFinalLoc;
};

//////////////////////////////////////////////////////////////////////////

/**
 * \ingroup NtlSimulation
 * \brief 마인의 근거리 공간 도약 스킬 (타겟의 근처로 이동)
 *
 * \date 2008-11-21
 * \author agebreak
 */
class CNtlBehaviorCharActiveWarp : public CNtlBehaviorBase
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorCharActiveWarp, NTL_DEFAULT_MEMORY_POOL)
    DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharActiveWarp)

public:
    CNtlBehaviorCharActiveWarp();
    ~CNtlBehaviorCharActiveWarp();
    
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);    

	void	SetFinalLoc(RwV3d& loc);

protected:

    void    OnWarpTarget();                 ///< 타겟의 주위로 워프한다.

protected:

	RwV3d		m_vFinalLoc;
};

//////////////////////////////////////////////////////////////////////////

#define SKILL_THROUGH_SPEED 15.0f;
#define SKILL_THROUGH_PAST_POS 10.0f;

/**
 * \ingroup NtlSimulation
 * \brief 스루 어택(일섬 스타일) 스킬
 *
 * \date 2008-11-21
 * \author agebreak
 */
class CNtlBehaviorCharActiveThroughAttack : public CNtlBehaviorBase
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorCharActiveThroughAttack, NTL_DEFAULT_MEMORY_POOL)
    DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharActiveThroughAttack)

public:

    CNtlBehaviorCharActiveThroughAttack();
    ~CNtlBehaviorCharActiveThroughAttack();

    virtual void Enter(void);
    virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);    

	void	SetFinalLoc(RwV3d& loc);

protected:

	void    OnThroughAttack();																		 ///< 
    void    CalcHitDamage();                                                                    ///< Show damage to the right target

protected:

	RwV3d		m_vFinalLoc;
};

class CNtlBehaviorCharActivePull : public CNtlBehaviorBase
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharActivePull, NTL_DEFAULT_MEMORY_POOL)
		DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharActivePull)

public:

	CNtlBehaviorCharActivePull();
	~CNtlBehaviorCharActivePull();

	virtual void Enter(void);
	virtual void Update(RwReal fElapsed);
	virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);

	void	SetFinalLoc(RwV3d& loc);

protected:

	void    OnPull();	///< 

protected:

	RwV3d		m_vFinalLoc;
};

/**
 * \ingroup NtlSimulation
 * \brief 몹이 변신 스킬을 사용할때 사용되는 클래스 (몹 한정)
 *
 * \date 2009-01-16
 * \author agebreak
 */
class CNtlBehaviorCharTransform : public CNtlBehaviorBase
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorCharTransform, NTL_DEFAULT_MEMORY_POOL)
    DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharTransform)

public:
    CNtlBehaviorCharTransform();
    ~CNtlBehaviorCharTransform();

    virtual void Enter(void);
    virtual void Update(RwReal fElapsed);
    virtual RwUInt32 HandleEvents(RWS::CMsg &pMsg);   

    void    SetAnimation(RwUInt32 uiAction, RwUInt32 uiActionLoop);       ///< 애니메이션을 세팅한다.
protected:

protected:
    RwUInt32    m_uiAnimAction;
    RwUInt32    m_uiAnimLoop;
};


#endif
