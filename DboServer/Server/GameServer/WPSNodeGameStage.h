#ifndef __SERVER_WPS_NODE_GAMESTAGE__
#define __SERVER_WPS_NODE_GAMESTAGE__

#include "ControlScriptNodeState.h"


class CWPSNodeGameStage : public CControlScriptNodeState
{

public:
	CWPSNodeGameStage(BYTE byStageNumber, int ctrlScrNodeID = WPS_NODE_STATE_GAME_STAGE, const char* lpszNodeName = "WPS_NODE_STATE_GAME_STAGE");
	virtual ~CWPSNodeGameStage();


public:

	bool					Create() { return CControlScriptNode::Create(true); }
		
	BYTE					GetStageNumber() { return m_byStageNumber; }



private:

	BYTE	m_byStageNumber;

};


#endif