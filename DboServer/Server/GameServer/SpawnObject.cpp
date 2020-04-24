#include "stdafx.h"
#include "CPlayer.h"
#include "NtlPacketGU.h"
#include "NtlStringW.h"


CSpawnObject::CSpawnObject(eOBJTYPE eObjType)
	:CGameObject(eObjType)
{
	Init();

	m_vCurLoc = CNtlVector::ZERO;
	m_vCurDir = CNtlVector::ZERO;

	m_worldLinker.SetOwner(this);
	m_worldCellLinker.SetOwner(this);
	m_worldZoneLinker.SetOwner(this);
}

CSpawnObject::~CSpawnObject()
{
	if (!m_bIsDeleted)
		this->Destroy();
}


//--------------------------------------------------------------------------------------//
//		SPAWN MY OBJECT
//--------------------------------------------------------------------------------------//
void CSpawnObject::EncodeInsertPacket(CPlayer* pPlayer)
{
	//if (pPlayer->GetVisibleCount() > NTL_MAX_NUMBER_OF_PLAYERS_IN_VISIBLE_AREA)
	//	return;

	if (pPlayer->IsVisible(GetID()) == false)
	{
		pPlayer->AddVisibleObject(GetID());

		CNtlPacket packet(sizeof(sGU_OBJECT_CREATE));
		sGU_OBJECT_CREATE * res = (sGU_OBJECT_CREATE *)packet.GetPacketData();
		res->wOpCode = GU_OBJECT_CREATE;
		res->handle = GetID();
		res->sObjectInfo.objType = GetObjType();
		CopyToObjectInfo(&res->sObjectInfo, pPlayer->GetCharID());
		packet.SetPacketLen(sizeof(sGU_OBJECT_CREATE));
		pPlayer->SendPacket(&packet);
	}
}

//--------------------------------------------------------------------------------------//
//		REMOVE MY OBJECT
//--------------------------------------------------------------------------------------//
void CSpawnObject::EncodeRemovePacket(CPlayer* pPlayer)
{
	if (!pPlayer || !pPlayer->IsInitialized())
		return;

	if (pPlayer->IsVisible(GetID()))
	{
		pPlayer->RemoveVisibleObject(GetID());

		CNtlPacket packet(sizeof(sGU_OBJECT_DESTROY));
		sGU_OBJECT_DESTROY * sPacket = (sGU_OBJECT_DESTROY *)packet.GetPacketData();
		sPacket->handle = GetID();
		sPacket->wOpCode = GU_OBJECT_DESTROY;
		packet.SetPacketLen(sizeof(sGU_OBJECT_DESTROY));
		pPlayer->SendPacket(&packet);
	}
}


bool CSpawnObject::Create()
{
	return CGameObject::Create();
}

void CSpawnObject::Destroy()
{
}


void CSpawnObject::Init()
{
	m_worldID = INVALID_WORLDID;
	m_bIsLocValidityChecked = false;
	mapNameTblidx = INVALID_TBLIDX;

	m_pCurWorld = nullptr;
	m_pCurWorldCell = nullptr;
	m_pCurWorldZone = nullptr;
}


bool CSpawnObject::IsLocInWorldBoundary(CNtlVector& rLoc, CWorld* pWorld)
{
	if (pWorld)
	{
		if(pWorld->IsInBoundary(rLoc))
			return true;

		if (!m_bIsLocValidityChecked)
		{
			ERR_LOG(LOG_USER, "rLoc is invalid location., ObjType[%u], GetID() = %u, rLoc = (%f, %f, %f), pWorld->GetID() = %u", GetObjType(), GetID(), rLoc.x, rLoc.y, rLoc.z, pWorld->GetID());
			m_bIsLocValidityChecked = true;
		}

		return false;
	}

	return true;
}


void CSpawnObject::OnEnterWorld(CWorld* pWorld)
{
	m_pCurWorld = pWorld;
}

void CSpawnObject::OnLeaveWorld(CWorld* pWorld)
{
	m_pCurWorld = NULL;
}

void CSpawnObject::OnEnterWorldCell(CWorldCell* pWorldCell)
{
	if (pWorldCell)
	{
		m_pCurWorldCell = pWorldCell;
		m_pCurWorldCell->AddObject(this);
		if (IsPC())
			pWorldCell->UpdateNearbyPlayerCount_Enter();
	}
}

