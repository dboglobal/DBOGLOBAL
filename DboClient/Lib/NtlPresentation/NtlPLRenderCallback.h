/*****************************************************************************
 *
 * File			: NtlPLRenderCallback.h
 * Author		: HyungSuk, Jang
 * Copyright	: (주)NTL
 * Date			: 2005. 12. 06	
 * Abstract		: Presentation render callback 
 *****************************************************************************
 * Desc         : renderware rendering callback 관한 함수.
 *
 *****************************************************************************/

#ifndef __NTL_PLRENDERCALLBACK_H__
#define __NTL_PLRENDERCALLBACK_H__

#include <rwcore.h>
#include <rpworld.h>
#include <rppvs.h>

// #ifndef dNTL_WORLD_CULLING_NEW
// RpWorldSector*	NtlWorldSectorCullingCallback(RpWorldSector *pWorldSector, void *data);
// #endif

RpWorldSector*	NtlWorldSectorAtomicCallback(RpWorldSector *pWorldSector, void *data); 
RpWorldSector*	NtlWorldSectorRenderCallback(RpWorldSector *pWorldSector, void *data);

// 인도어 : TEST
RpWorldSector*	NtlWorldSectorRenderCallBack_PVSBBox(RpWorldSector* worldSector);
RpWorldSector*	NtlWorldSectorRenderCallBack_PVSWire(RpWorldSector* worldSector);
RpWorldSector*	NtlWorldSectorRenderCallBack_PVS(RpWorldSector* worldSector);

#endif