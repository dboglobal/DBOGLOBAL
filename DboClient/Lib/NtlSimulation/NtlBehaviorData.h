/*****************************************************************************
 *
 * File			: NtlBehaviorBase.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 2. 13	
 * Abstract		: actor behavior data stuff
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_BEHAVIORDATA_H__
#define __NTL_BEHAVIORDATA_H__

// shared 
#include "NtlBattle.h"
#include "NtlSkill.h"

// PL
#include "NtlAnimEventData.h"

// simulation
#include "NtlSLDef.h"

struct SNtlEventSobMoveSync;

////////////////////////////////////////////////////////////////////////////////////////////////
// attack behavior stuff
////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// 공격 

// 공격 성공의 형태.
#define NTL_ATTACK_RESULT_SUCCESS		0		// 공격 성공
#define NTL_ATTACK_RESULT_BLOCK			1		// 공격 방어
#define NTL_ATTACK_RESULT_DODGE			2		// 공격 회피
#define NTL_ATTACK_RESULT_KNOCKDOWN		3		// 공격 knockdown
#define NTL_ATTACK_RESULT_SLIDING		4		// 공격 sliding ( NTL_ATTACK_RESULT_KNOCKDOWN | NTL_ATTACK_RESULT_BLOCK )
#define NTL_ATTACK_RESULT_HEALING		5		// heading
#define NTL_ATTACK_RESULT_BUFF			6		// heading
#define NTL_ATTACK_RESULT_STEAL			7		// 공격한 대상의 체력을 뺏서온다. (이 플래그는 Projectile 전용 플래그다)
#define NTL_ATTACK_RESULT_IMMUNE		8		// 
#define NTL_ATTACK_RESULT_RESIST		9		// 


// behavior 형태
#define NTL_ATTACK_NORMAL				0		// 일반적인 공격형태.
#define NTL_ATTACK_PUSH					1		// push
#define NTL_ATTACK_KNOCKDOWN			2		// knock down
#define NTL_ATTACK_SLIDING				3		// sliding
#define NTL_ATTACK_TOSS_UP				4		// 공중으로 띄우는 공격
#define NTL_ATTACK_TOSS_DOWN			5		// 공중에서 아래로 내려 꽂는 공격.

// 물리적 공격 형태
#define NTL_ATTACK_PHYSIC_MELEE			0		// 직접 공격.
#define NTL_ATTACK_PHYSIC_PROJECTILE	1		// 발사체 공격.
#define NTL_ATTACK_PHYSIC_SPELL			2		// 마법 공격

// 논리적 공격 형태
#define NTL_ATTACK_LOGIC_NORMAL			0		// 일반 공격(기본으로 설정된 공격 방식)
#define NTL_ATTACK_LOGIC_SKILL			1		// skill 공격

// attribute 공격 형태
#define NTL_ATTACK_ATTR_NORMAL			0		// 일반 공격
#define NTL_ATTACK_ATTR_CRITICAL		1		// critical 공격.


struct SAttackDamageStuff
{
    RwInt32 iDamage;        ///< 일반 데미지
    
    // DD/DOT        
    RwInt32 iAbsorbedDamage;
    RwInt32 iReflectedDamage;

	sDBO_LP_EP_RECOVERED		lpEpRecovered;	// attacked target recover
        
    // 스틸 스킬
    float						LP_EP_STEAL_fLpDamage;
    float						LP_EP_STEAL_fEpDamage;
    float						LP_EP_STEAL_fAbsorbedDamage;
    float						LP_EP_STEAL_fReflectedDamage;
    float						LP_EP_STEAL_fLpGained;
    float						LP_EP_STEAL_fEpGained;
};

typedef union _UAttackAttr
{
	struct 
	{
		RwUInt32 bitResult		: 4;	// 공격 성공의 result.
		RwUInt32 bitPhysic		: 2;	// 물리적 공격 형태
		RwUInt32 bitBehavior	: 4;	// behavior 형태
		RwUInt32 bitLogic		: 4;	// 논리적 공격 형태
		RwUInt32 bitCritical	: 1;	// attribute 공격 형태
		RwUInt32 bitChainAttack : 1;    // chain attack
		RwUInt32 bitHitSeq		: 4;	// hit sequence
		RwUInt32 bitPowerEffect : 1;	// power effect
		RwUInt32 bitBlocking	: 8;	// blocking mode 종류.
		RwUInt32 bitReserved	: 5;        
	};
	RwUInt32 uiFlags;

}UAttackAttr; 

struct SHitStuff
{
	DECLEAR_MEMORY_POOL(SHitStuff, NTL_LARGE_MEMORY_POOL);

public:

	UAttackAttr			uiAttr;									// 공격의 모든 속성
	SERIAL_HANDLE		hAttackerSerialId;						// attacker serial id
	SERIAL_HANDLE		hDefenderSerialId;						// defender serial id
	SAttackDamageStuff	sDamageStuff;							// damage stuff
	RwUInt32			uiAttrLogicTblId;						// 무기 공격 : item id, skill 공격 : skill idRw
	RwUInt32			uiProjectileBehaviorId;					// projectile이 존재하면 id를 setting 한다.
	RwUInt8				byShotType;								// projectile  발사 위치(character, weapon, sub weapon)			
	RwUInt8				bySubWeaponShotFlag;					// projectile  발사 위치가 sub weapon일 경우.(character, weapon, sub weapon)
	RwReal				fSpeed;									// projectile speed
	RwBool				bLeftHand;								// left or right hand
	RwBool				bCreateHitStuffEffect;					// Projectile or Target Effct or Word Effect를 생성할 것인가?
	RwChar				chBoneName[NTL_MAX_BONE_NAME];			// bone name
	RwChar				chProjEffName[NTL_MAX_EFFECT_NAME];		// projectile effect name
	
	RwChar				chTargetEffName[NTL_MAX_EFFECT_NAME];	// target effect name
	RwUInt8				byTargetAttach;							// target에 attach할 것인가?
	RwReal				fTargetLocHeight;						// target effect  터질 높이 값(persent이며, 1.0이 100%이다.)
	RwUInt8				byTargetEffectDir;						// target effect 방향(character anim event 참조)
	RwChar				chWordEffName[NTL_MAX_EFFECT_NAME];		// target word effect name

	RwV3d				vPushDir;								// push 방향.
	RwBool				bCameraShake;							// 카메라 흔들기.
    RwReal              fShakeFactor;                           // 카메라 흔들기 팩터
    RwReal              fShakeMaxHeight;                        // 카메라 흔들기 최대치 팩터
	SYNCEVENT_HANDLE	hSyncEventId;
	void				*pProjExtData;
    RwBool              bHitSoundEcho;                          // Hit Sound의 Echo 적용 유무

	SHitStuff& operator = (const SHitStuff& stuff)
	{
		uiAttr.uiFlags			= stuff.uiAttr.uiFlags;
		hAttackerSerialId		= stuff.hAttackerSerialId;
	    hDefenderSerialId		= stuff.hDefenderSerialId;
		sDamageStuff			= stuff.sDamageStuff;
		uiAttrLogicTblId		= stuff.uiAttrLogicTblId;
		uiProjectileBehaviorId	= stuff.uiProjectileBehaviorId;
		byShotType				= stuff.byShotType;
		bySubWeaponShotFlag		= stuff.bySubWeaponShotFlag;
		fSpeed					= stuff.fSpeed;
		bLeftHand				= stuff.bLeftHand;
		bCreateHitStuffEffect	= stuff.bCreateHitStuffEffect;
		memcpy(chBoneName, stuff.chBoneName, NTL_MAX_BONE_NAME);
		memcpy(chProjEffName, stuff.chProjEffName, NTL_MAX_EFFECT_NAME);

		memcpy(chTargetEffName, stuff.chTargetEffName, NTL_MAX_EFFECT_NAME);
		byTargetAttach			= stuff.byTargetAttach;
		fTargetLocHeight		= stuff.fTargetLocHeight;
		byTargetEffectDir		= 0;
		memcpy(chWordEffName, stuff.chWordEffName, NTL_MAX_EFFECT_NAME);

		RwV3dAssignMacro(&vPushDir, &stuff.vPushDir);
		bCameraShake			= stuff.bCameraShake;
        fShakeFactor            = stuff.fShakeFactor;
        fShakeMaxHeight         = stuff.fShakeMaxHeight;
		hSyncEventId			= INVALID_SYNCEVENT_ID;
		pProjExtData			= stuff.pProjExtData;
        bHitSoundEcho           = stuff.bHitSoundEcho;
	}
	
	void Reset(void)
	{
		uiAttr.uiFlags					= 0;
		hAttackerSerialId				= INVALID_SERIAL_ID;
		hDefenderSerialId				= INVALID_SERIAL_ID;
		sDamageStuff.iDamage			= 0;
		sDamageStuff.iAbsorbedDamage	= 0;
		sDamageStuff.iReflectedDamage	= 0;
		sDamageStuff.lpEpRecovered.Reset();
		uiAttrLogicTblId				= 0;
		uiProjectileBehaviorId			= 0;
		byShotType						= 0;
		bySubWeaponShotFlag				= 0;
		fSpeed							= 10.0f;
		bLeftHand						= TRUE;
		bCreateHitStuffEffect			= TRUE;
		chBoneName[0]					= 0;
		chProjEffName[0]				= 0;
		chTargetEffName[0]				= 0;
		byTargetAttach					= FALSE;
		fTargetLocHeight				= 0.7f;
		chWordEffName[0]				= 0;
		vPushDir.x						= 0.0f;
		vPushDir.y						= 0.0f;
		vPushDir.z						= 0.0f;
		bCameraShake					= FALSE;
        fShakeFactor                    = CAMERA_SHAKE_FACTOR;
        fShakeMaxHeight                 = CAMERA_SHAKE_MAX_HEIGHT_FACTOR;
		pProjExtData					= NULL; 
        bHitSoundEcho                   = FALSE;
	}
};


struct SAttackStuff
{
	UAttackAttr			uiType;						// 공격의 모든 속성
	SERIAL_HANDLE		hAttackerSerialId;			// attacker
	SERIAL_HANDLE		hDefenderSerialId;			// defender
	RwUInt8				byAttSeq;					// chain attack sequence
	SAttackDamageStuff	sDamage;					// damage stuff
	RwUInt32			uiAttrLogicTblId;			// 무기 공격 : item id, skill 공격 : skill id
	RwV3d				vPushDir;
	RwBool				bAnimSpeedApply;			// 공격 animation speed를 적용할 것인가?
	RwUInt32			uiSyncEventId;
	RwV3d				vFinalLoc;
		
	void Reset(void)
	{
		uiType.uiFlags				= 0;
		hAttackerSerialId			= INVALID_SERIAL_ID;
		hDefenderSerialId			= INVALID_SERIAL_ID;
		byAttSeq					= 0;
		sDamage.iDamage				= 0;
		sDamage.iAbsorbedDamage		= 0;
		sDamage.iReflectedDamage	= 0;
		sDamage.lpEpRecovered.Reset();
		uiAttrLogicTblId			= 0;
		vPushDir.x					= 0.0f;
		vPushDir.y					= 0.0f;
		vPushDir.z					= 0.0f;
		bAnimSpeedApply				= FALSE;
		uiSyncEventId				= INVALID_SYNCEVENT_ID;
		vFinalLoc.x = 0.0f;
		vFinalLoc.y = 0.0f;
		vFinalLoc.z = 0.0f;
	}
};

class CNtlAttackData
{
	DECLEAR_MEMORY_POOL(CNtlAttackData, NTL_DEFAULT_MEMORY_POOL)

private:

	enum { MAX_ATTACK_STUFF = NTL_MAX_NUMBER_OF_SKILL_TARGET};

	RwUInt8 m_byAttCount;
	RwUInt8	m_byAttLogic;
	SAttackStuff m_sStuff[MAX_ATTACK_STUFF];

public:

	void Reset(void)
	{
		m_byAttCount = 0;
		m_byAttLogic = NTL_ATTACK_LOGIC_NORMAL;
	}

	void SetAttackStuffCount(RwUInt8 byAttCount)
	{
		m_byAttCount = byAttCount;
	}

	void SetAttackStuffLogic(RwUInt8 byLogic)
	{
		m_byAttLogic = byLogic;
	}

	RwInt32 GetAttackStuffCount(void) const 
	{
		return m_byAttCount;
	}

	RwUInt8 GetAttackStuffLogic(void) const
	{
		return m_byAttLogic;
	}

	SAttackStuff* GetAttackStuff(RwInt32 iIdx)
	{
		if(iIdx < 0 && iIdx >= MAX_ATTACK_STUFF)
			return NULL;

		return &m_sStuff[iIdx];
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////
// move behavior stuff
////////////////////////////////////////////////////////////////////////////////////////////////

// move form
#define NTL_MOVEFORM_GROUND			0
#define NTL_MOVEFORM_SWIMMING		1
#define NTL_MOVEFORM_FLY			2
#define NTL_MOVEFORM_SPLINE			3
#define NTL_MOVEFORM_BUS            4               // 버스 이동


// move target
#define NTL_MOVETARGET_NONE			0
#define NTL_MOVETARGET_DIR			1	// move keyboard (?)
#define NTL_MOVETARGET_LOC			2	// move dest (mouse) (?)
#define NTL_MOVETARGET_ACTOR		3	// follow (?)
#define NTL_MOVETARGET_DASH			4
#define NTL_MOVETARGET_DASH_ACTOR	5	// dash by skill
#define NTL_MOVETARGET_AIR_DASH		6	// air-jump dash

// move result
#define NTL_MOVE_RESULT_COLLISION	0x01
#define NTL_MOVE_RESULT_FALLING		0x02

// move behavior stuff
struct SMoveStuff
{
	RwUInt8			byType;							// direction, location, actor
	RwUInt8			byForm;							// ground, fly
	RwUInt8			byFormFlag;						// Walking, running, swimming, etc.
	RwUInt8			byMoveFlags;					// byMoveDirection
	RwV3d			vCurrLoc;						// 이동 현재 위치.
	RwV3d			vDir;							// dash일 경우 방향 vector로 사용된다.
	RwV3d			vDest;							// mouse 및 dash 이동일 경우 최종 목적지.
	RwV3d			vSendDest;						// Second shift destination (on bus)
	RwReal			fFollowRange;					// follow range
	SERIAL_HANDLE	hFollowSerialId;				// follow serial
	RwUInt8			byPathNodeNum;					// path node number
	RwV3d			arrPathNode[DBO_MAX_NEXT_DEST_LOC_COUNT];
	RwUInt8			byMoveResult;					// Results after moving.
	RwBool			bSplineGroundCheck;		
	RwBool			bSplineRunMove;					// run 이동.
	RwChar			chSpline[NTL_MAX_SPLINE_NAME];

	void Reset(void)
	{
		byType			= NTL_MOVETARGET_NONE;
		byForm			= NTL_MOVEFORM_GROUND;
		byFormFlag		= NTL_MOVE_FLAG_WALK;
		hFollowSerialId = INVALID_SERIAL_ID;
		byMoveFlags		= 0;
		byPathNodeNum	= 0;
		byMoveResult	= 0;
	}
};

/// Move Sync 정보를 가지고 있는 구조체 (클라이언트에서만 사용한다)
struct SMoveSyncStuff
{
    std::deque<SNtlEventSobMoveSync*>   m_MoveSyncQ;             ///< Move Sync 패킷 큐
    SNtlEventSobMoveSync*               m_pMoveSyncCurr;         ///< 현재 Move Sync값    

    SMoveSyncStuff()
    {
        m_pMoveSyncCurr = NULL;
    }

    ~SMoveSyncStuff();

    void    Clear();
    SNtlEventSobMoveSync*  Next();
    SNtlEventSobMoveSync*  Pop();    
};

enum ECharJumpState
{
    JUMPSTATE_NONE,
    JUMPSTATE_START,
    JUMPSTATE_LOOP,
    JUMPSTATE_STAND_LANDING,
    JUMPSTATE_MOVE_LANDING,
    JUMPSTATE_WATER_LANDING,
    JUMPSTATE_END
};


// jump behavior stuff
struct SJumpStuff
{
	RwV3d vJumpDir;
    ECharJumpState eState;
	
	void Reset(void)
	{
		vJumpDir.x = 1.0f;
		vJumpDir.y = 0.0f;
		vJumpDir.z = 0.0f;
        eState = JUMPSTATE_NONE;
	}
};

// skill, item use bahavior stuff
struct SSkillStuff
{
	SERIAL_HANDLE	hTargetSerialId;
	SERIAL_HANDLE	hSkillSerialId;
	RwUInt32		uiSkillTblId;
	RwUInt8			bySlotIdx;
	RwUInt8			byRpBonusType;
	RwReal			fCurrCastingTime;
	RwReal			fMaxCastingTime;
	RwV3d			vFinalLoc;			// caster final location
	RwV3d			vFinalSubjectLoc;	// target final location

	void Reset(void)
	{
		hTargetSerialId		= INVALID_SERIAL_ID;
		hSkillSerialId		= INVALID_SERIAL_ID;
		uiSkillTblId		= 0;
		bySlotIdx			= 0;
		byRpBonusType		= 0xff;
		fCurrCastingTime	= 0.0f;
		fMaxCastingTime		= 0.0f;

		vFinalLoc.x = 0.0f;
		vFinalLoc.y = 0.0f;
		vFinalLoc.z = 0.0f;

		vFinalSubjectLoc.x = 0.0f;
		vFinalSubjectLoc.y = 0.0f;
		vFinalSubjectLoc.z = 0.0f;
	}
};

struct SHTBStuff
{
	SERIAL_HANDLE hAttackSerialId;
	SERIAL_HANDLE hTargetSerialId;
	RwUInt32 uiSkillTblId;
	RwUInt8 bySlotIdx;
	RwUInt8 byCurrStep;
	RwUInt8 byStepCount;
	RwUInt8 byResultCount;
	sHTBSKILL_RESULT sResult[NTL_HTB_MAX_SKILL_COUNT_IN_SET];

	void Reset(void)
	{
		hAttackSerialId = INVALID_SERIAL_ID;
		hTargetSerialId = INVALID_SERIAL_ID;
		uiSkillTblId = 0;
		bySlotIdx = 0;
		byCurrStep = 0;
		byStepCount = 0;
		byResultCount = 0;
	}

	sHTBSKILL_RESULT* FindResult(RwUInt8 byStep)
	{
		for(RwInt8 i = 0; i < byResultCount; ++i)
		{
			if(sResult[i].byStep == byStep)
			{
				return &sResult[i];
			}
		}

		return NULL;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
// input control behavior stuff
////////////////////////////////////////////////////////////////////////////////////////////////
struct SCtrlStuffFalling
{
	RwUInt8	byMoveDirection;
	RwV3d	vFallingDir;
	RwReal	fSearchHeight;
	RwReal	fSpeed;
};

struct SCtrlStuffFollow
{
	
};

struct SCtrlStuffRotate
{
	RwReal	fDeltaAngleY;

	bool	bSetAngleX;
	RwReal	fDeltaAngleX;
};


struct SCtrlStuffPrivateShop
{
	WCHAR	wchShopName[NTL_MAX_PRIVATESHOP_NAME];	// shop name.
	RwBool	bShopOwnerEmpty;						// shop owner empty.
	RwUInt8	byShopState;							// shop state.
};

struct SCtrlStuffTeleport
{
	RwUInt8			byTeleportType;							// teleport type
	SERIAL_HANDLE	hTeleportNpc;
};

struct SCtrlStuffDirection
{
	RwBool			bAnimUse;
	RwBool			bTimeLoop;
	RwReal			fLoopTime;
	RwUInt8			byDirectType;
	RwUInt32		uiTblId;
	RwUInt32		uiAnimId;
};

struct SCtrlStuffOperate
{
	RwUInt32		hTargetSerialId;
	RwUInt32		uiOperateTime;
	RwUInt32		uiTblId;
};

/**
* \brief 겜블 Npc에 대한 정보
*/
struct SCtrlStuffGamble
{
	SERIAL_HANDLE	hGambleNpc;
};

