/******************************************************************************************************************************
 *
 * ntlworldinterlockingtile.h
 *
 * copyright (c) 2005 ntl-inc Ltd.
 *
 * author : woody1019
 *
 * purpose : implement interlocking tile geometry
 *
 ******************************************************************************************************************************/


/******************************************************************************************************************************
 * headers
 ******************************************************************************************************************************/
#include "ntlworldinterlockingtile.h"


sNtlWorldSectorIB *pNtlWorldSectorIB = NULL;


/******************************************************************************************************************************
* 2 access unified index buff
******************************************************************************************************************************/
LPDIRECT3DINDEXBUFFER9 GetBody(RwUInt32 MyLvl, RwBool T, RwBool L, RwBool R, RwBool B)
{
}

LPDIRECT3DINDEXBUFFER9 GetLink(RwUInt32 MyLvl, RwUInt32 NeiborLvl, RwUInt32 Dir)
{
}

RwUInt16* GetSystemBody(RwUInt32 MyLvl, RwBool T, RwBool L, RwBool R, RwBool B)
{
}

RwUInt16* GetSystemLink(RwUInt32 MyLvl, RwUInt32 NeiborLvl, RwUInt32 Dir)
{
}

RwUInt32 GetBodyPrimitiveNum(RwUInt32 MyLvl, RwBool T, RwBool L, RwBool R, RwBool B)
{
}

RwUInt32 GetLinkPrimitiveNum(RwUInt32 MyLvl, RwUInt32 NeiborLvl, RwUInt32 Dir)
{
}

/******************************************************************************************************************************
* internal
******************************************************************************************************************************/
sNtlWorldLocals* _RpNtlWorldSectorGeometryCreate(RpWorldSector *pWorldSector)
{
	sNtlWorldLocals *pNtlWorldSector = RwFreeListAlloc(dNTL_WORLD_GLOBAL(pSectorFreelist), rwMEMHINTDUR_EVENT);

	RpAtomic *pAtomic;
	RpGeometry *pGeometry;

	//pGeometry = RpGeometryCreate(dWORLD_SECTOR_VERT_NUM * dWORLD_SECTOR_VERT_NUM, numTris, rpGEOMETRYLIGHT | rpGEOMETRYNORMALS | rpGEOMETRYTEXTURED);

	//pNtlWorldSector->pNtlWorldSector->pAtomic;
}

/******************************************************************************************************************************
* RenderSector
******************************************************************************************************************************/
int RenderNtlWorldSector()
{
	return TRUE;
}