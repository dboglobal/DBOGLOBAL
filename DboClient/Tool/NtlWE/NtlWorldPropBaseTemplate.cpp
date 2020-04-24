#include "StdAfx.h"
#include "NtlWorldPropBaseTemplate.h"
#include "NtlWorldFieldManager.h"
#include "NtlWE.h"
#include "NtlPLWorldEntity.h"
#include "ProgressWnd.h"
#include "NtlWEFrm.h"


CNtlWorldPropBaseTemplate::CNtlWorldPropBaseTemplate(void)
{
}

CNtlWorldPropBaseTemplate::~CNtlWorldPropBaseTemplate(void)
{
}

DWORD CNtlWorldPropBaseTemplate::GetHexCode(RwInt32 _Idx)
{
	switch(_Idx)
	{
	case 0:		return 0x00000001;
	case 1:		return 0x00000002;
	case 2:		return 0x00000004;
	case 3:		return 0x00000008;
	case 4:		return 0x00000010;
	case 5:		return 0x00000020;
	case 6:		return 0x00000040;
	case 7:		return 0x00000080;
	case 8:		return 0x00000100;
	case 9:		return 0x00000200;
	case 10:	return 0x00000400;
	case 11:	return 0x00000800;
	case 12:	return 0x00001000;
	case 13:	return 0x00002000;
	case 14:	return 0x00004000;
	case 15:	return 0x00008000;
	}

	return 0x00000000;
}

