#include "precomp_ntlpresentation.h"

#include "NtlWorldCommon.h"
#include "NtlOccluderManager.h"

#include "OccluderProxy.h"

COccluderProxy::COccluderProxy()
{
	m_pOccluderProxyData		= NULL;

	m_fOccluderQueryTime		= RwRealMAXVAL;
	m_uiOccluderTestNum			= 0;
}

COccluderProxy::~COccluderProxy()
{
	NTL_ARRAY_DELETE(m_pOccluderProxyData);
}

RwBool COccluderProxy::CreateOccluderProxy(RwReal fOccluderQueryTime, RwUInt32 uiTestNum)
{
	m_uiOccluderTestNum			= uiTestNum;
	m_fOccluderQueryTime		= fOccluderQueryTime;

	if (m_uiOccluderTestNum)
	{
		m_pOccluderProxyData		= NTL_NEW SOCCLDER_PROXY_DATA[m_uiOccluderTestNum];
		for (RwUInt32 i = 0; i < m_uiOccluderTestNum; ++i)
		{
			m_pOccluderProxyData[i].pPLOccluder					= NULL;
			m_pOccluderProxyData[i].eOccluderProxyState			= EOCCLUDER_CHECK;
			m_pOccluderProxyData[i].fOccluderQueryUpdateTime	= m_fOccluderQueryTime;
		}
	}

	return TRUE;
}

void COccluderProxy::DestroyOccluderProxy()
{
	CNtlOccluderManager::GetInstance()->UnRegisterProxyInOccluder(this);

	m_fOccluderQueryTime		= RwRealMAXVAL;	
	m_uiOccluderTestNum			= 0;

	NTL_ARRAY_DELETE(m_pOccluderProxyData);
}

RwBool COccluderProxy::OccluderQuery(RwUInt32 eType, RwUInt32 eFlag, void* pData, RwUInt32 uiTestCnt)
{
	if (m_fOccluderQueryTime == RwRealMAXVAL || m_uiOccluderTestNum <= uiTestCnt)
	{
		return FALSE;
	}

	return CNtlOccluderManager::GetInstance()->OccluderQuery(this, eType, eFlag, pData, uiTestCnt);
}