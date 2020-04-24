#include "precomp_dboclient.h"
#include "JudgeDump.h"

// core
#include "NtlDebug.h"

// shared
#include "NtlPacketUG.h"
#include "NtlPacketGU.h"
#include "NtlPacketUtil.h"

// simulation
#include "NtlSLDef.h"
#include "NtlSLGlobal.h"
#include "NtlSob.h"
#include "NtlSobAvatar.h"
#include "NtlSobManager.h"
#include "NtlSLEventFunc.h"
#include "NtlSLLogic.h"

// cleint
#include "DumpCommand.h"
#include "DboGlobal.h"

namespace
{
#define dBUFFER_SIZE		1024

	char acBuffer[dBUFFER_SIZE] = "";
}


//////////////////////////////////////////////////////////////////////////
//			CJudgeDefaultDump
//////////////////////////////////////////////////////////////////////////

const char* CJudgeDefaultDump::GetDump_Message(const WCHAR*  pwcMessage)
{
	::WideCharToMultiByte(GetACP(), 0, pwcMessage, -1, acBuffer, dBUFFER_SIZE, NULL, NULL);
	return acBuffer;
}


//////////////////////////////////////////////////////////////////////////
//			CJudgeActionDump
//////////////////////////////////////////////////////////////////////////

char* CJudgeActionDump::GetDump_SendPacket(void* pPacket)
{
	sNTLPACKETHEADER *pHeader = (sNTLPACKETHEADER*)pPacket;
	RwUInt16 wOpCode = pHeader->wOpCode;

	// avooo's command : 너무 많은 패킷이 출력이 되어 주석처리 하겠습니다
	/*
	const char *pOpString = NtlGetPacketName(wOpCode);
	if(pOpString)
	{
	dump.Color(255, 255, 0);
	dump.Format("Send Net(%s) => Net op code \n\n", pOpString);
	dump.Dump(); 
	}
	*/

	switch(wOpCode)
	{
	case UG_CHAR_MOVE:
		{
			sUG_CHAR_MOVE *pCharMove = (sUG_CHAR_MOVE*)pPacket;
			sprintf_s(acBuffer, dBUFFER_SIZE, "Send Net(UG_CHAR_MOVE) => Avatar , curr pos(%f, %f, %f), dir(%f, %f), dirflag(%d)\n\n", 
								pCharMove->vCurLoc.x, pCharMove->vCurLoc.y, pCharMove->vCurLoc.z,
								pCharMove->vCurDir.x, pCharMove->vCurDir.z, 
								pCharMove->byMoveDirection);
			return acBuffer;
		}
	case UG_CHAR_DEST_MOVE:
		{
			sUG_CHAR_DEST_MOVE *pCharDestMove = (sUG_CHAR_DEST_MOVE*)pPacket;
			sprintf_s(acBuffer, dBUFFER_SIZE, "Send Net(UG_CHAR_DEST_MOVE) => Avatar , dest pos(%f, %f, %f)\n\n", 
								pCharDestMove->vDestLoc.x, pCharDestMove->vDestLoc.y, pCharDestMove->vDestLoc.z);
			return acBuffer;
		}
	case UG_CHAR_CHANGE_DIRECTION_ON_FLOATING:
		{
			sprintf_s(acBuffer, dBUFFER_SIZE, "Send Net(UG_CHAR_CHANGE_DIRECTION_ON_FLOATING) => Avatar \n\n"); 
			return acBuffer;
		}
	case UG_CHAR_JUMP:
		{
			sprintf_s(acBuffer, dBUFFER_SIZE, "Send Net(UG_CHAR_JUMP) => Avatar\n\n");
			return acBuffer;
		}
	case UG_CHAR_JUMP_END:
		{
			sprintf_s(acBuffer, dBUFFER_SIZE, "Send Net(UG_CHAR_JUMP_END) => Avatar\n\n");
			return acBuffer;
		}
	case UG_CHAR_TOGG_SITDOWN:
		{
			sprintf_s(acBuffer, dBUFFER_SIZE, "Send Net(UG_CHAR_TOGG_SITDOWN) => Avatar\n\n");
			return acBuffer;
		}
	case UG_CHAR_CHARGE:
		{
			sUG_CHAR_CHARGE *pJumpEnd = (sUG_CHAR_CHARGE*)pPacket;
			sprintf_s(acBuffer, dBUFFER_SIZE, "Send Net(UG_CHAR_CHARGE, %d ) => Avatar\n\n", pJumpEnd->bCharge);
			return acBuffer;
		}
	case UG_ITEM_MOVE_REQ:
		{
			sprintf_s(acBuffer, dBUFFER_SIZE, "Send Net(UG_ITEM_MOVE_REQ) => Avatar \n\n");
			return acBuffer;
		}
	case UG_CHAR_TOGG_FIGHTING:
		{
			sUG_CHAR_TOGG_FIGHTING *pFightingMode = (sUG_CHAR_TOGG_FIGHTING*)pPacket;
			sprintf_s(acBuffer, dBUFFER_SIZE, "Send Net(UG_CHAR_TOGG_FIGHTING) => flag(%d)\n\n", pFightingMode->bFightMode);
			return acBuffer;
		}
	}

	return NULL;
}

