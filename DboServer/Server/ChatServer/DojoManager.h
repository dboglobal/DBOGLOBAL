#ifndef __SERVER_DOJO_MANAGER__
#define __SERVER_DOJO_MANAGER__

#include "NtlSingleton.h"
#include "Dojo.h"
#include <map>

class CDojoManager : public CNtlSingleton<CDojoManager>
{

public:

	CDojoManager();
	virtual ~CDojoManager();


private:

	typedef std::map<GUILDID, CDojo*> TDOJO_MAP;
	TDOJO_MAP m_map_Dojos;

private:

	void						Init();
	void						Destroy();

public:

	CDojo*						GetDojo(GUILDID guildId);
	CDojo*						GetDojoByDojoTblidx(TBLIDX dojoTblidx);

	sDBO_DOJO_DATA*				CreateDojo(sDBO_DOJO_DATA & rData);
	sDBO_DOJO_DATA*				CreateDojo(CHARACTERID charId, GUILDID guildId, TBLIDX dojoTblidx);
	void						DestroyDojo(GUILDID guildId, TBLIDX dojoTblidx);

	void						SendDojoInfo(TBLIDX dojoTblidx, sDBO_DOJO_NPC_INFO* pNpcInfo, WORD& rwResultcode);

	void						SendDojoData(HSESSION hSession);

};

#define GetDojoManager()		CDojoManager::GetInstance()
#define g_pDojoManager			GetDojoManager()

#endif