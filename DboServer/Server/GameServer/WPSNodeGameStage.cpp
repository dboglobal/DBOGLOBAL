#include "stdafx.h"
#include "WPSNodeGameStage.h"

#include "WorldPlayScriptManager.h"
#include "WpsAlgoStage.h"


CWPSNodeGameStage::CWPSNodeGameStage(BYTE byStageNumber, int ctrlScrNodeID/* = WPS_NODE_STATE_GAME_STAGE*/, const char* lpszNodeName/* = "WPS_NODE_STATE_GAME_STAGE"*/): 
m_byStageNumber(byStageNumber), CControlScriptNodeState(ctrlScrNodeID, lpszNodeName)
{
}

CWPSNodeGameStage::~CWPSNodeGameStage()
{
}


