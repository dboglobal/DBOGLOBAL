#pragma once

#include "NtlSfx.h"
#include "NtlSharedType.h"
#include "SessionFactory.h"
#include "SubNeighborServerInfoManager.h"

struct sSERVERCONFIG
{
	BYTE			ServerAcceptLimit;

	CNtlString		strAuthServerAcceptIP;
	WORD			wAuthServerAcceptPort;
	CNtlString		strCharServerAcceptIP;
	WORD			wCharServerAcceptPort;
	CNtlString		strChatServerAcceptIP;
	WORD			wChatServerAcceptPort;
	CNtlString		strGameServerAcceptIP;
	WORD			wGameServerAcceptPort;

	int				ServerPlayerLimit;

	//web admin
	CNtlString		strWebServerAcceptIP;
	WORD			wWebServerAcceptPort;
};



class CMasterServer : public CNtlServerApp
{

public:

	virtual int				OnInitApp();
	virtual int				OnAppStart();
	virtual int				OnCreate();
	virtual void			OnDestroy();


	virtual int		OnConfiguration(const char * lpszConfigFile)
	{
		CNtlIniFile file;

		int rc = file.Create( lpszConfigFile );
		if( NTL_SUCCESS != rc )
		{
			return rc;
		}
		if( !file.Read("Master Server", "ServerAcceptLimit",  m_config.ServerAcceptLimit) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}


		//AUTH SERVER
		if( !file.Read("Auth Accept", "Address", m_config.strAuthServerAcceptIP) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if( !file.Read("Auth Accept", "Port",  m_config.wAuthServerAcceptPort) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		//CHAR SERVER
		if( !file.Read("Char Accept", "Address", m_config.strCharServerAcceptIP) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if( !file.Read("Char Accept", "Port",  m_config.wCharServerAcceptPort) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		//CHAT SERVER
		if( !file.Read("Chat Accept", "Address", m_config.strChatServerAcceptIP) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if( !file.Read("Chat Accept", "Port",  m_config.wChatServerAcceptPort) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		//GAME SERVER
		if( !file.Read("Game Accept", "Address", m_config.strGameServerAcceptIP) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if( !file.Read("Game Accept", "Port",  m_config.wGameServerAcceptPort) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		// WEB SERVER
		if( !file.Read("WEB Accept", "Address", m_config.strWebServerAcceptIP) )
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}
		if (!file.Read("WEB Accept", "Port", m_config.wWebServerAcceptPort))
		{
			return NTL_ERR_SYS_CONFIG_FILE_READ_FAIL;
		}

		return NTL_SUCCESS;
	}


	virtual void	Run();

	virtual BOOL	OnCommandInput(std::string& sCmd);


public:

	CNtlLog  					m_log;

private:

	CNtlAcceptor				m_AuthServerAcceptor;
	CNtlAcceptor				m_CharServerAcceptor;
	CNtlAcceptor				m_ChatServerAcceptor;
	CNtlAcceptor				m_GameServerAcceptor;
	CNtlAcceptor				m_WebServerAcceptor;

public:

	sSERVERCONFIG				m_config;


};