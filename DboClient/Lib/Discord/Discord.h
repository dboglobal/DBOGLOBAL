/*****************************************************************************
*
* File			: Discord.h
* Author		: Daneos
* Copyright		: Daneos
* Date			: 7/29/2018
* Abstract		: Discord
*****************************************************************************
*
* Desc         : Discord rich presence integration
*
*****************************************************************************/
#ifndef __DISCORD_H__
#define __DISCORD_H__

#include <string>
#include "discord_rpc.h"
#pragma comment(lib, "discord-rpc.lib")

class CDiscord
{
	enum eSTATE
	{
		STATE_AUTH,
		STATE_CHAR,
		STATE_CHANNEL,
		STATE_DISABLED,
	};

public:

	CDiscord();
	virtual ~CDiscord();

	static bool		CreateInstance();
	static void		DestroyInstance();

	static CDiscord* GetInstance() { return m_pInstance; }

public:

	static void handleDiscordReady(const DiscordUser* connectedUser);

	static void handleDiscordDisconnected(int errcode, const char* message);

	static void handleDiscordError(int errcode, const char* message);

private:

	void InitDiscord();

	void UpdatePresence();

	void ClearPresence();

public:

	void			UpdateDiscordConnection();

	void			SetUseDiscord(bool bFlag); // if true, then we will use discord and connect to discord

	void			SetHideName(bool bFlag, bool bUpdatePresence);

	void			LoginAuthServer(); // called when player logged into auth server

	void			LoginCharServer(); // called when player logged into char server

	void			LoginChannel(unsigned char channelId, char* name, unsigned char level, char* className, unsigned char classID); // called when player logged into channel

	void			SetConnected(bool bFlag); //set connected to discord or not

	void			UpdateLevel(unsigned char byLevel);

private:

	int				nState;

	std::string		m_state; // "Logging In", "Character Selection", "In-Game"

	std::string		m_characterName;
	unsigned char	m_characterLevel;
	unsigned char	m_characterClassID;
	std::string		m_characterClass;
	unsigned char	m_channel;

	bool			m_bHideName;

	bool			m_bEnableDiscord;

	bool			m_bConnected;

protected:

	static CDiscord* m_pInstance;
};

static CDiscord* GetDiscordManager()
{
	return CDiscord::GetInstance();
};


#endif