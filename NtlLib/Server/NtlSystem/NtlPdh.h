//***********************************************************************************
//
//	File		:	NtlPdh.h
//
//	Begin		:	2007-02-07
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlString.h"

#include <windows.h>
#include <pdh.h>

class CNtlPdh
{
private:

	enum eCONSTANT
	{
		MAX_COUNTER_COUNT = 16
	};


public:

	CNtlPdh();

	~CNtlPdh(void);


public:

	int							Prepare();

	int							RegisterCounter(const char * lpszCounter);

	int							Start(const char * lpszFileName);

	void						Stop();

	LONG						GetValue();

	void						UpdateLog();


public:

	static int					UnitTest();


private:

	void						Init();


public:

	HQUERY						m_hQuery;

	HLOG						m_hLog;

	DWORD						m_dwCounterCount;

	HCOUNTER					m_ahCounter[MAX_COUNTER_COUNT];

	std::string					m_astrCounterName[MAX_COUNTER_COUNT];
};