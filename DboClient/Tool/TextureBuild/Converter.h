#pragma once
/*
D3DXIFF_BMP = 0,
D3DXIFF_JPG = 1,
D3DXIFF_TGA = 2,
D3DXIFF_PNG = 3,
D3DXIFF_DDS = 4,
D3DXIFF_PPM = 5,
D3DXIFF_DIB = 6,
D3DXIFF_HDR = 7,
D3DXIFF_PFM = 8,
*/

#include "ConverterDef.h"

class CConverter
{
public:
	CConverter(void);
	virtual ~CConverter(void);

	virtual BOOL Create();
	virtual VOID Destroy();

	virtual BOOL ImageResize(LPCSTR pcFilenameIn, LPCSTR pcFilenameOut, float fDeltaWidth, float fDeltaHeight);
};
