#ifndef __INC_DBOG_GAME_OBJECT_H__
#define __INC_DBOG_GAME_OBJECT_H__

#include "ObjectController.h"
#include "EventableObject.h"

#include "ObjectMsg.h"
#include "ClientSession.h"

//====================================================================
//  CGameObject
//  class for every object except OBJTYPE_ITEM
//====================================================================
class CGameObject : public EventableObject
{

public:
	CGameObject(eOBJTYPE eObjType);
	virtual	~CGameObject();


	bool			Create();
	void			Destroy();

	void			Init();

public:

	virtual void					TickProcess(DWORD dwTickDiff, float fMultiple);

	virtual int						OnObjectMsg(CObjectMsg* pObjectMsg);


	virtual void					LeaveGame() {}

	virtual CObjectController*		CreateObjectController() { return NULL; }

	inline bool						IsInitialized() { return m_bIsInitialized; }
	inline void						SetInitialized(bool bFlag) { m_bIsInitialized = bFlag; }

	eOBJTYPE						GetObjType();
	bool							IsObjType(BYTE type);

	bool							IsPC() const		{ return m_objType == OBJTYPE_PC; }
	bool							IsNPC()	const		{ return m_objType == OBJTYPE_NPC; }
	bool							IsMonster()	const	{ return m_objType == OBJTYPE_MOB; }
	bool							IsSummonPet()const	{ return m_objType == OBJTYPE_SUMMON_PET; }
	bool							IsItemPet()	const	{ return m_objType == OBJTYPE_ITEM_PET; }
	bool							IsToObj()	const	{ return m_objType == OBJTYPE_TOBJECT; }
	bool							IsDynObj()	const	{ return m_objType == OBJTYPE_DYNAMIC; }

	HOBJECT							GetID() { return m_handle; }
	void							SetID(HOBJECT hHandle) { m_handle = hHandle; }


	void							SendPacket(CNtlPacket* pPacket);

	inline void						SetClientSession(CClientSession* session) { m_ClientSession = session; }
	inline CClientSession*			GetClientSession()  { return m_ClientSession; }
	inline void						SetClientSessionID(HSESSION id) { hSessionID = id; }
	inline HSESSION					GetClientSessionID()  { return hSessionID; }

	int								SendObjectMsg(CObjectMsg* pObjectMsg);
	void							PostObjectMsg(CObjectMsg* pObjectMsg, DWORD dwDelayTime);


protected:

	eOBJTYPE				m_objType;
	HOBJECT					m_handle;
	CObjectMsgQueue			m_objectMsgQueue;
	CObjectController*		m_pObjectController;

	bool					m_bIsInitialized;
	bool					m_bIsDeleted;

	CClientSession*			m_ClientSession;
	HSESSION				hSessionID;

};

#endif
