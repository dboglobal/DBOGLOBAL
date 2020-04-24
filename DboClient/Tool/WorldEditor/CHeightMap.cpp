//*****************************************************************************
// File       : CHeightMap.cpp
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

#include "CEditorApplication.h"
#include "CHeightMap.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CFixedString CHeightMap::m_FilePath = CFixedString( _T( "world\\heightmap" ) );
CFixedString CHeightMap::m_FileExt = CFixedString( _T( "whb" ) );


//-----------------------------------------------------------------------------
// Name			: Initialize
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CHeightMap::Initialize( void )
{
	m_fPrecision	= 0.0f;
	m_iNodeCount	= 0;

	m_pHeightMap	= NULL;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Create
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CHeightMap::Create( const SBoundingBox * pBounding, RwReal fPrecision )
{
	m_fPrecision	= fPrecision;

	m_sBounding.Set( *pBounding );

	m_sNodeCount.SetX( static_cast<RwUInt32>(pBounding->GetSizeX() / fPrecision) + 1 );
	m_sNodeCount.SetY( 1 );
	m_sNodeCount.SetZ( static_cast<RwUInt32>(pBounding->GetSizeZ() / fPrecision) + 1 );

	if( static_cast<RwReal>(static_cast<RwInt32>(pBounding->GetSizeX() / fPrecision)) * fPrecision != pBounding->GetSizeX() )
	{
		m_sNodeCount.IncreaseX();
	}

	if( static_cast<RwReal>(static_cast<RwInt32>(pBounding->GetSizeZ() / fPrecision)) * fPrecision != pBounding->GetSizeZ() )
	{
		m_sNodeCount.IncreaseZ();
	}

	m_iNodeCount	= m_sNodeCount.GetX() * m_sNodeCount.GetY() * m_sNodeCount.GetZ();

	if( m_iNodeCount > 1 )
	{
		m_pHeightMap	= NTL_NEW RwReal[m_iNodeCount];
	}
	else
	{
		m_pHeightMap	= NTL_NEW RwReal;
	}

	ZeroMemory( m_pHeightMap, sizeof( RwReal ) * m_iNodeCount );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CHeightMap::Destroy( void )
{
	if( m_pHeightMap != NULL )
	{
		if( m_iNodeCount > 1 )
		{
			NTL_ARRAY_DELETE( m_pHeightMap );
		}
		else
		{
			NTL_DELETE( m_pHeightMap );
		}

		m_pHeightMap = NULL;
	}

	Initialize();

	return;
}


//-----------------------------------------------------------------------------
// Name			: GetNode
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwReal CHeightMap::GetNode( SLocation3DInt * pNodeIndex ) const
{
	Assert( m_pHeightMap != NULL );
	Assert( pNodeIndex != NULL );
	Assert( pNodeIndex->GetX() < m_sNodeCount.GetX() );
	Assert( pNodeIndex->GetZ() < m_sNodeCount.GetZ() );

	RwUInt32	iNodeOffset = CalculateNodeOffset( pNodeIndex );

	if( m_iNodeCount > 1 )
	{
		return m_pHeightMap[iNodeOffset];
	}
	else
	{
		return *m_pHeightMap;
	}
}


//-----------------------------------------------------------------------------
// Name			: SetNode
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CHeightMap::SetNode( SLocation3DInt * pNodeIndex, RwReal fHeight )
{
	Assert( m_pHeightMap != NULL );
	Assert( pNodeIndex != NULL );
	Assert( pNodeIndex->GetX() < m_sNodeCount.GetX() );
	Assert( pNodeIndex->GetZ() < m_sNodeCount.GetZ() );

	RwUInt32	iNodeOffset = CalculateNodeOffset( pNodeIndex );

	if( m_iNodeCount > 1 )
	{
		m_pHeightMap[iNodeOffset] = fHeight;
	}
	else
	{
		*m_pHeightMap = fHeight;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: CalculateNodeOffset
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwUInt32 CHeightMap::CalculateNodeOffset( SLocation3DInt * pNodeIndex ) const
{
	Assert( pNodeIndex != NULL );
	Assert( pNodeIndex->GetX() < m_sNodeCount.GetX() && pNodeIndex->GetZ() < m_sNodeCount.GetZ() );

	return pNodeIndex->GetZ() * m_sNodeCount.GetX() + pNodeIndex->GetX();
}


//-----------------------------------------------------------------------------
// Name			: GetNodeIndex
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CHeightMap::GetNodeIndex( SLocation3D * pPosition, SLocation3DInt * pNodeIndex ) const
{
	Assert( pPosition != NULL );
	Assert( pNodeIndex != NULL );
	Assert( m_fPrecision > 0.0f );
//	Assert( pPosition->GetX() >= m_sBounding.GetMinX() && pPosition->GetX() <= m_sBounding.GetMaxX() );
//	Assert( pPosition->GetZ() >= m_sBounding.GetMinZ() && pPosition->GetZ() <= m_sBounding.GetMaxZ() );

	SLocation3D		sOffset( pPosition->GetX() - m_sBounding.GetMinX(), 0.0f, pPosition->GetZ() - m_sBounding.GetMinZ() );

	pNodeIndex->Set( static_cast<RwUInt32>(sOffset.GetX() / m_fPrecision), 0, static_cast<RwUInt32>(sOffset.GetZ() / m_fPrecision) );

	Assert( pNodeIndex->GetX() >= 0 );
	Assert( pNodeIndex->GetX() < m_sNodeCount.GetX() );
	Assert( pNodeIndex->GetZ() >= 0 );
	Assert( pNodeIndex->GetZ() < m_sNodeCount.GetZ() );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: GetHeight
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwReal CHeightMap::GetHeight( SLocation3D * pPosition ) const
{
	Assert( pPosition != NULL );
	Assert( pPosition->GetX() >= m_sBounding.GetMinX() && pPosition->GetX() <= m_sBounding.GetMaxX() );
	Assert( pPosition->GetZ() >= m_sBounding.GetMinZ() && pPosition->GetZ() <= m_sBounding.GetMaxZ() );

	SLocation3DInt		LeftTopIndex;
	GetNodeIndex( pPosition, &LeftTopIndex );

	SLocation3D			LeftTop;
	LeftTop.Set( m_sBounding.GetMinX() + static_cast<RwReal>(LeftTopIndex.GetX()) * m_fPrecision,
				GetNode( &LeftTopIndex ),
				m_sBounding.GetMinZ() + static_cast<RwReal>(LeftTopIndex.GetZ()) * m_fPrecision );

	SLocation3DInt		RightBottomIndex( LeftTopIndex );
	RightBottomIndex.IncreaseX();
	RightBottomIndex.IncreaseZ();

	SLocation3D			RightBottom;
	RightBottom.Set( LeftTop.GetX() + m_fPrecision,
					GetNode( &RightBottomIndex ),
					LeftTop.GetZ() + m_fPrecision );

	SLocation3D			Offset( pPosition->GetX() - LeftTop.GetX(), 0.0f, pPosition->GetZ() - LeftTop.GetZ() );

	if( Offset.GetX() == 0.0f && Offset.GetZ() == 0.0f )
	{
		return GetNode( &LeftTopIndex );
	}
	else
	{
		SLocation3DInt		SideIndex( LeftTopIndex );
		SLocation3D			Side( LeftTop );

		if( Offset.GetX() >= Offset.GetZ() )
		{
			SideIndex.IncreaseX();
			Side.SetX( Side.GetX() + m_fPrecision );
			Side.SetY( GetNode( &SideIndex ) );

			return GetInterpolatedHeightRightSide( &Offset, LeftTop.GetY(), Side.GetY(), RightBottom.GetY() );
		}
		else
		{
			SideIndex.IncreaseZ();
			Side.SetZ( Side.GetZ() + m_fPrecision );
			Side.SetY( GetNode( &SideIndex ) );

			return GetInterpolatedHeightBottomSide( &Offset, LeftTop.GetY(), Side.GetY(), RightBottom.GetY() );
		}
	}
}


//-----------------------------------------------------------------------------
// Name			: GetInterpolatedHeightRightSide
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwReal CHeightMap::GetInterpolatedHeightRightSide( SLocation3D * pOffset, RwReal fTop, RwReal fLeft, RwReal fRight ) const
{
	RwReal		fBottomZ = pOffset->GetZ() * m_fPrecision / pOffset->GetX();
	RwReal		fBottomY = fLeft + ( fRight - fLeft ) * fBottomZ / m_fPrecision;

	return fTop + ( fBottomY - fTop ) * pOffset->GetX() / m_fPrecision;
}


//-----------------------------------------------------------------------------
// Name			: GetInterpolatedHeightBottomSide
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwReal CHeightMap::GetInterpolatedHeightBottomSide( SLocation3D * pOffset, RwReal fTop, RwReal fLeft, RwReal fRight ) const
{
	RwReal		fBottomX = pOffset->GetX() * m_fPrecision / pOffset->GetZ();
	RwReal		fBottomY = fLeft + ( fRight - fLeft ) * fBottomX / m_fPrecision;

	return fTop + ( fBottomY - fTop ) * pOffset->GetZ() / m_fPrecision;
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CHeightMap::Import( const CFixedString * pName )
{
	HANDLE				hFile;

	Assert( pName != NULL );
	Assert( pName->GetBuffer() != NULL );

	RwChar				szFullPathName[1024];

	rwsprintf( szFullPathName, RWSTRING( "%s\\%s\\%s.%s" ), g_pEngineApp->GetAppPath()->GetBuffer(),
				GetFilePath()->GetBuffer(), pName->GetBuffer(), GetFileExt()->GetBuffer() );

	hFile = CreateFileSafe( szFullPathName, true );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		Assert( !"CreateFile() Failure" );

		return FALSE;
	}

	DWORD				dwReadSize;
	DWORD				dwResultSize;

	// Read Header

	dwReadSize = sizeof( SBoundingBox );

	if( ReadFile( hFile, reinterpret_cast<void *>(&m_sBounding), dwReadSize, &dwResultSize, (LPOVERLAPPED)NULL ) == false )
	{
		CloseHandle( hFile );

		return FALSE;
	}

	dwReadSize = sizeof( RwReal );

	if( ReadFile( hFile, reinterpret_cast<void *>(&m_fPrecision), dwReadSize, &dwResultSize, (LPOVERLAPPED)NULL ) == false )
	{
		CloseHandle( hFile );

		return FALSE;
	}

	dwReadSize = sizeof( SLocation3DInt );

	if( ReadFile( hFile, reinterpret_cast<void *>(&m_sNodeCount), dwReadSize, &dwResultSize, (LPOVERLAPPED)NULL ) == false )
	{
		CloseHandle( hFile );

		return FALSE;
	}

	m_iNodeCount	= m_sNodeCount.GetX() * m_sNodeCount.GetY() * m_sNodeCount.GetZ();

	if( m_iNodeCount > 1 )
	{
		m_pHeightMap	= NTL_NEW RwReal[m_iNodeCount];
	}
	else
	{
		m_pHeightMap	= NTL_NEW RwReal;
	}

	ZeroMemory( m_pHeightMap, sizeof( RwReal ) * m_iNodeCount );

	// Read Nodes

	if( m_iNodeCount > 1 )
	{
		dwReadSize = sizeof( RwReal ) * m_iNodeCount;
	}
	else
	{
		dwReadSize = sizeof( RwReal );
	}

	if( ReadFile( hFile, reinterpret_cast<void *>(m_pHeightMap), dwReadSize, &dwResultSize, (LPOVERLAPPED)NULL ) == false )
	{
		CloseHandle( hFile );

		return FALSE;
	}

	CloseHandle( hFile );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Export
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CHeightMap::Export( const CFixedString * pName )
{
	HANDLE				hFile;

	Assert( pName != NULL );
	Assert( pName->GetBuffer() != NULL );

	RwChar				szFullPathName[1024];
	rwsprintf( szFullPathName, RWSTRING( "%s\\%s.%s" ),
				GetFilePath()->GetBuffer(), pName->GetBuffer(), GetFileExt()->GetBuffer() );

	hFile = CreateFileSafe( szFullPathName, false );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		Assert( !"CreateFile() Failure" );

		return FALSE;
	}

	DWORD				dwWriteSize;
	DWORD				dwWrittenSize;

	// Write Header

	dwWriteSize = sizeof( SBoundingBox );

	if( WriteFile( hFile, reinterpret_cast<void *>(&m_sBounding), dwWriteSize, &dwWrittenSize, (LPOVERLAPPED)NULL ) == false )
	{
		CloseHandle( hFile );

		return FALSE;
	}

	dwWriteSize = sizeof( RwReal );

	if( WriteFile( hFile, reinterpret_cast<void *>(&m_fPrecision), dwWriteSize, &dwWrittenSize, (LPOVERLAPPED)NULL ) == false )
	{
		CloseHandle( hFile );

		return FALSE;
	}

	dwWriteSize = sizeof( SLocation3DInt );

	if( WriteFile( hFile, reinterpret_cast<void *>(&m_sNodeCount), dwWriteSize, &dwWrittenSize, (LPOVERLAPPED)NULL ) == false )
	{
		CloseHandle( hFile );

		return FALSE;
	}

	// Write Nodes

	if( m_iNodeCount > 1 )
	{
		dwWriteSize = sizeof( RwReal ) * m_iNodeCount;
	}
	else
	{
		dwWriteSize = sizeof( RwReal );
	}

	if( WriteFile( hFile, (void *)m_pHeightMap, dwWriteSize, &dwWrittenSize, (LPOVERLAPPED)NULL ) == false )
	{
		CloseHandle( hFile );

		return FALSE;
	}

	CloseHandle( hFile );

	return TRUE;
}


//*****************************************************************************
//
// End of File : CHeightMap.cpp
//
//*****************************************************************************