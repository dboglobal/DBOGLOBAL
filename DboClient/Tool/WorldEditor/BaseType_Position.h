//*****************************************************************************
// File       : BaseType_Position.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __BASE_TYPE_POSITION_H__
#define __BASE_TYPE_POSITION_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Forward Refenence Table
//-----------------------------------------------------------------------------

struct SPosition3D;
struct SLocation3D;
class CWorldEntity;


//-----------------------------------------------------------------------------
// Standard Template Library Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Const Definition Table
//-----------------------------------------------------------------------------

const RwV3d		AXIS_X	= { 1.0f, 0.0f, 0.0f };
const RwV3d		AXIS_Y	= { 0.0f, 1.0f, 0.0f };
const RwV3d		AXIS_Z	= { 0.0f, 0.0f, 1.0f };


//-----------------------------------------------------------------------------
// Macro & Template Function Table
//-----------------------------------------------------------------------------

RwV3d * GetPositionInLine( const RwLine * pLine, const RwReal fRatio, RwV3d * pPosition );

RwBool IsPointInSphere( const RwV3d * pPoint, const RwSphere * pSphere );
RwBool IsLineInSphere( const RwLine * pLine, const RwSphere * pSphere );
RwBool IsPointInBBox( const RwV3d * pPoint, const RwBBox * pBBox );
RwBool IsLineInBox( const RwLine * pLine, const RwBBox * pBBox );

void SetFrame( RwFrame * pFrame, SPosition3D * pPosition );
void SetFrame( RwFrame * pFrame, SPosition3D * pPosition, SLocation3D * pScale );

void SetMatrix( RwMatrix * pMatrix, SPosition3D * pPosition );
void SetMatrix( RwMatrix * pMatrix, SPosition3D * pPosition, SLocation3D * pScale );


inline RwBool IsPointInRect( POINT & rPoint, RECT & rRect )
{
	if( rPoint.x < rRect.left || rPoint.x >= rRect.right )
		return FALSE;

	if( rPoint.y < rRect.top || rPoint.y >= rRect.bottom )
		return FALSE;

	return TRUE;
}


inline float NormalizeDegree( float fDegree )
{
	int		nDegree = int( fDegree );
	float	fRest	= fDegree - float( nDegree );
	int		nRemain = nDegree % 360;
	float	fRemain = float(nRemain) + fRest;

	return ( ( fRemain >= 0.0f ) ? fRemain : fRemain + 360.0f );
}


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

#ifdef SLocation3D
#undef SLocation3D
#endif //SLocation3D

