#pragma once


#include "ntlworldcommon.h"
#include "OccluderProxy.h"
#include <vector>

using std::vector;


struct sSECTOR_LOD_ATTR
{
	int MipMapLevel;

	// to verify null neibors
	int LSectionLevel;
	int RSectionLevel;
	int TSectionLevel;
	int BSectionLevel;
};

// Field Properties
#pragma pack(1)

struct sSECTOR_WATER_ATTR
{
public:
	sSECTOR_WATER_ATTR::sSECTOR_WATER_ATTR()
	{
		_pAtom		= NULL;
		_pDepthMap	= NULL;
	}

	sSECTOR_WATER_ATTR::~sSECTOR_WATER_ATTR()
	{
		if(_pAtom)
		{
			RpAtomicSetFrame(_pAtom, NULL);
			RpAtomicDestroy(_pAtom);
			_pAtom = NULL;
		}

		if(_pDepthMap)
		{
			RwTextureDestroy(_pDepthMap);
		}
	}

public:
	inline void operator = (sSECTOR_WATER_ATTR& Attr)
	{
		_Height						= Attr._Height;			
		_RGBA						= Attr._RGBA;							
		_RenderstateSrc				= Attr._RenderstateSrc;
		_RenderstateDst				= Attr._RenderstateDst;				
		_RotTrans					= Attr._RotTrans;				
		_SclTrans					= Attr._SclTrans;					
		_DepthMapSize				= Attr._DepthMapSize;				
		_DepthMapHeightVariation[0]	= Attr._DepthMapHeightVariation[0];	
		_DepthMapHeightVariation[1]	= Attr._DepthMapHeightVariation[1];	
		_IdxSequence				= Attr._IdxSequence;		

		// texture matrix
		float			UVWrap = _SclTrans;
		D3DXMATRIXA16	Temp;

		D3DXMatrixIdentity(&_matTex[0]);
		D3DXMatrixIdentity(&_matTex[1]);
		_matTex[1]._11 *= UVWrap;
		_matTex[1]._22 *= UVWrap;

		float Error = 0.5f * UVWrap;
		D3DXMatrixTranslation(&Temp, -Error, -Error, 0);
		D3DXMatrixMultiply(&_matTex[1], &_matTex[1], &Temp);
		D3DXMatrixRotationZ(&Temp, _RotTrans);
		D3DXMatrixMultiply(&_matTex[1], &_matTex[1], &Temp);
		D3DXMatrixTranslation(&Temp, Error, Error, 0);
		D3DXMatrixMultiply(&_matTex[1], &_matTex[1], &Temp);

		_matTex[1].m[2][0] = _matTex[1].m[3][0];
		_matTex[1].m[2][1] = _matTex[1].m[3][1];
		_matTex[1].m[3][0] = 0.0f;
		_matTex[1].m[3][1] = 0.0f;
	}

public:
	RwReal			_Height;
	RwRGBA			_RGBA;
	DWORD			_RenderstateSrc;
	DWORD			_RenderstateDst;
	RwReal			_RotTrans;
	RwReal			_SclTrans;
	RwInt32			_DepthMapSize;
	RwReal			_DepthMapHeightVariation[2];
	RwInt32			_IdxSequence;
	RpAtomic*		_pAtom;
	RwTexture*		_pDepthMap;

	// 0 : alpha map matrix, 1 : diffuse map matrix
	D3DXMATRIXA16	_matTex[2];
};

struct sSECTOR_PLANT_SET_ATTR
{
	struct sSECTOR_PLANT_OBJ_ATTR
	{
		typedef std::vector<RwMatrix>		TRANSFORM_VEC;
		typedef std::vector<TRANSFORM_VEC*>	TRANSFORM_OBJ_VEC;

		RwInt32				iTileIdx;
		TRANSFORM_OBJ_VEC	vecObjMatrix;

		~sSECTOR_PLANT_OBJ_ATTR()
		{
			for (int i = 0; i < (int)vecObjMatrix.size(); ++i)
			{
				TRANSFORM_VEC* pTransform = vecObjMatrix.at(i);
				NTL_DELETE(pTransform);
			}
			vecObjMatrix.clear();
		}
	};
	typedef std::vector<sSECTOR_PLANT_OBJ_ATTR*> PLANT_OBJ_VEC;
	typedef PLANT_OBJ_VEC::iterator				 PLANT_OBJ_VEC_IT;

	RwInt32			iSetIdx;
	PLANT_OBJ_VEC	vecPlantObj;

	~sSECTOR_PLANT_SET_ATTR()
	{
		for (int i = 0; i < (int)vecPlantObj.size(); ++i)
		{
			sSECTOR_PLANT_OBJ_ATTR* pPlantObj = vecPlantObj.at(i);
			NTL_DELETE(pPlantObj);
		}
		vecPlantObj.clear();
	}
};

#pragma pack()

class CNtlPLEntity;
class CNtlWorldShadow;
class CNtlWorldSectorPVS;

struct sNTL_WORLD_SL;

class CNtlWorldSector : public COccluderProxy
{
public:
	CNtlWorldSector(void);
	virtual ~CNtlWorldSector(void);
	void Init();
	void Free();

private:
	void UpdateShadow();

public:
	// Loaded Sector
	RwBool m_bLoaded;

	// Sector Cull
	RwBool m_IsCulled;

