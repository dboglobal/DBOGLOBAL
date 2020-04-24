#include "stdafx.h"
#include "SpawnObject.h"
#include "WorldManager.h"
#include "GameServer.h"
#include "GameMain.h"
#include "NtlNavi.h"



CWorldCell::CWorldCell()
{
	Init();
}

CWorldCell::~CWorldCell()
{
	Destroy();
}


void CWorldCell::Init()
{
	m_cellID = INVALID_CELLID;
	m_vStart.Reset();
	m_vEnd.Reset();
	m_pParent = NULL;
	m_pWorldZoneRef = NULL;
	memset(m_sibling, NULL, sizeof(m_sibling));
	m_dwNearbyPlayerCount = 0;
}


int CWorldCell::Create(CWorld *pWorld, CELLID cellID, CNtlVector& rStart, CNtlVector& rEnd)
{
	m_pParent = pWorld;
	m_cellID = cellID;
	m_vStart.operator=(rStart);
	m_vEnd.operator=(rEnd);

	for (int i = SELF; i < MAX_CELLDIR; i++)
	{
		CELLID siblingId = GetSiblingID((CELLDIR)i);
		if (siblingId != INVALID_CELLID)
		{
			m_sibling[i] = m_pParent->GetWorldCell(siblingId);
		}
	}

	CNtlVector vCenter(rStart.operator+(rEnd));
	vCenter.operator/=(2.0f);

	
	CWorldZone* pWorldZone = m_pParent->FindWorldZone(GetNaviEngine()->GetZoneIndex(m_pParent->GetNaviInstanceHandle(), vCenter.x, vCenter.z));
	if (pWorldZone)
		m_pWorldZoneRef = pWorldZone;

	//CGameServer* app = (CGameServer*)g_pApp;
	//CWorldZone* pWorldZone = m_pParent->FindWorldZone(app->GetGameMain()->GetWorldManager()->GetDboNavi()->GetZoneIndex(m_pParent->GetIdx(), vCenter.x, vCenter.z));
	//if (pWorldZone)
	//	m_pWorldZoneRef = pWorldZone;

	return NTL_SUCCESS;
}


void CWorldCell::Destroy()
{
}


CELLID CWorldCell::GetCellId()
{
	return m_cellID;
}


CWorldCell* CWorldCell::GetSibling(CELLDIR dir)
{
	return m_sibling[dir];
}


CWorldCell* CWorldCell::GetQuadSibling(QUADPAGE page, QUADDIR dir)
{
	switch (page)
	{
		case QUADPAGE_FIRST:
		{
			if (dir == QUADDIR_SELF)
				return m_sibling[SELF];
			else if (dir == QUADDIR_HOTIZONTAL)
				return m_sibling[RIGHT];
			else if (dir == QUADDIR_DIAGONAL)
				return m_sibling[R_BOTTOM];
			else if (dir == QUADDIR_VERTICAL)
				return m_sibling[BOTTOM];
		}
		break;
		case QUADPAGE_SECOND:
		{
			if (dir == QUADDIR_SELF)
				return m_sibling[SELF];
			else if (dir == QUADDIR_HOTIZONTAL)
				return m_sibling[LEFT];
			else if (dir == QUADDIR_DIAGONAL)
				return m_sibling[L_BOTTOM];
			else if (dir == QUADDIR_VERTICAL)
				return m_sibling[BOTTOM];
		}
		break;
		case QUADPAGE_THIRD:
		{
			if (dir == QUADDIR_SELF)
				return m_sibling[SELF];
			else if (dir == QUADDIR_HOTIZONTAL)
				return m_sibling[RIGHT];
			else if (dir == QUADDIR_DIAGONAL)
				return m_sibling[R_TOP];
			else if (dir == QUADDIR_VERTICAL)
				return m_sibling[TOP];
		}
		break;
		case QUADPAGE_FOURTH:
		{
			if (dir == QUADDIR_SELF)
				return m_sibling[SELF];
			else if (dir == QUADDIR_HOTIZONTAL)
				return m_sibling[LEFT];
			else if (dir == QUADDIR_DIAGONAL)
				return m_sibling[L_TOP];
			else if (dir == QUADDIR_VERTICAL)
				return m_sibling[TOP];
		}
		break;

		default: break;
	}

	return NULL;
}


