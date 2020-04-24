/*****************************************************************************
 *
 * File			: NtlSobProjectile.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 12. 08	
 * Abstract		: Simulation projectile object
 *****************************************************************************
 * Desc         : 발사체 오브젝트로서, 화살, 폭탄, 미사일, 마법 발사체에 해당한다.
 *				  간단한 상태 머신을 가지고 있다.
                  자신의 owner serial를 기억하고 있다.
				  발사체는 범위 공격, 타겟 공격, 관통 스타일이 존재하며, 타겟 공격의 경우에는 
				  target을 따라갈 수 도 있다.
 *****************************************************************************/

#ifndef __NTL_SOB_PROJECTILE_H__
#define __NTL_SOB_PROJECTILE_H__

#include "NtlSobActor.h"
#include "NtlBehaviorData.h"

class CNtlSobActor;
class CNtlBehaviorBase;

class CNtlSobProjectile : public CNtlSobActor
{
	DECLEAR_MEMORY_POOL(CNtlSobProjectile, NTL_DEFAULT_MEMORY_POOL)
			
protected:

	// attribute
	RwReal	m_fSpeed;								/** speed */
	RwBool	m_bLeftHand;							/** left hand */
	RwV3d	m_vDest;								/** dest position */
	RwReal	m_fTargetLocHeight;						/** target position offset */
	std::list<SHitStuff> m_listHitStuff;			/** hit list */
	std::list<CNtlBehaviorBase*> m_listBehavior;	/** behavior list */

	RwBool	m_bDamageProc;							/** projectile의 damage procedure */
	RwBool	m_bFinish;								/** projectile의 life flag */

	
	// transform
	RwMatrix m_matTrans;

protected:

	// update
	RwBool UpdateBehavior(RwReal fElapsed);
	void UpdateVisual(void);
	
	// event handler
	void CreateEventHandler(RWS::CMsg &pMsg);

	// damage proc
	void DamageProc(void);
	
public:

	CNtlSobProjectile();
	virtual ~CNtlSobProjectile();

	virtual RwBool Create(void);

	virtual void Destroy(void);

	virtual CNtlFSMLayer* CreateFSM(void);

	virtual CNtlSobInfluence* CreateSobInfluence(void);

	virtual void Update(RwReal fElapsed);

	virtual RwBool IsFinish(void);

	virtual void HandleEvents(RWS::CMsg &pMsg);
	
public:

	void SetSpeed(RwReal fSpeed);
	RwReal GetSpeed(void) const;

	void SetLeftHand(RwBool bLeftHand);
	RwBool GetLeftHand(void) const;

	void SetDestPositin(const RwV3d *pDest);
	RwV3d GetDestPosition(void);

	void AddHit(SHitStuff& sHit);
	RwInt32 GetHitCount(void) const;
	RwReal GetHitTargetHeight(void);

	void AddBehavior(CNtlBehaviorBase *pBehavior);
	RwInt32 GetBehaviorCount(void) const;
};

inline void CNtlSobProjectile::SetSpeed(RwReal fSpeed)
{
	m_fSpeed = fSpeed;
}

inline RwReal CNtlSobProjectile::GetSpeed(void) const
{
	return m_fSpeed;
}

inline void CNtlSobProjectile::SetLeftHand(RwBool bLeftHand)
{
	m_bLeftHand = bLeftHand;
}

inline RwBool CNtlSobProjectile::GetLeftHand(void) const
{
	return m_bLeftHand;
}

inline void CNtlSobProjectile::SetDestPositin(const RwV3d *pDest)
{
	RwV3dAssignMacro(&m_vDest, pDest);
}

inline RwV3d CNtlSobProjectile::GetDestPosition(void)
{
	return m_vDest;
}


#endif
