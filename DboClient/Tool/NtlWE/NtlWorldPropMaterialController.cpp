#include "StdAfx.h"
#include "NtlWE.h"
#include "NtlWEFrm.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"

#include "NtlWorldSectorInfo.h"

#include "ProgressWnd.h"

#include "NtlWorldPropMaterialController.h"

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#ifdef dNTL_WORLD_FILE

#define dWORLD_PROP_MATERIAL_CONTROLLER_SELECT_MATERIAL 105

CNtlWorldPropMaterialController::CNtlWorldPropMaterialController()
{
	for (int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		m_apbyAlphaBuffer[i] = NTL_NEW BYTE [(dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2)];
	}
	m_pbyBuffer = NTL_NEW BYTE [(dGET_WORLD_PARAM()->WorldSectorTileNum) * (dGET_WORLD_PARAM()->WorldSectorTileNum)];

	ClearTemporary();
}

CNtlWorldPropMaterialController::~CNtlWorldPropMaterialController()
{
	for (int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		NTL_ARRAY_DELETE(m_apbyAlphaBuffer[i]);
	}
	NTL_ARRAY_DELETE(m_pbyBuffer);
}

void CNtlWorldPropMaterialController::UpdatePropertyPosInLoadedWorld(RwV3d& vPos)
{
	CNtlWorldFieldManager*	pFieldMgr	= dGETMGR();
	RwInt32					iFieldIndex	= GetIndexPosInWorld(vPos);

	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
	CNtlWorldSector*		pSector		= pFieldMgr->m_pSectors;

	RwInt32 aSectorIndex[4];
	pFieldMgr->GetFieldSectorIndices(iFieldIndex, aSectorIndex);
	CopyMemory(m_cBTextureName, pFields[iFieldIndex].GetTexAttr().BLT.StrName, rwTEXTUREBASENAMELENGTH);
	for (int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		CopyMemory(m_acDTextureName[i], pFields[iFieldIndex].GetTexAttr().DLT[i].StrName, rwTEXTUREBASENAMELENGTH);
		for (int j = 0; j < 4; ++j)
		{
			CopyAlphaFromTexture(m_pbyBuffer, pSector[aSectorIndex[j]].m_pTextureAlpha[i]);
			CopyQuadBuffer(m_apbyAlphaBuffer[i], j, m_pbyBuffer);
		}
	}

	UpdatePropertyFromTemporary(vPos);
	ClearTemporary();
}

void CNtlWorldPropMaterialController::UpdatePropertyPosInUnloadedWorld(RwV3d& vPos)
{
	/*
	WorldFileFormat - FieldProperty
	*/

	CNtlWorldFieldManager*	pFieldMgr	= dGETMGR();
	RwInt32					iFieldIndex	= GetIndexPosInWorld(vPos);

	CFileMem				FieldFileMem;
	BYTE*					pFileMem;

	chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", iFieldIndex);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	if (!FieldFileMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
	{
		FieldFileMem.Free();
		return;
	}

	pFileMem = FieldFileMem.GetDat();
	//pFileMem = CNtlWorldFileMemAccessor::GetSPosOfMaterial(FieldFileMem.GetDat());

	for (int i = 0; i < dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldFieldSectorNum; ++i)
	{
		// base layer
		::CopyMemory(m_cBTextureName, pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);
		pFileMem += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH) + (sizeof(D3DXMATRIX) * 2) + (sizeof(RwReal)) + (sizeof(RwUInt8));

		// detail layer
		for (int j = 0; j < dNTL_SPLAT_LAYER_NUM; ++j)
		{
			RwBool bBool;

			::CopyMemory(&bBool, pFileMem, sizeof(RwBool));
			pFileMem += (sizeof(RwBool));

			if (bBool) // detail layer 蜡公
			{
				::CopyMemory(m_acDTextureName[j], pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);
				pFileMem += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

				::CopyMemory(&bBool, pFileMem, sizeof(RwBool));
				pFileMem += (sizeof(RwBool));

				if (bBool) // alpha layer 蜡公
				{
					pFileMem += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);
					
					::CopyMemory(m_pbyBuffer, pFileMem, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);
					pFileMem += (sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);
					CopyQuadBuffer(m_apbyAlphaBuffer[j], i, m_pbyBuffer);
				}

				pFileMem += (sizeof(D3DXMATRIX) * 2) + (sizeof(RwReal)) + (sizeof(RwUInt8));
			}
		}

		pFileMem = GetNtlWorldSectorInfo()->SectorHeightfieldSkipToFileMem(pFileMem);
		pFileMem = GetNtlWorldSectorInfo()->SectorPrelightsSkipToFileMem(pFileMem);
		pFileMem = GetNtlWorldSectorInfo()->SectorObjectSkipToFileMem(pFileMem, AW_HEGITHFIELD);
		pFileMem = GetNtlWorldSectorInfo()->SectorWaterSkipToFileMem(pFileMem);
		pFileMem = GetNtlWorldSectorInfo()->SectorShadowSkipToFileMem(pFileMem);
		pFileMem = GetNtlWorldSectorInfo()->SectorEffectSkipToFileMem(pFileMem);
		pFileMem = GetNtlWorldSectorInfo()->SectorSoundEffectSkipToFileMem(pFileMem);
		pFileMem = GetNtlWorldSectorInfo()->SectorCullSkipToFileMem(pFileMem);
		pFileMem = GetNtlWorldSectorInfo()->SectorTileTransparencySkipToFileMem(pFileMem);
		pFileMem = GetNtlWorldSectorInfo()->SectorShoreLineSkipToFileMem(pFileMem);
		pFileMem = GetNtlWorldSectorInfo()->SectorDecalSkipToFileMem(pFileMem);
		pFileMem = GetNtlWorldSectorInfo()->SectorPlantSkipToFileMem(pFileMem);
		pFileMem = GetNtlWorldSectorInfo()->SectorWorldLightSkipToFileMem(pFileMem);
		pFileMem = GetNtlWorldSectorInfo()->SectorOccluderSkipToFileMem(pFileMem);
		pFileMem = GetNtlWorldSectorInfo()->SectorHeatHazeObjectSkipToFileMem(pFileMem);
		pFileMem = GetNtlWorldSectorInfo()->SectorLightObjectSkipToFileMem(pFileMem);
		pFileMem = GetNtlWorldSectorInfo()->SectorDojoSkipToFileMem(pFileMem);
	}
	FieldFileMem.Free();

	UpdatePropertyFromTemporary(vPos);
	ClearTemporary();
}

void CNtlWorldPropMaterialController::DeletePropertyPosInWorld(RwV3d& vPos)
{
	CNtlWorldFieldManager*	pFieldMgr	= dGETMGR();
	pFieldMgr->SetAFieldProp(vPos, m_NtlFieldProp, eNFP_MATERIALPROP);
}

RwBool CNtlWorldPropMaterialController::LoadScript(const RwChar* pFileName)
{
	CNtlXMLDoc doc;
	doc.Create();
	if(doc.Load((char*)pFileName) == false)
	{
		return FALSE;
	}
	IXMLDOMNodeList *pList = doc.SelectNodeList((char*)"/object_property/object_element/element");

	long			lNum	= 0;
	IXMLDOMNode*	pNode	= NULL;

	pList->get_length(&lNum);
	for (long i = 0; i < lNum; ++i)
	{
		char cName[rwTEXTUREBASENAMELENGTH];
		char cMaterial[rwTEXTUREBASENAMELENGTH];

		pList->get_item(i, &pNode);
		if(doc.GetTextWithAttributeName(pNode, "name", cName, rwTEXTUREBASENAMELENGTH))
		{
			if (doc.GetTextWithAttributeName(pNode, "material", cMaterial, rwTEXTUREBASENAMELENGTH))
			{
				m_mapMaterial[cName] = ::atoi(cMaterial);
			}
		}

		if (pNode)
		{
			pNode->Release();
			pNode = NULL;
		}
	}

	if (pList)
	{
		pList->Release();
		pList = NULL;
	}
	return TRUE;
}

void CNtlWorldPropMaterialController::AddMaterial(char *pName, BYTE byMaterial)
{
	MAP_MATERIAL_IT it = m_mapMaterial.find(pName);
	if (it == m_mapMaterial.end())
	{
		m_mapMaterial.insert(MAP_MATERIAL::value_type(pName, byMaterial));
	}
}

BYTE CNtlWorldPropMaterialController::GetMaterial(char *pName)
{
	MAP_MATERIAL_IT it = m_mapMaterial.find(pName);
	if (it != m_mapMaterial.end())
	{
		return it->second;
	}
	return 0;
}

void CNtlWorldPropMaterialController::CopyAlphaFromTexture(BYTE* pBuffer, RwTexture* pTexture)
{
	if (!pTexture)
	{
		::ZeroMemory(pBuffer, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum) * (dGET_WORLD_PARAM()->WorldSectorTileNum));
		return;
	}

	RwInt32 RwTexW	= RwRasterGetWidth(pTexture->raster);
	RwInt32 RwTexH	= RwRasterGetWidth(pTexture->raster);
	DWORD*	pBits	= (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKREAD);
	DWORD*	pDest	= NULL;
	DWORD	Temp	= 0;
	RwInt32	iIndex	= 0;

	for (RwInt32 PosY = 0; PosY < RwTexH; ++PosY)
	{
		for (RwInt32 PosX = 0; PosX < RwTexW ; ++PosX)
		{
			pDest = pBits + RwTexW * PosY + PosX;
			Temp = (*pDest);

			pBuffer[RwTexW * PosY + PosX] = static_cast<BYTE>(Temp >> 24);
		}
	}

	RwRasterUnlock(pTexture->raster);
}

void CNtlWorldPropMaterialController::CopyQuadBuffer(BYTE* pQuadBuffer, RwInt32 iQuad, BYTE* pBuffer)
{
	RwInt32 RwTexW	= dGET_WORLD_PARAM()->WorldSectorTileNum;
	RwInt32 RwTexH	= dGET_WORLD_PARAM()->WorldSectorTileNum;
	BYTE*	pBits	= NULL;

	RwInt32 OffsetX = 0;
	RwInt32 OffsetY = 0;
	switch (iQuad)
	{
	case 0:
		OffsetX = 0;
		OffsetY = 0;
		break;
	case 1:
		OffsetX = dGET_WORLD_PARAM()->WorldSectorTileNum;
		OffsetY = 0;
		break;
	case 2:
		OffsetX = 0;
		OffsetY = dGET_WORLD_PARAM()->WorldSectorTileNum;
		break;
	case 3:
		OffsetX = dGET_WORLD_PARAM()->WorldSectorTileNum;
		OffsetY = dGET_WORLD_PARAM()->WorldSectorTileNum;
		break;
	}

	for (RwInt32 PosY = RwTexH - 1; PosY >= 0; --PosY)
	{
		for (RwInt32 PosX = RwTexW - 1; PosX >= 0; --PosX)
		{
			pBits = pBuffer + RwTexW * PosY + PosX;
			pQuadBuffer[((RwTexH - 1) - PosY + OffsetY) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) + ((RwTexW - 1) - PosX + OffsetX)] = *pBits;
		}
	}
}

void CNtlWorldPropMaterialController::ClearTemporary()
{
	for (int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		m_acDTextureName[i][0] = '\0';
		::ZeroMemory(m_apbyAlphaBuffer[i], sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2));
	}

	m_cBTextureName[0] = '\0';
	//::ZeroMemory(m_pbyBuffer, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum) * (dGET_WORLD_PARAM()->WorldSectorTileNum));
	::ZeroMemory(m_NtlFieldProp._pMaterialProp, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2));
}

void CNtlWorldPropMaterialController::UpdatePropertyFromTemporary(RwV3d& vPos)
{
	CNtlWorldFieldManager*	pFieldMgr	= dGETMGR();
	BYTE*					pResult		= m_NtlFieldProp._pMaterialProp;

	for (int i = 0; i < (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2); ++i)
	{
		BYTE	iSel = 255;
		BYTE	iMax = 0;
		RwInt32 iSum = 0;
		for (int j = dNTL_SPLAT_LAYER_NUM - 1; j >= 0 ; --j)
		{
			if (m_apbyAlphaBuffer[j][i] > iMax)
			{
				iMax = m_apbyAlphaBuffer[j][i];
				iSel = j;
			}
			iSum += m_apbyAlphaBuffer[j][i];
		}

		if (iSum >= dWORLD_PROP_MATERIAL_CONTROLLER_SELECT_MATERIAL)
		{
			pResult[i] = GetMaterial(m_acDTextureName[iSel]);
		}
		else
		{
			pResult[i] = GetMaterial(m_cBTextureName);
		}
	}
	pFieldMgr->SetAFieldProp(vPos, m_NtlFieldProp, eNFP_MATERIALPROP);
}

RwBool CNtlWorldPropMaterialController::SaveImage(const char* pFileName)
{
	RwInt32 iWidth	= dGET_WORLD_PARAM()->WorldFieldNum;
	RwInt32 iHeight = dGET_WORLD_PARAM()->WorldFieldNum;
	RwInt32 iDepth  = 32;

	CNtlWorldFieldManager*	pFieldMgr	= dGETMGR();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());

	RwImage*				pImage		= RwImageCreate(iWidth, iHeight, iDepth);
	if (!pImage)
	{
		DBO_TRACE(FALSE, "CNtlWorldPropMaterialController::SaveImage, image create failed.");
		return FALSE;
	}

	RwImageAllocatePixels(pImage);

	CProgressWnd WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "export material tile property.");
	WndProgress.GoModal();
	WndProgress.SetRange(0, dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum);	
	for (int i = 0; i < dGET_WORLD_PARAM()->WorldFieldNum; ++i)
	{
		for (int j = 0; j < dGET_WORLD_PARAM()->WorldFieldNum; ++j)
		{
			RwInt32 iFieldIndex = j + i * dGET_WORLD_PARAM()->WorldFieldNum;
			RwRGBA	rgba;
			rgba.alpha	= 255;
			rgba.red	= 0;
			rgba.green	= 0;
			rgba.blue	= 0;

			RwChar cText[64];
			::sprintf_s(cText, 64, "%d / %d fields is now saving.", iFieldIndex, dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum);
			WndProgress.SetText(cText);
			
			pFieldMgr->GetAFieldProp(pFields[iFieldIndex].GetSPos(), m_NtlFieldProp);
			for (int k = 0; k < (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2); ++k)
			{
				if (m_NtlFieldProp._pMaterialProp[k] > 0)
				{
					rgba.alpha	= 255;
					rgba.red	= 255;
					rgba.green	= 255;
					rgba.blue	= 255;
					break;
				}
			}
			::ZeroMemory(m_NtlFieldProp._pMaterialProp, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2));
			
			
			RwInt32 iPerWidth	= iWidth / dGET_WORLD_PARAM()->WorldFieldNum;
			RwInt32 iPerHeight	= iHeight / dGET_WORLD_PARAM()->WorldFieldNum;
			for (int l = 0; l < iPerHeight; ++l)
			{
				for (int m = 0; m < iPerWidth; ++m)
				{ 
					SetARGB(pImage, iWidth - (j * iPerWidth + m + 1), iHeight - (i * iPerHeight + l + 1), rgba);
				}
			}

			WndProgress.StepIt();
			WndProgress.PeekAndPump();
		}
	}

	RwChar acDir[NTL_MAX_DIR_PATH] = { 0, };
	RwChar acPath[NTL_MAX_DIR_PATH] = { 0, };

	GetCurrentDirectory(NTL_MAX_DIR_PATH, acDir);

	strcpy_s(acPath, pFileName);

	WndProgress.SetText("finalizing images.");
	RwImageWrite(pImage, acPath);

	WndProgress.SetText("Done.");
	RwImageDestroy(pImage);

	SetCurrentDirectory(acDir);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#else

#define dWORLD_PROP_MATERIAL_CONTROLLER_SELECT_MATERIAL 105

CNtlWorldPropMaterialController::CNtlWorldPropMaterialController()
{
	for (int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		m_apbyAlphaBuffer[i] = NTL_NEW BYTE [(dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2)];
	}
	m_pbyBuffer = NTL_NEW BYTE [(dGET_WORLD_PARAM()->WorldSectorTileNum) * (dGET_WORLD_PARAM()->WorldSectorTileNum)];

	ClearTemporary();
}

CNtlWorldPropMaterialController::~CNtlWorldPropMaterialController()
{
	for (int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		NTL_ARRAY_DELETE(m_apbyAlphaBuffer[i]);
	}
	NTL_ARRAY_DELETE(m_pbyBuffer);
}

void CNtlWorldPropMaterialController::UpdatePropertyPosInLoadedWorld(RwV3d& vPos)
{
	CNtlWorldFieldManager*	pFieldMgr	= dGETMGR();
	RwInt32					iFieldIndex	= GetIndexPosInWorld(vPos);

	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());
	CNtlWorldSector*		pSector		= pFieldMgr->m_pSectors;

	RwInt32 aSectorIndex[4];
	pFieldMgr->GetFieldSectorIndices(iFieldIndex, aSectorIndex);
	CopyMemory(m_cBTextureName, pFields[iFieldIndex].GetTexAttr().BLT.StrName, rwTEXTUREBASENAMELENGTH);
	for (int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		CopyMemory(m_acDTextureName[i], pFields[iFieldIndex].GetTexAttr().DLT[i].StrName, rwTEXTUREBASENAMELENGTH);
		for (int j = 0; j < 4; ++j)
		{
			CopyAlphaFromTexture(m_pbyBuffer, pSector[aSectorIndex[j]].m_pTextureAlpha[i]);
			CopyQuadBuffer(m_apbyAlphaBuffer[i], j, m_pbyBuffer);
		}
	}

	UpdatePropertyFromTemporary(vPos);
	ClearTemporary();
}

void CNtlWorldPropMaterialController::UpdatePropertyPosInUnloadedWorld(RwV3d& vPos)
{
	/*
	WorldFileFormat - FieldProperty
	*/

	CNtlWorldFieldManager*	pFieldMgr	= dGETMGR();
	RwInt32					iFieldIndex	= GetIndexPosInWorld(vPos);

	CFileMem				FieldFileMem;
	BYTE*					pFileMem;

	chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
	_chdir("fields");
	sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", iFieldIndex);
	_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);
	if (!FieldFileMem.Load(dGET_WORLD_PARAM()->WorldChar64Buf, "rb"))
	{
		FieldFileMem.Free();
		return;
	}

	pFileMem = CNtlWorldFileMemAccessor::GetSPosOfMaterial(FieldFileMem.GetDat());

	for (int i = 0; i < dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldFieldSectorNum; ++i)
	{
		// base layer
		::CopyMemory(m_cBTextureName, pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);
		pFileMem += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH) + (sizeof(D3DXMATRIX) * 2) + (sizeof(RwReal)) + (sizeof(RwUInt8));

		// detail layer
		for (int j = 0; j < dNTL_SPLAT_LAYER_NUM; ++j)
		{
			RwBool bBool;

			::CopyMemory(&bBool, pFileMem, sizeof(RwBool));
			pFileMem += (sizeof(RwBool));

			if (bBool) // detail layer 蜡公
			{
				::CopyMemory(m_acDTextureName[j], pFileMem, sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);
				pFileMem += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

				::CopyMemory(&bBool, pFileMem, sizeof(RwBool));
				pFileMem += (sizeof(RwBool));

				if (bBool) // alpha layer 蜡公
				{
					pFileMem += (sizeof(RwChar) * rwTEXTUREBASENAMELENGTH);

					::CopyMemory(m_pbyBuffer, pFileMem, sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);
					pFileMem += (sizeof(BYTE) * dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldSectorTileNum);
					CopyQuadBuffer(m_apbyAlphaBuffer[j], i, m_pbyBuffer);
				}

				pFileMem += (sizeof(D3DXMATRIX) * 2) + (sizeof(RwReal)) + (sizeof(RwUInt8));
			}
		}

		pFileMem = CNtlWorldFileMemAccessor::Skip_Heightfield(pFileMem);
		pFileMem = CNtlWorldFileMemAccessor::Skip_Diffuse(pFileMem);
		pFileMem = CNtlWorldFileMemAccessor::Skip_Doodads(pFileMem);
		pFileMem = CNtlWorldFileMemAccessor::Skip_Water(pFileMem);
		pFileMem = CNtlWorldFileMemAccessor::Skip_Shadow(pFileMem);
		pFileMem = CNtlWorldFileMemAccessor::Skip_Effects(pFileMem);
		pFileMem = CNtlWorldFileMemAccessor::Skip_SE(pFileMem);
		// CZ-SS
		//pFileMem = CNtlWorldFileMemAccessor::Skip_SS(pFileMem);
		pFileMem = CNtlWorldFileMemAccessor::Skip_SectorCull(pFileMem);
		pFileMem = CNtlWorldFileMemAccessor::Skip_TileTransparency(pFileMem);
		pFileMem = CNtlWorldFileMemAccessor::Skip_Shoreline(pFileMem);
		pFileMem = CNtlWorldFileMemAccessor::Skip_Decal(pFileMem);
		pFileMem = CNtlWorldFileMemAccessor::Skip_Plant(pFileMem);
		pFileMem = CNtlWorldFileMemAccessor::Skip_WorldLight(pFileMem);
		pFileMem = CNtlWorldFileMemAccessor::Skip_Occluder(pFileMem);
		pFileMem = CNtlWorldFileMemAccessor::Skip_HeatHazeObject(pFileMem);
		pFileMem = CNtlWorldFileMemAccessor::Skip_LightObject(pFileMem);
		pFileMem = CNtlWorldFileMemAccessor::Skip_Dojo(pFileMem);
	}
	FieldFileMem.Free();

	UpdatePropertyFromTemporary(vPos);
	ClearTemporary();
}

void CNtlWorldPropMaterialController::DeletePropertyPosInWorld(RwV3d& vPos)
{
	CNtlWorldFieldManager*	pFieldMgr	= dGETMGR();
	pFieldMgr->SetAFieldProp(vPos, m_NtlFieldProp, eNFP_MATERIALPROP);
}

RwBool CNtlWorldPropMaterialController::LoadScript(const RwChar* pFileName)
{
	CNtlXMLDoc doc;
	doc.Create();
	if(doc.Load((char*)pFileName) == false)
	{
		return FALSE;
	}
	IXMLDOMNodeList *pList = doc.SelectNodeList((char*)"/object_property/object_element/element");

	long			lNum	= 0;
	IXMLDOMNode*	pNode	= NULL;

	pList->get_length(&lNum);
	for (long i = 0; i < lNum; ++i)
	{
		char cName[rwTEXTUREBASENAMELENGTH];
		char cMaterial[rwTEXTUREBASENAMELENGTH];

		pList->get_item(i, &pNode);
		if(doc.GetTextWithAttributeName(pNode, "name", cName, rwTEXTUREBASENAMELENGTH))
		{
			if (doc.GetTextWithAttributeName(pNode, "material", cMaterial, rwTEXTUREBASENAMELENGTH))
			{
				m_mapMaterial[cName] = ::atoi(cMaterial);
			}
		}

		if (pNode)
		{
			pNode->Release();
			pNode = NULL;
		}
	}

	if (pList)
	{
		pList->Release();
		pList = NULL;
	}
	return TRUE;
}

void CNtlWorldPropMaterialController::AddMaterial(char *pName, BYTE byMaterial)
{
	MAP_MATERIAL_IT it = m_mapMaterial.find(pName);
	if (it == m_mapMaterial.end())
	{
		m_mapMaterial.insert(MAP_MATERIAL::value_type(pName, byMaterial));
	}
}

BYTE CNtlWorldPropMaterialController::GetMaterial(char *pName)
{
	MAP_MATERIAL_IT it = m_mapMaterial.find(pName);
	if (it != m_mapMaterial.end())
	{
		return it->second;
	}
	return 0;
}

void CNtlWorldPropMaterialController::CopyAlphaFromTexture(BYTE* pBuffer, RwTexture* pTexture)
{
	if (!pTexture)
	{
		::ZeroMemory(pBuffer, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum) * (dGET_WORLD_PARAM()->WorldSectorTileNum));
		return;
	}

	RwInt32 RwTexW	= RwRasterGetWidth(pTexture->raster);
	RwInt32 RwTexH	= RwRasterGetWidth(pTexture->raster);
	DWORD*	pBits	= (DWORD*)RwRasterLock(pTexture->raster, 0, rwRASTERLOCKREAD);
	DWORD*	pDest	= NULL;
	DWORD	Temp	= 0;
	RwInt32	iIndex	= 0;

	for (RwInt32 PosY = 0; PosY < RwTexH; ++PosY)
	{
		for (RwInt32 PosX = 0; PosX < RwTexW ; ++PosX)
		{
			pDest = pBits + RwTexW * PosY + PosX;
			Temp = (*pDest);

			pBuffer[RwTexW * PosY + PosX] = static_cast<BYTE>(Temp >> 24);
		}
	}

	RwRasterUnlock(pTexture->raster);
}

void CNtlWorldPropMaterialController::CopyQuadBuffer(BYTE* pQuadBuffer, RwInt32 iQuad, BYTE* pBuffer)
{
	RwInt32 RwTexW	= dGET_WORLD_PARAM()->WorldSectorTileNum;
	RwInt32 RwTexH	= dGET_WORLD_PARAM()->WorldSectorTileNum;
	BYTE*	pBits	= NULL;

	RwInt32 OffsetX = 0;
	RwInt32 OffsetY = 0;
	switch (iQuad)
	{
	case 0:
		OffsetX = 0;
		OffsetY = 0;
		break;
	case 1:
		OffsetX = dGET_WORLD_PARAM()->WorldSectorTileNum;
		OffsetY = 0;
		break;
	case 2:
		OffsetX = 0;
		OffsetY = dGET_WORLD_PARAM()->WorldSectorTileNum;
		break;
	case 3:
		OffsetX = dGET_WORLD_PARAM()->WorldSectorTileNum;
		OffsetY = dGET_WORLD_PARAM()->WorldSectorTileNum;
		break;
	}

	for (RwInt32 PosY = RwTexH - 1; PosY >= 0; --PosY)
	{
		for (RwInt32 PosX = RwTexW - 1; PosX >= 0; --PosX)
		{
			pBits = pBuffer + RwTexW * PosY + PosX;
			pQuadBuffer[((RwTexH - 1) - PosY + OffsetY) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) + ((RwTexW - 1) - PosX + OffsetX)] = *pBits;
		}
	}
}

void CNtlWorldPropMaterialController::ClearTemporary()
{
	for (int i = 0; i < dNTL_SPLAT_LAYER_NUM; ++i)
	{
		m_acDTextureName[i][0] = '\0';
		::ZeroMemory(m_apbyAlphaBuffer[i], sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2));
	}

	m_cBTextureName[0] = '\0';
	//::ZeroMemory(m_pbyBuffer, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum) * (dGET_WORLD_PARAM()->WorldSectorTileNum));
	::ZeroMemory(m_NtlFieldProp._pMaterialProp, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2));
}

void CNtlWorldPropMaterialController::UpdatePropertyFromTemporary(RwV3d& vPos)
{
	CNtlWorldFieldManager*	pFieldMgr	= dGETMGR();
	BYTE*					pResult		= m_NtlFieldProp._pMaterialProp;

	for (int i = 0; i < (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2); ++i)
	{
		BYTE	iSel = 255;
		BYTE	iMax = 0;
		RwInt32 iSum = 0;
		for (int j = dNTL_SPLAT_LAYER_NUM - 1; j >= 0 ; --j)
		{
			if (m_apbyAlphaBuffer[j][i] > iMax)
			{
				iMax = m_apbyAlphaBuffer[j][i];
				iSel = j;
			}
			iSum += m_apbyAlphaBuffer[j][i];
		}

		if (iSum >= dWORLD_PROP_MATERIAL_CONTROLLER_SELECT_MATERIAL)
		{
			pResult[i] = GetMaterial(m_acDTextureName[iSel]);
		}
		else
		{
			pResult[i] = GetMaterial(m_cBTextureName);
		}
	}
	pFieldMgr->SetAFieldProp(vPos, m_NtlFieldProp, eNFP_MATERIALPROP);
}

RwBool CNtlWorldPropMaterialController::SaveImage(const char* pFileName)
{
	RwInt32 iWidth	= dGET_WORLD_PARAM()->WorldFieldNum;
	RwInt32 iHeight = dGET_WORLD_PARAM()->WorldFieldNum;
	RwInt32 iDepth  = 32;

	CNtlWorldFieldManager*	pFieldMgr	= dGETMGR();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFieldMgr->GetFields());

	RwImage*				pImage		= RwImageCreate(iWidth, iHeight, iDepth);
	if (!pImage)
	{
		DBO_TRACE(FALSE, "CNtlWorldPropMaterialController::SaveImage, image create failed.");
		return FALSE;
	}

	RwImageAllocatePixels(pImage);

	CProgressWnd WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "export material tile property.");
	WndProgress.GoModal();
	WndProgress.SetRange(0, dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum);	
	for (int i = 0; i < dGET_WORLD_PARAM()->WorldFieldNum; ++i)
	{
		for (int j = 0; j < dGET_WORLD_PARAM()->WorldFieldNum; ++j)
		{
			RwInt32 iFieldIndex = j + i * dGET_WORLD_PARAM()->WorldFieldNum;
			RwRGBA	rgba;
			rgba.alpha	= 255;
			rgba.red	= 0;
			rgba.green	= 0;
			rgba.blue	= 0;

			RwChar cText[64];
			::sprintf_s(cText, 64, "%d / %d fields is now saving.", iFieldIndex, dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldNum);
			WndProgress.SetText(cText);

			pFieldMgr->GetAFieldProp(pFields[iFieldIndex].GetSPos(), m_NtlFieldProp);
			for (int k = 0; k < (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2); ++k)
			{
				if (m_NtlFieldProp._pMaterialProp[k] > 0)
				{
					rgba.alpha	= 255;
					rgba.red	= 255;
					rgba.green	= 255;
					rgba.blue	= 255;
					break;
				}
			}
			::ZeroMemory(m_NtlFieldProp._pMaterialProp, sizeof(BYTE) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2));


			RwInt32 iPerWidth	= iWidth / dGET_WORLD_PARAM()->WorldFieldNum;
			RwInt32 iPerHeight	= iHeight / dGET_WORLD_PARAM()->WorldFieldNum;
			for (int l = 0; l < iPerHeight; ++l)
			{
				for (int m = 0; m < iPerWidth; ++m)
				{ 
					SetARGB(pImage, iWidth - (j * iPerWidth + m + 1), iHeight - (i * iPerHeight + l + 1), rgba);
				}
			}

			WndProgress.StepIt();
			WndProgress.PeekAndPump();
		}
	}

	RwChar acDir[NTL_MAX_DIR_PATH] = { 0, };
	RwChar acPath[NTL_MAX_DIR_PATH] = { 0, };

	GetCurrentDirectory(NTL_MAX_DIR_PATH, acDir);

	strcpy_s(acPath, pFileName);

	WndProgress.SetText("finalizing images.");
	RwImageWrite(pImage, acPath);

	WndProgress.SetText("Done.");
	RwImageDestroy(pImage);

	SetCurrentDirectory(acDir);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// dNTL_WORLD_FILE : DEFINE
//////////////////////////////////////////////////////////////////////////
#endif