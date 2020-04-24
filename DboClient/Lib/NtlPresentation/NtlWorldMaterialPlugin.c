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


#include <d3d9.h>
#include "ntlworldmaterialplugin.h"


/* global variables */
static RwInt32 LocalOffset = -1;


static void *rpNtlWorldSectorSplatConstructor(void *object, RwInt32 offset, RwInt32 size __RWUNUSED__)
{
	RwInt32 i = 0;
	sRpNtlWorldSectorSplat *pNtlWorlSectorSplat = dGetNtlWorldSectorSplatInfo(object, offset);

	// initialize expanded structures
	pNtlWorlSectorSplat->pTextureBase = NULL;
	for(i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		pNtlWorlSectorSplat->pTextureSplat[i] = NULL;
		pNtlWorlSectorSplat->pTextureAlpha[i] = NULL;
	}

	return object;
}

static void *rpNtlWorldSectorSplatDestructor(void *object, RwInt32 offset, RwInt32  size __RWUNUSED__)
{
	RwUInt32 i = 0;
	sRpNtlWorldSectorSplat *pNtlWorlSectorSplat = dGetNtlWorldSectorSplatInfo(object, offset);

	// destroy
	if(pNtlWorlSectorSplat->pTextureBase)
		RwTextureDestroy(pNtlWorlSectorSplat->pTextureBase);

	for(i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		if(pNtlWorlSectorSplat->pTextureSplat[i])
			RwTextureDestroy(pNtlWorlSectorSplat->pTextureSplat[i]);

		if(pNtlWorlSectorSplat->pTextureAlpha[i])
			RwTextureDestroy(pNtlWorlSectorSplat->pTextureAlpha[i]);
	}

	return object;
}

static void *rpNtlWorldSectorSplatCopier(void *dstObject, const void *srcObject, RwInt32 offset, RwInt32 size __RWUNUSED__)
{
	RwUInt32 i = 0;
	sRpNtlWorldSectorSplat *pDestSectorSplat;
	const sRpNtlWorldSectorSplat *pSrcSectorSplat;

	pDestSectorSplat = dGetNtlWorldSectorSplatInfo(dstObject, LocalOffset);
	pSrcSectorSplat  = dGetNtlWorldSectorSplatInfoConst(srcObject, LocalOffset);

	if(pSrcSectorSplat->pTextureBase)
	{
		pDestSectorSplat->pTextureBase = pSrcSectorSplat->pTextureBase;
		RwTextureAddRef(pDestSectorSplat->pTextureBase);
	}

	for(i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		if(pSrcSectorSplat->pTextureSplat[i])
		{
			pDestSectorSplat->pTextureSplat[i] = pSrcSectorSplat->pTextureSplat[i];
			RwTextureAddRef(pDestSectorSplat->pTextureSplat[i]);
		}
		
		if(pSrcSectorSplat->pTextureAlpha[i])
		{
			pDestSectorSplat->pTextureAlpha[i] = pSrcSectorSplat->pTextureAlpha[i];
			RwTextureAddRef(pDestSectorSplat->pTextureAlpha[i]);
		}
	}

	return pDestSectorSplat;
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
	RwInt32 i;
	sRpNtlWorldSectorSplat *pNtlWorlSectorSplat = dGetNtlWorldSectorSplatInfo(pMaterial, LocalOffset);

	if(!pNtlWorlSectorSplat)
		return FALSE;

	if(pSectorSplat)
	{
		if(pNtlWorlSectorSplat->pTextureBase)
			RwTextureDestroy(pNtlWorlSectorSplat->pTextureBase);

		for(i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
		{
			if(pNtlWorlSectorSplat->pTextureSplat[i])
				RwTextureDestroy(pNtlWorlSectorSplat->pTextureSplat[i]);

			if(pNtlWorlSectorSplat->pTextureAlpha[i])
				RwTextureDestroy(pNtlWorlSectorSplat->pTextureAlpha[i]);
		}

		if(pSectorSplat->pTextureBase)
			pNtlWorlSectorSplat->pTextureBase = RwTextureAddRef(pSectorSplat->pTextureBase);

		for(i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
		{
			if(pSectorSplat->pTextureSplat[i])
				pNtlWorlSectorSplat->pTextureSplat[i] = RwTextureAddRef(pSectorSplat->pTextureSplat[i]);

			if(pSectorSplat->pTextureAlpha[i])
				pNtlWorlSectorSplat->pTextureAlpha[i] = RwTextureAddRef(pSectorSplat->pTextureAlpha[i]);
		}
	}
	else
		return FALSE;

	return TRUE;
}