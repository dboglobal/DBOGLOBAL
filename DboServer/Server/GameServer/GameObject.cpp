#include "stdafx.h"
#include "GameObject.h"
#include "GameServer.h"


CGameObject::CGameObject(eOBJTYPE eObjType)
{
	m_objType = eObjType;
	m_handle = INVALID_HOBJECT;
	m_bIsDeleted = false;
	m_bIsInitialized = false;

	m_ClientSession = NULL;
	hSessionID = INVALID_HSESSION;

	Init();
}

CGameObject::~CGameObject()
{
	if (!m_bIsDeleted)
		this->Destroy();

	m_bIsDeleted = true;
}



bool CGameObject::Create()
{
	m_pObjectController = CreateObjectController();
	return true;
}


void CGameObject::Destroy()
{
	SAFE_DELETE(m_pObjectController);
}


void CGameObject::Init()
{
	m_pObjectController = NULL;
}


void CGameObject::TickProcess(DWORD dwTickDiff, float fMultiple)
{
	m_objectMsgQueue.UpdateTick(dwTickDiff);

	DWORD dwProcessTime = dwTickDiff;

	int nInfiniteLoop = 0;

	for (CObjectMsg* pObjectMsg = m_objectMsgQueue.Pop(dwTickDiff);
		pObjectMsg;
		pObjectMsg = m_objectMsgQueue.Pop(dwProcessTime))
	{
		++nInfiniteLoop;
		if (nInfiniteLoop > 10000)
		{
			ERR_LOG(LOG_SYSTEM, "INFINITE LOOP DETECTED");
		}

		OnObjectMsg(pObjectMsg);

		dwProcessTime = UnsignedSafeDecrease<DWORD>(dwProcessTime, pObjectMsg->GetTime());

		pObjectMsg->Release(pObjectMsg);
	}
}


int CGameObject::OnObjectMsg(CObjectMsg* pObjectMsg)
{
	if (m_pObjectController)
		return m_pObjectController->OnObjectMsg(pObjectMsg);

	return FALSE;
}


eOBJTYPE CGameObject::GetObjType()
{
	return m_objType;
}


bool CGameObject::IsObjType(BYTE type)
{
	return (m_objType == type ? true : false);
}


void CGameObject::SendPacket(CNtlPacket* pPacket)
{
	if (GetClientSession() && GetClientSessionID() != INVALID_HSESSION)
		g_pApp->Send(GetClientSessionID(), pPacket);
}


int CGameObject::SendObjectMsg(CObjectMsg* pObjectMsg)
{
	return OnObjectMsg(pObjectMsg);
}


void CGameObject::PostObjectMsg(CObjectMsg* pObjectMsg, DWORD dwDelayTime)
{
	m_objectMsgQueue.Push(pObjectMsg, dwDelayTime);
}


