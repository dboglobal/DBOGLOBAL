#include "StdAfx.h"
#include <direct.h>
#include <errno.h>
#include "NtlLogSystem.h"
#include "NtlStringHandler.h"

#include <string.h>

const char* CNtlLogSystem::m_pszShortMonthName[] =
{
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec",
	NULL
};

CNtlLogSystem::CNtlLogSystem(void)
{
	::InitializeCriticalSection(&m_lock);

	Init();
}

CNtlLogSystem::~CNtlLogSystem(void)
{
	Destroy();

	::DeleteCriticalSection(&m_lock);
}

bool CNtlLogSystem::Create()
{
	return true;
}

void CNtlLogSystem::Destroy()
{
	::EnterCriticalSection(&m_lock);

	::ZeroMemory(m_szLogPath, sizeof(m_szLogPath));

	std::map<DWORD, sSOURCE_INFO*>::iterator iterSource;
	for (iterSource = m_mapSource.begin() ; m_mapSource.end() != iterSource ; iterSource++)
	{
		sSOURCE_INFO* pSourceInfo = iterSource->second;

		if (NULL != pSourceInfo)
		{
			std::map<BYTE, sCHANNEL_INFO*>::iterator iterChannel;
			for (iterChannel = (pSourceInfo->mapChannel).begin() ; (pSourceInfo->mapChannel).end() != iterChannel ; iterChannel++)
			{
				sCHANNEL_INFO* pChannelInfo = iterChannel->second;
				SAFE_DELETE(pChannelInfo);
			}
			(pSourceInfo->mapChannel).clear();

			std::map<CNtlString, sLOG_FILE_INFO*>::iterator iterLogFile;
			for (iterLogFile = (pSourceInfo->mapLoggingFileInfo).begin() ; (pSourceInfo->mapLoggingFileInfo).end() != iterLogFile ; iterLogFile++)
			{
				sLOG_FILE_INFO* pLogFileInfo = iterLogFile->second;

				// Waits until the current logging process ends.
				::EnterCriticalSection(&(pLogFileInfo->lock));
				::LeaveCriticalSection(&(pLogFileInfo->lock));

				CloseLogFile(pLogFileInfo);

				SAFE_DELETE(pLogFileInfo);
			}
			(pSourceInfo->mapLoggingFileInfo).clear();
		}
	}
	m_mapSource.clear();

	m_mapLogFileRef.clear();

	::LeaveCriticalSection(&m_lock);
}

bool CNtlLogSystem::Init()
{
	m_bIsEnabled = true;

	m_szLogPath[0] = '.';
	m_szLogPath[1] = '\0';

	m_mapSource.clear();
	m_mapLogFileRef.clear();

	return true;
}

bool CNtlLogSystem::SetLogPath(char* pszLogPath)
{
	::EnterCriticalSection(&m_lock);

	if (NULL == pszLogPath)
	{
		m_szLogPath[0] = '.';
		m_szLogPath[1] = '\0';
	}
	else
	{
		char szLogPathRefined[MAX_LOG_PATH_NAME_LENGTH + 1];

		strncpy_s<_countof(szLogPathRefined)>(szLogPathRefined, pszLogPath, MAX_LOG_PATH_NAME_LENGTH);
		szLogPathRefined[MAX_LOG_PATH_NAME_LENGTH] = '\0';

		size_t logPathLength = strlen(szLogPathRefined);

		// Eliminates back-slash character.
		if ('\\' == szLogPathRefined[logPathLength - 1])
		{
			szLogPathRefined[logPathLength - 1] = '\0';
		}

		if (false == MakeSurePathIsValid(szLogPathRefined))
		{
			return false;
		}

		strncpy_s<_countof(m_szLogPath)>(m_szLogPath, szLogPathRefined, MAX_LOG_PATH_NAME_LENGTH);
		m_szLogPath[MAX_LOG_PATH_NAME_LENGTH] = '\0';
	}

	RefreshLogFileFullName();

	::LeaveCriticalSection(&m_lock);
	return true;
}

