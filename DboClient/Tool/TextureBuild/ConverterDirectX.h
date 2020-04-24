#pragma once

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#include "converter.h"
#include <d3d9.h>
#include <d3dx9.h>

class CConverterDirectX : public CConverter
{
public:
	CConverterDirectX(void);
	virtual ~CConverterDirectX(void);

	virtual BOOL	Create();
	virtual VOID	Destroy();

	virtual BOOL	ImageResize(LPCSTR pcFilenameIn, LPCSTR pcFilenameOut, float fDeltaWidth, float fDeltaHeight);

private:
	LPDIRECT3D9				m_pD3D;
	LPDIRECT3DDEVICE9		m_pD3DDevice;
	D3DDISPLAYMODE			m_D3dDisplayMode;	
};
