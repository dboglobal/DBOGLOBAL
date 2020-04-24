#ifndef __TQS_ALGO_DBOG_OBJECT_H__
#define __TQS_ALGO_DBOG_OBJECT_H__

#include "ScriptAlgoObject.h"
#include "TimeQuestTable.h"

class CTimeQuest;

class CTqsAlgoObject : public CScriptAlgoObject
{

public:

	CTqsAlgoObject(TBLIDX tblidx, sTIMEQUEST_TBLDAT* pTimeQuestTbldat, DWORD dwHandle);
	virtual	~CTqsAlgoObject();

protected:

	void					Init();

public:

	virtual void			Start();
	virtual void			Stop(bool bEnterFailStage = false, bool bRemoveFromWorld = true);	// !IMPORTANT! ONLY CALL BY MAIN-THREAD

public:

	sTIMEQUEST_TBLDAT*		GetTimeQuestTbldat() { return m_pTimeQuestTbldat; }

	void					SetTimeQuest(CTimeQuest* pTqs) { m_pTimeQuest = pTqs; }

	CTimeQuest*				GetTimeQuest() { return m_pTimeQuest; }

private:

	sTIMEQUEST_TBLDAT*		m_pTimeQuestTbldat;

	CTimeQuest*				m_pTimeQuest;


};

#endif