#include "precomp_trigger.h"
#include "DboTSEMSideIcon.h"
#include "DboTSCoreDefine.h"
#include "NtlTSMemIO.h"

#include "DboTSClickSideIcon.h"


/**
Event mapper side icon
*/

struct sSIDEICON_PARAM
{
	sSIDEICON_PARAM(void) : bFind(false), bySideIconType(INVALID_BYTE) { return; }

	bool						bFind;
	BYTE						bySideIconType;
};

class CDboTSEMSideIcon_Recv : public CNtlTSRecv
{
public:
	NTL_TSRESULT				Search(CNtlTSEntity* pEntity, void* pParam);
	NTL_TSRESULT				Run(CNtlTSEntity*, void*) { return 0; }
};

NTL_TSRESULT CDboTSEMSideIcon_Recv::Search(CNtlTSEntity* pEntity, void* pParam)
{
	if (pEntity->GetEntityType() == DBO_EVENT_TYPE_ID_CLICK_SIDEICON)
	{
		((sSIDEICON_PARAM*)pParam)->bFind = true;

		CDboTSClickSideIcon* pTS = (CDboTSClickSideIcon*)pEntity;
		((sSIDEICON_PARAM*)pParam)->bySideIconType = pTS->GetSideIconType();
	}

	return 0;
}


NTL_TS_IMPLEMENT_RTTI(CDboTSEMSideIcon, CNtlTSEvtMapper)


CDboTSEMSideIcon::CDboTSEMSideIcon(void)
{
}

CDboTSEMSideIcon::~CDboTSEMSideIcon(void)
{
}

bool CDboTSEMSideIcon::AddBuildData(const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter)
{
	UNREFERENCED_PARAMETER(pTblList);
	UNREFERENCED_PARAMETER(pParameter);

	sSIDEICON_PARAM sParam;
	CDboTSEMSideIcon_Recv clRecv;

	CNtlTSTrigger* pTrig;
	CNtlTSMain::mapdef_TLIST::const_iterator citTSBegin = TList.begin();
	CNtlTSMain::mapdef_TLIST::const_iterator citTSEnd = TList.end();

	for (; citTSBegin != citTSEnd; ++citTSBegin)
	{
		pTrig = citTSBegin->second;

		sParam.bFind = false;

		pTrig->SearchTarget(NTL_TS_MAIN_GROUP_ID, START_CONTAINER_ID, &clRecv, &sParam);

		if (sParam.bFind)
		{
			if (m_defMapper.find(sParam.bySideIconType) == m_defMapper.end())
			{
				m_defMapper[sParam.bySideIconType][strKey] = vecdef_TID_LIST();

				m_defMapper[sParam.bySideIconType][strKey].push_back(pTrig->GetID());
			}
			else
			{
				if (m_defMapper[sParam.bySideIconType].find(strKey) == m_defMapper[sParam.bySideIconType].end())
				{
					m_defMapper[sParam.bySideIconType][strKey] = vecdef_TID_LIST();

					m_defMapper[sParam.bySideIconType][strKey].push_back(pTrig->GetID());
				}
				else
				{
					m_defMapper[sParam.bySideIconType][strKey].push_back(pTrig->GetID());
				}
			}
		}
	}

	return true;
}

bool CDboTSEMSideIcon::DelBuildData(const std::string& strKey)
{
	mapdef_MAPPER::iterator itMapper = m_defMapper.begin();
	for (; itMapper != m_defMapper.end(); ++itMapper)
	{
		hashdef_TS_LIST& TSList = itMapper->second;
		hashdef_TS_LIST::iterator itFind = TSList.find(strKey);
		if (itFind != TSList.end())
		{
			TSList.erase(itFind);
		}
	}

	return true;
}

