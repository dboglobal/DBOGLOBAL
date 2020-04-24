/******************************************************************************************************************************
*
* ntlworldcommon_h
*
* copyright (c) 2005 ntl-inc Ltd.
*
* author : woody1019
*
* purpose : ntl world structure collection
*
******************************************************************************************************************************/

#ifndef ntlworldcommon_h
#define ntlworldcommon_h 


#pragma warning(disable:4819)


/******************************************************************************************************************************
* headers 
******************************************************************************************************************************/


#include <WinSock2.h>
#include <windows.h>
#include "ntlworldmouse.h"
#include "NtlPLEntity.h"
#include <rwcore.h>
#include <rpworld.h>
#include <rtimport.h>
#include <d3dx9.h>
#include "NtlDebug.h"
#include "NtlPostEffectCamera.h"
#include "NtlPLSceneManager.h"

#include <vector>
using std::vector;

class CNtlWorldSector;
struct sNtlWorldParam;
/******************************************************************************************************************************
* prototypes
******************************************************************************************************************************/
VOID SaveCurWorldState();
VOID SaveFileWorldState(sNtlWorldParam* pNtlWorldParam, FILE* pFile);
VOID LoadFileWorldState(sNtlWorldParam* pNtlWorldParam, FILE* pFile);

/******************************************************************************************************************************
* enumerations : direction
******************************************************************************************************************************/
enum eNTL_WORLD_DIR { eC = -1, eN, eNE, eE, eES, eS, eSW, eW, eWN, eIDLE, ePORTAL };

enum eNTL_FIELD_PROPID
{
	eNFP_NAME = 0,
	eNFP_BITPROP,
	eNFP_FOG,
	eNFP_SKY,
	eNFP_LIGHT,
	eNFP_SOUND,
	eNFP_BLOOM,
	eNFP_PLANET,
	eNFP_SPECULAR,
	eNFP_MATERIALPROP,
	eNFP_FIELDCOLOR,
	eNFP_WEATHER,
	eNFP_FIELDHEATHAZE,
};

enum eNTL_BLOCK_PROPID
{
	eNBP_GAMEPROP = 0,
};

// When you want to use the client, be sure to comment and rebuild - Woody
//#define dNTL_WORLD_TOOL_MODE "DEV"

#define dNTL_WORLD_FILE

#ifndef dNTL_WORLD_TOOL_MODE
#define dNTL_WORLD_SCHEDULE_LOADING
#endif


#define dNTL_WORLD_VERSION_OLD					("1.1")
#define dNTL_WORLD_VERSION						("1.4")
#define dNTL_WORLD_VERSION_COMPARE(src, dst)	(!strcmp(src, dst))

// #define dNTL_WORLD_FORMAT_CONVERT
// #ifdef dNTL_WORLD_FORMAT_CONVERT
// #define dNTL_WORLD_FORMAT_CONVERT_COMPLTE
// #endif

//Terrain Texture Combine Use : Realtime
//#define dNTL_WORLD_COMBINE		// by using this, the texture on ground gets weird

// definitions : plugin developers id, plugin mode
#define dID_NTL_WORLD				(0xff)
#define rwID_NTL_WORLD				MAKECHUNKID(rwVENDORID_CRITERIONTK, 0xff)
#define dNTL_SPLAT_LAYER_NUM		(5)
#define dNTL_WORLD_FILE_NAME_SIZE	(40)
#define dNTL_SPLAT_ALPHA_SIZE		(32)
#define dNTL_WORLD_EXC_AOI_SIZE		(512)

// definitions : plugin data access macros
#define dNTL_WORLD_GLOBAL(var)				(RWPLUGINOFFSET(sNtlWorldGlobals, RwEngineInstance, NtlWorldGlobalOffset)->var)
#define dNTL_WORLD_LOCAL(sector, var)		(RWPLUGINOFFSET(sNtlWorldLocals, sector, NtlWorldLocalOffset)->var)
#define dNTL_WORLD_SECTOR_LOCAL(world, var)	(RWPLUGINOFFSET(sNtlSectorLocals, world, NtlWorldSectorLocalOffset)->var)

