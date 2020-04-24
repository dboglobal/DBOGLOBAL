#ifndef __INC_DBOG_SPAWN_OBJECT_H__
#define __INC_DBOG_SPAWN_OBJECT_H__

#include "GameObject.h"
#include "NtlVector.h"

#include "NtlWorld.h"

#include "World.h"
#include "WorldCell.h"
#include "WorldZone.h"
#include "WorldObjectLinker.h"

class CPlayer;

//====================================================================
//  CSpawnObject
//  Class for everything which spawn on world
//====================================================================
class CSpawnObject : public CGameObject
{

public:
	CSpawnObject(eOBJTYPE eObjType);
	virtual	~CSpawnObject();

	virtual void	EncodeInsertPacket(CPlayer* pPlayer); //SHOW OBJECT PACKET
	void			EncodeRemovePacket(CPlayer* pPlayer); //HIDE OBJECT PACKET

	bool			Create();
	void			Destroy();

protected:

	void			Init();

public:

	bool			IsLocInWorldBoundary(CNtlVector& rLoc, CWorld* pWorld);

public:

	virtual void			OnEnterWorld(CWorld* pWorld);
	virtual void			OnLeaveWorld(CWorld* pWorld);
	virtual void			OnEnterWorldCell(CWorldCell* pWorldCell);
	virtual void			OnLeaveWorldCell(CWorldCell* pWorldCell);
	virtual void			OnEnterWorldZone(CWorldZone* pWorldZone);
	virtual void			OnLeaveWorldZone(CWorldZone* pWorldZone);
	virtual void			OnEnterWorldComplete();

	virtual void			CopyToObjectInfo(sOBJECT_INFO *pObjectInfo, CHARACTERID playerCharID) = 0;

	void					Broadcast(CNtlPacket* pPacket, CSpawnObject* except = nullptr); //send to others with exception
	void					BroadcastToNeighbor(CNtlPacket* pPacket); //send to others but not to myself

	WORLDID					GetWorldID() { return m_worldID; }
	TBLIDX					GetWorldTblidx();
	inline void				SetWorldID(WORLDID world) { m_worldID = world; }

	CWorld*					GetCurWorld() { return m_pCurWorld; }
	CWorldCell*				GetCurWorldCell() { return m_pCurWorldCell; }
	CWorldZone*				GetCurWorldZone() { return m_pCurWorldZone; }
	CWorldObjectLinker*		GetWorldObjectLinker() { return &m_worldLinker; }
	CWorldObjectLinker*		GetWorldCellObjectLinker() { return &m_worldCellLinker; }
	CWorldObjectLinker*		GetWorldZoneObjectLinker() { return &m_worldZoneLinker; }


	CNtlVector&				GetCurLoc() { return m_vCurLoc; }
	CNtlVector&				GetCurDir() { return m_vCurDir; }


	bool					SetCurLoc(sVECTOR3& loc, CWorld *pDestWorld = nullptr);
	bool					SetCurLoc(CNtlVector& loc, CWorld *pDestWorld = nullptr);

	inline void				SetCurDir(sVECTOR3& dir)			{ m_vCurDir.operator=(dir); }
	inline void				SetCurDir(CNtlVector& dir)			{ m_vCurDir.operator=(dir); }
	inline void				SetCurDir(float x, float y, float z){ m_vCurDir.x = x; m_vCurDir.y = y; m_vCurDir.z = z; }

	bool					IsInRange(CNtlVector& rLoc, float fRange);
	bool					IsInRange(CSpawnObject* pTarget, float fRange);

	bool					IsInRange3(CNtlVector& rLoc, float fRange);
	bool					IsInRange3(CSpawnObject* pTarget, float fRange);

	bool					IsInRangeWithTolerance(CNtlVector& rLoc, float fRange, float fTolerance);

	bool					IsOutRange(CNtlVector& rLoc, float fRange);
	bool					IsOutRangeWithTolerance(CNtlVector& rLoc, float fRange, float fTolerance);

	inline void				SetMapNameTblidx(TBLIDX idx){ mapNameTblidx = idx; }
	inline TBLIDX			GetMapNameTblidx() 	{ return mapNameTblidx; }

	virtual float			GetObjectRadius() { return 0.0f; }

	float					GetDistance(CSpawnObject* pTarget);
	float					GetDistance(CNtlVector& rLoc);
	float					GetDistance3(CNtlVector& rLoc);

	bool					IsFrontSide(CSpawnObject* pTarget);
	bool					IsInDegreeAngle(CSpawnObject* pTarget, int nDegree);
	bool					IsInRangeFront(CSpawnObject* pTarget, float fRange);
	bool					IsInRangeBack(CSpawnObject* pTarget, float fRange);

	void					GetPcInRange(float fRange, std::vector<CPlayer*>& rVec);


protected:

	WORLDID					m_worldID;		// Worlds instance handle

	CNtlVector				m_vCurLoc;
	CNtlVector				m_vCurDir;
	bool					m_bIsLocValidityChecked;

private:
	TBLIDX					mapNameTblidx;

protected:
	CWorld*					m_pCurWorld;
	CWorldCell*				m_pCurWorldCell;
	CWorldZone*				m_pCurWorldZone;
	CWorldObjectLinker		m_worldLinker;
	CWorldObjectLinker		m_worldCellLinker;
	CWorldObjectLinker		m_worldZoneLinker;

	DWORD					m_dwWorldUpadateTick;

};

#endif
