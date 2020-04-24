#include "gui_precomp.h"
#include "gui_texture.h"

START_GUI


CTexture::CTexture()
{
	m_pTexture = NULL;
}

CTexture::~CTexture()
{
}

bool CTexture::Load(const char *pFileName)
{
	m_pTexture = RwTextureRead(pFileName, NULL);//pFileName);

	if (m_pTexture == NULL)
	{
		return false;
	}

	RwTextureSetFilterMode(m_pTexture, rwFILTERMIPLINEAR); 
	RwTextureSetAddressingU(m_pTexture, rwTEXTUREADDRESSCLAMP);
	RwTextureSetAddressingV(m_pTexture, rwTEXTUREADDRESSCLAMP);

	return true;
}

bool CTexture::Load(RwTexture *pRwTexture)
{
	m_pTexture = pRwTexture;
	
	RwTextureSetFilterMode(m_pTexture, rwFILTERMIPLINEAR); 
	RwTextureSetAddressingU(m_pTexture, rwTEXTUREADDRESSCLAMP);
	RwTextureSetAddressingV(m_pTexture, rwTEXTUREADDRESSCLAMP);

	return true;
}

VOID CTexture::Unload(VOID)
{
	if( m_pTexture == NULL )
		return;

	BOOL bDestroy = FALSE;

	if( m_pTexture->refCount == 1 )
		bDestroy = TRUE;

	RwTextureDestroy(m_pTexture);
	
	if(bDestroy)	
	{
		m_pTexture = NULL;
	}
}

const char* CTexture::GetName(VOID)
{
	return (m_pTexture) ? RwTextureGetNameMacro(m_pTexture) : NULL;
}

int CTexture::GetWidth(VOID)
{
	if(m_pTexture)
		return RwRasterGetWidth(m_pTexture->raster);

	return 0;
}

int	CTexture::GetHeight(VOID)
{
	if(m_pTexture)
		return RwRasterGetHeight(m_pTexture->raster);

	return 0;
}

int CTexture::GetStride(VOID)
{
	if(m_pTexture)
		return RwRasterGetStride(m_pTexture->raster);

	return 0;
}

int CTexture::GetFormat(VOID)
{
	if(m_pTexture)
		return RwRasterGetFormat(m_pTexture->raster);

	return 0;
}

bool CTexture::IsMaskColor(int nX, int nY, unsigned int dwMaskColor /*= 0*/)
{
	if(m_pTexture == NULL)
		return false;

	int nTWidth = GetWidth();
	int nTHeight = GetHeight();

	if(nX < 0 || nX >= nTWidth || nY < 0 || nY >= nTHeight)
		return false;

	bool bSuccess = false;

	unsigned char *pBits = LockWrite(); 

	if( !pBits )
	{
		DBO_ASSERT( pBits, "pBits is Null" );
		return false;
	}	

	int nStride = GetStride();
	int nFormat = GetFormat();

	unsigned int dwColor = 0;

	unsigned char buMaskAlpha = (dwMaskColor >> 25) & 0x000000ff;
	unsigned char buMaskRed = (dwMaskColor >> 16) & 0x000000ff;
	unsigned char buMaskGreen = (dwMaskColor >> 8) & 0x000000ff;
	unsigned char buMaskBlue = (dwMaskColor ) & 0x000000ff;

	// 16 bit
	if(nFormat == rwRASTERFORMAT1555)
	{
		WORD wColor = *((WORD*)(pBits + nStride * nY + nX*2));
		WORD wMaskColor = ((buMaskAlpha >> 7) << 15) || ((buMaskRed >> 3) << 10) || ((buMaskGreen >> 3) << 5) || (buMaskBlue >> 3);
		if(wColor == wMaskColor)
			bSuccess = true;
	}
	// 16 bit
	else if(nFormat == rwRASTERFORMAT555)
	{
		WORD wColor = *((WORD*)(pBits + nStride * nY + nX*2));
		WORD wMaskColor = ((buMaskRed >> 3) << 10) || ((buMaskGreen >> 3) << 5) || (buMaskBlue >> 3);
		if(wColor == wMaskColor)
			bSuccess = true;
	}
	// 16 bit
	else if(nFormat == rwRASTERFORMAT565)
	{
		WORD wColor = *((WORD*)(pBits + nStride * nY + nX*2));
		WORD wMaskColor = ((buMaskRed >> 3) << 11) || ((buMaskGreen >> 2) << 6) || (buMaskBlue >> 3);
		if(wColor == wMaskColor)
			bSuccess = true;
	}
	// 16 bit
	else if(nFormat == rwRASTERFORMAT4444)
	{
		WORD wColor = *((WORD*)(pBits + nStride * nY + nX*2));
		WORD wMaskColor = ((buMaskAlpha >> 4) << 12) || ((buMaskRed >> 4) << 8) || ((buMaskGreen >> 4) << 4) || (buMaskBlue >> 4);
		if(wColor == wMaskColor)
			bSuccess = true;
	}
	// 24 bit
	else if(nFormat == rwRASTERFORMAT888)
	{
		unsigned char byRed = *(pBits + nStride * nY + nX*3);
		unsigned char byGreen = *(pBits + nStride * nY + nX*3+1);
		unsigned char byBlue = *(pBits + nStride * nY + nX*3+2);

		if(byRed == buMaskRed && byGreen == buMaskGreen && byBlue == buMaskBlue)
			bSuccess = true;
	}
	// 32 bit
	else if(nFormat == rwRASTERFORMAT8888) 
	{
		dwColor = *((DWORD*)(pBits + nStride * nY + nX*4));
		if(dwColor == dwMaskColor)
			bSuccess = true;
	}
		
	UnLock();

	return bSuccess;
}

unsigned char* CTexture::LockWrite(VOID)
{
	if(m_pTexture == NULL)
		return NULL;

	return (unsigned char*)RwRasterLock(m_pTexture->raster, 0, rwRASTERLOCKWRITE); 
}

VOID CTexture::UnLock(VOID)
{
	if(m_pTexture == NULL)
		return;

	RwRasterUnlock(m_pTexture->raster); 
}


VOID CTexture::FillClear(VOID)
{
	if(m_pTexture == NULL)
		return;

	int nTWidth = GetWidth();
	int nTHeight = GetHeight();

	unsigned char *pBits = LockWrite(); 
	int nStride = GetStride();

	if( !pBits )
	{
		DBO_ASSERT( pBits, "pBits is Null" );
		return;
	}	

#ifdef FONT_USE_32BIT
	DWORD *pDest32;

	for (int i = 0; i < nTHeight; ++i)
	{
		pDest32 = (DWORD*)(pBits + nStride*i);

		for (int j = 0; j < nTWidth; ++j)
		{
			*pDest32 = (DWORD)0;
			++pDest32;
		}
	}
#else
	WORD *pDest16;

	for (int i = 0; i < nTHeight; ++i)
	{
		pDest16 = (WORD*)(pBits + nStride*i);

		for (int j = 0; j < nTWidth; ++j)
		{
			*pDest16 = (WORD)0;
			++pDest16;
		}
	}
#endif

	UnLock();
}


RwTexture* CTexture::GetTexture(VOID)
{
	return m_pTexture;
}


bool CTexture::IsValid(VOID)
{
	return (m_pTexture) ? true : false;
}

END_GUI
