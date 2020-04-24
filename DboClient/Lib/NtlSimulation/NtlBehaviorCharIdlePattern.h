/*****************************************************************************
 *
 * File			: NtlBehaviorCharIdlePattern.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2008. 12. 23	
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_BEHAVIOR_CHAR_IDLE_PATTERN_H__
#define __NTL_BEHAVIOR_CHAR_IDLE_PATTERN_H__

class CNtlSobActor;
class CMobMovePatternTable;

class CNtlBehaviorCharIdlePattern
{
private:

	enum ECharIdlePattern
	{
		IDLE_PATTERN_MOVE,
		IDLE_PATTERN_NORMAL_IDLE,
		IDLE_PATTERN_SPECIAL_IDLE1,
		IDLE_PATTERN_SPECIAL_IDLE2,
	};

	ECharIdlePattern	m_eCurrPattern;
	RwReal				m_byCurrTime;
	RwReal				m_byIdleTime;

	CNtlSobActor		*m_pActor;

private:

	RwBool					IsVaildIncrementPattern(void);

	void					IncrementTablePatternId(void);

	RwUInt8					GetTablePatternId(void);

	RwUInt8					GetCurrentUnitId(void);

	void					MoveProc(void);

	void					NormalIdleProc(RwReal fLifeTime);

	void					NormalSpecialIdleProc(ECharIdlePattern ePattern);

	void					ExceptIdleProc(void);

	void					CurrentUnitProc(void);

public:

	CNtlBehaviorCharIdlePattern();
	~CNtlBehaviorCharIdlePattern();

	void	SetActor(CNtlSobActor *pActor);

	void	Update(RwReal fElapsed);

	void	SyncUnit(void);

	void	NextUnit(void);
};


#endif