typedef struct SLocation3D : public RwV3d
{
	public:

		SLocation3D( void ) { Reset(); }
		SLocation3D( RwReal fDefaultX, RwReal fDefaultY, RwReal fDefaultZ ) { Set( fDefaultX, fDefaultY, fDefaultZ ); }
		SLocation3D( const RwV3d * pDefault ) { Set( pDefault ); }
		SLocation3D( const SLocation3D & rDefault ) { Set( rDefault ); }

		operator const RwV3d*() const { return GetConstRwV3d(); }
		operator RwV3d*() const { return GetRwV3d(); }

		SLocation3D & operator =( const SLocation3D & rcSourceLocation ) { Set( &rcSourceLocation ); return *this; }
		SLocation3D & operator =( const RwV3d * pSourceVector ) { Set( pSourceVector ); return *this; }

		RwBool operator ==( const SLocation3D & rcCompareLocation ) const { return IsEqual( rcCompareLocation ); }
		RwBool operator ==( const RwV3d * lpCompareVector ) const { return IsEqual( lpCompareVector ); }

		RwBool operator !=( const SLocation3D & rcCompareLocation ) const { return !IsEqual( rcCompareLocation ); }
		RwBool operator !=( const RwV3d * lpCompareVector ) const { return !IsEqual( lpCompareVector ); }

		inline const RwV3d * GetConstRwV3d( void ) const { return static_cast<const RwV3d *>(this); }
		inline RwV3d * GetRwV3d( void ) const { return const_cast<RwV3d *>(GetConstRwV3d()); }

		inline void Reset( void )
		{
			Set( LOCATION_DEFAULT );
		}

		inline void Set( const RwV3d * pNewLocation )
		{
			memcpy( this, pNewLocation, sizeof( RwV3d ) );
		}

		inline void Set( const SLocation3D & rNewLocation )
		{
			memcpy( this, &rNewLocation, sizeof( SLocation3D ) );
		}

		inline void Scale( RwReal fScale )
		{
			RwV3dScale( this, this, fScale );
		}

		inline void Scale( RwV3d * pScale )
		{
			x *= pScale->x;
			y *= pScale->y;
			z *= pScale->z;
		}

		inline void Set( RwReal fNewX, RwReal fNewY, RwReal fNewZ )
		{
			x = fNewX;
			y = fNewY;
			z = fNewZ;
		}

		inline void Add( const SLocation3D & rMovement )
		{
			x += rMovement.GetX();
			y += rMovement.GetY();
			z += rMovement.GetZ();
		}

		inline void Add( RwReal fNewX, RwReal fNewY, RwReal fNewZ )
		{
			x += fNewX;
			y += fNewY;
			z += fNewZ;
		}

		inline void Sub( const SLocation3D & rMovement )
		{
			x -= rMovement.GetX();
			y -= rMovement.GetY();
			z -= rMovement.GetZ();
		}

		inline void Sub( RwReal fNewX, RwReal fNewY, RwReal fNewZ )
		{
			x -= fNewX;
			y -= fNewY;
			z -= fNewZ;
		}

		inline RwReal GetX( void ) const { return x; }
		inline RwReal GetY( void ) const { return y; }
		inline RwReal GetZ( void ) const { return z; }

		inline void SetX( RwReal fNewX ) { x = fNewX; }
		inline void SetY( RwReal fNewY ) { y = fNewY; }
		inline void SetZ( RwReal fNewZ ) { z = fNewZ; }

		inline RwReal GetLengthSqured( void ) const { return( x * x + y * y + z * z ); }
		inline RwReal GetLength( void ) const { return sqrtf( GetLengthSqured() ); }

		inline RwBool Normalize( void )
		{
			RwReal		fLength = GetLength();

			if( fLength > 0.0f )
			{
				Scale( 1.0f / fLength );

				return TRUE;
			}
			else
			{
				return FALSE;
			}
		}

		inline void GetDistanceVector( SLocation3D & Distance, SLocation3D & Destination ) const
		{
			Distance.Set( Destination.x - x, Destination.y - y, Destination.z - z );
		}

		inline RwReal GetDistance( SLocation3D & Destination ) const
		{
			SLocation3D		Distant;
			GetDistanceVector( Distant, Destination );

			return Distant.GetLength();
		}

		inline void ConvertFromRwMatrix( RwMatrix * pMatrix )
		{
			Set( RwMatrixGetPos( pMatrix ) );
		}

		inline void ConvertToRwMatrix( RwMatrix * pMatrix, RwOpCombineType CombineType ) const
		{
			RwMatrixTranslate( pMatrix, (RwV3d *)this, CombineType );
		}

		inline RwBool IsEqual( const SLocation3D & rcSourceLocation ) const
		{
			if( GetX() != rcSourceLocation.GetX() )
			{
				return FALSE;
			}

			if( GetY() != rcSourceLocation.GetY() )
			{
				return FALSE;
			}

			if( GetZ() != rcSourceLocation.GetZ() )
			{
				return FALSE;
			}

			return TRUE;
		}

		inline RwBool IsEquivalent( const RwV3d * lpCompareVector ) const
		{
			return IsEqual( lpCompareVector );
		}

		inline RwBool IsEquivalent( const SLocation3D & rcSourceLocation ) const
		{
			return IsEqual( rcSourceLocation );
		}

		inline RwBool IsEqual( const RwV3d * lpCompareVector ) const
		{
			if( GetX() != lpCompareVector->x )
			{
				return FALSE;
			}

			if( GetY() != lpCompareVector->y )
			{
				return FALSE;
			}

			if( GetZ() != lpCompareVector->z )
			{
				return FALSE;
			}

			return TRUE;
		}

	public:

		static const SLocation3D		LOCATION_DEFAULT;
		static const SLocation3D		SCALE_DEFAULT;

} SLocation3D;


#ifdef SGlobalLocation
#undef SGlobalLocation
#endif //SGlobalLocation

