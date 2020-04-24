/*****************************************************************************
 *
 * File			: NtlSobActor.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 9. 01	
 * Abstract		: Simulation object actor
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SOB_ACTOR_H__
#define __NTL_SOB_ACTOR_H__

#include "NtlSob.h"
#include "NtlPLGlobal.h"
#include "NtlCharacter.h"

class CNtlPLCharacter;

class CNtlFSMStateBase;
class CNtlFSMLayer;
class CNtlSobInfluence;

class CNtlSobActor : public CNtlSob
{
protected:

	RwUInt32			m_uiActFlags;

	CNtlFSMLayer		*m_pFSMLayer;				/** fsm */
	CNtlSobInfluence	*m_pSobInfluence;			/** influence */
	RwUInt8				m_byServerSyncAvatarType;	/** avatar type */

	SERIAL_HANDLE		m_hFinialHurtSerialId;
	RwReal				m_fHurtOverTime;

	typedef void (CNtlSobActor::*FuncEventHander)(RWS::CMsg &pMsg);
	typedef std::map<const RWS::CRegisteredMsgs*, FuncEventHander> MapFuncEventHandler;
	MapFuncEventHandler m_mapEventFunc;
	
protected:

	// 생성자.
	CNtlSobActor();
	
public:
	
	virtual ~CNtlSobActor();

	virtual RwBool Create(void);

	virtual void Destroy(void);

	virtual void Update(RwReal fElapsed);

	virtual void ServerChangeOut(void);

	virtual void ServerChangeIn(void);

	virtual CNtlFSMLayer* CreateFSM(void) { return NULL; }

	virtual CNtlSobInfluence* CreateSobInfluence(void) { return NULL; }

	virtual void HandleEvents(RWS::CMsg &pMsg);

	virtual void Dump(CNtlDumpTarget& dump);

	virtual void AddSob(CNtlSob *pSobObj);

	virtual void RemoveSob(CNtlSob *pSobObj);

public:

	virtual RwBool CanMove(void);
	virtual RwBool CanAttack(void);
	virtual RwBool CanHurted(void);
	virtual RwBool CanClick(void);	// 이 액터를 클릭해서 선택할 수 있는지에 대한 유무

	virtual RwBool IsAirMode(void);
	virtual RwBool IsFightingMode(void);
	virtual RwBool IsClickDisable(void);
	virtual RwBool IsClientUIDisable(void);
	virtual RwBool IsDie(void);    

public:

	void				SetActorFlags(RwUInt32 uiActorFlags);
	RwUInt32			GetActorFlags(void) const;

	void				SetServerSyncAvatarType(RwUInt8 byAvatarType);
	RwUInt8				GetServerSyncAvatarType(void) const;

	void				NotifyStateEnter(RwUInt32 uiStateId);
	void				NotifyStateExit(RwUInt32 uiOldState, RwUInt32 uiNewState);

	void				SetFinialHurtSerialId(SERIAL_HANDLE hAttackSerialId);
	SERIAL_HANDLE		GetFinialHurtSerialId(void) const;

	CNtlFSMLayer*		GetFSMLayer(void) const;
	sCHARSTATE*			GetServerFullState(void) const;
	CNtlFSMStateBase*	GetCurrentState(void);
	CNtlSobInfluence*	GetSobInfluence(void) const;  

	/////////////////////////////////////////////////////
	// control data station
	virtual void*		GetBehaviorData(void) { return NULL; }


///////////////////////////////////////////////////////
// event handler
///////////////////////////////////////////////////////

public:
	void				SobTargetSelectEventHandler(RWS::CMsg &pMsg);
	void				SobAttackSelectEventHandler(RWS::CMsg &pMsg);
	void				SobTargetSelectReleaseEventHandler(RWS::CMsg &pMsg);	
    void                SobProxyEventHandler(RWS::CMsg& pMsg);
};

inline void	CNtlSobActor::SetActorFlags(RwUInt32 uiActorFlags)
{
	m_uiActFlags = uiActorFlags;
}

inline RwUInt32 CNtlSobActor::GetActorFlags(void) const
{
	return m_uiActFlags;
}

inline void CNtlSobActor::SetServerSyncAvatarType(RwUInt8 byAvatarType)
{
	m_byServerSyncAvatarType = byAvatarType;
}

inline RwUInt8 CNtlSobActor::GetServerSyncAvatarType(void) const
{
	return m_byServerSyncAvatarType;
}

inline SERIAL_HANDLE CNtlSobActor::GetFinialHurtSerialId(void) const
{
	return m_hFinialHurtSerialId;
}

inline CNtlFSMLayer* CNtlSobActor::GetFSMLayer(void) const
{
	return m_pFSMLayer;
}

inline CNtlSobInfluence* CNtlSobActor::GetSobInfluence(void) const
{
	return m_pSobInfluence;
}

#endif
