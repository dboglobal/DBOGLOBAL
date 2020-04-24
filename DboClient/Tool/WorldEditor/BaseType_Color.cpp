//*****************************************************************************
// File       : BaseType_Color.cpp
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

#include "BaseType_Color.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

const SColorFloat SColorFloat::COLOR_BLANK		= SColorFloat( 0.0f, 0.0f, 0.0f, 0.0f );
const SColorFloat SColorFloat::COLOR_BLACK		= SColorFloat( 0.0f, 0.0f, 0.0f, 1.0f );
const SColorFloat SColorFloat::COLOR_RED		= SColorFloat( 1.0f, 0.0f, 0.0f, 1.0f );
const SColorFloat SColorFloat::COLOR_GREEN		= SColorFloat( 0.0f, 1.0f, 0.0f, 1.0f );
const SColorFloat SColorFloat::COLOR_BLUE		= SColorFloat( 0.0f, 0.0f, 1.0f, 1.0f );
const SColorFloat SColorFloat::COLOR_WHITE		= SColorFloat( 1.0f, 1.0f, 1.0f, 1.0f );

const SColorInt SColorInt::COLOR_BLANK			= SColorInt( 0, 0, 0, 0 );
const SColorInt SColorInt::COLOR_BLACK			= SColorInt( 0, 0, 0, 255 );
const SColorInt SColorInt::COLOR_RED			= SColorInt( 255, 0, 0, 255 );
const SColorInt SColorInt::COLOR_GREEN			= SColorInt( 0, 255, 0, 255 );
const SColorInt SColorInt::COLOR_BLUE			= SColorInt( 0, 0, 255, 255 );
const SColorInt SColorInt::COLOR_WHITE			= SColorInt( 255, 255, 255, 255 );


//-----------------------------------------------------------------------------
// Name			: ConvertFromFloat
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void SColorInt::ConvertFromFloat( SColorFloat * pColorFloat )
{
	Assert( pColorFloat != NULL );

	RwRGBAFromRwRGBAReal( this, pColorFloat );
}


//-----------------------------------------------------------------------------
// Name			: ConvertToFloat
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void SColorInt::ConvertToFloat( SColorFloat * pColorFloat ) const
{
	Assert( pColorFloat != NULL );

	RwRGBARealFromRwRGBA( pColorFloat->GetRwRGBAReal(), GetRwRGBA() );
}


//-----------------------------------------------------------------------------
// Name			: ConvertFromInt
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void SColorFloat::ConvertFromInt( SColorInt * pColorInt )
{
	Assert( pColorInt != NULL );

	RwRGBARealFromRwRGBA( this, pColorInt->GetRwRGBA() );
}


//-----------------------------------------------------------------------------
// Name			: ConvertToInt
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void SColorFloat::ConvertToInt( SColorInt * pColorInt ) const
{
	Assert( pColorInt != NULL );

	RwRGBAFromRwRGBAReal( pColorInt, this );
}


//*****************************************************************************
//
// End of File : BaseType_Color.cpp
//
//*****************************************************************************