char* CJudgeActionDump::GetDump_RecvPacket(void* pPacket)
{
	sNTLPACKETHEADER *pHeader = (sNTLPACKETHEADER*)pPacket;
	RwUInt16 wOpCode = pHeader->wOpCode;
	const char *pOpString = NtlGetPacketName(wOpCode);

	switch(wOpCode)
	{
	case GU_CHAR_JUMP:
		{
			sGU_CHAR_JUMP *pJump = (sGU_CHAR_JUMP*)pPacket;
			CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pJump->handle);

			if(pSobObj)
			{
				const WCHAR *pName = Logic_GetName(pSobObj);
				if(pName)
					sprintf_s(acBuffer, dBUFFER_SIZE, "Recv Net(%s - %s) => handle(%u), name(%s)\n\n", pOpString, pSobObj->GetClassName(), pJump->handle, pName);
				else
					sprintf_s(acBuffer, dBUFFER_SIZE, "Recv Net(%s - %s) => handle(%u) \n\n", pOpString, pSobObj->GetClassName(), pJump->handle);
			}
			else
			{
				sprintf_s(acBuffer, dBUFFER_SIZE, "Recv Net(%s) => handle(%u) \n\n", pOpString, pJump->handle);
			}

			return acBuffer;
		}
	case GU_CHAR_JUMP_END:
		{
			sGU_CHAR_JUMP_END *pJumpEnd = (sGU_CHAR_JUMP_END*)pPacket;
			CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pJumpEnd->handle);

			if(pSobObj)
			{				
				const WCHAR *pName = Logic_GetName(pSobObj);
				if(pName)
					sprintf_s(acBuffer, dBUFFER_SIZE, "Recv Net(%s - %s) => handle(%u), name(%s)\n\n", pOpString, pSobObj->GetClassName(), pJumpEnd->handle, pName);
				else
					sprintf_s(acBuffer, dBUFFER_SIZE, "Recv Net(%s - %s) => handle(%u) \n\n", pOpString, pSobObj->GetClassName(), pJumpEnd->handle);
			}
			else
			{				
				sprintf_s(acBuffer, dBUFFER_SIZE, "Recv Net(%s) => handle(%u) \n\n", pOpString, pJumpEnd->handle);
			}

			return acBuffer;
		}
	case GU_CHAR_MOVE:
		{
			sGU_CHAR_MOVE *pCharMove = (sGU_CHAR_MOVE*)pPacket;

			if( pCharMove->handle == Logic_GetAvatarTargetHandle() )
			{
				sprintf_s(acBuffer, dBUFFER_SIZE, "Recv Net(GU_CHAR_MOVE) => handle(%u) , curr pos(%f, %f, %f), dir(%f, %f), dirflag(%d)\n\n", 
									pCharMove->handle,
									pCharMove->vCurLoc.x, pCharMove->vCurLoc.y, pCharMove->vCurLoc.z,
									pCharMove->vCurDir.x, pCharMove->vCurDir.z, 
									pCharMove->byMoveDirection);
				return acBuffer;
			}
		}
	case GU_CHAR_DEST_MOVE:
		{
			sGU_CHAR_DEST_MOVE *pCharDestMove = (sGU_CHAR_DEST_MOVE*)pPacket;
			if( GetDumpCmdManager()->IsOutputRecvPacketTarget(pCharDestMove->handle) )
			{
				CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pCharDestMove->handle);
				if(pSobObj)
				{
					RwUInt8 byServerState = 0, byClientState = 0;
					char chState[124];
					CNtlSLEventGenerator::SobGetState(pSobObj->GetSerialID(), byServerState, byClientState, chState);
					sprintf_s(acBuffer, dBUFFER_SIZE, "Recv Net(%s, %s) => handle(%u), dest count (%d), dest pos(%f, %f, %f), server state(%s), client state(%d, %s) \n\n", 
										pOpString,
										pSobObj->GetClassName(),
										pSobObj->GetSerialID(),
										pCharDestMove->byDestLocCount,
										pCharDestMove->avDestLoc[0].x,
										pCharDestMove->avDestLoc[0].y,
										pCharDestMove->avDestLoc[0].z,
										NtlGetCharStateString(byServerState), byClientState, chState);
					return acBuffer;
				}
			}
		}
	case GU_ITEM_MOVE_RES:
		{
			sprintf_s(acBuffer, dBUFFER_SIZE, "Recv Net(GU_ITEM_MOVE_RES) => Avatar \n\n");
			return acBuffer;
		}
	case GU_CHAR_ACTION_ATTACK:
		{
			sGU_CHAR_ACTION_ATTACK *pActionAttack = (sGU_CHAR_ACTION_ATTACK*)pPacket;

			if( GetDumpCmdManager()->IsOutputRecvPacketTarget(pActionAttack->hSubject) )
			{
				const char *pOpString = NtlGetPacketName(wOpCode);
				CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pActionAttack->hSubject);
				sprintf_s(acBuffer, dBUFFER_SIZE, "Recv Net(%s - %s : attresult %d, chainflag %d, attacksequence %d, shift(%f, %f, %f), tick time(%u)  \n\n", pOpString, pSobObj->GetClassName(), 
									pActionAttack->byAttackResult, pActionAttack->bChainAttack, pActionAttack->byAttackSequence,
									pActionAttack->vShift.x, pActionAttack->vShift.y, pActionAttack->vShift.z, GetTickCount());
				return acBuffer;
			}
		}
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////
//			CJudgeSobCreateDestoryDump
//////////////////////////////////////////////////////////////////////////