VOID CNtlWorldPropBaseTemplate::GetPropImageDat(RwV3d& _PosCur, RwRGBA& _RGBA, eIMAGE_TYPE _eIT, RwInt32 _Idx, FILE* pFile)
{
	CNtlWorldFieldManager*	pFMgr		= dGETMGR();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFMgr->GetFields());
	RwInt32					IdxField;

	IdxField = pFMgr->GetFieldIdx(_PosCur);

	if(IdxField == -1)
	{
		return;
	}

	RwV3d SPos = pFields[IdxField].GetSPos();

	DWORD	DWord;
	RwInt32	XMoveCnt	= static_cast<RwInt32>(abs(_PosCur.x - SPos.x)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwInt32	ZMoveCnt	= static_cast<RwInt32>(abs(_PosCur.z - SPos.z)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwInt32 PosFileSize	= (XMoveCnt * sizeof(DWORD)) + (ZMoveCnt * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * sizeof(DWORD));

	switch(_eIT)
	{
	case eIT_NMAP_PROP:
		{
			fseek(pFile, 0, SEEK_SET);
			fseek(pFile, 64 + PosFileSize, SEEK_CUR);
			fread(&DWord, sizeof(DWORD), 1, pFile);

			if(DWord & GetHexCode(_Idx))
			{
				_RGBA.alpha = 255;
				_RGBA.red	= 255;
				_RGBA.green = 255;
				_RGBA.blue	= 255;
			}
			else
			{
				_RGBA.alpha = 255;
				_RGBA.red	= 0;
				_RGBA.green = 0;
				_RGBA.blue	= 0;
			}
		}
		break;	

	case eIT_SMAP_PROP:
		{
		}
		break;

	case eIT_BGM_PROP:
		{
		}
		break;
	}
}

VOID CNtlWorldPropBaseTemplate::SetPropImageDat(RwV3d& _PosCur, RwRGBA& _RGBA, eIMAGE_TYPE _eIT, RwInt32 _Idx, FILE* pFile)
{
	CNtlWorldFieldManager*	pFMgr		= dGETMGR();
	CNtlWorldField*			pFields		= const_cast<CNtlWorldField*>(pFMgr->GetFields());
	RwInt32					IdxField;

	IdxField = pFMgr->GetFieldIdx(_PosCur);
	
	if(IdxField == -1)
	{
		return;
	}
	
	RwV3d SPos = pFields[IdxField].GetSPos();

	DWORD	DWord;
	RwInt32	XMoveCnt	= static_cast<RwInt32>(abs(_PosCur.x - SPos.x)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwInt32	ZMoveCnt	= static_cast<RwInt32>(abs(_PosCur.z - SPos.z)) / dGET_WORLD_PARAM()->WorldSectorTileSize;
	RwInt32 PosFileSize	= (XMoveCnt * sizeof(DWORD)) + (ZMoveCnt * (dGET_WORLD_PARAM()->WorldSectorTileNum * 2) * sizeof(DWORD));

	switch(_eIT)
	{
	case eIT_NMAP_PROP:
		{
			fseek(pFile, 0, SEEK_SET);
			fseek(pFile, 64 + PosFileSize, SEEK_CUR);	
			fread(&DWord, sizeof(DWORD), 1, pFile);

			if(!_RGBA.red && !_RGBA.green && !_RGBA.blue)
			{
				DWord &= ~GetHexCode(_Idx);
			}
			else
			{
				DWord |= GetHexCode(_Idx);
			}

			fseek(pFile, 0, SEEK_SET);
			fseek(pFile, 64 + PosFileSize, SEEK_CUR);
			fwrite(&DWord, sizeof(DWORD), 1, pFile);
		}
		break;	

	case eIT_SMAP_PROP:
		{
		}
		break;

	case eIT_BGM_PROP:
		{
		}
		break;
	}
}

VOID CNtlWorldPropBaseTemplate::SetARGB(RwImage* _pImg, RwInt32 x, RwInt32 y, RwRGBA& RGBA)
{
	RwInt32 RwImgW	= RwImageGetWidth(_pImg);
	RwInt32 RwImgH	= RwImageGetHeight(_pImg);

	NTL_ASSERTE((x < RwImgW) && (x >= 0));
	NTL_ASSERTE((y < RwImgH) && (y >= 0));

	DWORD*	pDest;
	DWORD*	pBits	= (DWORD*)RwImageGetPixels(_pImg);

	pDest		= pBits + (RwImgW * y) + x;
	(*pDest)	= (DWORD)(RGBA.alpha << 24) | (DWORD)(RGBA.blue << 16) | (DWORD)(RGBA.green << 8) | (DWORD)(RGBA.red << 0);
}

VOID CNtlWorldPropBaseTemplate::GetARGB(RwImage* _pImg, RwInt32 x, RwInt32 y, RwRGBA& RGBA)
{
	RwInt32 RwImgW	= RwImageGetWidth(_pImg);
	RwInt32 RwImgH	= RwImageGetHeight(_pImg);

	NTL_ASSERTE((x < RwImgW) && (x >= 0));
	NTL_ASSERTE((y < RwImgH) && (y >= 0));

	// Pixel format for RwImage:
	// Red		: bits 00 - 07
	// Green	: bits 08 - 15
	// Blue		: bits 16 - 23
	// Alpha	: bits 24 - 31

	DWORD*	pDest;
	DWORD	Texel;
	DWORD*	pBits = (DWORD*)RwImageGetPixels(_pImg);

	pDest		= pBits + (RwImgW * y) + x;
	Texel		= (*pDest);
	RGBA.alpha	= (RwUInt8)((Texel & 0xff000000) >> 24);
	RGBA.blue	= (RwUInt8)((Texel & 0x00ff0000) >> 16);
	RGBA.green	= (RwUInt8)((Texel & 0x0000ff00) >> 8);
	RGBA.red	= (RwUInt8)((Texel & 0x000000ff) >> 0);
}

VOID CNtlWorldPropBaseTemplate::SetPropImage(RwImage* _pImg, RwInt32 _IdxData, eIMAGE_TYPE _eIT)
{
	NTL_FUNCTION("CNtlWorldPropBaseTemplate::SetPropImage");


	CNtlWorldFieldManager*	pFMgr	= dGETMGR();
	CNtlWorldField*			pFields	= const_cast<CNtlWorldField*>(pFMgr->GetFields());

	switch(_eIT)
	{
	case eIT_NMAP_PROP:
		{
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
			CProgressWnd WndProgress((CNtlWEFrm*)AfxGetMainWnd(), "Save normal map tile property.");
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
								GetARGB(_pImg, IdxXTexel, IdxZTexel, RGBA);

								SetPropImageDat(PosCur, RGBA, eIT_NMAP_PROP, _IdxData, pFile);
							}
						}

						fclose(pFile);
						_chdir(dGET_WORLD_PARAM()->CurWorkingFolderName);
					}

					WndProgress.StepIt();
					WndProgress.PeekAndPump();
				}
			}

			WndProgress.SetText("Done.");
		}
		break;	

	case eIT_SMAP_PROP:
		{
			
		}
		break;

	case eIT_BGM_PROP:
		{
			
		}
		break;
	}


	NTL_RETURNVOID();
}