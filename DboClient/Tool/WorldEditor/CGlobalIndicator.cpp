//*****************************************************************************
// File       : CGlobalIndicator.cpp
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "StdAfx.h"

#include "CSerializer.h"

#include "CGlobalIndicator.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CGlobalIndicator * CGlobalIndicator::m_pInstance	= NULL;


//-----------------------------------------------------------------------------
// Name			: CreateInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CGlobalIndicator::CreateInstance( void )
{
	if( m_pInstance == NULL )
	{
		m_pInstance = NTL_NEW CGlobalIndicator;

		if( m_pInstance != NULL )
		{
			return TRUE;
		}
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name			: DestroyInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CGlobalIndicator::DestroyInstance( void )
{
	if( m_pInstance != NULL )
	{
		NTL_DELETE( m_pInstance );
		m_pInstance = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: CGlobalIndicator
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CGlobalIndicator::CGlobalIndicator()
{
	Init();

	return;
}


//-----------------------------------------------------------------------------
// Name			: ~CGlobalIndicator
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CGlobalIndicator::~CGlobalIndicator()
{
	Destroy();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Init
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CGlobalIndicator::Init( void )
{
	CSerializer			cSerializer;

	cSerializer.LoadFromFile( RWSTRING( "Enviroment.ini" ) );

//	SetCameraMoveSpeedByKeyboard( cSerializer.SearchStringAndGetFloat( "#CameraMoveSpeedByKeyboard" ) );
//	SetCameraRotateSpeedByKeyboard( cSerializer.SearchStringAndGetFloat( "#CameraRotateSpeedByKeyboard" ) );
//	SetCameraMoveSpeedByMouse( cSerializer.SearchStringAndGetFloat( "#CameraMoveSpeedByMouse" ) );
//	SetCameraRotateSpeedByMouse( cSerializer.SearchStringAndGetFloat( "#CameraRotateSpeedByMouse" ) );

	SetCameraMoveSpeedByKeyboard( 100.0f );
	SetCameraRotateSpeedByKeyboard( 100.0f );
	SetCameraMoveSpeedByMouse( 100.0f );
	SetCameraRotateSpeedByMouse( 100.0f );

	for( RwUInt32 iTextIndex = 0; iTextIndex < 16; ++iTextIndex )
	{
        ZeroMemory( m_TextBuffer[iTextIndex], sizeof( RwChar ) * 256 );
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CGlobalIndicator::Destroy( void )
{
	BEGIN_FUNCTION( "CGlobalIndicator::Destroy" );

	END_FUNCTION_VOID();
}


//*****************************************************************************
//
// End of File : CGlobalIndicator.cpp
//
//*****************************************************************************