bool CNtlLogSystem::RegisterSource(DWORD dwSource, char* pszSourceName)
{
	if (NULL == pszSourceName)
	{
		return false;
	}

	::EnterCriticalSection(&m_lock);

	std::map<DWORD, sSOURCE_INFO*>::iterator iter;
	iter = m_mapSource.find(dwSource);

	if (m_mapSource.end() != iter)
	{
		::LeaveCriticalSection(&m_lock);
		return false;
	}

	sSOURCE_INFO* pSourceInfo = new sSOURCE_INFO;
	if (NULL == pSourceInfo)
	{
		::LeaveCriticalSection(&m_lock);
		return false;
	}

	NTL_SAFE_STRCPY(pSourceInfo->szSourceName, pszSourceName);

	bool bResult = m_mapSource.insert(std::pair<DWORD, sSOURCE_INFO*>(dwSource, pSourceInfo)).second;

	::LeaveCriticalSection(&m_lock);

	if (false == bResult)
	{
		SAFE_DELETE(pSourceInfo);
	}

	return bResult;
}

bool CNtlLogSystem::RegisterSource(DWORD dwSource, CNtlString strSourceName)
{
	return RegisterSource(dwSource, (char*)(strSourceName.c_str()));
}

bool CNtlLogSystem::RegisterChannel(
						DWORD dwSource, BYTE byChannel, char* pszChannelName,
						char* pszLogFileNamePrefix, char* pszLogFileNameSuffix, char* pszLogFileExtName, bool bIsOn)
{
	if (NULL == pszChannelName || NULL == pszLogFileNamePrefix || NULL == pszLogFileNameSuffix || NULL == pszLogFileExtName)
	{
		return false;
	}

	::EnterCriticalSection(&m_lock);

	sSOURCE_INFO* pSourceInfo = GetSourceInfo(dwSource);
	if (NULL == pSourceInfo)
	{
		::LeaveCriticalSection(&m_lock);
		return false;
	}

	sCHANNEL_INFO* pChannelInfo = GetChannelInfo(pSourceInfo, byChannel);
	if (NULL != pChannelInfo)
	{
		::LeaveCriticalSection(&m_lock);
		return false;
	}

	// Creates channel info.
	{
		pChannelInfo = new sCHANNEL_INFO(bIsOn);
		if (NULL == pChannelInfo)
		{
			::LeaveCriticalSection(&m_lock);
			return false;
		}

		NTL_SAFE_STRCPY(pChannelInfo->szChannelName, pszChannelName);

		if (false == (pSourceInfo->mapChannel).insert(std::pair<BYTE, sCHANNEL_INFO*>(byChannel, pChannelInfo)).second)
		{
			SAFE_DELETE(pChannelInfo);

			::LeaveCriticalSection(&m_lock);
			return false;
		}
	}

	sLOG_FILE_INFO* pLogFileInfo = NULL;

	// Creates log file info.
	{
		CNtlString strLogFileKey;
		strLogFileKey.Format("%s_%s", pszLogFileNamePrefix, pszLogFileNameSuffix);

		std::map<CNtlString, sLOG_FILE_INFO*>::iterator iterLogFile;
		iterLogFile = (pSourceInfo->mapLoggingFileInfo).find(strLogFileKey);

		if ((pSourceInfo->mapLoggingFileInfo).end() == iterLogFile)
		{
			pLogFileInfo = new sLOG_FILE_INFO;
			if (NULL == pLogFileInfo)
			{
				(pSourceInfo->mapChannel).erase(byChannel);
				SAFE_DELETE(pChannelInfo);

				::LeaveCriticalSection(&m_lock);
				return false;
			}

			NTL_SAFE_STRCPY(pLogFileInfo->szLogFileNamePrefix, pszLogFileNamePrefix);
			NTL_SAFE_STRCPY(pLogFileInfo->szLogFileNameSuffix, pszLogFileNameSuffix);
			NTL_SAFE_STRCPY(pLogFileInfo->szLogFileExtName, pszLogFileExtName);

			if (false == (pSourceInfo->mapLoggingFileInfo).insert(std::pair<CNtlString, sLOG_FILE_INFO*>(strLogFileKey, pLogFileInfo)).second)
			{
				SAFE_DELETE(pLogFileInfo);

				(pSourceInfo->mapChannel).erase(byChannel);
				SAFE_DELETE(pChannelInfo);

				::LeaveCriticalSection(&m_lock);
				return false;
			}

			if (false == OpenLogFile(pLogFileInfo))
			{
				(pSourceInfo->mapLoggingFileInfo).erase(strLogFileKey);
				SAFE_DELETE(pLogFileInfo);

				(pSourceInfo->mapChannel).erase(byChannel);
				SAFE_DELETE(pChannelInfo);

				::LeaveCriticalSection(&m_lock);
				return false;
			}

			::EnterCriticalSection(&(pLogFileInfo->lock));

			pLogFileInfo->dwRefCount = 1;
		}
		else
		{
			pLogFileInfo = iterLogFile->second;

			::EnterCriticalSection(&(pLogFileInfo->lock));

			(pLogFileInfo->dwRefCount)++;
		}

		sLOG_FILE_KEY logKey(dwSource, byChannel);

		if (false == m_mapLogFileRef.insert(std::pair<sLOG_FILE_KEY, sLOG_FILE_INFO*>(logKey, pLogFileInfo)).second)
		{
			(pLogFileInfo->dwRefCount)--;

			if (0 == pLogFileInfo->dwRefCount)
			{
				::LeaveCriticalSection(&(pLogFileInfo->lock));

				(pSourceInfo->mapLoggingFileInfo).erase(strLogFileKey);
				SAFE_DELETE(pLogFileInfo);
			}
			else
			{
				::LeaveCriticalSection(&(pLogFileInfo->lock));
			}

			(pSourceInfo->mapChannel).erase(byChannel);
			SAFE_DELETE(pChannelInfo);

			::LeaveCriticalSection(&m_lock);
			return false;
		}
		else
		{
			::LeaveCriticalSection(&(pLogFileInfo->lock));
		}
	}

	::LeaveCriticalSection(&m_lock);
	return true;
}

