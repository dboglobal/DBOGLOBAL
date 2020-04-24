#include "precomp_ntlpresentation.h"
#include "SpawnMacro.h"


CSpawnMacro::CSpawnMacro(void)
{
	m_CoolTime		= 1;
	m_MoveType		= 1;
	m_WanderRange	= 1.0f;
	m_MoveRange		= 1.0f;
	m_DelayTime		= 1.0f;
}

CSpawnMacro::~CSpawnMacro(void)
{
}

VOID CSpawnMacro::SetProp(CSpawnMacro& _SpawnMacro)
{
	m_CoolTime		= _SpawnMacro.m_CoolTime;
	m_MoveType		= _SpawnMacro.m_MoveType;		
	m_WanderRange	= _SpawnMacro.m_WanderRange;
	m_MoveRange		= _SpawnMacro.m_MoveRange;
	m_DelayTime		= _SpawnMacro.m_DelayTime;
}

VOID CSpawnMacro::SetProp(RwInt32 _CoolTime, RwInt32 _MoveType, RwReal _WanderRange, RwReal _MoveRange, RwReal _DelayTime)
{
	m_CoolTime		= _CoolTime;
	m_MoveType		= _MoveType;	
	m_WanderRange	= _WanderRange;
	m_MoveRange		= _MoveRange;
	m_DelayTime		= _DelayTime;
}	
