/*****************************************************************************
 *
 * File			: NtlFSMEventQueue.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2006. 2. 14	
 * Abstract		: FSM event queue class
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_FSM_EVENTQUEUE_H__
#define __NTL_FSM_EVENTQUEUE_H__

#include "ceventhandler.h"
#include "NtlSLDef.h"

#define NLT_FSM_EVENT_DATA_LEN		1024

class CNtlFSMEventData
{
	DECLEAR_MEMORY_POOL(CNtlFSMEventData, NTL_DEFAULT_MEMORY_POOL)
public:

	RWS::CMsg msg;
	RwInt32 iDataLen;
	RwChar chData[NLT_FSM_EVENT_DATA_LEN];
};

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlFSMEventQueue
{
	DECLEAR_MEMORY_POOL(CNtlFSMEventQueue, NTL_DEFAULT_MEMORY_POOL)

private:

	std::list<CNtlFSMEventData*> m_listEvent;

public:

	CNtlFSMEventQueue();
	~CNtlFSMEventQueue();

	void Push(RWS::CMsg &pMsg, void *pData, RwInt32 iLen);
	CNtlFSMEventData* Pop(void);

	void RemoveAll(void);

	RwBool IsEventExist(void);
};

#endif
