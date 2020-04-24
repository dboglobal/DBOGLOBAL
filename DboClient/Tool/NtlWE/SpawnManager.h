#pragma once


#include "SpawnMacroManager.h"
#include "ceventhandler.h"
#include "Spawn.h"
#include "NtlXMLDoc.h"

using std::map;
#include <map>


class CSpawnContainer;
class CNtlWorldBrush;
class CSpawnInstNode;
class CSpawnTreeElement;
class CSpawnManager;
class CPathHandler;


enum eSPAWN_EDIT
{
	eSPAWN_IDLE = 0,
};

struct sIS_CLUMP_NEST
{
	CNtlPLEntity* _pNtlPLEntity;
};


class CSpawnXML : public CNtlXMLDoc
{
public:
	CSpawnXML(void) {;}
	virtual ~CSpawnXML(void) {;}

private:
	IXMLDOMElement* m_pRootElem;
	IXMLDOMElement* m_pListElem;

private:
	VOID CreateSpawnNode(CSpawn* pSpawn, CSpawnManager* _pMgr, DWORD _CurTblIdx);

public:
	VOID CreateSpawnTbl(string& _PathName, CSpawnManager* _pMgr);
	VOID DestroySpawnTbl();
};

class CSpawnManager : public RWS::CEventHandler, public CSpawnMacroManager
{
public:
	static CSpawnManager* g_pSpawnManager;

public:
	CSpawnManager(void);
	virtual ~CSpawnManager(void);

private:
	// Visible Data
	RwBool					m_bVisibleCircle;
	RwBool					m_bVisibleCircleScan;

	// Field area
	RwInt32					m_Fields6x6[36];

	// Loaded spawns of the current fields; entity memory
	vector<CSpawn*>			m_vecSpawnLoad;

	// Selected spawns
	vector<CSpawn*>			m_vecSpawnSelect;

	// multiselect brushes
	CNtlWorldBrush*			m_pBrushMultiSelector;
	CNtlWorldBrush*			m_pBrushMultiSelectorFrustum;
	CNtlWorldBrush*			m_pBrushMultiSelection;
	CNtlWorldBrush*			m_pBrushNestCircle;

	RwBool					m_PathModeActive;
	RwBool					m_NavigatorModeActive;
	eSPAWN_TYPE				m_CurSpawnType;
	CSpawnContainer*		m_pSpawnContainer;

	// spawn path handler
	CPathHandler*			m_pPathHandler;

public:
	// Nest indicator
	CNtlPLEntity*			m_pNtlPLEntity;

	// Nest Clump comparing structure
	sIS_CLUMP_NEST			m_sIsClumpNest;

	RwBool					m_bMouseMoveSnap;

private:
	static RpAtomic*	WorldIntersectionSpawnCB(RpIntersection* pIntersection, RpWorldSector* pWorldSector, RpAtomic* pAtomic, RwReal fDistance, void* pData);
	static RpClump*		NestUIDrawAllClumpCB(RpClump* _pClump, void* _pData);
	static RpAtomic*	NestUIDrawAllAtomicCB(RpAtomic* _pAtomic, void* _pData);
	static RpAtomic*	NestUIDrawAllAtomicCB2(RpAtomic* _pAtomic, void* _pData);

	RwBool				GetSpawnCapture(RwV3d& _PosObjMid);
	RwBool				GetSpawnCaptured(CSpawn* _pSpawn);

	VOID				InsertSpawnLoadAtField(RwInt32 FieldIdx, vector<CSpawn*>& _vecSpawnLoad);
	VOID				DeleteSpawnLoadAtField(RwInt32 FieldIdx, vector<CSpawn*>& _vecSpawnLoad);
	VOID				InsertSpawnLoad(CSpawn* _pSpawn);
	VOID				DeleteSpawnLoad(CSpawn* _pSpawn);
	VOID				PopSpawnCaptured(CSpawn* _pSpawn);
	VOID				PushSpawnSelected(CSpawn* _pSpawn);
	VOID				OnCreateParty();
	VOID				OnCreatePartyLeader();
	VOID				OnCreatePartyFollow();
	VOID				DestroySelectedSpawn();
	VOID				MoveSpawn();
	VOID				RotateSpawn();

	RwV3d				GetSpawnIntersectionPos();

public:
	virtual void		 HandleEvents(RWS::CMsg &pMsg);

