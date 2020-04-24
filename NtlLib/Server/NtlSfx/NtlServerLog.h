//***********************************************************************************
//
//	File		:	NtlServerLog.h
//
//	Begin		:	2007-03-14
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlLog.h"

#include "NtlMemoryPool.h"
#include "NtlMutex.h"

#include <deque>


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class CNtlServerLogData
{
	DECLARE_DYNAMIC_MEMORYPOOL_THREADSAFE( CNtlServerLogData );

	enum 
	{
		SERVER_BUFSIZE_LOG = 2048
	};

public:

	CNtlServerLogData():
#ifdef _WIN64
	sequence( 0xFFFFFFFFFFFFFFFF )
#else
	sequence( 0xFFFFFFFF )
#endif
	{
		achLogText[0] = '\0';
	}

	~CNtlServerLogData() {}


public:

	int				GetBufSize() { return SERVER_BUFSIZE_LOG; }

public:

	DWORD			dwSource;
	BYTE			byChannel;
	WORD			wStrLen;
	CHAR 			achLogText[SERVER_BUFSIZE_LOG + 1];

#ifdef _WIN64
	DWORD64			sequence;
	static DWORD64	curSequence;
#else
	DWORD			sequence;
	static DWORD	curSequence;
#endif

};


//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class CNtlServerLog : public CNtlLog
{
public:

	CNtlServerLog(void);

	virtual ~CNtlServerLog(void);


public:

// overrides

	void							Log(BYTE byLogChannel, bool bDate, LPCTSTR lpszFile, int nLine,	LPCTSTR lpszFunc, LPCTSTR lpszText, ...);


protected:

	bool							PushLog(CNtlServerLogData* pLogData);

	CNtlServerLogData*				PopLog();

	CNtlServerLogData*				PeekLog();


private:

	void							Init();

	void							Destroy();


private:

	CNtlMutex						m_mutex;

	std::deque<CNtlServerLogData*>	m_logQueue;
};
