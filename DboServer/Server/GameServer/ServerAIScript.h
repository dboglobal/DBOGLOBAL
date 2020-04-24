#ifndef __SERVER_AIS_SCRIPT__
#define __SERVER_AIS_SCRIPT__

#include "ControlScript.h"
#include "AISNodeScene.h"

enum eAIS_NODE;

class CServerAIScript : public CControlScript
{

public:
	CServerAIScript(CControlScriptManager* pManager, char const* lpszFileName);
	virtual ~CServerAIScript();


public:

	virtual bool			Load(char * achBuffer) { return true; }
	virtual bool			Load(const char* lpszScriptFile);
	virtual bool			CheckIntegrity();

public:

	void					Destroy() {}

	CAISNodeScene*			Find(DWORD dwSceneNumber);

	bool					AddScene(DWORD dwSceneNumber, CAISNodeScene * pScene);

	bool					AttachScript(CControlState* pControlState, CControlStateFactory* pFactory, DWORD dwSceneNumber, eAIS_NODE eAttachAisNode);

	static bool				LuaBind();

private: 
	
	void					Init();


private:

	std::map<DWORD, CAISNodeScene*> m_sceneList;

};



#endif