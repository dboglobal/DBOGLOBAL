#ifndef __SERVER_SPSONODE_SCENE__
#define __SERVER_SPSONODE_SCENE__


#include "ControlScriptNodeState.h"


class CSPSNodeScene : public CControlScriptNodeState
{

public:
	CSPSNodeScene(DWORD dwSceneNumber, const char* lpszSceneName);
	virtual ~CSPSNodeScene();

private:

	DWORD			m_dwSceneNumber;
};


#endif