// util
#define CLAMP(x, min, max) (x = (x < min ? min : x < max ? x : max))

// virtual world coordinates
#define dGET_POSITIVE_VALUE(Value) (Value + static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf))
#define dGET_NEGATIVE_VALUE(Value) (Value - static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSizeHalf))

//
#define dNTL_PL_DIRECTIONAL_LIGHT_NUM 3

/******************************************************************************************************************************
* structures
******************************************************************************************************************************/
typedef struct sNtlWorldParam sNtlWorldParam;
struct sNtlWorldParam
{	
	RwChar		WorldVer[10];
	RwChar		WorldSaveVer[10];
	RwChar		WorldLoadVer[10];

	RwUInt32	WorldResourceID;	//new in ver 1.4

	RwInt32		WorldHeightfieldNum;
	RwInt32		WorldSectorTileSize;
	RwInt32		WorldSectorTileNum;
	RwInt32		WorldSectorVertNum;
	RwInt32		WorldSectorSize;
	RwInt32		WorldSectorNum;
	RwInt32		WorldSectorHalfNum;
	RwInt32		WorldSectorPolyNum;
	RwInt32		WorldFieldSectorNum;
	RwInt32		WorldFieldSize;
	RwInt32		WorldFieldNum;
	RwInt32		WorldFieldHalfNum;
	RwInt32		WorldSize;
	RwInt32		WorldSizeHalf;
	RwInt32		WorldValueMax;
	RwInt32		WorldValueMin;
	RwInt32		WorldTexAddr;
	RwInt32		WorldPolyCnt;

	// 인도어
	RwInt32		WorldBlockSize;
	// 인도어
	RwInt32		WorldBlockNum;

	RwReal		WorldNearPlane;
	RwReal		WorldFarPlane;
	RwReal		WorldCamSpeedPerSec;

	RwUInt32	WorldObjectSerialID;
	RwUInt32	WorldTriggerSerialID;

	// these aren't 4 saving but ignore 2 save
	RwChar		WorldProjectFolderName[NTL_MAX_DIR_PATH];
	RwChar		CurWorkingFolderName[NTL_MAX_DIR_PATH];
	RwChar		CurDefaultTexPath[NTL_MAX_DIR_PATH];
	RwChar		WorldChar64Buf[64];

	// 인도어
	RwInt32		WorldType; // 0:HeightWorld // 1:IndoorWorld(RwWorld)

	RwChar		WorldRAW[64];

	// 인도어
	RwChar		WorldBSP[64];

	RwBool		LODEnable;
	RwBool		Loading;

	RwReal		NumBrushRadiusInTile;
	RwReal		ValHeightfieldSensitivity;
	RwReal		ValTextureSensitivity;
	RwReal		ValDiffuseOpacitySensitivity;

	RwBool		BlockGuideLine;
	RwBool		FieldGuideLine;
	RwBool		SectorGuideLine;
	RwBool		FlgHeightValueShow;
	RwBool		FlgObjInfoShow;
	DWORD		ClrDayAndNight;
	DWORD		ClrDiffuserPicker;
	RwInt32		ClrBaseAmbient[3];
	DWORD		ClrShadow;

	RwInt32		IdxSkyType;
};

typedef struct sNtlWorldPlugInParam sNtlWorldPlugInParam;
struct sNtlWorldPlugInParam
{
	RpWorld *pWorld;
	RwFrame *pParentFrame;
};

typedef struct sNtlWorldSector sNtlWorldSector;
struct sNtlWorldSector
{
	CNtlWorldSector *pNtlWorldSector;
};

typedef struct sNtlWorldGlobals	sNtlWorldGlobals;
struct sNtlWorldGlobals
{
	/* rpworldsector rendering callback */
	RpWorldSectorCallBackRender WorldSectorCBRender;

	RwFreeList *pSectorFreelist;
};

typedef struct sNtlWorldLocals sNtlWorldLocals;
struct sNtlWorldLocals
{
	sNtlWorldSector *pNtlSector;
};

typedef struct sNtlSectorLocals	sNtlSectorLocals;
struct sNtlSectorLocals
{
	/* All the sector use this one frame */
	RwFrame *pParentFrame;
};

typedef struct sNtlPluginParam sNtlPluginParam;
struct sNtlPluginParam
{
	RpWorldSector *pWorldSector;
};

typedef struct sNtlWorldCollisionInfo sNtlWorldCollisionInfo;
struct sNtlWorldCollisionInfo
{
	RwV3d RayOri;
	RwV3d RayDir;
	RwV3d RayIntersectionPt4Terrain;
	RwBool IsCollidedAtSectors;
	RwBool FlgStopColliding;
	RwReal fDist;
};

struct sFIELD_TEXTURE
{
	RwChar	StrName[rwTEXTUREBASENAMELENGTH];
	RwReal	AngTexMat;
	RwUInt8 AngTexAdd;
};

struct sCUR_FIELD_TEX_INFO
{
	RwInt32			IdxCurLT;
	sFIELD_TEXTURE	BLT;
	sFIELD_TEXTURE	DLT[dNTL_SPLAT_LAYER_NUM];
};

struct sNtlMapObjCollisionParam
{
	RpAtomic	*pCurAtomic;
	RwReal		DistFromCam;
	RwV3d		PtIntersection;
	RwBool		IsCollided;
	RwV3d		vNormal;
	void*		pUser;
};

/* global world info. */
extern sNtlWorldParam g_sNtlWorldParam;
#define dGET_WORLD_PARAM() (&g_sNtlWorldParam)

// Field Properties
#pragma pack(1)

struct sNTL_DOODADS_PROP_PARAM
{
	RwChar		_Name[128];
	RwBool		_MilePostEnable;
	RwUInt32	_MilePostID;
	RwUInt32	SerialID;
	RwV3d		AABB[2];
	RwReal		VisibilityDist;
	RwBool		EnableTrigger;
	RwBool		EnableSelection;
	RwBool		EnableAlterable;
	RwBool		EnablePortal;
	RwBool		EnablePECollision;
	RwBool		EnableShadowProp;
	DWORD		NameIdx;
};

struct sNTL_PLANET
{
	RwV3d	_RelativePos;
	RwReal	_Scale;
	RwChar	_TexName[10];
	RwBool	_LensFlareExistence;

	sNTL_PLANET::sNTL_PLANET()
	{
		_RelativePos.x = 0.0f;
		_RelativePos.y = 200.0f;
		_RelativePos.z = 300.0f;
		_Scale = 100.0f; // dNTLPL_PLANET_DEF_SCL
		strcpy_s(_TexName, 10, "NTL");
		_LensFlareExistence = FALSE;
	}

	RwBool IsEmpty()
	{
		if(!strcmp(_TexName, "NTL"))
		{
			return TRUE;
		}

		return FALSE;
	}

	RwBool Compare(sNTL_PLANET& _NtlPlanet)
	{
		if(	static_cast<RwInt32>(_RelativePos.x) == static_cast<RwInt32>(_NtlPlanet._RelativePos.x) &&
			static_cast<RwInt32>(_RelativePos.y) == static_cast<RwInt32>(_NtlPlanet._RelativePos.y) &&
			static_cast<RwInt32>(_RelativePos.z) == static_cast<RwInt32>(_NtlPlanet._RelativePos.z))
		{
			if(static_cast<RwInt32>(_Scale) == static_cast<RwInt32>(_NtlPlanet._Scale))
			{
				if(!strcmp(_TexName, _NtlPlanet._TexName))
				{
					if(_LensFlareExistence == _NtlPlanet._LensFlareExistence)
					{
						return TRUE;
					}
				}
			}
		}

		return FALSE;
	}
};

struct sNTL_HEATHAZE
{
	RwReal  _MetersPerSec;
	RwReal	_Extent;
	RwReal	_Extent_Rand;
	RwReal	_Scale;
	RwBool	_HeatHazeExistence;

	sNTL_HEATHAZE::sNTL_HEATHAZE()
	{
		_MetersPerSec		= 0.0f;
		_Extent				= 0.0f;
		_Extent_Rand		= 0.0f;
		_Scale				= 0.0f;
		_HeatHazeExistence	= FALSE;
	}

	RwBool Compare(const sNTL_HEATHAZE& _NtlHeatHaze)
	{
		if (_HeatHazeExistence == _NtlHeatHaze._HeatHazeExistence)
		{
			if (static_cast<RwInt32>(_MetersPerSec * 10000.0f) == static_cast<RwInt32>(_NtlHeatHaze._MetersPerSec * 10000.0f) &&
				static_cast<RwInt32>(_Extent* 10000.0f) == static_cast<RwInt32>(_NtlHeatHaze._Extent * 10000.0f) &&
				static_cast<RwInt32>(_Extent_Rand * 10000.0f) == static_cast<RwInt32>(_NtlHeatHaze._Extent_Rand * 10000.0f) &&
				static_cast<RwInt32>(_Scale * 10000.0f) == static_cast<RwInt32>(_NtlHeatHaze._Scale * 10000.0f))
			{
				return TRUE;
			}
		}

		return FALSE;
	}
};

struct sNTL_SPECULAR
{
	RwV3d	_Direction;
	RwV2d	_Scale;
	RwUInt8	_Power;
	RwBool	_SpecularExistence;

	sNTL_SPECULAR::sNTL_SPECULAR()
	{
		_Direction.x		= 0.0f;
		_Direction.y		= 500.0;
		_Direction.z		= 0.0f;
		_Scale.x			= 100.0f;
		_Scale.y			= 100.0f;
		_Power				= 255;
		_SpecularExistence	= FALSE;
	}

	RwBool Compare(const sNTL_SPECULAR& _NtlSpecular)
	{
		if (_SpecularExistence == _NtlSpecular._SpecularExistence)
		{
			if (static_cast<RwInt32>(_Direction.x) == static_cast<RwInt32>(_NtlSpecular._Direction.x) &&
				static_cast<RwInt32>(_Direction.y) == static_cast<RwInt32>(_NtlSpecular._Direction.y) &&
				static_cast<RwInt32>(_Direction.z) == static_cast<RwInt32>(_NtlSpecular._Direction.z))
			{
				if (static_cast<RwInt32>(_Scale.x) == static_cast<RwInt32>(_NtlSpecular._Scale.x) &&	
					static_cast<RwInt32>(_Scale.y) == static_cast<RwInt32>(_NtlSpecular._Scale.y))
				{
					if (_Power == _NtlSpecular._Power)
					{
						return TRUE;
					}
				}
			}
		}

		return FALSE;
	}
};

struct sNTL_BLOCK_PROP
{
	sNTL_BLOCK_PROP::sNTL_BLOCK_PROP()
	{
		_GameProp = 0x00000000;

		_BBox.inf.x = RwRealMAXVAL;
		_BBox.inf.y = RwRealMAXVAL;
		_BBox.inf.z = RwRealMAXVAL;
		_BBox.sup.x = RwRealMINVAL;
		_BBox.sup.y = RwRealMINVAL;
		_BBox.sup.z = RwRealMINVAL;
	}

	sNTL_BLOCK_PROP::~sNTL_BLOCK_PROP()
	{
	}

	inline void operator = (sNTL_BLOCK_PROP& Prop)
	{
		_GameProp	= Prop._GameProp;
		_BBox		= Prop._BBox;
	}

	RwUInt32	_GameProp;
	RwBBox		_BBox;
};

