#include "precomp_ntlpresentation.h"
#include "NtlPLEventGenerator.h"
#include "NtlDebug.h"
#include "NtlEventQueue.h"
#include "NtlPLEvent.h"

// 이벤트 Send를 간편히 사용하기 위한 매크로 (by agebrak 2007.10.29)
#define SEND_MSG(event, data)   \
    RWS::CMsg msg; \
    msg.Id = event;\
    msg.pData = reinterpret_cast<void*>(data);\
    _SendMsg(msg);

void CNtlPLEventGenerator::AnimEnd(RwUInt32 uiSerialId, RwUInt32 uiBaseAnimKey, RwUInt32 uiUpperAnimKey)
{
	SNtlEventAnimEnd sAnimEnd;
	sAnimEnd.uiSerialId		= uiSerialId;
	sAnimEnd.uiBaseAnimKey	= uiBaseAnimKey;
	sAnimEnd.uiUpperAnimKey	= uiUpperAnimKey;
    SEND_MSG(g_EventAnimEnd, &sAnimEnd);
}

void CNtlPLEventGenerator::AnimHit(RwUInt32 uiSerialId, void *pData)
{
	SNtlEventAnimHit sAnimHit;
	sAnimHit.uiSerialId = uiSerialId;
	sAnimHit.pData      = pData;
    SEND_MSG(g_EventAnimHit, &sAnimHit);
}

void CNtlPLEventGenerator::AnimFootStep(RwUInt32 uiSerialId, void* pData)
{
	SNtlEventFootStep sAnimFootStep;
	sAnimFootStep.uiSerialId = uiSerialId;
    sAnimFootStep.pData = pData;
    SEND_MSG(g_EventAnimFootStep, &sAnimFootStep);
}

void CNtlPLEventGenerator::AnimWeightTime(RwUInt32 uiSeiralId, void *pData)
{
	SNtlEventWeightTime sAnimWeightTime;
	sAnimWeightTime.uiSerialId = uiSeiralId;
	sAnimWeightTime.pData      = pData;
    SEND_MSG(g_EventWeightTime, &sAnimWeightTime);
}

void CNtlPLEventGenerator::AnimEventSubWeaponActive(RwUInt32 uiSerialId)
{
	SNtlEventSubWeaponActive sAnimSubWeaponActive;
	sAnimSubWeaponActive.uiSerialId = uiSerialId;
    SEND_MSG(g_EventSubWeaponActive, &sAnimSubWeaponActive);
}

void CNtlPLEventGenerator::AnimEventSubWeaponDeActive(RwUInt32 uiSerialId)
{
	SNtlEventSubWeaponDeActive sAnimSubWeaponDeActive;
	sAnimSubWeaponDeActive.uiSerialId = uiSerialId;
    SEND_MSG(g_EventSubWeaponDeActive, &sAnimSubWeaponDeActive);
}

void CNtlPLEventGenerator::AnimEventSummonActive(RwUInt32 uiSerialId)
{
	SNtlEventSummon sAnimSummon;
	sAnimSummon.uiSerialId = uiSerialId;
    SEND_MSG(g_EventSummon, &sAnimSummon);
}

void CNtlPLEventGenerator::AnimEventAlpha( RwUInt32 uiSerialId, void* pData ) 
{
    SNtlEventAlpha sAnimAlpha;
    sAnimAlpha.uiSerialId = uiSerialId;
    sAnimAlpha.pData = pData;
    SEND_MSG(g_EventAlpha, &sAnimAlpha);
}

void CNtlPLEventGenerator::AnimEventTMQ( RwUInt32 uiSerialID, RwBool bIn ) 
{
    SNtlPLEventTMQ data;
    data.uiSeiralID = uiSerialID;
    data.bIn = bIn;
    SEND_MSG(g_EventAnimTMQ, &data);
}

