#include "precomp_ntlcore.h"
#include "NtlEventQueue.h"
#include "NtlDebug.h"

RwFreeList* CNtlEventQueueSlot::m_pFreeList = 0;


CNtlEventQueueSlot::CNtlEventQueueSlot(RWS::CMsg &Msg, void *pData, RwInt32 iDataLen, RwReal fDelayTime)
{
	NTL_PRE(pData);
	NTL_ASSERT( iDataLen < MAX_EVENT_DATA_LEN, "iDataLen >= Event Queue MAX_EVENT_DATA_LEN");

	m_fCurrTime = 0.0f;
	m_fDelayTime = fDelayTime;
	m_Msg = Msg;
	m_iDataLen = iDataLen;
	memcpy(m_chData, pData, iDataLen);
	m_Msg.pData = (void*)m_chData;  
}

RwBool CNtlEventQueueSlot::AllocFreeList(void)
{
	NTL_FUNCTION("CNtlEventQueueSlot::AllocFreeList");

	NTL_PRE(m_pFreeList == 0);

	RwUInt32 iBlockSize = 50;
    
    m_pFreeList = RwFreeListCreate(sizeof(CNtlEventQueueSlot), iBlockSize, 16, rwMEMHINTDUR_GLOBAL);

    RWS_ASSERT(m_pFreeList, "failed to allocate free list");

    NTL_RETURN(TRUE);
}

void CNtlEventQueueSlot::FreeFreeList(void)
{
	NTL_FUNCTION("CNtlEventQueueSlot::FreeFreeList");

    if (m_pFreeList)
    {
        RwFreeListDestroy(m_pFreeList);
        m_pFreeList = 0;
    }

    NTL_RETURNVOID();
}

void* CNtlEventQueueSlot::operator new(size_t size)
{
	NTL_FUNCTION("CNtlEventQueueSlot::operator new");
    
    NTL_ASSERT(m_pFreeList, "System not initialized, need to call 'AllocFreeList'");

    NTL_RETURN(RwFreeListAlloc(m_pFreeList, rwMEMHINTDUR_GLOBAL));
}

void CNtlEventQueueSlot::operator delete(void *pObj)
{
	NTL_ASSERT(m_pFreeList, "System not initialized, need to call 'AllocFreeList'");

	if (pObj)
    {
        RwFreeListFree(m_pFreeList, pObj);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlEventQueue::CNtlEventQueue()
{
	m_iFilterNum = 0;
}

CNtlEventQueue::~CNtlEventQueue()
{

}

CNtlEventQueue* CNtlEventQueue::GetInstance(void)
{
	static CNtlEventQueue EventQueue;
	return &EventQueue;
}

RwBool CNtlEventQueue::Create(void)
{
    NTL_FUNCTION("CNtlEventQueue::Create");

	if(!CNtlEventQueueSlot::AllocFreeList())
	{
		NTL_RETURN(FALSE);
	}

    NTL_RETURN(TRUE);
}

void CNtlEventQueue::Destroy(void)
{
	NTL_FUNCTION("CNtlEventQueue::Destroy");

	CNtlEventQueueSlot::FreeFreeList(); 

    NTL_RETURNVOID();
}

RwBool CNtlEventQueue::Push(RWS::CMsg &Msg, void *pData, int iDataLen, RwReal fDelayTime)
{
	NTL_FUNCTION("CNtlEventQueue::Push" );

	CNtlEventQueueSlot *pEventSlot = NTL_NEW CNtlEventQueueSlot(Msg, pData, iDataLen, fDelayTime);

	m_listBuffer.push_back(pEventSlot);

	NTL_RETURN(TRUE );
}

void CNtlEventQueue::Process(RwReal fElapsed)
{
	NTL_FUNCTION("CNtlEventQueue::Process" );

	std::list<CNtlEventQueueSlot*>::iterator it;
	for(it = m_listBuffer.begin(); it != m_listBuffer.end(); ) 
	{
		CNtlEventQueueSlot *pEventSlot = (*it);
		pEventSlot->m_fCurrTime += fElapsed;

        if(pEventSlot->m_fCurrTime >= pEventSlot->m_fDelayTime)
		{
			if( !IsFilterMsg(pEventSlot->m_Msg) )
			{
				SendMsg(pEventSlot->m_Msg); 
			}

			it = m_listBuffer.erase(it); 

			NTL_DELETE( pEventSlot );
		}
		else
		{
			++it;
		}
	}

	m_iFilterNum = 0;

	NTL_RETURNVOID();
}


void CNtlEventQueue::CurrentProcessFilterMsg(RWS::CEventId& id)
{
	NTL_ASSERT(m_iFilterNum < 10, "CNtlEventQueue::CurrentProcessFilterMsg");

	m_msgFileter[m_iFilterNum].Id = id;
	m_iFilterNum++;
}

RwBool CNtlEventQueue::IsFilterMsg(RWS::CMsg& msg)
{
	if(m_iFilterNum == 0)
		return FALSE;

	for(RwUInt32 i = 0; i < m_iFilterNum; i++)
	{
		if(m_msgFileter[i].Id == msg.Id)
			return TRUE;
	}

	return FALSE;
}


void PostMsg(RWS::CMsg &Msg, void *pData, RwInt32 iDataLen, RwReal fDelayTime /*= 0.0f*/)
{
	CNtlEventQueue::GetInstance()->Push(Msg, pData, iDataLen, fDelayTime); 
}

void EventQueueCurrentProcessFilterMsg(RWS::CEventId& id)
{
	CNtlEventQueue::GetInstance()->CurrentProcessFilterMsg(id); 
}