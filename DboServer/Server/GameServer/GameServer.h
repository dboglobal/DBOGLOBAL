#pragma once

#include "NtlSfx.h"
#include "NtlSharedDef.h"
#include "NtlServer.h"

#include "GameSessionFactory.h"


class CChatServerSession;
class CMasterServerSession;
class CQueryServerSession;
class CGameProcessor;
class CGameMain;
class CGameData;
class CActionPatternSystem;


class CGameServer : public CNtlServerApp
{

public:

	CGameServer();
	virtual ~CGameServer();

private:

	DWORD			m_dwCurTickCount;
	DWORD			m_dwLastTimePerformanceLogged;
	DWORD			m_dwLastTimeLoadReported;
	DWORD			m_dwLastTimeMemoryUseLogged;

	time_t			m_tmCurrentTime;

public:

	virtual int		OnInitApp();
	virtual int		OnAppStart();
	virtual int		OnCreate();

	virtual int		OnConfiguration(const char * lpszConfigFile);
	virtual BOOL	OnCommandInput(std::string& sCmd);

	virtual void	Run();

private:

	void			Init();
	void			Destroy();

private:

	void			DoUpdatePerformanceLog(DWORD dwNow);
	void			DoReportLoad(DWORD dwNow);
	void			DoUpdateMemoryUseLog(DWORD dwNow);

public:

	inline DWORD			GetCurTickCount() { return m_dwCurTickCount; }
	inline time_t			GetTime()			{ return m_tmCurrentTime; }

	BOOL			IsTestServer()					{ return m_config.bTestServer; }

	CNtlString		GetConfigChatServerIP()			{ return m_config.strChatServerIP; }
	WORD			GetConfigChatServerPort()		{ return m_config.wChatServerPort; }
	CNtlString		GetConfigCharServerIP()			{ return m_config.strCharServerIP; }
	WORD			GetConfigCharServerPort()		{ return m_config.wCharServerPort; }

	CNtlString		GetLogPath()					{ return m_config.strLogPath; }
	CNtlString		GetTsPath()						{ return m_config.strTsPath; }

	SERVERCHANNELID	GetGsChannel()	{ return m_config.byChannel; }
	SERVERFARMID	GetGsServerId()	{ return m_config.byServerID; }

	BYTE			GetPlayerMaxLevel()		{ return m_config.MaxLevel; }
	int				GetSoloExpRate()		{ return m_config.SoloExpRate; }
	int				GetPartyExpRate()		{ return m_config.PartyExpRate; }
	int				GetItemDropRate()		{ return m_config.ItemDropRate; }
	int				GetZeniDropRate()		{ return m_config.ZeniDropRate; }
	int				GetZeniBonusRate()		{ return m_config.ZeniBonusRate; }
	int				GetZeniPartyBonusRate() { return m_config.ZeniPartyBonusRate; }
	int				GetQuestMoneyRate()		{ return m_config.QuestMoneyRate; }
	int				GetQuestExpRate()		{ return m_config.QuestExpRate; }

	CNtlString		GetDatabaseHost()	{	return m_config.DatabaseHost;	}
	CNtlString		GetDatabaseUser()	{	return m_config.DatabaseUser;	}
	CNtlString		GetDatabasePassword()	{	return m_config.DatabasePassword;	}
	CNtlString		GetDatabaseDatabase()	{	return m_config.Database;	}
	CNtlString		GetAccDbHost()	{ return m_config.AccDatabaseHost; }
	CNtlString		GetAccDbUser()	{ return m_config.AccDatabaseUser; }
	CNtlString		GetAccDbPass()	{ return m_config.AccDatabasePassword; }
	CNtlString		GetAccDbDatabase() { return m_config.AccDatabase; }

	inline bool		IsDojoChannel() { return m_config.byChannel == DOJO_CHANNEL_INDEX; }


	CActionPatternSystem*			GetActionPatternSystem() { return m_pActionPatternSystem; }
	CGameData*						GetGameData() { return m_pGameData; }
	CGameMain*						GetGameMain() { return m_pGameMain; }
	CGameProcessor*					GetGameProcessor() { return m_pGameProcessor; }

	CNtlLog*						GetLog() { return &m_log; }



public:
	sGAME_SERVERCONFIG			m_config;

private:

	CNtlLog  					m_log;

private:

	CChatServerSession *		m_pChatServerSession;
	CMasterServerSession *		m_pMasterServerSession;
	CQueryServerSession *		m_pQueryServerSession;

private:

	CNtlConnector				m_serverChatConnector;
	CNtlConnector				m_serverQueryConnector;
	CNtlConnector				m_serverMasterConnector;
	CNtlAcceptor				m_clientAcceptor;

private:

	CGameProcessor*				m_pGameProcessor;
	CGameMain *					m_pGameMain;
	CGameData *					m_pGameData;
	CActionPatternSystem *		m_pActionPatternSystem;

public:


	CChatServerSession* GetChatServerSession() { return m_pChatServerSession; }
	void SetChatServerSession(CChatServerSession * pServerSession)
	{
		m_pChatServerSession = pServerSession;
	}

	CQueryServerSession* GetQueryServerSession() { return m_pQueryServerSession; }
	void SetQueryServerSession(CQueryServerSession * pServerSession)
	{
		m_pQueryServerSession = pServerSession;
	}

	CNtlConnector*		GetMasterServerConnector() { return &m_serverMasterConnector; }

	CMasterServerSession* GetMasterServerSession() { return m_pMasterServerSession; }
	void SetMasterServerSession(CMasterServerSession * pServerSession)
	{
		m_pMasterServerSession = pServerSession;
	}

};