/// 스턴 타입
struct SCtrlStuffStun
{
    BYTE    byStunType;
};

// 버스 및 탈것에 대한 정보
struct SCtrlStuffRide
{
    RwUInt32        hTargetSerialId;
};

/// Pushing 상태 정보
struct SCtrlStuffPushing
{
    BYTE    byAttackResult;
	WORD    wAttackResultValue;
    RwV3d   vDestPos;
};

// Vehicle에 대한 정보

enum eVehicleEngineMessgeType
{
	VEHICLE_READY,
	VEHICLE_ENGINE_ASK_REG_FROM_BAG,
	VEHICLE_ENGINE_REG_FUEL,
	VEHICLE_ENGINE_UNREG_FUEL,
	VEHICLE_ENGINE_UPDATE_FUEL,
	VEHICLE_ENGINE_END,
};

struct SCtrlStuffVehicle
{
	RwBool	bCreateUpdate;
	RwBool	bRideOn;
//	RwBool	bEngineOn; // engine disabled by daneos
	RwV3d	vSpawnPos;
	SERIAL_HANDLE hVehicleItem;
	TBLIDX	idxVehicleItem;
};

union UCtrlStuffExtra
{
	SCtrlStuffFalling		sFalling;
	SCtrlStuffFollow		sFollow;
	SCtrlStuffRotate		sRotate;
	SCtrlStuffPrivateShop	sPrivateShop;
	SCtrlStuffTeleport		sTeleport;
	SCtrlStuffDirection		sDirection;
	SCtrlStuffOperate		sOperate;    
	SCtrlStuffGamble		sGamble;
    SCtrlStuffStun          sStun;
    SCtrlStuffPushing       sPushing;
};

