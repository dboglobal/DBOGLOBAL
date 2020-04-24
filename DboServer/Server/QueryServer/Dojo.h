#pragma once

#include "NtlSingleton.h"
#include "NtlDojo.h"
#include <map>


class CDojo : public CNtlSingleton<CDojo>
{

public:

	CDojo();
	virtual ~CDojo();

private:

	void							Init();
	void							Destroy();

public:

	void							SendDojoData(HSESSION hSession);
	void							CreateDojo(GUILDID guildId, TBLIDX dojoTblidx, WCHAR* guildName);
	bool							DeleteDojo(GUILDID guildId, TBLIDX dojoTblidx);
	bool							DeleteDojo(GUILDID guildId);

	sDBO_DOJO_DATA*					GetDojoData(GUILDID guildId);
	sDBO_DOJO_DATA*					GetDojoDataWithIdx(TBLIDX dojoIdx);

	void							RemoveDojo(GUILDID guildId);
	void							InsertDojo(GUILDID guildId, sDBO_DOJO_DATA* pData);

private:

	std::map<GUILDID, sDBO_DOJO_DATA*>				m_mapDojo;

};

#define GetDojo()					CDojo::GetInstance()
#define g_pDojo						GetDojo()
