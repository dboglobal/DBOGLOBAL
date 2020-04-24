#include "precomp_ntlpresentation.h"
#include "NtlPLCullingScheduling.h"


#include "NtlTimer.h"

#include "NtlMath.h"
#include "NtlPLDef.h"
#include "NtlPLGlobal.h"

#ifndef dNTL_WORLD_CULLING_NEW

CNtlPLCullingOperator::CNtlPLCullingOperator()
{
	m_iMaxCullAtomicNum = 500;
	m_iCurrAtomicNum	= 0;

	m_sCullOp.iStartPointer = 0;
	m_sCullOp.iEndPointer = 0;

	m_bFpsRatio = TRUE;
}

CNtlPLCullingOperator::~CNtlPLCullingOperator()
{
}

RwReal CNtlPLCullingOperator::GetCullingFpsRatio(void)
{
	if(!m_bFpsRatio)
		return 1.0f;

	RwReal fFps = CNtlTimer::GetFps();

	if(fFps < 5.0f)
		return 0.3f;
	else if(fFps < 10.0f)
		return 0.5f;
	else if(fFps < 20.0f)
		return 0.85f;

	return 1.0f;
}

void CNtlPLCullingOperator::Update(RwReal fElapsed)
{
	if(IsFinish())
	{
		return;
	}

	m_iCurrAtomicNum = 0;

	RwInt32 iMaxCullAtomicNum = (RwInt32)( GetCullingFpsRatio() * (RwReal)m_iMaxCullAtomicNum );

	CNtlPLEntity *pPLEntity;

	while(1)
	{
		pPLEntity = m_sCullOp.pPLEntity[m_sCullOp.iStartPointer];
		if(pPLEntity)
		{
			pPLEntity->CullingTest(CNtlPLGlobal::m_RwCamera);
		}

		m_sCullOp.iStartPointer++;

		if(m_sCullOp.iStartPointer >= m_sCullOp.iEndPointer)
		{
			m_sCullOp.iStartPointer = 0;
			m_sCullOp.iEndPointer = 0;
			break;
		}

		// ½ºÄÉÁì °¹¼ö¸¦ ³Ñ¾î¼­¸é...
		if(m_iCurrAtomicNum >= iMaxCullAtomicNum)
		{
			break;
		}
	}
}

void CNtlPLCullingOperator::AddCulling(CNtlPLEntity *pPLEntity)
{
	pPLEntity->SetCullingID(m_sCullOp.iEndPointer);

	m_sCullOp.pPLEntity[m_sCullOp.iEndPointer++] = pPLEntity;
}

void CNtlPLCullingOperator::RemoveCulling(CNtlPLEntity *pPLEntity)
{
	if(pPLEntity == NULL)
		return;

	RwUInt32 iCullID = pPLEntity->GetCullingID();
	if(iCullID == 0xffffffff)
	{
		return;
	}

	m_sCullOp.pPLEntity[iCullID] = NULL;
}

void CNtlPLCullingOperator::EnableFpsRatio(RwBool bEnable)
{
	m_bFpsRatio = bEnable;
}

RwBool CNtlPLCullingOperator::IsFinish(void)
{
	if(m_sCullOp.iStartPointer >= m_sCullOp.iEndPointer)
		return TRUE;

	return FALSE;
}

void CNtlPLCullingOperator::SetMaxCullAtomicNum(RwInt32 iNum)
{
	m_iMaxCullAtomicNum = iNum;
}

