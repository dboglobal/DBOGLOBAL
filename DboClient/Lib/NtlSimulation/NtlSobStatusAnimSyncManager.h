/*****************************************************************************
*
* File			: NtlSobStatusSyncManager.h
* Author		: HyungSuk, Jang
* Copyright	: (주)NTL
* Date			: 2008. 11. 08	
* Abstract		: Simulation object LP/EP Animation Hit Event 동기 관리자.
*****************************************************************************
* Desc         : 
*
*****************************************************************************/


#ifndef __NTL_SOBSTATUS_ANIMSYNC_MANAGER_H__
#define __NTL_SOBSTATUS_ANIMSYNC_MANAGER_H__

#include "NtlSLDef.h"

class CNtlSobStatusAnimSyncUnit
{
	DECLEAR_MEMORY_POOL(CNtlSobStatusAnimSyncUnit, NTL_DEFAULT_MEMORY_POOL)

private:

	SERIAL_HANDLE	m_hUnit;
	RwInt32			m_iCurValue;
	RwInt32			m_iMaxValue;

public:

	CNtlSobStatusAnimSyncUnit();

	void			SetUIntHandle(SERIAL_HANDLE hUnit);
	SERIAL_HANDLE	GetUIntHandle(void);
	void			SetValue(RwInt32 iCurrValue, RwInt32 iMaxValue);
	void			GetValue(RwInt32& iCurrvalue, RwInt32& iMaxValue);
};



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////


class CNtlSobStatusAnimSync
{
	DECLEAR_MEMORY_POOL(CNtlSobStatusAnimSync, NTL_DEFAULT_MEMORY_POOL)

private:

	typedef std::list<CNtlSobStatusAnimSyncUnit*> ListAnimSyncUnit;

	SERIAL_HANDLE				m_hSerialId;
	SYNCEVENT_HANDLE			m_hSyncStatus;
	ListAnimSyncUnit			m_listAnimSyncUnit;
	CNtlSobStatusAnimSyncUnit	*m_pServerSyncUnit;

	RwReal						m_fExceptTime;
	RwReal						m_fLifeTime;
	RwBool						m_bFinish;

public:

	CNtlSobStatusAnimSync();
	~CNtlSobStatusAnimSync();

	void						Update(RwReal fElapsed);

	void						SetSerialId(SERIAL_HANDLE hSerialId);
	void						SetServerSyncUnit(RwInt32 iCurrValue, RwInt32 iMaxValue);

	void						SetSyncHandle(SYNCEVENT_HANDLE hSyncStatus);
	SYNCEVENT_HANDLE			GetSyncHandle(void);

	SYNCEVENT_HANDLE			AddAnimSyncUnit(RwInt32 iDamageValue);
	void						PopAnimSyncUnit(void);
	void						PopAnimSyncUnitAll(void);


	void						SetAnimSyncUnitLifeTime(RwReal fLifeTime);

	void						ProcFinialServerSyncUnit(void);

	RwBool						IsFinish(void);
};



////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlSobStatusAnimSyncContainer
{
	DECLEAR_MEMORY_POOL(CNtlSobStatusAnimSyncContainer, NTL_DEFAULT_MEMORY_POOL)
	
private:

	typedef std::list<CNtlSobStatusAnimSync*> ListAnimSync;

	SERIAL_HANDLE	m_hSerialId;
	ListAnimSync	m_listAnimSync;
	RwBool			m_bFinish;

private:

	CNtlSobStatusAnimSync* FindAnimSync(SYNCEVENT_HANDLE hSyncStatus);

public:

	CNtlSobStatusAnimSyncContainer();
	~CNtlSobStatusAnimSyncContainer();

	void			Update(RwReal fElapsed);

	void			SetSerialId(SERIAL_HANDLE hSerialId);
	void			SetFinialServerSyncUnit(SYNCEVENT_HANDLE hSyncStatus, RwInt32 iCurrValue, RwInt32 iMaxValue);

	SERIAL_HANDLE	AddAnimSyncUnit(SYNCEVENT_HANDLE hSyncStatus, RwInt32 iValue);
	void			PopAnimSyncUnit(SYNCEVENT_HANDLE hSyncStatus);
	void			PopAnimSyncUnitAll(SYNCEVENT_HANDLE hSyncStatus);

	void			SetAnimSyncUnitLifeTime(SYNCEVENT_HANDLE hSyncStatus, RwReal fLifeTime);

	RwBool			IsFinish(void);
};


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlSobStatusAnimSyncManager
{
private:

	typedef std::map<SERIAL_HANDLE, CNtlSobStatusAnimSyncContainer*> MapAnimSync;
	typedef std::list<SERIAL_HANDLE> ListAnimSyncTarget;

	MapAnimSync			m_mapLPAnimSync;
	ListAnimSyncTarget	m_listAnimSyncTarget;	

private:

	void				RemoveLPAnimSyncContainer(SERIAL_HANDLE hSerialId);
	RwBool				IsActive(void);

public:

	CNtlSobStatusAnimSyncManager();
	~CNtlSobStatusAnimSyncManager();

	void				Update(RwReal fElasped);

	void				SetFinialServerSyncUnit(SERIAL_HANDLE hSerialId, SYNCEVENT_HANDLE hSyncStatus, RwInt32 iCurrValue, RwInt32 iMaxValue);

	SERIAL_HANDLE		AddLPAnimSyncUnit(SERIAL_HANDLE hSerialId, SYNCEVENT_HANDLE hSyncStatus, RwInt32 iValue);
	void				PopLPAnimSyncUnit(SERIAL_HANDLE hSerialId, SYNCEVENT_HANDLE hSyncStatus);
	void				PopLPAnimSyncUnitAll(SERIAL_HANDLE hSerialId, SYNCEVENT_HANDLE hSyncStatus);

	void				SetLPAnimSyncUnitLifeTime(SERIAL_HANDLE hSerialId, SYNCEVENT_HANDLE hSyncStatus, RwReal fLifeTime);

	void				AddAnimSyncTarget(SERIAL_HANDLE hSerialId);
	void				RemoveAnimSyncTarget(SERIAL_HANDLE hSerialId);

	RwBool				IsExistTarget(SERIAL_HANDLE hSerialId);
};


#endif