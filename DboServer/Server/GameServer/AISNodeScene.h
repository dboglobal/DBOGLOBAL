#ifndef __SERVER_AISONODE_SCENE__
#define __SERVER_AISONODE_SCENE__


#include "ControlScriptNodeState.h"

class CAISNodeState;
#include <map>

class CAISNodeScene : public CControlScriptNodeState
{

public:
	CAISNodeScene(DWORD dwSceneNumber, const char* lpszSceneName);
	virtual ~CAISNodeScene();


public:

	void			Destroy() {}

	bool			AddState(CAISNodeState* pState);
	
	CAISNodeState*	FindState(int nStateID);

	void			AllDelete() {}

private:

	DWORD			m_dwSceneNumber;

	bool			m_bIsDestroy;

	std::map<int, CAISNodeState*> m_mapState;
};


#endif
