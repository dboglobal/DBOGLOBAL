/*****************************************************************************
 *
 * File			: NtlSLApi.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2006. 10. 25	
 * Abstract		: Simulation layer API Function
 *****************************************************************************
 * Desc         : 
 *
 *****************************************************************************/


#ifndef __NTL_SLAPI_H__
#define __NTL_SLAPI_H__

class CTableContainer;

class CNtlCamera;

class CNtlLogic;
class CNtlRender;
class CNtlSLPacketGenerator;
class CNtlPacketLockManager;
class CDboTSCQAgency;
class CDboTSCTAgency;
class CNtlClock;
class CNtlDTContainer;

//------------------------------------------------------------------
// dev master define
//------------------------------------------------------------------

#define DEVMASTER_KEY				"Dbo_20190413"

#define DEVMASTER_SUCCESS			0
#define DEVMASTER_INVALID_SERIAL	1
#define DEVMASTER_NOT_FOUND_FILE	2
#define DEVMASTER_READ_ERROR		3

#define DEF_MOUSE_CLICK_INTERVAL	(200.f)
#define MIN_MOUSE_CLICK_INTERVAL	(150.f)
#define MAX_MOUSE_CLICK_INTERVAL	(500.f)

typedef struct _SDevMasterInfo
{
	RwBool	bDevUser;				// user가 개발자 인가?
	RwReal	fCoolingTimeFactor;		// cooling update time factor
	RwBool	bLoadDataXml;			// loading data type xml
	RwBool	bPackFileUse;			// pack file use
	RwBool	bThreadLoading;			// thread loading을 할 것인가?
	RwReal	fCameraFov;				// camera fov
	RwReal	fCameraNear;			// camera near
	RwReal	fCameraFar;				// camera far
	RwReal	fCameraZoomMin;			// camera zoom min
	RwReal	fCameraZoomMax;			// camera zoom max	
	RwBool	bScreenChangeMode;		// screen chagne mode
}SDevMasterInfo;

MEMORYSTATUS*			API_GetGlobalMemoryStatus(void);			
RwUInt8					API_LoadSLDevMasterInformation(void);
SDevMasterInfo*			API_GetSLDevMasterInformation(void);

RwBool					API_SLInit(RwBool bVirtualNet, CNtlCamera *pCamera, RwUInt8 byLoadMethod, RwBool bDTDataLoad = FALSE);
void					API_SLReset(void);
void					API_SLTerminate(void);

RwBool					API_SLResourceInit(void);
void					API_SLResourceTerminate(void);

CTableContainer*		API_GetTableContainer(void);
void					API_SetTableContainer(CTableContainer* pTblContainer);

CNtlSLPacketGenerator*	API_GetSLPacketGenerator(void);
CNtlPacketLockManager*	API_GetSLPacketLockManager(void);

CDboTSCQAgency*			API_GetQuestAgency(void);
CDboTSCTAgency*			API_GetTriggerAgency(void);

CNtlClock*				API_GetGameClock(void);

CNtlDTContainer*		API_GetDTContainter(void);

void					API_InitDoubleClickInterval(void);
void					API_DestroyDoubleClickInterval(void);
RwReal					API_GetDoubleClickInterval(void);
void					API_SetDoubleClickInterval( RwReal fIntervalRatio = 0.f );

#endif
