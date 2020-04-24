#ifndef __SERVER_WPSNODE_ACTION_MOBLIST__
#define __SERVER_WPSNODE_ACTION_MOBLIST__


#include "ControlScriptNodeAction.h"

class CWPSNodeAction_MobList : public CControlScriptNodeAction
{

public:
	CWPSNodeAction_MobList(const char* lpszName);
	virtual ~CWPSNodeAction_MobList();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter();

public:

	std::string					m_strMobList[10];

	int							m_nIndex;

	std::vector<TBLIDX>			m_aMobList[10];

	BYTE						m_byEffectiveLevel;

	BYTE						m_byActualLevel;

	WORD						m_wMobClass;

	BYTE						m_byMobGrade;

};


#endif
