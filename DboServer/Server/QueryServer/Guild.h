#pragma once

#include "NtlSingleton.h"
#include "NtlGuild.h"
#include <map>

struct sITEM_DATA;

struct sGUILD_BANK
{
	sGUILD_BANK()
	{
		dwZeni = 0;
	}

	sITEM_DATA* GetItemData(ITEMID itemId)
	{
		std::map<ITEMID, sITEM_DATA*>::iterator it = m_mapItems.find(itemId);
		if (it != m_mapItems.end())
		{
			return it->second;
		}

		return NULL;
	}

	DWORD								dwZeni;
	std::map<ITEMID, sITEM_DATA*>		m_mapItems;
};


class CGuild : public CNtlSingleton<CGuild>
{

public:

	CGuild();
	virtual ~CGuild();

private:

	void							Init();
	void							Destroy();

	void							LoadGuildMemberData(GUILDID guilId, DWORD dwLimit);

public:

	void							SendGuildData(HSESSION hSession);

	sDBO_GUILD_DATA*				GetGuildData(GUILDID guildId);

	sDBO_GUILD_DATA*				CreateGuild(CHARACTERID guildMaster, WCHAR* wszName, QWORD qwGuildFunctionFlag);

	void							AddGuildMember(GUILDID guildId, sDBO_GUILD_MEMBER_DATA* pData, WCHAR* wszGuildName);

	bool							RemoveGuildMember(GUILDID guildId, CHARACTERID charId);

	sDBO_GUILD_MEMBER_DATA*			GetGuildMemberData(GUILDID guildId, CHARACTERID charId);

	void							RemoveGuild(GUILDID guildId);

private:

	GUILDID											uiHighestGuildID;

	std::map<GUILDID, sDBO_GUILD_DATA*>				m_mapGuild;

	std::multimap<GUILDID, sDBO_GUILD_MEMBER_DATA*>	m_mapGuildMember;

	std::map<GUILDID, sGUILD_BANK*>					m_mapGuildBank;


public:

	sGUILD_BANK*					GetGuildBank(GUILDID guildId);
	void							InsertGuildBank(GUILDID guildId, sGUILD_BANK* pBank);

};

#define GetGuild()					CGuild::GetInstance()
#define g_pGuild					GetGuild()
