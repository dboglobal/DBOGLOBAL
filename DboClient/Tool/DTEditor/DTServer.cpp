#include "stdafx.h"
#include "DirectApp.h"
#include "DTServer.h"

// shared
#include "NtlPacketGU.h"
#include "NtlPacketUG.h"
#include "HTBSetTable.h"
#include "SkillTable.h"
#include "TableContainer.h"

// core
#include "NtlMath.h"

// simulation
#include "precomp_ntlsimulation.h"
#include "NtlSLEventFunc.h"

// tool
#include "DTEditorGlobal.h"

CDTServer* CDTServer::m_pInstace = NULL;

CDTServer::CDTServer(void)
{
	m_pInstace				= this;
	m_uiTargetSerialId		= INVALID_SERIAL_ID;
}

CDTServer::~CDTServer(void)
{
}

CDTServer* CDTServer::GetInstance(void)
{
	return m_pInstace;
}

void CDTServer::PacketProcTargetSelect(void *pPacket)
{
	sUG_CHAR_TARGET_SELECT *pCharTargetSelect = (sUG_CHAR_TARGET_SELECT*)pPacket;
	m_uiTargetSerialId = pCharTargetSelect->hTarget;
}

void CDTServer::PacketProcHTBReq(void *pPacket)
{
	// 상태를 업데이트 한다.

	sUG_HTB_START_REQ *pCharHTBSkillReq = (sUG_HTB_START_REQ*)pPacket;

	RwV3d vLoc, vDir;

	CNtlSobAvatar *pSobAvatar = GetNtlSLGlobal()->GetSobAvatar();
	vLoc = pSobAvatar->GetPosition();
	vDir = pSobAvatar->GetDirection();

	const sHTB_SET_TBLDAT *pHTBSetTblData = GetActiveHTBTableData();

	sCHARSTATE sCharState;

	sCharState.sCharStateBase.byStateID = CHARSTATE_HTB;
	sCharState.sCharStateBase.vCurLoc.x = vLoc.x;
	sCharState.sCharStateBase.vCurLoc.y = vLoc.y;
	sCharState.sCharStateBase.vCurLoc.z = vLoc.z;
	sCharState.sCharStateBase.vCurDir.x = vDir.x;
	sCharState.sCharStateBase.vCurDir.y = vDir.y;
	sCharState.sCharStateBase.vCurDir.z = vDir.z;
	sCharState.sCharStateBase.dwConditionFlag = 0;	

	sCharState.sCharStateDetail.sCharStateHTB.hTarget		= pCharHTBSkillReq->hTarget;
	sCharState.sCharStateDetail.sCharStateHTB.byStepCount	= pHTBSetTblData->bySetCount;
	sCharState.sCharStateDetail.sCharStateHTB.byCurStep		= 0;
	sCharState.sCharStateDetail.sCharStateHTB.byResultCount = 0;
	sCharState.sCharStateDetail.sCharStateHTB.HTBId			= pHTBSetTblData->tblidx;
	
	RwInt8 byResultCount = 0;
	for(RwInt32 i = 0; i < pHTBSetTblData->bySetCount; ++i)
	{
		if(pHTBSetTblData->aHTBAction[i].skillTblidx != INVALID_TBLIDX)
		{
			sCharState.sCharStateDetail.sCharStateHTB.aHTBSkillResult[byResultCount].byStep = i;
			sCharState.sCharStateDetail.sCharStateHTB.aHTBSkillResult[byResultCount].sSkillResult.byAttackResult		= BATTLE_ATTACK_RESULT_HIT;
			sCharState.sCharStateDetail.sCharStateHTB.aHTBSkillResult[byResultCount].sSkillResult.effectResult[0].fResultValue	= 1;
			sCharState.sCharStateDetail.sCharStateHTB.aHTBSkillResult[byResultCount].sSkillResult.effectResult[1].fResultValue	= 1;
			sCharState.sCharStateDetail.sCharStateHTB.aHTBSkillResult[byResultCount].sSkillResult.vShift.x				= 0.0f;
			sCharState.sCharStateDetail.sCharStateHTB.aHTBSkillResult[byResultCount].sSkillResult.vShift.y				= 0.0f;
			sCharState.sCharStateDetail.sCharStateHTB.aHTBSkillResult[byResultCount].sSkillResult.vShift.z				= 1.0f;

			sCharState.sCharStateDetail.sCharStateHTB.byResultCount++;
			byResultCount++;
		}
	}

	CNtlSLEventGenerator::SobServerUpdateState(pSobAvatar->GetSerialID(), &sCharState);


	// sansbag...
	CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pCharHTBSkillReq->hTarget);
	vLoc = pSobObj->GetPosition();
	vDir = pSobObj->GetDirection();
	
	sCharState.sCharStateBase.byStateID = CHARSTATE_SANDBAG;
	sCharState.sCharStateBase.vCurLoc.x = vLoc.x;
	sCharState.sCharStateBase.vCurLoc.y = vLoc.y;
	sCharState.sCharStateBase.vCurLoc.z = vLoc.z;
	sCharState.sCharStateBase.vCurDir.x = vDir.x;
	sCharState.sCharStateBase.vCurDir.y = vDir.y;
	sCharState.sCharStateBase.vCurDir.z = vDir.z;

	CNtlSLEventGenerator::SobServerUpdateState(pSobObj->GetSerialID(), &sCharState);
}

void CDTServer::Update(RwReal fElapsed)
{
	ListPacket::iterator it;
	for(it = m_listPacket.begin(); it != m_listPacket.end(); it++)
	{
		PacketProc((*it)->nSize, (*it)->chData);
		delete (*it);
	}

	m_listPacket.clear();
}


void CDTServer::PacketProc(RwInt32 iDataLen, void *pPacket)
{
	sNTLPACKETHEADER *pHeader = (sNTLPACKETHEADER*)pPacket;
	RwUInt16 wOpCode = pHeader->wOpCode;

	switch(wOpCode)
	{
	case UG_CHAR_TARGET_SELECT:
		PacketProcTargetSelect(pPacket);
		break;
	case UG_HTB_START_REQ:
		PacketProcHTBReq(pPacket);
		break;
	}
}


void CDTServer::PushPacket(RwInt32 iDataLen, void *pPacket)
{
	SPacketData *pPacketData	= new SPacketData;
	pPacketData->nSize			= iDataLen;
	memcpy(pPacketData->chData, pPacket, iDataLen);

	m_listPacket.push_back(pPacketData);
}

bool DTServerPacketProc(RwInt32 iDataLen, void *pData)
{
	CDTServer::GetInstance()->PushPacket(iDataLen, pData);

	return true;
}