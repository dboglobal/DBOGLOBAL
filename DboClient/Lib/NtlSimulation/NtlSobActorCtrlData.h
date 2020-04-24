/*****************************************************************************
 *
 * File			: NtlSobActorCtrlData.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 10. 31	
 * Abstract		: Simulation object actor control data
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOB_ACTOR_CTRLDATA_H__
#define __NTL_SOB_ACTOR_CTRLDATA_H__

#include "NtlDumpTarget.h"
#include "debugostream.h"

class CNtlPLEntity;
class CNtlSobActor;

// move control data
enum EMoveTagetType
{
	EMOVE_TARGET_NONE,
	EMOVE_TARGET_DIRECTION,		/** keyboard move */
	EMOVE_TARGET_LOCATION,		/** mouse move */
	EMOVE_TARGET_ACTOR			/** actor follow */
};

// jump control data
enum EJumpState
{
	CTRL_JUMP_NONE,
	CTRL_JUMP_START,
	CTRL_JUMP_LOOP,
	CTRL_JUMP_END
};

enum EJumpType
{
	JUMP_STAND,
	JUMP_RUN
};

///////////////////////////////////////////////////////////////
// control data base class

enum ESobCtrlDataType
{
	SOB_CTRLDATA_NORMAL,
	SOB_CTRLDATA_MOVE,
	SOB_CTRLDATA_ATTACK,
	SOB_CTRLDATA_HURT,
	SOB_CTRLDATA_JUMP,
	SOB_CTRLDATA_SKILL_CASTING,
	SOB_CTRLDATA_SKILL_ACT,
	MAX_SOB_CTRLDATA

};


class CSobCtrlData
{
private:

	RwUInt32 m_uiType;

public:

	CSobCtrlData() { m_uiType = 0; }
	virtual ~CSobCtrlData() { ;}

	virtual void Reset(void) { ;}

	virtual void Dump(CNtlDumpTarget &dump) {}
	
	void SetType(RwUInt32 uiType);
	RwUInt32 GetType(void);
};

inline void CSobCtrlData::SetType(RwUInt32 uiType)
{
	m_uiType = uiType;
}

inline RwUInt32 CSobCtrlData::GetType(void)
{
	return m_uiType;
}

///////////////////////////////////////////////////////////////
// normal control data class

class CSobNormalCtrlData : public CSobCtrlData
{
private:

	RwReal m_fTime;
	RwBool m_bSit;
	RwUInt8 m_byMoveFlag;
	RwBool m_bRegen;
	RwBool m_bTerrainCorrect;
	
public:

	CSobNormalCtrlData();
	~CSobNormalCtrlData();

	virtual void Reset(void);

	virtual void Dump(CNtlDumpTarget &dump);
	
	void* operator new(size_t size);

	void operator delete(void *pObj);

	void SetTime(RwReal fTime);
	void SetAddTime(RwReal fElapsed);
	RwReal GetTime(void) const;

	void SetSit(RwBool bSitDown);
	RwBool GetSit(void) const;

	void SetMoveFlag(RwUInt8 byMoveFlag);
	RwUInt8 GetMoveFlag(void) const;

	void SetRegen(RwBool bRegen);
	RwBool GetRegen(void) const;

	void SetTerrainCorrect(RwBool bCorrect);
	RwBool GetTerrainCorrect(void) const;
};


///////////////////////////////////////////////////////////////
// move control data class
class CSobMoveCtrlData : public CSobCtrlData
{
public:

	EMoveTagetType m_eMoveType;
    RwUInt32 m_uiMoveFlags;
	RwV3d m_vDir;
	RwV3d m_vDest;
	RwUInt32 m_uiTime;				// fighting pos 와 keyboard move time에 사용.
	RwUInt32 m_uiTargetSerialId;	// follow target serial id;
    
	// adjust move
	RwV3d m_vAdjustDest;
	RwV3d m_vAdjustHeading;

public:

	CSobMoveCtrlData(); 
	~CSobMoveCtrlData();

	virtual void Reset(void);

	virtual void Dump(CNtlDumpTarget &dump);
	
	void* operator new(size_t size);

	void operator delete(void *pObj);
};

///////////////////////////////////////////////////////////////
// attack control data class

typedef struct _SDamageInfo
{
	RwUInt32 uiAttackerSerialId;
	RwUInt32 uiDefenderSerialId;
	RwUInt32 uiDamage;
	RwUInt32 uiCurrentHp;
	RwUInt32 uiCritical;
	RwUInt32 uiStatusSeiralId;
}SDamageInfo;

class CSobAttackCtrlData : public CSobCtrlData
{
private:

	RwBool m_bAttackMode;
	RwBool m_bCirtical;