struct SCtrlStuff
{
	RwReal			fTime;
	SERIAL_HANDLE	hTargetSerialId;						// attack serial.
	RwReal			fFollowRange;

	UCtrlStuffExtra	        uExtra;
    SCtrlStuffRide          sRide;
	SCtrlStuffVehicle		sVehicle;
		
	void Reset(void)
	{
		fTime				= 0.0f;
		hTargetSerialId		= INVALID_SERIAL_ID;
        sRide.hTargetSerialId = INVALID_SERIAL_ID;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////
// time sequence
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlTimeSequence
{
public:

	CNtlTimeSequence();
	virtual ~CNtlTimeSequence();
    
    void SetTime(RwReal fTime) {m_fTime = fTime;}
    RwReal GetTime(void) const {return m_fTime;}

	void Add(CNtlTimeSequence *pSeq);
	void Remove(CNtlTimeSequence *pSeq);
	
	void SetPrev(CNtlTimeSequence *pSeq);
	CNtlTimeSequence* GetPrev(void) const;
	
	void SetNext(CNtlTimeSequence *pSeq);
	CNtlTimeSequence* GetNext(void) const;

	virtual void* GetData(RwReal fTime) { return NULL; }
	virtual void* GetDeltaData(RwReal fPrevTime, RwReal fCurrTime) { return NULL; }

protected:
    CNtlTimeSequence *m_pPrev;
    CNtlTimeSequence *m_pNext;
    RwReal m_fTime;
};


class CNtlDistTimeSequence : public CNtlTimeSequence
{
	DECLEAR_MEMORY_POOL(CNtlDistTimeSequence, NTL_DEFAULT_MEMORY_POOL)

private:

	RwReal m_fDist;

	static RwReal m_fCurrDist;
	static RwReal m_fOldDist;
	static RwReal m_fDeltaDist;
	static RwBool m_bCalcOld;
	static RwInt32 m_iOldTimeNode;

public:

	CNtlDistTimeSequence();
	~CNtlDistTimeSequence();

	void SetDistance(RwReal fDist); 
	RwReal GetDistance(void) const;

	RwReal RecursiveData(RwReal fTime);
	RwReal RecursiveDeltaData(RwReal fOldTime, RwReal fCurrTime);

	virtual void* GetData(RwReal fTime);
	virtual void* GetDeltaData(RwReal fOldTime, RwReal fCurrTime);
};



class CNtl2DTimeSequence : public CNtlTimeSequence
{
	DECLEAR_MEMORY_POOL(CNtl2DTimeSequence, NTL_DEFAULT_MEMORY_POOL)

public:

	typedef struct _S2DDist
	{
		RwReal fX;
		RwReal fY;
	}S2DDist;

private:
	S2DDist m_sDist;	
	static S2DDist m_sCurrDist;

public:
	CNtl2DTimeSequence();
	~CNtl2DTimeSequence();

	void SetDistance(RwReal fDistX, RwReal fDistY); 
	S2DDist GetDistance(void) const;

	S2DDist RecursiveData(RwReal fTime);

	virtual void* GetData(RwReal fTime);
};

// 3차원 좌표계의 Time Sequence 클래스 (by agebreak)
class CNtl3DTimeSequence : public CNtlTimeSequence
{
    DECLEAR_MEMORY_POOL(CNtl3DTimeSequence, NTL_DEFAULT_MEMORY_POOL);

public:
    CNtl3DTimeSequence();
    ~CNtl3DTimeSequence();

    virtual void*   GetData(RwReal fTime);
    RwV3d           GetHaveData() {return m_vData;}
    void            SetData(const RwV3d& vData) {m_vData = vData;}

protected:
    RwV3d           RecursiveData(RwReal fTime);

protected:
    RwV3d m_vData;                      ///< 현재 시퀀스의 데이터

    static RwV3d m_svCurrData;          ///< 포인터형 반환을 위해서 가지고 있어야하는 정적 데이터 
};


////////////////////////////////////////////////////////////////////////////////////////////////
// behavior data class
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBeCharData
{
	DECLEAR_MEMORY_POOL(CNtlBeCharData, NTL_DEFAULT_MEMORY_POOL)

private:

	SCtrlStuff	m_sCtrlStuff;
	SMoveStuff	m_sMoveStuff;
    SMoveSyncStuff m_sMoveSyncStuff;                ///< Move Sync 정보를 담고 있는 구조체
	SJumpStuff	m_sJumpStuff;
	SHitStuff	m_sHitStuff;
	SSkillStuff m_sSkillStuff;    
	SHTBStuff	m_sHTBStuff;
	CNtlAttackData m_AttData;
		
public:

	CNtlBeCharData()
	{
		m_sCtrlStuff.Reset();
		m_sMoveStuff.Reset();
	}
	
	SCtrlStuff* GetCtrlStuff(void) { return &m_sCtrlStuff; }
	SMoveStuff* GetMoveStuff(void) { return &m_sMoveStuff; }
    SMoveSyncStuff* GetMoveSyncStuff() {return &m_sMoveSyncStuff;} // no longer used
	SJumpStuff* GetJumpStuff(void) { return &m_sJumpStuff; }
	SHitStuff* GetHitStuff(void) { return &m_sHitStuff; }
	SSkillStuff* GetSkillStuff(void) { return &m_sSkillStuff; }
	SHTBStuff* GetHTBStuff(void) { return &m_sHTBStuff; }
    
	CNtlAttackData* GetAttackData(void) { return &m_AttData; }

};




////////////////////////////////////////////////////////////////////////////////////////////////
// behavior logic function
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlSobActor;

// actor 좌표 보정에 관한 함수.
void Behavior_AdjustPositionJumping(CNtlSobActor *pActor, const RwV3d *pTarLoc);



#endif
