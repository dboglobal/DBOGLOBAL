#ifndef __DBOG_GAME_DATA_H__
#define __DBOG_GAME_DATA_H__

#include "TableContainerManager.h"
#include "PlayScriptManager.h"
#include "AIScriptManager.h"
#include "TimeQuestScriptManager.h"
#include "WorldPlayScriptManager.h"



class CGameData
{

public:

	CGameData();
	virtual ~CGameData();

public:

	bool					Create(DWORD dwCodePage);

	void					Destroy();

	CTableContainerManager*	GetTableContainer() { return &m_tableContainer; }

	CPlayScriptManager*		GetPlayScriptManager() { return &m_playScriptManager; }

	CAIScriptManager*		GetAIScriptManager() { return &m_AiScriptManager; }


protected:

	void					Init();


private:

	CTableContainerManager	m_tableContainer;

	CPlayScriptManager		m_playScriptManager;

	CAIScriptManager		m_AiScriptManager;

	CTimeQuestScriptManager	m_timeQuestScriptManager;

	CWorldPlayScriptManager	m_WorldPlayScriptManager;

};

#endif
