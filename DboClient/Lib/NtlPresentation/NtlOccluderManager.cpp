#include "precomp_ntlpresentation.h"

#include "NtlWorldCommon.h"
#include "NtlWorldInterface.h"

#include "NtlPLOccluder_Base.h"

#include "OccluderProxy.h"

#include "NtlOccluderManager.h"

#include "NtlProfiler.h"

CNtlOccluderManager* CNtlOccluderManager::g_pOccluderManager = NULL;

CNtlOccluderManager::CNtlOccluderManager()
{
	m_fErrorTime	= 1.0f;
	m_bActivation	= TRUE;
}

CNtlOccluderManager::~CNtlOccluderManager()
{
}

void CNtlOccluderManager::CreateInstance()
{
	if (!g_pOccluderManager)
	{
		g_pOccluderManager = NTL_NEW CNtlOccluderManager;
	}
}

void CNtlOccluderManager::DestroyInstance()
{
	NTL_DELETE(g_pOccluderManager);
}

RwBool CNtlOccluderManager::Create()
{
	NTL_FUNCTION("CNtlOccluderManager::Create");

	NTL_RETURN(TRUE);
}


void CNtlOccluderManager::Destroy()
{
	NTL_FUNCTION("CNtlOccluderManager::Destory");

	NTL_RETURNVOID();
}

RwBool CNtlOccluderManager::Update(RwReal fElapsed)
{
	NTL_FUNCTION("CNtlOccluderManager::Update");

	// Update Time에서 Occluder visibility를 클리어 해준다.
	m_vecOccluderVisibility.clear();

	/*RwV3d* pCameraPos = RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(CNtlPLGlobal::m_RwCamera)));

	m_fErrorTime -= fElapsed;
	if (m_fErrorTime < 0)
	{
		m_fErrorPos		= *pCameraPos;
		m_fErrorTime	= 0.25f;
		m_fErrorPoint	= 1.0f;
	}

	m_fErrorPoint += RwV3dLength(&(*pCameraPos - m_fErrorPos));
	if (m_fErrorPoint < 1.0f) 
	{
		m_fErrorPoint = 1.0f;
	}*/

	//char acTemp[256];
	//sprintf_s(acTemp, 256, "%f(Elapsed) * %f(ErrorPoint) = %f\n", g_GetElapsedTime(), m_fErrorPoint, g_GetElapsedTime() * m_fErrorPoint);
	//OutputDebugString(acTemp);
	
	NTL_RETURN(TRUE);
}

void CNtlOccluderManager::AddOccluderVisiblity(CNtlPLOccluder_Base* pPLOccluder)
{
	NTL_PRE(pPLOccluder);

	m_vecOccluderVisibility.push_back(pPLOccluder);
}

RwBool CNtlOccluderManager::OccluderQuery(COccluderProxy* pOccluderProxy, RwUInt32 eType, RwUInt32 eFlag, void* pData, RwUInt32 uiTestCnt)
{
	NTL_SPROFILE("CNtlOccluderManager::OccluderQuery");

 	NTL_PRE(pOccluderProxy);
	NTL_PRE(pData);

	if (!m_bActivation)
	{
		NTL_RPROFILE(FALSE);
		//return FALSE;
	}
#ifdef dNTL_WORLD_TOOL_MODE
	else if (!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_OCCLUDER_ACTIVATION))
	{	
		NTL_RPROFILE(FALSE);
		//return FALSE;
	}
