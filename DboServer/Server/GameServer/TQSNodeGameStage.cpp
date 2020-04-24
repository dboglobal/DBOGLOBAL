#include "stdafx.h"
#include "TQSNodeGameStage.h"



CTQSNodeGameStage::CTQSNodeGameStage(DWORD dwStageNumber, int ctrlScrNodeID/* = TQS_NODE_STATE_GAME_STAGE*/, const char* lpszName/* = "TQS_NODE_STATE_GAME_STAGE"*/)
	: m_dwStageNumber(dwStageNumber), CControlScriptNodeState(ctrlScrNodeID, lpszName)
{
}

CTQSNodeGameStage::~CTQSNodeGameStage()
{

}