void CSpawnObject::OnLeaveWorldCell(CWorldCell* pWorldCell)
{
	if (m_pCurWorldCell)
	{
		if (IsPC())
		{
			if (m_pCurWorldCell == pWorldCell)
				pWorldCell->UpdateNearbyPlayerCount_Leave();
			else
			{
				ERR_LOG(LOG_USER, "m_pCurWorldCell != pWorldCell");
			}
		}

		m_pCurWorldCell->RemoveObject(this);
		m_pCurWorldCell = NULL;
	}
}

void CSpawnObject::OnEnterWorldZone(CWorldZone* pWorldZone)
{
	if (pWorldZone)
	{
		m_pCurWorldZone = pWorldZone;
		pWorldZone->AddObject(this);
	}
	else
	{
		ERR_LOG(LOG_SYSTEM, "pWorldZone == NULL");
	}
}

void CSpawnObject::OnLeaveWorldZone(CWorldZone* pWorldZone)
{
	if (pWorldZone)
	{
		pWorldZone->RemoveObject(this);
		m_pCurWorldZone = NULL;
	}
}

void CSpawnObject::OnEnterWorldComplete()
{
}


bool CSpawnObject::SetCurLoc(sVECTOR3& loc, CWorld *pDestWorld/*= NULL*/)
{
	CNtlVector rVec(loc);
	if (IsLocInWorldBoundary(rVec, pDestWorld))
		m_vCurLoc = loc;
	else
		return false;

	return true;
}

bool CSpawnObject::SetCurLoc(CNtlVector& loc, CWorld *pDestWorld/*= NULL*/)
{
	if (IsLocInWorldBoundary(loc, pDestWorld))
		m_vCurLoc = loc;
	else
		return false;

	//if (IsPC())
	//{
	//	printf("%f %f %f | %f %f %f \n", m_vCurLoc.x, m_vCurLoc.y, m_vCurLoc.z, loc.x, loc.y, loc.z);
	//}

	return true;
}

void CSpawnObject::Broadcast(CNtlPacket* pPacket, CSpawnObject* except)
{
	if (m_pCurWorldCell)
		m_pCurWorldCell->Broadcast(pPacket, except, false);
	else
	{
		if (IsPC())
			SendPacket(pPacket);
	}
}

void CSpawnObject::BroadcastToNeighbor(CNtlPacket* pPacket)
{
	if (m_pCurWorldCell)
		m_pCurWorldCell->Broadcast(pPacket, this, false);
}


//WORLDID CSpawnObject::GetWorldID()
//{
//	if (m_pCurWorld)
//		return m_pCurWorld->GetID();
//
//	return INVALID_WORLDID;
//}

TBLIDX CSpawnObject::GetWorldTblidx()
{
	if (m_pCurWorld)
		return m_pCurWorld->GetIdx();

	return INVALID_TBLIDX;
}


bool CSpawnObject::IsInRange(CNtlVector& rLoc, float fRange)
{
	CNtlVector result(rLoc - GetCurLoc());
	result.y = 0.0f;

	//if (IsPC())
	//	printf("result.SquaredLength() %f range %f\n", result.SquaredLength(), fRange * fRange);

	return result.SquaredLength() <= (fRange*fRange);
}

bool CSpawnObject::IsInRange(CSpawnObject* pTarget, float fRange)
{
	if (m_worldID == pTarget->GetWorldID())
	{
		return IsInRange(pTarget->GetCurLoc(), fRange);
	}
	return false;
}


bool CSpawnObject::IsInRange3(CNtlVector& rLoc, float fRange)
{
	CNtlVector result(rLoc - GetCurLoc());

	return result.SquaredLength() <= (fRange*fRange);
}

bool CSpawnObject::IsInRange3(CSpawnObject* pTarget, float fRange)
{
	if (m_worldID == pTarget->GetWorldID())
	{
		return IsInRange3(pTarget->GetCurLoc(), fRange);
	}
	return false;
}


bool CSpawnObject::IsInRangeWithTolerance(CNtlVector& rLoc, float fRange, float fTolerance)
{
	return IsInRange(rLoc, fRange + fTolerance);
}

