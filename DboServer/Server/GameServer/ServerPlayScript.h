#ifndef __SERVER_SPS_SCRIPT__
#define __SERVER_SPS_SCRIPT__

#include "ControlScript.h"
#include "SPSNodeScene.h"

enum eSPS_NODE;

class CServerPlayScript : public CControlScript
{

public:
	CServerPlayScript(CControlScriptManager* pManager, char const* lpszFileName);
	virtual ~CServerPlayScript();


public:

	virtual bool			Load(char * achBuffer) { return true; }
	virtual bool			Load(const char* lpszScriptFile);
	virtual bool			CheckIntegrity();

public:

	void					Destroy() {}

	CSPSNodeScene*			Find(DWORD dwSceneNumber);

	bool					AddScene(DWORD dwSceneNumber, CSPSNodeScene * pScene);

	bool					AttachScript(CControlState* pControlState, CControlStateFactory* pFactory, DWORD dwSceneNumber);

	static bool				LuaBind();

private:

	void					Init();


private:

	std::map<DWORD, CSPSNodeScene*> m_sceneList;

};


#endif