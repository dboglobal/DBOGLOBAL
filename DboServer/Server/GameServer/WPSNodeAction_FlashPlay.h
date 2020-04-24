#ifndef __SERVER_WPSNODE_ACTION_FLASHPLAY__
#define __SERVER_WPSNODE_ACTION_FLASHPLAY__


#include "ControlScriptNodeAction.h"

enum eDIRECTION_TYPE;

class CWPSNodeAction_FlashPlay : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_FlashPlay(const char* lpszName);
	virtual ~CWPSNodeAction_FlashPlay();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }


public:

	eDIRECTION_TYPE				m_eDirectionType;

	TBLIDX						m_directTblidx;

	bool						m_bSynchronize;

	DWORD						m_dwPlayTime;

	DWORD						m_dwWaitTime;

};


#endif
