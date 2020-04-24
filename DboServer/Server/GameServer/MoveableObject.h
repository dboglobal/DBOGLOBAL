#ifndef __INC_DBOG_MOVEABLE_OBJECT_H__
#define __INC_DBOG_MOVEABLE_OBJECT_H__

#include "SpawnObject.h"
#include "NtlMovement.h"

//====================================================================
//  CMovableObject
//  class for every object which can move
//====================================================================
class CMoveableObject : public CSpawnObject
{

public:
	CMoveableObject(eOBJTYPE eObjType);
	virtual	~CMoveableObject();


	bool			Create();
	void			Destroy();

	void			Init();

public:

	virtual void	UpdateMove(DWORD dwTickDiff, float fMultiple);
	virtual bool	UpdateFollow(DWORD dwTickDiff, float fMultiple);

	virtual float	GetCurSpeed() { return NTL_DEFAULT_MOVEMENT_SPEED; }
	virtual float	GetWalkingSpeed() { return NTL_DEFAULT_MOVEMENT_SPEED; }
	virtual float	GetRunSpeed() { return NTL_DEFAULT_MOVEMENT_SPEED; }

	virtual void	TickProcess(DWORD dwTickDiff, float fMultiple);

	CNtlVector&		GetDestLoc() { return m_vDestLoc; }
	void			SetDestLoc(CNtlVector& rLoc) { m_vDestLoc.operator=(rLoc); }
	void			SetDestLoc(sVECTOR3& rLoc) { m_vDestLoc.CopyFrom(rLoc); }

	bool			SetMoveDirection(BYTE byMoveDirection);
	inline BYTE		GetMoveDirection() { return this->m_byMoveDirection; }
	inline void		SetMoveFlag(BYTE byFlag) { this->m_byMoveFlag = byFlag; }
	inline BYTE		GetMoveFlag() { return this->m_byMoveFlag; }

	inline void		SetCollision(bool bFlag) { m_bIsCollision = bFlag; }
	inline bool		GetCollision() { return m_bIsCollision; }

	inline void		SetHaveSecondDestLoc(bool bHave) { m_bHaveSecondDestLoc = bHave; }
	inline bool		GetHaveSecondDestLoc() { return m_bHaveSecondDestLoc; }

	BYTE			GetDestLocCount();

	CNtlVector&		GetSecondDestLoc() { return m_vSecondDestLoc; }
	inline void		SetSecondDestLoc(sVECTOR3& rLoc) { m_vSecondDestLoc.CopyFrom(rLoc); }

	bool			SetMultipleDestLoc(BYTE byDestLocCount, sVECTOR3* pavDestLoc);

	bool			IsMoveComplete();

	HOBJECT			GetFollowTarget() { return m_hFollowTarget; }
	void			SetFollowTarget(HOBJECT hTarget);

	float			GetFollowDistance() { return m_fFollowDistance; }
	void			SetFollowDistance(float fFollowDistance);

	BYTE			GetFollowMoveReason() { return m_byFollowMoveReason; }
	void			SetFollowMoveReason(BYTE byReason) { m_byFollowMoveReason = byReason; }

	bool			IsPassiveDashComplete();

	bool			JumpStart(CNtlVector& rvCurDir);
	bool			JumpEnd();
	inline void		SetJumpHeading(CNtlVector& rDir) { m_vJumpHeading.operator=(rDir); }
	CNtlVector&		GetJumpHeading(){ return m_vJumpHeading; }

	bool			PredictMovement(DWORD dwTickDiff, CNtlVector& rvNewDir, CNtlVector& rvNewLoc, CNtlVector& rvNewDestLoc, std::list<sVECTOR3> *rlistConsumedDestLoc, float *rfActualSpeed);

	bool			CalculateJumpDir(CNtlVector* pvNewCurDir, CNtlVector* pvJumpHeading); // to do

	bool			ConsumeDestLoc(std::list<sVECTOR3>& rlistConsumedDestLoc);

	void			AdjustCurSpeed(float *fActualSpeed, float *fTurningSpeedRatio);


private:

	CNtlVector			m_vDestLoc;
	CNtlVector			m_vSecondDestLoc;
	bool				m_bHaveSecondDestLoc;
	std::list<sVECTOR3> m_listRemainingDestLoc;
	CNtlVector			m_vDestTurningDir;
	CNtlVector			m_vJumpHeading;
	BYTE				m_byMoveDirection;
	BYTE				m_byMoveFlag;
	HOBJECT				m_hFollowTarget;
	float				m_fFollowDistance;
	BYTE				m_byFollowMoveReason;

	bool				m_bIsCollision;

};

#endif