	SDamageInfo m_sTempInfo;
	std::list<SDamageInfo> m_listDamage;
	
public:

	CSobAttackCtrlData(); 
	~CSobAttackCtrlData();

	virtual void Reset(void);

	virtual void Dump(CNtlDumpTarget &dump);
	
	void* operator new(size_t size);

	void operator delete(void *pObj);

	void PushDamage(RwUInt32 uiAttackerSerialId, RwUInt32 uiDefenderSerialId, RwUInt32 uiDamage, RwUInt32 uiCurrentHP, RwUInt32 uiCirtical, RwUInt32 uiStatusSerialId);
    SDamageInfo* PopDamage(void);
	SDamageInfo* PeekDamage(void);

	void SetAttackMode(RwBool bAttackMode);
	RwBool GetAttackMode(void) const;

	void SetCritical(RwBool bCritical);
	RwBool GetCritical(void) const;
};

///////////////////////////////////////////////////////////////
// hurt control data class
class CSobHurtCtrlData : public CSobCtrlData
{
public:

	RwUInt32 m_uiAttackerSerialId;
	RwUInt32 m_uiDefenderSerialId;
	RwUInt32 m_uiDamage;
	RwUInt32 m_uiCurrentHp;
	RwUInt32 m_uiStatusSerialId;

public:

	CSobHurtCtrlData(); 
	~CSobHurtCtrlData();

	virtual void Reset(void);

	virtual void Dump(CNtlDumpTarget &dump);
	
	void* operator new(size_t size);

	void operator delete(void *pObj);
};

///////////////////////////////////////////////////////////////
// jump control data class
class CSobJumpCtrlData : public CSobCtrlData
{
public:

	RwReal		m_fJumpSpeed;
	RwReal		m_fJumpStartElapsed;
	EJumpState	m_eJumpState;
	EJumpType	m_eJumpType;
	RwBool		m_bLandingEffectFlag;
	
public:

	CSobJumpCtrlData();
	~CSobJumpCtrlData();

	virtual void Reset(void);

	virtual void Dump(CNtlDumpTarget &dump);

	void* operator new(size_t size);

	void operator delete(void *pObj);
};

///////////////////////////////////////////////////////////////
// skill casting control data class
class CSobSkillCastingCtrlData : public CSobCtrlData
{
public:

	RwReal m_fDeltaTime;
	RwBool m_bFire;
	RwReal m_fFireSpeed;
	RwUInt32 m_uiFireNum;
	RwUInt32 m_uiTargetSerialId;
	RwV3d m_vTargetDest;
	RwBool m_bExplosion;

private:

	RwUInt32 m_uiSkillType;
	std::list<CNtlPLEntity*> m_listPLEntity;

public:

	CSobSkillCastingCtrlData();
	~CSobSkillCastingCtrlData();

	virtual void Reset(void);
	
	void* operator new(size_t size);

	void operator delete(void *pObj);

public:

	void PushPLEntity(const CNtlPLEntity *pPLEntity);

	void SetSkillType(RwUInt32 uiType);
	RwUInt32 GetSkillType(void);
};

///////////////////////////////////////////////////////////////
// skill act control data class
class CSobSkillActCtrlData : public CSobCtrlData
{
private:

	std::list<CNtlPLEntity*> m_listPLEntity;

public:

	CSobSkillActCtrlData();
	~CSobSkillActCtrlData();

	virtual void Reset(void);

	void* operator new(size_t size);

	void operator delete(void *pObj);

public:

	void PushPLEntity(const CNtlPLEntity *pPLEntity);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// actor control data station

class CSobCtrlDataStation
{
public:

	static RwFreeList *m_pNormalFreeList;
	static RwFreeList *m_pMoveFreeList;
	static RwFreeList *m_pAttackFreeList;
	static RwFreeList *m_pHurtFreeList;
	static RwFreeList *m_pJumpFreeList;
	static RwFreeList *m_pSkillCastingFreeList;
	static RwFreeList *m_pSkillActFreeList;

public:

	static RwBool AllocFreeList(void);
	static void DeAllocFreeList(void);

	static void* AllocCtrlData(ESobCtrlDataType eType);
	static void FreeCtrlData(ESobCtrlDataType eType, void *pObj);

private:

	CSobCtrlData *m_pCtrlData[MAX_SOB_CTRLDATA];

public:

	CSobCtrlDataStation();
	~CSobCtrlDataStation();

	RwBool Create(void);
	void Destroy(void);

	RwBool CreateCtrlData(ESobCtrlDataType eType);

	void ResetCtrlDataAll(void);
	void ResetCtrlData(ESobCtrlDataType eType);
	CSobCtrlData* GetCtrlData(ESobCtrlDataType eType);

	void Dump(CNtlDumpTarget &dump);
	
};

#endif