struct sNTL_FIELD_PROP
{
	sNTL_FIELD_PROP::sNTL_FIELD_PROP(RwBool bTileDataAlloc = TRUE)
	{
		// Name
		::ZeroMemory(_Name, 64);
		strcpy_s(_Name, "null");

		// Normal Property
		if (bTileDataAlloc && GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
		{
			_pBitProp = NTL_NEW DWORD [(dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2)];
			::ZeroMemory(_pBitProp, sizeof(DWORD) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2));
		}
		else
		{
			_pBitProp = NULL;
		}

		// Fog
		_FogColor.red	= 255;
		_FogColor.green	= 255;
		_FogColor.blue	= 255;
		_FogColor.alpha	= 255;
		_FogCamPlane[0]	= 10.0f; 
		_FogCamPlane[1]	= 456.0f;

		// Sky
		_BaseSkyMode = 1;//eBSM_RGB_MODE; in sky

		_RGBSkyColor[0].red		= 77;
		_RGBSkyColor[0].green	= 14;
		_RGBSkyColor[0].blue	= 149;
		_RGBSkyColor[1].red		= 0;
		_RGBSkyColor[1].green	= 162;
		_RGBSkyColor[1].blue	= 255;
		_RGBSkyColor[2].red		= 242;
		_RGBSkyColor[2].green	= 244;
		_RGBSkyColor[2].blue	= 190;

		_NewSkyValue = 0.0f;

		strcpy_s(_BaseSkyTexName, "null");
		_BaseSkySpeed	= 0.0f;
		strcpy_s(_BlendedTexName[0], "null");
		strcpy_s(_BlendedTexName[1], "null");
		_BlendedTexSpeed[0]	= 0.0f;
		_BlendedTexSpeed[1]	= 0.0f;

		// Real light
		RwReal fColor = 0.0f;
		if (bTileDataAlloc && GetSceneManager()->GetActiveWorldType() == AW_RWWORLD)
		{
			fColor = 100.0f / 255.0f;
		}
		else
		{
			fColor = 1.0f;
		}


		_ClrLightAmbient.red		= fColor;
		_ClrLightAmbient.green		= fColor;
		_ClrLightAmbient.blue		= fColor;
		_ClrLightAmbient.alpha		= fColor;

		for (int i = 0; i < dNTL_PL_DIRECTIONAL_LIGHT_NUM; ++i)
		{
			_ClrLightDirectional[i].red		= (i == 0 ? fColor : 0.0f);
			_ClrLightDirectional[i].green	= (i == 0 ? fColor : 0.0f);
			_ClrLightDirectional[i].blue	= (i == 0 ? fColor : 0.0f);
			_ClrLightDirectional[i].alpha	= (i == 0 ? fColor : 0.0f);
			_AngLightDirectional[i].x		= 135.0f;	
			_AngLightDirectional[i].y		= 45.0f;
			_AngLightDirectional[i].z		= 0.0f;
		}

		// BGM table index
		_IdxBGMTbl = -1;

		// Environment BGM table index
		_IdxEnvTbl = -1;

		// share BGM table index
		for (int i = 0; i < 5; ++i)
		{
			_IdxShareTbl[i] = -1;
		}		

		// Mono power
		_MonoPower = dMONO_POWER_DEFAULT;

		// foot effect property
		if (GetSceneManager()->GetActiveWorldType() == AW_HEGITHFIELD)
		{
			_pMaterialProp = NTL_NEW BYTE [(dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2)];
			::ZeroMemory(_pMaterialProp, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2));
		}
		else
		{
			_pMaterialProp = NULL;
		}		

		// field color
		_FieldColor.red		= 255;
		_FieldColor.green	= 255;
		_FieldColor.blue	= 255;

		// weather idx
		for (int i = 0; i < 5; ++i)
		{
			_IdxWeather[i] = -1;
		}
	}

	sNTL_FIELD_PROP::~sNTL_FIELD_PROP()
	{
		NTL_ARRAY_DELETE(_pBitProp);
		NTL_ARRAY_DELETE(_pMaterialProp);
	};