	// CullCheck & FullFoged
	RwBool m_bFullFogged;
	RwBool m_bCullCheck;

	// Occlusion
	//RwBool m_IsOccluded; // -> COccluderManager 에게 Query 한다.

	// Tile transparency
	RwBool m_IsTileTransparency;

	// LOD var.
	D3DXVECTOR3			DatumPoint;
	sSECTOR_LOD_ATTR	m_LODAttr;

	// world sector
	RpWorldSector *m_pWorldSector;

	// sector geometry;
	RpAtomic *m_pAtomic;

	sSECTOR_WATER_ATTR* m_pWater;

	// plant set
	vector<sSECTOR_PLANT_SET_ATTR*> m_vecPlantSet;

	// map objects
	vector<CNtlPLEntity* > m_vecNtlPLEntityOrnament;

	// effect objects
	vector<CNtlPLEntity* > m_vecNtlPLEntityEffect;

	// sound entities
	vector<CNtlPLEntity* > m_vecNtlPLEntitySound;

	// sound entities bgm
	vector<CNtlPLEntity* > m_vecNtlPLEntitySoundBGM;

	// CZ-SS
	// soft spot entities	
	//vector<CNtlPLEntity* > m_vecNtlPLEntitySS;

	// shoreline entities
	CNtlPLEntity* m_pNtlPLEntityShoreLine;

	// decal entitis
	vector<CNtlPLEntity* > m_vecNtlPLEntityDecal;

	// world light
	vector<CNtlPLEntity* > m_vecNtlPLEntityWorldLight_Sphere;
	vector<CNtlPLEntity* > m_vecNtlPLEntityWorldLight_Box;

	// occluder
	vector<CNtlPLEntity* > m_vecNtlPLEntityOccluder_Quad;

	// heathaze
	vector<CNtlPLEntity*>  m_vecNtlPLEntityHeatHaze;

	// indoor light
	vector<CNtlPLEntity*>	m_vecNtlPLEntityLightObject;
	vector<CNtlPLEntity*>	m_vecNtlPLEntityLightObjectDummy;

	// game property
	vector<CNtlPLEntity*>	m_vecNtlPLEntityGameProperty;

	// decal volume
	vector<CNtlPLEntity*>	m_vecNtlPLEntityDecalVolume;

	// dojo
	vector<CNtlPLEntity*>	m_vecNtlPLEntityDojo;

	// material extension members
	CNtlWorldShadow* m_pNtlWorldShadow;

	// PVS
	CNtlWorldSectorPVS* m_pNtlWorldSectorPVS;

	// terrain textures
	RwTexture *m_pTextureBase;
#ifdef dNTL_WORLD_COMBINE
	RwTexture *m_pTextureCombine;
#endif
	RwTexture *m_pTextureSplat[dNTL_SPLAT_LAYER_NUM];
	RwTexture *m_pTextureAlpha[dNTL_SPLAT_LAYER_NUM];

	// base layer prop.
	D3DXMATRIX	m_TextureMatBase[2];
#ifdef dNTL_WORLD_COMBINE
	D3DXMATRIX	m_TextureMatCombine;
#endif
	RwReal		m_TextureMatAngBase;
	RwUInt8		m_TextureMatAddBase;

	// detail layer prop.
	D3DXMATRIX	m_TextureMat[dNTL_SPLAT_LAYER_NUM][2];
	RwReal		m_TextureMatAng[dNTL_SPLAT_LAYER_NUM];
	RwUInt8		m_TextureMatAdd[dNTL_SPLAT_LAYER_NUM];

#ifdef dNTL_WORLD_TOOL_MODE
	
		DWORD*			m_pClrDiffusePalette;
		sNTL_WORLD_SL*	m_pNtlWorldSL;
#endif 
		RwReal*			m_pNtlWorldPathEngineData;

public:
	// Interlocking LOD
	const LPDIRECT3DINDEXBUFFER9	GetBodyIndexBuff(gbDxGenLODIdx *pLODIdx);
	const LPDIRECT3DINDEXBUFFER9	GetLinkIndexBuff(gbDxGenLODIdx *pLODIdx, int Dir);
	const WORD*						GetBodySystemIndexBuff(gbDxGenLODIdx *pLODIdx);
	const WORD*						GetLinkSystemIndexBuff(gbDxGenLODIdx *pLODIdx, int Dir);
	const int						GetBodyPrimitiveNum(gbDxGenLODIdx *pLODIdx);
	const int						GetLinkPrimitiveNum(gbDxGenLODIdx *pLODIdx, int Dir);

	void DeleteEntity(CNtlPLEntity* pNtlPLEntity, RwInt32 MenuIdx = 0);
	void DeleteShadow();
	void MaterialDestructor(RwBool SaveSwapInToolMode = TRUE);
	void MaterialContructor();
	void MaterialCopier(void *dstObject, const void *srcObject);
	void InsertIn2World(CNtlPLEntity *pNtlPLEntity, RwInt32 IdxMenu = 0);
	void Update();

	VOID CreateSectorPVS(RwInt32 _SelfSectorIdx, RwReal fEpsilsonHeight = 10.0f/*CharHeight*/);
	VOID DestroySectorPVS(RwInt32 _SelfSectorIdx);

#ifdef dNTL_WORLD_TOOL_MODE
	
	void SaveSwapFile();
	void SetTransparentTile(RwBool _IsTileTransparency, RwV3d& Pos);
	
#endif
};
