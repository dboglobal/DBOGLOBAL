//*****************************************************************************
// File       : BaseType_Color.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __BASE_TYPE_COLOR_H__
#define __BASE_TYPE_COLOR_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Forward Refenence Table
//-----------------------------------------------------------------------------

struct SColorInt;
struct SColorFloat;

//-----------------------------------------------------------------------------
// Standard Template Library Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Const Definition Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef struct SColorInt : public RwRGBA
{
		SColorInt( void ) { Reset(); }
		SColorInt( RwUInt8 iRed, RwUInt8 iGreen, RwUInt8 iBlue, RwUInt8 iAlpha ) { Set( iRed, iGreen, iBlue, iAlpha ); }
		SColorInt( const RwRGBA & rDefault ) { Set( rDefault ); }
		SColorInt( const SColorInt & rDefault ) { Set( rDefault ); }

		SColorInt &	operator =( const SColorInt & rcSourceColor ) { Set( rcSourceColor ); return *this; }
		SColorInt &	operator =( const RwRGBA * pSourceColor ) { Set( pSourceColor ); return *this; }

		RwBool operator ==( const SColorInt & rcCompareColor ) const { return IsEqual( rcCompareColor ); }
		RwBool operator ==( const RwRGBA * pCompareColor ) const { return IsEqual( pCompareColor ); }

		RwBool operator !=( const SColorInt & rcCompareColor ) const { return !IsEqual( rcCompareColor ); }
		RwBool operator !=( const RwRGBA * pCompareColor ) const { return !IsEqual( pCompareColor ); }

		operator const RwRGBA*() const { return GetConstRwRGBA(); }
		operator RwRGBA*() const { return GetRwRGBA(); }

		inline const RwRGBA * GetConstRwRGBA( void ) const { return static_cast<const RwRGBA *>(this); }
		inline RwRGBA * GetRwRGBA( void ) const { return const_cast<RwRGBA *>(GetConstRwRGBA()); }

		inline void Reset( void )
		{
			ZeroMemory( this, sizeof( SColorInt ) );
		}

		inline void Set( const RwRGBA * pNewColor )
		{
			memcpy( this, pNewColor, sizeof( RwRGBA ) );
		}

		inline void Set( const SColorInt & rNewColor )
		{
			memcpy( this, &rNewColor, sizeof( SColorInt ) );
		}

		inline void Set( RwUInt8 iRed, RwUInt8 iGreen, RwUInt8 iBlue, RwUInt8 iAlpha )
		{
			SetRed( iRed );
			SetGreen( iGreen );
			SetBlue( iBlue );
			SetAlpha( iAlpha );
		}

		inline RwBool IsEqual( const SColorInt & rcCompareColor ) const
		{
			if( GetRed() != rcCompareColor.GetRed() )
			{
				return FALSE;
			}
			
			if( GetGreen() != rcCompareColor.GetGreen() )
			{
				return FALSE;
			}

			if( GetBlue() != rcCompareColor.GetBlue() )
			{
				return FALSE;
			}

			if( GetAlpha() != rcCompareColor.GetAlpha() )
			{
				return FALSE;
			}

			return TRUE;
		}

		inline RwBool IsEqual( const RwRGBA * pNewColor ) const
		{
			if( GetRed() != pNewColor->red )
			{
				return FALSE;
			}
			
			if( GetGreen() != pNewColor->green )
			{
				return FALSE;
			}

			if( GetBlue() != pNewColor->blue )
			{
				return FALSE;
			}

			if( GetAlpha() != pNewColor->alpha )
			{
				return FALSE;
			}

			return TRUE;
		}

		inline RwBool IsValid( void ) const
		{
			return TRUE;
		}

		inline RwUInt8 GetRed( void ) const { return RwRGBA::red; }
		inline RwUInt8 GetGreen( void ) const { return RwRGBA::green; }
		inline RwUInt8 GetBlue( void ) const { return RwRGBA::blue; }
		inline RwUInt8 GetAlpha( void ) const { return RwRGBA::alpha; }

		inline void SetRed( RwUInt8 iRed ) { RwRGBA::red = iRed; }
		inline void SetGreen( RwUInt8 iGreen ) { RwRGBA::green = iGreen; }
		inline void SetBlue( RwUInt8 iBlue ) { RwRGBA::blue = iBlue; }
		inline void SetAlpha( RwUInt8 iAlpha ) { RwRGBA::alpha = iAlpha; }

		void ConvertFromFloat( SColorFloat * pColorFloat );
		void ConvertToFloat( SColorFloat * pColorFloat ) const;

		RwUInt32 ConvertToUInt32( void ) const
		{
			RwUInt32		iColor = GetAlpha() << 24 | GetRed() << 16 | GetGreen() << 8 | GetBlue();

			return iColor;
		}

		void ConverFromDWORD( RwUInt32 iColor )
		{
			SetBlue( iColor & 0xFF );
			iColor >>= 8;
			SetGreen( iColor & 0xFF );
			iColor >>= 8;
			SetRed( iColor & 0xFF );
			iColor >>= 8;
			SetAlpha( iColor & 0xFF );
		}

	public:

		static const SColorInt			COLOR_BLANK;
		static const SColorInt			COLOR_BLACK;
		static const SColorInt			COLOR_WHITE;
		static const SColorInt			COLOR_RED;
		static const SColorInt			COLOR_GREEN;
		static const SColorInt			COLOR_BLUE;

} SColorInt;


typedef struct SColorFloat : public RwRGBAReal
{
		SColorFloat( void ) { Reset(); }
		SColorFloat( RwReal fRed, RwReal fGreen, RwReal fBlue, RwReal fAlpha ) { Set( fRed, fGreen, fBlue, fAlpha ); }
		SColorFloat( const RwRGBAReal & rDefault ) { Set( &rDefault ); }
		SColorFloat( const SColorFloat & rDefault ) { Set( &rDefault ); }

		SColorFloat & operator =( const SColorFloat & rcSourceColor ) { Set( rcSourceColor ); return *this; }
		SColorFloat & operator =( const RwRGBAReal * pSourceColor ) { Set( pSourceColor ); return *this; }

		RwBool operator ==( const SColorFloat & rcCompareColor ) const { return IsEqual( rcCompareColor ); }
		RwBool operator ==( const RwRGBAReal * pCompareColor ) const { return IsEqual( pCompareColor ); }

		RwBool operator !=( const SColorFloat & rcCompareColor ) const { return !IsEqual( rcCompareColor ); }
		RwBool operator !=( const RwRGBAReal * pCompareColor ) const { return !IsEqual( pCompareColor ); }

		operator const RwRGBAReal*() const { return GetConstRwRGBAReal(); }
		operator RwRGBAReal*() const { return GetRwRGBAReal(); }

		inline const RwRGBAReal * GetConstRwRGBAReal( void ) const { return static_cast<const RwRGBAReal *>(this); }
		inline RwRGBAReal * GetRwRGBAReal( void ) const { return const_cast<RwRGBAReal *>(GetConstRwRGBAReal()); }

		inline void Reset( void )
		{
			ZeroMemory( this, sizeof( SColorFloat ) );
		}

		inline void Set( const RwRGBAReal * pNewColor )
		{
			memcpy( this, pNewColor, sizeof( RwRGBAReal ) );
		}

		inline void Set( const SColorInt & rNewColor )
		{
			memcpy( this, &rNewColor, sizeof( SColorFloat ) );
		}

		inline void Set( RwReal fRed, RwReal fGreen, RwReal fBlue, RwReal fAlpha )
		{
			SetRed( fRed );
			SetGreen( fGreen );
			SetBlue( fBlue );
			SetAlpha( fAlpha );
		}

		inline void Scale( RwReal fScale, RwBool bApplyToAlpha = FALSE )
		{
			SetRed( GetRed() * fScale );
			SetGreen( GetGreen() * fScale );
			SetBlue( GetBlue() * fScale );

			if( bApplyToAlpha != FALSE )
			{
				SetAlpha( GetAlpha() * fScale );
			}
		}
/*
		inline void Scale( RwReal fScale, RwBool bApplyToAlpha = FALSE )
		{
			SetRed( GetRed() * fScale );
			SetGreen( GetGreen() * fScale );
			SetBlue( GetBlue() * fScale );

			if( bApplyToAlpha != FALSE )
			{
				SetAlpha( GetAlpha() * fScale );
			}
		}
*/
		inline void Scale( SColorFloat & rcScale )
		{
			SetRed( GetRed() * rcScale.GetRed() );
			SetGreen( GetGreen() * rcScale.GetGreen() );
			SetBlue( GetBlue() * rcScale.GetGreen() );
			SetAlpha( GetAlpha() * rcScale.GetAlpha() );
		}

		inline RwBool IsEqual( const SColorFloat & rcCompareColor ) const
		{
			if( GetRed() != rcCompareColor.GetRed() )
			{
				return FALSE;
			}
			
			if( GetGreen() != rcCompareColor.GetGreen() )
			{
				return FALSE;
			}

			if( GetBlue() != rcCompareColor.GetBlue() )
			{
				return FALSE;
			}

			if( GetAlpha() != rcCompareColor.GetAlpha() )
			{
				return FALSE;
			}

			return TRUE;
		}

		inline RwBool IsEqual( const RwRGBAReal * pNewColor ) const
		{
			if( GetRed() != pNewColor->red )
			{
				return FALSE;
			}
			
			if( GetGreen() != pNewColor->green )
			{
				return FALSE;
			}

			if( GetBlue() != pNewColor->blue )
			{
				return FALSE;
			}

			if( GetAlpha() != pNewColor->alpha )
			{
				return FALSE;
			}

			return TRUE;
		}

		inline RwBool IsValid( void ) const
		{
			if( GetRed() < 0.0f || GetRed() > 1.0f )
			{
				return FALSE;
			}
			
			if( GetGreen() < 0.0f || GetGreen() > 1.0f )
			{
				return FALSE;
			}

			if( GetBlue() < 0.0f || GetBlue() > 1.0f )
			{
				return FALSE;
			}

			if( GetAlpha() < 0.0f || GetAlpha() > 1.0f )
			{
				return FALSE;
			}

			return TRUE;
		}

		inline void Get( RwRGBAReal * pColor )
		{
			memcpy( pColor, this, sizeof( SColorFloat ) );
		}

		inline RwReal GetRed( void ) const { return RwRGBAReal::red; }
		inline RwReal GetGreen( void ) const { return RwRGBAReal::green; }
		inline RwReal GetBlue( void ) const { return RwRGBAReal::blue; }
		inline RwReal GetAlpha( void ) const { return RwRGBAReal::alpha; }

		inline void SetRed( RwReal fRed ) { RwRGBAReal::red = fRed; }
		inline void SetGreen( RwReal fGreen ) { RwRGBAReal::green = fGreen; }
		inline void SetBlue( RwReal fBlue ) { RwRGBAReal::blue = fBlue; }
		inline void SetAlpha( RwReal fAlpha ) { RwRGBAReal::alpha = fAlpha; }

		void ConvertFromInt( SColorInt * pColorInt );
		void ConvertToInt( SColorInt * pColorInt ) const;

	public:

		static const SColorFloat			COLOR_BLANK;
		static const SColorFloat			COLOR_BLACK;
		static const SColorFloat			COLOR_WHITE;
		static const SColorFloat			COLOR_RED;
		static const SColorFloat			COLOR_GREEN;
		static const SColorFloat			COLOR_BLUE;

} SColorFloat;

//-----------------------------------------------------------------------------
// Macro & Template Function Table
//-----------------------------------------------------------------------------

#endif //__BASE_TYPE_COLOR_H__

//*****************************************************************************
//
// End of File : BaseType_Color.h
//
//*****************************************************************************