#ifndef __INC_DBOG_MEHATERMANAGER_H__
#define __INC_DBOG_MEHATERMANAGER_H__


#include "ContainerMeHater.h"

#include "ObjectManager.h"
#include "CharacterObject.h"

class CCharacter;

class CMeHaterManager
{

public:
	CMeHaterManager();
	virtual	~CMeHaterManager();

public:

	bool				Create(CCharacter* pChar);
	void				Destroy();
	void				Init();
	CContainerMeHater*	GetContainerMeHater() { return &m_cContainerMeHater; }

	template <class _Func> bool ForEachNearRange(_Func & func)
	{
		struct sFOREACH_RANGE
		{
			std::multimap<float, CCharacterObject*> mapRange;
			CNtlVector vTemp;
			CCharacterObject *pOwner;

			void operator () (std::pair<HOBJECT, HOBJECT> rMeHater)
			{
				CCharacterObject* pNpc = g_pObjectManager->GetCharacterObject(rMeHater.second);
				if (pNpc)
				{
					vTemp = pOwner->GetCurLoc() - pNpc->GetCurLoc();
					vTemp.y = 0.0f;
					mapRange.insert(std::pair<float, CCharacterObject*>(vTemp.Length(), pNpc));
				}
			}
		};


		sFOREACH_RANGE sForEach;
		sForEach.pOwner = m_pChar;

		GetContainerMeHater()->ForEachCondition(sForEach);

		if (sForEach.mapRange.empty())
			return true;
		else
		{
			for (std::multimap<float, CCharacterObject*>::iterator it = sForEach.mapRange.begin(); it != sForEach.mapRange.end(); it++)
			{
				if (it->second)
					func(it->second);
			}
			return true;
		}

		return false;
	}

private:

	CCharacter* m_pChar;
	CContainerMeHater m_cContainerMeHater;

};

#endif
