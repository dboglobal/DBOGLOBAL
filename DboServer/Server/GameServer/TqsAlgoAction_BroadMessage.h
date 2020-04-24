#ifndef __TQS_ALGO_DBOG_ACTION_BROADMESSAGE_H__
#define __TQS_ALGO_DBOG_ACTION_BROADMESSAGE_H__

#include "TqsAlgoAction_Base.h"
#include "DboTSCoreDefine.h"


class CTqsAlgoAction_BroadMessage : public CTqsAlgoAction_Base
{

public:

	CTqsAlgoAction_BroadMessage(CTqsAlgoObject* pObject);
	virtual	~CTqsAlgoAction_BroadMessage();

public:

	virtual bool		AttachControlScriptNode(CControlScriptNode* pControlScriptNode);

	virtual int			OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

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