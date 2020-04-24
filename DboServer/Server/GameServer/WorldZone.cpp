#include "stdafx.h"
#include "SpawnObject.h"
#include "NtlPacketGU.h"


CWorldZone::CWorldZone()
{
	Init();
}

CWorldZone::~CWorldZone()
{
	Destroy();
}


bool CWorldZone::Create(sWORLD_ZONE_TBLDAT* pWorldZoneData, CWorld* pWorldRef)
{
	if (pWorldRef)
	{
		m_pWorldZoneData = pWorldZoneData;
		m_pWorldRef = pWorldRef;
		return true;
	}

	ERR_LOG(LOG_SYSTEM, "pWorldRef == NULL");

	return false;
}

void CWorldZone::Destroy()
{
	this->m_pWorldZoneData = NULL;
	this->m_pWorldRef = NULL;
}


void CWorldZone::TickProcess(DWORD dwTickDiff, float fMultiple)
{
}

int CWorldZone::CopyToInfo(sWORLD_ZONE_INFO & rZoneInfo)
{
	rZoneInfo.zoneId = GetZoneId();
	rZoneInfo.bIsDark = m_bIsDark;

	return sizeof(sWORLD_ZONE_INFO);
}


int CWorldZone::EnterObject(CSpawnObject* pSpawnObject)
{
	if (!pSpawnObject)
	{
		ERR_LOG(LOG_GENERAL, "Spawn object is null");
		return 1;
	}

	pSpawnObject->OnEnterWorldZone(this);

	return NTL_SUCCESS;
}

int CWorldZone::LeaveObject(CSpawnObject* pSpawnObject)
{
	if (!pSpawnObject)
	{
		ERR_LOG(LOG_GENERAL, "Spawn object is null");
		return 1;
	}

	pSpawnObject->OnLeaveWorldZone(this);

	return NTL_SUCCESS;
}


void CWorldZone::AddObject(CSpawnObject* pSpawnObject)
{
	if (pSpawnObject->GetWorldZoneObjectLinker()->GetPrev() || pSpawnObject->GetWorldZoneObjectLinker()->GetNext())
	{
		ERR_LOG(LOG_SYSTEM, "pSpawnObject->GetWorldZoneObjectLinker()->GetPrev() == NULL, pSpawnObject->GetWorldZoneObjectLinker()->GetNext() == NULL");
	}
	else
	{
		m_objectList.Add(pSpawnObject->GetWorldZoneObjectLinker());
	}
}

void CWorldZone::RemoveObject(CSpawnObject* pSpawnObject)
{
	m_objectList.Remove(pSpawnObject->GetWorldZoneObjectLinker());
}


void CWorldZone::Broadcast(CNtlPacket* pPacket, CSpawnObject* pExcept/* = NULL*/)
{
	int n = 0;

	for (CSpawnObject* pExistObject = m_objectList.GetFirst(OBJTYPE_PC); 
		pExistObject; 
		pExistObject = m_objectList.GetNext( pExistObject->GetWorldZoneObjectLinker() ) )
	{
		n++;
		if (pExistObject && pExistObject != pExcept)
		{
			pExistObject->SendPacket(pPacket);
		}

		if (n >= MAX_PC)
		{
			ERR_LOG(LOG_GENERAL, "ERROR !!! ENDLESS LOOP.");
			break;
		}
	}
}

bool CWorldZone::HasFunc(WORD wFuncFlag)
{
	return BIT_FLAG_TEST(wFuncFlag, m_pWorldZoneData->wFunctionBitFlag);
}


void CWorldZone::UpdateZoneInfo(bool bIsDark)
{
	m_bIsDark = bIsDark;

	CNtlPacket packet(sizeof(sGU_AVATAR_ZONE_INFO));
	sGU_AVATAR_ZONE_INFO * zone = (sGU_AVATAR_ZONE_INFO *)packet.GetPacketData();
	zone->wOpCode = GU_AVATAR_ZONE_INFO;
	zone->zoneInfo.bIsDark = bIsDark;
	zone->zoneInfo.zoneId = GetZoneId();
	packet.SetPacketLen(sizeof(sGU_AVATAR_ZONE_INFO));
	Broadcast(&packet);
}


void CWorldZone::Init()
{
	this->m_pWorldZoneData = NULL;
	this->m_pWorldRef = NULL;
	m_bIsDark = false;
}

