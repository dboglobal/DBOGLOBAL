#include "precomp_ntlpresentation.h"

#include "NtlWorldPaletteDefine.h"
#include "NtlPLWorldDecal.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

CNtlPLWorldDecal::CNtlPLWorldDecal()
{
}

CNtlPLWorldDecal::~CNtlPLWorldDecal()
{
}

VOID CNtlPLWorldDecal::GetProp(sDECAL_PROP_PARAM& _DecalPropParam)
{
	strcpy_s(_DecalPropParam._TexName, sizeof(RwChar) * 128, GetName());

	_DecalPropParam._TexAngle		= GetRotate();
	_DecalPropParam._TexScale		= GetBaseScale();
	_DecalPropParam._SizeX			= GetSize().x;
	_DecalPropParam._SizeZ			= GetSize().z;
	_DecalPropParam._VisibilityDist	= GetVisibleDist();

	if((GetEffectFlag() & rpPRTSTDEMITTERDATAFLAGUVANIM) && GetUVChunk().fMaxOffset < 0.01f)
	{
		_DecalPropParam._UVAniSpeed			= GetUVChunk().fVelocity;
		_DecalPropParam._UVAniOffset		= GetUVChunk().fMaxOffset;
		_DecalPropParam._UVAniPatternIdx	= 0;
		_DecalPropParam._UVAniCheck			= TRUE;
	}
	else if((GetEffectFlag() & rpPRTSTDEMITTERDATAFLAGUVANIM) && GetUVChunk().fMaxOffset > 0.01f)
	{
		_DecalPropParam._UVAniSpeed			= GetUVChunk().fVelocity;
		_DecalPropParam._UVAniOffset		= GetUVChunk().fMaxOffset;
		_DecalPropParam._UVAniPatternIdx	= 1;
		_DecalPropParam._UVAniCheck			= TRUE;
	}
	else if((GetEffectFlag() & rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
	{
		RwReal Ang, Gap;
		GetRotateActionValue(Ang, Gap);

		_DecalPropParam._UVAniSpeed			= Gap;
		_DecalPropParam._UVAniOffset		= Ang;
		_DecalPropParam._UVAniPatternIdx	= 2;
		_DecalPropParam._UVAniCheck			= TRUE;
	}
	else if((GetEffectFlag() & rpPRTSTDEMITTERDATAFLAGPRTSIZE))
	{
		RwReal StartScale, EndScale, TimeGap;
		GetScaleActionValue(StartScale, EndScale, TimeGap);

		_DecalPropParam._UVAniSpeed			= TimeGap;
		_DecalPropParam._UVAniOffset		= EndScale;
		_DecalPropParam._UVAniPatternIdx	= 3;
		_DecalPropParam._UVAniCheck			= TRUE;
	}
	else
	{
		_DecalPropParam._UVAniSpeed			= -999.0f;
		_DecalPropParam._UVAniOffset		= -999.0f;
		_DecalPropParam._UVAniPatternIdx	= -999;
		_DecalPropParam._UVAniCheck			= FALSE;
	}

	if(	(GetEffectFlag() & E_FLAG_DECAL_RENDER_WORLD) &&
		!(GetEffectFlag() & E_FLAG_DECAL_RENDER_OBJECT) &&
		!(GetEffectFlag() & E_FLAG_DECAL_WATER))
	{
		_DecalPropParam._DecalMode = 0;
	}
	else if(!(GetEffectFlag() & E_FLAG_DECAL_RENDER_WORLD) &&
		(GetEffectFlag() & E_FLAG_DECAL_RENDER_OBJECT) &&
		!(GetEffectFlag() & E_FLAG_DECAL_WATER))
	{
		_DecalPropParam._DecalMode = 1;
	}
	else if((GetEffectFlag() & E_FLAG_DECAL_RENDER_WORLD) &&
		(GetEffectFlag() & E_FLAG_DECAL_RENDER_OBJECT) &&
		!(GetEffectFlag() & E_FLAG_DECAL_WATER))
	{
		_DecalPropParam._DecalMode = 2;
	}
	else if(!(GetEffectFlag() & E_FLAG_DECAL_RENDER_WORLD) &&
		(GetEffectFlag() & E_FLAG_DECAL_RENDER_OBJECT) &&
		(GetEffectFlag() & E_FLAG_DECAL_WATER))
	{
		_DecalPropParam._DecalMode = 3;
	}
	else
	{
		_DecalPropParam._DecalMode = 999;
	}

	_DecalPropParam._RGBA	= GetColor();
	_DecalPropParam._Pos	= GetPosition();
}

// VOID CNtlPLWorldDecal::Save(FILE* pFile)
// {
// 	sDECAL_PROP_PARAM DecalPropParam;
// 	GetProp(DecalPropParam);
// 
// 	fwrite(&DecalPropParam, sizeof(sDECAL_PROP_PARAM), 1, pFile);
// }
// 
VOID CNtlPLWorldDecal::Load(sDECAL_PROP_PARAM& _DecalPropParam, FILE* pFile)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION_OLD))
	{
		fread(&_DecalPropParam, sizeof(sDECAL_PROP_PARAM), 1, pFile);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		fread(&uiLength, sizeof(RwUInt32), 1, pFile);
		fread(_DecalPropParam._TexName, sizeof(RwChar) * uiLength, 1 , pFile);
		_DecalPropParam._TexName[uiLength] = '\0';

		fread(&_DecalPropParam._TexAngle, sizeof(RwReal), 1 , pFile);
		fread(&_DecalPropParam._TexScale, sizeof(RwReal), 1 , pFile);
		fread(&_DecalPropParam._SizeX, sizeof(RwReal), 1 , pFile);
		fread(&_DecalPropParam._SizeZ, sizeof(RwReal), 1 , pFile);
		fread(&_DecalPropParam._VisibilityDist, sizeof(RwReal), 1 , pFile);
		fread(&_DecalPropParam._UVAniSpeed, sizeof(RwReal), 1 , pFile);
		fread(&_DecalPropParam._UVAniOffset, sizeof(RwReal), 1 , pFile);
		fread(&_DecalPropParam._UVAniPatternIdx, sizeof(RwInt32), 1 , pFile);
		fread(&_DecalPropParam._UVAniCheck, sizeof(RwBool), 1 , pFile);
		fread(&_DecalPropParam._DecalMode, sizeof(RwInt32), 1 , pFile);
		fread(&_DecalPropParam._RGBA, sizeof(RwRGBA), 1 , pFile);
		fread(&_DecalPropParam._Pos, sizeof(RwV3d), 1 , pFile);
	}
}

RwBool CNtlPLWorldDecal::SaveIntoFile(FILE* pFile)
{
	sDECAL_PROP_PARAM DecalPropParam;
	GetProp(DecalPropParam);

	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION_OLD))
	{
		fwrite(&DecalPropParam, sizeof(sDECAL_PROP_PARAM), 1, pFile);
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldSaveVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = strlen(DecalPropParam._TexName);

		fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		fwrite(DecalPropParam._TexName, sizeof(RwChar) * uiLength, 1 , pFile);

		fwrite(&DecalPropParam._TexAngle, sizeof(RwReal), 1 , pFile);
		fwrite(&DecalPropParam._TexScale, sizeof(RwReal), 1 , pFile);
		fwrite(&DecalPropParam._SizeX, sizeof(RwReal), 1 , pFile);
		fwrite(&DecalPropParam._SizeZ, sizeof(RwReal), 1 , pFile);
		fwrite(&DecalPropParam._VisibilityDist, sizeof(RwReal), 1 , pFile);
		fwrite(&DecalPropParam._UVAniSpeed, sizeof(RwReal), 1 , pFile);
		fwrite(&DecalPropParam._UVAniOffset, sizeof(RwReal), 1 , pFile);
		fwrite(&DecalPropParam._UVAniPatternIdx, sizeof(RwInt32), 1 , pFile);
		fwrite(&DecalPropParam._UVAniCheck, sizeof(RwBool), 1 , pFile);
		fwrite(&DecalPropParam._DecalMode, sizeof(RwInt32), 1 , pFile);
		fwrite(&DecalPropParam._RGBA, sizeof(RwRGBA), 1 , pFile);
		fwrite(&DecalPropParam._Pos, sizeof(RwV3d), 1 , pFile);
	}

	return TRUE;
}

