#ifndef __CHARACTER_MANAGER_H__
#define __CHARACTER_MANAGER_H__

#include "NtlSingleton.h"
#include "NtlCharacter.h"

class CCharacterManager : public CNtlSingleton<CCharacterManager>
{

public:

	CCharacterManager();
	virtual ~CCharacterManager();


private:

	void						Init();

public:

	void						CreateCharacter(ACCOUNTID accountId, sPC_SUMMARY& sSum, SERVERFARMID serverFarmId);

	CHARACTERID					AcquireCharID() { return ++m_uiLastCharacterId; }

private:

	CHARACTERID					m_uiLastCharacterId;

};

#define GetCharacterManager()	CCharacterManager::GetInstance()
#define g_pCharacterManager		GetCharacterManager()

#endif