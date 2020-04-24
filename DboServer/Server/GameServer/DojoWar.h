#ifndef __SERVER_DOJO_WAR___
#define __SERVER_DOJO_WAR___

#include "NtlSingleton.h"
#include <set>
#include "NtlSharedType.h"

class CDojo;

class CDojoWar : public CNtlSingleton<CDojoWar>
{

public:

	CDojoWar();
	virtual ~CDojoWar();


private:

	void							Init();

	void							Destroy();


public:

	void							AddDojo(CDojo* pDojo);

	CDojo*							GetDojo(GUILDID guildId);

	void							RemoveDojo(TBLIDX dojoTblidx);

private:

	std::set<CDojo*>				m_set_dojoWar;
};

#define GetDojoWarManager()			CDojoWar::GetInstance()
#define g_pDojoWarManager			GetDojoWarManager()

#endif