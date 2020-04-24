#include "precomp_ntlsimulation.h"
#include "NtlTriggerDirectSync.h"

// core
#include "NtlDebug.h"

// simulation
#include "NtlSLEvent.h"
#include "NtlSLEventFunc.h"

DEFINITION_MEMORY_POOL(CNtlTCUnit)

CNtlTCUnit::CNtlTCUnit()
{
	m_hHandle = INVALID_TC_HANDLE;
}

CNtlTCUnit::~CNtlTCUnit()
{
	m_hHandle = INVALID_TC_HANDLE;
}

void CNtlTCUnit::SetHandle(const TC_HANDLE hHandle)
{
	m_hHandle = hHandle;
}

TC_HANDLE CNtlTCUnit::GetHandle(void) const
{
	return m_hHandle;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// tirgger community unit register

DEFINITION_MEMORY_POOL(CNtlTCUnitReg)

CNtlTCUnitReg::CNtlTCUnitReg()
{
	m_iRef = 0;
	m_eTDType = ETD_None;
}

CNtlTCUnitReg::~CNtlTCUnitReg()
{
	m_iRef = 0;
	m_eTDType = ETD_None;
}

void CNtlTCUnitReg::SetTSKey(const sTS_KEY& key)
{
	m_sKey = key;
}

const sTS_KEY& CNtlTCUnitReg::GetTSKey(void) const
{
	return m_sKey;
}

void CNtlTCUnitReg::SetTriggerDirectType(ETriggerDirectType eTDType)
{
	m_eTDType = eTDType;
}

ETriggerDirectType CNtlTCUnitReg::GetTriggerDirectType(void) const
{
	return m_eTDType;
}

void CNtlTCUnitReg::SetData(const void *pData, RwInt32 iSize)
{
	NTL_ASSERT(iSize < MAX_TC_UNIT_DATA, "CNtlTCUnitReg::SetData is data size over!!!");
	memcpy(m_chData, pData, iSize); 
}

const RwChar* CNtlTCUnitReg::GetData(void)
{
	return m_chData;
}

void CNtlTCUnitReg::IncRef(void)
{
	m_iRef++;
}

void CNtlTCUnitReg::DecRef(void)
{
	m_iRef--;
}

RwBool CNtlTCUnitReg::IsValid(void)
{
	return (m_iRef >= 0) ? TRUE : FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
// tirgger community manager

RwUInt32 CNtlTriggerSyncManager::m_uiHandle = 0;
CNtlTriggerSyncManager* CNtlTriggerSyncManager::m_pInstance = NULL;

CNtlTriggerSyncManager::CNtlTriggerSyncManager()
{
	NTL_ASSERT(m_pInstance == NULL, "CNtlTriggerCommunity::CNtlTriggerCommunity()");
	m_pInstance = this;
}

CNtlTriggerSyncManager::~CNtlTriggerSyncManager()
{
	m_pInstance = NULL;
}

CNtlTriggerSyncManager* CNtlTriggerSyncManager::GetInstace(void)
{
	return m_pInstance;
}

RwBool CNtlTriggerSyncManager::Create(void)
{
	LinkMsg(g_EventQuestProposalDialog_Req, 0);
	LinkMsg(g_EventQuestUserSelectDialog_Req, 0);
	LinkMsg(g_EventQuestRewardDialog_Req, 0);
	LinkMsg(g_EventQuestNarrationDialog_Req, 0);
	LinkMsg(g_EventQuestNarrationUserSelectDialog_Req, 0);

	LinkMsg(g_EventQuestDirectEcho, 0);

	return TRUE;
}

void CNtlTriggerSyncManager::Destroy(void)
{
	UnLinkMsg(g_EventQuestProposalDialog_Req);
	UnLinkMsg(g_EventQuestUserSelectDialog_Req);
	UnLinkMsg(g_EventQuestRewardDialog_Req);
	UnLinkMsg(g_EventQuestNarrationDialog_Req);
	UnLinkMsg(g_EventQuestNarrationUserSelectDialog_Req);

	UnLinkMsg(g_EventQuestDirectEcho);
}

void CNtlTriggerSyncManager::Reset(void)
{
	MapUnitRegister::iterator it;
	for(it = m_mapUnitReg.begin(); it != m_mapUnitReg.end(); it++)
	{
		NTL_DELETE( (*it).second );
	}

	m_mapUnitReg.clear();

	m_uiHandle = 0;
}

void CNtlTriggerSyncManager::HandleEvents(RWS::CMsg &pMsg)
{
	if(pMsg.Id == g_EventQuestProposalDialog_Req)
	{
		QuestProposalDialogEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventQuestUserSelectDialog_Req)
	{
		QuestUserSelectDialogEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventQuestRewardDialog_Req)
	{
		QuestRewardDialogEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventQuestNarrationDialog_Req)
	{
		QuestNarrationDialogEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventQuestNarrationUserSelectDialog_Req)
	{
		QuestNarrationUserSelectDialogEventHandler(pMsg);
	}
	else if(pMsg.Id == g_EventQuestDirectEcho)
	{
		QuestDirectEchoEventHandler(pMsg);
	}
}

RwUInt32 CNtlTriggerSyncManager::AcquireHandle(void)
{
	return m_uiHandle++;
}

CNtlTCUnit* CNtlTriggerSyncManager::CreateUnit(void)
{
	CNtlTCUnit *pUnit = NTL_NEW CNtlTCUnit;
	TC_HANDLE hHandle = AcquireHandle();
	pUnit->SetHandle(hHandle);

	return pUnit;
}

CNtlTCUnit* CNtlTriggerSyncManager::CreateUnit(TC_HANDLE hHandle)
{
	MapUnitRegister::iterator it = m_mapUnitReg.find(hHandle);
	if(it == m_mapUnitReg.end())
		return CreateUnit();

	CNtlTCUnit *pTCUnit = NTL_NEW CNtlTCUnit;
	pTCUnit->SetHandle(hHandle);
	return pTCUnit;
}

void CNtlTriggerSyncManager::ReleaseUnit(CNtlTCUnit* pUnit, void *pData)
{
	if(pUnit == NULL)
		return;

	TC_HANDLE hHandle = pUnit->GetHandle();
	if(hHandle == INVALID_TC_HANDLE)
		return;

	NTL_DELETE( pUnit );

	MapUnitRegister::iterator it = m_mapUnitReg.find(hHandle);
	if(it == m_mapUnitReg.end())
		return;

	CNtlTCUnitReg *pUnitReg = (*it).second;
	pUnitReg->DecRef();
	if(!pUnitReg->IsValid())
	{
		// trigger system 으로 응답을 보낸다
		ResponseQuestTriggerSystem(pUnitReg, pData);

		ReleaseUnitReg(pUnitReg);
		m_mapUnitReg.erase(it);
	}
}

CNtlTCUnitReg* CNtlTriggerSyncManager::CreateUnitReg(const sTS_KEY& key, ETriggerDirectType eTDType, const void *pData, RwInt32 iSize)
{
	CNtlTCUnitReg *pTCUnitReg = NTL_NEW CNtlTCUnitReg;

	pTCUnitReg->SetTSKey(key);
	pTCUnitReg->SetTriggerDirectType(eTDType);
	pTCUnitReg->SetData(pData, iSize);

	return pTCUnitReg;
}

void CNtlTriggerSyncManager::ReleaseUnitReg(CNtlTCUnitReg *pUnitReg)
{
	NTL_DELETE( pUnitReg );
}

void CNtlTriggerSyncManager::RegisterUnit(CNtlTCUnit *pTCUnit, CNtlTCUnitReg *pTCUnitReg)
{
	m_mapUnitReg[pTCUnit->GetHandle()] = pTCUnitReg;
}

/////////////////////////////////////////////////////////////////////////////
// evnet send

void CNtlTriggerSyncManager::ResponseQuestTriggerSystem(CNtlTCUnitReg *pTCUnitReg, void *pData)
{
	switch(pTCUnitReg->GetTriggerDirectType())
	{
	case ETD_QuestProposal:
		ResponseQuestProposalDialog(pTCUnitReg->GetTSKey(), pData);
		break;
	case ETD_QuestUserSelect:
		ResponseQuestUserSelectDialog(pTCUnitReg->GetTSKey(), pData);
		break;
	case ETD_QuestReward:
		ResponseQuestRewardDialog(pTCUnitReg->GetTSKey(), pData);
		break;
	case ETD_QuestNarration:
		ResponseQuestNarrationDialog(pTCUnitReg->GetTSKey(), pData);
		break;
	case ETD_QuestNarrationUserSelect:
		ResponseQuestNarrationUserSelectDialog(pTCUnitReg->GetTSKey(), pData);
		break;
	case ETD_Camera:
		ResponseCameraDirect(pTCUnitReg->GetTSKey(), pData);
		break;
	}
}


void CNtlTriggerSyncManager::ResponseQuestProposalDialog(const sTS_KEY& key, void *pData)
{
	SNtlEventQuestDirect_Echo *pDirectEcho = reinterpret_cast<SNtlEventQuestDirect_Echo*>( pData );
	CNtlSLEventGenerator::QuestProposalDialog_Res(pDirectEcho->sProposal.bResult, const_cast<sTS_KEY&>( key ));
}

void CNtlTriggerSyncManager::ResponseQuestUserSelectDialog(const sTS_KEY& key, void *pData)
{
	SNtlEventQuestDirect_Echo *pDirectEcho = reinterpret_cast<SNtlEventQuestDirect_Echo*>( pData );
	CNtlSLEventGenerator::QuestUserSelectDialog_Res(pDirectEcho->sUserSelect.bResult, const_cast<sTS_KEY&>( key ), pDirectEcho->sUserSelect.tcSelID);
}

void CNtlTriggerSyncManager::ResponseQuestRewardDialog(const sTS_KEY& key, void *pData)
{
	SNtlEventQuestDirect_Echo *pDirectEcho = reinterpret_cast<SNtlEventQuestDirect_Echo*>( pData );
	CNtlSLEventGenerator::QuestRewardDialog_Res(pDirectEcho->sReward.bResult, const_cast<sTS_KEY&>( key ), pDirectEcho->sReward.nSelRwdIdx);
}

void CNtlTriggerSyncManager::ResponseQuestNarrationDialog(const sTS_KEY& key, void *pData)
{
	SNtlEventQuestDirect_Echo *pDirectEcho = reinterpret_cast<SNtlEventQuestDirect_Echo*>( pData );
	CNtlSLEventGenerator::QuestNarrationDialog_Res(pDirectEcho->sNarration.bResult, const_cast<sTS_KEY&>( key ));
}

void CNtlTriggerSyncManager::ResponseQuestNarrationUserSelectDialog(const sTS_KEY& key, void *pData)
{
	SNtlEventQuestDirect_Echo *pDirectEcho = reinterpret_cast<SNtlEventQuestDirect_Echo*>( pData );
	CNtlSLEventGenerator::QuestNarrationUserSelectDialog_Res(pDirectEcho->sNarrationUserSelect.bResult, const_cast<sTS_KEY&>( key ), pDirectEcho->sUserSelect.tcSelID );
}

void CNtlTriggerSyncManager::ResponseCameraDirect(const sTS_KEY& key, void *pData)
{
}

/////////////////////////////////////////////////////////////////////////////
// evnet handler

void CNtlTriggerSyncManager::QuestProposalDialogEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventQuestProposalDialog_Req *pProposalReq = reinterpret_cast<SNtlEventQuestProposalDialog_Req*>( pMsg.pData );

	CNtlTCUnitReg *pTCUnitReg = CreateUnitReg(pProposalReq->sTSKey, ETD_QuestProposal, pProposalReq, sizeof(SNtlEventQuestProposalDialog_Req));
	CNtlTCUnit *pTCUnit = CreateUnit();
	RegisterUnit(pTCUnit, pTCUnitReg);

	// event 작성.
	SNtlEventQuestDirect_Forward sDirect;
	sDirect.pTCUnit = pTCUnit;
	sDirect.eTDType = ETD_QuestProposal;
	memcpy(sDirect.chData, pMsg.pData, sizeof(SNtlEventQuestProposalDialog_Req));

	CNtlSLEventGenerator::QuestTriggerDirectForward(&sDirect);
}

void CNtlTriggerSyncManager::QuestUserSelectDialogEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventQuestUserSelectDialog_Req *pUserSelectReq = reinterpret_cast<SNtlEventQuestUserSelectDialog_Req*>( pMsg.pData );

	CNtlTCUnitReg *pTCUnitReg = CreateUnitReg(pUserSelectReq->sTSKey, ETD_QuestUserSelect, pUserSelectReq, sizeof(SNtlEventQuestUserSelectDialog_Req));
	CNtlTCUnit *pTCUnit = CreateUnit();
	RegisterUnit(pTCUnit, pTCUnitReg);

	// event 작성.
	SNtlEventQuestDirect_Forward sDirect;
	sDirect.pTCUnit = pTCUnit;
	sDirect.eTDType = ETD_QuestUserSelect;
	memcpy(sDirect.chData, pMsg.pData, sizeof(SNtlEventQuestUserSelectDialog_Req));

	CNtlSLEventGenerator::QuestTriggerDirectForward(&sDirect);
}

void CNtlTriggerSyncManager::QuestRewardDialogEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventQuestRewardDialog_Req *pRewardReq = reinterpret_cast<SNtlEventQuestRewardDialog_Req*>( pMsg.pData );

	CNtlTCUnitReg *pTCUnitReg = CreateUnitReg(pRewardReq->sTSKey, ETD_QuestReward, pRewardReq, sizeof(SNtlEventQuestRewardDialog_Req));
	CNtlTCUnit *pTCUnit = CreateUnit();
	RegisterUnit(pTCUnit, pTCUnitReg);

	// event 작성.
	SNtlEventQuestDirect_Forward sDirect;
	sDirect.pTCUnit = pTCUnit;
	sDirect.eTDType = ETD_QuestReward;
	memcpy(sDirect.chData, pMsg.pData, sizeof(SNtlEventQuestRewardDialog_Req));

	CNtlSLEventGenerator::QuestTriggerDirectForward(&sDirect);
}

void CNtlTriggerSyncManager::QuestNarrationDialogEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventQuestNarrationDialog_Req *pNarrationReq = reinterpret_cast<SNtlEventQuestNarrationDialog_Req*>( pMsg.pData );

	CNtlTCUnitReg *pTCUnitReg = CreateUnitReg(pNarrationReq->sTSKey, ETD_QuestNarration, pNarrationReq, sizeof(SNtlEventQuestNarrationDialog_Req));
	CNtlTCUnit *pTCUnit = CreateUnit();
	RegisterUnit(pTCUnit, pTCUnitReg);
	
	// event 작성.
	SNtlEventQuestDirect_Forward sDirect;
	sDirect.pTCUnit = pTCUnit;
	sDirect.eTDType = ETD_QuestNarration;
	memcpy(sDirect.chData, pMsg.pData, sizeof(SNtlEventQuestNarrationDialog_Req));

	CNtlSLEventGenerator::QuestTriggerDirectForward(&sDirect);
}

