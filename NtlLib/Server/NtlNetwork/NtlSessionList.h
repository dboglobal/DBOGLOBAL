//***********************************************************************************
//
//	File		:	SessionList.h
//
//	Begin		:	2007-01-02
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlSessionListBase.h"

class CNtlSessionList : public CNtlSessionListBase
{

public:

	CNtlSessionList();

	virtual ~CNtlSessionList();

public:

	virtual bool			Add(CNtlSession* pSession);

	virtual bool			Remove(CNtlSession* pSession);

	virtual void			ValidCheck(DWORD dwTickTime);

};