char* CJudgeSobCreateDestoryDump::GetDump_RecvPacket(void* pPacket)
{
	sNTLPACKETHEADER *pHeader = (sNTLPACKETHEADER*)pPacket;
	RwUInt16 wOpCode = pHeader->wOpCode;
	const char *pOpString = NtlGetPacketName(wOpCode);

	switch(wOpCode)
	{
	case GU_OBJECT_CREATE:
		{
			sGU_OBJECT_CREATE *pObjCreate = (sGU_OBJECT_CREATE*)pPacket;
			sprintf_s(acBuffer, dBUFFER_SIZE, "Recv Net(%s) => handle(%u)\n\n", pOpString, pObjCreate->handle);
			return acBuffer;
		}
	case GU_OBJECT_DESTROY:
		{
			sGU_OBJECT_DESTROY *pObjDestroy = (sGU_OBJECT_DESTROY*)pPacket;
			CNtlSob *pSobObj = GetNtlSobManager()->GetSobObject(pObjDestroy->handle);

			if(pSobObj)
			{
				const WCHAR *pName = Logic_GetName(pSobObj);
				if(pName)
					sprintf_s(acBuffer, dBUFFER_SIZE, "Recv Net(%s - %s) => handle(%u), name(%s)\n\n", pOpString, pSobObj->GetClassName(), pObjDestroy->handle, pName);
				else
					sprintf_s(acBuffer, dBUFFER_SIZE, "Recv Net(%s - %s) => handle(%u) \n\n", pOpString, pSobObj->GetClassName(), pObjDestroy->handle);

				return acBuffer;
			}
			else
			{
				sprintf_s(acBuffer, dBUFFER_SIZE, "Recv Net(%s) => handle(%u) \n\n", pOpString, pObjDestroy->handle);
				return acBuffer;
			}
		}	
	}

	return NULL;
}