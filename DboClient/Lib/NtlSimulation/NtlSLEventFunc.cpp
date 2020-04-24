#include "precomp_ntlsimulation.h"
#include "NtlSLEventFunc.h"
#include "NtlDebug.h"
#include "NtlSobActor.h"
#include "NtlSobAvatar.h"
#include "NtlSobTriggerObject.h"
#include "NtlSLGlobal.h"
#include "NtlSobManager.h"
#include "NtlEventQueue.h"
#include "NtlSLLogic.h"
#include "NtlSLApi.h"
#include "TableContainer.h"
#include "QuestTextDataTable.h"
#include "QuestNarrationTable.h"
#include "NtlSLPacketGenerator.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* system message
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNtlSLEventGenerator::SysMsg(SERIAL_HANDLE hSerialId, const char* chMsg, RwBool bServerMsg /* = TRUE */)
{
	SNtlEventSysMsg sSysMsg;
	sSysMsg.hSerialId = hSerialId;
	sSysMsg.strMsg = chMsg;
	sSysMsg.bServerMsg = bServerMsg;

	RWS::CMsg msg;
	msg.Id = g_EventSysMsg;
	msg.pData = reinterpret_cast<void*>(&sSysMsg);

	_SendMsg(msg);
}

void CNtlSLEventGenerator::SysMsg( const WCHAR* pMsgString, RwUInt32 eType /*= SNtlEventSysStringMsg::TYPE_CHAT_WARN */ )
{
	SNtlEventSysStringMsg sSysMsg;
	sSysMsg.szMessage = const_cast<WCHAR*>(pMsgString);
	sSysMsg.eType = eType;

	RWS::CMsg msg;
	msg.Id = g_EventSysStringMsg;
	msg.pData = reinterpret_cast<void*>(&sSysMsg);

	_SendMsg(msg);
}

void CNtlSLEventGenerator::SysMsg(WCHAR* pMsgString, RwUInt32 eType /* = SNtlEventSysStringMsg::TYPE_CHAT_WARN  */)
{
	SNtlEventSysStringMsg sSysMsg;
	sSysMsg.szMessage = pMsgString;
	sSysMsg.eType = eType;

	RWS::CMsg msg;
	msg.Id = g_EventSysStringMsg;
	msg.pData = reinterpret_cast<void*>(&sSysMsg);

	_SendMsg(msg);
}

void CNtlSLEventGenerator::FormatSysMsg( SERIAL_HANDLE hSerialId, const char* csMsg, RwBool bServerMsg, ... )
{
	SNtlEventFormatSysMsg sSysMsg;
	sSysMsg.hSerialID = hSerialId;
	sSysMsg.strMsg = csMsg;
	sSysMsg.bServerMsg = bServerMsg;

	va_list args;
	va_start( args, bServerMsg );
	sSysMsg.args = args;

	RWS::CMsg msg;
	msg.Id = g_EventFormatSysMsg;
	msg.pData = reinterpret_cast<void*>(&sSysMsg);

	_SendMsg(msg);
	va_end( args );
}

