#pragma once

#include "NtlString.h"
#include "NtlFile.h"

#include <stdio.h>
#include <map>

class CNtlLogSystem
{
public:
	enum eMaxLength
	{
		MAX_SOURCE_NAME = 32,
		MAX_CHANNEL_NAME = 32,

		MAX_LOG_PATH_NAME_LENGTH = 64,
		MAX_DATE_NAME_LENGTH = 8,
		MAX_FILE_PREFIX_LENGTH = 32,
		MAX_FILE_SUFFIX_LENGTH = 32,
		MAX_FILE_EXT_LENGTH = 8,

		// ex) C:\Work\20070731\20070731_GAMESERVER_ASSERT.csv
		// MAX_LOG_PATH_NAME_LENGTH -> C:\Work
		// MAX_DATA_NAME_LENGTH -> 20070731
		// MAX_FILE_PREFIX_LENGTH -> GAMESERVER
		// MAX_FILE_SUFFIX_LENGTH -> ASSERT
		// MAX_FILE_EXT_LENGTH -> csv

		// "C:\Work" + "\" + "20070731"
		MAX_PATH_FULL_NAME = MAX_LOG_PATH_NAME_LENGTH + 1 + MAX_DATE_NAME_LENGTH,
		// "C:\Work" + "\" + "20070731" + "\" + "20070731" + "_" + "Name" + "." + "Ext"
		MAX_FILE_FULL_NAME = MAX_LOG_PATH_NAME_LENGTH + 1 + MAX_DATE_NAME_LENGTH + 1 + MAX_DATE_NAME_LENGTH + 1 + MAX_FILE_PREFIX_LENGTH + 1 + MAX_FILE_SUFFIX_LENGTH + 1 + MAX_FILE_EXT_LENGTH
	};

private:
	struct sLOG_FILE_KEY
	{
		sLOG_FILE_KEY(DWORD dwInitialSource, BYTE byInitialChannel) :
				dwSource(dwInitialSource),
				byChannel(byInitialChannel)
		{
		}

		bool operator<(const sLOG_FILE_KEY& rLogFileKey) const
		{
			if (dwSource < rLogFileKey.dwSource)
			{
				return true;
			}
			if (dwSource > rLogFileKey.dwSource)
			{
				return false;
			}
			if (byChannel < rLogFileKey.byChannel)
			{
				return true;
			}

			return false;
		};

		DWORD dwSource;
		BYTE byChannel;
	};

	struct sLOG_FILE_INFO
	{
		sLOG_FILE_INFO() :
				wLastLogYear(0),
				wLastLogMonth(0),
				wLastLogDay(0),
				dwRefCount(0)
		{
			::InitializeCriticalSection(&lock);

			::ZeroMemory(szLogFileFullName, sizeof(szLogFileFullName));
			::ZeroMemory(szLogFileNamePrefix, sizeof(szLogFileNamePrefix));
			::ZeroMemory(szLogFileNameSuffix, sizeof(szLogFileNameSuffix));
			::ZeroMemory(szLogFileExtName, sizeof(szLogFileExtName));
		}

		~sLOG_FILE_INFO()
		{
			::DeleteCriticalSection(&lock);
		}

		CRITICAL_SECTION lock;

		WORD wLastLogYear;
		WORD wLastLogMonth;
		WORD wLastLogDay;

		char szLogFileFullName[MAX_FILE_FULL_NAME + 1];
		char szLogFileNamePrefix[MAX_FILE_PREFIX_LENGTH + 1];
		char szLogFileNameSuffix[MAX_FILE_SUFFIX_LENGTH + 1];
		char szLogFileExtName[MAX_FILE_EXT_LENGTH  + 1];

		CNtlFileStream fileStream;

		DWORD dwRefCount;
	};

	struct sCHANNEL_INFO
	{
		sCHANNEL_INFO(bool bIsOnWhenStarting) :
				bIsOn(bIsOnWhenStarting)
		{
			::ZeroMemory(szChannelName, sizeof(szChannelName));
		}

		bool bIsOn;

		char szChannelName[MAX_CHANNEL_NAME + 1];
	};

	struct sSOURCE_INFO
	{
		sSOURCE_INFO()
		{
			::ZeroMemory(szSourceName, sizeof(szSourceName));
		}

		char szSourceName[MAX_SOURCE_NAME + 1];

		std::map<BYTE, sCHANNEL_INFO*> mapChannel;
		std::map<CNtlString, sLOG_FILE_INFO*> mapLoggingFileInfo;
	};

public:
	CNtlLogSystem();
	virtual ~CNtlLogSystem();

	bool Create();
	void Destroy();

protected:
	bool Init();

public:
	void SetEnabled() { m_bIsEnabled = true; }
	void SetDisabled() { m_bIsEnabled = false; }
	bool IsEnabled() { return m_bIsEnabled; }

	// "PszLogPath" can be a NULL.
	// "PszLogPath" can be NULL.
	// In this case, the log file directory is created in the default working directory.
	// In this case, log file directories will be created in the default work directory.
	bool SetLogPath(char* pszLogPath);

	bool RegisterSource(DWORD dwSource, char* pszSourceName);
	bool RegisterSource(DWORD dwSource, CNtlString strSourceName);

	bool RegisterChannel(
				DWORD dwSource, BYTE byChannel, char* pszChannelName,
				char* pszLogFileNamePrefix, char* pszLogFileNameSuffix, char* pszLogFileExtName = "txt", bool bIsOn = true);
	bool RegisterChannel(
				DWORD dwSource, BYTE byChannel, CNtlString strChannelName,
				CNtlString strLogFileNamePrefix, CNtlString strLogFileNameSuffix, CNtlString strLogFileExtName, bool bIsOn = true);

	bool TurnOnChannel(DWORD dwSource, BYTE byChannel);
	bool TurnOffChannel(DWORD dwSource, BYTE byChannel);

	bool IsChannelOn(DWORD dwSource, BYTE byChannel);

	bool AddLog(DWORD dwSource, BYTE byChannel, const char* pszFormatString, ...);
	bool AddLog(DWORD dwSource, BYTE byChannel, char* pszFormatString, ...);
	bool AddLogAlternative(DWORD dwSource, BYTE byChannel, char* pszFormatString, va_list args);

public:
	static bool UnitTest();

protected:
	sSOURCE_INFO* GetSourceInfo(DWORD dwSource);
	sCHANNEL_INFO* GetChannelInfo(sSOURCE_INFO* pSourceInfo, BYTE byChannel);
	sLOG_FILE_INFO* GetLogFileInfo(DWORD dwSource, BYTE byChannel);

	void RefreshLogFileFullName();

	bool OpenLogFile(sLOG_FILE_INFO* pLogFileInfo);
	bool CloseLogFile(sLOG_FILE_INFO* pLogFileInfo);

	bool MakeSurePathIsValid(char* pszLogFilePath);

private:
	const static char* m_pszShortMonthName[];

private:
	bool m_bIsEnabled;

	char m_szLogPath[MAX_LOG_PATH_NAME_LENGTH + 1];

	std::map<DWORD, sSOURCE_INFO*> m_mapSource;
	std::map<sLOG_FILE_KEY, sLOG_FILE_INFO*> m_mapLogFileRef;

	CRITICAL_SECTION m_lock;
};