	RwInt32				GetSpawnCnt(RwInt32 _FieldIdx);
	eSPAWN_TYPE&		GetSpawnType() { return m_CurSpawnType; }
	CSpawnContainer*	GetSpawnContainer() const { return m_pSpawnContainer; } 
	RwBool				GetMdlNameOfTreeItem(RwChar* pName, BYTE Lvl, string& _MdlName, RwUInt32 _MdlGUID);
	RwUInt32			GetMdlCodeOfTreeItem(RwChar* pName);
	CSpawnTreeElement*	GetTreeElement(BYTE Lvl, string& _MdlName, string& _ObjName);
	RwInt32				GetCurNestMemberCnt();
	CPathHandler*		GetPathHandler() { return m_pPathHandler; }
	const vector<CSpawn*>* 	GetSpawnSelected() {return &m_vecSpawnSelect; }
	RwBool				GetOneSpawnSelected() { if(m_vecSpawnSelect.size() == 1) return TRUE; else return FALSE; }
	RwBool				GetSpawnSelectedAtLeastOnce() { if(m_vecSpawnSelect.size() > 1) return TRUE; else return FALSE; }
	RwInt32				GetPathIndex();
	VOID				SetNestMemberCnt(RwInt32 _Cnt);
	VOID				SetPathIndex(RwInt32 _PathIndex);
	VOID				SetPathModeActive(RwBool _Flag = TRUE);
	VOID				SetNavigatorModeActive(RwBool _Flag = TRUE);
	RwBool				SetSpawnMacroToCurSpawns(CSpawnMacro& _SpawnMacro);
	VOID				SetGroupIdx4SpawnSelected(DWORD _GroupIdx);
	VOID				SetPlayScriptIdx4SpawnSelected(DWORD _PlayScriptIdx);
	VOID				SetPlayScriptSceneIdx4SpawnSelected(DWORD _PlayScriptSceneIdx);
	VOID				SetAIScriptIdx4SpawnSelected(DWORD _AIScriptIdx);
	VOID				SetAIScriptSceneIdx4SpawnSelected(DWORD _AIScriptSceneIdx);

	void				SetObjectSnap(RwBool bObjectSnap) { m_bMouseMoveSnap = bObjectSnap; }

	VOID				ChangeSpawnModel(CSpawn* pSpawn, DWORD dwMdlGUID);
	VOID				ChangeSpawnNestCount(CSpawn* pSpawn, RwInt32 iNestCount);
	VOID				ChangeSpawnCoolTime(CSpawn* pSpawn, RwInt32 iCoolTime);
	VOID				ChangeSpawnMoveType(CSpawn* pSpawn, RwInt32 iMoveType);
	VOID				ChangeSpawnWanderRange(CSpawn* pSpawn, RwReal fWanderRange);
	VOID				ChangeSpawnMoveRange(CSpawn* pSpawn, RwReal fMoveRange);
	VOID				ChangeSpawnDelayTime(CSpawn* pSpawn, RwReal fDelayTime);
	VOID				ChangeSpawnPathIndex(CSpawn* pSpawn, RwInt32 iPathIndex);
	VOID				ChangeSpawnGroupIndex(CSpawn* pSpawn, DWORD dwGroupIndex);
	VOID				ChangeSpawnPlayScriptIndex(CSpawn* pSpawn, DWORD dwPlayScriptIndex);
	VOID				ChangeSpawnPlayScriptSceneIndex(CSpawn* pSpawn, DWORD dwPlayScriptSceneIndex);
	VOID				ChangeSpawnAIScriptIndex(CSpawn* pSpawn, DWORD dwAIScriptIndex);
	VOID				ChangeSpawnAIScriptSceneIndex(CSpawn* pSpawn, DWORD dwAIScriptSceneIndex);

	VOID				OnPortalTo(RwV3d& _Pos);
	VOID				PopAllSpawnCaptured();
	VOID				CreateEntity();
	VOID				DeleteEntity();
	VOID				Create(eSPAWN_TYPE _eSpawnType);
	VOID				CreateEntitiesAtCurFields();
	VOID				Destroy();
	VOID				DestroyEntitiesAtCurFields();
	VOID				OnDeleteSpawn(RwV3d& _Pos);
	VOID				Update();
	VOID				Render();
	VOID				RenderNestCircle(RwV3d& Pos, RwInt32 Radius, D3DCOLOR dwClr = 0xFFFFFFFF);
	VOID				RenderNestLookatLine(RwV3d& _NestPos, RwV3d& _NestRot, RwReal NestScl, D3DCOLOR Color, CNtlWorldBrush* _pBrushLine);
	VOID				RenderText(RtCharset* _pCharset, RwInt32 LenWindowWidth, RwInt32 LenWindowHeight);

	RwBool				RenderMMProp(DWORD _MMProp, RwInt32 _FieldIdx);
	RwBool				RenderMMPropMob(RwInt32 _FieldIdx);
	RwBool				RenderMMPropNpc(RwInt32 _FieldIdx);
	RwBool				RenderMMPropTrigger(RwInt32 _FieldIdx);


	VOID				Save();
	VOID				Load();
	VOID				Export();
	VOID				SaveIntoFile(const char* pFilemame);
	VOID				LoadFromFile(const char* pFilemame);

	RwBool				IsVisibleCircle();
	RwBool				IsVisibleCircleScan();

	void				SetVisibleCircle(RwBool bVisible);
	void				SetVisibleCircleScan(RwBool bVisible);
};

// Spawn instance func.
CSpawnManager*	GetSpawnInstance(VOID);
VOID			DestroySpawnInstance(VOID);