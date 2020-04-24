#ifndef __NTL_PL_WORLD_DECAL_H__
#define __NTL_PL_WORLD_DECAL_H__


#include "NtlDebug.h"
#include "ceventhandler.h"
#include "NtlPLDecal.h"


#pragma pack(1)

// Parameters using properties
struct sDECAL_PROP_PARAM
{
	RwInt32 _TexNameLen;
	RwChar	_TexName[128];
	RwReal	_TexAngle;
	RwReal	_TexScale;
	RwReal	_SizeX;
	RwReal	_SizeZ;
	RwReal	_VisibilityDist;
	RwReal	_UVAniSpeed;
	RwReal	_UVAniOffset;
	RwInt32	_UVAniPatternIdx;
	RwBool	_UVAniCheck;
	RwInt32	_DecalMode;
	RwRGBA	_RGBA;
	RwV3d	_Pos;

public:
	inline void operator = (sDECAL_PROP_PARAM& _DecalPropParam)
	{
		_TexNameLen				= _DecalPropParam._TexNameLen;
		strcpy_s(_TexName, _DecalPropParam._TexName);

		_RGBA					= _DecalPropParam._RGBA;
		_TexAngle				= _DecalPropParam._TexAngle;
		_TexScale				= _DecalPropParam._TexScale;
		_SizeX					= _DecalPropParam._SizeX;
		_SizeZ					= _DecalPropParam._SizeZ;
		_VisibilityDist			= _DecalPropParam._VisibilityDist;
		_UVAniSpeed				= _DecalPropParam._UVAniSpeed;
		_UVAniOffset			= _DecalPropParam._UVAniOffset;
		_UVAniPatternIdx		= _DecalPropParam._UVAniPatternIdx;
		_UVAniCheck				= _DecalPropParam._UVAniCheck;
		_DecalMode				= _DecalPropParam._DecalMode;
	}
};

#pragma pack()

class CNtlPLWorldDecal : public CNtlPLDecal
{
public:
	CNtlPLWorldDecal(void);
	virtual	~CNtlPLWorldDecal(void);

protected:
	VOID GetProp(sDECAL_PROP_PARAM& _DecalPropParam);

public:
	static VOID Load(sDECAL_PROP_PARAM& _DecalPropParam, FILE* pFile);

public:
	virtual RwBool	SaveIntoFile(FILE* pFile);
	static	RwInt32 SkipToFile(FILE* pFile);

	static	BYTE*	SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset = NULL);
	static	BYTE*	SkipToFileMem(BYTE* pFileMem);
};

#endif __NTL_PL_WORLD_DECAL_H__