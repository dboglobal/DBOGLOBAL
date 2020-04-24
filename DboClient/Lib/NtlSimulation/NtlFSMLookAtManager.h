/*****************************************************************************
*
* File			: NtlFSMLookAtManager.h
* Author		: HyungSuk, Jang
* Copyright	: (аж)NTL
* Date			: 2006. 2. 9	
* Abstract		: fsm look at manager class
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __NTL_FSM_LOOKATMANAGER_H__
#define __NTL_FSM_LOOKATMANAGER_H__

class CNtlFSMCharAct;
class CNtlFSMVehicleAct;

class CNtlFSMLookAtManager
{
private:

	static CNtlFSMLookAtManager *m_pInstance;

	CNtlFSMCharAct				*m_pFSMCharAct;
	CNtlFSMVehicleAct			*m_pFSMVehicleAct;

public:

	CNtlFSMLookAtManager();
	~CNtlFSMLookAtManager();

	static CNtlFSMLookAtManager* GetInstance(void);

	RwBool Create(void);
	void Destroy(void);

	CNtlFSMCharAct*		GetFSMCharAct(void) const;
	CNtlFSMVehicleAct*	GetFSMVehicleAct(void) const;
};

static CNtlFSMLookAtManager* GetNtlFSMLookAtManager(void)
{
	return CNtlFSMLookAtManager::GetInstance();
}

inline CNtlFSMCharAct* CNtlFSMLookAtManager::GetFSMCharAct(void) const 
{
	return m_pFSMCharAct;
}

inline CNtlFSMVehicleAct* CNtlFSMLookAtManager::GetFSMVehicleAct(void) const
{
	return m_pFSMVehicleAct;
}


#endif