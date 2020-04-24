#include "precomp_ntlpresentation.h"

#include "NtlWorldFieldManager.h"
#include "NtlWorldField.h"

#include "NtlPLWorldDecal.h"
#include "NtlPLDecalVolume.h"
#include "NtlPLDojo.h"
#include "NtlPLPlanet.h"

#include "NtlWorldFileAccessor.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifndef dNTL_WORLD_FILE

CNtlWorldFileAccessor::CNtlWorldFileAccessor(void)
{
}

CNtlWorldFileAccessor::~CNtlWorldFileAccessor(void)
{
}

RwInt32 CNtlWorldFileAccessor::Skip_Sector(FILE* pFile)
{
	Skip_Material(pFile);
	Skip_Heightfield(pFile);
	Skip_Diffuse(pFile);
	Skip_Doodads(pFile);
	Skip_Water(pFile);
	Skip_Shadow(pFile);
	Skip_Effects(pFile);
	Skip_SE(pFile);
	// CZ-SS
	//Skip_SS(pFile);
	Skip_SectorCull(pFile);
	Skip_TileTransparency(pFile);
	Skip_Shoreline(pFile);
	Skip_Decal(pFile);
	Skip_Plant(pFile);
	Skip_WorldLight(pFile);
	Skip_Occluder(pFile);
	Skip_HeatHazeObject(pFile);
	Skip_LightObject(pFile);
	Skip_Dojo(pFile);

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_SectorIndoor(FILE* pFile)
{
	Skip_DoodadsIndoor(pFile);
	Skip_Water(pFile);
	Skip_Shadow(pFile);
	Skip_Effects(pFile);
	Skip_SE(pFile);
	// CZ-SS
	//Skip_SS(pFile);
	Skip_SectorCull(pFile);
	Skip_TileTransparency(pFile);
	Skip_Shoreline(pFile);
	Skip_Decal(pFile);
	Skip_Plant(pFile);
	Skip_WorldLight(pFile);
	Skip_Occluder(pFile);
	Skip_HeatHazeObject(pFile);
	Skip_LightObject(pFile);
	Skip_DecalVolume(pFile);
	Skip_GameProperty(pFile);
	Skip_Dojo(pFile);

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_Material(FILE* pFile)
{
	RwBool TmpBool;

	::fseek(pFile, (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH) + (sizeof(D3DXMATRIX) * 2) +	sizeof(RwReal) + sizeof(RwUInt8), SEEK_CUR);

	for(int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		::fread(&TmpBool, sizeof(RwBool), 1, pFile);

		if(TmpBool)
		{
			::fseek(pFile, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, SEEK_CUR);
			::fread(&TmpBool, sizeof(RwBool), 1, pFile);

			if(TmpBool)
			{
				::fseek(pFile, (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH) + (sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum), SEEK_CUR);
			}

			::fseek(pFile, (sizeof(D3DXMATRIX) * 2) + sizeof(RwReal) + sizeof(RwUInt8), SEEK_CUR);
		}
	}

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_Heightfield(FILE* pFile)
{
	::fseek(pFile, sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, SEEK_CUR);

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_Diffuse(FILE* pFile)
{
	::fseek(pFile, sizeof(RwRGBA) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum, SEEK_CUR);

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_Doodads(FILE* pFile)
{
	//RwInt32 SkipRange;
	RwBool	EnableFlag;
	RwInt32 TmpObjNum;
	::fread(&TmpObjNum, 1, sizeof(RwInt32), pFile);

	for(int i = 0; i < TmpObjNum; ++i)
	{
		::fseek(pFile, (sizeof(RwChar) * 128), SEEK_CUR); // Obj. Name
		::fseek(pFile, (sizeof(RwV3d) * 3), SEEK_CUR); // Scaling, Rotation, Translation

		RwUInt32 uiObjectType;
		::fread(&uiObjectType, 1, sizeof(RwUInt32), pFile);
		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			// Object Propperty Range
			RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
				(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
				sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
				(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);
			::fseek(pFile, iPropertyRange, SEEK_CUR);
		}

		::fseek(pFile, (sizeof(RwUInt32)), SEEK_CUR); // trigger obj serial id
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
		::fseek(pFile, (sizeof(RwUInt32)), SEEK_CUR); // object serial id
#endif
		::fseek(pFile, (sizeof(DWORD)), SEEK_CUR); // object name index
		::fseek(pFile, (sizeof(RwReal)), SEEK_CUR); // visibility distance
		::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // enable trigger
		::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // enable selection
		::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // enable alterable
		::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // enable portal
		::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // enable p.e. collision
		::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // shadow prop

// 		SkipRange = (sizeof(RwChar) * 128) +	// Obj. Name
// 			(sizeof(RwV3d) * 3) +		// Scaling, Rotation, Translation
// 			(sizeof(RwUInt32)) +		// trigger obj serial id
// #ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
// 			(sizeof(RwUInt32)) +		// object serial id
// #endif
// 			(sizeof(DWORD)) +			// object name index
// 			(sizeof(RwReal)) +			// visibility distance
// 			(sizeof(RwBool)) +			// enable trigger
// 			(sizeof(RwBool)) +			// enable selection
// 			(sizeof(RwBool)) +			// enable alterable
// 			(sizeof(RwBool)) +			// enable portal
// 			(sizeof(RwBool)) +			// enable p.e. collision
// 			(sizeof(RwBool));			// shadow prop
// 
// 		::fseek(pFile, SkipRange, SEEK_CUR);

		// PS Map Size
		RwInt32 PSAtomicCnt = 0;
		::fread(&PSAtomicCnt, sizeof(RwInt32), 1, pFile);

		for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
		{
			// Map name
			::fseek(pFile, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, SEEK_CUR);

			// Frustum view window extent
			::fseek(pFile, sizeof(RwReal), SEEK_CUR);

			// Light matrices
			::fseek(pFile, sizeof(RwMatrix), SEEK_CUR);

			// Map size
			RwInt32 Size = 0;
			::fread(&Size, sizeof(RwInt32), 1, pFile);
			::fseek(pFile, sizeof(DWORD) * Size * Size, SEEK_CUR);
		}

		// milepost
		::fread(&EnableFlag, sizeof(RwBool), 1, pFile);
		if(EnableFlag)
		{
			::fseek(pFile, sizeof(RwUInt32), SEEK_CUR);
		}

		// attach light object skip
		RwInt32 iLightObjectNum;
		fread(&iLightObjectNum, sizeof(RwInt32), 1, pFile);

		for (int j = 0; j < iLightObjectNum; ++j)
		{
			fseek(pFile, sizeof(RwChar) * MAX_ATTR_BONE_NAME, SEEK_CUR);

			fseek(pFile, sizeof(RpLightType), SEEK_CUR);
			fseek(pFile, sizeof(RwBool), SEEK_CUR);

			fseek(pFile, sizeof(RwV3d), SEEK_CUR);

			fseek(pFile, sizeof(RwV3d), SEEK_CUR);
			fseek(pFile, sizeof(RwRGBAReal), SEEK_CUR);
			fseek(pFile, sizeof(RwReal), SEEK_CUR);
			fseek(pFile, sizeof(RwReal), SEEK_CUR);

			fseek(pFile, sizeof(RwBool), SEEK_CUR);
			fseek(pFile, sizeof(RwBool), SEEK_CUR);
		}
	}	

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_DoodadsIndoor(FILE* pFile)
{
	//RwInt32 SkipRange;
	RwBool	EnableFlag;
	RwInt32 TmpObjNum;
	::fread(&TmpObjNum, 1, sizeof(RwInt32), pFile);

	for(int i = 0; i < TmpObjNum; ++i)
	{
		::fseek(pFile, (sizeof(RwChar) * 128), SEEK_CUR); // Obj. Name
		::fseek(pFile, (sizeof(RwV3d) * 3), SEEK_CUR); // Scaling, Rotation, Translation

		RwUInt32 uiObjectType;
		::fread(&uiObjectType, 1, sizeof(RwUInt32), pFile);
		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			// Object Propperty Range
			RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
				(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
				sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
				(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);
			::fseek(pFile, iPropertyRange, SEEK_CUR);
		}

		::fseek(pFile, (sizeof(RwUInt32)), SEEK_CUR); // trigger obj serial id
		::fseek(pFile, (sizeof(RwUInt32)), SEEK_CUR); // object serial id
		::fseek(pFile, (sizeof(DWORD)), SEEK_CUR); // object name index
		::fseek(pFile, (sizeof(RwReal)), SEEK_CUR); // visibility distance
		::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // enable trigger
		::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // enable selection
		::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // enable alterable
		::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // enable portal
		::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // enable p.e. collision
		::fseek(pFile, (sizeof(RwBool)), SEEK_CUR); // shadow prop

// 		SkipRange = (sizeof(RwChar) * 128) +	// Obj. Name
// 			(sizeof(RwV3d) * 3) +		// Scaling, Rotation, Translation
// 			(sizeof(RwUInt32)) +		// trigger obj serial id
// 			(sizeof(RwUInt32)) +		// object serial id
// 			(sizeof(DWORD)) +			// object name index
// 			(sizeof(RwReal)) +			// visibility distance
// 			(sizeof(RwBool)) +			// enable trigger
// 			(sizeof(RwBool)) +			// enable selection
// 			(sizeof(RwBool)) +			// enable alterable
// 			(sizeof(RwBool)) +			// enable portal
// 			(sizeof(RwBool)) +			// enable p.e. collision
// 			(sizeof(RwBool));			// shadow prop
// 
// 		::fseek(pFile, SkipRange, SEEK_CUR);

		// PS Map Size
		RwInt32 PSAtomicCnt = 0;
		::fread(&PSAtomicCnt, sizeof(RwInt32), 1, pFile);

		for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
		{
			// Map name
			::fseek(pFile, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, SEEK_CUR);

			// Frustum view window extent
			::fseek(pFile, sizeof(RwReal), SEEK_CUR);

			// Light matrices
			::fseek(pFile, sizeof(RwMatrix), SEEK_CUR);

			// Map size
			RwInt32 Size = 0;
			::fread(&Size, sizeof(RwInt32), 1, pFile);
			::fseek(pFile, sizeof(DWORD) * Size * Size, SEEK_CUR);
		}

		// milepost
		::fread(&EnableFlag, sizeof(RwBool), 1, pFile);
		if(EnableFlag)
		{
			::fseek(pFile, sizeof(RwUInt32), SEEK_CUR);
		}

		// attach light object skip
		RwInt32 iLightObjectNum;
		fread(&iLightObjectNum, sizeof(RwInt32), 1, pFile);

		for (int j = 0; j < iLightObjectNum; ++j)
		{
			fseek(pFile, sizeof(RwChar) * MAX_ATTR_BONE_NAME, SEEK_CUR);

			fseek(pFile, sizeof(RpLightType), SEEK_CUR);
			fseek(pFile, sizeof(RwBool), SEEK_CUR);

			fseek(pFile, sizeof(RwV3d), SEEK_CUR);

			fseek(pFile, sizeof(RwV3d), SEEK_CUR);
			fseek(pFile, sizeof(RwRGBAReal), SEEK_CUR);
			fseek(pFile, sizeof(RwReal), SEEK_CUR);
			fseek(pFile, sizeof(RwReal), SEEK_CUR);

			fseek(pFile, sizeof(RwBool), SEEK_CUR);
			fseek(pFile, sizeof(RwBool), SEEK_CUR);
		}

		// lightmap
		::fseek(pFile, sizeof(RwUInt32), SEEK_CUR); // uiLtMapObjectFlag
		::fseek(pFile, sizeof(RwUInt32), SEEK_CUR); // uiLtMapMaterialFlags

		RwInt32 iAtomicCnt;
		::fread(&iAtomicCnt, sizeof(RwInt32), 1, pFile);

		for (int j = 0; j < iAtomicCnt; ++j)
		{
			RwBool bLitmapExisted;
			::fread(&bLitmapExisted, sizeof(RwBool), 1, pFile);

			if (bLitmapExisted)
			{
				::fseek(pFile, sizeof(RwChar) * 64, SEEK_CUR);
				::fseek(pFile, sizeof(RwChar) * 64, SEEK_CUR);

				RwInt32 iUVCnt;
				::fread(&iUVCnt, sizeof(RwInt32), 1, pFile);
				::fseek(pFile, sizeof(RwTexCoords) * iUVCnt, SEEK_CUR);
			}
		}
	}	

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_Water(FILE* pFile)
{
	RwBool BooleanFlag;
	::fread(&BooleanFlag, sizeof(RwBool), 1, pFile);

	if(BooleanFlag)
	{
		::fseek(pFile,	sizeof(RwReal) +
			sizeof(RwRGBA) +
			(sizeof(DWORD) * 2) +
			(sizeof(RwReal) * 2), SEEK_CUR);

		RwInt32 Size;
		::fread(&Size, sizeof(RwInt32), 1, pFile);
		::fseek(pFile, (sizeof(RwReal) * 2) + sizeof(RwInt32), SEEK_CUR);

		::fread(&BooleanFlag, sizeof(RwBool), 1, pFile);
		if(BooleanFlag)
		{
			::fseek(pFile, sizeof(BYTE) * Size * Size, SEEK_CUR);
		}
	}

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_Shadow(FILE* pFile)
{
	RwBool BooleanFlag;
	::fread(&BooleanFlag, sizeof(RwBool), 1, pFile);
	if(BooleanFlag)
	{
		::fseek(pFile, sizeof(RwRGBA), SEEK_CUR);

		::fseek(pFile, sizeof(RwReal) + sizeof(RwMatrix), SEEK_CUR);

		RwInt32 Size;
		::fread(&Size, sizeof(RwInt32), 1, pFile);
		::fseek(pFile, sizeof(DWORD) * Size * Size, SEEK_CUR);
	}

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_Effects(FILE* pFile)
{
	RwInt32 SkipRange;
	RwInt32 TmpObjNum;

	::fread(&TmpObjNum, sizeof(RwInt32), 1, pFile);

	for(int i = 0; i < TmpObjNum; ++i)
	{
		SkipRange = (sizeof(RwChar) * 128) +	// Obj. Name
			(sizeof(RwV3d) * 3) +		// Scaling, Rotation, Traslation
			(sizeof(RwReal));			// visibility distance

		::fseek(pFile, SkipRange, SEEK_CUR);
	}

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_SE(FILE* pFile)
{
	RwInt32 SkipRange;

	RwInt32 NumSound;
	::fread(&NumSound, sizeof(RwInt32), 1, pFile);
	for(int i = 0; i < NumSound; ++i)
	{
		SkipRange = (sizeof(RwChar) * 32) +		// Obj. Name
			(sizeof(RwReal) * 3) +		// Vol, Radius Min, Max
			(sizeof(RwV3d));			// Translation

		::fseek(pFile, SkipRange, SEEK_CUR);
	}

	// CZ-BGM
	RwInt32 NumSoundBGM;
	::fread(&NumSoundBGM, sizeof(RwInt32), 1, pFile);
	for (int i = 0; i < NumSoundBGM; ++i)
	{
		SkipRange = (sizeof(RwChar) * 32) +		// Obj. Name
			//(sizeof(RwReal)) +		// Vol
			(sizeof(RwReal)) +		// Radius
			(sizeof(RwV3d));			// Translation

		::fseek(pFile, SkipRange, SEEK_CUR);
	}

	return ::ftell(pFile);
}

// CZ-SS
// RwInt32 CNtlWorldFileAccessor::Skip_SS(FILE* pFile)
// {
// 	RwInt32 SkipRange;
// 	RwInt32 SSNum;
// 	::fread(&SSNum, sizeof(RwInt32), 1, pFile);
// 
// 	for(int i = 0; i < SSNum; ++i)
// 	{
// 		SkipRange = sizeof(RwV3d) +
// 			sizeof(RwV3d) +
// 			sizeof(RwRGBAReal) + 
// 			sizeof(RwReal) +
// 			sizeof(RwReal);
// 
// 		::fseek(pFile, SkipRange, SEEK_CUR);
// 	}
// 
// 	return ::ftell(pFile);
// }

RwInt32 CNtlWorldFileAccessor::Skip_SectorCull(FILE* pFile)
{
	::fseek(pFile, sizeof(RwBool), SEEK_CUR);

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_TileTransparency(FILE* pFile)
{
	::fseek(pFile, sizeof(RwBool), SEEK_CUR);

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_Shoreline(FILE* pFile)
{
	RwBool IsShoreline = FALSE;
	::fread(&IsShoreline, sizeof(RwInt32), 1, pFile);
	if(IsShoreline)
	{
		RwInt32 VertNum;
		::fread(&VertNum, sizeof(RwInt32), 1, pFile);
		::fseek(pFile, sizeof(RwInt32), SEEK_CUR);
		::fseek(pFile, sizeof(RwChar) * 32, SEEK_CUR);
		::fseek(pFile, sizeof(RwIm3DVertex) * VertNum, SEEK_CUR);
	}

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_Decal(FILE* pFile)
{
	RwInt32	DecalNum;
	::fread(&DecalNum, sizeof(RwInt32), 1, pFile);

	for(RwInt32 i = 0; i < DecalNum; ++i)
	{
		CNtlPLWorldDecal::SkipToFile(pFile);
	}

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_Plant(FILE* pFile)
{
	// plant
	RwInt32 iPlantSetNum;
	::fread(&iPlantSetNum, sizeof(RwInt32), 1, pFile);
	for (int i = 0; i < iPlantSetNum; ++i)
	{
		// skip set id
		::fseek(pFile, sizeof(RwInt32), SEEK_CUR);

		// skip set num
		RwInt32 iPlantObjNum;			
		::fread(&iPlantObjNum, sizeof(RwInt32), 1, pFile);
		::fseek(pFile, sizeof(RwInt32) * iPlantObjNum, SEEK_CUR);
	}

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_WorldLight(FILE* pFile)
{
	RwInt32 SkipRange;

	// WorldLight_Sphere
	RwInt32 iWLSphere;
	fread(&iWLSphere, sizeof(RwInt32), 1, pFile);
	for (int i = 0; i < iWLSphere; ++i)
	{
		SkipRange = sizeof(RwV3d) + sizeof(RwRGBA) + sizeof(RwReal);

		// skip set id
		::fseek(pFile, SkipRange, SEEK_CUR);
	}

	// WorldLight_Box
	RwInt32 iWLBox;
	fread(&iWLBox, sizeof(RwInt32), 1, pFile);
	for (int i = 0; i < iWLBox; ++i)
	{
		SkipRange = sizeof(RwV3d) + sizeof(RwRGBA) + sizeof(RwV3d) + sizeof(RwV3d);

		// skip set id
		::fseek(pFile, SkipRange, SEEK_CUR);
	}

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_Occluder(FILE* pFile)
{
	RwInt32 SkipRange;
	RwInt32 iOccluderQuad;
	fread(&iOccluderQuad, sizeof(RwInt32), 1, pFile);
	for (int i = 0; i < iOccluderQuad; ++i)
	{
		SkipRange = sizeof(RwUInt32) + sizeof(RwV3d) + sizeof(RwInt32) + (sizeof(RwChar) * 128) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d);

		// skip set id
		::fseek(pFile, SkipRange, SEEK_CUR);
	}

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_HeatHazeObject(FILE* pFile)
{
	RwInt32 SkipRange;
	RwInt32 iHeatHaze;
	fread(&iHeatHaze, sizeof(RwInt32), 1, pFile);
	for (int i = 0; i < iHeatHaze; ++i)
	{
		SkipRange = sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwReal) + sizeof(sNTL_HEATHAZE);

		// skip set id
		::fseek(pFile, SkipRange, SEEK_CUR);
	}

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_LightObject(FILE* pFile)
{
	RwInt32 SkipRange;
	RwInt32 iLightObject;
	fread(&iLightObject, sizeof(RwInt32), 1, pFile);
	for (int i = 0; i < iLightObject; ++i)
	{
		SkipRange = sizeof(RpLightType) + sizeof(RwBool) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwRGBAReal) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwBool) + sizeof(RwBool);

		// skip set id
		::fseek(pFile, SkipRange, SEEK_CUR);
	}

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_DecalVolume(FILE* pFile)
{
	RwInt32 SkipRange;
	RwInt32 iDecalVolume;
	fread(&iDecalVolume, sizeof(RwInt32), 1, pFile);
	for (int i = 0; i < iDecalVolume; ++i)
	{		
		SkipRange = (sizeof(RwChar) * 128) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwUInt32);

		// skip set id
		::fseek(pFile, SkipRange, SEEK_CUR);
	}

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_GameProperty(FILE* pFile)
{
	RwInt32 SkipRange;
	RwInt32 iGameProperty;
	fread(&iGameProperty, sizeof(RwInt32), 1, pFile);
	for (int i = 0; i < iGameProperty; ++i)
	{
		SkipRange = sizeof(RwUInt32) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwUInt32);

		// skip set id
		::fseek(pFile, SkipRange, SEEK_CUR);
	}

	return ::ftell(pFile);
}

RwInt32 CNtlWorldFileAccessor::Skip_Dojo(FILE* pFile)
{
	RwInt32 SkipRange;
	RwInt32 iDojo;
	fread(&iDojo, sizeof(RwInt32), 1, pFile);
	for (int i = 0; i < iDojo; ++i)
	{
		SkipRange = sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwInt32) + sizeof(RwReal) + sizeof(RwReal);
		
		for (int j = 0; j < dPL_DOJO_MAX_LEVEL; ++j)
		{
			SkipRange += sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN;
			SkipRange += sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN;
			SkipRange += sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN;
		}
			
		// skip set id
		::fseek(pFile, SkipRange, SEEK_CUR);
	}

	return ::ftell(pFile);
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif