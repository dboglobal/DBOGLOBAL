#ifndef __INC_DBOG_HELPMELISTMANAGER_H__
#define __INC_DBOG_HELPMELISTMANAGER_H__


class CNpc;
class CCharacter;
#include "NtlSharedType.h"

class CHelpMeListManager
{
	struct sHELPMEDATA
	{
		HOBJECT hTarget;
		HOBJECT hCallMe;
		bool bSend;
	};

	typedef std::map<HOBJECT, sHELPMEDATA*> HELPME_MAP;
	typedef HELPME_MAP::value_type HELPME_VAL;


public:
	CHelpMeListManager();
	virtual	~CHelpMeListManager();

public:

	bool			Create(CNpc* pNpc);
	void			AddHelpMeObj(HOBJECT hTarget, HOBJECT hCallMe);
	void			SendBotCautionHelpMe(HOBJECT hTarget, bool bIsRequester);
	void			SetSend(HOBJECT hTarget);
	HOBJECT			FindHelpMeObj(HOBJECT hTarget);
	void			DeleteHelpMeObj(HOBJECT hTarget);
	void			AllDeleteHelpMeObj();


private:

	bool m_bIsDestroy;
	HELPME_MAP m_mapHelpMeList;
	CNpc* m_pBotNpc;

};

#endif
