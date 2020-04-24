/*****************************************************************************
 *
 * File			: NtlSobInfluence.h
 * Author		: HyungSuk, Jang
 * Copyright	: (¡÷)NTL
 * Date			: 2006. 3. 14	
 * Abstract		: Simulation Influence object(buff, status, skill)
 *****************************************************************************
 * Desc         : buff, sync status, skill µÓ¿« container ∞¥√º.
 *
 *****************************************************************************/

#ifndef __NTL_SOB_INFLUENCE_H__
#define __NTL_SOB_INFLUENCE_H__

#include "NtlSob.h"

class CNtlSobSyncStatus;
class CNtlSobBuff;

class CNtlSobInfluence : public CNtlSob
{
	DECLEAR_MEMORY_POOL(CNtlSobInfluence, NTL_DEFAULT_MEMORY_POOL)

	typedef std::list<CNtlSobSyncStatus*> ListSyncStatus;

public:
	
	ListSyncStatus m_listSyncStatus;

private:

	void AddSyncStatus(CNtlSobSyncStatus *pSobSyncStatus);
	void RemoveSyncStatus(CNtlSobSyncStatus *pSobSyncStatus);

public:

	CNtlSobInfluence();
	~CNtlSobInfluence();
	
	virtual void Destroy(void);
	virtual void Update(RwReal fElapsed);

	virtual void AddSob(CNtlSob *pSobObj);
	virtual void RemoveSob(CNtlSob *pSobObj);
};

#endif