void CNtlPLEventGenerator::AnimEventExplosion(RwUInt32 uiSerialID, void* pData)
{
    SNtlPlEventExplosion data;
    data.uiSerialID = uiSerialID;
    data.pData = pData;
    SEND_MSG(g_EventExplosion, &data);
}

void CNtlPLEventGenerator::AnimEventPostEffect( RwUInt32 uiSerialID, void* pData ) 
{
    SNtlEventPostEffect data;
    data.uiSerialID = uiSerialID;
    data.pData = pData;
    SEND_MSG(g_EventAnimPostEffect, &data);
}

void CNtlPLEventGenerator::AnimEventDirect( RwUInt32 uiSerialID, void* pData ) 
{
    SNtlEventDirect data;
    data.uiSerialID = uiSerialID;
    data.pData = pData;
    SEND_MSG(g_EventAnimDirect, &data);
}

void CNtlPLEventGenerator::AnimEventColorChange( RwUInt32 uiSerialID, void* pData ) 
{
    SNtlEventColorChange data;
    data.uiSerialID = uiSerialID;
    data.pData = pData;
    SEND_MSG(g_EventAnimColorChange, &data);
}

void CNtlPLEventGenerator::AnimEventStretch( RwUInt32 uiSerialID, void* pData ) 
{   
    SNtlEventStretch data;
    data.uiSerialID = uiSerialID;
    data.pData = pData;
    SEND_MSG(g_EventAnimStretch, &data);
}

void CNtlPLEventGenerator::AnimEventTrigger(RwUInt32 uiSerialID, void* pData)
{
    SNtlEventTrigger data;
    data.uiSerialID = uiSerialID;
    data.pData = pData;
    SEND_MSG(g_EventAnimTrigger, &data);
}

void CNtlPLEventGenerator::AnimEventSkillCancel(RwUInt32 uiSerialID)
{
	SNtlEventAnimSkillCancel data;
    data.uiSerialID = uiSerialID;
    SEND_MSG(g_EventAnimSkillCancel, &data);
}

// world field switching effects
void CNtlPLEventGenerator::IsAnotherFieldChanged(void* pData)
{
	RWS::CMsg msg;
	msg.Id = NPEI_IS_ANOTHER_FIELD_CHANGED;
	msg.pData = pData;
	_SendMsg(msg);
}


void CNtlPLEventGenerator::IsAnotherFieldChangedWithoutDelay(void* pData)
{
	RWS::CMsg msg;
	msg.Id		= NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY;
	msg.pData	= pData;
	_SendMsg(msg);
}

void CNtlPLEventGenerator::IsAnotherBlockChanged(void* pData)
{
	RWS::CMsg msg;
	msg.Id		= NPEI_IS_ANOTHER_BLOCK_CHANGED;
	msg.pData	= pData;
	_SendMsg(msg);
}

void CNtlPLEventGenerator::IsAnotherBlockChangedWithoutDelay(void* pData)
{
	RWS::CMsg msg;
	msg.Id		= NPEI_IS_ANOTHER_BLOCK_CHANGED_WITHOUT_DELAY;
	msg.pData	= pData;
	_SendMsg(msg);
}

void CNtlPLEventGenerator::IsAnotherObjectChanged(void* pData)
{
	RWS::CMsg msg;
	msg.Id		= NPEI_IS_ANOTHER_OBJECT_CHANGED;
	msg.pData	= pData;
	_SendMsg(msg);
}

void CNtlPLEventGenerator::IsAnotherObjectChangedWithoutDelay(void* pData)
{
	RWS::CMsg msg;
	msg.Id		= NPEI_IS_ANOTHER_OBJECT_CHANGED_WITHOUT_DELAY;
	msg.pData	= pData;
	_SendMsg(msg);
}

void CNtlPLEventGenerator::IsAnotherSectorChangedWithoutDelay(void* pData)
{
	RWS::CMsg msg;
	msg.Id		= NPEI_IS_ANOTHER_SECTOR_CHANGED_WITHOUT_DELAY;
	msg.pData	= pData;
	_SendMsg(msg);
}

