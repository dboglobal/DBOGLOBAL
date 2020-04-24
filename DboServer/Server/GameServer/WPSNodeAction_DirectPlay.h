#ifndef __SERVER_WPSNODE_ACTION_DIRECTPLAY__
#define __SERVER_WPSNODE_ACTION_DIRECTPLAY__


#include "ControlScriptNodeAction.h"

enum eDIRECTION_TYPE;

class CWPSNodeAction_DirectPlay : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_DirectPlay(const char* lpszName);
	virtual ~CWPSNodeAction_DirectPlay();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter();

public:

	eDIRECTION_TYPE				m_eDirectionType;

	TBLIDX						m_directTblidx;

	bool						m_bSynchronize;

	DWORD						m_dwPlayTime;

	DWORD						m_dwWaitTime;

};


#endif
