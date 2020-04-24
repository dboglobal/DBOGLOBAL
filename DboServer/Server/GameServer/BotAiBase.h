#ifndef __AI_DBOG_BOTBASE_H__
#define __AI_DBOG_BOTBASE_H__


#include "ComplexState.h"
#include "Npc.h"


class CBotAiBase : public CComplexState
{

public:
	CBotAiBase(CNpc* pBot, eBOTCONTROLID controlID, const char* lpszControlName);
	virtual	~CBotAiBase();

public:
	
//	virtual void		AttachBotAiData(sBOTAI_DATA_BASE* __formal);

public:

	bool				AddSubControlQueue(CBotAiBase* pAiBase, bool bFront);

	CNpc*				GetBot() { return m_pBotNpc; }


private:

	CNpc*			m_pBotNpc;

};





#endif