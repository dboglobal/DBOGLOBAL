#ifndef __INC_DBOG_CONTAINERMEHATER_H__
#define __INC_DBOG_CONTAINERMEHATER_H__

#include "NtlSharedType.h"
class CCharacter;
class CMeHaterManager;

class CContainerMeHater
{

	enum eRESULTCODE
	{
		eSUCCESS = 0x0,
		eFAIL = 0x1,
		eCONTINUE = 0x2,
	};

	typedef std::map<HOBJECT, HOBJECT>	MEHATER_MAP;
	typedef MEHATER_MAP::value_type MEHATER_VALUE;

public:
	CContainerMeHater();
	virtual	~CContainerMeHater();

public:

	bool				Create(CCharacter* pChar, CMeHaterManager* pMeHaterManager);
	void				Destroy();

	bool				AddMeHater(HOBJECT hMeHater);
	bool				ModefiMeHater(HOBJECT hMeHater, HOBJECT hModifyObj);
	void				DeleteMeHater(HOBJECT hMeHater);
	void				AllDeleteMeHater();

	bool				IsMeHater();
	int					GetMeHaterCount();

	template <class _Func> eRESULTCODE ForEachCondition(_Func& func)
	{
		for (MEHATER_MAP::iterator it = m_mapMeHater.begin(); it != m_mapMeHater.end(); it++)
		{
			func(*it);
		}

		return eSUCCESS;
	}

private:

	CCharacter*			m_pChar;
	CMeHaterManager*	m_pMeHaterManager;

	MEHATER_MAP			m_mapMeHater;

};

#endif