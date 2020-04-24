#include "StdAfx.h"
#include "NtlWorldFieldManager.h"
#include "NtlWE.h"
#include "NtlPLWorldEntity.h"
#include "NtlWEFrm.h"

#include "NtlWorldPropBaseTemplateEx.h"

//////////////////////////////////////////////////////////////////////////
// CNtlWorldPropBaseTemplateEx
//////////////////////////////////////////////////////////////////////////

CNtlWorldPropBaseTemplateEx::CNtlWorldPropBaseTemplateEx(void)
{
}

CNtlWorldPropBaseTemplateEx::~CNtlWorldPropBaseTemplateEx(void)
{
}

void CNtlWorldPropBaseTemplateEx::UpdatePropertyPosInWorld(RwV3d& vPos)
{
	if (IsLoaded(GetIndexPosInWorld(vPos)))
	{
		UpdatePropertyPosInLoadedWorld(vPos);
	}
	else
	{
		UpdatePropertyPosInUnloadedWorld(vPos);
	}
}

void CNtlWorldPropBaseTemplateEx::SetARGB(RwImage* _pImg, RwInt32 x, RwInt32 y, RwRGBA& RGBA)
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

void CNtlWorldPropBaseTemplateEx::GetARGB(RwImage* _pImg, RwInt32 x, RwInt32 y, RwRGBA& RGBA)
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

//////////////////////////////////////////////////////////////////////////
// CNtlWorldPropBaseTemplateExSector
//////////////////////////////////////////////////////////////////////////

CNtlWorldPropBaseTemplateExSector::CNtlWorldPropBaseTemplateExSector()
{
}

CNtlWorldPropBaseTemplateExSector::~CNtlWorldPropBaseTemplateExSector()
{
}

RwInt32 CNtlWorldPropBaseTemplateExSector::GetIndexPosInWorld(RwV3d& vPos)
{
	return dGETMGR()->GetSectorIdx(vPos);
}

RwBool CNtlWorldPropBaseTemplateExSector::IsLoaded(const RwUInt32& iIndex)
{
	return dGETMGR()->IsSectorLoaded(iIndex);
}

//////////////////////////////////////////////////////////////////////////
// CNtlWorldPropBaseTemplateExField
//////////////////////////////////////////////////////////////////////////

CNtlWorldPropBaseTemplateExField::CNtlWorldPropBaseTemplateExField()
{
}

CNtlWorldPropBaseTemplateExField::~CNtlWorldPropBaseTemplateExField()
{
}

RwInt32 CNtlWorldPropBaseTemplateExField::GetIndexPosInWorld(RwV3d& vPos)
{
	return dGETMGR()->GetFieldIdx(vPos);
}

RwBool CNtlWorldPropBaseTemplateExField::IsLoaded(const RwUInt32& iIndex)
{
	return dGETMGR()->IsFieldLoaded(iIndex);
}