void CNtlSLEventGenerator::FormatSysMsg(WCHAR* pMsgString, ...)
{
	SNtlEventFormatSysStringMsg sSysMsg;
	sSysMsg.szMessage = pMsgString;

	va_list args;
	va_start( args, pMsgString );
	sSysMsg.args = args;

	RWS::CMsg msg;
	msg.Id = g_EventFormatSysStringMsg;
	msg.pData = reinterpret_cast<void*>(&sSysMsg);

	_SendMsg(msg);
	va_end( args );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* network
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNtlSLEventGenerator::NetSendError(void *pNetSender)
{
	SNtlEventNetSendError sSendError;
	sSendError.pNetSender = pNetSender;

	RWS::CMsg msg;
	msg.Id = g_EventNetSendError;
	msg.pData = reinterpret_cast<void*>(&sSendError);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::NotifySendPacket(void *pPacket)
{
	SEND_MSG(g_EventNotifySendPacket, pPacket);
}

void CNtlSLEventGenerator::NotifyReceivePacket(void *pPacket)
{
	SEND_MSG(g_EventNotifyReceivePacket, pPacket);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* world change
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNtlSLEventGenerator::WorldChange(RwBool bWorldChange)
{
	SNtlEventWorldChange sWorldChange;
	sWorldChange.bWorldChange = bWorldChange;

	SEND_MSG(g_EventWorldChange, &sWorldChange);
}

void CNtlSLEventGenerator::CreateWorld( RwBool bCreate ) 
{
	SNtlEventCreateWorld sCreateWorld;
	sCreateWorld.bCreate = bCreate;

	SEND_MSG(g_EventCreateWorld, &sCreateWorld);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* avatar/camera 이동 및 회전
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNtlSLEventGenerator::CameraSmoothingMove(RwBool bEnable)
{
	SNtlEventCameraSmoothingMove sCameraSmoothingMove;
	sCameraSmoothingMove.bEnable = bEnable;

	RWS::CMsg msg;
	msg.Id = g_EventCameraSmoothingMove;
	msg.pData = reinterpret_cast<void*>(&sCameraSmoothingMove);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::CameraShake(SERIAL_HANDLE hSerialId, RwBool bLowShakeFlag)
{
	CNtlSob *pObj = GetNtlSobManager()->GetSobObject(hSerialId);
	CNtlSobActor *pActor = static_cast<CNtlSobActor*>(pObj);

	SNtlEventCameraShake sCameraShake;
	sCameraShake.pActor = pActor;
	sCameraShake.bLowShakeFlag = bLowShakeFlag;

	RWS::CMsg msg;
	msg.Id = g_EventCameraShake;
	msg.pData = reinterpret_cast<void*>(&sCameraShake);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::CameraDash(SERIAL_HANDLE hSerialId, RwReal fDashSpeed, RwV3d vDest)
{
	CNtlSob *pObj = GetNtlSobManager()->GetSobObject(hSerialId);
	CNtlSobActor *pActor = static_cast<CNtlSobActor*>(pObj);

	SNtlEventCameraDash sCameraDash;
	sCameraDash.pActor = pActor;
	sCameraDash.fDashSpeed = fDashSpeed;
	RwV3dAssignMacro(&sCameraDash.vDest, &vDest);

	RWS::CMsg msg;
	msg.Id = g_EventCameraDash;
	msg.pData = reinterpret_cast<void*>(&sCameraDash);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::CameraNpc(SERIAL_HANDLE hSerialId)
{
	CNtlSob *pObj = GetNtlSobManager()->GetSobObject(hSerialId);
	CNtlSobActor *pActor = static_cast<CNtlSobActor*>(pObj);

	GetNtlSLGlobal()->SetNpcFacingHandle(hSerialId); // set facing
	API_GetSLPacketGenerator()->SendCharTargetFacing(hSerialId, true);

	SNtlEventCameraNpc sCameraNpc;
	sCameraNpc.pActor = pActor;

	RWS::CMsg msg;
	msg.Id = g_EventCameraNpc;
	msg.pData = reinterpret_cast<void*>(&sCameraNpc);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::CameraDB(SERIAL_HANDLE hSerialId)
{
	CNtlSob *pObj = GetNtlSobManager()->GetSobObject(hSerialId);
	CNtlSobActor *pActor = static_cast<CNtlSobActor*>(pObj);

	SNtlEventCameraDB sCameraDB;
	sCameraDB.pActor = pActor;

	SEND_MSG(g_EventCameraDB, &sCameraDB);
}

// 타임머신 연출용 카메라
void CNtlSLEventGenerator::CameraTimeMachine( SERIAL_HANDLE hSerialID ) 
{
	CNtlSobActor* pObj = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(hSerialID);
	SNtlEventCameraTimeMachine sCameraTM;
	sCameraTM.pObject = pObj;

	SEND_MSG(g_EventCameraTimeMachine, &sCameraTM);
}

void CNtlSLEventGenerator::CameraExplosion(SERIAL_HANDLE hSerialID, unsigned int uiExplosionId)
{
	CNtlSobActor* pObject = (CNtlSobActor*)GetNtlSobManager()->GetSobObject( hSerialID );

	SNtlEventCameraExplosion sExplosion;
	sExplosion.pObject = pObject;
	sExplosion.uiExplosionId = uiExplosionId;

	SEND_MSG(g_EventCameraExplosion, &sExplosion);
}

void CNtlSLEventGenerator::CameraTimeMachineArrive(RwBool bStart, SERIAL_HANDLE hSerialID)
{
	CNtlSobActor* pObject = (CNtlSobActor*)GetNtlSobManager()->GetSobObject( hSerialID );

	SNtlEventCameraTimeMachineArrive sTMArrive;
	sTMArrive.bStart = bStart;
	sTMArrive.pObject = pObject;

	SEND_MSG(g_EventCameraTimeMachineArrive, &sTMArrive);
}

void CNtlSLEventGenerator::CameraKnockDownMatrix(SERIAL_HANDLE hSerialID, RwReal fLifeTime)
{
	CNtlSobActor* pActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject( hSerialID );

	SNtlEventCameraKnockDownMatrix sKDMatrix;
	sKDMatrix.pActor = pActor;
	sKDMatrix.fLifeTime = fLifeTime;

	SEND_MSG(g_EventCameraKnockDownMatrix, &sKDMatrix);
}

void CNtlSLEventGenerator::CameraDirectNodeClear(void)
{
}


void CNtlSLEventGenerator::CameraDirectSplineNode(RwReal fDelayTime, RwV3d vStartPos, RwV3d vEndPos, RwV3d vLookAtPos, RwUInt8 byDirectLookAtType, const RwChar *pKey)
{
	SNtlEventCameraDTSplineNode sCameraDTSplineNode;
	sCameraDTSplineNode.fDelayTime = fDelayTime;
	RwV3dAssignMacro(&sCameraDTSplineNode.vStartPos, &vStartPos);
	RwV3dAssignMacro(&sCameraDTSplineNode.vEndPos, &vEndPos);
	sCameraDTSplineNode.vLookAtPos = vLookAtPos;
	sCameraDTSplineNode.byDirectLookAtType = byDirectLookAtType;

	if(pKey)
		strcpy_s(sCameraDTSplineNode.chKey, NTL_MAX_DIRECT_NAME, pKey);

	RWS::CMsg msg;
	msg.Id = g_EventCameraDTSplineNode;
	msg.pData = reinterpret_cast<void*>(&sCameraDTSplineNode);
	_SendMsg(msg);
}

// 프리카메라로 카메라를 체인지 한다.
void CNtlSLEventGenerator::CameraFree() 
{
	SEND_MSG(g_EventCameraFree, NULL);
}

// 1인칭 카메라로 전환한다.
void CNtlSLEventGenerator::CameraFPS(RwV3d& vCurrLookAt, RwV3d& vTargetLookAt)
{
	SNtlEventCameraFPS data;    
	data.vCurrLookAt = vCurrLookAt;
	data.vTargetLookAt = vTargetLookAt;

	SEND_MSG(g_EventCameraFPS, &data);
}

// 버스 카메라로 전환한다
void CNtlSLEventGenerator::CameraBus(CNtlSobActor* pActor)
{
	SNtlEventCameraControlBus data;    
	data.pActor = pActor;

	SEND_MSG(g_EventCameraBus, &data);
}

void CNtlSLEventGenerator::CameraControlDelete(RwUInt8 byControllerType)
{
	SNtlEventCameraControlDelete sCameraDelete;
	sCameraDelete.byControllerType = byControllerType;

	SEND_MSG(g_EventCameraControlDelete, &sCameraDelete);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* action mapping
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNtlSLEventGenerator::ActionMapHalt(void)
{
	RWS::CMsg msg;
	msg.Id = g_EventActionMapHalt;
	msg.pData = NULL;
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ActionMapJump(RwUInt32 uiMoveFlags)
{
	SNtlEventActionMapJump sJump;
	sJump.uiMoveFlags = uiMoveFlags;

	RWS::CMsg msg;
	msg.Id = g_EventActionMapJump;
	msg.pData = reinterpret_cast<void*>(&sJump);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ActionMapAirJump(RwUInt32 uiMoveFlags)
{
	SNtlEventActionMapAirJump sJump;
	sJump.uiMoveFlags = uiMoveFlags;

	SEND_MSG(g_EventActionMapAirJump, &sJump);
}

void CNtlSLEventGenerator::ActionMapCharging(RwBool bStartOrEnd)
{
	SNtlEventActionMapCharging sCharging;
	sCharging.bStartOrEnd = bStartOrEnd;		

	RWS::CMsg msg;
	msg.Id = g_EventActionMapCharging;
	msg.pData = reinterpret_cast<void*>(&sCharging);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ActionMapBlocking(RwBool bStartOrEnd)
{
	SNtlEventActionMapBlocking sBlocking;
	sBlocking.bStartOrEnd = bStartOrEnd;		

	RWS::CMsg msg;
	msg.Id = g_EventActionMapBlocking;
	msg.pData = reinterpret_cast<void*>(&sBlocking);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ActionMapSitAndStand(void)
{
	RWS::CMsg msg;
	msg.Id = g_EventActionMapSitAndStand;
	msg.pData = NULL;
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ActionMapSkillUse(SERIAL_HANDLE hSerialId, RwBool bExistTarget, SERIAL_HANDLE hTargetSerialId, 
											 SERIAL_HANDLE hSkillSerialId, RwReal fFollowRange, RwUInt8 bySkillSlotIdx, RwUInt8 byRpBonusType, TBLIDX skillTblidx)
{

	SNtlEventActionMapSkillUse sSkillUse;
	sSkillUse.hSerialId			= hSerialId;
	sSkillUse.bExistTarget		= bExistTarget;
	sSkillUse.hTargetSerialId	= hTargetSerialId;
	sSkillUse.hSkillSerialId	= hSkillSerialId;
	sSkillUse.fFollowRange		= fFollowRange;
	sSkillUse.bySkillSlotIdx	= bySkillSlotIdx;
	sSkillUse.byRpBonusType		= byRpBonusType;
	sSkillUse.skillTblidx = skillTblidx;

	RWS::CMsg msg;
	msg.Id = g_EventActionMapSkillUse;
	msg.pData = reinterpret_cast<void*>(&sSkillUse);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ActionMapSocialUse(RwUInt32 uiTblId)
{
	SNtlEventActionMapSocialUse sSocialUse;
	sSocialUse.uiTblId = uiTblId;

	RWS::CMsg msg;
	msg.Id = g_EventActionMapSocialUse;
	msg.pData = reinterpret_cast<void*>(&sSocialUse);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ActionMapPrivateShopOpen(void)
{
	RWS::CMsg msg;
	msg.Id = g_EventActionMapPrivateShopOpen;
	msg.pData = reinterpret_cast<void*>(NULL);

	_SendMsg(msg);
}

void CNtlSLEventGenerator::ActionMapHTBUse(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hTargetSerialId, SERIAL_HANDLE hHTBSerialId, 
										   RwReal fFollowRange, RwUInt8 byHTBSlotIdx)
{
	SNtlEventActionMapHTBUse sHTBUse;
	sHTBUse.hSerialId		= hSerialId;
	sHTBUse.hTargetSerialId	= hTargetSerialId;
	sHTBUse.hSkillSerialId	= hHTBSerialId;
	sHTBUse.fFollowRange	= fFollowRange;
	sHTBUse.bySkillSlotIdx	= byHTBSlotIdx;

	RWS::CMsg msg;
	msg.Id = g_EventActionMapHTBUse;
	msg.pData = reinterpret_cast<void*>(&sHTBUse);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ActionMapItemUse(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hTargetSerialId, RwUInt8 byPlace, RwUInt8 byPos, RwUInt8 byCastingTime, SERIAL_HANDLE hRefObject, RwUInt8 byKeyPlace, RwUInt8 byKeyPos)
{
	SNtlEventActionMapItemUse sItemUse;
	sItemUse.hSerialId			= hSerialId;
	sItemUse.hTargetSerialId	= hTargetSerialId;
	sItemUse.byPlace			= byPlace;
	sItemUse.byPos				= byPos;
	sItemUse.byCastingTime		= byCastingTime;
	sItemUse.hRefObject = hRefObject;
	sItemUse.byKeyPlace = byKeyPlace;
	sItemUse.byKeyPos = byKeyPos;


	SEND_MSG(g_EventActionMapItemUse, &sItemUse);
}

void CNtlSLEventGenerator::ActionMapAutoFollow(SERIAL_HANDLE hTargetSerialId)
{
	SNtlEventActionMapAutoFollow sAutoFollow;
	sAutoFollow.hTargetSerialId	= hTargetSerialId;

	RWS::CMsg msg;
	msg.Id = g_EventActionMapAutoFollow;
	msg.pData = reinterpret_cast<void*>(&sAutoFollow);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ActionMapDice()
{
	SEND_MSG(g_EventActionMapDice, NULL);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/** 
*simulation object event
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void CNtlSLEventGenerator::SobShowNfy(SERIAL_HANDLE hSerialId, RwBool bVisible)
{
	SNtlEventSobShowNfy sShowNfy;
	sShowNfy.hSerialId = hSerialId;
	sShowNfy.bShow = bVisible;

	RWS::CMsg msg;
	msg.Id = g_EventSobShowNfy;
	msg.pData = reinterpret_cast<void*>(&sShowNfy);
	_SendMsg(msg);
}


void CNtlSLEventGenerator::TerrainClick(RwV3d vPickPos, RwBool bServerSend /*= TRUE*/)
{
	SNtlEventTerrainClick sTerrainClick;
	RwV3dAssignMacro(&sTerrainClick.vPickPos, &vPickPos);
	sTerrainClick.bServerSend = bServerSend;

	RWS::CMsg msg;
	msg.Id = g_EventTerrainClick;
	msg.pData = reinterpret_cast<void*>(&sTerrainClick);
	_SendMsg(msg);
}

RwBool CNtlSLEventGenerator::SobTargetSelect(SERIAL_HANDLE hSerialId, RwBool bFollow/* = FALSE*/)
{
	CNtlSob* pObj = GetNtlSobManager()->GetSobObject( hSerialId );
	if (pObj && pObj->GetClassID() == SLCLASS_VEHICLE )
	{
		hSerialId = pObj->GetOwnerID();
	}

	SNtlEventSobTargetSelect sSobTargetSelect;
	sSobTargetSelect.bSuccess	= TRUE;	
	sSobTargetSelect.hSerialId	= hSerialId;
	sSobTargetSelect.bFollow = bFollow;

	RWS::CMsg msg;
	msg.Id = g_EventSobTargetSelect;
	msg.pData = reinterpret_cast<void*>(&sSobTargetSelect);
	_SendMsg(msg);

	return sSobTargetSelect.bSuccess;
}

void CNtlSLEventGenerator::SobTargetSelectRelease(SERIAL_HANDLE hSerialId)
{
	CNtlSob* pObj = GetNtlSobManager()->GetSobObject( hSerialId );
	if ( pObj && pObj->GetClassID() == SLCLASS_VEHICLE )
	{
		hSerialId = pObj->GetOwnerID();
	}

	SNtlEventSobTargetSelectRelease sSobTargetSelectRelease;
	sSobTargetSelectRelease.hSerialId = hSerialId;

	RWS::CMsg msg;
	msg.Id = g_EventSobTargetSelectRelease;
	msg.pData = reinterpret_cast<void*>(&sSobTargetSelectRelease);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobAttackMarkRelease(SERIAL_HANDLE hSerialId)
{
	SNtlEventSobAttackMarkRelease sSobAttackMarkRelease;
	sSobAttackMarkRelease.hSerialId = hSerialId;

	RWS::CMsg msg;
	msg.Id = g_EventSobAttackMarkRelease;
	msg.pData = reinterpret_cast<void*>(&sSobAttackMarkRelease);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobAttackSelect(SERIAL_HANDLE hSerialId)
{
	CNtlSobAvatar* pAvatar = GetNtlSLGlobal()->GetSobAvatar();

	CNtlSobActor *pActor = (CNtlSobActor*)GetNtlSobManager()->GetSobObject(hSerialId);

	// -- targeting shit
	Logic_SobTarget(hSerialId, 0); // null coz we dont wanna follow in there
	//SERIAL_HANDLE hCurTarget = pAvatar->GetTargetID();
	//if (hCurTarget != INVALID_SERIAL_ID && hCurTarget != hSerialId)
	//	CNtlSLEventGenerator::SobTargetSelectRelease(hCurTarget);

	//pAvatar->SetTargetID(hSerialId);
	//CNtlSLEventGenerator::SobTargetSelect(hSerialId);
	//
	//API_GetSLPacketGenerator()->SendCharTargetSelect(hSerialId);
	// --

	SNtlEventSobAttackSelect sSobAttackSelect;
	sSobAttackSelect.hSerialId = hSerialId;
	sSobAttackSelect.pSobActor = pActor;

	SEND_MSG(g_EventSobAttackSelect, &sSobAttackSelect);
}

void CNtlSLEventGenerator::SobGotFocus(SERIAL_HANDLE hSerialId)
{
	SNtlEventSobGotFocus sSobGotFocus;
	sSobGotFocus.hSerialId = hSerialId;

	RWS::CMsg msg;
	msg.Id = g_EventSobGotFocus;
	msg.pData = reinterpret_cast<void*>(&sSobGotFocus);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobLostFocus(SERIAL_HANDLE hSerialId)
{
	SNtlEventSobLostFocus sSobLostFocus;
	sSobLostFocus.hSerialId = hSerialId;

	RWS::CMsg msg;
	msg.Id = g_EventSobLostFocus;
	msg.pData = reinterpret_cast<void*>(&sSobLostFocus);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::KeyboardMove(RwUInt8 byMoveFlags, RwBool bAvatarAngleChange, RwReal fAvatarAngleY) 
{
	if(Logic_IsDirectMode(GetNtlSLGlobal()->GetSobAvatar()))
		return;

	SNtlEventKeyboardMove sKeyboardMove;
	sKeyboardMove.byMoveFlags			= byMoveFlags;
	sKeyboardMove.bAvatarAngleChange	= bAvatarAngleChange;
	sKeyboardMove.fAvatarAngleY			= fAvatarAngleY;

	SEND_MSG(g_EventKeyboardMove, &sKeyboardMove);
}


void CNtlSLEventGenerator::KeyboardMoveStop(void) 
{
	if(Logic_IsDirectMode(GetNtlSLGlobal()->GetSobAvatar()))
		return;

	SNtlEventKeyboardMove sKeyboardMove;

	SEND_MSG(g_EventKeyboardMoveStop, &sKeyboardMove);
}

void CNtlSLEventGenerator::MouseDashMove(RwV3d vPickPos, RwReal fLimitDist)
{
	SNtlEventMouseDashMove sMouseDashMove;
	RwV3dAssignMacro(&sMouseDashMove.vPickPos, &vPickPos);
	sMouseDashMove.fLimitDist = fLimitDist;

	SEND_MSG(g_EventMouseDashMove, &sMouseDashMove);
}


void CNtlSLEventGenerator::KeyboardDashMove(RwUInt32 uiMoveFlags)
{
	SNtlEventKeyboardDashMove sKeyboardDashMove;
	sKeyboardDashMove.uiMoveFlags = uiMoveFlags;

	SEND_MSG(g_EventKeyboardDashMove, &sKeyboardDashMove);
}

void CNtlSLEventGenerator::AirDashMove(RwBool bMouse, RwV3d* pvPickPos, RwUInt32 uiMoveFlags, RwBool bUpdateMove/* = TRUE*/)
{
	SNtlEventAirDashMove sDashMove;

	if (bMouse)
	{
		RwV3dAssignMacro(&sDashMove.vPickPos, pvPickPos);
	}
	else
	{
		sDashMove.vPickPos.x = 0.f;
		sDashMove.vPickPos.y = 0.f;
		sDashMove.vPickPos.z = 0.f;
	}

	sDashMove.uiMoveFlags = uiMoveFlags;
	sDashMove.bMouse = bMouse;
	sDashMove.bUpdateMove = bUpdateMove;

	SEND_MSG(g_EventAirDashMove, &sDashMove);
}

void CNtlSLEventGenerator::AirAccelMove(BYTE byMoveDirection, RwBool bUpdateMove/* = TRUE*/)
{
	SNtlEventAirAccelMove sMove;

	sMove.byMoveDirection = byMoveDirection;
	sMove.bUpdateMove = bUpdateMove;

	SEND_MSG(g_EventAirAccelMove, &sMove);
}

void CNtlSLEventGenerator::InputChagneHeading(RwBool bSmoothing, RwReal fDeltaAngleY, bool bSetAngleX, RwReal fDeltaAngleX, RwBool& bSuccess)
{
	SNtlEventInputChangeHading sChangeHeading;
	sChangeHeading.bSuccess = TRUE;
	sChangeHeading.bSoothing = bSmoothing;
	sChangeHeading.fDeltaAngleY = fDeltaAngleY;

	sChangeHeading.bSetAngleX = bSetAngleX;
	sChangeHeading.fDeltaAngleX = -fDeltaAngleX;

	RWS::CMsg msg;
	msg.Id = g_EventInputChangeHeading;
	msg.pData = reinterpret_cast<void*>(&sChangeHeading);
	_SendMsg(msg);

	bSuccess = sChangeHeading.bSuccess;
}


void* CNtlSLEventGenerator::SobAvatarCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, sCHARSTATE *pCharState, RwBool bUIAvatarCreate /*= FALSE*/)
{
	SNtlEventSobAvatarCreate sSobCreate;

	sSobCreate.uiClassId = uiClassId;
	sSobCreate.hSerialId = hSerialId;
	sSobCreate.pCharState = pCharState;
	sSobCreate.bUIAvatarCreate = bUIAvatarCreate;
	sSobCreate.pSobObj = 0;

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj; 
}

void* CNtlSLEventGenerator::SobPlayerCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwV3d vLoc, RwV3d vDir, sPC_BRIEF *pPcBrief, sCHARSTATE *pCharState, RwBool bIsClientCreate /* = FALSE */)
{
	SNtlEventSobPlayerCreate sSobCreate;
	sSobCreate.uiClassId = uiClassId;
	sSobCreate.hSerialId = hSerialId;
	RwV3dAssignMacro(&sSobCreate.vLoc, &vLoc);
	RwV3dAssignMacro(&sSobCreate.vDir, &vDir);
	sSobCreate.pPcBrief = pPcBrief;
	sSobCreate.pCharState = pCharState;
	sSobCreate.pSobObj = 0;
	sSobCreate.bIsClientCreate = bIsClientCreate;

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj; 
}

void* CNtlSLEventGenerator::SobMonsterCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwV3d vLoc, RwV3d vDir, sMOB_BRIEF *pMobBrief, sCHARSTATE *pCharState, RwBool bIsClientCreate /* = FALSE */)
{
	SNtlEventSobMonsterCreate sSobCreate;
	sSobCreate.uiClassId = uiClassId;
	sSobCreate.hSerialId = hSerialId;
	RwV3dAssignMacro(&sSobCreate.vLoc, &vLoc);
	RwV3dAssignMacro(&sSobCreate.vDir, &vDir);
	sSobCreate.pCharState = pCharState;
	sSobCreate.pMobBrief = pMobBrief;
	sSobCreate.bIsClientCreate = bIsClientCreate;

	sSobCreate.pSobObj = 0;

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj; 
}

void* CNtlSLEventGenerator::SobNpcCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwV3d vLoc, RwV3d vDir, sNPC_BRIEF *pNpcBrief, sCHARSTATE *pCharState, RwBool bNameCreate, RwBool bIsClientCreate /* = FALSE */)
{
	SNtlEventSobNpcCreate sSobCreate;
	sSobCreate.uiClassId = uiClassId;
	sSobCreate.hSerialId = hSerialId;
	RwV3dAssignMacro(&sSobCreate.vLoc, &vLoc);
	RwV3dAssignMacro(&sSobCreate.vDir, &vDir);
	sSobCreate.pCharState = pCharState;
	sSobCreate.pNpcBrief = pNpcBrief;
	sSobCreate.bCreateName = bNameCreate;
	sSobCreate.bIsClientCreate = bIsClientCreate;

	sSobCreate.pSobObj = 0;

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj; 
}

void* CNtlSLEventGenerator::SobPetCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwV3d vLoc, RwV3d vDir, RwBool bOwnerAvatar, UPetData uPetBrief, sCHARSTATE *pCharState)
{
	SNtlEventSobPetCreate sSobCreate;
	sSobCreate.uiClassId = uiClassId;
	sSobCreate.hSerialId = hSerialId;
	RwV3dAssignMacro(&sSobCreate.vLoc, &vLoc);
	RwV3dAssignMacro(&sSobCreate.vDir, &vDir);
	sSobCreate.pCharState	= pCharState;
	sSobCreate.bOwnerAvatar = bOwnerAvatar;
	sSobCreate.uPetBrief	= uPetBrief;

	sSobCreate.pSobObj = 0;

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj; 
}

void* CNtlSLEventGenerator::SobWorldItemCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwV3d vLoc, RwInt32 eObjType, void* pBrief, void* pState, void* pItemOptionSet)
{
	SNtlEventSobWorldItemCreate sWorldItemCreate;
	sWorldItemCreate.uiClassId = uiClassId;
	sWorldItemCreate.hSerialId = hSerialId;
	RwV3dAssignMacro( &sWorldItemCreate.vLoc, &vLoc );
	sWorldItemCreate.eObjType = eObjType;
	sWorldItemCreate.pBrief = pBrief;
	sWorldItemCreate.pState = pState;
	sWorldItemCreate.pItemOptionSet = pItemOptionSet;

	sWorldItemCreate.pSobObj = 0;

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>( &sWorldItemCreate );
	_SendMsg( msg );

	return sWorldItemCreate.pSobObj;
}

void* CNtlSLEventGenerator::SobSlotItemCreate(void *pParentSobItem, RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, 
											  RwUInt32 uiItemTblId, sITEM_OPTION_SET& sOptionSet, RwUInt8 byPlace, RwUInt8 bySlotIdx, RwUInt8 byStackCount,
											  RwUInt8 byRank, RwUInt8 byGrade, RwUInt8 byDur, RwBool bNeedToIdentify, RwBool bCreateItemModel, RwUInt8 byBattleAttribute,
											  RwUInt8 byRestrictState, const WCHAR* wszMaker, RwUInt8 byDurationType, DBOTIME StartTime, DBOTIME EndTime)
{
	SNtlEventSobItemCreate sSobCreate;
	sSobCreate.uiClassId = uiClassId;
	sSobCreate.hSerialId = hSerialId;
	sSobCreate.pSobObj = 0;
	sSobCreate.pParentSobObj = pParentSobItem;
	sSobCreate.uiItemTblId = uiItemTblId;
	sSobCreate.sOptionSet = sOptionSet;
	memcpy(&sSobCreate.sOptionSet, &sOptionSet, sizeof(sITEM_OPTION_SET));
	sSobCreate.byPlace = byPlace;
	sSobCreate.bySlotIdx = bySlotIdx;
	sSobCreate.byStackCount = byStackCount;
	sSobCreate.byRank = byRank;
	sSobCreate.byGrade = byGrade;
	sSobCreate.byDur = byDur;
	sSobCreate.bNeedToIdentify = bNeedToIdentify;
	sSobCreate.bCreateItemModel = bCreateItemModel;
	sSobCreate.byBattleAttribute = byBattleAttribute;
	sSobCreate.byRestrictState = byRestrictState;
	sSobCreate.wszMaker = wszMaker;
	sSobCreate.byDurationType = byDurationType;
	sSobCreate.StartTime = StartTime;
	sSobCreate.EndTime = EndTime;

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj; 
}

void* CNtlSLEventGenerator::SobQuestItemCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwUInt32 uiQuestItemTblId, RwUInt8 bySlotIdx, RwUInt8 byStackCount)
{
	SNtlEventSobQuestItemCreate sSobCreate;
	sSobCreate.uiClassId = uiClassId;
	sSobCreate.hSerialId = hSerialId;
	sSobCreate.pSobObj = 0;
	sSobCreate.uiQuestItemTblId = uiQuestItemTblId;
	sSobCreate.bySlotIdx = bySlotIdx;
	sSobCreate.byStackCount = byStackCount;

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj; 
}

void* CNtlSLEventGenerator::SobStatusCreate(RwUInt32 uiClassId, RwUInt32 uiCurrHp, RwUInt32 uiDamage, RwUInt32 uiFillHp, RwUInt32 uiFillMp, RwUInt32 uiFillCp, RwBool bDamage)
{
	SNtlEventSobStatusCreate sSobCreate;
	sSobCreate.uiClassId = uiClassId;
	sSobCreate.hSerialId = INVALID_SERIAL_ID;
	sSobCreate.pSobObj = 0;
	sSobCreate.bDamage = bDamage;
	sSobCreate.uiCurrHp = uiCurrHp;
	sSobCreate.uiDamage = uiDamage;
	sSobCreate.uiFillHp = uiFillHp;
	sSobCreate.uiFillMp = uiFillMp;
	sSobCreate.uiFillCp = uiFillCp;

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj; 
}

void* CNtlSLEventGenerator::SobTriggerObject(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwUInt32 uiTblId, RwV3d vLoc, void *pPLEntity)
{
	SNtlEventSobTriggerObjectCreate sSobCreate;
	sSobCreate.uiClassId			= uiClassId;
	sSobCreate.hSerialId			= hSerialId;
	sSobCreate.uiTriggerObjectTblid = uiTblId;
	sSobCreate.vLoc					= vLoc;
	sSobCreate.pPLEntity			= pPLEntity;

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj; 
}

void* CNtlSLEventGenerator::SobDynamicObject(SERIAL_HANDLE hOwner, RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwUInt8 byType, RwUInt32 uiTblId, RwV3d vLoc, RwUInt8 byState)
{
	SNtlEventSobDynamicObjectCreate sSobCreate;
	sSobCreate.hOwner				= hOwner;
	sSobCreate.uiClassId			= uiClassId;
	sSobCreate.hSerialId			= hSerialId;
	sSobCreate.uiTableIndexForType	= uiTblId;
	sSobCreate.byType				= byType;
	sSobCreate.vLoc					= vLoc;
	sSobCreate.byState				= byState;

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj;
}

void* CNtlSLEventGenerator::SobEventObjectCreate( RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, void* pPLEntity ) 
{
	SNtlEventSobEventObjectCreate sSobCreate;
	sSobCreate.uiClassId = uiClassId;
	sSobCreate.hSerialId = hSerialId;
	sSobCreate.pPlEntity = pPLEntity;

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj;
}


void* CNtlSLEventGenerator::SobProjectileCreate(RwUInt32 uiClassId, SERIAL_HANDLE hOwnerSerialId, SERIAL_HANDLE hTargetSerialId, RwUInt32 uiBehavior, RwUInt8 byShotType,
												RwReal fSpeed, RwBool bLeftHand, RwBool bCreateHitStuffEffect, const RwChar *pBoneName, const RwChar *pEffKey, RwReal fTargetHeight, void *pProjExtData, RwReal fWeightElapsedValue /*= 1.0f*/)
{
	SNtlEventSobProjectileCreate sSobCreate;
	sSobCreate.chBoneName[0]		= 0;
	sSobCreate.chEffName[0]			= 0;

	sSobCreate.uiClassId			= uiClassId;
	sSobCreate.hSerialId			= INVALID_SERIAL_ID;
	sSobCreate.pSobObj				= 0;
	sSobCreate.hOwnerSerialId		= hOwnerSerialId;
	sSobCreate.hTargetSerialId		= hTargetSerialId;
	sSobCreate.uiBehavior			= uiBehavior;
	sSobCreate.byShotType			= byShotType;
	sSobCreate.fSpeed				= fSpeed;
	sSobCreate.bLeftHand			= bLeftHand;
	sSobCreate.bCreateHitStuffEffect = bCreateHitStuffEffect;
	if(pBoneName && strlen(pBoneName) > 0)
		strcpy_s(sSobCreate.chBoneName, NTL_MAX_BONE_NAME, pBoneName);
	if(pEffKey && strlen(pEffKey) > 0)
		strcpy_s(sSobCreate.chEffName, NTL_MAX_EFFECT_NAME, pEffKey);

	sSobCreate.fTargetHeight		= fTargetHeight;
	sSobCreate.pProjExtData			= pProjExtData;
	sSobCreate.fWeightElapsedValue	= fWeightElapsedValue;

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj; 
}

void* CNtlSLEventGenerator::SobSkillCreate(RwUInt32 uiOwnerID, RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, RwUInt32 uiSkillTblId, RwUInt8 bySlodIdx, RwUInt8 byRpBonusType, RwBool bRPBonusAutoMode, RwInt32 iExp, RwUInt32 uiRemainTime /* = 0 */)
{
	CNtlEventSobSkillCreate sSobCreate;
	sSobCreate.uiOwnerID = uiOwnerID;
	sSobCreate.uiClassId = uiClassId;
	sSobCreate.hSerialId = hSerialId;
	sSobCreate.pSobObj = 0;
	sSobCreate.uiSkillTblId = uiSkillTblId;
	sSobCreate.bySlodIdx = bySlodIdx;
	sSobCreate.byRPBonusType = byRpBonusType;
	sSobCreate.bRPBonusAutoMode = bRPBonusAutoMode;
	sSobCreate.iExp = iExp;
	sSobCreate.uiRemainTime = uiRemainTime;

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj; 
}

void* CNtlSLEventGenerator::SobHTBSkillCreate(RwUInt32 uiOwnerID, RwUInt8 uiClassId, SERIAL_HANDLE hSerialId, RwUInt32 uiHTBTblId, RwUInt8 bySlotIdx, RwUInt32 uiRemainTime /* = 0 */)
{
	SNtlEventSobHTBSkillCreate sSobCreate;
	sSobCreate.uiOwnerID = uiOwnerID;
	sSobCreate.uiClassId = uiClassId;
	sSobCreate.hSerialId = hSerialId;
	sSobCreate.pSobObj = 0;
	sSobCreate.uiHTBSkillTblId = uiHTBTblId;
	sSobCreate.bySlotIdx = bySlotIdx;
	sSobCreate.uiRemainTime = uiRemainTime;


	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj; 
}

void* CNtlSLEventGenerator::SobActionSkillCreate(RwUInt8 uiClassId, SERIAL_HANDLE hSerialId, RwUInt32 uiActionTblId, RwUInt8 bySlotIdx)
{
	SNtlEventSobActionSkillCreate sSobCreate;

	sSobCreate.uiClassId = uiClassId;
	sSobCreate.hSerialId = hSerialId;
	sSobCreate.pSobObj = 0;
	sSobCreate.uiActionSkillTblId = uiActionTblId;
	sSobCreate.bySlotIdx = bySlotIdx;

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj;
}

void* CNtlSLEventGenerator::SobBuffCreate(RwUInt32 uiClassId, SERIAL_HANDLE hSerialId, BYTE byBuffIndex, RwUInt8 byBuffType, RwUInt32 uiTblId, RwUInt32 uiRemainTime, RwUInt32 uiKeepTime, sDBO_BUFF_PARAMETER* aBuffParameter, bool bActive)
{
	CNtlEventSobBuffCreate sSobCreate;
	sSobCreate.uiClassId = uiClassId;
	sSobCreate.hSerialId = hSerialId;
	sSobCreate.pSobObj = 0;
	sSobCreate.byBuffType = byBuffType;
	sSobCreate.uiTblId = uiTblId;
	sSobCreate.uiRemainTime = uiRemainTime;
	sSobCreate.uiKeepTime = uiKeepTime;
	sSobCreate.byBuffIndex = byBuffIndex;
	sSobCreate.bActive = bActive;

	for (RwInt32 i = 0; i < NTL_MAX_EFFECT_IN_SKILL; ++i)
		memcpy(&sSobCreate.aBuffParameter[i], &aBuffParameter[i], sizeof(sDBO_BUFF_PARAMETER));
	

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj; 
}

void* CNtlSLEventGenerator::SobVehicleCreate(RwUInt32 uiClassId, sCHARSTATE* pCharState, const RwV3d& vLoc, const RwV3d& vDir, RwUInt32 uiVehicleTblIdx, RwBool bForChild, RwBool bOwnerAvatar )
{
	SNtlEventSobVehicleCreate sSobCreate;

	sSobCreate.uiClassId = uiClassId;
	sSobCreate.hSerialId = INVALID_SERIAL_ID;
	sSobCreate.pCharState = pCharState;
	sSobCreate.vLoc = vLoc;
	sSobCreate.vDir = vDir;
	sSobCreate.uiVehicleTblIdx = uiVehicleTblIdx;
	sSobCreate.bForChild = bForChild;
	sSobCreate.bOwnerAvatar = bOwnerAvatar;

	RWS::CMsg msg;
	msg.Id = g_EventSobCreate;
	msg.pData = reinterpret_cast<void*>(&sSobCreate);
	_SendMsg(msg);

	return sSobCreate.pSobObj; 
}

void CNtlSLEventGenerator::SobDelete(SERIAL_HANDLE hSerialId)
{
	SNtlEventSobDelete sSobDelete;
	sSobDelete.hSerialId = hSerialId;

	RWS::CMsg msg;
	msg.Id = g_EventSobDelete;
	msg.pData = reinterpret_cast<void*>(&sSobDelete);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobDeletePostMsg(SERIAL_HANDLE hSerialId)
{
	SNtlEventSobDelete sSobDelete;
	sSobDelete.hSerialId = hSerialId;

	RWS::CMsg msg;
	msg.Id = g_EventSobDelete;
	msg.pData = reinterpret_cast<void*>(&sSobDelete);
	PostMsg( msg, reinterpret_cast<void*>(&sSobDelete), sizeof(SNtlEventSobDelete) );
}

void CNtlSLEventGenerator::SobSummonPetSpawnSync(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hPetSerialId)
{
	SNtlEventSobSummonPetSpawnSync sSobSummonPet;
	sSobSummonPet.hSerialId		= hSerialId;
	sSobSummonPet.hPetSerialId	= hPetSerialId;

	RWS::CMsg msg;
	msg.Id = g_EventSobSummonPetSpawnSync;
	msg.pData = reinterpret_cast<void*>(&sSobSummonPet);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobPetSkillInfo( RwUInt8 bySkillCount, const sSKILL_INFO_PET* pSkillInfoPet ) 
{
	SNtlEventSobPetSkillInfo sPetSkillInfo;
	sPetSkillInfo.bySkillCount = bySkillCount;
	memcpy( sPetSkillInfo.aSkillInfo, pSkillInfoPet, bySkillCount*sizeof(sSKILL_INFO_PET) );

	RWS::CMsg msg;
	msg.Id = g_EventSobPetSkillInfo;
	msg.pData = reinterpret_cast<void*>(&sPetSkillInfo);
	_SendMsg(msg);
}


void CNtlSLEventGenerator::SobMove(SERIAL_HANDLE hSerialId, RwUInt8 byMoveFlags, RwUInt8 byFormFlag, RwV3d vCurrLoc, RwV3d vDir, RwUInt32 uiTimeStamp)
{
	SNtlEventSobMove sSobMove;
	sSobMove.hSerialId		= hSerialId;
	sSobMove.bMouseMove		= FALSE;
	sSobMove.vCurrLoc		= vCurrLoc;
	sSobMove.vDir			= vDir;
	sSobMove.byMoveFlags	= byMoveFlags;
	sSobMove.byFormFlag		= byFormFlag;
	sSobMove.uiTimeStamp	= uiTimeStamp;
	sSobMove.byPathNodeNum	= 0;
	sSobMove.pPathNode		= NULL;

    SEND_MSG(g_EventSobMove, &sSobMove);
}

void CNtlSLEventGenerator::SobMoveSync(SERIAL_HANDLE hSerialId, RwV3d& vCurLoc, RwV3d& vCurDir, DWORD dwTimeStamp)
{
    SNtlEventSobMoveSync sMoveSync;
    sMoveSync.hSerialId = hSerialId;
    sMoveSync.vLoc = vCurLoc;
    sMoveSync.vDir = vCurDir;
    sMoveSync.dwTimeStamp = dwTimeStamp;

    SEND_MSG(g_EventSobMoveSync, &sMoveSync);
}

void CNtlSLEventGenerator::SobSplineMove(SERIAL_HANDLE hSerialId, RwBool bGroundCheck, RwBool bRunMove, RwChar *pSplineName, RwV3d vStartPos)
{
	SNtlEventSobSplineMove sSobSplineMove;
	sSobSplineMove.hSerialId	= hSerialId;
	sSobSplineMove.bGroundCheck	= bGroundCheck;
	sSobSplineMove.bRunMove		= bRunMove;
	sSobSplineMove.pSplineName	= pSplineName;
    sSobSplineMove.vStartPos    = vStartPos;	

    SEND_MSG(g_EventSobSplineMove, &sSobSplineMove);
}

void CNtlSLEventGenerator::SobMoveStop(SERIAL_HANDLE hSerialId, RwV3d vCurrLoc, RwV3d vDir, RwUInt32 uiTimeStamp)
{
	SNtlEventSobMoveStop sSobMoveStop;

	sSobMoveStop.hSerialId = hSerialId;
	sSobMoveStop.vLoc = vCurrLoc;
	sSobMoveStop.vDir = vDir;

	RWS::CMsg msg;
	msg.Id = g_EventSobMoveStop;
	msg.pData = reinterpret_cast<void*>(&sSobMoveStop);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobDestMove(SERIAL_HANDLE hSerialId, RwUInt8 byFormFlag, RwV3d vCurrLoc, RwV3d vDest, RwUInt32 uiTimeStamp, RwUInt8 byPathNodeNum /*= 0*/, RwV3d *pPathNode /*= NULL*/)
{
	SNtlEventSobMove sSobMove;
	sSobMove.hSerialId		= hSerialId;
	sSobMove.bMouseMove		= TRUE;
	sSobMove.vCurrLoc		= vCurrLoc;
	sSobMove.vLoc			= vDest;
	sSobMove.byFormFlag		= byFormFlag;
	sSobMove.uiTimeStamp	= uiTimeStamp;
	sSobMove.byPathNodeNum	= byPathNodeNum;
	sSobMove.pPathNode		= pPathNode;

	RWS::CMsg msg;
	msg.Id = g_EventSobMove;
	msg.pData = reinterpret_cast<void*>(&sSobMove);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobSecondDestMove(SERIAL_HANDLE hSerialId, RwUInt8 byFormFlag, RwV3d vCurrLoc, RwV3d vDest, RwV3d vSecondDest, RwUInt32 uiTimeStamp)
{
	SNtlEventSobSecondDestMove sSobSecondDestMove;
	sSobSecondDestMove.hSerialId		= hSerialId;
	sSobSecondDestMove.byFormFlag		= byFormFlag;
	sSobSecondDestMove.vCurrLoc			= vCurrLoc;
	sSobSecondDestMove.vLoc				= vDest;
	sSobSecondDestMove.vSecondDest		= vSecondDest;
	sSobSecondDestMove.uiTimeStamp		= uiTimeStamp;

	RWS::CMsg msg;
	msg.Id = g_EventSobSecondDestMove;
	msg.pData = reinterpret_cast<void*>(&sSobSecondDestMove);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobDirectionFloat(SERIAL_HANDLE hSerialId, RwUInt8 byMoveFlags)
{
	SNtlEventSobDirectionFloat sFloat;
	sFloat.hSerialId		= hSerialId;
	sFloat.byMoveFlags		= byMoveFlags;

	RWS::CMsg msg;
	msg.Id = g_EventSobDirectionFloat;
	msg.pData = reinterpret_cast<void*>(&sFloat);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobAdjustMovePosition(SERIAL_HANDLE hSerialId, RwV3d vAdjustPos)
{
	SNtlEventSobAdjustMovePosition sSobAdjustMovePos;
	sSobAdjustMovePos.hSerialId		= hSerialId;
	sSobAdjustMovePos.vAdjustPos	= vAdjustPos;

	RWS::CMsg msg;
	msg.Id = g_EventSobAdjustMovePosition;
	msg.pData = reinterpret_cast<void*>(&sSobAdjustMovePos);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobDash(SERIAL_HANDLE hSerialId, RwV3d vDest, RwUInt8 byMoveFlags, RwUInt32 uiTimeStamp)
{
	SNtlEventSobDash sSobDash;

	sSobDash.hSerialId = hSerialId;
	sSobDash.vDest = vDest;
	sSobDash.byMoveFlags = byMoveFlags;
	sSobDash.uiTimeStamp = uiTimeStamp;

	RWS::CMsg msg;
	msg.Id = g_EventSobDash;
	msg.pData = reinterpret_cast<void*>(&sSobDash);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobFollowMove(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hTargetSerialId, RwUInt8 byFormFlag, RwV3d vCurrLoc, RwReal fFollowRange, RwUInt8 byReason, RwUInt32 uiTimeStamp)
{
	SNtlEventSobFollowMove sSobFollowMove;
	sSobFollowMove.hSerialId		= hSerialId;
	sSobFollowMove.hTargetSerialId	= hTargetSerialId;
	sSobFollowMove.byFormFlag		= byFormFlag;
	sSobFollowMove.vCurrLoc			= vCurrLoc;
	sSobFollowMove.fFollowRange		= fFollowRange;
	sSobFollowMove.byReason			= byReason;
	sSobFollowMove.uiTimeStamp		= uiTimeStamp;

	RWS::CMsg msg;
	msg.Id = g_EventSobFollowMove;
	msg.pData = reinterpret_cast<void*>(&sSobFollowMove);
	_SendMsg(msg);
}


void CNtlSLEventGenerator::SobAdjustMovement(SERIAL_HANDLE hSerialId, RwV3d vDest, RwV3d vHeading)
{
	SNtlEventSobAdjustMove sSobAdjustMove;
	sSobAdjustMove.hSerialId	= hSerialId;
	sSobAdjustMove.vDest		= vDest;
	sSobAdjustMove.vHeading		= vHeading;

	RWS::CMsg msg;
	msg.Id = g_EventSobAdjustMove;
	msg.pData = reinterpret_cast<void*>(&sSobAdjustMove);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobItemAdd(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hItemSerialId, RwUInt32 uiItemTblId, 
										sITEM_OPTION_SET* sOptionSet, RwUInt8 byPlace, RwUInt8 bySlotIdx, RwUInt8 byStackCount,
									  RwUInt8 byRank, RwUInt8 byGrade, RwUInt8 byDur, RwUInt8 byMaxDur, RwBool bNeedToIdentify, RwUInt8 byBattleAttribute,
									  RwUInt8 byRestrictState, const WCHAR* wszMaker, RwUInt8 byDurationType, DBOTIME StartTime, DBOTIME EndTime)
{
	SNtlEventSobItemAdd sSobItemAdd;

	sSobItemAdd.hSerialId= hSerialId;
	sSobItemAdd.hItemSerialId = hItemSerialId;
	memcpy(&sSobItemAdd.sOptionSet, sOptionSet, sizeof(sITEM_OPTION_SET));
	sSobItemAdd.uiItemTblId = uiItemTblId;
	sSobItemAdd.byPlace = byPlace;
	sSobItemAdd.bySlotIdx = bySlotIdx;
	sSobItemAdd.byStackCount = byStackCount;
	sSobItemAdd.byRank = byRank;
	sSobItemAdd.byGrade = byGrade;
	sSobItemAdd.byDur = byDur;
	sSobItemAdd.byMaxDur = byMaxDur;
	sSobItemAdd.bNeedToIdentify = bNeedToIdentify;
	sSobItemAdd.byBattleAttribute = byBattleAttribute;
	sSobItemAdd.byRestrictState = byRestrictState;
	sSobItemAdd.wszMaker = wszMaker;
	sSobItemAdd.byDurationType = byDurationType;
	sSobItemAdd.StartTime = StartTime;
	sSobItemAdd.EndTime = EndTime;

	RWS::CMsg msg;
	msg.Id = g_EventSobItemAdd;
	msg.pData = reinterpret_cast<void*>(&sSobItemAdd);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobItemUpdate(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hItemSerialId, RwUInt32 uiItemTblId, 
										sITEM_OPTION_SET* sOptionSet, RwUInt8 byPlace, RwUInt8 bySlotIdx, RwUInt8 byStackCount,
										 RwUInt8 byRank, RwUInt8 byGrade, RwUInt8 byDur, RwUInt8 byMaxDur, RwBool bNeedToIdentify, RwUInt8 byBattleAttribute,
										 RwUInt8 byRestrictState, const WCHAR* wszMaker, RwUInt8 byDurationType, DBOTIME StartTime, DBOTIME EndTime)
{
	SNtlEventSobItemUpdate sSobItemUpdate;

	sSobItemUpdate.hSerialId= hSerialId;
	sSobItemUpdate.hItemSerialId = hItemSerialId;
	sSobItemUpdate.uiItemTblId = uiItemTblId;
	memcpy(&sSobItemUpdate.sOptionSet, sOptionSet, sizeof(sITEM_OPTION_SET));
	sSobItemUpdate.byPlace = byPlace;
	sSobItemUpdate.bySlotIdx = bySlotIdx;
	sSobItemUpdate.byStackCount = byStackCount;
	sSobItemUpdate.byRank = byRank;
	sSobItemUpdate.byGrade = byGrade;
	sSobItemUpdate.byDur = byDur;
	sSobItemUpdate.byMaxDur = byMaxDur;
	sSobItemUpdate.bNeedToIdentify = bNeedToIdentify;
	sSobItemUpdate.byBattleAttribute = byBattleAttribute;
	sSobItemUpdate.byRestrictState = byRestrictState;
	sSobItemUpdate.wszMaker = wszMaker;
	sSobItemUpdate.byDurationType = byDurationType;
	sSobItemUpdate.StartTime = StartTime;
	sSobItemUpdate.EndTime = EndTime;

	RWS::CMsg msg;
	msg.Id = g_EventSobItemUpdate;
	msg.pData = reinterpret_cast<void*>(&sSobItemUpdate);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobItemDelete(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hItemSerialId, RwUInt8 byPlace, RwUInt8 bySlotIdx)
{
	SNtlEventSobItemDelete sSobItemDelete;

	sSobItemDelete.hSerialId = hSerialId;
	sSobItemDelete.hItemSerialId = hItemSerialId;
	sSobItemDelete.byPlace = byPlace;
	sSobItemDelete.bySlotIdx = bySlotIdx;

	RWS::CMsg msg;
	msg.Id = g_EventSobItemDelete;
	msg.pData = reinterpret_cast<void*>(&sSobItemDelete);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ItemSocketInsertBead(SERIAL_HANDLE hSerialId, RwBool bSuccess, RwUInt8 byItemPlace, RwUInt8 byItemPos, RwUInt8 byRestrictState, RwUInt8 byDurationType,
	DBOTIME nUseStartTime, DBOTIME nUseEndTime, sITEM_RANDOM_OPTION* pRandomOption, RwUInt8 byBeadPlace, RwUInt8 byBeadPos, RwUInt8 byBeadRemainStack)
{
	SNtlEventItemSocketInsertBead sEvent;

	sEvent.hSerialId = hSerialId;
	sEvent.bSuccess = bSuccess;
	sEvent.byItemPlace = byItemPlace;
	sEvent.byItemPos = byItemPos;
	sEvent.byRestrictState = byRestrictState;
	sEvent.byDurationType = byDurationType;
	sEvent.nUseStartTime = nUseStartTime;
	sEvent.nUseEndTime = nUseEndTime;
	sEvent.pRandomOption = pRandomOption;
	sEvent.byBeadPlace = byBeadPlace;
	sEvent.byBeadPos = byBeadPos;
	sEvent.byBeadRemainStack = byBeadRemainStack;

	RWS::CMsg msg;
	msg.Id = g_EventItemSocketInsertBead;
	msg.pData = reinterpret_cast<void*>(&sEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ItemSocketDestroyBead(SERIAL_HANDLE hSerialId, RwUInt8 byPlace, RwUInt8 byPos, RwUInt8 byRestrictState, RwUInt8 byDurationType)
{
	SNtlEventItemSocketDestroyBead sEvent;

	sEvent.hSerialId = hSerialId;
	sEvent.byPlace = byPlace;
	sEvent.byPos = byPos;
	sEvent.byRestrictState = byRestrictState;
	sEvent.byDurationType = byDurationType;

	RWS::CMsg msg;
	msg.Id = g_EventItemSocketDestroyBead;
	msg.pData = reinterpret_cast<void*>(&sEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobAvatarItemMove(SERIAL_HANDLE hSerial, SERIAL_HANDLE hSrcSerial, RwUInt8 bySrcPlace, RwUInt8 bySrcSlotIdx, 
											 SERIAL_HANDLE hDestSerial, RwUInt8 byDestPlace, RwUInt8 byDestSlotIdx)
{
	SNtlEventSobItemMove sSobItemMove;

	sSobItemMove.hSerialId= hSerial;
	sSobItemMove.hSrcSerial = hSrcSerial;
	sSobItemMove.bySrcPlace = bySrcPlace;
	sSobItemMove.bySrcSlotIdx = bySrcSlotIdx;
	sSobItemMove.hDestSerial = hDestSerial;
	sSobItemMove.byDestPlace = byDestPlace;
	sSobItemMove.byDestSlotIdx = byDestSlotIdx;

	RWS::CMsg msg;
	msg.Id = g_EventSobItemMove;
	msg.pData = reinterpret_cast<void*>(&sSobItemMove);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobAvatarItemStackMove(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hSrcSerial, RwUInt8 bySrcPlace, RwUInt8 bySrcSlotIdx, RwUInt8 bySrcStackCount, 
												  SERIAL_HANDLE hDestSerial, RwUInt8 byDestPlace, RwUInt8 byDestSlotIdx, RwUInt8 byDestStackCount)
{
	SNtlEventSobItemStackMove sSobItemStackMove;

	sSobItemStackMove.hSerialId = hSerialId;
	sSobItemStackMove.hSrcSerial = hSrcSerial;
	sSobItemStackMove.bySrcPlace = bySrcPlace;
	sSobItemStackMove.bySrcSlotIdx = bySrcSlotIdx;
	sSobItemStackMove.bySrcStackCount = bySrcStackCount;
	sSobItemStackMove.hDestSerial = hDestSerial;
	sSobItemStackMove.byDestPlace = byDestPlace;
	sSobItemStackMove.byDestSlotIdx = byDestSlotIdx;
	sSobItemStackMove.byDestStackCount = byDestStackCount;

	RWS::CMsg msg;
	msg.Id = g_EventSobItemStackMove;
	msg.pData = reinterpret_cast<void*>(&sSobItemStackMove);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobEquipModelChange(SERIAL_HANDLE hSerialId, RwUInt8 bySlotIdx, RwUInt32 uiTblId, RwUInt8 byGrade, RwUInt8 byBattleAttribute)
{
	SNtlEventSobEquipChange sSobEquipChange;
	sSobEquipChange.hSerialId	= hSerialId;
	sSobEquipChange.bySlotIdx	= bySlotIdx;
	sSobEquipChange.uiTblId		= uiTblId;
	sSobEquipChange.byGrade		= byGrade;
	sSobEquipChange.byBattleAttribute = byBattleAttribute;

	SEND_MSG(g_EventSobEquipChange, &sSobEquipChange);
}

void CNtlSLEventGenerator::SobEquipItemDurDown(RwUInt8* pArrDurablity)
{
	RWS::CMsg msg;
	msg.Id = g_EventSobEquipItemDurDown;
	msg.pData = reinterpret_cast<void*>( pArrDurablity );
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobEquipItemDurUpdate(SERIAL_HANDLE hSerialId, RwUInt8 byArrDurablity)
{
	SNtlEventSobItemDurUpdate sSobItemDurUpdate;
	sSobItemDurUpdate.hSerialId	= hSerialId;
	sSobItemDurUpdate.byDur		= byArrDurablity;

	RWS::CMsg msg;
	msg.Id = g_EventSobItemDurUpdate;
	msg.pData = reinterpret_cast<void*>(&sSobItemDurUpdate );
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobItemUseAction(SERIAL_HANDLE hSerialId, RwUInt32 uiTableIdx, RwUInt8 ucSkillResultCount, sSKILL_RESULT* pSkillResult)
{
	SNtlEventSobItemUseAction sSobItemUseAction;
	sSobItemUseAction.hSerialId				= hSerialId;
	sSobItemUseAction.uiItemTableIdx		= uiTableIdx;
	sSobItemUseAction.ucSkillResultCount	= ucSkillResultCount;
	sSobItemUseAction.pSkillResult			= pSkillResult;

	SEND_MSG(g_EventSobItemUseAction, &sSobItemUseAction);
}

void CNtlSLEventGenerator::SobItemUpgrade(BYTE byItemGrade, BYTE byItemPlace, BYTE byItemPos, BYTE byStonePlace, BYTE byStonePos, BYTE byStoneStack, BYTE byCorePlace, BYTE byCorePos, BYTE byCoreStack, bool bItemDestroy)
{
	SNtlEventSobItemUpgrade sSobItemUpgrade;
	sSobItemUpgrade.byItemGrade = byItemGrade;
	sSobItemUpgrade.byItemPlace = byItemPlace;
	sSobItemUpgrade.byItemPos = byItemPos;
	sSobItemUpgrade.byStonePlace = byStonePlace;
	sSobItemUpgrade.byStonePos = byStonePos;
	sSobItemUpgrade.byStoneStack = byStoneStack;
	sSobItemUpgrade.byCorePlace = byCorePlace;
	sSobItemUpgrade.byCorePos = byCorePos;
	sSobItemUpgrade.byCoreStack = byCoreStack;
	sSobItemUpgrade.bItemDestroy = bItemDestroy;

	SEND_MSG(g_EventSobItemUpgrade, &sSobItemUpgrade);
}

void CNtlSLEventGenerator::SobItemChangeOptionSet(void * pOptionSet, BYTE byItemPlace, BYTE byItemPos, BYTE byKitPlace, BYTE byKitPos, BYTE byKitRemainStack)
{
	SNtlEventSobItemChangeOptionSet sSobItem;
	sSobItem.pOptionSet = pOptionSet;
	sSobItem.byItemPlace = byItemPlace;
	sSobItem.byItemPos = byItemPos;
	sSobItem.byKitPlace = byKitPlace;
	sSobItem.byKitPos = byKitPos;
	sSobItem.byKitRemainStack = byKitRemainStack;

	SEND_MSG(g_EventSobItemChangeOptionSet, &sSobItem);
}

void CNtlSLEventGenerator::SobItemChangeBattleAttribute(RwUInt8 byBattleAttribute, RwUInt8 byItemPlace, RwUInt8 byItemPos)
{
	SNtlEventSobItemChangeBattleAttribute sSobUpdate;
	sSobUpdate.byBattleAttribute = byBattleAttribute;
	sSobUpdate.byItemPlace = byItemPlace;
	sSobUpdate.byItemPos = byItemPos;

	SEND_MSG(g_EventSobItemChangeBattleAttribute, &sSobUpdate);
}

void CNtlSLEventGenerator::SobEquipItemUpgradeAll(RwUInt8 byCount, RwUInt8* abyUpgradeValue)
{
	SNtlEventSobEquippedItemUpgradeAll sSobItemUpgrade;
	sSobItemUpgrade.byCount = byCount;
	sSobItemUpgrade.abyUpgradeValue = abyUpgradeValue;

	RWS::CMsg msg;
	msg.Id = g_EventSobEquippedItemUpgradeAll;
	msg.pData = reinterpret_cast<void*>( &sSobItemUpgrade );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::SobItemCancel(SERIAL_HANDLE hSerialId)
{
	SNtlEventSobItemCancel sSobItemCancel;
	sSobItemCancel.hSerialId = hSerialId;

	RWS::CMsg msg;
	msg.Id = g_EventSobItemCancel;
	msg.pData = reinterpret_cast<void*>( &sSobItemCancel );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::SobItemUseResFailed( SERIAL_HANDLE hSerialId )
{
	SNtlEventSob sSobEvent;
	sSobEvent.hSerialId = hSerialId;

	SEND_MSG( g_EventSobItemUseResFailed, &sSobEvent );
}

void CNtlSLEventGenerator::SobItemCasting(SERIAL_HANDLE hSerialId, RwUInt32 uiTargetSerialI, RwUInt32 uiItemTblId, RwReal fCurrCastingTime, RwReal fMaxCastingTime)
{
	SNtlEventSobItemCasting sSobItemCasting;
	sSobItemCasting.hSerialId			= hSerialId;
	sSobItemCasting.hTargetSerialId		= uiTargetSerialI;
	sSobItemCasting.uiItemTblId			= uiItemTblId;
	sSobItemCasting.fCurrCastingTime	= fCurrCastingTime;
	sSobItemCasting.fMaxCastingTime		= fMaxCastingTime;

	RWS::CMsg msg;
	msg.Id = g_EventSobItemCasting;
	msg.pData = reinterpret_cast<void*>( &sSobItemCasting );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::SobCheckItemInOut(void)
{
	RWS::CMsg msg;
	msg.Id = g_EventSobCheckItemInOut;
	msg.pData = reinterpret_cast<void*>( NULL );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::SobItemRestrictUpdate(BYTE byPlace, BYTE byPos, BYTE byRestrictState)
{
	SNtlEventSobItemRestrictionUpdate sEvent;
	sEvent.byPlace = byPlace;
	sEvent.byPos = byPos;
	sEvent.byRestrictState = byRestrictState;

	SEND_MSG(g_EventSobItemRestrictionUpdate, &sEvent);
}


void CNtlSLEventGenerator::EventPartySelectStateInit(BYTE bySelectState, DWORD dwParam, bool bLastStage)
{
	SNtlEventPartySelectStateInit sEvent;
	sEvent.bySelectState = bySelectState;
	sEvent.dwParam = dwParam;
	sEvent.bLastStage = bLastStage;

	SEND_MSG(g_EventPartySelectStateInit, &sEvent);
}

void CNtlSLEventGenerator::EventPartySelectState(HOBJECT hTarget, BYTE bySelectState)
{
	SNtlEventPartySelectState sEvent;
	sEvent.hTarget = hTarget;
	sEvent.bySelectState = bySelectState;

	SEND_MSG(g_EventPartySelectState, &sEvent);
}

void CNtlSLEventGenerator::EventBattleDungeonStateUpdate(BYTE byStage, TBLIDX titleTblidx, TBLIDX subTitleTblidx, DWORD dwLimitTime)
{
	SNtlEventBattleDungeonStateUpdate sEvent;
	sEvent.byStage = byStage;
	sEvent.titleTblidx = titleTblidx;
	sEvent.subTitleTblidx = subTitleTblidx;
	sEvent.dwLimitTime = dwLimitTime;

	SEND_MSG(g_EventBattleDungeonStateUpdate, &sEvent);
}

void CNtlSLEventGenerator::EventBattleDungeonStageClear()
{
	SEND_MSG(g_EventBattleDungeonStageClear, NULL);
}

void CNtlSLEventGenerator::EventBattleDungeonLimitTime(DWORD dwLimitTime, bool bStart)
{
	SNtlEventBattleDungeonLimitTime sEvent;
	sEvent.dwLimitTime = dwLimitTime;
	sEvent.bStart = bStart;

	SEND_MSG(g_EventBattleDungeonLimitTime, &sEvent);
}

void CNtlSLEventGenerator::EventBattleDungeonFail()
{
	SEND_MSG(g_EventBattleDungeonFail, NULL);
}

void CNtlSLEventGenerator::EventBattleDungeonRewardFinish()
{
	SEND_MSG(g_EventBattleDungeonRewardFinish, NULL);
}


void CNtlSLEventGenerator::SobDeleteQuickSlotIcon(SERIAL_HANDLE hSerialId)
{
	SNtlEventSobDeleteQuickSlotIcon sEvent;
	sEvent.hSerialId		= hSerialId;

	SEND_MSG(g_EventSobDeleteQuickSlotIcon, &sEvent);
}

void CNtlSLEventGenerator::SobWarehouseUpdate(RwUInt32 uiMessageType, RwUInt32 uiSerial, RwUInt32 uiValue /* =0 */, RwInt32 iValue2 /* =0 */)
{
	SNtlEventSobWarehouseUpdate sSobWarehouseUpdate;

	sSobWarehouseUpdate.uiMessageType	= uiMessageType;
	sSobWarehouseUpdate.uiSerial		= uiSerial;
	sSobWarehouseUpdate.uiValue			= uiValue;
	sSobWarehouseUpdate.iValue2			= iValue2;

	RWS::CMsg msg;
	msg.Id = g_EventSobWarehouseUpdate;
	msg.pData = reinterpret_cast<void*>(&sSobWarehouseUpdate);
	_SendMsg( msg );
}

void CNtlSLEventGenerator::SobWarehouseItemCreate(RwUInt8 byItemCount, void* pData)
{
	SNtlEventSobWarehouseItemCreate sSobWarehouseItemCreate;

	sSobWarehouseItemCreate.byItemCount = byItemCount;
	sSobWarehouseItemCreate.pData		= pData;

	RWS::CMsg msg;
	msg.Id = g_EventSobWarehouseItemCreate;
	msg.pData = reinterpret_cast<void*>(&sSobWarehouseItemCreate);
	_SendMsg( msg );
}

void CNtlSLEventGenerator::SobWarehouseItemAdd(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hItemSerialId, RwUInt32 uiItemTblId, 
												sITEM_OPTION_SET* sOptionSet, RwUInt8 byPlace, RwUInt8 bySlotIdx, RwUInt8 byStackCount,
											   RwUInt8 byRank, RwUInt8 byGrade, RwUInt8 byDur, RwUInt8 byMaxDur, RwBool bNeedToIdentify, RwUInt8 byBattleAttribute,
											   RwUInt8 byRestrictState, const WCHAR* wszMaker, RwUInt8 byDurationType, DBOTIME StartTime, DBOTIME EndTime)
{
	SNtlEventSobItemAdd sSobItemAdd;

	sSobItemAdd.hSerialId= hSerialId;
	sSobItemAdd.hItemSerialId = hItemSerialId;
	memcpy(&sSobItemAdd.sOptionSet, sOptionSet, sizeof(sITEM_OPTION_SET));
	sSobItemAdd.uiItemTblId = uiItemTblId;
	sSobItemAdd.byPlace = byPlace;
	sSobItemAdd.bySlotIdx = bySlotIdx;
	sSobItemAdd.byStackCount = byStackCount;
	sSobItemAdd.byRank = byRank;
	sSobItemAdd.byGrade = byGrade;
	sSobItemAdd.byDur = byDur;
	sSobItemAdd.byMaxDur = byMaxDur;
	sSobItemAdd.bNeedToIdentify = bNeedToIdentify;
	sSobItemAdd.byBattleAttribute = byBattleAttribute;
	sSobItemAdd.byRestrictState = byRestrictState;
	sSobItemAdd.wszMaker = wszMaker;
	sSobItemAdd.byDurationType = byDurationType;
	sSobItemAdd.StartTime = StartTime;
	sSobItemAdd.EndTime = EndTime;

	RWS::CMsg msg;
	msg.Id = g_EventSobWarehouseItemAdd;
	msg.pData = reinterpret_cast<void*>(&sSobItemAdd);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobWarehouseItemDelete(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hItemSerialId, RwUInt8 byPlace, RwUInt8 bySlotIdx)
{
	SNtlEventSobItemDelete sSobItemDelete;

	sSobItemDelete.hSerialId = hSerialId;
	sSobItemDelete.hItemSerialId = hItemSerialId;
	sSobItemDelete.byPlace = byPlace;
	sSobItemDelete.bySlotIdx = bySlotIdx;

	RWS::CMsg msg;
	msg.Id = g_EventSobWarehouseItemDelete;
	msg.pData = reinterpret_cast<void*>(&sSobItemDelete);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobWarehouseItemMove(SERIAL_HANDLE hSerial, SERIAL_HANDLE hSrcSerial, RwUInt8 bySrcPlace, RwUInt8 bySrcSlotIdx, 
												SERIAL_HANDLE hDestSerial, RwUInt8 byDestPlace, RwUInt8 byDestSlotIdx)
{
	SNtlEventSobItemMove sSobItemMove;

	sSobItemMove.hSerialId= hSerial;
	sSobItemMove.hSrcSerial = hSrcSerial;
	sSobItemMove.bySrcPlace = bySrcPlace;
	sSobItemMove.bySrcSlotIdx = bySrcSlotIdx;
	sSobItemMove.hDestSerial = hDestSerial;
	sSobItemMove.byDestPlace = byDestPlace;
	sSobItemMove.byDestSlotIdx = byDestSlotIdx;

	RWS::CMsg msg;
	msg.Id = g_EventSobWarehouseItemMove;
	msg.pData = reinterpret_cast<void*>(&sSobItemMove);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobWarehouseItemMoveStack(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hSrcSerial, RwUInt8 bySrcPlace, RwUInt8 bySrcSlotIdx, RwUInt8 bySrcStackCount, 
													 SERIAL_HANDLE hDestSerial, RwUInt8 byDestPlace, RwUInt8 byDestSlotIdx, RwUInt8 byDestStackCount)
{
	SNtlEventSobItemStackMove sSobItemStackMove;

	sSobItemStackMove.hSerialId = hSerialId;
	sSobItemStackMove.hSrcSerial = hSrcSerial;
	sSobItemStackMove.bySrcPlace = bySrcPlace;
	sSobItemStackMove.bySrcSlotIdx = bySrcSlotIdx;
	sSobItemStackMove.bySrcStackCount = bySrcStackCount;
	sSobItemStackMove.hDestSerial = hDestSerial;
	sSobItemStackMove.byDestPlace = byDestPlace;
	sSobItemStackMove.byDestSlotIdx = byDestSlotIdx;
	sSobItemStackMove.byDestStackCount = byDestStackCount;

	RWS::CMsg msg;
	msg.Id = g_EventSobWarehouseItemStackMove;
	msg.pData = reinterpret_cast<void*>(&sSobItemStackMove);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobQuestItemAdd(SERIAL_HANDLE hSerialId, RwUInt32 uiQuestItemTblId, RwUInt8 bySlotIdx, RwUInt8 byStackCount)
{
	SNtlEventSobQuestItemAdd sSobQuestItemAdd;
	sSobQuestItemAdd.hSerialId = hSerialId;
	sSobQuestItemAdd.uiQuestItemTblId = uiQuestItemTblId;
	sSobQuestItemAdd.bySlotIdx = bySlotIdx;
	sSobQuestItemAdd.byStackCount = byStackCount;

	RWS::CMsg msg;
	msg.Id = g_EventSobQuestItemAdd;
	msg.pData = reinterpret_cast<void*>( &sSobQuestItemAdd );
	_SendMsg( msg );
}


void CNtlSLEventGenerator::SobQuestItemDelete(SERIAL_HANDLE hSerialId, RwUInt8 ucDeleteSlotIdx)
{
	SNtlEventSobQuestItemDelete sSobQuestItemDelete;
	sSobQuestItemDelete.hSerialId = hSerialId;
	sSobQuestItemDelete.bySlotIdx = ucDeleteSlotIdx;

	RWS::CMsg msg;
	msg.Id = g_EventSobQuestItemDelete;
	msg.pData = reinterpret_cast<void*>( &sSobQuestItemDelete );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::SobAvatarQuestItemMove(SERIAL_HANDLE hSerialId, RwUInt8 ucSrcSlotIdx, RwUInt32 uiSrcTblIdx, RwUInt8 ucDestSlotIdx, RwUInt32 uiDestTblIdx)
{
	SNtlEventSobQuestItemMove	sSobQuestItemMove;
	sSobQuestItemMove.hSerialId = hSerialId;
	sSobQuestItemMove.bySrcSlotIdx = ucSrcSlotIdx;
	sSobQuestItemMove.uiSrcTblIdx = uiSrcTblIdx;
	sSobQuestItemMove.byDestSlotIdx = ucDestSlotIdx;
	sSobQuestItemMove.uiDestTblIdx = uiDestTblIdx;

	RWS::CMsg msg;
	msg.Id = g_EventSobQuestItemMove;
	msg.pData = reinterpret_cast<void*>( &sSobQuestItemMove );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::SobQuestItemUpdate(SERIAL_HANDLE hSerialId, RwUInt8 ucSlotIdx, RwUInt8 ucCount)
{
	SNtlEventSobQuestItemUpdate sSobQuestItemUpdate;
	sSobQuestItemUpdate.hSerialId = hSerialId;
	sSobQuestItemUpdate.bySlotIdx = ucSlotIdx;
	sSobQuestItemUpdate.byCount = ucCount;

	RWS::CMsg msg;
	msg.Id = g_EventSobQuestItemUpdate;
	msg.pData = reinterpret_cast<void*>( &sSobQuestItemUpdate );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::SobStanding(SERIAL_HANDLE hSerialId, RwV3d vLoc, RwV3d vDir)
{
	SNtlEventSobStanding sSobStanding;

	sSobStanding.hSerialId = hSerialId;
	RwV3dAssignMacro(&sSobStanding.vLoc, &vLoc);
	RwV3dAssignMacro(&sSobStanding.vDir, &vDir);

	SEND_MSG(g_EventSobStanding, &sSobStanding);
}

void CNtlSLEventGenerator::SobFightingMode(SERIAL_HANDLE hSerialId, RwBool bFightingMode)
{
	SNtlEventSobFightingMode sSobFightingMode;

	sSobFightingMode.hSerialId = hSerialId;
	sSobFightingMode.bFightingMode = bFightingMode;

	RWS::CMsg msg;
	msg.Id = g_EventSobFightingMode;
	msg.pData = reinterpret_cast<void*>(&sSobFightingMode);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobAttack(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hAttackerSerialId, SERIAL_HANDLE hDefenderSpawnId, 
									 RwBool bChainAtt, RwUInt8 byAttRes, RwUInt8 byAttSeq, RwUInt8 byBlocking, 
									 RwUInt32 uiDamage, RwUInt32 uiAbsorbedDamage, RwUInt32 uiReflectedDamage,
	sDBO_LP_EP_RECOVERED& lpEpRecovered, RwUInt32 uiSyncEventId)
{
	SNtlEventSobAttack sSobAttack;

	sSobAttack.hSerialId			= hSerialId;
	sSobAttack.hAttackerSerialId	= hAttackerSerialId;
	sSobAttack.hDefenderSerialId	= hDefenderSpawnId;
	sSobAttack.bChainAtt			= bChainAtt;
	sSobAttack.byAttRes				= byAttRes;
	sSobAttack.byAttSeq				= byAttSeq;
	sSobAttack.byBlocking			= byBlocking;
	sSobAttack.uiDamage				= uiDamage;
	sSobAttack.uiAbsorbedDamage		= uiAbsorbedDamage;
	sSobAttack.uiReflectedDamage	= uiReflectedDamage;
	sSobAttack.lpEpRecovered		= lpEpRecovered;
	sSobAttack.uiSyncEventId		= uiSyncEventId;

	sSobAttack.vPushDir.x = 0.0f;
	sSobAttack.vPushDir.y = 0.0f;
	sSobAttack.vPushDir.z = 0.0f;

	SEND_MSG(g_EventSobAttack, &sSobAttack);
}

void CNtlSLEventGenerator::SobSpecialAttack(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hAttackerSerialId, SERIAL_HANDLE hDefenderSpawnId, BYTE byAttackType, BYTE bySourceType, TBLIDX sourceTblidx, BYTE byAttackResult, RwUInt32 uiDamage)
{
	SNtlEventSobSpecialAttack sSobAttack;

	sSobAttack.hSerialId = hSerialId;
	sSobAttack.hAttackerSerialId = hAttackerSerialId;
	sSobAttack.hDefenderSerialId = hDefenderSpawnId;
	sSobAttack.byAttackType = byAttackType;
	sSobAttack.bySourceType = bySourceType;
	sSobAttack.sourceTblidx = sourceTblidx;
	sSobAttack.byAttackResult = byAttackResult;
	sSobAttack.uiDamage = uiDamage;

	SEND_MSG(g_EventSobSpecialAttack, &sSobAttack);
}

void CNtlSLEventGenerator::SobBattleNotification(SERIAL_HANDLE hSerialId)
{
	SNtlEventSob sEvent;
	sEvent.hSerialId = hSerialId;

	SEND_MSG(g_EventSobBattleNotification, &sEvent);
}

void CNtlSLEventGenerator::SobAttacked(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hAttackerSerialId)
{
	SNtlEventSobAttacked sSobAttacked;

	sSobAttacked.hSerialId			= hSerialId;
	sSobAttacked.hAttackerSerialId	= hAttackerSerialId;

	RWS::CMsg msg;
	msg.Id = g_EventSobAttacked;
	msg.pData = reinterpret_cast<void*>(&sSobAttacked);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobPetBeginAttack(SERIAL_HANDLE hAttackerSerialIdx, SERIAL_HANDLE hDefenderSpawnId)
{
	SNtlEventSobPetBeginAttack sSobPetBeginAttack;

	sSobPetBeginAttack.hSerialId = hAttackerSerialIdx;
	sSobPetBeginAttack.hDefenderSerialId = hDefenderSpawnId;

	RWS::CMsg msg;
	msg.Id = g_EventSobPetBeginAttack;
	msg.pData = reinterpret_cast<void*>(&sSobPetBeginAttack);
	_SendMsg(msg);
}


void CNtlSLEventGenerator::SobHit(SERIAL_HANDLE hSerialId, const SHitStuff *pHitStuff)
{
	SNtlEventSobHit sSobHit;

	sSobHit.hSerialId = hSerialId;
	memcpy(&sSobHit.sHitStuff, pHitStuff, sizeof(SHitStuff));

	RWS::CMsg msg;
	msg.Id = g_EventSobHit;
	msg.pData = reinterpret_cast<void*>(&sSobHit);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobAttackFollow(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hDefenderSerialId)
{
	SNtlEventSobAttackFollow sSobAttackFollow;

	sSobAttackFollow.hSerialId			= hSerialId;
	sSobAttackFollow.hDefenderSerialId	= hDefenderSerialId;

	RWS::CMsg msg;
	msg.Id = g_EventSobAttackFollow;
	msg.pData = reinterpret_cast<void*>(&sSobAttackFollow);
	_SendMsg(msg);
}


void CNtlSLEventGenerator::SobAutoAttackMode(SERIAL_HANDLE hSerialId, RwBool bAttackMode)
{
	SNtlEventSobAttackMode sSobAttackMode;

	sSobAttackMode.hSerialId = hSerialId;
	sSobAttackMode.bAttackMode = bAttackMode;

	RWS::CMsg msg;
	msg.Id = g_EventSobAttackMode;
	msg.pData = reinterpret_cast<void*>(&sSobAttackMode);
	_SendMsg(msg);
}


void CNtlSLEventGenerator::SobJump(SERIAL_HANDLE hSerialId, RwUInt8 byMoveDirection, RwV3d vDir, RwV3d vJumpDir)
{
	SNtlEventSobJump sSobJump;

	sSobJump.hSerialId = hSerialId;
	sSobJump.byMoveDirection = byMoveDirection;
	sSobJump.vDir = vDir;
	sSobJump.vJumpDir = vJumpDir;

	RWS::CMsg msg;
	msg.Id = g_EventSobJump;
	msg.pData = reinterpret_cast<void*>(&sSobJump);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobJumpDirection(SERIAL_HANDLE hSerialId, RwV3d vJumpDir, RwUInt32 uiTimeStamp)
{
	SNtlEventSobJumpDirection sSobJumpDirection;

	sSobJumpDirection.hSerialId = hSerialId;
	sSobJumpDirection.vJumpDir = vJumpDir;
	sSobJumpDirection.uiTimeStamp = uiTimeStamp;

	RWS::CMsg msg;
	msg.Id = g_EventSobJumpDirection;
	msg.pData = reinterpret_cast<void*>(&sSobJumpDirection);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobJumpEnd(SERIAL_HANDLE hSerialId)
{
	SNtlEventSobJump sSobJump;

	sSobJump.hSerialId = hSerialId;

	RWS::CMsg msg;
	msg.Id = g_EventSobJumpEnd;
	msg.pData = reinterpret_cast<void*>(&sSobJump);
	_SendMsg(msg);
}


void CNtlSLEventGenerator::SobSit(SERIAL_HANDLE hSerialId, RwBool bSitDown)
{
	SNtlEventSobSit sSobSit;

	sSobSit.hSerialId = hSerialId;
	sSobSit.bSitDown = bSitDown;

	RWS::CMsg msg;
	msg.Id = g_EventSobSit;
	msg.pData = reinterpret_cast<void*>(&sSobSit);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobPrivateShop(SERIAL_HANDLE hSerialId, WCHAR *pName, RwBool bOwnerEmpty, RwUInt8 byShopState)
{
	SNtlEventSobPrivateShop sSobPrivateShop;

	sSobPrivateShop.hSerialId			= hSerialId;
	wcscpy_s(sSobPrivateShop.wcShopName, NTL_MAX_PRIVATESHOP_NAME, pName);
	sSobPrivateShop.bIsOwnerEmpty		= bOwnerEmpty;
	sSobPrivateShop.byShopState			= byShopState;

	RWS::CMsg msg;
	msg.Id = g_EventSobPrivateShop;
	msg.pData = reinterpret_cast<void*>(&sSobPrivateShop);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobConvertClass(SERIAL_HANDLE hSerialId, RwUInt8 byClass)
{
	SNtlEventSobConvertClass sSobConvertClass;

	sSobConvertClass.hSerialId	= hSerialId;
	sSobConvertClass.byClass	= byClass;

	RWS::CMsg msg;
	msg.Id = g_EventSobConvertClass;
	msg.pData = reinterpret_cast<void*>(&sSobConvertClass);
	_SendMsg(msg);
}


void CNtlSLEventGenerator::SobDirectPlay(SERIAL_HANDLE hSerialId, RwUInt8 byDirectType, RwUInt32 uiTblId, RwBool bTimeLoop, RwReal fLoopTime)
{
	SNtlEventSobDirectPlay sSobDirectPlay;

	sSobDirectPlay.hSerialId	= hSerialId;
	sSobDirectPlay.byDirectType	= byDirectType;
	sSobDirectPlay.uiTblId		= uiTblId;
	sSobDirectPlay.bTimeLoop	= bTimeLoop;
	sSobDirectPlay.fLoopTime	= fLoopTime;

	RWS::CMsg msg;
	msg.Id = g_EventSobDirectPlay;
	msg.pData = reinterpret_cast<void*>(&sSobDirectPlay);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobAnimPlay(SERIAL_HANDLE hSerialId, RwUInt32 uiAnimId, RwBool bTimeLoop, RwReal fLoopTime)
{
	SNtlEventSobAnimPlay sSobAnimPlay;

	sSobAnimPlay.hSerialId		= hSerialId;
	sSobAnimPlay.uiAnimId		= uiAnimId;
	sSobAnimPlay.bTimeLoop		= bTimeLoop;
	sSobAnimPlay.fLoopTime		= fLoopTime;

	RWS::CMsg msg;
	msg.Id = g_EventSobAnimPlay;
	msg.pData = reinterpret_cast<void*>(&sSobAnimPlay);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobProxyGUIUpdate(SERIAL_HANDLE hSerialId, RwUInt32 uiUpdateFlags, RwBool bShow)
{
	SNtlEventSobProxyGUIUpdate sSobProxyGUIUpdate;

	sSobProxyGUIUpdate.hSerialId		= hSerialId;
	sSobProxyGUIUpdate.uiUpdateFlags	= uiUpdateFlags;
	sSobProxyGUIUpdate.bShow			= bShow;
	
	RWS::CMsg msg;
	msg.Id = g_EventSobProxyGUIUpdate;
	msg.pData = reinterpret_cast<void*>(&sSobProxyGUIUpdate);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobCooling(SERIAL_HANDLE hSerialId, RwBool bStart)
{
	SNtlEventSobCooling sSobCooling;

	sSobCooling.hSerialId = hSerialId;
	sSobCooling.bStartOrEnd = bStart;

	RWS::CMsg msg;
	msg.Id = g_EventSobCooling;
	msg.pData = reinterpret_cast<void*>(&sSobCooling);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobSkillCancel(SERIAL_HANDLE hSerialId, RwUInt16 wReason)
{
	SNtlEventSobSkillCancel sSobSkillCancel;

	sSobSkillCancel.hSerialId	= hSerialId;
	sSobSkillCancel.wReason		= wReason;

	RWS::CMsg msg;
	msg.Id = g_EventSobSkillCancel;
	msg.pData = reinterpret_cast<void*>(&sSobSkillCancel);
	_SendMsg(msg);
}


void CNtlSLEventGenerator::SobSkillAffectingCancel(SERIAL_HANDLE hSerialId)
{
	SNtlEventSobSkillAffectingCancel sSobSkillAffectingCancel;

	sSobSkillAffectingCancel.hSerialId = hSerialId;

	RWS::CMsg msg;
	msg.Id = g_EventSobSkillAffectingCancel;
	msg.pData = reinterpret_cast<void*>(&sSobSkillAffectingCancel);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobSkillCasting(SERIAL_HANDLE hSerialId, SERIAL_HANDLE hTargetSerialI, RwUInt32 uiSkillTblId, RwReal fCurrCastingTime, RwReal fMaxCastingTime)
{
	SNtlEventSobSkillCasting sSobSkillCasting;

	sSobSkillCasting.hSerialId			= hSerialId;
	sSobSkillCasting.hTargetSerialId	= hTargetSerialI;
	sSobSkillCasting.uiSkillTblId		= uiSkillTblId;
	sSobSkillCasting.fCurrCastingTime	= fCurrCastingTime;
	sSobSkillCasting.fMaxCastingTime	= fMaxCastingTime;	

	RWS::CMsg msg;
	msg.Id = g_EventSobSkillCasting;
	msg.pData = reinterpret_cast<void*>(&sSobSkillCasting);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobSkillAction(SERIAL_HANDLE hSerialId, RwUInt16 wResultCode, RwUInt32 uiTargetSerialI, RwUInt32 uiSkillTblId, 
										  RwUInt8 byRpBonusType, RwUInt8 byResultCount,  sSKILL_RESULT *pSkillAct, RwUInt32 uiSyncEventId, RwV3d& vFinalLoc)
{
	SNtlEventSobSkillAction sSobSkillAction;

	sSobSkillAction.hSerialId		= hSerialId;
	sSobSkillAction.wResultCode		= wResultCode;
	sSobSkillAction.hTargetSerialId = uiTargetSerialI;
	sSobSkillAction.uiSkillTblId	= uiSkillTblId;
	sSobSkillAction.byRpBonusType	= byRpBonusType;
	sSobSkillAction.byResultCount	= byResultCount;
	sSobSkillAction.pSkillAct		= pSkillAct;
	sSobSkillAction.uiSyncEventId	= uiSyncEventId;
	sSobSkillAction.vFinalLoc = vFinalLoc;

	RWS::CMsg msg;
	msg.Id = g_EventSobSkillAction;
	msg.pData = reinterpret_cast<void*>(&sSobSkillAction);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobSkillActioned(SERIAL_HANDLE hSerialId, RwUInt32 uiAttackerSerialI)
{
	SNtlEventSobSkillActioned sSobSkillActioned;

	sSobSkillActioned.hSerialId		= hSerialId;
	sSobSkillActioned.hAttackerSerialId = uiAttackerSerialI;

	RWS::CMsg msg;
	msg.Id = g_EventSobSkillActioned;
	msg.pData = reinterpret_cast<void*>(&sSobSkillActioned);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobSkillAdd(SERIAL_HANDLE hSerialId, RwUInt8 bySkillSlotIdx, RwUInt32 uiSkillTblId)
{
	SNtlEventSobSkillAdd sSobSkillAdd;

	sSobSkillAdd.hSerialId = hSerialId;
	sSobSkillAdd.bySkillSlotIdx = bySkillSlotIdx;
	sSobSkillAdd.uiSkillTblId = uiSkillTblId;

	RWS::CMsg msg;
	msg.Id = g_EventSobSkillAdd;
	msg.pData = reinterpret_cast<void*>(&sSobSkillAdd);
	_SendMsg(msg);
}


void CNtlSLEventGenerator::SobSkillUpgrade(SERIAL_HANDLE hSerialId, RwUInt8 bySkillSlotIdx, RwUInt32 uiUpgradeSkillTblId)
{
	SNtlEventSobSkillUpgrade sSobSkillUpgrade;

	sSobSkillUpgrade.hSerialId = hSerialId;
	sSobSkillUpgrade.bySkillSlotIdx = bySkillSlotIdx;
	sSobSkillUpgrade.uiUpgradeSkillTblId = uiUpgradeSkillTblId;

	RWS::CMsg msg;
	msg.Id = g_EventSobSkillUpgrade;
	msg.pData = reinterpret_cast<void*>(&sSobSkillUpgrade);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobHTBSkillAdd(SERIAL_HANDLE hSerialId, RwUInt8 byHTBSkillSlotIdx, RwUInt32 uiHTBSkillTblId)
{
	SNtlEventSobHTBSkillAdd sSobHTBSkillAdd;

	sSobHTBSkillAdd.hSerialId = hSerialId;
	sSobHTBSkillAdd.byHTBSkillSlotIdx = byHTBSkillSlotIdx;
	sSobHTBSkillAdd.uiHTBSkillTblId = uiHTBSkillTblId;

	RWS::CMsg msg;
	msg.Id = g_EventSobHTBSkillAdd;
	msg.pData = reinterpret_cast<void*>(&sSobHTBSkillAdd);
	_SendMsg(msg);
}


void CNtlSLEventGenerator::SobHTBSkillAction(SERIAL_HANDLE hSerialId, sCHARSTATE *pCharState)
{
	SNtlEventSobHTBSkillAction sSobHTBSkillAction;

	sSobHTBSkillAction.hSerialId = hSerialId;
	sSobHTBSkillAction.pCharState = pCharState;

	RWS::CMsg msg;
	msg.Id = g_EventSobHTBSkillAction;
	msg.pData = reinterpret_cast<void*>(&sSobHTBSkillAction);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobSocialAction(SERIAL_HANDLE hSerialId, RwUInt32 tblIdx)
{
	SNtlEventSobSocialAction sSobSocialAction;

	sSobSocialAction.hSerialId = hSerialId;
	sSobSocialAction.uiTblIdx = tblIdx;

	RWS::CMsg msg;
	msg.Id = g_EventSobSocialAction;
	msg.pData = reinterpret_cast<void*>(&sSobSocialAction);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobSocialActionCooling(RwUInt32 tblIdx)
{
	SNtlEventSobSocialActionCooling sSobSocialActionCooling;

	sSobSocialActionCooling.uiTblIdx = tblIdx;

	RWS::CMsg msg;
	msg.Id = g_EventSobSocialActionCooling;
	msg.pData = reinterpret_cast<void*>(&sSobSocialActionCooling);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobSkillInit(void)
{
	RWS::CMsg msg;
	msg.Id = g_EventSkillInit;
	_SendMsg( msg );
}

void CNtlSLEventGenerator::SobSkillSetPassiveEffect( RwUInt32 uiSkillTblidx, RwUInt8 bySkillSlotIdx, sDBO_UPDATED_SKILL_DATA* pPassiveEffectData )
{
	SNtlEventSetSkillPassiveEffect sSetSkillPassiveEffect;
	sSetSkillPassiveEffect.uiSkillTblidx = uiSkillTblidx;
	sSetSkillPassiveEffect.bySkillSlotIdx = bySkillSlotIdx;
	sSetSkillPassiveEffect.pPassiveEffectData = pPassiveEffectData;

	RWS::CMsg msg;
	msg.Id = g_EventSetSkillPassiveEffect;
	msg.pData = reinterpret_cast<void*>( &sSetSkillPassiveEffect );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::SobSkillInfoText(WCHAR * wchMsg, unsigned int uiColor, BYTE byMsgType)
{
	SNtlEventSobSkillInfoText sInfo;
	sInfo.wchMsg = wchMsg;
	sInfo.uiColor = uiColor;
	sInfo.byMsgType = byMsgType;

	SEND_MSG(g_EventSkillInfoText, &sInfo);
}

void CNtlSLEventGenerator::SobBuffAdd(SERIAL_HANDLE hSerialId, BYTE byBuffIndex, BYTE byBuffType, RwUInt32 uiTblId, RwUInt32 uiRemainTime, RwUInt32 uiKeepTime, sDBO_BUFF_PARAMETER* aBuffParameter)
{
	SNtlEventSobBuffAdd sSobBuffAdd;

	sSobBuffAdd.byBuffIndex = byBuffIndex;
	sSobBuffAdd.hSerialId = hSerialId;
	sSobBuffAdd.byBuffType = byBuffType;
	sSobBuffAdd.uiTblId = uiTblId;
	sSobBuffAdd.uiRemainTime = uiRemainTime;
	sSobBuffAdd.uiKeepTime = uiKeepTime;
	sSobBuffAdd.bActive = true;
	for (RwInt32 i = 0; i < NTL_MAX_EFFECT_IN_SKILL; ++i)
		memcpy(&sSobBuffAdd.aBuffParameter[i], &aBuffParameter[i], sizeof(sDBO_BUFF_PARAMETER));

	RWS::CMsg msg;
	msg.Id = g_EventSobBuffAdd;
	msg.pData = reinterpret_cast<void*>(&sSobBuffAdd);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobBuffDrop(SERIAL_HANDLE hSerialId, BYTE byBuffIndex, BYTE byBuffType)
{
	SNtlEventSobBuffDrop sSobBuffDrop;

	sSobBuffDrop.byBuffIndex = byBuffIndex;
	sSobBuffDrop.hSerialId = hSerialId;
	sSobBuffDrop.byBuffType = byBuffType;

	RWS::CMsg msg;
	msg.Id = g_EventSobBuffDrop;
	msg.pData = reinterpret_cast<void*>(&sSobBuffDrop);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobBuffRefreshAll(SERIAL_HANDLE hSerial, RwUInt8 byCount, sBUFF_INFO* pBuffInfo)
{
	SNtlEventSobBuffRefreshAll	sSobBuffRefreshAll;

	sSobBuffRefreshAll.hSerialId = hSerial;
	sSobBuffRefreshAll.byCount = byCount;
	sSobBuffRefreshAll.pBuffInfo = pBuffInfo;

	SEND_MSG(g_EventSobBuffRefreshAll, &sSobBuffRefreshAll);
}

void CNtlSLEventGenerator::SobBuffActivate(SERIAL_HANDLE hSerial, sBUFF_INFO * pBuffInfo)
{
	SNtlEventSobBuffActivate pData;

	pData.hSerialId = hSerial;
	pData.pBuffInfo = pBuffInfo;

	SEND_MSG(g_EventSobBuffActivate, &pData);
}

void CNtlSLEventGenerator::SobBuffAutoRecover(SERIAL_HANDLE hSerial, BYTE byBuffIndex, DWORD dwRemainTime, DWORD dwRemainValue)
{
	SNtlEventSobBuffAutoRecover	sData;

	sData.hSerialId = hSerial;
	sData.byBuffIndex = byBuffIndex;
	sData.dwRemainTime = dwRemainTime;
	sData.dwRemainValue = dwRemainValue;

	SEND_MSG(g_EventSobBuffAutoRecover, &sData);
}

void CNtlSLEventGenerator::SobFakeBuffAdd(SERIAL_HANDLE hSerialId, BYTE byBuffIndex, BYTE byBuffType, RwUInt32 uiTblId, RwUInt32 uiRemainTime, RwUInt32 uiKeepTime, sDBO_BUFF_PARAMETER* aBuffParameter)
{
	SNtlEventFakeBuffAdd sSobFakeBuffAdd;

	sSobFakeBuffAdd.byBuffIndex = byBuffIndex;
	sSobFakeBuffAdd.hSerialId = hSerialId;
	sSobFakeBuffAdd.byBuffType = byBuffType;
	sSobFakeBuffAdd.uiTblId = uiTblId;
	sSobFakeBuffAdd.uiRemainTime = uiRemainTime;
	sSobFakeBuffAdd.uiKeepTime = uiKeepTime;
	sSobFakeBuffAdd.bActive = true;
	for( RwInt32 i = 0 ; i < NTL_MAX_EFFECT_IN_SKILL ; ++i )
		memcpy(&sSobFakeBuffAdd.aBuffParameter[i], &aBuffParameter[i], sizeof(sDBO_BUFF_PARAMETER));

	RWS::CMsg msg;
	msg.Id = g_EventSobFakeBuffAdd;
	msg.pData = reinterpret_cast<void*>(&sSobFakeBuffAdd);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobFakeBuffDrop(SERIAL_HANDLE hSerialId, BYTE byBuffIndex, BYTE byBuffType)
{
	SNtlEventFakeBuffDrop sSobFakeBuffDrop;

	sSobFakeBuffDrop.byBuffIndex = byBuffIndex;
	sSobFakeBuffDrop.hSerialId = hSerialId;
	sSobFakeBuffDrop.byBuffType = byBuffType;

	RWS::CMsg msg;
	msg.Id = g_EventSobFakeBuffDrop;
	msg.pData = reinterpret_cast<void*>(&sSobFakeBuffDrop);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobFakeBuffRefreshAll(SERIAL_HANDLE hSerialId, RwUInt8 byCount, sBUFF_INFO* pBuffInfo)
{
	SNtlEventFakeBuffRefreshAll sSobFakeBuffRefeshAll;

	sSobFakeBuffRefeshAll.hSerialId = hSerialId;
	sSobFakeBuffRefeshAll.byCount = byCount;
	sSobFakeBuffRefeshAll.pBuffInfo = pBuffInfo;

	RWS::CMsg msg;
	msg.Id = g_EventSobFakeBuffRefreshAll;
	msg.pData = reinterpret_cast<void*>(&sSobFakeBuffRefeshAll);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobFakeBuffActivate(SERIAL_HANDLE hSerial, sBUFF_INFO* pBuffInfo)
{
	SNtlEventSobBuffActivate pData;

	pData.hSerialId = hSerial;
	pData.pBuffInfo = pBuffInfo;

	SEND_MSG(g_EventSobFakeBuffActivate, &pData);
}

void CNtlSLEventGenerator::SobStateTransition(SERIAL_HANDLE hSerialId, RwUInt32 uiTransState)
{
	SNtlEventSobStateTransition sSobStateTrans;
	sSobStateTrans.hSerialId = hSerialId;
	sSobStateTrans.uiTransState = uiTransState;

	SEND_MSG(g_EventSobStateTransition, &sSobStateTrans);
}

void CNtlSLEventGenerator::SobServerUpdateState(SERIAL_HANDLE hSerialId, sCHARSTATE *pState)
{
	SNtlEventSobServerUpdateState sSobServerState;
	sSobServerState.hSerialId = hSerialId;
	sSobServerState.pState = pState;

	SEND_MSG(g_EventSobServerUpdateState, &sSobServerState);
}

void CNtlSLEventGenerator::SobServerUpdateStatePostMsg(SERIAL_HANDLE hSerialId, sCHARSTATE *pState)
{
	SNtlEventSobServerUpdateState sSobServerState;
	sSobServerState.hSerialId = hSerialId;
	sSobServerState.pState = pState;

	RWS::CMsg msg;
	msg.Id = g_EventSobServerUpdateState;
	msg.pData = reinterpret_cast<void*>( &sSobServerState );

	PostMsg( msg, reinterpret_cast<void*>( &sSobServerState ), sizeof( SNtlEventSobServerUpdateState ) );
}

void CNtlSLEventGenerator::SobServerUpdateCondition(SERIAL_HANDLE hSerialId, QWORD qwCondition)
{
	SNtlEventSobServerUpdateCondition sSobServerCondition;
	sSobServerCondition.hSerialId		= hSerialId;
	sSobServerCondition.dwCondition		= qwCondition;

	SEND_MSG(g_EventSobServerUpdateCondition, &sSobServerCondition);
}

void CNtlSLEventGenerator::SobServerUpdateAspect(SERIAL_HANDLE hSerialId, sASPECTSTATE& sAspectState)
{
	SNtlEventSobServerUpdateAspect sSobServerAspect;
	sSobServerAspect.hSerialId      = hSerialId;
	sSobServerAspect.sApectState    = sAspectState;

	SEND_MSG(g_EventSobServerUpdateAspect, &sSobServerAspect);
}

void CNtlSLEventGenerator::SobFainting(SERIAL_HANDLE hSerialId)
{
	SNtlEventSobFainting sSobFainting;
	sSobFainting.hSerialId = hSerialId;

	RWS::CMsg msg;
	msg.Id = g_EventSobFainting;
	msg.pData = reinterpret_cast<void*>(&sSobFainting);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobUpdate(SERIAL_HANDLE hSerialId, RwUInt32 uiUpdateType, RwUInt32 uiParam1)
{
	SNtlEventSobInfoUpdate sSobInfoUpdate;
	sSobInfoUpdate.hSerialId = hSerialId;
	sSobInfoUpdate.uiUpdateType = uiUpdateType;
	sSobInfoUpdate.uiParam1 = uiParam1;

	RWS::CMsg msg;
	msg.Id = g_EventSobInfoUpdate;
	msg.pData = reinterpret_cast<void*>(&sSobInfoUpdate);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobNpcCommunity(SERIAL_HANDLE hSerialId, RwBool bShow, RwUInt32 uiTriggerId, RwUInt8 byInputType)
{
	SNtlEventSobNpcCommunity sSobNpcCommunity;
	sSobNpcCommunity.hSerialId = hSerialId;
	sSobNpcCommunity.bShow = bShow;
	sSobNpcCommunity.uiTriggerId = uiTriggerId;
	sSobNpcCommunity.byInputType = byInputType;

	RWS::CMsg msg;
	msg.Id = g_EventSobNpcCommunity;
	msg.pData = reinterpret_cast<void*>(&sSobNpcCommunity);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobTriggerObjCommunity(SERIAL_HANDLE hSerialId, RwBool bShow, RwUInt32 uiTriggerId, RwUInt8 byInputType)
{
	SNtlEventSobTriggerObjCommunity sSobTriggerObjCommunity;
	sSobTriggerObjCommunity.hSerialId	= hSerialId;
	sSobTriggerObjCommunity.bShow		= bShow;
	sSobTriggerObjCommunity.uiTriggerId = uiTriggerId;
	sSobTriggerObjCommunity.byInputType = byInputType;

	RWS::CMsg msg;
	msg.Id = g_EventSobTriggerObjCommunity;
	msg.pData = reinterpret_cast<void*>(&sSobTriggerObjCommunity);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobPrivateShopCommunity(SERIAL_HANDLE hSerialId, RwBool bOwnerEmpty, RwUInt8 byShopState)
{
	SNtlEventSobPrivateShopCommunity sSobPrivateShopCommunity;
	sSobPrivateShopCommunity.hSerialId		= hSerialId;
	sSobPrivateShopCommunity.bOwnerEmpty	= bOwnerEmpty;
	sSobPrivateShopCommunity.byShopState		= byShopState;

	RWS::CMsg msg;
	msg.Id = g_EventSobPrivateShopCommunity;
	msg.pData = reinterpret_cast<void*>(&sSobPrivateShopCommunity);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobDynamicObjCommunity( SERIAL_HANDLE hSerialId, RwBool bShow )
{
	SNtlEventSobDynamicObjCommunity sSobDynamicObjCommunity;
	sSobDynamicObjCommunity.hSerialId = hSerialId;
	sSobDynamicObjCommunity.bShow = bShow;
	
	SEND_MSG( g_EventSobDynamicObjCommunity, &sSobDynamicObjCommunity );
}

void CNtlSLEventGenerator::SobTargetUpdate(SERIAL_HANDLE hSerialId, RwBool bShow)
{
	SNtlEventSobTargetInfoUpdate sSobTargetInfoUpdate;
	sSobTargetInfoUpdate.hSerialId = hSerialId;
	sSobTargetInfoUpdate.bShow = bShow;

	SEND_MSG(g_EventSobTargetInfoUpdate, &sSobTargetInfoUpdate);
}

void CNtlSLEventGenerator::SobTargetChanged(HOBJECT hObject, HOBJECT hTarget)
{
	SNtlEventSobTargetChanged sInfo;
	sInfo.hSerialId = hObject;
	sInfo.hTarget = hTarget;

	SEND_MSG(g_EventSobTargetChanged, &sInfo);
}

void CNtlSLEventGenerator::SobPickWorldItem(SERIAL_HANDLE hSerialId)
{
	SNtlEventSobPickWorldItem sSobPickWorldItem;
	sSobPickWorldItem.hSerialId = hSerialId;

	RWS::CMsg msg;
	msg.Id = g_EventSobPickWorldItem;
	msg.pData = reinterpret_cast<void*>(&sSobPickWorldItem);
	_SendMsg(msg);
}


void CNtlSLEventGenerator::SobCastingDirect( CNtlSobActor* pActor, RwBool bSkillOrItemCasting, RwBool bStartOrEnd )
{
	SNtlEventSobCastingDirect sSobCastingDirect;
	sSobCastingDirect.pActor				= pActor;
	sSobCastingDirect.bSkillOrItemCasting	= bSkillOrItemCasting;
	sSobCastingDirect.bStartOrEnd			= bStartOrEnd;

	RWS::CMsg msg;
	msg.Id = g_EventSobCastingDirect;
	msg.pData = reinterpret_cast<void*>(&sSobCastingDirect);
	_SendMsg(msg);
}


void CNtlSLEventGenerator::SobRevivalNotify(void)
{
	RWS::CMsg msg;
	msg.Id = g_EventSobRevivalNotify;
	msg.pData = NULL;
	_SendMsg(msg);
}


void CNtlSLEventGenerator::PartyUpdate(RwInt32 iMessage, SERIAL_HANDLE hSerial /* = INVALID_SERIAL_ID */,
									   RwUInt32 uiValue /* =0 */, RwInt32 iValue2 /* =0 */)
{
	SNtlEventPartyUpdate sPartyUpdate;
	sPartyUpdate.iMessage = iMessage;
	sPartyUpdate.hSerialId = hSerial;
	sPartyUpdate.uiValue = uiValue;
	sPartyUpdate.iValue2 = iValue2;

	RWS::CMsg msg;
	msg.Id = g_EventPartyUpdate;
	msg.pData = reinterpret_cast<void*>(&sPartyUpdate);

	_SendMsg(msg);
}

void CNtlSLEventGenerator::PartyCreate(WCHAR* pcName)
{
	SNtlEventPartyCreate sPartyCreate;	
	sPartyCreate.pcName = pcName;

	RWS::CMsg msg;
	msg.Id = g_EventPartyCreate;
	msg.pData = reinterpret_cast<void*>(&sPartyCreate);

	_SendMsg(msg);
}

void CNtlSLEventGenerator::PartyDisband()
{
	RWS::CMsg msg;
	msg.Id = g_EventPartyLeave;

	_SendMsg(msg);
}

void CNtlSLEventGenerator::PartyMemberAdd(SERIAL_HANDLE hSerial, WCHAR* pwcName, RwUInt8 byRace /* = 0 */, RwUInt8 byClass /* = 0 */,
										  RwUInt8 byLevel /* = 0 */, RwInt32 curLP /* = 0 */, RwInt32 maxLP /* = 0 */,
										  RwUInt32 wCurEP /* = 0 */, RwUInt32 wMaxEP /* = 0 */, 
										  RwUInt32 uiWorldTableIndex /* = 0xffffffff */, RwUInt32 uiWorldID /* = 0xffffffff */,
										  RwReal fX /* = 0.f */, RwReal fY /* = 0.f */, RwReal fZ /* = 0.f */)
{	
	SNtlEventPartyAdd sPartyAdd;
	sPartyAdd.hSerial = hSerial;
	sPartyAdd.pcName = pwcName;
	sPartyAdd.byRace = byRace;
	sPartyAdd.byClass = byClass;
	sPartyAdd.byLevel = byLevel;
	sPartyAdd.curLP = curLP;
	sPartyAdd.maxLP = maxLP;
	sPartyAdd.wCurEP = (RwUInt16)wCurEP;
	sPartyAdd.wMaxEP = (RwUInt16)wMaxEP;
	sPartyAdd.uiWorldTableIndex = uiWorldTableIndex;
	sPartyAdd.uiWorldID = uiWorldID;
	sPartyAdd.v3Pos.x = fX;
	sPartyAdd.v3Pos.y = fY;
	sPartyAdd.v3Pos.z = fZ;

	RWS::CMsg msg;
	msg.Id = g_EventPartyMemberAdd;
	msg.pData = reinterpret_cast<void*>(&sPartyAdd);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::PartyMemberDel(SERIAL_HANDLE hSerialId)
{
	RWS::CMsg msg;
	msg.Id = g_EventPartyMemberDel;
	msg.pData = reinterpret_cast<void*>(&hSerialId);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::PartyLeaderChange(SERIAL_HANDLE hSerialId)
{
	RWS::CMsg msg;
	msg.Id = g_EventPartyLeaderChange;
	msg.pData = reinterpret_cast<void*>(&hSerialId);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::PartyUpdateValue(RwInt32 iValueType, SERIAL_HANDLE hSerial /* = INVALID_SERIAL_ID */, RwUInt32 uiValue /* = 0 */, RwInt32 iValue2 /* = 0 */)
{
	SNtlEventPartyUpdateValue sPacket;

	sPacket.iValueType = iValueType;
	sPacket.hSerial = hSerial;
	sPacket.uiValue = uiValue;
	sPacket.iValue2 = iValue2;

	RWS::CMsg msg;
	msg.Id = g_EventPartyUpdateValue;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::PartyMemberLocation(RwUInt32 uiSerial, RwUInt32 uiWorldID, RwUInt32 uiWorldTbl, RwV3d& vCurLoc)
{
	SNtlEventPartyMemberLocation sPacket;

	sPacket.uiSerial = uiSerial;
	sPacket.uiWorldID = uiWorldID;
	sPacket.uiWorldTbl = uiWorldTbl;
	sPacket.vCurLoc = vCurLoc;

	RWS::CMsg msg;
	msg.Id = g_EventPartyMemberLocation;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SLGuildEvent(RwInt32 iMessage, RwUInt32 uiParam /* = 0xffffffff */,
										void* pData /* = NULL */, void* pExData /* = NULL*/)
{
	SNtlEventGuild sPacket;

	sPacket.iMessage = iMessage;
	sPacket.uiParam = uiParam;
	sPacket.pData	= pData;
	sPacket.pExData	= pExData;

	RWS::CMsg msg;
	msg.Id = g_EventSLGuild;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::NotifyGuildEvent(RwInt32 iMessage, RwUInt32 uiParam /* = 0xffffffff */)
{
	SNtlEventNotifyGuild sPacket;

	sPacket.iMessage = iMessage;
	sPacket.uiParam = uiParam;

	RWS::CMsg msg;
	msg.Id = g_EventNotifyGuild;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ChangeGuildName(SERIAL_HANDLE hHandle, WCHAR* pwcGuildName)
{
	SNtlEventChangleGuildName sPacket;

	sPacket.hSerialId		= hHandle;
	sPacket.pwcGuildName	= pwcGuildName;

	RWS::CMsg msg;
	msg.Id = g_EventChangeGuildName;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ChangeGuildEmblem(SERIAL_HANDLE hHandle, RwUInt8 byTypeA, RwUInt8 byTypeAColor,
											 RwUInt8 byTypeB, RwUInt8 byTypeBColor,
											 RwUInt8 byTypeC, RwUInt8 byTypeCColor)
{
	SNtlEventChangleGuildEmblem sPacket;

	sPacket.hHandle				= hHandle;;
	sPacket.byTypeA				= byTypeA;
	sPacket.byTypeAColor		= byTypeAColor;
	sPacket.byTypeB				= byTypeB;
	sPacket.byTypeBColor		= byTypeBColor;
	sPacket.byTypeC				= byTypeC;
	sPacket.byTypeCColor		= byTypeCColor;

	RWS::CMsg msg;
	msg.Id = g_EventChangeGuildEmblem;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::GuildWarehouseEvent(RwUInt8 byMessage, RwUInt32 uiParam /* = 0xffffffff */,
											   RwUInt32 uiParam2 /* = 0xffffffff */)
{
	SNtlGuildWarehouse sPacket;
	sPacket.byMessage		= byMessage;
	sPacket.uiParam			= uiParam;
	sPacket.uiParam2		= uiParam2;

	RWS::CMsg msg;
	msg.Id = g_EventGuildWarehouse;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::GuildWarehousePostEvent(RwUInt8 byMessage, RwUInt32 uiParam /* = 0xffffffff */,
												   RwUInt32 uiParam2 /* = 0xffffffff */)
{
	SNtlGuildWarehouse sPacket;
	sPacket.byMessage		= byMessage;
	sPacket.uiParam			= uiParam;
	sPacket.uiParam2		= uiParam2;

	RWS::CMsg msg;
	msg.Id = g_EventGuildWarehouse;
	msg.pData = reinterpret_cast<void*>(&sPacket);

	PostMsg(msg, reinterpret_cast<void*>(&sPacket), sizeof(SNtlGuildWarehouse));
}

void CNtlSLEventGenerator::GuildWarehouseNotify(RwUInt8 byMessage, RwUInt32 uiParam /* = 0xffffffff */,
												RwUInt32 uiParam2 /* = 0xffffffff */)
{
	SNtlGuildWarehouseNotify sPacket;
	sPacket.byMessage		= byMessage;
	sPacket.uiParam			= uiParam;
	sPacket.uiParam2		= uiParam2;

	RWS::CMsg msg;
	msg.Id = g_EventGuildWarehouseNotify;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::GuildWarehouseItemInfo(RwUInt8 byPlace, RwUInt8 byItemCount, VOID* pData)
{
	SNtlGuildWarehouseItemInfo sPacket;

	sPacket.byPlace			= byPlace;
	sPacket.byItemCount		= byItemCount;
	sPacket.pData			= pData;

	RWS::CMsg msg;
	msg.Id = g_EventGuildWarehouseItemInfo;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::GuildWarehouseItemMove(SERIAL_HANDLE hSrcItem, SERIAL_HANDLE hDestItem,
												  RwUInt8 bySrcPlace, RwUInt8 bySrcPos,
												  RwUInt8 byDestPlace, RwUInt8 byDestPos)
{
	SNtlGuildWarehouseItemMove sPacket;

	sPacket.hSrcItem			= hSrcItem;
	sPacket.hDestItem			= hDestItem;
	sPacket.bySrcPlace			= bySrcPlace;
	sPacket.bySrcPos			= bySrcPos;
	sPacket.byDestPlace			= byDestPlace;
	sPacket.byDestPos			= byDestPos;

	RWS::CMsg msg;
	msg.Id = g_EventGuildWarehouseItemMove;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::GuildWarehouseItemStackMove(SERIAL_HANDLE hSrcItem, SERIAL_HANDLE hDestItem,
													   RwUInt8 bySrcPlace, RwUInt8 bySrcPos,
													   RwUInt8 byDestPlace, RwUInt8 byDestPos,
													   RwUInt8 byStackCount, RwUInt8 byStackCount2)
{
	SNtlGuildWarehouseItemStackMove sPacket;

	sPacket.hSrcItem			= hSrcItem;
	sPacket.hDestItem			= hDestItem;
	sPacket.bySrcPlace			= bySrcPlace;
	sPacket.bySrcPos			= bySrcPos;
	sPacket.byDestPlace			= byDestPlace;
	sPacket.byDestPos			= byDestPos;
	sPacket.bySrcStackCount		= byStackCount;
	sPacket.byDestStackCount	= byStackCount2;

	RWS::CMsg msg;
	msg.Id = g_EventGuildWarehouseItemStackMove;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::DojoEvent(RwUInt8 byDojoEvent, RwUInt32 uiParam /* = 0xffffffff */,
									 VOID* pExData /* = NULL */, VOID* pExData2 /* = NULL */,
									 VOID* pExData3 /* = NULL */)
{
	SNtlEventDojo sPacket;

	sPacket.byDojoEvent			= byDojoEvent;
	sPacket.uiParam				= uiParam;
	sPacket.pExData				= pExData;
	sPacket.pExData2			= pExData2;
	sPacket.pExData3			= pExData3;

	RWS::CMsg msg;
	msg.Id = g_EventDojo;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::DojoEventNotify(RwUInt8 byDojoEvent, RwUInt32 uiParam /* = 0xffffffff */,
										   VOID* pExData /* = NULL */, VOID* pExData2 /* = NULL */,
										   VOID* pExData3 /* = NULL */)
{
	SNtlEventDojo sPacket;

	sPacket.byDojoEvent			= byDojoEvent;
	sPacket.uiParam				= uiParam;
	sPacket.pExData				= pExData;
	sPacket.pExData2			= pExData2;
	sPacket.pExData3			= pExData3;

	RWS::CMsg msg;
	msg.Id = g_EventDojoNotify;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ScrambleDefaultInfo(RwUInt32 uiDojoTableIndex,
											   RwUInt32 uiDefenceGuildID, RwUInt32 uiOffenceGuildID,
											   WCHAR* pwcDefenceGuildName, WCHAR* pwcOffenceGuildName,
											   VOID* pDefenceGuildEmblem, VOID* pOffenceGuildEmblem)
{
	SNtlEventScrambleDefaultInfo sPacket;

	sPacket.uiDojoTableIndex		= uiDojoTableIndex;
	sPacket.uiDefenceGuildID		= uiDefenceGuildID;
	sPacket.uiOffenceGuildID		= uiOffenceGuildID;
	sPacket.pwcDefenceGuildName		= pwcDefenceGuildName;
	sPacket.pwcOffenceGuildName		= pwcOffenceGuildName;
	sPacket.pDefenceGuildEmblem		= pDefenceGuildEmblem;
	sPacket.pOffenceGuildEmblem		= pOffenceGuildEmblem;

	RWS::CMsg msg;
	msg.Id = g_EventScrambleDefaultInfo;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ScrambleDefaultInfoNotify(RwUInt32 uiDojoTableIndex,
													 RwUInt32 uiDefenceGuildID, RwUInt32 uiOffenceGuildID,
													 WCHAR* pwcDefenceGuildName, WCHAR* pwcOffenceGuildName,
													 VOID* pDefenceGuildEmblem, VOID* pOffenceGuildEmblem)
{
	SNtlEventScrambleDefaultInfo sPacket;

	sPacket.uiDojoTableIndex		= uiDojoTableIndex;
	sPacket.uiDefenceGuildID		= uiDefenceGuildID;
	sPacket.uiOffenceGuildID		= uiOffenceGuildID;
	sPacket.pwcDefenceGuildName		= pwcDefenceGuildName;
	sPacket.pwcOffenceGuildName		= pwcOffenceGuildName;
	sPacket.pDefenceGuildEmblem		= pDefenceGuildEmblem;
	sPacket.pOffenceGuildEmblem		= pOffenceGuildEmblem;

	RWS::CMsg msg;
	msg.Id = g_EventScrambleDefaultInfoNotify;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::DojoState(RwUInt32 uiDojoTableIndex, RwUInt8 byDojoState, RwUInt64 ui64time)
{
	SNtlEventDojoMessage sPacket;

	sPacket.uiDojoTableIndex	= uiDojoTableIndex;
	sPacket.byDojoState			= byDojoState;
	sPacket.ui64time			= ui64time;

	RWS::CMsg msg;
	msg.Id = g_EventDojoState;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::DojoStateNotify(RwUInt32 uiDojoTableIndex, RwUInt8 byDojoState, RwUInt64 ui64time)
{
	SNtlEventDojoMessage sPacket;

	sPacket.uiDojoTableIndex	= uiDojoTableIndex;
	sPacket.byDojoState			= byDojoState;
	sPacket.ui64time			= ui64time;

	RWS::CMsg msg;
	msg.Id = g_EventDojoStateNotify;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::HTBRPStock(void)
{
	RWS::CMsg msg;
	msg.Id = g_EventHTBRPStock;
	msg.pData = NULL;
	_SendMsg(msg);
}

void CNtlSLEventGenerator::HTBRPResult(RwInt32 iAttackPoint, RwInt32 iDefenderPoint, RwBool bWin, RwBool bAttacker)
{
	SNtlEventHTPRpResult sPacket;

	sPacket.iAttackPoint	= iAttackPoint;
	sPacket.iDefenderPoint	= iDefenderPoint;
	sPacket.bWin			= bWin;
	sPacket.bAttacker		= bAttacker;

	RWS::CMsg msg;
	msg.Id = g_EventHTBRPResult;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::HelpHintEvent(RwUInt32 uiHelpHint)
{	
	SNtlEventHelpHint sHelpHint;

	sHelpHint.uiHelpHint = uiHelpHint;

	RWS::CMsg msg;
	msg.Id = g_EventHelpHint;
	msg.pData = reinterpret_cast<void*>(&sHelpHint);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::BindEvent(RwUInt8 byBindType, RwUInt32 uiWorldID, RwUInt32 uiBindObjectTblIdx)
{
	SNtlEventBind sBind;
	sBind.byBindType			= byBindType;
	sBind.BindedWorldID			= uiWorldID;
	sBind.uiBindObjectTblIdx	= uiBindObjectTblIdx;

	RWS::CMsg msg;
	msg.Id = g_EventBind;
	msg.pData = &sBind;
	_SendMsg(msg);
}

void CNtlSLEventGenerator::BindNotify()
{
	RWS::CMsg msg;
	msg.Id = g_EventBindNotify;
	msg.pData = NULL;
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SobGetState(SERIAL_HANDLE hSerialId, RwUInt8& byServerState, RwUInt8& byClientState, char* chClientState)
{
	SNtlEventSobGetState sSobGetState;
	sSobGetState.hSerialId = hSerialId;
	sSobGetState.byServerState = 0;
	sSobGetState.byClientState = 0;
	sSobGetState.chClientState = "";

	RWS::CMsg msg;
	msg.Id = g_EventSobGetState;
	msg.pData = reinterpret_cast<void*>(&sSobGetState);
	_SendMsg(msg);

	byServerState = sSobGetState.byServerState;
	byClientState = sSobGetState.byClientState;
	chClientState = sSobGetState.chClientState;
}

void CNtlSLEventGenerator::SobGetFightingMode(SERIAL_HANDLE hSerialId, RwBool& bFightingMode)
{
	SNtlEventSobGetFightingMode sSobGetFightingMode;
	sSobGetFightingMode.hSerialId = hSerialId;
	sSobGetFightingMode.bFightingMode = 0;

	RWS::CMsg msg;
	msg.Id = g_EventSobGetFightingMode;
	msg.pData = reinterpret_cast<void*>(&sSobGetFightingMode);
	_SendMsg(msg);

	bFightingMode = (RwBool)sSobGetFightingMode.bFightingMode;
}

void CNtlSLEventGenerator::SobGetAirMode(SERIAL_HANDLE hSerialId, RwBool& bAirMode)
{
	SNtlEventSobGetAirMode sSobGetAirMode;
	sSobGetAirMode.hSerialId = hSerialId;
	sSobGetAirMode.bAirMode = 0;

	RWS::CMsg msg;
	msg.Id = g_EventSobGetFightingMode;
	msg.pData = reinterpret_cast<void*>(&sSobGetAirMode);
	_SendMsg(msg);

	bAirMode = (RwBool)sSobGetAirMode.bAirMode;
}


void CNtlSLEventGenerator::ReloadTS( void )
{
	sNtlEventReloadTS sReloadTS;

	RWS::CMsg msg;
	msg.Id = g_EventReloadTS;
	msg.pData = reinterpret_cast<void*>(&sReloadTS);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::RegQuest_Nfy( sTS_KEY& sTSKey,
										bool bNewRegister,						// 퀘스트 등록시 New type의 등록 여부
										bool bQuestShare,						// 퀘스트 공유
										RwUInt32 uiQuestAreaName,				// 퀘스트 지역 이름
										RwUInt32 uiQuestState,					// 퀘스트 상태
										RwUInt32 uiQuestTitle,					// 퀘스트 제목
										RwUInt32 uiQuestGoal,					// 퀘스트 목적
										eQUEST_SORT_TYPE eQuestSortType )		// 퀘스트 종류의 타입
{
	SNtlEventRegQuest_Nfy sRegQuestNfy;
	sRegQuestNfy.sTSKey = sTSKey;
	sRegQuestNfy.bNewRegister = bNewRegister;
	sRegQuestNfy.bQuestShare = bQuestShare;
	sRegQuestNfy.uiQuestAreaName = uiQuestAreaName;
	sRegQuestNfy.uiQuestState = uiQuestState;
	sRegQuestNfy.uiQuestTitle = uiQuestTitle;
	sRegQuestNfy.uiQuestGoal = uiQuestGoal;
	sRegQuestNfy.eQuestSortType = eQuestSortType;

	RWS::CMsg msg;
	msg.Id = g_EventRegQuest_Nfy;
	msg.pData = reinterpret_cast<void*>(&sRegQuestNfy);
	_SendMsg(msg);
}


void CNtlSLEventGenerator::UnregQuest_Nfy( sTS_KEY& sTSKey )
{
	SNtlEventUnregQuest_Nfy sUnregQuestNfy;
	sUnregQuestNfy.sTSKey = sTSKey;

	RWS::CMsg msg;
	msg.Id = g_EventUnregQuest_Nfy;
	msg.pData = reinterpret_cast<void*>(&sUnregQuestNfy);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ShowQuestWindow( sTS_KEY& sTSKey )
{
	SNtlEventShowQuestWindow sShowQuestWindow;
	sShowQuestWindow.sTSKey = sTSKey;

	RWS::CMsg msg;
	msg.Id = g_EventShowQuestWindow;
	msg.pData = reinterpret_cast<void*>(&sShowQuestWindow);
	_SendMsg(msg);

}

void CNtlSLEventGenerator::ShowQuestWindow_Nfy( sTS_KEY& sTSKey,
											    RwUInt32 uiQuestAreaName,			// 퀘스트 지역 이름
											    RwUInt32 uiQuestTitle,				// 퀘스트 제목
											    RwUInt32 uiQuestSort,				// 퀘스트 종류
											    RwUInt32 uiQuestGrade,				// 퀘스트 난이도
											    eGRADE_TYPE eGradeType,				// 퀘스트 난이도 타입
											    RwUInt32 uiQuestGoal,				// 퀘스트 목표
											    RwUInt32 uiQuestContents,			// 퀘스트 내용
											    sREWARD_INFO* psDefaultReward,		// 퀘스트 보상 정보 - 기본 보상
											    sREWARD_INFO* psSelectReward,		// 퀘스트 보상 정보 - 선택 보상
	unsigned int uiRewardExp,
	unsigned int uiRewardZeni,
											    eSTOC_EVT_DATA_TYPE eEvtInfoType,	// 서버 이벤트 정보 - 데이터 타입
											    const uSTOC_EVT_DATA& uEvtInfoData,	// 서버 이벤트 정보 - 데이터
											    RwUInt32 uiTimeLimit,				// 타임 제한 정보
											    RwUInt32 uiEventType,				// eEVENT_GEN_TYPE ( 현재는 eEVENT_GEN_TYPE_CLICK_NPC 만 사용함 )
											    RwUInt32 uiEventID,					// uiEventType 에 따른 인덱스 eEVENT_GEN_TYPE_CLICK_NPC	: Mob table index
												RwBool bIsEventStarter )			// Event type [ True : Starter, False : Reward ]

{
	SNtlEventShowQuestWindow_Nfy sShowQuestWindow_Nfy;
	sShowQuestWindow_Nfy.sTSKey = sTSKey;
	sShowQuestWindow_Nfy.uiQuestAreaName = uiQuestAreaName;
	sShowQuestWindow_Nfy.uiQuestTitle = uiQuestTitle;
	sShowQuestWindow_Nfy.uiQuestSort = uiQuestSort;
	sShowQuestWindow_Nfy.uiQuestGrade = uiQuestGrade;
	sShowQuestWindow_Nfy.eGradeType = eGradeType;
	sShowQuestWindow_Nfy.uiQuestGoal = uiQuestGoal;
	sShowQuestWindow_Nfy.uiQuestContents = uiQuestContents;
	sShowQuestWindow_Nfy.uiEventType = uiEventType;
	sShowQuestWindow_Nfy.uiEventID = uiEventID;
	sShowQuestWindow_Nfy.bIsEventStarter = bIsEventStarter;

	sShowQuestWindow_Nfy.uiRewardExp = uiRewardExp;
	sShowQuestWindow_Nfy.uiRewardZeni = uiRewardZeni;

	if ( psDefaultReward )
	{
		memcpy( sShowQuestWindow_Nfy.sDefaultReward, psDefaultReward, sizeof(sShowQuestWindow_Nfy.sDefaultReward) );
	}
	else
	{
		memset( sShowQuestWindow_Nfy.sDefaultReward, 0xff, sizeof(sShowQuestWindow_Nfy.sDefaultReward) );
	}

	if ( psSelectReward )
	{
		memcpy( sShowQuestWindow_Nfy.sSelectReward, psSelectReward, sizeof(sShowQuestWindow_Nfy.sSelectReward) );
	}
	else
	{
		memset( sShowQuestWindow_Nfy.sSelectReward, 0xff, sizeof(sShowQuestWindow_Nfy.sSelectReward) );
	}

	sShowQuestWindow_Nfy.eEvtInfoType = eEvtInfoType;
	memcpy( &sShowQuestWindow_Nfy.uEvtInfoData, &uEvtInfoData, sizeof(sShowQuestWindow_Nfy.uEvtInfoData) );
	sShowQuestWindow_Nfy.uiTimeLimit = uiTimeLimit;

	RWS::CMsg msg;
	msg.Id = g_EventShowQuestWindow_Nfy;
	msg.pData = reinterpret_cast<void*>(&sShowQuestWindow_Nfy);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::UpdateQuestState_Nfy( sTS_KEY& sTSKey,
												 RwBool bOutStateMsg,
												 RwUInt32 uiUpdatedQuestFlag,
												 RwUInt32 uiQuestState,
												 RwUInt32 uiQuestTitle,
												 eSTOC_EVT_DATA_TYPE eEvtInfoType,
												 const uSTOC_EVT_DATA& uEvtInfoData,
												 RwUInt32 uiEventType,
												 RwUInt32 uiEventID,
												 RwBool bIsEventStarter )
{
	SNtlEventUpdateQuestState_Nfy sUpdateQuestStateNfy;
	sUpdateQuestStateNfy.sTSKey = sTSKey;
	sUpdateQuestStateNfy.bOutStateMsg = bOutStateMsg;
	sUpdateQuestStateNfy.uiUpdatedQuestFlag = uiUpdatedQuestFlag;
	sUpdateQuestStateNfy.uiQuestState = uiQuestState;
	sUpdateQuestStateNfy.uiQuestTitle = uiQuestTitle;
	sUpdateQuestStateNfy.eEvtInfoType = eEvtInfoType;
	memcpy( &sUpdateQuestStateNfy.uEvtInfoData, &uEvtInfoData, sizeof( uSTOC_EVT_DATA ) );
	sUpdateQuestStateNfy.uiEventType = uiEventType;
	sUpdateQuestStateNfy.uiEventID = uiEventID;
	sUpdateQuestStateNfy.bIsEventStarter = bIsEventStarter;

	RWS::CMsg msg;
	msg.Id = g_EventUpdateQuestState_Nfy;
	msg.pData = reinterpret_cast<void*>(&sUpdateQuestStateNfy);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::UpdateQuestProgressInfo_Nfy( sTS_KEY& sTSKey,
													   eSTOC_EVT_DATA_TYPE eEvtInfoType,
													   uSTOC_EVT_DATA& uEvtInfoData,
													   RwUInt32 uiTimeLimit )
{
	SNtlEventUpdateQuestProgressInfo_Nfy sUpdateQuestProgressInfoNfy;
	sUpdateQuestProgressInfoNfy.sTSKey = sTSKey;
	sUpdateQuestProgressInfoNfy.eEvtInfoType = eEvtInfoType;
	memcpy( &sUpdateQuestProgressInfoNfy.uEvtInfoData, &uEvtInfoData, sizeof(sUpdateQuestProgressInfoNfy.uEvtInfoData) );
	sUpdateQuestProgressInfoNfy.uiTimeLimit = uiTimeLimit;

	RWS::CMsg msg;
	msg.Id = g_EventUpdateQuestProgressInfo_Nfy;
	msg.pData = reinterpret_cast<void*>(&sUpdateQuestProgressInfoNfy);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::GiveUpQuest( sTS_KEY& sTSKey )
{
	SNtlEventGiveUpQuest sGiveUpQuest;
	sGiveUpQuest.sTSKey = sTSKey;

	RWS::CMsg msg;
	msg.Id = g_EventGiveUpQuest;
	msg.pData = reinterpret_cast<void*>(&sGiveUpQuest);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ShowIndicator( sTS_KEY& sTSKey, RwBool bAuto )
{
	SNtlEventShowIndicator sShowIndicator;
	sShowIndicator.sTSKey = sTSKey;
	sShowIndicator.bAuto = bAuto;

	RWS::CMsg msg;
	msg.Id = g_EventShowIndicator;
	msg.pData = reinterpret_cast<void*>(&sShowIndicator);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::HideIndicator( sTS_KEY& sTSKey )
{
	SNtlEventHideIndicator sHideIndicator;
	sHideIndicator.sTSKey = sTSKey;

	RWS::CMsg msg;
	msg.Id = g_EventHideIndicator;
	msg.pData = reinterpret_cast<void*>(&sHideIndicator);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ShowIndicator_Nfy( sTS_KEY& sTSKey,
											 RwUInt32 uiQState,
											 RwUInt32 uiQuestTitle,
											 RwUInt32 uiQuestGoal,
											 eSTOC_EVT_DATA_TYPE eEvtInfoType,
											 const uSTOC_EVT_DATA& uEvtInfoData,
											 RwUInt32 uiTimeLimit,
											 RwBool bAuto )
{
	SNtlEventShowIndicator_Nfy sShowIndicatorNfy;
	sShowIndicatorNfy.sTSKey = sTSKey;
	sShowIndicatorNfy.uiQState = uiQState;
	sShowIndicatorNfy.uiQuestTitle = uiQuestTitle;
	sShowIndicatorNfy.uiQuestGoal = uiQuestGoal;
	sShowIndicatorNfy.eEvtInfoType = eEvtInfoType;
	memcpy( &sShowIndicatorNfy.uEvtInfoData, &uEvtInfoData, sizeof(sShowIndicatorNfy.uEvtInfoData) );
	sShowIndicatorNfy.uiTimeLimit = uiTimeLimit;
	sShowIndicatorNfy.bAuto = bAuto;

	RWS::CMsg msg;
	msg.Id = g_EventShowIndicator_Nfy;
	msg.pData = reinterpret_cast<void*>(&sShowIndicatorNfy);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::DoQuest( sQUEST_INFO& sQuestInfo,		// Quest 정보
								    eEVENT_GEN_TYPE eEventGenType,	// Event 종류
									RwUInt32 uiOwnerID,				// Owner ID
									RwBool bTransCameraCtrlRight)	// 카메라 제어권을 넘기는 경우 TRUE
{
	SNtlEventDoQuest sDoQuest;
	sDoQuest.sQuestInfo = sQuestInfo;
	sDoQuest.eEventGenType = eEventGenType;
	sDoQuest.uiOwnerID = uiOwnerID;
	sDoQuest.bTransCameraCtrlRight = bTransCameraCtrlRight;

	RWS::CMsg msg;
	msg.Id = g_EventDoQuest;
	msg.pData = reinterpret_cast<void*>(&sDoQuest);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestProposalDialog_Req( sTS_KEY& sTSKey,
												   RwBool bDisplaySummary,
												   RwUInt32 uiQuestTitle,
												   RwUInt32 uiQuestGoal,
												   RwUInt32 uiQuestSort,
												   RwUInt32 uiQuestGrade,
												   eGRADE_TYPE eGradeType,
												   RwUInt32 uiQuestContents,
												   const sREWARD_INFO* psDefaultReward,
												   const sREWARD_INFO* psSelectReward,
	unsigned int uiRewardExp,
	unsigned int uiRewardZeni)
{
	SNtlEventQuestProposalDialog_Req sQuestProposalDialogReq;
	sQuestProposalDialogReq.sTSKey = sTSKey;
	sQuestProposalDialogReq.bDisplaySummary = bDisplaySummary;
	sQuestProposalDialogReq.uiQuestTitle = uiQuestTitle;
	sQuestProposalDialogReq.uiQuestGoal = uiQuestGoal;
	sQuestProposalDialogReq.uiQuestSort = uiQuestSort;
	sQuestProposalDialogReq.uiQuestGrade = uiQuestGrade;
	sQuestProposalDialogReq.eGradeType = eGradeType;
	sQuestProposalDialogReq.uiQuestContents = uiQuestContents;
	sQuestProposalDialogReq.uiRewardExp = uiRewardExp;
	sQuestProposalDialogReq.uiRewardZeni = uiRewardZeni;

	if ( psDefaultReward )
	{
		memcpy( sQuestProposalDialogReq.sDefaultReward, psDefaultReward, sizeof(sQuestProposalDialogReq.sDefaultReward) );
	}
	else
	{
		memset( sQuestProposalDialogReq.sDefaultReward, 0xffffffff, sizeof(sQuestProposalDialogReq.sDefaultReward) );
	}

	if ( psSelectReward )
	{
		memcpy( sQuestProposalDialogReq.sSelectReward, psSelectReward, sizeof(sQuestProposalDialogReq.sSelectReward) );
	}
	else
	{
		memset( sQuestProposalDialogReq.sSelectReward, 0xffffffff, sizeof(sQuestProposalDialogReq.sSelectReward) );
	}

	RWS::CMsg msg;
	msg.Id = g_EventQuestProposalDialog_Req;
	msg.pData = reinterpret_cast<void*>(&sQuestProposalDialogReq);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestProposalDialog_Res( bool bResult, sTS_KEY& sTSKey )
{
	SNtlEventQuestProposalDialog_Res sQuestProposalDialogRes;
	sQuestProposalDialogRes.bResult = bResult;
	sQuestProposalDialogRes.sTSKey = sTSKey;

	RWS::CMsg msg;
	msg.Id = g_EventQuestProposalDialog_Res;
	msg.pData = reinterpret_cast<void*>(&sQuestProposalDialogRes);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestUserSelectDialog_Req( sTS_KEY& sTSKey,
													 RwUInt32 uiTargetType,
													 RwUInt32 uiTargetTblIdx,
													 RwUInt32 uiQuestTitle,				
													 RwUInt32 uiConv,					
													 RwInt32 nBranchCnt,
													 sTS_BRANCH_INFO* pBranchInfo )
{
	SNtlEventQuestUserSelectDialog_Req sQuestUserSelectDialogReq;
	sQuestUserSelectDialogReq.sTSKey = sTSKey;
	sQuestUserSelectDialogReq.uiTargetType = uiTargetType;
	sQuestUserSelectDialogReq.uiTargetTblIdx = uiTargetTblIdx;
	sQuestUserSelectDialogReq.uiQuestTitle = uiQuestTitle;
	sQuestUserSelectDialogReq.uiConv = uiConv;
	sQuestUserSelectDialogReq.nBranchCnt = nBranchCnt;
	memcpy( sQuestUserSelectDialogReq.asBranchInfo, pBranchInfo, sizeof(sQuestUserSelectDialogReq.asBranchInfo) );

	RWS::CMsg msg;
	msg.Id = g_EventQuestUserSelectDialog_Req;
	msg.pData = reinterpret_cast<void*>(&sQuestUserSelectDialogReq);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestUserSelectDialog_Res( bool bResult, sTS_KEY& sTSKey, NTL_TS_TC_ID tcSelID )
{
	SNtlEventQuestUserSelectDialog_Res sQuestUserSelectDialogRes;
	sQuestUserSelectDialogRes.bResult = bResult;
	sQuestUserSelectDialogRes.sTSKey = sTSKey;
	sQuestUserSelectDialogRes.tcSelID = tcSelID;

	RWS::CMsg msg;
	msg.Id = g_EventQuestUserSelectDialog_Res;
	msg.pData = reinterpret_cast<void*>(&sQuestUserSelectDialogRes);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestNarrationUserSelectDialog_Req( sTS_KEY& sTSKey, eUSER_SEL_PROGRESS_STATE eProgState, RwUInt32 uiConv, RwInt32 nBranchCnt, sTS_BRANCH_INFO* pBranchInfo )
{
	SNtlEventQuestNarrationUserSelectDialog_Req sData;
	sData.sTSKey = sTSKey;
	sData.eProgState = eProgState;
	sData.uiConv = uiConv;
	sData.nBranchCnt = nBranchCnt;
	memcpy( sData.asBranchInfo, pBranchInfo, sizeof(sData.asBranchInfo) );

	RWS::CMsg msg;
	msg.Id = g_EventQuestNarrationUserSelectDialog_Req;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::QuestNarrationUserSelectDialog_Res( bool bResult, sTS_KEY& sTSKey, NTL_TS_TC_ID tcSelID )
{
	SNtlEventQuestNarrationUserSelectDialog_Res sData;
	sData.bResult = bResult;
	sData.sTSKey = sTSKey;
	sData.tcSelID = tcSelID;

	RWS::CMsg msg;
	msg.Id = g_EventQuestNarrationUserSelectDialog_Res;
	msg.pData = reinterpret_cast<void*>(&sData);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::TSFinished( unsigned char byTSType, NTL_TS_T_ID tID )
{
	SNtlEventTSFinished sData;
	sData.byTSType = byTSType;
	sData.tID = tID;

	RWS::CMsg msg;
	msg.Id = g_EventTSFinished;
	msg.pData = reinterpret_cast<void*>(&sData);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::TSSkipContainer( NTL_TS_T_ID tID, NTL_TS_TC_ID tcID )
{
	SNtlEventTSSkipContainer sData;
	sData.tID = tID;
	sData.tcID = tcID;

	RWS::CMsg msg;
	msg.Id = g_EventTSSkipContainer;
	msg.pData = reinterpret_cast<void*>(&sData);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestRewardDialog_Req( sTS_KEY& sTSKey,
												 eREWARD_CONTAINER_TYPE eRewardType,
												 RwUInt32 uiLimitTime,
												 RwUInt32 uiQuestTitle,					
												 RwUInt32 uiQuestGoal,
												 RwUInt32 uiQuestSort,
												 RwUInt32 uiQuestGrade,
												 eGRADE_TYPE eGradeType,
												 RwUInt32 uiQuestContents,				
												 const sREWARD_INFO* psDefaultReward,	
												 const sREWARD_INFO* psSelectReward,
	unsigned int uiRewardExp,
	unsigned int uiRewardZeni)
{
	SNtlEventQuestRewardDialog_Req sQuestRewardDialogReq;
	sQuestRewardDialogReq.sTSKey = sTSKey;
	sQuestRewardDialogReq.eRewardType = eRewardType;
	sQuestRewardDialogReq.uiLimitTime = uiLimitTime;
	sQuestRewardDialogReq.uiQuestTitle = uiQuestTitle;
	sQuestRewardDialogReq.uiQuestGoal = uiQuestGoal;
	sQuestRewardDialogReq.uiQuestSort = uiQuestSort;
	sQuestRewardDialogReq.uiQuestGrade = uiQuestGrade;
	sQuestRewardDialogReq.eGradeType = eGradeType;
	sQuestRewardDialogReq.uiQuestContents = uiQuestContents;
	memcpy( sQuestRewardDialogReq.sDefaultReward, psDefaultReward, sizeof(sQuestRewardDialogReq.sDefaultReward) );
	memcpy( sQuestRewardDialogReq.sSelectReward, psSelectReward, sizeof(sQuestRewardDialogReq.sSelectReward) );
	sQuestRewardDialogReq.uiRewardExp = uiRewardExp;
	sQuestRewardDialogReq.uiRewardZeni = uiRewardZeni;

	RWS::CMsg msg;
	msg.Id = g_EventQuestRewardDialog_Req;
	msg.pData = reinterpret_cast<void*>(&sQuestRewardDialogReq);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestRewardDialog_Res( bool bResult, sTS_KEY& sTSKey, int nSelRwdIdx )
{
	SNtlEventQuestRewardDialog_Res sQuestRewardDialogRes;
	sQuestRewardDialogRes.bResult = bResult;
	sQuestRewardDialogRes.sTSKey = sTSKey;
	sQuestRewardDialogRes.nSelRwdIdx = nSelRwdIdx;

	RWS::CMsg msg;
	msg.Id = g_EventQuestRewardDialog_Res;
	msg.pData = reinterpret_cast<void*>(&sQuestRewardDialogRes);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestNarrationDialog_Req( sTS_KEY& sTSKey,
													eNARRATION_PROGRESS_STATE eProgState,		// Narration 진행 상태
													eNARRATION_OWNER_TYPE eOwnerType,			// Owner의 종류
													RwUInt32 uiOwnerIdx,						// Owner의 테이블 인덱스
													eNARRATION_OWNER_STATE eOwnerState,		// Owner의 상태
													eNARRATION_DIALOG_DIR_TYPE eDialogDirType,	// 대화 연출 종류
													RwUInt32 uiDialog,							// 대화 내용
													eNARRATION_GUI_TYPE eGUIType,				// GUI 종류
													RwUInt32 uiMaxLifeTime )					// 이 시간 이상 나레이션 창이 유지 되는 경우 자동으로 Next로 넘어가야 한다.
{
	SNtlEventQuestNarrationDialog_Req sQuestNarrationDialogReq;
	sQuestNarrationDialogReq.sTSKey = sTSKey;
	sQuestNarrationDialogReq.eProgState = eProgState;
	sQuestNarrationDialogReq.eOwnerType = eOwnerType;
	sQuestNarrationDialogReq.uiOwnerIdx = uiOwnerIdx;
	sQuestNarrationDialogReq.eOwnerState = eOwnerState;
	sQuestNarrationDialogReq.eDialogDirType = eDialogDirType;
	sQuestNarrationDialogReq.uiDialog = uiDialog;
	sQuestNarrationDialogReq.eGUIType = eGUIType;
	sQuestNarrationDialogReq.uiMaxLifeTime = uiMaxLifeTime;

	RWS::CMsg msg;
	msg.Id = g_EventQuestNarrationDialog_Req;
	msg.pData = reinterpret_cast<void*>(&sQuestNarrationDialogReq);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestNarrationDialog_Res( bool bResult, sTS_KEY& sTSKey )
{
	SNtlEventQuestNarrationDialog_Res sQuestNarrationDialogRes;
	sQuestNarrationDialogRes.bResult = bResult;
	sQuestNarrationDialogRes.sTSKey = sTSKey;

	RWS::CMsg msg;
	msg.Id = g_EventQuestNarrationDialog_Res;
	msg.pData = reinterpret_cast<void*>(&sQuestNarrationDialogRes);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestNPCDialog_Req( sTS_KEY& sTSKey, eNPCCONV_TYPE eNPCConvType, unsigned int uiNPCIdx, RwUInt32 uiDesc )
{
	SNtlEventQuestNPCDialog_Req sQuestNPCDiagReq;
	sQuestNPCDiagReq.sTSKey = sTSKey;
	sQuestNPCDiagReq.eNPCConvType = eNPCConvType;
	sQuestNPCDiagReq.uiNPCIdx = uiNPCIdx;
	sQuestNPCDiagReq.uiDesc = uiDesc;

	RWS::CMsg msg;
	msg.Id = g_EventQuestNPCDialog_Req;
	msg.pData = reinterpret_cast<void*>(&sQuestNPCDiagReq);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestNPCDialog_Res( sTS_KEY& sTSKey )
{
	SNtlEventQuestNPCDialog_Res sQuestNPCDiagRes;
	sQuestNPCDiagRes.sTSKey = sTSKey;

	RWS::CMsg msg;
	msg.Id = g_EventQuestNPCDialog_Res;
	msg.pData = reinterpret_cast<void*>(&sQuestNPCDiagRes);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestShare_Nfy( sTS_KEY& sTSKey )
{
	SNtlEventQuestShare_Nfy sQuestShareNfy;
	sQuestShareNfy.sTSKey = sTSKey;

	RWS::CMsg msg;
	msg.Id = g_EventQuestShare_Nfy;
	msg.pData = reinterpret_cast<void*>(&sQuestShareNfy);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestAcceptProposal_Nfy( sTS_KEY& sTSKey )
{
	SNtlEventQuestAcceptProposal_Nfy sQuestAcceptProposal_Nfy;
	sQuestAcceptProposal_Nfy.sTSKey = sTSKey;

	RWS::CMsg msg;
	msg.Id = g_EventQuestAcceptProposal_Nfy;
	msg.pData = reinterpret_cast<void*>(&sQuestAcceptProposal_Nfy);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestAcceptReward_Nfy( sTS_KEY& sTSKey, RwUInt32 uiQTitle, RwBool bOutStateMsg )
{
	SNtlEventQuestAcceptReward_Nfy sQuestAcceptReward_Nfy;
	sQuestAcceptReward_Nfy.sTSKey = sTSKey;
	sQuestAcceptReward_Nfy.uiQTitle = uiQTitle;
	sQuestAcceptReward_Nfy.bOutStateMsg = bOutStateMsg;

	RWS::CMsg msg;
	msg.Id = g_EventQuestAcceptReward_Nfy;
	msg.pData = reinterpret_cast<void*>(&sQuestAcceptReward_Nfy);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestAcceptGiveUp_Nfy( sTS_KEY& sTSKey, RwUInt32 uiQTitle )
{
	SNtlEventQuestAcceptGiveUp_Nfy sQuestAcceptGiveUp_Nfy;
	sQuestAcceptGiveUp_Nfy.sTSKey = sTSKey;
	sQuestAcceptGiveUp_Nfy.uiQTitle = uiQTitle;

	RWS::CMsg msg;
	msg.Id = g_EventQuestAcceptGiveUp_Nfy;
	msg.pData = reinterpret_cast<void*>(&sQuestAcceptGiveUp_Nfy);
	_SendMsg(msg);
}


void CNtlSLEventGenerator::RegisterQuestPosMark( QM_KEY QMKey,
												eQMI_TARGET_TYPE eTargetType,
												unsigned int uiTargetWorldTblIdx,
												unsigned int uiTargetTableIdx,
												float fTargetPosX,
												float fTargetPosY,
												float fTargetPosZ,
												unsigned int uiTooltipTblIdx )
{
	SNtlEventRegisterQuestPosMark sRegisterQuestPosMark;

	sRegisterQuestPosMark.QMKey = QMKey;
	sRegisterQuestPosMark.eTargetType = eTargetType;
	sRegisterQuestPosMark.uiTargetWorldTblIdx = uiTargetWorldTblIdx;
	sRegisterQuestPosMark.uiTargetTableIdx = uiTargetTableIdx;
	sRegisterQuestPosMark.fTargetPosX = fTargetPosX;
	sRegisterQuestPosMark.fTargetPosY = fTargetPosY;
	sRegisterQuestPosMark.fTargetPosZ = fTargetPosZ;
	sRegisterQuestPosMark.uiTooltipTblIdx = uiTooltipTblIdx;

	RWS::CMsg msg;
	msg.Id = g_EventRegisterQuestPosMark;
	msg.pData = reinterpret_cast<void*>(&sRegisterQuestPosMark);
	_SendMsg(msg);
}


void CNtlSLEventGenerator::UnregisterQuestPosMark( QM_KEY QMKey )
{
	SNtlEventUnregisterQuestPosMark sUnregisterQuestPosMark;

	sUnregisterQuestPosMark.QMKey = QMKey;

	RWS::CMsg msg;
	msg.Id = g_EventUnregisterQuestPosMark;
	msg.pData = reinterpret_cast<void*>(&sUnregisterQuestPosMark);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ShowQuestPosMark( sTS_KEY& sTSKey, RwBool bShow )
{
	SNtlEventShowQuestPosMark sShowPosMark;

	sShowPosMark.sTSKey = sTSKey;
	sShowPosMark.bShow = bShow;

	RWS::CMsg msg;
	msg.Id = g_EventShowQuestPosMark;
	msg.pData = reinterpret_cast<void*>( &sShowPosMark );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::QuestObjDialog_Req( sTS_KEY& sTSKey,
											  eOBJCONV_TYPE eObjConvType,			// Object 대화 타입
											  RwUInt32 uiWorldIdx,					// Object 소속 world index
											  RwUInt32 uiObjIdx,					// Object 테이블 인덱스
											  RwUInt32 uiDesc )					// Object 대화
{
	SNtlEventQuestObjDialog_Req sObjDlg;

	sObjDlg.sTSKey = sTSKey;
	sObjDlg.eObjConvType = eObjConvType;
	sObjDlg.uiWorldIdx = uiWorldIdx;
	sObjDlg.uiObjIdx = uiObjIdx;
	sObjDlg.uiDesc = uiDesc;

	RWS::CMsg msg;
	msg.Id = g_EventQuestObjDialog_Req;
	msg.pData = reinterpret_cast<void*>( &sObjDlg );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::QuestObjDialog_Res( sTS_KEY& sTSKey )
{
	SNtlEventQuestObjDialog_Res sObjDlg;

	sObjDlg.sTSKey = sTSKey;

	RWS::CMsg msg;
	msg.Id = g_EventQuestObjDialog_Res;
	msg.pData = reinterpret_cast<void*>( &sObjDlg );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TutorialNfy(RwUInt32 uiMessage, RwBool bShow /* = true */, RwUInt32 uiParam /* = 0xffffffff */)
{
	SNtlEventTutorialNfy sTutorialNfy;

	sTutorialNfy.uiMessage	= uiMessage;
	sTutorialNfy.bShow		= bShow;
	sTutorialNfy.uiParam	= uiParam;

	RWS::CMsg msg;
	msg.Id = g_EventShowTutorialNfy;
	msg.pData = reinterpret_cast<void*>( &sTutorialNfy );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::RemovingTMQQuest_Nfy( NTL_TS_T_ID tRmvTSId )
{
	SNtlEventTSRemovingTMQQuest_Nfy sData;

	sData.tRmvTSId = tRmvTSId;

	RWS::CMsg msg;
	msg.Id = g_EventTSRemovingTMQQuest_Nfy;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSPCDialog_Nfy( RwUInt32 uiMessage )
{
	SNtlEventTSPCDialog_Nfy sPCDialog;

	sPCDialog.uiMessage = uiMessage;

	RWS::CMsg msg;
	msg.Id = g_EventTSPCDialog_Nfy;
	msg.pData = reinterpret_cast<void*>( &sPCDialog );
	_SendMsg( msg );
}


void CNtlSLEventGenerator::TSMudosaTeleport_Req( sTS_KEY& sTSKey )
{
	SNtlEventTSMudosaTeleport_Req data;

	data.sTSKey = sTSKey;

	RWS::CMsg msg;
	msg.Id = g_EventTSMudosaTeleport_Req;
	msg.pData = reinterpret_cast<void*>( &data );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSMudosaTeleport_Res( sTS_KEY& sTSKey, RwUInt8 byMudosaIndex, RwBool bCancel )
{
	SNtlEventTSMudosaTeleport_Res data;

	data.sTSKey = sTSKey;
	data.byMudosaIndex = byMudosaIndex;
	data.bCancel = bCancel;

	RWS::CMsg msg;
	msg.Id = g_EventTSMudosaTeleport_Res;
	msg.pData = reinterpret_cast<void*>( &data );
	_SendMsg( msg );
}



void CNtlSLEventGenerator::TSItemUse( sGU_CHAR_ACTION_ITEM* pItemAction )
{
	if ( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() != pItemAction->handle )
	{
		return;
	}

	SNtlEventTSItemUse sItemUse;

	sItemUse.pItemAction = pItemAction;

	RWS::CMsg msg;
	msg.Id = g_EventTSItemUse;
	msg.pData = reinterpret_cast<void*>( &sItemUse );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSItemGet( RwUInt32 uiItemIdx )
{
	SNtlEventTSItemGet sItemGet;

	sItemGet.uiItemIdx = uiItemIdx;

	RWS::CMsg msg;
	msg.Id = g_EventTSItemGet;
	msg.pData = reinterpret_cast<void*>( &sItemGet );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSItemEquip( RwUInt32 uiItemIdx )
{
	SNtlEventTSItemEquip sItemEquip;

	sItemEquip.uiItemIdx = uiItemIdx;

	RWS::CMsg msg;
	msg.Id = g_EventTSItemEquip;
	msg.pData = reinterpret_cast<void*>( &sItemEquip );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSScoutUse( RwUInt32 uiItemIdx, RwUInt32 uiTargetClassID, RwUInt32 uiTargetIdx )
{
	SNtlEventTSScoutUse sScoutUse;

	sScoutUse.uiItemIdx = uiItemIdx;
	sScoutUse.uiTargetClassID = uiTargetClassID;
	sScoutUse.uiTargetIdx = uiTargetIdx;

	RWS::CMsg msg;
	msg.Id = g_EventTSScoutUse;
	msg.pData = reinterpret_cast<void*>( &sScoutUse );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSSkillUse_Guard( SERIAL_HANDLE hSource )
{
	// TS skill use 이벤트는 아바타가 주체일 경우만 동작한다
	if ( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() != hSource )
	{
		return;
	}

	SNtlEventTSSkillUse sSkillUse;

	sSkillUse.uiSkillType = eEVENT_SKILL_TYPE_GUARD;
	sSkillUse.uiSkillIdx = 0xffffffff;
	sSkillUse.byRpBonusType = DBO_RP_BONUS_TYPE_INVALID;
	memset( sSkillUse.arhTargetHandle, 0xffffffff, sizeof( sSkillUse.arhTargetHandle ) );

	RWS::CMsg msg;
	msg.Id = g_EventTSSkillUse;
	msg.pData = reinterpret_cast<void*>( &sSkillUse );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSSkillUse_Dash( SERIAL_HANDLE hSource )
{
	// TS skill use 이벤트는 아바타가 주체일 경우만 동작한다
	if ( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() != hSource )
	{
		return;
	}

	SNtlEventTSSkillUse sSkillUse;

	sSkillUse.uiSkillType = eEVENT_SKILL_TYPE_DASH;
	sSkillUse.uiSkillIdx = 0xffffffff;
	sSkillUse.byRpBonusType = DBO_RP_BONUS_TYPE_INVALID;
	memset( sSkillUse.arhTargetHandle, 0xffffffff, sizeof( sSkillUse.arhTargetHandle ) );

	RWS::CMsg msg;
	msg.Id = g_EventTSSkillUse;
	msg.pData = reinterpret_cast<void*>( &sSkillUse );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSSkillUse_Charging( SERIAL_HANDLE hSource )
{
	// TS skill use 이벤트는 아바타가 주체일 경우만 동작한다
	if ( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() != hSource )
	{
		return;
	}

	SNtlEventTSSkillUse sSkillUse;

	sSkillUse.uiSkillType = eEVENT_SKILL_TYPE_CHARGING;
	sSkillUse.uiSkillIdx = 0xffffffff;
	sSkillUse.byRpBonusType = DBO_RP_BONUS_TYPE_INVALID;
	memset( sSkillUse.arhTargetHandle, 0xffffffff, sizeof( sSkillUse.arhTargetHandle ) );

	RWS::CMsg msg;
	msg.Id = g_EventTSSkillUse;
	msg.pData = reinterpret_cast<void*>( &sSkillUse );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSSkillUse_Skill( SERIAL_HANDLE hSource, SSkillStuff* pSkillStuff, CNtlAttackData* pAttackData )
{
	// TS skill use 이벤트는 아바타가 주체일 경우만 동작한다
	if ( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() != hSource )
	{
		return;
	}

	SNtlEventTSSkillUse sSkillUse;

	sSkillUse.uiSkillType = eEVENT_SKILL_TYPE_SKILLIDX;
	sSkillUse.uiSkillIdx = pSkillStuff->uiSkillTblId;
	sSkillUse.byRpBonusType = pSkillStuff->byRpBonusType;

	for ( int i = 0; i < NTL_MAX_NUMBER_OF_SKILL_TARGET; i++ )
	{
		if ( i < pAttackData->GetAttackStuffCount() )
		{
			sSkillUse.arhTargetHandle[i] = pAttackData->GetAttackStuff( i )->hDefenderSerialId;
		}
		else
		{
			sSkillUse.arhTargetHandle[i] = INVALID_SERIAL_ID;
		}
	}

	RWS::CMsg msg;
	msg.Id = g_EventTSSkillUse;
	msg.pData = reinterpret_cast<void*>( &sSkillUse );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSSkillUse_HTB( SERIAL_HANDLE hSource, SERIAL_HANDLE hTarget, RwUInt32 uiSkillTblId )
{
	// TS skill use 이벤트는 아바타가 주체일 경우만 동작한다
	if ( GetNtlSLGlobal()->GetSobAvatar()->GetSerialID() != hSource )
	{
		return;
	}

	SNtlEventTSSkillUse sSkillUse;

	sSkillUse.uiSkillIdx = uiSkillTblId;
	sSkillUse.byRpBonusType = DBO_RP_BONUS_TYPE_INVALID;

	memset( sSkillUse.arhTargetHandle, 0xffffffff, sizeof( sSkillUse.arhTargetHandle ) );
	sSkillUse.arhTargetHandle[0] = hTarget;

	RWS::CMsg msg;
	msg.Id = g_EventTSSkillUse;
	msg.pData = reinterpret_cast<void*>( &sSkillUse );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSRankBattle( RwUInt32 uiRBTblIdx, eRB_COND_FLAG eRBCondFlag )
{
	SNtlEventTSRankBattle sRankBattle;

	sRankBattle.uiRBTblIdx = uiRBTblIdx;
	sRankBattle.eRBCondFlag = eRBCondFlag;

	RWS::CMsg msg;
	msg.Id = g_EventTSRankBattle;
	msg.pData = reinterpret_cast<void*>( &sRankBattle );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSColTriggerObject( SERIAL_HANDLE hTriggerObject )
{
	SNtlEventTSColTriggerObject sObj;

	sObj.hTriggerObject = hTriggerObject;

	RWS::CMsg msg;
	msg.Id = g_EventTSColTriggerObject;
	msg.pData = reinterpret_cast<void*>( &sObj );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSBindStone( void )
{
	SNtlEventTSBindStone sData;

	RWS::CMsg msg;
	msg.Id = g_EventTSBindStone;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSSearchQuest( void )
{
	SNtlEventTSSearchQuest sData;

	RWS::CMsg msg;
	msg.Id = g_EventTSSearchQuest;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSItemUpgrade( void )
{
	SNtlEventTSItemUpgrade sData;

	RWS::CMsg msg;
	msg.Id = g_EventTSItemUpgrade;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSTeleport( void )
{
	SNtlEventTSTeleport sData;

	RWS::CMsg msg;
	msg.Id = g_EventTSTeleport;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSBudokai( void )
{
	SNtlEventTSBudokai sData;

	RWS::CMsg msg;
	msg.Id = g_EventTSBudokai;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSSlotMachine( void )
{
	SNtlEventTSSlotMachine sData;

	RWS::CMsg msg;
	msg.Id = g_EventTSSlotMachine;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSHoipoiMix( void )
{
	SNtlEventTSHoipoiMix sData;

	RWS::CMsg msg;
	msg.Id = g_EventTSHoipoiMix;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSPrivateShop( void )
{
	SNtlEventTSPrivateShop sData;

	RWS::CMsg msg;
	msg.Id = g_EventTSPrivateShop;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSFreeBattle( void )
{
	SNtlEventTSFreeBattle sData;

	RWS::CMsg msg;
	msg.Id = g_EventTSFreeBattle;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSItemIdentity( void )
{
	SNtlEventTSItemIdentity sData;

	RWS::CMsg msg;
	msg.Id = g_EventTSItemIdentity;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSUseMail( void )
{
	SNtlEventTSUseMail sData;

	RWS::CMsg msg;
	msg.Id = g_EventTSUseMail;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSParty( void )
{
	SNtlEventTSParty sData;

	RWS::CMsg msg;
	msg.Id = g_EventTSParty;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::TSClickSideIcon()
{
	SNtlEventTSClickSideIcon sData;

	RWS::CMsg msg;
	msg.Id = g_EventTSClickSideIcon;
	msg.pData = reinterpret_cast<void*>(&sData);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::TSLevelCheck()
{
	SNtlEventTSLevelCheck sData;

	RWS::CMsg msg;
	msg.Id = g_EventTSLevelCheck;
	msg.pData = reinterpret_cast<void*>(&sData);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::TSQuest()
{
	SNtlEventTSQuest sData;

	RWS::CMsg msg;
	msg.Id = g_EventTSQuest;
	msg.pData = reinterpret_cast<void*>(&sData);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::TSDialogOpen()
{
	SNtlEventTSDialogOpen sData;

	RWS::CMsg msg;
	msg.Id = g_EventTSDialogOpen;
	msg.pData = reinterpret_cast<void*>(&sData);
	_SendMsg(msg);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* trigger direct event
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNtlSLEventGenerator::QuestTriggerDirectForward(SNtlEventQuestDirect_Forward *pDirect)
{
	RWS::CMsg msg;
	msg.Id = g_EventQuestDirectForward;
	msg.pData = reinterpret_cast<void*>(pDirect);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestTriggerDirectEcho(SNtlEventQuestDirect_Echo *pDirect)
{
	RWS::CMsg msg;
	msg.Id = g_EventQuestDirectEcho;
	msg.pData = reinterpret_cast<void*>(pDirect);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::QuestMark(SERIAL_HANDLE hSerialId, RwUInt8 byMarkType, void *pQuestInfo)
{
	SNtlEventQuestMark sQuestMark;
	sQuestMark.hSerialId	= hSerialId;
	sQuestMark.byMarkType	= byMarkType;
	sQuestMark.pQuestInfo	= pQuestInfo;

	RWS::CMsg msg;
	msg.Id = g_EventQuestMark;
	msg.pData = reinterpret_cast<void*>(&sQuestMark);
	_SendMsg(msg);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* trigger object
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CNtlSLEventGenerator::TObjectUpdateState( SERIAL_HANDLE hSerialId, TBLIDX tblIdx, RwUInt8 byMainState, RwUInt8 bySubState, RwUInt32 uiStateTime )
{
	SNtlEventTObjectUpdateState TObjUpdateState;

	TObjUpdateState.hSerialId = hSerialId;
	TObjUpdateState.tblIdx = tblIdx;
	TObjUpdateState.byMainState = byMainState;
	TObjUpdateState.bySubState = bySubState;
	TObjUpdateState.uiStateTime = uiStateTime;

	RWS::CMsg msg;
	msg.Id = g_EventTObjectUpdateState;
	msg.pData = reinterpret_cast<void*>(&TObjUpdateState);
	_SendMsg(msg);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
* 트리거 오브젝트 스카우터 연출 관련
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------
// Char Ready
void CNtlSLEventGenerator::CharReady(VOID)
{
	RWS::CMsg msg;
	msg.Id = g_EventCharReady;
	msg.pData = NULL;
	_SendMsg(msg);
}

void CNtlSLEventGenerator::ChangeWorldConceptState(EWorldPlayConcept eWorldConcept, RwUInt32 uiState)
{
	SNtlEventWorldConceptState worldConceptState;
	worldConceptState.eWorldConcept	= eWorldConcept;
	worldConceptState.uiState = uiState;

	RWS::CMsg msg;
	msg.Id		= g_EventChangeWorldConceptState;
	msg.pData	= reinterpret_cast<void*>(&worldConceptState);
	_SendMsg(msg);
}

// SideIcon : HintIcon
void CNtlSLEventGenerator::RegSideHintIcon(RwUInt32 uiEventType, RwUInt32 uiTableIndex)
{
	SNtlEventRegSideHintIcon sSideHint;
	sSideHint.uiEventType	= uiEventType;
	sSideHint.uiTableIndex	= uiTableIndex;

	RWS::CMsg msg;
	msg.Id		= g_EventRegSideHintIcon;
	msg.pData	= reinterpret_cast<void*>(&sSideHint);
	_SendMsg(msg);
}

// PrivateShop
void CNtlSLEventGenerator::PrivateShopState(RwUInt32 uiEventType,  RwUInt32 uiPrivateShopState, sPRIVATESHOP_SHOP_DATA* pPrivateShopData, RwBool IsAvatarPrivateShop)
{
	SNtlEventPrivateShopState sPrivateShopState;
	sPrivateShopState.uiEventType			= uiEventType;
	sPrivateShopState.uiPrivateShopState	= uiPrivateShopState;
	sPrivateShopState.pPrivateShopData		= pPrivateShopData;
	sPrivateShopState.IsAvatarPrivateShop	= IsAvatarPrivateShop;

	RWS::CMsg msg;
	msg.Id		= g_EventPrivateShopState;
	msg.pData	= reinterpret_cast<void*>(&sPrivateShopState);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::PrivateShopStateVisitor(RwUInt32 uiEventType, sPRIVATESHOP_SHOP_DATA* pPrivateShopData)
{
	SNtlEventPrivateShopStateVisitor sPrivateShopStateVisitor;
	sPrivateShopStateVisitor.uiEventType		= uiEventType;
	sPrivateShopStateVisitor.pPrivateShopData	= pPrivateShopData;

	RWS::CMsg msg;
	msg.Id		= g_EventPrivateShopStateVisitor;
	msg.pData	= reinterpret_cast<void*>(&sPrivateShopStateVisitor);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::PrivateShopItem(RwUInt32 uiEventType, sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData)
{
	SNtlEventPrivateShopItem sPrivateShopItem;
	sPrivateShopItem.uiEventType			= uiEventType;
	sPrivateShopItem.sPrivateShopItemData	= *pPrivateShopItemData;

	RWS::CMsg msg;
	msg.Id		= g_EventPrivateShopItem;
	msg.pData	= reinterpret_cast<void*>(&sPrivateShopItem);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::PrivateShopItemSelect(RwUInt8 uiPrivateShopPos, RwUInt8 uiItemState)
{
	SNtlEventPrivateShopItemSelect sPrivateShopItemSelect;
	sPrivateShopItemSelect.uiPrivateShopPos		= uiPrivateShopPos;
	sPrivateShopItemSelect.uiItemState			= uiItemState;

	RWS::CMsg msg;
	msg.Id		= g_EventPrivateShopItemSelect;
	msg.pData	= reinterpret_cast<void*>(&sPrivateShopItemSelect);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::PrivateShopItemDataInfo(SERIAL_HANDLE hOwnerSerial, sPRIVATESHOP_ITEM_DATA* pPrivateShopItemData)
{
	SNtlEventPrivateShopItemDataInfo sPrivateShopItemDataInfo;
	sPrivateShopItemDataInfo.hOwnerSerial			= hOwnerSerial;
	sPrivateShopItemDataInfo.sPrivateShopItemData	= *pPrivateShopItemData;

	RWS::CMsg msg;
	msg.Id		= g_EventPrivateShopItemDataInfo;
	msg.pData	= reinterpret_cast<void*>(&sPrivateShopItemDataInfo);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::PrivateShopItemState(RwUInt8 auiStateData[NTL_MAX_PRIVATESHOP_INVENTORY], RwInt32 IStateCount)
{
	SNtlEventPrivateShopItemState sPrivateShopItemState;
	sPrivateShopItemState.iStateCount	= IStateCount;

	memcpy(sPrivateShopItemState.auiItemState, auiStateData, sizeof(RwUInt8) * NTL_MAX_PRIVATESHOP_INVENTORY);

	RWS::CMsg msg;
	msg.Id		= g_EventPrivateShopItemState;
	msg.pData	= reinterpret_cast<void*>(&sPrivateShopItemState);
	_SendMsg(msg);
} 

void CNtlSLEventGenerator::PrivateShopItemBuying(SERIAL_HANDLE hOwnerSerial, RwBool bIsBuyAvatar, sPRIVATESHOP_ITEM_DATA asPrivateShopItemData[NTL_MAX_BUY_SHOPPING_CART], RwUInt8 uiBuyCount)
{
	SNtlEventPrivateShopItemBuying sPrivateShopItemBuying;
	sPrivateShopItemBuying.hOwnerSerial	= hOwnerSerial;
	sPrivateShopItemBuying.uiBuyCount	= uiBuyCount;
	sPrivateShopItemBuying.bIsBuyAvatar	= bIsBuyAvatar;

	memcpy(sPrivateShopItemBuying.asPrivateShopItemData, asPrivateShopItemData, sizeof(sPRIVATESHOP_ITEM_DATA) * NTL_MAX_BUY_SHOPPING_CART);

	RWS::CMsg msg;
	msg.Id		= g_EventPrivateShopItemBuying;
	msg.pData	= reinterpret_cast<void*>(&sPrivateShopItemBuying);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::PrivateShopNameTag(SERIAL_HANDLE hOwnerSerial, RwBool bIsOwnerEmpty, RwUInt32 uiPrivateShopState, WCHAR* pwcPrivateShopName)
{
	SNtlEventPrivateShopNameTag sPrivateShopNameTag;
	sPrivateShopNameTag.hOwnerSerial		= hOwnerSerial;
	sPrivateShopNameTag.bIsOwnerEmpty		= bIsOwnerEmpty;
	sPrivateShopNameTag.uiPrivateShopState	= uiPrivateShopState;
	sPrivateShopNameTag.pwcPrivateShopName  = pwcPrivateShopName;

	RWS::CMsg msg;
	msg.Id		= g_EventPrivateShopNameTag;
	msg.pData	= reinterpret_cast<void*>(&sPrivateShopNameTag);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SLTMQEnterRes()
{
	SEND_MSG(g_EventTMQEnterRes, NULL);
}

void CNtlSLEventGenerator::SLTMQEvent(RwUInt32 uiEventType, RwUInt32 uiTargetHandle /* = 0xffffffff */,
									  RwUInt32 uiValue /* = 0xffffffff */, void* pExData /* = NULL */)
{
	SNtlEventTMQ sTMQEvent;
	sTMQEvent.uiEventType	= uiEventType;
	sTMQEvent.hTargetHandle = uiTargetHandle;
	sTMQEvent.uiValue = uiValue;
	sTMQEvent.pExData = pExData;

	RWS::CMsg msg;
	msg.Id		= g_EventTMQ;
	msg.pData	= reinterpret_cast<void*>(&sTMQEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::SLTMQNotify(RwUInt32 uiEventType, RwUInt32 uiValue /* = 0xffffffff */)
{
	SNtlEventTMQNotify sTMQEvent;
	sTMQEvent.uiEventType	= uiEventType;
	sTMQEvent.uiValue		= uiValue;

	RWS::CMsg msg;
	msg.Id		= g_EventTMQNotify;
	msg.pData	= reinterpret_cast<void*>(&sTMQEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::NightEffect( RwBool bOn ) 
{
	SNtlEventNightEffect Data;
	Data.bOn = bOn;

	SEND_MSG(g_EventNightEffect, &Data);
}

void CNtlSLEventGenerator::DBCNarration(const char* pszTextureName, const WCHAR* pwszText, RwInt32 eNarrationState)
{
	SNtlEventDBCNarration data;
	data.pszTextureName = pszTextureName;
	data.pwszText = pwszText;
	data.eNarrationState = eNarrationState;

	SEND_MSG(g_EventDBCNarration, &data);
}

void CNtlSLEventGenerator::DBCNarrationEnd(void)
{
	SEND_MSG(g_EventDBCNarrationEnd, NULL);
}

void CNtlSLEventGenerator::DBCCheck_Res( WORD wResultCode, RwUInt32 uiAltarID ) 
{
	SNtlEventDBC_Res data;
	data.uiAltarID = uiAltarID;
	data.wResultCode = wResultCode;

	SEND_MSG(g_EventDBCCheck_Res, &data);
}

void CNtlSLEventGenerator::DBCReward_Res( WORD wResultCode, RwUInt32 uiAltarID ) 
{
	SNtlEventDBC_Res data;
	data.uiAltarID = uiAltarID;
	data.wResultCode = wResultCode;

	SEND_MSG(g_EventDBCReward_Res, &data);
}

void CNtlSLEventGenerator::DBCCollect_Nfy( WORD wResultCode, RwUInt32 uiAltarID ) 
{
	SNtlEventDBC_Res data;
	data.uiAltarID = uiAltarID;
	data.wResultCode = wResultCode;

	SEND_MSG(g_EventDBCCollect_Nfy, &data);
}

void CNtlSLEventGenerator::DBCSchedule_Info( bool bIsAlive, BYTE byEventType, RwInt64 nStartTime, RwInt64 nEndTime, DWORD dwMainTerm, DWORD dwSubTerm, BYTE byTermType ) 
{
	SNtlEventDBCSchedule_Info data;
	data.bIsAlive = bIsAlive;
	data.byEventType = byEventType;
	data.nStartTime = nStartTime;
	data.nEndTime = nEndTime;
	data.dwMainTerm = dwMainTerm;
	data.dwSubTerm = dwSubTerm;
	data.byTermType = byTermType;

	SEND_MSG(g_EventDBCShedule_Info, &data);
}

void CNtlSLEventGenerator::DBCScatter() 
{
	SEND_MSG(g_EventDBCScatter, NULL);
}

//void CNtlSLEventGenerator::RBBoardUpdate(RwUInt32 tblidx, RwUInt16 wTotalPage, RwUInt16 wCurPage, RwUInt8 byRoomCount, RwUInt8 byRoomType, void* pRoomList)
//{
//	SNtlEventRBBoardUpdate sRBBoardUpdate;
//	sRBBoardUpdate.tblidx = tblidx;
//	sRBBoardUpdate.wTotalPage = wTotalPage;
//	sRBBoardUpdate.wCurPage = wCurPage;
//	sRBBoardUpdate.byRoomCount = byRoomCount;
//	sRBBoardUpdate.byRoomType = byRoomType;
//	sRBBoardUpdate.pRoomList = pRoomList;
//
//	RWS::CMsg msg;
//	msg.Id = g_EventRBBoardUpdate;
//	msg.pData = reinterpret_cast<void*>(&sRBBoardUpdate);
//	_SendMsg(msg);
//}
//
//void CNtlSLEventGenerator::RBRoomInfo(RwUInt8 byRoomMode, void* pRoomInfo)
//{
//	SNtlEventRBRoomInfo sRBRoomInfo;
//	sRBRoomInfo.byRoomMode = byRoomMode;
//	sRBRoomInfo.pRoomInfo = pRoomInfo;
//
//	RWS::CMsg msg;
//	msg.Id = g_EventRBRoomInfo;
//	msg.pData = reinterpret_cast<void*>(&sRBRoomInfo);
//	_SendMsg(msg);
//}
//
//void CNtlSLEventGenerator::RBRoomEvent(RwInt32 type, RwUInt32 tblidx, RwUInt32 roomidx)
//{
//	SNtlEventRBRoom sRBRoom;
//	sRBRoom.type = type;
//	sRBRoom.tblidx = tblidx;
//	sRBRoom.roomidx = roomidx;
//
//	RWS::CMsg msg;
//	msg.Id = g_EventRBRoom;
//	msg.pData = reinterpret_cast<void*>(&sRBRoom);
//	_SendMsg(msg);
//}
//
//void CNtlSLEventGenerator::RBMatchEvent(RwInt32 type, void* pReadyData /* = NULL */, RwUInt8 byDataCount /* = 0 */)
//{
//	SNtlEventRBMatch sRBMatch;
//	sRBMatch.type = type;
//	sRBMatch.pReadyData = pReadyData;
//	sRBMatch.byDataCount = byDataCount;
//
//	RWS::CMsg msg;
//	msg.Id = g_EventRBMatch;
//	msg.pData = reinterpret_cast<void*>(&sRBMatch);
//	_SendMsg(msg);
//}

void CNtlSLEventGenerator::RBBoardInfo( RwUInt8 byBattleMode, RwUInt32 uiRemainTime, RwUInt8 byArenaCount, sRANKBATTLE_ARENA_INFO* pArenaInfo )
{
	SNtlEventRBBoardInfo sData;
	sData.byBattleMode = byBattleMode;
	sData.uiRemainTime = uiRemainTime;
	sData.byArenaCount = byArenaCount;
	sData.pArenaInfo = pArenaInfo;

	RWS::CMsg msg;
	msg.Id = g_EventRBBoardInfo;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::RBJoin( RwBool bRes, RwUInt32 hBoardObject, RwUInt32 tblidx, RwUInt32 uiRemainTime, RwUInt8 byArenaCount, sRANKBATTLE_ARENA_INFO* pArenaInfo )
{
	SNtlEventRBJoin	sData;
	sData.eType = SNtlEventRBJoin::STATE_JOIN;
	sData.bRes = bRes;
	sData.hBoardObject = hBoardObject;
	sData.tblidx = tblidx;
	sData.uiRemainTime = uiRemainTime;
	sData.byArenaCount = byArenaCount;
	sData.pArenaInfo = pArenaInfo;

	RWS::CMsg msg;
	msg.Id = g_EventRBJoin;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::RBJoinFail(void)
{
	SNtlEventRBJoin sData;
	sData.eType = SNtlEventRBJoin::STATE_JOINFAIL;

	RWS::CMsg msg;
	msg.Id = g_EventRBJoin;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::RBLeave(void)
{
	SNtlEventRBJoin sData;
	sData.eType = SNtlEventRBJoin::STATE_LEAVE;

	RWS::CMsg msg;
	msg.Id = g_EventRBJoin;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::RBCancled(void)
{
	SNtlEventRBJoin sData;
	sData.eType = SNtlEventRBJoin::STATE_CANCELED;

	RWS::CMsg msg;
	msg.Id = g_EventRBJoin;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::RBStart( RwInt16 wResultCode )
{
	SNtlEventRBJoin sData;
	sData.eType = SNtlEventRBJoin::STATE_START;
	sData.wResultCode = wResultCode;

	RWS::CMsg msg;
	msg.Id = g_EventRBJoin;
	msg.pData = reinterpret_cast<void*>( &sData );
	_SendMsg( msg );
}

void CNtlSLEventGenerator::RBBattleStateUpdate(RwUInt8 byBattleState, RwUInt8 byStage)
{
	SNtlEventRBBattleStateUpdate sRBBattleStateUpdate;
	sRBBattleStateUpdate.byBattleState = byBattleState;
	sRBBattleStateUpdate.byStage = byStage;

	RWS::CMsg msg;
	msg.Id = g_EventRBBattleStateUpdate;
	msg.pData = reinterpret_cast<void*>(&sRBBattleStateUpdate);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::RBBattleTeamInfo(RwUInt16 wStraightWinCount, RwUInt16 wPerfectWinCount, RwUInt8 byCount, void* pMemberList, WCHAR* wcOwnerPartyName, WCHAR* wcChallengerPartyName, RwUInt32 uiOwnerPartyNameSize, RwUInt32 uiChallengerPartyNameSize )
{
	SNtlEventRBBattleTeamInfo sRBTeamInfo;
	memset( &sRBTeamInfo, 0, sizeof( SNtlEventRBBattleTeamInfo ) );
	sRBTeamInfo.byCount = byCount;
	sRBTeamInfo.pMemberList = pMemberList;
	sRBTeamInfo.wStraightWinCount = wStraightWinCount;	
	sRBTeamInfo.wPerfectWinCount = wPerfectWinCount;
	if( wcOwnerPartyName )
		memcpy_s( sRBTeamInfo.wcOwnerPartyName, sizeof(sRBTeamInfo.wcOwnerPartyName), wcOwnerPartyName, uiOwnerPartyNameSize );
        
	if( wcChallengerPartyName )
		memcpy_s( sRBTeamInfo.wcChallengerPartyName, sizeof(sRBTeamInfo.wcChallengerPartyName), wcChallengerPartyName, uiChallengerPartyNameSize );

	RWS::CMsg msg;
	msg.Id = g_EventRBBattleTeamInfo;
	msg.pData = reinterpret_cast<void*>( &sRBTeamInfo );
	_SendMsg(msg);
}

void CNtlSLEventGenerator::RBBattlePlayerState(RwUInt32 hSerial, RwUInt8 byState)
{
	SNtlEventRBBattlePlayerState sRBPlayerState;
	sRBPlayerState.hSerial = hSerial;
	sRBPlayerState.byState = byState;

	RWS::CMsg msg;
	msg.Id = g_EventRBBattlePlayerState;
	msg.pData = reinterpret_cast<void*>(&sRBPlayerState);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::RBBattleKillScoreUpdate(RwUInt8 byOwnerKillScore, RwUInt8 byChallengerKillScore)
{
	SNtlEventRBBattleKillScoreUpdate sRBKillScoreUpdate;
	sRBKillScoreUpdate.byOwnerKillScore = byOwnerKillScore;
	sRBKillScoreUpdate.byChallengerKillScore = byChallengerKillScore;

	RWS::CMsg msg;
	msg.Id = g_EventRBBattleKillScoreUpdate;
	msg.pData = reinterpret_cast<void*>(&sRBKillScoreUpdate);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::RBBattleStageFinish(RwUInt8 byBattleResult, void* pStageScoreData)
{
	SNtlEventRBBattleStageFinish sRBStageFinish;
	sRBStageFinish.byBattleResult = byBattleResult;
	sRBStageFinish.pStageScoreData = pStageScoreData;

	RWS::CMsg msg;
	msg.Id = g_EventRBBattleStageFinish;
	msg.pData = reinterpret_cast<void*>(&sRBStageFinish);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::RBBattleMatchFinish(void* pMatchResult)
{
	SNtlEventRBBattleMatchFinish sRBMatchFinish;
	sRBMatchFinish.pMatchResult = pMatchResult;

	RWS::CMsg msg;
	msg.Id = g_EventRBBattleMatchFinish;
	msg.pData = reinterpret_cast<void*>(&sRBMatchFinish);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::RBBattleEnd(RwUInt8 byWinner)
{
	SNtlEventRBBattleEnd sRBBattleEnd;
	sRBBattleEnd.byWinner = byWinner;

	RWS::CMsg msg;
	msg.Id = g_EventRBBattleEnd;
	msg.pData = reinterpret_cast<void*>(&sRBBattleEnd);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::RBTotalScoreUpdate(RwUInt8 byBattleMode, void* pTotalScoreData)
{
	SNtlEventRBTotalScoreUpdate sRBTotalScoreUpdate;
	sRBTotalScoreUpdate.byBattleMode = byBattleMode;
	sRBTotalScoreUpdate.pTotalScoreData = pTotalScoreData;

	RWS::CMsg msg;
	msg.Id = g_EventRBTotalScoreUpdate;
	msg.pData = reinterpret_cast<void*>(&sRBTotalScoreUpdate);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::RBForcedEnd(void)
{
	RWS::CMsg msg;
	msg.Id = g_EventRBForcedEnd;
	msg.pData = NULL;
	_SendMsg(msg);
}

// MailSystem
void CNtlSLEventGenerator::MailStart(SERIAL_HANDLE hSerialID, RwBool bIsAway, RwBool bSucceeded)
{
	SNtlEventMailStart sEvent;
	sEvent.hSerial = hSerialID;
	sEvent.bIsAway = bIsAway;

	RWS::CMsg msg;
	msg.Id		= g_EventMailStart;
	msg.pData	= reinterpret_cast<void*>(&sEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::MailLoadInfo(sMAIL_PROFILE* pMailProfile)
{
	SNtlEventMailLoadInfo sEvent;
	memcpy(&sEvent.sMailProfile, pMailProfile, sizeof(sMAIL_PROFILE));

	RWS::CMsg msg;
	msg.Id		= g_EventMailLoadInfo;
	msg.pData	= reinterpret_cast<void*>(&sEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::MailLoadData(sMAIL_PROFILE* pMailProfile)
{
	SNtlEventMailLoadData sEvent;
	memcpy(&sEvent.sMailProfile, pMailProfile, sizeof(sMAIL_PROFILE));

	RWS::CMsg msg;
	msg.Id		= g_EventMailLoadData;
	msg.pData	= reinterpret_cast<void*>(&sEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::MailSend(SERIAL_HANDLE hSerialID, WCHAR* pwcTargetName)
{
	SNtlEventMailSend sEvent;
	sEvent.hSerialID	= hSerialID;
	sEvent.pwcTargetName= pwcTargetName;

	RWS::CMsg msg;
	msg.Id		= g_EventMailSend;
	msg.pData	= reinterpret_cast<void*>(&sEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::MailRead(SERIAL_HANDLE hSerialID, MAILID mailID, DBOTIME endTime)
{
	SNtlEventMailRead sEvent;
	sEvent.hSerialID	= hSerialID;
	sEvent.mailID		= mailID;
	sEvent.endTime		= endTime;

	RWS::CMsg msg;
	msg.Id		= g_EventMailRead;
	msg.pData	= reinterpret_cast<void*>(&sEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::MailDel(SERIAL_HANDLE hSerialID, MAILID mailID)
{
	SNtlEventMailDel sEvent;
	sEvent.hSerialID	= hSerialID;
	sEvent.mailID		= mailID;

	RWS::CMsg msg;
	msg.Id		= g_EventMailDel;
	msg.pData	= reinterpret_cast<void*>(&sEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::MailReturn(SERIAL_HANDLE hSerialID, MAILID mailID)
{
	SNtlEventMailReturn sEvent;
	sEvent.hSerialID	= hSerialID;
	sEvent.mailID		= mailID;

	RWS::CMsg msg;
	msg.Id		= g_EventMailReturn;
	msg.pData	= reinterpret_cast<void*>(&sEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::MailReload(SERIAL_HANDLE hSerialID, MAILID* pmailID, RwUInt8 uiMailCount, RwUInt8 uiUnreadMailCountNormal, RwUInt8 uiUnreadMailCountManager)
{
	SNtlEventMailReload sEvent;
	sEvent.hSerialID				= hSerialID;
	sEvent.pmailID					= pmailID;
	sEvent.uiMailCount				= uiMailCount;
	sEvent.uiUnreadMailCountNormal	= uiUnreadMailCountNormal;
	sEvent.uiUnreadMailCountManager	= uiUnreadMailCountManager;

	RWS::CMsg msg;
	msg.Id		= g_EventMailReload;
	msg.pData	= reinterpret_cast<void*>(&sEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::MailReloadNfy(MAILID* pmailID, RwUInt8 uiMailCount, RwUInt8 uiUnreadMailCountNormal, RwUInt8 uiUnreadMailCountManager)
{
	SNtlEventMailReloadNfy sEvent;
	sEvent.pmailID					= pmailID;
	sEvent.uiMailCount				= uiMailCount;
	sEvent.uiUnreadMailCountNormal	= uiUnreadMailCountNormal;
	sEvent.uiUnreadMailCountManager	= uiUnreadMailCountManager;

	RWS::CMsg msg;
	msg.Id		= g_EventMailReloadNfy;
	msg.pData	= reinterpret_cast<void*>(&sEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::MailLoad(SERIAL_HANDLE hSerialID)
{
	SNtlEventMailLoad sEvent;
	sEvent.hSerialID	= hSerialID;

	RWS::CMsg msg;
	msg.Id		= g_EventMailLoad;
	msg.pData	= reinterpret_cast<void*>(&sEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::MailItemReceive(SERIAL_HANDLE hSerialID, MAILID mailID)
{
	SNtlEventMailItemReceive sEvent;
	sEvent.hSerialID	= hSerialID;
	sEvent.mailID		= mailID;

	RWS::CMsg msg;
	msg.Id		= g_EventMailItemReceive;
	msg.pData	= reinterpret_cast<void*>(&sEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::MailLock(SERIAL_HANDLE hSerialID, MAILID mailID, RwBool bIsLock, DBOTIME endTime)
{
	SNtlEventMailLock sEvent;
	sEvent.hSerialID	= hSerialID;
	sEvent.mailID		= mailID;
	sEvent.bIsLock		= bIsLock;
	sEvent.endTime		= endTime;

	RWS::CMsg msg;
	msg.Id		= g_EventMailLock;
	msg.pData	= reinterpret_cast<void*>(&sEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::MailCloseNfy()
{
	RWS::CMsg msg;
	msg.Id		= g_EventMailCloseNfy;
	msg.pData	= NULL;
	_SendMsg(msg);
}

void CNtlSLEventGenerator::MailSystemNfy(RwUInt32 uiNotifyType, RwUInt32 uiNotifyValue)
{
	SNtlEventMailSystemNfy sEvent;
	sEvent.uiNotifyType		= uiNotifyType;
	sEvent.uiNotifyValue	= uiNotifyValue;

	RWS::CMsg msg;
	msg.Id		= g_EventMailSystemNfy;
	msg.pData	= reinterpret_cast<void*>(&sEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::MailMultiDel( SERIAL_HANDLE hObject, RwUInt8 byCount, MAILID* pMailID )
{
	SNtlEventMailMultiDelRes sResult;
	sResult.hObject = hObject;
	sResult.byCount = byCount;
	memcpy( sResult.aMailID, pMailID, sizeof( MAILID ) * byCount );

	SEND_MSG( g_EventMailMultiDelRes, &sResult );
}

void CNtlSLEventGenerator::AuctionHouseListRes(RwUInt32 uiPage, RwUInt32 uiMaxPage, BYTE byListNum, BYTE byPage)
{
	SNtlEventAuctionHouseListRes sResult;
	sResult.uiPage = uiPage;
	sResult.uiMaxPage = uiMaxPage;
	sResult.byListNum = byListNum;
	sResult.byPage = byPage;

	SEND_MSG(g_EventAuctionHouseListRes, &sResult);
}

void CNtlSLEventGenerator::AuctionHouseListData(void * pItemData)
{
	SNtlEventAuctionHouseListData sResult;
	sResult.pItemData = pItemData;

	SEND_MSG(g_EventAuctionHouseListData, &sResult);
}

void CNtlSLEventGenerator::AuctionHouseSellRes(BYTE byPlace, BYTE byPosition, BYTE byCount)
{
	SNtlEventAuctionHouseSellRes sResult;
	sResult.byPlace = byPlace;
	sResult.byPosition = byPosition;
	sResult.byCount = byCount;

	SEND_MSG(g_EventAuctionHouseSellRes, &sResult);
}

void CNtlSLEventGenerator::AuctionHouseSellCancelRes(ITEMID nItem)
{
	SNtlEventAuctionHouseSellCancelRes sResult;
	sResult.nItem = nItem;

	SEND_MSG(g_EventAuctionHouseSellCancelRes, &sResult);
}

void CNtlSLEventGenerator::AuctionHouseBuyRes(ITEMID nItem)
{
	SNtlEventAuctionHouseBuyRes sResult;
	sResult.nItem = nItem;

	SEND_MSG(g_EventAuctionHouseBuyRes, &sResult);
}

void CNtlSLEventGenerator::CharAway(bool bIsAway)
{
	SNtlEventCharAway sEvent;
	sEvent.bIsAway = bIsAway;

	RWS::CMsg msg;
	msg.Id		= g_EventCharAway;
	msg.pData	= reinterpret_cast<void*>(&sEvent);
	_SendMsg(msg);
}

void CNtlSLEventGenerator::TMQCleintState(EWorldPlayConcept eWorldConcept, RwUInt32 uiState)
{
	SNtlEventTMQServerState data;
	data.eWorldConcept	= eWorldConcept;
	data.uiState		= uiState;

	RWS::CMsg msg;
	msg.Id = g_EventTMQCleintState;
	msg.pData = reinterpret_cast<void*>(&data);
	_SendMsg(msg);	
}

void CNtlSLEventGenerator::ShowToolTip( RwBool bShow, RwInt32 nX, RwInt32 nY, const std::wstring& wstrText ) 
{
	SNtlEventToolTip data;
	data.bShow = bShow;
	data.nX = nX;
	data.nY = nY;
	data.wstrText = wstrText;

	RWS::CMsg msg;
	msg.Id = g_EventToolTip;
	msg.pData = reinterpret_cast<void*>(&data);
	_SendMsg(msg);
}

//--------------------------------------------
// ActionMap
void CNtlSLEventGenerator::ActionMapLoadInfo( RwUInt8 byCount, sSHORTCUT_DATA* pasData ) 
{
	SNtlEventActionMapLoadInfo data;
	data.byCount = byCount;
	memcpy( &data.asData, pasData, sizeof(sSHORTCUT_DATA) * byCount );
	SEND_MSG(g_EventActionMapLoadInfo, &data);
}

void CNtlSLEventGenerator::ActionMapUpdateRes( RwBool bSuccess )
{
	SNtlEventActionMapUpdateRes data;
	data.bSuccess = bSuccess;
	SEND_MSG(g_EventActionMapUpdateRes, &data);
}

void CNtlSLEventGenerator::ActionMapClientNotify( RwUInt8 byType, RwUInt16 wParam1 /*= 0xFFFF*/, RwUInt16 wParam2 /*= 0xFFFF */ ) 
{
	SNtlEventActionMapClientNotify data;
	data.byType = byType;
	data.wParam1 = wParam1;
	data.wParam2 = wParam2;
	SEND_MSG(g_EventActionMapClientNotify, &data);
}

void CNtlSLEventGenerator::PortalInfoRes( BYTE byCount, PORTALID* paPortalID )
{
	SNtlEventPortalInfoRes data;
	data.byCount = byCount;
	data.paPortalID = paPortalID;
	SEND_MSG(g_EventPortalInfoRes, &data);
}

void CNtlSLEventGenerator::PortalAddReq(RwUInt32 hNPCSerialID)
{
	SNtlEventPortalAddReq data;
	data.hNPCSerailID = hNPCSerialID;
	SEND_MSG(g_EventPortalAddReq, &data);
}

void CNtlSLEventGenerator::PortalAddRes(PORTALID portalID )
{
	SNtlEventPortalAddRes data;
	data.portalID = portalID;
	SEND_MSG(g_EventPortalAddRes, &data);
}

void CNtlSLEventGenerator::PortalRes(PORTALID byPoint )
{
	SNtlEventPortalRes data;
	data.byPoint = byPoint;
	SEND_MSG(g_EventPortalRes, &data);
}

void CNtlSLEventGenerator::OperateObjectTimeBar(bool bShow, RwUInt32 uiPlayTime)
{
	SNtlEventOperateObjectTimeBar data;
	data.bShow = bShow;
	data.uiPlayTime = uiPlayTime;
	SEND_MSG(g_EventOperateObjectTimeBar, &data);
}

void CNtlSLEventGenerator::UIConfigReload() 
{
	SEND_MSG(g_EventUIConfigReload, NULL);
}

void CNtlSLEventGenerator::SLUDEnterRes()
{
	SEND_MSG(g_EventUDEnterRes, NULL);
}

void CNtlSLEventGenerator::SLDirection_Nfy(RwBool bCreate, RwV3d vLoc)
{
	SNtlEventDirectionNfy data;
	data.bCreate = bCreate;
	data.vLoc = vLoc;
	SEND_MSG(g_EventDirectionNfy, &data);
}

void CNtlSLEventGenerator::BroadMsgNfy_TSBeg( eBROAD_MSG_UI_DIR_TYPE eUIDirType,
											  eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE eUIShowHideType,
											  eBROAD_MSG_UI_BALLOON_SHAPE_TYPE	eUIBalloonShapeType,
											  eBROAD_MSG_UI_SPEECH_DIR_TYPE eUISpeechDirType,
											  eBROAD_MSG_OWNER_CONDITION eOwnerCondition,
											  eBROAD_MSG_OWNER_TYPE eOwnerType,
											  unsigned int uiOwnerTblIdx,
											  unsigned int uiQTblIdx,
											  float fDisplayTime )
{
	sBROAD_MSG_DATA sBroadMsgData;

	sBroadMsgData.eUIDirType = eUIDirType;
	sBroadMsgData.eUIShowHideType = eUIShowHideType;
	sBroadMsgData.eUIBalloonShapeType = eUIBalloonShapeType;
	sBroadMsgData.eUISpeechDirType = eUISpeechDirType;
	sBroadMsgData.eOwnerCondition = eOwnerCondition;
	sBroadMsgData.eOwnerType = eOwnerType;
	sBroadMsgData.uiOwnerTblIdx = uiOwnerTblIdx;
	sBroadMsgData.fDisplayTime = fDisplayTime;

	CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
	sQUEST_TEXT_DATA_TBLDAT* pQuestText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( uiQTblIdx ) );
	if ( pQuestText )
	{
		sBroadMsgData.wstrSpeech = pQuestText->wstrText;
	}

	SNtlEventBroadMsgBegNfy data;
	data.byMsgType = eBROAD_MSG_TYPE_TS;
	data.defMsgDataList.push_back( sBroadMsgData );

	SEND_MSG( g_EventBroadMsgBegNfy, &data );
}

void CNtlSLEventGenerator::BroadMsgNfy_TSEnd( void )
{
	SNtlEventBroadMsgEndNfy data;
	data.byMsgType = eBROAD_MSG_TYPE_TS;

	SEND_MSG( g_EventBroadMsgEndNfy, &data );
}

void CNtlSLEventGenerator::BroadMsgNfy_Emergency( std::wstring& wstrSpeech )
{
	sBROAD_MSG_DATA sBroadMsgData;

	// 추후 테이블로 뺄지 말지 토의 필요 ( 현재는 하드코딩 )
	//////////////////////////////////////////////////////////////////////////
	sBroadMsgData.eUIDirType = eBROAD_MSG_UI_DIR_TYPE_WARNING;
	sBroadMsgData.eUIShowHideType = eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_FADE_SLIDE;
	sBroadMsgData.eUIBalloonShapeType = eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_STAR;
	sBroadMsgData.eUISpeechDirType = eBROAD_MSG_UI_SPEECH_DIR_TYPE_NORMAL;
	sBroadMsgData.eOwnerCondition = eBROAD_MSG_OWNER_CONDITION_1;
	sBroadMsgData.eOwnerType = eBROAD_MSG_OWNER_TYPE_NPC;
	sBroadMsgData.uiOwnerTblIdx = 0;
	sBroadMsgData.fDisplayTime = 5;
	//////////////////////////////////////////////////////////////////////////

	sBroadMsgData.wstrSpeech = wstrSpeech;

	SNtlEventBroadMsgBegNfy data;
	data.byMsgType = eBROAD_MSG_TYPE_EMERGENCY;
	data.defMsgDataList.push_back( sBroadMsgData );

	SEND_MSG( g_EventBroadMsgBegNfy, &data );
}

void CNtlSLEventGenerator::BroadMsgNfy_MiniNarration( RwUInt32 uiMiniNarrationTblIdx )
{
	CQuestNarrationTable* pQuestNarrationTbl = API_GetTableContainer()->GetQuestNarrationTable();
	sQUEST_NARRATION_TBLDAT* pData = (sQUEST_NARRATION_TBLDAT*) pQuestNarrationTbl->FindData( uiMiniNarrationTblIdx );
	if ( NULL == pData || pData->bType == false )
	{
		return;
	}

	CQuestTextDataTable* pQuestTextTable = API_GetTableContainer()->GetQuestTextDataTable();
	sQUEST_TEXT_DATA_TBLDAT* pQuestText = NULL;

	sBROAD_MSG_DATA sBroadMsgData;


	SNtlEventBroadMsgBegNfy data;
	data.byMsgType = eBROAD_MSG_TYPE_MINI_NARRATION;

	sQUEST_NARRATION_DATASET sQNData;
	for ( int i = 0; i < pData->byNumber; ++i )
	{
		memcpy( &sQNData, &pData->asData[i], sizeof( sQUEST_NARRATION_DATASET ) );

		sBroadMsgData.eUIDirType = (sQNData.byUIDirection == 255 ? eBROAD_MSG_UI_DIR_TYPE_NORMAL : (eBROAD_MSG_UI_DIR_TYPE)sQNData.byUIDirection);
		sBroadMsgData.eUIShowHideType = (sQNData.byUIShowHideDirection == 255 ? eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE_NORMAL : (eBROAD_MSG_UI_SHOWHIDE_DIR_TYPE)sQNData.byUIShowHideDirection);
		sBroadMsgData.eUIBalloonShapeType = (sQNData.byUiType == 255 ? eBROAD_MSG_UI_BALLOON_SHAPE_TYPE_BC_NORMAL : (eBROAD_MSG_UI_BALLOON_SHAPE_TYPE)sQNData.byUiType);
		sBroadMsgData.eUISpeechDirType = (sQNData.byDirection == 255 ? eBROAD_MSG_UI_SPEECH_DIR_TYPE_NORMAL : (eBROAD_MSG_UI_SPEECH_DIR_TYPE)sQNData.byDirection);
		sBroadMsgData.eOwnerCondition = (sQNData.byCondition == 255 ? eBROAD_MSG_OWNER_CONDITION_1 : (eBROAD_MSG_OWNER_CONDITION)sQNData.byCondition);
		sBroadMsgData.eOwnerType = (sQNData.byOwnerType == 255 ? eBROAD_MSG_OWNER_TYPE_NPC : (eBROAD_MSG_OWNER_TYPE)sQNData.byOwnerType);
		sBroadMsgData.uiOwnerTblIdx = sQNData.dwOwner;
		sBroadMsgData.fDisplayTime = (pData->byTime == 255 ? FLT_MAX : (float)pData->byTime);

		pQuestText = reinterpret_cast<sQUEST_TEXT_DATA_TBLDAT*>( pQuestTextTable->FindData( sQNData.dwDialog ) );

		if ( pQuestText )
		{
			sBroadMsgData.wstrSpeech = pQuestText->wstrText;
		}
		else
		{
			sBroadMsgData.wstrSpeech.clear();
		}

		data.defMsgDataList.push_back( sBroadMsgData );
	}

	SEND_MSG( g_EventBroadMsgBegNfy, &data );
}

void CNtlSLEventGenerator::RpBonusSetupRes(RwUInt32 skillid, RwUInt8 bySlotIndex, RwUInt8 byRpbonusType, RwBool bRpBonusAutoMode)
{
	SNtlEventRpBonusSetupRes data;
	data.skillId = skillid;
	data.bySlotIndex = bySlotIndex;
	data.byRpBonusType = byRpbonusType;
	data.bRpBonusAutoMode = bRpBonusAutoMode;

	SEND_MSG( g_EventRpBonusSetupRes, &data);
}

void CNtlSLEventGenerator::FriendAddRes(WORD wResultCode, WCHAR* wchName, RwUInt32 uiTargetID)
{
	SNtlEventFriendAddRes data;
	data.wResultCode = wResultCode;    
	data.uiTargetID = uiTargetID;
	NTL_SAFE_WCSCPY(data.wchName, wchName);

	SEND_MSG(g_EventFriendAdd_Res, &data);
}

void CNtlSLEventGenerator::FriendDelRes(WORD wResultCode, RwUInt32 uiTargetID)
{
	SNtlEventFriendDelRes data;
	data.wResultCode = wResultCode;    
	data.uiTargetID = uiTargetID;

	SEND_MSG(g_EventFriendDel_Res, &data);
}

void CNtlSLEventGenerator::FriendMoveRes(WORD wReslutCode, RwUInt32 uiTargetID)
{
	SNtlEventFriendMoveRes data;
	data.wResultCode = wReslutCode;    
	data.uiTargetID = uiTargetID;

	// Character name (required for blacklist)
	NTL_SAFE_WCSCPY(data.wchName, GetNtlSLGlobal()->GetSobAvatar()->GetFriendList()->GetMemberbyKey(uiTargetID)->wszMemberName);

	SEND_MSG(g_EventFriendMove_Res, &data);
}

void CNtlSLEventGenerator::FriendListInfo( BYTE byCount, sFRIEND_FULL_INFO* paInfo ) 
{
	SNtlEventFriendListInfo data;
	ZeroMemory(&data, sizeof(SNtlEventFriendListInfo));
	data.byCount = byCount;
	memcpy(data.asInfo, paInfo, sizeof(sFRIEND_FULL_INFO) * byCount);

	SEND_MSG(g_EventFriendList_Info, &data);
}

void CNtlSLEventGenerator::BlackListAddRes( WORD wResultCode, WCHAR* wchName, RwUInt32 uiTargetID ) 
{
	SNtlEventBlackListAddRes data;
	data.wResultCode = wResultCode;
	data.uiTargetID = uiTargetID;
	NTL_SAFE_WCSCPY(data.wchName, wchName);

	SEND_MSG(g_EventBlackListAdd_Res, &data);
}

void CNtlSLEventGenerator::BlackListDelRes( WORD wResultCode, RwUInt32 uiTargetID ) 
{
	SNtlEventBlackListDelRes data;
	data.wResultCode = wResultCode;
	data.uiTargetID = uiTargetID;

	SEND_MSG(g_EventBlackListDel_Res, &data);
}

void CNtlSLEventGenerator::FriendInfo(sFRIEND_INFO* pInfo, bool bIsFirst)
{
	SNtlEventFriendInfo data;
	memcpy(&data.sInfo, pInfo, sizeof(sFRIEND_INFO));
    data.bIsFirst = bIsFirst;

	SEND_MSG(g_EventFriendInfo, &data);
}

void CNtlSLEventGenerator::FriendInfoChange(RwUInt32 uiTargetID, BYTE byChangeType, DWORD dwChangeValue)
{
	SNtlEventFriendInfoChange data;
	data.targetID = uiTargetID;
	data.byChangeType = byChangeType;
	data.dwChangeValue = dwChangeValue;

	SEND_MSG(g_EventFriendInfoChange, &data);
}

void CNtlSLEventGenerator::FriendInfoNotify( RwInt32 nNotifyType, RwUInt32 uiTargetID ) 
{
	SNtlEventFriendInfoNotify data;
	data.nNotifyMsg = nNotifyType;
	data.targetID = uiTargetID;

	SEND_MSG(g_EventFriendInfoNotify, &data);
}



void CNtlSLEventGenerator::KnockDownNfy() 
{
	SEND_MSG(g_EventKnockDownNfy, NULL);
}

void CNtlSLEventGenerator::KnockDownWakeUpNfy() 
{
	SEND_MSG(g_EventKnockDownWakeUpNfy, NULL );
}

void CNtlSLEventGenerator::BlockingNfy(RwBool bStartOrEnd, DWORD dwCoolingTime)
{
	SNtlEventBlockingNfy data;
	data.bStartOrEnd	= bStartOrEnd;
	data.dwCoolingTime	= dwCoolingTime;

	SEND_MSG(g_EventBlockingNfy, &data);
}

void CNtlSLEventGenerator::ChargingNfy(RwBool bStartOrEnd)
{
	SNtlEventChargingNfy data;
	data.bStartOrEnd = bStartOrEnd;

	SEND_MSG(g_EventChargingNfy, &data);
}

void CNtlSLEventGenerator::CommuTargetSelect(CHARACTERID targetID, SERIAL_HANDLE hSerialId, WCHAR* wchName, ECommuTargetType type)
{
	SNtlEventCommuTargetSelect data;
	data.uiCharID = targetID;
	data.uiSerialID = hSerialId;
	wcscpy_s(data.wchName, NTL_MAX_SIZE_CHAR_NAME + 1, wchName);
	data.eTargetType = type;

	SEND_MSG(g_EventCommuTargetSelect, &data);
}

void CNtlSLEventGenerator::CommuTargetRelease( SERIAL_HANDLE hSerialId ) 
{
	SNtlEventCommuTargetRelease data;
	data.uiSerialID = hSerialId;

	SEND_MSG(g_EventCommuTargetRelease, &data);
}

void CNtlSLEventGenerator::PopoNotify( const WCHAR* pwcTitle, const WCHAR* pwcBuffer, RwBool bImmediate /*= FALSE*/, RwReal fLifeTime /*= 0.0f */ ) 
{
	SNtlEventPopoNotify data;
	data.pwcTitle = pwcTitle;
	data.pwcBuffer = pwcBuffer;
	data.bImmediate = bImmediate;
	data.fLifeTime = fLifeTime;

	SEND_MSG(g_EventPopoNotify, &data);
}

void CNtlSLEventGenerator::SobTransform(SERIAL_HANDLE hSerialId, const sASPECTSTATE& aspectState)
{
	SNtlEventTransform data;
    data.hSerialId = hSerialId;	
	data.sApsectState = aspectState;

	SEND_MSG(g_EventTransform, &data);    
}

void CNtlSLEventGenerator::SobMobTransform(SERIAL_HANDLE hSerialId, TBLIDX tblTransformMobId)
{
    SNtlEventMobTransform data;
    data.hSerialId = hSerialId;
    data.tblTransformMobID = tblTransformMobId;

    SEND_MSG(g_EventMobTransform, &data);
}

void CNtlSLEventGenerator::BotCaution_Nfy( SERIAL_HANDLE hSerialId ) 
{
	SNtlEventBotCaution_Nfy data;
	data.hSerialId = hSerialId;

	SEND_MSG(g_EventBotCaution_Nfy, &data);
}

void CNtlSLEventGenerator::ScouterEvent(int iType, RwUInt32 uiSerailID /* = 0 */, int iUserData2 /* = 0 */, int iUserData3 /* = 0 */, 
	int iUserData4 /* = 0 */, int iUserData5 /* = 0 */, int iUserData6 /* = 0 */, int iUserData7 /* = 0 */, int iUserData8 /* = 0 */)
{
	SDboEventScouter packet;

	packet.iType = iType;
	packet.uiUserData = uiSerailID;
	packet.iUserData2 = iUserData2;
	packet.iUserData3 = iUserData3;
	packet.iUserData4 = iUserData4;
	packet.iUserData5 = iUserData5;
	packet.iUserData6 = iUserData6;
	packet.iUserData7 = iUserData7;
	packet.iUserData8 = iUserData8;

	SEND_MSG(g_EventScouter, &packet);
}

void CNtlSLEventGenerator::PartyShareTargetRes( WORD wResultCode ) 
{
	SNtlEventPartyShareTargetRes data;
	data.wResultCode = wResultCode;

	SEND_MSG(g_EventPartyShareTargetRes, &data);
}

void CNtlSLEventGenerator::PartyShareTargetNfy( sSHARETARGET_INFO* aShareTargetInfos ) 
{
	SNtlEventPartyShareTargetNfy data;
	memcpy(data.sharetargetInfo, aShareTargetInfos, sizeof(sSHARETARGET_INFO) * NTL_MAX_SHARETARGET_COUNT);

	SEND_MSG(g_EventPartyShareTargetNfy, &data);
}

void CNtlSLEventGenerator::ShareTargetMarking( SERIAL_HANDLE hSerialId, RwUInt8 bySlot ) 
{
	SNtlEventShareTargetSelect data;
	data.hSerialId = hSerialId;
	data.bySlot = bySlot;

	SEND_MSG(g_EventShareTargetSelect, &data);
}

void CNtlSLEventGenerator::ShareTargetRelease( SERIAL_HANDLE hSerialId ) 
{
	SNtlEventShareTargetRelease data;
	data.hSerialId = hSerialId;

	SEND_MSG(g_EventShareTargetRelease, &data);
}

void CNtlSLEventGenerator::AvatarTargetStateChange(void)
{
	SEND_MSG(g_EventAvatarTargetStateChange, NULL);
}

void CNtlSLEventGenerator::SobOnBus(SERIAL_HANDLE hSerialId, RwBool bRideOn, SERIAL_HANDLE hBusSerialId)
{
	SNtlEventSobOnBus data;
	data.hSerialId = hSerialId;
	data.bRideOn = bRideOn;
	data.hBusSerialId = hBusSerialId;

	SEND_MSG(g_EventSobOnBus, &data);    
}

void CNtlSLEventGenerator::SobOnVehicle( SERIAL_HANDLE hSerialId, RwBool bRideOn, SERIAL_HANDLE hDriverId, RwUInt32 uiVehicleTblIdx )
{
	SNtlEventSobOnVehicle data;
	data.hSerialId = hSerialId;
	data.bRideOn = bRideOn;
	data.hDriverId = hDriverId;
	data.uiVehicleTblIdx = uiVehicleTblIdx;

	SEND_MSG(g_EventSobOnVehicle, &data);
}

void CNtlSLEventGenerator::BusMove(RwUInt8 byMessage, SERIAL_HANDLE hSerialId, TBLIDX idxBusTable, RwV3d* pV3Pos, RwV3d* pV3Dir)
{
	SNtlEventBusMove data;
	data.byMessage		= byMessage;
	data.hBus			= hSerialId;
	data.idxBusTable	= idxBusTable;
	data.pV3Pos			= pV3Pos;
	data.pV3Dir			= pV3Dir;

	SEND_MSG(g_EventBusMove, &data);
}

void CNtlSLEventGenerator::SobVehicleStart( SERIAL_HANDLE hDriver,
											SERIAL_HANDLE hVehicleItem, TBLIDX tblVehicleItem)
{
	SNtlEventSobVehicleStart data;
	data.hSerialId		= hDriver;
	data.hVehicleItem	= hVehicleItem;
	data.tblVehicleItem	= tblVehicleItem;

	SEND_MSG(g_EventSobVehicleStart, &data);
}

void CNtlSLEventGenerator::SobRideOnOffVehicle( SERIAL_HANDLE hSerialId, RwBool bRideOn, RwUInt32 uiVehicleTblIdx )
{
	SNtlEventSobRideOnOffVehicle data;
	data.hSerialId = hSerialId;
	data.bRideOn = bRideOn;
	data.uiVehicleTblIdx = uiVehicleTblIdx;

	SEND_MSG(g_EventSobRideOnOffVehicle, &data);
}

void CNtlSLEventGenerator::SobVehicleEngine(SERIAL_HANDLE hSerialId, RwUInt8 byMessage, RwUInt32 uiParam /*= 0xffffffff*/)
{
	SNtlEventSobVehicleEngine data;
	data.hSerialId	= hSerialId;
	data.byMessage	= byMessage;
	data.uiParam	= uiParam;

	SEND_MSG(g_EventSobVehicleEngine, &data);
}

void CNtlSLEventGenerator::SobVehicleStunt(SERIAL_HANDLE hSerialId)
{
	SNtlEventSobVehicleStunt data;
	data.hSerialId	= hSerialId;

	SEND_MSG(g_EventSobVehicleStunt, &data);
}

void CNtlSLEventGenerator::SobAfterEffectRemainTimeNfy( DWORD dwTimeRemaining ) 
{
	SNtlEventAfterEffectRemainTimeNfy data;
	data.dwTimeRemaining = dwTimeRemaining;

	SEND_MSG(g_EventAfterEffectRemainTimeNfy, &data);
}

void CNtlSLEventGenerator::TLDefaultSkin( const char* pFontName, RwRGBA& rgbFontBackColor, RwRGBA& rgbFontColor, RwInt32 nFontSize, RwUInt32 uiFontAllign, RwV2d& vFontOffset, RwV2d& vMsgLayerOffset, RwV2d& vOkOffset, RwV2d& vIMGOffset )
{
	SNtlEventTLDefaultSkin data;
	data.pFontName = pFontName;
	data.rgbFontBackColor = rgbFontBackColor;
	data.rgbFontColor = rgbFontColor;
	data.nFontSize = nFontSize;
	data.uiFontAllign = uiFontAllign;
	data.vFontOffset = vFontOffset;
	data.vMsgLayerOffset = vMsgLayerOffset;
	data.vOkOffset = vOkOffset;
	data.vIMGOffset = vIMGOffset;

	SEND_MSG( g_EventTLDefaultSkin, &data );
}

void CNtlSLEventGenerator::TLGuideMsg_ShowHide( RwUInt32 uiTextTblIdx, RwReal fFadeInOutTime, RwBool bShow )
{
	SNtlEventTLGuideMsg_ShowHide data;
	data.uiTextTblIdx = uiTextTblIdx;
	data.fFadeInOutTime = fFadeInOutTime;
	data.bShow = bShow;

	SEND_MSG( g_EventTLGuideMsg_ShowHide, &data );
}

void CNtlSLEventGenerator::TLGuideMsg_ChangeColor( RwUInt32 uiTextTblIdx, RwReal fFadeInOutTime, RwRGBA& rgbFontBackColor, RwRGBA& rgbFontColor )
{
	SNtlEventTLGuideMsg_ChangeColor data;
	data.uiTextTblIdx = uiTextTblIdx;
	data.fFadeInOutTime = fFadeInOutTime;
	data.rgbFontBackColor = rgbFontBackColor;
	data.rgbFontColor = rgbFontColor;

	SEND_MSG( g_EventTLGuideMsg_ChangeColor, &data );
}

void CNtlSLEventGenerator::TLGuideIMG_ShowHide( RwChar* pIMGName, RwReal fFadeInOutTime, RwBool bShow, RwReal fSizeRate /* = 1.f */)
{
	SNtlEventTLGuideIMG_ShowHide data;
	data.pIMGName		= pIMGName;
	data.fFadeInOutTime	= fFadeInOutTime;
	data.bShow			= bShow;
	data.fSizeRate		= fSizeRate;

	SEND_MSG( g_EventTLGuideIMG_ShowHide, &data );
}

void CNtlSLEventGenerator::TLGuideIMGOk_ShowHide( RwReal fFadeInOutTime, RwBool bShow )
{
	SNtlEventTLGuideIMGOk_ShowHide data;
	data.fFadeInOutTime = fFadeInOutTime;
	data.bShow = bShow;

	SEND_MSG( g_EventTLGuideIMGOk_ShowHide, &data );
}

void CNtlSLEventGenerator::TLUIIndicator_ShowHide( RwUInt32 uiTLIndicatorType, RwChar* pcFlashName,
												   RwV2d& vFocusOffPos, RwBool bShow )
{
	SNtlEventTLUIIndicator_ShowHide data;
	data.uiTLIndicatorType = uiTLIndicatorType;
	data.pcFlashName = pcFlashName;
	data.vFocusOffPos = vFocusOffPos;
	data.bShow = bShow;

	SEND_MSG( g_EventTLUIIndicator_ShowHide, &data );
}

void CNtlSLEventGenerator::TLMouseInputLockUnlock( RwUInt32 uiMouseInputType, RwBool bLock )
{
	SNtlEventTLMouseInputLockUnlock data;
	data.uiMouseInputType = uiMouseInputType;
	data.bLock = bLock;

	SEND_MSG( g_EventTLMouseInputLockUnlock, &data );
}

void CNtlSLEventGenerator::TLKeyboardInputLockUnlock( RwUInt32 uiKeyboardInputType, RwBool bLock )
{
	SNtlEventTLKeyboardInputLockUnlock data;
	data.uiKeyboardInputType = uiKeyboardInputType;
	data.bLock = bLock;

	SEND_MSG( g_EventTLKeyboardInputLockUnlock, &data );
}

void CNtlSLEventGenerator::TLActionMapBackup( void )
{
	SNtlEventTLActionMapBackup data;

	SEND_MSG( g_EventTLActionMapBackup, &data );
}

void CNtlSLEventGenerator::TLActionMapRestore( void )
{
	SNtlEventTLActionMapRestore data;

	SEND_MSG( g_EventTLActionMapRestore, &data );
}

void CNtlSLEventGenerator::TLExcuteQuest( RwUInt16 tQuestID )
{
	SNtlEventTLExcuteQuest data;
	data.tQuestID = tQuestID;

	SEND_MSG( g_EventTLExcuteQuest, &data );
}

void CNtlSLEventGenerator::TLAttackLockUnlock( RwUInt32 uiAttackType, RwBool bLock )
{
	SNtlEventTLAttackLockUnlock data;
	data.uiAttackType = uiAttackType;
	data.bLock = bLock;

	SEND_MSG( g_EventTLAttackLockUnlock, &data );
}

void CNtlSLEventGenerator::TLPlayCamera_TBLIDX( RwUInt32 uiCameraType, RwUInt32 uiTblIdx )
{
	SNtlEventTLPlayCamera data;
	data.uiCameraType = uiCameraType;
	data.uiTblIdx = uiTblIdx;

	SEND_MSG( g_EventTLPlayCamera, &data );
}

void CNtlSLEventGenerator::TLMinimap( RwBool bShow )
{
	SNtlEventTLMinimap data;
	data.bShow = bShow;

	SEND_MSG( g_EventTLMinimap, &data );
}

void CNtlSLEventGenerator::TLNotifyLockUnlock( RwUInt8 byLockType, RwUInt8 byIndex, RwBool bLock )
{
	SNtlEventTLLockUnlcok data;
	data.byLockType = byLockType;
	data.byIndex = byIndex;
	data.bLock		= bLock;

	SEND_MSG( g_EventTLNotifyLockUnlock, &data );
}

void CNtlSLEventGenerator::TLPlayCamera_POS( RwUInt32 uiCameraType, RwV3d& vTargetPos )
{
	SNtlEventTLPlayCamera data;
	data.uiCameraType = uiCameraType;
	data.vTargetPos = vTargetPos;

	SEND_MSG( g_EventTLPlayCamera, &data );
}

void CNtlSLEventGenerator::TLCondition( RwUInt32 uiCondType, RwUInt32 uiParam1 /*= 0xffffffff*/ )
{
	SNtlEventTLCondition data;
	data.uiCondType = uiCondType;
	data.uiParam1 = uiParam1;

	SEND_MSG( g_EventTLCondition, &data );
}

void CNtlSLEventGenerator::SobCondConfused(RwBool bAffect, SERIAL_HANDLE hSubject, SERIAL_HANDLE hTargetSerialId)
{
	SNtlEventCondConfused data;
	data.bAffect = bAffect;
	data.hSubject = hSubject;
	data.hTargetSerialId = hTargetSerialId;

	SEND_MSG(g_EventSobCondConfused, &data);
}

void CNtlSLEventGenerator::SobCondTerror(RwBool bAffect, SERIAL_HANDLE hSubject, SERIAL_HANDLE hAttackerSerialId)
{
	SNtlEventCondTerror data;
	data.bAffect = bAffect;
	data.hSubject = hSubject;
	data.hAttackerSerialId = hAttackerSerialId;    

	SEND_MSG(g_EventSobCondTerror, &data);
}

void CNtlSLEventGenerator::TLDisableMinimap()
{
	SEND_MSG(g_EventTLDisableMinimap, NULL);
}

void CNtlSLEventGenerator::TLDisableDialogMoveOption(RwBool bDiableDialogMoveOption)
{
	SNtlEventTLDisableDialogMoveOption data;
	data.bDiableDialogMoveOption = bDiableDialogMoveOption;

	SEND_MSG(g_EventTLDisableDialogMoveOption, &data);
}

void CNtlSLEventGenerator::TLEnableSkip(RwBool bEnableSkip)
{
	SNtlEventTLEnableSkip data;
	data.bEnableSkip = bEnableSkip;

	SEND_MSG(g_EventTLEnableSkip, &data);
}

void CNtlSLEventGenerator::MinorMatchStateUpdate( RwUInt8 byMatchState, RwUInt8 byStage, RwUInt32 dwRemainTime, RwBool bEnterState )
{
	SNtlEventMinorMatchStateUpdate data;
	data.byMatchState = byMatchState;
	data.byStage = byStage;
	data.dwRemainTime = dwRemainTime;
	data.bEnterState = bEnterState;

	SEND_MSG( g_EventMinorMatchStateUpdate, &data );
}

void CNtlSLEventGenerator::MinorMatchTeamInfo( RwUInt8 byMatchIndex, RwUInt8 byTeamCount, RwUInt16 wTeamInfo_var, sVARIABLE_DATA* pData )
{
	SNtlEventMinorMatchTeamInfo data;
	data.byMatchIndex = byMatchIndex;
	data.byTeamCount = byTeamCount;
	data.wTeamInfo_var = wTeamInfo_var;
	data.pData = pData;

	SEND_MSG( g_EventMinorMatchTeamInfo, &data );
}

void CNtlSLEventGenerator::MinorMatchPlayerState( RwUInt32 hPlayer, RwUInt8 byPcState )
{
	SNtlEventMinorMatchPlayerState data;
	data.hPlayer = hPlayer;
	data.byPcState = byPcState;

	SEND_MSG( g_EventMinorMatchPlayerState, &data );
}

void CNtlSLEventGenerator::MinorMatchUpdateScore( RwUInt16 wTeamType, RwUInt8 byScore, RwUInt32 hSlayer, RwUInt32 hFainter )
{
	SNtlEventMinorMatchUpdateScore data;
	data.wTeamType = wTeamType;
	data.byScore = byScore;
	data.hSlayer = hSlayer;
	data.hFainter = hFainter;

	SEND_MSG( g_EventMinorMatchUpdateScore, &data );
}

void CNtlSLEventGenerator::MinorMatchTeamScore( RwUInt8 byCount, sMATCH_MINORMATCH_TEAM_SCORE* pTeamScore )
{
	SNtlEventMinorMatchTeamScore data;
	data.byCount = byCount;
	memcpy( data.aTeamScore, pTeamScore, sizeof( sMATCH_MINORMATCH_TEAM_SCORE ) * byCount );

	SEND_MSG( g_EventMinorMatchTeamScore, &data );
}

void CNtlSLEventGenerator::MinorMatchSelection( RwUInt16 wWinnerTeam, RwUInt8 byTeamCount, sMINORMATCH_SELECTION_DATA* pSelection )
{
	SNtlEventMinorMatchSelection data;
	data.wWinnerTeam = wWinnerTeam;
	data.byTeamCount = byTeamCount;
	memcpy( data.aSelection, pSelection, sizeof( sMINORMATCH_SELECTION_DATA ) * byTeamCount );

	SEND_MSG( g_EventMinorMatchSelection, &data );
}

void CNtlSLEventGenerator::MinorMatchStageFinish( RwUInt8 byStageResult, RwUInt16 wStageWinner )
{
	SNtlEventMinorMatchStageFinish data;
	data.byStageResult = byStageResult;
	data.wStageWinner = wStageWinner;

	SEND_MSG( g_EventMinorMatchStageFinish, &data );
}

void CNtlSLEventGenerator::MinorMatchMatchFinish( RwUInt8 byMatchResult, RwUInt16 wMatchWinner, RwUInt8 byKillCount )
{
	SNtlEventMinorMatchMatchFinish data;
	data.byMatchResult = byMatchResult;
	data.wMatchWinner = wMatchWinner;
	data.byKillCount = byKillCount;

	SEND_MSG( g_EventMinorMatchMatchFinish, &data );
}

void CNtlSLEventGenerator::MajorMatchStateUpdate( RwUInt8 byMatchState, RwUInt8 byStage, RwUInt32 dwRemainTime, RwBool bEnterState )
{
	SNtlEventMajorMatchStateUpdate data;
	data.byMatchState = byMatchState;
	data.byStage = byStage;
	data.dwRemainTime = dwRemainTime;
	data.bEnterState = bEnterState;

	SEND_MSG( g_EventMajorMatchStateUpdate, &data );
}

void CNtlSLEventGenerator::MajorMatchTeamInfo(RwUInt8 byMatchDepth, RwUInt8 byMatchIndex, RwUInt8 byTeamCount, RwUInt16 wTeamInfo_var, sVARIABLE_DATA* pData )
{
	SNtlEventMajorMatchTeamInfo data;
	data.byMatchDepth = byMatchDepth;
	data.byMatchIndex = byMatchIndex;
	data.byTeamCount = byTeamCount;
	data.wTeamInfo_var = wTeamInfo_var;
	data.pData = pData;

	SEND_MSG( g_EventMajorMatchTeamInfo, &data );
}

void CNtlSLEventGenerator::MajorMatchPlayerState(RwUInt32 hPlayer, RwUInt8 byPcState)
{
	SNtlEventMajorMatchPlayerState data;
	data.hPlayer = hPlayer;
	data.byPcState = byPcState;

	SEND_MSG( g_EventMajorMatchPlayerState, &data );
}

void CNtlSLEventGenerator::MajorMatchUpdateScore(sMATCH_SCORE* pStageScore)
{
	SNtlEventMajorMatchUpdateScore data;
	data.pStageScore = pStageScore;

	SEND_MSG( g_EventMajorMatchUpdateScore, &data );
}

void CNtlSLEventGenerator::MajorMatchStageFinish( sMATCH_SCORE* pStageScore, RwUInt16 wStageWinner, RwUInt8 byStageResult, bool bIsObserver )
{
	SNtlEventMajorMatchStageFinish data;
	data.pStageScore = pStageScore;
	data.wStageWinner = wStageWinner;
	data.byStageResult = byStageResult;
	data.bIsObserver = bIsObserver;

	SEND_MSG( g_EventMajorMatchStageFinish, &data );
}

void CNtlSLEventGenerator::MajorMatchMatchFinish( sMATCH_SCORE* pStageScore, RwUInt16 wMatchWinner, RwUInt8 byMatchResult )
{
	SNtlEventMajorMatchMatchFinish data;
	data.pStageScore = pStageScore;
	data.wMatchWinner = wMatchWinner;
	data.byMatchResult = byMatchResult;

	SEND_MSG( g_EventMajorMatchMatchFinish, &data );
}

void CNtlSLEventGenerator::FinalMatchStateUpdate( RwUInt8 byMatchState, RwUInt8 byStage, RwUInt32 dwRemainTime, RwBool bEnterState )
{
	SNtlEventFinalMatchStateUpdate data;
	data.byMatchState = byMatchState;
	data.byStage = byStage;
	data.dwRemainTime = dwRemainTime;
	data.bEnterState = bEnterState;

	SEND_MSG( g_EventFinalMatchStateUpdate, &data );
}

void CNtlSLEventGenerator::FinalMatchTeamInfo(RwUInt8 byMatchDepth, RwUInt8 byMatchIndex, RwUInt8 byTeamCount, RwUInt16 wTeamInfo_var, sVARIABLE_DATA* pData )
{
	SNtlEventFinalMatchTeamInfo data;
	data.byMatchDepth = byMatchDepth;
	data.byMatchIndex = byMatchIndex;
	data.byTeamCount = byTeamCount;
	data.wTeamInfo_var = wTeamInfo_var;
	data.pData = pData;

	SEND_MSG( g_EventFinalMatchTeamInfo, &data );
}

void CNtlSLEventGenerator::FinalMatchPlayerState(RwUInt32 hPlayer, RwUInt8 byPcState)
{
	SNtlEventFinalMatchPlayerState data;
	data.hPlayer = hPlayer;
	data.byPcState = byPcState;

	SEND_MSG( g_EventFinalMatchPlayerState, &data );
}

void CNtlSLEventGenerator::FinalMatchUpdateScore(sMATCH_SCORE* pStageScore)
{
	SNtlEventFinalMatchUpdateScore data;
	data.pStageScore = pStageScore;

	SEND_MSG( g_EventFinalMatchUpdateScore, &data );
}

void CNtlSLEventGenerator::FinalMatchStageFinish( sMATCH_SCORE* pStageScore, RwUInt16 wStageWinner, RwUInt8 byStageResult, bool bIsObserver )
{
	SNtlEventFinalMatchStageFinish data;
	data.pStageScore = pStageScore;
	data.wStageWinner = wStageWinner;
	data.byStageResult = byStageResult;
	data.bIsObserver = bIsObserver;

	SEND_MSG( g_EventFinalMatchStageFinish, &data );
}

void CNtlSLEventGenerator::FinalMatchMatchFinish( sMATCH_SCORE* pStageScore, RwUInt16 wMatchWinner, RwUInt8 byMatchResult )
{
	SNtlEventFinalMatchMatchFinish data;
	data.pStageScore = pStageScore;
	data.wMatchWinner = wMatchWinner;
	data.byMatchResult = byMatchResult;

	SEND_MSG( g_EventFinalMatchMatchFinish, &data );
}

void CNtlSLEventGenerator::AwardInfo( RwUInt16* paTeamNameIndex, sVARIABLE_DATA* pData )
{
	SNtlEventAwardInfo data;
	data.paTeamNameIndex = paTeamNameIndex;
	data.pData = pData;

	SEND_MSG( g_EventAwardInfo, &data );
}

void CNtlSLEventGenerator::BudokaiStateInfoNfy( RwUInt16 wSeasonCount, 
											   BUDOKAITIME tmDefaultOpen, sBUDOKAI_UPDATE_STATE_INFO* pStateInfo, 
											   sBUDOKAI_UPDATE_MATCH_STATE_INFO* pMatchStateInfo, sBUDOKAI_JOIN_INFO* pJoinInfo )
{
	sNtlEventBudokaiStateInfoNfy data;
	::ZeroMemory( &data, sizeof(sNtlEventBudokaiStateInfoNfy) );
	data.wSeasonCount = wSeasonCount;
	data.tmDefaultOpenTime = tmDefaultOpen;
	::CopyMemory(&data.sStateInfo, pStateInfo, sizeof(sBUDOKAI_UPDATE_STATE_INFO));
	::CopyMemory(&data.aMatchStateInfo, pMatchStateInfo, sizeof(sBUDOKAI_UPDATE_MATCH_STATE_INFO)*MAX_BUDOKAI_MATCH_TYPE);
	::CopyMemory(&data.sJoinInfo, pJoinInfo, sizeof(sBUDOKAI_JOIN_INFO) );

	SEND_MSG(g_EventBudokaiStateInfoNfy, &data);
}

void CNtlSLEventGenerator::BudokaiUpdateStateNfy( sBUDOKAI_UPDATE_STATE_INFO* pStateInfo )
{
	sNtlEventBudokaiUpdateStateNfy data;
	::ZeroMemory( &data, sizeof( sNtlEventBudokaiUpdateStateNfy ) );
	::CopyMemory(&data.sStateInfo, pStateInfo, sizeof(sBUDOKAI_UPDATE_STATE_INFO) );

	SEND_MSG(g_EventBudokaiUpdateStateNfy, &data);
}

void CNtlSLEventGenerator::BudokaiUpdateMatchStateNfy( RwUInt8 byMatchType, sBUDOKAI_UPDATE_MATCH_STATE_INFO* pStateInfo )
{
	sNtlEventBudokaiUpdateMatchStateNfy data;
	::ZeroMemory( &data, sizeof( sNtlEventBudokaiUpdateMatchStateNfy ) );
	data.byMatchType = byMatchType;
	::CopyMemory(&data.sStateInfo, pStateInfo, sizeof(sBUDOKAI_UPDATE_MATCH_STATE_INFO) );

	SEND_MSG( g_EventBudokaiUpdateMatchStateNfy, &data );
}

void CNtlSLEventGenerator::BudokaiJoinIndividualRes( RwUInt16 wResultCode, RwUInt16 wJoinId, RwBool bDojoRecommender )
{
	SNtlEventBudokaiJoinIndividualRes data;
	data.wResultCode = wResultCode;
	data.wJoinId = wJoinId;
	data.bDojoRecommender = bDojoRecommender;

	SEND_MSG( g_EventBudokaiJoinIndividualRes, &data );
}

void CNtlSLEventGenerator::BudokaiJoinTeamInfoRes( RwUInt16 wResultCode, RwUInt8 byMemberCount, sBUDOKAI_TEAM_POINT_INFO* pPointInfo )
{
	SNtlEventBudokaiJoinTeamInfoRes data;
	data.wResultCode = wResultCode;
	data.byMemberCount = byMemberCount;
	::CopyMemory( &data.asPointInfo, pPointInfo, sizeof( sBUDOKAI_TEAM_POINT_INFO ) * byMemberCount );

	SEND_MSG( g_EventBudokaiJoinTeamInfoRes, &data );
}

void CNtlSLEventGenerator::BudokaiLeaveIndividualRes( RwUInt16 wResultCode )
{
	SNtlEventBudokaiLeaveIndividualRes data;
	data.wResultCode = wResultCode;

	SEND_MSG( g_EventBudokaiLeaveIndividualRes, &data );
}

void CNtlSLEventGenerator::BudokaiJoinTeamRes( RwUInt16 wResultCode, RwUInt32 hTroublePC,
											  RwReal fTotalRankPoint, RwUInt16 wJoinId, WCHAR* pTeamName,
											  RwUInt8 byMemberCount, sBUDOKAI_TEAM_POINT_INFO* pPointInfo )
{
	SNtlEventBudokaiJoinTeamRes data;
	::ZeroMemory( &data, sizeof( SNtlEventBudokaiJoinTeamRes ) );
	data.wResultCode = wResultCode;
	data.hTroublePc = hTroublePC;
	data.fTotalRankPoint = fTotalRankPoint;
	data.wJoinId = wJoinId;
	::CopyMemory( &data.wszTeamName, pTeamName, sizeof(WCHAR) * (NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE+1) );
	data.byMemberCount = byMemberCount;
	::CopyMemory( &data.aTeamInfo, pPointInfo, sizeof( sBUDOKAI_TEAM_POINT_INFO ) * byMemberCount );


	SEND_MSG( g_EventBudokaiJoinTeamRes, &data );
}

void CNtlSLEventGenerator::BudokaiJoinTeamNfy( RwReal fTotalRankPoint, RwUInt16 wJoinId, WCHAR* pTeamName , RwUInt8 byMemberCount, sBUDOKAI_TEAM_POINT_INFO* pPointInfo )
{
	SNtlEventBudokaiJoinTeamNfy data;
	::ZeroMemory( &data, sizeof( SNtlEventBudokaiJoinTeamNfy ) );
	data.fTotalRankPoint = fTotalRankPoint;
	data.wJoinId = wJoinId;
	::CopyMemory( &data.wszTeamName, pTeamName, sizeof(WCHAR) * (NTL_MAX_LENGTH_BUDOKAI_TEAM_NAME_IN_UNICODE+1) );
	data.byMemberCount = byMemberCount;
	::CopyMemory( &data.aTeamInfo, pPointInfo, sizeof( sBUDOKAI_TEAM_POINT_INFO ) * byMemberCount );

	SEND_MSG( g_EventBudokaiJoinTeamNfy, &data );
}

void CNtlSLEventGenerator::BudokaiLeaveTeamRes( RwUInt16 wResultCode )
{
	SNtlEventBudokaiLeaveTeamRes data;
	data.wResultCode = wResultCode;

	SEND_MSG( g_EventBudokaiLeaveTeamRes, &data );
}

void CNtlSLEventGenerator::BudokaiLeaveTeamNfy( void )
{
	SEND_MSG( g_EventBudokaiLeaveTeamNfy, NULL );
}

void CNtlSLEventGenerator::BudokaiLeaveTeamMemberRes( RwUInt16 wResultCode )
{
	SNtlEventBudokaiLeaveTeamMemberRes data;
	data.wResultCode = wResultCode;

	SEND_MSG( g_EventBudokaiLeaveTeamMemberRes, &data );
}

void CNtlSLEventGenerator::BudokaiLeaveTeamMemberNfy( WCHAR* pwcMemberName )
{
	SNtlEventBudokaiLeaveTeamMemberNfy data;
	data.pwcMemberName = pwcMemberName;

	SEND_MSG( g_EventBudokaiLeaveTeamMemberNfy, &data );
}

void CNtlSLEventGenerator::BudokaiJoinInfoRes( RwUInt8 wResultCode, RwUInt8 byMatchType, sBUDOKAI_REGISTER_INDIVIDUAL_INFO* pIndividualInfo, sBUDOKAI_REGISTER_TEAM_INFO* pTeamInfo )
{
	SNtlEventBudokaiJoinInfoRes data;
	data.wResultCode = wResultCode;
	data.byMatchType = byMatchType;

	if( byMatchType == BUDOKAI_MATCH_TYPE_INDIVIDIAUL )
	{
		::CopyMemory( &data.sIndividualInfo, pIndividualInfo, sizeof( sBUDOKAI_REGISTER_INDIVIDUAL_INFO ) );
	}
	else if( byMatchType == BUDOKAI_MATCH_TYPE_TEAM )
	{
		::CopyMemory( &data.sTeamInfo, pTeamInfo, sizeof( sBUDOKAI_REGISTER_TEAM_INFO ) );
	}
}

void CNtlSLEventGenerator::BudokaiMudosaInfoRes( RwUInt16 wResultCode, RwUInt8 byMudosaCount, sBUDOKAI_MUDOSA_INFO* paMudosaInfo)
{
	SNtlEventBudokaiMudosaInfoRes data;
	data.wResultCode = wResultCode;
	data.byMudosaCount = byMudosaCount;

	memset( data.aMudosaInfo, 0, sizeof( sBUDOKAI_MUDOSA_INFO ) * BUDOKAI_MAX_MUDOSA_INFO_COUNT );

	for ( int i = 0; i < byMudosaCount; ++i )
	{
		memcpy( &data.aMudosaInfo[i], &paMudosaInfo[i], sizeof( sBUDOKAI_MUDOSA_INFO ) );
	}

	SEND_MSG( g_EventBudokaiMudosaInfoRes, &data );
}

void CNtlSLEventGenerator::BotHelpMe_Nfy( SERIAL_HANDLE hSerialId, RwBool bisRequester ) 
{
	SNtlEventBotHelpMe_Nfy data;
	data.hSerialId = hSerialId;
	data.bIsRequester = bisRequester;

	SEND_MSG(g_EventBotHelpMe_Nfy, &data);
}

void CNtlSLEventGenerator::ActionScreenShot( const std::string& strFileName ) 
{
	SNtlEventScreenShot data;
	data.strFileName = strFileName;

	SEND_MSG(g_EventScreenShot, &data);
}

void CNtlSLEventGenerator::ActionSkillSelectMode(bool bSelect, SERIAL_HANDLE hSerialId)
{
	CNtlSLGlobal::m_bSelectSkillMode = bSelect;

	SNtlEventSkillSelectMode data;
	data.bSelect = bSelect;

	if (bSelect)
	{
		CNtlSLGlobal::m_hSelectSkillModeHandle = hSerialId;
		data.hSerialId = hSerialId;
	}
	else
	{
		data.hSerialId = CNtlSLGlobal::m_hSelectSkillModeHandle;
		CNtlSLGlobal::m_hSelectSkillModeHandle = hSerialId;
	}

	SEND_MSG(g_EventSkillSelectMode, &data);
}

void CNtlSLEventGenerator::BudokaiJoinStateNfy( RwUInt8 byMatchType, RwUInt8 byJoinState, RwUInt8 byJoinResult )
{
	SNtlEventBudokaiJoinStateNfy data;
	data.byMatchType = byMatchType;
	data.byJoinState = byJoinState;
	data.byJoinResult = byJoinResult;

	SEND_MSG( g_EventBudokaiJoinStateNfy, &data );
}

void CNtlSLEventGenerator::BudokaiJoinStateRes( RwUInt16 wResultCode, RwUInt8 byMatchType, RwUInt8 byJoinState, RwUInt8 byJoinResult )
{
	SNtlEventBudokaiJoinStateRes data;
	data.wResultCode = wResultCode;
	data.byMatchType = byMatchType;
	data.byJoinState = byJoinState;
	data.byJoinResult = byJoinResult;

	SEND_MSG( g_EventBudokaiJoinStateRes, &data );
}

void CNtlSLEventGenerator::BudokaiNoticeNfy( RwUInt8 byNoticeType, RwUInt32 tblidxNotice, void* pData )
{
	SNtlEventBudokaiNoticeNfy sData;
	sData.byNoticeType = byNoticeType;
	sData.tblidxNotice = tblidxNotice;
	sData.pData = pData;

	SEND_MSG( g_EventBudokaiNoticeNfy, &sData );
}

void CNtlSLEventGenerator::CinematicGuiCommand(RwChar* szCommand, RwBool bShow)
{
	SNtlEventCinematicGuiCommand sData;
	sData.szCommand = szCommand;
	sData.bShow = bShow;

	SEND_MSG( g_EventCinematicGuiCommand, &sData );
}

void CNtlSLEventGenerator::DialogEvent( RwInt32 nType )
{
	SNtlEventDialogMove sData;
	sData.nType = nType;

	SEND_MSG( g_EventDialogMove, &sData );
}

void CNtlSLEventGenerator::GambleAniStart( SERIAL_HANDLE hGambleNpc )
{
	SNtlEventGambleAniStart sData;
	sData.hSerialId = hGambleNpc;

	SEND_MSG( g_EventGambleAniStart, &sData );

	// Test
//	GambleAniEnd( hGambleNpc );
}

void CNtlSLEventGenerator::GambleAniEnd( SERIAL_HANDLE hGambleNpc )
{
	SNtlEventGambleAniEnd sData;
	sData.hGambleNpc = hGambleNpc;

	SEND_MSG( g_EventGambleAniEnd, &sData );
}

void CNtlSLEventGenerator::GameServerChangeOut(void)
{
	SEND_MSG( g_EventGameServerChangeOut, NULL );
}

void CNtlSLEventGenerator::GameServerChangeIn(void)
{
	SEND_MSG( g_EventGameServerChangeIn, NULL );
}

void CNtlSLEventGenerator::GameServerChannelChanged(void)
{
	SEND_MSG( g_EventGameServerChannelChanged, NULL );
}

void CNtlSLEventGenerator::CameraObserver( EObserverCameraType eType, RwInt32 nIndex ) 
{
    SNtlEventCameraObserver data;
    data.eType = eType;
    data.nIndex = nIndex;

    SEND_MSG(g_EventCameraObserver, &data);
}

void CNtlSLEventGenerator::GameChatOption( RwUInt8 byData1, RwUInt8 byData2, RwUInt8 byData3 )
{
	SNtlEventGameChatOption data;
	data.byData1 = byData1;
	data.byData2 = byData2;
	data.byData3 = byData3;

	SEND_MSG( g_EventGameChatOption, &data );
}

void CNtlSLEventGenerator::TenkaichiMarking( SERIAL_HANDLE hSerialID, BYTE byType ) 
{
    SNtlEventTenkaichiMarking data;
    data.hSerialId = hSerialID;
    data.byType = byType;

    SEND_MSG(g_EventTenkaichiMarking, &data);
}

void CNtlSLEventGenerator::SobEventTitleEffect(SERIAL_HANDLE hSerialID, bool bCreate, const char * pchBone, const char * pchEffect)
{
	SNtlEventTitleEffect data;
	data.hSerialId = hSerialID;
	data.bCreate = bCreate;
	data.pchBone = pchBone;
	data.pchEffect = pchEffect;

	SEND_MSG(g_EventSobEventTitleEffect, &data);
}

void CNtlSLEventGenerator::BudokaiProgressMessageNfy( RwUInt8 byMsgId )
{
	SNtlEventBudokaiProgressMessageNfy data;
	data.byMsgId = byMsgId;

	SEND_MSG( g_EventBudokaiProgressMessageNfy, &data );
}

void CNtlSLEventGenerator::SobTargetMarkSelect( SERIAL_HANDLE hSerialId ) 
{
    SNtlEventSobTargetMarkSelect data;
    data.hSerialId = hSerialId;    

    Logic_SetTargetMarkingID(hSerialId);

    SEND_MSG(g_EventSobTargetMarkSelect, &data);
}

void CNtlSLEventGenerator::SobTargetMarkRelease( SERIAL_HANDLE hSerialId ) 
{
    SNtlEventSobTargetMarkRelease data;
    data.hSerialId = hSerialId;

    Logic_SetTargetMarkingID(INVALID_SERIAL_ID);

    SEND_MSG(g_EventSobTargetMarkRelease, &data);
}

void CNtlSLEventGenerator::SobPushingNfy(SERIAL_HANDLE hSerialId, BYTE byAttackResult, WORD wAttackResultValue, const RwV3d& vDestPos)
{
    SNtlEventSobPushingNfy data;
    data.hSerialId = hSerialId;
    data.byAttackResult = byAttackResult;
    data.wAttackResultValue = wAttackResultValue;
    data.vDestPos = vDestPos;

    SEND_MSG(g_EventSobPushingNfy, &data);
}

void CNtlSLEventGenerator::SobGuardCrush(SERIAL_HANDLE hSerialId)
{
    SNtlEventSobGuardCrush data;
    data.hSerialId = hSerialId;

    SEND_MSG(g_EventSobGuardCrush, &data);
}

void CNtlSLEventGenerator::SobMovePatternSync(SERIAL_HANDLE hSerialId, RwUInt8 bySyncUnitId)
{
    SNtlEventSobMovePatternSync data;
    data.hSerialId		= hSerialId;
	data.bySyncUnitId	= bySyncUnitId;

    SEND_MSG(g_EventSobMovePatternSync, &data);
}

void CNtlSLEventGenerator::SobAirJump(SERIAL_HANDLE hSerialId, RwUInt8 byMoveDirection, RwV3d& vDir)
{
	SNtlEventSobAirJump sSobMove;
	sSobMove.hSerialId = hSerialId;
	sSobMove.vDir = vDir;
	sSobMove.byMoveDirection = byMoveDirection;

	SEND_MSG(g_EventSobAirJump, &sSobMove);
}

void CNtlSLEventGenerator::SobAirAccel(SERIAL_HANDLE hSerialId, RwUInt8 byMoveFlag, RwUInt8 byMoveDirection, RwV3d& vLoc, RwV3d& vDir)
{
	SNtlEventSobAirAccel sSobMove;
	sSobMove.hSerialId = hSerialId;
	sSobMove.vLoc = vLoc;
	sSobMove.vDir = vDir;
	sSobMove.byMoveFlag = byMoveFlag;
	sSobMove.byMoveDirection = byMoveDirection;

	SEND_MSG(g_EventSobAirAccel, &sSobMove);
}

void CNtlSLEventGenerator::ChangeCharName(CHARACTERID charid, WCHAR* wchCharName, WCHAR* wchOldCharName)
{
	SNtlEventChangeCharName data;
	data.charid = charid;
	wcscpy_s(data.wchCharName, NTL_MAX_SIZE_CHAR_NAME + 1, wchCharName);
	wcscpy_s(data.wchOldCharName, NTL_MAX_SIZE_CHAR_NAME + 1, wchOldCharName);

	SEND_MSG(g_EventChangeCharName, &data);
}

void CNtlSLEventGenerator::ChangeNickName( SERIAL_HANDLE hSerialID, TBLIDX nicknameTblidx )
{
	SNtlEventChangeNickName data;
	data.hSerialID = hSerialID;
	data.nicknameTblidx = nicknameTblidx;

	SEND_MSG(g_EventChangeNickName, &data);
}

void CNtlSLEventGenerator::ChangeClassAuthorityChanged( RwBool bCanChangeClass )
{
	SNtlEventChangeClassAuthorityChanged data;
	data.bCanChangeClass = bCanChangeClass;

	SEND_MSG( g_EventChangeClassAuthorityChangedNfy, &data );
}

void CNtlSLEventGenerator::SobCondHidingKi( RwBool bAffect, SERIAL_HANDLE hSubject ) 
{
    SNtlEventCondHidingKi data;
    data.bAffect = bAffect;
    data.hSerialID = hSubject;

    SEND_MSG(g_EventSobCondHindingKi, &data);
}

void CNtlSLEventGenerator::DynamicObjectUpdateState( SERIAL_HANDLE hSerial, sDYNAMIC_OBJECT_BRIEF* pBrief, sDYNAMIC_OBJECT_STATE* pState )
{
	SNtlEventDynamicObjectUpdateState data;
	data.uiHandle = hSerial;
	data.pBrief = pBrief;
	data.pState = pState;

	SEND_MSG( g_EventDynamicObjectUpdateState, &data );
}

void CNtlSLEventGenerator::HoipoiMixRecipeReg( TBLIDX idxRecipeTbl )
{
	SNtlEventHoipoiMixRecipeRegNfy sData;
	sData.idxRecipeTbl = idxRecipeTbl;

	SEND_MSG( g_EventHoipoiMixRecipeRegNfy, &sData );

}

void CNtlSLEventGenerator::HoipoiMixItemRecipeInfo(RwUInt16 wCount, void* pRecipeData )
{
	SNtlEventHoipoiMixItemRecipeInfo sData;
	sData.wCount = wCount;
	sData.pRecipeData = pRecipeData;

	SEND_MSG( g_EventHoipoiMixItemRecipeInfo, &sData );
}

void CNtlSLEventGenerator::HoipoiMixItemMakeExp( RwUInt32 dwCurExp, RwUInt8 byCurLevel, DWORD dwExpGained)
{
	SNtlEventHoipoiMixItemMakeExpNfy sData;
	sData.dwCurExp = dwCurExp;
	sData.byCurLevel = byCurLevel;
	sData.dwExpGained = dwExpGained;

	SEND_MSG( g_EventHoipoiMixItemMakeExpNfy, &sData );
}

void CNtlSLEventGenerator::HoipoiMixInfoUpdate( RwUInt8 byType, RwUInt32 dwData )
{
	SNtlEventHoipoiMixInfoUpdate sData;
	sData.byType = byType;
	sData.dwData = dwData;

	SEND_MSG( g_EventHoipoiMixInfoUpdate, &sData );
}

void CNtlSLEventGenerator::SobChangeAdult( SERIAL_HANDLE hSerialId, RwBool bAdult ) 
{
    SNtlEventSobChangeAdult data;
    data.hSerialId = hSerialId;
    data.bAdult = bAdult;

    SEND_MSG(g_EventSobChangeAdult, &data);
}

void CNtlSLEventGenerator::EventSobCharTitleSelectNfy(HOBJECT hHandle, TBLIDX uiTitle)
{
	SDboEventCharTitleSelectNfy data;
	data.hSerialId = hHandle;
	data.uiTitle = uiTitle;

	SEND_MSG(g_EventCharTitleSelectNfy, &data);
}

void CNtlSLEventGenerator::EnableAir(bool bFlag)
{
	SNtlEventEnableAir sData;
	sData.bFlag = bFlag;

	SEND_MSG(g_EventEnableAir, &sData);
}

void CNtlSLEventGenerator::SobSetAp(int nAp, int nMaxAp)
{
	SNtlEventSetAp sData;
	sData.nAp = nAp;
	sData.nMaxAp = nMaxAp;

	SEND_MSG(g_EventSetAp, &sData);
}

void CNtlSLEventGenerator::SobTransformCandy( SERIAL_HANDLE hSerialId, RwBool bTransform ) 
{
    SNtlEventTransformCandy data;
    data.hSerialId = hSerialId;
    data.bTransform = bTransform;

    SEND_MSG(g_EventTransformCandy, &data);

}

void CNtlSLEventGenerator::CharRefreshBattleAttributeModifier( SERIAL_HANDLE hSerial, RwReal fHonestOffenceModifier, RwReal fHonestDefenceModifier, RwReal fStrangeOffenceModifier, RwReal fStrangeDefenceModifier, RwReal fWildOffenceModifier, RwReal fWildDefenceModifier, RwReal fEleganceOffenceModifier, RwReal fEleganceDefenceModifier, RwReal fFunnyOffenceModifier, RwReal fFunnyDefenceModifier )
{
	SNtlEventCharRefreshBattleAttributeModifier data;
	data.hSerialID = hSerial;
	data.fHonestOffenceModifier = fHonestOffenceModifier;
	data.fHonestDefenceModifier = fHonestDefenceModifier;
	data.fStrangeOffenceModifier = fStrangeOffenceModifier;
	data.fStrangeDefenceModifier = fStrangeDefenceModifier;
	data.fWildOffenceModifier = fWildOffenceModifier;
	data.fWildDefenceModifier = fWildDefenceModifier;
	data.fEleganceOffenceModifier = fEleganceOffenceModifier;
	data.fEleganceDefenceModifier = fEleganceDefenceModifier;
	data.fFunnyOffenceModifier = fFunnyOffenceModifier;
	data.fFunnyDefenceModifier = fFunnyDefenceModifier;

	SEND_MSG(g_EventCharRefreshBattleAttributeModifier, &data);
}

void CNtlSLEventGenerator::NPCCommuExit() 
{
    SEND_MSG(g_EventNPCCommuExit, NULL);
}

void CNtlSLEventGenerator::CharRankPointResetNfy( void )
{
	SEND_MSG(g_EventCharRankPointResetNfy, NULL );
}

void CNtlSLEventGenerator::SobTransformSequela(SERIAL_HANDLE hSerialId)
{
    SNtlEventSobTransfromSequela data;
    data.hSerialId = hSerialId;

    SEND_MSG(g_EventSobTransformSequela, &data);
}

void CNtlSLEventGenerator::SobCinematicScale(SERIAL_HANDLE hSerialId, float fScale, float fTime)
{
	SNtlEventSobCinematicScale data;
	data.hSerialId = hSerialId;
	data.fScale = fScale;
	data.fTime = fTime;

	SEND_MSG(g_EventCinematicScale, &data);
}

void CNtlSLEventGenerator::QuickSlotGuiMode( RwBool bExpand1, RwBool bExpand2 )
{
	SNtlEventQuickSlotGuiMode data;
	data.bExpand1 = bExpand1;
	data.bExpand2 = bExpand2;

	SEND_MSG( g_EventQuickSlotGuiMode, &data );
}

void CNtlSLEventGenerator::QuickSlotLockMode( RwBool bLock )
{
	SNtlEventQuickSlotLockMode data;
	data.bLock = bLock;

	SEND_MSG( g_EventQuickSlotLockMode, &data );
}

void CNtlSLEventGenerator::MobDie( RwUInt32 uiSerialID )
{
	SNtlEventMobDie data;
	data.m_hSerialID = uiSerialID;

	SEND_MSG( g_EventMobDie, &data );
}

void CNtlSLEventGenerator::ActionFuncSitAndStand() 
{
    SEND_MSG(g_EventActionFuncSitAndStand, NULL);
}
void CNtlSLEventGenerator::ActionFuncBasicAttack() 
{
    SEND_MSG(g_EventActionFuncBasicAttack, NULL);
}
void CNtlSLEventGenerator::ActionFuncAssist() 
{
    SEND_MSG(g_EventActionFuncAssist, NULL);
}
void CNtlSLEventGenerator::ActionFuncGetItem() 
{
    SEND_MSG(g_EventActionFuncGetItem, NULL);
}

void CNtlSLEventGenerator::ESCinNarrationMode()
{
	SEND_MSG(g_EventESCinNarrationMode, NULL);
}

void CNtlSLEventGenerator::SobUpdateLPStatusNfy( SERIAL_HANDLE hSerialId, RwBool bEmergency ) 
{
    SNtlEventSobUpdateLPStatusNfy data;
    data.hSerialId   = hSerialId;
    data.bEmergency  = bEmergency;

    SEND_MSG(g_EventSobUpdateLPStatusNfy, &data);
}

void CNtlSLEventGenerator::SobSendAttackEnd( SERIAL_HANDLE hSerialId )
{
    SNtlEventSobSendAttackEnd data;
    data.hSerialId = hSerialId;

    SEND_MSG(g_EventSobSendAttackEnd, &data);
}