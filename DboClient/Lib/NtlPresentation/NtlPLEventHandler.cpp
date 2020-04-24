#include "precomp_ntlpresentation.h"
#include "NtlPLEventHandler.h"

// core
#include "NtlDebug.h"

// presentation
#include "NtlPLEvent.h"

CNtlPLEventHandler::CNtlPLEventHandler()
{
}

CNtlPLEventHandler::~CNtlPLEventHandler()
{
}


RwBool CNtlPLEventHandler::Create(void)
{
	NTL_FUNCTION("CNtlPLEventHandler::Create");
	
	RegisterMsg (g_EventAnimEnd, "g_EventAnimEndStr", "SNtlEventAnimEnd");
	RegisterMsg (g_EventAnimHit, "g_EventAnimHitStr", "SNtlEventAnimHit");
	RegisterMsg (g_EventAnimFootStep, "g_EventAnimFootStepStr", "SNtlEventAnimFootStep");
	RegisterMsg (g_EventWeightTime, "g_EventWeightTimeStr", "SNtlEventWeightTime");
	RegisterMsg (g_EventSubWeaponActive, "g_EventSubWeaponActiveStr", "SNtlEventSubWeaponActive");
	RegisterMsg (g_EventSubWeaponDeActive, "g_EventSubWeaponDeActiveStr", "SNtlEventSubWeaponDeActive");
	RegisterMsg (g_EventSummon, "g_EventSummonStr", "SNtlEventSummon");
    RegisterMsg (g_EventAlpha, "g_EventAlpha", "SNtlEventAlpha");
    RegisterMsg (g_EventAnimTMQ, "g_EventAnimTMQ", "SNtlPLEventTMQ");
    RegisterMsg (g_EventExplosion, "g_EventExplosion", "SNtlPLEventExplosion");
    RegisterMsg (g_EventAnimPostEffect, "g_EventAnimPostEffect", "SNtlEventPostEffect");
    RegisterMsg (g_EventAnimDirect, "g_EventAnimDirect", "SNtlEventAnimDirect");
    RegisterMsg (g_EventAnimColorChange, "g_EventAnimColorChange", "SNtlCololChangeEvent");
    RegisterMsg (g_EventAnimStretch, "g_EventAnimStretch", "SNtlEventStretch");
    RegisterMsg (g_EventAnimTrigger, "g_EventAnimTrigger", "SNtlEventTrigger");
	RegisterMsg (g_EventAnimSkillCancel, "g_EventAnimCancelPoint", "SNtlEventAnimCancelPoint");

	// world field switching effects
	RegisterMsg(NPEI_IS_ANOTHER_FIELD_CHANGED, "NPEI_IS_ANOTHER_FIELD_CHANGED", "sNTL_FIELD_PROP");
	RegisterMsg(NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY, "NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY", "sNTL_FIELD_PROP");
	RegisterMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED, "NPEI_IS_ANOTHER_BLOCK_CHANGED", "sNTL_FIELD_PROP");
	RegisterMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED_WITHOUT_DELAY, "NPEI_IS_ANOTHER_BLOCK_CHANGED_WITHOUT_DELAY", "sNTL_FIELD_PROP");
	RegisterMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED, "NPEI_IS_ANOTHER_OBJECT_CHANGED", "sNTL_FIELD_PROP");
	RegisterMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED_WITHOUT_DELAY, "NPEI_IS_ANOTHER_OBJECT_CHANGED_WITHOUT_DELAY", "sNTL_FIELD_PROP");
	RegisterMsg(NPEI_IS_MAPNAME_CHANGE, "NPEI_IS_MAPNAME_CHANGE", "sNTL_EVENT_MAPNAME");
	RegisterMsg(NPEI_IS_ANOTHER_SECTOR_CHANGED_WITHOUT_DELAY, "NPEI_IS_ANOTHER_SECTOR_CHANGED_WITHOUT_DELAY", "CNtlWorldSector");

	// trigger doodads
	RegisterMsg(g_EventTriDoodads, "g_EventTriDoodads", "sNTL_EVENT_TRI_DOODADS");

    // Event Object
    RegisterMsg(g_EventCreateEventObject, "g_EventCreateEventObject", "SNtlPLEventEventObject");
    RegisterMsg(g_EventThreadLoadingComplete, "g_EventThreadLoadingComplete", "SNtlPlThreadLoadingComplete");

	// gui event
	RegisterMsg(g_EventResize, "g_EventResizeStr", "g_EventResize");

	RegisterMsg(g_EventCinematicViewShow, "g_EventCinematicViewShow", "SNtlEventCinematicViewShow");
	RegisterMsg(g_EventCinematicBalloon, "g_EventCinematicBalloon", "SNtlEventCinematicBalloon");
	RegisterMsg(g_EventCinematicFlash, "g_EventCinematicFlash", "SNtlEventCinematicFlash");
	RegisterMsg(g_EventCinematicEcho, "g_EventCinematicEcho", "SNtlEventCinematicEcho");
	RegisterMsg(g_EventResetCinematic, "g_EventResetCinematic", " ");

	RegisterMsg(g_EventCinematicFadeOut, "g_EventCinematicFadeOut", "SNtlPLEventCinematicFadeOut");
	RegisterMsg(g_EventCinematicFadeIn, "g_EventCinematicFadeIn", "SNtlPLEventCinematicFadeIn");

	RegisterMsg(g_EventCinematicBreak, "g_EventCinematicBreak", "SNtlPLEventCinematicBreak");

	RegisterMsg( g_EventShowProfilerGui, "g_EventShowProfilerGui", "SNtlPLEventShowProfilerGui" );

	RegisterMsg(g_EventShowSpectatorBalloon, "g_EventShowSpectatorBalloon", "SNtlEventShowSpectatorBalloon" );
	RegisterMsg(g_EventHideAllSpectatorBalloon, "g_EventHideAllSpectatorBalloon", " " );

	RegisterMsg(g_EventShowPlayerName, "g_EventShowPlayerName", "SNtlEventShowPlayerName" );    

	RegisterMsg(g_EventDirectPlayCancel, "g_EventDirectPlayCancel", "");

	NTL_RETURN(TRUE);
}