void CNtlTriggerSyncManager::QuestNarrationUserSelectDialogEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventQuestNarrationUserSelectDialog_Req *pNarrationUserSelectReq = reinterpret_cast<SNtlEventQuestNarrationUserSelectDialog_Req*>( pMsg.pData );

	CNtlTCUnitReg *pTCUnitReg = CreateUnitReg(pNarrationUserSelectReq->sTSKey, ETD_QuestNarrationUserSelect, pNarrationUserSelectReq, sizeof( SNtlEventQuestNarrationUserSelectDialog_Req));
	CNtlTCUnit *pTCUnit = CreateUnit();
	RegisterUnit(pTCUnit, pTCUnitReg);

	// event 작성.
	SNtlEventQuestDirect_Forward sDirect;
	sDirect.pTCUnit = pTCUnit;
	sDirect.eTDType = ETD_QuestNarrationUserSelect;
	memcpy(sDirect.chData, pMsg.pData, sizeof(SNtlEventQuestNarrationUserSelectDialog_Req));

	CNtlSLEventGenerator::QuestTriggerDirectForward(&sDirect);
}

void CNtlTriggerSyncManager::QuestDirectEchoEventHandler(RWS::CMsg &pMsg)
{
	SNtlEventQuestDirect_Echo *pDirectEcho = reinterpret_cast<SNtlEventQuestDirect_Echo*>( pMsg.pData );

	ReleaseUnit(pDirectEcho->pTCUnit, pDirectEcho);
}