#endif

	if (pOccluderProxy->m_pOccluderProxyData[uiTestCnt].eOccluderProxyState == COccluderProxy::EOCCLUDER_CULLED &&
		pOccluderProxy->m_pOccluderProxyData[uiTestCnt].fOccluderQueryUpdateTime > 0.01f)
	{
		pOccluderProxy->m_pOccluderProxyData[uiTestCnt].fOccluderQueryUpdateTime = 0.01f;
	}
	
	pOccluderProxy->m_pOccluderProxyData[uiTestCnt].fOccluderQueryUpdateTime -= g_GetElapsedTime();
	if (pOccluderProxy->m_pOccluderProxyData[uiTestCnt].fOccluderQueryUpdateTime < 0.0f)
	{
		pOccluderProxy->m_pOccluderProxyData[uiTestCnt].fOccluderQueryUpdateTime = pOccluderProxy->m_fOccluderQueryTime;
		COccluderProxy::EOccluderProxyState eState = pOccluderProxy->m_pOccluderProxyData[uiTestCnt].eOccluderProxyState;
		switch (eState)
		{
		case COccluderProxy::EOCCLUDER_CHECK: // 오클루더 체크해야 한다.
			pOccluderProxy->m_pOccluderProxyData[uiTestCnt].pPLOccluder			= NULL;
			pOccluderProxy->m_pOccluderProxyData[uiTestCnt].eOccluderProxyState	= COccluderProxy::EOCCLUDER_NONE;
		case COccluderProxy::EOCCLUDER_NONE: // 오클루더 체크 영역
			{
				if (m_vecOccluderVisibility.empty())
				{
					NTL_RPROFILE((pOccluderProxy->m_pOccluderProxyData[uiTestCnt].eOccluderProxyState == COccluderProxy::EOCCLUDER_CULLED ? TRUE : FALSE));
				}

				VEC_OCC_IT it		= m_vecOccluderVisibility.begin();
				VEC_OCC_IT itEnd	= m_vecOccluderVisibility.end();

				do 
				{
					RwBool					bCulled		= TRUE;
					CNtlPLOccluder_Base*	pPLOccluder	= *it;

					if (!(pPLOccluder->GetOccluderFuncFlag() & eFlag))
					{
						continue;
					}
#ifdef dNTL_WORLD_TOOL_MODE // Tool에서만 로드 하기 때문에.. 툴코드로 넣는다.
					else if (pPLOccluder->IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS))
					{
						continue;
					}
#endif

					switch (eType)
					{
					case COccluderProxy::EOCCLUDER_SPHERE:
						{
							bCulled = pPLOccluder->OccluderTestSphere(static_cast<RwSphere*>(pData));
							break;
						}
					case COccluderProxy::EOCCLUDER_BOX:
						{
							bCulled = pPLOccluder->OccluderTestBox(static_cast<RwBBox*>(pData));
							break;
						}
					}

					if (bCulled)
					{
						pOccluderProxy->m_pOccluderProxyData[uiTestCnt].pPLOccluder				= pPLOccluder;
						pOccluderProxy->m_pOccluderProxyData[uiTestCnt].eOccluderProxyState		= COccluderProxy::EOCCLUDER_CULLED;

						MAP_OCC_IT itMap = m_mapOccluder.find(pPLOccluder);

						if (itMap != m_mapOccluder.end())
						{	
							PAIR_PROXY pair;
							pair.first	= pOccluderProxy;
							pair.second = uiTestCnt;
							m_mapOccluder[pPLOccluder].push_back(pair);
						}
						break;
					}
				} while (++it != itEnd);
				break;
			}
		case COccluderProxy::EOCCLUDER_CULLED: // 컬링된 오브젝트라면 자신을 가린 오클루더에게 다시 묻는다.
			{
				RwBool bCulled = TRUE;
				if (!pOccluderProxy->m_pOccluderProxyData[uiTestCnt].pPLOccluder->IsRunable())
				{
					bCulled = FALSE;
				}
				else
				{
					switch (eType)
					{
					case COccluderProxy::EOCCLUDER_SPHERE:
						{
							bCulled = pOccluderProxy->m_pOccluderProxyData[uiTestCnt].pPLOccluder->OccluderTestSphere(static_cast<RwSphere*>(pData));
							break;
						}
					case COccluderProxy::EOCCLUDER_BOX:
						{
							bCulled = pOccluderProxy->m_pOccluderProxyData[uiTestCnt].pPLOccluder->OccluderTestBox(static_cast<RwBBox*>(pData));
							break;
						}
					}
				}

				if (!bCulled)
				{
					VEC_PROXY& vecProxy = m_mapOccluder[pOccluderProxy->m_pOccluderProxyData[uiTestCnt].pPLOccluder];
					for (VEC_PROXY_IT itProxy = vecProxy.begin(); itProxy != vecProxy.end(); ++itProxy)
					{
						PAIR_PROXY pair = *itProxy;
						if (pair.first == pOccluderProxy && pair.second == uiTestCnt)
						{
							vecProxy.erase(itProxy);
							break;
						}
					}

					pOccluderProxy->m_pOccluderProxyData[uiTestCnt].pPLOccluder					= NULL;
					pOccluderProxy->m_pOccluderProxyData[uiTestCnt].eOccluderProxyState			= COccluderProxy::EOCCLUDER_CHECK;
					pOccluderProxy->m_pOccluderProxyData[uiTestCnt].fOccluderQueryUpdateTime	+= pOccluderProxy->m_fOccluderQueryTime; // 컬링 되었다가 다시 보인 것은 업데이트 타임을 늘린다.
				}
				break;
			}
		}		
	}
	NTL_RPROFILE((pOccluderProxy->m_pOccluderProxyData[uiTestCnt].eOccluderProxyState == COccluderProxy::EOCCLUDER_CULLED ? TRUE : FALSE));
}

