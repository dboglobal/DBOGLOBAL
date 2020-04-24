/*****************************************************************************
 *
 * File			: NtlSobTriggerObject.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 11. 16	
 * Abstract		: Simulation trigger object
 *****************************************************************************
 * Desc         : trigger가 설정되어 있는 object 대해 기능을 제공해주며, 
 *				  서버와 동기를 이룬다.
 *				  handle은 일반적인 simulation object 방식처럼 서버에 의한 handle 
 *                할당 방식이 아니라, 서버와 클라이언트간의 동일한 알고리즘을
 *                사용하여, 클라이언트 자체적으로 handle을 할당한다.
 *****************************************************************************/

#ifndef __NTL_SOB_TRIGGEROBJECT_H__
#define __NTL_SOB_TRIGGEROBJECT_H__

#include "NtlSobActor.h"

struct SNtlEventTObjectUpdateState;

struct SNtlSobTriggerObjectState
{
	RwUInt8			byMainState;
	RwUInt8			bySubState;
};

class CNtlSobTriggerObject : public CNtlSobActor
{
	DECLEAR_MEMORY_POOL( CNtlSobTriggerObject, NTL_DEFAULT_MEMORY_POOL )

protected:

	RwReal			m_fTime;
	RwUInt8			m_byQuestMark;
	RwBool			m_bActiveQuestMark;
	RwUInt16		m_tID;

	// 트리거 오브젝트의 상태
	RwUInt8						m_byStateType;
	SNtlSobTriggerObjectState	m_sPreState;
	SNtlSobTriggerObjectState	m_sCurState;

protected:

	void	QeustMarkCheck(RwReal fElapsed);

public:

	CNtlSobTriggerObject(VOID);
	virtual ~CNtlSobTriggerObject(VOID);

	virtual RwBool Create(VOID);
	virtual void Destroy(VOID);

	virtual CNtlFSMLayer* CreateFSM(void);

	virtual CNtlSobInfluence* CreateSobInfluence(void);

	virtual void Update( RwReal fElapsed );

	virtual void HandleEvents( RWS::CMsg& pMsg );

	virtual void SetDirection(const RwV3d *pDir);

public:

	RwBool CanClicked(void);

	RwBool IsScouterObject( void );

	RwBool IsTLQObject( void );

	RwBool IsPIDObject( void );

	RwBool IsDojoSealObject( void );

	void ActiveQuestMark(RwBool bQuestMark);

	RwBool ShowTOForSCT( RwBool bShow );

	SNtlSobTriggerObjectState GetCurrentTriggerObject(void) { return m_sCurState; }

protected:

	void InitState( void );
	void UpdateState( SNtlEventTObjectUpdateState *pSobTObjUpdateState );

	void PlayClickSound( void );
};

#endif