void CNtlPLCullingOperator::IncAtomicOperator(void)	
{
	m_iCurrAtomicNum++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

CNtlPLCullingScheduling* CNtlPLCullingScheduling::m_pInstance = NULL;


CNtlPLCullingScheduling::CNtlPLCullingScheduling()
{
	m_CullObjOp.SetMaxCullAtomicNum(1000);
	m_CullObjOp.EnableFpsRatio(FALSE);

	m_pInstance = this;
}

CNtlPLCullingScheduling::~CNtlPLCullingScheduling()
{
	m_pInstance = NULL;
}

CNtlPLCullingScheduling* CNtlPLCullingScheduling::GetInstance(void)
{
	return m_pInstance;
}

void CNtlPLCullingScheduling::FillObjOp(void)
{
	MapCulling::iterator it;

	for(it = m_mapObjCulling.begin(); it != m_mapObjCulling.end(); it++)
	{
		m_CullObjOp.AddCulling( (*it).second );
	}
}

void CNtlPLCullingScheduling::FillOp(void)
{
	MapCulling::iterator it;
	
	for(it = m_mapCharCulling.begin(); it != m_mapCharCulling.end(); it++)
	{
		m_CullOp.AddCulling( (*it).second );
	}

	for(it = m_mapEffCulling.begin(); it != m_mapEffCulling.end(); it++)
	{
		m_CullOp.AddCulling( (*it).second );
	}
}

void CNtlPLCullingScheduling::Update(RwReal fElapsed)
{
	// object culling

	m_CullObjOp.Update(fElapsed);

	if(m_CullObjOp.IsFinish())
	{
		FillObjOp();
	}

	// 
	m_CullOp.Update(fElapsed);

	if(m_CullOp.IsFinish())
	{
		FillOp();
	}
}

void CNtlPLCullingScheduling::AddCulling(CNtlPLEntity *pPLEntity)
{
	if(pPLEntity == NULL)
		return;

	MapCulling::iterator it;

	ENtlPLEntityType eType = pPLEntity->GetClassType();

	if(eType == PLENTITY_OBJECT)
	{
		it = m_mapObjCulling.find(pPLEntity);
		if(it != m_mapObjCulling.end())
			return;
		m_mapObjCulling[pPLEntity] = pPLEntity;
	}
	else if(eType == PLENTITY_CHARACTER)
	{
		it = m_mapCharCulling.find(pPLEntity);
		if(it != m_mapCharCulling.end())
			return;
		m_mapCharCulling[pPLEntity] = pPLEntity;
	}
	else if(eType == PLENTITY_EFFECT)
	{
		it = m_mapEffCulling.find(pPLEntity);
		if(it != m_mapEffCulling.end())
			return;
		m_mapEffCulling[pPLEntity] = pPLEntity;
	}
}

void CNtlPLCullingScheduling::RemoveCulling(RwUInt32 uiClassType, CNtlPLEntity *pPLEntity)
{
	MapCulling::iterator it;

	if(uiClassType == PLENTITY_OBJECT)
	{
		it = m_mapObjCulling.find(pPLEntity);
		if(it == m_mapObjCulling.end())
			return;

		m_mapObjCulling.erase(it);

		m_CullObjOp.RemoveCulling(pPLEntity);
	}
	else if(uiClassType == PLENTITY_CHARACTER)
	{
		it = m_mapCharCulling.find(pPLEntity);
		if(it == m_mapCharCulling.end())
			return;

		m_mapCharCulling.erase(it);

		m_CullOp.RemoveCulling(pPLEntity);
	}
	else if(uiClassType == PLENTITY_EFFECT)
	{
		it = m_mapEffCulling.find(pPLEntity);
		if(it == m_mapEffCulling.end())
			return;

		m_mapEffCulling.erase(it);

		m_CullOp.RemoveCulling(pPLEntity);
	}
}


void CNtlPLCullingScheduling::SetMaxCullAtomicNum(RwUInt32 uiClassType, RwInt32 iNum)
{
	if(uiClassType == PLENTITY_OBJECT)
		m_CullObjOp.SetMaxCullAtomicNum(iNum);
	else
		m_CullOp.SetMaxCullAtomicNum(iNum);
}

void CNtlPLCullingScheduling::IncAtomicOperator(RwUInt32 uiClassType)
{
	if(uiClassType == PLENTITY_OBJECT)
		m_CullObjOp.IncAtomicOperator();
	else
		m_CullOp.IncAtomicOperator();
}

#endif