RwBool CNtlOccluderManager::OccluderQuery(RwUInt32 eType, RwUInt32 eFlag, void* pData, RwInt8 iNum)
{
	NTL_SPROFILE("CNtlOccluderManager::OccluderQuery");

	NTL_PRE(pData);
	NTL_PRE(iNum > 0);

	if (!m_bActivation)
	{
		NTL_RPROFILE(FALSE);
	}
#ifdef dNTL_WORLD_TOOL_MODE
	else if (!CNtlWorldObjVisionAnalyzer::GetInstance()->GetCheck(eNWOVA_OCCLUDER_ACTIVATION))
	{	
		NTL_RPROFILE(FALSE);
	}
#endif
			
	if (m_vecOccluderVisibility.empty())
	{
		NTL_RPROFILE(FALSE);
	}

	VEC_OCC_IT it		= m_vecOccluderVisibility.begin();
	VEC_OCC_IT itEnd	= m_vecOccluderVisibility.end();

	do 
	{
		RwBool					bCulled		= TRUE;
		CNtlPLOccluder_Base*	pPLOccluder	= *it;

		if (!(pPLOccluder->GetOccluderFuncFlag() & eFlag))
		{
			continue;
		}
#ifdef dNTL_WORLD_TOOL_MODE // Tool에서만 로드 하기 때문에.. 툴코드로 넣는다.
		else if (pPLOccluder->IsOccluderFuncFlag(EPLOCCLUDER_FUNC_PVS))
		{
			continue;
		}
#endif

		RwInt32 i = 0;
		do
		{
			switch (eType)
			{
			case COccluderProxy::EOCCLUDER_SPHERE:
				bCulled &= pPLOccluder->OccluderTestSphere(static_cast<RwSphere*>(pData) + i);
				break;
			case COccluderProxy::EOCCLUDER_BOX:
				bCulled &= pPLOccluder->OccluderTestBox(static_cast<RwBBox*>(pData) + i);
				break;
			}
		} while (++i < iNum && bCulled);

		if (bCulled)
		{
			return TRUE;
		}
	} while (++it != itEnd);
	
	return FALSE;
}

RwBool CNtlOccluderManager::RegisterOccluder(CNtlPLOccluder_Base* pPLOccluder)
{
	MAP_OCC_IT itMap = m_mapOccluder.find(pPLOccluder);
	if (itMap != m_mapOccluder.end())
	{
		return FALSE;
	}
	
	m_mapOccluder[pPLOccluder] = VEC_PROXY();

	return TRUE;
}

