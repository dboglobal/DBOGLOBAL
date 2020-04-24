#ifndef __SCRIPT_ALGO_DBOG_ACTION_BROADMESSAGE_H__
#define __SCRIPT_ALGO_DBOG_ACTION_BROADMESSAGE_H__

#include "ScriptAlgoAction.h"
#include "DboTSCoreDefine.h"


class CWpsAlgoAction_BroadMessage : public CScriptAlgoAction
{

public:

	CWpsAlgoAction_BroadMessage(CWpsAlgoObject* pObject);
	virtual	~CWpsAlgoAction_BroadMessage();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	eBROAD_MSG_EXCUTION_TYPE			m_eExecutionType;

	eBROAD_MSG_UI_DIR_TYPE				m_eUIDirType;

	eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE		m_eUIShowHideType;

	eBROAD_MSG_UI_BALLOON_SHAPE_TYPE	m_eUIBalloonShapeType;

	eBROAD_MSG_UI_SPEECH_DIR_TYPE		m_eUISpeechDirType;

	eBROAD_MSG_OWNER_CONDITION			m_eOwnerCondition;

	eBROAD_MSG_OWNER_TYPE				m_eOwnerType;

	TBLIDX								m_uiOwnerTblIdx;

	TBLIDX								m_uiQuestTextTblidx;

	float								m_fDisplayTime;


};

#endif