void CNtlPLEventHandler::Destroy(void)
{
	NTL_FUNCTION("CNtlPLEventHandler::Destroy");
	
	UnRegisterMsg (g_EventAnimEnd);
	UnRegisterMsg (g_EventAnimHit);
	UnRegisterMsg (g_EventAnimFootStep);
	UnRegisterMsg (g_EventWeightTime);
	UnRegisterMsg (g_EventSubWeaponActive);
	UnRegisterMsg (g_EventSubWeaponDeActive);
	UnRegisterMsg (g_EventSummon);
    UnRegisterMsg (g_EventAlpha);
    UnRegisterMsg (g_EventAnimTMQ);
    UnRegisterMsg (g_EventExplosion);
    UnRegisterMsg (g_EventAnimPostEffect);
    UnRegisterMsg (g_EventAnimDirect);
    UnRegisterMsg (g_EventAnimColorChange);
    UnRegisterMsg (g_EventAnimStretch);
    UnRegisterMsg (g_EventAnimTrigger);
	UnRegisterMsg (g_EventAnimSkillCancel);

	// world field switching effects
	UnRegisterMsg(NPEI_IS_ANOTHER_FIELD_CHANGED);
	UnRegisterMsg(NPEI_IS_ANOTHER_FIELD_CHANGED_WITHOUT_DELAY);
	UnRegisterMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED);
	UnRegisterMsg(NPEI_IS_ANOTHER_BLOCK_CHANGED_WITHOUT_DELAY);
	UnRegisterMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED);
	UnRegisterMsg(NPEI_IS_ANOTHER_OBJECT_CHANGED_WITHOUT_DELAY);
	UnRegisterMsg(NPEI_IS_ANOTHER_SECTOR_CHANGED_WITHOUT_DELAY);
	UnRegisterMsg(NPEI_IS_MAPNAME_CHANGE);

	// trigger doodads
	UnRegisterMsg(g_EventTriDoodads);

    // Event Object
    UnRegisterMsg(g_EventCreateEventObject);
    UnRegisterMsg(g_EventThreadLoadingComplete);

	// gui event
	UnRegisterMsg (g_EventResize);

	// gui event
	UnRegisterMsg (g_EventCinematicViewShow);
	UnRegisterMsg (g_EventCinematicBalloon);
	UnRegisterMsg (g_EventCinematicFlash);
	UnRegisterMsg (g_EventCinematicEcho);
	UnRegisterMsg (g_EventResetCinematic);

	UnRegisterMsg (g_EventCinematicFadeOut);
	UnRegisterMsg (g_EventCinematicFadeIn);

	UnRegisterMsg (g_EventCinematicBreak);

	UnRegisterMsg (g_EventShowProfilerGui);

	UnRegisterMsg(g_EventShowSpectatorBalloon);
	UnRegisterMsg(g_EventHideAllSpectatorBalloon);

	UnRegisterMsg(g_EventShowPlayerName);    

	UnRegisterMsg(g_EventDirectPlayCancel);

	NTL_RETURNVOID();
}

void CNtlPLEventHandler::HandleEvents(RWS::CMsg &pMsg)
{
	NTL_FUNCTION("CNtlPLEventHandler::HandleEvents");


	NTL_RETURNVOID();
} 


