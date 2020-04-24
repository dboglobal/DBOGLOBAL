#ifndef __SERVER_WPSNODE_ACTION_CHANGESPSSCENE__
#define __SERVER_WPSNODE_ACTION_CHANGESPSSCENE__


#include "ControlScriptNodeAction.h"

class CWPSNodeAction_ChangeSpsScene : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_ChangeSpsScene(const char* lpszName);
	virtual ~CWPSNodeAction_ChangeSpsScene();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	TBLIDX						m_npcTblidx;

	DWORD						m_dwPlayScript;

	DWORD						m_dwPlayScene;

};


#endif
