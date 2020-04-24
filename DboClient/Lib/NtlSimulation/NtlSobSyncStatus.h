/*****************************************************************************
 *
 * File			: NtlSobSyncStatus.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 16	
 * Abstract		: Simulation Status object
 *****************************************************************************
 * Desc         : 서버에서 내려주는 damage와 hp/mp/cp 채우기 packet에 동기화 처리에 사용한다.
 *				  서버에서 먼저 damage를 내려주어도, 공격 animation 동기화로 인하여, 나중에 처리될 수 있다.
                  이런 경우에 current hp를 그대로 actor에 적용한다면, avatar gui hp bar가 들쑥날쑥 이상해
				  보인다. 이런 경우에 animation 동기화 damage 및 hp/mp/cp 처리를 하기 위하여 만들었다.
 *****************************************************************************/

#ifndef __NTL_SOB_SYNC_STATUS_H__
#define __NTL_SOB_SYNC_STATUS_H__

#include "NtlSob.h"

class CNtlSobActor;

class CNtlSobSyncStatus : public CNtlSob
{
protected:

	RwBool m_bActive;
	RwBool m_bFinish;
	
	/**
	* 생성자
	*/
	CNtlSobSyncStatus();
	
public:

	/**
	* 소멸자
	*/
	virtual ~CNtlSobSyncStatus();

	virtual RwBool Create(void);

	virtual void Destroy(void);

	virtual RwBool IsFinish(void);

	virtual void Update(RwReal fElapsed);

	
public:

	void Active(void);

	virtual void EventGenerator(void) {}
};

inline void CNtlSobSyncStatus::Active(void)
{
	m_bActive = TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlSobAttackSyncStatus : public CNtlSobSyncStatus
{
	DECLEAR_MEMORY_POOL(CNtlSobAttackSyncStatus, NTL_DEFAULT_MEMORY_POOL)

private:

	RwReal m_fActiveTime;
	RwReal m_fCurrTime;

public:

	CNtlSobAttackSyncStatus();
	virtual ~CNtlSobAttackSyncStatus();

	virtual void Destroy(void);

	virtual RwBool IsFinish(void);

	virtual void Update(RwReal fElapsed);

	void ActiveTime(RwReal fTime);
};

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlSobDieSyncStatus : public CNtlSobSyncStatus
{
	DECLEAR_MEMORY_POOL(CNtlSobDieSyncStatus, NTL_DEFAULT_MEMORY_POOL)

protected:

	RwReal m_fTime;

public:

	CNtlSobDieSyncStatus();
	virtual ~CNtlSobDieSyncStatus();

	virtual void Destroy(void);
	virtual void EventGenerator(void);
    virtual void Update(RwReal fElapsed);
};


#endif
