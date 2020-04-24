/******************************************************************************************************************************
 *
 * ntlworldmaterialplugin.h
 *
 * copyright (c) 2005 ntl-inc Ltd.
 *
 * author : woody1019
 *
 * purpose : expand material plugin 2 use multitextures
 *
 ******************************************************************************************************************************/

#include "precomp_ntlpresentation.h"
#include "ntlworldmaterialplugin.h"
#include "ntlworldsector.h"
#include "NtlDebug.h"


/* global variables */
RwInt32 LocalOffset = -1;


static void *rpNtlWorldSectorSplatConstructor(void *object, RwInt32 offset, RwInt32 size __RWUNUSED__)
{
	NTL_FUNCTION("rpNtlWorldSectorSplatConstructor");

	RwInt32 i = 0;
	sRpNtlWorldSectorSplat *pNtlWorlSectorSplat = dGetNtlWorldSectorSplatInfo(object, offset);
	pNtlWorlSectorSplat->pNtlWorldSector = NULL;

	NTL_RETURN(object);
}

static void *rpNtlWorldSectorSplatDestructor(void *object, RwInt32 offset, RwInt32  size __RWUNUSED__)
{
	NTL_FUNCTION("rpNtlWorldSectorSplatDestructor");

	RwUInt32 i = 0;
	//sRpNtlWorldSectorSplat *pNtlWorlSectorSplat = dGetNtlWorldSectorSplatInfo(object, offset);
	//pNtlWorlSectorSplat->pNtlWorldSector = NULL;
	// pNtlWorldSector 내부에서 아토믹이 파괴되면 이 파괴자가 불리므로 뻑남

	NTL_RETURN(object);
}

static void *rpNtlWorldSectorSplatCopier(void *dstObject, const void *srcObject, RwInt32 offset, RwInt32 size __RWUNUSED__)
{
	NTL_FUNCTION("rpNtlWorldSectorSplatCopier");

	RwUInt32 i = 0;
	sRpNtlWorldSectorSplat *pDestSectorSplat;
	pDestSectorSplat = dGetNtlWorldSectorSplatInfo(dstObject, LocalOffset);

	if(!pDestSectorSplat || !pDestSectorSplat->pNtlWorldSector)
	{
		NTL_RETURN(NULL);
	}

	pDestSectorSplat->pNtlWorldSector->MaterialCopier(dstObject, srcObject);

	NTL_RETURN(pDestSectorSplat);
}

static RwStream*
rpNtlWorldSectorSplatStreamRead(RwStream *stream,
								RwInt32	length __RWUNUSED__,
								void *object,
								RwInt32	offset,
								RwInt32	sizeInObj __RWUNUSED__)
{
	return stream;
}

static RwStream*
rpNtlWorldSectorSplatStreamWrite(RwStream *stream,
								 RwInt32 length __RWUNUSED__,
								 const void *object,
								 RwInt32 offset,
								 RwInt32 sizeInObj __RWUNUSED__)
{
	return stream;
}

static RwInt32
rpNtlWorldSectorSplatStreamGetSize(const void *object,
								   RwInt32 offset,
								   RwInt32 sizeInObj __RWUNUSED__)
{
	return sizeof(sRpNtlWorldSectorSplat);
	//return 4;
}

RwInt32
RpNtlWorldMaterialPluginAttach(void)
{
	RwInt32 offset;
	
	/* Register clump extension space... */
	LocalOffset = RpMaterialRegisterPlugin(	sizeof(sRpNtlWorldSectorSplat),
											MAKECHUNKID(rwVENDORID_CRITERIONTK, rwID_NTL_WORLD_MATERIAL_PLUGIN),
											rpNtlWorldSectorSplatConstructor,
											rpNtlWorldSectorSplatDestructor,
											rpNtlWorldSectorSplatCopier);

	if(LocalOffset < 0)
		return FALSE;

	/* Register binary stream functionality for extension data... */
	offset = -1;
	offset = RpMaterialRegisterPluginStream(MAKECHUNKID(rwVENDORID_CRITERIONTK, rwID_NTL_WORLD_MATERIAL_PLUGIN),
											rpNtlWorldSectorSplatStreamRead,
											rpNtlWorldSectorSplatStreamWrite,
											rpNtlWorldSectorSplatStreamGetSize);

	if(offset != LocalOffset)
		return FALSE;

	return TRUE;
}

sRpNtlWorldSectorSplat* RpNtlWorldGetSectorSplat(RpMaterial *pMaterial)
{
	return dGetNtlWorldSectorSplatInfo(pMaterial, LocalOffset);
}

RwBool RpNtlWorldSetSectorSplat(RpMaterial *pMaterial, sRpNtlWorldSectorSplat *pSectorSplat)
{
	sRpNtlWorldSectorSplat *pNtlWorlSectorSplat = dGetNtlWorldSectorSplatInfo(pMaterial, LocalOffset);

	if(pSectorSplat)
		pNtlWorlSectorSplat->pNtlWorldSector = pSectorSplat->pNtlWorldSector;
	else
		return FALSE;

	return TRUE;
}