bool CSpawnObject::IsOutRange(CNtlVector& rLoc, float fRange)
{
	CNtlVector result(rLoc);

	result.operator-=(GetCurLoc());
	result.y = 0;

	return fRange <= result.Length();
}

bool CSpawnObject::IsOutRangeWithTolerance(CNtlVector& rLoc, float fRange, float fTolerance)
{
	if (fRange <= 10.0f)
		return true;

	return IsOutRange(rLoc, fRange - 10.0f);
}


float CSpawnObject::GetDistance(CSpawnObject* pTarget)
{
	return GetDistance(pTarget->GetCurLoc()) - (GetObjectRadius() + pTarget->GetObjectRadius());
}


float CSpawnObject::GetDistance(CNtlVector& rLoc)
{
	CNtlVector vTemp(rLoc - GetCurLoc());
	vTemp.y = 0.0f;
	return vTemp.Normalize();
}

float CSpawnObject::GetDistance3(CNtlVector & rLoc)
{
	CNtlVector vTemp(rLoc - GetCurLoc());
	return vTemp.Normalize();
}


bool CSpawnObject::IsFrontSide(CSpawnObject* pTarget)
{
	CNtlVector vTemp, vDir;
	float fValue;

	vDir = GetCurDir() * GetObjectRadius();

	vTemp = GetCurLoc() + vDir;

	vTemp = pTarget->GetCurLoc() - vTemp;
	vTemp.y = 0.0f;

	vDir = GetCurDir();
	vDir.y = 0.0f;

	fValue = vDir.Dot(vTemp);

	return fValue >= 0.0f;
}


bool CSpawnObject::IsInDegreeAngle(CSpawnObject* pTarget, int nDegree)
{
	int nNewDegree = 0;
	CNtlVector vCurDir, vStandard, vTemp;

	if (nDegree < 0)
		nNewDegree = 360 - -nDegree % 360;
	else
		nNewDegree = nDegree % 360;

	vCurDir = GetCurDir();
	vCurDir.y = 0.0f;
	vCurDir.SafeNormalize();

	vCurDir *= GetObjectRadius();
	
	vTemp = GetCurLoc() + vCurDir;

	RotateVector(vStandard, &vCurDir, ConvertDegreeToRadian(nNewDegree / 2));

	vTemp = pTarget->GetCurLoc() - vTemp;
	vTemp.y = 0.0f;
	vTemp.SafeNormalize();

	return vCurDir.Dot(vTemp) >= vCurDir.Dot(vStandard);
}


bool CSpawnObject::IsInRangeFront(CSpawnObject* pTarget, float fRange)
{
	if (IsInRange(pTarget, fRange))
		return IsFrontSide(pTarget);
	
	return false;
}


bool CSpawnObject::IsInRangeBack(CSpawnObject* pTarget, float fRange)
{
	if (IsInRange(pTarget, fRange))
	{
		return IsFrontSide(pTarget) == false;
	}

	return false;
}


void CSpawnObject::GetPcInRange(float fRange, std::vector<CPlayer*>& rVec)
{
	CWorldCell* pWorldCell = GetCurWorldCell();
	if (pWorldCell)
	{
		int nLoopCount = 0;

		CWorldCell::QUADPAGE page = pWorldCell->GetCellQuadPage(GetCurLoc());
		for (int dir = CWorldCell::QUADDIR_SELF; dir <= CWorldCell::QUADDIR_VERTICAL; dir++)
		{
			CWorldCell* pWorldCellSibling = pWorldCell->GetQuadSibling(page, (CWorldCell::QUADDIR)dir);
			if (pWorldCellSibling)
			{
				CPlayer* pTarget = (CPlayer*)pWorldCellSibling->GetObjectList()->GetFirst(OBJTYPE_PC);
				while (pTarget && pTarget->IsInitialized())
				{
					++nLoopCount;
					if (nLoopCount > 5000)
					{
						ERR_LOG(LOG_GENERAL, "INFINITE LOOP FOUND");
					}

					if (IsInRange(pTarget, fRange))
					{
						rVec.push_back(pTarget);
					}
					pTarget = (CPlayer*)pWorldCellSibling->GetObjectList()->GetNext(pTarget->GetWorldCellObjectLinker());
				}
			}
		}
	}
}