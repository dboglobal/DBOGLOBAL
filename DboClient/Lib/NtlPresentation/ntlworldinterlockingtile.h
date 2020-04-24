/******************************************************************************************************************************
 *
 * ntlworldinterlockingtile_h
 *
 * copyright (c) 2005 ntl-inc Ltd.
 *
 * author : woody1019
 *
 * purpose : creates a bsp in a quad-tree arrangement for the interlocking tiles to reside in.
 *
 ******************************************************************************************************************************/

#ifndef ntlworldinterlockingtile_h
#define ntlworldinterlockingtile_h


/******************************************************************************************************************************
 * headers 
 ******************************************************************************************************************************/
#include "ntlworldcommon.h"
#include <d3d9.h>


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/******************************************************************************************************************************
* prototypes 
******************************************************************************************************************************/

/* 2 access unified index buff */
extern LPDIRECT3DINDEXBUFFER9 GetBody(RwUInt32 MyLvl, RwBool T, RwBool L, RwBool R, RwBool B);
extern LPDIRECT3DINDEXBUFFER9 GetLink(RwUInt32 MyLvl, RwUInt32 NeiborLvl, RwUInt32 Dir);
extern RwUInt16* GetSystemBody(RwUInt32 MyLvl, RwBool T, RwBool L, RwBool R, RwBool B);
extern RwUInt16* GetSystemLink(RwUInt32 MyLvl, RwUInt32 NeiborLvl, RwUInt32 Dir);
extern RwUInt32 GetBodyPrimitiveNum(RwUInt32 MyLvl, RwBool T, RwBool L, RwBool R, RwBool B);
extern RwUInt32 GetLinkPrimitiveNum(RwUInt32 MyLvl, RwUInt32 NeiborLvl, RwUInt32 Dir);

extern int RenderNtlWorldSector();

/* create atomic geometry only for vertices */
sNtlWorldLocals* _RpNtlWorldSectorGeometryCreate(RpWorldSector *pWorldSector);

/******************************************************************************************************************************
 * global variables
 ******************************************************************************************************************************/

/******************************************************************************************************************************
 * type definitions
 ******************************************************************************************************************************/

/******************************************************************************************************************************
 * structures
 ******************************************************************************************************************************/
typedef struct sNtlWorldSectorIBInfo sNtlWorldSectorIBInfo;
struct sNtlWorldSectorIBInfo
{
	LPDIRECT3DINDEXBUFFER9 _pIB;
	RwInt32 _NumIndices;
};

typedef struct sNtlWorldSectorIB sNtlWorldSectorIB;
struct sNtlWorldSectorIB
{
	/* 4 directions; 2 * 2 * 2 * 2 = 16 cases */
	sNtlWorldSectorIBInfo Body[16];

	/* linker * x levels; */
	sNtlWorldSectorIBInfo *Linker[4];
};

extern sNtlWorldSectorIB *pNtlWorldSectorIB;


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ntlworldinterlockingtile_h */