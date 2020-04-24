#pragma once


#include "NtlWorldCommon.h"
#include "NtlPLEvent.h"

#define dNTL_PL_HEATHAZE_INTERPOLATION_TIME (7.0f)

class CNtlPLHeatHaze : public RWS::CEventHandler
{
public:
	CNtlPLHeatHaze(RwInt32 _ScrWidth, RwInt32 _ScrHeight);
	virtual ~CNtlPLHeatHaze(void);

protected:
	RwBool				m_EnableFlag;

	RwIm2DVertex*		m_pVertices;
	RwImVertexIndex*	m_pIndices;

	RwInt32				m_VertexGridDensity;
	RwReal				m_ScrWidth;
	RwReal				m_ScrHeight;
	RwReal				m_AccumulatedTime;

	sNTL_HEATHAZE		m_CurHeatHaze;
	sNTL_HEATHAZE		m_OldHeatHaze;
	sNTL_HEATHAZE		m_NewHeatHaze;

	sNTL_HEATHAZE		m_FieldHeatHaze;
	sNTL_HEATHAZE*		m_pObjHeatHaze;

	RwBool				m_bUpdateInterpolation;
	RwReal				m_fInterpolationTime;

#ifdef dNTL_WORLD_TOOL_MODE
public:
	sNTL_HEATHAZE*		m_pToolHeatHaze;
#endif

public:
	RwBool			Create();
	void			Destroy();
	RwBool			Update(RwReal fElapsed, RwCamera* _pRenderTargetCam);
	RwBool			Render(RwTexture* _pCurSceneTex, RwCamera* _pRWCam);
	void			ReleaseHandler();
	void			RestoreHandler(RwCamera* _pRenderTargetCam);
	RwBool			GetEnabled() { return m_EnableFlag; }
	void			SetEnabled(RwBool _EnabledFlag) { m_EnableFlag = _EnabledFlag; }
	void			SetScrSize(RwRect* _pScrSize) { m_ScrWidth = static_cast<RwReal>(_pScrSize->w); m_ScrHeight = static_cast<RwReal>(_pScrSize->h); }
	void			ResizeGrid(RwInt32 iNum);

	void			HandleEvents(RWS::CMsg &pMsg);

	const sNTL_HEATHAZE* GetCurHeatHaze() { return &m_CurHeatHaze; }

protected:
	RwBool	Interpolation(const sNTL_HEATHAZE& sNtlHeatHaze);
	RwBool	UpdateInterpolation(RwReal fElapsed);
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CNtlPLHeatHazeObject : public CNtlPLEntity
{
protected:
	RwV3d			m_vVertex[4];
	RwSphere		m_Sphere;
	sNTL_HEATHAZE	m_HeatHaze;
	RwReal			m_fDistance;

public:
	CNtlPLHeatHazeObject();
	virtual ~CNtlPLHeatHazeObject();

	virtual RwBool	Create(const SPLEntityCreateParam *pParam = NULL);
	virtual void	Destroy(void);
	virtual RwBool	Render(void);
	virtual RwBool	SetProperty(const CNtlPLProperty *pData);

	RwBool			FrustumTest();
	void			CalcBoundingSphere();
	
	sNTL_HEATHAZE*	HeatHazeData()				{ return &m_HeatHaze; }

	void			SetDistance(RwReal fDist)	{ m_fDistance = fDist; }
	RwReal			GetDistance()				{ return m_fDistance; }

	void			SetVertex(RwInt32 iIndex, RwV3d* pVertex)	{ m_vVertex[iIndex] = *pVertex; }
	RwV3d*			GetVertex(RwInt32 iIndex)					{ return &m_vVertex[iIndex]; }

#ifdef dNTL_WORLD_TOOL_MODE
public:
	struct SHeatHazeObjectSwap
	{
		RwV3d vScale;
		RwV3d vRotate;
		RwV3d vTrans;
	};

	SHeatHazeObjectSwap m_HeatHazeObjectSwap;

	RwBool				m_bToolUI;

	void				SetToolUI(RwBool bToolUI);
	RwBool				Pick(RwReal* pfDist);
	void				CalcHeatHazeData();

	static void			SaveSwapFile(RwReal x, RwReal y, RwReal z);
	static void			LoadSwapFile(RwReal x, RwReal y, RwReal z);

	virtual void		SetPosition(const RwV3d *pPos)	{ m_HeatHazeObjectSwap.vTrans = *pPos; CalcHeatHazeData(); }
	virtual RwV3d		GetPosition(void)				{ return m_HeatHazeObjectSwap.vTrans; }
#endif

public:
	virtual RwBool	LoadFromFile(FILE* pFile);
	virtual RwBool	SaveIntoFile(FILE* pFile);
	static	RwInt32	SkipToFile(FILE* pFile);

	static	BYTE*	SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem, RwV3d* pvOffset = NULL);
	static	BYTE*	SkipToFileMem(BYTE* pFileMem);
};