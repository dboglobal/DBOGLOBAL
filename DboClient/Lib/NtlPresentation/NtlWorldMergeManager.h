#pragma once


#include "ntlworldcommon.h"
#include "FileController.h"
#include "FileMem.h"
#include "NtlWorldFileMemAccessor.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

#ifdef dNTL_WORLD_TOOL_MODE

class CNtlWorldFieldManager;

struct sMERGE_PARAM
{
	//////////////////////////////////////////////////////////////////////////
	// Information
	//////////////////////////////////////////////////////////////////////////
	RwBool	bMaterial;
	RwBool	bHeightField;
	RwBool	bDiffuse;
	RwBool	bDoodads;
	RwBool	bWater;
	RwBool	bShadow;
	RwBool	bEffects;
	RwBool	bSE;
	// CZ-SS
	//RwBool	bSS;
	RwBool	bSectorCull;	
	RwBool	bTileTransparency;
	RwBool	bShoreline;
	RwBool	bDecal;
	RwBool	bPlant;
	RwBool	bWorldLight;
	RwBool	bOccluder;
	RwBool	bHeatHazeObject;
	RwBool	bLightObject;
	RwBool	bDojo;

	//////////////////////////////////////////////////////////////////////////
	// World Property
	//////////////////////////////////////////////////////////////////////////
	RwBool	bName;
	RwBool	bBitProp;
	RwBool	bFog;
	RwBool	bSky;
	RwBool	bRealLight;
	RwBool	bBGM;
	RwBool	bBloom;
	RwBool	bPlanet;
	RwBool	bSpecular;
	RwBool	bWorldMaterial;
	RwBool	bFieldColor;
	RwBool	bWeather;
	RwBool	bFieldHeatHaze;

	sMERGE_PARAM()
	{
		// Information
		bMaterial			= FALSE;
		bHeightField		= FALSE;
		bDiffuse			= FALSE;
		bDoodads			= FALSE;
		bWater				= FALSE;
		bShadow				= FALSE;
		bEffects			= FALSE;
		bSE					= FALSE;
		// CZ-SS
		//bSS					= FALSE;
		bSectorCull			= FALSE;
		bTileTransparency	= FALSE;
		bShoreline			= FALSE;
		bDecal				= FALSE;
		bPlant				= FALSE;
		bWorldLight			= FALSE;
		bOccluder			= FALSE;
		bHeatHazeObject		= FALSE;
		bLightObject		= FALSE;
		bDojo				= FALSE;

		// World Property
		bName				= FALSE;
		bBitProp			= FALSE;
		bFog				= FALSE;
		bSky				= FALSE;
		bRealLight			= FALSE;
		bBGM				= FALSE;
		bBloom				= FALSE;
		bPlanet				= FALSE;
		bSpecular			= FALSE;
		bWorldMaterial		= FALSE;
		bFieldColor			= FALSE;
		bWeather			= FALSE;
		bFieldHeatHaze		= FALSE;
	}

	RwBool IsInformationUpdate()
	{
		return (
			bMaterial ||
			bHeightField ||
			bDiffuse ||
			bDoodads ||
			bWater ||
			bShadow ||
			bEffects ||
			bSE ||
			// CZ-SS
			//bSS ||
			bSectorCull ||
			bTileTransparency ||
			bShoreline ||
			bDecal ||
			bPlant ||
			bWorldLight ||
			bOccluder ||
			bHeatHazeObject ||
			bLightObject ||
			bDojo
			);
	}

	RwBool IsWorldPropertyUpdate()
	{
		return (
			bName ||
			bBitProp ||
			bFog ||
			bSky ||
			bRealLight ||
			bBGM ||
			bBloom	||
			bPlanet ||
			bSpecular ||
			bWorldMaterial ||
			bFieldColor ||
			bWeather ||
			bFieldHeatHaze
			);
	}
	RwBool IsSwapDataUpdate()
	{
		return (			
			bDiffuse ||			
			bShadow ||
			bOccluder ||
			bHeatHazeObject ||
			bDoodads ||
			bEffects
			);
	}
	RwBool IsWorldParamUpdate()
	{
		return (
			bDoodads
			);
	}
};

class CNtlWorldMergeManager : public CFileController
{
public:
	CNtlWorldMergeManager(void);
	~CNtlWorldMergeManager(void);

private:
	sNtlWorldParam			m_NtlWorldParam2Merge;
	CNtlWorldFieldManager*	m_pFieldMgr;
	CFileMem				m_FileMemSrc;
	CFileMem				m_FileMemDst;

private:
	RwBool VerifyMergeWorld(const char* _pPathName);

public:
	VOID	SetFieldManager(CNtlWorldFieldManager* _pFieldMgr) { m_pFieldMgr = _pFieldMgr; }
	RwBool	Load(const char* _pPathName);
	RwBool	LoadSpawnDst(char* _pFileName);
	RwBool	LoadSpawnSrc(char* _pFileName);

	RwBool	MergeSpawn(RwBBox bBoxDst, RwBBox bBoxSrc);

	// this func. works with only files, not memory
	RwBool  OnHeightfieldVariation(RwInt32 IdxField);

	// for exporting
	RwBool	OnExportPathEngineOutDoor(const char* _pPathNameOfProject, RwUInt32 uiWorldID, const char* _pPathName4Exporting,
									const char* _pWorldName, const char* _pAgency, RwInt32 _iGroupFieldNum, RwInt32 _iOverlapSize);

	RwBool	OnExportPathEngineInDoor(const char* _pPathNameOfProject, RwUInt32 uiWorldID, const char* _pPathName4Exporting,
									const char* _pWorldName, const char* _pAgency);

	RwBool	OnExportTriggerInfoOutdoor(const char* _pPathNameOfProject, const char* _pPathName4Exporting);
	RwBool	OnExportTriggerInfoIndoor(const char* _pPathNameOfProject, const char* _pPathName4Exporting);

	RwBool	OnExportHeightfield(const char* _pPathNameOfProject, const char* _pPathName4Exporting);
	RwBool	OnExportHeightfieldY(const char* _pPathNameOfProject, const char* _pPathName4Exporting);
	RwBool	OnExportMapObj(const char* _pPathNameOfProject, const char* _pPathName4Exporting);
	RwBool	OnExportMapProp(const char* _pPathNameOfProject, const char* _pPathName4Exporting);
	RwBool	OnExportMapName(const char* _pPathNameOfProject, const char* _pPathName4Exporting);
	RwBool	OnExportPECollision(const char* _pPathNameOfProject, const char* _pPathName4Exporting);

	RwBool	OnExportHeightfieldPE(const char* _pPathNameOfProject, const char* _pPathName4Exporting);
	RwBool	OnExportMapPropPE(const char* _pPathNameOfProject, const char* _pPathName4Exporting);

	RwBool	OnExportInvalidSectorLink(const char* _pPathNameOfProject, const char* _pPathName4Exporting);
	RwBool	OnProofsInvalidSectorLink(RwInt32 FieldIdx);

	RwBool	OnReindexingObjectSerialID();

	// for some other world file handling
	RwBool OnObjVisibilityDistChange(RwInt32 FieldIdx, RwReal VisibilityDist, RwReal Min, RwReal Max);

	static CNtlWorldMergeManager*	GetInstance(VOID);
	const sNtlWorldParam*			GetNtlWorldParam(VOID) { return &m_NtlWorldParam2Merge; }

	RwBool	Merge(sMERGE_PARAM &sParam, RwInt32 IdxFieldSrc, RwInt32 IdxFieldDst);
	RwBool	IsFieldValid(RwInt32 IdxSField, RwInt32 IdxEField);

private:
	// New Merge
	RwBool	MergeInformation(sMERGE_PARAM &sParam, RwInt32 IdxFieldSrc, RwInt32 IdxFieldDst);
	RwBool	MergeProperty(sMERGE_PARAM &sParam, RwInt32 IdxFieldSrc, RwInt32 IdxFieldDst);
	RwBool	MergeSwapFile(sMERGE_PARAM &sParam, RwInt32 IdxFieldSrc, RwInt32 IdxFieldDst);

	void	RefreshField(RwInt32 IdxField);
};

#endif

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

#ifdef dNTL_WORLD_TOOL_MODE

class CNtlWorldFieldManager;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "SpawnMacro.h"

class CSpawnMergeContainer
{
	struct SSpawn;
	struct SParty;

	typedef std::map<DWORD, SParty*>	MAP_PARTY;
	typedef MAP_PARTY::iterator			MAP_PARTY_IT;


	typedef std::vector<SSpawn*>		VEC_SPAWN;
	typedef VEC_SPAWN::iterator			VEC_SPAWN_IT;

	struct SSpawn
	{
		DWORD					dwMdlGUID;
		RwInt32					iNestRange;
		RwInt32					iNestSpawnCnt;
		RwInt32					iNestPathIndex;

		RwInt32					iPartyIndex;
		RwBool					bPartyLeader;

		RwV3d					vFollowDist;

		CSpawnMacro				sSpawnMacro;

		RwInt32					iLenModelName;
		char					acModelName[NTL_MAX_DIR_PATH];

		RwInt32					iLenDisplayName;
		char					acDisplayName[NTL_MAX_DIR_PATH];

		RwReal					fS;
		RwV3d					vR;
		RwV3d					vT;

		DWORD					iGroupIdx;
		DWORD					iPlayScriptIdx;
		DWORD					iPlayScriptSceneIdx;

		DWORD					iAIScriptIdx;
		DWORD					iAIScriptSceneIdx;
	};

	struct SParty
	{
		DWORD					dwPartyIndex;
		RwRGBA					rgbaPartyColor;
		RwBool					bFollow;
		VEC_SPAWN				vecPartySpawn;

		DWORD					dwCurPartyIndex; // (?)
	};

	MAP_PARTY					m_mapParty;
	VEC_SPAWN					m_vecSpawn;

public:
	CSpawnMergeContainer();
	virtual ~CSpawnMergeContainer();

	RwBool		Load(char* pcFileName);
	RwBool		Save();

	void		Destroy();

	RwInt32		GetSpawnType() { return m_eSpawnType; }

	void		DeleteSpawn(RwBBox bBox);
	void		InsertSpawn(CSpawnMergeContainer* pSpawnMergeContainer, RwBBox bBoxSrc, RwBBox bBoxDst);

protected:
	void		LoadParty(FILE* pFile);
	void		LoadSpawn(FILE* pFile);
	void		SaveParty(FILE* pFile);
	void		SaveSpawn(FILE* pFile);

	void		LinkSpawnParty();
	void		OptSpawnParty();

protected:
	std::string	m_strFilename;

	RwInt32		m_eSpawnType;
	RwInt32		m_iPartyIdx;
	RwInt32		m_iPartyColorIdx;
};

struct sMERGE_PARAM
{
	//////////////////////////////////////////////////////////////////////////
	// Information
	//////////////////////////////////////////////////////////////////////////
	RwBool	bMaterial;
	RwBool	bHeightField;
	RwBool	bDiffuse;
	RwBool	bDoodads;
	RwBool	bWater;
	RwBool	bShadow;
	RwBool	bEffects;
	RwBool	bSE;
	// CZ-SS
	//RwBool	bSS;
	RwBool	bSectorCull;	
	RwBool	bTileTransparency;
	RwBool	bShoreline;
	RwBool	bDecal;
	RwBool	bPlant;
	RwBool	bWorldLight;
	RwBool	bOccluder;
	RwBool	bHeatHazeObject;
	RwBool	bLightObject;
	RwBool	bDojo;

	//////////////////////////////////////////////////////////////////////////
	// World Property
	//////////////////////////////////////////////////////////////////////////
	RwBool	bName;
	RwBool	bBitProp;
	RwBool	bFog;
	RwBool	bSky;
	RwBool	bRealLight;
	RwBool	bBGM;
	RwBool	bBloom;
	RwBool	bPlanet;
	RwBool	bSpecular;
	RwBool	bWorldMaterial;
	RwBool	bFieldColor;
	RwBool	bWeather;
	RwBool	bFieldHeatHaze;

	sMERGE_PARAM()
	{
		// Information
		bMaterial			= FALSE;
		bHeightField		= FALSE;
		bDiffuse			= FALSE;
		bDoodads			= FALSE;
		bWater				= FALSE;
		bShadow				= FALSE;
		bEffects			= FALSE;
		bSE					= FALSE;
		// CZ-SS
		//bSS					= FALSE;
		bSectorCull			= FALSE;
		bTileTransparency	= FALSE;
		bShoreline			= FALSE;
		bDecal				= FALSE;
		bPlant				= FALSE;
		bWorldLight			= FALSE;
		bOccluder			= FALSE;
		bHeatHazeObject		= FALSE;
		bLightObject		= FALSE;
		bDojo				= FALSE;

		// World Property
		bName				= FALSE;
		bBitProp			= FALSE;
		bFog				= FALSE;
		bSky				= FALSE;
		bRealLight			= FALSE;
		bBGM				= FALSE;
		bBloom				= FALSE;
		bPlanet				= FALSE;
		bSpecular			= FALSE;
		bWorldMaterial		= FALSE;
		bFieldColor			= FALSE;
		bWeather			= FALSE;
		bFieldHeatHaze		= FALSE;
	}

	RwBool IsInformationUpdate()
	{
		return (
			bMaterial ||
			bHeightField ||
			bDiffuse ||
			bDoodads ||
			bWater ||
			bShadow ||
			bEffects ||
			bSE ||
			// CZ-SS
			//bSS ||
			bSectorCull ||
			bTileTransparency ||
			bShoreline ||
			bDecal ||
			bPlant ||
			bWorldLight ||
			bOccluder ||
			bHeatHazeObject ||
			bLightObject ||
			bDojo
			);
	}

