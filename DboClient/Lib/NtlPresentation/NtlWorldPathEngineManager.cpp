#include "precomp_ntlpresentation.h"

#include "NtlPLSceneManager.h"
#include "NtlPLVisualManager.h"
#include "NtlWorldFieldManager.h"

#include "NtlPLRenderState.h"

#include "NtlPLWorldEntity.h"

#include "NtlWorldPathEngineManager.h"

CNtlWorldPathEngineManager* CNtlWorldPathEngineManager::g_pNtlWorldPathEngineManager = NULL;

CNtlWorldPathEngineManager::CNtlWorldPathEngineManager()
:m_iChunkSize(dGET_WORLD_PARAM()->WorldSectorTileSize)
,m_iChunkNum(dGET_WORLD_PARAM()->WorldSectorTileNum)
,m_bPathEngineMode(FALSE)
,m_bNeedUpdate(FALSE)
,m_pIm3dVertices(NULL)
,m_iSLmtX(0)
,m_iELmtX(0)
,m_iSLmtZ(0)
,m_iELmtZ(0)
,m_RenderVerticesNum(0)
{
}

CNtlWorldPathEngineManager::~CNtlWorldPathEngineManager()
{
	if (m_pIm3dVertices)
	{
		NTL_ARRAY_DELETE(m_pIm3dVertices);
	}
}

void CNtlWorldPathEngineManager::CreateInstance()
{
	if (!g_pNtlWorldPathEngineManager)
	{
		g_pNtlWorldPathEngineManager = NTL_NEW CNtlWorldPathEngineManager;
	}
}

void CNtlWorldPathEngineManager::DestroyInstance()
{
	NTL_DELETE(g_pNtlWorldPathEngineManager);
}

RwBool CNtlWorldPathEngineManager::LoadSwapFile(RwInt32 iSectorIdx, RwReal* pPEData)
{
	if (!m_bPathEngineMode)
		return FALSE;

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	if(_chdir("swap") == -1)
	{
		// create folders
		_mkdir("swap");
		_chdir("swap");
	}

	if(_chdir("pe") == -1)
	{
		// create folders
		_mkdir("pe");
		_chdir("pe");
	}

	FILE* pFile;

	if(::fopen_s(&pFile, "header.pe", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "header.pe" << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return FALSE;
	}
	
	RwInt32 iCunkSize;
	fread(&iCunkSize, sizeof(RwInt32), 1, pFile);
	fclose(pFile);

	if (iCunkSize != CNtlWorldPathEngineManager::GetInstance()->GetChunkSize())
	{
		DBO_TRACE(FALSE, "CNtlWorldPathEngineManager::LoadSwapFile, chunk size error.");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return FALSE;
	}	

	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.pe", iSectorIdx);

	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return FALSE;
	}

	RwInt32 iDataSize = (dGET_WORLD_PARAM()->WorldSectorSize / CNtlWorldPathEngineManager::GetInstance()->GetChunkSize())
		* (dGET_WORLD_PARAM()->WorldSectorSize / CNtlWorldPathEngineManager::GetInstance()->GetChunkSize());

	::fread(pPEData, sizeof(RwReal) * iDataSize, 1, pFile);

	::fclose(pFile);
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

RwBool CNtlWorldPathEngineManager::SaveSwapFile(RwInt32 iSectorIdx, RwReal* pPEData)
{
	if (!m_bPathEngineMode)
		return FALSE;

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	if(_chdir("swap") == -1)
	{
		// create folders
		_mkdir("swap");
		_chdir("swap");
	}

	if(_chdir("pe") == -1)
	{
		// create folders
		_mkdir("pe");
		_chdir("pe");
	}

	FILE* pFile;

	RwInt32 iCunkSize;
	if(::fopen_s(&pFile, "header.pe", "rb"))
	{
		if(::fopen_s(&pFile, "header.pe", "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << "header.pe" << ")");
			::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return FALSE;
		}

		iCunkSize = CNtlWorldPathEngineManager::GetInstance()->GetChunkSize();
		fwrite(&iCunkSize, sizeof(RwInt32), 1, pFile);
		fclose(pFile);
	}
	else
	{
		fread(&iCunkSize, sizeof(RwInt32), 1, pFile);
		fclose(pFile);

		if (iCunkSize != CNtlWorldPathEngineManager::GetInstance()->GetChunkSize())
		{
			DBO_TRACE(FALSE, "CNtlWorldPathEngineManager::SaveSwapFile, chunk size error.");
			::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			return FALSE;
		}
	}

	::sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.pe", iSectorIdx);

	if(::fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
		return FALSE;
	}

	RwInt32 iDataSize = (dGET_WORLD_PARAM()->WorldSectorSize / CNtlWorldPathEngineManager::GetInstance()->GetChunkSize())
		* (dGET_WORLD_PARAM()->WorldSectorSize / CNtlWorldPathEngineManager::GetInstance()->GetChunkSize());

	::fwrite(pPEData, sizeof(RwReal) * iDataSize, 1, pFile);
	
	::fclose(pFile);
	::_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

RwReal CNtlWorldPathEngineManager::GetHeight(RwV3d& vPos)
{
	RwReal					fHeight;
	CNtlWorldFieldManager*	pFMgr = GetSceneManager()->GetWorld()->GetWorldFieldMgr();

	fHeight = GetPathEngineData(vPos);
	
	if (fHeight == RwRealMAXVAL)
	{
		fHeight = pFMgr->GetWorldSectorHeight(vPos);
	}	
	if (fHeight == -999.0f)
	{
		return RwRealMAXVAL;
	}
	return fHeight;
}

RwReal CNtlWorldPathEngineManager::GetPathEngineData(RwV3d& vPos)
{
	CNtlWorldFieldManager*	pFMgr		= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	RwInt32					iSectorIdx	= pFMgr->GetSectorIdx(vPos);
	
	if(iSectorIdx == -1)
		NTL_RETURN(-999.0f);

	RwV3d	vPosS;
	vPosS.x = pFMgr->m_pSectors[iSectorIdx].DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2);
	vPosS.y = pFMgr->m_pSectors[iSectorIdx].DatumPoint.y;
	vPosS.z = pFMgr->m_pSectors[iSectorIdx].DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2);

	RwInt32	iMoveCntX	= static_cast<RwInt32>(abs(vPos.x - vPosS.x)) / m_iChunkSize;
	RwInt32	iMoveCntZ	= static_cast<RwInt32>(abs(vPos.z - vPosS.z)) / m_iChunkSize;
	RwInt32 iDataPos	= iMoveCntX + (iMoveCntZ * m_iChunkNum);

	if (!pFMgr->m_pSectors[iSectorIdx].m_pNtlWorldPathEngineData)
	{
		return RwRealMAXVAL;
	}
	return pFMgr->m_pSectors[iSectorIdx].m_pNtlWorldPathEngineData[iDataPos];
}

