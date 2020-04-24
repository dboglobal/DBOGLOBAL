#pragma once

#include "NtlBehaviorCharMove.h"

class CNtlBehaviorCharAirMove : public CNtlBehaviorCharMove
{
	DECLEAR_MEMORY_POOL(CNtlBehaviorCharAirMove, NTL_DEFAULT_MEMORY_POOL)
	DECLEAR_BEHAVIOR_FACTROY(CNtlBehaviorCharAirMove)

public:

	CNtlBehaviorCharAirMove();
	~CNtlBehaviorCharAirMove();

	virtual void		Enter(void);
	virtual void		Exit(void);
	virtual void		Update(RwReal fElapsed);
	virtual RwUInt32	HandleEvents(RWS::CMsg &pMsg);
	virtual void		UpdateData(void);


protected:

	// move stuff
	SMoveStuff			m_MoveStuff;
	SWorldHeightStuff	m_sHStuff;				/** world height stuff */

	CNtlPLEntity		*m_pPLAirEffect;
	RwV3d				m_vEffectOffset;

protected:

	void	SetAnim(RwUInt8 byMoveFlags);

	void	CreateAirEffect(const RwChar* effectName);
	void	DestroyAirEffect(void);


	void	UpdateAirEffect(RwV3d& vDir);

private:

	RwBool	UpdatePositionMove(SMoveStuff *pMoveStuff, OUT RwV3d& vPos, RwBool bIncHeight, RwReal fElapsed, RwReal fSpeed);

	RwBool	UpdateAirStart(SMoveStuff *pMoveStuff, RwReal fElapsed);
	RwBool	UpdateAirFly(SMoveStuff *pMoveStuff, RwReal fElapsed);
	RwBool	UpdateAirDash(SMoveStuff *pMoveStuff, RwReal fElapsed);
	RwBool	UpdateAirAccel(SMoveStuff *pMoveStuff, RwReal fElapsed);

	RwBool	UpdateMoveSync(RwV3d vPos, RwReal fElapsedTime);       ///< Udpate function to calculate the current MoveSync value

	void	SetFalling();
};