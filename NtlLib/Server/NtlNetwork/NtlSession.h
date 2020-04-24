#pragma once


#include "NtlConnection.h"


// Session delete macro
#define RELEASE_SESSION(p)			if(p) { p->Release(); }


class CNtlSession : public CNtlConnection
{
	friend class CNtlSessionListBase;

public:

	CNtlSession(SESSIONTYPE sessionType);
	virtual ~CNtlSession();


public:

	virtual int						OnConnect();

	virtual int						OnAccept();

	virtual void					OnClose() {}

	virtual void					SetDestroy() { SetStatus(STATUS_DESTROY); } 

	virtual int						OnDispatch(CNtlPacket * pPacket);

public:

	int								ProcessPacket();


public:

	HSESSION						GetHandle() { return m_hSession; }

	HSESSION						GetAcceptingHandle() { return m_hAcceptingSession; }

	SESSIONTYPE						GetSessionType() { return m_sessionType; }

	const UINT32					GetUniqueHandle() { return m_uniqueHandle; }


public:

	void							Acquire();

	void							Release();


public:

	static bool						IsInternalConnection(char* pIp);


protected:

	void							Init();

	void							Destroy();

	void							PacketLog() {}

private:

	CNtlSession&					operator=(CNtlSession&);


private:

	DWORD							m_dwReferenceCount;

	HSESSION						m_hSession;

	HSESSION						m_hAcceptingSession;

	const SESSIONTYPE				m_sessionType;

	int								ALLOWED_DECRYPTION_FAILURE_COUNT = 1;

	int								m_nDecryptionFailureCount;

	UINT32							m_uniqueHandle;

};