typedef struct SGlobalLocation
{
	SLocation3D				Location;
	CWorldEntity *			pWorld;

	inline void Reset( void )
	{
		pWorld = NULL;

		Location.Reset();
	}

	inline void Set( SGlobalLocation * pNewLocation )
	{
		SetWorld( pNewLocation->GetWorld() );
		SetLocation( pNewLocation->GetLocation() );
	}

	inline void Set( const SLocation3D * pNewLocation, CWorldEntity * pNewWorld )
	{
		Location.Set( pNewLocation );
		pWorld = pNewWorld;
	}

	inline RwReal				GetX( void ) const { return Location.GetX(); }
	inline RwReal				GetY( void ) const { return Location.GetY(); }
	inline RwReal				GetZ( void ) const { return Location.GetZ(); }

	inline void					SetX( RwReal fNewX ) { Location.SetX( fNewX ); }
	inline void					SetY( RwReal fNewY ) { Location.SetY( fNewY ); }
	inline void					SetZ( RwReal fNewZ ) { Location.SetZ( fNewZ ); }

	inline CWorldEntity *		GetWorld( void ) const { return pWorld; }
	inline void					SetWorld( CWorldEntity * pNewWorld ) { pWorld = pNewWorld; }


	inline SLocation3D *		GetLocation( void ) const { return const_cast<SLocation3D *>(&Location); }
	inline void					SetLocation( SLocation3D * pNewLocation ) { Location.Set( pNewLocation ); }

} SGlobalLocation;


#ifdef SRotation3D
#undef SRotation3D
#endif //SRotation3D

typedef struct SRotation3D
{
    RwReal		fYaw;
	RwReal		fPitch;
	RwReal		fRoll;

	public:

		SRotation3D( void ) : fYaw( 0.0f ), fPitch( 0.0f ), fRoll( 0.0f ) {}
		SRotation3D( RwReal fDefaultYaw, RwReal fDefaultPitch = 0.0f, RwReal fDefaultRoll = 0.0f )
		: fYaw( fDefaultYaw ), fPitch( fDefaultPitch ), fRoll( fDefaultRoll ) {}

		SRotation3D & operator =( const SLocation3D & rcSourceRotation ) { Set( rcSourceRotation ); return *this; }
		SRotation3D & operator =( const RwV3d * pSourceVector ) { Set( pSourceVector ); return *this; }

		RwBool operator ==( const SRotation3D & rcCompareRotation ) const { return IsEqual( rcCompareRotation ); }
		RwBool operator !=( const SRotation3D & rcCompareRotation ) const { return !IsEqual( rcCompareRotation ); }

		inline void Reset( void )
		{
			ZeroMemory( this, sizeof( SRotation3D ) );
		}

		inline void Set( const SRotation3D & rNewRotation )
		{
			memcpy( this, &rNewRotation, sizeof( SRotation3D ) );
		}

		inline void Set( const RwV3d * pNewVector )
		{
			Set( pNewVector->y, pNewVector->x, pNewVector->z );
		}

		RwV3d * GetDirectionVector( RwV3d * pNewVector );

		inline void Set( RwReal fNewYaw, RwReal fNewPitch = 0.0f, RwReal fNewRoll = 0.0f )
		{
			SetYaw( fNewYaw );
			SetPitch( fNewPitch );
			SetRoll( fNewRoll );
		}

		inline void Add( const SRotation3D & rNewRotation )
		{
			Add( rNewRotation.GetYaw(), rNewRotation.GetPitch(), rNewRotation.GetRoll() );
		}

		inline void Add( RwReal fAddedYaw, RwReal fAddedPitch = 0.0f, RwReal fAddedRoll = 0.0f )
		{
			SetYaw( GetYaw() + fAddedYaw );
			SetPitch( GetPitch() + fAddedPitch );
			SetRoll( GetRoll() + fAddedRoll );
		}

		inline void Scale( RwReal fScale )
		{
			SetYaw( GetYaw() * fScale );
			SetPitch( GetPitch() * fScale );
			SetRoll( GetRoll() * fScale );
		}

		inline RwBool IsEqual( const SRotation3D & rcCompareRotation ) const
		{
			if( GetYaw() != rcCompareRotation.GetYaw() )
			{
				return FALSE;
			}

			if( GetPitch() != rcCompareRotation.GetPitch() )
			{
				return FALSE;
			}

			if( GetRoll() != rcCompareRotation.GetRoll() )
			{
				return FALSE;
			}

			return TRUE;
		}

		inline RwBool IsEquivalent( const SRotation3D & rcCompareRotation ) const
		{
			if( NormalizeDegree( GetYaw() ) != NormalizeDegree( rcCompareRotation.GetYaw() ) )
			{
				return FALSE;
			}

			if( NormalizeDegree( GetPitch() ) != NormalizeDegree( rcCompareRotation.GetPitch() ) )
			{
				return FALSE;
			}

			if( NormalizeDegree( GetRoll() ) != NormalizeDegree( rcCompareRotation.GetRoll() ) )
			{
				return FALSE;
			}

			return TRUE;
		}

		inline void SetYaw( RwReal fNewYaw ) { fYaw = fNewYaw; }
		inline void SetPitch( RwReal fNewPitch ) { fPitch = fNewPitch; }
		inline void SetRoll( RwReal fNewRoll ) { fRoll = fNewRoll; }

		inline RwReal GetYaw( void ) const { return fYaw; }
		inline RwReal GetPitch( void ) const { return fPitch; }
		inline RwReal GetRoll( void ) const { return fRoll; }

		inline void Normalize( void )
		{
			SetYaw( NormalizeDegree( GetYaw() ) );
			SetPitch( NormalizeDegree( GetPitch() ) );
			SetRoll( NormalizeDegree( GetRoll() ) );
		}

//		inline void ConvertFromRwMatrix( RwMatrix * pMatrix )
//		{
//			RwMatrixQueryRotate();
//			Set( 0.0f, 0.0f, 0.0f );
//		}

		inline void ConvertToRwMatrix( RwMatrix * pMatrix, RwOpCombineType CombineType ) const
		{
			RwV3d		Axis;

			RwMatrixRotate( pMatrix, &AXIS_Y, GetYaw(), CombineType );
			RwV3dTransformVector( &Axis, &AXIS_X, pMatrix );

			RwMatrixRotate( pMatrix, &Axis, GetPitch(), rwCOMBINEPOSTCONCAT );
			RwV3dTransformVector( &Axis, &Axis, pMatrix );

			RwMatrixRotate( pMatrix, &Axis, GetRoll(), rwCOMBINEPOSTCONCAT );
		}

	public:

		static const SRotation3D					ROTATION_DEFAULT;

} SRotation3D;


