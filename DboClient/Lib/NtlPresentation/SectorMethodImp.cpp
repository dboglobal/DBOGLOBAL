#include "precomp_ntlpresentation.h"
#include "SectorMethodImp.h"
#include "NtlPLSceneManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldFieldManager.h"
#include "ISectorMethod.h"


CSectorMethodImp::CSectorMethodImp(DWORD _MethodStyle) : ISectorMethodContainer(_MethodStyle)
{
}

CSectorMethodImp::~CSectorMethodImp(void)
{
	for(RwUInt32 i = 0; i < m_vecSM.size(); ++i)
	{
		m_vecSM[i]->DestroySelfRetMap();
	}
}

RwBool CSectorMethodImp::Analyze()
{
	for(RwUInt32 i = 0; i < m_vecSM.size(); ++i)
	{
		m_vecSM[i]->DestroySelfRetMap();
	}

	CNtlWorldFieldManager*	_pMgr		= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	CNtlWorldField*			_pField		= const_cast<CNtlWorldField*>(_pMgr->GetFields());
	RwInt32					SectorCnt	= 0;
	RwV3d					SPos;
	RwV3d					SectorSPos;
	RwInt32					SectorIdx;
	VOID*					pAnalysis;
	RwBool					AnalyzeRet;

	for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum; ++i)
	{
		SectorCnt	= 0;
		SPos		= _pField[i].GetSPos();

		for(RwInt32 l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(RwInt32 m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x	= (RwReal)m;
				SectorSPos.z	= (RwReal)l;
				SectorIdx		= _pMgr->GetSectorIdx(SectorSPos);

				for(RwUInt32 k = 0; k < m_vecSM.size(); ++k)
				{
					AnalyzeRet = m_vecSM[k]->Analyze(SectorIdx);					
					if(AnalyzeRet)
					{						
						pAnalysis = m_vecSM[k]->GetAnalysis();
						m_vecSM[k]->m_SelfRetMap.insert(ISectorMethod::tSELF_VALUE_TYPE(SectorIdx, pAnalysis));
					}
				}
			}
		}	
	}

	return TRUE;
}

RwBool CSectorMethodImp::Execute()
{
	for(RwUInt32 i = 0; i < m_vecSM.size(); ++i)
	{
		m_vecSM[i]->Execute(eSMETI_SECTOR);
	}

	return TRUE;
}

RwBool CSectorMethodImp::Report()
{
	for(RwUInt32 i = 0; i < m_vecSM.size(); ++i)
	{
		m_vecSM[i]->Report();
	}

	return TRUE;
}