CELLID CWorldCell::GetSiblingID(CELLDIR dir)
{
	CELLID cellID = INVALID_CELLID;

	if (IsValidCellID(m_cellID))
	{
		switch (dir)
		{
		case SELF:
		{
			cellID = m_cellID;
		}
		break;
		case L_TOP:
		{
			cellID = m_cellID - m_pParent->GetCellHorizontalCount() - 1;
		}
		break;
		case TOP:
		{
			cellID = m_cellID - m_pParent->GetCellHorizontalCount();
		}
		break;
		case R_TOP:
		{
			cellID = m_cellID - m_pParent->GetCellHorizontalCount() + 1;
		}
		break;
		case RIGHT:
		{
			cellID = m_cellID + 1;
		}
		break;
		case R_BOTTOM:
		{
			cellID = m_cellID + m_pParent->GetCellHorizontalCount() + 1;
		}
		break;
		case BOTTOM:
		{
			cellID = m_cellID + m_pParent->GetCellHorizontalCount();
		}
		break;
		case L_BOTTOM:
		{
			cellID = m_cellID + m_pParent->GetCellHorizontalCount() - 1;
		}
		break;
		case LEFT:
		{
			cellID = m_cellID - 1;
		}
		break;

		default: break;
		}

		if (IsValidCellID(cellID))
			return cellID;

	}

	return INVALID_CELLID;
}


bool CWorldCell::IsValidCellID(CELLID cellID)
{
	if (cellID < m_pParent->GetCellTotalCount())
	{
		if (cellID / m_pParent->GetCellHorizontalCount())
		{
			if (m_pParent->GetCellVerticalCount() - 1 == cellID / m_pParent->GetCellHorizontalCount())
				return false;
			else
			{
				if (cellID % m_pParent->GetCellHorizontalCount())
				{
					return m_pParent->GetCellHorizontalCount() - 1 != cellID % m_pParent->GetCellHorizontalCount();
				}
			}
		}
	}

	return false;
}


CWorldObjectList* CWorldCell::GetObjectList()
{
	return &m_objectList;
}


CWorldZone* CWorldCell::GetWorldZone()
{
	return m_pWorldZoneRef;
}


CNtlVector& CWorldCell::GetStartLoc()
{
	return m_vStart;
}

CNtlVector& CWorldCell::GetEndLoc()
{
	return m_vEnd;
}


DWORD CWorldCell::GetNearbyPlayerCount()
{
	return m_dwNearbyPlayerCount;
}


void CWorldCell::Broadcast(std::list<CNtlPacket*>& rPacketList)
{
	int n = 0;
	for (int i = 0; i < MAX_CELLDIR; i++)
	{
		CWorldCell* pCell = m_sibling[i];
		if (pCell)
		{
			for (std::list<CNtlPacket*>::iterator it = rPacketList.begin(); it != rPacketList.end(); it++)
			{
				n++;
				CNtlPacket* pPacket = *it;
				if (pPacket)
					pCell->SendPacket(pPacket);

				if (n >= MAX_PC)
				{
					ERR_LOG(LOG_GENERAL, "ERROR !! ENDLESS LOOP.");
					return;
				}
			}
		}
	}
}


void CWorldCell::Broadcast(CNtlPacket * pPacket, CSpawnObject* pExcept/*= NULL*/, bool bQuad/* = false*/)
{
	int n = 0;
	for (int i = 0; i < MAX_CELLDIR; i++)
	{
		CWorldCell* pCell = m_sibling[i];
		if (pCell)
		{
			CWorldObjectList* pObjList = pCell->GetObjectList();
			if (pObjList)
			{
				for (CSpawnObject* pObj = pObjList->GetFirst(OBJTYPE_PC); pObj; pObj = pObjList->GetNext(pObj->GetWorldCellObjectLinker()))
				{
					n++;
					if (pObj != pExcept)
					{
						pObj->SendPacket(pPacket);
					}
					if (n >= MAX_PC)
					{
						ERR_LOG(LOG_GENERAL, "ERROR !! ENDLESS LOOP.");
						return;
					}
				}
			}
		}
	}
}


int CWorldCell::EnterObject(CSpawnObject * pSpawnObject, bool bIsServerStart/* = false*/)
{
	if (pSpawnObject)
	{
		if (pSpawnObject->GetCurWorldCell())
		{
			ERR_LOG(LOG_USER, "[Handle : %u] (NULL != pSpawnObject->GetCurWorldCell())", pSpawnObject->GetID());
		}
		else
		{
			if (MoveObject_SELF(pSpawnObject, bIsServerStart))
			{
				if (MoveZone(pSpawnObject))
					return NTL_SUCCESS;
			}
		}
	}
	else
	{
		ERR_LOG(LOG_USER, "[Handle : %u] (pSpawnObject == NULL)", pSpawnObject->GetID());
	}

	return 100001;
}

int CWorldCell::LeaveObject(CSpawnObject * pSpawnObject)
{
	if (pSpawnObject)
	{
		if (!pSpawnObject->GetCurWorldCell())
		{
			ERR_LOG(LOG_USER, "[Handle : %u] (NULL == pSpawnObject->GetCurWorldCell())", pSpawnObject->GetID());
		}
		
		if (MoveObject_NONE(pSpawnObject, false))
		{
			if (MoveZone(pSpawnObject))
				return NTL_SUCCESS;
		}
	}
	else
	{
		CNtlMiniDump::Snapshot();
	}

	return 100001;
}


void CWorldCell::AddObject(CSpawnObject * pSpawnObject)
{
	if (pSpawnObject->GetWorldCellObjectLinker()->GetPrev() || pSpawnObject->GetWorldCellObjectLinker()->GetNext())
	{
		ERR_LOG(LOG_USER, "[Handle : %u] pPrevLinker = %016x, pNextLinker = %016x", pSpawnObject->GetID(), pSpawnObject->GetWorldCellObjectLinker()->GetPrev(), pSpawnObject->GetWorldCellObjectLinker()->GetNext());

		if (pSpawnObject->GetWorldCellObjectLinker()->GetPrev() && !pSpawnObject->GetWorldCellObjectLinker()->GetNext())
		{
			ERR_LOG(LOG_USER, "[Handle : %u] pPrevLinker actually is not linked with the current linker.(NULL == pPrevLinker->GetNext())", pSpawnObject->GetID());
		}
		if (!pSpawnObject->GetWorldCellObjectLinker()->GetPrev() && pSpawnObject->GetWorldCellObjectLinker()->GetNext())
		{
			ERR_LOG(LOG_USER, "[Handle : %u] pNextLinker actually is not linked with the current linker.(NULL == pNextLinker->GetPrev())", pSpawnObject->GetID());
		}
	}

	m_objectList.Add(pSpawnObject->GetWorldCellObjectLinker());
	OnObjectAdded(pSpawnObject);
}


void CWorldCell::RemoveObject(CSpawnObject * pSpawnObject)
{
	m_objectList.Remove(pSpawnObject->GetWorldCellObjectLinker());

	if (pSpawnObject->GetWorldCellObjectLinker()->GetPrev() || pSpawnObject->GetWorldCellObjectLinker()->GetNext())
	{
		ERR_LOG(LOG_USER, "[Handle : %u] pPrevLinker = %016x, pNextLinker = %016x", pSpawnObject->GetID(), pSpawnObject->GetWorldCellObjectLinker()->GetPrev(), pSpawnObject->GetWorldCellObjectLinker()->GetNext());

		if (pSpawnObject->GetWorldCellObjectLinker()->GetPrev() && !pSpawnObject->GetWorldCellObjectLinker()->GetNext())
		{
			ERR_LOG(LOG_USER, "[Handle : %u] pPrevLinker actually is not linked with the current linker.(NULL == pPrevLinker->GetNext())", pSpawnObject->GetID());
		}
		if (!pSpawnObject->GetWorldCellObjectLinker()->GetPrev() && pSpawnObject->GetWorldCellObjectLinker()->GetNext())
		{
			ERR_LOG(LOG_USER, "[Handle : %u] pNextLinker actually is not linked with the current linker.(NULL == pNextLinker->GetPrev())", pSpawnObject->GetID());
		}
	}

	OnObjectRemoved(pSpawnObject);
}


bool CWorldCell::MoveObject(CSpawnObject* pSpawnObject)
{
	CWorldCell::CELLDIR dir = GetCellDir(pSpawnObject->GetCurLoc());
	if (dir)
	{
		if (MoveCell(pSpawnObject, dir))
		{
			MoveZone(pSpawnObject);
			return true;
		}
	}

	return false;
}


CWorldCell::CELLDIR CWorldCell::GetCellDir(CNtlVector & rLoc)
{
	if (IsAdjoining(rLoc))
	{
		if (m_vEnd.x < rLoc.x)
		{
			if (rLoc.x <= m_vStart.x)
			{
				if (rLoc.z <= m_vStart.z)
				{
					if (m_vEnd.z < rLoc.z)
						return SELF;
					else
						return BOTTOM;
				}
				else
				{
					return TOP;
				}
			}
			else if (rLoc.z <= m_vStart.z)
			{
				if (m_vEnd.z < rLoc.z)
					return LEFT;
				else
					return L_BOTTOM;
			}
			else
			{
				return L_TOP;
			}
		}
		else if (rLoc.z <= m_vStart.z)
		{
			if (m_vEnd.z < rLoc.z)
				return RIGHT;
			else
				return R_BOTTOM;
		}
		else
		{
			return R_TOP;
		}
	}

	return MAX_CELLDIR;
}


CWorldCell::QUADPAGE CWorldCell::GetCellQuadPage(CNtlVector & rLoc)
{
	CNtlVector vHalf(m_vStart.operator+(m_vEnd));
	if (vHalf.x != 0.0f)
		vHalf.x = vHalf.x / 2.0f;
	if (vHalf.z != 0.0f)
		vHalf.z = vHalf.z / 2.0f;
	if (vHalf.x < rLoc.x)
	{
		if (vHalf.z < rLoc.z)
			return QUADPAGE_FOURTH;
		else
			return QUADPAGE_SECOND;
	}
	else if (vHalf.z < rLoc.z)
	{
		return QUADPAGE_THIRD;
	}

	return QUADPAGE_FIRST;
}


bool CWorldCell::IsInsideObject(CSpawnObject* pSpawnObject)
{
	return IsInBoundary(pSpawnObject->GetCurLoc());
}


bool CWorldCell::IsInBoundary(CNtlVector& rhs)
{
	return rhs.x <= this->m_vStart.x && rhs.z <= this->m_vStart.z && this->m_vEnd.x < rhs.x && this->m_vEnd.z < rhs.z;
}


bool CWorldCell::IsAdjoining(CNtlVector& rhs)
{
	return rhs.x <= (m_vStart.x + m_pParent->GetSplitSize())
		&& rhs.z <= (m_vStart.z + m_pParent->GetSplitSize())
		&& (m_vEnd.x - m_pParent->GetSplitSize()) < rhs.x
		&& (m_vEnd.z - m_pParent->GetSplitSize()) < rhs.z;
}


void CWorldCell::UpdateNearbyPlayerCount_Enter()
{
	for (int i = 0; i < MAX_CELLDIR; i++)
	{
		CWorldCell* pCell = m_sibling[i];
		if (pCell)
			pCell->m_dwNearbyPlayerCount = UnsignedSafeIncrease<DWORD>(pCell->m_dwNearbyPlayerCount, 1);
	}
}

void CWorldCell::UpdateNearbyPlayerCount_Leave()
{
	for (int i = 0; i < MAX_CELLDIR; i++)
	{
		CWorldCell* pCell = m_sibling[i];
		if (pCell)
			pCell->m_dwNearbyPlayerCount = UnsignedSafeDecrease<DWORD>(pCell->m_dwNearbyPlayerCount, 1);
	}
}


void CWorldCell::NotifyEnterObject(CSpawnObject * pObject)
{
	for (int objType = 0; objType <= OBJTYPE_DROPITEM; objType++)
	{
		for (CSpawnObject* pObj = m_objectList.GetFirst((eOBJTYPE)objType); pObj; pObj = m_objectList.GetNext(pObj->GetWorldCellObjectLinker()))
		{
			if (pObj != pObject)
			{
				if (pObj->IsInitialized() && pObject->IsInitialized())
				{
					if (pObject->IsPC())
						pObj->EncodeInsertPacket((CPlayer*)pObject);

					if (pObj->IsPC())
						pObject->EncodeInsertPacket((CPlayer*)pObj);
				}
			}
		}
	}

	if (pObject->IsPC() && pObject->IsInitialized())
	{
		for (CSpawnObject* pObj = m_objectList.GetFirst(OBJTYPE_TOBJECT); pObj; pObj = m_objectList.GetNext(pObj->GetWorldCellObjectLinker()))
		{
			if (pObj != pObject)
			{
				if (pObj->IsInitialized())
				{
					pObj->EncodeInsertPacket((CPlayer*)pObject);
				}
			}
		}
	}
}

void CWorldCell::NotifyLeaveObject(CSpawnObject * pObject)
{
	for (int objType = 0; objType <= OBJTYPE_DROPITEM; objType++)
	{
		for (CSpawnObject* pObj = m_objectList.GetFirst((eOBJTYPE)objType); pObj; pObj = m_objectList.GetNext(pObj->GetWorldCellObjectLinker()))
		{
			if (pObj != pObject)
			{
				if (pObject->IsPC())
					pObj->EncodeRemovePacket((CPlayer*)pObject);

				if (pObj->IsPC())
					pObject->EncodeRemovePacket((CPlayer*)pObj);
			}
		}
	}
}


bool CWorldCell::MoveCell(CSpawnObject * pSpawnObject, CELLDIR dir)
{
	switch (dir)
	{
		case SELF:
		{
			return MoveObject_SELF(pSpawnObject);
		}
		break;
		case L_TOP:
		{
			return MoveObject_LTOP(pSpawnObject);
		}
		break;
		case TOP:
		{
			return MoveObject_TOP(pSpawnObject);
		}
		break;
		case R_TOP:
		{
			return MoveObject_RTOP(pSpawnObject);
		}
		break;
		case RIGHT:
		{
			return MoveObject_RIGHT(pSpawnObject);
		}
		break;
		case R_BOTTOM:
		{
			return MoveObject_RBOTTOM(pSpawnObject);
		}
		break;
		case BOTTOM:
		{
			return MoveObject_BOTTOM(pSpawnObject);
		}
		break;
		case L_BOTTOM:
		{
			return MoveObject_LBOTTOM(pSpawnObject);
		}
		break;
		case LEFT:
		{
			return MoveObject_LEFT(pSpawnObject);
		}
		break;
		case MAX_CELLDIR:
		{
			return MoveObject_NONE(pSpawnObject, true);
		}
		break;

		default: break;
	}

	return false;
}


