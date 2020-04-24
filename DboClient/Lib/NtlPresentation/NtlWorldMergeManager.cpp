#include "precomp_ntlpresentation.h"
#include "NtlNaviDataMng.h"
#include "NtlWorldMergeManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlMath.h"
#include "NtlStringUtil.h"
#include "NtlPLGameProperty.h"

#include "NtlPLWorldEntity.h"
#include "NtlPLObject.h"
#include "NtlPLPlanet.h"
#include "NtlPLObjectType.h"

#include "NtlWorldSectorInfo.h"
#include "NtlWorldFieldInfo.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

#ifdef dNTL_WORLD_TOOL_MODE


CNtlWorldMergeManager::CNtlWorldMergeManager(void)
{
}

CNtlWorldMergeManager::~CNtlWorldMergeManager(void)
{
}

CNtlWorldMergeManager* CNtlWorldMergeManager::GetInstance(VOID)
{
	static CNtlWorldMergeManager g_NtlWorldMergeManager;
	return &g_NtlWorldMergeManager;
}

RwBool CNtlWorldMergeManager::VerifyMergeWorld(const char* _pPathName)
{
	sNtlWorldParam NtlWorldParamSrc;
	CopyMemory(&NtlWorldParamSrc, dGET_WORLD_PARAM(), sizeof(sNtlWorldParam));


	if(strcmp(NtlWorldParamSrc.WorldVer, m_NtlWorldParam2Merge.WorldVer))
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldHeightfieldNum != m_NtlWorldParam2Merge.WorldHeightfieldNum)
	{
		return FALSE;
	}
	
	if(NtlWorldParamSrc.WorldSectorTileSize != m_NtlWorldParam2Merge.WorldSectorTileSize)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSectorTileNum != m_NtlWorldParam2Merge.WorldSectorTileNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSectorVertNum != m_NtlWorldParam2Merge.WorldSectorVertNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSectorSize != m_NtlWorldParam2Merge.WorldSectorSize)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSectorNum != m_NtlWorldParam2Merge.WorldSectorNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSectorHalfNum != m_NtlWorldParam2Merge.WorldSectorHalfNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSectorPolyNum != m_NtlWorldParam2Merge.WorldSectorPolyNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldFieldSectorNum != m_NtlWorldParam2Merge.WorldFieldSectorNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldFieldSize != m_NtlWorldParam2Merge.WorldFieldSize)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldFieldNum != m_NtlWorldParam2Merge.WorldFieldNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldFieldHalfNum != m_NtlWorldParam2Merge.WorldFieldHalfNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSize != m_NtlWorldParam2Merge.WorldSize)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSizeHalf != m_NtlWorldParam2Merge.WorldSizeHalf)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldValueMax != m_NtlWorldParam2Merge.WorldValueMax)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldValueMin != m_NtlWorldParam2Merge.WorldValueMin)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldTexAddr != m_NtlWorldParam2Merge.WorldTexAddr)
	{
		return FALSE;
	}
/*
	if(strcmp(NtlWorldParamSrc.WorldRAW, m_NtlWorldParam2Merge.WorldRAW))
	{
		return FALSE;
	}
*/
	// same project check
	RwChar BackSlashVerOfPathName[NTL_MAX_DIR_PATH];
	strcpy_s(BackSlashVerOfPathName, NTL_MAX_DIR_PATH, _pPathName);
	strcat_s(BackSlashVerOfPathName, "\\");

	if(!strcmp(dGET_WORLD_PARAM()->WorldProjectFolderName, BackSlashVerOfPathName))
	{
		return FALSE;
	}

	// set the folder path of the world 2 merge
	strcpy_s(m_NtlWorldParam2Merge.WorldProjectFolderName, 256, BackSlashVerOfPathName);

	return TRUE;
}

RwBool CNtlWorldMergeManager::Load(const char* _pPathName)
{
	_chdir(_pPathName);

	FILE* pFile;
	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&m_NtlWorldParam2Merge, pFile);
		fclose(pFile);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	if(!VerifyMergeWorld(_pPathName))
	{
		DBO_TRACE(FALSE, "CNtlWorldMergeManager::Load, VerifyMergeWorld(" << _pPathName << ")");
	
		::MessageBox(NULL, "These two worlds can't be merged. Please try again.", "SAVER", MB_OK);

		return FALSE;
	}

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnHeightfieldVariation(RwInt32 IdxField)
{
	// sector variables
	CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(m_pFieldMgr->GetFields());
	int				i, j;
	RwV3d			SPos = pFields[IdxField].GetSPos();
	RwV3d			SectorSPos, PosSectorS;

	// heightfield variables
	RwInt32				IdxSector;
	RwInt32				NumVert			= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;
	RwReal				TileSize		= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
	RwInt32				LenTile			= static_cast<RwInt32>(TileSize);
	RwReal				LenSector		= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
	RwReal				LenSectorHalf	= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2.0f);
	RwInt32				NumTilePerSec	= dGET_WORLD_PARAM()->WorldSectorTileNum;
	sNTL_WORLD_SL*		pNtlWorldSL		= NULL;
	CNtlWorldSector*	pSectors		= m_pFieldMgr->CNtlWorldSectorManager::m_pSectors;
	RwV3d*				pPosVertList	= NTL_NEW RwV3d [NumVert];
	RwV3d				PosSectorDatum;
	RwV3d				CurVert;
	RwInt32				LenWidth, LenHeight;
	RwInt32				NumTileX, NumTileZ;
	RwV3d				PosNbrSecDatum[8];
	RwV3d				PosCurSecDatum;
	RwV3d				PosRetHeight[8];
	RwV3d				PosResult;
	RwReal				ValMergeHeight;
	RwBool				FlgBreak;


	// loop sectors
	for(i = static_cast<RwInt32>(SPos.z); i < static_cast<RwInt32>(SPos.z) + dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(j = static_cast<RwInt32>(SPos.x); j < static_cast<RwInt32>(SPos.x) + dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = static_cast<RwReal>(j);
			SectorSPos.z = static_cast<RwReal>(i);
			IdxSector = m_pFieldMgr->GetSectorIdx(SectorSPos);

			// exception
			if(IdxSector == -1)
			{
				DBO_TRACE(FALSE, "CNtlWorldMergeManager::OnHeightfieldVariation, invalid sector index,");
				continue;
			}

			CNtlMath::MathRwV3dAssign(&PosSectorDatum, pSectors[IdxSector].DatumPoint.x, pSectors[IdxSector].DatumPoint.y, pSectors[IdxSector].DatumPoint.z);
			if(!m_pFieldMgr->GetPosVertList(PosSectorDatum, pPosVertList))
			{
				DBO_TRACE(FALSE, "CNtlWorldMergeManager::OnHeightfieldVariation, GetPosVertList return failed.");
				continue;
			}

			// change to the sector starting pos.
			CNtlMath::MathRwV3dAssign(&PosSectorS, pSectors[IdxSector].DatumPoint.x - LenSectorHalf, 0.0f, pSectors[IdxSector].DatumPoint.z - LenSectorHalf);

			for(int i = 0; i < NumVert; ++i)
			{
				// current vert.
				CNtlMath::MathRwV3dAssign(&CurVert, pPosVertList[i].x, 0.0f, pPosVertList[i].z);

				// based on starting pos.
				LenWidth	= static_cast<RwInt32>(fabs(CurVert.x - PosSectorS.x));
				LenHeight	= static_cast<RwInt32>(fabs(CurVert.z - PosSectorS.z));
				NumTileX	= LenWidth / LenTile;
				NumTileZ	= LenHeight / LenTile;

				// This points should be the edge vertices, LenSRadiusMax was calculated to include only the edge vertices.
				if(!NumTileX || NumTileX == NumTilePerSec || !NumTileZ || NumTileZ == NumTilePerSec)
				{
					// 3---2---1
					// |       | 
					// 4  cur  0
					// |       |
					// 5---6---7

					// classify vertices
					if(!NumTileX && (NumTileZ > 0 && NumTileZ < NumTilePerSec))
					{
						// 0
						CNtlMath::MathRwV3dAssign(&PosCurSecDatum, CurVert.x + TileSize, 0.0f, CurVert.z);
						if(m_pFieldMgr->GetHeightFromFile(PosCurSecDatum, PosCurSecDatum))
						{
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], pSectors[IdxSector].DatumPoint.x - LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z);
							CNtlMath::MathRwV3dAssign(&PosRetHeight[0], CurVert.x - TileSize, 0.0f, CurVert.z);
							if(m_pFieldMgr->GetHeightFromFile(PosNbrSecDatum[0], PosRetHeight[0]))
							{
								ValMergeHeight	= (PosCurSecDatum.y + PosRetHeight[0].y) / 2.0f;
								PosResult		= CurVert;
								PosResult.y		= ValMergeHeight;

								m_pFieldMgr->SetHeight(PosCurSecDatum, PosResult);
								m_pFieldMgr->SetHeight(PosNbrSecDatum[0], PosResult);
							}
						}
					}
					else if(!NumTileX && NumTileZ == NumTilePerSec)
					{
						// 1
						CNtlMath::MathRwV3dAssign(&PosCurSecDatum, CurVert.x + TileSize, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], CurVert.x - TileSize, 0.0f, CurVert.z);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[1], CurVert.x - TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[2], CurVert.x, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[3], CurVert.x + TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[4], CurVert.x + TileSize, 0.0f, CurVert.z);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[5], CurVert.x + TileSize, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[6], CurVert.x, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[7], CurVert.x - TileSize, 0.0f, CurVert.z - TileSize);

						CopyMemory(PosRetHeight, PosNbrSecDatum, sizeof(RwV3d) * 8);

						FlgBreak = FALSE;
						for(RwInt32 i = 0; i < 8; ++i)
						{
							if(!m_pFieldMgr->GetHeightFromFile(PosNbrSecDatum[i], PosRetHeight[i]))
							{
								FlgBreak = TRUE;
								break;
							}
						}

						if(!FlgBreak)
						{
							ValMergeHeight = 0.0f;

							for(RwInt32 i = 0; i < 8; ++i)
							{
								ValMergeHeight += PosRetHeight[i].y;
							}

							ValMergeHeight /= 8.0f;
							PosResult		= CurVert;
							PosResult.y		= ValMergeHeight;

							// set values for each sector
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], pSectors[IdxSector].DatumPoint.x - LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z);
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[1], pSectors[IdxSector].DatumPoint.x - LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z + LenSector);
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[2], pSectors[IdxSector].DatumPoint.x, 0.0f, pSectors[IdxSector].DatumPoint.z + LenSector);
							m_pFieldMgr->SetHeight(PosCurSecDatum, PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[0], PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[1], PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[2], PosResult);
						}
					}
					else if((NumTileX > 0 && NumTileX < NumTilePerSec) && NumTileZ == NumTilePerSec)
					{
						// 2
						CNtlMath::MathRwV3dAssign(&PosCurSecDatum, CurVert.x, 0.0f, CurVert.z - TileSize);
						if(m_pFieldMgr->GetHeightFromFile(PosCurSecDatum, PosCurSecDatum))
						{
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], pSectors[IdxSector].DatumPoint.x, 0.0f, pSectors[IdxSector].DatumPoint.z + LenSector);
							CNtlMath::MathRwV3dAssign(&PosRetHeight[0], CurVert.x, 0.0f, CurVert.z + TileSize);
							if(m_pFieldMgr->GetHeightFromFile(PosNbrSecDatum[0], PosRetHeight[0]))
							{
								ValMergeHeight	= (PosCurSecDatum.y + PosRetHeight[0].y) / 2.0f;
								PosResult		= CurVert;
								PosResult.y		= ValMergeHeight;

								m_pFieldMgr->SetHeight(PosCurSecDatum, PosResult);
								m_pFieldMgr->SetHeight(PosNbrSecDatum[0], PosResult);
							}
						}
					}
					else if(NumTileX == NumTilePerSec && NumTileZ == NumTilePerSec)
					{
						// 3
						CNtlMath::MathRwV3dAssign(&PosCurSecDatum, CurVert.x - TileSize, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], CurVert.x - TileSize, 0.0f, CurVert.z);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[1], CurVert.x - TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[2], CurVert.x, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[3], CurVert.x + TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[4], CurVert.x + TileSize, 0.0f, CurVert.z);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[5], CurVert.x + TileSize, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[6], CurVert.x, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[7], CurVert.x - TileSize, 0.0f, CurVert.z - TileSize);

						CopyMemory(PosRetHeight, PosNbrSecDatum, sizeof(RwV3d) * 8);

						FlgBreak = FALSE;
						for(RwInt32 i = 0; i < 8; ++i)
						{
							if(!m_pFieldMgr->GetHeightFromFile(PosNbrSecDatum[i], PosRetHeight[i]))
							{
								FlgBreak = TRUE;
								break;
							}
						}

						if(!FlgBreak)
						{
							ValMergeHeight = 0.0f;

							for(RwInt32 i = 0; i < 8; ++i)
							{
								ValMergeHeight += PosRetHeight[i].y;
							}

							ValMergeHeight /= 8.0f;
							PosResult		= CurVert;
							PosResult.y		= ValMergeHeight;

							// set values for each sector
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], pSectors[IdxSector].DatumPoint.x, 0.0f, pSectors[IdxSector].DatumPoint.z + LenSector);
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[1], pSectors[IdxSector].DatumPoint.x + LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z + LenSector);
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[2], pSectors[IdxSector].DatumPoint.x + LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z);
							m_pFieldMgr->SetHeight(PosCurSecDatum, PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[0], PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[1], PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[2], PosResult);
						}
					}
					else if(NumTileX == NumTilePerSec && (NumTileZ > 0 && NumTileZ < NumTilePerSec))
					{
						// 4
						CNtlMath::MathRwV3dAssign(&PosCurSecDatum, CurVert.x - TileSize, 0.0f, CurVert.z);
						if(m_pFieldMgr->GetHeightFromFile(PosCurSecDatum, PosCurSecDatum))
						{
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], pSectors[IdxSector].DatumPoint.x + LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z);
							CNtlMath::MathRwV3dAssign(&PosRetHeight[0], CurVert.x + TileSize, 0.0f, CurVert.z);
							if(m_pFieldMgr->GetHeightFromFile(PosNbrSecDatum[0], PosRetHeight[0]))
							{
								ValMergeHeight	= (PosCurSecDatum.y + PosRetHeight[0].y) / 2.0f;
								PosResult		= CurVert;
								PosResult.y		= ValMergeHeight;

								m_pFieldMgr->SetHeight(PosCurSecDatum, PosResult);
								m_pFieldMgr->SetHeight(PosNbrSecDatum[0], PosResult);
							}
						}
					}
					else if(NumTileX == NumTilePerSec && !NumTileZ)
					{
						// 5
						CNtlMath::MathRwV3dAssign(&PosCurSecDatum, CurVert.x - TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], CurVert.x - TileSize, 0.0f, CurVert.z);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[1], CurVert.x - TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[2], CurVert.x, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[3], CurVert.x + TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[4], CurVert.x + TileSize, 0.0f, CurVert.z);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[5], CurVert.x + TileSize, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[6], CurVert.x, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[7], CurVert.x - TileSize, 0.0f, CurVert.z - TileSize);

						CopyMemory(PosRetHeight, PosNbrSecDatum, sizeof(RwV3d) * 8);

						FlgBreak = FALSE;
						for(RwInt32 i = 0; i < 8; ++i)
						{
							if(!m_pFieldMgr->GetHeightFromFile(PosNbrSecDatum[i], PosRetHeight[i]))
							{
								FlgBreak = TRUE;
								break;
							}
						}

						if(!FlgBreak)
						{
							ValMergeHeight = 0.0f;

							for(RwInt32 i = 0; i < 8; ++i)
							{
								ValMergeHeight += PosRetHeight[i].y;
							}

							ValMergeHeight /= 8.0f;
							PosResult		= CurVert;
							PosResult.y		= ValMergeHeight;

							// set values for each sector
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], pSectors[IdxSector].DatumPoint.x + LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z);
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[1], pSectors[IdxSector].DatumPoint.x + LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z - LenSector);
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[2], pSectors[IdxSector].DatumPoint.x, 0.0f, pSectors[IdxSector].DatumPoint.z - LenSector);
							m_pFieldMgr->SetHeight(PosCurSecDatum, PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[0], PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[1], PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[2], PosResult);
						}
					}
					else if((NumTileX > 0 && NumTileX < NumTilePerSec) && !NumTileZ)
					{
						// 6
						CNtlMath::MathRwV3dAssign(&PosCurSecDatum, CurVert.x, 0.0f, CurVert.z + TileSize);
						if(m_pFieldMgr->GetHeightFromFile(PosCurSecDatum, PosCurSecDatum))
						{
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], pSectors[IdxSector].DatumPoint.x, 0.0f, pSectors[IdxSector].DatumPoint.z - LenSector);
							CNtlMath::MathRwV3dAssign(&PosRetHeight[0], CurVert.x, 0.0f, CurVert.z - TileSize);
							if(m_pFieldMgr->GetHeightFromFile(PosNbrSecDatum[0], PosRetHeight[0]))
							{
								ValMergeHeight	= (PosCurSecDatum.y + PosRetHeight[0].y) / 2.0f;
								PosResult		= CurVert;
								PosResult.y		= ValMergeHeight;

								m_pFieldMgr->SetHeight(PosCurSecDatum, PosResult);
								m_pFieldMgr->SetHeight(PosNbrSecDatum[0], PosResult);
							}
						}
					}
					else if(!NumTileX && !NumTileZ)
					{
						// 7
						CNtlMath::MathRwV3dAssign(&PosCurSecDatum, CurVert.x + TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], CurVert.x - TileSize, 0.0f, CurVert.z);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[1], CurVert.x - TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[2], CurVert.x, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[3], CurVert.x + TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[4], CurVert.x + TileSize, 0.0f, CurVert.z);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[5], CurVert.x + TileSize, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[6], CurVert.x, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[7], CurVert.x - TileSize, 0.0f, CurVert.z - TileSize);

						CopyMemory(PosRetHeight, PosNbrSecDatum, sizeof(RwV3d) * 8);

						FlgBreak = FALSE;
						for(RwInt32 i = 0; i < 8; ++i)
						{
							if(!m_pFieldMgr->GetHeightFromFile(PosNbrSecDatum[i], PosRetHeight[i]))
							{
								FlgBreak = TRUE;
								break;
							}
						}

						if(!FlgBreak)
						{
							ValMergeHeight = 0.0f;

							for(RwInt32 i = 0; i < 8; ++i)
							{
								ValMergeHeight += PosRetHeight[i].y;
							}

							ValMergeHeight /= 8.0f;
							PosResult		= CurVert;
							PosResult.y		= ValMergeHeight;

							// set values for each sector
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], pSectors[IdxSector].DatumPoint.x, 0.0f, pSectors[IdxSector].DatumPoint.z - LenSector);
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[1], pSectors[IdxSector].DatumPoint.x - LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z - LenSector);
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[2], pSectors[IdxSector].DatumPoint.x - LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z);
							m_pFieldMgr->SetHeight(PosCurSecDatum, PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[0], PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[1], PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[2], PosResult);
						}
					}
				}
			}
		}
	}

	NTL_ARRAY_DELETE(pPosVertList);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportMapObj(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	/*
	WorldFileFormat - Export
	Export 처리를 위한 부분이다.
	*/

	sNtlWorldParam	sTmpPrjParam;
	FILE*			pFile;
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;
	RwChar			szFFPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar			szOldPath[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sTmpPrjParam, pFile);
		fclose(pFile);
	}

	strcpy_s(sTmpPrjParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	for(int i = 0; i < sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum; ++i)
	{
		// current field file name
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);

		// name the file for exporting
		strcpy_s(szFFPathName, NTL_MAX_DIR_PATH, _pPathName4Exporting);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "\\");
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, sTmpPrjParam.WorldChar64Buf);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, ".mobj");
		if(fopen_s(&pFile, szFFPathName, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << szFFPathName << ")");
			return FALSE;
		}

		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sTmpPrjParam.WorldChar64Buf);

		// export
		FileMem.Load(sTmpPrjParam.WorldChar64Buf, "rb");

		pFileMemTmp = FileMem.GetDat();
		//pFileMemTmp = GetSPosOfMaterial(FileMem.GetDat());

		for(RwInt32 j = 0; j < sTmpPrjParam.WorldFieldSectorNum * sTmpPrjParam.WorldFieldSectorNum; ++j)
		{			
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorMaterialSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorPrelightsSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorObjectSaveIntoFileFromFileMem(pFile, pFileMemTmp, AW_HEGITHFIELD);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWaterSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShadowSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorEffectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorCullSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorTileTransparencySkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShoreLineSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDecalSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorPlantSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWorldLightSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorOccluderSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorLightObjectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDojoSkipToFileMem(pFileMemTmp);
		}

		FileMem.Free();
		fclose(pFile);

		_chdir("../");
	}

	::SetCurrentDirectory(szOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportMapName(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	/*
	WorldFileFormat - ExportMap
	*/

	sNtlWorldParam	sTmpPrjParam;
	FILE*			pFile;
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;
	RwChar			szFFPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar			szOldPath[NTL_MAX_DIR_PATH]	= {0,};
	RwChar			szWFPFName[32]			= {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sTmpPrjParam, pFile);
		fclose(pFile);
	}

	strcpy_s(sTmpPrjParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	// current field file name
	strcpy_s(szFFPathName, NTL_MAX_DIR_PATH, _pPathName4Exporting);
	strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "\\");
	strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "MapNameChunk");
	FILE* pChunkFile;
	if(fopen_s(&pChunkFile, szFFPathName, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << szFFPathName << ")");
		return FALSE;
	}

	// count
	RwInt32 Num = sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum;
	fwrite(&Num, 1, sizeof(RwInt32), pChunkFile);

	// data
	for(int i = 0; i < sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum; ++i)
	{
		// current field file name
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);
		sprintf_s(szWFPFName, 32, "wfpf%d", i);

		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sTmpPrjParam.WorldChar64Buf);

		// export
		FileMem.Load(szWFPFName, "rb");
		pFileMemTmp = FileMem.GetDat();

		RwChar CurMapName[64];
		CopyMemory(CurMapName, pFileMemTmp, 64);
		TBLIDX CurTblIdx = static_cast<TBLIDX>(atoi(CurMapName));
		fwrite(&CurTblIdx, sizeof(TBLIDX), 1, pChunkFile);

		FileMem.Free();

		_chdir("../");
	}

	fclose(pChunkFile);

	::SetCurrentDirectory(szOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportMapProp(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	/*
	WorldFileFormat - ExportMap
	*/
	
	sNtlWorldParam	sTmpPrjParam;
	FILE*			pFile;
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;
	RwChar			szFFPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar			szOldPath[NTL_MAX_DIR_PATH]	= {0,};
	RwChar			szWFPFName[32]			= {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sTmpPrjParam, pFile);
		fclose(pFile);
	}

	strcpy_s(sTmpPrjParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	// current field file name
	strcpy_s(szFFPathName, NTL_MAX_DIR_PATH, _pPathName4Exporting);
	strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "\\");
	strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "MapPropChunk");
	FILE* pChunkFile;
	if(fopen_s(&pChunkFile, szFFPathName, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << szFFPathName << ")");
		return FALSE;
	}

	// count
	RwInt32 Num = sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum;
	fwrite(&Num, 1, sizeof(RwInt32), pChunkFile);

	RwInt32 iTileNumInField =	(sTmpPrjParam.WorldSectorTileNum * sTmpPrjParam.WorldFieldSectorNum) *
								(sTmpPrjParam.WorldSectorTileNum * sTmpPrjParam.WorldFieldSectorNum);
	
	// data
	for(int i = 0; i < sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum; ++i)
	{
		// current field file name
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);
		sprintf_s(szWFPFName, 32, "wfpf%d", i);

		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sTmpPrjParam.WorldChar64Buf);

		// export
		FileMem.Load(szWFPFName, "rb");
		pFileMemTmp = FileMem.GetDat();
		pFileMemTmp = GetNtlWorldFieldInfo()->FieldNameSkipToFileMem(pFileMemTmp);
		pFileMemTmp = GetNtlWorldFieldInfo()->FieldBitPropSaveIntoFileFromFileMem(pChunkFile, pFileMemTmp, iTileNumInField);
		FileMem.Free();

		_chdir("../");
	}

	fclose(pChunkFile);

	::SetCurrentDirectory(szOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportHeightfield(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	/*
	WorldFileFormat - Export
	Export 처리를 위한 부분이다.
	*/

	sNtlWorldParam	sTmpPrjParam;
	FILE*			pFile;
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;
	RwChar			szFFPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar			szOldPath[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sTmpPrjParam, pFile);
		fclose(pFile);
	}

	strcpy_s(sTmpPrjParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	for(int i = 0; i < sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum; ++i)
	{
		// current field file name
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);

		// name the file for exporting
		strcpy_s(szFFPathName, NTL_MAX_DIR_PATH, _pPathName4Exporting);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "\\");
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, sTmpPrjParam.WorldChar64Buf);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, ".vert");
		if(fopen_s(&pFile, szFFPathName, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << szFFPathName << ")");
			return FALSE;
		}

		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sTmpPrjParam.WorldChar64Buf);

		// export
		FileMem.Load(sTmpPrjParam.WorldChar64Buf, "rb");

		pFileMemTmp = FileMem.GetDat();
		//pFileMemTmp = GetSPosOfMaterial(FileMem.GetDat());

		RwInt32		iWorldSectorVertNum		= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;
		RwV3d*		pvHeightfield			= NTL_NEW RwV3d[iWorldSectorVertNum];
		for(RwInt32 j = 0; j < sTmpPrjParam.WorldFieldSectorNum * sTmpPrjParam.WorldFieldSectorNum; ++j)
		{
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorMaterialSkipToFileMem(pFileMemTmp);
			
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileMemGetVertex(pFileMemTmp, 0, iWorldSectorVertNum, pvHeightfield);
			fwrite(pvHeightfield, sizeof(RwV3d) * iWorldSectorVertNum, 1, pFile);

			pFileMemTmp = GetNtlWorldSectorInfo()->SectorPrelightsSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorObjectSkipToFileMem(pFileMemTmp, AW_HEGITHFIELD);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWaterSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShadowSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorEffectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFileMem(pFileMemTmp);
			// CZ-SS
			//pFileMemTmp = Skip_SS(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorCullSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorTileTransparencySkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShoreLineSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDecalSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorPlantSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWorldLightSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorOccluderSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorLightObjectSkipToFileMem(pFileMemTmp);
			pFileMemTmp	= GetNtlWorldSectorInfo()->SectorDojoSkipToFileMem(pFileMemTmp);
		}
		NTL_ARRAY_DELETE(pvHeightfield);

		FileMem.Free();
		fclose(pFile);

		_chdir("../");
	}

	::SetCurrentDirectory(szOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportHeightfieldPE(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	/*
	WorldFileFormat - ExportMap
	*/

	sNtlWorldParam	sTmpPrjParam;
	FILE*			pFile;
	CFileMem		FileMem;
	//BYTE*			pFileMemTmp;
	RwChar			szFFPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar			szOldPath[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sTmpPrjParam, pFile);
		fclose(pFile);
	}

	strcpy_s(sTmpPrjParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	for(int i = 0; i < sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum; ++i)
	{
		// name the file for exporting
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);

		strcpy_s(szFFPathName, NTL_MAX_DIR_PATH, _pPathName4Exporting);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "\\");
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, sTmpPrjParam.WorldChar64Buf);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, ".peh");
		if(fopen_s(&pFile, szFFPathName, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << szFFPathName << ")");
			return FALSE;
		}

		RwUInt32	uiTemp;
		RwV3d		vTemp;

		//////////////////////////////////////////////////////////////////////////
		// 
		//////////////////////////////////////////////////////////////////////////
		// idx
		uiTemp = i;
		fwrite(&uiTemp, sizeof(RwUInt32), 1, pFile);

		// aabb
		vTemp.z = static_cast<RwReal>(((i / sTmpPrjParam.WorldFieldNum) * sTmpPrjParam.WorldFieldSize) - sTmpPrjParam.WorldSizeHalf);
		vTemp.y = 0.0f;
		vTemp.x = static_cast<RwReal>(((i % sTmpPrjParam.WorldFieldNum) * sTmpPrjParam.WorldFieldSize) - sTmpPrjParam.WorldSizeHalf);
		fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

		vTemp.z = static_cast<RwReal>(vTemp.z + sTmpPrjParam.WorldFieldSize);
		vTemp.y = 0.0f;
		vTemp.x = static_cast<RwReal>(vTemp.x + sTmpPrjParam.WorldFieldSize);
		fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

		//////////////////////////////////////////////////////////////////////////
		// filed data
		//////////////////////////////////////////////////////////////////////////
// 		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);
// 
// 		_chdir(sTmpPrjParam.WorldProjectFolderName);
// 		_chdir("fields");
// 		_chdir(sTmpPrjParam.WorldChar64Buf);
// 
// 		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfpf%d", i);
// 
// 		FileMem.Load(sTmpPrjParam.WorldChar64Buf, "rb");
// 
// 		pFileMemTmp = FileMem.GetDat();
// 
// 		RwChar CurMapName[64];
// 		CopyMemory(CurMapName, pFileMemTmp, 64);
// 		TBLIDX CurTblIdx = static_cast<TBLIDX>(atoi(CurMapName));
// 		fwrite(&CurTblIdx, sizeof(TBLIDX), 1, pFile);
// 
// 		FileMem.Free();

		//////////////////////////////////////////////////////////////////////////
		// sector data
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// swap data
		//////////////////////////////////////////////////////////////////////////
		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("swap");
		_chdir("pe");

		FILE* pTempFile;
		if(fopen_s(&pTempFile, "header.pe", "rb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << "header.pe" << ")");
			return FALSE;
		}

		RwInt32 iTemp;
		fread(&iTemp, sizeof(RwInt32), 1, pTempFile);
		fclose(pTempFile);

		// ChunkSize
		uiTemp = iTemp;
		fwrite(&uiTemp, sizeof(RwUInt32), 1, pFile);

		uiTemp = (sTmpPrjParam.WorldSectorSize / iTemp) * (sTmpPrjParam.WorldSectorSize / iTemp);

		// total count of path tile in a sector
		fwrite(&uiTemp, sizeof(RwUInt32), 1, pFile);

		// error
		RwReal fError = RwRealMAXVAL;
		fwrite(&fError, sizeof(RwReal), 1, pFile);

		vTemp.x = static_cast<RwReal>((i % sTmpPrjParam.WorldFieldNum) * sTmpPrjParam.WorldFieldSize);
		vTemp.z = static_cast<RwReal>((i / sTmpPrjParam.WorldFieldNum) * sTmpPrjParam.WorldFieldSize);

		RwV3d vCount;
		for (vCount.z = vTemp.z; vCount.z < vTemp.z + sTmpPrjParam.WorldFieldSize; vCount.z += sTmpPrjParam.WorldSectorSize)
		{
			for (vCount.x = vTemp.x; vCount.x < vTemp.x + sTmpPrjParam.WorldFieldSize; vCount.x += sTmpPrjParam.WorldSectorSize)
			{
				RwInt32 iWidthNum = (int)(vCount.x / (float)sTmpPrjParam.WorldSectorSize);
				RwInt32 iDepthNum = (int)(vCount.z / (float)sTmpPrjParam.WorldSectorSize); 

				RwInt32 RetIdx = iWidthNum + (iDepthNum * sTmpPrjParam.WorldSectorNum);

				sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "%d_sec.pe", RetIdx);

				FILE* pTempFile;
				if(fopen_s(&pTempFile, sTmpPrjParam.WorldChar64Buf, "rb"))
				{
					DBO_TRACE(FALSE, "file open failed. (" << sTmpPrjParam.WorldChar64Buf << ")");
					return FALSE;
				}

				RwReal* pfHeight = NTL_NEW RwReal [uiTemp];

				fread(pfHeight, sizeof(RwReal) * uiTemp, 1, pTempFile);

				fclose(pTempFile);

				fwrite(pfHeight, sizeof(RwReal) * uiTemp, 1, pFile);

				NTL_DELETE(pfHeight);
			}
		}

		fclose(pFile);
	}

	::SetCurrentDirectory(szOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportMapPropPE(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	/*
	WorldFileFormat - ExportMap
	*/

	sNtlWorldParam	sTmpPrjParam;
	FILE*			pFile;
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;
	RwChar			szFFPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar			szOldPath[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sTmpPrjParam, pFile);
		fclose(pFile);
	}

	strcpy_s(sTmpPrjParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	for(int i = 0; i < sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum; ++i)
	{
		// name the file for exporting
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);

		strcpy_s(szFFPathName, NTL_MAX_DIR_PATH, _pPathName4Exporting);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "\\");
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, sTmpPrjParam.WorldChar64Buf);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, ".pep");
		if(fopen_s(&pFile, szFFPathName, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << szFFPathName << ")");
			return FALSE;
		}

		RwUInt32	uiTemp;
		//////////////////////////////////////////////////////////////////////////
		// 
		//////////////////////////////////////////////////////////////////////////
		// idx
		uiTemp = i;
		fwrite(&uiTemp, sizeof(RwUInt32), 1, pFile);

		//////////////////////////////////////////////////////////////////////////
		// filed data
		//////////////////////////////////////////////////////////////////////////
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);

		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sTmpPrjParam.WorldChar64Buf);

		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfpf%d", i);

		FileMem.Load(sTmpPrjParam.WorldChar64Buf, "rb");

		pFileMemTmp = FileMem.GetDat();

		RwChar CurMapName[64];
		CopyMemory(CurMapName, pFileMemTmp, 64);
		TBLIDX CurTblIdx = static_cast<TBLIDX>(atoi(CurMapName));
		fwrite(&CurTblIdx, sizeof(TBLIDX), 1, pFile);

		DWORD* pBitProp = NTL_NEW DWORD [(sTmpPrjParam.WorldSectorTileNum * 2) * (sTmpPrjParam.WorldSectorTileNum * 2)];

		pFileMemTmp = FileMem.GetDat();
		pFileMemTmp = GetNtlWorldFieldInfo()->FieldNameSkipToFileMem(pFileMemTmp);

		CopyMemory(pBitProp, pFileMemTmp, sizeof(DWORD) * (sTmpPrjParam.WorldSectorTileNum * 2) * (sTmpPrjParam.WorldSectorTileNum * 2));

		FileMem.Free();

		//////////////////////////////////////////////////////////////////////////
		// sector data
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// swap data
		//////////////////////////////////////////////////////////////////////////
		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("swap");
		_chdir("pe");

		FILE* pTempFile;
		if(fopen_s(&pTempFile, "header.pe", "rb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << "header.pe" << ")");
			return FALSE;
		}

		RwInt32 iTemp;
		fread(&iTemp, sizeof(RwInt32), 1, pTempFile);
		fclose(pTempFile);

		// ChunkSize
		uiTemp = iTemp;
		fwrite(&uiTemp, sizeof(RwUInt32), 1, pFile);

		uiTemp = (sTmpPrjParam.WorldFieldSize / iTemp) * (sTmpPrjParam.WorldFieldSize / iTemp);

		// total count of path tile in a field
		fwrite(&uiTemp, sizeof(RwUInt32), 1, pFile);

		// 여기서 위에서 읽은 pBitProp을 fwrite 해준다. Chunk 크기에 따라 갯수를 늘려야 한다.

		for (int e = 0; e < (sTmpPrjParam.WorldSectorTileNum * 2); ++e)
		{
			for (int w = 0; w < sTmpPrjParam.WorldSectorTileSize / iTemp; ++w)
			{
				for (int r = 0; r < (sTmpPrjParam.WorldSectorTileNum * 2); ++r)
				{
					for (int q = 0; q < sTmpPrjParam.WorldSectorTileSize / iTemp; ++q)
					{
						fwrite(&pBitProp[r + (e * (sTmpPrjParam.WorldSectorTileNum * 2))], sizeof(DWORD), 1, pFile);
					}
				}
			}
		}

		NTL_DELETE(pBitProp);
			
		fclose(pFile);
	}

	::SetCurrentDirectory(szOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportPathEngineOutDoor(const char* _pPathNameOfProject, RwUInt32 uiWorldID, const char* _pPathName4Exporting,
														const char* _pWorldName, const char* _pAgency, RwInt32 _iGroupFieldNum, RwInt32 _iOverlapSize)
{
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;

	RwChar			acTempPath[NTL_MAX_DIR_PATH] = {0,};
	RwChar			acOldPath[NTL_MAX_DIR_PATH] = {0,};
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	sNtlWorldParam	sWorldParam;
	FILE*			pFile;
	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sWorldParam, pFile);
		fclose(pFile);
	}
	strcpy_s(sWorldParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	// Swap Header Load
	RwInt32 iTileSize;

	_chdir(sWorldParam.WorldProjectFolderName);
	_chdir("swap");
	_chdir("pe");
	if(fopen_s(&pFile, "header.pe", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "header.pe" << ")");
	}
	else
	{
		fread(&iTileSize, sizeof(RwInt32), 1, pFile);
		fclose(pFile);
	}

	RwInt32 iSectorTileCnt	= (sWorldParam.WorldSectorSize / iTileSize) * (sWorldParam.WorldSectorSize / iTileSize);
	RwInt32 iFieldTileCnt	= iSectorTileCnt * (sWorldParam.WorldFieldSectorNum * sWorldParam.WorldFieldSectorNum);

	CNtlNaviDataMng				NtlNaviDataMgr;	NtlNaviDataMgr.Create(_pPathName4Exporting);
	CNtlNaviWorldOutDoorInfo*	pNtlNaviWorldOutDoorInfo = NtlNaviDataMgr.Create_WorldOutDoor();

	pNtlNaviWorldOutDoorInfo->SetWorldID(uiWorldID);
	pNtlNaviWorldOutDoorInfo->SetWorldName(_pWorldName);
	pNtlNaviWorldOutDoorInfo->SetWorldMinPos((float)-sWorldParam.WorldSizeHalf, (float)-sWorldParam.WorldSizeHalf);
	pNtlNaviWorldOutDoorInfo->SetWorldMaxPos((float)sWorldParam.WorldSizeHalf, (float)sWorldParam.WorldSizeHalf);
	pNtlNaviWorldOutDoorInfo->SetTileSize((float)iTileSize);
	pNtlNaviWorldOutDoorInfo->SetFieldSize((float)sWorldParam.WorldFieldSize);
	pNtlNaviWorldOutDoorInfo->SetCrossFieldCntOfGroup(_iGroupFieldNum);
	pNtlNaviWorldOutDoorInfo->SetOverlapSize((RwReal)_iOverlapSize);

	
	std::vector<std::string> vecString;
	StringTokenize(vecString, std::string(_pAgency), std::string(","));
	for (int i = 0; i < (int)vecString.size(); ++i)
	{
		pNtlNaviWorldOutDoorInfo->AttachAgency(::atof(vecString.at(i).c_str()));
	}

	for (int iZ = 0; iZ < sWorldParam.WorldFieldNum; iZ += _iGroupFieldNum)
	{
		for (int iX = 0; iX < sWorldParam.WorldFieldNum; iX += _iGroupFieldNum)
		{
			CNtlNaviGroupOutDoorInfo*	pNtlNaviGroupOutDoorInfo	= NtlNaviDataMgr.Create_GroupOutDoor();
			RwInt32						iGroupIdx					= ((iZ / _iGroupFieldNum) * (sWorldParam.WorldFieldNum / _iGroupFieldNum)) + (iX / _iGroupFieldNum);
			
			pNtlNaviGroupOutDoorInfo->SetGroupID(iGroupIdx);
			pNtlNaviGroupOutDoorInfo->SetInvalidHeightVal(RwRealMAXVAL);
			pNtlNaviGroupOutDoorInfo->SetFieldCntOfGroup(_iGroupFieldNum * _iGroupFieldNum);
			pNtlNaviGroupOutDoorInfo->SetTileCntOfSector(iSectorTileCnt);

			for (int iGZ = iZ; iGZ < iZ + _iGroupFieldNum; ++iGZ)
			{
				for (int iGX = iX; iGX < iX + _iGroupFieldNum; ++iGX)
				{
					CNtlNaviPropOutDoorInfo*	pNtlNaviPropOutDoorInfo	= NtlNaviDataMgr.Create_PropOutDoor();
					RwInt32						iFieldIdx				= iGZ * sWorldParam.WorldFieldNum + iGX;

					pNtlNaviPropOutDoorInfo->SetFieldID(iFieldIdx);
					pNtlNaviPropOutDoorInfo->SetTileCntOfField(iFieldTileCnt);
					
					// FIELD FILE OPEN
					sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d", iFieldIdx);
					_chdir(sWorldParam.WorldProjectFolderName);
					_chdir("fields");
					_chdir(sWorldParam.WorldChar64Buf);
					sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfpf%d", iFieldIdx);

					FileMem.Load(sWorldParam.WorldChar64Buf, "rb");
					
					pFileMemTmp = FileMem.GetDat();

					// NAME
					RwChar CurMapName[64];
					CopyMemory(CurMapName, pFileMemTmp, 64);
					pNtlNaviPropOutDoorInfo->SetFieldProp(atoi(CurMapName));
					pFileMemTmp = GetNtlWorldFieldInfo()->FieldNameSkipToFileMem(pFileMemTmp);

					// TILE PROP
					DWORD* pBitProp = NTL_NEW DWORD [(sWorldParam.WorldSectorTileNum * sWorldParam.WorldSectorTileNum) * (sWorldParam.WorldFieldSectorNum * sWorldParam.WorldFieldSectorNum)];
					CopyMemory(pBitProp, pFileMemTmp, sizeof(DWORD) * (sWorldParam.WorldSectorTileNum * sWorldParam.WorldSectorTileNum) * (sWorldParam.WorldFieldSectorNum * sWorldParam.WorldFieldSectorNum));

					DWORD*	pTileProp	= NTL_NEW DWORD [iFieldTileCnt];
					RwInt32 iTileCnt	= 0;
					for (int e = 0; e < (sWorldParam.WorldSectorTileNum * sWorldParam.WorldFieldSectorNum); ++e)
					{
						for (int w = 0; w < sWorldParam.WorldSectorTileSize / iTileSize; ++w)
						{
							for (int r = 0; r < (sWorldParam.WorldSectorTileNum * sWorldParam.WorldFieldSectorNum); ++r)
							{
								for (int q = 0; q < sWorldParam.WorldSectorTileSize / iTileSize; ++q)
								{
									pTileProp[iTileCnt++] = pBitProp[r + (e * (sWorldParam.WorldSectorTileNum * sWorldParam.WorldFieldSectorNum))];
								}
							}
						}
					}
					NTL_ARRAY_DELETE(pBitProp);
					pNtlNaviPropOutDoorInfo->SetTileProp((unsigned int*)pTileProp);

					// FIELD FILE FREE
					FileMem.Free();

					// SECTOR FILE OPEN
					sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d", iFieldIdx);
					_chdir(sWorldParam.WorldProjectFolderName);
					_chdir("fields");
					_chdir(sWorldParam.WorldChar64Buf);
					sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d",iFieldIdx);

					FileMem.Load(sWorldParam.WorldChar64Buf, "rb");

					pFileMemTmp = FileMem.GetDat();

					for(RwInt32 iSectorCnt = 0; iSectorCnt < sWorldParam.WorldFieldSectorNum * sWorldParam.WorldFieldSectorNum; ++iSectorCnt)
					{
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorMaterialSkipToFileMem(pFileMemTmp);
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileMem(pFileMemTmp);
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorPrelightsSkipToFileMem(pFileMemTmp);

						// Doodads
						RwInt32 iNumDoodads;
						CopyMemory(&iNumDoodads, pFileMemTmp, sizeof(RwInt32));
						pFileMemTmp += sizeof(RwInt32);

						for (int iDoodadCnt = 0; iDoodadCnt < iNumDoodads; ++iDoodadCnt)
						{
							RwChar	acDoodadName[128]; // Obj. Name
							RwV3d	avDoodadTransform[3]; // Scale, Rotation, Translation

							if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
							{
								CopyMemory(acDoodadName, pFileMemTmp, sizeof(RwChar) * 128);
								pFileMemTmp += sizeof(RwChar) * 128;

							}
							else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
							{
								RwUInt32	uiLength = 0;

								CopyMemory(&uiLength, pFileMemTmp, sizeof(RwUInt32));			
								pFileMemTmp  += sizeof(RwUInt32);

								CopyMemory(acDoodadName, pFileMemTmp , (sizeof(RwChar) * uiLength));
								pFileMemTmp  += (sizeof(RwChar) * uiLength);
								acDoodadName[uiLength] = '\0';
							}
							
							CopyMemory(avDoodadTransform, pFileMemTmp, sizeof(RwV3d) * 3);
							pFileMemTmp += sizeof(RwV3d) * 3;

							RwUInt32 uiObjectType;
							CopyMemory(&uiObjectType, pFileMemTmp, sizeof(RwUInt32));
							pFileMemTmp += sizeof(RwUInt32);

							if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
								uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
							{
								pFileMemTmp = CNtlPLObjectTypeHasProperty::SkipToFileMem(pFileMemTmp);
							}

							pFileMemTmp += (sizeof(RwUInt32)); // trigger obj serial id

							if (sWorldParam.WorldType == 1)
							{
								pFileMemTmp += (sizeof(RwUInt32)); // object serial id
							}

							pFileMemTmp += (sizeof(DWORD)); // Obj. name index
							pFileMemTmp += (sizeof(RwReal)); // visibility distance

							if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
							{
								pFileMemTmp += (sizeof(RwBool)); // enable trigger
								pFileMemTmp += (sizeof(RwBool)); // enable selection
								pFileMemTmp += (sizeof(RwBool)); // enable alterable
								pFileMemTmp += (sizeof(RwBool)); // enable portal
								pFileMemTmp += (sizeof(RwBool)); // enable path engine collision
								pFileMemTmp += (sizeof(RwBool)); // enable shadowprop
							}
							else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
							{
								pFileMemTmp += (sizeof(RwUInt32));
							}
							
							// PS Map Size
							pFileMemTmp = CNtlPLObject::SkipPSMapToFileMem(pFileMemTmp);
							
							// milepost
							RwBool EnableFlg;
							CopyMemory(&EnableFlg, pFileMemTmp, sizeof(RwBool));
							pFileMemTmp += sizeof(RwBool);

							if(EnableFlg)
							{
								pFileMemTmp += sizeof(RwUInt32);
							}

							// attach light object skip
							pFileMemTmp = CNtlPLObject::SkipLightObjectAttachDataToFileMem(pFileMemTmp);

							if (sWorldParam.WorldType == 1)
							{
								pFileMemTmp = CNtlPLObject::SkipLightmapToFileMem(pFileMemTmp);
							}

							// DATA ATTACH
							WIN32_FIND_DATA findData;
							HANDLE			hfindFile;

							// NaviMesh Find
							RwChar			acNaviFind[NTL_MAX_DIR_PATH];
							sprintf_s(acNaviFind, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s_PE_Nav_*.dff",
										dGET_WORLD_PARAM()->CurWorkingFolderName, acDoodadName);
							
							hfindFile = ::FindFirstFile(acNaviFind, &findData);
							if(hfindFile != INVALID_HANDLE_VALUE)
							{
								do
								{
									if (findData.cFileName[0] != '.')
									{
										if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
										{
											continue;
										}
										else
										{								
											pNtlNaviGroupOutDoorInfo->AttachNaviMesh(acDoodadName,
																					avDoodadTransform[2].x,
																					avDoodadTransform[2].y,
																					avDoodadTransform[2].z,
																					avDoodadTransform[0].x,
																					avDoodadTransform[0].y,
																					avDoodadTransform[0].z,
																					avDoodadTransform[1].y);
											break;
										}
									}
								} while(::FindNextFile(hfindFile, &findData));

								::FindClose(hfindFile);
							}

							// ObstMesh Find
							RwChar			acObstFind[NTL_MAX_DIR_PATH];
							sprintf_s(acObstFind, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s_PE_Obs_*.dff",
								dGET_WORLD_PARAM()->CurWorkingFolderName, acDoodadName);

							hfindFile = ::FindFirstFile(acObstFind, &findData);
							if(hfindFile != INVALID_HANDLE_VALUE)
							{
								do
								{
									if (findData.cFileName[0] != '.')
									{
										if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
										{
											continue;
										}
										else
										{								
											pNtlNaviGroupOutDoorInfo->AttachObsMesh(acDoodadName,
												avDoodadTransform[2].x,
												avDoodadTransform[2].y,
												avDoodadTransform[2].z,
												avDoodadTransform[0].x,
												avDoodadTransform[0].y,
												avDoodadTransform[0].z,
												avDoodadTransform[1].y);
											break;
										}
									}
								} while(::FindNextFile(hfindFile, &findData));

								::FindClose(hfindFile);
							}
						}
						
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorWaterSkipToFileMem(pFileMemTmp);
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorShadowSkipToFileMem(pFileMemTmp);
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorEffectSkipToFileMem(pFileMemTmp);
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFileMem(pFileMemTmp);
						// CZ-SS
						//pFileMemTmp = Skip_SS(pFileMemTmp);
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorCullSkipToFileMem(pFileMemTmp);
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorTileTransparencySkipToFileMem(pFileMemTmp);
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorShoreLineSkipToFileMem(pFileMemTmp);
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorDecalSkipToFileMem(pFileMemTmp);
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorPlantSkipToFileMem(pFileMemTmp);
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorWorldLightSkipToFileMem(pFileMemTmp);
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorOccluderSkipToFileMem(pFileMemTmp);
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSkipToFileMem(pFileMemTmp);
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorLightObjectSkipToFileMem(pFileMemTmp);
						pFileMemTmp = GetNtlWorldSectorInfo()->SectorDojoSkipToFileMem(pFileMemTmp);
					}

					FileMem.Free();

					// SWAP FILE OPEN
					_chdir(sWorldParam.WorldProjectFolderName);
					_chdir("swap");
					_chdir("pe");

					pFileMemTmp = FileMem.GetDat();

					RwV3d vTemp;
					vTemp.x = static_cast<RwReal>((iFieldIdx % sWorldParam.WorldFieldNum) * sWorldParam.WorldFieldSize);
					vTemp.z = static_cast<RwReal>((iFieldIdx / sWorldParam.WorldFieldNum) * sWorldParam.WorldFieldSize);

					RwV3d	vCount;
					RwInt32 iCount = 0;
					for (vCount.z = vTemp.z; vCount.z < vTemp.z + sWorldParam.WorldFieldSize; vCount.z += sWorldParam.WorldSectorSize)
					{
						for (vCount.x = vTemp.x; vCount.x < vTemp.x + sWorldParam.WorldFieldSize; vCount.x += sWorldParam.WorldSectorSize)
						{
							RwInt32 iWidthNum = (int)(vCount.x / (float)sWorldParam.WorldSectorSize);
							RwInt32 iDepthNum = (int)(vCount.z / (float)sWorldParam.WorldSectorSize); 

							RwInt32 RetIdx = iWidthNum + (iDepthNum * sWorldParam.WorldSectorNum);

							sprintf_s(sWorldParam.WorldChar64Buf, 64, "%d_sec.pe", RetIdx);

							FILE* pTempFile;
							if(fopen_s(&pTempFile, sWorldParam.WorldChar64Buf, "rb"))
							{
								DBO_TRACE(FALSE, "file open failed. (" << sWorldParam.WorldChar64Buf << ")");
							}
							else
							{
								RwReal* pfHeight = NTL_NEW RwReal [iSectorTileCnt];
								fread(pfHeight, sizeof(RwReal) * iSectorTileCnt, 1, pTempFile);
								fclose(pTempFile);

								pNtlNaviGroupOutDoorInfo->AttachSectorHeightList(iFieldIdx, iCount++, pfHeight);
							}
						}
					}				
				}
			}
			NtlNaviDataMgr.Flush();
		}
	}

	NtlNaviDataMgr.Delete();
	::SetCurrentDirectory(acOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportPathEngineInDoor(const char* _pPathNameOfProject, RwUInt32 uiWorldID, const char* _pPathName4Exporting,
														const char* _pWorldName, const char* _pAgency)
{
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;

	RwChar			acTempPath[NTL_MAX_DIR_PATH] = {0,};
	RwChar			acOldPath[NTL_MAX_DIR_PATH] = {0,};
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	sNtlWorldParam	sWorldParam;
	FILE*			pFile;
	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << sWorldParam.WorldChar64Buf << ")");		
	}
	else
	{
		LoadFileWorldState(&sWorldParam, pFile);
		fclose(pFile);
	}
	strcpy_s(sWorldParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	CNtlNaviDataMng				NtlNaviDataMgr;	NtlNaviDataMgr.Create(_pPathName4Exporting);
	CNtlNaviWorldInDoorInfo*	pNtlNaviWorldInDoorInfo = NtlNaviDataMgr.Create_WorldInDoor();
	
	pNtlNaviWorldInDoorInfo->SetWorldID(uiWorldID);
	pNtlNaviWorldInDoorInfo->SetWorldName(_pWorldName);
	pNtlNaviWorldInDoorInfo->SetWorldMinPos((float)-sWorldParam.WorldSizeHalf, (float)-sWorldParam.WorldSizeHalf);
	pNtlNaviWorldInDoorInfo->SetWorldMaxPos((float)sWorldParam.WorldSizeHalf, (float)sWorldParam.WorldSizeHalf);

	pNtlNaviWorldInDoorInfo->SetBlockSize((float)sWorldParam.WorldBlockSize);

	std::vector<std::string> vecString;
	StringTokenize(vecString, std::string(_pAgency), std::string(","));
	for (int i = 0; i < (int)vecString.size(); ++i)
	{
		pNtlNaviWorldInDoorInfo->AttachAgency(::atof(vecString.at(i).c_str()));
	}

	CNtlNaviGroupInDoorInfo* pNtlNaviGroupInDoorInfo = NtlNaviDataMgr.Create_GroupInDoor();
	pNtlNaviGroupInDoorInfo->SetModelName(sWorldParam.WorldBSP);

	struct SGamePropertyData
	{
		RwUInt32	uiShape;
		RwV3d		vPos;
		RwV3d		vSize;
		RwUInt32	uiProperty;
	};
	std::vector<SGamePropertyData> vecGamePropertyData;
	for (int iFieldIdx = 0; iFieldIdx < sWorldParam.WorldFieldNum * sWorldParam.WorldFieldNum; ++iFieldIdx)
	{
		sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d", iFieldIdx);
		_chdir(sWorldParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sWorldParam.WorldChar64Buf);
		sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d",iFieldIdx);

		FileMem.Load(sWorldParam.WorldChar64Buf, "rb");

		pFileMemTmp = FileMem.GetDat();
		for(RwInt32 iSectorCnt = 0; iSectorCnt < sWorldParam.WorldFieldSectorNum * sWorldParam.WorldFieldSectorNum; ++iSectorCnt)
		{			
			// Doodads
			RwInt32 iNumDoodads;
			CopyMemory(&iNumDoodads, pFileMemTmp, sizeof(RwInt32));
			pFileMemTmp += sizeof(RwInt32);

			for (int iDoodadCnt = 0; iDoodadCnt < iNumDoodads; ++iDoodadCnt)
			{
				RwChar	acDoodadName[128]; // Obj. Name
				RwV3d	avDoodadTransform[3]; // Scale, Rotation, Translation

				if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
				{
					CopyMemory(acDoodadName, pFileMemTmp, sizeof(RwChar) * 128);
					pFileMemTmp += sizeof(RwChar) * 128;

				}
				else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
				{
					RwUInt32	uiLength = 0;

					CopyMemory(&uiLength, pFileMemTmp, sizeof(RwUInt32));			
					pFileMemTmp  += sizeof(RwUInt32);

					CopyMemory(acDoodadName, pFileMemTmp , (sizeof(RwChar) * uiLength));
					pFileMemTmp  += (sizeof(RwChar) * uiLength);
					acDoodadName[uiLength] = '\0';
				}
				
				CopyMemory(avDoodadTransform, pFileMemTmp, sizeof(RwV3d) * 3);
				pFileMemTmp += sizeof(RwV3d) * 3;

				RwUInt32 uiObjectType;
				CopyMemory(&uiObjectType, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += sizeof(RwUInt32);

				if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
					uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
				{
					pFileMemTmp = CNtlPLObjectTypeHasProperty::SkipToFileMem(pFileMemTmp);
				}

				pFileMemTmp += (sizeof(RwUInt32)); // trigger obj serial id

				if (sWorldParam.WorldType == 1)
				{
					pFileMemTmp += (sizeof(RwUInt32)); // object serial id
				}

				pFileMemTmp += (sizeof(DWORD)); // Obj. name index
				pFileMemTmp += (sizeof(RwReal)); // visibility distance

				if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
				{					
					pFileMemTmp += (sizeof(RwBool)); // enable trigger
					pFileMemTmp += (sizeof(RwBool)); // enable selection
					pFileMemTmp += (sizeof(RwBool)); // enable alterable
					pFileMemTmp += (sizeof(RwBool)); // enable portal
					pFileMemTmp += (sizeof(RwBool)); // enable path engine collision
					pFileMemTmp += (sizeof(RwBool)); // enable shadowprop
				}
				else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
				{
					pFileMemTmp += (sizeof(RwUInt32));
				}

				// PS Map Size
				pFileMemTmp = CNtlPLObject::SkipPSMapToFileMem(pFileMemTmp);

				// milepost
				RwBool EnableFlg;
				CopyMemory(&EnableFlg, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += sizeof(RwBool);

				if(EnableFlg)
				{
					pFileMemTmp += sizeof(RwUInt32);
				}

				// attach light object skip
				pFileMemTmp = CNtlPLObject::SkipLightObjectAttachDataToFileMem(pFileMemTmp);

				if (sWorldParam.WorldType == 1)
				{
					pFileMemTmp = CNtlPLObject::SkipLightmapToFileMem(pFileMemTmp);
				}

				// DATA ATTACH
				WIN32_FIND_DATA findData;
				HANDLE			hfindFile;

				// NaviMesh Find
				RwChar			acNaviFind[NTL_MAX_DIR_PATH];
				sprintf_s(acNaviFind, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s_PE_Nav_*.dff",
					dGET_WORLD_PARAM()->CurWorkingFolderName, acDoodadName);

				hfindFile = ::FindFirstFile(acNaviFind, &findData);
				if(hfindFile != INVALID_HANDLE_VALUE)
				{
					do
					{
						if (findData.cFileName[0] != '.')
						{
							if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
							{
								continue;
							}
							else
							{								
								pNtlNaviGroupInDoorInfo->AttachNaviMesh(acDoodadName,
									avDoodadTransform[2].x,
									avDoodadTransform[2].y,
									avDoodadTransform[2].z,
									avDoodadTransform[0].x,
									avDoodadTransform[0].y,
									avDoodadTransform[0].z,
									avDoodadTransform[1].y);
								break;
							}
						}
					} while(::FindNextFile(hfindFile, &findData));

					::FindClose(hfindFile);
				}

				// ObstMesh Find
				RwChar			acObstFind[NTL_MAX_DIR_PATH];
				sprintf_s(acObstFind, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s_PE_Obs_*.dff",
					dGET_WORLD_PARAM()->CurWorkingFolderName, acDoodadName);

				hfindFile = ::FindFirstFile(acObstFind, &findData);
				if(hfindFile != INVALID_HANDLE_VALUE)
				{
					do
					{
						if (findData.cFileName[0] != '.')
						{
							if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
							{
								continue;
							}
							else
							{								
								pNtlNaviGroupInDoorInfo->AttachObsMesh(acDoodadName,
									avDoodadTransform[2].x,
									avDoodadTransform[2].y,
									avDoodadTransform[2].z,
									avDoodadTransform[0].x,
									avDoodadTransform[0].y,
									avDoodadTransform[0].z,
									avDoodadTransform[1].y);
								break;
							}
						}
					} while(::FindNextFile(hfindFile, &findData));

					::FindClose(hfindFile);
				}
			}

			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWaterSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShadowSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorEffectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorCullSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorTileTransparencySkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShoreLineSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDecalSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorPlantSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWorldLightSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorOccluderSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorLightObjectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDecalVolumeSkipToFileMem(pFileMemTmp);

			RwInt32 iGamePropertyNum;
			CopyMemory(&iGamePropertyNum, pFileMemTmp, sizeof(RwInt32));
			pFileMemTmp += sizeof(RwInt32);
			for (int i = 0; i < iGamePropertyNum; ++i)
			{
				SGamePropertyData sGamePropertyData;

				CopyMemory(&sGamePropertyData.uiShape, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += sizeof(RwUInt32);

				CopyMemory(&sGamePropertyData.vPos, pFileMemTmp, sizeof(RwV3d));
				pFileMemTmp += sizeof(RwV3d);

				CopyMemory(&sGamePropertyData.vSize, pFileMemTmp, sizeof(RwV3d));
				pFileMemTmp += sizeof(RwV3d);

				CopyMemory(&sGamePropertyData.uiProperty, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += sizeof(RwUInt32);

				vecGamePropertyData.push_back(sGamePropertyData);
			}

			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDojoSkipToFileMem(pFileMemTmp);
		}

		FileMem.Free();
	}

	_chdir(sWorldParam.WorldProjectFolderName);
	FileMem.Load("wfp", "rb");

	for (int iBlockIdx = 0; iBlockIdx < sWorldParam.WorldBlockNum * sWorldParam.WorldBlockNum; ++iBlockIdx)
	{
		CNtlNaviPropInDoorInfo* pNtlNaviPropInDoorInfo = NtlNaviDataMgr.Create_PropInDoor();

		pFileMemTmp = FileMem.GetDat();

		char acMapName[64];
		CopyMemory(acMapName, pFileMemTmp, 64);
		TBLIDX tlbIdx = static_cast<TBLIDX>(atoi(acMapName));
		pFileMemTmp = GetNtlWorldFieldInfo()->FieldNameSkipToFileMem(pFileMemTmp);

		//pFileMemTmp = Skip_BitProp(pFileMemTmp);
		pFileMemTmp = GetNtlWorldFieldInfo()->FieldFogSkipToFileMem(pFileMemTmp);
		pFileMemTmp = GetNtlWorldFieldInfo()->FieldSkySkipToFileMem(pFileMemTmp);
		pFileMemTmp = GetNtlWorldFieldInfo()->FieldRealLightSkipToFileMem(pFileMemTmp);
		pFileMemTmp = GetNtlWorldFieldInfo()->FieldBGMSkipToFileMem(pFileMemTmp);
		pFileMemTmp = GetNtlWorldFieldInfo()->FieldBloomSkipToFileMem(pFileMemTmp);
		pFileMemTmp = GetNtlWorldFieldInfo()->FieldPlanetSkipToFileMem(pFileMemTmp);
		pFileMemTmp = GetNtlWorldFieldInfo()->FieldSpecularSkipToFileMem(pFileMemTmp);
		//pFileMemTmp = Skip_WorldMaterial(pFileMemTmp);
		pFileMemTmp = GetNtlWorldFieldInfo()->FieldColorSkipToFileMem(pFileMemTmp);
		pFileMemTmp = GetNtlWorldFieldInfo()->FieldWeatherSkipToFileMem(pFileMemTmp);
		pFileMemTmp = GetNtlWorldFieldInfo()->FieldHeatHazeSkipToFileMem(pFileMemTmp);

		RwUInt32 uiGameProperty;
		CopyMemory(&uiGameProperty, pFileMemTmp, sizeof(RwUInt32));
		pFileMemTmp += sizeof(RwUInt32);

		pNtlNaviPropInDoorInfo->SetBlockID(iBlockIdx);
		pNtlNaviPropInDoorInfo->SetBlockProp(tlbIdx);		
		pNtlNaviPropInDoorInfo->SetGameProp(uiGameProperty);	

		RwV3d vBlockPos;
		vBlockPos.x = static_cast<RwReal>((iBlockIdx % sWorldParam.WorldBlockNum) * sWorldParam.WorldBlockSize - sWorldParam.WorldSizeHalf);
		vBlockPos.y = 0.0f;
		vBlockPos.z = static_cast<RwReal>((iBlockIdx / sWorldParam.WorldBlockNum) * sWorldParam.WorldBlockSize - sWorldParam.WorldSizeHalf);

		RwBBox bboxBlock;
		bboxBlock.inf	= vBlockPos;
		bboxBlock.sup.x	= static_cast<RwReal>(vBlockPos.x + sWorldParam.WorldBlockSize);
		bboxBlock.sup.y = 0.0f;
		bboxBlock.sup.z	= static_cast<RwReal>(vBlockPos.z + sWorldParam.WorldBlockSize);

		std::vector<SGamePropertyData>::iterator it = vecGamePropertyData.begin();
		while (it != vecGamePropertyData.end())
		{
			SGamePropertyData& sGamePropertyData = *it;

			if (sGamePropertyData.vPos.x > bboxBlock.inf.x && sGamePropertyData.vPos.z > bboxBlock.inf.z &&
				sGamePropertyData.vPos.x < bboxBlock.sup.x && sGamePropertyData.vPos.z < bboxBlock.sup.z)
			{
				switch (sGamePropertyData.uiProperty)
				{
				case ENTL_PL_GAME_PROPERTY_SHAPE_CIRCLE:
					pNtlNaviPropInDoorInfo->AttachEntityInfo_Sphere(sGamePropertyData.vPos.x, sGamePropertyData.vPos.y, sGamePropertyData.vPos.z, sGamePropertyData.vSize.x);
					break;
				case ENTL_PL_GAME_PROPERTY_SHAPE_PLANE:
					pNtlNaviPropInDoorInfo->AttachEntityInfo_Plane(sGamePropertyData.vPos.x, sGamePropertyData.vPos.y, sGamePropertyData.vPos.z, sGamePropertyData.vSize.x * 2.0f, sGamePropertyData.vSize.z * 2.0f); // Half Size이기 때문에
					break;
				}

				it = vecGamePropertyData.erase(it);
				continue;
			}
			++it;
		}
	}

	FileMem.Free();

	NtlNaviDataMgr.Flush();
	NtlNaviDataMgr.Delete();
	::SetCurrentDirectory(acOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportTriggerInfoIndoor(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;

	RwChar			acTempPath[NTL_MAX_DIR_PATH] = {0,};
	RwChar			acOldPath[NTL_MAX_DIR_PATH] = {0,};
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	sNtlWorldParam	sWorldParam;
	FILE*			pFile;
	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << sWorldParam.WorldChar64Buf << ")");		
	}
	else
	{
		LoadFileWorldState(&sWorldParam, pFile);
		fclose(pFile);
	}
	strcpy_s(sWorldParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	if(::fopen_s(&pFile, _pPathName4Exporting, "wt") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << _pPathName4Exporting << ")");
	}
	
	for (int iFieldIdx = 0; iFieldIdx < sWorldParam.WorldFieldNum * sWorldParam.WorldFieldNum; ++iFieldIdx)
	{
		sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d", iFieldIdx);
		_chdir(sWorldParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sWorldParam.WorldChar64Buf);
		sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d",iFieldIdx);

		FileMem.Load(sWorldParam.WorldChar64Buf, "rb");

		::SetCurrentDirectory(acOldPath);

		pFileMemTmp = FileMem.GetDat();
		for(RwInt32 iSectorCnt = 0; iSectorCnt < sWorldParam.WorldFieldSectorNum * sWorldParam.WorldFieldSectorNum; ++iSectorCnt)
		{			
			// Doodads
			RwInt32 iNumDoodads;
			CopyMemory(&iNumDoodads, pFileMemTmp, sizeof(RwInt32));
			pFileMemTmp += sizeof(RwInt32);

			for (int iDoodadCnt = 0; iDoodadCnt < iNumDoodads; ++iDoodadCnt)
			{
				RwChar	acDoodadName[128];
				RwV3d	avDoodadTransform[3]; // Scale, Rotation, Translation

				// Use : Name
				if (dNTL_WORLD_VERSION_COMPARE(sWorldParam.WorldVer, dNTL_WORLD_VERSION_OLD))
				{
					CopyMemory(acDoodadName, pFileMemTmp, sizeof(RwChar) * 128);
					pFileMemTmp += (sizeof(RwChar) * 128);

				}
				else if (dNTL_WORLD_VERSION_COMPARE(sWorldParam.WorldVer, dNTL_WORLD_VERSION))
				{
					RwUInt32	uiLength = 0;

					CopyMemory(&uiLength, pFileMemTmp, sizeof(RwUInt32));			
					pFileMemTmp  += sizeof(RwUInt32);

					CopyMemory(acDoodadName, pFileMemTmp , (sizeof(RwChar) * uiLength));
					pFileMemTmp  += (sizeof(RwChar) * uiLength);
					acDoodadName[uiLength] = '\0';
				}

				CopyMemory(avDoodadTransform, pFileMemTmp, sizeof(RwV3d) * 3);
				pFileMemTmp += sizeof(RwV3d) * 3;

				RwUInt32 uiObjectType;
				CopyMemory(&uiObjectType, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += sizeof(RwUInt32);

				if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
					uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
				{
					pFileMemTmp = CNtlPLObjectTypeHasProperty::SkipToFileMem(pFileMemTmp);
				}

				SPLEntityCreateParam sPLEntityCreateParam;
				sPLEntityCreateParam.pPos = &avDoodadTransform[2];

				CNtlPLEntity* pNtlPLObject = static_cast<CNtlPLEntity*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, acDoodadName, &sPLEntityCreateParam));
				DBO_ASSERT(pNtlPLObject, "Entity create failed.");

				RwBBox bboxAABB = pNtlPLObject->GetTriggerAABBInfo(avDoodadTransform[2], avDoodadTransform[1], avDoodadTransform[0]);

				GetSceneManager()->DeleteEntity(static_cast<CNtlPLEntity*>(pNtlPLObject));

				RwUInt32 uiTriggerID;
				CopyMemory(&uiTriggerID, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += (sizeof(RwUInt32));		 // trigger obj serial id

				if (sWorldParam.WorldType == 1)
				{
					pFileMemTmp += (sizeof(RwUInt32));		// Object  serial id	
				}

				DWORD dwObjectNameIdx;
				CopyMemory(&dwObjectNameIdx, pFileMemTmp, sizeof(DWORD));
				pFileMemTmp += (sizeof(DWORD)); 		// Obj. name index

				pFileMemTmp += (sizeof(RwReal));		// visibility distance

				RwBool bEnableTrigger;
				RwBool bEnableSelection;
				RwBool bEnableAlterable;
				RwBool bEnablePortal;

				if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
				{
					CopyMemory(&bEnableTrigger, pFileMemTmp, sizeof(RwBool));
					pFileMemTmp += (sizeof(RwBool));		// enable trigger


					CopyMemory(&bEnableSelection, pFileMemTmp, sizeof(RwBool));
					pFileMemTmp += (sizeof(RwBool));		// enable selection


					CopyMemory(&bEnableAlterable, pFileMemTmp, sizeof(RwBool));
					pFileMemTmp += (sizeof(RwBool));		// enable alterable


					CopyMemory(&bEnablePortal, pFileMemTmp, sizeof(RwBool));
					pFileMemTmp += (sizeof(RwBool));		// enable portal

					pFileMemTmp += (sizeof(RwBool));		// enable path engine collision
					pFileMemTmp += (sizeof(RwBool));		// enable shadowprop
				}
				else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
				{
					RwUInt32 uiFlags;
					CopyMemory(&uiFlags, pFileMemTmp, sizeof(RwUInt32));
					pFileMemTmp += (sizeof(RwUInt32));

					bEnableTrigger = (uiFlags & NTL_PLEFLAG_TRIGGER);
					bEnableSelection = (uiFlags & NTL_PLEFLAG_SELECTION);
					bEnableAlterable = (uiFlags & NTL_PLEFLAG_ALTERABLE);
					bEnablePortal = (uiFlags & NTL_PLEFLAG_PORTAL);
				}

				if(uiTriggerID != 0xffffffff && bEnableTrigger)
				{
					RwV3d		vTempScale	= { 1.0f, 1.0f, 1.0f };
					RwV3d		vTempRot	= avDoodadTransform[1];
					RwV3d		vTempPos	= { 0.0f, 0.0f, 0.0f };
					RwV3d		vTempRotResult;
					RwMatrix	matTemp;

					CNtlMath::MathGetMatrix(&matTemp, &vTempScale, &vTempRot, &vTempPos);
					RwV3dTransformVector(&vTempRotResult, &CNtlPLGlobal::m_vZAxisV3, &matTemp);

					vTempRotResult.y = 0.0f;
					RwV3dNormalize(&vTempRotResult, &vTempRotResult);

					fprintf_s(pFile, "%d\t%u\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%d\t%d\t%d\t%s\t\n",
						uiTriggerID,
						dwObjectNameIdx,
						avDoodadTransform[2].x, avDoodadTransform[2].y, avDoodadTransform[2].z,
						vTempRotResult.x, vTempRotResult.y, vTempRotResult.z,						
						bboxAABB.inf.x, bboxAABB.inf.y, bboxAABB.inf.z,
						bboxAABB.sup.x, bboxAABB.sup.y, bboxAABB.sup.z,
						CLAMP(bEnableAlterable, 0, 1),
						CLAMP(bEnableSelection, 0, 1),
						CLAMP(bEnablePortal, 0, 1),
						acDoodadName);
				}

				// PS Map Size
				pFileMemTmp = CNtlPLObject::SkipPSMapToFileMem(pFileMemTmp);

				// milepost
				RwBool EnableFlg;
				CopyMemory(&EnableFlg, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += sizeof(RwBool);

				if(EnableFlg)
				{
					pFileMemTmp += sizeof(RwUInt32);
				}

				// attach light object skip
				pFileMemTmp = CNtlPLObject::SkipLightObjectAttachDataToFileMem(pFileMemTmp);

				if (sWorldParam.WorldType == 1)
				{
					pFileMemTmp = CNtlPLObject::SkipLightmapToFileMem(pFileMemTmp);
				}
			}

			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWaterSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShadowSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorEffectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorCullSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorTileTransparencySkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShoreLineSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDecalSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorPlantSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWorldLightSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorOccluderSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorLightObjectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDecalVolumeSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorGamePropertySkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDojoSkipToFileMem(pFileMemTmp);
		}

		FileMem.Free();
	}

	if (pFile)
	{
		fclose(pFile);
	}

	::SetCurrentDirectory(acOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportTriggerInfoOutdoor(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;

	RwChar			acTempPath[NTL_MAX_DIR_PATH] = {0,};
	RwChar			acOldPath[NTL_MAX_DIR_PATH] = {0,};
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	sNtlWorldParam	sWorldParam;
	FILE*			pFile;
	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << sWorldParam.WorldChar64Buf << ")");		
	}
	else
	{
		LoadFileWorldState(&sWorldParam, pFile);
		fclose(pFile);
	}
	strcpy_s(sWorldParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	if(::fopen_s(&pFile, _pPathName4Exporting, "wt") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << _pPathName4Exporting << ")");
	}
	
	for (int iFieldIdx = 0; iFieldIdx < sWorldParam.WorldFieldNum * sWorldParam.WorldFieldNum; ++iFieldIdx)
	{
		sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d", iFieldIdx);
		_chdir(sWorldParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sWorldParam.WorldChar64Buf);
		sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d",iFieldIdx);

		FileMem.Load(sWorldParam.WorldChar64Buf, "rb");

		::SetCurrentDirectory(acOldPath);

		pFileMemTmp = FileMem.GetDat();
		for(RwInt32 iSectorCnt = 0; iSectorCnt < sWorldParam.WorldFieldSectorNum * sWorldParam.WorldFieldSectorNum; ++iSectorCnt)
		{
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorMaterialSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorPrelightsSkipToFileMem(pFileMemTmp);

			// Doodads
			RwInt32 iNumDoodads;
			CopyMemory(&iNumDoodads, pFileMemTmp, sizeof(RwInt32));
			pFileMemTmp += sizeof(RwInt32);

			for (int iDoodadCnt = 0; iDoodadCnt < iNumDoodads; ++iDoodadCnt)
			{
				RwChar	acDoodadName[128];
				RwV3d	avDoodadTransform[3]; // Scale, Rotation, Translation

				// Use : Name
				if (dNTL_WORLD_VERSION_COMPARE(sWorldParam.WorldVer, dNTL_WORLD_VERSION_OLD))
				{
					CopyMemory(acDoodadName, pFileMemTmp, sizeof(RwChar) * 128);
					pFileMemTmp += (sizeof(RwChar) * 128);

				}
				else if (dNTL_WORLD_VERSION_COMPARE(sWorldParam.WorldVer, dNTL_WORLD_VERSION))
				{
					RwUInt32	uiLength = 0;

					CopyMemory(&uiLength, pFileMemTmp, sizeof(RwUInt32));			
					pFileMemTmp  += sizeof(RwUInt32);

					CopyMemory(acDoodadName, pFileMemTmp , (sizeof(RwChar) * uiLength));
					pFileMemTmp  += (sizeof(RwChar) * uiLength);
					acDoodadName[uiLength] = '\0';
				}

				CopyMemory(avDoodadTransform, pFileMemTmp, sizeof(RwV3d) * 3);
				pFileMemTmp += sizeof(RwV3d) * 3;

				RwUInt32 uiObjectType;
				CopyMemory(&uiObjectType, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += sizeof(RwUInt32);

				if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
					uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
				{
					pFileMemTmp = CNtlPLObjectTypeHasProperty::SkipToFileMem(pFileMemTmp);
				}

				SPLEntityCreateParam sPLEntityCreateParam;
				sPLEntityCreateParam.pPos = &avDoodadTransform[2];

				CNtlPLEntity* pNtlPLObject = static_cast<CNtlPLEntity*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, acDoodadName, &sPLEntityCreateParam));
				DBO_ASSERT(pNtlPLObject, "Entity create failed.");

				RwBBox bboxAABB = pNtlPLObject->GetTriggerAABBInfo(avDoodadTransform[2], avDoodadTransform[1], avDoodadTransform[0]);

				GetSceneManager()->DeleteEntity(static_cast<CNtlPLEntity*>(pNtlPLObject));

				RwUInt32 uiTriggerID;
				CopyMemory(&uiTriggerID, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += (sizeof(RwUInt32));		 // trigger obj serial id

				if (sWorldParam.WorldType == 1)
				{
					pFileMemTmp += (sizeof(RwUInt32));		// Object  serial id	
				}

				DWORD dwObjectNameIdx;
				CopyMemory(&dwObjectNameIdx, pFileMemTmp, sizeof(DWORD));
				pFileMemTmp += (sizeof(DWORD)); 		// Obj. name index

				pFileMemTmp += (sizeof(RwReal));		// visibility distance

				RwBool bEnableTrigger;
				RwBool bEnableSelection;
				RwBool bEnableAlterable;
				RwBool bEnablePortal;

				if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
				{
					CopyMemory(&bEnableTrigger, pFileMemTmp, sizeof(RwBool));
					pFileMemTmp += (sizeof(RwBool));		// enable trigger


					CopyMemory(&bEnableSelection, pFileMemTmp, sizeof(RwBool));
					pFileMemTmp += (sizeof(RwBool));		// enable selection


					CopyMemory(&bEnableAlterable, pFileMemTmp, sizeof(RwBool));
					pFileMemTmp += (sizeof(RwBool));		// enable alterable


					CopyMemory(&bEnablePortal, pFileMemTmp, sizeof(RwBool));
					pFileMemTmp += (sizeof(RwBool));		// enable portal

					pFileMemTmp += (sizeof(RwBool));		// enable path engine collision
					pFileMemTmp += (sizeof(RwBool));		// enable shadowprop
				}
				else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
				{
					RwUInt32 uiFlags;
					CopyMemory(&uiFlags, pFileMemTmp, sizeof(RwUInt32));
					pFileMemTmp += (sizeof(RwUInt32));

					bEnableTrigger = (uiFlags & NTL_PLEFLAG_TRIGGER);
					bEnableSelection = (uiFlags & NTL_PLEFLAG_SELECTION);
					bEnableAlterable = (uiFlags & NTL_PLEFLAG_ALTERABLE);
					bEnablePortal = (uiFlags & NTL_PLEFLAG_PORTAL);
				}

				if(uiTriggerID != 0xffffffff && bEnableTrigger)
				{
					RwV3d		vTempScale	= { 1.0f, 1.0f, 1.0f };
					RwV3d		vTempRot	= avDoodadTransform[1];
					RwV3d		vTempPos	= { 0.0f, 0.0f, 0.0f };
					RwV3d		vTempRotResult;
					RwMatrix	matTemp;

					CNtlMath::MathGetMatrix(&matTemp, &vTempScale, &vTempRot, &vTempPos);
					RwV3dTransformVector(&vTempRotResult, &CNtlPLGlobal::m_vZAxisV3, &matTemp);

					vTempRotResult.y = 0.0f;
					RwV3dNormalize(&vTempRotResult, &vTempRotResult);

					fprintf_s(pFile, "%d\t%u\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%d\t%d\t%d\t%s\t\n",
						uiTriggerID,
						dwObjectNameIdx,
						avDoodadTransform[2].x, avDoodadTransform[2].y, avDoodadTransform[2].z,
						vTempRotResult.x, vTempRotResult.y, vTempRotResult.z,						
						bboxAABB.inf.x, bboxAABB.inf.y, bboxAABB.inf.z,
						bboxAABB.sup.x, bboxAABB.sup.y, bboxAABB.sup.z,
						CLAMP(bEnableAlterable, 0, 1),
						CLAMP(bEnableSelection, 0, 1),
						CLAMP(bEnablePortal, 0, 1),
						acDoodadName);
				}

				// PS Map Size
				pFileMemTmp = CNtlPLObject::SkipPSMapToFileMem(pFileMemTmp);

				// milepost
				RwBool EnableFlg;
				CopyMemory(&EnableFlg, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += sizeof(RwBool);

				if(EnableFlg)
				{
					pFileMemTmp += sizeof(RwUInt32);
				}

				// attach light object skip
				pFileMemTmp = CNtlPLObject::SkipLightObjectAttachDataToFileMem(pFileMemTmp);

				if (sWorldParam.WorldType == 1)
				{
					pFileMemTmp = CNtlPLObject::SkipLightmapToFileMem(pFileMemTmp);
				}
			}

			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWaterSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShadowSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorEffectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFileMem(pFileMemTmp);
			// CZ-SS
			//pFileMemTmp = Skip_SS(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorCullSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorTileTransparencySkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShoreLineSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDecalSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorPlantSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWorldLightSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorOccluderSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorLightObjectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDojoSkipToFileMem(pFileMemTmp);
		}

		FileMem.Free();
	}

	if (pFile)
	{
		fclose(pFile);
	}

	::SetCurrentDirectory(acOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportHeightfieldY(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	/*
	WorldFileFormat - Export
	Export 처리를 위한 부분이다.
	*/

	sNtlWorldParam	sTmpPrjParam;
	FILE*			pFile;
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;
	RwChar			szFFPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar			szOldPath[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sTmpPrjParam, pFile);
		fclose(pFile);
	}

	RwInt32		iWorldSectorVertNum		= sTmpPrjParam.WorldSectorVertNum * sTmpPrjParam.WorldSectorVertNum;
	RwV3d*		pvHeightfield			= NTL_NEW RwV3d[iWorldSectorVertNum];

	strcpy_s(sTmpPrjParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	for(int i = 0; i < sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum; ++i)
	{
		// current field file name
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);

		// name the file for exporting
		strcpy_s(szFFPathName, NTL_MAX_DIR_PATH, _pPathName4Exporting);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "\\");
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, sTmpPrjParam.WorldChar64Buf);
		_mkdir(szFFPathName);
		
		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sTmpPrjParam.WorldChar64Buf);

		// export
		FileMem.Load(sTmpPrjParam.WorldChar64Buf, "rb");

		pFileMemTmp = FileMem.GetDat();
		//pFileMemTmp = GetSPosOfMaterial(FileMem.GetDat());

		for(RwInt32 j = 0; j < sTmpPrjParam.WorldFieldSectorNum * sTmpPrjParam.WorldFieldSectorNum; ++j)
		{
			RwChar szPathName[NTL_MAX_DIR_PATH];
			sprintf_s(szPathName, NTL_MAX_DIR_PATH, "%s\\shfy%d.txt", szFFPathName, j);
			if(fopen_s(&pFile, szPathName, "w"))
			{
				DBO_TRACE(FALSE, "file open failed. (" << szPathName << ")");
				NTL_DELETE(pvHeightfield);
				return FALSE;
			}

			pFileMemTmp = GetNtlWorldSectorInfo()->SectorMaterialSkipToFileMem(pFileMemTmp);

			pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileMemGetVertex(pFileMemTmp, 0, iWorldSectorVertNum, pvHeightfield);

			for (int x = 0; x < sTmpPrjParam.WorldSectorVertNum; ++x)
			{
				for (int y = sTmpPrjParam.WorldSectorVertNum - 1; y >= 0; --y)
				{
					::fprintf_s(pFile, "%f\n", pvHeightfield[y * sTmpPrjParam.WorldSectorVertNum + x].y);
				}
			}
			
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorPrelightsSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorObjectSkipToFileMem(pFileMemTmp, AW_HEGITHFIELD);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWaterSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShadowSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorEffectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFileMem(pFileMemTmp);
			// CZ-SS
			//pFileMemTmp = Skip_SS(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorCullSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorTileTransparencySkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShoreLineSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDecalSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorPlantSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWorldLightSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorOccluderSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorLightObjectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDojoSkipToFileMem(pFileMemTmp);

			fclose(pFile);
		}
		FileMem.Free();

		_chdir("../");
	}

	NTL_DELETE(pvHeightfield);
	::SetCurrentDirectory(szOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportPECollision(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	/*
	WorldFileFormat - Export
	Export 처리를 위한 부분이다.
	*/

	sNtlWorldParam	sTmpPrjParam;
	FILE*			pFile;
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;
	RwChar			szFFPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar			szOldPath[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sTmpPrjParam, pFile);
		fclose(pFile);
	}

	strcpy_s(sTmpPrjParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	for(int i = 0; i < sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum; ++i)
	{
		// current field file name
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);

		// name the file for exporting
		strcpy_s(szFFPathName, NTL_MAX_DIR_PATH, _pPathName4Exporting);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "\\");
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, sTmpPrjParam.WorldChar64Buf);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, ".pec");
		if(fopen_s(&pFile, szFFPathName, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << szFFPathName << ")");
			return FALSE;
		}

		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sTmpPrjParam.WorldChar64Buf);

		// export
		FileMem.Load(sTmpPrjParam.WorldChar64Buf, "rb");

		pFileMemTmp = FileMem.GetDat();
		//pFileMemTmp = GetSPosOfMaterial(FileMem.GetDat());

		for(RwInt32 j = 0; j < sTmpPrjParam.WorldFieldSectorNum * sTmpPrjParam.WorldFieldSectorNum; ++j)
		{
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorMaterialSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorPrelightsSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorObjectSaveIntoFileFromFileMemPEC(pFile, pFileMemTmp, AW_HEGITHFIELD);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorObjectSkipToFileMem(pFileMemTmp, AW_HEGITHFIELD);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWaterSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShadowSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorEffectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorCullSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorTileTransparencySkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShoreLineSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDecalSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorPlantSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWorldLightSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorOccluderSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorLightObjectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDojoSkipToFileMem(pFileMemTmp);
		}

		FileMem.Free();
		fclose(pFile);

		_chdir("../");
	}

	::SetCurrentDirectory(szOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportInvalidSectorLink(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;

	RwChar			acTempPath[NTL_MAX_DIR_PATH] = {0,};
	RwChar			acOldPath[NTL_MAX_DIR_PATH] = {0,};
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	sNtlWorldParam	sWorldParam;
	FILE*			pFile;
	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << sWorldParam.WorldChar64Buf << ")");		
	}
	else
	{
		LoadFileWorldState(&sWorldParam, pFile);
		fclose(pFile);
	}
	strcpy_s(sWorldParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	if(::fopen_s(&pFile, _pPathName4Exporting, "wt") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << _pPathName4Exporting << ")");
	}

	for (int iFieldIdx = 0; iFieldIdx < sWorldParam.WorldFieldNum * sWorldParam.WorldFieldNum; ++iFieldIdx)
	{
		sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d", iFieldIdx);
		_chdir(sWorldParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sWorldParam.WorldChar64Buf);
		sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d",iFieldIdx);

		FileMem.Load(sWorldParam.WorldChar64Buf, "rb");

		::SetCurrentDirectory(acOldPath);

		pFileMemTmp = FileMem.GetDat();
		for(RwInt32 iSectorCnt = 0; iSectorCnt < sWorldParam.WorldFieldSectorNum * sWorldParam.WorldFieldSectorNum; ++iSectorCnt)
		{
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorMaterialSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorPrelightsSkipToFileMem(pFileMemTmp);

			// Doodads
			RwInt32 iNumDoodads;
			CopyMemory(&iNumDoodads, pFileMemTmp, sizeof(RwInt32));
			pFileMemTmp += sizeof(RwInt32);

			for (int iDoodadCnt = 0; iDoodadCnt < iNumDoodads; ++iDoodadCnt)
			{
				RwChar	acDoodadName[128];
				RwV3d	avDoodadTransform[3]; // Scale, Rotation, Translation

				if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
				{
					CopyMemory(acDoodadName, pFileMemTmp, sizeof(RwChar) * 128);
					pFileMemTmp += sizeof(RwChar) * 128;

				}
				else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
				{
					RwUInt32	uiLength = 0;

					CopyMemory(&uiLength, pFileMemTmp, sizeof(RwUInt32));			
					pFileMemTmp  += sizeof(RwUInt32);

					CopyMemory(acDoodadName, pFileMemTmp , (sizeof(RwChar) * uiLength));
					pFileMemTmp  += (sizeof(RwChar) * uiLength);
					acDoodadName[uiLength] = '\0';
				}
				
				CopyMemory(avDoodadTransform, pFileMemTmp, sizeof(RwV3d) * 3);
				pFileMemTmp += sizeof(RwV3d) * 3;

				RwUInt32 uiObjectType;
				CopyMemory(&uiObjectType, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += sizeof(RwUInt32);

				if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
					uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
				{
					pFileMemTmp = CNtlPLObjectTypeHasProperty::SkipToFileMem(pFileMemTmp);
				}

				RwUInt32 uiTriggerID;
				CopyMemory(&uiTriggerID, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += (sizeof(RwUInt32));		 // trigger obj serial id

				if (sWorldParam.WorldType == 1)
				{
					pFileMemTmp += (sizeof(RwUInt32));		// Object  serial id	
				}

				DWORD dwObjectNameIdx;
				CopyMemory(&dwObjectNameIdx, pFileMemTmp, sizeof(DWORD));
				pFileMemTmp += (sizeof(DWORD)); 		// Obj. name index

				pFileMemTmp += (sizeof(RwReal));		// visibility distance

				if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
				{
					RwBool bEnableTrigger;
					CopyMemory(&bEnableTrigger, pFileMemTmp, sizeof(RwBool));
					pFileMemTmp += (sizeof(RwBool));		// enable trigger

					RwBool bEnableSelection;
					CopyMemory(&bEnableSelection, pFileMemTmp, sizeof(RwBool));
					pFileMemTmp += (sizeof(RwBool));		// enable selection

					RwBool bEnableAlterable;
					CopyMemory(&bEnableAlterable, pFileMemTmp, sizeof(RwBool));
					pFileMemTmp += (sizeof(RwBool));		// enable alterable

					RwBool bEnablePortal;
					CopyMemory(&bEnablePortal, pFileMemTmp, sizeof(RwBool));
					pFileMemTmp += (sizeof(RwBool));		// enable portal

					pFileMemTmp += (sizeof(RwBool));		// enable path engine collision
					pFileMemTmp += (sizeof(RwBool));		// enable shadowprop
				}
				else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
				{
					// Skip : enable trigger : enable selection : enable alterable : enable portal : enable path engine collision : enable shadowprop
					pFileMemTmp += (sizeof(RwUInt32));
				}

				RwBBox bboxSector;
				bboxSector.inf.x = static_cast<RwReal>((iFieldIdx % sWorldParam.WorldFieldNum) * sWorldParam.WorldFieldSize) - sWorldParam.WorldSizeHalf;
				bboxSector.inf.z = static_cast<RwReal>((iFieldIdx / sWorldParam.WorldFieldNum) * sWorldParam.WorldFieldSize) - sWorldParam.WorldSizeHalf;

				if (iSectorCnt & 1)
				{
					bboxSector.inf.x += static_cast<RwReal>(sWorldParam.WorldSectorSize);
				}
				if (iSectorCnt & 2)
				{
					bboxSector.inf.z += static_cast<RwReal>(sWorldParam.WorldSectorSize);
				}

				bboxSector.sup.x = bboxSector.inf.x + static_cast<RwReal>(sWorldParam.WorldSectorSize);
				bboxSector.sup.z = bboxSector.inf.z + static_cast<RwReal>(sWorldParam.WorldSectorSize);

				if (!(avDoodadTransform[2].x >= bboxSector.inf.x &&
					avDoodadTransform[2].z >= bboxSector.inf.z &&
					avDoodadTransform[2].x < bboxSector.sup.x &&
					avDoodadTransform[2].z < bboxSector.sup.z))
				{

					RwInt32	iWidthNum, iDepthNum;
					RwReal	fXBasedOn0, fZBasedOn0;
					RwInt32	iRetInvalidIdx, iRetValidIdx;

					fXBasedOn0		= bboxSector.inf.x + (sWorldParam.WorldSectorSize / 2) + sWorldParam.WorldSizeHalf;
					fZBasedOn0		= bboxSector.inf.z + (sWorldParam.WorldSectorSize / 2) + sWorldParam.WorldSizeHalf;
					iWidthNum		= (RwInt32)(fXBasedOn0 / (RwReal)sWorldParam.WorldSectorSize);
					iDepthNum		= (RwInt32)(fZBasedOn0 / (RwReal)sWorldParam.WorldSectorSize); 
					iRetInvalidIdx	= iWidthNum + (iDepthNum * dGET_WORLD_PARAM()->WorldSectorNum);
					
					fXBasedOn0		= avDoodadTransform[2].x + sWorldParam.WorldSizeHalf;
					fZBasedOn0		= avDoodadTransform[2].z + sWorldParam.WorldSizeHalf;
					iWidthNum		= (RwInt32)(fXBasedOn0 / (RwReal)sWorldParam.WorldSectorSize);
					iDepthNum		= (RwInt32)(fZBasedOn0 / (RwReal)sWorldParam.WorldSectorSize); 
					iRetValidIdx	= iWidthNum + (iDepthNum * dGET_WORLD_PARAM()->WorldSectorNum);

					fprintf_s(pFile, "POS(%.2f, %.2f, %.2f) INVALID_SECTOR(%d) VALID_SECTOR(%d)\n",
						avDoodadTransform[2].x, avDoodadTransform[2].y, avDoodadTransform[2].z, iRetInvalidIdx, iRetValidIdx);
				}

				// PS Map Size
				pFileMemTmp = CNtlPLObject::SkipPSMapToFileMem(pFileMemTmp);

				// milepost
				RwBool EnableFlg;
				CopyMemory(&EnableFlg, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += sizeof(RwBool);

				if(EnableFlg)
				{
					pFileMemTmp += sizeof(RwUInt32);
				}

				// attach light object skip
				pFileMemTmp = CNtlPLObject::SkipLightObjectAttachDataToFileMem(pFileMemTmp);

				if (sWorldParam.WorldType == 1)
				{
					pFileMemTmp = CNtlPLObject::SkipLightmapToFileMem(pFileMemTmp);
				}
			}

			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWaterSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShadowSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorEffectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFileMem(pFileMemTmp);
			// CZ-SS
			//pFileMemTmp = Skip_SS(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorCullSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorTileTransparencySkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShoreLineSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDecalSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorPlantSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWorldLightSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorOccluderSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorLightObjectSkipToFileMem(pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDojoSkipToFileMem(pFileMemTmp);
		}

		FileMem.Free();
	}

	if (pFile)
	{
		fclose(pFile);
	}

	::SetCurrentDirectory(acOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnProofsInvalidSectorLink(RwInt32 FieldIdx)
{
	CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(m_pFieldMgr->GetFields());

	if(!m_pFieldMgr->IsFieldValid(FieldIdx))
	{
		DBO_TRACE(FALSE, "CNtlWorldMergeManager::OnProofsInvalidSectorLink, invalid field index.");
		return FALSE;
	}

	CFileMem	FileMem;
	BYTE*		pPosCurFileMem;
	FILE*		pFile;

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	FileMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");

	if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		return FALSE;
	}

	pPosCurFileMem = FileMem.GetDat();

	for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldFieldSectorNum; ++i)
	{
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorMaterialSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorHeightfieldSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorPrelightsSaveIntoFileFromFileMem(pFile, pPosCurFileMem);


		RwBBox bboxSector;
		bboxSector.inf.x = static_cast<RwReal>((FieldIdx % dGET_WORLD_PARAM()->WorldFieldNum) * dGET_WORLD_PARAM()->WorldFieldSize) - dGET_WORLD_PARAM()->WorldSizeHalf;
		bboxSector.inf.z = static_cast<RwReal>((FieldIdx / dGET_WORLD_PARAM()->WorldFieldNum) * dGET_WORLD_PARAM()->WorldFieldSize) - dGET_WORLD_PARAM()->WorldSizeHalf;

		if (i & 1)
		{
			bboxSector.inf.x += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
		}
		if (i & 2)
		{
			bboxSector.inf.z += static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
		}

		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorObjectSaveIntoFileFromFileMemRevisionPos(pFile, pPosCurFileMem, AW_HEGITHFIELD, &bboxSector);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorWaterSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorShadowSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorEffectSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorSoundEffectSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorCullSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorTileTransparencySaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorShoreLineSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorDecalSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorPlantSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorWorldLightSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorOccluderSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorLightObjectSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorDojoSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
	}

	fclose(pFile);

	m_pFieldMgr->RefreshField(FieldIdx);

	FileMem.Free();

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnObjVisibilityDistChange(RwInt32 FieldIdx, RwReal VisibilityDist, RwReal Min, RwReal Max)
{
	CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(m_pFieldMgr->GetFields());

	if(!m_pFieldMgr->IsFieldValid(FieldIdx))
	{
		DBO_TRACE(FALSE, "CNtlWorldMergeManager::OnObjVisibilityDistChange, invalid field index.");
		return FALSE;
	}

	CFileMem	FileMem;
	BYTE*		pPosCurFileMem;
	FILE*		pFile;

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	FileMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");

	if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		return FALSE;
	}

	pPosCurFileMem = FileMem.GetDat();

	for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldFieldSectorNum; ++i)
	{
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorMaterialSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorHeightfieldSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorPrelightsSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorObjectSaveIntoFileFromFileMemVisibilityDistance(pFile, pPosCurFileMem, AW_HEGITHFIELD, VisibilityDist, Min, Max);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorWaterSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorShadowSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorEffectSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorSoundEffectSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		// CZ-SS
		//pPosCurFileMem = Save_SS(pPosCurFileMem, pFile);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorCullSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorTileTransparencySaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorShoreLineSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorDecalSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorPlantSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorWorldLightSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorOccluderSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorLightObjectSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
		pPosCurFileMem = GetNtlWorldSectorInfo()->SectorDojoSaveIntoFileFromFileMem(pFile, pPosCurFileMem);
	}

	fclose(pFile);

	m_pFieldMgr->RefreshField(FieldIdx);

	FileMem.Free();

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

RwBool CNtlWorldMergeManager::MergeInformation(sMERGE_PARAM &sParam, RwInt32 IdxFieldSrc, RwInt32 IdxFieldDst)
{
	BYTE*	pPosCurFileMemSrc;
	BYTE*	pPosCurFileMemDst;
	FILE*	pFileMerge;

	_chdir(m_NtlWorldParam2Merge.WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(m_NtlWorldParam2Merge.WorldChar64Buf, 64, "wfif%d", IdxFieldSrc);
	_chdir(m_NtlWorldParam2Merge.WorldChar64Buf);
	m_FileMemSrc.Load(m_NtlWorldParam2Merge.WorldChar64Buf, "rb");

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxFieldDst);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	m_FileMemDst.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");

	// open dest. field file 2 merge as "wb"
	if(fopen_s(&pFileMerge, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		return FALSE;
	}

	// starting file memory position
	pPosCurFileMemSrc = m_FileMemSrc.GetDat();
	pPosCurFileMemDst = m_FileMemDst.GetDat();
// 	pPosCurFileMemSrc = GetSPosOfMaterial(m_FileMemSrc.GetDat());
// 	pPosCurFileMemDst = GetSPosOfMaterial(m_FileMemDst.GetDat());

	// loop sector's num per field & merge the proper spec.
	for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldFieldSectorNum; ++i)
	{
		//////////////////////////////////////////////////////////////////////////
		// Information Update : Material
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bMaterial)
		{
			// merge area : move to material and save
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorMaterialSkipToFileMem(pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorMaterialSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
		}
		else
		{
			// old area : move to material and save
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorMaterialSkipToFileMem(pPosCurFileMemSrc);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorMaterialSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : HeightField
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bHeightField)
		{
			// merge area : heightfield
			GetNtlWorldSectorInfo()->SectorHeightfieldSaveIntoFileFromFileMemDiff(pFileMerge, pPosCurFileMemDst, pPosCurFileMemSrc);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileMem(pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileMem(pPosCurFileMemSrc);
		}
		else
		{
			// old area : heightfield
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorHeightfieldSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileMem(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Diffuse
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bDiffuse)
		{
			// merge area : light(limited to diffuse here, (sdw, sl, dif) belong to the swap files)
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorPrelightsSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorPrelightsSkipToFileMem(pPosCurFileMemDst);
		}
		else
		{
			// old area : light(limited to diffuse here, (sdw, sl, dif) belong to the swap files)
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorPrelightsSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorPrelightsSkipToFileMem(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Doodads
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bDoodads)
		{
			// merge area : doodads
			GetNtlWorldSectorInfo()->SectorObjectSaveIntoFileFromFileMemDiff(pFileMerge, pPosCurFileMemDst, pPosCurFileMemSrc, AW_HEGITHFIELD, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorObjectSkipToFileMem(pPosCurFileMemSrc, AW_HEGITHFIELD);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorObjectSkipToFileMem(pPosCurFileMemDst, AW_HEGITHFIELD);
		}
		else
		{
			// old area : doodads
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorObjectSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst, AW_HEGITHFIELD);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorObjectSkipToFileMem(pPosCurFileMemSrc, AW_HEGITHFIELD);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Water
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bWater)
		{
			// merge area : water
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorWaterSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorWaterSkipToFileMem(pPosCurFileMemDst);
		}
		else
		{
			// old area : water
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorWaterSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorWaterSkipToFileMem(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Shadow : Field Index가 다르면 안된다.
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bShadow && IdxFieldDst == IdxFieldSrc)
		{
			// merge area : static projection shadow
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorShadowSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorShadowSkipToFileMem(pPosCurFileMemDst);
		}
		else
		{
			// old area : static projection shadow
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorShadowSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorShadowSkipToFileMem(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Effects
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bEffects)
		{
			// merge area : effects
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorEffectSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorEffectSkipToFileMem(pPosCurFileMemDst);
		}
		else
		{
			// old area : effects
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorEffectSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorEffectSkipToFileMem(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : SE
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bSE)
		{
			// merge area : SE
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorSoundEffectSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFileMem(pPosCurFileMemDst);
		}
		else
		{
			// old area : SE
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorSoundEffectSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFileMem(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Sector Cull
		//////////////////////////////////////////////////////////////////////////			
		if (sParam.bSectorCull)
		{
			// merge area : Sector Cull
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorCullSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorCullSkipToFileMem(pPosCurFileMemDst);
		}
		else
		{
			// old area : Sector Cull
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorCullSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorCullSkipToFileMem(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Tile Transparency
		//////////////////////////////////////////////////////////////////////////			
		if (sParam.bTileTransparency)
		{
			// merge area : Tile Transparency
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorTileTransparencySaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorTileTransparencySkipToFileMem(pPosCurFileMemDst);
		}
		else
		{
			// old area : Tile Transparency
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorTileTransparencySaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorTileTransparencySkipToFileMem(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Shoreline
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bShoreline)
		{
			// merge area : Shoreline
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorShoreLineSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorShoreLineSkipToFileMem(pPosCurFileMemDst);
		}
		else
		{
			// old area : shoreline
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorShoreLineSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorShoreLineSkipToFileMem(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Decal
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bDecal)
		{
			// merge area : terrain decals
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorDecalSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorDecalSkipToFileMem(pPosCurFileMemDst);
		}
		else
		{
			// old area
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorDecalSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorDecalSkipToFileMem(pPosCurFileMemSrc);
		}	

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Plant
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bPlant)
		{
			// merge area : plant
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorPlantSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorPlantSkipToFileMem(pPosCurFileMemDst);
		}
		else
		{
			// old area
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorPlantSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorPlantSkipToFileMem(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : WorldLight
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bWorldLight)
		{
			// merge area : world light
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorWorldLightSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorWorldLightSkipToFileMem(pPosCurFileMemDst);
		}
		else
		{
			// old area
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorWorldLightSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorWorldLightSkipToFileMem(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Occluder
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bOccluder)
		{
			// merge area : occluder
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorOccluderSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorOccluderSkipToFileMem(pPosCurFileMemDst);
		}
		else
		{
			// old area
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorOccluderSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorOccluderSkipToFileMem(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : HeatHaze
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bHeatHazeObject)
		{
			// merge area : haethaze
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSkipToFileMem(pPosCurFileMemDst);
		}
		else
		{
			// old area
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSkipToFileMem(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Light Object
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bLightObject)
		{
			// merge area : light object
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorLightObjectSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorLightObjectSkipToFileMem(pPosCurFileMemDst);
		}
		else
		{
			// old area
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorLightObjectSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorLightObjectSkipToFileMem(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Dojo
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bDojo)
		{
			// merge area : dojo
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorDojoSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorDojoSkipToFileMem(pPosCurFileMemDst);
		}
		else
		{
			// old area
			pPosCurFileMemDst = GetNtlWorldSectorInfo()->SectorDojoSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
			pPosCurFileMemSrc = GetNtlWorldSectorInfo()->SectorDojoSkipToFileMem(pPosCurFileMemSrc);
		}
	}

	fclose(pFileMerge);

	m_FileMemSrc.Free();
	m_FileMemDst.Free();

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	//////////////////////////////////////////////////////////////////////////
	// Heightfield Variation
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bHeightField)
	{
		OnHeightfieldVariation(IdxFieldDst);
	}

	return TRUE;
}

RwBool CNtlWorldMergeManager::MergeProperty(sMERGE_PARAM &sParam, RwInt32 IdxFieldSrc, RwInt32 IdxFieldDst)
{
	BYTE*	pPosCurFileMemSrc;
	BYTE*	pPosCurFileMemDst;
	FILE*	pFileMerge;

	_chdir(m_NtlWorldParam2Merge.WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(m_NtlWorldParam2Merge.WorldChar64Buf, 64, "wfif%d", IdxFieldSrc);
	_chdir(m_NtlWorldParam2Merge.WorldChar64Buf);
	sprintf_s(m_NtlWorldParam2Merge.WorldChar64Buf, 64, "wfpf%d", IdxFieldSrc);
	m_FileMemSrc.Load(m_NtlWorldParam2Merge.WorldChar64Buf, "rb");

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxFieldDst);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", IdxFieldDst);
	m_FileMemDst.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");

	// open dest. field file 2 merge as "wb"
	if(fopen_s(&pFileMerge, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		return FALSE;
	}

	RwInt32 iTileNumInField =	(dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldSectorTileNum) *
								(dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldSectorTileNum);

	pPosCurFileMemSrc = m_FileMemSrc.GetDat();
	pPosCurFileMemDst = m_FileMemDst.GetDat();

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : Name
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bName)
	{
		// merge area : name
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldNameSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldNameSkipToFileMem(pPosCurFileMemDst);
		
	}
	else
	{
		// old area : name
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldNameSkipToFileMem(pPosCurFileMemSrc);
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldNameSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : BitProp
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bBitProp)
	{
		// merge area : bitprop
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldBitPropSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc, iTileNumInField);
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldBitPropSkipToFileMem(pPosCurFileMemDst, iTileNumInField);
	}
	else
	{
		// old area : bitprop
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldBitPropSkipToFileMem(pPosCurFileMemSrc, iTileNumInField);
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldBitPropSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst, iTileNumInField);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : Fog
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bFog)
	{
		// merge area : fog
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldFogSkipToFileMem(pPosCurFileMemDst);
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldFogSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
	}
	else
	{
		// old area : fog
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldFogSkipToFileMem(pPosCurFileMemSrc);
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldFogSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : Sky
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bSky)
	{
		// merge area : sky
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldSkySkipToFileMem(pPosCurFileMemDst);
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldSkySaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
	}
	else
	{
		// old area : sky
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldSkySkipToFileMem(pPosCurFileMemSrc);
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldSkySaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : RealLight
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bRealLight)
	{
		// merge area : real light
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldRealLightSkipToFileMem(pPosCurFileMemDst);
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldRealLightSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
	}
	else
	{
		// old area : real light
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldRealLightSkipToFileMem(pPosCurFileMemSrc);
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldRealLightSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : BGM
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bBGM)
	{
		// merge area : bgm
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldBGMSkipToFileMem(pPosCurFileMemDst);
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldBGMSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
	}
	else
	{
		// old area : bgm
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldBGMSkipToFileMem(pPosCurFileMemSrc);
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldBGMSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : Bloom
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bBloom)
	{
		// merge area : bloom
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldBloomSkipToFileMem(pPosCurFileMemDst);
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldBloomSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
	}
	else
	{
		// old area : bloom
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldBloomSkipToFileMem(pPosCurFileMemSrc);
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldBloomSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
	}			

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : Planet
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bPlanet)
	{
		// merge area : planets
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldPlanetSkipToFileMem(pPosCurFileMemDst);
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldPlanetSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
	}
	else
	{
		// old area : planet
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldPlanetSkipToFileMem(pPosCurFileMemSrc);
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldPlanetSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : Specular
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bSpecular)
	{
		// merge area : specular
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldSpecularSkipToFileMem(pPosCurFileMemDst);
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldSpecularSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
	}
	else
	{
		// old area : specular
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldSpecularSkipToFileMem(pPosCurFileMemSrc);
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldSpecularSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : WorldMaterial
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bWorldMaterial)
	{
		// merge area : world material
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldWorldMaterialSkipToFileMem(pPosCurFileMemDst, iTileNumInField);
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldWorldMaterialSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc, iTileNumInField);
	}
	else
	{
		// old area : world material
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldWorldMaterialSkipToFileMem(pPosCurFileMemSrc, iTileNumInField);
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldWorldMaterialSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst, iTileNumInField);
	}		

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : FieldColor
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bFieldColor)
	{
		// merge area : field color
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldColorSkipToFileMem(pPosCurFileMemDst);
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldColorSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
	}
	else
	{
		// old area : field color
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldColorSkipToFileMem(pPosCurFileMemSrc);
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldColorSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : Weather
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bWeather)
	{
		// merge area : weather
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldWeatherSkipToFileMem(pPosCurFileMemDst);
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldWeatherSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
	}
	else
	{
		// old area : weather
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldWeatherSkipToFileMem(pPosCurFileMemSrc);
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldWeatherSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : HeatHaze
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bFieldHeatHaze)
	{
		// merge area : heathaze
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldHeatHazeSkipToFileMem(pPosCurFileMemDst);
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldHeatHazeSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemSrc);
	}
	else
	{
		// old area : heathaze
		pPosCurFileMemSrc = GetNtlWorldFieldInfo()->FieldHeatHazeSkipToFileMem(pPosCurFileMemSrc);
		pPosCurFileMemDst = GetNtlWorldFieldInfo()->FieldHeatHazeSaveIntoFileFromFileMem(pFileMerge, pPosCurFileMemDst);
	}

	fclose(pFileMerge);

	m_FileMemSrc.Free();
	m_FileMemDst.Free();

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

RwBool CNtlWorldMergeManager::MergeSwapFile(sMERGE_PARAM &sParam, RwInt32 IdxFieldSrc, RwInt32 IdxFieldDst)
{
	CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(m_pFieldMgr->GetFields());

	int		l, m;
	int		SectorIdx;
	RwChar	strSrcPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar	strDstPathName[NTL_MAX_DIR_PATH] = {0,};

	// find sector indices at source merge area
	vector<RwInt32>	vecSectorIndices;
	RwV3d	SPos = pFields[IdxFieldSrc].GetSPos();
	RwV3d	SectorSPos;

	for(l = static_cast<RwInt32>(SPos.z); l < static_cast<RwInt32>(SPos.z) + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = static_cast<RwInt32>(SPos.x); m < static_cast<RwInt32>(SPos.x) + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			vecSectorIndices.push_back(m_pFieldMgr->GetSectorIdx(SectorSPos));
		}
	}

	// match each index and merge
	RwInt32 IdxSectorVec = 0;
	RwChar	SrcFileName[64] = {0,};

	SPos = pFields[IdxFieldDst].GetSPos();

	for(l = static_cast<RwInt32>(SPos.z); l < static_cast<RwInt32>(SPos.z) + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = static_cast<RwInt32>(SPos.x); m < static_cast<RwInt32>(SPos.x) + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = m_pFieldMgr->GetSectorIdx(SectorSPos);

			//////////////////////////////////////////////////////////////////////////
			// Swap Data Update : SL
			//////////////////////////////////////////////////////////////////////////
			if (sParam.bDiffuse)
			{
				// sl files
				sprintf_s(SrcFileName, 64, "%d_sec.sl", vecSectorIndices[IdxSectorVec]);
				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.sl", SectorIdx);
				strcpy_s(strSrcPathName, NTL_MAX_DIR_PATH, m_NtlWorldParam2Merge.WorldProjectFolderName);
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, "swap\\sl\\");
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, SrcFileName);

				strcpy_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, "swap\\sl\\");
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

				DBO_TRACE(CopyFile(strSrcPathName, strDstPathName), "CNtlWorldMergeManager::MergeSwapFile, file copy failed.");
			}

			//////////////////////////////////////////////////////////////////////////
			// Swap Data Update : DIF
			//////////////////////////////////////////////////////////////////////////
			if (sParam.bDiffuse)
			{
				// dif files
				sprintf_s(SrcFileName, 64, "%d_sec.dif", vecSectorIndices[IdxSectorVec]);
				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.dif", SectorIdx);
				strcpy_s(strSrcPathName, NTL_MAX_DIR_PATH, m_NtlWorldParam2Merge.WorldProjectFolderName);
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, "swap\\");
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, SrcFileName);

				strcpy_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, "swap\\");
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

				DBO_TRACE(CopyFile(strSrcPathName, strDstPathName), "CNtlWorldMergeManager::MergeSwapFile, file copy failed.");
			}

			//////////////////////////////////////////////////////////////////////////
			// Swap Data Update : SDW
			//////////////////////////////////////////////////////////////////////////
			if (sParam.bShadow)
			{
				// sdw files
				sprintf_s(SrcFileName, 64, "%d_sec.sdw", vecSectorIndices[IdxSectorVec]);
				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.sdw", SectorIdx);
				strcpy_s(strSrcPathName, NTL_MAX_DIR_PATH, m_NtlWorldParam2Merge.WorldProjectFolderName);
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, "swap\\");
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, SrcFileName);

				strcpy_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, "swap\\");
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

				DBO_TRACE(CopyFile(strSrcPathName, strDstPathName), "CNtlWorldMergeManager::MergeSwapFile, file copy failed.");
			}

			//////////////////////////////////////////////////////////////////////////
			// Swap Data Update : OCQ
			//////////////////////////////////////////////////////////////////////////
			if (sParam.bOccluder)
			{
				// sdw files
				sprintf_s(SrcFileName, 64, "%d_sec.ocq", vecSectorIndices[IdxSectorVec]);
				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.ocq", SectorIdx);
				strcpy_s(strSrcPathName, NTL_MAX_DIR_PATH, m_NtlWorldParam2Merge.WorldProjectFolderName);
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, "swap\\occ\\");
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, SrcFileName);

				strcpy_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, "swap\\occ\\");
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

				DBO_TRACE(CopyFile(strSrcPathName, strDstPathName), "CNtlWorldMergeManager::MergeSwapFile, file copy failed.");
			}

			//////////////////////////////////////////////////////////////////////////
			// Swap Data Update : HHO
			//////////////////////////////////////////////////////////////////////////
			if (sParam.bHeatHazeObject)
			{
				// sdw files
				sprintf_s(SrcFileName, 64, "%d_sec.hho", vecSectorIndices[IdxSectorVec]);
				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.hho", SectorIdx);
				strcpy_s(strSrcPathName, NTL_MAX_DIR_PATH, m_NtlWorldParam2Merge.WorldProjectFolderName);
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, "swap\\hho\\");
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, SrcFileName);

				strcpy_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, "swap\\hho\\");
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

				DBO_TRACE(CopyFile(strSrcPathName, strDstPathName), "CNtlWorldMergeManager::MergeSwapFile, file copy failed.");
			}

			//////////////////////////////////////////////////////////////////////////
			// Swap Data Update : OBJ
			//////////////////////////////////////////////////////////////////////////
			if (sParam.bDoodads)
			{
				// sdw files
				sprintf_s(SrcFileName, 64, "%d_sec.obj", vecSectorIndices[IdxSectorVec]);
				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.obj", SectorIdx);
				strcpy_s(strSrcPathName, NTL_MAX_DIR_PATH, m_NtlWorldParam2Merge.WorldProjectFolderName);
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, "swap\\obj\\");
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, SrcFileName);

				strcpy_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, "swap\\obj\\");
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

				DBO_TRACE(CopyFile(strSrcPathName, strDstPathName), "CNtlWorldMergeManager::MergeSwapFile, file copy failed.");
			}

			//////////////////////////////////////////////////////////////////////////
			// Swap Data Update : EFF
			//////////////////////////////////////////////////////////////////////////
			if (sParam.bEffects)
			{
				// sdw files
				sprintf_s(SrcFileName, 64, "%d_sec.eff", vecSectorIndices[IdxSectorVec]);
				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.eff", SectorIdx);
				strcpy_s(strSrcPathName, NTL_MAX_DIR_PATH, m_NtlWorldParam2Merge.WorldProjectFolderName);
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, "swap\\eff\\");
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, SrcFileName);

				strcpy_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, "swap\\eff\\");
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

				DBO_TRACE(CopyFile(strSrcPathName, strDstPathName), "CNtlWorldMergeManager::MergeSwapFile, file copy failed.");
			}

			++IdxSectorVec;
		}
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

void CNtlWorldMergeManager::RefreshField(RwInt32 IdxField)
{
	// refresh fields
	vector<RwInt32> vecIdxFields;
	m_pFieldMgr->GetNeighborFields(IdxField, vecIdxFields);

	for(RwUInt32 i = 0; i < vecIdxFields.size(); ++i)
	{
		if(vecIdxFields[i] != -1)
		{
			m_pFieldMgr->RefreshField(vecIdxFields[i]);
		}
	}
	m_pFieldMgr->RefreshField(IdxField);

	// refresh slope lighting
	for(RwUInt32 i = 0; i < vecIdxFields.size(); ++i)
	{
		if(vecIdxFields[i] != -1)
		{
			m_pFieldMgr->OnSetSlopeLighting(static_cast<RwUInt32>(vecIdxFields[i]));
		}
	}
	m_pFieldMgr->OnSetSlopeLighting(static_cast<RwUInt32>(IdxField));
}

RwBool CNtlWorldMergeManager::Merge(sMERGE_PARAM &sParam, RwInt32 IdxFieldSrc, RwInt32 IdxFieldDst)
{
	//////////////////////////////////////////////////////////////////////////
	// Information Update
	//////////////////////////////////////////////////////////////////////////
	if (sParam.IsInformationUpdate())
	{
		MergeInformation(sParam, IdxFieldSrc, IdxFieldDst);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.IsWorldPropertyUpdate())
	{
		MergeProperty(sParam, IdxFieldSrc, IdxFieldDst);			
 	}

	//////////////////////////////////////////////////////////////////////////
	// Swap Data Update
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.IsSwapDataUpdate())
	{
		MergeSwapFile(sParam, IdxFieldSrc, IdxFieldDst);
	}

	if (sParam.IsWorldParamUpdate())
	{
		SaveCurWorldState();
	}

	//////////////////////////////////////////////////////////////////////////
	// Refresh Fields
	//////////////////////////////////////////////////////////////////////////	
	RefreshField(IdxFieldDst);

	return TRUE;
}


RwBool CNtlWorldMergeManager::IsFieldValid(RwInt32 IdxSField, RwInt32 IdxEField)
{
	// verify field indices
	if(!m_pFieldMgr->IsFieldValid(IdxSField))
	{
		DBO_TRACE(FALSE, "CNtlWorldMergeManager::IsFieldValid, invalid field index");
		return FALSE;
	}
	else
	{
		if(IdxEField != -1)
		{
			if(!m_pFieldMgr->IsFieldValid(IdxEField))
			{
				DBO_TRACE(FALSE, "CNtlWorldMergeManager::IsFieldValid, invalid field index");
				return FALSE;
			}
		}
	}
	return TRUE;
}

RwBool CNtlWorldMergeManager::OnReindexingObjectSerialID()
{
	if (dGET_WORLD_PARAM()->WorldType != 1)
	{
		return FALSE;
	}

	dGET_WORLD_PARAM()->WorldObjectSerialID = 1;

	FILE*			pFile;
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;
	RwChar			szOldPath[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(dGET_WORLD_PARAM()->WorldProjectFolderName);

	for(int i = 0; i < dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum; ++i)
	{
		// current field file name
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", i);
		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		_chdir("fields");
		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

		// export
		FileMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");

		if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			return FALSE;
		}

		pFileMemTmp = FileMem.GetDat();

		for(RwInt32 j = 0; j < dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldFieldSectorNum; ++j)
		{			
			// the number of doodads
			RwInt32 iNumObject = 0;
			CopyMemory(&iNumObject, pFileMemTmp, sizeof(RwInt32));
			::fwrite(&iNumObject, sizeof(RwInt32), 1, pFile);
			pFileMemTmp += sizeof(RwInt32);

			for(RwInt32 i = 0; i < iNumObject; ++i)
			{
				// Obj. name
				RwChar acName[128];
				CopyMemory(acName, pFileMemTmp, sizeof(RwChar) * 128);
				::fwrite(pFileMemTmp, sizeof(RwChar) * 128, 1, pFile);
				pFileMemTmp += sizeof(RwChar) * 128;

				// Scaling, Rotation, Translation
				::fwrite(pFileMemTmp, sizeof(RwV3d), 1, pFile);
				pFileMemTmp += sizeof(RwV3d);

				::fwrite(pFileMemTmp, sizeof(RwV3d), 1, pFile);
				pFileMemTmp += sizeof(RwV3d);

				RwV3d vPos;
				CopyMemory(&vPos, pFileMemTmp, sizeof(RwV3d));
				::fwrite(&vPos, sizeof(RwV3d), 1, pFile);
				pFileMemTmp += sizeof(RwV3d);				

				RwUInt32 uiObjectType;
				CopyMemory(&uiObjectType, pFileMemTmp, sizeof(RwUInt32));
				::fwrite(&uiObjectType, sizeof(RwUInt32), 1, pFile);
				pFileMemTmp += sizeof(RwUInt32);

				if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
					uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
				{
					pFileMemTmp = CNtlPLObjectTypeHasProperty::SaveIntoFileFromFileMem(pFile, pFileMemTmp);
				}

				// trigger obj serial id
				::fwrite(pFileMemTmp, sizeof(RwUInt32), 1, pFile);
				pFileMemTmp += sizeof(RwUInt32);

				if (dGET_WORLD_PARAM()->WorldType == 1)
				{
					// object serial id
					::fwrite(&dGET_WORLD_PARAM()->WorldObjectSerialID, sizeof(RwUInt32), 1, pFile);
					pFileMemTmp += sizeof(RwUInt32);
					dGET_WORLD_PARAM()->WorldObjectSerialID++;
				}

				// trigger obj object name index
				::fwrite(pFileMemTmp, sizeof(DWORD), 1, pFile);
				pFileMemTmp += sizeof(DWORD);

				// visibility distance
				::fwrite(pFileMemTmp, sizeof(RwReal), 1, pFile);
				pFileMemTmp += sizeof(RwReal);

				if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION_OLD))
				{
					// enable trigger
					::fwrite(pFileMemTmp, sizeof(RwBool), 1, pFile);
					pFileMemTmp += sizeof(RwBool);

					// enable selection
					::fwrite(pFileMemTmp, sizeof(RwBool), 1, pFile);
					pFileMemTmp += sizeof(RwBool);

					// enable alterable
					::fwrite(pFileMemTmp, sizeof(RwBool), 1, pFile);
					pFileMemTmp += sizeof(RwBool);

					// enable portal
					::fwrite(pFileMemTmp, sizeof(RwBool), 1, pFile);
					pFileMemTmp += sizeof(RwBool);

					// enable path engine collision
					::fwrite(pFileMemTmp, sizeof(RwBool), 1, pFile);
					pFileMemTmp += sizeof(RwBool);

					// enable ShadowProp
					::fwrite(pFileMemTmp, sizeof(RwBool), 1, pFile);
					pFileMemTmp += sizeof(RwBool);
				}
				else if (dNTL_WORLD_VERSION_COMPARE(dGET_WORLD_PARAM()->WorldVer, dNTL_WORLD_VERSION))
				{
					::fwrite(pFileMemTmp, sizeof(RwUInt32), 1, pFile);
					pFileMemTmp += (sizeof(RwUInt32));
				}			

				// doodads psm
				pFileMemTmp = CNtlPLObject::SavePSMapFromFileMem(pFile, pFileMemTmp);

				// milepost
				RwBool EnableFlg;
				CopyMemory(&EnableFlg, pFileMemTmp, sizeof(RwBool));
				fwrite(&EnableFlg, sizeof(RwBool), 1, pFile);
				pFileMemTmp += sizeof(RwBool);

				if(EnableFlg)
				{
					fwrite(pFileMemTmp, sizeof(RwUInt32), 1, pFile);
					pFileMemTmp += sizeof(RwUInt32);
				}

				// attach light object skip
				pFileMemTmp = CNtlPLObject::SaveLightObjectAttachDataFromFileMem(pFile, pFileMemTmp);

				if (dGET_WORLD_PARAM()->WorldType == 1)
				{
					pFileMemTmp = CNtlPLObject::SaveLightmapFromFileMem(pFile, pFileMemTmp);
				}
			}

			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWaterSaveIntoFileFromFileMem(pFile, pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShadowSaveIntoFileFromFileMem(pFile, pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorEffectSaveIntoFileFromFileMem(pFile, pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorSoundEffectSaveIntoFileFromFileMem(pFile, pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorCullSaveIntoFileFromFileMem(pFile, pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorTileTransparencySaveIntoFileFromFileMem(pFile, pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorShoreLineSaveIntoFileFromFileMem(pFile, pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDecalSaveIntoFileFromFileMem(pFile, pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorPlantSaveIntoFileFromFileMem(pFile, pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorWorldLightSaveIntoFileFromFileMem(pFile, pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorOccluderSaveIntoFileFromFileMem(pFile, pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSaveIntoFileFromFileMem(pFile, pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorLightObjectSaveIntoFileFromFileMem(pFile, pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDecalVolumeSaveIntoFileFromFileMem(pFile, pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorGamePropertySaveIntoFileFromFileMem(pFile, pFileMemTmp);
			pFileMemTmp = GetNtlWorldSectorInfo()->SectorDojoSaveIntoFileFromFileMem(pFile, pFileMemTmp);
		}

		FileMem.Free();
		fclose(pFile);
	}

	SaveCurWorldState();

	::SetCurrentDirectory(szOldPath);

	GetSceneManager()->GetWorld()->GetWorldFieldMgr()->RefreshFieldAll();

	return TRUE;
}

#endif

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

#ifdef dNTL_WORLD_TOOL_MODE


CNtlWorldMergeManager::CNtlWorldMergeManager(void)
{
}

CNtlWorldMergeManager::~CNtlWorldMergeManager(void)
{
}

CNtlWorldMergeManager* CNtlWorldMergeManager::GetInstance(VOID)
{
	static CNtlWorldMergeManager g_NtlWorldMergeManager;
	return &g_NtlWorldMergeManager;
}

RwBool CNtlWorldMergeManager::VerifyMergeWorld(const char* _pPathName)
{
	sNtlWorldParam NtlWorldParamSrc;
	CopyMemory(&NtlWorldParamSrc, dGET_WORLD_PARAM(), sizeof(sNtlWorldParam));


	if(strcmp(NtlWorldParamSrc.WorldVer, m_NtlWorldParam2Merge.WorldVer))
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldHeightfieldNum != m_NtlWorldParam2Merge.WorldHeightfieldNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSectorTileSize != m_NtlWorldParam2Merge.WorldSectorTileSize)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSectorTileNum != m_NtlWorldParam2Merge.WorldSectorTileNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSectorVertNum != m_NtlWorldParam2Merge.WorldSectorVertNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSectorSize != m_NtlWorldParam2Merge.WorldSectorSize)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSectorNum != m_NtlWorldParam2Merge.WorldSectorNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSectorHalfNum != m_NtlWorldParam2Merge.WorldSectorHalfNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSectorPolyNum != m_NtlWorldParam2Merge.WorldSectorPolyNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldFieldSectorNum != m_NtlWorldParam2Merge.WorldFieldSectorNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldFieldSize != m_NtlWorldParam2Merge.WorldFieldSize)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldFieldNum != m_NtlWorldParam2Merge.WorldFieldNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldFieldHalfNum != m_NtlWorldParam2Merge.WorldFieldHalfNum)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSize != m_NtlWorldParam2Merge.WorldSize)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldSizeHalf != m_NtlWorldParam2Merge.WorldSizeHalf)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldValueMax != m_NtlWorldParam2Merge.WorldValueMax)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldValueMin != m_NtlWorldParam2Merge.WorldValueMin)
	{
		return FALSE;
	}

	if(NtlWorldParamSrc.WorldTexAddr != m_NtlWorldParam2Merge.WorldTexAddr)
	{
		return FALSE;
	}
	/*
	if(strcmp(NtlWorldParamSrc.WorldRAW, m_NtlWorldParam2Merge.WorldRAW))
	{
	return FALSE;
	}
	*/
	// same project check
	RwChar BackSlashVerOfPathName[NTL_MAX_DIR_PATH];
	strcpy_s(BackSlashVerOfPathName, NTL_MAX_DIR_PATH, _pPathName);
	strcat_s(BackSlashVerOfPathName, "\\");

	if(!strcmp(dGET_WORLD_PARAM()->WorldProjectFolderName, BackSlashVerOfPathName))
	{
		return FALSE;
	}

	// set the folder path of the world 2 merge
	strcpy_s(m_NtlWorldParam2Merge.WorldProjectFolderName, 256, BackSlashVerOfPathName);

	return TRUE;
}

RwBool CNtlWorldMergeManager::Load(const char* _pPathName)
{
	_chdir(_pPathName);

	FILE* pFile;
	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&m_NtlWorldParam2Merge, pFile);
		fclose(pFile);
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	if(!VerifyMergeWorld(_pPathName))
	{
		DBO_TRACE(FALSE, "CNtlWorldMergeManager::Load, VerifyMergeWorld(" << _pPathName << ")");

		::MessageBox(NULL, "These two worlds can't be merged. Please try again.", "SAVER", MB_OK);

		return FALSE;
	}

	return TRUE;
}

RwBool CNtlWorldMergeManager::LoadSpawnDst(char* _pFileName)
{
	return m_SpawnMergeContainerDst.Load(_pFileName);
}

RwBool CNtlWorldMergeManager::LoadSpawnSrc(char* _pFileName)
{
	return m_SpawnMergeContainerSrc.Load(_pFileName);
}

RwBool CNtlWorldMergeManager::MergeSpawn(RwBBox bBoxDst, RwBBox bBoxSrc)
{
	if (m_SpawnMergeContainerDst.GetSpawnType() == -1 || m_SpawnMergeContainerSrc.GetSpawnType() == -1)
	{
		return FALSE;
	}

	if (m_SpawnMergeContainerDst.GetSpawnType() != m_SpawnMergeContainerSrc.GetSpawnType())
	{
		return FALSE;
	}

	m_SpawnMergeContainerDst.DeleteSpawn(bBoxDst);
	m_SpawnMergeContainerDst.InsertSpawn(&m_SpawnMergeContainerSrc, bBoxSrc, bBoxDst);
	m_SpawnMergeContainerDst.Save();

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnHeightfieldVariation(RwInt32 IdxField)
{
	// sector variables
	CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(m_pFieldMgr->GetFields());
	int				i, j;
	RwV3d			SPos = pFields[IdxField].GetSPos();
	RwV3d			SectorSPos, PosSectorS;

	// heightfield variables
	RwInt32				IdxSector;
	RwInt32				NumVert			= dGET_WORLD_PARAM()->WorldSectorVertNum * dGET_WORLD_PARAM()->WorldSectorVertNum;
	RwReal				TileSize		= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorTileSize);
	RwInt32				LenTile			= static_cast<RwInt32>(TileSize);
	RwReal				LenSector		= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize);
	RwReal				LenSectorHalf	= static_cast<RwReal>(dGET_WORLD_PARAM()->WorldSectorSize / 2.0f);
	RwInt32				NumTilePerSec	= dGET_WORLD_PARAM()->WorldSectorTileNum;
	sNTL_WORLD_SL*		pNtlWorldSL		= NULL;
	CNtlWorldSector*	pSectors		= m_pFieldMgr->CNtlWorldSectorManager::m_pSectors;
	RwV3d*				pPosVertList	= NTL_NEW RwV3d [NumVert];
	RwV3d				PosSectorDatum;
	RwV3d				CurVert;
	RwInt32				LenWidth, LenHeight;
	RwInt32				NumTileX, NumTileZ;
	RwV3d				PosNbrSecDatum[8];
	RwV3d				PosCurSecDatum;
	RwV3d				PosRetHeight[8];
	RwV3d				PosResult;
	RwReal				ValMergeHeight;
	RwBool				FlgBreak;


	// loop sectors
	for(i = static_cast<RwInt32>(SPos.z); i < static_cast<RwInt32>(SPos.z) + dGET_WORLD_PARAM()->WorldFieldSize; i += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(j = static_cast<RwInt32>(SPos.x); j < static_cast<RwInt32>(SPos.x) + dGET_WORLD_PARAM()->WorldFieldSize; j += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = static_cast<RwReal>(j);
			SectorSPos.z = static_cast<RwReal>(i);
			IdxSector = m_pFieldMgr->GetSectorIdx(SectorSPos);

			// exception
			if(IdxSector == -1)
			{
				DBO_TRACE(FALSE, "CNtlWorldMergeManager::OnHeightfieldVariation, invalid sector index,");
				continue;
			}

			CNtlMath::MathRwV3dAssign(&PosSectorDatum, pSectors[IdxSector].DatumPoint.x, pSectors[IdxSector].DatumPoint.y, pSectors[IdxSector].DatumPoint.z);
			if(!m_pFieldMgr->GetPosVertList(PosSectorDatum, pPosVertList))
			{
				DBO_TRACE(FALSE, "CNtlWorldMergeManager::OnHeightfieldVariation, GetPosVertList return failed.");
				continue;
			}

			// change to the sector starting pos.
			CNtlMath::MathRwV3dAssign(&PosSectorS, pSectors[IdxSector].DatumPoint.x - LenSectorHalf, 0.0f, pSectors[IdxSector].DatumPoint.z - LenSectorHalf);

			for(int i = 0; i < NumVert; ++i)
			{
				// current vert.
				CNtlMath::MathRwV3dAssign(&CurVert, pPosVertList[i].x, 0.0f, pPosVertList[i].z);

				// based on starting pos.
				LenWidth	= static_cast<RwInt32>(fabs(CurVert.x - PosSectorS.x));
				LenHeight	= static_cast<RwInt32>(fabs(CurVert.z - PosSectorS.z));
				NumTileX	= LenWidth / LenTile;
				NumTileZ	= LenHeight / LenTile;

				// This points should be the edge vertices, LenSRadiusMax was calculated to include only the edge vertices.
				if(!NumTileX || NumTileX == NumTilePerSec || !NumTileZ || NumTileZ == NumTilePerSec)
				{
					// 3---2---1
					// |       | 
					// 4  cur  0
					// |       |
					// 5---6---7

					// classify vertices
					if(!NumTileX && (NumTileZ > 0 && NumTileZ < NumTilePerSec))
					{
						// 0
						CNtlMath::MathRwV3dAssign(&PosCurSecDatum, CurVert.x + TileSize, 0.0f, CurVert.z);
						if(m_pFieldMgr->GetHeightFromFile(PosCurSecDatum, PosCurSecDatum))
						{
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], pSectors[IdxSector].DatumPoint.x - LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z);
							CNtlMath::MathRwV3dAssign(&PosRetHeight[0], CurVert.x - TileSize, 0.0f, CurVert.z);
							if(m_pFieldMgr->GetHeightFromFile(PosNbrSecDatum[0], PosRetHeight[0]))
							{
								ValMergeHeight	= (PosCurSecDatum.y + PosRetHeight[0].y) / 2.0f;
								PosResult		= CurVert;
								PosResult.y		= ValMergeHeight;

								m_pFieldMgr->SetHeight(PosCurSecDatum, PosResult);
								m_pFieldMgr->SetHeight(PosNbrSecDatum[0], PosResult);
							}
						}
					}
					else if(!NumTileX && NumTileZ == NumTilePerSec)
					{
						// 1
						CNtlMath::MathRwV3dAssign(&PosCurSecDatum, CurVert.x + TileSize, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], CurVert.x - TileSize, 0.0f, CurVert.z);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[1], CurVert.x - TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[2], CurVert.x, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[3], CurVert.x + TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[4], CurVert.x + TileSize, 0.0f, CurVert.z);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[5], CurVert.x + TileSize, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[6], CurVert.x, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[7], CurVert.x - TileSize, 0.0f, CurVert.z - TileSize);

						CopyMemory(PosRetHeight, PosNbrSecDatum, sizeof(RwV3d) * 8);

						FlgBreak = FALSE;
						for(RwInt32 i = 0; i < 8; ++i)
						{
							if(!m_pFieldMgr->GetHeightFromFile(PosNbrSecDatum[i], PosRetHeight[i]))
							{
								FlgBreak = TRUE;
								break;
							}
						}

						if(!FlgBreak)
						{
							ValMergeHeight = 0.0f;

							for(RwInt32 i = 0; i < 8; ++i)
							{
								ValMergeHeight += PosRetHeight[i].y;
							}

							ValMergeHeight /= 8.0f;
							PosResult		= CurVert;
							PosResult.y		= ValMergeHeight;

							// set values for each sector
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], pSectors[IdxSector].DatumPoint.x - LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z);
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[1], pSectors[IdxSector].DatumPoint.x - LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z + LenSector);
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[2], pSectors[IdxSector].DatumPoint.x, 0.0f, pSectors[IdxSector].DatumPoint.z + LenSector);
							m_pFieldMgr->SetHeight(PosCurSecDatum, PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[0], PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[1], PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[2], PosResult);
						}
					}
					else if((NumTileX > 0 && NumTileX < NumTilePerSec) && NumTileZ == NumTilePerSec)
					{
						// 2
						CNtlMath::MathRwV3dAssign(&PosCurSecDatum, CurVert.x, 0.0f, CurVert.z - TileSize);
						if(m_pFieldMgr->GetHeightFromFile(PosCurSecDatum, PosCurSecDatum))
						{
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], pSectors[IdxSector].DatumPoint.x, 0.0f, pSectors[IdxSector].DatumPoint.z + LenSector);
							CNtlMath::MathRwV3dAssign(&PosRetHeight[0], CurVert.x, 0.0f, CurVert.z + TileSize);
							if(m_pFieldMgr->GetHeightFromFile(PosNbrSecDatum[0], PosRetHeight[0]))
							{
								ValMergeHeight	= (PosCurSecDatum.y + PosRetHeight[0].y) / 2.0f;
								PosResult		= CurVert;
								PosResult.y		= ValMergeHeight;

								m_pFieldMgr->SetHeight(PosCurSecDatum, PosResult);
								m_pFieldMgr->SetHeight(PosNbrSecDatum[0], PosResult);
							}
						}
					}
					else if(NumTileX == NumTilePerSec && NumTileZ == NumTilePerSec)
					{
						// 3
						CNtlMath::MathRwV3dAssign(&PosCurSecDatum, CurVert.x - TileSize, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], CurVert.x - TileSize, 0.0f, CurVert.z);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[1], CurVert.x - TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[2], CurVert.x, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[3], CurVert.x + TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[4], CurVert.x + TileSize, 0.0f, CurVert.z);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[5], CurVert.x + TileSize, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[6], CurVert.x, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[7], CurVert.x - TileSize, 0.0f, CurVert.z - TileSize);

						CopyMemory(PosRetHeight, PosNbrSecDatum, sizeof(RwV3d) * 8);

						FlgBreak = FALSE;
						for(RwInt32 i = 0; i < 8; ++i)
						{
							if(!m_pFieldMgr->GetHeightFromFile(PosNbrSecDatum[i], PosRetHeight[i]))
							{
								FlgBreak = TRUE;
								break;
							}
						}

						if(!FlgBreak)
						{
							ValMergeHeight = 0.0f;

							for(RwInt32 i = 0; i < 8; ++i)
							{
								ValMergeHeight += PosRetHeight[i].y;
							}

							ValMergeHeight /= 8.0f;
							PosResult		= CurVert;
							PosResult.y		= ValMergeHeight;

							// set values for each sector
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], pSectors[IdxSector].DatumPoint.x, 0.0f, pSectors[IdxSector].DatumPoint.z + LenSector);
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[1], pSectors[IdxSector].DatumPoint.x + LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z + LenSector);
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[2], pSectors[IdxSector].DatumPoint.x + LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z);
							m_pFieldMgr->SetHeight(PosCurSecDatum, PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[0], PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[1], PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[2], PosResult);
						}
					}
					else if(NumTileX == NumTilePerSec && (NumTileZ > 0 && NumTileZ < NumTilePerSec))
					{
						// 4
						CNtlMath::MathRwV3dAssign(&PosCurSecDatum, CurVert.x - TileSize, 0.0f, CurVert.z);
						if(m_pFieldMgr->GetHeightFromFile(PosCurSecDatum, PosCurSecDatum))
						{
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], pSectors[IdxSector].DatumPoint.x + LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z);
							CNtlMath::MathRwV3dAssign(&PosRetHeight[0], CurVert.x + TileSize, 0.0f, CurVert.z);
							if(m_pFieldMgr->GetHeightFromFile(PosNbrSecDatum[0], PosRetHeight[0]))
							{
								ValMergeHeight	= (PosCurSecDatum.y + PosRetHeight[0].y) / 2.0f;
								PosResult		= CurVert;
								PosResult.y		= ValMergeHeight;

								m_pFieldMgr->SetHeight(PosCurSecDatum, PosResult);
								m_pFieldMgr->SetHeight(PosNbrSecDatum[0], PosResult);
							}
						}
					}
					else if(NumTileX == NumTilePerSec && !NumTileZ)
					{
						// 5
						CNtlMath::MathRwV3dAssign(&PosCurSecDatum, CurVert.x - TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], CurVert.x - TileSize, 0.0f, CurVert.z);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[1], CurVert.x - TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[2], CurVert.x, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[3], CurVert.x + TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[4], CurVert.x + TileSize, 0.0f, CurVert.z);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[5], CurVert.x + TileSize, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[6], CurVert.x, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[7], CurVert.x - TileSize, 0.0f, CurVert.z - TileSize);

						CopyMemory(PosRetHeight, PosNbrSecDatum, sizeof(RwV3d) * 8);

						FlgBreak = FALSE;
						for(RwInt32 i = 0; i < 8; ++i)
						{
							if(!m_pFieldMgr->GetHeightFromFile(PosNbrSecDatum[i], PosRetHeight[i]))
							{
								FlgBreak = TRUE;
								break;
							}
						}

						if(!FlgBreak)
						{
							ValMergeHeight = 0.0f;

							for(RwInt32 i = 0; i < 8; ++i)
							{
								ValMergeHeight += PosRetHeight[i].y;
							}

							ValMergeHeight /= 8.0f;
							PosResult		= CurVert;
							PosResult.y		= ValMergeHeight;

							// set values for each sector
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], pSectors[IdxSector].DatumPoint.x + LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z);
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[1], pSectors[IdxSector].DatumPoint.x + LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z - LenSector);
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[2], pSectors[IdxSector].DatumPoint.x, 0.0f, pSectors[IdxSector].DatumPoint.z - LenSector);
							m_pFieldMgr->SetHeight(PosCurSecDatum, PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[0], PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[1], PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[2], PosResult);
						}
					}
					else if((NumTileX > 0 && NumTileX < NumTilePerSec) && !NumTileZ)
					{
						// 6
						CNtlMath::MathRwV3dAssign(&PosCurSecDatum, CurVert.x, 0.0f, CurVert.z + TileSize);
						if(m_pFieldMgr->GetHeightFromFile(PosCurSecDatum, PosCurSecDatum))
						{
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], pSectors[IdxSector].DatumPoint.x, 0.0f, pSectors[IdxSector].DatumPoint.z - LenSector);
							CNtlMath::MathRwV3dAssign(&PosRetHeight[0], CurVert.x, 0.0f, CurVert.z - TileSize);
							if(m_pFieldMgr->GetHeightFromFile(PosNbrSecDatum[0], PosRetHeight[0]))
							{
								ValMergeHeight	= (PosCurSecDatum.y + PosRetHeight[0].y) / 2.0f;
								PosResult		= CurVert;
								PosResult.y		= ValMergeHeight;

								m_pFieldMgr->SetHeight(PosCurSecDatum, PosResult);
								m_pFieldMgr->SetHeight(PosNbrSecDatum[0], PosResult);
							}
						}
					}
					else if(!NumTileX && !NumTileZ)
					{
						// 7
						CNtlMath::MathRwV3dAssign(&PosCurSecDatum, CurVert.x + TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], CurVert.x - TileSize, 0.0f, CurVert.z);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[1], CurVert.x - TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[2], CurVert.x, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[3], CurVert.x + TileSize, 0.0f, CurVert.z + TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[4], CurVert.x + TileSize, 0.0f, CurVert.z);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[5], CurVert.x + TileSize, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[6], CurVert.x, 0.0f, CurVert.z - TileSize);
						CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[7], CurVert.x - TileSize, 0.0f, CurVert.z - TileSize);

						CopyMemory(PosRetHeight, PosNbrSecDatum, sizeof(RwV3d) * 8);

						FlgBreak = FALSE;
						for(RwInt32 i = 0; i < 8; ++i)
						{
							if(!m_pFieldMgr->GetHeightFromFile(PosNbrSecDatum[i], PosRetHeight[i]))
							{
								FlgBreak = TRUE;
								break;
							}
						}

						if(!FlgBreak)
						{
							ValMergeHeight = 0.0f;

							for(RwInt32 i = 0; i < 8; ++i)
							{
								ValMergeHeight += PosRetHeight[i].y;
							}

							ValMergeHeight /= 8.0f;
							PosResult		= CurVert;
							PosResult.y		= ValMergeHeight;

							// set values for each sector
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[0], pSectors[IdxSector].DatumPoint.x, 0.0f, pSectors[IdxSector].DatumPoint.z - LenSector);
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[1], pSectors[IdxSector].DatumPoint.x - LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z - LenSector);
							CNtlMath::MathRwV3dAssign(&PosNbrSecDatum[2], pSectors[IdxSector].DatumPoint.x - LenSector, 0.0f, pSectors[IdxSector].DatumPoint.z);
							m_pFieldMgr->SetHeight(PosCurSecDatum, PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[0], PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[1], PosResult);
							m_pFieldMgr->SetHeight(PosNbrSecDatum[2], PosResult);
						}
					}
				}
			}
		}
	}

	NTL_ARRAY_DELETE(pPosVertList);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportMapObj(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	/*
	WorldFileFormat - Export
	Export 처리를 위한 부분이다.
	*/

	sNtlWorldParam	sTmpPrjParam;
	FILE*			pFile;
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;
	RwChar			szFFPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar			szOldPath[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sTmpPrjParam, pFile);
		fclose(pFile);
	}

	strcpy_s(sTmpPrjParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	for(int i = 0; i < sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum; ++i)
	{
		// current field file name
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);

		// name the file for exporting
		strcpy_s(szFFPathName, NTL_MAX_DIR_PATH, _pPathName4Exporting);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "\\");
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, sTmpPrjParam.WorldChar64Buf);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, ".mobj");
		if(fopen_s(&pFile, szFFPathName, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << szFFPathName << ")");
			return FALSE;
		}

		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sTmpPrjParam.WorldChar64Buf);

		// export
		FileMem.Load(sTmpPrjParam.WorldChar64Buf, "rb");

		pFileMemTmp = FileMem.GetDat();
		pFileMemTmp = GetSPosOfMaterial(FileMem.GetDat());

		for(RwInt32 j = 0; j < sTmpPrjParam.WorldFieldSectorNum * sTmpPrjParam.WorldFieldSectorNum; ++j)
		{
			pFileMemTmp = Skip_Material(pFileMemTmp);
			pFileMemTmp = Skip_Heightfield(pFileMemTmp);
			pFileMemTmp = Skip_Diffuse(pFileMemTmp);
			pFileMemTmp = Save_Doodads(pFileMemTmp, pFile);
			pFileMemTmp = Skip_Water(pFileMemTmp);
			pFileMemTmp = Skip_Shadow(pFileMemTmp);
			pFileMemTmp = Skip_Effects(pFileMemTmp);
			pFileMemTmp = Skip_SE(pFileMemTmp);
			// CZ-SS
			//pFileMemTmp = Skip_SS(pFileMemTmp);
			pFileMemTmp = Skip_SectorCull(pFileMemTmp);
			pFileMemTmp = Skip_TileTransparency(pFileMemTmp);
			pFileMemTmp = Skip_Shoreline(pFileMemTmp);
			pFileMemTmp = Skip_Decal(pFileMemTmp);
			pFileMemTmp = Skip_Plant(pFileMemTmp);
			pFileMemTmp = Skip_WorldLight(pFileMemTmp);
			pFileMemTmp = Skip_Occluder(pFileMemTmp);
			pFileMemTmp = Skip_HeatHazeObject(pFileMemTmp);
			pFileMemTmp = Skip_LightObject(pFileMemTmp);
			pFileMemTmp = Skip_Dojo(pFileMemTmp);
		}

		FileMem.Free();
		fclose(pFile);

		_chdir("../");
	}

	::SetCurrentDirectory(szOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportMapName(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	/*
	WorldFileFormat - ExportMap
	*/

	sNtlWorldParam	sTmpPrjParam;
	FILE*			pFile;
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;
	RwChar			szFFPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar			szOldPath[NTL_MAX_DIR_PATH]	= {0,};
	RwChar			szWFPFName[32]			= {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sTmpPrjParam, pFile);
		fclose(pFile);
	}

	strcpy_s(sTmpPrjParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	// current field file name
	strcpy_s(szFFPathName, NTL_MAX_DIR_PATH, _pPathName4Exporting);
	strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "\\");
	strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "MapNameChunk");
	FILE* pChunkFile;
	if(fopen_s(&pChunkFile, szFFPathName, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << szFFPathName << ")");
		return FALSE;
	}

	// count
	RwInt32 Num = sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum;
	fwrite(&Num, 1, sizeof(RwInt32), pChunkFile);

	// data
	for(int i = 0; i < sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum; ++i)
	{
		// current field file name
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);
		sprintf_s(szWFPFName, 32, "wfpf%d", i);

		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sTmpPrjParam.WorldChar64Buf);

		// export
		FileMem.Load(szWFPFName, "rb");
		pFileMemTmp = FileMem.GetDat();

		RwChar CurMapName[64];
		CopyMemory(CurMapName, pFileMemTmp, 64);
		TBLIDX CurTblIdx = static_cast<TBLIDX>(atoi(CurMapName));
		fwrite(&CurTblIdx, sizeof(TBLIDX), 1, pChunkFile);

		FileMem.Free();

		_chdir("../");
	}

	fclose(pChunkFile);

	::SetCurrentDirectory(szOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportMapProp(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	/*
	WorldFileFormat - ExportMap
	*/

	sNtlWorldParam	sTmpPrjParam;
	FILE*			pFile;
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;
	RwChar			szFFPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar			szOldPath[NTL_MAX_DIR_PATH]	= {0,};
	RwChar			szWFPFName[32]			= {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sTmpPrjParam, pFile);
		fclose(pFile);
	}

	strcpy_s(sTmpPrjParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	// current field file name
	strcpy_s(szFFPathName, NTL_MAX_DIR_PATH, _pPathName4Exporting);
	strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "\\");
	strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "MapPropChunk");
	FILE* pChunkFile;
	if(fopen_s(&pChunkFile, szFFPathName, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << szFFPathName << ")");
		return FALSE;
	}

	// count
	RwInt32 Num = sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum;
	fwrite(&Num, 1, sizeof(RwInt32), pChunkFile);

	// data
	for(int i = 0; i < sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum; ++i)
	{
		// current field file name
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);
		sprintf_s(szWFPFName, 32, "wfpf%d", i);

		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sTmpPrjParam.WorldChar64Buf);

		// export
		FileMem.Load(szWFPFName, "rb");
		pFileMemTmp = FileMem.GetDat();
		pFileMemTmp = Skip_Name(pFileMemTmp);
		pFileMemTmp = Save_BitProp(pFileMemTmp, pChunkFile);
		FileMem.Free();

		_chdir("../");
	}

	fclose(pChunkFile);

	::SetCurrentDirectory(szOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportHeightfield(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	/*
	WorldFileFormat - Export
	Export 처리를 위한 부분이다.
	*/

	sNtlWorldParam	sTmpPrjParam;
	FILE*			pFile;
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;
	RwChar			szFFPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar			szOldPath[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sTmpPrjParam, pFile);
		fclose(pFile);
	}

	strcpy_s(sTmpPrjParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	for(int i = 0; i < sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum; ++i)
	{
		// current field file name
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);

		// name the file for exporting
		strcpy_s(szFFPathName, NTL_MAX_DIR_PATH, _pPathName4Exporting);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "\\");
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, sTmpPrjParam.WorldChar64Buf);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, ".vert");
		if(fopen_s(&pFile, szFFPathName, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << szFFPathName << ")");
			return FALSE;
		}

		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sTmpPrjParam.WorldChar64Buf);

		// export
		FileMem.Load(sTmpPrjParam.WorldChar64Buf, "rb");

		pFileMemTmp = FileMem.GetDat();
		pFileMemTmp = GetSPosOfMaterial(FileMem.GetDat());

		for(RwInt32 j = 0; j < sTmpPrjParam.WorldFieldSectorNum * sTmpPrjParam.WorldFieldSectorNum; ++j)
		{
			pFileMemTmp = Skip_Material(pFileMemTmp);
			pFileMemTmp = Save_Heightfield(pFileMemTmp, pFile);
			pFileMemTmp = Skip_Diffuse(pFileMemTmp);
			pFileMemTmp = Skip_Doodads(pFileMemTmp);
			pFileMemTmp = Skip_Water(pFileMemTmp);
			pFileMemTmp = Skip_Shadow(pFileMemTmp);
			pFileMemTmp = Skip_Effects(pFileMemTmp);
			pFileMemTmp = Skip_SE(pFileMemTmp);
			// CZ-SS
			//pFileMemTmp = Skip_SS(pFileMemTmp);
			pFileMemTmp = Skip_SectorCull(pFileMemTmp);
			pFileMemTmp = Skip_TileTransparency(pFileMemTmp);
			pFileMemTmp = Skip_Shoreline(pFileMemTmp);
			pFileMemTmp = Skip_Decal(pFileMemTmp);
			pFileMemTmp = Skip_Plant(pFileMemTmp);
			pFileMemTmp = Skip_WorldLight(pFileMemTmp);
			pFileMemTmp = Skip_Occluder(pFileMemTmp);
			pFileMemTmp = Skip_HeatHazeObject(pFileMemTmp);
			pFileMemTmp = Skip_LightObject(pFileMemTmp);
			pFileMemTmp	= Skip_Dojo(pFileMemTmp);
		}

		FileMem.Free();
		fclose(pFile);

		_chdir("../");
	}

	::SetCurrentDirectory(szOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportHeightfieldPE(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	/*
	WorldFileFormat - ExportMap
	*/

	sNtlWorldParam	sTmpPrjParam;
	FILE*			pFile;
	CFileMem		FileMem;
	//BYTE*			pFileMemTmp;
	RwChar			szFFPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar			szOldPath[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sTmpPrjParam, pFile);
		fclose(pFile);
	}

	strcpy_s(sTmpPrjParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	for(int i = 0; i < sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum; ++i)
	{
		// name the file for exporting
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);

		strcpy_s(szFFPathName, NTL_MAX_DIR_PATH, _pPathName4Exporting);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "\\");
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, sTmpPrjParam.WorldChar64Buf);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, ".peh");
		if(fopen_s(&pFile, szFFPathName, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << szFFPathName << ")");
			return FALSE;
		}

		RwUInt32	uiTemp;
		RwV3d		vTemp;

		//////////////////////////////////////////////////////////////////////////
		// 
		//////////////////////////////////////////////////////////////////////////
		// idx
		uiTemp = i;
		fwrite(&uiTemp, sizeof(RwUInt32), 1, pFile);

		// aabb
		vTemp.z = static_cast<RwReal>(((i / sTmpPrjParam.WorldFieldNum) * sTmpPrjParam.WorldFieldSize) - sTmpPrjParam.WorldSizeHalf);
		vTemp.y = 0.0f;
		vTemp.x = static_cast<RwReal>(((i % sTmpPrjParam.WorldFieldNum) * sTmpPrjParam.WorldFieldSize) - sTmpPrjParam.WorldSizeHalf);
		fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

		vTemp.z = static_cast<RwReal>(vTemp.z + sTmpPrjParam.WorldFieldSize);
		vTemp.y = 0.0f;
		vTemp.x = static_cast<RwReal>(vTemp.x + sTmpPrjParam.WorldFieldSize);
		fwrite(&vTemp, sizeof(RwV3d), 1, pFile);

		//////////////////////////////////////////////////////////////////////////
		// filed data
		//////////////////////////////////////////////////////////////////////////
		// 		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);
		// 
		// 		_chdir(sTmpPrjParam.WorldProjectFolderName);
		// 		_chdir("fields");
		// 		_chdir(sTmpPrjParam.WorldChar64Buf);
		// 
		// 		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfpf%d", i);
		// 
		// 		FileMem.Load(sTmpPrjParam.WorldChar64Buf, "rb");
		// 
		// 		pFileMemTmp = FileMem.GetDat();
		// 
		// 		RwChar CurMapName[64];
		// 		CopyMemory(CurMapName, pFileMemTmp, 64);
		// 		TBLIDX CurTblIdx = static_cast<TBLIDX>(atoi(CurMapName));
		// 		fwrite(&CurTblIdx, sizeof(TBLIDX), 1, pFile);
		// 
		// 		FileMem.Free();

		//////////////////////////////////////////////////////////////////////////
		// sector data
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// swap data
		//////////////////////////////////////////////////////////////////////////
		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("swap");
		_chdir("pe");

		FILE* pTempFile;
		if(fopen_s(&pTempFile, "header.pe", "rb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << "header.pe" << ")");
			return FALSE;
		}

		RwInt32 iTemp;
		fread(&iTemp, sizeof(RwInt32), 1, pTempFile);
		fclose(pTempFile);

		// ChunkSize
		uiTemp = iTemp;
		fwrite(&uiTemp, sizeof(RwUInt32), 1, pFile);

		uiTemp = (sTmpPrjParam.WorldSectorSize / iTemp) * (sTmpPrjParam.WorldSectorSize / iTemp);

		// total count of path tile in a sector
		fwrite(&uiTemp, sizeof(RwUInt32), 1, pFile);

		// error
		RwReal fError = RwRealMAXVAL;
		fwrite(&fError, sizeof(RwReal), 1, pFile);

		vTemp.x = static_cast<RwReal>((i % sTmpPrjParam.WorldFieldNum) * sTmpPrjParam.WorldFieldSize);
		vTemp.z = static_cast<RwReal>((i / sTmpPrjParam.WorldFieldNum) * sTmpPrjParam.WorldFieldSize);

		RwV3d vCount;
		for (vCount.z = vTemp.z; vCount.z < vTemp.z + sTmpPrjParam.WorldFieldSize; vCount.z += sTmpPrjParam.WorldSectorSize)
		{
			for (vCount.x = vTemp.x; vCount.x < vTemp.x + sTmpPrjParam.WorldFieldSize; vCount.x += sTmpPrjParam.WorldSectorSize)
			{
				RwInt32 iWidthNum = (int)(vCount.x / (float)sTmpPrjParam.WorldSectorSize);
				RwInt32 iDepthNum = (int)(vCount.z / (float)sTmpPrjParam.WorldSectorSize); 

				RwInt32 RetIdx = iWidthNum + (iDepthNum * sTmpPrjParam.WorldSectorNum);

				sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "%d_sec.pe", RetIdx);

				FILE* pTempFile;
				if(fopen_s(&pTempFile, sTmpPrjParam.WorldChar64Buf, "rb"))
				{
					DBO_TRACE(FALSE, "file open failed. (" << sTmpPrjParam.WorldChar64Buf << ")");
					return FALSE;
				}

				RwReal* pfHeight = NTL_NEW RwReal [uiTemp];

				fread(pfHeight, sizeof(RwReal) * uiTemp, 1, pTempFile);

				fclose(pTempFile);

				fwrite(pfHeight, sizeof(RwReal) * uiTemp, 1, pFile);

				NTL_DELETE(pfHeight);
			}
		}

		fclose(pFile);
	}

	::SetCurrentDirectory(szOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportMapPropPE(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	/*
	WorldFileFormat - ExportMap
	*/

	sNtlWorldParam	sTmpPrjParam;
	FILE*			pFile;
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;
	RwChar			szFFPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar			szOldPath[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sTmpPrjParam, pFile);
		fclose(pFile);
	}

	strcpy_s(sTmpPrjParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	for(int i = 0; i < sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum; ++i)
	{
		// name the file for exporting
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);

		strcpy_s(szFFPathName, NTL_MAX_DIR_PATH, _pPathName4Exporting);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "\\");
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, sTmpPrjParam.WorldChar64Buf);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, ".pep");
		if(fopen_s(&pFile, szFFPathName, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << szFFPathName << ")");
			return FALSE;
		}

		RwUInt32	uiTemp;
		//////////////////////////////////////////////////////////////////////////
		// 
		//////////////////////////////////////////////////////////////////////////
		// idx
		uiTemp = i;
		fwrite(&uiTemp, sizeof(RwUInt32), 1, pFile);

		//////////////////////////////////////////////////////////////////////////
		// filed data
		//////////////////////////////////////////////////////////////////////////
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);

		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sTmpPrjParam.WorldChar64Buf);

		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfpf%d", i);

		FileMem.Load(sTmpPrjParam.WorldChar64Buf, "rb");

		pFileMemTmp = FileMem.GetDat();

		RwChar CurMapName[64];
		CopyMemory(CurMapName, pFileMemTmp, 64);
		TBLIDX CurTblIdx = static_cast<TBLIDX>(atoi(CurMapName));
		fwrite(&CurTblIdx, sizeof(TBLIDX), 1, pFile);

		DWORD* pBitProp = NTL_NEW DWORD [(sTmpPrjParam.WorldSectorTileNum * 2) * (sTmpPrjParam.WorldSectorTileNum * 2)];

		pFileMemTmp = FileMem.GetDat();
		pFileMemTmp = Skip_Name(pFileMemTmp);

		CopyMemory(pBitProp, pFileMemTmp, sizeof(DWORD) * (sTmpPrjParam.WorldSectorTileNum * 2) * (sTmpPrjParam.WorldSectorTileNum * 2));

		FileMem.Free();

		//////////////////////////////////////////////////////////////////////////
		// sector data
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////
		// swap data
		//////////////////////////////////////////////////////////////////////////
		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("swap");
		_chdir("pe");

		FILE* pTempFile;
		if(fopen_s(&pTempFile, "header.pe", "rb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << "header.pe" << ")");
			return FALSE;
		}

		RwInt32 iTemp;
		fread(&iTemp, sizeof(RwInt32), 1, pTempFile);
		fclose(pTempFile);

		// ChunkSize
		uiTemp = iTemp;
		fwrite(&uiTemp, sizeof(RwUInt32), 1, pFile);

		uiTemp = (sTmpPrjParam.WorldFieldSize / iTemp) * (sTmpPrjParam.WorldFieldSize / iTemp);

		// total count of path tile in a field
		fwrite(&uiTemp, sizeof(RwUInt32), 1, pFile);

		// 여기서 위에서 읽은 pBitProp을 fwrite 해준다. Chunk 크기에 따라 갯수를 늘려야 한다.

		for (int e = 0; e < (sTmpPrjParam.WorldSectorTileNum * 2); ++e)
		{
			for (int w = 0; w < sTmpPrjParam.WorldSectorTileSize / iTemp; ++w)
			{
				for (int r = 0; r < (sTmpPrjParam.WorldSectorTileNum * 2); ++r)
				{
					for (int q = 0; q < sTmpPrjParam.WorldSectorTileSize / iTemp; ++q)
					{
						fwrite(&pBitProp[r + (e * (sTmpPrjParam.WorldSectorTileNum * 2))], sizeof(DWORD), 1, pFile);
					}
				}
			}
		}

		NTL_DELETE(pBitProp);

		fclose(pFile);
	}

	::SetCurrentDirectory(szOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportPathEngineOutDoor(const char* _pPathNameOfProject, RwUInt32 uiWorldID, const char* _pPathName4Exporting,
														const char* _pWorldName, const char* _pAgency, RwInt32 _iGroupFieldNum, RwInt32 _iOverlapSize)
{
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;

	RwChar			acTempPath[NTL_MAX_DIR_PATH] = {0,};
	RwChar			acOldPath[NTL_MAX_DIR_PATH] = {0,};
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	sNtlWorldParam	sWorldParam;
	FILE*			pFile;
	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sWorldParam, pFile);
		fclose(pFile);
	}
	strcpy_s(sWorldParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	// Swap Header Load
	RwInt32 iTileSize;

	_chdir(sWorldParam.WorldProjectFolderName);
	_chdir("swap");
	_chdir("pe");
	if(fopen_s(&pFile, "header.pe", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "header.pe" << ")");
	}
	else
	{
		fread(&iTileSize, sizeof(RwInt32), 1, pFile);
		fclose(pFile);
	}

	RwInt32 iSectorTileCnt	= (sWorldParam.WorldSectorSize / iTileSize) * (sWorldParam.WorldSectorSize / iTileSize);
	RwInt32 iFieldTileCnt	= iSectorTileCnt * (sWorldParam.WorldFieldSectorNum * sWorldParam.WorldFieldSectorNum);

	CNtlNaviDataMng				NtlNaviDataMgr;	NtlNaviDataMgr.Create(_pPathName4Exporting);
	CNtlNaviWorldOutDoorInfo*	pNtlNaviWorldOutDoorInfo = NtlNaviDataMgr.Create_WorldOutDoor();

	pNtlNaviWorldOutDoorInfo->SetWorldID(uiWorldID);
	pNtlNaviWorldOutDoorInfo->SetWorldName(_pWorldName);
	pNtlNaviWorldOutDoorInfo->SetWorldMinPos((float)-sWorldParam.WorldSizeHalf, (float)-sWorldParam.WorldSizeHalf);
	pNtlNaviWorldOutDoorInfo->SetWorldMaxPos((float)sWorldParam.WorldSizeHalf, (float)sWorldParam.WorldSizeHalf);
	pNtlNaviWorldOutDoorInfo->SetTileSize((float)iTileSize);
	pNtlNaviWorldOutDoorInfo->SetFieldSize((float)sWorldParam.WorldFieldSize);
	pNtlNaviWorldOutDoorInfo->SetCrossFieldCntOfGroup(_iGroupFieldNum);
	pNtlNaviWorldOutDoorInfo->SetOverlapSize((RwReal)_iOverlapSize);


	std::vector<std::string> vecString;
	StringTokenize(vecString, std::string(_pAgency), std::string(","));
	for (int i = 0; i < (int)vecString.size(); ++i)
	{
		pNtlNaviWorldOutDoorInfo->AttachAgency(::atof(vecString.at(i).c_str()));
	}

	for (int iZ = 0; iZ < sWorldParam.WorldFieldNum; iZ += _iGroupFieldNum)
	{
		for (int iX = 0; iX < sWorldParam.WorldFieldNum; iX += _iGroupFieldNum)
		{
			CNtlNaviGroupOutDoorInfo*	pNtlNaviGroupOutDoorInfo	= NtlNaviDataMgr.Create_GroupOutDoor();
			RwInt32						iGroupIdx					= ((iZ / _iGroupFieldNum) * (sWorldParam.WorldFieldNum / _iGroupFieldNum)) + (iX / _iGroupFieldNum);

			pNtlNaviGroupOutDoorInfo->SetGroupID(iGroupIdx);
			pNtlNaviGroupOutDoorInfo->SetInvalidHeightVal(RwRealMAXVAL);
			pNtlNaviGroupOutDoorInfo->SetFieldCntOfGroup(_iGroupFieldNum * _iGroupFieldNum);
			pNtlNaviGroupOutDoorInfo->SetTileCntOfSector(iSectorTileCnt);

			for (int iGZ = iZ; iGZ < iZ + _iGroupFieldNum; ++iGZ)
			{
				for (int iGX = iX; iGX < iX + _iGroupFieldNum; ++iGX)
				{
					CNtlNaviPropOutDoorInfo*	pNtlNaviPropOutDoorInfo	= NtlNaviDataMgr.Create_PropOutDoor();
					RwInt32						iFieldIdx				= iGZ * sWorldParam.WorldFieldNum + iGX;

					pNtlNaviPropOutDoorInfo->SetFieldID(iFieldIdx);
					pNtlNaviPropOutDoorInfo->SetTileCntOfField(iFieldTileCnt);

					// FIELD FILE OPEN
					sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d", iFieldIdx);
					_chdir(sWorldParam.WorldProjectFolderName);
					_chdir("fields");
					_chdir(sWorldParam.WorldChar64Buf);
					sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfpf%d", iFieldIdx);

					FileMem.Load(sWorldParam.WorldChar64Buf, "rb");

					pFileMemTmp = FileMem.GetDat();

					// NAME
					RwChar CurMapName[64];
					CopyMemory(CurMapName, pFileMemTmp, 64);
					pNtlNaviPropOutDoorInfo->SetFieldProp(atoi(CurMapName));
					pFileMemTmp = Skip_Name(pFileMemTmp);

					// TILE PROP
					DWORD* pBitProp = NTL_NEW DWORD [(sWorldParam.WorldSectorTileNum * sWorldParam.WorldSectorTileNum) * (sWorldParam.WorldFieldSectorNum * sWorldParam.WorldFieldSectorNum)];
					CopyMemory(pBitProp, pFileMemTmp, sizeof(DWORD) * (sWorldParam.WorldSectorTileNum * sWorldParam.WorldSectorTileNum) * (sWorldParam.WorldFieldSectorNum * sWorldParam.WorldFieldSectorNum));

					DWORD*	pTileProp	= NTL_NEW DWORD [iFieldTileCnt];
					RwInt32 iTileCnt	= 0;
					for (int e = 0; e < (sWorldParam.WorldSectorTileNum * sWorldParam.WorldFieldSectorNum); ++e)
					{
						for (int w = 0; w < sWorldParam.WorldSectorTileSize / iTileSize; ++w)
						{
							for (int r = 0; r < (sWorldParam.WorldSectorTileNum * sWorldParam.WorldFieldSectorNum); ++r)
							{
								for (int q = 0; q < sWorldParam.WorldSectorTileSize / iTileSize; ++q)
								{
									pTileProp[iTileCnt++] = pBitProp[r + (e * (sWorldParam.WorldSectorTileNum * sWorldParam.WorldFieldSectorNum))];
								}
							}
						}
					}
					NTL_ARRAY_DELETE(pBitProp);
					pNtlNaviPropOutDoorInfo->SetTileProp((unsigned int*)pTileProp);

					// FIELD FILE FREE
					FileMem.Free();

					// SECTOR FILE OPEN
					sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d", iFieldIdx);
					_chdir(sWorldParam.WorldProjectFolderName);
					_chdir("fields");
					_chdir(sWorldParam.WorldChar64Buf);
					sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d",iFieldIdx);

					FileMem.Load(sWorldParam.WorldChar64Buf, "rb");

					pFileMemTmp = GetSPosOfMaterial(FileMem.GetDat());

					for(RwInt32 iSectorCnt = 0; iSectorCnt < sWorldParam.WorldFieldSectorNum * sWorldParam.WorldFieldSectorNum; ++iSectorCnt)
					{
						pFileMemTmp = Skip_Material(pFileMemTmp);
						pFileMemTmp = Skip_Heightfield(pFileMemTmp);
						pFileMemTmp = Skip_Diffuse(pFileMemTmp);

						// Doodads
						RwInt32 iNumDoodads;
						CopyMemory(&iNumDoodads, pFileMemTmp, sizeof(RwInt32));
						pFileMemTmp += sizeof(RwInt32);

						for (int iDoodadCnt = 0; iDoodadCnt < iNumDoodads; ++iDoodadCnt)
						{
							RwChar	acDoodadName[128]; // Obj. Name
							RwV3d	avDoodadTransform[3]; // Scale, Rotation, Translation

							CopyMemory(acDoodadName, pFileMemTmp, sizeof(RwChar) * 128);
							pFileMemTmp += sizeof(RwChar) * 128;
							CopyMemory(avDoodadTransform, pFileMemTmp, sizeof(RwV3d) * 3);
							pFileMemTmp += sizeof(RwV3d) * 3;

							RwUInt32 uiObjectType;
							CopyMemory(&uiObjectType, pFileMemTmp, sizeof(RwUInt32));
							pFileMemTmp += sizeof(RwUInt32);

							if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
								uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
							{
								// Object Propperty Range
								RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
									(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
									sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
									(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

								pFileMemTmp += iPropertyRange;
							}

							pFileMemTmp += (sizeof(RwUInt32)); // trigger obj serial id
#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
							pFileMemTmp += (sizeof(RwUInt32)); // object serial id
#endif
							pFileMemTmp += (sizeof(DWORD)); // Obj. name index
							pFileMemTmp += (sizeof(RwReal)); // visibility distance
							pFileMemTmp += (sizeof(RwBool)); // enable trigger
							pFileMemTmp += (sizeof(RwBool)); // enable selection
							pFileMemTmp += (sizeof(RwBool)); // enable alterable
							pFileMemTmp += (sizeof(RwBool)); // enable portal
							pFileMemTmp += (sizeof(RwBool)); // enable path engine collision
							pFileMemTmp += (sizeof(RwBool)); // enable shadowprop

							// PS Map Size
							RwInt32 PSAtomicCnt = 0;
							CopyMemory(&PSAtomicCnt, pFileMemTmp, sizeof(RwInt32));
							pFileMemTmp += sizeof(RwInt32);

							for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
							{
								// Map name
								pFileMemTmp += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

								// Frustum view window extent
								pFileMemTmp += (sizeof(RwReal));

								// Light matrices
								pFileMemTmp += (sizeof(RwMatrix));

								// Map size
								RwInt32 Size = 0;
								CopyMemory(&Size, pFileMemTmp, sizeof(RwInt32));
								pFileMemTmp += (sizeof(RwInt32));

								// Map
								pFileMemTmp += (sizeof(DWORD) * Size * Size);
							}

							// milepost
							RwBool EnableFlg;
							CopyMemory(&EnableFlg, pFileMemTmp, sizeof(RwBool));
							pFileMemTmp += sizeof(RwBool);

							if(EnableFlg)
							{
								pFileMemTmp += sizeof(RwUInt32);
							}

							// attach light object skip
							RwInt32 iLightObjectNum;
							CopyMemory(&iLightObjectNum, pFileMemTmp, sizeof(RwInt32));
							pFileMemTmp += sizeof(RwInt32);

							for (int j = 0; j < iLightObjectNum; ++j)
							{
								pFileMemTmp += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

								pFileMemTmp += sizeof(RpLightType);
								pFileMemTmp += sizeof(RwBool);

								pFileMemTmp += sizeof(RwV3d);

								pFileMemTmp += sizeof(RwV3d);
								pFileMemTmp += sizeof(RwRGBAReal);
								pFileMemTmp += sizeof(RwReal);
								pFileMemTmp += sizeof(RwReal);

								pFileMemTmp += sizeof(RwBool);
								pFileMemTmp += sizeof(RwBool);
							}

							// DATA ATTACH
							WIN32_FIND_DATA findData;
							HANDLE			hfindFile;

							// NaviMesh Find
							RwChar			acNaviFind[NTL_MAX_DIR_PATH];
							sprintf_s(acNaviFind, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s_PE_Nav_*.dff",
								dGET_WORLD_PARAM()->CurWorkingFolderName, acDoodadName);

							hfindFile = ::FindFirstFile(acNaviFind, &findData);
							if(hfindFile != INVALID_HANDLE_VALUE)
							{
								do
								{
									if (findData.cFileName[0] != '.')
									{
										if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
										{
											continue;
										}
										else
										{								
											pNtlNaviGroupOutDoorInfo->AttachNaviMesh(acDoodadName,
												avDoodadTransform[2].x,
												avDoodadTransform[2].y,
												avDoodadTransform[2].z,
												avDoodadTransform[0].x,
												avDoodadTransform[0].y,
												avDoodadTransform[0].z,
												avDoodadTransform[1].y);
										}
									}
								} while(::FindNextFile(hfindFile, &findData));

								::FindClose(hfindFile);
							}

							// ObstMesh Find
							RwChar			acObstFind[NTL_MAX_DIR_PATH];
							sprintf_s(acObstFind, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s_PE_Obs_*.dff",
								dGET_WORLD_PARAM()->CurWorkingFolderName, acDoodadName);

							hfindFile = ::FindFirstFile(acObstFind, &findData);
							if(hfindFile != INVALID_HANDLE_VALUE)
							{
								do
								{
									if (findData.cFileName[0] != '.')
									{
										if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
										{
											continue;
										}
										else
										{								
											pNtlNaviGroupOutDoorInfo->AttachObsMesh(acDoodadName,
												avDoodadTransform[2].x,
												avDoodadTransform[2].y,
												avDoodadTransform[2].z,
												avDoodadTransform[0].x,
												avDoodadTransform[0].y,
												avDoodadTransform[0].z,
												avDoodadTransform[1].y);
										}
									}
								} while(::FindNextFile(hfindFile, &findData));

								::FindClose(hfindFile);
							}
						}

						pFileMemTmp = Skip_Water(pFileMemTmp);
						pFileMemTmp = Skip_Shadow(pFileMemTmp);
						pFileMemTmp = Skip_Effects(pFileMemTmp);
						pFileMemTmp = Skip_SE(pFileMemTmp);
						// CZ-SS
						//pFileMemTmp = Skip_SS(pFileMemTmp);
						pFileMemTmp = Skip_SectorCull(pFileMemTmp);
						pFileMemTmp = Skip_TileTransparency(pFileMemTmp);
						pFileMemTmp = Skip_Shoreline(pFileMemTmp);
						pFileMemTmp = Skip_Decal(pFileMemTmp);
						pFileMemTmp = Skip_Plant(pFileMemTmp);
						pFileMemTmp = Skip_WorldLight(pFileMemTmp);
						pFileMemTmp = Skip_Occluder(pFileMemTmp);
						pFileMemTmp = Skip_HeatHazeObject(pFileMemTmp);
						pFileMemTmp = Skip_LightObject(pFileMemTmp);
						pFileMemTmp = Skip_Dojo(pFileMemTmp);
					}

					FileMem.Free();

					// SWAP FILE OPEN
					_chdir(sWorldParam.WorldProjectFolderName);
					_chdir("swap");
					_chdir("pe");

					pFileMemTmp = FileMem.GetDat();

					RwV3d vTemp;
					vTemp.x = static_cast<RwReal>((iFieldIdx % sWorldParam.WorldFieldNum) * sWorldParam.WorldFieldSize);
					vTemp.z = static_cast<RwReal>((iFieldIdx / sWorldParam.WorldFieldNum) * sWorldParam.WorldFieldSize);

					RwV3d	vCount;
					RwInt32 iCount = 0;
					for (vCount.z = vTemp.z; vCount.z < vTemp.z + sWorldParam.WorldFieldSize; vCount.z += sWorldParam.WorldSectorSize)
					{
						for (vCount.x = vTemp.x; vCount.x < vTemp.x + sWorldParam.WorldFieldSize; vCount.x += sWorldParam.WorldSectorSize)
						{
							RwInt32 iWidthNum = (int)(vCount.x / (float)sWorldParam.WorldSectorSize);
							RwInt32 iDepthNum = (int)(vCount.z / (float)sWorldParam.WorldSectorSize); 

							RwInt32 RetIdx = iWidthNum + (iDepthNum * sWorldParam.WorldSectorNum);

							sprintf_s(sWorldParam.WorldChar64Buf, 64, "%d_sec.pe", RetIdx);

							FILE* pTempFile;
							if(fopen_s(&pTempFile, sWorldParam.WorldChar64Buf, "rb"))
							{
								DBO_TRACE(FALSE, "file open failed. (" << sWorldParam.WorldChar64Buf << ")");
							}
							else
							{
								RwReal* pfHeight = NTL_NEW RwReal [iSectorTileCnt];
								fread(pfHeight, sizeof(RwReal) * iSectorTileCnt, 1, pTempFile);
								fclose(pTempFile);

								pNtlNaviGroupOutDoorInfo->AttachSectorHeightList(iFieldIdx, iCount++, pfHeight);
							}
						}
					}				
				}
			}
			NtlNaviDataMgr.Flush();
		}
	}

	NtlNaviDataMgr.Delete();
	::SetCurrentDirectory(acOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportPathEngineInDoor(const char* _pPathNameOfProject, RwUInt32 uiWorldID, const char* _pPathName4Exporting,
													   const char* _pWorldName, const char* _pAgency)
{
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;

	RwChar			acTempPath[NTL_MAX_DIR_PATH] = {0,};
	RwChar			acOldPath[NTL_MAX_DIR_PATH] = {0,};
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	sNtlWorldParam	sWorldParam;
	FILE*			pFile;
	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << sWorldParam.WorldChar64Buf << ")");		
	}
	else
	{
		LoadFileWorldState(&sWorldParam, pFile);
		fclose(pFile);
	}
	strcpy_s(sWorldParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	CNtlNaviDataMng				NtlNaviDataMgr;	NtlNaviDataMgr.Create(_pPathName4Exporting);
	CNtlNaviWorldInDoorInfo*	pNtlNaviWorldInDoorInfo = NtlNaviDataMgr.Create_WorldInDoor();

	pNtlNaviWorldInDoorInfo->SetWorldID(uiWorldID);
	pNtlNaviWorldInDoorInfo->SetWorldName(_pWorldName);
	pNtlNaviWorldInDoorInfo->SetWorldMinPos((float)-sWorldParam.WorldSizeHalf, (float)-sWorldParam.WorldSizeHalf);
	pNtlNaviWorldInDoorInfo->SetWorldMaxPos((float)sWorldParam.WorldSizeHalf, (float)sWorldParam.WorldSizeHalf);

	pNtlNaviWorldInDoorInfo->SetBlockSize((float)sWorldParam.WorldBlockSize);

	std::vector<std::string> vecString;
	StringTokenize(vecString, std::string(_pAgency), std::string(","));
	for (int i = 0; i < (int)vecString.size(); ++i)
	{
		pNtlNaviWorldInDoorInfo->AttachAgency(::atof(vecString.at(i).c_str()));
	}

	CNtlNaviGroupInDoorInfo* pNtlNaviGroupInDoorInfo = NtlNaviDataMgr.Create_GroupInDoor();
	pNtlNaviGroupInDoorInfo->SetModelName(sWorldParam.WorldBSP);

	struct SGamePropertyData
	{
		RwUInt32	uiShape;
		RwV3d		vPos;
		RwV3d		vSize;
		RwUInt32	uiProperty;
	};
	std::vector<SGamePropertyData> vecGamePropertyData;
	for (int iFieldIdx = 0; iFieldIdx < sWorldParam.WorldFieldNum * sWorldParam.WorldFieldNum; ++iFieldIdx)
	{
		sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d", iFieldIdx);
		_chdir(sWorldParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sWorldParam.WorldChar64Buf);
		sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d",iFieldIdx);

		FileMem.Load(sWorldParam.WorldChar64Buf, "rb");

		pFileMemTmp = FileMem.GetDat();
		for(RwInt32 iSectorCnt = 0; iSectorCnt < sWorldParam.WorldFieldSectorNum * sWorldParam.WorldFieldSectorNum; ++iSectorCnt)
		{			
			// Doodads
			RwInt32 iNumDoodads;
			CopyMemory(&iNumDoodads, pFileMemTmp, sizeof(RwInt32));
			pFileMemTmp += sizeof(RwInt32);

			for (int iDoodadCnt = 0; iDoodadCnt < iNumDoodads; ++iDoodadCnt)
			{
				RwChar	acDoodadName[128]; // Obj. Name
				RwV3d	avDoodadTransform[3]; // Scale, Rotation, Translation

				CopyMemory(acDoodadName, pFileMemTmp, sizeof(RwChar) * 128);
				pFileMemTmp += sizeof(RwChar) * 128;
				CopyMemory(avDoodadTransform, pFileMemTmp, sizeof(RwV3d) * 3);
				pFileMemTmp += sizeof(RwV3d) * 3;

				RwUInt32 uiObjectType;
				CopyMemory(&uiObjectType, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += sizeof(RwUInt32);

				if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
					uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
				{
					// Object Propperty Range
					RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
						(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
						sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
						(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

					pFileMemTmp += iPropertyRange;
				}

				pFileMemTmp += (sizeof(RwUInt32)); // trigger obj serial id
				pFileMemTmp += (sizeof(RwUInt32)); // object serial id
				pFileMemTmp += (sizeof(DWORD)); // Obj. name index
				pFileMemTmp += (sizeof(RwReal)); // visibility distance
				pFileMemTmp += (sizeof(RwBool)); // enable trigger
				pFileMemTmp += (sizeof(RwBool)); // enable selection
				pFileMemTmp += (sizeof(RwBool)); // enable alterable
				pFileMemTmp += (sizeof(RwBool)); // enable portal
				pFileMemTmp += (sizeof(RwBool)); // enable path engine collision
				pFileMemTmp += (sizeof(RwBool)); // enable shadowprop

				// PS Map Size
				RwInt32 PSAtomicCnt = 0;
				CopyMemory(&PSAtomicCnt, pFileMemTmp, sizeof(RwInt32));
				pFileMemTmp += sizeof(RwInt32);

				for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
				{
					// Map name
					pFileMemTmp += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

					// Frustum view window extent
					pFileMemTmp += (sizeof(RwReal));

					// Light matrices
					pFileMemTmp += (sizeof(RwMatrix));

					// Map size
					RwInt32 Size = 0;
					CopyMemory(&Size, pFileMemTmp, sizeof(RwInt32));
					pFileMemTmp += (sizeof(RwInt32));

					// Map
					pFileMemTmp += (sizeof(DWORD) * Size * Size);
				}

				// milepost
				RwBool EnableFlg;
				CopyMemory(&EnableFlg, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += sizeof(RwBool);

				if(EnableFlg)
				{
					pFileMemTmp += sizeof(RwUInt32);
				}

				// attach light object skip
				RwInt32 iLightObjectNum;
				CopyMemory(&iLightObjectNum, pFileMemTmp, sizeof(RwInt32));
				pFileMemTmp += sizeof(RwInt32);

				for (int j = 0; j < iLightObjectNum; ++j)
				{
					pFileMemTmp += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

					pFileMemTmp += sizeof(RpLightType);
					pFileMemTmp += sizeof(RwBool);

					pFileMemTmp += sizeof(RwV3d);

					pFileMemTmp += sizeof(RwV3d);
					pFileMemTmp += sizeof(RwRGBAReal);
					pFileMemTmp += sizeof(RwReal);
					pFileMemTmp += sizeof(RwReal);

					pFileMemTmp += sizeof(RwBool);
					pFileMemTmp += sizeof(RwBool);
				}

				RwUInt32 uiLtMapObjectFlags;
				RwUInt32 uiLtMapMaterialFlags;

				CopyMemory(&uiLtMapObjectFlags, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += sizeof(RwUInt32);

				CopyMemory(&uiLtMapMaterialFlags, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += sizeof(RwUInt32);

				RwInt32 iAtomicCnt;
				CopyMemory(&iAtomicCnt, pFileMemTmp, sizeof(RwInt32));
				pFileMemTmp += sizeof(RwInt32);

				for (int i = 0; i < iAtomicCnt; ++i)
				{
					RwBool bLitmapExisted;
					CopyMemory(&bLitmapExisted, pFileMemTmp, sizeof(RwBool));
					pFileMemTmp += sizeof(RwBool);

					if (bLitmapExisted)
					{
						pFileMemTmp += sizeof(RwChar) * 64;
						pFileMemTmp += sizeof(RwChar) * 64;

						RwInt32 iUVCnt;
						CopyMemory(&iUVCnt, pFileMemTmp, sizeof(RwInt32));
						pFileMemTmp += sizeof(RwInt32);
						pFileMemTmp += sizeof(RwTexCoords) * iUVCnt;
					}
				}

				// DATA ATTACH
				WIN32_FIND_DATA findData;
				HANDLE			hfindFile;

				// NaviMesh Find
				RwChar			acNaviFind[NTL_MAX_DIR_PATH];
				sprintf_s(acNaviFind, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s_PE_Nav_*.dff",
					dGET_WORLD_PARAM()->CurWorkingFolderName, acDoodadName);

				hfindFile = ::FindFirstFile(acNaviFind, &findData);
				if(hfindFile != INVALID_HANDLE_VALUE)
				{
					do
					{
						if (findData.cFileName[0] != '.')
						{
							if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
							{
								continue;
							}
							else
							{								
								pNtlNaviGroupInDoorInfo->AttachNaviMesh(acDoodadName,
									avDoodadTransform[2].x,
									avDoodadTransform[2].y,
									avDoodadTransform[2].z,
									avDoodadTransform[0].x,
									avDoodadTransform[0].y,
									avDoodadTransform[0].z,
									avDoodadTransform[1].y);
							}
						}
					} while(::FindNextFile(hfindFile, &findData));

					::FindClose(hfindFile);
				}

				// ObstMesh Find
				RwChar			acObstFind[NTL_MAX_DIR_PATH];
				sprintf_s(acObstFind, NTL_MAX_DIR_PATH, "%s\\Tool\\ToolShareData\\NaviModel\\%s_PE_Obs_*.dff",
					dGET_WORLD_PARAM()->CurWorkingFolderName, acDoodadName);

				hfindFile = ::FindFirstFile(acObstFind, &findData);
				if(hfindFile != INVALID_HANDLE_VALUE)
				{
					do
					{
						if (findData.cFileName[0] != '.')
						{
							if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
							{
								continue;
							}
							else
							{								
								pNtlNaviGroupInDoorInfo->AttachObsMesh(acDoodadName,
									avDoodadTransform[2].x,
									avDoodadTransform[2].y,
									avDoodadTransform[2].z,
									avDoodadTransform[0].x,
									avDoodadTransform[0].y,
									avDoodadTransform[0].z,
									avDoodadTransform[1].y);
							}
						}
					} while(::FindNextFile(hfindFile, &findData));

					::FindClose(hfindFile);
				}
			}

			pFileMemTmp = Skip_Water(pFileMemTmp);
			pFileMemTmp = Skip_Shadow(pFileMemTmp);
			pFileMemTmp = Skip_Effects(pFileMemTmp);
			pFileMemTmp = Skip_SE(pFileMemTmp);
			// CZ-SS
			//pFileMemTmp = Skip_SS(pFileMemTmp);
			pFileMemTmp = Skip_SectorCull(pFileMemTmp);
			pFileMemTmp = Skip_TileTransparency(pFileMemTmp);
			pFileMemTmp = Skip_Shoreline(pFileMemTmp);
			pFileMemTmp = Skip_Decal(pFileMemTmp);
			pFileMemTmp = Skip_Plant(pFileMemTmp);
			pFileMemTmp = Skip_WorldLight(pFileMemTmp);
			pFileMemTmp = Skip_Occluder(pFileMemTmp);
			pFileMemTmp = Skip_HeatHazeObject(pFileMemTmp);
			pFileMemTmp = Skip_LightObject(pFileMemTmp);
			pFileMemTmp = Skip_DecalVolume(pFileMemTmp);

			RwInt32 iGamePropertyNum;
			CopyMemory(&iGamePropertyNum, pFileMemTmp, sizeof(RwInt32));
			pFileMemTmp += sizeof(RwInt32);
			for (int i = 0; i < iGamePropertyNum; ++i)
			{
				SGamePropertyData sGamePropertyData;

				CopyMemory(&sGamePropertyData.uiShape, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += sizeof(RwUInt32);

				CopyMemory(&sGamePropertyData.vPos, pFileMemTmp, sizeof(RwV3d));
				pFileMemTmp += sizeof(RwV3d);

				CopyMemory(&sGamePropertyData.vSize, pFileMemTmp, sizeof(RwV3d));
				pFileMemTmp += sizeof(RwV3d);

				CopyMemory(&sGamePropertyData.uiProperty, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += sizeof(RwUInt32);

				vecGamePropertyData.push_back(sGamePropertyData);
			}

			pFileMemTmp = Skip_Dojo(pFileMemTmp);
		}

		FileMem.Free();
	}

	_chdir(sWorldParam.WorldProjectFolderName);
	FileMem.Load("wfp", "rb");

	for (int iBlockIdx = 0; iBlockIdx < sWorldParam.WorldBlockNum * sWorldParam.WorldBlockNum; ++iBlockIdx)
	{
		CNtlNaviPropInDoorInfo* pNtlNaviPropInDoorInfo = NtlNaviDataMgr.Create_PropInDoor();

		pFileMemTmp = FileMem.GetDat();

		char acMapName[64];
		CopyMemory(acMapName, pFileMemTmp, 64);
		TBLIDX tlbIdx = static_cast<TBLIDX>(atoi(acMapName));
		pFileMemTmp = Skip_Name(pFileMemTmp);

		//pFileMemTmp = Skip_BitProp(pFileMemTmp);
		pFileMemTmp = Skip_Fog(pFileMemTmp);
		pFileMemTmp = Skip_Sky(pFileMemTmp);
		pFileMemTmp = Skip_RealLight(pFileMemTmp);
		pFileMemTmp = Skip_BGM(pFileMemTmp);
		pFileMemTmp = Skip_Bloom(pFileMemTmp);
		pFileMemTmp = Skip_Planet(pFileMemTmp);
		pFileMemTmp = Skip_Specular(pFileMemTmp);
		//pFileMemTmp = Skip_WorldMaterial(pFileMemTmp);
		pFileMemTmp = Skip_FieldColor(pFileMemTmp);
		pFileMemTmp = Skip_Weather(pFileMemTmp);
		pFileMemTmp = Skip_FieldHeatHaze(pFileMemTmp);

		RwUInt32 uiGameProperty;
		CopyMemory(&uiGameProperty, pFileMemTmp, sizeof(RwUInt32));
		pFileMemTmp = Skip_BlockGameProperty(pFileMemTmp);		

		pNtlNaviPropInDoorInfo->SetBlockID(iBlockIdx);
		pNtlNaviPropInDoorInfo->SetBlockProp(tlbIdx);		
		pNtlNaviPropInDoorInfo->SetGameProp(uiGameProperty);	

		RwV3d vBlockPos;
		vBlockPos.x = static_cast<RwReal>((iBlockIdx % sWorldParam.WorldBlockNum) * sWorldParam.WorldBlockSize - sWorldParam.WorldSizeHalf);
		vBlockPos.y = 0.0f;
		vBlockPos.z = static_cast<RwReal>((iBlockIdx / sWorldParam.WorldBlockNum) * sWorldParam.WorldBlockSize - sWorldParam.WorldSizeHalf);

		RwBBox bboxBlock;
		bboxBlock.inf	= vBlockPos;
		bboxBlock.sup.x	= static_cast<RwReal>(vBlockPos.x + sWorldParam.WorldBlockSize);
		bboxBlock.sup.y = 0.0f;
		bboxBlock.sup.z	= static_cast<RwReal>(vBlockPos.z + sWorldParam.WorldBlockSize);

		std::vector<SGamePropertyData>::iterator it = vecGamePropertyData.begin();
		while (it != vecGamePropertyData.end())
		{
			SGamePropertyData& sGamePropertyData = *it;

			if (sGamePropertyData.vPos.x > bboxBlock.inf.x && sGamePropertyData.vPos.z > bboxBlock.inf.z &&
				sGamePropertyData.vPos.x < bboxBlock.sup.x && sGamePropertyData.vPos.z < bboxBlock.sup.z)
			{
				switch (sGamePropertyData.uiProperty)
				{
				case ENTL_PL_GAME_PROPERTY_SHAPE_CIRCLE:
					pNtlNaviPropInDoorInfo->AttachEntityInfo_Sphere(sGamePropertyData.vPos.x, sGamePropertyData.vPos.y, sGamePropertyData.vPos.z, sGamePropertyData.vSize.x);
					break;
				case ENTL_PL_GAME_PROPERTY_SHAPE_PLANE:
					pNtlNaviPropInDoorInfo->AttachEntityInfo_Plane(sGamePropertyData.vPos.x, sGamePropertyData.vPos.y, sGamePropertyData.vPos.z, sGamePropertyData.vSize.x * 2.0f, sGamePropertyData.vSize.z * 2.0f); // Half Size이기 때문에
					break;
				}

				it = vecGamePropertyData.erase(it);
				continue;
			}
			++it;
		}
	}

	FileMem.Free();

	NtlNaviDataMgr.Flush();
	NtlNaviDataMgr.Delete();
	::SetCurrentDirectory(acOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportTriggerInfoIndoor(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;

	RwChar			acTempPath[NTL_MAX_DIR_PATH] = {0,};
	RwChar			acOldPath[NTL_MAX_DIR_PATH] = {0,};
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	sNtlWorldParam	sWorldParam;
	FILE*			pFile;
	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << sWorldParam.WorldChar64Buf << ")");		
	}
	else
	{
		LoadFileWorldState(&sWorldParam, pFile);
		fclose(pFile);
	}
	strcpy_s(sWorldParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	if(::fopen_s(&pFile, _pPathName4Exporting, "wt") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << _pPathName4Exporting << ")");
	}

	for (int iFieldIdx = 0; iFieldIdx < sWorldParam.WorldFieldNum * sWorldParam.WorldFieldNum; ++iFieldIdx)
	{
		sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d", iFieldIdx);
		_chdir(sWorldParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sWorldParam.WorldChar64Buf);
		sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d",iFieldIdx);

		FileMem.Load(sWorldParam.WorldChar64Buf, "rb");

		::SetCurrentDirectory(acOldPath);

		pFileMemTmp = FileMem.GetDat();
		for(RwInt32 iSectorCnt = 0; iSectorCnt < sWorldParam.WorldFieldSectorNum * sWorldParam.WorldFieldSectorNum; ++iSectorCnt)
		{			
			// Doodads
			RwInt32 iNumDoodads;
			CopyMemory(&iNumDoodads, pFileMemTmp, sizeof(RwInt32));
			pFileMemTmp += sizeof(RwInt32);

			for (int iDoodadCnt = 0; iDoodadCnt < iNumDoodads; ++iDoodadCnt)
			{
				RwChar	acDoodadName[128];
				RwV3d	avDoodadTransform[3]; // Scale, Rotation, Translation

				CopyMemory(acDoodadName, pFileMemTmp, sizeof(RwChar) * 128);
				pFileMemTmp += sizeof(RwChar) * 128;
				CopyMemory(avDoodadTransform, pFileMemTmp, sizeof(RwV3d) * 3);
				pFileMemTmp += sizeof(RwV3d) * 3;

				RwUInt32 uiObjectType;
				CopyMemory(&uiObjectType, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += sizeof(RwUInt32);

				if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
					uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
				{
					// Object Propperty Range
					RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
						(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
						sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
						(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

					pFileMemTmp += iPropertyRange;
				}

				SPLEntityCreateParam sPLEntityCreateParam;
				sPLEntityCreateParam.pPos = &avDoodadTransform[2];

				CNtlPLEntity* pNtlPLObject = static_cast<CNtlPLEntity*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, acDoodadName, &sPLEntityCreateParam));
				DBO_ASSERT(pNtlPLObject, "Entity create failed.");

				RwBBox bboxAABB = pNtlPLObject->GetTriggerAABBInfo(avDoodadTransform[2], avDoodadTransform[1], avDoodadTransform[0]);

				GetSceneManager()->DeleteEntity(static_cast<CNtlPLEntity*>(pNtlPLObject));

				RwUInt32 uiTriggerID;
				CopyMemory(&uiTriggerID, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += (sizeof(RwUInt32));		 // trigger obj serial id

				pFileMemTmp += (sizeof(RwUInt32));		// Object  serial id	

				DWORD dwObjectNameIdx;
				CopyMemory(&dwObjectNameIdx, pFileMemTmp, sizeof(DWORD));
				pFileMemTmp += (sizeof(DWORD)); 		// Obj. name index

				pFileMemTmp += (sizeof(RwReal));		// visibility distance

				RwBool bEnableTrigger;
				CopyMemory(&bEnableTrigger, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += (sizeof(RwBool));		// enable trigger

				RwBool bEnableSelection;
				CopyMemory(&bEnableSelection, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += (sizeof(RwBool));		// enable selection

				RwBool bEnableAlterable;
				CopyMemory(&bEnableAlterable, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += (sizeof(RwBool));		// enable alterable

				RwBool bEnablePortal;
				CopyMemory(&bEnablePortal, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += (sizeof(RwBool));		// enable portal

				pFileMemTmp += (sizeof(RwBool));		// enable path engine collision
				pFileMemTmp += (sizeof(RwBool));		// enable shadowprop

				if(uiTriggerID != 0xffffffff && bEnableTrigger)
				{
					RwV3d		vTempScale	= { 1.0f, 1.0f, 1.0f };
					RwV3d		vTempRot	= avDoodadTransform[1];
					RwV3d		vTempPos	= { 0.0f, 0.0f, 0.0f };
					RwV3d		vTempRotResult;
					RwMatrix	matTemp;

					CNtlMath::MathGetMatrix(&matTemp, &vTempScale, &vTempRot, &vTempPos);
					RwV3dTransformVector(&vTempRotResult, &CNtlPLGlobal::m_vZAxisV3, &matTemp);

					vTempRotResult.y = 0.0f;
					RwV3dNormalize(&vTempRotResult, &vTempRotResult);

					fprintf_s(pFile, "%d\t%u\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%d\t%d\t%d\t%s\t\n",
						uiTriggerID,
						dwObjectNameIdx,
						avDoodadTransform[2].x, avDoodadTransform[2].y, avDoodadTransform[2].z,
						vTempRotResult.x, vTempRotResult.y, vTempRotResult.z,						
						bboxAABB.inf.x, bboxAABB.inf.y, bboxAABB.inf.z,
						bboxAABB.sup.x, bboxAABB.sup.y, bboxAABB.sup.z,
						CLAMP(bEnableAlterable, 0, 1),
						CLAMP(bEnableSelection, 0, 1),
						CLAMP(bEnablePortal, 0, 1),
						acDoodadName);
				}

				// PS Map Size
				RwInt32 PSAtomicCnt = 0;
				CopyMemory(&PSAtomicCnt, pFileMemTmp, sizeof(RwInt32));
				pFileMemTmp += sizeof(RwInt32);

				for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
				{
					// Map name
					pFileMemTmp += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

					// Frustum view window extent
					pFileMemTmp += (sizeof(RwReal));

					// Light matrices
					pFileMemTmp += (sizeof(RwMatrix));

					// Map size
					RwInt32 Size = 0;
					CopyMemory(&Size, pFileMemTmp, sizeof(RwInt32));
					pFileMemTmp += (sizeof(RwInt32));

					// Map
					pFileMemTmp += (sizeof(DWORD) * Size * Size);
				}

				// milepost
				RwBool EnableFlg;
				CopyMemory(&EnableFlg, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += sizeof(RwBool);

				if(EnableFlg)
				{
					pFileMemTmp += sizeof(RwUInt32);
				}

				// attach light object skip
				RwInt32 iLightObjectNum;
				CopyMemory(&iLightObjectNum, pFileMemTmp, sizeof(RwInt32));
				pFileMemTmp += sizeof(RwInt32);

				for (int j = 0; j < iLightObjectNum; ++j)
				{
					pFileMemTmp += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

					pFileMemTmp += sizeof(RpLightType);
					pFileMemTmp += sizeof(RwBool);

					pFileMemTmp += sizeof(RwV3d);

					pFileMemTmp += sizeof(RwV3d);
					pFileMemTmp += sizeof(RwRGBAReal);
					pFileMemTmp += sizeof(RwReal);
					pFileMemTmp += sizeof(RwReal);

					pFileMemTmp += sizeof(RwBool);
					pFileMemTmp += sizeof(RwBool);
				}

				RwUInt32 uiLtMapObjectFlags;
				RwUInt32 uiLtMapMaterialFlags;

				CopyMemory(&uiLtMapObjectFlags, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += sizeof(RwUInt32);

				CopyMemory(&uiLtMapMaterialFlags, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += sizeof(RwUInt32);

				RwInt32 iAtomicCnt;
				CopyMemory(&iAtomicCnt, pFileMemTmp, sizeof(RwInt32));
				pFileMemTmp += sizeof(RwInt32);

				for (int i = 0; i < iAtomicCnt; ++i)
				{
					RwBool bLitmapExisted;
					CopyMemory(&bLitmapExisted, pFileMemTmp, sizeof(RwBool));
					pFileMemTmp += sizeof(RwBool);

					if (bLitmapExisted)
					{
						pFileMemTmp += sizeof(RwChar) * 64;
						pFileMemTmp += sizeof(RwChar) * 64;

						RwInt32 iUVCnt;
						CopyMemory(&iUVCnt, pFileMemTmp, sizeof(RwInt32));
						pFileMemTmp += sizeof(RwInt32);
						pFileMemTmp += sizeof(RwTexCoords) * iUVCnt;
					}
				}
			}

			pFileMemTmp = Skip_Water(pFileMemTmp);
			pFileMemTmp = Skip_Shadow(pFileMemTmp);
			pFileMemTmp = Skip_Effects(pFileMemTmp);
			pFileMemTmp = Skip_SE(pFileMemTmp);
			// CZ-SS
			//pFileMemTmp = Skip_SS(pFileMemTmp);
			pFileMemTmp = Skip_SectorCull(pFileMemTmp);
			pFileMemTmp = Skip_TileTransparency(pFileMemTmp);
			pFileMemTmp = Skip_Shoreline(pFileMemTmp);
			pFileMemTmp = Skip_Decal(pFileMemTmp);
			pFileMemTmp = Skip_Plant(pFileMemTmp);
			pFileMemTmp = Skip_WorldLight(pFileMemTmp);
			pFileMemTmp = Skip_Occluder(pFileMemTmp);
			pFileMemTmp = Skip_HeatHazeObject(pFileMemTmp);
			pFileMemTmp = Skip_LightObject(pFileMemTmp);
			pFileMemTmp = Skip_DecalVolume(pFileMemTmp);
			pFileMemTmp = Skip_GameProperty(pFileMemTmp);
			pFileMemTmp = Skip_Dojo(pFileMemTmp);
		}

		FileMem.Free();
	}

	if (pFile)
	{
		fclose(pFile);
	}

	::SetCurrentDirectory(acOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportTriggerInfoOutdoor(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;

	RwChar			acTempPath[NTL_MAX_DIR_PATH] = {0,};
	RwChar			acOldPath[NTL_MAX_DIR_PATH] = {0,};
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	sNtlWorldParam	sWorldParam;
	FILE*			pFile;
	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << sWorldParam.WorldChar64Buf << ")");		
	}
	else
	{
		LoadFileWorldState(&sWorldParam, pFile);
		fclose(pFile);
	}
	strcpy_s(sWorldParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	if(::fopen_s(&pFile, _pPathName4Exporting, "wt") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << _pPathName4Exporting << ")");
	}

	for (int iFieldIdx = 0; iFieldIdx < sWorldParam.WorldFieldNum * sWorldParam.WorldFieldNum; ++iFieldIdx)
	{
		sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d", iFieldIdx);
		_chdir(sWorldParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sWorldParam.WorldChar64Buf);
		sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d",iFieldIdx);

		FileMem.Load(sWorldParam.WorldChar64Buf, "rb");

		::SetCurrentDirectory(acOldPath);

		pFileMemTmp = FileMem.GetDat();
		for(RwInt32 iSectorCnt = 0; iSectorCnt < sWorldParam.WorldFieldSectorNum * sWorldParam.WorldFieldSectorNum; ++iSectorCnt)
		{
			pFileMemTmp = Skip_Material(pFileMemTmp);
			pFileMemTmp = Skip_Heightfield(pFileMemTmp);
			pFileMemTmp = Skip_Diffuse(pFileMemTmp);

			// Doodads
			RwInt32 iNumDoodads;
			CopyMemory(&iNumDoodads, pFileMemTmp, sizeof(RwInt32));
			pFileMemTmp += sizeof(RwInt32);

			for (int iDoodadCnt = 0; iDoodadCnt < iNumDoodads; ++iDoodadCnt)
			{
				RwChar	acDoodadName[128];
				RwV3d	avDoodadTransform[3]; // Scale, Rotation, Translation

				CopyMemory(acDoodadName, pFileMemTmp, sizeof(RwChar) * 128);
				pFileMemTmp += sizeof(RwChar) * 128;
				CopyMemory(avDoodadTransform, pFileMemTmp, sizeof(RwV3d) * 3);
				pFileMemTmp += sizeof(RwV3d) * 3;

				RwUInt32 uiObjectType;
				CopyMemory(&uiObjectType, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += sizeof(RwUInt32);

				if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
					uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
				{
					// Object Propperty Range
					RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
						(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
						sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
						(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

					pFileMemTmp += iPropertyRange;
				}

				SPLEntityCreateParam sPLEntityCreateParam;
				sPLEntityCreateParam.pPos = &avDoodadTransform[2];

				CNtlPLEntity* pNtlPLObject = static_cast<CNtlPLEntity*>(GetSceneManager()->CreateEntity(PLENTITY_OBJECT, acDoodadName, &sPLEntityCreateParam));
				DBO_ASSERT(pNtlPLObject, "Entity create failed.");

				RwBBox bboxAABB = pNtlPLObject->GetTriggerAABBInfo(avDoodadTransform[2], avDoodadTransform[1], avDoodadTransform[0]);

				GetSceneManager()->DeleteEntity(static_cast<CNtlPLEntity*>(pNtlPLObject));

				RwUInt32 uiTriggerID;
				CopyMemory(&uiTriggerID, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += (sizeof(RwUInt32));		 // trigger obj serial id

#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
				pFileMemTmp += (sizeof(RwUInt32));		// Object  serial id	
#endif
				DWORD dwObjectNameIdx;
				CopyMemory(&dwObjectNameIdx, pFileMemTmp, sizeof(DWORD));
				pFileMemTmp += (sizeof(DWORD)); 		// Obj. name index

				pFileMemTmp += (sizeof(RwReal));		// visibility distance

				RwBool bEnableTrigger;
				CopyMemory(&bEnableTrigger, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += (sizeof(RwBool));		// enable trigger

				RwBool bEnableSelection;
				CopyMemory(&bEnableSelection, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += (sizeof(RwBool));		// enable selection

				RwBool bEnableAlterable;
				CopyMemory(&bEnableAlterable, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += (sizeof(RwBool));		// enable alterable

				RwBool bEnablePortal;
				CopyMemory(&bEnablePortal, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += (sizeof(RwBool));		// enable portal

				pFileMemTmp += (sizeof(RwBool));		// enable path engine collision
				pFileMemTmp += (sizeof(RwBool));		// enable shadowprop

				if(uiTriggerID != 0xffffffff && bEnableTrigger)
				{
					RwV3d		vTempScale	= { 1.0f, 1.0f, 1.0f };
					RwV3d		vTempRot	= avDoodadTransform[1];
					RwV3d		vTempPos	= { 0.0f, 0.0f, 0.0f };
					RwV3d		vTempRotResult;
					RwMatrix	matTemp;

					CNtlMath::MathGetMatrix(&matTemp, &vTempScale, &vTempRot, &vTempPos);
					RwV3dTransformVector(&vTempRotResult, &CNtlPLGlobal::m_vZAxisV3, &matTemp);

					vTempRotResult.y = 0.0f;
					RwV3dNormalize(&vTempRotResult, &vTempRotResult);

					fprintf_s(pFile, "%d\t%u\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%d\t%d\t%d\t%s\t\n",
						uiTriggerID,
						dwObjectNameIdx,
						avDoodadTransform[2].x, avDoodadTransform[2].y, avDoodadTransform[2].z,
						vTempRotResult.x, vTempRotResult.y, vTempRotResult.z,						
						bboxAABB.inf.x, bboxAABB.inf.y, bboxAABB.inf.z,
						bboxAABB.sup.x, bboxAABB.sup.y, bboxAABB.sup.z,
						CLAMP(bEnableAlterable, 0, 1),
						CLAMP(bEnableSelection, 0, 1),
						CLAMP(bEnablePortal, 0, 1),
						acDoodadName);
				}

				// PS Map Size
				RwInt32 PSAtomicCnt = 0;
				CopyMemory(&PSAtomicCnt, pFileMemTmp, sizeof(RwInt32));
				pFileMemTmp += sizeof(RwInt32);

				for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
				{
					// Map name
					pFileMemTmp += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

					// Frustum view window extent
					pFileMemTmp += (sizeof(RwReal));

					// Light matrices
					pFileMemTmp += (sizeof(RwMatrix));

					// Map size
					RwInt32 Size = 0;
					CopyMemory(&Size, pFileMemTmp, sizeof(RwInt32));
					pFileMemTmp += (sizeof(RwInt32));

					// Map
					pFileMemTmp += (sizeof(DWORD) * Size * Size);
				}

				// milepost
				RwBool EnableFlg;
				CopyMemory(&EnableFlg, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += sizeof(RwBool);

				if(EnableFlg)
				{
					pFileMemTmp += sizeof(RwUInt32);
				}

				// attach light object skip
				RwInt32 iLightObjectNum;
				CopyMemory(&iLightObjectNum, pFileMemTmp, sizeof(RwInt32));
				pFileMemTmp += sizeof(RwInt32);

				for (int j = 0; j < iLightObjectNum; ++j)
				{
					pFileMemTmp += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

					pFileMemTmp += sizeof(RpLightType);
					pFileMemTmp += sizeof(RwBool);

					pFileMemTmp += sizeof(RwV3d);

					pFileMemTmp += sizeof(RwV3d);
					pFileMemTmp += sizeof(RwRGBAReal);
					pFileMemTmp += sizeof(RwReal);
					pFileMemTmp += sizeof(RwReal);

					pFileMemTmp += sizeof(RwBool);
					pFileMemTmp += sizeof(RwBool);
				}
			}

			pFileMemTmp = Skip_Water(pFileMemTmp);
			pFileMemTmp = Skip_Shadow(pFileMemTmp);
			pFileMemTmp = Skip_Effects(pFileMemTmp);
			pFileMemTmp = Skip_SE(pFileMemTmp);
			// CZ-SS
			//pFileMemTmp = Skip_SS(pFileMemTmp);
			pFileMemTmp = Skip_SectorCull(pFileMemTmp);
			pFileMemTmp = Skip_TileTransparency(pFileMemTmp);
			pFileMemTmp = Skip_Shoreline(pFileMemTmp);
			pFileMemTmp = Skip_Decal(pFileMemTmp);
			pFileMemTmp = Skip_Plant(pFileMemTmp);
			pFileMemTmp = Skip_WorldLight(pFileMemTmp);
			pFileMemTmp = Skip_Occluder(pFileMemTmp);
			pFileMemTmp = Skip_HeatHazeObject(pFileMemTmp);
			pFileMemTmp = Skip_LightObject(pFileMemTmp);
			pFileMemTmp = Skip_Dojo(pFileMemTmp);
		}

		FileMem.Free();
	}

	if (pFile)
	{
		fclose(pFile);
	}

	::SetCurrentDirectory(acOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportHeightfieldY(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	/*
	WorldFileFormat - Export
	Export 처리를 위한 부분이다.
	*/

	sNtlWorldParam	sTmpPrjParam;
	FILE*			pFile;
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;
	RwChar			szFFPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar			szOldPath[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sTmpPrjParam, pFile);
		fclose(pFile);
	}

	RwInt32		iWorldSectorVertNum		= sTmpPrjParam.WorldSectorVertNum * sTmpPrjParam.WorldSectorVertNum;
	RwV3d*		pvHeightfield			= NTL_NEW RwV3d[iWorldSectorVertNum];

	strcpy_s(sTmpPrjParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	for(int i = 0; i < sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum; ++i)
	{
		// current field file name
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);

		// name the file for exporting
		strcpy_s(szFFPathName, NTL_MAX_DIR_PATH, _pPathName4Exporting);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "\\");
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, sTmpPrjParam.WorldChar64Buf);
		_mkdir(szFFPathName);

		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sTmpPrjParam.WorldChar64Buf);

		// export
		FileMem.Load(sTmpPrjParam.WorldChar64Buf, "rb");

		pFileMemTmp = FileMem.GetDat();
		pFileMemTmp = GetSPosOfMaterial(FileMem.GetDat());

		for(RwInt32 j = 0; j < sTmpPrjParam.WorldFieldSectorNum * sTmpPrjParam.WorldFieldSectorNum; ++j)
		{
			RwChar szPathName[NTL_MAX_DIR_PATH];
			sprintf_s(szPathName, NTL_MAX_DIR_PATH, "%s\\shfy%d.txt", szFFPathName, j);
			if(fopen_s(&pFile, szPathName, "w"))
			{
				DBO_TRACE(FALSE, "file open failed. (" << szPathName << ")");
				NTL_DELETE(pvHeightfield);
				return FALSE;
			}

			pFileMemTmp = Skip_Material(pFileMemTmp);

			CopyMemory(pvHeightfield, pFileMemTmp, sizeof(RwV3d) * iWorldSectorVertNum);
			pFileMemTmp += (sizeof(RwV3d) * iWorldSectorVertNum);

			for (int x = 0; x < sTmpPrjParam.WorldSectorVertNum; ++x)
			{
				for (int y = sTmpPrjParam.WorldSectorVertNum - 1; y >= 0; --y)
				{
					::fprintf_s(pFile, "%f\n", pvHeightfield[y * sTmpPrjParam.WorldSectorVertNum + x].y);
				}
			}

			pFileMemTmp = Skip_Diffuse(pFileMemTmp);
			pFileMemTmp = Skip_Doodads(pFileMemTmp);
			pFileMemTmp = Skip_Water(pFileMemTmp);
			pFileMemTmp = Skip_Shadow(pFileMemTmp);
			pFileMemTmp = Skip_Effects(pFileMemTmp);
			pFileMemTmp = Skip_SE(pFileMemTmp);
			// CZ-SS
			//pFileMemTmp = Skip_SS(pFileMemTmp);
			pFileMemTmp = Skip_SectorCull(pFileMemTmp);
			pFileMemTmp = Skip_TileTransparency(pFileMemTmp);
			pFileMemTmp = Skip_Shoreline(pFileMemTmp);
			pFileMemTmp = Skip_Decal(pFileMemTmp);
			pFileMemTmp = Skip_Plant(pFileMemTmp);
			pFileMemTmp = Skip_WorldLight(pFileMemTmp);
			pFileMemTmp = Skip_Occluder(pFileMemTmp);
			pFileMemTmp = Skip_HeatHazeObject(pFileMemTmp);
			pFileMemTmp = Skip_LightObject(pFileMemTmp);
			pFileMemTmp = Skip_Dojo(pFileMemTmp);

			fclose(pFile);
		}
		FileMem.Free();

		_chdir("../");
	}

	NTL_DELETE(pvHeightfield);
	::SetCurrentDirectory(szOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportPECollision(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	/*
	WorldFileFormat - Export
	Export 처리를 위한 부분이다.
	*/

	sNtlWorldParam	sTmpPrjParam;
	FILE*			pFile;
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;
	RwChar			szFFPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar			szOldPath[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << "#######.gwp" << ")");
	}
	else
	{
		LoadFileWorldState(&sTmpPrjParam, pFile);
		fclose(pFile);
	}

	strcpy_s(sTmpPrjParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	for(int i = 0; i < sTmpPrjParam.WorldFieldNum * sTmpPrjParam.WorldFieldNum; ++i)
	{
		// current field file name
		sprintf_s(sTmpPrjParam.WorldChar64Buf, 64, "wfif%d", i);

		// name the file for exporting
		strcpy_s(szFFPathName, NTL_MAX_DIR_PATH, _pPathName4Exporting);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, "\\");
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, sTmpPrjParam.WorldChar64Buf);
		strcat_s(szFFPathName, NTL_MAX_DIR_PATH, ".pec");
		if(fopen_s(&pFile, szFFPathName, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << szFFPathName << ")");
			return FALSE;
		}

		_chdir(sTmpPrjParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sTmpPrjParam.WorldChar64Buf);

		// export
		FileMem.Load(sTmpPrjParam.WorldChar64Buf, "rb");

		pFileMemTmp = FileMem.GetDat();
		pFileMemTmp = GetSPosOfMaterial(FileMem.GetDat());

		for(RwInt32 j = 0; j < sTmpPrjParam.WorldFieldSectorNum * sTmpPrjParam.WorldFieldSectorNum; ++j)
		{
			pFileMemTmp = Skip_Material(pFileMemTmp);
			pFileMemTmp = Skip_Heightfield(pFileMemTmp);
			pFileMemTmp = Skip_Diffuse(pFileMemTmp);
			Save_Doodads_Num_PEC(pFileMemTmp, pFile);
			Save_Doodads_PEC(pFileMemTmp, pFile);
			pFileMemTmp = Skip_Doodads(pFileMemTmp);
			pFileMemTmp = Skip_Water(pFileMemTmp);
			pFileMemTmp = Skip_Shadow(pFileMemTmp);
			pFileMemTmp = Skip_Effects(pFileMemTmp);
			pFileMemTmp = Skip_SE(pFileMemTmp);
			// CZ-SS
			//pFileMemTmp = Skip_SS(pFileMemTmp);
			pFileMemTmp = Skip_SectorCull(pFileMemTmp);
			pFileMemTmp = Skip_TileTransparency(pFileMemTmp);
			pFileMemTmp = Skip_Shoreline(pFileMemTmp);
			pFileMemTmp = Skip_Decal(pFileMemTmp);
			pFileMemTmp = Skip_Plant(pFileMemTmp);
			pFileMemTmp = Skip_WorldLight(pFileMemTmp);
			pFileMemTmp = Skip_Occluder(pFileMemTmp);
			pFileMemTmp = Skip_HeatHazeObject(pFileMemTmp);
			pFileMemTmp = Skip_LightObject(pFileMemTmp);
			pFileMemTmp = Skip_Dojo(pFileMemTmp);
		}

		FileMem.Free();
		fclose(pFile);

		_chdir("../");
	}

	::SetCurrentDirectory(szOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnReindexingObjectSerialID()
{
	if (dGET_WORLD_PARAM()->WorldType != 1)
	{
		return FALSE;
	}

	dGET_WORLD_PARAM()->WorldObjectSerialID = 1;

	FILE*			pFile;
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;
	RwChar			szOldPath[NTL_MAX_DIR_PATH] = {0,};

	::GetCurrentDirectory(NTL_MAX_DIR_PATH, szOldPath);
	::SetCurrentDirectory(dGET_WORLD_PARAM()->WorldProjectFolderName);

	for(int i = 0; i < dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum; ++i)
	{
		// current field file name
		sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", i);
		_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
		_chdir("fields");
		_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

		// export
		FileMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");

		if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
		{
			DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
			return FALSE;
		}

		pFileMemTmp = FileMem.GetDat();
		pFileMemTmp = GetSPosOfMaterial(FileMem.GetDat());

		for(RwInt32 j = 0; j < dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldFieldSectorNum; ++j)
		{			
			// the number of doodads
			RwInt32 iNumObject = 0;
			CopyMemory(&iNumObject, pFileMemTmp, sizeof(RwInt32));
			::fwrite(&iNumObject, sizeof(RwInt32), 1, pFile);
			pFileMemTmp += sizeof(RwInt32);

			for(RwInt32 i = 0; i < iNumObject; ++i)
			{
				// Obj. name
				RwChar acName[128];
				CopyMemory(acName, pFileMemTmp, sizeof(RwChar) * 128);
				::fwrite(pFileMemTmp, sizeof(RwChar) * 128, 1, pFile);
				pFileMemTmp += sizeof(RwChar) * 128;

				// Scaling, Rotation, Translation
				::fwrite(pFileMemTmp, sizeof(RwV3d), 1, pFile);
				pFileMemTmp += sizeof(RwV3d);

				::fwrite(pFileMemTmp, sizeof(RwV3d), 1, pFile);
				pFileMemTmp += sizeof(RwV3d);

				RwV3d vPos;
				CopyMemory(&vPos, pFileMemTmp, sizeof(RwV3d));
				::fwrite(&vPos, sizeof(RwV3d), 1, pFile);
				pFileMemTmp += sizeof(RwV3d);				

				RwUInt32 uiObjectType;
				CopyMemory(&uiObjectType, pFileMemTmp, sizeof(RwUInt32));
				::fwrite(&uiObjectType, sizeof(RwUInt32), 1, pFile);
				pFileMemTmp += sizeof(RwUInt32);

				if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
					uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
				{
					pFileMemTmp = CNtlPLObjectTypeHasProperty::SaveIntoFileFromFileMem(pFile, pFileMemTmp);
				}

				// trigger obj serial id
				::fwrite(pFileMemTmp, sizeof(RwUInt32), 1, pFile);
				pFileMemTmp += sizeof(RwUInt32);

				if (dGET_WORLD_PARAM()->WorldType == 1)
				{
					// object serial id
					::fwrite(&dGET_WORLD_PARAM()->WorldObjectSerialID, sizeof(RwUInt32), 1, pFile);
					pFileMemTmp += sizeof(RwUInt32);
					dGET_WORLD_PARAM()->WorldObjectSerialID++;
				}

				// trigger obj object name index
				::fwrite(pFileMemTmp, sizeof(DWORD), 1, pFile);
				pFileMemTmp += sizeof(DWORD);

				// visibility distance
				::fwrite(pFileMemTmp, sizeof(RwReal), 1, pFile);
				pFileMemTmp += sizeof(RwReal);

				// enable trigger
				::fwrite(pFileMemTmp, sizeof(RwBool), 1, pFile);
				pFileMemTmp += sizeof(RwBool);

				// enable selection
				::fwrite(pFileMemTmp, sizeof(RwBool), 1, pFile);
				pFileMemTmp += sizeof(RwBool);

				// enable alterable
				::fwrite(pFileMemTmp, sizeof(RwBool), 1, pFile);
				pFileMemTmp += sizeof(RwBool);

				// enable portal
				::fwrite(pFileMemTmp, sizeof(RwBool), 1, pFile);
				pFileMemTmp += sizeof(RwBool);

				// enable path engine collision
				::fwrite(pFileMemTmp, sizeof(RwBool), 1, pFile);
				pFileMemTmp += sizeof(RwBool);

				// enable ShadowProp
				::fwrite(pFileMemTmp, sizeof(RwBool), 1, pFile);
				pFileMemTmp += sizeof(RwBool);

				// doodads psm
				RwInt32 PSAtomicCnt = 0;
				CopyMemory(&PSAtomicCnt, pFileMemTmp, sizeof(RwInt32));

				::fwrite(&PSAtomicCnt, sizeof(RwInt32), 1, pFile);
				pFileMemTmp += sizeof(RwInt32);

				for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
				{
					// Map name
					fwrite(pFileMemTmp, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH, 1, pFile);
					pFileMemTmp += sizeof(RwChar) * rwTEXTUREBASENAMELENGTH;

					// Frustum view window extent
					fwrite(pFileMemTmp, sizeof(RwReal), 1, pFile);
					pFileMemTmp += sizeof(RwReal);

					// Light matrices
					fwrite(pFileMemTmp, sizeof(RwMatrix), 1, pFile);
					pFileMemTmp += (sizeof(RwMatrix));

					// Map size
					RwInt32 Size = 0;
					CopyMemory(&Size, pFileMemTmp, sizeof(RwInt32));
					fwrite(&Size, sizeof(RwInt32), 1, pFile);
					pFileMemTmp += (sizeof(RwInt32));

					// Map
					fwrite(pFileMemTmp, sizeof(DWORD) * Size * Size, 1, pFile);
					pFileMemTmp += (sizeof(DWORD) * Size * Size);
				}

				// milepost
				RwBool EnableFlg;
				CopyMemory(&EnableFlg, pFileMemTmp, sizeof(RwBool));
				fwrite(&EnableFlg, sizeof(RwBool), 1, pFile);
				pFileMemTmp += sizeof(RwBool);

				if(EnableFlg)
				{
					fwrite(pFileMemTmp, sizeof(RwUInt32), 1, pFile);
					pFileMemTmp += sizeof(RwUInt32);
				}

				// attach light object skip
				pFileMemTmp = CNtlPLObject::SaveLightObjectAttachDataFromFileMem(pFile, pFileMemTmp);

				if (dGET_WORLD_PARAM()->WorldType == 1)
				{
					pFileMemTmp = CNtlPLObject::SaveLightmapFromFileMem(pFile, pFileMemTmp);
				}
			}

			pFileMemTmp = Save_Water(pFileMemTmp, pFile);
			pFileMemTmp = Save_Shadow(pFileMemTmp, pFile);
			pFileMemTmp = Save_Effects(pFileMemTmp, pFile);
			pFileMemTmp = Save_SE(pFileMemTmp, pFile);
			// CZ-SS
			//pFileMemTmp = Save_SS(pFileMemTmp, pFile);
			pFileMemTmp = Save_SectorCull(pFileMemTmp, pFile);
			pFileMemTmp = Save_TileTransparency(pFileMemTmp, pFile);
			pFileMemTmp = Save_Shoreline(pFileMemTmp, pFile);
			pFileMemTmp = Save_Decal(pFileMemTmp, pFile);
			pFileMemTmp = Save_Plant(pFileMemTmp, pFile);
			pFileMemTmp = Save_WorldLight(pFileMemTmp, pFile);
			pFileMemTmp = Save_Occluder(pFileMemTmp, pFile);
			pFileMemTmp = Save_HeatHazeObject(pFileMemTmp, pFile);
			pFileMemTmp = Save_LightObject(pFileMemTmp, pFile);
			pFileMemTmp = Save_DecalVolume(pFileMemTmp, pFile);
			pFileMemTmp = Save_GameProperty(pFileMemTmp, pFile);
			pFileMemTmp = Save_Dojo(pFileMemTmp, pFile);
		}

		FileMem.Free();
		fclose(pFile);
	}

	SaveCurWorldState();

	::SetCurrentDirectory(szOldPath);

	GetSceneManager()->GetWorld()->GetWorldFieldMgr()->RefreshFieldAll();

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnExportInvalidSectorLink(const char* _pPathNameOfProject, const char* _pPathName4Exporting)
{
	CFileMem		FileMem;
	BYTE*			pFileMemTmp;

	RwChar			acTempPath[NTL_MAX_DIR_PATH] = {0,};
	RwChar			acOldPath[NTL_MAX_DIR_PATH] = {0,};
	::GetCurrentDirectory(NTL_MAX_DIR_PATH, acOldPath);
	::SetCurrentDirectory(_pPathNameOfProject);

	sNtlWorldParam	sWorldParam;
	FILE*			pFile;
	if(fopen_s(&pFile, "#######.gwp", "rb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << sWorldParam.WorldChar64Buf << ")");		
	}
	else
	{
		LoadFileWorldState(&sWorldParam, pFile);
		fclose(pFile);
	}
	strcpy_s(sWorldParam.WorldProjectFolderName, 256, _pPathNameOfProject);

	if(::fopen_s(&pFile, _pPathName4Exporting, "wt") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << _pPathName4Exporting << ")");
	}

	for (int iFieldIdx = 0; iFieldIdx < sWorldParam.WorldFieldNum * sWorldParam.WorldFieldNum; ++iFieldIdx)
	{
		sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d", iFieldIdx);
		_chdir(sWorldParam.WorldProjectFolderName);
		_chdir("fields");
		_chdir(sWorldParam.WorldChar64Buf);
		sprintf_s(sWorldParam.WorldChar64Buf, 64, "wfif%d",iFieldIdx);

		FileMem.Load(sWorldParam.WorldChar64Buf, "rb");

		::SetCurrentDirectory(acOldPath);

		pFileMemTmp = FileMem.GetDat();
		for(RwInt32 iSectorCnt = 0; iSectorCnt < sWorldParam.WorldFieldSectorNum * sWorldParam.WorldFieldSectorNum; ++iSectorCnt)
		{
			pFileMemTmp = Skip_Material(pFileMemTmp);
			pFileMemTmp = Skip_Heightfield(pFileMemTmp);
			pFileMemTmp = Skip_Diffuse(pFileMemTmp);

			// Doodads
			RwInt32 iNumDoodads;
			CopyMemory(&iNumDoodads, pFileMemTmp, sizeof(RwInt32));
			pFileMemTmp += sizeof(RwInt32);

			for (int iDoodadCnt = 0; iDoodadCnt < iNumDoodads; ++iDoodadCnt)
			{
				RwChar	acDoodadName[128];
				RwV3d	avDoodadTransform[3]; // Scale, Rotation, Translation

				CopyMemory(acDoodadName, pFileMemTmp, sizeof(RwChar) * 128);
				pFileMemTmp += sizeof(RwChar) * 128;
				CopyMemory(avDoodadTransform, pFileMemTmp, sizeof(RwV3d) * 3);
				pFileMemTmp += sizeof(RwV3d) * 3;

				RwUInt32 uiObjectType;
				CopyMemory(&uiObjectType, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += sizeof(RwUInt32);

				if (uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_CLOSE ||
					uiObjectType == EPL_OBJECT_TYPE_MINI_INDOOR_OPEN)
				{
					// Object Propperty Range
					RwInt32 iPropertyRange = 64 + sizeof(RwRGBA) + sizeof(RwReal) + sizeof(RwReal) + sizeof(RwInt32) + sizeof(RwRGBA) + sizeof(RwRGBA) + sizeof(RwRGBA) +
						(sizeof(RwChar) * 10) + sizeof(RwReal) + (sizeof(RwChar) * 10) + (sizeof(RwChar) * 10) + sizeof(RwReal) + sizeof(RwReal) +
						sizeof(RwRGBAReal) + ((sizeof(RwRGBAReal) + sizeof(RwV3d)) * dNTL_PL_DIRECTIONAL_LIGHT_NUM) + sizeof(RwInt32) + sizeof(RwInt32) + sizeof(RwReal) +
						(sizeof(sNTL_PLANET) * dNTLPL_PLANET_MAX_CNT) + sizeof(sNTL_SPECULAR) + sizeof(RwRGBA) + (sizeof(RwInt32) * 5) + sizeof(sNTL_HEATHAZE) + (sizeof(RwInt32) * 5);

					pFileMemTmp += iPropertyRange;
				}

				RwUInt32 uiTriggerID;
				CopyMemory(&uiTriggerID, pFileMemTmp, sizeof(RwUInt32));
				pFileMemTmp += (sizeof(RwUInt32));		 // trigger obj serial id

#ifdef dNTL_WORLD_FORMAT_CONVERT_COMPLTE
				pFileMemTmp += (sizeof(RwUInt32));		// Object  serial id	
#endif
				DWORD dwObjectNameIdx;
				CopyMemory(&dwObjectNameIdx, pFileMemTmp, sizeof(DWORD));
				pFileMemTmp += (sizeof(DWORD)); 		// Obj. name index

				pFileMemTmp += (sizeof(RwReal));		// visibility distance

				RwBool bEnableTrigger;
				CopyMemory(&bEnableTrigger, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += (sizeof(RwBool));		// enable trigger

				RwBool bEnableSelection;
				CopyMemory(&bEnableSelection, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += (sizeof(RwBool));		// enable selection

				RwBool bEnableAlterable;
				CopyMemory(&bEnableAlterable, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += (sizeof(RwBool));		// enable alterable

				RwBool bEnablePortal;
				CopyMemory(&bEnablePortal, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += (sizeof(RwBool));		// enable portal

				pFileMemTmp += (sizeof(RwBool));		// enable path engine collision
				pFileMemTmp += (sizeof(RwBool));		// enable shadowprop

				RwBBox bboxSector;
				bboxSector.inf.x = static_cast<RwReal>((iFieldIdx % sWorldParam.WorldFieldNum) * sWorldParam.WorldFieldSize) - sWorldParam.WorldSizeHalf;
				bboxSector.inf.z = static_cast<RwReal>((iFieldIdx / sWorldParam.WorldFieldNum) * sWorldParam.WorldFieldSize) - sWorldParam.WorldSizeHalf;

				if (iSectorCnt & 1)
				{
					bboxSector.inf.x += static_cast<RwReal>(sWorldParam.WorldSectorSize);
				}
				if (iSectorCnt & 2)
				{
					bboxSector.inf.z += static_cast<RwReal>(sWorldParam.WorldSectorSize);
				}

				bboxSector.sup.x = bboxSector.inf.x + static_cast<RwReal>(sWorldParam.WorldSectorSize);
				bboxSector.sup.z = bboxSector.inf.z + static_cast<RwReal>(sWorldParam.WorldSectorSize);

				if (!(avDoodadTransform[2].x >= bboxSector.inf.x &&
					avDoodadTransform[2].z >= bboxSector.inf.z &&
					avDoodadTransform[2].x < bboxSector.sup.x &&
					avDoodadTransform[2].z < bboxSector.sup.z))
				{

					RwInt32	iWidthNum, iDepthNum;
					RwReal	fXBasedOn0, fZBasedOn0;
					RwInt32	iRetInvalidIdx, iRetValidIdx;

					fXBasedOn0		= bboxSector.inf.x + (sWorldParam.WorldSectorSize / 2) + sWorldParam.WorldSizeHalf;
					fZBasedOn0		= bboxSector.inf.z + (sWorldParam.WorldSectorSize / 2) + sWorldParam.WorldSizeHalf;
					iWidthNum		= (RwInt32)(fXBasedOn0 / (RwReal)sWorldParam.WorldSectorSize);
					iDepthNum		= (RwInt32)(fZBasedOn0 / (RwReal)sWorldParam.WorldSectorSize); 
					iRetInvalidIdx	= iWidthNum + (iDepthNum * dGET_WORLD_PARAM()->WorldSectorNum);

					fXBasedOn0		= avDoodadTransform[2].x + sWorldParam.WorldSizeHalf;
					fZBasedOn0		= avDoodadTransform[2].z + sWorldParam.WorldSizeHalf;
					iWidthNum		= (RwInt32)(fXBasedOn0 / (RwReal)sWorldParam.WorldSectorSize);
					iDepthNum		= (RwInt32)(fZBasedOn0 / (RwReal)sWorldParam.WorldSectorSize); 
					iRetValidIdx	= iWidthNum + (iDepthNum * dGET_WORLD_PARAM()->WorldSectorNum);

					fprintf_s(pFile, "POS(%.2f, %.2f, %.2f) INVALID_SECTOR(%d) VALID_SECTOR(%d)\n",
						avDoodadTransform[2].x, avDoodadTransform[2].y, avDoodadTransform[2].z, iRetInvalidIdx, iRetValidIdx);
				}

				// PS Map Size
				RwInt32 PSAtomicCnt = 0;
				CopyMemory(&PSAtomicCnt, pFileMemTmp, sizeof(RwInt32));
				pFileMemTmp += sizeof(RwInt32);

				for(RwInt32 j = 0; j < PSAtomicCnt; ++j)
				{
					// Map name
					pFileMemTmp += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

					// Frustum view window extent
					pFileMemTmp += (sizeof(RwReal));

					// Light matrices
					pFileMemTmp += (sizeof(RwMatrix));

					// Map size
					RwInt32 Size = 0;
					CopyMemory(&Size, pFileMemTmp, sizeof(RwInt32));
					pFileMemTmp += (sizeof(RwInt32));

					// Map
					pFileMemTmp += (sizeof(DWORD) * Size * Size);
				}

				// milepost
				RwBool EnableFlg;
				CopyMemory(&EnableFlg, pFileMemTmp, sizeof(RwBool));
				pFileMemTmp += sizeof(RwBool);

				if(EnableFlg)
				{
					pFileMemTmp += sizeof(RwUInt32);
				}

				// attach light object skip
				RwInt32 iLightObjectNum;
				CopyMemory(&iLightObjectNum, pFileMemTmp, sizeof(RwInt32));
				pFileMemTmp += sizeof(RwInt32);

				for (int j = 0; j < iLightObjectNum; ++j)
				{
					pFileMemTmp += sizeof(RwChar) * MAX_ATTR_BONE_NAME;

					pFileMemTmp += sizeof(RpLightType);
					pFileMemTmp += sizeof(RwBool);

					pFileMemTmp += sizeof(RwV3d);

					pFileMemTmp += sizeof(RwV3d);
					pFileMemTmp += sizeof(RwRGBAReal);
					pFileMemTmp += sizeof(RwReal);
					pFileMemTmp += sizeof(RwReal);

					pFileMemTmp += sizeof(RwBool);
					pFileMemTmp += sizeof(RwBool);
				}
			}

			pFileMemTmp = Skip_Water(pFileMemTmp);
			pFileMemTmp = Skip_Shadow(pFileMemTmp);
			pFileMemTmp = Skip_Effects(pFileMemTmp);
			pFileMemTmp = Skip_SE(pFileMemTmp);
			// CZ-SS
			//pFileMemTmp = Skip_SS(pFileMemTmp);
			pFileMemTmp = Skip_SectorCull(pFileMemTmp);
			pFileMemTmp = Skip_TileTransparency(pFileMemTmp);
			pFileMemTmp = Skip_Shoreline(pFileMemTmp);
			pFileMemTmp = Skip_Decal(pFileMemTmp);
			pFileMemTmp = Skip_Plant(pFileMemTmp);
			pFileMemTmp = Skip_WorldLight(pFileMemTmp);
			pFileMemTmp = Skip_Occluder(pFileMemTmp);
			pFileMemTmp = Skip_HeatHazeObject(pFileMemTmp);
			pFileMemTmp = Skip_LightObject(pFileMemTmp);
			pFileMemTmp = Skip_Dojo(pFileMemTmp);
		}

		FileMem.Free();
	}

	if (pFile)
	{
		fclose(pFile);
	}

	::SetCurrentDirectory(acOldPath);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnProofsInvalidSectorLink(RwInt32 FieldIdx)
{
	CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(m_pFieldMgr->GetFields());

	if(!m_pFieldMgr->IsFieldValid(FieldIdx))
	{
		DBO_TRACE(FALSE, "CNtlWorldMergeManager::OnProofsInvalidSectorLink, invalid field index.");
		return FALSE;
	}

	CFileMem	FileMem;
	BYTE*		pPosCurFileMem;
	FILE*		pFile;

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	FileMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");

	if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		return FALSE;
	}

	pPosCurFileMem = FileMem.GetDat();

	for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldFieldSectorNum; ++i)
	{
		pPosCurFileMem = Save_Material(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Heightfield(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Diffuse(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Doodads_ProofsInvalidSectorLink(pPosCurFileMem, pFile, FieldIdx, i);
		pPosCurFileMem = Save_Water(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Shadow(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Effects(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_SE(pPosCurFileMem, pFile);
		// CZ-SS
		//pPosCurFileMem = Save_SS(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_SectorCull(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_TileTransparency(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Shoreline(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Decal(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Plant(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_WorldLight(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Occluder(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_HeatHazeObject(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_LightObject(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Dojo(pPosCurFileMem, pFile);
	}

	fclose(pFile);

	m_pFieldMgr->RefreshField(FieldIdx);

	FileMem.Free();

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

RwBool CNtlWorldMergeManager::OnObjVisibilityDistChange(RwInt32 FieldIdx, RwReal VisibilityDist, RwReal Min, RwReal Max)
{
	CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(m_pFieldMgr->GetFields());

	if(!m_pFieldMgr->IsFieldValid(FieldIdx))
	{
		DBO_TRACE(FALSE, "CNtlWorldMergeManager::OnObjVisibilityDistChange, invalid field index.");
		return FALSE;
	}

	CFileMem	FileMem;
	BYTE*		pPosCurFileMem;
	FILE*		pFile;

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", FieldIdx);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	FileMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");

	if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		return FALSE;
	}

	pPosCurFileMem = FileMem.GetDat();

	for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldFieldSectorNum; ++i)
	{
		pPosCurFileMem = Save_Material(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Heightfield(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Diffuse(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Doodads_VisibilityDist(pPosCurFileMem, pFile, VisibilityDist, Min, Max);
		pPosCurFileMem = Save_Water(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Shadow(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Effects(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_SE(pPosCurFileMem, pFile);
		// CZ-SS
		//pPosCurFileMem = Save_SS(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_SectorCull(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_TileTransparency(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Shoreline(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Decal(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Plant(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_WorldLight(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Occluder(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_HeatHazeObject(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_LightObject(pPosCurFileMem, pFile);
		pPosCurFileMem = Save_Dojo(pPosCurFileMem, pFile);
	}

	fclose(pFile);

	m_pFieldMgr->RefreshField(FieldIdx);

	FileMem.Free();

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

RwBool CNtlWorldMergeManager::MergeInformation(sMERGE_PARAM &sParam, RwInt32 IdxFieldSrc, RwInt32 IdxFieldDst)
{
	BYTE*	pPosCurFileMemSrc;
	BYTE*	pPosCurFileMemDst;
	FILE*	pFileMerge;

	_chdir(m_NtlWorldParam2Merge.WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(m_NtlWorldParam2Merge.WorldChar64Buf, 64, "wfif%d", IdxFieldSrc);
	_chdir(m_NtlWorldParam2Merge.WorldChar64Buf);
	m_FileMemSrc.Load(m_NtlWorldParam2Merge.WorldChar64Buf, "rb");

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxFieldDst);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	m_FileMemDst.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");

	// open dest. field file 2 merge as "wb"
	if(fopen_s(&pFileMerge, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		return FALSE;
	}

	// starting file memory position
	pPosCurFileMemSrc = GetSPosOfMaterial(m_FileMemSrc.GetDat());
	pPosCurFileMemDst = GetSPosOfMaterial(m_FileMemDst.GetDat());

	// loop sector's num per field & merge the proper spec.
	for(RwInt32 i = 0; i < dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldFieldSectorNum; ++i)
	{
		//////////////////////////////////////////////////////////////////////////
		// Information Update : Material
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bMaterial)
		{
			// merge area : move to material and save
			pPosCurFileMemDst = Skip_Material(pPosCurFileMemDst);
			pPosCurFileMemSrc = Save_Material(pPosCurFileMemSrc, pFileMerge);
		}
		else
		{
			// old area : move to material and save
			pPosCurFileMemSrc = Skip_Material(pPosCurFileMemSrc);
			pPosCurFileMemDst = Save_Material(pPosCurFileMemDst, pFileMerge);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : HeightField
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bHeightField)
		{
			// merge area : heightfield
			Save_Heightfield(pPosCurFileMemSrc, pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemDst = Skip_Heightfield(pPosCurFileMemDst);
			pPosCurFileMemSrc = Skip_Heightfield(pPosCurFileMemSrc);
		}
		else
		{
			// old area : heightfield
			pPosCurFileMemDst = Save_Heightfield(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_Heightfield(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Diffuse
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bDiffuse)
		{
			// merge area : light(limited to diffuse here, (sdw, sl, dif) belong to the swap files)
			pPosCurFileMemSrc = Save_Diffuse(pPosCurFileMemSrc, pFileMerge);
			pPosCurFileMemDst = Skip_Diffuse(pPosCurFileMemDst);
		}
		else
		{
			// old area : light(limited to diffuse here, (sdw, sl, dif) belong to the swap files)
			pPosCurFileMemDst = Save_Diffuse(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_Diffuse(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Doodads
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bDoodads)
		{
			// merge area : doodads
			Save_Doodads_Num(pPosCurFileMemSrc, pPosCurFileMemDst, pFileMerge);
			// Dest Stream이 꼭 인자로 들어가야 한다. 내부에서 Pos 이동을 하지 않는 왜냐면 Dest에 있는 Trigger를 그대로 유지하기 위해서다.
			Save_Doodads_Trigger(pPosCurFileMemDst, pFileMerge, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			// Src Stream이 꼭 인자로 들어가야 한다. 내부에서 Idx 계산에 따라 Pos가 이동 된다.
			Save_Doodads_NonTrigger(pPosCurFileMemSrc, pFileMerge, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemSrc = Skip_Doodads(pPosCurFileMemSrc);
			pPosCurFileMemDst = Skip_Doodads(pPosCurFileMemDst);
		}
		else
		{
			// old area : doodads
			pPosCurFileMemDst = Save_Doodads(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_Doodads(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Water
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bWater)
		{
			// merge area : water
			pPosCurFileMemSrc = Save_Water(pPosCurFileMemSrc, pFileMerge);
			pPosCurFileMemDst = Skip_Water(pPosCurFileMemDst);
		}
		else
		{
			// old area : water
			pPosCurFileMemDst = Save_Water(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_Water(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Shadow : Field Index가 다르면 안된다.
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bShadow && IdxFieldDst == IdxFieldSrc)
		{
			// merge area : static projection shadow
			pPosCurFileMemSrc = Save_Shadow(pPosCurFileMemSrc, pFileMerge);
			pPosCurFileMemDst = Skip_Shadow(pPosCurFileMemDst);
		}
		else
		{
			// old area : static projection shadow
			pPosCurFileMemDst = Save_Shadow(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_Shadow(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Effects
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bEffects)
		{
			// merge area : effects
			pPosCurFileMemSrc = Save_Effects(pPosCurFileMemSrc, pFileMerge, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = Skip_Effects(pPosCurFileMemDst);
		}
		else
		{
			// old area : effects
			pPosCurFileMemDst = Save_Effects(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_Effects(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : SE
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bSE)
		{
			// merge area : SE
			pPosCurFileMemSrc = Save_SE(pPosCurFileMemSrc, pFileMerge, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = Skip_SE(pPosCurFileMemDst);
		}
		else
		{
			// old area : SE
			pPosCurFileMemDst = Save_SE(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_SE(pPosCurFileMemSrc);
		}

		// CZ-SS
		//////////////////////////////////////////////////////////////////////////
		// Information Update : SS
		//////////////////////////////////////////////////////////////////////////
		// 		if (sParam.bSS)
		// 		{
		// 			// merge area : SS
		// 			pPosCurFileMemSrc = Save_SS(pPosCurFileMemSrc, pFileMerge, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
		// 			pPosCurFileMemDst = Skip_SS(pPosCurFileMemDst);
		// 		}
		// 		else
		// 		{
		// 			// old area : SS
		// 			pPosCurFileMemDst = Save_SS(pPosCurFileMemDst, pFileMerge);
		// 			pPosCurFileMemSrc = Skip_SS(pPosCurFileMemSrc);
		// 		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Sector Cull
		//////////////////////////////////////////////////////////////////////////			
		if (sParam.bSectorCull)
		{
			// merge area : Sector Cull
			pPosCurFileMemSrc = Save_SectorCull(pPosCurFileMemSrc, pFileMerge);
			pPosCurFileMemDst = Skip_SectorCull(pPosCurFileMemDst);
		}
		else
		{
			// old area : Sector Cull
			pPosCurFileMemDst = Save_SectorCull(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_SectorCull(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Tile Transparency
		//////////////////////////////////////////////////////////////////////////			
		if (sParam.bTileTransparency)
		{
			// merge area : Tile Transparency
			pPosCurFileMemSrc = Save_TileTransparency(pPosCurFileMemSrc, pFileMerge);
			pPosCurFileMemDst = Skip_TileTransparency(pPosCurFileMemDst);
		}
		else
		{
			// old area : Tile Transparency
			pPosCurFileMemDst = Save_TileTransparency(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_TileTransparency(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Shoreline
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bShoreline)
		{
			// merge area : Shoreline
			pPosCurFileMemSrc = Save_Shoreline(pPosCurFileMemSrc, pFileMerge, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = Skip_Shoreline(pPosCurFileMemDst);
		}
		else
		{
			// old area : shoreline
			pPosCurFileMemDst = Save_Shoreline(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_Shoreline(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Decal
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bDecal)
		{
			// merge area : terrain decals
			pPosCurFileMemSrc = Save_Decal(pPosCurFileMemSrc, pFileMerge, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = Skip_Decal(pPosCurFileMemDst);
		}
		else
		{
			// old area
			pPosCurFileMemDst = Save_Decal(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_Decal(pPosCurFileMemSrc);
		}	

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Plant
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bPlant)
		{
			// merge area : plant
			pPosCurFileMemSrc = Save_Plant(pPosCurFileMemSrc, pFileMerge);
			pPosCurFileMemDst = Skip_Plant(pPosCurFileMemDst);
		}
		else
		{
			// old area
			pPosCurFileMemDst = Save_Plant(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_Plant(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : WorldLight
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bWorldLight)
		{
			// merge area : world light
			pPosCurFileMemSrc = Save_WorldLight(pPosCurFileMemSrc, pFileMerge, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = Skip_WorldLight(pPosCurFileMemDst);
		}
		else
		{
			// old area
			pPosCurFileMemDst = Save_WorldLight(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_WorldLight(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Occluder
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bOccluder)
		{
			// merge area : occluder
			pPosCurFileMemSrc = Save_Occluder(pPosCurFileMemSrc, pFileMerge, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = Skip_Occluder(pPosCurFileMemDst);
		}
		else
		{
			// old area
			pPosCurFileMemDst = Save_Occluder(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_Occluder(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : HeatHaze
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bHeatHazeObject)
		{
			// merge area : haethaze
			pPosCurFileMemSrc = Save_HeatHazeObject(pPosCurFileMemSrc, pFileMerge, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = Skip_HeatHazeObject(pPosCurFileMemDst);
		}
		else
		{
			// old area
			pPosCurFileMemDst = Save_HeatHazeObject(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_HeatHazeObject(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Light Object
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bLightObject)
		{
			// merge area : light object
			pPosCurFileMemSrc = Save_LightObject(pPosCurFileMemSrc, pFileMerge, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = Skip_LightObject(pPosCurFileMemDst);
		}
		else
		{
			// old area
			pPosCurFileMemDst = Save_LightObject(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_LightObject(pPosCurFileMemSrc);
		}

		//////////////////////////////////////////////////////////////////////////
		// Information Update : Dojo
		//////////////////////////////////////////////////////////////////////////
		if (sParam.bDojo)
		{
			// merge area : dojo
			pPosCurFileMemSrc = Save_Dojo(pPosCurFileMemSrc, pFileMerge, IdxFieldSrc, IdxFieldDst, m_pFieldMgr);
			pPosCurFileMemDst = Skip_Dojo(pPosCurFileMemDst);
		}
		else
		{
			// old area
			pPosCurFileMemDst = Save_Dojo(pPosCurFileMemDst, pFileMerge);
			pPosCurFileMemSrc = Skip_Dojo(pPosCurFileMemSrc);
		}
	}

	fclose(pFileMerge);

	m_FileMemSrc.Free();
	m_FileMemDst.Free();

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	//////////////////////////////////////////////////////////////////////////
	// Heightfield Variation
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bHeightField)
	{
		OnHeightfieldVariation(IdxFieldDst);
	}

	return TRUE;
}

RwBool CNtlWorldMergeManager::MergeProperty(sMERGE_PARAM &sParam, RwInt32 IdxFieldSrc, RwInt32 IdxFieldDst)
{
	BYTE*	pPosCurFileMemSrc;
	BYTE*	pPosCurFileMemDst;
	FILE*	pFileMerge;

	_chdir(m_NtlWorldParam2Merge.WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(m_NtlWorldParam2Merge.WorldChar64Buf, 64, "wfif%d", IdxFieldSrc);
	_chdir(m_NtlWorldParam2Merge.WorldChar64Buf);
	sprintf_s(m_NtlWorldParam2Merge.WorldChar64Buf, 64, "wfpf%d", IdxFieldSrc);
	m_FileMemSrc.Load(m_NtlWorldParam2Merge.WorldChar64Buf, "rb");

	_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxFieldDst);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", IdxFieldDst);
	m_FileMemDst.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb");

	// open dest. field file 2 merge as "wb"
	if(fopen_s(&pFileMerge, dGET_WORLD_PARAM()->WorldChar64Buf, "wb"))
	{
		DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
		return FALSE;
	}

	pPosCurFileMemSrc = m_FileMemSrc.GetDat();
	pPosCurFileMemDst = m_FileMemDst.GetDat();

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : Name
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bName)
	{
		// merge area : name
		pPosCurFileMemSrc = Save_Name(pPosCurFileMemSrc, pFileMerge);
		pPosCurFileMemDst = Skip_Name(pPosCurFileMemDst);

	}
	else
	{
		// old area : name
		pPosCurFileMemSrc = Skip_Name(pPosCurFileMemSrc);
		pPosCurFileMemDst = Save_Name(pPosCurFileMemDst, pFileMerge);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : BitProp
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bBitProp)
	{
		// merge area : bitprop
		pPosCurFileMemSrc = Save_BitProp(pPosCurFileMemSrc, pFileMerge);
		pPosCurFileMemDst = Skip_BitProp(pPosCurFileMemDst);
	}
	else
	{
		// old area : bitprop
		pPosCurFileMemSrc = Skip_BitProp(pPosCurFileMemSrc);
		pPosCurFileMemDst = Save_BitProp(pPosCurFileMemDst, pFileMerge);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : Fog
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bFog)
	{
		// merge area : fog
		pPosCurFileMemDst = Skip_Fog(pPosCurFileMemDst);
		pPosCurFileMemSrc = Save_Fog(pPosCurFileMemSrc, pFileMerge);
	}
	else
	{
		// old area : fog
		pPosCurFileMemSrc = Skip_Fog(pPosCurFileMemSrc);
		pPosCurFileMemDst = Save_Fog(pPosCurFileMemDst, pFileMerge);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : Sky
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bSky)
	{
		// merge area : sky
		pPosCurFileMemDst = Skip_Sky(pPosCurFileMemDst);
		pPosCurFileMemSrc = Save_Sky(pPosCurFileMemSrc, pFileMerge);
	}
	else
	{
		// old area : sky
		pPosCurFileMemSrc = Skip_Sky(pPosCurFileMemSrc);
		pPosCurFileMemDst = Save_Sky(pPosCurFileMemDst, pFileMerge);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : RealLight
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bRealLight)
	{
		// merge area : real light
		pPosCurFileMemDst = Skip_RealLight(pPosCurFileMemDst);
		pPosCurFileMemSrc = Save_RealLight(pPosCurFileMemSrc, pFileMerge);
	}
	else
	{
		// old area : real light
		pPosCurFileMemSrc = Skip_RealLight(pPosCurFileMemSrc);
		pPosCurFileMemDst = Save_RealLight(pPosCurFileMemDst, pFileMerge);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : BGM
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bBGM)
	{
		// merge area : bgm
		pPosCurFileMemDst = Skip_BGM(pPosCurFileMemDst);
		pPosCurFileMemSrc = Save_BGM(pPosCurFileMemSrc, pFileMerge);
	}
	else
	{
		// old area : bgm
		pPosCurFileMemSrc = Skip_BGM(pPosCurFileMemSrc);
		pPosCurFileMemDst = Save_BGM(pPosCurFileMemDst, pFileMerge);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : Bloom
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bBloom)
	{
		// merge area : bloom
		pPosCurFileMemDst = Skip_Bloom(pPosCurFileMemDst);
		pPosCurFileMemSrc = Save_Bloom(pPosCurFileMemSrc, pFileMerge);
	}
	else
	{
		// old area : bloom
		pPosCurFileMemSrc = Skip_Bloom(pPosCurFileMemSrc);
		pPosCurFileMemDst = Save_Bloom(pPosCurFileMemDst, pFileMerge);
	}			

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : Planet
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bPlanet)
	{
		// merge area : planets
		pPosCurFileMemDst = Skip_Planet(pPosCurFileMemDst);
		pPosCurFileMemSrc = Save_Planet(pPosCurFileMemSrc, pFileMerge);
	}
	else
	{
		// old area : planet
		pPosCurFileMemSrc = Skip_Planet(pPosCurFileMemSrc);
		pPosCurFileMemDst = Save_Planet(pPosCurFileMemDst, pFileMerge);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : Specular
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bSpecular)
	{
		// merge area : specular
		pPosCurFileMemDst = Skip_Specular(pPosCurFileMemDst);
		pPosCurFileMemSrc = Save_Specular(pPosCurFileMemSrc, pFileMerge);
	}
	else
	{
		// old area : specular
		pPosCurFileMemSrc = Skip_Specular(pPosCurFileMemSrc);
		pPosCurFileMemDst = Save_Specular(pPosCurFileMemDst, pFileMerge);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : WorldMaterial
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bWorldMaterial)
	{
		// merge area : world material
		pPosCurFileMemDst = Skip_WorldMaterial(pPosCurFileMemDst);
		pPosCurFileMemSrc = Save_WorldMaterial(pPosCurFileMemSrc, pFileMerge);
	}
	else
	{
		// old area : world material
		pPosCurFileMemSrc = Skip_WorldMaterial(pPosCurFileMemSrc);
		pPosCurFileMemDst = Save_WorldMaterial(pPosCurFileMemDst, pFileMerge);
	}		

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : FieldColor
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bFieldColor)
	{
		// merge area : field color
		pPosCurFileMemDst = Skip_FieldColor(pPosCurFileMemDst);
		pPosCurFileMemSrc = Save_FieldColor(pPosCurFileMemSrc, pFileMerge);
	}
	else
	{
		// old area : field color
		pPosCurFileMemSrc = Skip_FieldColor(pPosCurFileMemSrc);
		pPosCurFileMemDst = Save_FieldColor(pPosCurFileMemDst, pFileMerge);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : Weather
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bWeather)
	{
		// merge area : weather
		pPosCurFileMemDst = Skip_Weather(pPosCurFileMemDst);
		pPosCurFileMemSrc = Save_Weather(pPosCurFileMemSrc, pFileMerge);
	}
	else
	{
		// old area : weather
		pPosCurFileMemSrc = Skip_Weather(pPosCurFileMemSrc);
		pPosCurFileMemDst = Save_Weather(pPosCurFileMemDst, pFileMerge);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update : HeatHaze
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.bFieldHeatHaze)
	{
		// merge area : heathaze
		pPosCurFileMemDst = Skip_FieldHeatHaze(pPosCurFileMemDst);
		pPosCurFileMemSrc = Save_FieldHeatHaze(pPosCurFileMemSrc, pFileMerge);
	}
	else
	{
		// old area : heathaze
		pPosCurFileMemSrc = Skip_FieldHeatHaze(pPosCurFileMemSrc);
		pPosCurFileMemDst = Save_FieldHeatHaze(pPosCurFileMemDst, pFileMerge);
	}

	fclose(pFileMerge);

	m_FileMemSrc.Free();
	m_FileMemDst.Free();

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

RwBool CNtlWorldMergeManager::MergeSwapFile(sMERGE_PARAM &sParam, RwInt32 IdxFieldSrc, RwInt32 IdxFieldDst)
{
	CNtlWorldField*	pFields	= const_cast<CNtlWorldField*>(m_pFieldMgr->GetFields());

	int		l, m;
	int		SectorIdx;
	RwChar	strSrcPathName[NTL_MAX_DIR_PATH] = {0,};
	RwChar	strDstPathName[NTL_MAX_DIR_PATH] = {0,};

	// find sector indices at source merge area
	vector<RwInt32>	vecSectorIndices;
	RwV3d	SPos = pFields[IdxFieldSrc].GetSPos();
	RwV3d	SectorSPos;

	for(l = static_cast<RwInt32>(SPos.z); l < static_cast<RwInt32>(SPos.z) + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = static_cast<RwInt32>(SPos.x); m < static_cast<RwInt32>(SPos.x) + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			vecSectorIndices.push_back(m_pFieldMgr->GetSectorIdx(SectorSPos));
		}
	}

	// match each index and merge
	RwInt32 IdxSectorVec = 0;
	RwChar	SrcFileName[64] = {0,};

	SPos = pFields[IdxFieldDst].GetSPos();

	for(l = static_cast<RwInt32>(SPos.z); l < static_cast<RwInt32>(SPos.z) + dGET_WORLD_PARAM()->WorldFieldSize; l += dGET_WORLD_PARAM()->WorldSectorSize)
	{
		for(m = static_cast<RwInt32>(SPos.x); m < static_cast<RwInt32>(SPos.x) + dGET_WORLD_PARAM()->WorldFieldSize; m += dGET_WORLD_PARAM()->WorldSectorSize)
		{
			SectorSPos.x = (RwReal)m;
			SectorSPos.z = (RwReal)l;
			SectorIdx = m_pFieldMgr->GetSectorIdx(SectorSPos);

			//////////////////////////////////////////////////////////////////////////
			// Swap Data Update : SL
			//////////////////////////////////////////////////////////////////////////
			if (sParam.bDiffuse)
			{
				// sl files
				sprintf_s(SrcFileName, 64, "%d_sec.sl", vecSectorIndices[IdxSectorVec]);
				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.sl", SectorIdx);
				strcpy_s(strSrcPathName, NTL_MAX_DIR_PATH, m_NtlWorldParam2Merge.WorldProjectFolderName);
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, "swap\\sl\\");
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, SrcFileName);

				strcpy_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, "swap\\sl\\");
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

				DBO_TRACE(CopyFile(strSrcPathName, strDstPathName), "CNtlWorldMergeManager::MergeSwapFile, file copy failed.");
			}

			//////////////////////////////////////////////////////////////////////////
			// Swap Data Update : DIF
			//////////////////////////////////////////////////////////////////////////
			if (sParam.bDiffuse)
			{
				// dif files
				sprintf_s(SrcFileName, 64, "%d_sec.dif", vecSectorIndices[IdxSectorVec]);
				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.dif", SectorIdx);
				strcpy_s(strSrcPathName, NTL_MAX_DIR_PATH, m_NtlWorldParam2Merge.WorldProjectFolderName);
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, "swap\\");
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, SrcFileName);

				strcpy_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, "swap\\");
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

				DBO_TRACE(CopyFile(strSrcPathName, strDstPathName), "CNtlWorldMergeManager::MergeSwapFile, file copy failed.");
			}

			//////////////////////////////////////////////////////////////////////////
			// Swap Data Update : SDW
			//////////////////////////////////////////////////////////////////////////
			if (sParam.bShadow)
			{
				// sdw files
				sprintf_s(SrcFileName, 64, "%d_sec.sdw", vecSectorIndices[IdxSectorVec]);
				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.sdw", SectorIdx);
				strcpy_s(strSrcPathName, NTL_MAX_DIR_PATH, m_NtlWorldParam2Merge.WorldProjectFolderName);
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, "swap\\");
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, SrcFileName);

				strcpy_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, "swap\\");
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

				DBO_TRACE(CopyFile(strSrcPathName, strDstPathName), "CNtlWorldMergeManager::MergeSwapFile, file copy failed.");
			}

			//////////////////////////////////////////////////////////////////////////
			// Swap Data Update : OCQ
			//////////////////////////////////////////////////////////////////////////
			if (sParam.bOccluder)
			{
				// sdw files
				sprintf_s(SrcFileName, 64, "%d_sec.ocq", vecSectorIndices[IdxSectorVec]);
				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.ocq", SectorIdx);
				strcpy_s(strSrcPathName, NTL_MAX_DIR_PATH, m_NtlWorldParam2Merge.WorldProjectFolderName);
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, "swap\\occ\\");
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, SrcFileName);

				strcpy_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, "swap\\occ\\");
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

				DBO_TRACE(CopyFile(strSrcPathName, strDstPathName), "CNtlWorldMergeManager::MergeSwapFile, file copy failed.");
			}

			//////////////////////////////////////////////////////////////////////////
			// Swap Data Update : HHO
			//////////////////////////////////////////////////////////////////////////
			if (sParam.bHeatHazeObject)
			{
				// sdw files
				sprintf_s(SrcFileName, 64, "%d_sec.hho", vecSectorIndices[IdxSectorVec]);
				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.hho", SectorIdx);
				strcpy_s(strSrcPathName, NTL_MAX_DIR_PATH, m_NtlWorldParam2Merge.WorldProjectFolderName);
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, "swap\\hho\\");
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, SrcFileName);

				strcpy_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, "swap\\hho\\");
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

				DBO_TRACE(CopyFile(strSrcPathName, strDstPathName), "CNtlWorldMergeManager::MergeSwapFile, file copy failed.");
			}

			//////////////////////////////////////////////////////////////////////////
			// Swap Data Update : OBJ
			//////////////////////////////////////////////////////////////////////////
			if (sParam.bDoodads)
			{
				// sdw files
				sprintf_s(SrcFileName, 64, "%d_sec.obj", vecSectorIndices[IdxSectorVec]);
				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.obj", SectorIdx);
				strcpy_s(strSrcPathName, NTL_MAX_DIR_PATH, m_NtlWorldParam2Merge.WorldProjectFolderName);
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, "swap\\obj\\");
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, SrcFileName);

				strcpy_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, "swap\\obj\\");
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

				DBO_TRACE(CopyFile(strSrcPathName, strDstPathName), "CNtlWorldMergeManager::MergeSwapFile, file copy failed.");
			}

			//////////////////////////////////////////////////////////////////////////
			// Swap Data Update : EFF
			//////////////////////////////////////////////////////////////////////////
			if (sParam.bEffects)
			{
				// sdw files
				sprintf_s(SrcFileName, 64, "%d_sec.eff", vecSectorIndices[IdxSectorVec]);
				sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "%d_sec.eff", SectorIdx);
				strcpy_s(strSrcPathName, NTL_MAX_DIR_PATH, m_NtlWorldParam2Merge.WorldProjectFolderName);
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, "swap\\eff\\");
				strcat_s(strSrcPathName, NTL_MAX_DIR_PATH, SrcFileName);

				strcpy_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldProjectFolderName);
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, "swap\\eff\\");
				strcat_s(strDstPathName, NTL_MAX_DIR_PATH, dGET_WORLD_PARAM()->WorldChar64Buf);

				DBO_TRACE(CopyFile(strSrcPathName, strDstPathName), "CNtlWorldMergeManager::MergeSwapFile, file copy failed.");
			}

			++IdxSectorVec;
		}
	}

	_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);

	return TRUE;
}

void CNtlWorldMergeManager::RefreshField(RwInt32 IdxField)
{
	// refresh fields
	vector<RwInt32> vecIdxFields;
	m_pFieldMgr->GetNeighborFields(IdxField, vecIdxFields);

	for(RwUInt32 i = 0; i < vecIdxFields.size(); ++i)
	{
		if(vecIdxFields[i] != -1)
		{
			m_pFieldMgr->RefreshField(vecIdxFields[i]);
		}
	}
	m_pFieldMgr->RefreshField(IdxField);

	// refresh slope lighting
	for(RwUInt32 i = 0; i < vecIdxFields.size(); ++i)
	{
		if(vecIdxFields[i] != -1)
		{
			m_pFieldMgr->OnSetSlopeLighting(static_cast<RwUInt32>(vecIdxFields[i]));
		}
	}
	m_pFieldMgr->OnSetSlopeLighting(static_cast<RwUInt32>(IdxField));
}

RwBool CNtlWorldMergeManager::Merge(sMERGE_PARAM &sParam, RwInt32 IdxFieldSrc, RwInt32 IdxFieldDst)
{
	//////////////////////////////////////////////////////////////////////////
	// Information Update
	//////////////////////////////////////////////////////////////////////////
	if (sParam.IsInformationUpdate())
	{
		MergeInformation(sParam, IdxFieldSrc, IdxFieldDst);
	}

	//////////////////////////////////////////////////////////////////////////
	// World Property Update
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.IsWorldPropertyUpdate())
	{
		MergeProperty(sParam, IdxFieldSrc, IdxFieldDst);			
	}

	//////////////////////////////////////////////////////////////////////////
	// Swap Data Update
	//////////////////////////////////////////////////////////////////////////	
	if (sParam.IsSwapDataUpdate())
	{
		MergeSwapFile(sParam, IdxFieldSrc, IdxFieldDst);
	}

	if (sParam.IsWorldParamUpdate())
	{
		SaveCurWorldState();
	}

	//////////////////////////////////////////////////////////////////////////
	// Refresh Fields
	//////////////////////////////////////////////////////////////////////////	
	RefreshField(IdxFieldDst);

	return TRUE;
}


RwBool CNtlWorldMergeManager::IsFieldValid(RwInt32 IdxSField, RwInt32 IdxEField)
{
	// verify field indices
	if(!m_pFieldMgr->IsFieldValid(IdxSField))
	{
		DBO_TRACE(FALSE, "CNtlWorldMergeManager::IsFieldValid, invalid field index");
		return FALSE;
	}
	else
	{
		if(IdxEField != -1)
		{
			if(!m_pFieldMgr->IsFieldValid(IdxEField))
			{
				DBO_TRACE(FALSE, "CNtlWorldMergeManager::IsFieldValid, invalid field index");
				return FALSE;
			}
		}
	}
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
RwUInt8 g_auiPartyColor[7][3] = {{255, 0, 0}, {255, 128, 64}, {255, 255, 0}, {0, 255, 0}, {0, 0, 255}, {0, 0, 64}, {128, 0, 128}};

CSpawnMergeContainer::CSpawnMergeContainer()
:m_eSpawnType(-1)
{
}

CSpawnMergeContainer::~CSpawnMergeContainer()
{
	Destroy();
}

void CSpawnMergeContainer::Destroy()
{
	for (MAP_PARTY_IT it = m_mapParty.begin(); it != m_mapParty.end(); ++it)
	{
		SParty* pParty = it->second;
		NTL_DELETE(pParty);
	}
	m_mapParty.clear();

	for (VEC_SPAWN_IT it = m_vecSpawn.begin(); it != m_vecSpawn.end(); ++it)
	{
		SSpawn* pSpawn = *it;
		NTL_DELETE(pSpawn);
	}
	m_vecSpawn.clear();

	m_eSpawnType = -1;
}

RwBool CSpawnMergeContainer::Load(char *pcFileName)
{
	if (m_eSpawnType != -1)
	{
		Destroy();
	}

	FILE* pFile;
	if(::fopen_s(&pFile, pcFileName, "rb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << pcFileName << ")");
		return FALSE;
	}

	m_strFilename = pcFileName;

	fread(&m_eSpawnType, sizeof(RwInt32), 1, pFile);

	LoadParty(pFile);
	LoadSpawn(pFile);

	LinkSpawnParty();

	fclose(pFile);

	return TRUE;
}

RwBool CSpawnMergeContainer::Save()
{
	if (m_eSpawnType == -1)
	{
		return FALSE;
	}

	FILE* pFile;
	if(::fopen_s(&pFile, m_strFilename.c_str(), "wb") != 0)
	{
		DBO_TRACE(FALSE, "file open failed. (" << m_strFilename.c_str() << ")");
		return FALSE;
	}

	fwrite(&m_eSpawnType, sizeof(RwInt32), 1, pFile);

	SaveParty(pFile);
	SaveSpawn(pFile);

	fclose(pFile);

	return TRUE;
}

void CSpawnMergeContainer::LoadParty(FILE* pFile)
{
	fread(&m_iPartyIdx, sizeof(DWORD), 1, pFile);
	fread(&m_iPartyColorIdx, sizeof(RwInt32), 1, pFile);

	// Party count
	RwInt32	iNum = 0;
	fread(&iNum, sizeof(RwInt32), 1, pFile);

	for(RwInt32 i = 0; i < iNum; ++i)
	{
		SParty* pParty = NTL_NEW SParty;
		// Party index
		fread(&pParty->dwPartyIndex, sizeof(DWORD), 1, pFile);

		// Party prop.
		fread(&pParty->rgbaPartyColor.red, sizeof(RwUInt8), 1, pFile);
		fread(&pParty->rgbaPartyColor.green, sizeof(RwUInt8), 1, pFile);
		fread(&pParty->rgbaPartyColor.blue, sizeof(RwUInt8), 1, pFile);
		pParty->rgbaPartyColor.alpha = 255;

		// Follow
		fread(&pParty->bFollow, sizeof(RwBool), 1, pFile);

		fread(&pParty->dwCurPartyIndex, sizeof(DWORD), 1, pFile);

		m_mapParty.insert(MAP_PARTY::value_type(pParty->dwPartyIndex, pParty));
	}
}

void CSpawnMergeContainer::LoadSpawn(FILE *pFile)
{
	RwInt32 iNum;
	fread(&iNum, sizeof(RwInt32), 1, pFile);

	for(RwInt32 i = 0; i < iNum; ++i)
	{
		SSpawn* pSpawn = NTL_NEW SSpawn;
		// GUID
		fread(&pSpawn->dwMdlGUID, sizeof(DWORD), 1, pFile);

		// Nest prop.
		fread(&pSpawn->iNestRange, sizeof(RwInt32), 1, pFile);
		fread(&pSpawn->iNestSpawnCnt, sizeof(RwInt32), 1, pFile);
		fread(&pSpawn->iNestPathIndex, sizeof(RwInt32), 1, pFile);

		// Party index.
		fread(&pSpawn->iPartyIndex, sizeof(RwInt32), 1, pFile);

		fread(&pSpawn->bPartyLeader, sizeof(RwBool), 1, pFile);

		// Follow distance.
		fread(&pSpawn->vFollowDist, sizeof(RwV3d), 1, pFile);

		// Macro var.
		fread(&pSpawn->sSpawnMacro.m_CoolTime, sizeof(RwInt32), 1, pFile);
		fread(&pSpawn->sSpawnMacro.m_MoveType, sizeof(RwInt32), 1, pFile);
		fread(&pSpawn->sSpawnMacro.m_WanderRange, sizeof(RwReal), 1, pFile);
		fread(&pSpawn->sSpawnMacro.m_MoveRange, sizeof(RwReal), 1, pFile);
		fread(&pSpawn->sSpawnMacro.m_DelayTime, sizeof(RwReal), 1, pFile);

		// model name
		fread(&pSpawn->iLenModelName, sizeof(RwInt32), 1, pFile);
		fread(pSpawn->acModelName, sizeof(RwChar) * pSpawn->iLenModelName, 1, pFile);

		// display name
		fread(&pSpawn->iLenDisplayName, sizeof(RwInt32), 1, pFile);
		fread(pSpawn->acDisplayName, sizeof(RwChar) * pSpawn->iLenDisplayName, 1, pFile);

		// S, R, T
		fread(&pSpawn->fS, sizeof(RwReal), 1, pFile);
		fread(&pSpawn->vR, sizeof(RwV3d), 1, pFile);
		fread(&pSpawn->vT, sizeof(RwV3d), 1, pFile);

		// group indices
		fread(&pSpawn->iGroupIdx, sizeof(DWORD), 1, pFile);
		fread(&pSpawn->iPlayScriptIdx, sizeof(DWORD), 1, pFile);
		fread(&pSpawn->iPlayScriptSceneIdx, sizeof(DWORD), 1, pFile);

		// SPAWN FORMAT
		fread(&pSpawn->iAIScriptIdx, sizeof(DWORD), 1, pFile);
		fread(&pSpawn->iAIScriptSceneIdx, sizeof(DWORD), 1, pFile);

		m_vecSpawn.push_back(pSpawn);
	}
}

void CSpawnMergeContainer::SaveParty(FILE* pFile)
{
	fwrite(&m_iPartyIdx, sizeof(DWORD), 1, pFile);
	fwrite(&m_iPartyColorIdx, sizeof(RwInt32), 1, pFile);

	// Party count
	RwInt32	iNum = 0;
	for (MAP_PARTY_IT it = m_mapParty.begin(); it != m_mapParty.end(); ++it)
	{
		SParty* pParty = it->second;
		if (pParty != NULL)
		{
			++iNum;
		}
	}
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);

	for (MAP_PARTY_IT it = m_mapParty.begin(); it != m_mapParty.end(); ++it)
	{
		SParty* pParty = it->second;
		if (pParty != NULL)
		{
			// Party index
			fwrite(&pParty->dwPartyIndex, sizeof(DWORD), 1, pFile);

			// Party prop.
			fwrite(&pParty->rgbaPartyColor.red, sizeof(RwUInt8), 1, pFile);
			fwrite(&pParty->rgbaPartyColor.green, sizeof(RwUInt8), 1, pFile);
			fwrite(&pParty->rgbaPartyColor.blue, sizeof(RwUInt8), 1, pFile);

			// Follow
			fwrite(&pParty->bFollow, sizeof(RwBool), 1, pFile);

			fwrite(&pParty->dwCurPartyIndex, sizeof(DWORD), 1, pFile);
		}
	}
}

void CSpawnMergeContainer::SaveSpawn(FILE *pFile)
{
	RwInt32 iNum = m_vecSpawn.size();
	fwrite(&iNum, sizeof(RwInt32), 1, pFile);

	for(RwInt32 i = 0; i < iNum; ++i)
	{
		SSpawn* pSpawn = m_vecSpawn.at(i);
		// GUID
		fwrite(&pSpawn->dwMdlGUID, sizeof(DWORD), 1, pFile);

		// Nest prop.
		fwrite(&pSpawn->iNestRange, sizeof(RwInt32), 1, pFile);
		fwrite(&pSpawn->iNestSpawnCnt, sizeof(RwInt32), 1, pFile);
		fwrite(&pSpawn->iNestPathIndex, sizeof(RwInt32), 1, pFile);

		// Party index.
		fwrite(&pSpawn->iPartyIndex, sizeof(RwInt32), 1, pFile);

		fwrite(&pSpawn->bPartyLeader, sizeof(RwBool), 1, pFile);

		// Follow distance.
		fwrite(&pSpawn->vFollowDist, sizeof(RwV3d), 1, pFile);

		// Macro var.
		fwrite(&pSpawn->sSpawnMacro.m_CoolTime, sizeof(RwInt32), 1, pFile);
		fwrite(&pSpawn->sSpawnMacro.m_MoveType, sizeof(RwInt32), 1, pFile);
		fwrite(&pSpawn->sSpawnMacro.m_WanderRange, sizeof(RwReal), 1, pFile);
		fwrite(&pSpawn->sSpawnMacro.m_MoveRange, sizeof(RwReal), 1, pFile);
		fwrite(&pSpawn->sSpawnMacro.m_DelayTime, sizeof(RwReal), 1, pFile);

		// model name
		fwrite(&pSpawn->iLenModelName, sizeof(RwInt32), 1, pFile);
		fwrite(pSpawn->acModelName, sizeof(RwChar) * pSpawn->iLenModelName, 1, pFile);

		// display name
		fwrite(&pSpawn->iLenDisplayName, sizeof(RwInt32), 1, pFile);
		fwrite(pSpawn->acDisplayName, sizeof(RwChar) * pSpawn->iLenDisplayName, 1, pFile);

		// S, R, T
		fwrite(&pSpawn->fS, sizeof(RwReal), 1, pFile);
		fwrite(&pSpawn->vR, sizeof(RwV3d), 1, pFile);
		fwrite(&pSpawn->vT, sizeof(RwV3d), 1, pFile);

		// group indices
		fwrite(&pSpawn->iGroupIdx, sizeof(DWORD), 1, pFile);
		fwrite(&pSpawn->iPlayScriptIdx, sizeof(DWORD), 1, pFile);
		fwrite(&pSpawn->iPlayScriptSceneIdx, sizeof(DWORD), 1, pFile);

		// SPAWN FORMAT
		fwrite(&pSpawn->iAIScriptIdx, sizeof(DWORD), 1, pFile);
		fwrite(&pSpawn->iAIScriptSceneIdx, sizeof(DWORD), 1, pFile);
	}
}

void CSpawnMergeContainer::LinkSpawnParty()
{
	for (int i = 0; i < (int)m_vecSpawn.size(); ++i)
	{
		SSpawn* pSpawn = m_vecSpawn.at(i);
		if (pSpawn->iPartyIndex != -1)
		{
			MAP_PARTY_IT it = m_mapParty.find(pSpawn->iPartyIndex);
			if (it != m_mapParty.end())
			{
				it->second->vecPartySpawn.push_back(pSpawn);
			}
		}
	}
}

void CSpawnMergeContainer::OptSpawnParty()
{
	MAP_PARTY	mapPartyNew;
	DWORD		dwPartyIdxNew = 0;
	for (MAP_PARTY_IT it = m_mapParty.begin(); it != m_mapParty.end(); ++it)
	{
		SParty* pParty = it->second;
		if (pParty == NULL)
		{
			continue;
		}

		pParty->dwPartyIndex		= ++dwPartyIdxNew;
		pParty->rgbaPartyColor.red	= g_auiPartyColor[dwPartyIdxNew % 7][0];
		pParty->rgbaPartyColor.green= g_auiPartyColor[dwPartyIdxNew % 7][1];
		pParty->rgbaPartyColor.blue	= g_auiPartyColor[dwPartyIdxNew % 7][2];

		for (int i = 0; i < (int)pParty->vecPartySpawn.size(); ++i)
		{
			pParty->vecPartySpawn.at(i)->iPartyIndex = dwPartyIdxNew;
		}
		mapPartyNew[dwPartyIdxNew] = pParty;
	}
	m_iPartyIdx			= dwPartyIdxNew;
	m_iPartyColorIdx	= (dwPartyIdxNew % 7);

	m_mapParty.swap(mapPartyNew);
}

void CSpawnMergeContainer::DeleteSpawn(RwBBox bBox)
{
	// Non Party Spawn
	VEC_SPAWN_IT itSpawn = m_vecSpawn.begin();
	while (itSpawn != m_vecSpawn.end())
	{
		SSpawn* pSpawn = *itSpawn;
		if (pSpawn->iPartyIndex == -1)
		{
			if (bBox.inf.x <= pSpawn->vT.x && bBox.sup.x > pSpawn->vT.x &&
				bBox.inf.z <= pSpawn->vT.z && bBox.sup.z > pSpawn->vT.z)
			{
				itSpawn = m_vecSpawn.erase(itSpawn);
				NTL_DELETE(pSpawn);
				continue;
			}
		}
		++itSpawn;
	}

	// Party Spawn
	MAP_PARTY_IT itParty = m_mapParty.begin();
	while (itParty != m_mapParty.end())
	{
		SParty* pParty	= itParty->second;
		RwBool	bCheck	= FALSE;

		itSpawn = pParty->vecPartySpawn.begin();
		while (itSpawn != pParty->vecPartySpawn.end())
		{
			SSpawn* pSpawn = *itSpawn;

			if (bBox.inf.x <= pSpawn->vT.x && bBox.sup.x > pSpawn->vT.x &&
				bBox.inf.z <= pSpawn->vT.z && bBox.sup.z > pSpawn->vT.z)
			{
				itSpawn = pParty->vecPartySpawn.erase(itSpawn);

				VEC_SPAWN_IT itSpawnDelete = find(m_vecSpawn.begin(), m_vecSpawn.end(), pSpawn);
				if (itSpawnDelete != m_vecSpawn.end())
				{
					m_vecSpawn.erase(itSpawnDelete);
				}

				NTL_DELETE(pSpawn);
				bCheck = TRUE;
				continue;
			}
			++itSpawn;
		}
		if (bCheck)
		{
			if (pParty->vecPartySpawn.empty())
			{
				itParty = m_mapParty.erase(itParty);
				NTL_DELETE(pParty);

				continue;
			}
			else
			{
				DBO_TRACE(FALSE, "LOG SPAWN MERGE DELETE PARTY");
			}
		}
		++itParty;
	}
}

void CSpawnMergeContainer::InsertSpawn(CSpawnMergeContainer* pSpawnMergeContainer, RwBBox bBoxSrc, RwBBox bBoxDst)
{
	// Non Party Spawn
	VEC_SPAWN_IT itSpawn = pSpawnMergeContainer->m_vecSpawn.begin();
	while (itSpawn != pSpawnMergeContainer->m_vecSpawn.end())
	{
		SSpawn* pSpawn = *itSpawn;
		if (pSpawn->iPartyIndex == -1)
		{
			if (bBoxSrc.inf.x <= pSpawn->vT.x && bBoxSrc.sup.x > pSpawn->vT.x &&
				bBoxSrc.inf.z <= pSpawn->vT.z && bBoxSrc.sup.z > pSpawn->vT.z)
			{
				SSpawn* pSpawnCopy = NTL_NEW SSpawn;
				*pSpawnCopy = *pSpawn;
				m_vecSpawn.push_back(pSpawnCopy);

				pSpawnCopy->vT.x = bBoxDst.inf.x + (pSpawn->vT.x - bBoxSrc.inf.x);
				pSpawnCopy->vT.z = bBoxDst.inf.z + (pSpawn->vT.z - bBoxSrc.inf.z);
			}
		}
		++itSpawn;
	}

	// Party Spawn
	MAP_PARTY_IT itParty = pSpawnMergeContainer->m_mapParty.begin();
	while (itParty != pSpawnMergeContainer->m_mapParty.end())
	{
		SParty* pParty	= itParty->second;
		RwBool	bCheck	= FALSE;

		SParty* pPartyNew = NULL;

		itSpawn = pParty->vecPartySpawn.begin();
		while (itSpawn != pParty->vecPartySpawn.end())
		{
			SSpawn* pSpawn = *itSpawn;

			if (bBoxSrc.inf.x <= pSpawn->vT.x && bBoxSrc.sup.x > pSpawn->vT.x &&
				bBoxSrc.inf.z <= pSpawn->vT.z && bBoxSrc.sup.z > pSpawn->vT.z)
			{
				if (bCheck == FALSE)
				{
					pPartyNew = NTL_NEW SParty;
					*pPartyNew = *pParty;
					pPartyNew->vecPartySpawn.clear();

					pPartyNew->dwPartyIndex			= ++m_iPartyIdx;
					m_iPartyColorIdx				= (m_iPartyIdx % 7);
					pPartyNew->rgbaPartyColor.red	= g_auiPartyColor[m_iPartyColorIdx][0];
					pPartyNew->rgbaPartyColor.green	= g_auiPartyColor[m_iPartyColorIdx][1];
					pPartyNew->rgbaPartyColor.blue	= g_auiPartyColor[m_iPartyColorIdx][2];

					m_mapParty.insert(MAP_PARTY::value_type(pPartyNew->dwPartyIndex, pPartyNew));					
				}

				SSpawn* pSpawnCopy = NTL_NEW SSpawn;
				*pSpawnCopy = *pSpawn;
				pSpawnCopy->iPartyIndex = pPartyNew->dwPartyIndex;
				pPartyNew->vecPartySpawn.push_back(pSpawnCopy);
				m_vecSpawn.push_back(pSpawnCopy);

				pSpawnCopy->vT.x = bBoxDst.inf.x + (pSpawn->vT.x - bBoxSrc.inf.x);
				pSpawnCopy->vT.z = bBoxDst.inf.z + (pSpawn->vT.z - bBoxSrc.inf.z);

				bCheck = TRUE;
			}
			++itSpawn;
		}
		if (bCheck)
		{
			if (pParty->vecPartySpawn.size() != pPartyNew->vecPartySpawn.size())
			{
				DBO_TRACE(FALSE, "LOG SPAWN MERGE INSERT PARTY");
			}
		}
		++itParty;
	}
}

#endif

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif
