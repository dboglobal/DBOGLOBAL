#include "precomp_ntlsimulation.h"
#include "NtlSobActorCtrlData.h"
#include "NtlDebug.h"
#include "NtlMovement.h"
#include "NtlMath.h"
#include "NtlSLDef.h"
#include "NtlPLEntity.h"
#include "NtlPLSceneManager.h"

CSobNormalCtrlData::CSobNormalCtrlData()
{
	m_fTime = 0.0f;
	m_bSit = FALSE;
	m_byMoveFlag = NTL_MOVE_FLAG_INVALID;
	m_bRegen = FALSE;
	m_bTerrainCorrect = FALSE;
}

CSobNormalCtrlData::~CSobNormalCtrlData()
{

}

void CSobNormalCtrlData::Reset(void)
{
	m_fTime = 0.0f;
	m_bSit = FALSE;
	m_byMoveFlag = NTL_MOVE_FLAG_INVALID;
	m_bRegen = FALSE;
	m_bTerrainCorrect = FALSE;
}

void CSobNormalCtrlData::Dump(CNtlDumpTarget &dump)
{
	CSobCtrlData::Dump(dump);

	dump << "Normal Ctrl Data (server status) : " << m_byMoveFlag << "\n";
	dump.Dump();
	dump << "Normal Ctrl Data (sit flag) : " << m_bSit << "\n"; 
	dump.Dump();
	dump << "Normal Ctrl Data (regen flag) : " << m_bRegen << "\n"; 
	dump.Dump();
}


void* CSobNormalCtrlData::operator new(size_t size)
{
	NTL_FUNCTION("CSobNormalCtrlData::operator new");
    
	NTL_RETURN(CSobCtrlDataStation::AllocCtrlData(SOB_CTRLDATA_NORMAL));
}

void CSobNormalCtrlData::operator delete(void *pObj)
{
	NTL_FUNCTION("CSobNormalCtrlData::operator delete");

	CSobCtrlDataStation::FreeCtrlData(SOB_CTRLDATA_NORMAL, pObj);

	NTL_RETURNVOID();
}


void CSobNormalCtrlData::SetTime(RwReal fTime)
{
	m_fTime = fTime;
}

void CSobNormalCtrlData::SetAddTime(RwReal fElapsed)
{
	m_fTime += fElapsed;
}

RwReal CSobNormalCtrlData::GetTime(void) const
{
	return m_fTime;
}


void CSobNormalCtrlData::SetSit(RwBool bSitDown)
{
	m_bSit = bSitDown;
}

RwBool CSobNormalCtrlData::GetSit(void) const
{
	return m_bSit;
}

void CSobNormalCtrlData::SetMoveFlag(RwUInt8 byMoveFlag)
{
	m_byMoveFlag = byMoveFlag;
}

RwUInt8 CSobNormalCtrlData::GetMoveFlag(void) const
{
	return m_byMoveFlag;
}

void CSobNormalCtrlData::SetRegen(RwBool bRegen)
{
	m_bRegen = bRegen;
}

RwBool CSobNormalCtrlData::GetRegen(void) const
{
	return m_bRegen;
}

void CSobNormalCtrlData::SetTerrainCorrect(RwBool bCorrect)
{
	m_bTerrainCorrect = bCorrect;
}

