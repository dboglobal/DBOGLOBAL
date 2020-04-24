//***********************************************************************************
//
//	File		:	NtlLog.h
//
//	Begin		:	2006-01-05
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlSingleton.h"

#include "NtlMiniDump.h"
#include "NtlBase.h"
#include "NtlString.h"
#include <list>
#include <crtdbg.h>
#include "NtlFile.h"


enum eNTL_LOG_SOURCE
{
	NTL_LOG_SOURCE_LOCAL = 0
};

//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
enum eNTL_LOG_CHANNEL
{
	LOG_FIRST = 0,

	LOG_GENERAL = LOG_FIRST,
	LOG_SYSTEM,
	LOG_WARNING,
	LOG_ASSERT,
	LOG_NETWORK,
	LOG_TRAFFIC,
	LOG_USER,
	LOG_QUEST,
	LOG_SCRIPT,
	LOG_BOTAI,
	LOG_HACK,
	LOG_RANKBATTLE,

	LOG_LAST
};

struct sLOG_BRIEF
{
	int			nLine;
	CNtlString	szFuntion;
	CNtlString	szFile;
};

typedef std::list<sLOG_BRIEF*> LOGLIST;
typedef LOGLIST::iterator LOGLISTIT;

//-----------------------------------------------------------------------------------

LPCTSTR GetLogChannelString(BYTE byLogChannel);

extern FILE * s_log_stream;
//-----------------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------------
class CNtlLog : public CNtlSingleton<CNtlLog>
{
public:

	CNtlLog() 
	{
		::InitializeCriticalSection(&m_loglock);
		m_dwDefaultLogSource = NTL_LOG_SOURCE_LOCAL; 
		ResetList();

	}

	virtual ~CNtlLog() 
	{
		ResetList();
		::DeleteCriticalSection(&m_loglock);
	}

public:

	virtual int			RegisterSource(DWORD dwSource, LPCTSTR lpszSourceName);

	virtual int			RegisterChannel(DWORD dwSource, BYTE byChannel, LPCTSTR lpszChannelName, LPCTSTR lpszFileNamePrefix, LPCTSTR lpszFileNameSuffix);

	virtual void		Log(BYTE byLogChannel, bool bDate, LPCTSTR lpszFile, int nLine, LPCTSTR lpszFunc, LPCTSTR lpszText, ...);

	virtual void		Log(LPCTSTR lpszText, ...);


public:

	DWORD				GetDefaultLogSource() { return m_dwDefaultLogSource; }

	void				SetDefaultLogSource(DWORD dwDefaultLogSource);

	int					AttachLogStream(FILE * fp);

	void				DetachLogStream();

	int					RegisterBaseChannel(DWORD dwSource, LPCTSTR lpszSourceName);

	void				ResetList();

	bool				IsNew(LPCTSTR lpszFile, int nLine, LPCTSTR lpszFunc);

public:

	static int			UnitTest();

private:

	CRITICAL_SECTION	m_loglock;

	DWORD				m_dwDefaultLogSource;
	LOGLIST				m_listLog;

};


//-----------------------------------------------------------------------------------
// D ¡æ Date, L ¡æ Location
//-----------------------------------------------------------------------------------
#define g_pLog CNtlLog::GetInstance()
#define NTL_LOG( LOGCHANNEL, LOGMSG, ... ) if( g_pLog ) g_pLog->Log( LOGCHANNEL, true, NULL, 0, NULL, LOGMSG, __VA_ARGS__ );
#define NTL_LOGD( LOGCHANNEL, LOGMSG, ... ) if( g_pLog ) g_pLog->Log( LOGCHANNEL, true, NULL, 0, NULL, LOGMSG, __VA_ARGS__ )
#define NTL_LOGL( LOGCHANNEL, LOGMSG, ... ) if( g_pLog ) g_pLog->Log( LOGCHANNEL, false, __FILE__, __LINE__, __FUNCTION__, LOGMSG, __VA_ARGS__ )
#define NTL_LOGDL( LOGCHANNEL, LOGMSG, ... ) if( g_pLog ) g_pLog->Log( LOGCHANNEL, true, __FILE__, __LINE__, __FUNCTION__, LOGMSG, __VA_ARGS__ )
#define ERR_LOG( LOGCHANNEL, LOGMSG, ... ) if( g_pLog ) g_pLog->Log( LOGCHANNEL, true, __FILE__, __LINE__, __FUNCTION__, LOGMSG, __VA_ARGS__ )
#define HACK_LOG(LOGMSG, ... ) if( g_pLog ) g_pLog->Log( LOGMSG, __VA_ARGS__ )

#if defined( _DEBUG ) && defined( _DEVEL )
	#define NTL_LOG_ASSERT( LOGMSG, ... )																\
	{																									\
		if( g_pLog )																					\
			g_pLog->Log( LOG_ASSERT, true, __FILE__, __LINE__, __FUNCTION__, LOGMSG, __VA_ARGS__ );		\
		if( 1 == _CrtDbgReport( _CRT_ASSERT, __FILE__, __LINE__, __FUNCTION__, LOGMSG, __VA_ARGS__ ) )	\
			_CrtDbgBreak();																				\
	}
#elif defined( _DEBUG )
	#define NTL_LOG_ASSERT( LOGMSG, ... )																\
	{																									\
		if( g_pLog )																					\
			g_pLog->Log( LOG_ASSERT, true, __FILE__, __LINE__, __FUNCTION__, LOGMSG, __VA_ARGS__ );		\
		if ( g_pLog->IsNew( __FILE__, __LINE__, __FUNCTION__ ) )										\
		{																								\
			CNtlMiniDump::Snapshot();																	\
		}																								\
	}
#else
	#define NTL_LOG_ASSERT( LOGMSG, ... )																\
	{																									\
		if( g_pLog )																					\
			g_pLog->Log( LOG_ASSERT, true, __FILE__, __LINE__, __FUNCTION__, LOGMSG, __VA_ARGS__ );		\
		if ( g_pLog->IsNew( __FILE__, __LINE__, __FUNCTION__ ) )										\
		{																								\
			CNtlMiniDump::Snapshot();																	\
		}																								\
	}
#endif
//-----------------------------------------------------------------------------------