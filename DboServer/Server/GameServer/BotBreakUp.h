#ifndef __INC_DBOG_BOTBREAKUP_H__
#define __INC_DBOG_BOTBREAKUP_H__

#include "NtlSharedType.h"
class CCharacter;
class CNpc;
class CNtlVector;
#include <vector>


class CBotBreakUp
{
	typedef std::vector<HOBJECT> HOBJ_VEC;
	typedef std::vector<float> RADIAN_VEC;

public:
	enum eDIR
	{
		DIR_1 = 0x0,
		DIR_2 = 0x1,
		DIR_3 = 0x2,
		DIR_4 = 0x3,
		DIR_COUNT = 0x4,
		INVALID_DIR = 0xFFFFFFFF,
	};

	enum eBREAKUP_MOVETYPE
	{
		BREAKUP_MOVETYPE_LEVEL1 = 0x0,
		BREAKUP_MOVETYPE_LEVEL2 = 0x1,
		BREAKUP_MOVETYPE_LEVEL3 = 0x2,
		BREAKUP_MOVETYPE_LEVEL4 = 0x3,
		BREAKUP_MOVETYPE_CHASE = 0x4,
	};

	enum eDEFINE
	{
		BREAKUP_LEVEL1_SIZE = 0x8,
		BREAKUP_MAX_SIZE = 0x10,
	};


public:
	CBotBreakUp();
	virtual	~CBotBreakUp();

public:

	bool				Create(CCharacter* pChar);
	void				Destroy();
	void				Init();
	void				Reset();
	bool				Reserve(BYTE byCurBotCount);
	bool				AddNavMove(CNpc *pBotNpc, float fAttackRange, BYTE byBotCount, CBotBreakUp::eBREAKUP_MOVETYPE eMoveType);
	void				ResetPosition();
	void				UnsetBreakUp(HOBJECT hEnemy);

private:

	bool				GetPosition(CNpc *pBotNpc, std::vector<CNtlVector>& rDir, BYTE *byDirCount, const BYTE byBotCount);
	bool				GetPosition(CNpc *pBotNpc, CNtlVector& rDir, const BYTE byBotCount);

	CBotBreakUp::eDIR	GetCoordinates(int nPosIdx);

	CNtlVector&			GetRadianToCoordinates(CNtlVector& pRes, CBotBreakUp::eDIR eDir, float fRadian);
	BYTE				GetArrayPosition(CNpc *pBotNpc, CBotBreakUp::eDIR& rDir);

	bool				IsNullPos(BYTE byArrayPos);

	void				SetBreakUp(HOBJECT hEnemy, BYTE byArrayPos);
	void				AllUnsetBreakUp();
	void				InitRadian();

public:

	int					GetChaseSlot();
	void				SetChaseSlot(int nBreakUpSlot);
	int					CalcBreakUpSlot(CCharacter* pChar);
	void				SetBreakUpSlot(int nSlot, CCharacter* pChar);
	CNtlVector			GetBreakUpLocation(CNtlVector& rVecRes, int nBreakUpSlot, CCharacter* pAttacker);

private:

	bool				AddMove_Level1(CNpc* pBotNpc, float fAttackRange, BYTE byBotCount);
	bool				AddMove_Level2(CNpc* pBotNpc, float fAttackRange, BYTE byBotCount);
	bool				AddMove_Level3(CNpc* pBotNpc, float fAttackRange, BYTE byBotCount);
	bool				AddMove_Level4(CNpc* pBotNpc, float fAttackRange, BYTE byBotCount);
	bool				AddMove_Chase(CNpc* pBotNpc, float fAttackRange, BYTE byBotCount);
	bool				Following(CNpc* pBotNpc, CNtlVector& rDestLoc);
	

private:

	CCharacter* m_pOwnerChar;
	BYTE		m_byOldMaxMobCount;
	BYTE		m_byCurMobCount;

	HOBJ_VEC	m_vecBotSave;
	RADIAN_VEC	m_vecRadian;

	float		m_fRed;
	int			m_nBreakUpSlot;
	int			m_ahBreakup[16];

};

#endif