RwBool CSobNormalCtrlData::GetTerrainCorrect(void) const
{
	return m_bTerrainCorrect;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSobMoveCtrlData::CSobMoveCtrlData()
{
	m_eMoveType = EMOVE_TARGET_NONE;
    m_uiMoveFlags = NTL_MOVE_NONE;
	CNtlMath::MathRwV3dAssign(&m_vDir, 0.0f, 0.0f, 1.0f);
	CNtlMath::MathRwV3dAssign(&m_vDest, 0.0f, 0.0f, 1.0f);
	m_uiTime = 0;
	m_uiTargetSerialId = INVALID_SERIAL_ID;

	SetType(SOB_CTRLDATA_MOVE);
}

CSobMoveCtrlData::~CSobMoveCtrlData()
{

}

void CSobMoveCtrlData::Reset(void)
{
	m_eMoveType = EMOVE_TARGET_NONE;
    m_uiMoveFlags = NTL_MOVE_NONE;
	CNtlMath::MathRwV3dAssign(&m_vDir, 0.0f, 0.0f, 1.0f);
	CNtlMath::MathRwV3dAssign(&m_vDest, 0.0f, 0.0f, 1.0f);
	m_uiTime = 0;
	m_uiTargetSerialId = INVALID_SERIAL_ID;
}

void CSobMoveCtrlData::Dump(CNtlDumpTarget &dump)
{
	CSobCtrlData::Dump(dump);

	dump << "Move Ctrl Data (move type) : " << m_eMoveType << "\n"; 
	dump.Dump(); 
	dump << "Move Ctrl Data (move flags) : " << m_uiMoveFlags << "\n"; 
	dump.Dump();
	dump << "Move Ctrl Data (move target) : " << m_uiTargetSerialId << "\n"; 
	dump.Dump();
}


void* CSobMoveCtrlData::operator new(size_t size)
{
	NTL_FUNCTION("CSobMoveCtrlData::operator new");
    
	NTL_RETURN(CSobCtrlDataStation::AllocCtrlData(SOB_CTRLDATA_MOVE));
}

void CSobMoveCtrlData::operator delete(void *pObj)
{
	NTL_FUNCTION("CSobMoveCtrlData::operator delete");

	CSobCtrlDataStation::FreeCtrlData(SOB_CTRLDATA_MOVE, pObj);

	NTL_RETURNVOID();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSobAttackCtrlData::CSobAttackCtrlData()
{
	m_bAttackMode = FALSE;
	m_bCirtical = FALSE;
	SetType(SOB_CTRLDATA_ATTACK);
}

CSobAttackCtrlData::~CSobAttackCtrlData()
{
}

void CSobAttackCtrlData::Reset(void)
{
	m_bAttackMode = FALSE;
	m_bCirtical = FALSE;
}

void CSobAttackCtrlData::Dump(CNtlDumpTarget &dump)
{
	CSobCtrlData::Dump(dump);
}


void* CSobAttackCtrlData::operator new(size_t size)
{
	NTL_FUNCTION("CSobAttackCtrlData::operator new");
    
	NTL_RETURN(CSobCtrlDataStation::AllocCtrlData(SOB_CTRLDATA_ATTACK));
}

void CSobAttackCtrlData::operator delete(void *pObj)
{
	NTL_FUNCTION("CSobAttackCtrlData::operator delete");

	CSobCtrlDataStation::FreeCtrlData(SOB_CTRLDATA_ATTACK, pObj);

	NTL_RETURNVOID();
}

void CSobAttackCtrlData::PushDamage(RwUInt32 uiAttackerSerialId, RwUInt32 uiDefenderSerialId, RwUInt32 uiDamage, RwUInt32 uiCurrentHP, RwUInt32 uiCirtical, RwUInt32 uiStatusSerialId)
{
	SDamageInfo sInfo = { uiAttackerSerialId, uiDefenderSerialId, uiDamage, uiCurrentHP, uiCirtical, uiStatusSerialId };
	m_listDamage.push_back(sInfo);
}

SDamageInfo* CSobAttackCtrlData::PopDamage(void)
{
	if(m_listDamage.size() == 0)
		return NULL;

	std::list<SDamageInfo>::iterator it = m_listDamage.begin();
	m_sTempInfo = (*it);
	m_listDamage.erase(it);

	return &m_sTempInfo;
}

SDamageInfo* CSobAttackCtrlData::PeekDamage(void)
{
	if(m_listDamage.size() == 0)
		return NULL;

	std::list<SDamageInfo>::iterator it = m_listDamage.begin();
	m_sTempInfo = (*it);
	
	return &m_sTempInfo;
}

void CSobAttackCtrlData::SetAttackMode(RwBool bAttackMode)
{
	m_bAttackMode = bAttackMode;
}

RwBool CSobAttackCtrlData::GetAttackMode(void) const
{
	return m_bAttackMode;
}

void CSobAttackCtrlData::SetCritical(RwBool bCritical)
{
	m_bCirtical = bCritical;
}

RwBool CSobAttackCtrlData::GetCritical(void) const
{
	return m_bCirtical;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSobHurtCtrlData::CSobHurtCtrlData()
{
	m_uiAttackerSerialId = INVALID_SERIAL_ID;
	m_uiDefenderSerialId = INVALID_SERIAL_ID;
	m_uiDamage = 0;
	m_uiCurrentHp = 0;
	m_uiStatusSerialId = INVALID_SERIAL_ID;

	SetType(SOB_CTRLDATA_HURT);
}

CSobHurtCtrlData::~CSobHurtCtrlData()
{
}

void CSobHurtCtrlData::Reset(void)
{
	m_uiAttackerSerialId = INVALID_SERIAL_ID;
	m_uiDefenderSerialId = INVALID_SERIAL_ID;
	m_uiDamage = 0;
	m_uiCurrentHp = 0;
}

void CSobHurtCtrlData::Dump(CNtlDumpTarget &dump)
{
	CSobCtrlData::Dump(dump);
}


void* CSobHurtCtrlData::operator new(size_t size)
{
	NTL_FUNCTION("CSobHurtCtrlData::operator new");
    
	NTL_RETURN(CSobCtrlDataStation::AllocCtrlData(SOB_CTRLDATA_HURT));
}

void CSobHurtCtrlData::operator delete(void *pObj)
{
	NTL_FUNCTION("CSobHurtCtrlData::operator delete");

	CSobCtrlDataStation::FreeCtrlData(SOB_CTRLDATA_HURT, pObj);

	NTL_RETURNVOID();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSobJumpCtrlData::CSobJumpCtrlData()
{
	m_eJumpState = CTRL_JUMP_NONE;
	m_eJumpType  = JUMP_STAND;

	SetType(SOB_CTRLDATA_JUMP);
}

CSobJumpCtrlData::~CSobJumpCtrlData()
{
}

void CSobJumpCtrlData::Reset(void)
{
	m_eJumpState = CTRL_JUMP_NONE;
	m_eJumpType  = JUMP_STAND;
}


void CSobJumpCtrlData::Dump(CNtlDumpTarget &dump)
{
	CSobCtrlData::Dump(dump);
}


void* CSobJumpCtrlData::operator new(size_t size)
{
	NTL_FUNCTION("CSobJumpCtrlData::operator new");
    
	NTL_RETURN(CSobCtrlDataStation::AllocCtrlData(SOB_CTRLDATA_JUMP));
}

void CSobJumpCtrlData::operator delete(void *pObj)
{
	NTL_FUNCTION("CSobJumpCtrlData::operator delete");

	CSobCtrlDataStation::FreeCtrlData(SOB_CTRLDATA_JUMP, pObj);

	NTL_RETURNVOID();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSobSkillCastingCtrlData::CSobSkillCastingCtrlData()
{
	m_uiSkillType = 0;
	SetType(SOB_CTRLDATA_SKILL_CASTING);
}

CSobSkillCastingCtrlData::~CSobSkillCastingCtrlData()
{
	Reset();
}

void CSobSkillCastingCtrlData::Reset(void)
{
	m_uiSkillType = 0;
	m_uiTargetSerialId = INVALID_SERIAL_ID;
	m_bFire = FALSE;
	m_bExplosion = FALSE;

	std::list<CNtlPLEntity*>::iterator it;
	for(it = m_listPLEntity.begin(); it != m_listPLEntity.end(); ++it)
	{
		CNtlPLEntity *pPLEntity = (*it);
		GetSceneManager()->DeleteEntity(pPLEntity);
	}

	m_listPLEntity.clear(); 
}


void* CSobSkillCastingCtrlData::operator new(size_t size)
{
	NTL_FUNCTION("CSobSkillCastingCtrlData::operator new");
    
	NTL_RETURN(CSobCtrlDataStation::AllocCtrlData(SOB_CTRLDATA_SKILL_CASTING));
}

void CSobSkillCastingCtrlData::operator delete(void *pObj)
{
	NTL_FUNCTION("CSobSkillCastingCtrlData::operator delete");

	CSobCtrlDataStation::FreeCtrlData(SOB_CTRLDATA_SKILL_CASTING, pObj);

	NTL_RETURNVOID();
}

void CSobSkillCastingCtrlData::PushPLEntity(const CNtlPLEntity *pPLEntity)
{
	m_listPLEntity.push_back(const_cast<CNtlPLEntity*>(pPLEntity)); 
}

void CSobSkillCastingCtrlData::SetSkillType(RwUInt32 uiType)
{
	m_uiSkillType = uiType;
}

RwUInt32 CSobSkillCastingCtrlData::GetSkillType(void)
{
	return m_uiSkillType;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSobSkillActCtrlData::CSobSkillActCtrlData()
{
	SetType(SOB_CTRLDATA_SKILL_ACT);
}

CSobSkillActCtrlData::~CSobSkillActCtrlData()
{
	Reset();
}

void CSobSkillActCtrlData::Reset(void)
{
	std::list<CNtlPLEntity*>::iterator it;
	for(it = m_listPLEntity.begin(); it != m_listPLEntity.end(); ++it)
	{
		CNtlPLEntity *pPLEntity = (*it);
		GetSceneManager()->DeleteEntity(pPLEntity);
	}

	m_listPLEntity.clear(); 
}

void* CSobSkillActCtrlData::operator new(size_t size)
{
	NTL_FUNCTION("CSobSkillActCtrlData::operator new");
    
	NTL_RETURN(CSobCtrlDataStation::AllocCtrlData(SOB_CTRLDATA_SKILL_ACT));
}

void CSobSkillActCtrlData::operator delete(void *pObj)
{
	NTL_FUNCTION("CSobSkillActCtrlData::operator delete");

	CSobCtrlDataStation::FreeCtrlData(SOB_CTRLDATA_SKILL_ACT, pObj);

	NTL_RETURNVOID();
}

void CSobSkillActCtrlData::PushPLEntity(const CNtlPLEntity *pPLEntity)
{
	m_listPLEntity.push_back(const_cast<CNtlPLEntity*>(pPLEntity)); 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

RwFreeList* CSobCtrlDataStation::m_pNormalFreeList = NULL;
RwFreeList* CSobCtrlDataStation::m_pMoveFreeList = NULL;
RwFreeList* CSobCtrlDataStation::m_pAttackFreeList = NULL;
RwFreeList* CSobCtrlDataStation::m_pHurtFreeList = NULL;
RwFreeList* CSobCtrlDataStation::m_pJumpFreeList = NULL;
RwFreeList* CSobCtrlDataStation::m_pSkillCastingFreeList = NULL;
RwFreeList* CSobCtrlDataStation::m_pSkillActFreeList = NULL;


RwBool CSobCtrlDataStation::AllocFreeList(void)
{
	NTL_FUNCTION("CSobCtrlDataStation::AllocFreeList");

	NTL_PRE(m_pMoveFreeList == NULL);

	RwUInt32 iBlockSize = 30;

	m_pNormalFreeList = RwFreeListCreate(sizeof(CSobNormalCtrlData), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pNormalFreeList, "simulation control normal data failed to allocate free list");

    m_pMoveFreeList = RwFreeListCreate(sizeof(CSobMoveCtrlData), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pMoveFreeList, "simulation control move data failed to allocate free list");
	
    m_pAttackFreeList = RwFreeListCreate(sizeof(CSobAttackCtrlData), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pAttackFreeList, "simulation control attack data failed to allocate free list");

	m_pHurtFreeList = RwFreeListCreate(sizeof(CSobHurtCtrlData), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pHurtFreeList, "simulation control hurt data failed to allocate free list");

    m_pJumpFreeList = RwFreeListCreate(sizeof(CSobJumpCtrlData), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pJumpFreeList, "simulation control jump data failed to allocate free list");

	m_pSkillCastingFreeList = RwFreeListCreate(sizeof(CSobSkillCastingCtrlData), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pSkillCastingFreeList, "simulation control skill casting data failed to allocate free list");

    m_pSkillActFreeList = RwFreeListCreate(sizeof(CSobSkillActCtrlData), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);
    RWS_ASSERT(m_pSkillActFreeList, "simulation control skill act data failed to allocate free list");
	
	NTL_RETURN(TRUE);
}

void CSobCtrlDataStation::DeAllocFreeList(void)
{
	NTL_FUNCTION("CSobCtrlDataStation::DeAllocFreeList");

	if(m_pNormalFreeList)
    {
        RwFreeListDestroy(m_pNormalFreeList);
        m_pNormalFreeList = NULL;
    }

	if(m_pMoveFreeList)
    {
        RwFreeListDestroy(m_pMoveFreeList);
        m_pMoveFreeList = NULL;
    }

	if(m_pAttackFreeList)
    {
        RwFreeListDestroy(m_pAttackFreeList);
        m_pAttackFreeList = NULL;
    }

	if(m_pHurtFreeList)
	{
		RwFreeListDestroy(m_pHurtFreeList);
        m_pHurtFreeList = NULL;
	}

	if(m_pJumpFreeList)
    {
        RwFreeListDestroy(m_pJumpFreeList);
        m_pJumpFreeList = NULL;
    }

	if(m_pSkillCastingFreeList)
	{
		RwFreeListDestroy(m_pSkillCastingFreeList);
        m_pSkillCastingFreeList = NULL;
	}

	if(m_pSkillActFreeList)
    {
        RwFreeListDestroy(m_pSkillActFreeList);
        m_pSkillActFreeList = NULL;
    }
	
    NTL_RETURNVOID();
}


void* CSobCtrlDataStation::AllocCtrlData(ESobCtrlDataType eType)
{
	NTL_FUNCTION("CSobCtrlDataStation::AllocCtrlData");
	
	switch(eType)
	{
	case SOB_CTRLDATA_NORMAL:
		NTL_RETURN(RwFreeListAlloc(m_pNormalFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	case SOB_CTRLDATA_MOVE:
		NTL_RETURN(RwFreeListAlloc(m_pMoveFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	case SOB_CTRLDATA_ATTACK:
		NTL_RETURN(RwFreeListAlloc(m_pAttackFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	case SOB_CTRLDATA_HURT:
		NTL_RETURN(RwFreeListAlloc(m_pHurtFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	case SOB_CTRLDATA_JUMP:
		NTL_RETURN(RwFreeListAlloc(m_pJumpFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	case SOB_CTRLDATA_SKILL_CASTING:
		NTL_RETURN(RwFreeListAlloc(m_pSkillCastingFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	case SOB_CTRLDATA_SKILL_ACT:
		NTL_RETURN(RwFreeListAlloc(m_pSkillActFreeList, rwMEMHINTDUR_GLOBAL));
		break;
	}
	
	NTL_RETURN(NULL);
}

void CSobCtrlDataStation::FreeCtrlData(ESobCtrlDataType eType, void *pObj)
{
	NTL_FUNCTION("CSobCtrlDataStation::FreeCtrlData");
	NTL_PRE(pObj);

	switch(eType)
	{
	case SOB_CTRLDATA_NORMAL:
		RwFreeListFree(m_pNormalFreeList, pObj);
		break;
	case SOB_CTRLDATA_MOVE:
		RwFreeListFree(m_pMoveFreeList, pObj);
		break;
	case SOB_CTRLDATA_ATTACK:
		RwFreeListFree(m_pAttackFreeList, pObj);
		break;
	case SOB_CTRLDATA_HURT:
		RwFreeListFree(m_pHurtFreeList, pObj);
		break;
	case SOB_CTRLDATA_JUMP:
		RwFreeListFree(m_pJumpFreeList, pObj);
		break;
	case SOB_CTRLDATA_SKILL_CASTING:
		RwFreeListFree(m_pSkillCastingFreeList, pObj);
		break;
	case SOB_CTRLDATA_SKILL_ACT:
		RwFreeListFree(m_pSkillActFreeList, pObj);
		break;
	}

	NTL_RETURNVOID();
}


CSobCtrlDataStation::CSobCtrlDataStation()
{
	for(RwInt32 i = 0; i < MAX_SOB_CTRLDATA; ++i)
	{
		m_pCtrlData[i] = NULL;
	}
}

CSobCtrlDataStation::~CSobCtrlDataStation()
{
	Destroy();
}

RwBool CSobCtrlDataStation::Create(void)
{
	NTL_FUNCTION("CSobCtrlDataStation::Create");
	
	NTL_RETURN(TRUE);
}

void CSobCtrlDataStation::Destroy(void)
{
	for(RwInt32 i = 0; i < MAX_SOB_CTRLDATA; ++i)
	{
		NTL_DELETE( m_pCtrlData[i] );
	}
}

RwBool CSobCtrlDataStation::CreateCtrlData(ESobCtrlDataType eType)
{
	NTL_FUNCTION("CSobCtrlDataStation::CreateCtrlData");

	NTL_ASSERTE(m_pCtrlData[eType] == NULL);

	switch(eType)
	{
	case SOB_CTRLDATA_NORMAL:
		m_pCtrlData[eType] = NTL_NEW CSobNormalCtrlData;
		break;
	case SOB_CTRLDATA_MOVE:
		m_pCtrlData[eType] = NTL_NEW CSobMoveCtrlData;
		break;
	case SOB_CTRLDATA_ATTACK:
		m_pCtrlData[eType] = NTL_NEW CSobAttackCtrlData;
		break;
	case SOB_CTRLDATA_HURT:
		m_pCtrlData[eType] = NTL_NEW CSobHurtCtrlData;
		break;
	case SOB_CTRLDATA_JUMP:
		m_pCtrlData[eType] = NTL_NEW CSobJumpCtrlData;
		break;
	case SOB_CTRLDATA_SKILL_CASTING:
		m_pCtrlData[eType] = NTL_NEW CSobSkillCastingCtrlData;
		break;
	case SOB_CTRLDATA_SKILL_ACT:
		m_pCtrlData[eType] = NTL_NEW CSobSkillActCtrlData;
		break;
	default:
		NTL_RETURN(FALSE);
		break;
	}
	
	NTL_RETURN(TRUE);
}

void CSobCtrlDataStation::ResetCtrlDataAll(void)
{
	for(RwInt32 i = 0; i < MAX_SOB_CTRLDATA; ++i)
	{
		if(m_pCtrlData[i])
			m_pCtrlData[i]->Reset(); 
	}
}

void CSobCtrlDataStation::ResetCtrlData(ESobCtrlDataType eType)
{
	if(m_pCtrlData[eType])
		m_pCtrlData[eType]->Reset(); 
}

CSobCtrlData* CSobCtrlDataStation::GetCtrlData(ESobCtrlDataType eType)
{
	return m_pCtrlData[eType];
}

void CSobCtrlDataStation::Dump(CNtlDumpTarget &dump)
{
	dump << "\n"; 
	dump.Dump(); 

	for(RwInt32 i = 0; i < MAX_SOB_CTRLDATA; ++i)
	{
		if(m_pCtrlData[i])
			m_pCtrlData[i]->Dump(dump); 
	}
}

