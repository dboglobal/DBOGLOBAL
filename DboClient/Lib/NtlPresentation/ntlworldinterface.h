/******************************************************************************************************************************
 *
 * ntlworldinterface_h
 *
 * copyright (c) 2005 ntl-inc Ltd.
 *
 * author : woody1019
 *
 * purpose : creates a bsp in a quad-tree arrangement for the interlocking tiles to reside in.
 *
 ******************************************************************************************************************************/


#ifndef ntlworldinterface_h
#define ntlworldinterface_h


/******************************************************************************************************************************
 * headers 
 ******************************************************************************************************************************/
#include "ntlworldcommon.h"


/******************************************************************************************************************************
 * prototypes 
 ******************************************************************************************************************************/

extern RpWorld*			RpNtlWorldCreate();
extern RpWorldSector*   RpNtlWorldSectorRender(RpWorldSector *pRpWorldSector);
extern RwReal			RpNtlWorldSectorGetHeight(RpWorldSector *pRpWorldSector, RwV3d *pPos);
extern RwReal			RpNtlWorldSectorGetHeight(CNtlWorldSector* pNtlWorldSector, RwV3d* pPos, RwV3d* pVList);
extern RwReal			RpNtlWorldRealPropLerp(RwReal Src, RwReal Dst, RwReal SrcTime, RwReal DstTime);
extern RwUInt8			RpNtlWorldUInt8PropLerp(RwUInt8 Src, RwUInt8 Dst, RwReal SrcTime, RwReal DstTime);


extern void				RpNtlWorldGetVisibleSectors(vector<RwInt32>* vecVisibleSectors);
extern RpWorldSector*	RpNtlWorldGetVisibleSectorsCB(RpWorldSector* pWorldSector, void* pData);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum eNTL_WORLD_OBJ_VISION_ANLYZER
{
	eNWOVA_DOODADS					= 0x00000001,	// display menu
	eNWOVA_WATER					= 0x00000002,
	eNWOVA_ANIMATIBLE_DOODAD_DUMMY	= 0x00000004,
	eNWOVA_PERFORMANCE_SECTORCULL	= 0x00000008,
	eNWOVA_WIRE_SOLID				= 0x00000010,
	eNWOVA_EFFECTS					= 0x00000020,
	eNWOVA_VISIBILTITY_DIST			= 0x00000040,
	eNWOVA_COLLISION_OBJ			= 0x00000080,
	eNWOVA_BLOOM_EFFECT				= 0x00000100,
	eNWOVA_PVS_LOCKING				= 0x00000200,
	eNWOVA_PROJECTION_SHADOW		= 0x00000400,
	eNWOVA_FOG_ACTIVATION			= 0x00000800,
	eNWOVA_PLNAT					= 0x00001000,
	eNWOVA_OCCLUDER_ACTIVATION		= 0x00002000,
	eNWOVA_WIRE_OBJECT				= 0x00004000,
	eNWOVA_WEATHER					= 0x00008000,
	eNWOVA_HEATHAZE					= 0x00010000,

	eNWOVA_BSP_SECTOR				= 0x00020000,	// outdoor
	eNWOVA_BSP_WIREFRAME			= 0x00040000,
	eNWOVA_BSP_WIREFRAME_NV			= 0x00080000,

	eNWOVA_LIGHTMAP					= 0x00100000,	// lightmap visibility toggling
	eNWOVA_OBJECT_ANIMATION			= 0x00200000,

	eNWOVA_TRIGGER_INFO				= 0x00400000,

	eNWOVA_MINI_INDOOR_CULL			= 0x00800000,

	eNWOVA_PSTEX					= 0x10000000,	// ui

	eNWOVA_SOUND					= 0x20000000,	// sound
	eNWOVA_BGM						= 0x40000000,	
	eNWOVA_ENV						= 0x80000000,	
};


class CNtlWorldObjVisionAnalyzer
{
public:
	CNtlWorldObjVisionAnalyzer(void);
	~CNtlWorldObjVisionAnalyzer(void);

private:
	DWORD m_ObjVision;

public:
	static CNtlWorldObjVisionAnalyzer* GetInstance(void);

public:
	BOOL GetCheck(eNTL_WORLD_OBJ_VISION_ANLYZER eNWOVA_ID);
	VOID SetCheck(eNTL_WORLD_OBJ_VISION_ANLYZER eNWOVA_ID, BOOL SetFlag = TRUE);
	VOID Toggle(eNTL_WORLD_OBJ_VISION_ANLYZER eNWOVA_ID);
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif /* ntlworldinterface_h */