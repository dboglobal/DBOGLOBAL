/******************************************************************************************************************************
 *
 * ntlworldplugin.h
 *
 * copyright (c) 2005 ntl-inc Ltd.
 *
 * author : woody1019
 *
 * purpose : creates a bsp in a quad-tree arrangement for the interlocking tiles to reside in.
 *
 ******************************************************************************************************************************/

#ifndef ntlworldplugin_h
#define ntlworldplugin_h

/******************************************************************************************************************************
 * headers 
 ******************************************************************************************************************************/
#include "ntlworldcommon.h"
#include "FileMem.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

/******************************************************************************************************************************
 * prototypes 
 ******************************************************************************************************************************/
extern RwBool RpNtlWorldAttach(void);
extern RpWorldSector *RpNtlWorldSectorRender(RpWorldSector *pRpWorldSector);

// 인도어
//extern RpWorldSector *RpNtlWorldSectorWaterRender(RpWorldSector *pRpWorldSector);
extern CNtlWorldSector *RpNtlWorldSectorWaterRender(CNtlWorldSector *pNtlWorldSector);

#ifdef dNTL_WORLD_TOOL_MODE
extern RwBool RpNtlWorldSectorGeometryCreate(CNtlWorldSector *pNtlWorldSector);
#endif
extern RwBool RpNtlWorldSectorGeometryDeleteWithoutSwapFileSaving(CNtlWorldSector *pNtlWorldSector);

extern RwBool RpNtlWorldSectorGeometryDelete(CNtlWorldSector *pNtlWorldSector);
extern RwBool RpNtlWorldSectorGeometryCreateFromFile(sCUR_FIELD_TEX_INFO& FieldTexInfo, CNtlWorldSector *pNtlWorldSector, FILE* pFile);
extern RwBool RpNtlWorldSectorGeometryDeleteIntoFile(CNtlWorldSector *pNtlWorldSector, FILE* pFile);
extern RwBool RpNtlWorldSectorUpdateTexLyr(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RwInt32 IdxLyr, RwInt32 IdxMenu);
extern RwBool RpNtlWorldSectorUpdateTexMat(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RwInt32 IdxLyr);
extern RwBool RpNtlWorldSectorGeomSetRenderCB();
extern RwBool RpNtlWorldSectorWaterGeomSetRenderCB();

RwBool	_RpNtlWorldSectorCullCheck(CNtlWorldSector *pNtlWorldSector);
RwBool	_RpNtlWorldSectorIsFullFogged(CNtlWorldSector *pNtlWorldSector);

void	_RpNtlWorldWaterD3D9RenderCallback(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags);

// TextrueStageNum : 4
void	_RpNtlWorldSectorBLayerRender(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader, RwBool bRenderTex);
void	_RpNtlWorldSectorDLayerRender(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader);
void	_rpNtlWorldD3D9RenderCallback(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags);

// TextrueStageNum : 8
void	_RpNtlWorldD3D9RenderCallback01(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags);

void	_RpNtlWorldSectorWireSolid(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader);
void	_RpNtlWorldSectorDrawPrimitive(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader);

#ifdef dNTL_WORLD_COMBINE
RwTexture*	NtlTerrainCombineTextureGenerator(RwInt32 iSectorIdx);
void		_RpNtlWorldD3D9RenderCallback02(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags);
#endif

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

/******************************************************************************************************************************
* prototypes 
******************************************************************************************************************************/
extern RwBool RpNtlWorldAttach(void);
extern RpWorldSector *RpNtlWorldSectorRender(RpWorldSector *pRpWorldSector);

// 인도어
//extern RpWorldSector *RpNtlWorldSectorWaterRender(RpWorldSector *pRpWorldSector);
extern CNtlWorldSector *RpNtlWorldSectorWaterRender(CNtlWorldSector *pNtlWorldSector);

#ifdef dNTL_WORLD_TOOL_MODE

extern RwBool RpNtlWorldSectorGeometryCreate(CNtlWorldSector *pNtlWorldSector);

#endif

extern RwBool RpNtlWorldSectorGeometryDelete(CNtlWorldSector *pNtlWorldSector);
extern RwBool RpNtlWorldSectorGeometryCreateFromFile(sCUR_FIELD_TEX_INFO& FieldTexInfo, CNtlWorldSector *pNtlWorldSector, FILE* pFile);
//extern RwBool RpNtlWorldSectorGeometryDeleteIntoFile(RpWorldSector *pRpWorldSector, FILE* pFile);
extern RwBool RpNtlWorldSectorGeometryDeleteIntoFile(CNtlWorldSector *pNtlWorldSector, FILE* pFile);
extern RwBool RpNtlWorldSectorUpdateTexLyr(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RwInt32 IdxLyr, RwInt32 IdxMenu);
extern RwBool RpNtlWorldSectorUpdateTexMat(sCUR_FIELD_TEX_INFO& CurFieldTexInfo, CNtlWorldSector *pNtlWorldSector, RwInt32 IdxLyr);
extern RwBool RpNtlWorldSectorGeomSetRenderCB();
extern RwBool RpNtlWorldSectorWaterGeomSetRenderCB();

RwBool	_RpNtlWorldSectorCullCheck(CNtlWorldSector *pNtlWorldSector);
RwBool	_RpNtlWorldSectorIsFullFogged(CNtlWorldSector *pNtlWorldSector);

void	_RpNtlWorldWaterD3D9RenderCallback(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags);

// TextrueStageNum : 4
void	_RpNtlWorldSectorBLayerRender(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader, RwBool bRenderTex);
void	_RpNtlWorldSectorDLayerRender(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader);
void	_rpNtlWorldD3D9RenderCallback(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags);

// TextrueStageNum : 8
void	_RpNtlWorldD3D9RenderCallback01(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags);

void	_RpNtlWorldSectorWireSolid(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader);
void	_RpNtlWorldSectorDrawPrimitive(RxD3D9InstanceData *pInstancedData, RxD3D9ResEntryHeader *pResEntryHeader);

#ifdef dNTL_WORLD_COMBINE
RwTexture*	NtlTerrainCombineTextureGenerator(RwInt32 iSectorIdx);
void		_RpNtlWorldD3D9RenderCallback02(RwResEntry *repEntry, void *object, RwUInt8 type, RwUInt32 flags);
#endif


//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif

#endif ntlworldplugin_h