#include "stdafx.h"
#include "Discord.h"
#include <stdio.h>


const char* DISCORD_APPLICATION_ID = "YOUR-APPLICATION-ID"; // https://discordapp.com/developers/applications

CDiscord* CDiscord::m_pInstance = NULL;


CDiscord::CDiscord()
{
	nState = STATE_DISABLED;
	m_state = "Logging In";
	m_characterClassID = 0;
	m_characterLevel = 0;
	m_channel = 0;
	m_bHideName = false;
	m_bEnableDiscord = false;
	m_bConnected = false;
}

CDiscord::~CDiscord()
{
}

bool CDiscord::CreateInstance()
{
	if (m_pInstance)
		return false;

	m_pInstance = new CDiscord;

	return true;
}

void CDiscord::DestroyInstance()
{
	if (!m_pInstance)
		return;

	Discord_Shutdown(); // shut down discord

	delete m_pInstance;
}

void CDiscord::handleDiscordReady(const DiscordUser * connectedUser)
{
	GetDiscordManager()->SetConnected(true);
}

void CDiscord::handleDiscordDisconnected(int errcode, const char* message)
{
	GetDiscordManager()->SetConnected(false);
}

void CDiscord::handleDiscordError(int errcode, const char* message)
{
}



void CDiscord::InitDiscord()
{
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));

	handlers.ready = CDiscord::handleDiscordReady;
	handlers.disconnected = CDiscord::handleDiscordDisconnected;
	handlers.errored = CDiscord::handleDiscordError;
	handlers.joinGame = NULL;
	handlers.spectateGame = NULL;
	handlers.joinRequest = NULL;

	Discord_Initialize(DISCORD_APPLICATION_ID, &handlers, 1, NULL);
}

void CDiscord::UpdatePresence()
{
	if (m_bEnableDiscord == false)
		return;

	if (m_bConnected == false)
		return;

		DiscordRichPresence discordPresence;
		memset(&discordPresence, 0, sizeof(discordPresence));

		discordPresence.details = m_state.c_str();

		char buffer[128];
		if (nState == STATE_CHANNEL)
		{
			if (m_bHideName)
			{
				sprintf_s(buffer, sizeof(buffer), "Level: %d", m_characterLevel);
			}
			else
			{
				sprintf_s(buffer, sizeof(buffer), "%s - Lv: %d", m_characterName.c_str(), m_characterLevel);
			}

			discordPresence.state = buffer;

			char buffer2[32];
			char buffer3[128];

			sprintf_s(buffer2, sizeof(buffer2), "%d", m_characterClassID);
			sprintf_s(buffer3, sizeof(buffer3), "%s", m_characterClass.c_str());

			discordPresence.smallImageKey = buffer2;
			discordPresence.smallImageText = buffer3;
		}

		discordPresence.largeImageKey = "1stardb";
		discordPresence.largeImageText = "https://dboglobal.to";
		

		/*discordPresence.startTimestamp = StartTime;
		discordPresence.endTimestamp = time(0) + 5 * 60;
		discordPresence.partyId = "party1234";
		discordPresence.partySize = 1;
		discordPresence.partyMax = 6;
		discordPresence.matchSecret = "xyzzy";
		discordPresence.joinSecret = "join";
		discordPresence.spectateSecret = "look";*/

		discordPresence.instance = 0;
		Discord_UpdatePresence(&discordPresence);
}


void CDiscord::ClearPresence()
{
	Discord_ClearPresence();
}


void CDiscord::UpdateDiscordConnection()
{
	if (m_bEnableDiscord == false)
		return;

#ifdef DISCORD_DISABLE_IO_THREAD
	Discord_UpdateConnection();
#endif
	Discord_RunCallbacks();
}

void CDiscord::SetUseDiscord(bool bFlag)
{
	m_bEnableDiscord = bFlag;

	if (bFlag)
	{
		InitDiscord();
	}
}


void CDiscord::SetHideName(bool bFlag, bool bUpdatePresence)
{
	if (m_bEnableDiscord == false)
		return;

	m_bHideName = bFlag;


	if (bUpdatePresence && m_bConnected)
		UpdatePresence();
}

void CDiscord::LoginAuthServer()
{
	if (m_bEnableDiscord == false)
		return;

	nState = STATE_AUTH;
	m_state = "Logging In";


	if (m_bConnected)
		UpdatePresence();
}

void CDiscord::LoginCharServer()
{
	if (m_bEnableDiscord == false)
		return;

	nState = STATE_CHAR;
	m_state = "Character Selection";


	if (m_bConnected)
		UpdatePresence();
}

void CDiscord::LoginChannel(unsigned char channelId, char * name, unsigned char level, char* className, unsigned char classID)
{
	if (m_bEnableDiscord == false)
		return;

	nState = STATE_CHANNEL;

	char buff[100];
	sprintf_s(buff, sizeof(buff), "In Game - Channel: %d", channelId);

	m_state = buff;

	m_characterName = name;
	m_characterLevel = level;
	m_characterClass = className;
	m_characterClassID = classID;
	m_channel = channelId;

	if (m_bConnected)
		UpdatePresence();
}

void CDiscord::SetConnected(bool bFlag)
{
	m_bConnected = bFlag;
}

void CDiscord::UpdateLevel(unsigned char byLevel)
{
	if (m_bEnableDiscord == false)
		return;

	m_characterLevel = byLevel;

	if (m_bConnected)
		UpdatePresence();
}