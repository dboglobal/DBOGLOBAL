#ifndef __SERVER_GUILD_MANAGER__
#define __SERVER_GUILD_MANAGER__

#include "NtlSingleton.h"
#include "Guild.h"


class CGuildManager : public CNtlSingleton<CGuildManager>
{

public:
	CGuildManager();
	virtual ~CGuildManager();


private:

	typedef std::map<GUILDID, CGuild*> TGUILD_MAP;
	TGUILD_MAP m_map_Guilds;

public:

	void				Init();

	void				DeleteGuild(GUILDID GuildId);

	CGuild*				GetGuild(GUILDID GuildId);

	CGuild*				CreateGuild(sDBO_GUILD_DATA& rData);
	void				CreateGuild(sDBO_GUILD_DATA& rData, sDBO_GUILD_MEMBER_DATA* paMemberData, BYTE byMemberCount);

	void				SendGuildData(HSESSION hSession);
};

#define GetGuildManager()		CGuildManager::GetInstance()
#define g_pGuildManager			GetGuildManager()

#endif