void CNtlPLEventGenerator::OnMapNameChange(void* pData)
{
	RWS::CMsg msg;
	msg.Id		= NPEI_IS_MAPNAME_CHANGE;
	msg.pData	= pData;
	_SendMsg(msg);
}

void CNtlPLEventGenerator::OnTriDoodadsEvent(void* pData)
{
	RWS::CMsg msg;

	msg.Id		= g_EventTriDoodads;
	msg.pData	= pData;

	_SendMsg(msg);
}

void CNtlPLEventGenerator::CreateEventObject(RwBool bCreate, void* pData)
{
    SNtlPLEventEventObject data;
    data.bCreate = bCreate;
    data.pData = pData;

    RWS::CMsg msg;
    msg.Id = g_EventCreateEventObject;
    msg.pData = &data;

    _SendMsg(msg);
}

void CNtlPLEventGenerator::CreateEventThreadLoading( RwUInt32 uiSerialID ) 
{
    SNtlPlThreadLoadingComplete data;
    data.uiSerialID = uiSerialID;

    SEND_MSG(g_EventThreadLoadingComplete, &data);
}

void CNtlPLEventGenerator::Resize(RwInt32 iWidht, RwInt32 iHeight, RwBool bRestore_from_Minimize)
{
	SNtlPLEventResize sPacket;
	sPacket.iWidht					= iWidht;
	sPacket.iHeight					= iHeight;
	sPacket.bRestore_from_Minimize	= bRestore_from_Minimize;

	RWS::CMsg msg;
	msg.Id = g_EventResize;
	msg.pData = reinterpret_cast<void*>(&sPacket);
	_SendMsg(msg);
}

void CNtlPLEventGenerator::CinematicViewShow(RwBool bShow)
{
	SNtlEventCinematicViewShow data;
	data.bShow = bShow;

	RWS::CMsg msg;
	msg.Id = g_EventCinematicViewShow;
	msg.pData = reinterpret_cast<void*>(&data);
	_SendMsg(msg);	
}

void CNtlPLEventGenerator::CinematicBalloonShow(RwUInt32 hSerialId, RwUInt32 uiQuestTextTblId, RwUInt8 byBalloonType, RwUInt8 byEmotionType, RwUInt8 byPlayType)
{
	SNtlEventCinematicBalloon data;
	data.bShow				= TRUE;
	data.hSerialId			= hSerialId;
	data.uiQuestTextTblId	= uiQuestTextTblId;
	data.byBalloonType		= byBalloonType;
	data.byEmotionType		= byEmotionType;
	data.byPlayType			= byPlayType;

	RWS::CMsg msg;
	msg.Id = g_EventCinematicBalloon;
	msg.pData = reinterpret_cast<void*>(&data);
	_SendMsg(msg);	
}

void CNtlPLEventGenerator::CinematicBalloonHide(void)
{
	SNtlEventCinematicBalloon data;
	data.bShow			= FALSE;

	RWS::CMsg msg;
	msg.Id = g_EventCinematicBalloon;
	msg.pData = reinterpret_cast<void*>(&data);
	_SendMsg(msg);	
}

void CNtlPLEventGenerator::CinematicFlashShow( RwChar* pFilename, RwBool bUseTime, RwReal fLifeTime, void* pEchoData, 
											   RwReal fBeginAlphaTime /* = 0.0f */, RwReal fEndAlphaTime /* = 0.0f */, RwUInt32 flagPlayTimeType /* = 0  */)
{
	SNtlEventCinematicFlash data;
	data.bShow = TRUE;
	data.pFilename = pFilename;
	data.bUseTime = bUseTime;
	data.fLifeTime = fLifeTime;
	data.pEchoData = pEchoData;
	data.fBeginAlphaTime = fBeginAlphaTime;
	data.fEndAlphaTime = fEndAlphaTime;
	data.flagPlayTimeType = flagPlayTimeType;

	RWS::CMsg msg;
	msg.Id = g_EventCinematicFlash;
	msg.pData = reinterpret_cast<void*>(&data);
	_SendMsg(msg);
}

