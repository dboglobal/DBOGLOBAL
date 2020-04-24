/*****************************************************************************
 *
 * File			: NtlSLLuaGlueStuff.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2006. 2. 20	
 * Abstract		: simulation lua glue stuff
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SLLUA_GLUE_STUFF_H__
#define __NTL_SLLUA_GLUE_STUFF_H__

#include "NtlBehaviorData.h"
#include "NtlCameraController.h"

class CNtlSobActor;
class CNtlPLEntity;

class CNtlFSMStateBase;
class CNtlBehaviorBase;
class CNtlSob;

enum ELuaGlueStackEdit
{
	LUA_STACK_EDIT_ACTOR,		// simulation actor
	LUA_STACK_EDIT_OBJECT,		// presentation entity
};

enum ELuaGlueActiveObjType
{
	LUA_STACK_ACTIVEOBJ_NONE,		// simulation none
	LUA_STACK_ACTIVEOBJ_STATE,		// simulation state
	LUA_STACK_ACTIVEOBJ_BEHAVIOR,	// simulation behavior
	LUA_STACK_ACTIVEOBJ_SOB,		// simulation object
};

enum ELuaGlueActiveTransType
{
	LUA_STACK_ACTIVETRANS_POS = 0x01,
	LUA_STACK_ACTIVETRANS_DIR = 0x02,
	LUA_STACK_ACTIVETRANS_SCALE = 0x04,
};

typedef struct _SLuaStackTF
{
	RwUInt8 byTransMask;
	RwV3d vPos;
	RwV3d vDir;
	RwV3d vAddAxisValue;
	RwV3d vScale;
}SLuaStackTF;

typedef union _UActiveObj
{
	CNtlFSMStateBase *pState;
	CNtlBehaviorBase *pBehavior;
	CNtlSob *pSobObj;
	void *pObj;
}UActiveObj; 

typedef struct _SActiveObj
{
	ELuaGlueActiveObjType eType;
	UActiveObj uObj;
}SActiveObj;

class CNtlLuaGlueStack
{
private:

	static CNtlLuaGlueStack *m_pInstance;

	CNtlSobActor		*m_pActor;
	
	CNtlPLEntity		*m_pPLEntity;
	ELuaGlueStackEdit	m_eEditType;
	SActiveObj			m_sActiveObj;
	SHitStuff			m_sHit;

	// transform
	SLuaStackTF			m_TM;

	// HTB camera setting
	SERIAL_HANDLE		m_hHTBAttack;
	SERIAL_HANDLE		m_hHTBTarget;
	RwV3d				m_vCameraLookAt;

private:

	void ResetTransform(void);

public:

	CNtlLuaGlueStack();
	~CNtlLuaGlueStack();

	static CNtlLuaGlueStack* GetInstance(void);

	// reset
	void Reset(void);

	// edit data setting
	void SetReferActor(const CNtlSobActor *pActor);
	CNtlSobActor* GetReferActor(void) const;

	void SetActor(const CNtlSobActor *pActor);
	CNtlSobActor* GetActor(void) const;

	void SetTargetActor(const CNtlSobActor *pActor);
	CNtlSobActor* GetTargetActor(void) const;

	void SetPLEntity(const CNtlPLEntity *pPLEntity);
	CNtlPLEntity* GetPLEntity(void) const;

	void SetEditType(ELuaGlueStackEdit eType);
	ELuaGlueStackEdit GetEditType(void) const;

	void SetCameraLookAt(RwV3d vLookAt);
	RwV3d GetCameraLookAt(void) const;
	

	void SetHTBAttackSerialId(SERIAL_HANDLE hAttackSerialId);
	SERIAL_HANDLE GetHTBAttackSerialId(void) const;

	void SetHTBTargetSerialId(SERIAL_HANDLE hTargetSerialId);
	SERIAL_HANDLE GetHTBTargetSerialId(void) const;

	void SetActiveObj(ELuaGlueActiveObjType eType, const void *pObj);
	RwBool AddActivePLEntity(const CNtlPLEntity *pPLEntity);

	void SetHit(const SHitStuff *pHit);
	const SHitStuff* GetHit(void) const;

	// transform
	void BeginTransform(void);
	void EndTransform(void);
	void SetPosition(RwReal fPosX, RwReal fPosY, RwReal fPosZ);
	void AddPosition(RwReal fPosX, RwReal fPosY, RwReal fPosZ);
	void SetDirection(RwReal fDirX, RwReal fDirY, RwReal fDirZ);
	void AddDirectionAxisX(RwReal fMinValue, RwReal fMaxValue);
	void AddDirectionAxisY(RwReal fMinValue, RwReal fMaxValue);
	void AddDirectionAxisZ(RwReal fMinValue, RwReal fMaxValue);
	void SetScale(RwReal fScaleX, RwReal fScaleY, RwReal fScaleZ);
};

static CNtlLuaGlueStack* GetLuaGlueStack(void)
{
	return CNtlLuaGlueStack::GetInstance(); 
}


inline void CNtlLuaGlueStack::SetActor(const CNtlSobActor *pActor)
{
	m_pActor = const_cast<CNtlSobActor*>(pActor);
}

inline CNtlSobActor* CNtlLuaGlueStack::GetActor(void) const
{
	return m_pActor;
}

inline void CNtlLuaGlueStack::SetPLEntity(const CNtlPLEntity *pPLEntity)
{
	m_pPLEntity = const_cast<CNtlPLEntity*>(pPLEntity);
}

inline CNtlPLEntity* CNtlLuaGlueStack::GetPLEntity(void) const
{
	return m_pPLEntity;
}

inline void CNtlLuaGlueStack::SetEditType(ELuaGlueStackEdit eType)
{
	m_eEditType = eType;
}

inline ELuaGlueStackEdit CNtlLuaGlueStack::GetEditType(void) const
{
	return m_eEditType;
}

inline void CNtlLuaGlueStack::SetCameraLookAt(RwV3d vLookAt)
{
	m_vCameraLookAt = vLookAt;
}

inline RwV3d CNtlLuaGlueStack::GetCameraLookAt(void) const
{
	return m_vCameraLookAt;
}


inline void CNtlLuaGlueStack::SetHTBAttackSerialId(SERIAL_HANDLE hAttackSerialId)
{
	m_hHTBAttack = hAttackSerialId;
}

inline SERIAL_HANDLE CNtlLuaGlueStack::GetHTBAttackSerialId(void) const
{
	return m_hHTBAttack;
}

inline void CNtlLuaGlueStack::SetHTBTargetSerialId(SERIAL_HANDLE hTargetSerialId)
{
	m_hHTBTarget = hTargetSerialId;
}

inline SERIAL_HANDLE CNtlLuaGlueStack::GetHTBTargetSerialId(void) const
{
	return m_hHTBTarget;
}


inline const SHitStuff* CNtlLuaGlueStack::GetHit(void) const
{
	return &m_sHit;
}


#endif