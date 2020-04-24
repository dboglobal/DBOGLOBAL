//-----------------------------------------------------------------------------
// Name: class CDevIL
// Desc: DevIL SDK를 이용하여 이미지를 로드하는 클래스.
//
//			 
// 2004.08	  KlayMan@gs
// 2006.01.16 Peessi@hitel.net  정리 및 모듈화
//-----------------------------------------------------------------------------

#include "stdafx.h"					// for Windows MFC
#include "CDevIL.h"

CDevIL::CDevIL()
{
	ClearMember();
	Init();
}

CDevIL::~CDevIL()
{
	Shutdown();
}

void CDevIL::ClearMember()
{
	m_imageIdx	= 0;
	m_nWidth	= 0;  
	m_nHeight	= 0; 
	m_nBytes	= 0;
	m_nBits		= 0;  
	m_nFormat	= 0; 
	m_nType		= 0;   
	m_nFSet		= 0;
	m_nFMode	= 0;
	m_nTSet		= 0;
	m_nTMode	= 0;
	m_nOrgMode	= 0;

	m_pPic		= NULL;
}


void CDevIL::Init()
{
	ilInit();
	iluInit();
	ilutRenderer( ILUT_WIN32 );
}

void CDevIL::Shutdown()
{
	Unload();		// 로드되어 있는걸 우선 지운다.
	ilShutDown();	
}

BOOL CDevIL::Load( LPCTSTR szFilename )
{
	// IL0. 기존 Image를 파괴한다. 
	Unload();


	// IL1. 1개의 Image를 'imageIdx'라는 이름(숫자)으로 만든다.
	ilGenImages( 1, &m_imageIdx );


	// IL2. 'imageIdx'라는 이름을 현재 다루고 있는 Image로 정한다.
	ilBindImage( m_imageIdx );


	// IL2-1. 그림 파일포멧마다 픽셀이 상하로 진행되는 방향이 다르므로,
	//      무조건 '위에서 아래' 방향이 되도록 '강제로' 정한다.
	ilEnable( IL_ORIGIN_SET );
	ilOriginFunc( IL_ORIGIN_UPPER_LEFT );


	// IL3. 그림을 읽어서 현재 Image에 넣는다.
	//      파일형식은 알아서 인식한다.
	if( ilLoadImage( (char*)szFilename ) == IL_FALSE ) 
	{
		Unload();
		return FALSE;
	}

	// IL3-1. 파일형식을 맞춰서 읽고 싶다면 ilLoad()를 이용한다.
	//ilLoad( fileType, szFileName );

	// IL4. Image의 Pixel들이 어떤 식이던 간에
	//      BGRA순서/32비트의 ubyte단위 형태로 바꾼다. 
	//      --> Alpha 정보를 포함했을 경우 때문임.
	ilConvertImage( IL_BGRA, IL_UNSIGNED_BYTE );

	// IL4-1. Alpha 채널이 없다면 무시하고 로딩해도 무방.
	//ilConvertImage( IL_BGR, IL_UNSIGNED_BYTE );

	// IL4-2. Alpha 채널이 없는데 BGRA식으로 Alpha 채널 포멧으로 컨버팅하면
	//        Alpha 값은 무조건 0xFF(255)가 된다.

	// IL4-2. 일단 컨버팅이 되면 내부정보는 컨버팅 된 상태로 유지된다.
	//        따라서 파일포멧정보를 보존하는 것이 중요하다면,
	//        이렇게 무식하게 컨버팅해서는 안된다.

	//// Infomation Setting
	m_nWidth	= ilGetInteger( IL_IMAGE_WIDTH );
	m_nHeight	= ilGetInteger( IL_IMAGE_HEIGHT );
	m_nBytes	= ilGetInteger( IL_IMAGE_BYTES_PER_PIXEL );
	m_nBits		= ilGetInteger( IL_IMAGE_BITS_PER_PIXEL );
	m_nFormat	= ilGetInteger( IL_IMAGE_FORMAT );
	m_nType		= ilGetInteger( IL_IMAGE_TYPE );
	m_nFSet		= ilGetInteger( IL_FORMAT_SET );
	m_nFMode	= ilGetInteger( IL_FORMAT_MODE );
	m_nTSet		= ilGetInteger( IL_TYPE_SET );
	m_nTMode	= ilGetInteger( IL_TYPE_MODE );
	m_nOrgMode  = ilGetInteger( IL_ORIGIN_MODE );	

	// pixel data pointer setting
	m_pPic		= ilGetData();

	return TRUE;
}

void CDevIL::Unload()
{
	// 1. IL이 읽어들인 이미지를 삭제한다. 
	//    하지만 인덱스와 해제한 포인터 자체는 남아 있다.
	ilDeleteImages( 1, &m_imageIdx );	

	// 2. 내가 정의했던 속성 값 초기화. 
	ClearMember();						
}

BOOL CDevIL::GetPixel( INT x, INT y, BYTE* pRed, BYTE* pGreen, BYTE* pBlue, BYTE* pAlpha )
{
	if( m_imageIdx && x >= 0 && x < m_nWidth && y >= 0 && y < m_nHeight )
	{
		*pRed	= m_pPic[ ( m_nWidth * m_nBytes ) * y + ( m_nBytes * x ) + 2 ];
		*pGreen = m_pPic[ ( m_nWidth * m_nBytes ) * y + ( m_nBytes * x ) + 1 ];
		*pBlue  = m_pPic[ ( m_nWidth * m_nBytes ) * y + ( m_nBytes * x )     ];
		*pAlpha = m_pPic[ ( m_nWidth * m_nBytes ) * y + ( m_nBytes * x ) + 3 ];
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