bool CNtlLogSystem::RegisterChannel(
							DWORD dwSource, BYTE byChannel, CNtlString strChannelName,
							CNtlString strLogFileNamePrefix, CNtlString strLogFileNameSuffix, CNtlString strLogFileExtName, bool bIsOn)
{
	if (0 == strLogFileExtName.GetString().length())
	{
		return RegisterChannel(dwSource, byChannel, (char*)(strChannelName.c_str()), (char*)(strLogFileNamePrefix.c_str()), (char*)(strLogFileNameSuffix.c_str()), "csv", bIsOn);
	}
	else
	{
		return RegisterChannel(dwSource, byChannel, (char*)(strChannelName.c_str()), (char*)(strLogFileNamePrefix.c_str()), (char*)(strLogFileNameSuffix.c_str()), (char*)(strLogFileExtName.c_str()), bIsOn);
	}
}

bool CNtlLogSystem::TurnOnChannel(DWORD dwSource, BYTE byChannel)
{
	::EnterCriticalSection(&m_lock);

	sSOURCE_INFO* pSourceInfo = GetSourceInfo(dwSource);
	if (NULL == pSourceInfo)
	{
		::LeaveCriticalSection(&m_lock);
		return false;
	}

	sCHANNEL_INFO* pChannelInfo = GetChannelInfo(pSourceInfo, byChannel);
	if (NULL == pChannelInfo)
	{
		::LeaveCriticalSection(&m_lock);
		return false;
	}

	pChannelInfo->bIsOn = true;

	::LeaveCriticalSection(&m_lock);
	return true;
}

bool CNtlLogSystem::TurnOffChannel(DWORD dwSource, BYTE byChannel)
{
	::EnterCriticalSection(&m_lock);

	sSOURCE_INFO* pSourceInfo = GetSourceInfo(dwSource);
	if (NULL == pSourceInfo)
	{
		::LeaveCriticalSection(&m_lock);
		return false;
	}

	sCHANNEL_INFO* pChannelInfo = GetChannelInfo(pSourceInfo, byChannel);
	if (NULL == pChannelInfo)
	{
		::LeaveCriticalSection(&m_lock);
		return false;
	}

	pChannelInfo->bIsOn = false;

	::LeaveCriticalSection(&m_lock);
	return true;
}

bool CNtlLogSystem::IsChannelOn(DWORD dwSource, BYTE byChannel)
{
	::EnterCriticalSection(&m_lock);

	sSOURCE_INFO* pSourceInfo = GetSourceInfo(dwSource);
	if (NULL == pSourceInfo)
	{
		::LeaveCriticalSection(&m_lock);
		return false;
	}

	sCHANNEL_INFO* pChannelInfo = GetChannelInfo(pSourceInfo, byChannel);
	if (NULL == pChannelInfo)
	{
		::LeaveCriticalSection(&m_lock);
		return false;
	}

	bool bIsOn = pChannelInfo->bIsOn;

	::LeaveCriticalSection(&m_lock);
	return bIsOn;
}

bool CNtlLogSystem::AddLog(DWORD dwSource, BYTE byChannel, const char* pszFormatString, ...)
{
	bool bResult = false;
	va_list args;

	va_start(args, pszFormatString);
	bResult = AddLogAlternative(dwSource, byChannel, (char*)pszFormatString, args);
	va_end(args);

	return bResult;
}

bool CNtlLogSystem::AddLog(DWORD dwSource, BYTE byChannel, char* pszFormatString, ...)
{
	bool bResult = false;
	va_list args;

	va_start(args, pszFormatString);
	bResult = AddLogAlternative(dwSource, byChannel, pszFormatString, args);
	va_end(args);

	return bResult;
}

bool CNtlLogSystem::AddLogAlternative(DWORD dwSource, BYTE byChannel, char* pszFormatString, va_list args)
{
	if (NULL == pszFormatString)
	{
		return false;
	}

	::EnterCriticalSection(&m_lock);

	if (false == m_bIsEnabled)
	{
		::LeaveCriticalSection(&m_lock);
		return true;
	}

	sSOURCE_INFO* pSourceInfo = GetSourceInfo(dwSource);
	if (NULL == pSourceInfo)
	{
		::LeaveCriticalSection(&m_lock);
		return false;
	}

	sCHANNEL_INFO* pChannelInfo = GetChannelInfo(pSourceInfo, byChannel);
	if (NULL == pChannelInfo)
	{
		::LeaveCriticalSection(&m_lock);
		return false;
	}

	if (false == pChannelInfo->bIsOn)
	{
		::LeaveCriticalSection(&m_lock);
		return true;
	}

	sLOG_FILE_KEY key(dwSource, byChannel);

	std::map<sLOG_FILE_KEY, sLOG_FILE_INFO*>::iterator iter;
	iter = m_mapLogFileRef.find(key);

	if (m_mapLogFileRef.end() == iter)
	{
		::LeaveCriticalSection(&m_lock);
		return false;
	}

	sLOG_FILE_INFO* pLogFileInfo = iter->second;

	::LeaveCriticalSection(&m_lock);

	::EnterCriticalSection(&(pLogFileInfo->lock));

	SYSTEMTIME localTime;
	GetLocalTime(&localTime);

	if (pLogFileInfo->wLastLogYear != localTime.wYear ||
		pLogFileInfo->wLastLogMonth != localTime.wMonth ||
		pLogFileInfo->wLastLogDay != localTime.wDay)
	{
		CloseLogFile(pLogFileInfo);
		OpenLogFile(pLogFileInfo);
	}

	fprintf( pLogFileInfo->fileStream.GetFilePtr(), "[%s],", pChannelInfo->szChannelName);
	vfprintf( pLogFileInfo->fileStream.GetFilePtr(), pszFormatString, args);
	fprintf( pLogFileInfo->fileStream.GetFilePtr(), "\n");
	fflush( pLogFileInfo->fileStream.GetFilePtr() );

	::LeaveCriticalSection(&(pLogFileInfo->lock));
	return true;
}

bool CNtlLogSystem::UnitTest()
{
	enum eLogSource
	{
		NTL_LOG_SOURCE_GAME_SERVER = 0,
		NTL_LOG_SOURCE_NPC_SERVER
	};

	enum eLogChannel
	{
		NTL_LOG_CHANNEL_SYSTEM = 0,
		NTL_LOG_CHANNEL_ASSERT,
		NTL_LOG_CHANNEL_DEBUG,

		NTL_LOG_CHANNEL_ITEM_TRADE,
		NTL_LOG_CHANNEL_NPC_AI,

		NTL_LOG_CHANNEL_COUNT,

		NTL_LOG_CHANNEL_FIRST = NTL_LOG_CHANNEL_SYSTEM,
		NTL_LOG_CHANNEL_LAST = NTL_LOG_CHANNEL_ITEM_TRADE
	};

	CNtlLogSystem log;
	log.Create();

	log.RegisterSource(NTL_LOG_SOURCE_GAME_SERVER, "GAME_SERVER");
	log.RegisterSource(NTL_LOG_SOURCE_NPC_SERVER, "NPC_SERVER");

	// Both of char* type and CNtlString type can be used to specify a log file name and a log file extension.

	// Creates "SYSTEM" channel.(Basically, a channel is turned on when it's created.)
	log.RegisterChannel(NTL_LOG_SOURCE_GAME_SERVER, NTL_LOG_CHANNEL_SYSTEM, "SYSTEM", "GAME_SERVER", "LOG");
	// Creates "ITEM_TRADE" channel.
	log.RegisterChannel(NTL_LOG_SOURCE_GAME_SERVER, NTL_LOG_CHANNEL_ITEM_TRADE, "ITEM_TRADE", "GAME_SERVER", "LOG");
	// Creates "SYSTEM" channel and turns off it.
	log.RegisterChannel(NTL_LOG_SOURCE_NPC_SERVER, NTL_LOG_CHANNEL_SYSTEM, CNtlString("SYSTEM"), CNtlString("NPC_SERVER"), CNtlString("LOG"), CNtlString("txt"));
	// Creates "NPC_AI" channel and turns off it.
	log.RegisterChannel(NTL_LOG_SOURCE_NPC_SERVER, NTL_LOG_CHANNEL_NPC_AI, CNtlString("NPC_AI"), CNtlString("NPC_SERVER"), CNtlString("LOG"), CNtlString("txt"), false);

	DWORD dwBegin = ::GetTickCount();
	DWORD dwLast = dwBegin;
	int nCount = 0;

	bool bLoop = true;
	while (bLoop == true)
	{
		DWORD dwNow = ::GetTickCount();

		// Updates log file once per 100 milliseconds.
		if (dwNow - dwLast >= 100)
		{
			log.AddLog(NTL_LOG_SOURCE_GAME_SERVER, NTL_LOG_CHANNEL_SYSTEM, "%s(STATEMACHINE : %d)", "Some messages here.", 1354);
			log.AddLog(NTL_LOG_SOURCE_GAME_SERVER, NTL_LOG_CHANNEL_ITEM_TRADE, "Item traded!");

			log.AddLog(NTL_LOG_SOURCE_NPC_SERVER, NTL_LOG_CHANNEL_SYSTEM, "%s(STATEMACHINE : %d)", "Some messages here.", 1354);
			log.AddLog(NTL_LOG_SOURCE_NPC_SERVER, NTL_LOG_CHANNEL_NPC_AI, "Npc moved by AI logic!");

			dwLast = dwNow;
			nCount++;

			if (0 == nCount % 3)
			{
				// Turns on ITEM_TRADE channel if it's turned off, and turn off it if it's turned on.
				if (false == log.IsChannelOn(NTL_LOG_SOURCE_GAME_SERVER, NTL_LOG_CHANNEL_ITEM_TRADE))
				{
					log.TurnOnChannel(NTL_LOG_SOURCE_GAME_SERVER, NTL_LOG_CHANNEL_ITEM_TRADE);
					log.AddLog(NTL_LOG_SOURCE_GAME_SERVER, NTL_LOG_CHANNEL_SYSTEM, "Turned on \"ITEM_TRADE\" channel.");
					log.AddLog(NTL_LOG_SOURCE_NPC_SERVER, NTL_LOG_CHANNEL_SYSTEM, "Turned on \"ITEM_TRADE\" channel.");
				}
				else
				{
					log.TurnOffChannel(NTL_LOG_SOURCE_GAME_SERVER, NTL_LOG_CHANNEL_ITEM_TRADE);
					log.AddLog(NTL_LOG_SOURCE_GAME_SERVER, NTL_LOG_CHANNEL_SYSTEM, "Turned off \"ITEM_TRADE\" channel.");
					log.AddLog(NTL_LOG_SOURCE_NPC_SERVER, NTL_LOG_CHANNEL_SYSTEM, "Turned off \"ITEM_TRADE\" channel.");
				}

				// Turns on NPC_AI channel if it's turned off, and turn off it if it's turned on.
				if (false == log.IsChannelOn(NTL_LOG_SOURCE_NPC_SERVER, NTL_LOG_CHANNEL_NPC_AI))
				{
					log.TurnOnChannel(NTL_LOG_SOURCE_NPC_SERVER, NTL_LOG_CHANNEL_NPC_AI);
					log.AddLog(NTL_LOG_SOURCE_NPC_SERVER, NTL_LOG_CHANNEL_SYSTEM, "Turned on \"NPC_AI\" channel.");
					log.AddLog(NTL_LOG_SOURCE_GAME_SERVER, NTL_LOG_CHANNEL_SYSTEM, "Turned on \"NPC_AI\" channel.");
				}
				else
				{
					log.TurnOffChannel(NTL_LOG_SOURCE_NPC_SERVER, NTL_LOG_CHANNEL_NPC_AI);
					log.AddLog(NTL_LOG_SOURCE_NPC_SERVER, NTL_LOG_CHANNEL_SYSTEM, "Turned off \"NPC_AI\" channel.");
					log.AddLog(NTL_LOG_SOURCE_GAME_SERVER, NTL_LOG_CHANNEL_SYSTEM, "Turned off \"NPC_AI\" channel.");
				}
			}
		}

		if (30 <= nCount)
		{
			bLoop = false;
		}
	}

	return true;
}

CNtlLogSystem::sSOURCE_INFO* CNtlLogSystem::GetSourceInfo(DWORD dwSource)
{
	std::map<DWORD, sSOURCE_INFO*>::iterator iter;
	iter = m_mapSource.find(dwSource);

	if (m_mapSource.end() == iter)
	{
		return NULL;
	}

	return iter->second;
}

CNtlLogSystem::sCHANNEL_INFO* CNtlLogSystem::GetChannelInfo(sSOURCE_INFO* pSourceInfo, BYTE byChannel)
{
	if (NULL == pSourceInfo)
	{
		return NULL;
	}

	std::map<BYTE, sCHANNEL_INFO*>::iterator iter;
	iter = (pSourceInfo->mapChannel).find(byChannel);

	if ((pSourceInfo->mapChannel).end() == iter)
	{
		return NULL;
	}

	return iter->second;
}

CNtlLogSystem::sLOG_FILE_INFO* CNtlLogSystem::GetLogFileInfo(DWORD dwSource, BYTE byChannel)
{
	sLOG_FILE_KEY key(dwSource, byChannel);

	std::map<sLOG_FILE_KEY, sLOG_FILE_INFO*>::iterator iter;
	iter = m_mapLogFileRef.find(key);

	if (m_mapLogFileRef.end() == iter)
	{
		return NULL;
	}

	return iter->second;
}

