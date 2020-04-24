#include "precomp_ntlpresentation.h"
#include "FieldMethodImp.h"
#include "NtlPLSceneManager.h"
#include "NtlPLWorldEntity.h"
#include "NtlWorldFieldManager.h"
#include "ISectorMethod.h"


CFieldMethodImp::CFieldMethodImp(DWORD _MethodStyle) : ISectorMethodContainer(_MethodStyle)
{
}

CFieldMethodImp::~CFieldMethodImp(void)
{
	for(RwUInt32 i = 0; i < m_vecSM.size(); ++i)
	{
		m_vecSM[i]->DestroySelfRetMap();
	}
}

RwBool CFieldMethodImp::Analyze()
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
	RwInt32					SectorIdx[4];
	VOID*					pAnalysis[4];
	RwBool					AnalyzeRet[4];

	for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum; ++i)
	{
		SectorCnt	= 0;
		SPos		= _pField[i].GetSPos();

		for(RwInt32 l = (int)SPos.z; l < (int)SPos.z + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			for(RwInt32 m = (int)SPos.x; m < (int)SPos.x + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
			{
				SectorSPos.x			= (RwReal)m;
				SectorSPos.z			= (RwReal)l;
				SectorIdx[SectorCnt++]	= _pMgr->GetSectorIdx(SectorSPos);
			}
		}	

		for(RwUInt32 k = 0; k < m_vecSM.size(); ++k)
		{
			AnalyzeRet[0] = m_vecSM[k]->Analyze(SectorIdx[0]); if (!AnalyzeRet[0]) continue;
			AnalyzeRet[1] = m_vecSM[k]->Analyze(SectorIdx[1]); if (!AnalyzeRet[1]) continue;	
			AnalyzeRet[2] = m_vecSM[k]->Analyze(SectorIdx[2]); if (!AnalyzeRet[2]) continue;	
			AnalyzeRet[3] = m_vecSM[k]->Analyze(SectorIdx[3]); if (!AnalyzeRet[3]) continue;

			AnalyzeRet[0] = m_vecSM[k]->Analyze(SectorIdx[0]);	pAnalysis[0] = m_vecSM[k]->GetAnalysis();
			AnalyzeRet[1] = m_vecSM[k]->Analyze(SectorIdx[1]);	pAnalysis[1] = m_vecSM[k]->GetAnalysis();
			AnalyzeRet[2] = m_vecSM[k]->Analyze(SectorIdx[2]);	pAnalysis[2] = m_vecSM[k]->GetAnalysis();
			AnalyzeRet[3] = m_vecSM[k]->Analyze(SectorIdx[3]);	pAnalysis[3] = m_vecSM[k]->GetAnalysis();

			m_vecSM[k]->m_SelfRetMap.insert(ISectorMethod::tSELF_VALUE_TYPE(SectorIdx[0], pAnalysis[0]));
			m_vecSM[k]->m_SelfRetMap.insert(ISectorMethod::tSELF_VALUE_TYPE(SectorIdx[1], pAnalysis[1]));
			m_vecSM[k]->m_SelfRetMap.insert(ISectorMethod::tSELF_VALUE_TYPE(SectorIdx[2], pAnalysis[2]));
			m_vecSM[k]->m_SelfRetMap.insert(ISectorMethod::tSELF_VALUE_TYPE(SectorIdx[3], pAnalysis[3]));

// 			AnalyzeRet[0] = m_vecSM[k]->Analyze(SectorIdx[0]);	pAnalysis[0] = m_vecSM[k]->GetAnalysis();
// 			AnalyzeRet[1] = m_vecSM[k]->Analyze(SectorIdx[1]);	pAnalysis[1] = m_vecSM[k]->GetAnalysis();
// 			AnalyzeRet[2] = m_vecSM[k]->Analyze(SectorIdx[2]);	pAnalysis[2] = m_vecSM[k]->GetAnalysis();
// 			AnalyzeRet[3] = m_vecSM[k]->Analyze(SectorIdx[3]);	pAnalysis[3] = m_vecSM[k]->GetAnalysis();
// 
// 			if(AnalyzeRet[0] && AnalyzeRet[1] && AnalyzeRet[2] && AnalyzeRet[3])
// 			{
// 				m_vecSM[k]->m_SelfRetMap.insert(ISectorMethod::tSELF_VALUE_TYPE(SectorIdx[0], pAnalysis[0]));
// 				m_vecSM[k]->m_SelfRetMap.insert(ISectorMethod::tSELF_VALUE_TYPE(SectorIdx[1], pAnalysis[1]));
// 				m_vecSM[k]->m_SelfRetMap.insert(ISectorMethod::tSELF_VALUE_TYPE(SectorIdx[2], pAnalysis[2]));
// 				m_vecSM[k]->m_SelfRetMap.insert(ISectorMethod::tSELF_VALUE_TYPE(SectorIdx[3], pAnalysis[3]));
// 			}
// 			else
// 			{
// 				// 메모리 누수 남. void 형 삭제로 인해 소멸자 호출이 안됨.
// 				NTL_DELETE(pAnalysis[0]);
// 				NTL_DELETE(pAnalysis[1]);
// 				NTL_DELETE(pAnalysis[2]);
// 				NTL_DELETE(pAnalysis[3]);
// 			}
		}
	}

	return TRUE;
}

RwBool CFieldMethodImp::Execute()
{
	for(RwUInt32 i = 0; i < m_vecSM.size(); ++i)
	{
		m_vecSM[i]->Execute(eSMETI_FIELD);
	}

	return TRUE;
}

RwBool CFieldMethodImp::Report()
{
	for(RwUInt32 i = 0; i < m_vecSM.size(); ++i)
	{
		m_vecSM[i]->Report();
	}

	return TRUE;
}