public:
	inline void operator = (sNTL_FIELD_PROP& Prop)
	{
		::strcpy_s(_Name, 64, Prop._Name);
		if (_pBitProp && Prop._pBitProp)
		{
			::CopyMemory(_pBitProp, Prop._pBitProp, sizeof(DWORD) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2));
		}
		_FogColor		= Prop._FogColor;
		_FogCamPlane[0] = Prop._FogCamPlane[0];
		_FogCamPlane[1] = Prop._FogCamPlane[1];
		_BaseSkyMode	= Prop._BaseSkyMode;
		_RGBSkyColor[0] = Prop._RGBSkyColor[0];
		_RGBSkyColor[1] = Prop._RGBSkyColor[1];
		_RGBSkyColor[2] = Prop._RGBSkyColor[2];

		_NewSkyValue = Prop._NewSkyValue;
	//	DBO_WARNING_MESSAGE("_NewSkyValue: " << _NewSkyValue);
		::strcpy_s(_BaseSkyTexName, sizeof(RwChar) * 10, Prop._BaseSkyTexName);
		_BaseSkySpeed	= Prop._BaseSkySpeed;
		::strcpy_s(_BlendedTexName[0], sizeof(RwChar) * 10, Prop._BlendedTexName[0]);
		::strcpy_s(_BlendedTexName[1], sizeof(RwChar) * 10, Prop._BlendedTexName[1]);
		_BlendedTexSpeed[0]	= Prop._BlendedTexSpeed[0];
		_BlendedTexSpeed[1]	= Prop._BlendedTexSpeed[1];

		// Real light
		::CopyMemory(&_ClrLightAmbient, &Prop._ClrLightAmbient, sizeof(RwRGBAReal));
		::CopyMemory(&_ClrLightDirectional[0], &Prop._ClrLightDirectional[0], sizeof(RwRGBAReal));
		::CopyMemory(&_AngLightDirectional[0], &Prop._AngLightDirectional[0], sizeof(RwV3d));
		::CopyMemory(&_ClrLightDirectional[1], &Prop._ClrLightDirectional[1], sizeof(RwRGBAReal));
		::CopyMemory(&_AngLightDirectional[1], &Prop._AngLightDirectional[1], sizeof(RwV3d));
		::CopyMemory(&_ClrLightDirectional[2], &Prop._ClrLightDirectional[2], sizeof(RwRGBAReal));
		::CopyMemory(&_AngLightDirectional[2], &Prop._AngLightDirectional[2], sizeof(RwV3d));

		// bgm
		_IdxBGMTbl = Prop._IdxBGMTbl;

		// env
		_IdxEnvTbl = Prop._IdxEnvTbl;

		// share
		for (int i = 0; i < 5; ++i)
		{
			_IdxShareTbl[i] = Prop._IdxShareTbl[i];
		}

		// Mono power
		_MonoPower = Prop._MonoPower;

		// planet
		_NtlPlanet[0] =  Prop._NtlPlanet[0];
		_NtlPlanet[1] =  Prop._NtlPlanet[1];
		_NtlPlanet[2] =  Prop._NtlPlanet[2];

		// specular
		_NtlSpecular = Prop._NtlSpecular;

		_NtlHeatHaze = Prop._NtlHeatHaze;

		// material prop
		if (_pMaterialProp && Prop._pMaterialProp)
		{
			::CopyMemory(_pMaterialProp, Prop._pMaterialProp, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2));
		}

		// filed color
		_FieldColor = Prop._FieldColor;

		// waeher idx
		for (int i = 0; i < 5; ++i)
		{
			_IdxWeather[i] = Prop._IdxWeather[i];
		}
	}

	// Name
	RwChar	_Name[64];

	// Normal Property
	DWORD* _pBitProp;

	// Fog
	RwRGBA	_FogColor;
	RwReal	_FogCamPlane[2];

	// Sky
	RwInt32	_BaseSkyMode;
	RwRGBA	_RGBSkyColor[3];
	RwReal	_NewSkyValue; //new in 1.4 to do by daneos
	RwChar	_BaseSkyTexName[10];
	RwReal	_BaseSkySpeed;
	RwChar	_BlendedTexName[2][10];
	RwReal	_BlendedTexSpeed[2];

	// Real light
	RwRGBAReal	_ClrLightAmbient;
	RwRGBAReal	_ClrLightDirectional[dNTL_PL_DIRECTIONAL_LIGHT_NUM];
	RwV3d		_AngLightDirectional[dNTL_PL_DIRECTIONAL_LIGHT_NUM];

	// BGM table index(-1 not for playing)
	RwInt32	_IdxBGMTbl;

	// Env BGM table index(-1 not for playing)
	RwInt32	_IdxEnvTbl;

	// Share BGM table index(-1 not for playing)
	RwInt32 _IdxShareTbl[5];

	// Mono power
	RwReal _MonoPower;

	// planet
	sNTL_PLANET _NtlPlanet[3]; // dNTLPL_PLANET_MAX_CNT

	// specualr
	sNTL_SPECULAR _NtlSpecular;

	// heathaze
	sNTL_HEATHAZE _NtlHeatHaze;

	// foot effect property
	BYTE* _pMaterialProp;

	// field color
	RwRGBA _FieldColor;

	// weather idx
	RwInt32 _IdxWeather[5];
};

