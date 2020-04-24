#ifndef __AI_DBOG_BOTCONDITION_SCANTARGET_H__
#define __AI_DBOG_BOTCONDITION_SCANTARGET_H__

#include "BotAiCondition.h"


class CBotAiCondition_ScanTarget : public CBotAiCondition
{

public:

	enum eBOTAI_SMARTSCAN_TYPE
	{
		BOTAI_SMARTSCAN_NORMAL = 0x0,
		BOTAI_SMARTSCAN_UNDER = 0x1,
		MAX_BOTAI_SMARTSCAN_OVER = 0x2,
	};

	CBotAiCondition_ScanTarget(CNpc* pBot, CBotAiCondition_ScanTarget::eBOTAI_SMARTSCAN_TYPE eSmartScantype, BYTE bySmartLevel);
	virtual	~CBotAiCondition_ScanTarget();


public:

	virtual void OnEnter();

	virtual int OnUpdate(DWORD dwTickDiff, float fMultiple);

	bool ScanTarget(bool bSmartOffence);

private:

	DWORD m_dwTime;
	BYTE m_bySmartScantype;
	BYTE m_bySmartLevel;

};

#endif