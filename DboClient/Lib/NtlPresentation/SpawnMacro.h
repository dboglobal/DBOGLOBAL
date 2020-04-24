#pragma once


class CSpawnMacro
{
public:
	CSpawnMacro(void);
	virtual ~CSpawnMacro(void);

public:
	RwInt32	m_CoolTime;
	RwInt32	m_MoveType;
	RwReal	m_WanderRange;
	RwReal	m_MoveRange;
	RwReal	m_DelayTime;

public:
	inline void operator = (CSpawnMacro SM)
	{
		m_CoolTime		= SM.m_CoolTime;
		m_MoveType		= SM.m_MoveType;
		m_WanderRange	= SM.m_WanderRange;
		m_MoveRange		= SM.m_MoveRange;
		m_DelayTime		= SM.m_DelayTime;
	}

public:
	VOID	SetProp(RwInt32 _CoolTime, RwInt32 _MoveType, RwReal _WanderRange, RwReal _MoveRange, RwReal _DelayTime);
	VOID	SetProp(CSpawnMacro& _SpawnMacro);
};

class CSpawnMacroIndicator : public CSpawnMacro
{
public:
	CSpawnMacroIndicator(void) { m_MacroIdx = 0; }
	virtual ~CSpawnMacroIndicator(void) { ; }

public:
	RwInt32	m_MacroIdx;
};