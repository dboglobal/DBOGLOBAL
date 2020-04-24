#include "stdafx.h"
#include "ObjectMsg.h"



CObjectMsg::CObjectMsg(int msgID)
{
	m_msgID = msgID;
	m_refCount = 1;
}

CObjectMsg::~CObjectMsg()
{
}

int CObjectMsg::Release(CObjectMsg* pObjMsg)
{
	if (--pObjMsg->m_refCount > 0)
	{
		return pObjMsg->m_refCount;
	}

	SAFE_DELETE(pObjMsg);
	return 0;
}




CObjectMsgQueue::CObjectMsgQueue()
{
}

CObjectMsgQueue::~CObjectMsgQueue()
{
}


void CObjectMsgQueue::Clear()
{
	for (CObjectMsgLinker* j = (CObjectMsgLinker*)m_queue.GetFirst(); j; j = (CObjectMsgLinker*)j->GetNext())
	{
		CObjectMsg *pObjectMsg = j->m_pObjectMsg;
		if (pObjectMsg)
			pObjectMsg->Release(pObjectMsg);

		SAFE_DELETE(j);
	}
}

void CObjectMsgQueue::Push(CObjectMsg *pObjectMsg, DWORD dwDelayTime)
{
	pObjectMsg->m_dwTime = dwDelayTime;

	bool bInsertAfter = true;
	CObjectMsgLinker* pExistLinker = NULL;

	for (pExistLinker = (CObjectMsgLinker*)m_queue.GetFirst(); pExistLinker; pExistLinker = (CObjectMsgLinker*)pExistLinker->GetNext())
	{
		CObjectMsg* pExistMsg = pExistLinker->m_pObjectMsg;

		if (pExistMsg->m_dwTime >= pObjectMsg->m_dwTime)
		{
			if (pExistMsg->m_dwTime != pObjectMsg->m_dwTime)
			{
				pExistMsg->m_dwTime -= pObjectMsg->m_dwTime;
				bInsertAfter = false;
				break;
			}

			pObjectMsg->m_dwTime = 0;
		}
		else
		{
			pObjectMsg->m_dwTime -= pObjectMsg->m_dwTime;
		}
	}

	CObjectMsgLinker* pNewObjectLinker = new CObjectMsgLinker;
	pNewObjectLinker->m_pObjectMsg = pObjectMsg;

	if (bInsertAfter)
		m_queue.InsertAfter(pExistLinker, pNewObjectLinker);
	else
		m_queue.InsertBefore(pExistLinker, pNewObjectLinker);
}

CObjectMsg* CObjectMsgQueue::Pop(DWORD dwProcessTime)
{
	CObjectMsgLinker* pExistLinker = (CObjectMsgLinker*)m_queue.GetFirst();
	if (pExistLinker == NULL)
		return NULL;

	CObjectMsg* pObjectMsg = pExistLinker->m_pObjectMsg;
	if (pObjectMsg == NULL)
		return NULL;

	if (pObjectMsg->m_dwTime < dwProcessTime)
	{
		m_queue.PopFront();
		SAFE_DELETE(pExistLinker);

		return pObjectMsg;
	}

	return NULL;
}

void CObjectMsgQueue::UpdateTick(DWORD dwTickDiff)
{
	CObjectMsgLinker* pExistLinker = (CObjectMsgLinker*)m_queue.GetFirst();
	if (pExistLinker == NULL)
		return;

	CObjectMsg* pObjectMsg = pExistLinker->m_pObjectMsg;
	if (pObjectMsg == NULL)
		return;

	pObjectMsg->m_dwTime = UnsignedSafeDecrease<DWORD>(pObjectMsg->m_dwTime, dwTickDiff);
}
