//***********************************************************************************
//
//	File		:	NtlSessionListBase.h
//
//	Begin		:	25 OCT 17
//
//	Copyright	:	DBO GLOBAL
//
//	Author		:	Daneos
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlLinkArray.h"
#include "NtlMutex.h"


class CNtlNetwork;
class CNtlSession;

class CNtlSessionListBase
{
public:

	CNtlSessionListBase();

	virtual ~CNtlSessionListBase();


protected:

	typedef CNtlLinkArray_Static<CNtlSession*> LIST;
	typedef LIST::POSITION LISTIT;

	LIST								m_sessionList;

	CNtlMutex							m_mutex;

	CNtlNetwork *						m_pNetworkRef;


public:

	int									Create(CNtlNetwork * pNetwork, int nSessionSize, int nExtraSize);

	void								Destroy();


public:

	CNtlSession *						Find(LISTIT pos);

	HSESSION							_Add(CNtlSession * pSession);

	HSESSION							_Remove(HSESSION hSession);
	HSESSION							_Remove(CNtlSession * pSession);

	virtual bool						Add(CNtlSession * pSession) = 0;

	virtual bool						Remove(CNtlSession * pSession) = 0;
	
	virtual void						ValidCheck(DWORD dwTickTime) = 0;

	//virtual void						RemoveInvalidAcceptSession();

public:

	int									GetCurCount();

	int									GetMaxCount();


protected:

	void								Init();

	void								PacketLog(CNtlSession * pSession);

	void								SetSessionHandle(CNtlSession * pSession, HSESSION hSession);

	void								SetAcceptingSessionHandle(CNtlSession * pSession, HSESSION hSession);

};

