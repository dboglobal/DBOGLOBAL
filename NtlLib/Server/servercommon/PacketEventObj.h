#ifndef __INC_DBOG_PACKET_EVENT_OBJ_H__
#define __INC_DBOG_PACKET_EVENT_OBJ_H__

#include "NtlLinkList.h"
#include "NtlPacket.h"
#include "NtlBase.h"

class CClientSession;
class CNtlSession;

class CPacketEventObj : public CNtlLinkObject
{

public:

	CPacketEventObj(HSESSION hSessionHandle, CNtlPacket* pPacket, UINT32 SessionUniqueID, CNtlSession* pSession)
		:m_hSessionHandle(hSessionHandle), m_packet(*pPacket), m_hSessionUniqueID(SessionUniqueID), m_pSession(pSession) {}

	virtual ~CPacketEventObj() {}

public:

	const HSESSION			GetHandle() { return m_hSessionHandle; }

	const HSESSION			GetUniqueSessionID() { return m_hSessionUniqueID; }

	CNtlSession*			GetSession() { return m_pSession; }

	CNtlPacket*				GetPacket() { return &m_packet; }

public:

	virtual void			Call() = 0;

protected:

	HSESSION				m_hSessionHandle;
	UINT32					m_hSessionUniqueID;
	CNtlPacket				m_packet;
	CNtlSession*			m_pSession;
};

template<class TYPE>
class TPacketEventObj : public CPacketEventObj
{
public:

	TPacketEventObj(TYPE* pObject, void (TYPE::*func)(CNtlPacket* pPacket), HSESSION hSessionHandle, CNtlPacket* pPacket, UINT32 SessionUniqueID)
		:CPacketEventObj(hSessionHandle, pPacket, SessionUniqueID, pObject),
		m_pObject(pObject), m_func(func) {}

	virtual ~TPacketEventObj() {}

public:

	virtual void	Call() { (m_pObject->*m_func)(&m_packet); }

private:

	TYPE*			m_pObject;
	void			(TYPE::*m_func)(CNtlPacket* pPacket);
};



#endif