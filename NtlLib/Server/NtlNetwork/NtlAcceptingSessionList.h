//***********************************************************************************
//
//	File		:	NtlAcceptingSessionList.h
//
//	Begin		:	25 OCT 17
//
//	Copyright	:	DBO GLOBAL
//
//	Author		:	DANEOS
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlSessionListBase.h"

class CNtlAcceptingSessionList : public CNtlSessionListBase
{

public:

	CNtlAcceptingSessionList();

	virtual ~CNtlAcceptingSessionList();

public:

	virtual bool			Add(CNtlSession* pSession);

	virtual bool			Remove(CNtlSession* pSession);

	virtual void			ValidCheck(DWORD dwTickTime);

};

