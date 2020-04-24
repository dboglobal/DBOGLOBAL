#ifndef __WPS_ALGO_DBOG_OBJECT_H__
#define __WPS_ALGO_DBOG_OBJECT_H__

#include "ScriptAlgoObject.h"

#include "WorldPlayTable.h"


class CObjectMsg;
class CQuestProgress;

class CWpsAlgoObject : public CScriptAlgoObject
{

public:

	CWpsAlgoObject(TBLIDX tblidx, sWORLDPLAY_TBLDAT* pWorldPlayTbldat, DWORD dwHandle);
	virtual	~CWpsAlgoObject();

protected:

	void					Init();

public:

	virtual void			Start();
	virtual void			Stop(bool bEnterFailStage = false, bool bRemoveFromWorld = true); // !IMPORTANT! ONLY CALL BY MAIN-THREAD

public:

	virtual int				OnUpdate(DWORD dwTickDiff);

public:

	sWORLDPLAY_TBLDAT*		GetWorldPlayTbldat() { return m_pWorldPlayTbldat; }

	BYTE					GetExecuterType() { return m_pWorldPlayTbldat->byExecuterType; }
	BYTE					GetShareType() { return m_pWorldPlayTbldat->byShareType; }

	void					SetCCBDStage(BYTE byStage) { m_byCCBDStage = byStage; }
	BYTE					GetCCBDStage() { return m_byCCBDStage; }
	void					SetCCBDTimeLimit(bool bFlag) { m_bCCBDTimeLimit = bFlag; }
	bool					GetCCBDTimeLimit() { return m_bCCBDTimeLimit; }
	void					SetCCBDFail(bool bFlag) { m_bCCBDFail = bFlag; }
	bool					GetCCBDFail() { return m_bCCBDFail; }

	void					SetCCBDStartStage(BYTE byStage) { m_byCCBDStartStage = byStage; }
	BYTE					GetCCBDStartStage() { return m_byCCBDStartStage; }

public:

	virtual bool			NotifyPlayerLeave(HOBJECT hPlayer, bool bRemoveScriptFromPlayer = true);

private:

	sWORLDPLAY_TBLDAT*		m_pWorldPlayTbldat;

	BYTE					m_byCCBDStage;
	bool					m_bCCBDTimeLimit;
	bool					m_bCCBDFail;

	BYTE					m_byCCBDStartStage;
};

#endif