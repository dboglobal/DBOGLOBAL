#ifndef __SERVER_TQSNODEGAME_STAGE__
#define __SERVER_TQSNODEGAME_STAGE__


#include "ControlScriptNodeState.h"


class CTQSNodeGameStage : public CControlScriptNodeState
{

public:
	CTQSNodeGameStage(DWORD dwStageNumber, int ctrlScrNodeID = TQS_NODE_STATE_GAME_STAGE, const char* lpszName = "TQS_NODE_STATE_GAME_STAGE");
	virtual ~CTQSNodeGameStage();


public:

	bool			Create() { return CControlScriptNode::Create(true); }

	DWORD			GetStageNumber() { return m_dwStageNumber; }


private:

	DWORD			m_dwStageNumber;
};


#endif