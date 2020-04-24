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

#ifndef ntlworldmaterialplugin_h
#define ntlworldmaterialplugin_h


/******************************************************************************************************************************
 * headers 
 ******************************************************************************************************************************/
#include "ntlworldcommon.h"

class CNtlWorldSector;

/******************************************************************************************************************************
* structures
******************************************************************************************************************************/
typedef struct sRpNtlWorldSectorSplat sRpNtlWorldSectorSplat;
struct sRpNtlWorldSectorSplat
{
	CNtlWorldSector *pNtlWorldSector;
};

/******************************************************************************************************************************
* definition
******************************************************************************************************************************/
#define rwID_NTL_WORLD_MATERIAL_PLUGIN (0x77)
#define dGetNtlWorldSectorSplatInfo(material, offset)		((sRpNtlWorldSectorSplat*)((RwUInt8*)material + offset))
#define dGetNtlWorldSectorSplatInfoConst(material, offset)	((const sRpNtlWorldSectorSplat*)((const RwUInt8*)material + offset))

/******************************************************************************************************************************
 * prototypes 
 ******************************************************************************************************************************/
extern RwInt32					RpNtlWorldMaterialPluginAttach(void);
extern RwBool					RpNtlWorldSetSectorSplat(RpMaterial *pMaterial, sRpNtlWorldSectorSplat *pSectorSplat);
extern sRpNtlWorldSectorSplat*	RpNtlWorldGetSectorSplat(RpMaterial *pMaterial);

// globals
extern RwInt32 LocalOffset;


#endif /* ntlworldmaterialplugin_h */