void CNtlPLEventGenerator::CinematicFlashHide(void)
{
	SNtlEventCinematicFlash data;
	data.bShow = FALSE;

	RWS::CMsg msg;
	msg.Id = g_EventCinematicFlash;
	msg.pData = reinterpret_cast<void*>(&data);
	_SendMsg(msg);
}

void CNtlPLEventGenerator::CinematicEcho(void* hCinematic)
{
	SNtlEventCinematicEcho data;
	data.hCinematic = hCinematic;

	RWS::CMsg msg;
	msg.Id = g_EventCinematicEcho;
	msg.pData = reinterpret_cast<void*>(&data);
	_SendMsg(msg);	
}

void CNtlPLEventGenerator::ResetCinematic(RwUInt32 eResetDemandObject)
{
	SNtlEventResetCinematic data;
	data.eDemandResetObject = eResetDemandObject;

	RWS::CMsg msg;
	msg.Id = g_EventResetCinematic;
	msg.pData = reinterpret_cast<void*>( &data );
	_SendMsg(msg);
}

void CNtlPLEventGenerator::CinematicFadeOut(RwUInt8 byType, RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwUInt8 byEndAlpha)
{
	SNtlEventCinematicFadeInOut data;
	data.byType			= byType;
	data.fLifeTime		= fLifeTime;
	data.byRed			= byRed;
	data.byGreen		= byGreen;
	data.byBlue			= byBlue;
	data.byEndAlpha		= byEndAlpha;

	SEND_MSG(g_EventCinematicFadeOut, &data);
}

void CNtlPLEventGenerator::CinematicFadeIn(RwUInt8 byType, RwReal fLifeTime, RwUInt8 byRed, RwUInt8 byGreen, RwUInt8 byBlue, RwUInt8 byEndAlpha)
{
	SNtlEventCinematicFadeInOut data;
	data.byType			= byType;
	data.fLifeTime		= fLifeTime;
	data.byRed			= byRed;
	data.byGreen		= byGreen;
	data.byBlue			= byBlue;
	data.byEndAlpha		= byEndAlpha;

	SEND_MSG(g_EventCinematicFadeIn, &data);
}

void CNtlPLEventGenerator::CinematicBreak(void)
{
	SEND_MSG(g_EventCinematicBreak, NULL);
}

void CNtlPLEventGenerator::ShowProfilerGui( RwBool bShow )
{
	SNtlEventShowProfilerGui data;
	data.bShow = bShow;

	SEND_MSG(g_EventShowProfilerGui, &data);
}

void CNtlPLEventGenerator::ShowSpectatorBalloon( RwV3d& vPos, WCHAR* szMessage, RwReal fLifeTime, RwReal fFadeInTime /* = 0.0f */, RwUInt8 byBalloonType /* = 0  */ )
{
	SNtlEventShowSpectatorBalloon data;
	data.vPos = vPos;
	data.szMessage = szMessage;
	data.fLifeTime = fLifeTime;
	data.fFadeInTime = fFadeInTime;
	data.byBalloonType = byBalloonType;

	SEND_MSG( g_EventShowSpectatorBalloon, &data );
}

VOID CNtlPLEventGenerator::HideAllSpectatorBalloon(void)
{
	SEND_MSG( g_EventHideAllSpectatorBalloon, NULL );
}

void CNtlPLEventGenerator::ShowPlayerName( RwUInt32 hSerialId, RwBool bShow )
{
	SNtlEventShowPlayerName data;
	data.hSerialId = hSerialId;
	data.bShow = bShow;

	SEND_MSG( g_EventShowPlayerName, &data );
}

void CNtlPLEventGenerator::EventDirectPlayCancel(RwUInt32 hSerialId)
{
	SEND_MSG(g_EventDirectPlayCancel, hSerialId);
}