void CNtlLogSystem::RefreshLogFileFullName()
{
	sSOURCE_INFO* pSourceInfo = NULL;
	sLOG_FILE_INFO* pLogFileInfo = NULL;

	std::map<DWORD, sSOURCE_INFO*>::iterator iterSource;
	for (iterSource = m_mapSource.begin() ; m_mapSource.end() != iterSource ; iterSource++)
	{
		pSourceInfo = iterSource->second;

		if (NULL != pSourceInfo)
		{
			std::map<CNtlString, sLOG_FILE_INFO*>::iterator iterLogFile;
			for (iterLogFile = (pSourceInfo->mapLoggingFileInfo).begin() ; (pSourceInfo->mapLoggingFileInfo).end() != iterLogFile ; iterLogFile++)
			{
				pLogFileInfo = iterLogFile->second;

				if (NULL != pLogFileInfo)
				{
					::EnterCriticalSection(&(pLogFileInfo->lock));

					CloseLogFile(pLogFileInfo);
					OpenLogFile(pLogFileInfo);

					::LeaveCriticalSection(&(pLogFileInfo->lock));
				}
			}
		}
	}
}

bool CNtlLogSystem::OpenLogFile(sLOG_FILE_INFO* pLogFileInfo)
{
	if (NULL == pLogFileInfo)
	{
		return false;
	}

	if ( true == pLogFileInfo->fileStream.IsOpened() )
	{
		return false;
	}

	SYSTEMTIME localTime;
	GetLocalTime(&localTime);

	char szLogFilePathFinal[MAX_PATH_FULL_NAME + 1] = { 0x00, };
	sprintf_s<sizeof(szLogFilePathFinal)>(
											szLogFilePathFinal,
											"%s\\%04d%02d%02d",
											m_szLogPath,
											localTime.wYear,
											localTime.wMonth,	
											localTime.wDay);

	if (false == MakeSurePathIsValid(szLogFilePathFinal))
	{
		return false;
	}

	sprintf_s<_countof(pLogFileInfo->szLogFileFullName)>(
													pLogFileInfo->szLogFileFullName,
													"%s\\%04d%02d%02d_%s_%s.%s",
													szLogFilePathFinal,
													localTime.wYear,
													localTime.wMonth,
													localTime.wDay,
													pLogFileInfo->szLogFileNamePrefix,
													pLogFileInfo->szLogFileNameSuffix,
													pLogFileInfo->szLogFileExtName);

	int rc = pLogFileInfo->fileStream.Create( pLogFileInfo->szLogFileFullName );
	if( NTL_SUCCESS != rc )
	{
		return false;
	}

	pLogFileInfo->wLastLogYear = localTime.wYear;
	pLogFileInfo->wLastLogMonth = localTime.wMonth;
	pLogFileInfo->wLastLogDay = localTime.wDay;

	return true;
}

bool CNtlLogSystem::CloseLogFile(sLOG_FILE_INFO* pLogFileInfo)
{
	if (NULL == pLogFileInfo)
	{
		return false;
	}
	if (false == pLogFileInfo->fileStream.IsOpened() )
	{
		return false;
	}

	fflush( pLogFileInfo->fileStream.GetFilePtr() );

	pLogFileInfo->fileStream.Close();


	return true;
}

bool CNtlLogSystem::MakeSurePathIsValid(char* pszLogFilePath)
{
	int nResult = _mkdir(pszLogFilePath);
	if (0 == nResult)
	{
		// Not a problem.
	}
	else
	{
		if (EEXIST == errno)
		{
			// Not a problem at all.
		}
		else if (ENOENT == errno)
		{
			// Path was not found.
			return false;
		}
		else
		{
			return false;
		}
	}

	return true;
}