#pragma pack()
////////////////////////////////////////////////////////////////////////////////////////////////////
struct sRECT 
{
	RwV3d SPos;
	RwV3d EPos;
};

struct sLINE3D
{
	D3DXVECTOR3     Pos;
	D3DCOLOR        Col;
};

#define D3DFVF_LINE3D (D3DFVF_XYZ | D3DFVF_DIFFUSE)


////////////////////////////////////////////////////////////////////////////////////////////////////

/******************************************************************************************************************************
* global variables
******************************************************************************************************************************/
extern RwInt32			NtlWorldGlobalOffset;
extern RwInt32			NtlWorldLocalOffset;
extern RwInt32			NtlWorldSectorLocalOffset;
extern RwTexCoords*		NtlCommonTexCoord;
extern RxPipeline*		D3D9NtlWorldSectorAtomicPipeline;
extern RxPipeline*		D3D9NtlWorldWaterSectorAtomicPipeline;
extern RxPipelineNode*	g_pNodeWorld;
extern RxPipelineNode*	g_pNodeWorldWater;

class gbDxGenLODIdx;
extern gbDxGenLODIdx *g_pLODIdx;
#define dGET_SECTOR_LOD() (g_pLODIdx)

extern CNtlWorldMouse g_Mouse;
#define dGET_MOUSE() (&g_Mouse)

extern sNtlWorldCollisionInfo g_NtlWorldCollisionInfo;
#define dGET_COLLISION_INFO() (&g_NtlWorldCollisionInfo)

extern CNtlPLEntity* g_pBrushMapObjEntity;
#define dGET_BRUSH_ENTITY() (g_pBrushMapObjEntity)

#define dNMAP_TRANSPARENCY_TILE_FLAG	(0x00004000)
#define dNMAP_MINI_INDOOR_TILE_FLAG		(0x00002000)

extern RwBool g_bCollsionInfoUpdate;
#define dGET_COLLISION_INFO_UPDATE() (g_bCollsionInfoUpdate)

// class CNtlWorldResourceAccessor
// {
// public:
// 	virtual RwBool LoadFromFile(FILE* pFile, FILE* pFileSwap, sNtlWorldParam* pNtlWorldParam, void* pUserData) = 0;
// 	virtual RwBool SaveIntoFile(FILE* pFile, FILE* pFileSwap, sNtlWorldParam* pNtlWorldParam, void* pUserData) = 0;
// 	virtual RwBool SkipToFile(FILE* pFile, FILE* pFileSwap, sNtlWorldParam* pNtlWorldParam, void* pUserData) = 0;
// };

#endif /* ntlworldcommon_h */