#ifndef __SERVER_Trigger_MANAGER__
#define __SERVER_Trigger_MANAGER__


#include "DboTSMain.h"
#include "NtlSingleton.h"


class CTriggerManager : public CDboTSMain, public CNtlSingleton<CTriggerManager>
{

public:
	CTriggerManager();
	virtual ~CTriggerManager();


	void			Init(BOOL bLoadEncrypt);


	BOOL			LoadAllTriggers();
	BOOL			LoadAllEncTriggers();

///////////////////////////////////////////////////////////////////////////////////


private:
	// Path
	CNtlString							m_strQuestPath;
	CNtlString							m_strTriggerPath;
	CNtlString							m_strObjectPath;

public:
	CNtlTSMain::mapdef_TLIST			m_defQuest;
	CNtlTSMain::mapdef_TLIST			m_defTrigger;
	CNtlTSMain::mapdef_TLIST			m_defObject;


////////////////////////////////////////////////////////////////////////////////////



public:
	CNtlTSTrigger*						FindQuestFromTS( QUESTID tId );
	CNtlTSTrigger*						FindPcFromTS( QUESTID tId );
	CNtlTSTrigger*						FindObjectFromTS( QUESTID tId );

};

#define GetTriggerManager()		CTriggerManager::GetInstance()
#define g_pTriggerManager		GetTriggerManager()

#endif