	RwBool IsWorldPropertyUpdate()
	{
		return (
			bName ||
			bBitProp ||
			bFog ||
			bSky ||
			bRealLight ||
			bBGM ||
			bBloom	||
			bPlanet ||
			bSpecular ||
			bWorldMaterial ||
			bFieldColor ||
			bWeather ||
			bFieldHeatHaze
			);
	}
	RwBool IsSwapDataUpdate()
	{
		return (			
			bDiffuse ||			
			bShadow ||
			bOccluder ||
			bHeatHazeObject ||
			bDoodads ||
			bEffects
			);
	}
	RwBool IsWorldParamUpdate()
	{
		return (
			bDoodads
			);
	}
};

class CNtlWorldMergeManager : public CFileController, public CNtlWorldFileMemAccessor
{
public:
	CNtlWorldMergeManager(void);
	~CNtlWorldMergeManager(void);

private:
	sNtlWorldParam			m_NtlWorldParam2Merge;
	CNtlWorldFieldManager*	m_pFieldMgr;
	CFileMem				m_FileMemSrc;
	CFileMem				m_FileMemDst;

	CSpawnMergeContainer	m_SpawnMergeContainerDst;
	CSpawnMergeContainer	m_SpawnMergeContainerSrc;

private:
	RwBool VerifyMergeWorld(const char* _pPathName);

public:
	VOID	SetFieldManager(CNtlWorldFieldManager* _pFieldMgr) { m_pFieldMgr = _pFieldMgr; }
	RwBool	Load(const char* _pPathName);
	RwBool	LoadSpawnDst(char* _pFileName);
	RwBool	LoadSpawnSrc(char* _pFileName);

	RwBool	MergeSpawn(RwBBox bBoxDst, RwBBox bBoxSrc);

	// this func. works with only files, not memory
	RwBool  OnHeightfieldVariation(RwInt32 IdxField);

	// for exporting
	RwBool	OnExportPathEngineOutDoor(const char* _pPathNameOfProject, RwUInt32 uiWorldID, const char* _pPathName4Exporting,
		const char* _pWorldName, const char* _pAgency, RwInt32 _iGroupFieldNum, RwInt32 _iOverlapSize);

	RwBool	OnExportPathEngineInDoor(const char* _pPathNameOfProject, RwUInt32 uiWorldID, const char* _pPathName4Exporting,
		const char* _pWorldName, const char* _pAgency);

	RwBool	OnExportTriggerInfoOutdoor(const char* _pPathNameOfProject, const char* _pPathName4Exporting);
	RwBool	OnExportTriggerInfoIndoor(const char* _pPathNameOfProject, const char* _pPathName4Exporting);

	RwBool	OnExportHeightfield(const char* _pPathNameOfProject, const char* _pPathName4Exporting);
	RwBool	OnExportHeightfieldY(const char* _pPathNameOfProject, const char* _pPathName4Exporting);
	RwBool	OnExportMapObj(const char* _pPathNameOfProject, const char* _pPathName4Exporting);
	RwBool	OnExportMapProp(const char* _pPathNameOfProject, const char* _pPathName4Exporting);
	RwBool	OnExportMapName(const char* _pPathNameOfProject, const char* _pPathName4Exporting);
	RwBool	OnExportPECollision(const char* _pPathNameOfProject, const char* _pPathName4Exporting);

	RwBool	OnExportHeightfieldPE(const char* _pPathNameOfProject, const char* _pPathName4Exporting);
	RwBool	OnExportMapPropPE(const char* _pPathNameOfProject, const char* _pPathName4Exporting);

	RwBool	OnExportInvalidSectorLink(const char* _pPathNameOfProject, const char* _pPathName4Exporting);
	RwBool	OnProofsInvalidSectorLink(RwInt32 FieldIdx);

	RwBool	OnReindexingObjectSerialID();

	// for some other world file handling
	RwBool OnObjVisibilityDistChange(RwInt32 FieldIdx, RwReal VisibilityDist, RwReal Min, RwReal Max);

	static CNtlWorldMergeManager*	GetInstance(VOID);
	const sNtlWorldParam*			GetNtlWorldParam(VOID) { return &m_NtlWorldParam2Merge; }

	RwBool	Merge(sMERGE_PARAM &sParam, RwInt32 IdxFieldSrc, RwInt32 IdxFieldDst);
	RwBool	IsFieldValid(RwInt32 IdxSField, RwInt32 IdxEField);

private:
	// New Merge
	RwBool	MergeInformation(sMERGE_PARAM &sParam, RwInt32 IdxFieldSrc, RwInt32 IdxFieldDst);
	RwBool	MergeProperty(sMERGE_PARAM &sParam, RwInt32 IdxFieldSrc, RwInt32 IdxFieldDst);
	RwBool	MergeSwapFile(sMERGE_PARAM &sParam, RwInt32 IdxFieldSrc, RwInt32 IdxFieldDst);

	void	RefreshField(RwInt32 IdxField);
};

#endif

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif
