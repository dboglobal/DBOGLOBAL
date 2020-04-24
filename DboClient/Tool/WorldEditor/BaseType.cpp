//*****************************************************************************
// File       : BaseType.cpp
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

#include "CEntity.h"

#include "BaseType.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name			: CreateFileSafe
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

HANDLE CreateFileSafe( LPCTSTR lptFileName, bool bForRead )
{
	HANDLE		hFile;
	DWORD		dwAccessMode	= ( bForRead == true ) ? GENERIC_READ : GENERIC_WRITE;
	DWORD		dwShareMode		= ( bForRead == true ) ? FILE_SHARE_READ : ( FILE_SHARE_READ | FILE_SHARE_WRITE );
	DWORD		dwCreateMode	= ( bForRead == true ) ? OPEN_EXISTING : CREATE_ALWAYS;
	bool		bConvertUnicodeToAnsi;
	bool		bUseUnicodeAPI;

#ifdef _UNICODE
	OSVERSIONINFO		sOSVersionInfo;

	sOSVersionInfo.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );

	GetVersionEx( &sOSVersionInfo );

	switch( sOSVersionInfo.dwPlatformId )
	{
		case VER_PLATFORM_WIN32s:
		case VER_PLATFORM_WIN32_WINDOWS:
		{
			bConvertUnicodeToAnsi	= true;
			bUseUnicodeAPI			= false;

			break;
		}

		case VER_PLATFORM_WIN32_NT:
		{
			bConvertUnicodeToAnsi	= false;
			bUseUnicodeAPI			= true;

			break;
		}

		default:
		{
			return false;
		}
	}
#else //_UNICODE
	bConvertUnicodeToAnsi	= false;
	bUseUnicodeAPI			= false;
#endif //_UNICODE


	if( bConvertUnicodeToAnsi == true )
	{
#ifdef _UNICODE
		char		szFileNameAnsi[512];

		WideCharToMultiByte( CP_ACP, WC_NO_BEST_FIT_CHARS,
							lptFileName, -1,
							szFileNameAnsi, 512,
							NULL, NULL );

		hFile = CreateFileA( szFileNameAnsi, dwAccessMode, dwShareMode,
							NULL, dwCreateMode, FILE_ATTRIBUTE_NORMAL, NULL );
#else //_UNICODE
		hFile = INVALID_HANDLE_VALUE;
#endif //_UNICODE
	}
	else
	{
		hFile = CreateFile( lptFileName, dwAccessMode, dwShareMode,
							NULL, dwCreateMode, FILE_ATTRIBUTE_NORMAL, NULL );
	}

	return hFile;
}


//-----------------------------------------------------------------------------
// Name			: GetRpWorldSectorIndexByPositionFB
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const RpWorldSector * GetRpWorldSectorIndexByPositionFB( const RpSector * pRpSector, SLocation3DInt * pOffset, SLocation3DInt * pSize )
{
	switch( pRpSector->type )
	{
		case RP_SECTOR_TYPE_PLANE_X:
		{
			pSize->SetX( (pSize->GetX() + 1) >> 1 );

			if( pOffset->GetX() < pSize->GetX() )
			{
				return GetRpWorldSectorIndexByPositionFB( reinterpret_cast<const RpPlaneSector *>(pRpSector)->leftSubTree, pOffset, pSize );
			}
			else
			{
				pOffset->SetX( pOffset->GetX() - pSize->GetX() );

				return GetRpWorldSectorIndexByPositionFB( reinterpret_cast<const RpPlaneSector *>(pRpSector)->rightSubTree, pOffset, pSize );
			}
		}

		case RP_SECTOR_TYPE_PLANE_Y:
		{
			pSize->SetY( (pSize->GetY() + 1) >> 1 );

			if( pOffset->GetY() < pSize->GetY() )
			{
				return GetRpWorldSectorIndexByPositionFB( reinterpret_cast<const RpPlaneSector *>(pRpSector)->leftSubTree, pOffset, pSize );
			}
			else
			{
				pOffset->SetY( pOffset->GetY() - pSize->GetY() );

				return GetRpWorldSectorIndexByPositionFB( reinterpret_cast<const RpPlaneSector *>(pRpSector)->rightSubTree, pOffset, pSize );
			}
		}

		case RP_SECTOR_TYPE_PLANE_Z:
		{
			pSize->SetZ( (pSize->GetZ() + 1) >> 1 );

			if( pOffset->GetZ() < pSize->GetZ() )
			{
				return GetRpWorldSectorIndexByPositionFB( reinterpret_cast<const RpPlaneSector *>(pRpSector)->leftSubTree, pOffset, pSize );
			}
			else
			{
				pOffset->SetZ( pOffset->GetZ() - pSize->GetZ() );

				return GetRpWorldSectorIndexByPositionFB( reinterpret_cast<const RpPlaneSector *>(pRpSector)->rightSubTree, pOffset, pSize );
			}
		}

		default:
		{
			return reinterpret_cast<const RpWorldSector *>(pRpSector);
		}
	}
}


