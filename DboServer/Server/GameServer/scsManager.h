#pragma once

#include "NtlSingleton.h"

class CPlayer;

class CScsManager : public CNtlSingleton<CScsManager>
{
public:

	CScsManager();
	virtual ~CScsManager();

public:

	void						TestGenerate(CPlayer* pPlayer);


};

#define GetScsSystem()				CScsManager::GetInstance()
#define g_pScsManager				GetScsSystem()

