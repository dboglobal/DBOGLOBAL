#pragma once

#include "ntlworldcommon.h"

class CNtlWorldPathEngineManager;

class CNtlWorldPathEngineManager
{
protected:
	static CNtlWorldPathEngineManager* g_pNtlWorldPathEngineManager;

protected:
	RwInt32			m_iChunkSize;
	RwInt32			m_iChunkNum;
	RwBool			m_bPathEngineMode;
	RwBool			m_bNeedUpdate;
	RwIm3DVertex*	m_pIm3dVertices;
	RwInt32			m_RenderVerticesNum;

	RwInt32			m_iSLmtX;
	RwInt32			m_iELmtX;
	RwInt32			m_iSLmtZ;
	RwInt32			m_iELmtZ;

protected:
	CNtlWorldPathEngineManager();
	virtual ~CNtlWorldPathEngineManager();

public:
	static CNtlWorldPathEngineManager*	GetInstance()					{ return g_pNtlWorldPathEngineManager; }
	static void							CreateInstance();
	static void							DestroyInstance();

	RwBool								GetPathEngineMode()				{ return m_bPathEngineMode; }
	void								SetPathEngineMode(RwBool bMode)	{ m_bPathEngineMode = bMode; }

	void								SetChunkSize(RwInt32 iChunkSize);
	RwInt32								GetChunkSize()					{ return m_iChunkSize; }
	RwInt32								GetChunkNum()					{ return m_iChunkNum; }

	RwBool								LoadSwapFile(RwInt32 iSectorIdx, RwReal* pPEData);
	RwBool								SaveSwapFile(RwInt32 iSectorIdx, RwReal* pPEData);

	RwReal								GetHeight(RwV3d& vPos);
	RwReal								GetPathEngineData(RwV3d& vPos);
	void								SetPathEngineData(RwV3d& vPos, RwReal fHeight);

	void								RenderLmt(RwInt32 iSLmtX, RwInt32 iELmtX, RwInt32 iSLmtZ, RwInt32 iELmtZ);
	void								Render();
};