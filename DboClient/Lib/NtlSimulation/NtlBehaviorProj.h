/*****************************************************************************
 *
 * File			: NtlBehaviorProj.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 2. 27	
 * Abstract		: projectile behavior class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_BEHAVIOR_PROJ_H__
#define __NTL_BEHAVIOR_PROJ_H__

#include "NtlBehaviorBase.h"
#include "NtlBehaviorData.h"

class CNtlBehaviorProjectile : public CNtlBehaviorBase
{
public:

	virtual ~CNtlBehaviorProjectile() {}

	virtual RwBool	IsExplosion(void) { return FALSE; }
};

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBehaviorProjBall : public CNtlBehaviorProjectile
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorProjBall, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorProjBall)

private:

	RwReal m_fTargetHeight;

public:

	CNtlBehaviorProjBall(); 
	~CNtlBehaviorProjBall();

	virtual void Enter(void);
	virtual void Update(RwReal fElapsed);
};

///////////////////////////////////////////////////////////////////////////////////////////////

/**
 * \ingroup NtlSimulation
 * \brief 휘어서 상대의 뒤를 공격하는 Projectile
 *
 * \date 2008-10-16
 * \author agebreak
 */
class CNtlBehaviorProjMagare : public CNtlBehaviorProjectile
{
    DECLEAR_MEMORY_POOL(CNtlBehaviorProjMagare, NTL_DEFAULT_MEMORY_POOL)
    DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorProjMagare)

public:
    enum EMagareStatus
    {
        MAGARE_GO,
        MAGARE_MAGARE,
    };

public:
    CNtlBehaviorProjMagare();
    ~CNtlBehaviorProjMagare();

    virtual void Enter();
    virtual void Update(RwReal fElapsed);

protected:
    EMagareStatus   m_eStatus;                      ///< 현재 상태
    RwReal          m_fTotalAngle;                  ///< 회전한 총 각도
    
};

////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlBehaviorProjBeam : public CNtlBehaviorProjectile
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorProjBeam, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorProjBeam)

public:

	CNtlBehaviorProjBeam(); 
	~CNtlBehaviorProjBeam();

	virtual void Enter(void);
	virtual void Update(RwReal fElapsed);
};

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

class CNtlBehaviorProjHisshidan : public CNtlBehaviorProjectile
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorProjHisshidan, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorProjHisshidan)

private:

	RwReal m_fTargetHeight;
	RwBool m_bAngleUse;
	RwReal m_fAngleX;
	RwReal m_fAngleY;

public:

	CNtlBehaviorProjHisshidan(); 
	~CNtlBehaviorProjHisshidan();

	virtual void Enter(void);
	virtual void Update(RwReal fElapsed);

public:

	void	SetAngle(RwReal fAngleX, RwReal fAngleY);		
};

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _SHisshidanPieceSutff
{
	RwReal fAngleX;
	RwReal fAngleY;
	RwChar *pEffName;
}SHisshidanPieceSutff;

class CHisshidanPiece
{
public:

	CNtlSobActor	*m_pActor;
	RwV3d			m_vPos;
	RwV3d			m_vCurrDir;
	RwReal			m_fSpeed;
	RwReal			m_fIncSpeed;
	RwReal			m_fTargetHeight;
	RwMatrix		m_matTrans;

	CNtlPLEntity	*m_pEffect;
	
	RwBool			m_bExplosion;
	RwBool			m_bFinish;

private:

	void			UpdateVisual(void);

public:

	CHisshidanPiece();
	~CHisshidanPiece();

	void			Enter(CNtlSobActor *pActor, RwV3d vPos, RwV3d vCurrDir, const RwChar *pEffName, RwBool bExplosion);
	void			Exit(void);
	void			Update(RwReal fElapsed);
	RwBool			IsFinish(void) const;
};

class CNtlBehaviorProjMultiHisshidan : public CNtlBehaviorProjectile
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorProjMultiHisshidan, NTL_SMALL_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorProjMultiHisshidan)

private:

	typedef std::list<SHisshidanPieceSutff>		ListPieceStuff;
	typedef std::list<CHisshidanPiece*>			ListPiece;
	
	RwReal			m_fTargetHeight;

	ListPieceStuff	m_listPieceStuff;
	ListPiece		m_listPiece;

	RwBool			m_bExplosion;

public:

	CNtlBehaviorProjMultiHisshidan(); 
	~CNtlBehaviorProjMultiHisshidan();

	void			SetPieceStuff(std::list<SHisshidanPieceSutff>& listPieceStuff);

	virtual void	Enter(void);
	virtual void	Update(RwReal fElapsed);

	virtual RwBool	IsExplosion(void);
};

///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

// 마공 포위탄에 쓴다.
class CNtlBehaviorProjHelljone : public CNtlBehaviorProjectile
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorProjHelljone, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorProjHelljone)

private:

	enum 
	{
		PROJ_HELLJONE_WAIT,
		PROJ_HELLJONE_LOOP
	};

	RwReal	m_fTargetHeight;
	RwV3d	m_vCreateOffset;
	RwReal	m_fDelayTime;
	RwReal	m_fCurrTime;
	RwReal	m_fSeqTime;
	RwV3d	m_fBasePos;
	RwUInt8	m_byHelljoneState;
	CNtl2DTimeSequence *m_pSeq;

	void UpdateWait(RwReal fElapsed);
	void UpdateLoop(RwReal fElapsed);

public:

	CNtlBehaviorProjHelljone();
	~CNtlBehaviorProjHelljone();

	virtual void Enter(void);
	virtual void Update(RwReal fElapsed);

public:

	void	SetCreateTargetOffset(RwV3d vOffset);
	void	SetDelayTime(RwReal fDelayTime);
};


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

// drain skill에서 target의 energy를 흡수한다.
class CNtlBehaviorProjSteal : public CNtlBehaviorProjectile
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorProjSteal, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorProjSteal)

private:

	enum 
	{
		PROJ_STEAL_WAIT,
		PROJ_STEAL_LOOP
	};

	RwReal	m_fTargetHeight;
	RwReal	m_fWaitTime;
	RwUInt8	m_byStealState;

	void UpdateWait(RwReal fElapsed);
	void UpdateLoop(RwReal fElapsed);

public:

	static RwReal	m_ffIncSpeed;
	static RwReal	m_fWaitCheckTime;

public:

	CNtlBehaviorProjSteal();
	~CNtlBehaviorProjSteal();

	virtual void Enter(void);
	virtual void Update(RwReal fElapsed);
};

#endif