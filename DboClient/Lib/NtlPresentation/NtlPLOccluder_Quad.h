#pragma once

#include "NtlPLOccluder_Base.h"

#define dPLOCCLUDER_POINT_RADIOS 1.0f

enum EPLANE_OCCLUDER
{
	EPLANE_OCCLUDER_LEFT = 0,
	EPLANE_OCCLUDER_RIGHT,
	EPLANE_OCCLUDER_TOP,
	EPLANE_OCCLUDER_BOTTOM,
	EPLANE_OCCLUDER_NEAR,

	EPLANE_OCCLUDER_NUM,
};

class CNtlPLOccluder_Quad : public CNtlPLOccluder_Base
{
#ifdef dNTL_WORLD_TOOL_MODE
public:
	struct SOccluderSwap_Quad
	{
		RwV3d	vScale;
		RwV3d	vRotate;

		RwV3d	vTrans[4];
	};
#endif
public:
	CNtlPLOccluder_Quad();
	virtual ~CNtlPLOccluder_Quad();

	virtual RwBool			Create(const SPLEntityCreateParam *pParam = NULL);
	virtual void			Destroy();

	virtual RwBool			Render();

	virtual RwUInt32		GetOccluderType() { return EPLOCCLUDER_QUAD; }

	// 각 면의 종류의 따라 업데이트 호출이 다르다.
	virtual RwBool			Update(RwReal fElapsed);
	virtual RwBool			UpdateOneside(RwReal fElapsed);
	virtual RwBool			UpdateTwoside(RwReal fElapsed);
	virtual RwBool			UpdateBillboard(RwReal fElapsed);

	virtual RwBool			OccluderTestSphere(RwSphere* pSphere);
	virtual RwBool			OccluderTestBox(RwBBox* pBox);
	virtual RwBool			VisibilityTest();

	virtual RwBool			Pick(RwReal* pfDist, RwInt32* piIndex = NULL);

	virtual RwV3d*			GetVertexOriginal(RwInt32 iIndex)				{ return &m_vVertexOriginal[iIndex]; }
	virtual void			SetVertexOriginal(RwInt32 iIndex, RwV3d* pV)	{ m_vVertexOriginal[iIndex] = *pV; }

	virtual void			SetPosition(const RwV3d* pPos)					{ m_vPos = *pPos; CalcBoundingSphere(); }
	virtual void			CalcBoundingSphere();

	virtual void			SetTriggerName(const RwChar* pName);
	virtual const char*		GetTriggerName()								{ return m_acTriggerName; }
	virtual void			RunableTrigger(const RwChar* pName);

	virtual RwBool			PVSTest(RwV3d* pvStart, RwV3d* pvEnd);


#ifdef dNTL_WORLD_TOOL_MODE
public:
	static void				SaveSwapFile(RwReal x, RwReal y, RwReal z);
	static void				LoadSwapFile(RwReal x, RwReal y, RwReal z);

	virtual void			CalcOccluderData();
#endif

protected:
	CNtlPlane*			m_pPlane; // 0 : Left // 1 : Right // 2 : Up // 3 : Front // 4 : Down

	RwV3d				m_vVertexOriginal[4];
	RwV3d				m_vVertex[4];

	RwSphere			m_BoundingSphere;

	RwBool				m_bTriggerCheck;
	RwBool				m_bTriggerRunable;
	RwChar				m_acTriggerName[128];

#ifdef dNTL_WORLD_TOOL_MODE
public:
	SOccluderSwap_Quad m_sOccluderSwap;
#endif

public:
	virtual RwBool	LoadFromFile(FILE* pFile);
	virtual RwBool	SaveIntoFile(FILE* pFile);
	static	RwInt32	SkipToFile(FILE* pFile);

	static	BYTE*	SaveIntoFileFromFileMem(FILE* pFile, BYTE* pFileMem);
	static	BYTE*	SkipToFileMem(BYTE* pFileMem);
};