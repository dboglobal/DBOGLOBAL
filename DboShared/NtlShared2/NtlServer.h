//***********************************************************************************
//
//	File		:	NtlServer.h
//
//	Begin		:	2008-05-13
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	
//
//	Desc		:	
//
//***********************************************************************************

#pragma once

#include "NtlString.h"

struct sGAME_SERVERCONFIG
{
	CNtlString		strClientAcceptAddr;
	CNtlString		strPublicClientAcceptAddr;
	WORD			wClientAcceptPort;

	CNtlString		DatabaseHost;
	CNtlString		DatabaseUser;
	CNtlString		DatabasePassword;
	CNtlString		Database;
	CNtlString		AccDatabaseHost;
	CNtlString		AccDatabaseUser;
	CNtlString		AccDatabasePassword;
	CNtlString		AccDatabase;

	BYTE			byServerID;
	BYTE			byChannel;
	CNtlString		ServerName;
	CNtlString		ChannelName;

	BOOL			bTestServer;
	BOOL			bLoadTriggersEnc;

	int				nMaxConnection;
	int				SoloExpRate;		//in percent
	int				PartyExpRate;		//in percent
	int				ItemDropRate;		//in percent
	BYTE			MaxLevel;
	int				ZeniDropRate;		//in percent
	int				ZeniBonusRate;		//in percent
	int				ZeniPartyBonusRate;	//in percent
	int				QuestMoneyRate;		//in percent
	int				QuestExpRate;		//in percent

	int				LoadTableFormat;
	CNtlString		TablePath;

	CNtlString		strChatServerIP;
	WORD			wChatServerPort;
	CNtlString		strChatServerConAddr;
	WORD			wChatServerConPort;

	CNtlString		strLogPath;

	CNtlString		strTsPath;

	CNtlString		strCharServerIP;
	WORD			wCharServerPort;

	CNtlString		strMasterServerIP;
	WORD			wMasterServerPort;

	CNtlString		strQueryServerIP;
	WORD			wQueryServerPort;

	CNtlString		strPlayScriptPath;
	CNtlString		strAIScriptPath;
	CNtlString		strWorldPlayScriptPath;
	CNtlString		strTimeQuestScriptPath;

	CNtlString		m_strGraphicCharacterDataPath;
	CNtlString		m_strGraphicObjectDataPath;
	CNtlString		m_strWorldDataPath;

	CNtlString		m_strPathEngineDllName;
	CNtlString		m_strNavDataPath;
	bool			m_bEnableNavigator;
};

struct sCHAT_SERVERCONFIG
{
	CNtlString		strClientAcceptAddr;
	CNtlString		strPublicClientAcceptAddr;
	WORD			wClientAcceptPort;

	CNtlString		strServerAcceptAddr;
	unsigned int	ServerAcceptLimit;
	WORD			wServerAcceptPort;

	CNtlString		strMasterServerIP;
	WORD			wMasterServerPort;

	CNtlString		strQueryServerIP;
	WORD			wQueryServerPort;

	CNtlString		DatabaseHost;
	CNtlString		DatabaseUser;
	CNtlString		DatabasePassword;
	CNtlString		Database;
	CNtlString		AccDatabaseHost;
	CNtlString		AccDatabaseUser;
	CNtlString		AccDatabasePassword;
	CNtlString		AccDatabase;

	int				nMaxConnection;

	int				LoadTableFormat;
	CNtlString		TablePath;
};

struct sCHAR_SERVERCONFIG
{
	CNtlString		strClientAcceptAddr;
	CNtlString		strPublicClientAcceptAddr;
	WORD			wClientAcceptPort;
	BYTE			byServerID;

	CNtlString		DatabaseHost;
	CNtlString		DatabaseUser;
	CNtlString		DatabasePassword;
	CNtlString		Database;

	CNtlString		AccDatabaseHost;
	CNtlString		AccDatabaseUser;
	CNtlString		AccDatabasePassword;
	CNtlString		AccDatabase;

	CNtlString		strQueryServerIP;
	WORD			wQueryServerPort;

	CNtlString		strMasterServerIP;
	WORD			wMasterServerPort;

	int				LoadTableFormat;
	CNtlString		TablePath;

	int				nMaxConnection;

	BOOL			bReserveName;
};
