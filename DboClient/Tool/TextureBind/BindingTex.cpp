#include "StdAfx.h"
#include "BindingTex.h"
#include "FileInfo.h"
#include "TBApp.h"

//////////////////////////////////////////////////////////////////////////
// CBindingTex
//////////////////////////////////////////////////////////////////////////

CBindingTex::CBindingTex(void)
: m_pTexture(NULL)
, m_nIndex(0)
{

}

CBindingTex::~CBindingTex(void)
{
}

void CBindingTex::SetTexture( RwTexture* pTexture, RwInt32 nIndex )
{
	m_pTexture = pTexture;
	m_nIndex = nIndex;
}

void CBindingTex::VertexAssign( CRect rtTex )
{
	RwIm3DVertexSetPos(&m_pIm3dBuffer[0], (RwReal)rtTex.left,  (RwReal)rtTex.top, 0.0f);
	RwIm3DVertexSetNormal(&m_pIm3dBuffer[0], 0.0f, 0.0f, 0.0f );
	RwIm3DVertexSetRGBA(&m_pIm3dBuffer[0], 255, 255, 255, 255);
	RwIm3DVertexSetU(&m_pIm3dBuffer[0], 0.0f);
	RwIm3DVertexSetV(&m_pIm3dBuffer[0], 0.0f);

	RwIm3DVertexSetPos(&m_pIm3dBuffer[1], (RwReal)rtTex.left,  (RwReal)rtTex.bottom, 0.0f);
	RwIm3DVertexSetNormal(&m_pIm3dBuffer[1], 0.0f, 0.0f, 0.0f );
	RwIm3DVertexSetRGBA(&m_pIm3dBuffer[1], 255, 255, 255, 255);
	RwIm3DVertexSetU(&m_pIm3dBuffer[1], 0.0f);
	RwIm3DVertexSetV(&m_pIm3dBuffer[1], 1.0f);

	RwIm3DVertexSetPos(&m_pIm3dBuffer[2], (RwReal)rtTex.right,  (RwReal)rtTex.top, 0.0f);
	RwIm3DVertexSetNormal(&m_pIm3dBuffer[2], 0.0f, 0.0f, 0.0f );
	RwIm3DVertexSetRGBA(&m_pIm3dBuffer[2], 255, 255, 255, 255);
	RwIm3DVertexSetU(&m_pIm3dBuffer[2], 1.0f);
	RwIm3DVertexSetV(&m_pIm3dBuffer[2], 0.0f);

	RwIm3DVertexSetPos(&m_pIm3dBuffer[3], (RwReal)rtTex.right,  (RwReal)rtTex.bottom, 0.0f);
	RwIm3DVertexSetNormal(&m_pIm3dBuffer[3], 0.0f, 0.0f, 0.0f );
	RwIm3DVertexSetRGBA(&m_pIm3dBuffer[3], 255, 255, 255, 255);
	RwIm3DVertexSetU(&m_pIm3dBuffer[3], 1.0f);
	RwIm3DVertexSetV(&m_pIm3dBuffer[3], 1.0f);
}

void CBindingTex::Render()
{
	if( m_pTexture )
	{
		RwD3D9SetTexture(m_pTexture, 0);

		if( RwIm3DTransform(m_pIm3dBuffer, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXUV | rwIM3D_VERTEXRGBA) )
		{                         
			RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);  

			RwIm3DEnd();
		}

		RwD3D9SetTexture( NULL, 0 );
	}
}

/**
* \brief 직교 투영을 할 때 nCenter nWidth, nHeight을 기준으로 렌더가 필요하다면 하고 필요하지 않는다면 ( 직교투영에서 벗어난다면 하지 않는다. )
*/
void CBindingTex::RenderParallel( RwCamera* pRwCamera )
{
	if( m_pTexture )
	{
		// 프러스텀에 걸치는 부분이 조금이라도 있으면 찍는다.
		RwSphere sph[4];
		RwBool bBound = FALSE;
		for( int i=0; i < 4; ++i )
		{
			sph[i].center = m_pIm3dBuffer[i].objVertex;
			sph[i].radius = 0.2f;

			if( rwSPHEREBOUNDARY == RwCameraFrustumTestSphere( pRwCamera, &sph[i] ) )
				bBound = TRUE;

			if( rwSPHEREINSIDE == RwCameraFrustumTestSphere( pRwCamera, &sph[i] ) )
				bBound = TRUE;
		}

		if( !bBound )
			return;

		static int a = 0;

		CHAR acBuffer[256];
		sprintf_s( acBuffer, 256, "%d", a++ );
		OutputDebugString( acBuffer );
		
		RwD3D9SetTexture(m_pTexture, 0);

		if( RwIm3DTransform(m_pIm3dBuffer, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXUV | rwIM3D_VERTEXRGBA) )
		{                         
			RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);  
			RwIm3DEnd();
		}

		RwD3D9SetTexture( NULL, 0 );
	}
}

void CBindingTex::RenderPerspective( RwCamera* pRwCamera )
{
	if( m_pTexture )
	{
		// 프러스텀에 모든 좌표가 들어오지 않는 이상 출력하지 않는 다. ( 속도를 위하여 )
		RwSphere sph[4];
		for( int i=0; i < 4; ++i )
		{
			sph[i].center = m_pIm3dBuffer[i].objVertex;
			sph[i].radius = 0.1f;

			if( rwSPHEREOUTSIDE == RwCameraFrustumTestSphere( pRwCamera, &sph[i] ) )
				return;
		}

		RwD3D9SetTexture(m_pTexture, 0);

		if( RwIm3DTransform(m_pIm3dBuffer, 4, NULL, rwIM3D_VERTEXXYZ | rwIM3D_VERTEXUV | rwIM3D_VERTEXRGBA) )
		{                         
			RwIm3DRenderPrimitive(rwPRIMTYPETRISTRIP);  

			RwIm3DEnd();
		}

		RwD3D9SetTexture( NULL, 0 );
	}
}

RwInt32 CBindingTex::GetWidth()
{
	if( m_pTexture )
	{
		return m_pTexture->raster->width;
	}

	return 0;
}

RwInt32 CBindingTex::GetHeight()
{
	if( m_pTexture )
	{
		return m_pTexture->raster->height;
	}

	return 0;
}

CRect CBindingTex::GetRect()
{
	return CRect(
		(int)m_pIm3dBuffer[0].objVertex.x,
		(int)m_pIm3dBuffer[0].objVertex.y,
		(int)m_pIm3dBuffer[3].objVertex.x,
		(int)m_pIm3dBuffer[3].objVertex.y );
}


//////////////////////////////////////////////////////////////////////////
// CBindingTexManager
//////////////////////////////////////////////////////////////////////////

CBindingTexManager::CBindingTexManager( void )
: m_pBindingTex(NULL)
, m_nRows(0)
, m_nColumns(0)
, m_nWidth(0)
, m_nHeight(0)
, m_nCount(0)
{

}

CBindingTexManager::~CBindingTexManager( void )
{
	Destroy();
}

BOOL CBindingTexManager::LoadTexFromPath( eType type, eSize size, const CString strPath )
{
	Destroy();

	int nType = GetTypeInt( type );
	int nSize = GetSizeInt( size );

	CFileInfoArray fia;

	// 디렉토리에서 파일을 읽어오면서 정렬을 한다.
	int nCount = fia.AddDir(strPath + CString("\\"),			// Path
		CString("*.bmp"),	// Mask
		FALSE,
		CFileInfoArray::AP_SORTBYNAME | CFileInfoArray::AP_SORTASCENDING,
		FALSE );

	if( fia.GetSize() == 0 )
	{
		// 패스가 잘못되었거나 bmp 파일들이 하나도 없다.
		return FALSE;
	}

	m_nCount = nType * nType;
	m_nWidth = nSize * nType;
	m_nHeight = nSize * nType;
	m_nColumns = nType;
	m_nRows = nType;

	// Tex의 가로 갯수와 세로 갯수 체크
	m_pBindingTex = new CBindingTex[m_nCount];

	int i = 0;
	int nFileCount = 0;
	int ixCol = 0, ixRow = 0;
	int nX = 0, nY = 0;
	char acFileBuffer[MAX_PATH];
	char acPathBuffer[MAX_PATH];
	while( i < m_nCount )

	{
		// Index와 비교해서 현재 순서와 맞지 않는 인덱스라면 로드거나 버텍스를 정렬하지 않는다.
		CString strFilename = fia[nFileCount].GetFileTitle();

		RwInt32 nIndex = atoi(strFilename.GetBuffer());

		// 버텍스의 위치 초기화
		m_pBindingTex[i].VertexAssign( 
			CRect( nX+nSize,
			nY+nSize,
			nX, 
			nY ) );

		// 넓이 진행
		if (ixCol < m_nColumns)
		{
			nX += nSize;
			ixCol++;
		}

		// 높이 진행
		if (ixCol == m_nColumns) 
		{
			nY += nSize;
			ixCol = 0;
			nX = 0;
		}

		if( nIndex != i )
		{
			i++;
			continue;	
		}
		
		// 인덱스가 일치하다면 텍스쳐 로딩
		sprintf_s( acFileBuffer, MAX_PATH, _T("%s"), fia[nFileCount].GetFileName().GetBuffer() );
		sprintf_s( acPathBuffer, MAX_PATH, _T("%s"), fia[nFileCount].GetFileRoot().GetBuffer() );

		// Set Path
		RwImageSetPath( acPathBuffer );

		RwChar* pChar = RwImageGetPath();

		RwTexture* pTexture = RwTextureRead( acFileBuffer, NULL );

		if( pTexture == NULL )
		{
			delete[] m_pBindingTex;
			m_pBindingTex = NULL;
			return FALSE;
		}
		
		m_pBindingTex[i].SetTexture( pTexture, i );

		nFileCount++;
		if( nFileCount >= fia.GetSize() )
			return TRUE;

		i++;
	}

	return TRUE;
}

void CBindingTexManager::Create()
{

}

void CBindingTexManager::Destroy()
{
	if( m_pBindingTex )
	{
		for(int i=0; i < m_nCount; ++i )
		{
			RwTexture* pTexture = m_pBindingTex[i].GetTexture();
			if( pTexture )
			{
				RwTextureDestroy( pTexture );
				pTexture = NULL;
			}
		}
		delete[] m_pBindingTex;
		m_pBindingTex = NULL;
	}

	m_nRows = 0;
	m_nColumns = 0;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nCount = 0;
}

void CBindingTexManager::Render()
{
	// 과부하, 컬링을 할까?
	if( m_nWidth >= 1024*16 ||
		m_nHeight >= 1024*16 )
		return;

	if( m_pBindingTex )
	{
		for(int i=0; i<m_nCount; ++i )
		{
			m_pBindingTex[i].Render();
		}
	}
}

void CBindingTexManager::RenderPerspective( RwCamera* pCamera )
{
	if( m_pBindingTex )
	{
		for(int i=0; i<m_nCount; ++i )
		{
			m_pBindingTex[i].RenderPerspective( pCamera );
		}
	}
}

BOOL CBindingTexManager::SaveTexCurrentData( eSize size, RwCamera* pRwCamera, const CString strPath )
{
	if( !m_pBindingTex )
		return FALSE;

	int nSize = GetSizeInt( size );

	if( m_nWidth <= nSize )
		nSize = m_nWidth;

	int nSaveCol = m_nWidth / nSize;
	int nSaveRow = nSaveCol;
	int nResultTexCount = nSaveCol * nSaveRow;

	// MiniMap Color Foramt
	RwRGBA backColor;
	backColor.red = 98;
	backColor.green = 94;
	backColor.blue = 89;
	backColor.alpha = 255;

	// 이전 카메라 저장
	
	RwCamera rwCamera;
	memcpy( &rwCamera, pRwCamera, sizeof(RwCamera));

	
	//RwCameraView
	RwCameraSetProjection(pRwCamera, rwPARALLEL);

	RwV2d v2dSize;
	v2dSize.x = (RwReal)nSize/2;
	v2dSize.y = (RwReal)nSize/2;
	
	RwCameraSetViewWindow( pRwCamera, &v2dSize );

	// Old Raster
	/*RwRaster*	pMiniMapRaster	= RwRasterCreate(nSize, nSize, 32, rwRASTERTYPECAMERATEXTURE);*/
	RwRaster*	pMiniMapRaster	= RwRasterCreate(nSize, nSize, 32, rwRASTERTYPECAMERATEXTURE);
	RwRaster*	pMiniMapRasterZ	= RwRasterCreate(nSize, nSize, 32, rwRASTERTYPEZBUFFER);
	RwRaster*	pSrcRaster		= RwCameraGetRaster(pRwCamera);
	RwRaster*	pSrcRasterZ		= RwCameraGetZRaster(pRwCamera);

	RwCameraSetRaster(pRwCamera, pMiniMapRaster);
	RwCameraSetZRaster(pRwCamera, pMiniMapRasterZ);

	// 카메라를 옮겨가며 찍는다.
	int i=0;
	int nCol = 0, nRow = 0;
	int nX = (nSize/2), nY = (nSize/2);

	RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(pRwCamera)))->x = (RwReal)nSize/2;
	RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(pRwCamera)))->y = (RwReal)nSize/2;
	RwMatrixUpdate(RwFrameGetMatrix(RwCameraGetFrame(pRwCamera)));
	RwFrameUpdateObjects(RwCameraGetFrame(pRwCamera));

	while( i < nResultTexCount )
	{
		if (RwCameraClear(pRwCamera, &backColor , rwCAMERACLEARZ | rwCAMERACLEARIMAGE))
		{
			// 일단 한번에 다 찍고
			if(RwCameraBeginUpdate(pRwCamera))
			{	
				for(int i=0; i<m_nCount; ++i )
				{
					// 필요가 없으면 찍지 않는다.
					/*m_pBindingTex[i].RenderParallel( pRwCamera );*/
					m_pBindingTex[i].Render();
				}

				RwCameraEndUpdate(pRwCamera);
			}
		}

		RwChar Buffer[256];
		sprintf_s(Buffer, 256, "%s\\%.4d.bmp", strPath, i);

		// 한번 직고
		RwImage *image;
		RwInt32 x, y;

		if( RwCameraGetRaster(pRwCamera) )
		{
			x = RwRasterGetWidth(RwCameraGetRaster(pRwCamera));
			y = RwRasterGetHeight(RwCameraGetRaster(pRwCamera));
			image = RwImageCreate(x, y, 32);
			if( image )
			{
				RwImageAllocatePixels(image);
				RwImageSetFromRaster(image, const_cast<RwRaster *>(RwCameraGetRaster(pRwCamera)));
				RwImageWrite(image, Buffer);
				RwImageDestroy(image);
			}
		}

		// x, y 업데이트
		if( nCol < nSaveCol )
		{
			// x, y 업데이트
			RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(pRwCamera)))->x += nSize;
			nCol++;
		}

		if( nCol == nSaveCol )
		{
			RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(pRwCamera)))->y += nSize;
			nCol = 0;
			RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(pRwCamera)))->x = (RwReal)(nSize/2);;
		}

		RwMatrixUpdate(RwFrameGetMatrix(RwCameraGetFrame(pRwCamera)));
		RwFrameUpdateObjects(RwCameraGetFrame(pRwCamera));


		i++;
	}

	RwRasterDestroy(pMiniMapRaster);
	RwRasterDestroy(pMiniMapRasterZ);

	memcpy( pRwCamera, &rwCamera, sizeof(RwCamera));

	//// 복구
	RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(pRwCamera)))->x = (RwReal)(m_nWidth/2);
	RwMatrixGetPos(RwFrameGetMatrix(RwCameraGetFrame(pRwCamera)))->y = (RwReal)(m_nHeight/2);
	RwMatrixUpdate(RwFrameGetMatrix(RwCameraGetFrame(pRwCamera)));
	RwFrameUpdateObjects(RwCameraGetFrame(pRwCamera));

	return TRUE;
}

RwInt32 CBindingTexManager::GetTileSize()
{
	if( m_nWidth == 0 ||
		m_nColumns == 0 )
		return 0;

	return m_nWidth / m_nColumns;
}

RwInt32 CBindingTexManager::GetTileType()
{
	return m_nColumns;
}

RwInt32 CBindingTexManager::GetTypeInt( eType type )
{
	switch( type )
	{
	case TYPE_BY_1:
		return 1;
	case TYPE_BY_2:
		return 2;
	case TYPE_BY_4:
		return 4;
	case TYPE_BY_8:
		return 8;
	case TYPE_BY_16:
		return 16;
	case TYPE_BY_32:
		return 32;
	case TYPE_BY_64:
		return 64;
	default:
		{
			ASSERT(0 && "CBindingTexManager::GetSizeInt - out of type");

			return 1;
		}
	}
}

RwInt32 CBindingTexManager::GetSizeInt( eSize size )
{
	switch( size )
	{
	case SIZE_BY_128:
		return 128;
	case SIZE_BY_256:
		return 256;
	case SIZE_BY_512:
		return 512;
	case SIZE_BY_1024:
		return 1024;
	case SIZE_BY_2048:
		return 2048;
	case SIZE_BY_4096:
		return 4096;
	default:
		{
			ASSERT(0 && "CBindingTexManager::GetSizeInt - out of size");
		return 128;
		}
	}
}

