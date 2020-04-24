#include "StdAfx.h"
#include "ConverterDirectX.h"

CConverterDirectX::CConverterDirectX(void)
:m_pD3D(NULL)
,m_pD3DDevice(NULL)
{
}

CConverterDirectX::~CConverterDirectX(void)
{
}

BOOL CConverterDirectX::Create()
{	
	if (m_pD3D = Direct3DCreate9(D3D_SDK_VERSION))
	{
		if (SUCCEEDED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_D3dDisplayMode)))
		{
			D3DPRESENT_PARAMETERS d3dPresentParameters;
			ZeroMemory(&d3dPresentParameters, sizeof(D3DPRESENT_PARAMETERS));

			d3dPresentParameters.Windowed			= TRUE;
			d3dPresentParameters.SwapEffect			= D3DSWAPEFFECT_DISCARD;
			d3dPresentParameters.BackBufferFormat	= m_D3dDisplayMode.Format;

			if (SUCCEEDED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, GetDesktopWindow(), D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dPresentParameters, &m_pD3DDevice)))
			{
				return TRUE;
			}
		}
		RELEASE(m_pD3D);
	}

	return FALSE;
}

VOID CConverterDirectX::Destroy()
{
	RELEASE(m_pD3DDevice);
	RELEASE(m_pD3D);
}

BOOL CConverterDirectX::ImageResize(LPCSTR pcFilenameIn, LPCSTR pcFilenameOut, float fDeltaWidth, float fDeltaHeight)
{
	//SPLIT_PATH				splitPathIn(pcFilenameIn);
	//SPLIT_PATH				splitPathOut(pcFilenameOut);	

	BOOL			bResult = FALSE;
	D3DXIMAGE_INFO	d3dImageInfoIn;
	if (SUCCEEDED(D3DXGetImageInfoFromFileA(pcFilenameIn, &d3dImageInfoIn)))
	{
		LPDIRECT3DTEXTURE9 pD3dTextureIn	= NULL;
		LPDIRECT3DTEXTURE9 pD3dTextureOut	= NULL;

		if (SUCCEEDED(D3DXCreateTextureFromFileA(m_pD3DDevice, pcFilenameIn, &pD3dTextureIn)))
		{
			UINT uiResizeWidth	= static_cast<UINT>(static_cast<float>(d3dImageInfoIn.Width) * fDeltaWidth);
			UINT uiResizeHeight = static_cast<UINT>(static_cast<float>(d3dImageInfoIn.Height) * fDeltaHeight);

			if (SUCCEEDED(D3DXCreateTexture(m_pD3DDevice, uiResizeWidth, uiResizeHeight, d3dImageInfoIn.MipLevels, 0, d3dImageInfoIn.Format, D3DPOOL_MANAGED, &pD3dTextureOut)))
			{
				LPDIRECT3DSURFACE9 pD3dSurfaceIn	= NULL;
				LPDIRECT3DSURFACE9 pD3dSurfaceOut	= NULL;

				DWORD dwResult = 0;
				for (int i = 0; i < (int)pD3dTextureOut->GetLevelCount(); ++i)
				{
					if (SUCCEEDED(pD3dTextureIn->GetSurfaceLevel(i, &pD3dSurfaceIn)))
					{
						if (SUCCEEDED(pD3dTextureOut->GetSurfaceLevel(i, &pD3dSurfaceOut)))
						{
							if (SUCCEEDED(D3DXLoadSurfaceFromSurface(pD3dSurfaceOut, NULL, NULL, pD3dSurfaceIn, NULL, NULL, D3DX_FILTER_LINEAR, NULL)))
							{							
								++dwResult;
							}
 							RELEASE(pD3dSurfaceOut);
 						}
 						RELEASE(pD3dSurfaceIn);
 					}
 				}
 
 				if (dwResult == pD3dTextureOut->GetLevelCount())
 				{					
 					if (SUCCEEDED(D3DXSaveTextureToFileA(pcFilenameOut, d3dImageInfoIn.ImageFileFormat, pD3dTextureOut, NULL)))
 					{
 						bResult = TRUE;
 					}
 				}

				RELEASE(pD3dTextureOut);
				
			}			
			RELEASE(pD3dTextureIn);
		}		
	}
	return bResult;
}