#ifndef __DBOG_GAME_MAIN_H__
#define __DBOG_GAME_MAIN_H__

#include "ObjectManager.h"
#include "WorldManager.h"
#include "WorldFactory.h"

#include "ItemManager.h"
#include "ShenronManager.h"



class CGameData;
class CServerNaviLog;

class CGameMain
{

public:

	CGameMain();
	virtual ~CGameMain();

public:

	bool					Create(CGameData* pGameData);

	void					Destroy();

	bool					PrepareWorldAndObject();

	void					TickProcess(DWORD dwTickDiff, float fMultiple, LARGE_INTEGER *rWorld, LARGE_INTEGER *rObject, LARGE_INTEGER *rSpawn);

	CWorldManager*			GetWorldManager() { return &m_worldManager; }

	CWorldFactory*			GetWorldFactory() { return &m_worldFactory; }

	CObjectManager*			GetObjectManager() { return &m_objectManager; }

private:

	void					Init();

	bool					PrepareNavi();

	void					DestroyNavi();


private:

	bool					m_bIsActive;

	CServerNaviLog *		m_pNaviLog;

	CWorldFactory			m_worldFactory;

	CWorldManager			m_worldManager;

	CObjectManager			m_objectManager;
	
	//CSpawnLocationCache * m_pSpawnLocaitonCache;

};

#endif
