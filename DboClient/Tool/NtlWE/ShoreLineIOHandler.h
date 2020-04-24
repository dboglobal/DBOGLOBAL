#pragma once


class CNtlPLEntity;
class CNtlWorldBrush;
class CShoreLineAGenerator;
class CShoreLinePGenerator;
class CShoreLineCP;
class CNtlPLShoreLine;


enum eSHORE_LINE_MODE
{
	eSLM_IDLE_MODE = 0,
	eSLM_PICK_MODE,
	eSLM_AGEN_MODE,
	eSLM_PGEN_MODE,
};

#define dHEIGHT_MOVE_LEN (0.01f)

class CShoreLineIOHandler
{
public:
	CShoreLineIOHandler(void);
	~CShoreLineIOHandler(void);

private:
	struct sSHORELINE_CREATE_PARAM
	{
		RwBool _Snap;

		RwV3d _Pos;
		RwV3d _SSnap;
		RwV3d _ESnap;
	};

private:
	RwBool					m_Mode;
	CNtlWorldBrush*			m_pBrushSelector;
	vector<CNtlPLEntity*>	m_vecSelectedSector;
	CShoreLineAGenerator*	m_pSLAGen;
	CShoreLinePGenerator*	m_pSLPGen;
	CNtlPLEntity*			m_pSnapMarker;
	CShoreLineCP*			m_pSLCPNext;

private:
	RwBool	PickWater(RwV3d& PickPos);
	VOID	DeleteClientMeshPoly(CNtlPLShoreLine* _pSLEntity);
	VOID	DeleteClientMesh(RwInt32 _SectorIdx);
	RwBool	InsertClientMesh(RwInt32 _SectorIdx, RwIm3DVertex* _pIm3DVertex, RwInt32 _Size);
	VOID	CreateClientMesh();
	VOID	CreateShoreLinePGenerator(sSHORELINE_CREATE_PARAM& _sSCP);
	VOID	CreateShoreLineAGenerator();
	RwBool	GetSnapVert(RwV3d& _CurPos, RwV3d& SSnapVert, RwV3d& ESnapVert);

private:
	// idle time process handler
	VOID OnSLM_IDLE();

	// picking handler
	VOID OnSLM_PICK();

	// auto generation handler
	VOID OnSLM_AGEN();

	// passive generation handler
	VOID OnSLM_PGEN();

	// delete sector client mesh using polygon picking
	VOID OnSLM_DSCM();

public:
	static RpCollisionTriangle* IntersectionWaterTriCB(RpIntersection* _pIntersection, RpCollisionTriangle* _pCollTriangle, RwReal _Distance, void* _pData);


public:
	VOID	SetHeight(RwReal _Value, RwBool _AddFlag = TRUE);
	VOID	SetHeightClientMesh(RwReal _Value, RwBool _AddFlag = TRUE);
	VOID	Delete();
	VOID	Update();
	VOID	Render();
};