#ifdef SPosition3D
#undef SPosition3D
#endif //SPosition3D

typedef struct SPosition3D
{
    SLocation3D	Location;
	SRotation3D	Rotation;

	public:

		SPosition3D( void )
		{
			Reset();
		}

		SPosition3D( SLocation3D & rDefaultLocation, SRotation3D & rDefaultRotation )
		{
			SetLocation( rDefaultLocation );
			SetRotation( rDefaultRotation );
		}

		SPosition3D( const SLocation3D & rDefaultLocation, SRotation3D & rDefaultRotation )
		{
			SetLocation( rDefaultLocation );
			SetRotation( rDefaultRotation );
		}

		SPosition3D( SLocation3D & rDefaultLocation, const SRotation3D & rDefaultRotation )
		{
			SetLocation( rDefaultLocation );
			SetRotation( rDefaultRotation );
		}

		SPosition3D( const SLocation3D & rDefaultLocation, const SRotation3D & rDefaultRotation )
		{
			SetLocation( rDefaultLocation );
			SetRotation( rDefaultRotation );
		}

		SPosition3D( RwReal fLocationX, RwReal fLocationY, RwReal fLocationZ, RwReal fRotationYaw, RwReal fRotationPitch = 0.0f, RwReal fRotationRoll = 0.0f )
		{
			SetLocation( fLocationX, fLocationY, fLocationZ );
			SetRotation( fRotationYaw, fRotationPitch, fRotationRoll );
		}

		SPosition3D & operator =( const SPosition3D & rcSourcePosition ) { Set( rcSourcePosition ); return *this; }

		RwBool operator ==( const SPosition3D & rcComparePosition ) const { return IsEqual( rcComparePosition ); }

		RwBool operator !=( const SPosition3D & rcComparePosition ) const { return !IsEqual( rcComparePosition ); }

		inline void Reset( void ) { ZeroMemory( this, sizeof( SPosition3D ) ); }
		inline void Set( const SPosition3D & rPosition ) { memcpy( this, &rPosition, sizeof( SPosition3D ) ); }

		inline void Add( const SPosition3D & rAddPosition ) { Location.Add( *(rAddPosition.GetLocation()) ); Rotation.Add( *(rAddPosition.GetRotation()) ); }

		inline void SetLocation( const SLocation3D & rNewLocation ) { Location.Set( rNewLocation ); }
		inline void SetLocation( RwReal fNewX, RwReal fNewY, RwReal fNewZ ) { Location.Set( fNewX, fNewY, fNewZ ); }

		inline void AddLocation( const SLocation3D & rNewLocation ) { Location.Add( rNewLocation ); }
		inline void AddLocation( RwReal fNewX, RwReal fNewY, RwReal fNewZ ) { Location.Add( fNewX, fNewY, fNewZ ); }

		inline void SetRotation( const SRotation3D & rNewRotation ) { Rotation.Set( rNewRotation ); }
		inline void SetRotation( RwReal fNewYaw, RwReal fNewPitch, RwReal fNewRoll ) { Rotation.Set( fNewYaw, fNewPitch, fNewRoll ); }

		inline void AddRotation( const SRotation3D & rNewRotation ) { Rotation.Add( rNewRotation ); }
		inline void AddRotation( RwReal fAddedYaw, RwReal fAddedPitch, RwReal fAddedRoll ) { Rotation.Add( fAddedYaw, fAddedPitch, fAddedRoll ); }

		inline const SLocation3D * GetLocation( void ) const { return &Location; }
		inline const SRotation3D * GetRotation( void ) const { return &Rotation; }

		inline RwReal GetX( void ) const { return Location.GetX(); }
		inline RwReal GetY( void ) const { return Location.GetY(); }
		inline RwReal GetZ( void ) const { return Location.GetZ(); }

		inline void SetX( RwReal fNewX ) { Location.SetX( fNewX ); }
		inline void SetY( RwReal fNewY ) { Location.SetY( fNewY ); }
		inline void SetZ( RwReal fNewZ ) { Location.SetZ( fNewZ ); }

		inline RwReal GetYaw( void ) const { return Rotation.GetYaw(); }
		inline RwReal GetPitch( void ) const { return Rotation.GetPitch(); }
		inline RwReal GetRoll( void ) const { return Rotation.GetRoll(); }

		inline void SetYaw( RwReal fNewYaw ) { Rotation.SetYaw( fNewYaw ); }
		inline void SetPitch( RwReal fNewPitch ) { Rotation.SetPitch( fNewPitch ); }
		inline void SetRoll( RwReal fNewRoll ) { Rotation.SetRoll( fNewRoll ); }

		inline RwBool IsEqual( const SPosition3D & rcComparePosition ) const
		{
			if( GetLocation()->IsEqual( *(rcComparePosition.GetLocation()) ) == FALSE )
			{
				return FALSE;
			}

			if( GetRotation()->IsEqual( *(rcComparePosition.GetRotation()) ) == FALSE )
			{
				return FALSE;
			}

			return TRUE;
		}

		inline void ConvertFromRwFrame( RwMatrix * pMatrix )
		{
		//	Rotation.ConvertFromRwMatrix( pMatrix );
			Location.ConvertFromRwMatrix( pMatrix );
		
			RwMatrixGetPos( pMatrix );
		}

		inline void ConvertToRwFrame( RwMatrix * pMatrix, RwOpCombineType CombineType ) const
		{
			Rotation.ConvertToRwMatrix( pMatrix, CombineType );
			Location.ConvertToRwMatrix( pMatrix, rwCOMBINEPOSTCONCAT );
		}

	public:

		static const SPosition3D				POSITION_DEFAULT;

} SPosition3D;


#ifdef SLocation3DInt
#undef SLocation3DInt
#endif //SLocation3DInt

typedef struct SLocation3DInt
{
	RwInt32			iX;
	RwInt32			iY;
	RwInt32			iZ;

	RwInt32			GetX( void ) const { return iX; }
	RwInt32			GetY( void ) const { return iY; }
	RwInt32			GetZ( void ) const { return iZ; }

	void			SetX( RwInt32 iNewX ) { iX = iNewX; }
	void			SetY( RwInt32 iNewY ) { iY = iNewY; }
	void			SetZ( RwInt32 iNewZ ) { iZ = iNewZ; }

	void			IncreaseX( void ) { ++iX; }
	void			IncreaseY( void ) { ++iY; }
	void			IncreaseZ( void ) { ++iZ; }

	void			DecreaseX( void ) { --iX; }
	void			DecreaseY( void ) { --iY; }
	void			DecreaseZ( void ) { --iZ; }

	SLocation3DInt( void ) : iX( 0 ), iY( 0 ), iZ( 0 ) {}
	SLocation3DInt( RwInt32 iNewX, RwInt32 iNewY, RwInt32 iNewZ ) : iX( iNewX ), iY( iNewY ), iZ( iNewZ ) {}

	void			Set( RwInt32 iNewX, RwInt32 iNewY, RwInt32 iNewZ ) { SetX( iNewX ); SetY( iNewY ); SetZ( iNewZ ); }
	void			Set( const SLocation3DInt * pNewLocation ) { memcpy( this, pNewLocation, sizeof( SLocation3DInt ) ); }

	void			Size( const SLocation3DInt * pLocationDst, const SLocation3DInt * pLocationOrg )
					{
						SetX( pLocationDst->GetX() - pLocationOrg->GetX() + 1 );
						SetY( pLocationDst->GetY() - pLocationOrg->GetY() + 1 );
						SetZ( pLocationDst->GetZ() - pLocationOrg->GetZ() + 1 );
					}

	void			Offset( const SLocation3DInt * pLocationDst, const SLocation3DInt * pLocationOrg )
					{
						SetX( pLocationDst->GetX() - pLocationOrg->GetX() );
						SetY( pLocationDst->GetY() - pLocationOrg->GetY() );
						SetZ( pLocationDst->GetZ() - pLocationOrg->GetZ() );
					}

} SLocation3DInt;


typedef struct SBoundingBox : private RwBBox
{
	public:

		SBoundingBox( void )
		{
			ZeroMemory( this, sizeof( SBoundingBox ) );
		}

		SBoundingBox( RwV3d * pMax, RwV3d * pMin )
		{
			SetMax( pMax );
			SetMin( pMin );
		}

		operator RwBBox*() const { return GetRwBBox(); }
		operator const RwBBox*() const { return GetConstRwBBox(); }

		inline void Set( const SBoundingBox & rsBounding ) { memcpy( this, &rsBounding, sizeof( SBoundingBox ) ); }

		inline RwBBox * GetRwBBox( void ) const { return const_cast<RwBBox *>(GetConstRwBBox()); }
		inline const RwBBox * GetConstRwBBox( void ) const { return static_cast<const RwBBox *>(this); }

		inline void SetMax( const RwV3d * pMax ) { memcpy( &sup, pMax, sizeof( RwV3d ) ); }
		inline void SetMin( const RwV3d * pMin ) { memcpy( &inf, pMin, sizeof( RwV3d ) ); }

		inline const RwV3d * GetMin( void ) const { return &inf; }
		inline const RwV3d * GetMax( void ) const { return &sup; }

		inline RwReal GetMinX( void ) const { return inf.x; }
		inline RwReal GetMinY( void ) const { return inf.y; }
		inline RwReal GetMinZ( void ) const { return inf.z; }

		inline RwReal GetMaxX( void ) const { return sup.x; }
		inline RwReal GetMaxY( void ) const { return sup.y; }
		inline RwReal GetMaxZ( void ) const { return sup.z; }

		inline void SetMinX( RwReal fNewMinX ) { inf.x = fNewMinX; }
		inline void SetMinY( RwReal fNewMinY ) { inf.y = fNewMinY; }
		inline void SetMinZ( RwReal fNewMinZ ) { inf.z = fNewMinZ; }

		inline void SetMaxX( RwReal fNewMaxX ) { sup.x = fNewMaxX; }
		inline void SetMaxY( RwReal fNewMaxY ) { sup.y = fNewMaxY; }
		inline void SetMaxZ( RwReal fNewMaxZ ) { sup.z = fNewMaxZ; }

		inline RwReal GetSizeX( void ) const { return ( GetMaxX() - GetMinX() ); }
		inline RwReal GetSizeY( void ) const { return ( GetMaxY() - GetMinY() ); }
		inline RwReal GetSizeZ( void ) const { return ( GetMaxZ() - GetMinZ() ); }

		inline RwV3d * GetSize( RwV3d * pSize ) const { RwV3dSubMacro( pSize, &sup, &inf ); return pSize; }
		inline RwBool IsValid( void ) const
		{
			if( GetSizeX() < 0 ||  GetSizeY() < 0 || GetSizeZ() < 0 )
			{
				return FALSE;
			}

			return TRUE;
		}

} SBoundingBox;

#endif //__BASE_TYPE_POSITION_H__

//*****************************************************************************
//
// End of File : BaseType_Position.h
//
//*****************************************************************************