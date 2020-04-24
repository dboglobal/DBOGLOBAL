#include "precomp_ntlsimulation.h"
#include "NtlFSMEventQueue.h"
#include "NtlDebug.h"

DEFINITION_MEMORY_POOL(CNtlFSMEventData)

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

DEFINITION_MEMORY_POOL(CNtlFSMEventQueue)

CNtlFSMEventQueue::CNtlFSMEventQueue()
{

}

CNtlFSMEventQueue::~CNtlFSMEventQueue()
{
	RemoveAll();
}

void CNtlFSMEventQueue::Push(RWS::CMsg &pMsg, void *pData, RwInt32 iLen)
{
	NTL_FUNCTION("CNtlFSMEventQueue::Push");

	NTL_ASSERTE(iLen < NLT_FSM_EVENT_DATA_LEN);

	CNtlFSMEventData *pEventData = NTL_NEW CNtlFSMEventData;
	pEventData->msg.Id = pMsg.Id;
	pEventData->msg.pData = pEventData->chData;
	if(iLen > 0 && pData != NULL)
		memcpy(pEventData->chData, pData, iLen);

	pEventData->iDataLen = iLen;

	m_listEvent.push_back(pEventData); 

	NTL_RETURNVOID();
}

CNtlFSMEventData* CNtlFSMEventQueue::Pop(void)
{
	if(m_listEvent.size() == 0)
		return NULL;

	std::list<CNtlFSMEventData*>::iterator it = m_listEvent.begin();
	CNtlFSMEventData *pEventData = (*it);
	m_listEvent.erase(it);
	
	return pEventData;
}

void CNtlFSMEventQueue::RemoveAll(void)
{
	NTL_FUNCTION("CNtlFSMEventQueue::RemoveAll");

	std::list<CNtlFSMEventData*>::iterator it;
	for(it = m_listEvent.begin(); it != m_listEvent.end(); ++it)
	{
		CNtlFSMEventData *pData = (*it);

		NTL_DELETE( pData );
	}

	m_listEvent.clear();

	NTL_RETURNVOID();
}

RwBool CNtlFSMEventQueue::IsEventExist(void)
{
	if(m_listEvent.empty())
		return FALSE;

	return TRUE;
}