void CNtlWorldPathEngineManager::SetPathEngineData(RwV3d& vPos, RwReal fHeight)
{
	CNtlWorldFieldManager*	pFMgr		= GetSceneManager()->GetWorld()->GetWorldFieldMgr();
	RwInt32					iSectorIdx	= pFMgr->GetSectorIdx(vPos);

	if(iSectorIdx == -1)
		return;

	RwV3d	vPosS;
	vPosS.x = pFMgr->m_pSectors[iSectorIdx].DatumPoint.x - (dGET_WORLD_PARAM()->WorldSectorSize / 2);
	vPosS.y = pFMgr->m_pSectors[iSectorIdx].DatumPoint.y;
	vPosS.z = pFMgr->m_pSectors[iSectorIdx].DatumPoint.z - (dGET_WORLD_PARAM()->WorldSectorSize / 2);

	RwInt32	iMoveCntX	= static_cast<RwInt32>(abs(vPos.x - vPosS.x)) / m_iChunkSize;
	RwInt32	iMoveCntZ	= static_cast<RwInt32>(abs(vPos.z - vPosS.z)) / m_iChunkSize;
	RwInt32 iDataPos	= iMoveCntX + (iMoveCntZ * m_iChunkNum);

	pFMgr->m_pSectors[iSectorIdx].m_pNtlWorldPathEngineData[iDataPos] = fHeight;

	m_bNeedUpdate = TRUE;
}

void CNtlWorldPathEngineManager::SetChunkSize(RwInt32 iChunkSize)
{
	m_iChunkSize	= iChunkSize;
	m_iChunkNum		= dGET_WORLD_PARAM()->WorldSectorSize / m_iChunkSize;

	RwInt32	NumFieldTile	= dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldSectorSize / m_iChunkSize;
	RwInt32 NumRenderField	= 9;

	m_pIm3dVertices = NTL_NEW RwIm3DVertex [NumFieldTile * NumFieldTile * NumRenderField * 6];
}

void CNtlWorldPathEngineManager::RenderLmt(RwInt32 iSLmtX, RwInt32 iELmtX, RwInt32 iSLmtZ, RwInt32 iELmtZ)
{
	if (iSLmtX < iELmtX)
	{
		m_iSLmtX = iSLmtX;
		m_iELmtX = iELmtX;
	}
	else
	{
		m_iSLmtX = iELmtX;
		m_iELmtX = iSLmtX;
	}

	if (iSLmtZ < iELmtZ)
	{
		m_iSLmtZ = iSLmtZ;
		m_iELmtZ = iELmtZ;
	}
	else
	{
		m_iSLmtZ = iELmtZ;
		m_iELmtZ = iSLmtZ;
	}

	m_bNeedUpdate = TRUE;
}

void CNtlWorldPathEngineManager::Render()
{ 
	if (m_pIm3dVertices == NULL)
	{
		return;
	}
	
	if (m_bNeedUpdate)
	{		
		RwInt32	i = 0;
		RwV3d	PosRB, PosLT, PosLB, PosRT;
		for (int z = m_iSLmtZ; z < m_iELmtZ; ++z)
		{
			for (int x = m_iSLmtX; x < m_iELmtX; ++x)
			{
				PosRB.x = static_cast<RwReal>(x * m_iChunkSize - dGET_WORLD_PARAM()->WorldSizeHalf);
				PosRB.z = static_cast<RwReal>(z * m_iChunkSize - dGET_WORLD_PARAM()->WorldSizeHalf);
				PosRB.y = CNtlWorldPathEngineManager::GetInstance()->GetPathEngineData(PosRB);
				
				if (PosRB.y == RwRealMAXVAL)
				{
					continue;
				}
				PosLT.x = PosRB.x + m_iChunkSize;
				PosLT.z = PosRB.z + m_iChunkSize;
				PosLT.y = PosRB.y;
				PosLB.x = PosRB.x + m_iChunkSize;
				PosLB.z = PosRB.z;
				PosLB.y = PosRB.y;
				PosRT.x = PosRB.x;
				PosRT.z = PosRB.z + m_iChunkSize;
				PosRT.y = PosRB.y;
				 
				// 0  2
				// 1  3
				RwIm3DVertexSetU(&m_pIm3dVertices[i * 6 + 0], 0.0f);	
				RwIm3DVertexSetV(&m_pIm3dVertices[i * 6 + 0], 0.0f);
				RwIm3DVertexSetU(&m_pIm3dVertices[i * 6 + 1], 0.0f);		
				RwIm3DVertexSetV(&m_pIm3dVertices[i * 6 + 1], 1.0f);
				RwIm3DVertexSetU(&m_pIm3dVertices[i * 6 + 2], 1.0f);		
				RwIm3DVertexSetV(&m_pIm3dVertices[i * 6 + 2], 0.0f);
				RwIm3DVertexSetRGBA(&m_pIm3dVertices[i * 6 + 0], 255, 255, 255, 128);
				RwIm3DVertexSetRGBA(&m_pIm3dVertices[i * 6 + 1], 255, 255, 255, 128);
				RwIm3DVertexSetRGBA(&m_pIm3dVertices[i * 6 + 2], 255, 255, 255, 128);
				RwIm3DVertexSetPos(&m_pIm3dVertices[i * 6 + 0], PosLT.x, PosLT.y, PosLT.z);
				RwIm3DVertexSetPos(&m_pIm3dVertices[i * 6 + 1], PosLB.x, PosLB.y, PosLB.z);
				RwIm3DVertexSetPos(&m_pIm3dVertices[i * 6 + 2], PosRT.x, PosRT.y, PosRT.z);
				
				RwIm3DVertexSetU(&m_pIm3dVertices[i * 6 + 3], 1.0f);		
				RwIm3DVertexSetV(&m_pIm3dVertices[i * 6 + 3], 1.0f);
				RwIm3DVertexSetU(&m_pIm3dVertices[i * 6 + 4], 1.0f);		
				RwIm3DVertexSetV(&m_pIm3dVertices[i * 6 + 4], 0.0f);
				RwIm3DVertexSetU(&m_pIm3dVertices[i * 6 + 5], 0.0f);		
				RwIm3DVertexSetV(&m_pIm3dVertices[i * 6 + 5], 1.0f);
				
				RwIm3DVertexSetRGBA(&m_pIm3dVertices[i * 6 + 3], 255, 255, 255, 255);
				RwIm3DVertexSetRGBA(&m_pIm3dVertices[i * 6 + 4], 255, 255, 255, 255);
				RwIm3DVertexSetRGBA(&m_pIm3dVertices[i * 6 + 5], 255, 255, 255, 255);			
				RwIm3DVertexSetPos(&m_pIm3dVertices[i * 6 + 3], PosRB.x, PosRB.y, PosRB.z);
				RwIm3DVertexSetPos(&m_pIm3dVertices[i * 6 + 4], PosRT.x, PosRT.y, PosRT.z);
				RwIm3DVertexSetPos(&m_pIm3dVertices[i * 6 + 5], PosLB.x, PosLB.y, PosLB.z);		

				++i;
			}
		}
		m_RenderVerticesNum = i * 6;
		m_bNeedUpdate		= FALSE;
	}

	RwInt32			iNum = m_RenderVerticesNum;
	RwIm3DVertex*	pIm3dVertices = m_pIm3dVertices;

	BeginFogState(FALSE);

	while (iNum > 0)
	{
		if (iNum > 65535)
		{			
			if(RwIm3DTransform(pIm3dVertices, 65535, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA | rwIM3D_VERTEXUV))
			{
				RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
				RwIm3DEnd();
			}
			pIm3dVertices	+= 65535;
			iNum			-= 65535;
		}
		else
		{
			if(RwIm3DTransform(pIm3dVertices, iNum, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXRGBA | rwIM3D_VERTEXUV))
			{
				RwIm3DRenderPrimitive(rwPRIMTYPETRILIST);
				RwIm3DEnd();
			}
			pIm3dVertices	+= iNum;
			iNum			-= iNum;
		}
	}
	
	EndFogState();
}