bool CWorldCell::MoveZone(CSpawnObject * pSpawnObject)
{
	CWorldZone* pOldZone = pSpawnObject->GetCurWorldZone();
	CWorldZone* pNewZone = NULL;

	if (pSpawnObject->GetCurWorldCell())
	{
		pNewZone = pSpawnObject->GetCurWorldCell()->GetWorldZone();
	}

	if (pNewZone != pOldZone)
	{
		if (pOldZone)
			pSpawnObject->OnLeaveWorldZone(pOldZone);
		if (pNewZone)
			pSpawnObject->OnEnterWorldZone(pNewZone);
	}

	return true;
}


void CWorldCell::OnObjectAdded(CSpawnObject * pSpawnObject)
{
}

void CWorldCell::OnObjectRemoved(CSpawnObject * pSpawnObject)
{
}


bool CWorldCell::MoveObject_SELF(CSpawnObject * pSpawnObject, bool bIsServerStart/* = false*/)
{
	CWorldCell* pDestinationWorldCell = m_sibling[SELF];
	if (pDestinationWorldCell)
	{
		if (!bIsServerStart)
		{
			CWorldCell* pWorldCell = NULL;
			for (int i = 0; i < MAX_CELLDIR; i++)
			{
				pWorldCell = m_sibling[i];
				if (pWorldCell)
					pWorldCell->NotifyEnterObject(pSpawnObject);
			}
		}
		pSpawnObject->OnEnterWorldCell(pDestinationWorldCell);

		return true;
	}
	else
	{
		ERR_LOG(LOG_USER, "[Handle : %u] (pDestinationWorldCell == NULL)", pSpawnObject->GetID());
	}

	return false;
}

bool CWorldCell::MoveObject_LTOP(CSpawnObject * pSpawnObject)
{
	CWorldCell* pDestinationWorldCell = m_sibling[L_TOP];
	if (pDestinationWorldCell)
	{
		CWorldCell* pWorldCell = NULL;
		for (int i = R_TOP; i <= L_BOTTOM; i++)
		{
			pWorldCell = m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyLeaveObject(pSpawnObject);
		}

		pSpawnObject->OnLeaveWorldCell(this);

		for (int i = L_TOP; i <= R_TOP; i++)
		{
			pWorldCell = m_sibling[L_TOP]->m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyEnterObject(pSpawnObject);
		}

		for (int i = L_BOTTOM; i <= LEFT; i++)
		{
			pWorldCell = m_sibling[L_TOP]->m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyEnterObject(pSpawnObject);
		}

		pSpawnObject->OnEnterWorldCell(pDestinationWorldCell);

		return true;
	}

	return false;
}

bool CWorldCell::MoveObject_TOP(CSpawnObject * pSpawnObject)
{
	CWorldCell* pDestinationWorldCell = m_sibling[TOP];
	if (pDestinationWorldCell)
	{
		CWorldCell* pWorldCell = NULL;
		for (int i = R_BOTTOM; i <= L_BOTTOM; i++)
		{
			pWorldCell = m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyLeaveObject(pSpawnObject);
		}

		pSpawnObject->OnLeaveWorldCell(this);

		for (int i = L_TOP; i <= R_TOP; i++)
		{
			pWorldCell = m_sibling[TOP]->m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyEnterObject(pSpawnObject);
		}

		pSpawnObject->OnEnterWorldCell(pDestinationWorldCell);

		return true;
	}

	return false;
}

bool CWorldCell::MoveObject_RTOP(CSpawnObject * pSpawnObject)
{
	CWorldCell* pDestinationWorldCell = m_sibling[R_TOP];
	if (pDestinationWorldCell)
	{
		CWorldCell* pWorldCell = NULL;
		for (int i = R_BOTTOM; i <= LEFT; i++)
		{
			pWorldCell = m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyLeaveObject(pSpawnObject);
		}
		for (int i = L_TOP; i <= L_TOP; i++)
		{
			pWorldCell = m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyLeaveObject(pSpawnObject);
		}

		pSpawnObject->OnLeaveWorldCell(this);

		for (int i = L_TOP; i <= R_BOTTOM; i++)
		{
			pWorldCell = m_sibling[R_TOP]->m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyEnterObject(pSpawnObject);
		}

		pSpawnObject->OnEnterWorldCell(pDestinationWorldCell);

		return true;
	}

	return false;
}

bool CWorldCell::MoveObject_RIGHT(CSpawnObject * pSpawnObject)
{
	CWorldCell* pDestinationWorldCell = m_sibling[RIGHT];
	if (pDestinationWorldCell)
	{
		CWorldCell* pWorldCell = NULL;
		for (int i = L_BOTTOM; i <= LEFT; i++)
		{
			pWorldCell = m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyLeaveObject(pSpawnObject);
		}
		for (int i = L_TOP; i <= L_TOP; i++)
		{
			pWorldCell = m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyLeaveObject(pSpawnObject);
		}

		pSpawnObject->OnLeaveWorldCell(this);

		for (int i = R_TOP; i <= R_BOTTOM; i++)
		{
			pWorldCell = m_sibling[RIGHT]->m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyEnterObject(pSpawnObject);
		}

		pSpawnObject->OnEnterWorldCell(pDestinationWorldCell);

		return true;
	}

	return false;
}

bool CWorldCell::MoveObject_RBOTTOM(CSpawnObject * pSpawnObject)
{
	CWorldCell* pDestinationWorldCell = m_sibling[R_BOTTOM];
	if (pDestinationWorldCell)
	{
		CWorldCell* pWorldCell = NULL;
		for (int i = L_BOTTOM; i <= LEFT; i++)
		{
			pWorldCell = m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyLeaveObject(pSpawnObject);
		}
		for (int i = L_TOP; i <= R_TOP; i++)
		{
			pWorldCell = m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyLeaveObject(pSpawnObject);
		}

		pSpawnObject->OnLeaveWorldCell(this);

		for (int i = R_TOP; i <= L_BOTTOM; i++)
		{
			pWorldCell = m_sibling[R_BOTTOM]->m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyEnterObject(pSpawnObject);
		}

		pSpawnObject->OnEnterWorldCell(pDestinationWorldCell);

		return true;
	}

	return false;
}

bool CWorldCell::MoveObject_BOTTOM(CSpawnObject * pSpawnObject)
{
	CWorldCell* pDestinationWorldCell = m_sibling[BOTTOM];
	if (pDestinationWorldCell)
	{
		CWorldCell* pWorldCell = NULL;
		for (int i = L_TOP; i <= R_TOP; i++)
		{
			pWorldCell = m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyLeaveObject(pSpawnObject);
		}

		pSpawnObject->OnLeaveWorldCell(this);

		for (int i = R_BOTTOM; i <= L_BOTTOM; i++)
		{
			pWorldCell = m_sibling[BOTTOM]->m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyEnterObject(pSpawnObject);
		}

		pSpawnObject->OnEnterWorldCell(pDestinationWorldCell);

		return true;
	}

	return false;
}

bool CWorldCell::MoveObject_LBOTTOM(CSpawnObject * pSpawnObject)
{
	CWorldCell* pDestinationWorldCell = m_sibling[L_BOTTOM];
	if (pDestinationWorldCell)
	{
		CWorldCell* pWorldCell = NULL;
		for (int i = L_TOP; i <= R_BOTTOM; i++)
		{
			pWorldCell = m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyLeaveObject(pSpawnObject);
		}

		pSpawnObject->OnLeaveWorldCell(this);

		for (int i = R_BOTTOM; i <= LEFT; i++)
		{
			pWorldCell = m_sibling[L_BOTTOM]->m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyEnterObject(pSpawnObject);
		}

		for (int i = L_TOP; i <= L_TOP; i++)
		{
			pWorldCell = m_sibling[L_BOTTOM]->m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyEnterObject(pSpawnObject);
		}

		pSpawnObject->OnEnterWorldCell(pDestinationWorldCell);

		return true;
	}

	return false;
}

bool CWorldCell::MoveObject_LEFT(CSpawnObject * pSpawnObject)
{
	CWorldCell* pDestinationWorldCell = m_sibling[LEFT];
	if (pDestinationWorldCell)
	{
		CWorldCell* pWorldCell = NULL;
		for (int i = R_TOP; i <= R_BOTTOM; i++)
		{
			pWorldCell = m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyLeaveObject(pSpawnObject);
		}

		pSpawnObject->OnLeaveWorldCell(this);

		for (int i = L_BOTTOM; i <= LEFT; i++)
		{
			pWorldCell = m_sibling[LEFT]->m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyEnterObject(pSpawnObject);
		}

		for (int i = L_TOP; i <= L_TOP; i++)
		{
			pWorldCell = m_sibling[LEFT]->m_sibling[i];
			if (pWorldCell)
				pWorldCell->NotifyEnterObject(pSpawnObject);
		}

		pSpawnObject->OnEnterWorldCell(pDestinationWorldCell);

		return true;
	}

	return false;
}

bool CWorldCell::MoveObject_NONE(CSpawnObject * pSpawnObject, bool bEnter)
{
	CWorldCell* pDestWorldCell = NULL;

	if (bEnter == true)
	{
		pDestWorldCell = m_pParent->GetWorldCellWithLoc(pSpawnObject->GetCurLoc());
		if (pDestWorldCell == NULL)
		{
			ERR_LOG(LOG_USER, "[Handle : %u] (NULL == pCurrentWorldCell), pSpawnObject->GetCurLoc() = (%f, %f, %f), m_cellID = %u, m_pParent->GetID() = %u, pSpawnObject->GetObjType() = %u",
				pSpawnObject->GetID(), pSpawnObject->GetCurLoc().x, pSpawnObject->GetCurLoc().y, pSpawnObject->GetCurLoc().z, m_cellID, m_pParent->GetID(), pSpawnObject->GetObjType());

			return false;
		}
	}

	CWorldCell* pCurrentWorldCell = m_sibling[SELF];
	if (!pCurrentWorldCell)
	{
		ERR_LOG(LOG_USER, "[Handle : %u] (NULL == pCurrentWorldCell), m_cellID = %u, m_pParent->GetID() = %u", pSpawnObject->GetID(), m_cellID, m_pParent->GetID());
	}
		
	CWorldCell* pWorldCell = NULL;
	for (int i = SELF; i < MAX_CELLDIR; i++)
	{
		pWorldCell = m_sibling[i];
		if (pWorldCell)
			pWorldCell->NotifyLeaveObject(pSpawnObject);
	}
	pSpawnObject->OnLeaveWorldCell(this);

	if (bEnter)
	{
		if (pDestWorldCell)
		{
			for (int i = SELF; i < MAX_CELLDIR; i++)
			{
				pWorldCell = pDestWorldCell->m_sibling[i];
				if (pWorldCell)
					pWorldCell->NotifyEnterObject(pSpawnObject);
			}

			pSpawnObject->OnEnterWorldCell(pDestWorldCell);
		}
		else
		{
			ERR_LOG(LOG_USER, "pDestWorldCell is NULL even though bEnter is true.");

			return false;
		}
	}

	return true;
}

void CWorldCell::SendPacket(CNtlPacket * pPacket, CSpawnObject * pExcept/* = NULL*/)
{
	int i = 0;
	CSpawnObject* pObj = m_objectList.GetFirst(OBJTYPE_PC);
	while (pObj)
	{
		if (pObj != pExcept)
		{
			pObj->SendPacket(pPacket);
		}

		pObj = m_objectList.GetNext(pObj->GetWorldCellObjectLinker());

		if (++i >= MAX_PC)
		{
			ERR_LOG(LOG_SYSTEM, "tried to send packet to more than 3000 people");
			break;
		}
	}
}