//-----------------------------------------------------------------------------
// Name			: GetRpWorldSectorByIndexFB
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const RpWorldSector * GetRpWorldSectorByIndexFB( const RpSector * pRpSector, SLocation3DInt * pOffset, SLocation3DInt * pSize )
{
	switch( pRpSector->type )
	{
		case RP_SECTOR_TYPE_PLANE_X:
		{
			pSize->SetX( (pSize->GetX() + 1) >> 1 );

			if( pOffset->GetX() < pSize->GetX() )
			{
				return GetRpWorldSectorByIndexFB( reinterpret_cast<const RpPlaneSector *>(pRpSector)->leftSubTree, pOffset, pSize );
			}
			else
			{
				pOffset->SetX( pOffset->GetX() - pSize->GetX() );

				return GetRpWorldSectorByIndexFB( reinterpret_cast<const RpPlaneSector *>(pRpSector)->rightSubTree, pOffset, pSize );
			}
		}

		case RP_SECTOR_TYPE_PLANE_Y:
		{
			pSize->SetY( (pSize->GetY() + 1) >> 1 );

			if( pOffset->GetY() < pSize->GetY() )
			{
				return GetRpWorldSectorByIndexFB( reinterpret_cast<const RpPlaneSector *>(pRpSector)->leftSubTree, pOffset, pSize );
			}
			else
			{
				pOffset->SetY( pOffset->GetY() - pSize->GetY() );

				return GetRpWorldSectorByIndexFB( reinterpret_cast<const RpPlaneSector *>(pRpSector)->rightSubTree, pOffset, pSize );
			}
		}

		case RP_SECTOR_TYPE_PLANE_Z:
		{
			pSize->SetZ( (pSize->GetZ() + 1) >> 1 );

			if( pOffset->GetZ() < pSize->GetZ() )
			{
				return GetRpWorldSectorByIndexFB( reinterpret_cast<const RpPlaneSector *>(pRpSector)->leftSubTree, pOffset, pSize );
			}
			else
			{
				pOffset->SetZ( pOffset->GetZ() - pSize->GetZ() );

				return GetRpWorldSectorByIndexFB( reinterpret_cast<const RpPlaneSector *>(pRpSector)->rightSubTree, pOffset, pSize );
			}
		}

		default:
		{
			return reinterpret_cast<const RpWorldSector *>(pRpSector);
		}
	}
}


//-----------------------------------------------------------------------------
// Name			: GetRpWorldSectorIndexByPositionFB
// Desc			: 
// Privilege	: global
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

const RpWorldSector * GetRpWorldSectorIndexByPositionFB( const RpSector * pRpSector, const RwV3d * pPosition, SLocation3DInt * pOffset, SLocation3DInt * pSize )
{
	switch( pRpSector->type )
	{
		case RP_SECTOR_TYPE_PLANE_X:
		{
			const RpPlaneSector *	pPlaneSector = reinterpret_cast<const RpPlaneSector *>(pRpSector);

			pSize->SetX( pSize->GetX() >> 1 );

			if( pPosition->x < pPlaneSector->value )
			{
				return GetRpWorldSectorIndexByPositionFB( pPlaneSector->leftSubTree, pPosition, pOffset, pSize );
			}
			else
			{
				pOffset->SetX( pOffset->GetX() + pSize->GetX() );

				return GetRpWorldSectorIndexByPositionFB( pPlaneSector->rightSubTree, pPosition, pOffset, pSize );
			}
		}

		case RP_SECTOR_TYPE_PLANE_Y:
		{
			const RpPlaneSector *	pPlaneSector = reinterpret_cast<const RpPlaneSector *>(pRpSector);

			pSize->SetY( pSize->GetY() >> 1);

			if( pPosition->y < pPlaneSector->value )
			{
				return GetRpWorldSectorIndexByPositionFB( pPlaneSector->leftSubTree, pPosition, pOffset, pSize );
			}
			else
			{
				pOffset->SetY( pOffset->GetY() + pSize->GetY() );

				return GetRpWorldSectorIndexByPositionFB( pPlaneSector->rightSubTree, pPosition, pOffset, pSize );
			}
		}

		case RP_SECTOR_TYPE_PLANE_Z:
		{
			const RpPlaneSector *	pPlaneSector = reinterpret_cast<const RpPlaneSector *>(pRpSector);

			pSize->SetZ( pSize->GetZ() >> 1 );

			if( pPosition->z < pPlaneSector->value )
			{
				return GetRpWorldSectorIndexByPositionFB( pPlaneSector->leftSubTree, pPosition, pOffset, pSize );
			}
			else
			{
				pOffset->SetZ( pOffset->GetZ() + pSize->GetZ() );

				return GetRpWorldSectorIndexByPositionFB( pPlaneSector->rightSubTree, pPosition, pOffset, pSize );
			}
		}

		default:
		{
			const RpWorldSector *	pWorldSector = reinterpret_cast<const RpWorldSector *>(pRpSector);

			if( IsPointInBBox( pPosition, &(pWorldSector->boundingBox) ) != FALSE )
			{
				return pWorldSector;
			}
			else
			{
				return NULL;
			}
		}
	}
}


//*****************************************************************************
//
// End of File : BaseType.cpp
//
//*****************************************************************************