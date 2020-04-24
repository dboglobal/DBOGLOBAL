#ifndef __AI_DBOG_BOTCONDITION_SPEECH_H__
#define __AI_DBOG_BOTCONDITION_SPEECH_H__

#include "BotAiCondition.h"
#include "NpcSpeechTable.h"

class CBotAiCondition_Speech : public CBotAiCondition
{

public:

	CBotAiCondition_Speech(CNpc* pBot, eNPC_SPEECH_TYPE eSpeechType, DWORD dwCheckInterval, bool bDisposable, bool bPreCheck);
	virtual	~CBotAiCondition_Speech();


public:

	virtual void OnEnter();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

private:

	DWORD m_dwTime;
	eNPC_SPEECH_TYPE m_eSpeechType;
	DWORD m_dwCheckInterval;
	bool m_bDisposable;
	bool m_bPreCheck;

};

#endif