RwInt32 CNtlPLWorldDecal::SkipToFile(FILE* pFile)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldLoadVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;
		fread(&uiLength, sizeof(RwUInt32), 1, pFile);
		fseek(pFile, sizeof(RwChar) * uiLength, SEEK_CUR);

		fseek(pFile, sizeof(RwReal), SEEK_CUR);
		fseek(pFile, sizeof(RwReal), SEEK_CUR);
		fseek(pFile, sizeof(RwReal), SEEK_CUR);
		fseek(pFile, sizeof(RwReal), SEEK_CUR);
		fseek(pFile, sizeof(RwReal), SEEK_CUR);
		fseek(pFile, sizeof(RwReal), SEEK_CUR);
		fseek(pFile, sizeof(RwReal), SEEK_CUR);

		fseek(pFile, sizeof(RwInt32), SEEK_CUR);
		fseek(pFile, sizeof(RwBool), SEEK_CUR);
		fseek(pFile, sizeof(RwInt32), SEEK_CUR);
		fseek(pFile, sizeof(RwRGBA), SEEK_CUR);
		fseek(pFile, sizeof(RwV3d), SEEK_CUR);
	}

	return ftell(pFile);
}

BYTE* CNtlPLWorldDecal::SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
	{
		RwInt32 MemSizeExceptPos =	(sizeof(RwChar) * 128)	+	// TexName[128];
			(sizeof(RwReal) * 7)	+	// TexAngle, TexScale, SizeX, SizeZ, VisibilityDist, UVAniSpeed, UVAniOffset
			(sizeof(RwInt32))		+	// UVAniPatternIdx
			(sizeof(RwBool))		+	// UVAniCheck
			(sizeof(RwInt32))		+	// DecalMode
			(sizeof(RwRGBA));			// RGBA

		fwrite(pFileMem, MemSizeExceptPos, 1, pFile);
		pFileMem += MemSizeExceptPos;
	}
	else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
		pFileMem += sizeof(RwUInt32);

		fwrite(pFileMem, (sizeof(RwChar) * uiLength), 1, pFile);
		pFileMem += (sizeof(RwChar) * uiLength); // TexName[128];

		RwInt32 MemSizeExceptPos =	(sizeof(RwReal) * 7)	+	// TexAngle, TexScale, SizeX, SizeZ, VisibilityDist, UVAniSpeed, UVAniOffset
			(sizeof(RwInt32))		+	// UVAniPatternIdx
			(sizeof(RwBool))		+	// UVAniCheck
			(sizeof(RwInt32))		+	// DecalMode
			(sizeof(RwRGBA));			// RGBA

		fwrite(pFileMem, MemSizeExceptPos, 1, pFile);
		pFileMem += MemSizeExceptPos;
	}

	RwV3d vPos;
	CopyMemory(&vPos, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	if (pvOffset)
	{
		vPos.x += pvOffset->x;
		vPos.y += pvOffset->y;
		vPos.z += pvOffset->z;
	}
	::fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	return pFileMem;
}

