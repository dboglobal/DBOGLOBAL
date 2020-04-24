#include "StdAfx.h"
#include "NtlWE.h"
#include "NtlWorldPropNMapController.h"
#include "NtlPLResourceManager.h"
#include "NtlWorldFieldManager.h"
#include "NtlPLWorldEntity.h"
#include "ProgressWnd.h"
#include "NtlWEFrm.h"


CNtlWorldPropNMapController::CNtlWorldPropNMapController(void)
{
}

CNtlWorldPropNMapController::~CNtlWorldPropNMapController(void)
{
}

VOID CNtlWorldPropNMapController::AppendPropDat(RwV3d& PosTile, DWORD _PropDat)
{
	CNtlWorldFieldManager*	pFMgr		= dGETMGR();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFMgr->GetFields());
	RwInt32					IdxField;

	IdxField = pFMgr->GetFieldIdx(PosTile);
	if(IdxField == -1)
	{
		DBO_TRACE(FALSE, "CNtlWorldPropNMapController::AppendPropDat, invalid field index.");
		return;		
	}

	RwV3d	SPos		= pFields[IdxField].GetSPos();
	RwInt32	XMoveCnt	= static_cast<RwInt32>(abs(PosTile.x - SPos.x)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwInt32	ZMoveCnt	= static_cast<RwInt32>(abs(PosTile.z - SPos.z)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwInt32 PosBitFlag	= XMoveCnt + (ZMoveCnt * (dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldFieldSectorNum));

	pFields[IdxField].GetProp()->_pBitProp[PosBitFlag] |= _PropDat;
}

VOID CNtlWorldPropNMapController::DeletePropDat(RwV3d& PosTile, DWORD _PropDat)
{
	CNtlWorldFieldManager*	pFMgr		= dGETMGR();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFMgr->GetFields());
	RwInt32					IdxField;

	IdxField = pFMgr->GetFieldIdx(PosTile);
	if(IdxField == -1)
	{
		DBO_TRACE(FALSE, "CNtlWorldPropNMapController::DeletePropDat, invalid field index.");
		return;
	}

	RwV3d	SPos		= pFields[IdxField].GetSPos();
	RwInt32	XMoveCnt	= static_cast<RwInt32>(abs(PosTile.x - SPos.x)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwInt32	ZMoveCnt	= static_cast<RwInt32>(abs(PosTile.z - SPos.z)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwInt32 PosBitFlag	= XMoveCnt + (ZMoveCnt * (dGET_WORLD_PARAM()->WorldSectorTileNum * dGET_WORLD_PARAM()->WorldFieldSectorNum));

	pFields[IdxField].GetProp()->_pBitProp[PosBitFlag] &= ~_PropDat;
}

VOID CNtlWorldPropNMapController::SavePropImage(RwInt32 Width, RwInt32 Height, RwInt32 Depth, RwInt32 IdxProp, RwChar* pIdxName)
{
	NTL_FUNCTION("CNtlWorldPropNMapController::SavePropImage");

	// create images
	RwImage* pImage = RwImageCreate(Width, Height, Depth);
	if(!pImage)
	{
		DBO_TRACE(FALSE, "CNtlWorldPropNMapController::SavePropImage, image create failed.");
		NTL_RETURNVOID();
	}

	//RwImageSetStride(pImage, 4);
	RwImageAllocatePixels(pImage);

	// update images
	RwV3d	PosCur;
	RwInt32	LenHalfTile		= dGET_WORLD_PARAM()->WorldSectorTileSize / 2;
	RwInt32	LenWorld		= dGET_WORLD_PARAM()->WorldSize;
	RwInt32 NumWorldTile	= dGET_WORLD_PARAM()->WorldFieldNum * dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldSectorTileNum;
	RwInt32 NumWorldField	= dGET_WORLD_PARAM()->WorldFieldNum;
	RwInt32 NumTile4Field	= dGET_WORLD_PARAM()->WorldFieldSectorNum * dGET_WORLD_PARAM()->WorldSectorTileNum;
	RwRGBA	RGBA;
	RwChar	Text[40];
	RwInt32 IdxXTexel;
	RwInt32 IdxZTexel;
	RwInt32 NumTotTileNumPerField = NumTile4Field * NumTile4Field;
	RwV3d	PosFieldStart;
	RwInt32	IdxField;

	// init progress
	CNtlWorldFieldManager*	pFMgr	= dGETMGR();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pFMgr->GetFields());

	CProgressWnd WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "Export normal map tile property.");
	WndProgress.GoModal();
	WndProgress.SetRange(0, NumWorldField * NumWorldField);

	for(RwInt32 i = 0; i < NumWorldField; ++i)
	{
		for(RwInt32 j = 0; j < NumWorldField; ++j)
		{
			IdxField		= j + i * NumWorldField;
			PosFieldStart	= pFields[IdxField].GetSPos();

			// report current status
			::sprintf_s(Text, 40, "%d / %d fields is now saving.", IdxField, NumWorldField * NumWorldField);
			WndProgress.SetText(Text);

			// find world field property file
			_chdir(dGET_WORLD_PARAM()->WorldProjectFolderName);
			_chdir("fields");

			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfif%d", IdxField);
			_chdir(dGET_WORLD_PARAM()->WorldChar64Buf);

			FILE *pFile = NULL;
			sprintf_s(dGET_WORLD_PARAM()->WorldChar64Buf, 64, "wfpf%d", IdxField);
			if(fopen_s(&pFile, dGET_WORLD_PARAM()->WorldChar64Buf, "rb+") != 0)
			{
				DBO_TRACE(FALSE, "file open failed. (" << dGET_WORLD_PARAM()->WorldChar64Buf << ")");
				_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
				return;
			}
			else
			{
				for(RwInt32 ii = 0; ii < NumTile4Field; ++ii)
				{
					for(RwInt32 jj = 0; jj < NumTile4Field; ++jj)
					{
						PosCur.x	= PosFieldStart.x + jj * dGET_WORLD_PARAM()->WorldSectorTileSize;
						PosCur.z	= PosFieldStart.z + ii * dGET_WORLD_PARAM()->WorldSectorTileSize;
						IdxXTexel	= ((RwInt32)PosCur.x + dGET_WORLD_PARAM()->WorldSizeHalf) / dGET_WORLD_PARAM()->WorldSectorTileSize;
						IdxZTexel	= ((RwInt32)PosCur.z + dGET_WORLD_PARAM()->WorldSizeHalf) / dGET_WORLD_PARAM()->WorldSectorTileSize;
						IdxXTexel	= (NumWorldTile - 1) - IdxXTexel;
						IdxZTexel	= (NumWorldTile - 1) - IdxZTexel;
						
						GetPropImageDat(PosCur, RGBA, eIT_NMAP_PROP, IdxProp, pFile);

						SetARGB(pImage, IdxXTexel, IdxZTexel, RGBA);
					}
				}

				fclose(pFile);
				_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
			}

			WndProgress.StepIt();
			WndProgress.PeekAndPump();
		}
	}

	RwChar Dir[NTL_MAX_DIR_PATH] = { 0, };
	RwChar Path[NTL_MAX_DIR_PATH] = { 0, };
	GetCurrentDirectory(NTL_MAX_DIR_PATH, Dir);
	
	// save images
	strcpy_s(Path, "tool\\Saber\\Texture\\NP texture\\");
	strcat_s(Path, pIdxName);

	WndProgress.SetText("finalizing images.");
	RwImageWrite(pImage, Path);

	WndProgress.SetText("Done.");
	RwImageDestroy(pImage);

	SetCurrentDirectory(Dir);

	NTL_RETURNVOID();
}

VOID CNtlWorldPropNMapController::LoadPropImage(RwChar* pFileName, RwInt32 IdxProp)
{
	NTL_FUNCTION("CNtlWorldPropNMapController::LoadPropImage");

	RwImageSetPath("tool/SABER/Texture/NP texture/");

	RwImage* pImg = RwImageRead(pFileName);
	if(!pImg)
	{
		DBO_TRACE(FALSE, "CNtlWorldPropNMapController::LoadPropImage, image read faield.");
		NTL_RETURNVOID();
	}

	SetPropImage(pImg, IdxProp, eIT_NMAP_PROP);

	RwImageDestroy(pImg);

	NTL_RETURNVOID();
}