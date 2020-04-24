#ifndef __WORLD_CELL_H__
#define __WORLD_CELL_H__

#include "WorldObjectList.h"

class CWorld;
class CWorldZone;
class CSpawnObject;
class CNtlPacket;

class CWorldCell
{

public:

	enum CELLDIR
	{
		SELF = 0x0,
		L_TOP,
		TOP,
		R_TOP,
		RIGHT,
		R_BOTTOM,
		BOTTOM,
		L_BOTTOM,
		LEFT,

		MAX_CELLDIR,
		FIRST = SELF,
		LAST = MAX_CELLDIR - 1,
		NONE = MAX_CELLDIR,
	};

	enum QUADPAGE
	{
		QUADPAGE_FIRST = 0x0,
		QUADPAGE_SECOND,
		QUADPAGE_THIRD,
		QUADPAGE_FOURTH,

		QUADPAGE_COUNT,
		QUADPAGE_LAST = QUADPAGE_COUNT - 1,
	};

	enum QUADDIR
	{
		QUADDIR_SELF = 0x0,
		QUADDIR_HOTIZONTAL,
		QUADDIR_DIAGONAL,
		QUADDIR_VERTICAL,

		QUADDIR_COUNT,
		QUADDIR_FIRST = QUADDIR_SELF,
		QUADDIR_LAST = QUADDIR_COUNT - 1,
	};

public:

	CWorldCell();
	virtual ~CWorldCell();


public:

	void						Init();

	int							Create(CWorld *pWorld, CELLID cellID, CNtlVector& rStart, CNtlVector& rEnd);
	void						Destroy();

	CELLID						GetCellId();
	CWorldCell*					GetSibling(CELLDIR dir);
	CWorldCell*					GetQuadSibling(QUADPAGE page, QUADDIR dir);
	CELLID						GetSiblingID(CELLDIR dir);
	bool						IsValidCellID(CELLID cellID);

	CWorldObjectList*			GetObjectList();

	CWorldZone*					GetWorldZone();

	CNtlVector&					GetStartLoc();
	CNtlVector&					GetEndLoc();

	DWORD						GetNearbyPlayerCount();

	void						Broadcast(std::list<CNtlPacket*>& rPacketList);
	virtual void				Broadcast(CNtlPacket * pPacket, CSpawnObject* pExcept = NULL, bool bQuad = false);

	int							EnterObject(CSpawnObject * pSpawnObject, bool bIsServerStart = false);
	int							LeaveObject(CSpawnObject * pSpawnObject);

	void						AddObject(CSpawnObject * pSpawnObject);
	void						RemoveObject(CSpawnObject * pSpawnObject);
	bool						MoveObject(CSpawnObject * pSpawnObject);

	CELLDIR						GetCellDir(CNtlVector & rLoc);
	QUADPAGE					GetCellQuadPage(CNtlVector & rLoc);

	bool						IsInsideObject(CSpawnObject* pSpawnObject);
	bool						IsInBoundary(CNtlVector& rhs);
	bool						IsAdjoining(CNtlVector& rhs);
	void						UpdateNearbyPlayerCount_Enter();
	void						UpdateNearbyPlayerCount_Leave();
	void						NotifyEnterObject(CSpawnObject * pObject);
	void						NotifyLeaveObject(CSpawnObject * pObject);
	bool						MoveCell(CSpawnObject * pSpawnObject, CELLDIR dir);
	bool						MoveZone(CSpawnObject * pSpawnObject);


protected:

	virtual void				OnObjectAdded(CSpawnObject * pSpawnObject);
	virtual void				OnObjectRemoved(CSpawnObject * pSpawnObject);

	bool						MoveObject_SELF(CSpawnObject * pSpawnObject, bool bIsServerStart = false);
	bool						MoveObject_LTOP(CSpawnObject * pSpawnObject);
	bool						MoveObject_TOP(CSpawnObject * pSpawnObject);
	bool						MoveObject_RTOP(CSpawnObject * pSpawnObject);
	bool						MoveObject_RIGHT(CSpawnObject * pSpawnObject);
	bool						MoveObject_RBOTTOM(CSpawnObject * pSpawnObject);
	bool						MoveObject_BOTTOM(CSpawnObject * pSpawnObject);
	bool						MoveObject_LBOTTOM(CSpawnObject * pSpawnObject);
	bool						MoveObject_LEFT(CSpawnObject * pSpawnObject);
	bool						MoveObject_NONE(CSpawnObject * pSpawnObject, bool bEnter);

	void						SendPacket(CNtlPacket * pPacket, CSpawnObject * pExcept = NULL);


private:

	CELLID				m_cellID;

	CNtlVector			m_vStart;
	CNtlVector			m_vEnd;

	CWorld*				m_pParent;

	CWorldCell*			m_sibling[MAX_CELLDIR];

	CWorldObjectList	m_objectList;

	CWorldZone*			m_pWorldZoneRef;

	DWORD				m_dwNearbyPlayerCount;
	
};


#endif