BYTE* CNtlPLWorldDecal::SkipToFileMem(BYTE* pFileMem)
{
	if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
	{
		RwUInt32 uiLength = 0;

		CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
		pFileMem += sizeof(RwUInt32);

		pFileMem += (sizeof(RwChar) * uiLength); // TexName[128];

		RwInt32 MemSizeExceptPos =	(sizeof(RwReal) * 7)	+	// TexAngle, TexScale, SizeX, SizeZ, VisibilityDist, UVAniSpeed, UVAniOffset
			(sizeof(RwInt32))		+	// UVAniPatternIdx
			(sizeof(RwBool))		+	// UVAniCheck
			(sizeof(RwInt32))		+	// DecalMode
			(sizeof(RwRGBA))		+	// RGBA
			(sizeof(RwV3d));			// Pos

		pFileMem += MemSizeExceptPos;
	}

	return pFileMem;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

CNtlPLWorldDecal::CNtlPLWorldDecal()
{
}

CNtlPLWorldDecal::~CNtlPLWorldDecal()
{
}

VOID CNtlPLWorldDecal::GetProp(sDECAL_PROP_PARAM& _DecalPropParam)
{
	_DecalPropParam._TexNameLen = GetNameLen();
	strcpy_s(_DecalPropParam._TexName, sizeof(RwChar) * GetNameLen(), GetName());

	_DecalPropParam._TexAngle		= GetRotate();
	_DecalPropParam._TexScale		= GetBaseScale();
	_DecalPropParam._SizeX			= GetSize().x;
	_DecalPropParam._SizeZ			= GetSize().z;
	_DecalPropParam._VisibilityDist	= GetVisibleDist();

	if((GetEffectFlag() & rpPRTSTDEMITTERDATAFLAGUVANIM) && GetUVChunk().fMaxOffset < 0.01f)
	{
		_DecalPropParam._UVAniSpeed			= GetUVChunk().fVelocity;
		_DecalPropParam._UVAniOffset		= GetUVChunk().fMaxOffset;
		_DecalPropParam._UVAniPatternIdx	= 0;
		_DecalPropParam._UVAniCheck			= TRUE;
	}
	else if((GetEffectFlag() & rpPRTSTDEMITTERDATAFLAGUVANIM) && GetUVChunk().fMaxOffset > 0.01f)
	{
		_DecalPropParam._UVAniSpeed			= GetUVChunk().fVelocity;
		_DecalPropParam._UVAniOffset		= GetUVChunk().fMaxOffset;
		_DecalPropParam._UVAniPatternIdx	= 1;
		_DecalPropParam._UVAniCheck			= TRUE;
	}
	else if((GetEffectFlag() & rpPRTSTDEMITTERDATAFLAGPRT2DROTATE))
	{
		RwReal Ang, Gap;
		GetRotateActionValue(Ang, Gap);

		_DecalPropParam._UVAniSpeed			= Gap;
		_DecalPropParam._UVAniOffset		= Ang;
		_DecalPropParam._UVAniPatternIdx	= 2;
		_DecalPropParam._UVAniCheck			= TRUE;
	}
	else if((GetEffectFlag() & rpPRTSTDEMITTERDATAFLAGPRTSIZE))
	{
		RwReal StartScale, EndScale, TimeGap;
		GetScaleActionValue(StartScale, EndScale, TimeGap);

		_DecalPropParam._UVAniSpeed			= TimeGap;
		_DecalPropParam._UVAniOffset		= EndScale;
		_DecalPropParam._UVAniPatternIdx	= 3;
		_DecalPropParam._UVAniCheck			= TRUE;
	}
	else
	{
		_DecalPropParam._UVAniSpeed			= -999.0f;
		_DecalPropParam._UVAniOffset		= -999.0f;
		_DecalPropParam._UVAniPatternIdx	= -999;
		_DecalPropParam._UVAniCheck			= FALSE;
	}

	if(	(GetEffectFlag() & E_FLAG_DECAL_RENDER_WORLD) &&
		!(GetEffectFlag() & E_FLAG_DECAL_RENDER_OBJECT) &&
		!(GetEffectFlag() & E_FLAG_DECAL_WATER))
	{
		_DecalPropParam._DecalMode = 0;
	}
	else if(!(GetEffectFlag() & E_FLAG_DECAL_RENDER_WORLD) &&
		(GetEffectFlag() & E_FLAG_DECAL_RENDER_OBJECT) &&
		!(GetEffectFlag() & E_FLAG_DECAL_WATER))
	{
		_DecalPropParam._DecalMode = 1;
	}
	else if((GetEffectFlag() & E_FLAG_DECAL_RENDER_WORLD) &&
		(GetEffectFlag() & E_FLAG_DECAL_RENDER_OBJECT) &&
		!(GetEffectFlag() & E_FLAG_DECAL_WATER))
	{
		_DecalPropParam._DecalMode = 2;
	}
	else if(!(GetEffectFlag() & E_FLAG_DECAL_RENDER_WORLD) &&
		(GetEffectFlag() & E_FLAG_DECAL_RENDER_OBJECT) &&
		(GetEffectFlag() & E_FLAG_DECAL_WATER))
	{
		_DecalPropParam._DecalMode = 3;
	}
	else
	{
		_DecalPropParam._DecalMode = 999;
	}

	_DecalPropParam._RGBA	= GetColor();
	_DecalPropParam._Pos	= GetPosition();
}

// VOID CNtlPLWorldDecal::Save(FILE* pFile)
// {
// 	sDECAL_PROP_PARAM DecalPropParam;
// 	GetProp(DecalPropParam);
// 
// 	fwrite(&DecalPropParam, sizeof(sDECAL_PROP_PARAM), 1, pFile);
// }
// 
VOID CNtlPLWorldDecal::Load(sDECAL_PROP_PARAM& _DecalPropParam, FILE* pFile)
{
	RwUInt32 uiLength = 0;

	fread(&uiLength, sizeof(RwUInt32), 1, pFile);
	fread(_DecalPropParam._TexName, sizeof(RwChar) * uiLength, 1, pFile);
	_DecalPropParam._TexName[uiLength] = '\0';

	fread(&_DecalPropParam._TexAngle, sizeof(RwReal), 1, pFile);
	fread(&_DecalPropParam._TexScale, sizeof(RwReal), 1, pFile);
	fread(&_DecalPropParam._SizeX, sizeof(RwReal), 1, pFile);
	fread(&_DecalPropParam._SizeZ, sizeof(RwReal), 1, pFile);
	fread(&_DecalPropParam._VisibilityDist, sizeof(RwReal), 1, pFile);
	fread(&_DecalPropParam._UVAniSpeed, sizeof(RwReal), 1, pFile);
	fread(&_DecalPropParam._UVAniOffset, sizeof(RwReal), 1, pFile);
	fread(&_DecalPropParam._UVAniPatternIdx, sizeof(RwInt32), 1, pFile);
	fread(&_DecalPropParam._UVAniCheck, sizeof(RwBool), 1, pFile);
	fread(&_DecalPropParam._DecalMode, sizeof(RwInt32), 1, pFile);
	fread(&_DecalPropParam._RGBA, sizeof(RwRGBA), 1, pFile);
	fread(&_DecalPropParam._Pos, sizeof(RwV3d), 1, pFile);
}

RwBool CNtlPLWorldDecal::SaveIntoFile(FILE* pFile)
{
	sDECAL_PROP_PARAM DecalPropParam;
	GetProp(DecalPropParam);

	RwUInt32 uiLength = strlen(DecalPropParam._TexName);

	fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
	fwrite(DecalPropParam._TexName, sizeof(RwChar) * uiLength, 1, pFile);

	fwrite(&DecalPropParam._TexAngle, sizeof(RwReal), 1, pFile);
	fwrite(&DecalPropParam._TexScale, sizeof(RwReal), 1, pFile);
	fwrite(&DecalPropParam._SizeX, sizeof(RwReal), 1, pFile);
	fwrite(&DecalPropParam._SizeZ, sizeof(RwReal), 1, pFile);
	fwrite(&DecalPropParam._VisibilityDist, sizeof(RwReal), 1, pFile);
	fwrite(&DecalPropParam._UVAniSpeed, sizeof(RwReal), 1, pFile);
	fwrite(&DecalPropParam._UVAniOffset, sizeof(RwReal), 1, pFile);
	fwrite(&DecalPropParam._UVAniPatternIdx, sizeof(RwInt32), 1, pFile);
	fwrite(&DecalPropParam._UVAniCheck, sizeof(RwBool), 1, pFile);
	fwrite(&DecalPropParam._DecalMode, sizeof(RwInt32), 1, pFile);
	fwrite(&DecalPropParam._RGBA, sizeof(RwRGBA), 1, pFile);
	fwrite(&DecalPropParam._Pos, sizeof(RwV3d), 1, pFile);

	return TRUE;
}

RwInt32 CNtlPLWorldDecal::SkipToFile(FILE* pFile)
{
	RwUInt32 uiLength = 0;
	fread(&uiLength, sizeof(RwUInt32), 1, pFile);
	fseek(pFile, sizeof(RwChar) * uiLength, SEEK_CUR);

	fseek(pFile, sizeof(RwReal), SEEK_CUR);
	fseek(pFile, sizeof(RwReal), SEEK_CUR);
	fseek(pFile, sizeof(RwReal), SEEK_CUR);
	fseek(pFile, sizeof(RwReal), SEEK_CUR);
	fseek(pFile, sizeof(RwReal), SEEK_CUR);
	fseek(pFile, sizeof(RwReal), SEEK_CUR);
	fseek(pFile, sizeof(RwReal), SEEK_CUR);

	fseek(pFile, sizeof(RwInt32), SEEK_CUR);
	fseek(pFile, sizeof(RwBool), SEEK_CUR);
	fseek(pFile, sizeof(RwInt32), SEEK_CUR);
	fseek(pFile, sizeof(RwRGBA), SEEK_CUR);
	fseek(pFile, sizeof(RwV3d), SEEK_CUR);

	return ::ftell(pFile);
}

BYTE* CNtlPLWorldDecal::SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset /* = NULL */)
{
	RwUInt32 uiLength = 0;

	CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
	fwrite(&uiLength, sizeof(RwUInt32), 1, pFile);
	pFileMem += sizeof(RwUInt32);

	fwrite(pFileMem, (sizeof(RwChar) * uiLength), 1, pFile);
	pFileMem += (sizeof(RwChar) * uiLength); // TexName[128];

	RwInt32 MemSizeExceptPos = (sizeof(RwReal) * 7) +	// TexAngle, TexScale, SizeX, SizeZ, VisibilityDist, UVAniSpeed, UVAniOffset
		(sizeof(RwInt32)) +	// UVAniPatternIdx
		(sizeof(RwBool)) +	// UVAniCheck
		(sizeof(RwInt32)) +	// DecalMode
		(sizeof(RwRGBA));			// RGBA

	fwrite(pFileMem, MemSizeExceptPos, 1, pFile);
	pFileMem += MemSizeExceptPos;

	RwV3d vPos;
	CopyMemory(&vPos, pFileMem, sizeof(RwV3d));
	pFileMem += sizeof(RwV3d);
	if (pvOffset)
	{
		vPos.x += pvOffset->x;
		vPos.y += pvOffset->y;
		vPos.z += pvOffset->z;
	}
	::fwrite(&vPos, sizeof(RwV3d), 1, pFile);

	return pFileMem;
}

BYTE* CNtlPLWorldDecal::SkipToFileMem(BYTE* pFileMem)
{
	RwUInt32 uiLength = 0;

	CopyMemory(&uiLength, pFileMem, sizeof(RwUInt32));
	pFileMem += sizeof(RwUInt32);

	pFileMem += (sizeof(RwChar) * uiLength); // TexName[128];

	RwInt32 MemSizeExceptPos = (sizeof(RwReal) * 7) +	// TexAngle, TexScale, SizeX, SizeZ, VisibilityDist, UVAniSpeed, UVAniOffset
		(sizeof(RwInt32)) +	// UVAniPatternIdx
		(sizeof(RwBool)) +	// UVAniCheck
		(sizeof(RwInt32)) +	// DecalMode
		(sizeof(RwRGBA)) +	// RGBA
		(sizeof(RwV3d));			// Pos

	pFileMem += MemSizeExceptPos;


	return pFileMem;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif