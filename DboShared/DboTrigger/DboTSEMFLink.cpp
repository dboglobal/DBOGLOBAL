#include "precomp_trigger.h"
#include "DboTSEMFLink.h"
#include "DboTSCoreDefine.h"
#include "NtlTSMemIO.h"


struct sFLINK_PARAM
{
	sFLINK_PARAM(void) : bFind(false) { return; }

	bool						bFind;
};

class CDboTSEMFLink_Recv : public CNtlTSRecv
{
public:
	NTL_TSRESULT				Search(CNtlTSEntity* pEntity, void* pParam);
	NTL_TSRESULT				Run(CNtlTSEntity*, void*) { return 0; }
};

NTL_TSRESULT CDboTSEMFLink_Recv::Search(CNtlTSEntity* pEntity, void* pParam)
{
	if (pEntity->GetEntityType() == DBO_EVENT_TYPE_ID_FLINK)
	{
		((sFLINK_PARAM*)pParam)->bFind = true;
	}

	return 0;
}


/**
Event mapper flink
*/


NTL_TS_IMPLEMENT_RTTI(CDboTSEMFLink, CNtlTSEvtMapper)


CDboTSEMFLink::CDboTSEMFLink(void)
{
}

CDboTSEMFLink::~CDboTSEMFLink(void)
{
}

const CNtlTSEvtMapper::vecdef_TID_LIST* CDboTSEMFLink::FindTSList(unsigned int uiId, const std::string& strKey) const
{
	UNREFERENCED_PARAMETER(uiId);
	UNREFERENCED_PARAMETER(strKey);

	return NULL;
}

void CDboTSEMFLink::Clear(void)
{
	m_mapFlink.clear();
}

bool CDboTSEMFLink::Load(const std::string& strFileName)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, strFileName.c_str(), "rb");

	if (NULL == pFile)
	{
		return false;
	}

	m_mapFlink.clear();

	//////////////////////////////////////////////////////////////////////////
	//
	//	Mapper counter
	//
	//////////////////////////////////////////////////////////////////////////
	int nMapperCnt;
	fread(&nMapperCnt, sizeof(nMapperCnt), 1, pFile);

	for (int nCurMapperCnt = 0; nCurMapperCnt < nMapperCnt; ++nCurMapperCnt)
	{
		sFLINK* pLink = new sFLINK;

		WORD questId = 0;
		fread(&questId, sizeof(questId), 1, pFile);

		fread(&pLink->nPrevQuestCount, sizeof(pLink->nPrevQuestCount), 1, pFile);

		for (int i = 0; i < pLink->nPrevQuestCount; i++)
		{
			WORD wId = INVALID_WORD;
			fread(&wId, sizeof(wId), 1, pFile);

			pLink->vecPrevQuestIds.push_back(wId);
		}

		fread(&pLink->nNexteQuestCount, sizeof(pLink->nNexteQuestCount), 1, pFile);

		for (int i = 0; i < pLink->nNexteQuestCount; i++)
		{
			WORD wId = INVALID_WORD;
			fread(&wId, sizeof(wId), 1, pFile);

			pLink->vecNextQuestIds.push_back(wId);
		}

		m_mapFlink.insert(std::make_pair(questId, pLink));
	}

	fclose(pFile);

	return true;
}

bool CDboTSEMFLink::Load(const char* pData, int nDataSize)
{
	CNtlTSMemInput clMemInput(pData, nDataSize);

	m_mapFlink.clear();

	//////////////////////////////////////////////////////////////////////////
	//
	//	Mapper counter
	//
	//////////////////////////////////////////////////////////////////////////
	int nMapperCnt;
	if (!clMemInput.Read(&nMapperCnt, sizeof(nMapperCnt)))
	{
		m_mapFlink.clear();
		return false;
	}

	for (int nCurMapperCnt = 0; nCurMapperCnt < nMapperCnt; ++nCurMapperCnt)
	{
		sFLINK* pLink = new sFLINK;

		WORD questId = 0;
		clMemInput.Read(&questId, sizeof(questId));

		clMemInput.Read(&pLink->nPrevQuestCount, sizeof(pLink->nPrevQuestCount));

		for (int i = 0; i < pLink->nPrevQuestCount; i++)
		{
			WORD wId = INVALID_WORD;
			clMemInput.Read(&wId, sizeof(wId));

			pLink->vecPrevQuestIds.push_back(wId);
		}

		clMemInput.Read(&pLink->nNexteQuestCount, sizeof(pLink->nNexteQuestCount));

		for (int i = 0; i < pLink->nNexteQuestCount; i++)
		{
			WORD wId = INVALID_WORD;
			clMemInput.Read(&wId, sizeof(wId));

			pLink->vecNextQuestIds.push_back(wId);
		}

		m_mapFlink.insert(std::make_pair(questId, pLink));
	}

	return true;
}

bool CDboTSEMFLink::Save(const std::string& strFileName)
{
	FILE* pFile = NULL;
	fopen_s(&pFile, strFileName.c_str(), "wb");

	if (NULL == pFile)
	{
		return false;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Mapper counter
	//
	//////////////////////////////////////////////////////////////////////////
	int nMapperCnt = (int)m_mapFlink.size();
	fwrite(&nMapperCnt, sizeof(nMapperCnt), 1, pFile);

	std::map<WORD, sFLINK*>::iterator itMapper = m_mapFlink.begin();
	for (; itMapper != m_mapFlink.end(); ++itMapper)
	{
		WORD questId = itMapper->first;
		fwrite(&questId, sizeof(questId), 1, pFile);

		sFLINK* pLink = itMapper->second;

		int nPrevCount = pLink->nPrevQuestCount;
		fwrite(&nPrevCount, sizeof(nPrevCount), 1, pFile);

		for (std::vector<WORD>::iterator it = pLink->vecPrevQuestIds.begin(); it != pLink->vecPrevQuestIds.end(); it++)
		{
			WORD wPrevQuestID = *it;
			fwrite(&wPrevQuestID, sizeof(wPrevQuestID), 1, pFile);
		}

		fwrite(&pLink->nNexteQuestCount, sizeof(pLink->nNexteQuestCount), 1, pFile);

		for (std::vector<WORD>::iterator it = pLink->vecNextQuestIds.begin(); it != pLink->vecNextQuestIds.end(); it++)
		{
			WORD wNextQuestID = *it;
			fwrite(&wNextQuestID, sizeof(wNextQuestID), 1, pFile);
		}
	}

	fclose(pFile);

	return true;
}

bool CDboTSEMFLink::AddBuildData(const std::string& strKey, const CNtlTSMain::mapdef_TLIST& TList, void* pTblList, void* pParameter)
{
	UNREFERENCED_PARAMETER(pTblList);
	UNREFERENCED_PARAMETER(pParameter);

	WORD questId = (WORD)std::atoi(strKey.c_str());

	/*if (m_mapFlink.find(questId) == m_mapFlink.end())
	{
		m_mapFlink[questId] = std::map<WORD, sFLINK*>();
	}

	std::map<WORD, sFLINK*> defTIDList = m_mapFlink[questId];

	

	sFLINK_PARAM sParam;
	CDboTSEMFLink_Recv clRecv;

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
			defTIDList.push_back(citTSBegin->first);
		}
	}*/

	return true;
}

bool CDboTSEMFLink::DelBuildData(const std::string& strKey)
{
	WORD questId = std::atoi(strKey.c_str());

	std::map<WORD, sFLINK*>	::iterator it = m_mapFlink.find(questId);
	if (it != m_mapFlink.end())
	{
		delete it->second;
		m_mapFlink.erase(it);
	}

	return true;
}

WORD CDboTSEMFLink::GetNextQuestID(WORD questId)
{
	std::map<WORD, sFLINK*>::iterator it = m_mapFlink.find(questId);
	if (it != m_mapFlink.end())
	{
		if (it->second->nNexteQuestCount > 0)
			return (WORD)it->second->vecNextQuestIds.front();
	}

	return INVALID_WORD;
}

