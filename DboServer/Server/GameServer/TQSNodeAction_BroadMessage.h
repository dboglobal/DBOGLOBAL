#ifndef __SERVER_TQSONODE_ACTION_BROADMESSAGE__
#define __SERVER_TQSONODE_ACTION_BROADMESSAGE__


#include "ControlScriptNodeAction.h"
#include "DboTSCoreDefine.h"

class CTQSNodeAction_BroadMessage : public CControlScriptNodeAction
{

public:
	CTQSNodeAction_BroadMessage(const char* lpszName);
	virtual ~CTQSNodeAction_BroadMessage();


	virtual sPARAMETER_INFO*	GetParameterMap();

public:

	virtual bool				AddParam(CControlScriptNodeParam_Number* pNode);

	virtual bool				AddParam(CControlScriptNodeParam_String* pNode);

	virtual bool				CheckParameter() { return true; }

public:

	eBROAD_MSG_EXCUTION_TYPE	m_eExecutionType;

	eBROAD_MSG_UI_DIR_TYPE		m_eUIDirType;

	eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE	m_eUIShowHideType;

	eBROAD_MSG_UI_BALLOON_SHAPE_TYPE	m_eUIBalloonShapeType;

	eBROAD_MSG_UI_SPEECH_DIR_TYPE	m_eUISpeechDirType;

	eBROAD_MSG_OWNER_CONDITION	m_eOwnerCondition;

	eBROAD_MSG_OWNER_TYPE		m_eOwnerType;

	TBLIDX						m_uiOwnerTblIdx;

	TBLIDX						m_uiQuestTextTblidx;

	float						m_fDisplayTime;

};


#endif
