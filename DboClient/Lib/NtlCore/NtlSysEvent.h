/*****************************************************************************
 *
 * File			: NtlGlobalEvent.h
 * Author		: HyungSuk, Jang
 * Copyright	: (аж)NTL
 * Date			: 2005. 7. 14	
 * Abstract		: system event definition.
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/

#ifndef __NTL_SYSTEM_EVENT__
#define __NTL_SYSTEM_EVENT__

#include "ceventhandler.h"

/******************************************************************************
 *  Global System Events
 */

extern RWS::CMsg g_MsgTick;

/**
*  \ingroup SystemEvents
*  \page LogicEvents
*  \li g_EventUpdateTick
*  Sent each g_EventUpdateTick.
*/
extern RWS::CEventId g_EventUpdateTick;

/**
*  \ingroup SystemEvents
*  \page LogicEvents
*  \li g_EventSync30UpdateTick
*  Sent each g_EventSync30UpdateTick.
*/

extern RWS::CEventId g_EventSync30UpdateTick;

/**
*  \ingroup SystemEvents
*  \page LogicEvents
*  \li g_EventSync20UpdateTick
*  Sent each g_EventSync20UpdateTick.
*/
extern RWS::CEventId g_EventSync20UpdateTick;

/**
*  \ingroup SystemEvents
*  \page LogicEvents
*  \li g_EventSync10UpdateTick
*  Sent each g_EventSync10UpdateTick.
*/
extern RWS::CEventId g_EventSync10UpdateTick;

/**
*  \ingroup SystemEvents
*  \page RenderEvents
*  \li g_EventRenderTick
*  Sent each g_EventRenderTick.
*/
extern RWS::CEventId g_EventRenderTick;

/**
*  \ingroup SystemEvents
*  \page StageEvents
*  \li g_EventCreateStage
*  Sent just before each g_EventCreateStage.
*/
extern RWS::CEventId g_EventCreateStage;

/**
*  \ingroup SystemEvents
*  \page StageEvents
*  \li g_EventCreateStage
*  Sent just before each g_EventCreateStage.
*/
extern RWS::CEventId g_EventDeleteStage;


/**
*  \ingroup SystemEvents
*  \page StageEvents
*  \li g_EventCreateStage
*  Sent just before each g_EventCreateStage.
*/
extern RWS::CEventId g_EventQueueProcessFilter;

/**
* \ingroup SystemEnvets
* \page SystemEvents
* \li g_EventToggleFullScreen
* Sent each g_EventToggleFullScreen when application is toggled.
*/
extern RWS::CEventId g_EventToggleFullScreen;

/******************************************************************************
 *  Global System Events Data Structer
 */

/**
*  Stage Create and Delete event data structer.
*/

struct SNtlEventStageData
{
	char chStageName[128];
};

#endif