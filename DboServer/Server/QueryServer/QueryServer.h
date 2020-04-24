#pragma once

#include "NtlSfx.h"

#include "Servermanager.h"
#include "SessionFactory.h"
#include "NtlSharedType.h"

#include "DatabaseEnv.h"


struct sSERVERCONFIG
{
	CNtlString		strCharServerAcceptAddr;
	WORD			wCharServerAcceptPort;

	CNtlString		strChatServerAcceptAddr;
	WORD			wChatServerAcceptPort;

	CNtlString		strGameServerAcceptAddr;
	WORD			wGameServerAcceptPort;

	CNtlString		DatabaseHost;
	CNtlString		DatabaseUser;
	CNtlString		DatabasePassword;
	CNtlString		Database;

	CNtlString		AccDatabaseHost;
	CNtlString		AccDatabaseUser;
	CNtlString		AccDatabasePassword;
	CNtlString		AccDatabase;

	CNtlString		strMasterServerIP;
	WORD			wMasterServerPort;
};




//---------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------//
//---------------------------------------------------------------------------------------------------//

class CQueryServer : public CNtlServerApp
{

public:

	CNtlString		GetDatabaseHost()		{	return m_config.DatabaseHost;	}
	CNtlString		GetDatabaseUser()		{	return m_config.DatabaseUser;	}
	CNtlString		GetDatabasePassword()	{	return m_config.DatabasePassword;	}
	CNtlString		GetDatabase()			{	return m_config.Database;	}
	CNtlString		GetAccDbHost()	{ return m_config.AccDatabaseHost; }
	CNtlString		GetAccDbUser()	{ return m_config.AccDatabaseUser; }
	CNtlString		GetAccDbPass()	{ return m_config.AccDatabasePassword; }
	CNtlString		GetAccDbDatabase() { return m_config.AccDatabase; }

	int				OnInitApp();
	int				OnAppStart();
	int				OnCreate();
	void			OnDestroy();


	int	OnConfiguration(const char * lpszConfigFile)
	{
		CNtlIniFile file;
		int rc = file.Create( lpszConfigFile );
		if( NTL_SUCCESS != rc )
		{
			return rc;
		}

		if (!file.Read("Chat Accept", "Address", m_config.strChatServerAcceptAddr))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Chat Accept", "Port", m_config.wChatServerAcceptPort))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Game Accept", "Address", m_config.strGameServerAcceptAddr))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Game Accept", "Port", m_config.wGameServerAcceptPort))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Char Accept", "Address", m_config.strCharServerAcceptAddr))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Char Accept", "Port", m_config.wCharServerAcceptPort))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}


		//DATABASE_CHARACTER
		if( !file.Read("DATABASE_CHARACTER", "Host",  m_config.DatabaseHost) )
		{
			return NTL_ERR_DBC_CONNECTION_CONNECT_FAIL;
		}
		if( !file.Read("DATABASE_CHARACTER", "User",  m_config.DatabaseUser) )
		{
			return NTL_ERR_DBC_CONNECTION_CONNECT_FAIL;
		}
		if( !file.Read("DATABASE_CHARACTER", "Password",  m_config.DatabasePassword) )
		{
			return NTL_ERR_DBC_CONNECTION_CONNECT_FAIL;
		}
		if( !file.Read("DATABASE_CHARACTER", "Db",  m_config.Database) )
		{
			return NTL_ERR_DBC_CONNECTION_CONNECT_FAIL;
		}
		//DATABASE_ACCOUNT
		if (!file.Read("DATABASE_ACCOUNT", "Host", m_config.AccDatabaseHost))
		{
			return NTL_ERR_DBC_CONNECTION_CONNECT_FAIL;
		}
		if( !file.Read("DATABASE_ACCOUNT", "User",  m_config.AccDatabaseUser) )
		{
			return NTL_ERR_DBC_CONNECTION_CONNECT_FAIL;
		}
		if( !file.Read("DATABASE_ACCOUNT", "Password",  m_config.AccDatabasePassword) )
		{
			return NTL_ERR_DBC_CONNECTION_CONNECT_FAIL;
		}
		if( !file.Read("DATABASE_ACCOUNT", "Db",  m_config.AccDatabase) )
		{
			return NTL_ERR_DBC_CONNECTION_CONNECT_FAIL;
		}

		//MASTER SERVER
		if (!file.Read("Master Connect", "Address", m_config.strMasterServerIP))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("Master Connect", "Port", m_config.wMasterServerPort))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		return NTL_SUCCESS;
	}

	virtual void	Run();

	virtual BOOL	OnCommandInput(std::string& sCmd);

	void			CreateQueryTaskRunThread(int nAmount = 1);


private:
	sSERVERCONFIG				m_config;
	CNtlConnector				m_serverConnector; //connect to master server

	CNtlAcceptor				m_ChatServerAcceptor; //accept chat server connection
	CNtlAcceptor				m_GameServerAcceptor; //accept game server connection
	CNtlAcceptor				m_CharServerAcceptor; //accept chat server connection

public:
	CNtlLog  					m_log;


private:

	bool						m_bDragonballScrambleBegin;


public:

	inline void					SetDragonballScrambleBegin(bool bFlag) { m_bDragonballScrambleBegin = bFlag; }
	inline bool					GetDragonballScrambleBegin() { return m_bDragonballScrambleBegin; }

};