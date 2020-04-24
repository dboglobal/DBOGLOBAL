/*****************************************************************************
 *
 * File			: NtlFSMEventQueuingHelper.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 9. 26	
 * Abstract		: FSM event queuing helper
 *****************************************************************************
 * Desc         :  잠시 잠깐 임시로 쓴다.
 *				  - state를 stack 하는 기능을 agent에 만든다.
 *
 *****************************************************************************/


#ifndef __NTL_FSM_EVENTQUEUING_HELPER_H__
#define __NTL_FSM_EVENTQUEUING_HELPER_H__

class CNtlFSMAgent;

void AgentQueuingTerrainClick(CNtlFSMAgent *pAgent, RwV3d vPos);

void AgentQueuingKeyboardMove(CNtlFSMAgent *pAgent, RwUInt8 byMoveFlags);

#endif