void CNtlOccluderManager::UnRegisterOccluder(CNtlPLOccluder_Base* pPLOccluder)
{
	MAP_OCC_IT itMap = m_mapOccluder.find(pPLOccluder);
	if (itMap != m_mapOccluder.end())
	{
		VEC_PROXY& vecProxy = itMap->second;
		for (VEC_PROXY_IT itProxy = vecProxy.begin(); itProxy != vecProxy.end(); ++itProxy)
		{
			PAIR_PROXY& pair = *itProxy;
			
			pair.first->m_pOccluderProxyData[pair.second].pPLOccluder			= NULL;
			pair.first->m_pOccluderProxyData[pair.second].eOccluderProxyState	= COccluderProxy::EOCCLUDER_CHECK;
		}
		m_mapOccluder.erase(itMap);
	}
}

void CNtlOccluderManager::UnRegisterProxyInOccluder(COccluderProxy* pOccluderProxy)
{
	// 객체는 언제든지 파괴될 수 있다. 그러므로 파괴될 때 리스트에서 빼주어야 한다.
	for (RwUInt32 i = 0; i < pOccluderProxy->m_uiOccluderTestNum; ++i)
	{
		CNtlPLOccluder_Base* pOccluder = pOccluderProxy->m_pOccluderProxyData[i].pPLOccluder;
		if (pOccluder)
		{
			MAP_OCC_IT itMap = m_mapOccluder.find(pOccluder);
			if (itMap != m_mapOccluder.end())
			{
				VEC_PROXY& vecProxy = itMap->second;
				for (VEC_PROXY_IT itProxy = vecProxy.begin(); itProxy != vecProxy.end(); ++itProxy)
				{
					PAIR_PROXY& pair = *itProxy;

					if (pair.first == pOccluderProxy)
					{
						vecProxy.erase(itProxy);
						break;
					}
				}


			}
		}
	}
}

RwInt32 CNtlOccluderManager::GetOccluderCulledNum(CNtlPLOccluder_Base* pPLOccluder)
{
	MAP_OCC_IT itMap = m_mapOccluder.find(pPLOccluder); 
	if (itMap != m_mapOccluder.end())
	{
		return static_cast<RwInt32>(itMap->second.size());
	}
	return 0;
}

void CNtlOccluderManager::SetActivation(RwBool bActivation)
{
	m_bActivation = bActivation;
	if (!m_bActivation)	
	{
		ClearAllOccluderProxyList();			
	}
}

void CNtlOccluderManager::ClearOccluderProxyList(CNtlPLOccluder_Base* pPLOccluder)
{
	MAP_OCC_IT itMap = m_mapOccluder.find(pPLOccluder);
	if (itMap != m_mapOccluder.end())
	{
		VEC_PROXY& vecProxy = itMap->second;
		for (VEC_PROXY_IT itProxy = vecProxy.begin(); itProxy != vecProxy.end(); ++itProxy)
		{
			PAIR_PROXY& pair = *itProxy;

			pair.first->m_pOccluderProxyData[pair.second].pPLOccluder			= NULL;
			pair.first->m_pOccluderProxyData[pair.second].eOccluderProxyState	= COccluderProxy::EOCCLUDER_CHECK;
		}
		vecProxy.clear();
	}
}

void CNtlOccluderManager::ClearAllOccluderProxyList()
{
	for (MAP_OCC_IT itMap = m_mapOccluder.begin(); itMap != m_mapOccluder.end(); ++itMap)
	{
		VEC_PROXY& vecProxy = itMap->second;
		for (VEC_PROXY_IT itProxy = vecProxy.begin(); itProxy != vecProxy.end(); ++itProxy)
		{
			PAIR_PROXY& pair = *itProxy;

			pair.first->m_pOccluderProxyData[pair.second].pPLOccluder			= NULL;
			pair.first->m_pOccluderProxyData[pair.second].eOccluderProxyState	= COccluderProxy::EOCCLUDER_CHECK;
		}
		vecProxy.clear();
	}
}