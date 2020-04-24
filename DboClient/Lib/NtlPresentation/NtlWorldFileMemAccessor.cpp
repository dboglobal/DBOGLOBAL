#include "precomp_ntlpresentation.h"
#include "NtlWorldFileMemAccessor.h"
#include "ntlworldcommon.h"
#include "NtlWorldFieldManager.h"
#include "NtlWorldField.h"
#include "NtlPLWorldDecal.h"
#include "NtlPLDecalVolume.h"
#include "NtlPLPlanet.h"
#include "NtlPLDojo.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifndef dNTL_WORLD_FILE

CNtlWorldFileMemAccessor::CNtlWorldFileMemAccessor(void)
{
}

CNtlWorldFileMemAccessor::~CNtlWorldFileMemAccessor(void)
{
}

BYTE* CNtlWorldFileMemAccessor::Skip_Sector(BYTE* _pFileMem)
{
	_pFileMem = Skip_Material(_pFileMem);
	_pFileMem = Skip_Heightfield(_pFileMem);
	_pFileMem = Skip_Diffuse(_pFileMem);
	_pFileMem = Skip_Doodads(_pFileMem);
	_pFileMem = Skip_Water(_pFileMem);
	_pFileMem = Skip_Shadow(_pFileMem);
	_pFileMem = Skip_Effects(_pFileMem);
	_pFileMem = Skip_SE(_pFileMem);
	// CZ-SS
	//_pFileMem = Skip_SS(_pFileMem);
	_pFileMem = Skip_SectorCull(_pFileMem);
	_pFileMem = Skip_TileTransparency(_pFileMem);
	_pFileMem = Skip_Shoreline(_pFileMem);
	_pFileMem = Skip_Decal(_pFileMem);
	_pFileMem = Skip_Plant(_pFileMem);
	_pFileMem = Skip_WorldLight(_pFileMem);
	_pFileMem = Skip_Occluder(_pFileMem);
	_pFileMem = Skip_HeatHazeObject(_pFileMem);
	_pFileMem = Skip_LightObject(_pFileMem);
	_pFileMem = Skip_Dojo(_pFileMem);

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Skip_SectorIndoor(BYTE* _pFileMem)
{
	_pFileMem = Skip_DoodadsIndoor(_pFileMem);
	_pFileMem = Skip_Water(_pFileMem);
	_pFileMem = Skip_Shadow(_pFileMem);
	_pFileMem = Skip_Effects(_pFileMem);
	_pFileMem = Skip_SE(_pFileMem);
	// CZ-SS
	//_pFileMem = Skip_SS(_pFileMem);
	_pFileMem = Skip_SectorCull(_pFileMem);
	_pFileMem = Skip_TileTransparency(_pFileMem);
	_pFileMem = Skip_Shoreline(_pFileMem);
	_pFileMem = Skip_Decal(_pFileMem);
	_pFileMem = Skip_Plant(_pFileMem);
	_pFileMem = Skip_WorldLight(_pFileMem);
	_pFileMem = Skip_Occluder(_pFileMem);
	_pFileMem = Skip_HeatHazeObject(_pFileMem);
	_pFileMem = Skip_LightObject(_pFileMem);
	_pFileMem = Skip_DecalVolume(_pFileMem);
	_pFileMem = Skip_GameProperty(_pFileMem);
	_pFileMem = Skip_Dojo(_pFileMem);

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Skip_Material(BYTE* _pFileMem)
{
	RwInt32 PosMem = 0;

	// base texture name, rotation, scaling matrices, factors
	PosMem = (sizeof(BYTE) * rwTEXTUREBASENAMELENGTH) + (sizeof(D3DXMATRIX) * 2) + sizeof(RwReal) + sizeof(RwUInt8);

	// detail texture things
	for(int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		CopyMemory(&m_TmpBool, &_pFileMem[PosMem], sizeof(RwBool));
		PosMem += sizeof(RwBool);

		if(m_TmpBool)
		{
			PosMem += sizeof(BYTE) * rwTEXTUREBASENAMELENGTH;

			// layer registered?
			CopyMemory(&m_TmpBool, &_pFileMem[PosMem], sizeof(RwBool));
			PosMem += sizeof(RwBool);

			if(m_TmpBool)
			{
				PosMem += (sizeof(BYTE) * rwTEXTUREBASENAMELENGTH) + (sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);
			}

			PosMem += (sizeof(D3DXMATRIX) * 2) + sizeof(RwReal) + sizeof(RwUInt8);
		}
	}

	return &_pFileMem[PosMem];
}

BYTE* CNtlWorldFileMemAccessor::Skip_Heightfield(BYTE* _pFileMem)
{
	return (_pFileMem + (sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum));
}

BYTE* CNtlWorldFileMemAccessor::Skip_Diffuse(BYTE* _pFileMem)
{
	return (_pFileMem + (sizeof(RwRGBA) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum));
}

BYTE* CNtlWorldFileMemAccessor::Skip_Doodads(BYTE* _pFileMem)
{
// 	RwInt32 SizMemBlock =	(sizeof(RwChar) * 128) +	// Obj. Name
// 							(sizeof(RwV3d) * 3) +		// Scaling, Rotation, Translation
// 							(sizeof(RwUInt32)) +		// trigger obj serial id
// #ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
// 							(sizeof(RwUInt32)) +		// object serial id
// #endif
// 							(sizeof(DWORD)) +			// Obj. name index
// 							(sizeof(RwReal)) +			// visibility distance
// 							(sizeof(RwBool)) +			// enable trigger
// 							(sizeof(RwBool)) +			// enable selection
// 							(sizeof(RwBool)) +			// enable alterable
// 							(sizeof(RwBool)) +			// enable portal
// 							(sizeof(RwBool)) +			// enable path engine collision
// 							(sizeof(RwBool));			// enable shadowprop

	// the number of doodads
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
		//_pFileMem += SizMemBlock;

		_pFileMem += (sizeof(RwChar) * 128); // Obj. Name
		_pFileMem += (sizeof(RwV3d) * 3); // Scaling, Rotation, Translation

		RwUInt32 uiObjectType;
		CopyMemory(&uiObjectType, _pFileMem, sizeof(RwUInt32));
		_pFileMem += sizeof(RwUInt32);

		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			// Object Propperty Range
			RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
				(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
				sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
				(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);
			
			_pFileMem += iPropertyRange;
		}

		_pFileMem += (sizeof(RwUInt32)); // trigger obj serial id
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
		_pFileMem += (sizeof(RwUInt32)); // object serial id
#endif
		_pFileMem += (sizeof(DWORD)); // Obj. name index
		_pFileMem += (sizeof(RwReal)); // visibility distance
		_pFileMem += (sizeof(RwBool)); // enable trigger
		_pFileMem += (sizeof(RwBool)); // enable selection
		_pFileMem += (sizeof(RwBool)); // enable alterable
		_pFileMem += (sizeof(RwBool)); // enable portal
		_pFileMem += (sizeof(RwBool)); // enable path engine collision
		_pFileMem += (sizeof(RwBool)); // enable shadowprop

		// PS Map Size
		RwInt32 PSAtomicCnt = 0;
		CopyMemory(&PSAtomicCnt, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);

		for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
		{
			// Map name
			_pFileMem += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

			// Frustum view window extent
			_pFileMem += (sizeof(RwReal));

			// Light matrices
			_pFileMem += (sizeof(RwMatrix));

			// Map size
			RwInt32 Size = 0;
			CopyMemory(&Size, _pFileMem, sizeof(RwInt32));
			_pFileMem += (sizeof(RwInt32));

			// Map
			_pFileMem += (sizeof(DWORD) * Size * Size);
		}

		// milepost
		RwBool EnableFlg;
		CopyMemory(&EnableFlg, _pFileMem, sizeof(RwBool));
		_pFileMem += sizeof(RwBool);

		if(EnableFlg)
		{
			_pFileMem += sizeof(RwUInt32);
		}

		// attach light object skip
		RwInt32 iLightObjectNum;
		CopyMemory(&iLightObjectNum, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);

		for (int j = 0; j < iLightObjectNum; ++j)
		{
			_pFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

			_pFileMem += sizeof(RpLightType);
			_pFileMem += sizeof(RwBool);

			_pFileMem += sizeof(RwV3d);

			_pFileMem += sizeof(RwV3d);
			_pFileMem += sizeof(RwRGBAReal);
			_pFileMem += sizeof(RwReal);
			_pFileMem += sizeof(RwReal);

			_pFileMem += sizeof(RwBool);
			_pFileMem += sizeof(RwBool);
		}
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Skip_DoodadsIndoor(BYTE* _pFileMem)
{
// 	RwInt32 SizMemBlock =	(sizeof(RwChar) * 128) +	// Obj. Name
// 		(sizeof(RwV3d) * 3) +		// Scaling, Rotation, Translation
// 		(sizeof(RwUInt32)) +		// trigger obj serial id
// 		(sizeof(RwUInt32)) +		// object serial id
// 		(sizeof(DWORD)) +			// Obj. name index
// 		(sizeof(RwReal)) +			// visibility distance
// 		(sizeof(RwBool)) +			// enable trigger
// 		(sizeof(RwBool)) +			// enable selection
// 		(sizeof(RwBool)) +			// enable alterable
// 		(sizeof(RwBool)) +			// enable portal
// 		(sizeof(RwBool)) +			// enable path engine collision
// 		(sizeof(RwBool));			// enable shadowprop

	// the number of doodads
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
		//_pFileMem += SizMemBlock;
		_pFileMem += (sizeof(RwChar) * 128); // Obj. Name
		_pFileMem += (sizeof(RwV3d) * 3); // Scaling, Rotation, Translation

		RwUInt32 uiObjectType;
		CopyMemory(&uiObjectType, _pFileMem, sizeof(RwUInt32));
		_pFileMem += sizeof(RwUInt32);

		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			// Object Propperty Range
			RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
				(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
				sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
				(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

			_pFileMem += iPropertyRange;
		}

		_pFileMem += (sizeof(RwUInt32)); // trigger obj serial id
		_pFileMem += (sizeof(RwUInt32)); // object serial id
		_pFileMem += (sizeof(DWORD)); // Obj. name index
		_pFileMem += (sizeof(RwReal)); // visibility distance
		_pFileMem += (sizeof(RwBool)); // enable trigger
		_pFileMem += (sizeof(RwBool)); // enable selection
		_pFileMem += (sizeof(RwBool)); // enable alterable
		_pFileMem += (sizeof(RwBool)); // enable portal
		_pFileMem += (sizeof(RwBool)); // enable path engine collision
		_pFileMem += (sizeof(RwBool)); // enable shadowprop


		// PS Map Size
		RwInt32 PSAtomicCnt = 0;
		CopyMemory(&PSAtomicCnt, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);

		for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
		{
			// Map name
			_pFileMem += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

			// Frustum view window extent
			_pFileMem += (sizeof(RwReal));

			// Light matrices
			_pFileMem += (sizeof(RwMatrix));

			// Map size
			RwInt32 Size = 0;
			CopyMemory(&Size, _pFileMem, sizeof(RwInt32));
			_pFileMem += (sizeof(RwInt32));

			// Map
			_pFileMem += (sizeof(DWORD) * Size * Size);
		}

		// milepost
		RwBool EnableFlg;
		CopyMemory(&EnableFlg, _pFileMem, sizeof(RwBool));
		_pFileMem += sizeof(RwBool);

		if(EnableFlg)
		{
			_pFileMem += sizeof(RwUInt32);
		}

		// attach light object skip
		RwInt32 iLightObjectNum;
		CopyMemory(&iLightObjectNum, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);

		for (int j = 0; j < iLightObjectNum; ++j)
		{
			_pFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

			_pFileMem += sizeof(RpLightType);
			_pFileMem += sizeof(RwBool);

			_pFileMem += sizeof(RwV3d);

			_pFileMem += sizeof(RwV3d);
			_pFileMem += sizeof(RwRGBAReal);
			_pFileMem += sizeof(RwReal);
			_pFileMem += sizeof(RwReal);
			
			_pFileMem += sizeof(RwBool);
			_pFileMem += sizeof(RwBool);
		}

		// lightmap
		RwUInt32 uiLtMapObjectFlags;
		RwUInt32 uiLtMapMaterialFlags;

		CopyMemory(&uiLtMapObjectFlags, _pFileMem, sizeof(RwUInt32));
		_pFileMem += sizeof(RwUInt32);

		CopyMemory(&uiLtMapMaterialFlags, _pFileMem, sizeof(RwUInt32));
		_pFileMem += sizeof(RwUInt32);

		RwInt32 iAtomicCnt;
		CopyMemory(&iAtomicCnt, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);

		for (int i = 0; i < iAtomicCnt; ++i)
		{
			RwBool bLitmapExisted;
			CopyMemory(&bLitmapExisted, _pFileMem, sizeof(RwBool));
			_pFileMem += sizeof(RwBool);

			if (bLitmapExisted)
			{
				_pFileMem += sizeof(RwChar) * 64;
				_pFileMem += sizeof(RwChar) * 64;

				RwInt32 iUVCnt;
				CopyMemory(&iUVCnt, _pFileMem, sizeof(RwInt32));
				_pFileMem += sizeof(RwInt32);
				_pFileMem += sizeof(RwTexCoords) * iUVCnt;
			}
		}
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Skip_Water(BYTE* _pFileMem)
{
	RwInt32 SizMemBlock;
	RwInt32	Size;

	// water existence
	CopyMemory(&m_TmpBool, _pFileMem, sizeof(RwBool));
	_pFileMem += sizeof(RwBool);

	if(m_TmpBool)
	{
		SizMemBlock =	sizeof(RwReal) +		// height
						sizeof(RwRGBA) +		// RGBA
						(sizeof(DWORD) * 2) +	// renderstate source, dst
						(sizeof(RwReal) * 2);	// rotate, scale

		_pFileMem += SizMemBlock;

		// depth map size
		CopyMemory(&Size, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);

		// depth map height variation, texture squence index
		_pFileMem += (sizeof(RwReal) * 2) + sizeof(RwInt32);

		// depth map existence
		CopyMemory(&m_TmpBool, _pFileMem, sizeof(RwBool));
		_pFileMem += sizeof(RwBool);

		if(m_TmpBool)
		{
			// depth map alpha bits
			_pFileMem += (sizeof(BYTE) * Size * Size);
		}
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Skip_Shadow(BYTE* _pFileMem)
{
	// shadow existence
	CopyMemory(&m_TmpBool, _pFileMem, sizeof(RwBool));
	_pFileMem += sizeof(RwBool);

	if(m_TmpBool)
	{
		_pFileMem += (sizeof(RwRGBA));

		// camera view window x, light direction matrix
		_pFileMem += (sizeof(RwReal) + sizeof(RwMatrix));

		// render2texture size
		CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);

		// argb bits
		_pFileMem += (sizeof(DWORD) * m_TmpRwInt32 * m_TmpRwInt32);
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Skip_WorldLight(BYTE* _pFileMem)
{
	// WorldLight_Sphere
	RwInt32 iNumWLShpere;
	CopyMemory(&iNumWLShpere, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for (int i = 0; i < iNumWLShpere; ++i)
	{
		_pFileMem += sizeof(RwV3d) + sizeof(RwRGBA) + sizeof(RwReal);
	}

	// WorldLight_Box
	RwInt32 iNumWLBox;
	CopyMemory(&iNumWLBox, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for (int i = 0; i < iNumWLBox; ++i)
	{
		_pFileMem += sizeof(RwV3d) + sizeof(RwRGBA) + sizeof(RwV3d) + sizeof(RwV3d);
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Skip_Effects(BYTE* _pFileMem)
{
	RwInt32 SizMemBlock;

	// The number of Effects.
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
		SizMemBlock =	(sizeof(RwChar) * 128) +	// Obj. Name
						(sizeof(RwV3d) * 3) +		// Scaling, Rotation, Traslation
						(sizeof(RwReal));			// visibility distance

		_pFileMem += SizMemBlock;
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Skip_SE(BYTE* _pFileMem)
{
	RwInt32 SizMemBlock;

	// The number of SE.
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
		SizMemBlock =	(sizeof(RwChar) * 32) +		// Obj. Name
						(sizeof(RwReal) * 3) +		// Vol, Radius Min, Max
						(sizeof(RwV3d));			// Translation

		_pFileMem += SizMemBlock;
	}

	// The number of SE BGM
	// CZ-BGM
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
		SizMemBlock =	(sizeof(RwChar) * 32) +		// Obj. Name
			//(sizeof(RwReal)) +					// Vol
			(sizeof(RwReal)) +						// Radius
			(sizeof(RwV3d));						// Translation

		_pFileMem += SizMemBlock;
	}

	return _pFileMem;
}

// CZ-SS
// BYTE* CNtlWorldFileMemAccessor::Skip_SS(BYTE* _pFileMem)
// {
// 	RwInt32 SizMemBlock;
// 
// 	// The number of SS.
// 	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
// 	_pFileMem += sizeof(RwInt32);
// 
// 	for(int i = 0; i < m_TmpRwInt32; ++i)
// 	{
// 		SizMemBlock =	sizeof(RwV3d) +			// Position
// 						sizeof(RwV3d) +			// Rotation
// 						sizeof(RwRGBAReal) +	// Color
// 						sizeof(RwReal) +		// Angle
// 						sizeof(RwReal);			// Radius
// 
// 		_pFileMem += SizMemBlock;
// 	}
// 
// 	return _pFileMem;
// }

BYTE* CNtlWorldFileMemAccessor::Skip_SectorCull(BYTE* _pFileMem)
{
	return (_pFileMem + (sizeof(RwBool)));	
}

BYTE* CNtlWorldFileMemAccessor::Skip_Shoreline(BYTE* _pFileMem)
{
	RwBool IsShoreline;
	CopyMemory(&IsShoreline, _pFileMem, sizeof(RwBool));
	_pFileMem += sizeof(RwBool);

	if(IsShoreline)
	{
		RwInt32 VertNum;
		CopyMemory(&VertNum, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);

		_pFileMem += sizeof(RwInt32);
		_pFileMem += (sizeof(RwChar) * 32);
		_pFileMem += (sizeof(RwIm3DVertex) * VertNum);
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Skip_TileTransparency(BYTE* _pFileMem)
{
	return (_pFileMem + (sizeof(RwBool)));
}

BYTE* CNtlWorldFileMemAccessor::Skip_Decal(BYTE* _pFileMem)
{
	// The number of declas
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for(RwInt32 i = 0; i < m_TmpRwInt32; ++i)
	{
		_pFileMem += sizeof(sDECAL_PROP_PARAM);
	}

	return _pFileMem;
}


BYTE* CNtlWorldFileMemAccessor::Skip_Plant(BYTE* _pFileMem)
{
	RwInt32 iPlantSetNum;	
	CopyMemory(&iPlantSetNum, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for (RwInt32 i = 0; i < iPlantSetNum; ++i)
	{
		RwInt32 iPlantObjNum;
		
		// skip plant set id
		_pFileMem += sizeof(RwInt32);

		// copy plant obj num
		CopyMemory(&iPlantObjNum, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);
		_pFileMem += sizeof(RwInt32) * iPlantObjNum;
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Skip_Occluder(BYTE* _pFileMem)
{
	// Occluder_Quad
	RwInt32 iNumOccluder_Quad;
	CopyMemory(&iNumOccluder_Quad, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for (int i = 0; i < iNumOccluder_Quad; ++i)
	{
		_pFileMem += sizeof(RwUInt32) + sizeof(RwV3d) + sizeof(RwInt32) + (sizeof(RwChar) * 128) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d);
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Skip_HeatHazeObject(BYTE* _pFileMem)
{
	RwInt32 iNumHeatHaze;
	CopyMemory(&iNumHeatHaze, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for (int i = 0; i < iNumHeatHaze; ++i)
	{
		_pFileMem += sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwReal) + sizeof(sNTL_HEATHAZE);
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Skip_Dojo(BYTE* _pFileMem)
{
	RwInt32 iNumDojo;
	CopyMemory(&iNumDojo, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for (int i = 0; i < iNumDojo; ++i)
	{
		_pFileMem += sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwInt32) + sizeof(RwReal) + sizeof(RwReal);
		
		for (int j = 0; j < dPL_DOJO_MAX_LEVEL; ++j)
		{
			_pFileMem += sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN;
			_pFileMem += sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN;
			_pFileMem += sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN;
		}
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Skip_LightObject(BYTE* _pFileMem)
{
	RwInt32 iLightObjectNum;
	CopyMemory(&iLightObjectNum, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for (int i = 0; i < iLightObjectNum; ++i)
	{
		_pFileMem += sizeof(RpLightType) + sizeof(RwBool) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwRGBAReal) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwBool) + sizeof(RwBool);
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Skip_DecalVolume(BYTE* _pFileMem)
{
	RwInt32 iDecalVolumeNum;
	CopyMemory(&iDecalVolumeNum, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for (int i = 0; i < iDecalVolumeNum; ++i)
	{
		_pFileMem += (sizeof(RwChar) * 128) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwUInt32);
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Skip_GameProperty(BYTE* _pFileMem)
{
	RwInt32 iGamePropertyNum;
	CopyMemory(&iGamePropertyNum, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for (int i = 0; i < iGamePropertyNum; ++i)
	{
		_pFileMem += sizeof(RwUInt32) + sizeof(RwV3d) + sizeof(RwV3d) + sizeof(RwUInt32);
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::GetSPosOfMaterial(BYTE* _pFileMem)
{
	return &_pFileMem[0];
}

BYTE* CNtlWorldFileMemAccessor::GetSPosOfHeightfield(BYTE* _pFileMem)
{
	return Skip_Material(_pFileMem);
}

BYTE* CNtlWorldFileMemAccessor::GetSPosOfDoodad(BYTE* _pFileMem)
{
	return NULL;
}

BYTE* CNtlWorldFileMemAccessor::GetSPosOfWater(BYTE* _pFileMem)
{
	return NULL;
}

BYTE* CNtlWorldFileMemAccessor::GetSPosOfShadow(BYTE* _pFileMem)
{
	return NULL;
}

BYTE* CNtlWorldFileMemAccessor::GetSPosOfEffect(BYTE* _pFileMem)
{
	return NULL;
}

BYTE* CNtlWorldFileMemAccessor::GetSPosOfSE(BYTE* _pFileMem)
{
	return NULL;
}

BYTE* CNtlWorldFileMemAccessor::OnSM_SplatTexMaxBlend(BYTE* _pFileMem, FILE* _pFile, RwInt32 _StartIdx2Del)
{
	// texture base layer
	RwInt32 LenCopy =	(sizeof(RwChar) * rwTEXTUREBASENAMELENGTH) +
						(sizeof(D3DXMATRIX) * 2) +
						(sizeof(RwReal)) +
						(sizeof(RwUInt8));

	fwrite(_pFileMem, LenCopy, 1, _pFile);
	_pFileMem += LenCopy;

	// temporary variables
	RwBool		SplatTexExists							= FALSE;
	RwChar		SplatTexName[rwTEXTUREBASENAMELENGTH]	= {0,};
	RwBool		SplatATexExists							= FALSE;
	RwChar		SplatATexName[rwTEXTUREBASENAMELENGTH]	= {0,};
	BYTE*		pSplatAlphaTex							= NTL_NEW BYTE [dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum];
	D3DXMATRIX	SplatTexMat[2];
	RwReal		SplatAng;
	RwUInt8		SplatAdd;

	// texture detail layer
	for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		CopyMemory(&SplatTexExists, _pFileMem, sizeof(RwBool));
		_pFileMem += sizeof(RwBool);

		if(SplatTexExists)
		{
			CopyMemory(SplatTexName, _pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);
			_pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

			CopyMemory(&SplatATexExists, _pFileMem, sizeof(RwBool));
			_pFileMem += sizeof(RwBool);

			if(SplatATexExists)
			{
				CopyMemory(SplatATexName, _pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);
				_pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

				CopyMemory(pSplatAlphaTex, _pFileMem, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);
				_pFileMem += sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum;
			}

			CopyMemory(&SplatTexMat[0], _pFileMem, sizeof(D3DXMATRIX));
			_pFileMem += sizeof(D3DXMATRIX);

			CopyMemory(&SplatTexMat[1], _pFileMem, sizeof(D3DXMATRIX));
			_pFileMem += sizeof(D3DXMATRIX);

			CopyMemory(&SplatAng, _pFileMem, sizeof(RwReal));
			_pFileMem += sizeof(RwReal);

			CopyMemory(&SplatAdd, _pFileMem, sizeof(RwUInt8));
			_pFileMem += sizeof(RwUInt8); 
		}

		if(i <= _StartIdx2Del)
		{
			fwrite(&SplatTexExists, sizeof(RwBool), 1, _pFile);
			if(SplatTexExists)
			{
				fwrite(SplatTexName, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, _pFile);

				m_TmpBool = FALSE;
				fwrite(&m_TmpBool, sizeof(RwBool), 1, _pFile);

				fwrite(&SplatTexMat[0], sizeof(D3DXMATRIX), 1, _pFile);
				fwrite(&SplatTexMat[1], sizeof(D3DXMATRIX), 1, _pFile);
				fwrite(&SplatAng, sizeof(RwReal), 1, _pFile);
				fwrite(&SplatAdd, sizeof(RwUInt8), 1, _pFile);
			}
		}
		else
		{
			fwrite(&SplatTexExists, sizeof(RwBool), 1, _pFile);

			if(SplatTexExists)
			{
				fwrite(SplatTexName, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, _pFile);
				fwrite(&SplatATexExists, sizeof(RwBool), 1, _pFile);

				if(SplatATexExists)
				{
					fwrite(SplatATexName, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, _pFile);
					fwrite(pSplatAlphaTex, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum, 1, _pFile);
				}

				fwrite(&SplatTexMat[0], sizeof(D3DXMATRIX), 1, _pFile);
				fwrite(&SplatTexMat[1], sizeof(D3DXMATRIX), 1, _pFile);
				fwrite(&SplatAng, sizeof(RwReal), 1, _pFile);
				fwrite(&SplatAdd, sizeof(RwUInt8), 1, _pFile);
			}
		}
	}

	NTL_ARRAY_DELETE(pSplatAlphaTex);

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::OnSM_SplatTexMinBlend(BYTE* _pFileMem, FILE* _pFile, RwBool _Indices2Del[dNTL_SPLAT_LAYER_NUM])
{
	// texture base layer
	RwInt32 LenCopy =	(sizeof(RwChar) * rwTEXTUREBASENAMELENGTH) +
						(sizeof(D3DXMATRIX) * 2) +
						(sizeof(RwReal)) +
						(sizeof(RwUInt8));

	fwrite(_pFileMem, LenCopy, 1, _pFile);
	_pFileMem += LenCopy;

	// temporary variables
	RwBool		SplatTexExists							= FALSE;
	RwChar		SplatTexName[rwTEXTUREBASENAMELENGTH]	= {0,};
	RwBool		SplatATexExists							= FALSE;
	RwChar		SplatATexName[rwTEXTUREBASENAMELENGTH]	= {0,};
	BYTE*		pSplatAlphaTex							= NTL_NEW BYTE [dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum];
	D3DXMATRIX	SplatTexMat[2];
	RwReal		SplatAng;
	RwUInt8		SplatAdd;

	// texture detail layer
	for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		CopyMemory(&SplatTexExists, _pFileMem, sizeof(RwBool));
		_pFileMem += sizeof(RwBool);

		if(SplatTexExists)
		{
			CopyMemory(SplatTexName, _pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);
			_pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

			CopyMemory(&SplatATexExists, _pFileMem, sizeof(RwBool));
			_pFileMem += sizeof(RwBool);

			if(SplatATexExists)
			{
				CopyMemory(SplatATexName, _pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);
				_pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

				CopyMemory(pSplatAlphaTex, _pFileMem, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);
				_pFileMem += sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum;
			}

			CopyMemory(&SplatTexMat[0], _pFileMem, sizeof(D3DXMATRIX));
			_pFileMem += sizeof(D3DXMATRIX);

			CopyMemory(&SplatTexMat[1], _pFileMem, sizeof(D3DXMATRIX));
			_pFileMem += sizeof(D3DXMATRIX);

			CopyMemory(&SplatAng, _pFileMem, sizeof(RwReal));
			_pFileMem += sizeof(RwReal);

			CopyMemory(&SplatAdd, _pFileMem, sizeof(RwUInt8));
			_pFileMem += sizeof(RwUInt8); 
		}

		if(_Indices2Del[i])
		{
			fwrite(&SplatTexExists, sizeof(RwBool), 1, _pFile);

			if(SplatTexExists)
			{
				fwrite(SplatTexName, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, _pFile);

				m_TmpBool = FALSE;
				fwrite(&m_TmpBool, sizeof(RwBool), 1, _pFile);

				fwrite(&SplatTexMat[0], sizeof(D3DXMATRIX), 1, _pFile);
				fwrite(&SplatTexMat[1], sizeof(D3DXMATRIX), 1, _pFile);
				fwrite(&SplatAng, sizeof(RwReal), 1, _pFile);
				fwrite(&SplatAdd, sizeof(RwUInt8), 1, _pFile);
			}
		}
		else
		{
			fwrite(&SplatTexExists, sizeof(RwBool), 1, _pFile);

			if(SplatTexExists)
			{
				fwrite(SplatTexName, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, _pFile);
				fwrite(&SplatATexExists, sizeof(RwBool), 1, _pFile);

				if(SplatATexExists)
				{
					fwrite(SplatATexName, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, _pFile);
					fwrite(pSplatAlphaTex, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum, 1, _pFile);
				}

				fwrite(&SplatTexMat[0], sizeof(D3DXMATRIX), 1, _pFile);
				fwrite(&SplatTexMat[1], sizeof(D3DXMATRIX), 1, _pFile);
				fwrite(&SplatAng, sizeof(RwReal), 1, _pFile);
				fwrite(&SplatAdd, sizeof(RwUInt8), 1, _pFile);
			}
		}
	}

	NTL_ARRAY_DELETE(pSplatAlphaTex);

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_Sector(BYTE* _pFileMem, FILE* _pFile)
{
	_pFileMem = Save_Material(_pFileMem, _pFile);
	_pFileMem = Save_Heightfield(_pFileMem, _pFile);
	_pFileMem = Save_Diffuse(_pFileMem, _pFile);
	_pFileMem = Save_Doodads(_pFileMem, _pFile);
	_pFileMem = Save_Water(_pFileMem, _pFile);
	_pFileMem = Save_Shadow(_pFileMem, _pFile);
	_pFileMem = Save_Effects(_pFileMem, _pFile);
	_pFileMem = Save_SE(_pFileMem, _pFile);
	// CZ-SS
	//_pFileMem = Save_SS(_pFileMem, _pFile);
	_pFileMem = Save_SectorCull(_pFileMem, _pFile);
	_pFileMem = Save_TileTransparency(_pFileMem, _pFile);
	_pFileMem = Save_Shoreline(_pFileMem, _pFile);
	_pFileMem = Save_Decal(_pFileMem, _pFile);
	_pFileMem = Save_Plant(_pFileMem, _pFile);
	_pFileMem = Save_WorldLight(_pFileMem, _pFile);
	_pFileMem = Save_Occluder(_pFileMem, _pFile);
	_pFileMem = Save_HeatHazeObject(_pFileMem, _pFile);
	_pFileMem = Save_LightObject(_pFileMem, _pFile);
	_pFileMem = Save_Dojo(_pFileMem, _pFile);

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_SectorIndoor(BYTE* _pFileMem, FILE* _pFile)
{
	_pFileMem = Save_DoodadsIndoor(_pFileMem, _pFile);
	_pFileMem = Save_Water(_pFileMem, _pFile);
	_pFileMem = Save_Shadow(_pFileMem, _pFile);
	_pFileMem = Save_Effects(_pFileMem, _pFile);
	_pFileMem = Save_SE(_pFileMem, _pFile);
	// CZ-SS
	//_pFileMem = Save_SS(_pFileMem, _pFile);
	_pFileMem = Save_SectorCull(_pFileMem, _pFile);
	_pFileMem = Save_TileTransparency(_pFileMem, _pFile);
	_pFileMem = Save_Shoreline(_pFileMem, _pFile);
	_pFileMem = Save_Decal(_pFileMem, _pFile);
	_pFileMem = Save_Plant(_pFileMem, _pFile);
	_pFileMem = Save_WorldLight(_pFileMem, _pFile);
	_pFileMem = Save_Occluder(_pFileMem, _pFile);
	_pFileMem = Save_HeatHazeObject(_pFileMem, _pFile);
	_pFileMem = Save_LightObject(_pFileMem, _pFile);
	_pFileMem = Save_DecalVolume(_pFileMem, _pFile);
	_pFileMem = Save_GameProperty(_pFileMem, _pFile);
	_pFileMem = Save_Dojo(_pFileMem, _pFile);

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_TileTransparency(BYTE* _pFileMem, FILE* _pFile)
{
	fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
	return (_pFileMem + (sizeof(RwBool)));
}

BYTE* CNtlWorldFileMemAccessor::Save_SectorCull(BYTE* _pFileMem, FILE* _pFile)
{
	fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
	return (_pFileMem + (sizeof(RwBool)));
}

BYTE* CNtlWorldFileMemAccessor::Save_Material(BYTE* _pFileMem, FILE* _pFile)
{
	// texture base layer
	RwInt32 LenCopy =	(sizeof(RwChar) * rwTEXTUREBASENAMELENGTH) +
						(sizeof(D3DXMATRIX) * 2) +
						(sizeof(RwReal)) +
						(sizeof(RwUInt8));
								
	fwrite(_pFileMem, LenCopy, 1, _pFile);
	_pFileMem += LenCopy;

	// texture detail layer
	for(RwInt32 i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		CopyMemory(&m_TmpBool, _pFileMem, sizeof(RwBool));
		fwrite(&m_TmpBool, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		if(m_TmpBool)
		{
			fwrite(_pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, _pFile);
			_pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;
			
			CopyMemory(&m_TmpBool, _pFileMem, sizeof(RwBool));
			fwrite(&m_TmpBool, sizeof(RwBool), 1, _pFile);
			_pFileMem += sizeof(RwBool);

			if(m_TmpBool)
			{
				fwrite(_pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, _pFile);
				_pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

				fwrite(_pFileMem, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum, 1, _pFile);
				_pFileMem += sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum;
			}

			LenCopy = (sizeof(D3DXMATRIX) * 2) + sizeof(RwReal) + sizeof(RwUInt8);
			fwrite(_pFileMem, LenCopy, 1, _pFile);
			_pFileMem += LenCopy;
		}
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_Heightfield(BYTE* _pFileMem, FILE* _pFile)
{
	RwInt32 LenHeightfieldFileSize = (sizeof(RwV3d) * dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum);
		
	fwrite(_pFileMem, LenHeightfieldFileSize, 1, _pFile);
	_pFileMem += LenHeightfieldFileSize;

	return _pFileMem;
}

VOID CNtlWorldFileMemAccessor::Save_Heightfield(BYTE* _pFileMemSrc, BYTE* _pFileMemDst, FILE* _pFile)
{
	RwInt32 NumVert = dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;
	RwV3d	PosDst, PosSrc;
	RwV3d*	pBufVert = NTL_NEW RwV3d [NumVert];

	for(RwInt32 i = 0; i < NumVert; ++i)
	{
		CopyMemory(&PosSrc, _pFileMemSrc, sizeof(RwV3d));
		CopyMemory(&PosDst, _pFileMemDst, sizeof(RwV3d));
		_pFileMemSrc += sizeof(RwV3d);
		_pFileMemDst += sizeof(RwV3d);

		PosDst.y = PosSrc.y;
		CopyMemory(&pBufVert[i], &PosDst, sizeof(RwV3d));
	}

	fwrite(pBufVert, sizeof(RwV3d) * NumVert, 1, _pFile);

	NTL_ARRAY_DELETE(pBufVert);
}

BYTE* CNtlWorldFileMemAccessor::Save_Diffuse(BYTE* _pFileMem, FILE* _pFile)
{
	RwInt32 NumVert = dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;

	fwrite(_pFileMem, sizeof(RwRGBA) * NumVert, 1, _pFile);	

	return (_pFileMem + (sizeof(RwRGBA) * NumVert));
}

VOID CNtlWorldFileMemAccessor::Save_Doodads_Num_PEC(BYTE* _pFileMem, FILE* _pFile)
{	
	RwInt32		NumPECollision = 0;

	// the number of doodads
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
		RwChar		StrName[128];
		RwV3d		PosScaling;
		RwV3d		PosRotation;
		RwV3d		PosTranslation;

		CopyMemory(StrName, _pFileMem, (sizeof(RwChar) * 128));			_pFileMem += (sizeof(RwChar) * 128);
		CopyMemory(&PosScaling, _pFileMem, sizeof(RwV3d));				_pFileMem += sizeof(RwV3d);
		CopyMemory(&PosRotation, _pFileMem, sizeof(RwV3d));				_pFileMem += sizeof(RwV3d);
		CopyMemory(&PosTranslation, _pFileMem, sizeof(RwV3d));			_pFileMem += sizeof(RwV3d);

		RwUInt32 uiObjectType;
		CopyMemory(&uiObjectType, _pFileMem, sizeof(RwUInt32));
		_pFileMem += sizeof(RwUInt32);

		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			// Object Propperty Range
			RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
				(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
				sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
				(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) +  + (sizeof(RwInt32) * 5);

			_pFileMem += iPropertyRange;
		}

		_pFileMem += (sizeof(RwUInt32)); // trigger obj serial id
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
		_pFileMem += (sizeof(RwUInt32)); // object serial id
#endif
		_pFileMem += (sizeof(DWORD)); // Obj. name index
		_pFileMem += (sizeof(RwReal)); // visibility distance
		_pFileMem += (sizeof(RwBool)); // enable trigger
		_pFileMem += (sizeof(RwBool)); // enable selection
		_pFileMem += (sizeof(RwBool)); // enable alterable
		_pFileMem += (sizeof(RwBool)); // enable portal

		RwBool		FlgEnablePECollision;
		CopyMemory(&FlgEnablePECollision, _pFileMem, sizeof(RwBool));	_pFileMem += sizeof(RwBool); // enable path engine collision

		_pFileMem += (sizeof(RwBool)); // enable shadowprop

		if(FlgEnablePECollision)
		{
			++NumPECollision;
		}

		// PS Map Size
		RwInt32 PSAtomicCnt = 0;
		CopyMemory(&PSAtomicCnt, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);

		for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
		{
			// Map name
			_pFileMem += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

			// Frustum view window extent
			_pFileMem += (sizeof(RwReal));

			// Light matrices
			_pFileMem += (sizeof(RwMatrix));

			// Map size
			RwInt32 Size = 0;
			CopyMemory(&Size, _pFileMem, sizeof(RwInt32));
			_pFileMem += (sizeof(RwInt32));

			// Map
			_pFileMem += (sizeof(DWORD) * Size * Size);
		}

		// milepost
		RwBool EnableFlg;
		CopyMemory(&EnableFlg, _pFileMem, sizeof(RwBool));
		_pFileMem += sizeof(RwBool);

		if(EnableFlg)
		{
			_pFileMem += sizeof(RwUInt32);
		}

		// attach light object skip
		RwInt32 iLightObjectNum;
		CopyMemory(&iLightObjectNum, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);

		for (int j = 0; j < iLightObjectNum; ++j)
		{
			_pFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

			_pFileMem += sizeof(RpLightType);
			_pFileMem += sizeof(RwBool);

			_pFileMem += sizeof(RwV3d);

			_pFileMem += sizeof(RwV3d);
			_pFileMem += sizeof(RwRGBAReal);
			_pFileMem += sizeof(RwReal);
			_pFileMem += sizeof(RwReal);

			_pFileMem += sizeof(RwBool);
			_pFileMem += sizeof(RwBool);
		}
	}

	fwrite(&NumPECollision, sizeof(RwInt32), 1, _pFile);	
}

VOID CNtlWorldFileMemAccessor::Save_Doodads_Num(BYTE* _pPosSrcMem, BYTE* _pPosDstMem, FILE* _pFile)
{	
	RwInt32		NumTriObjDst	= 0;
	RwInt32		NumNonTriObjSrc = 0;
	RwInt32		NumResult;

	// src
	CopyMemory(&m_TmpRwInt32, _pPosSrcMem, sizeof(RwInt32));
	_pPosSrcMem += sizeof(RwInt32);

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
		_pPosSrcMem += (sizeof(RwChar) * 128); // Obj. Name
		_pPosSrcMem += (sizeof(RwV3d) * 3); // Scaling, Rotation, Translation

		RwUInt32 uiObjectType;
		CopyMemory(&uiObjectType, _pPosSrcMem, sizeof(RwUInt32));
		_pPosSrcMem += sizeof(RwUInt32);

		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			// Object Propperty Range
			RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
				(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
				sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
				(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

			_pPosSrcMem += iPropertyRange;
		}

		_pPosSrcMem += (sizeof(RwUInt32)); // trigger obj serial id
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
		_pPosSrcMem += (sizeof(RwUInt32)); // object serial id
#endif
		_pPosSrcMem += (sizeof(DWORD)); // Obj. name index
		_pPosSrcMem += (sizeof(RwReal)); // visibility distance
		
		RwBool		FlgEnableTrigger;
		CopyMemory(&FlgEnableTrigger, _pPosSrcMem, sizeof(RwBool));
		_pPosSrcMem += (sizeof(RwBool)); // enable trigger
		
		_pPosSrcMem += (sizeof(RwBool)); // enable selection
		_pPosSrcMem += (sizeof(RwBool)); // enable alterable
		_pPosSrcMem += (sizeof(RwBool)); // enable portal
		_pPosSrcMem += (sizeof(RwBool)); // enable path engine collision
		_pPosSrcMem += (sizeof(RwBool)); // enable shadowprop
		
		if(!FlgEnableTrigger)
		{
			++NumNonTriObjSrc;
		}

		// PS Map Size
		RwInt32 PSAtomicCnt = 0;
		CopyMemory(&PSAtomicCnt, _pPosSrcMem, sizeof(RwInt32));
		_pPosSrcMem += sizeof(RwInt32);

		for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
		{
			// Map name
			_pPosSrcMem += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

			// Frustum view window extent
			_pPosSrcMem += (sizeof(RwReal));

			// Light matrices
			_pPosSrcMem += (sizeof(RwMatrix));

			// Map size
			RwInt32 Size = 0;
			CopyMemory(&Size, _pPosSrcMem, sizeof(RwInt32));
			_pPosSrcMem += (sizeof(RwInt32));

			// Map
			_pPosSrcMem += (sizeof(DWORD) * Size * Size);
		}

		// milepost
		RwBool EnableFlg;
		CopyMemory(&EnableFlg, _pPosSrcMem, sizeof(RwBool));
		_pPosSrcMem += sizeof(RwBool);

		if(EnableFlg)
		{
			_pPosSrcMem += sizeof(RwUInt32);
		}

		// attach light object skip
		RwInt32 iLightObjectNum;
		CopyMemory(&iLightObjectNum, _pPosSrcMem, sizeof(RwInt32));
		_pPosSrcMem += sizeof(RwInt32);

		for (int j = 0; j < iLightObjectNum; ++j)
		{
			_pPosSrcMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

			_pPosSrcMem += sizeof(RpLightType);
			_pPosSrcMem += sizeof(RwBool);

			_pPosSrcMem += sizeof(RwV3d);

			_pPosSrcMem += sizeof(RwV3d);
			_pPosSrcMem += sizeof(RwRGBAReal);
			_pPosSrcMem += sizeof(RwReal);
			_pPosSrcMem += sizeof(RwReal);

			_pPosSrcMem += sizeof(RwBool);
			_pPosSrcMem += sizeof(RwBool);
		}
	}

	// dst.
	CopyMemory(&m_TmpRwInt32, _pPosDstMem, sizeof(RwInt32));
	_pPosDstMem += sizeof(RwInt32);

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
		_pPosDstMem += (sizeof(RwChar) * 128); // Obj. Name
		_pPosDstMem += (sizeof(RwV3d) * 3); // Scaling, Rotation, Translation

		RwUInt32 uiObjectType;
		CopyMemory(&uiObjectType, _pPosDstMem, sizeof(RwUInt32));
		_pPosDstMem += sizeof(RwUInt32);

		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			// Object Propperty Range
			RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
				(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
				sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
				(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

			_pPosDstMem += iPropertyRange;
		}

		_pPosDstMem += (sizeof(RwUInt32)); // trigger obj serial id
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
		_pPosDstMem += (sizeof(RwUInt32)); // object serial id
#endif
		_pPosDstMem += (sizeof(DWORD)); // Obj. name index
		_pPosDstMem += (sizeof(RwReal)); // visibility distance

		RwBool		FlgEnableTrigger;
		CopyMemory(&FlgEnableTrigger, _pPosDstMem, sizeof(RwBool));
		_pPosDstMem += (sizeof(RwBool)); // enable trigger

		_pPosDstMem += (sizeof(RwBool)); // enable selection
		_pPosDstMem += (sizeof(RwBool)); // enable alterable
		_pPosDstMem += (sizeof(RwBool)); // enable portal
		_pPosDstMem += (sizeof(RwBool)); // enable path engine collision
		_pPosDstMem += (sizeof(RwBool)); // enable shadowprop

		if(FlgEnableTrigger)
		{
			++NumTriObjDst;
		}

		// PS Map Size
		RwInt32 PSAtomicCnt = 0;
		CopyMemory(&PSAtomicCnt, _pPosDstMem, sizeof(RwInt32));
		_pPosDstMem += sizeof(RwInt32);

		for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
		{
			// Map name
			_pPosDstMem += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

			// Frustum view window extent
			_pPosDstMem += (sizeof(RwReal));

			// Light matrices
			_pPosDstMem += (sizeof(RwMatrix));

			// Map size
			RwInt32 Size = 0;
			CopyMemory(&Size, _pPosDstMem, sizeof(RwInt32));
			_pPosDstMem += (sizeof(RwInt32));

			// Map
			_pPosDstMem += (sizeof(DWORD) * Size * Size);
		}

		// milepost
		RwBool EnableFlg;
		CopyMemory(&EnableFlg, _pPosDstMem, sizeof(RwBool));
		_pPosDstMem += sizeof(RwBool);

		if(EnableFlg)
		{
			_pPosDstMem += sizeof(RwUInt32);
		}

		// attach light object skip
		RwInt32 iLightObjectNum;
		CopyMemory(&iLightObjectNum, _pPosDstMem, sizeof(RwInt32));
		_pPosDstMem += sizeof(RwInt32);

		for (int j = 0; j < iLightObjectNum; ++j)
		{
			_pPosDstMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

			_pPosDstMem += sizeof(RpLightType);
			_pPosDstMem += sizeof(RwBool);

			_pPosDstMem += sizeof(RwV3d);

			_pPosDstMem += sizeof(RwV3d);
			_pPosDstMem += sizeof(RwRGBAReal);
			_pPosDstMem += sizeof(RwReal);
			_pPosDstMem += sizeof(RwReal);

			_pPosDstMem += sizeof(RwBool);
			_pPosDstMem += sizeof(RwBool);
		}
	}

	NumResult = NumTriObjDst + NumNonTriObjSrc;
	fwrite(&NumResult, sizeof(RwInt32), 1, _pFile);	
}

VOID CNtlWorldFileMemAccessor::Save_Doodads_PEC(BYTE* _pFileMem, FILE* _pFile)
{
	// the number of doodads
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
		RwChar		StrName[128];
		RwV3d		PosScaling;
		RwV3d		PosRotation;
		RwV3d		PosTranslation;

		CopyMemory(StrName, _pFileMem, (sizeof(RwChar) * 128));			_pFileMem += (sizeof(RwChar) * 128);
		CopyMemory(&PosScaling, _pFileMem, sizeof(RwV3d));				_pFileMem += sizeof(RwV3d);
		CopyMemory(&PosRotation, _pFileMem, sizeof(RwV3d));				_pFileMem += sizeof(RwV3d);
		CopyMemory(&PosTranslation, _pFileMem, sizeof(RwV3d));			_pFileMem += sizeof(RwV3d);

		RwUInt32 uiObjectType;
		CopyMemory(&uiObjectType, _pFileMem, sizeof(RwUInt32));
		_pFileMem += sizeof(RwUInt32);

		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			// Object Propperty Range
			RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
				(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
				sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
				(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

			_pFileMem += iPropertyRange;
		}

		_pFileMem += (sizeof(RwUInt32)); // trigger obj serial id
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
		_pFileMem += (sizeof(RwUInt32)); // object serial id
#endif
		_pFileMem += (sizeof(DWORD)); // Obj. name index
		_pFileMem += (sizeof(RwReal)); // visibility distance
		_pFileMem += (sizeof(RwBool)); // enable trigger
		_pFileMem += (sizeof(RwBool)); // enable selection
		_pFileMem += (sizeof(RwBool)); // enable alterable
		_pFileMem += (sizeof(RwBool)); // enable portal

		RwBool		FlgEnablePECollision;
		CopyMemory(&FlgEnablePECollision, _pFileMem, sizeof(RwBool));	_pFileMem += sizeof(RwBool); // enable path engine collision
		
		_pFileMem += (sizeof(RwBool)); // enable shadowprop

		if(FlgEnablePECollision)
		{
			fwrite(StrName, (sizeof(RwChar) * 128), 1, _pFile);
			fwrite(&PosScaling, sizeof(RwV3d), 1, _pFile);
			fwrite(&PosRotation, sizeof(RwV3d), 1, _pFile);
			fwrite(&PosTranslation, sizeof(RwV3d), 1, _pFile);
		}

		// PS Map Size
		RwInt32 PSAtomicCnt = 0;
		CopyMemory(&PSAtomicCnt, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);

		for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
		{
			// Map name
			_pFileMem += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

			// Frustum view window extent
			_pFileMem += (sizeof(RwReal));

			// Light matrices
			_pFileMem += (sizeof(RwMatrix));

			// Map size
			RwInt32 Size = 0;
			CopyMemory(&Size, _pFileMem, sizeof(RwInt32));
			_pFileMem += (sizeof(RwInt32));

			// Map
			_pFileMem += (sizeof(DWORD) * Size * Size);
		}

		// milepost
		RwBool EnableFlg;
		CopyMemory(&EnableFlg, _pFileMem, sizeof(RwBool));
		_pFileMem += sizeof(RwBool);

		if(EnableFlg)
		{
			_pFileMem += sizeof(RwUInt32);
		}

		// attach light object skip
		RwInt32 iLightObjectNum;
		CopyMemory(&iLightObjectNum, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);

		for (int j = 0; j < iLightObjectNum; ++j)
		{
			_pFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

			_pFileMem += sizeof(RpLightType);
			_pFileMem += sizeof(RwBool);

			_pFileMem += sizeof(RwV3d);

			_pFileMem += sizeof(RwV3d);
			_pFileMem += sizeof(RwRGBAReal);
			_pFileMem += sizeof(RwReal);
			_pFileMem += sizeof(RwReal);

			_pFileMem += sizeof(RwBool);
			_pFileMem += sizeof(RwBool);
		}
	}
}

VOID CNtlWorldFileMemAccessor::Save_Doodads_NonTrigger(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField/* = -1*/, RwInt32 IdxDstField/* = -1*/, CNtlWorldFieldManager* pFieldMgr/* = NULL*/)
{
	RwChar		StrName[128];
	RwV3d		PosScaling;
	RwV3d		PosRotation;
	RwV3d		PosTranslation;
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
	RwUInt32	ValObjectID;
#endif
	RwUInt32	ValTriggerObjID;
	RwReal		LenVisibility;
	RwBool		FlgEnableTrigger;
	RwBool		FlgEnableSelection;
	RwBool		FlgEnableAlterable;
	RwBool		FlgEnablePortal;
	RwBool		FlgEnablePECollision;
	RwBool		FlgEnableShadowProp;
	RwBool		EnableFlg;
	RwV3d		PosSrcLocal, PosDstWorld;
	DWORD		ObjNameIdx;
	
	// Object Propperty Range
	RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
		(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
		sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
		(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

	RwUInt32	uiObjectType;
	BYTE*		pbyObjectProperty = NTL_NEW BYTE [iPropertyRange];

	// the number of doodads
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
		CopyMemory(StrName, _pFileMem, (sizeof(RwChar) * 128));			_pFileMem += (sizeof(RwChar) * 128);
		CopyMemory(&PosScaling, _pFileMem, sizeof(RwV3d));				_pFileMem += sizeof(RwV3d);
		CopyMemory(&PosRotation, _pFileMem, sizeof(RwV3d));				_pFileMem += sizeof(RwV3d);
		CopyMemory(&PosTranslation, _pFileMem, sizeof(RwV3d));			_pFileMem += sizeof(RwV3d);
		
		CopyMemory(&uiObjectType, _pFileMem, sizeof(RwUInt32));			_pFileMem += sizeof(RwUInt32);
		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			// Object Propperty Range
			CopyMemory(pbyObjectProperty, _pFileMem, iPropertyRange);	_pFileMem += iPropertyRange;
		}
		
		CopyMemory(&ValTriggerObjID, _pFileMem, sizeof(RwUInt32));		_pFileMem += sizeof(RwUInt32);
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
		CopyMemory(&ValObjectID, _pFileMem, sizeof(RwUInt32));			_pFileMem += sizeof(RwUInt32);
#endif
		CopyMemory(&ObjNameIdx, _pFileMem, sizeof(DWORD));				_pFileMem += sizeof(DWORD);
		CopyMemory(&LenVisibility, _pFileMem, sizeof(RwReal));			_pFileMem += sizeof(RwReal);
		CopyMemory(&FlgEnableTrigger, _pFileMem, sizeof(RwBool));		_pFileMem += sizeof(RwBool);
		CopyMemory(&FlgEnableSelection, _pFileMem, sizeof(RwBool));		_pFileMem += sizeof(RwBool);
		CopyMemory(&FlgEnableAlterable, _pFileMem, sizeof(RwBool));		_pFileMem += sizeof(RwBool);
		CopyMemory(&FlgEnablePortal, _pFileMem, sizeof(RwBool));		_pFileMem += sizeof(RwBool);
		CopyMemory(&FlgEnablePECollision, _pFileMem, sizeof(RwBool));	_pFileMem += sizeof(RwBool);
		CopyMemory(&FlgEnableShadowProp, _pFileMem, sizeof(RwBool));	_pFileMem += sizeof(RwBool);

		if(!FlgEnableTrigger)
		{
			fwrite(StrName, (sizeof(RwChar) * 128), 1, _pFile);
			fwrite(&PosScaling, sizeof(RwV3d), 1, _pFile);
			fwrite(&PosRotation, sizeof(RwV3d), 1, _pFile);

			// relative coordinates
			if(pFieldMgr)
			{
				CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
				RwV3d PosSrc = pFields[IdxSrcField].GetSPos();
				RwV3d PosDst = pFields[IdxDstField].GetSPos();

				PosSrcLocal.x = PosTranslation.x - PosSrc.x;
				PosSrcLocal.y = PosTranslation.y;
				PosSrcLocal.z = PosTranslation.z - PosSrc.z;
				PosSrcLocal.x = fabs(PosSrcLocal.x);
				PosSrcLocal.z = fabs(PosSrcLocal.z);

				PosDstWorld.x = PosSrcLocal.x + PosDst.x;
				PosDstWorld.z = PosSrcLocal.z + PosDst.z;
				PosDstWorld.y = PosTranslation.y;

				CopyMemory(&PosTranslation, &PosDstWorld, sizeof(RwV3d));
			}

			fwrite(&PosTranslation, sizeof(RwV3d), 1, _pFile);

			fwrite(&uiObjectType, sizeof(RwUInt32), 1, _pFile);
			if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
				uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
			{
				fwrite(pbyObjectProperty, iPropertyRange, 1, _pFile);
			}

			fwrite(&ValTriggerObjID, sizeof(RwUInt32), 1, _pFile);

#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
			ValObjectID = dGET_WORLD_PARAM()->WorldObjectSerialID;
			++dGET_WORLD_PARAM()->WorldObjectSerialID;
			fwrite(&ValObjectID, sizeof(RwUInt32), 1, _pFile);
#endif

			fwrite(&ObjNameIdx, sizeof(DWORD), 1, _pFile);
			fwrite(&LenVisibility, sizeof(RwReal), 1, _pFile);
			fwrite(&FlgEnableTrigger, sizeof(RwBool), 1, _pFile);
			fwrite(&FlgEnableSelection, sizeof(RwBool), 1, _pFile);
			fwrite(&FlgEnableAlterable, sizeof(RwBool), 1, _pFile);
			fwrite(&FlgEnablePortal, sizeof(RwBool), 1, _pFile);
			fwrite(&FlgEnablePECollision, sizeof(RwBool), 1, _pFile);
			fwrite(&FlgEnableShadowProp, sizeof(RwBool), 1, _pFile);

			// object PS map
			RwInt32 PSAtomicCnt = 0;
			CopyMemory(&PSAtomicCnt, _pFileMem, sizeof(RwInt32));

			::fwrite(&PSAtomicCnt, sizeof(RwInt32), 1, _pFile);
			_pFileMem += sizeof(RwInt32);

			for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
			{
				// Map name
				fwrite(_pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, _pFile);
				_pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

				// Frustum view window extent
				fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
				_pFileMem += sizeof(RwReal);

				// Light matrices
				fwrite(_pFileMem, sizeof(RwMatrix), 1, _pFile);
				_pFileMem += (sizeof(RwMatrix));

				// Map size
				RwInt32 Size = 0;
				CopyMemory(&Size, _pFileMem, sizeof(RwInt32));
				fwrite(&Size, sizeof(RwInt32), 1, _pFile);
				_pFileMem += (sizeof(RwInt32));

				// Map
				fwrite(_pFileMem, sizeof(DWORD) * Size * Size, 1, _pFile);
				_pFileMem += (sizeof(DWORD) * Size * Size);
			}

			// milepost
			CopyMemory(&EnableFlg, _pFileMem, sizeof(RwBool));
			fwrite(&EnableFlg, sizeof(RwBool), 1, _pFile);
			_pFileMem += (sizeof(RwBool));

			if(EnableFlg)
			{
				fwrite(_pFileMem, sizeof(RwUInt32), 1, _pFile);
				_pFileMem += (sizeof(RwUInt32));
			}

			// attach light object skip
			RwInt32 iLightObjectNum;
			CopyMemory(&iLightObjectNum, _pFileMem, sizeof(RwInt32));
			_pFileMem += sizeof(RwInt32);
			fwrite(&iLightObjectNum, sizeof(RwInt32), 1, _pFile);

			for (int j = 0; j < iLightObjectNum; ++j)
			{
				fwrite(_pFileMem, sizeof(RwChar) * MAX_ATTR_BONE_NAME, 1, _pFile);
				_pFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

				fwrite(_pFileMem, sizeof(RpLightType), 1, _pFile);
				_pFileMem += sizeof(RpLightType);
				fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
				_pFileMem += sizeof(RwBool);

				fwrite(_pFileMem, sizeof(RwV3d), 1, _pFile);
				_pFileMem += sizeof(RwV3d);

				fwrite(_pFileMem, sizeof(RwV3d), 1, _pFile);
				_pFileMem += sizeof(RwV3d);
				fwrite(_pFileMem, sizeof(RwRGBAReal), 1, _pFile);
				_pFileMem += sizeof(RwRGBAReal);
				fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
				_pFileMem += sizeof(RwReal);
				fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
				_pFileMem += sizeof(RwReal);

				fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
				_pFileMem += sizeof(RwBool);
				fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
				_pFileMem += sizeof(RwBool);
			}
		}
		else
		{
			// PS Map Size
			RwInt32 PSAtomicCnt = 0;
			CopyMemory(&PSAtomicCnt, _pFileMem, sizeof(RwInt32));
			_pFileMem += sizeof(RwInt32);

			for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
			{
				// Map name
				_pFileMem += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

				// Frustum view window extent
				_pFileMem += (sizeof(RwReal));

				// Light matrices
				_pFileMem += (sizeof(RwMatrix));

				// Map size
				RwInt32 Size = 0;
				CopyMemory(&Size, _pFileMem, sizeof(RwInt32));
				_pFileMem += (sizeof(RwInt32));

				// Map
				_pFileMem += (sizeof(DWORD) * Size * Size);
			}

			// milepost
			CopyMemory(&EnableFlg, _pFileMem, sizeof(RwBool));
			_pFileMem += sizeof(RwBool);

			if(EnableFlg)
			{
				_pFileMem += sizeof(RwUInt32);
			}

			// attach light object skip
			RwInt32 iLightObjectNum;
			CopyMemory(&iLightObjectNum, _pFileMem, sizeof(RwInt32));
			_pFileMem += sizeof(RwInt32);

			for (int j = 0; j < iLightObjectNum; ++j)
			{
				_pFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

				_pFileMem += sizeof(RpLightType);
				_pFileMem += sizeof(RwBool);

				_pFileMem += sizeof(RwV3d);

				_pFileMem += sizeof(RwV3d);
				_pFileMem += sizeof(RwRGBAReal);
				_pFileMem += sizeof(RwReal);
				_pFileMem += sizeof(RwReal);

				_pFileMem += sizeof(RwBool);
				_pFileMem += sizeof(RwBool);
			}
		}
	}

	NTL_ARRAY_DELETE(pbyObjectProperty);
}

VOID CNtlWorldFileMemAccessor::Save_Doodads_Trigger(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField/* = -1*/, RwInt32 IdxDstField/* = -1*/, CNtlWorldFieldManager* pFieldMgr/* = NULL*/)
{
	RwChar		StrName[128];
	RwV3d		PosScaling;
	RwV3d		PosRotation;
	RwV3d		PosTranslation;
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
	RwUInt32	ValObjectID;
#endif
	RwUInt32	ValTriggerObjID;
	RwReal		LenVisibility;
	RwBool		FlgEnableTrigger;
	RwBool		FlgEnableSelection;
	RwBool		FlgEnableAlterable;
	RwBool		FlgEnablePortal;
	RwBool		FlgEnablePECollision;
	RwBool		FlgEnableShadowProp;
	RwBool		EnableFlg;
//	RwV3d		PosSrcLocal, PosDstWorld;
	DWORD		ObjNameIdx;

	// Object Propperty Range
	RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
		(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
		sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
		(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

	RwUInt32	uiObjectType;
	BYTE*		pbyObjectProperty = NTL_NEW BYTE [iPropertyRange];

	// the number of doodads
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
		CopyMemory(StrName, _pFileMem, (sizeof(RwChar) * 128));			_pFileMem += (sizeof(RwChar) * 128);
		CopyMemory(&PosScaling, _pFileMem, sizeof(RwV3d));				_pFileMem += sizeof(RwV3d);
		CopyMemory(&PosRotation, _pFileMem, sizeof(RwV3d));				_pFileMem += sizeof(RwV3d);
		CopyMemory(&PosTranslation, _pFileMem, sizeof(RwV3d));			_pFileMem += sizeof(RwV3d);		

		CopyMemory(&uiObjectType, _pFileMem, sizeof(RwUInt32));			_pFileMem += sizeof(RwUInt32);
		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			// Object Propperty Range
			CopyMemory(pbyObjectProperty, _pFileMem, iPropertyRange);	_pFileMem += iPropertyRange;
		}
		
		CopyMemory(&ValTriggerObjID, _pFileMem, sizeof(RwUInt32));		_pFileMem += sizeof(RwUInt32);
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
		CopyMemory(&ValObjectID, _pFileMem, sizeof(RwUInt32));			_pFileMem += sizeof(RwUInt32);
#endif
		CopyMemory(&ObjNameIdx, _pFileMem, sizeof(DWORD));				_pFileMem += sizeof(DWORD);
		CopyMemory(&LenVisibility, _pFileMem, sizeof(RwReal));			_pFileMem += sizeof(RwReal);
		CopyMemory(&FlgEnableTrigger, _pFileMem, sizeof(RwBool));		_pFileMem += sizeof(RwBool);
		CopyMemory(&FlgEnableSelection, _pFileMem, sizeof(RwBool));		_pFileMem += sizeof(RwBool);
		CopyMemory(&FlgEnableAlterable, _pFileMem, sizeof(RwBool));		_pFileMem += sizeof(RwBool);
		CopyMemory(&FlgEnablePortal, _pFileMem, sizeof(RwBool));		_pFileMem += sizeof(RwBool);
		CopyMemory(&FlgEnablePECollision, _pFileMem, sizeof(RwBool));	_pFileMem += sizeof(RwBool);
		CopyMemory(&FlgEnableShadowProp, _pFileMem, sizeof(RwBool));	_pFileMem += sizeof(RwBool);

		if(FlgEnableTrigger)
		{
			fwrite(StrName, (sizeof(RwChar) * 128), 1, _pFile);
			fwrite(&PosScaling, sizeof(RwV3d), 1, _pFile);
			fwrite(&PosRotation, sizeof(RwV3d), 1, _pFile);

			// relative coordinates : Dest 저장일 경우는 Position이 이동 되면 안된다.
// 			if(pFieldMgr)
// 			{
// 				CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
// 				RwV3d PosSrc = pFields[IdxSrcField].GetSPos();
// 				RwV3d PosDst = pFields[IdxDstField].GetSPos();
// 
// 				PosSrcLocal.x = PosTranslation.x - PosSrc.x;
// 				PosSrcLocal.y = PosTranslation.y;
// 				PosSrcLocal.z = PosTranslation.z - PosSrc.z;
// 				PosSrcLocal.x = fabs(PosSrcLocal.x);
// 				PosSrcLocal.z = fabs(PosSrcLocal.z);
// 
// 				PosDstWorld.x = PosSrcLocal.x + PosDst.x;
// 				PosDstWorld.z = PosSrcLocal.z + PosDst.z;
// 				PosDstWorld.y = PosTranslation.y;
// 
// 				CopyMemory(&PosTranslation, &PosDstWorld, sizeof(RwV3d));
// 			}

			fwrite(&PosTranslation, sizeof(RwV3d), 1, _pFile);

			fwrite(&uiObjectType, sizeof(RwUInt32), 1, _pFile);
			if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
				uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
			{
				fwrite(pbyObjectProperty, iPropertyRange, 1, _pFile);
			}

			fwrite(&ValTriggerObjID, sizeof(RwUInt32), 1, _pFile);

#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
			ValObjectID = dGET_WORLD_PARAM()->WorldObjectSerialID;
			++dGET_WORLD_PARAM()->WorldObjectSerialID;
			fwrite(&ValObjectID, sizeof(RwUInt32), 1, _pFile);
#endif

			fwrite(&ObjNameIdx, sizeof(DWORD), 1, _pFile);
			fwrite(&LenVisibility, sizeof(RwReal), 1, _pFile);
			fwrite(&FlgEnableTrigger, sizeof(RwBool), 1, _pFile);
			fwrite(&FlgEnableSelection, sizeof(RwBool), 1, _pFile);
			fwrite(&FlgEnableAlterable, sizeof(RwBool), 1, _pFile);
			fwrite(&FlgEnablePortal, sizeof(RwBool), 1, _pFile);
			fwrite(&FlgEnablePECollision, sizeof(RwBool), 1, _pFile);
			fwrite(&FlgEnableShadowProp, sizeof(RwBool), 1, _pFile);

			// object PS map
			RwInt32 PSAtomicCnt = 0;
			CopyMemory(&PSAtomicCnt, _pFileMem, sizeof(RwInt32));

			::fwrite(&PSAtomicCnt, sizeof(RwInt32), 1, _pFile);
			_pFileMem += sizeof(RwInt32);

			for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
			{
				// Map name
				fwrite(_pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, _pFile);
				_pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

				// Frustum view window extent
				fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
				_pFileMem += sizeof(RwReal);

				// Light matrices
				fwrite(_pFileMem, sizeof(RwMatrix), 1, _pFile);
				_pFileMem += (sizeof(RwMatrix));

				// Map size
				RwInt32 Size = 0;
				CopyMemory(&Size, _pFileMem, sizeof(RwInt32));
				fwrite(&Size, sizeof(RwInt32), 1, _pFile);
				_pFileMem += (sizeof(RwInt32));

				// Map
				fwrite(_pFileMem, sizeof(DWORD) * Size * Size, 1, _pFile);
				_pFileMem += (sizeof(DWORD) * Size * Size);
			}

			// milepost
			CopyMemory(&EnableFlg, _pFileMem, sizeof(RwBool));
			fwrite(&EnableFlg, sizeof(RwBool), 1, _pFile);
			_pFileMem += (sizeof(RwBool));

			if(EnableFlg)
			{
				fwrite(_pFileMem, sizeof(RwUInt32), 1, _pFile);
				_pFileMem += (sizeof(RwUInt32));
			}

			// attach light object skip
			RwInt32 iLightObjectNum;
			CopyMemory(&iLightObjectNum, _pFileMem, sizeof(RwInt32));
			_pFileMem += sizeof(RwInt32);
			fwrite(&iLightObjectNum, sizeof(RwInt32), 1, _pFile);

			for (int j = 0; j < iLightObjectNum; ++j)
			{
				fwrite(_pFileMem, sizeof(RwChar) * MAX_ATTR_BONE_NAME, 1, _pFile);
				_pFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

				fwrite(_pFileMem, sizeof(RpLightType), 1, _pFile);
				_pFileMem += sizeof(RpLightType);
				fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
				_pFileMem += sizeof(RwBool);

				fwrite(_pFileMem, sizeof(RwV3d), 1, _pFile);
				_pFileMem += sizeof(RwV3d);

				fwrite(_pFileMem, sizeof(RwV3d), 1, _pFile);
				_pFileMem += sizeof(RwV3d);
				fwrite(_pFileMem, sizeof(RwRGBAReal), 1, _pFile);
				_pFileMem += sizeof(RwRGBAReal);
				fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
				_pFileMem += sizeof(RwReal);
				fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
				_pFileMem += sizeof(RwReal);

				fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
				_pFileMem += sizeof(RwBool);
				fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
				_pFileMem += sizeof(RwBool);
			}
		}
		else
		{
			// PS Map Size
			RwInt32 PSAtomicCnt = 0;
			CopyMemory(&PSAtomicCnt, _pFileMem, sizeof(RwInt32));
			_pFileMem += sizeof(RwInt32);

			for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
			{
				// Map name
				_pFileMem += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

				// Frustum view window extent
				_pFileMem += (sizeof(RwReal));

				// Light matrices
				_pFileMem += (sizeof(RwMatrix));

				// Map size
				RwInt32 Size = 0;
				CopyMemory(&Size, _pFileMem, sizeof(RwInt32));
				_pFileMem += (sizeof(RwInt32));

				// Map
				_pFileMem += (sizeof(DWORD) * Size * Size);
			}

			// milepost
			CopyMemory(&EnableFlg, _pFileMem, sizeof(RwBool));
			_pFileMem += sizeof(RwBool);

			if(EnableFlg)
			{
				_pFileMem += sizeof(RwUInt32);
			}

			// attach light object skip
			RwInt32 iLightObjectNum;
			CopyMemory(&iLightObjectNum, _pFileMem, sizeof(RwInt32));
			_pFileMem += sizeof(RwInt32);

			for (int j = 0; j < iLightObjectNum; ++j)
			{
				_pFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

				_pFileMem += sizeof(RpLightType);
				_pFileMem += sizeof(RwBool);

				_pFileMem += sizeof(RwV3d);

				_pFileMem += sizeof(RwV3d);
				_pFileMem += sizeof(RwRGBAReal);
				_pFileMem += sizeof(RwReal);
				_pFileMem += sizeof(RwReal);

				_pFileMem += sizeof(RwBool);
				_pFileMem += sizeof(RwBool);
			}
		}
	}

	NTL_ARRAY_DELETE(pbyObjectProperty);
}

BYTE* CNtlWorldFileMemAccessor::Save_Doodads_VisibilityDist(BYTE* _pFileMem, FILE* _pFile, RwReal VisibilityDist, RwReal Min, RwReal Max)
{
	// the number of doodads
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	::fwrite(&m_TmpRwInt32, sizeof(RwInt32), 1, _pFile);
	_pFileMem += sizeof(RwInt32);

	for(RwInt32 i = 0; i < m_TmpRwInt32; ++i)
	{
		// Obj. name
		::fwrite(_pFileMem, sizeof(RwChar) * 128, 1, _pFile);
		_pFileMem += sizeof(RwChar) * 128;

		// Scaling, Rotation, Translation
		::fwrite(_pFileMem, sizeof(RwV3d) * 3, 1, _pFile);
		_pFileMem += sizeof(RwV3d) * 3;

		RwUInt32 uiObjectType;
		CopyMemory(&uiObjectType, _pFileMem, sizeof(RwUInt32));
		::fwrite(&uiObjectType, sizeof(RwUInt32), 1, _pFile);
		_pFileMem += sizeof(RwUInt32);

		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			// Object Propperty Range
			RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
				(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
				sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
				(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

			::fwrite(_pFileMem, iPropertyRange, 1, _pFile);
			_pFileMem += iPropertyRange;
		}

		// trigger obj serial id
		::fwrite(_pFileMem, sizeof(RwUInt32), 1, _pFile);
		_pFileMem += sizeof(RwUInt32);

#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
		// object serial id
		::fwrite(_pFileMem, sizeof(RwUInt32), 1, _pFile);
		_pFileMem += sizeof(RwUInt32);
#endif

		// trigger obj object name index
		::fwrite(_pFileMem, sizeof(DWORD), 1, _pFile);
		_pFileMem += sizeof(DWORD);

		// visibility distance
		CLAMP(VisibilityDist, 70, 600);
		CLAMP(Min, 70, 600);
		CLAMP(Max, 70, 600);

		RwReal CurDist;
		CopyMemory(&CurDist, _pFileMem, sizeof(RwReal));
		if(CurDist > Min && CurDist < Max)
		{
			::fwrite(&VisibilityDist, sizeof(RwReal), 1, _pFile);
		}
		else
		{
			::fwrite(&CurDist, sizeof(RwReal), 1, _pFile);
		}
		_pFileMem += sizeof(RwReal);

		// enable trigger
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable selection
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable alterable
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable portal
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable path engine collision
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable ShadowProp
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// doodads psm
		RwInt32 PSAtomicCnt = 0;
		CopyMemory(&PSAtomicCnt, _pFileMem, sizeof(RwInt32));

		::fwrite(&PSAtomicCnt, sizeof(RwInt32), 1, _pFile);
		_pFileMem += sizeof(RwInt32);

		for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
		{
			// Map name
			fwrite(_pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, _pFile);
			_pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

			// Frustum view window extent
			fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
			_pFileMem += sizeof(RwReal);

			// Light matrices
			fwrite(_pFileMem, sizeof(RwMatrix), 1, _pFile);
			_pFileMem += (sizeof(RwMatrix));

			// Map size
			RwInt32 Size = 0;
			CopyMemory(&Size, _pFileMem, sizeof(RwInt32));
			fwrite(&Size, sizeof(RwInt32), 1, _pFile);
			_pFileMem += (sizeof(RwInt32));

			// Map
			fwrite(_pFileMem, sizeof(DWORD) * Size * Size, 1, _pFile);
			_pFileMem += (sizeof(DWORD) * Size * Size);
		}

		// milepost
		RwBool EnableFlg;
		CopyMemory(&EnableFlg, _pFileMem, sizeof(RwBool));
		fwrite(&EnableFlg, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		if(EnableFlg)
		{
			fwrite(_pFileMem, sizeof(RwUInt32), 1, _pFile);
			_pFileMem += sizeof(RwUInt32);
		}

		// attach light object skip
		RwInt32 iLightObjectNum;
		CopyMemory(&iLightObjectNum, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);
		fwrite(&iLightObjectNum, sizeof(RwInt32), 1, _pFile);

		for (int j = 0; j < iLightObjectNum; ++j)
		{
			fwrite(_pFileMem, sizeof(RwChar) * MAX_ATTR_BONE_NAME, 1, _pFile);
			_pFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

			fwrite(_pFileMem, sizeof(RpLightType), 1, _pFile);
			_pFileMem += sizeof(RpLightType);
			fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
			_pFileMem += sizeof(RwBool);

			fwrite(_pFileMem, sizeof(RwV3d), 1, _pFile);
			_pFileMem += sizeof(RwV3d);

			fwrite(_pFileMem, sizeof(RwV3d), 1, _pFile);
			_pFileMem += sizeof(RwV3d);
			fwrite(_pFileMem, sizeof(RwRGBAReal), 1, _pFile);
			_pFileMem += sizeof(RwRGBAReal);
			fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
			_pFileMem += sizeof(RwReal);
			fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
			_pFileMem += sizeof(RwReal);

			fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
			_pFileMem += sizeof(RwBool);
			fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
			_pFileMem += sizeof(RwBool);
		}
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_Doodads_ProofsInvalidSectorLink(BYTE* _pFileMem, FILE* _pFile, RwInt32 iFieldIdx, RwInt32 iSectorCnt)
{
	RwBBox bboxSector;
	bboxSector.inf.x = static_cast<RwReal>((iFieldIdx % dGET_WORLD_PARAM()->WorldFieldNum) * dGET_WORLD_PARAM()->WorldFieldSize) - dGET_WORLD_PARAM()->WorldSizeHalf;
	bboxSector.inf.z = static_cast<RwReal>((iFieldIdx / dGET_WORLD_PARAM()->WorldFieldNum) * dGET_WORLD_PARAM()->WorldFieldSize) - dGET_WORLD_PARAM()->WorldSizeHalf;

	if (iSectorCnt & 1)
	{
		bboxSector.inf.x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
	}
	if (iSectorCnt & 2)
	{
		bboxSector.inf.z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
	}

	bboxSector.sup.x = bboxSector.inf.x + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
	bboxSector.sup.z = bboxSector.inf.z + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);

	// the number of doodads
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	::fwrite(&m_TmpRwInt32, sizeof(RwInt32), 1, _pFile);
	_pFileMem += sizeof(RwInt32);

	for(RwInt32 i = 0; i < m_TmpRwInt32; ++i)
	{
		// Obj. name
		RwChar acName[128];
		CopyMemory(acName, _pFileMem, sizeof(RwChar) * 128);
		::fwrite(_pFileMem, sizeof(RwChar) * 128, 1, _pFile);
		_pFileMem += sizeof(RwChar) * 128;

		// Scaling, Rotation, Translation
		::fwrite(_pFileMem, sizeof(RwV3d), 1, _pFile);
		_pFileMem += sizeof(RwV3d);

		::fwrite(_pFileMem, sizeof(RwV3d), 1, _pFile);
		_pFileMem += sizeof(RwV3d);

		RwV3d vPos;
		CopyMemory(&vPos, _pFileMem, sizeof(RwV3d));
		_pFileMem += sizeof(RwV3d);

		if (!(vPos.x >= bboxSector.inf.x &&
			vPos.z >= bboxSector.inf.z &&
			vPos.x < bboxSector.sup.x &&
			vPos.z < bboxSector.sup.z))
		{
			vPos.x = bboxSector.inf.x + (dGET_WORLD_PARAM()->WorldSectorSize / 2);
			vPos.z = bboxSector.inf.z + (dGET_WORLD_PARAM()->WorldSectorSize / 2);

			DBO_TRACE((FALSE), "ProofsInvalidSectorLink : " << acName << "(" << vPos.x << "," << vPos.y << "," << vPos.z << ")");
		}
		::fwrite(&vPos, sizeof(RwV3d), 1, _pFile);

		RwUInt32 uiObjectType;
		CopyMemory(&uiObjectType, _pFileMem, sizeof(RwUInt32));
		::fwrite(&uiObjectType, sizeof(RwUInt32), 1, _pFile);
		_pFileMem += sizeof(RwUInt32);

		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			// Object Propperty Range
			RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
				(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
				sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
				(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

			::fwrite(_pFileMem, iPropertyRange, 1, _pFile);
			_pFileMem += iPropertyRange;
		}

		// trigger obj serial id
		::fwrite(_pFileMem, sizeof(RwUInt32), 1, _pFile);
		_pFileMem += sizeof(RwUInt32);

#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
		// object serial id
		::fwrite(_pFileMem, sizeof(RwUInt32), 1, _pFile);
		_pFileMem += sizeof(RwUInt32);
#endif

		// trigger obj object name index
		::fwrite(_pFileMem, sizeof(DWORD), 1, _pFile);
		_pFileMem += sizeof(DWORD);

		// visibility distance
		::fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
		_pFileMem += sizeof(RwReal);

		// enable trigger
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable selection
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable alterable
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable portal
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable path engine collision
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable ShadowProp
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// doodads psm
		RwInt32 PSAtomicCnt = 0;
		CopyMemory(&PSAtomicCnt, _pFileMem, sizeof(RwInt32));

		::fwrite(&PSAtomicCnt, sizeof(RwInt32), 1, _pFile);
		_pFileMem += sizeof(RwInt32);

		for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
		{
			// Map name
			fwrite(_pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, _pFile);
			_pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

			// Frustum view window extent
			fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
			_pFileMem += sizeof(RwReal);

			// Light matrices
			fwrite(_pFileMem, sizeof(RwMatrix), 1, _pFile);
			_pFileMem += (sizeof(RwMatrix));

			// Map size
			RwInt32 Size = 0;
			CopyMemory(&Size, _pFileMem, sizeof(RwInt32));
			fwrite(&Size, sizeof(RwInt32), 1, _pFile);
			_pFileMem += (sizeof(RwInt32));

			// Map
			fwrite(_pFileMem, sizeof(DWORD) * Size * Size, 1, _pFile);
			_pFileMem += (sizeof(DWORD) * Size * Size);
		}

		// milepost
		RwBool EnableFlg;
		CopyMemory(&EnableFlg, _pFileMem, sizeof(RwBool));
		fwrite(&EnableFlg, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		if(EnableFlg)
		{
			fwrite(_pFileMem, sizeof(RwUInt32), 1, _pFile);
			_pFileMem += sizeof(RwUInt32);
		}

		// attach light object skip
		RwInt32 iLightObjectNum;
		CopyMemory(&iLightObjectNum, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);
		fwrite(&iLightObjectNum, sizeof(RwInt32), 1, _pFile);

		for (int j = 0; j < iLightObjectNum; ++j)
		{
			fwrite(_pFileMem, sizeof(RwChar) * MAX_ATTR_BONE_NAME, 1, _pFile);
			_pFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

			fwrite(_pFileMem, sizeof(RpLightType), 1, _pFile);
			_pFileMem += sizeof(RpLightType);
			fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
			_pFileMem += sizeof(RwBool);

			fwrite(_pFileMem, sizeof(RwV3d), 1, _pFile);
			_pFileMem += sizeof(RwV3d);

			fwrite(_pFileMem, sizeof(RwV3d), 1, _pFile);
			_pFileMem += sizeof(RwV3d);
			fwrite(_pFileMem, sizeof(RwRGBAReal), 1, _pFile);
			_pFileMem += sizeof(RwRGBAReal);
			fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
			_pFileMem += sizeof(RwReal);
			fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
			_pFileMem += sizeof(RwReal);

			fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
			_pFileMem += sizeof(RwBool);
			fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
			_pFileMem += sizeof(RwBool);
		}
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_Doodads(BYTE* _pFileMem, FILE* _pFile)
{
// 	RwInt32 SizMemBlock =	(sizeof(RwChar) * 128) +	// Obj. Name
// 							(sizeof(RwV3d) * 3) +		// Scaling, Rotation, Translation
// 							(sizeof(RwUInt32)) +		// trigger obj serial id
// #ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
// 							(sizeof(RwUInt32)) +		// object serial id
// #endif
// 							(sizeof(DWORD)) +			// trigger obj name index
// 							(sizeof(RwReal)) +			// visibility distance
// 							(sizeof(RwBool)) +			// enable trigger
// 							(sizeof(RwBool)) +			// enable selection
// 							(sizeof(RwBool)) +			// enable alterable
// 							(sizeof(RwBool)) +			// enable portal
// 							(sizeof(RwBool)) +			// enable path engine collision
// 							(sizeof(RwBool));			// enable shadowprop

	// the number of doodads
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	::fwrite(&m_TmpRwInt32, sizeof(RwInt32), 1, _pFile);
	_pFileMem += sizeof(RwInt32);

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
// 		::fwrite(_pFileMem, SizMemBlock, 1, _pFile);
// 		_pFileMem += SizMemBlock;

		// Obj. name
		::fwrite(_pFileMem, sizeof(RwChar) * 128, 1, _pFile);
		_pFileMem += sizeof(RwChar) * 128;

		// Scaling, Rotation, Translation
		::fwrite(_pFileMem, sizeof(RwV3d) * 3, 1, _pFile);
		_pFileMem += sizeof(RwV3d) * 3;

		RwUInt32 uiObjectType;		
		CopyMemory(&uiObjectType, _pFileMem, sizeof(RwUInt32));
		::fwrite(&uiObjectType, sizeof(RwUInt32), 1, _pFile);
		_pFileMem += sizeof(RwUInt32);

		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			// Object Propperty Range
			RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
				(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
				sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
				(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

			::fwrite(_pFileMem, iPropertyRange, 1, _pFile);
			_pFileMem += iPropertyRange;
		}

		// trigger obj serial id
		::fwrite(_pFileMem, sizeof(RwUInt32), 1, _pFile);
		_pFileMem += sizeof(RwUInt32);

#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
		// object serial id
		::fwrite(_pFileMem, sizeof(RwUInt32), 1, _pFile);
		_pFileMem += sizeof(RwUInt32);
#endif

		// trigger obj object name index
		::fwrite(_pFileMem, sizeof(DWORD), 1, _pFile);
		_pFileMem += sizeof(DWORD);

		// visibility distance
		::fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
		_pFileMem += sizeof(RwReal);
		
		// enable trigger
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable selection
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable alterable
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable portal
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable path engine collision
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable ShadowProp
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		RwInt32 PSAtomicCnt = 0;
		CopyMemory(&PSAtomicCnt, _pFileMem, sizeof(RwInt32));
		
		::fwrite(&PSAtomicCnt, sizeof(RwInt32), 1, _pFile);
		_pFileMem += sizeof(RwInt32);

		for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
		{
			// Map name
			fwrite(_pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, _pFile);
			_pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

			// Frustum view window extent
			fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
			_pFileMem += sizeof(RwReal);

			// Light matrices
			fwrite(_pFileMem, sizeof(RwMatrix), 1, _pFile);
			_pFileMem += (sizeof(RwMatrix));

			// Map size
			RwInt32 Size = 0;
			CopyMemory(&Size, _pFileMem, sizeof(RwInt32));
			fwrite(&Size, sizeof(RwInt32), 1, _pFile);
			_pFileMem += (sizeof(RwInt32));

			// Map
			fwrite(_pFileMem, sizeof(DWORD) * Size * Size, 1, _pFile);
			_pFileMem += (sizeof(DWORD) * Size * Size);
		}

		// milepost
		RwBool EnableFlg;
		CopyMemory(&EnableFlg, _pFileMem, sizeof(RwBool));
		fwrite(&EnableFlg, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);
		if(EnableFlg)
		{
			fwrite(_pFileMem, sizeof(RwUInt32), 1, _pFile);
			_pFileMem += sizeof(RwUInt32);
		}

		// attach light object skip
		RwInt32 iLightObjectNum;
		CopyMemory(&iLightObjectNum, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);
		fwrite(&iLightObjectNum, sizeof(RwInt32), 1, _pFile);

		for (int j = 0; j < iLightObjectNum; ++j)
		{
			fwrite(_pFileMem, sizeof(RwChar) * MAX_ATTR_BONE_NAME, 1, _pFile);
			_pFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

			fwrite(_pFileMem, sizeof(RpLightType), 1, _pFile);
			_pFileMem += sizeof(RpLightType);
			fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
			_pFileMem += sizeof(RwBool);

			fwrite(_pFileMem, sizeof(RwV3d), 1, _pFile);
			_pFileMem += sizeof(RwV3d);

			fwrite(_pFileMem, sizeof(RwV3d), 1, _pFile);
			_pFileMem += sizeof(RwV3d);
			fwrite(_pFileMem, sizeof(RwRGBAReal), 1, _pFile);
			_pFileMem += sizeof(RwRGBAReal);
			fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
			_pFileMem += sizeof(RwReal);
			fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
			_pFileMem += sizeof(RwReal);

			fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
			_pFileMem += sizeof(RwBool);
			fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
			_pFileMem += sizeof(RwBool);
		}
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_DoodadsIndoor(BYTE* _pFileMem, FILE* _pFile)
{
// 	RwInt32 SizMemBlock =	(sizeof(RwChar) * 128) +	// Obj. Name
// 		(sizeof(RwV3d) * 3) +		// Scaling, Rotation, Translation
// 		(sizeof(RwUInt32)) +		// trigger obj serial id
// 		(sizeof(RwUInt32)) +		// object serial id
// 		(sizeof(DWORD)) +			// trigger obj name index
// 		(sizeof(RwReal)) +			// visibility distance
// 		(sizeof(RwBool)) +			// enable trigger
// 		(sizeof(RwBool)) +			// enable selection
// 		(sizeof(RwBool)) +			// enable alterable
// 		(sizeof(RwBool)) +			// enable portal
// 		(sizeof(RwBool)) +			// enable path engine collision
// 		(sizeof(RwBool));			// enable shadowprop

	// the number of doodads
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	::fwrite(&m_TmpRwInt32, sizeof(RwInt32), 1, _pFile);
	_pFileMem += sizeof(RwInt32);

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
// 		::fwrite(_pFileMem, SizMemBlock, 1, _pFile);
// 		_pFileMem += SizMemBlock;

		// Obj. name
		::fwrite(_pFileMem, sizeof(RwChar) * 128, 1, _pFile);
		_pFileMem += sizeof(RwChar) * 128;

		// Scaling, Rotation, Translation
		::fwrite(_pFileMem, sizeof(RwV3d) * 3, 1, _pFile);
		_pFileMem += sizeof(RwV3d) * 3;

		RwUInt32 uiObjectType;
		CopyMemory(&uiObjectType, _pFileMem, sizeof(RwUInt32));
		::fwrite(&uiObjectType, sizeof(RwUInt32), 1, _pFile);
		_pFileMem += sizeof(RwUInt32);

		if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
			uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
		{
			// Object Propperty Range
			RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
				(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
				sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
				(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

			::fwrite(_pFileMem, iPropertyRange, 1, _pFile);
			_pFileMem += iPropertyRange;
		}

		// trigger obj serial id
		::fwrite(_pFileMem, sizeof(RwUInt32), 1, _pFile);
		_pFileMem += sizeof(RwUInt32);

		// object serial id
		::fwrite(_pFileMem, sizeof(RwUInt32), 1, _pFile);
		_pFileMem += sizeof(RwUInt32);

		// trigger obj object name index
		::fwrite(_pFileMem, sizeof(DWORD), 1, _pFile);
		_pFileMem += sizeof(DWORD);

		// visibility distance
		::fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
		_pFileMem += sizeof(RwReal);

		// enable trigger
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable selection
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable alterable
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable portal
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable path engine collision
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		// enable ShadowProp
		::fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		RwInt32 PSAtomicCnt = 0;
		CopyMemory(&PSAtomicCnt, _pFileMem, sizeof(RwInt32));

		::fwrite(&PSAtomicCnt, sizeof(RwInt32), 1, _pFile);
		_pFileMem += sizeof(RwInt32);

		for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
		{
			// Map name
			fwrite(_pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, _pFile);
			_pFileMem += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

			// Frustum view window extent
			fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
			_pFileMem += sizeof(RwReal);

			// Light matrices
			fwrite(_pFileMem, sizeof(RwMatrix), 1, _pFile);
			_pFileMem += (sizeof(RwMatrix));

			// Map size
			RwInt32 Size = 0;
			CopyMemory(&Size, _pFileMem, sizeof(RwInt32));
			fwrite(&Size, sizeof(RwInt32), 1, _pFile);
			_pFileMem += (sizeof(RwInt32));

			// Map
			fwrite(_pFileMem, sizeof(DWORD) * Size * Size, 1, _pFile);
			_pFileMem += (sizeof(DWORD) * Size * Size);
		}

		// milepost
		RwBool EnableFlg;
		CopyMemory(&EnableFlg, _pFileMem, sizeof(RwBool));
		fwrite(&EnableFlg, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);
		if(EnableFlg)
		{
			fwrite(_pFileMem, sizeof(RwUInt32), 1, _pFile);
			_pFileMem += sizeof(RwUInt32);
		}

		// attach light object skip
		RwInt32 iLightObjectNum;
		CopyMemory(&iLightObjectNum, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);
		fwrite(&iLightObjectNum, sizeof(RwInt32), 1, _pFile);

		for (int j = 0; j < iLightObjectNum; ++j)
		{
			fwrite(_pFileMem, sizeof(RwChar) * MAX_ATTR_BONE_NAME, 1, _pFile);
			_pFileMem += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

			fwrite(_pFileMem, sizeof(RpLightType), 1, _pFile);
			_pFileMem += sizeof(RpLightType);
			fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
			_pFileMem += sizeof(RwBool);

			fwrite(_pFileMem, sizeof(RwV3d), 1, _pFile);
			_pFileMem += sizeof(RwV3d);

			fwrite(_pFileMem, sizeof(RwV3d), 1, _pFile);
			_pFileMem += sizeof(RwV3d);
			fwrite(_pFileMem, sizeof(RwRGBAReal), 1, _pFile);
			_pFileMem += sizeof(RwRGBAReal);
			fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
			_pFileMem += sizeof(RwReal);
			fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
			_pFileMem += sizeof(RwReal);

			fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
			_pFileMem += sizeof(RwBool);
			fwrite(_pFileMem, sizeof(RwBool), 1, _pFile);
			_pFileMem += sizeof(RwBool);
		}

		// light map
		RwUInt32 uiLtMapObjectFlags;
		CopyMemory(&uiLtMapObjectFlags, _pFileMem, sizeof(RwUInt32));
		fwrite(&uiLtMapObjectFlags, sizeof(RwUInt32), 1, _pFile);
		_pFileMem += sizeof(RwUInt32);

		RwUInt32 uiLtMapMaterialFlags;
		CopyMemory(&uiLtMapMaterialFlags, _pFileMem, sizeof(RwUInt32));
		fwrite(&uiLtMapMaterialFlags, sizeof(RwUInt32), 1, _pFile);
		_pFileMem += sizeof(RwUInt32);

		RwInt32 iAtomicCnt;
		CopyMemory(&iAtomicCnt, _pFileMem, sizeof(RwInt32));
		fwrite(&iAtomicCnt, sizeof(RwInt32), 1, _pFile);
		_pFileMem += sizeof(RwInt32);

		for (int j = 0; i < iAtomicCnt; ++j)
		{
			RwBool bLitmapExisted;
			CopyMemory(&bLitmapExisted, _pFileMem, sizeof(RwBool));
			fwrite(&bLitmapExisted, sizeof(RwBool), 1, _pFile);
			_pFileMem += sizeof(RwBool);

			if (bLitmapExisted)
			{
				RwChar szAtomName[64];
				CopyMemory(szAtomName, _pFileMem, sizeof(RwChar) * 64);
				fwrite(&szAtomName, sizeof(RwChar) * 64, 1, _pFile);
				_pFileMem += sizeof(RwChar) * 64;

				RwChar szTexName[64];
				CopyMemory(szTexName, _pFileMem, sizeof(RwChar) * 64);
				fwrite(&szTexName, sizeof(RwChar) * 64, 1, _pFile);
				_pFileMem += sizeof(RwChar) * 64;

				RwInt32 iUVCnt;
				CopyMemory(&iUVCnt, _pFileMem, sizeof(RwInt32));
				fwrite(&iUVCnt, sizeof(RwInt32), 1, _pFile);
				_pFileMem += sizeof(RwInt32);

				RwTexCoords* pTexCoords = NTL_NEW RwTexCoords [ iUVCnt ];
				CopyMemory(pTexCoords, _pFileMem, sizeof(RwTexCoords) * iUVCnt);
				fwrite(pTexCoords, sizeof(RwTexCoords) * iUVCnt, 1, _pFile);
				_pFileMem += sizeof(RwTexCoords) * iUVCnt;
			}
		}
	}

	return _pFileMem;
}


BYTE* CNtlWorldFileMemAccessor::Save_Water(BYTE* _pFileMem, FILE* _pFile)
{
	RwInt32 SizMemBlock;
	RwInt32	Size;

	// water existence
	CopyMemory(&m_TmpBool, _pFileMem, sizeof(RwBool));
	::fwrite(&m_TmpBool, sizeof(RwBool), 1, _pFile);
	_pFileMem += sizeof(RwBool);

	if(m_TmpBool)
	{
		SizMemBlock =	sizeof(RwReal) +		// height
						sizeof(RwRGBA) +		// RGBA
						(sizeof(DWORD) * 2) +	// renderstate source, dst
						(sizeof(RwReal) * 2);	// rotate, scale

		::fwrite(_pFileMem, SizMemBlock, 1, _pFile);
		_pFileMem += SizMemBlock;

		// depth map size
		CopyMemory(&Size, _pFileMem, sizeof(RwInt32));
		::fwrite(&Size, sizeof(RwInt32), 1, _pFile);
		_pFileMem += sizeof(RwInt32);

		// depth map height variation, texture squence index
		::fwrite(_pFileMem, (sizeof(RwReal) * 2) + sizeof(RwInt32), 1, _pFile);
		_pFileMem += (sizeof(RwReal) * 2) + sizeof(RwInt32);

		// depth map existence
		CopyMemory(&m_TmpBool, _pFileMem, sizeof(RwBool));
		::fwrite(&m_TmpBool, sizeof(RwBool), 1, _pFile);
		_pFileMem += sizeof(RwBool);

		if(m_TmpBool)
		{
			// depth map alpha bits
			::fwrite(_pFileMem, (sizeof(BYTE) * Size * Size), 1, _pFile);
			_pFileMem += (sizeof(BYTE) * Size * Size);
		}
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_Shadow(BYTE* _pFileMem, FILE* _pFile)
{
	// shadow existence
	CopyMemory(&m_TmpBool, _pFileMem, sizeof(RwBool));
	::fwrite(&m_TmpBool, sizeof(RwBool), 1, _pFile);
	_pFileMem += sizeof(RwBool);

	if(m_TmpBool)
	{
		fwrite(_pFileMem, sizeof(RwRGBA), 1, _pFile);
		_pFileMem += sizeof(RwRGBA);

		// camera view window x
		::fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
		_pFileMem += sizeof(RwReal);

		::fwrite(_pFileMem, sizeof(RwMatrix), 1, _pFile);
		_pFileMem += sizeof(RwMatrix);

		// render2texture size
		CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
		::fwrite(&m_TmpRwInt32, sizeof(RwInt32), 1, _pFile);
		_pFileMem += sizeof(RwInt32);

		// argb bits
		::fwrite(_pFileMem, (sizeof(DWORD) * m_TmpRwInt32 * m_TmpRwInt32), 1, _pFile);
		_pFileMem += (sizeof(DWORD) * m_TmpRwInt32 * m_TmpRwInt32);
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_Effects(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField/* = -1*/, RwInt32 IdxDstField/* = -1*/, CNtlWorldFieldManager* pFieldMgr/* = NULL*/)
{
	RwInt32		SizMemBlock;
	RwChar		StrObjName[128];
	RwV3d		PosScaling;
	RwV3d		PosRotation;
	RwV3d		PosTranslation;
	RwReal		LenVisibility;
	RwV3d		PosSrcLocal;
	RwV3d		PosDstWorld;


	// The number of Effects.
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	fwrite(&m_TmpRwInt32, sizeof(RwInt32), 1, _pFile);
	_pFileMem += sizeof(RwInt32);

	SizMemBlock =	(sizeof(RwChar) * 128) +	// Obj. Name
					(sizeof(RwV3d) * 3) +		// Scaling, Rotation, Traslation
					(sizeof(RwReal));			// visibility distance

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
		if(!pFieldMgr)
		{
			fwrite(_pFileMem, SizMemBlock, 1, _pFile);
			_pFileMem += SizMemBlock;
		}
		else
		{
			CopyMemory(StrObjName, _pFileMem, (sizeof(RwChar) * 128));	_pFileMem += (sizeof(RwChar) * 128);
			CopyMemory(&PosScaling, _pFileMem, sizeof(RwV3d));			_pFileMem += sizeof(RwV3d);
			CopyMemory(&PosRotation, _pFileMem, sizeof(RwV3d));			_pFileMem += sizeof(RwV3d);
			CopyMemory(&PosTranslation, _pFileMem, sizeof(RwV3d));		_pFileMem += sizeof(RwV3d);
			CopyMemory(&LenVisibility, _pFileMem, sizeof(RwReal));		_pFileMem += sizeof(RwReal);

			fwrite(StrObjName, (sizeof(RwChar) * 128), 1, _pFile);
			fwrite(&PosScaling, sizeof(RwV3d), 1, _pFile);
			fwrite(&PosRotation, sizeof(RwV3d), 1, _pFile);

			// relative coordinates
			CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
			RwV3d PosSrc = pFields[IdxSrcField].GetSPos();
			RwV3d PosDst = pFields[IdxDstField].GetSPos();

			PosSrcLocal.x = PosTranslation.x - PosSrc.x;
			PosSrcLocal.y = PosTranslation.y;
			PosSrcLocal.z = PosTranslation.z - PosSrc.z;
			PosSrcLocal.x = fabs(PosSrcLocal.x);
			PosSrcLocal.z = fabs(PosSrcLocal.z);

			PosDstWorld.x = PosSrcLocal.x + PosDst.x;
			PosDstWorld.z = PosSrcLocal.z + PosDst.z;
			PosDstWorld.y = PosTranslation.y;

			fwrite(&PosDstWorld, sizeof(RwV3d), 1, _pFile);
			fwrite(&LenVisibility, sizeof(RwReal), 1, _pFile);
		}
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_SE(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField/* = -1*/, RwInt32 IdxDstField/* = -1*/, CNtlWorldFieldManager* pFieldMgr/* = NULL*/)
{
	RwInt32		SizMemBlock;
	RwChar		StrObjName[32];
	RwReal		ValVol;
	RwReal		Radius;
	RwReal		MinRadius;
	RwReal		MaxRadius;
	RwV3d		Translation;

	RwV3d		PosSrcLocal;
	RwV3d		PosDstWorld;


	// The number of SE.
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	fwrite(&m_TmpRwInt32, sizeof(RwInt32), 1, _pFile);
	_pFileMem += sizeof(RwInt32);

	SizMemBlock =   (sizeof(RwChar) * 32) +		// Obj. Name
					(sizeof(RwReal) * 3) +		// Vol, Radius Min, Max
					(sizeof(RwV3d));			// Translation

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
		if(!pFieldMgr)
		{
			fwrite(_pFileMem, SizMemBlock, 1, _pFile);
			_pFileMem += SizMemBlock;
		}
		else
		{
			CopyMemory(StrObjName, _pFileMem, (sizeof(RwChar) * 32));	_pFileMem += (sizeof(RwChar) * 32);
			CopyMemory(&ValVol, _pFileMem, sizeof(RwReal));				_pFileMem += sizeof(RwReal);
			CopyMemory(&MinRadius, _pFileMem, sizeof(RwReal));			_pFileMem += sizeof(RwReal);
			CopyMemory(&MaxRadius, _pFileMem, sizeof(RwReal));			_pFileMem += sizeof(RwReal);
			CopyMemory(&Translation, _pFileMem, sizeof(RwV3d));			_pFileMem += sizeof(RwV3d);

			fwrite(StrObjName, (sizeof(RwChar) * 32), 1, _pFile);
			fwrite(&ValVol, sizeof(RwReal), 1, _pFile);
			fwrite(&MinRadius, sizeof(RwReal), 1, _pFile);
			fwrite(&MaxRadius, sizeof(RwReal), 1, _pFile);

			// relative coordinates
			CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
			RwV3d PosSrc = pFields[IdxSrcField].GetSPos();
			RwV3d PosDst = pFields[IdxDstField].GetSPos();

			PosSrcLocal.x = Translation.x - PosSrc.x;
			PosSrcLocal.y = Translation.y;
			PosSrcLocal.z = Translation.z - PosSrc.z;
			PosSrcLocal.x = fabs(PosSrcLocal.x);
			PosSrcLocal.z = fabs(PosSrcLocal.z);

			PosDstWorld.x = PosSrcLocal.x + PosDst.x;
			PosDstWorld.z = PosSrcLocal.z + PosDst.z;
			PosDstWorld.y = Translation.y;

			fwrite(&PosDstWorld, sizeof(RwV3d), 1, _pFile);
		}
	}

	// The number of SE BGM.
	// CZ-BGM
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	fwrite(&m_TmpRwInt32, sizeof(RwInt32), 1, _pFile);
	_pFileMem += sizeof(RwInt32);

	SizMemBlock =   (sizeof(RwChar) * 32) +		// Obj. Name
					//(sizeof(RwReal)) +		// Vol
					(sizeof(RwReal)) +			// Radius
					(sizeof(RwV3d));			// Translation

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
		if(!pFieldMgr)
		{
			fwrite(_pFileMem, SizMemBlock, 1, _pFile);
			_pFileMem += SizMemBlock;
		}
		else
		{
			CopyMemory(StrObjName, _pFileMem, (sizeof(RwChar) * 32));	_pFileMem += (sizeof(RwChar) * 32);
			//CopyMemory(&ValVol, _pFileMem, sizeof(RwReal));				_pFileMem += sizeof(RwReal);
			CopyMemory(&Radius, _pFileMem, sizeof(RwReal));				_pFileMem += sizeof(RwReal);
			CopyMemory(&Translation, _pFileMem, sizeof(RwV3d));			_pFileMem += sizeof(RwV3d);

			fwrite(StrObjName, (sizeof(RwChar) * 32), 1, _pFile);
			//fwrite(&ValVol, sizeof(RwReal), 1, _pFile);
			fwrite(&Radius, sizeof(RwReal), 1, _pFile);

			// relative coordinates
			CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
			RwV3d PosSrc = pFields[IdxSrcField].GetSPos();
			RwV3d PosDst = pFields[IdxDstField].GetSPos();

			PosSrcLocal.x = Translation.x - PosSrc.x;
			PosSrcLocal.y = Translation.y;
			PosSrcLocal.z = Translation.z - PosSrc.z;
			PosSrcLocal.x = fabs(PosSrcLocal.x);
			PosSrcLocal.z = fabs(PosSrcLocal.z);

			PosDstWorld.x = PosSrcLocal.x + PosDst.x;
			PosDstWorld.z = PosSrcLocal.z + PosDst.z;
			PosDstWorld.y = Translation.y;

			fwrite(&PosDstWorld, sizeof(RwV3d), 1, _pFile);
		}
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_Shoreline(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField /*= -1*/, RwInt32 IdxDstField /*= -1*/, CNtlWorldFieldManager* pFieldMgr /*= NULL*/)
{
	// the number of shoreline polygons
	RwBool	IsShoreline;
	RwInt32 VertNum;
	RwV3d	PosSrcLocal;

	CopyMemory(&IsShoreline, _pFileMem, sizeof(RwBool));
	fwrite(&IsShoreline, sizeof(RwBool), 1, _pFile);
	_pFileMem += sizeof(RwBool);

	if(IsShoreline)
	{
		CopyMemory(&VertNum, _pFileMem, sizeof(RwInt32));
		fwrite(&VertNum, sizeof(RwInt32), 1, _pFile);
		_pFileMem += sizeof(RwInt32);

		// script type
		fwrite(_pFileMem, sizeof(RwInt32), 1, _pFile);
		_pFileMem += sizeof(RwInt32);

		// script name
		fwrite(_pFileMem, (sizeof(RwChar) * 32), 1, _pFile);
		_pFileMem += (sizeof(RwChar) * 32);

		for(RwInt32 i = 0; i < VertNum; ++i)
		{
			if(!pFieldMgr)
			{
				fwrite(_pFileMem, sizeof(RwIm3DVertex), 1, _pFile);
				_pFileMem += (sizeof(RwIm3DVertex));
			}
			else
			{
				// relative coordinates
				CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
				RwV3d			PosSrc	= pFields[IdxSrcField].GetSPos();
				RwV3d			PosDst	= pFields[IdxDstField].GetSPos();
				RwIm3DVertex	Translation;
				RwIm3DVertex	TranslationDst;

				CopyMemory(&Translation, _pFileMem, sizeof(RwIm3DVertex));
				CopyMemory(&TranslationDst, &Translation, sizeof(RwIm3DVertex));
				_pFileMem += sizeof(RwIm3DVertex);

				PosSrcLocal.x = Translation.objVertex.x - PosSrc.x;
				PosSrcLocal.y = Translation.objVertex.y;
				PosSrcLocal.z = Translation.objVertex.z - PosSrc.z;
				//PosSrcLocal.x = fabs(PosSrcLocal.x);
				//PosSrcLocal.z = fabs(PosSrcLocal.z);

				TranslationDst.objVertex.x	= PosSrcLocal.x + PosDst.x;
				TranslationDst.objVertex.z	= PosSrcLocal.z + PosDst.z;	
				TranslationDst.objVertex.y	= Translation.objVertex.y;	

				fwrite(&TranslationDst, sizeof(RwIm3DVertex), 1, _pFile);
			}
		}
	}

	return _pFileMem;
}

// CZ-SS
// BYTE* CNtlWorldFileMemAccessor::Save_SS(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField /*= -1*/, RwInt32 IdxDstField /*= -1*/, CNtlWorldFieldManager* pFieldMgr /*= NULL*/)
// {
// 	RwInt32		SizMemBlock;
// 	RwRGBAReal	Clr;
// 	RwReal		Ang;
// 	RwReal		Rad;
// 	RwV3d		Position, Rotation;
// 	RwV3d		PosSrcLocal;
// 	RwV3d		PosDstWorld;
// 
// 
// 	// The number of SS.
// 	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
// 	fwrite(&m_TmpRwInt32, sizeof(RwInt32), 1, _pFile);
// 	_pFileMem += sizeof(RwInt32);
// 
// 	SizMemBlock =	sizeof(RwV3d) +			// Position
// 					sizeof(RwV3d) +			// Rotation
// 					sizeof(RwRGBAReal) +	// Color
// 					sizeof(RwReal) +		// Angle
// 					sizeof(RwReal);			// Radius
// 
// 	for(int i = 0; i < m_TmpRwInt32; ++i)
// 	{
// 		if(!pFieldMgr)
// 		{
// 			fwrite(_pFileMem, SizMemBlock, 1, _pFile);
// 			_pFileMem += SizMemBlock;
// 		}
// 		else
// 		{
// 			CopyMemory(&Position, _pFileMem, sizeof(RwV3d));	_pFileMem += sizeof(RwV3d);
// 			CopyMemory(&Rotation, _pFileMem, sizeof(RwV3d));	_pFileMem += sizeof(RwV3d);
// 			CopyMemory(&Clr, _pFileMem, sizeof(RwRGBAReal));	_pFileMem += sizeof(RwRGBAReal);
// 			CopyMemory(&Ang, _pFileMem, sizeof(RwReal));		_pFileMem += sizeof(RwReal);
// 			CopyMemory(&Rad, _pFileMem, sizeof(RwReal));		_pFileMem += sizeof(RwReal);
// 
// 			// relative coordinates
// 			CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
// 			RwV3d PosSrc = pFields[IdxSrcField].GetSPos();
// 			RwV3d PosDst = pFields[IdxDstField].GetSPos();
// 
// 			PosSrcLocal.x = Position.x - PosSrc.x;
// 			PosSrcLocal.y = Position.y;
// 			PosSrcLocal.z = Position.z - PosSrc.z;
// 			PosSrcLocal.x = fabs(PosSrcLocal.x);
// 			PosSrcLocal.z = fabs(PosSrcLocal.z);
// 
// 			PosDstWorld.x = PosSrcLocal.x + PosDst.x;
// 			PosDstWorld.z = PosSrcLocal.z + PosDst.z;
// 			PosDstWorld.y = Position.y;
// 
// 			fwrite(&PosDstWorld, sizeof(RwV3d), 1, _pFile);
// 			fwrite(&Rotation, sizeof(RwV3d), 1, _pFile);
// 			fwrite(&Clr, sizeof(RwRGBAReal), 1, _pFile);
// 			fwrite(&Ang, sizeof(RwReal), 1, _pFile);
// 			fwrite(&Rad, sizeof(RwReal), 1, _pFile);
// 		}
// 	}
// 
// 	return _pFileMem;
// }

BYTE* CNtlWorldFileMemAccessor::Save_Decal(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField/* = -1*/, RwInt32 IdxDstField/* = -1*/, CNtlWorldFieldManager* pFieldMgr/* = NULL*/)
{
	RwV3d	PosSrcLocal;
	RwV3d	PosDstWorld;
	RwV3d	PosTranslation;
	RwInt32 MemSizeExceptPos =	(sizeof(RwChar) * 128)	+	// TexName[128];
								(sizeof(RwReal) * 7)	+	// TexAngle, TexScale, SizeX, SizeZ, VisibilityDist, UVAniSpeed, UVAniOffset
								(sizeof(RwInt32))		+	// UVAniPatternIdx
								(sizeof(RwBool))		+	// UVAniCheck
								(sizeof(RwInt32))		+	// DecalMode
								(sizeof(RwRGBA));			// RGBA

	// The number of decals.
	CopyMemory(&m_TmpRwInt32, _pFileMem, sizeof(RwInt32));
	fwrite(&m_TmpRwInt32, sizeof(RwInt32), 1, _pFile);
	_pFileMem += sizeof(RwInt32);

	for(int i = 0; i < m_TmpRwInt32; ++i)
	{
		if(!pFieldMgr)
		{
			fwrite(_pFileMem, sizeof(sDECAL_PROP_PARAM), 1, _pFile);
			_pFileMem += sizeof(sDECAL_PROP_PARAM);
		}
		else
		{
			fwrite(_pFileMem, MemSizeExceptPos, 1, _pFile);
			_pFileMem += MemSizeExceptPos;
			
			CopyMemory(&PosTranslation, _pFileMem, sizeof(RwV3d));
			_pFileMem += sizeof(RwV3d);

			// relative coordinates
			CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
			RwV3d PosSrc = pFields[IdxSrcField].GetSPos();
			RwV3d PosDst = pFields[IdxDstField].GetSPos();

			PosSrcLocal.x = PosTranslation.x - PosSrc.x;
			PosSrcLocal.y = PosTranslation.y;
			PosSrcLocal.z = PosTranslation.z - PosSrc.z;
			PosSrcLocal.x = fabs(PosSrcLocal.x);
			PosSrcLocal.z = fabs(PosSrcLocal.z);

			PosDstWorld.x = PosSrcLocal.x + PosDst.x;
			PosDstWorld.z = PosSrcLocal.z + PosDst.z;
			PosDstWorld.y = PosTranslation.y;

			fwrite(&PosDstWorld, sizeof(RwV3d), 1, _pFile);
		}
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_Plant(BYTE* _pFileMem, FILE* _pFile)
{
	RwInt32 iPlantSetNum;	
	CopyMemory(&iPlantSetNum, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);
	fwrite(&iPlantSetNum, sizeof(RwInt32), 1, _pFile);

	for (RwInt32 i = 0; i < iPlantSetNum; ++i)
	{
		RwInt32 iPlnatSet;
		CopyMemory(&iPlnatSet, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);
		fwrite(&iPlnatSet, sizeof(RwInt32), 1, _pFile);

		RwInt32 iPlantObjNum;			
		CopyMemory(&iPlantObjNum, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);
		fwrite(&iPlantObjNum, sizeof(RwInt32), 1, _pFile);

		for (RwInt32 j = 0; j < iPlantObjNum; ++j)
		{
			RwInt32 iTileIdx;
			CopyMemory(&iTileIdx, _pFileMem, sizeof(RwInt32));
			_pFileMem += sizeof(RwInt32);
			fwrite(&iTileIdx, sizeof(RwInt32), 1, _pFile);
		}		
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_WorldLight(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField, RwInt32 IdxDstField, CNtlWorldFieldManager* pFieldMgr)
{
	// WorldLight_Sphere
	RwInt32 iNumWLShpere;
	CopyMemory(&iNumWLShpere, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);
	fwrite(&iNumWLShpere, sizeof(RwInt32), 1, _pFile);

	for (int i = 0; i < iNumWLShpere; ++i)
	{
		RwV3d	vPos;
		RwRGBA	clr;
		RwReal	fRadius;

		CopyMemory(&vPos, _pFileMem, sizeof(RwV3d));
		_pFileMem += sizeof(RwV3d);
		if(pFieldMgr)
		{
			RwV3d	PosSrcLocal;
			RwV3d	PosDstWorld;
			RwV3d	PosTranslation = vPos;

			// relative coordinates
			CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
			RwV3d PosSrc = pFields[IdxSrcField].GetSPos();
			RwV3d PosDst = pFields[IdxDstField].GetSPos();

			PosSrcLocal.x = PosTranslation.x - PosSrc.x;
			PosSrcLocal.y = PosTranslation.y;
			PosSrcLocal.z = PosTranslation.z - PosSrc.z;
			PosSrcLocal.x = fabs(PosSrcLocal.x);
			PosSrcLocal.z = fabs(PosSrcLocal.z);

			PosDstWorld.x = PosSrcLocal.x + PosDst.x;
			PosDstWorld.z = PosSrcLocal.z + PosDst.z;
			PosDstWorld.y = PosTranslation.y;

			fwrite(&PosDstWorld, sizeof(RwV3d), 1, _pFile);
		}
		else
		{
			fwrite(&vPos, sizeof(RwV3d), 1, _pFile);
		}

		CopyMemory(&clr, _pFileMem, sizeof(RwRGBA));
		_pFileMem += sizeof(RwRGBA);
		fwrite(&clr, sizeof(RwRGBA), 1, _pFile);

		CopyMemory(&fRadius, _pFileMem, sizeof(RwReal));
		_pFileMem += sizeof(RwReal);
		fwrite(&fRadius, sizeof(RwReal), 1, _pFile);
	}

	// WorldLight_Box
	RwInt32 iNumWLBox;
	CopyMemory(&iNumWLBox, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);
	fwrite(&iNumWLBox, sizeof(RwInt32), 1, _pFile);

	for (int i = 0; i < iNumWLBox; ++i)
	{
		RwV3d	vPos;
		RwRGBA	clr;
		RwV3d	vRot;
		RwV3d	vScale;

		CopyMemory(&vPos, _pFileMem, sizeof(RwV3d));
		_pFileMem += sizeof(RwV3d);
		if(pFieldMgr)
		{
			RwV3d	PosSrcLocal;
			RwV3d	PosDstWorld;
			RwV3d	PosTranslation = vPos;

			// relative coordinates
			CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
			RwV3d PosSrc = pFields[IdxSrcField].GetSPos();
			RwV3d PosDst = pFields[IdxDstField].GetSPos();

			PosSrcLocal.x = PosTranslation.x - PosSrc.x;
			PosSrcLocal.y = PosTranslation.y;
			PosSrcLocal.z = PosTranslation.z - PosSrc.z;
			PosSrcLocal.x = fabs(PosSrcLocal.x);
			PosSrcLocal.z = fabs(PosSrcLocal.z);

			PosDstWorld.x = PosSrcLocal.x + PosDst.x;
			PosDstWorld.z = PosSrcLocal.z + PosDst.z;
			PosDstWorld.y = PosTranslation.y;

			fwrite(&PosDstWorld, sizeof(RwV3d), 1, _pFile);
		}
		else
		{
			fwrite(&vPos, sizeof(RwV3d), 1, _pFile);
		}

		CopyMemory(&clr, _pFileMem, sizeof(RwRGBA));
		_pFileMem += sizeof(RwRGBA);
		fwrite(&clr, sizeof(RwRGBA), 1, _pFile);

		CopyMemory(&vRot, _pFileMem, sizeof(RwV3d));
		_pFileMem += sizeof(RwV3d);
		fwrite(&vRot, sizeof(RwV3d), 1, _pFile);

		CopyMemory(&vScale, _pFileMem, sizeof(RwV3d));
		_pFileMem += sizeof(RwV3d);
		fwrite(&vScale, sizeof(RwV3d), 1, _pFile);
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_Occluder(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField, RwInt32 IdxDstField, CNtlWorldFieldManager* pFieldMgr)
{
	// Occluder_Quad
	RwInt32 iNumOccluder_Quad;
	CopyMemory(&iNumOccluder_Quad, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);
	::fwrite(&iNumOccluder_Quad, sizeof(RwInt32), 1, _pFile);

	for (int i = 0; i < iNumOccluder_Quad; ++i)
	{
		RwUInt32	eFuncFlag;
		RwV3d		vTemp;
		RwInt32		iPlaneType;

		CopyMemory(&eFuncFlag, _pFileMem, sizeof(RwUInt32));
		_pFileMem += sizeof(RwUInt32);
		::fwrite(&eFuncFlag, sizeof(RwUInt32), 1, _pFile);

		CopyMemory(&vTemp, _pFileMem, sizeof(RwV3d)); // vPos
		_pFileMem += sizeof(RwV3d);
		if(pFieldMgr)
		{
			RwV3d	PosSrcLocal;
			RwV3d	PosDstWorld;
			RwV3d	PosTranslation = vTemp;

			// relative coordinates
			CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
			RwV3d PosSrc = pFields[IdxSrcField].GetSPos();
			RwV3d PosDst = pFields[IdxDstField].GetSPos();

			PosSrcLocal.x = PosTranslation.x - PosSrc.x;
			PosSrcLocal.y = PosTranslation.y;
			PosSrcLocal.z = PosTranslation.z - PosSrc.z;
			PosSrcLocal.x = fabs(PosSrcLocal.x);
			PosSrcLocal.z = fabs(PosSrcLocal.z);

			PosDstWorld.x = PosSrcLocal.x + PosDst.x;
			PosDstWorld.z = PosSrcLocal.z + PosDst.z;
			PosDstWorld.y = PosTranslation.y;

			fwrite(&PosDstWorld, sizeof(RwV3d), 1, _pFile);
		}
		else
		{
			fwrite(&vTemp, sizeof(RwV3d), 1, _pFile);
		}

		CopyMemory(&iPlaneType, _pFileMem, sizeof(RwInt32)); // iPlaneType
		_pFileMem += sizeof(RwInt32);
		::fwrite(&iPlaneType, sizeof(RwInt32), 1, _pFile);

		RwChar acTemp[128];
		CopyMemory(acTemp, _pFileMem, sizeof(RwChar) * 128);
		_pFileMem += sizeof(RwChar) * 128;
		::fwrite(acTemp, sizeof(RwChar) * 128, 1, _pFile);

		for (int i = 0; i < 4; ++i)
		{
			CopyMemory(&vTemp, _pFileMem, sizeof(RwV3d)); // vVertex
			_pFileMem += sizeof(RwV3d);
			::fwrite(&vTemp, sizeof(RwV3d), 1, _pFile);
		}
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_HeatHazeObject(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField, RwInt32 IdxDstField, CNtlWorldFieldManager* pFieldMgr)
{
	RwInt32 iNumHeatHaze;
	CopyMemory(&iNumHeatHaze, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);
	::fwrite(&iNumHeatHaze, sizeof(RwInt32), 1, _pFile);

	for (int i = 0; i < iNumHeatHaze; ++i)
	{
		RwV3d			vVertex;
		RwReal			fDistance;
		sNTL_HEATHAZE	HeatHaze;
	
		for (int i = 0; i < 4; ++i)
		{
			CopyMemory(&vVertex, _pFileMem, sizeof(RwV3d));
			_pFileMem += sizeof(RwV3d);
			if(pFieldMgr)
			{
				RwV3d	PosSrcLocal;
				RwV3d	PosDstWorld;
				RwV3d	PosTranslation = vVertex;

				// relative coordinates
				CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
				RwV3d PosSrc = pFields[IdxSrcField].GetSPos();
				RwV3d PosDst = pFields[IdxDstField].GetSPos();

				PosSrcLocal.x = PosTranslation.x - PosSrc.x;
				PosSrcLocal.y = PosTranslation.y;
				PosSrcLocal.z = PosTranslation.z - PosSrc.z;
				PosSrcLocal.x = fabs(PosSrcLocal.x);
				PosSrcLocal.z = fabs(PosSrcLocal.z);

				PosDstWorld.x = PosSrcLocal.x + PosDst.x;
				PosDstWorld.z = PosSrcLocal.z + PosDst.z;
				PosDstWorld.y = PosTranslation.y;

				fwrite(&PosDstWorld, sizeof(RwV3d), 1, _pFile);
			}
			else
			{
				fwrite(&vVertex, sizeof(RwV3d), 1, _pFile);
			}
		}

		CopyMemory(&fDistance, _pFileMem, sizeof(RwReal));
		_pFileMem += sizeof(RwReal);
		::fwrite(&fDistance, sizeof(RwReal), 1, _pFile);

		CopyMemory(&HeatHaze, _pFileMem, sizeof(sNTL_HEATHAZE));
		_pFileMem += sizeof(sNTL_HEATHAZE);
		::fwrite(&HeatHaze, sizeof(sNTL_HEATHAZE), 1, _pFile);
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_Dojo(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField, RwInt32 IdxDstField, CNtlWorldFieldManager* pFieldMgr)
{
	RwInt32 iNumDojo;
	CopyMemory(&iNumDojo, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);
	::fwrite(&iNumDojo, sizeof(RwInt32), 1, _pFile);

	for (int i = 0; i < iNumDojo; ++i)
	{
		RwV3d vScale;
		RwV3d vRot;
		
		CopyMemory(&vScale, _pFileMem, sizeof(RwV3d));
		_pFileMem += sizeof(RwV3d);
		::fwrite(&vScale, sizeof(RwV3d), 1, _pFile);

		CopyMemory(&vRot, _pFileMem, sizeof(RwV3d));
		_pFileMem += sizeof(RwV3d);
		::fwrite(&vRot, sizeof(RwV3d), 1, _pFile);

		RwV3d vTemp;
		CopyMemory(&vTemp, _pFileMem, sizeof(RwV3d));  // Pos
		_pFileMem += sizeof(RwV3d);
		if (pFieldMgr)
		{
			RwV3d	PosSrcLocal;
			RwV3d	PosDstWorld;
			RwV3d	PosTranslation = vTemp;

			// relative coordinates
			CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
			RwV3d PosSrc = pFields[IdxSrcField].GetSPos();
			RwV3d PosDst = pFields[IdxDstField].GetSPos();

			PosSrcLocal.x = PosTranslation.x - PosSrc.x;
			PosSrcLocal.y = PosTranslation.y;
			PosSrcLocal.z = PosTranslation.z - PosSrc.z;
			PosSrcLocal.x = fabs(PosSrcLocal.x);
			PosSrcLocal.z = fabs(PosSrcLocal.z);

			PosDstWorld.x = PosSrcLocal.x + PosDst.x;
			PosDstWorld.z = PosSrcLocal.z + PosDst.z;
			PosDstWorld.y = PosTranslation.y;

			::fwrite(&PosDstWorld, sizeof(RwV3d), 1, _pFile);
		}
		else
		{
			::fwrite(&vTemp, sizeof(RwV3d), 1, _pFile);
		}

		RwInt32 iDojoID;
		CopyMemory(&iDojoID, _pFileMem, sizeof(RwInt32));
		_pFileMem += sizeof(RwInt32);
		::fwrite(&iDojoID, sizeof(RwInt32), 1, _pFile);

		RwReal fFade;
		CopyMemory(&fFade, _pFileMem, sizeof(RwReal));
		_pFileMem += sizeof(RwReal);
		::fwrite(&fFade, sizeof(RwReal), 1, _pFile);

		CopyMemory(&fFade, _pFileMem, sizeof(RwReal));
		_pFileMem += sizeof(RwReal);
		::fwrite(&fFade, sizeof(RwReal), 1, _pFile);

		RwChar acTemp[dPL_DOJO_RES_NAME_LEN];
		for (int i = 0; i < dPL_DOJO_MAX_LEVEL; ++i)
		{
			// object name
			CopyMemory(acTemp, _pFileMem, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN);
			_pFileMem += sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN;
			::fwrite(acTemp, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, _pFile);

			// upgrade effect name
			CopyMemory(acTemp, _pFileMem, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN);
			_pFileMem += sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN;
			::fwrite(acTemp, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, _pFile);

			// downgrade effect name
			CopyMemory(acTemp, _pFileMem, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN);
			_pFileMem += sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN;
			::fwrite(acTemp, sizeof(RwChar) * dPL_DOJO_RES_NAME_LEN, 1, _pFile);
		}
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_LightObject(BYTE* _pFileMem, FILE* _pFile, RwInt32 IdxSrcField, RwInt32 IdxDstField, CNtlWorldFieldManager* pFieldMgr)
{
	RwInt32 iLightObjectNum;
	CopyMemory(&iLightObjectNum, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);
	::fwrite(&iLightObjectNum, sizeof(RwInt32), 1, _pFile);

	for (int i = 0; i < iLightObjectNum; ++i)
	{
		RpLightType eType;
		CopyMemory(&eType, _pFileMem, sizeof(RpLightType));
		_pFileMem += sizeof(RpLightType);
		::fwrite(&eType, sizeof(RpLightType), 1, _pFile);

		RwBool bTemp;
		CopyMemory(&bTemp, _pFileMem, sizeof(RwBool));
		_pFileMem += sizeof(RwBool);
		::fwrite(&bTemp, sizeof(RwBool), 1, _pFile);

		RwV3d vTemp;
		CopyMemory(&vTemp, _pFileMem, sizeof(RwV3d));
		_pFileMem += sizeof(RwV3d);
		if(pFieldMgr)
		{
			RwV3d	PosSrcLocal;
			RwV3d	PosDstWorld;
			RwV3d	PosTranslation = vTemp;

			// relative coordinates
			CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
			RwV3d PosSrc = pFields[IdxSrcField].GetSPos();
			RwV3d PosDst = pFields[IdxDstField].GetSPos();

			PosSrcLocal.x = PosTranslation.x - PosSrc.x;
			PosSrcLocal.y = PosTranslation.y;
			PosSrcLocal.z = PosTranslation.z - PosSrc.z;
			PosSrcLocal.x = fabs(PosSrcLocal.x);
			PosSrcLocal.z = fabs(PosSrcLocal.z);

			PosDstWorld.x = PosSrcLocal.x + PosDst.x;
			PosDstWorld.z = PosSrcLocal.z + PosDst.z;
			PosDstWorld.y = PosTranslation.y;

			fwrite(&PosDstWorld, sizeof(RwV3d), 1, _pFile);
		}
		else
		{
			::fwrite(&vTemp, sizeof(RwV3d), 1, _pFile);
		}
		
		CopyMemory(&vTemp, _pFileMem, sizeof(RwV3d));
		_pFileMem += sizeof(RwV3d);
		::fwrite(&vTemp, sizeof(RwV3d), 1, _pFile);

		RwRGBAReal rgbafTemp;
		CopyMemory(&rgbafTemp, _pFileMem, sizeof(RwRGBAReal));
		_pFileMem += sizeof(RwRGBAReal);
		::fwrite(&rgbafTemp, sizeof(RwRGBAReal), 1, _pFile);

		RwReal fTemp;
		CopyMemory(&fTemp, _pFileMem, sizeof(RwReal));
		_pFileMem += sizeof(RwReal);
		::fwrite(&fTemp, sizeof(RwReal), 1, _pFile);

		CopyMemory(&fTemp, _pFileMem, sizeof(RwReal));
		_pFileMem += sizeof(RwReal);
		::fwrite(&fTemp, sizeof(RwReal), 1, _pFile);

		CopyMemory(&bTemp, _pFileMem, sizeof(RwBool));
		_pFileMem += sizeof(RwBool);
		::fwrite(&bTemp, sizeof(RwBool), 1, _pFile);

		CopyMemory(&bTemp, _pFileMem, sizeof(RwBool));
		_pFileMem += sizeof(RwBool);
		::fwrite(&bTemp, sizeof(RwBool), 1, _pFile);
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_DecalVolume(BYTE* _pFileMem, FILE* _pFile,  RwInt32 IdxSrcField, RwInt32 IdxDstField, CNtlWorldFieldManager* pFieldMgr)
{
	RwInt32 iDecalVolumeNum;
	CopyMemory(&iDecalVolumeNum, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);
	::fwrite(&iDecalVolumeNum, sizeof(RwInt32), 1, _pFile);

	for (int i = 0; i < iDecalVolumeNum; ++i)
	{
		RwChar acTextureName[128];
		CopyMemory(acTextureName, _pFileMem, sizeof(RwChar) * 128);
		_pFileMem += sizeof(RwChar) * 128;
		::fwrite(acTextureName, sizeof(RwChar) * 128, 1, _pFile);

		RwV3d vTemp;
		CopyMemory(&vTemp, _pFileMem, sizeof(RwV3d));
		_pFileMem += sizeof(RwV3d);
		if(pFieldMgr)
		{
			RwV3d	PosSrcLocal;
			RwV3d	PosDstWorld;
			RwV3d	PosTranslation = vTemp;

			// relative coordinates
			CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
			RwV3d PosSrc = pFields[IdxSrcField].GetSPos();
			RwV3d PosDst = pFields[IdxDstField].GetSPos();

			PosSrcLocal.x = PosTranslation.x - PosSrc.x;
			PosSrcLocal.y = PosTranslation.y;
			PosSrcLocal.z = PosTranslation.z - PosSrc.z;
			PosSrcLocal.x = fabs(PosSrcLocal.x);
			PosSrcLocal.z = fabs(PosSrcLocal.z);

			PosDstWorld.x = PosSrcLocal.x + PosDst.x;
			PosDstWorld.z = PosSrcLocal.z + PosDst.z;
			PosDstWorld.y = PosTranslation.y;

			fwrite(&PosDstWorld, sizeof(RwV3d), 1, _pFile);
		}
		else
		{
			::fwrite(&vTemp, sizeof(RwV3d), 1, _pFile);
		}

		CopyMemory(&vTemp, _pFileMem, sizeof(RwV3d));
		_pFileMem += sizeof(RwV3d);
		::fwrite(&vTemp, sizeof(RwV3d), 1, _pFile);

		CopyMemory(&vTemp, _pFileMem, sizeof(RwV3d));
		_pFileMem += sizeof(RwV3d);
		::fwrite(&vTemp, sizeof(RwV3d), 1, _pFile);

		RwRGBA clrTemp;
		CopyMemory(&clrTemp, _pFileMem, sizeof(RwRGBA));
		_pFileMem += sizeof(RwRGBA);
		::fwrite(&clrTemp, sizeof(RwRGBA), 1, _pFile);

		RwReal fTemp;
		CopyMemory(&fTemp, _pFileMem, sizeof(RwReal));
		_pFileMem += sizeof(RwReal);
		::fwrite(&fTemp, sizeof(RwReal), 1, _pFile);

		CopyMemory(&fTemp, _pFileMem, sizeof(RwReal));
		_pFileMem += sizeof(RwReal);
		::fwrite(&fTemp, sizeof(RwReal), 1, _pFile);

		RwUInt32 uiTemp;
		CopyMemory(&uiTemp, _pFileMem, sizeof(RwUInt32));
		_pFileMem += sizeof(RwUInt32);
		::fwrite(&uiTemp, sizeof(RwUInt32), 1, _pFile);
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_GameProperty(BYTE* _pFileMem, FILE* _pFile,  RwInt32 IdxSrcField, RwInt32 IdxDstField, CNtlWorldFieldManager* pFieldMgr)
{
	RwInt32 iGamePropertyNum;
	CopyMemory(&iGamePropertyNum, _pFileMem, sizeof(RwInt32));
	_pFileMem += sizeof(RwInt32);
	::fwrite(&iGamePropertyNum, sizeof(RwInt32), 1, _pFile);

	for (int i = 0; i < iGamePropertyNum; ++i)
	{
		RwUInt32 uiTemp;
		CopyMemory(&uiTemp, _pFileMem, sizeof(RwUInt32));
		_pFileMem += sizeof(RwUInt32);
		::fwrite(&uiTemp, sizeof(RwUInt32), 1, _pFile);

		RwV3d vTemp;
		CopyMemory(&vTemp, _pFileMem, sizeof(RwV3d));
		_pFileMem += sizeof(RwV3d);
		if(pFieldMgr)
		{
			RwV3d	PosSrcLocal;
			RwV3d	PosDstWorld;
			RwV3d	PosTranslation = vTemp;

			// relative coordinates
			CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
			RwV3d PosSrc = pFields[IdxSrcField].GetSPos();
			RwV3d PosDst = pFields[IdxDstField].GetSPos();

			PosSrcLocal.x = PosTranslation.x - PosSrc.x;
			PosSrcLocal.y = PosTranslation.y;
			PosSrcLocal.z = PosTranslation.z - PosSrc.z;
			PosSrcLocal.x = fabs(PosSrcLocal.x);
			PosSrcLocal.z = fabs(PosSrcLocal.z);

			PosDstWorld.x = PosSrcLocal.x + PosDst.x;
			PosDstWorld.z = PosSrcLocal.z + PosDst.z;
			PosDstWorld.y = PosTranslation.y;

			fwrite(&PosDstWorld, sizeof(RwV3d), 1, _pFile);
		}
		else
		{
			::fwrite(&vTemp, sizeof(RwV3d), 1, _pFile);
		}

		CopyMemory(&vTemp, _pFileMem, sizeof(RwV3d));
		_pFileMem += sizeof(RwV3d);
		::fwrite(&vTemp, sizeof(RwV3d), 1, _pFile);

		CopyMemory(&uiTemp, _pFileMem, sizeof(RwUInt32));
		_pFileMem += sizeof(RwUInt32);
		::fwrite(&uiTemp, sizeof(RwUInt32), 1, _pFile);
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Skip_Name(BYTE* _pFileMem)
{
	return (_pFileMem + 64);
}

BYTE* CNtlWorldFileMemAccessor::Skip_BitProp(BYTE* _pFileMem)
{
	return (_pFileMem + (sizeof(DWORD) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2)));
}

BYTE* CNtlWorldFileMemAccessor::Skip_Fog(BYTE* _pFileMem)
{
	return (_pFileMem + sizeof(RwRGBA) + (sizeof(RwReal) * 2));
}

BYTE* CNtlWorldFileMemAccessor::Skip_Sky(BYTE* _pFileMem)
{
	RwInt32 SizMemBlock =	sizeof(RwInt32)	+			// base sky mode
							(sizeof(RwRGBA) * 3) +		// rgb sky color
							(sizeof(RwChar) * 10) +		// base sky texture name
							sizeof(RwReal) +			// base sky speed
							(sizeof(RwChar) * 10 * 2) +	// blended tex name
							sizeof(RwReal) * 2;			// blended tex speed

	return (_pFileMem + SizMemBlock);
}

BYTE* CNtlWorldFileMemAccessor::Skip_RealLight(BYTE* _pFileMem)
{
	return (_pFileMem +				
			sizeof(RwRGBAReal) +	// light ambient
			sizeof(RwRGBAReal) +	// light directional angle	
			sizeof(RwV3d) +
			sizeof(RwRGBAReal) +	// light directional angle	
			sizeof(RwV3d) +
			sizeof(RwRGBAReal) +	// light directional angle	
			sizeof(RwV3d));	
}

BYTE* CNtlWorldFileMemAccessor::Skip_BGM(BYTE* _pFileMem)
{
	return (_pFileMem + sizeof(RwInt32) + sizeof(RwInt32) + (sizeof(RwInt32) * 5));
}

BYTE* CNtlWorldFileMemAccessor::Skip_FieldColor(BYTE* _pFileMem)
{
	return (_pFileMem + sizeof(RwRGBA));
}

BYTE* CNtlWorldFileMemAccessor::Skip_Weather(BYTE* _pFileMem)
{
	return (_pFileMem + sizeof(RwInt32) * 5);
}

BYTE* CNtlWorldFileMemAccessor::Skip_FieldHeatHaze(BYTE* _pFileMem)
{
	return (_pFileMem + sizeof(sNTL_HEATHAZE));
}

BYTE* CNtlWorldFileMemAccessor::Skip_BlockGameProperty(BYTE* _pFileMem)
{
	return (_pFileMem + sizeof(RwUInt32));
}

BYTE* CNtlWorldFileMemAccessor::Skip_Bloom(BYTE* _pFileMem)
{
	return (_pFileMem + sizeof(RwReal));
}

BYTE* CNtlWorldFileMemAccessor::Skip_Planet(BYTE* _pFileMem)
{
	return (_pFileMem + sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT);
}

BYTE* CNtlWorldFileMemAccessor::Skip_Specular(BYTE* _pFileMem)
{
	return (_pFileMem + sizeof(sNTL_SPECULAR));
}

BYTE* CNtlWorldFileMemAccessor::Skip_WorldMaterial(BYTE* _pFileMem)
{
	return (_pFileMem + (sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2)));
}

BYTE* CNtlWorldFileMemAccessor::Save_Name(BYTE* _pFileMem, FILE* _pFile)
{
	fwrite(_pFileMem, sizeof(RwChar) * 64, 1, _pFile);
	_pFileMem += sizeof(RwChar) * 64;

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_BitProp(BYTE* _pFileMem, FILE* _pFile)
{
	RwInt32 SizMemBlock = sizeof(DWORD) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2);
	
	fwrite(_pFileMem, SizMemBlock, 1, _pFile);
	_pFileMem += SizMemBlock;

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_Fog(BYTE* _pFileMem, FILE* _pFile)
{
	RwInt32 SizMemBlock =	sizeof(RwRGBA) +		// fog color
							(sizeof(RwReal) * 2);	// fog cam plane; near, far
	
	fwrite(_pFileMem, SizMemBlock, 1, _pFile);
	_pFileMem += SizMemBlock;

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_Sky(BYTE* _pFileMem, FILE* _pFile)
{
	RwInt32 SizMemBlock =	sizeof(RwInt32)	+			// base sky mode
							(sizeof(RwRGBA) * 3) +		// rgb sky color
							(sizeof(RwChar) * 10) +		// base sky texture name
							sizeof(RwReal) +			// base sky speed
							(sizeof(RwChar) * 10 * 2) +	// blended tex name
							sizeof(RwReal) * 2;			// blended tex speed

	fwrite(_pFileMem, SizMemBlock, 1, _pFile);
	_pFileMem += SizMemBlock;

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_RealLight(BYTE* _pFileMem, FILE* _pFile)
{
	RwInt32 SizMemBlock =	sizeof(RwRGBAReal) +	// light ambient
							sizeof(RwRGBAReal) +	// light directional
							sizeof(RwV3d) +			// light directional angle	
							sizeof(RwRGBAReal) +	// light directional
							sizeof(RwV3d) +			// light directional angle	
							sizeof(RwRGBAReal) +	// light directional
							sizeof(RwV3d);			// light directional angle	

	fwrite(_pFileMem, SizMemBlock, 1, _pFile);
	_pFileMem += SizMemBlock;

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_BGM(BYTE* _pFileMem, FILE* _pFile)
{
	// Bgm
	fwrite(_pFileMem, sizeof(RwInt32), 1, _pFile);
	_pFileMem += sizeof(RwInt32);

	// Env
	fwrite(_pFileMem, sizeof(RwInt32), 1, _pFile);
	_pFileMem += sizeof(RwInt32);

	// Share
	for (int i = 0; i < 5; ++i)
	{
		fwrite(_pFileMem, sizeof(RwInt32), 1, _pFile);
		_pFileMem += sizeof(RwInt32);
	}

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_Bloom(BYTE* _pFileMem, FILE* _pFile)
{
	fwrite(_pFileMem, sizeof(RwReal), 1, _pFile);
	_pFileMem += sizeof(RwReal);

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_FieldColor(BYTE* _pFileMem, FILE* _pFile)
{
	fwrite(_pFileMem, sizeof(RwRGBA), 1, _pFile);
	_pFileMem += sizeof(RwRGBA);

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_Weather(BYTE* _pFileMem, FILE* _pFile)
{
	fwrite(_pFileMem, sizeof(RwInt32) * 5, 1, _pFile);
	_pFileMem += sizeof(RwInt32) * 5;

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_FieldHeatHaze(BYTE* _pFileMem, FILE* _pFile)
{
	fwrite(_pFileMem, sizeof(sNTL_HEATHAZE), 1, _pFile);
	_pFileMem += sizeof(sNTL_HEATHAZE);

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_BlockGameProperty(BYTE* _pFileMem, FILE* _pFile)
{
	fwrite(_pFileMem, sizeof(RwUInt32), 1, _pFile);
	_pFileMem += sizeof(RwUInt32);

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_Planet(BYTE* _pFileMem, FILE* _pFile)
{
	fwrite(_pFileMem, sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT, 1, _pFile);
	_pFileMem += sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT;

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_Specular(BYTE* _pFileMem, FILE* _pFile)
{
	fwrite(_pFileMem, sizeof(sNTL_SPECULAR), 1, _pFile);
	_pFileMem += sizeof(sNTL_SPECULAR);

	return _pFileMem;
}

BYTE* CNtlWorldFileMemAccessor::Save_WorldMaterial(BYTE* _pFileMem, FILE* _pFile)
{
	RwInt32 SizMemBlock = sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2);

	fwrite(_pFileMem, SizMemBlock, 1, _pFile);
	_pFileMem += SizMemBlock;

	return _pFileMem;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif