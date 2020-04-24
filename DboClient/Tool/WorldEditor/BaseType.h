//*****************************************************************************
// File       : BaseType.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __BASE_TYPE_H__
#define __BASE_TYPE_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

#define _COMPILE_OPTION_USE_LIGHT_MAP_

//#define LOG_TRACE

#ifdef LOG_TRACE
	#define BEGIN_FUNCTION( VAL )		NTL_FUNCTION( VAL ); RwChar __Buffer[256]; rwstrcpy( __Buffer, VAL ); g_Log( __Buffer )
	#define END_FUNCTION( VAL )			rwsprintf( __Buffer, RWSTRING( "%s End" ), __Buffer ); g_Log( __Buffer ); NTL_RETURN( VAL )
	#define END_FUNCTION_VOID()			rwsprintf( __Buffer, RWSTRING( "%s End" ), __Buffer ); g_Log( __Buffer ); NTL_RETURNVOID()
#else
	#define BEGIN_FUNCTION( VAL )		NTL_FUNCTION( VAL )
	#define END_FUNCTION( VAL )			NTL_RETURN( VAL )
	#define END_FUNCTION_VOID()			NTL_RETURNVOID()
#endif //LOG_TRACE

//#define NTL_NEW						new
//#define NTL_DELETE					delete
//#define NTL_ARRAY_DELETE				delete[]

#ifndef WC_NO_BEST_FIT_CHARS			// for Unicode/Ansi Converting
	#define WC_NO_BEST_FIT_CHARS		( 0x00000400 )
#endif //WC_NO_BEST_FIT_CHARS


//-----------------------------------------------------------------------------
// Forward Refenence Table
//-----------------------------------------------------------------------------

struct SColorInt;
struct SColorFloat;
class CEntityCollision;
class CEntityCollisionList;
class CEntity;
class CLightEntity;
class CWorldEntity;
class CSceneManager;


//-----------------------------------------------------------------------------
// Standard Template Library Table
//-----------------------------------------------------------------------------

using namespace std;
#include <list>


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "CFixedString.h"

#include "BaseType_Color.h"
#include "BaseType_Position.h"
#include "BaseType_Type.h"
#include "BaseType_VirtualKey.h"
#include "BaseType_RenderWare.h"


//-----------------------------------------------------------------------------
// Const Definition Table
//-----------------------------------------------------------------------------

#define rwID_WORLD_EDITOR				(0xF1)
#define rwID_WORLD_EDITOR_PLUG_IN_ID	(0xF2)



#define BIT_FLAG_MSB_32					(0x80000000)
#define BIT_FLAG_MSB_16					(0x8000)
#define BIT_FLAG_MSB_8					(0x80)
#define BIT_FLAG_LSB					(0x01)

#define RP_SECTOR_TYPE_WORLD			(-1)
#define RP_SECTOR_TYPE_PLANE_X			(0)
#define RP_SECTOR_TYPE_PLANE_Y			(4)
#define RP_SECTOR_TYPE_PLANE_Z			(8)

#ifndef MAX_PATH_LENGTH
	#define MAX_PATH_LENGTH				(1024)
#endif //MAX_PATH_LENGTH

#ifndef MAX_DESC_LENGTH
	#define MAX_DESC_LENGTH				(256)
#endif //MAX_DESC_LENGTH

const RwChar							RWCHAR_NULL						= '\0';
const RwChar							RWCHAR_DOT						= '.';
const RwChar							RWCHAR_SEPARATOR				= '\\';
const RwChar							GLOBAL_TEXTURE_PATH[]			= RWSTRING( "textures" );

const RwReal							ENTITY_MOVEMENT_SPEED			= 0.0008f;
const RwReal							ENTITY_ROTATION_SPEED			= 0.8f;
const RwReal							ENTITY_SCALE_SPEED				= 0.00008f;

const RwReal							CAMERA_ROTATION_SPEED			= 100.0f;
const RwReal							CAMERA_MOVEMENT_SPEED			= 100.0f;

const RwReal							CAMERA_PLANE_INFO_NEAR_DEFAULT	= 1.0f;
const RwReal							CAMERA_PLANE_INFO_FAR_DEFAULT	= 1000.0f;

const RwReal							MOUSE_DRAG_ROTATE_SPEED			= 0.1f;
const RwReal							MOUSE_DRAG_MOVE_SPEED			= 0.001f;


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef struct SRasterInfo
{
	RwInt32		iWidth;
	RwInt32		iHeight;
	RwBool		bZBuffer;

	public:

		SRasterInfo( void )
			: iWidth( 0 ), iHeight( 0 ), bZBuffer( FALSE )
		{
		}

		SRasterInfo( RwInt32 iDefaultWidth, RwInt32 iDefaultHeight, RwBool bDefaultZBuffer )
			: iWidth( iDefaultWidth ), iHeight( iDefaultHeight ), bZBuffer( bDefaultZBuffer )
		{
		}

		inline void Reset( void )
		{
			ZeroMemory( this, sizeof( SRasterInfo ) );
		}

		inline void Set( SRasterInfo * pNewRaster )
		{
			memcpy( this, pNewRaster, sizeof( SRasterInfo ) );
		}

		inline void Set( const SRasterInfo * pNewRaster )
		{
			memcpy( this, pNewRaster, sizeof( SRasterInfo ) );
		}

		inline RwInt32 GetWidth( void ) const { return iWidth; }
		inline RwInt32 GetHeight( void ) const { return iHeight; }
		inline RwBool HasZBuffer( void ) const { return bZBuffer; }

} SRasterInfo;


typedef struct SClipPlane
{
	RwReal		fNear;
	RwReal		fFar;

	public:

		SClipPlane( void )
			: fNear( CAMERA_PLANE_INFO_NEAR_DEFAULT ), fFar( CAMERA_PLANE_INFO_FAR_DEFAULT ) {}

		SClipPlane( RwReal fDefaultNear, RwReal fDefaultFar )
			: fNear( fDefaultNear ), fFar( fDefaultFar ) {}

		inline void Reset( void )
		{
			ZeroMemory( this, sizeof( SClipPlane ) );
		}

		inline const SClipPlane & Assign( const SClipPlane * pSource )
		{
			if( this == pSource )
			{
				return *this;
			}

			memcpy( this, pSource, sizeof( SClipPlane ) );

			return *this;
		}

		inline RwReal GetNear( void ) const { return fNear; }
		inline RwReal GetFar( void ) const { return fFar; }

		inline void SetNear( RwReal fNewNear ) { fNear = fNewNear; }
		inline void SetFar( RwReal fNewFar ) { fFar = fNewFar; }

} SClipPlane;


typedef struct SEntityContainerProperty
{
	EEntityContainerType			eType;
	RwBool							bDeep;

	SLocation3DInt					Min;
	SLocation3DInt					Max;

	CEntity *						pParent;
	CSceneManager *					pSceneManager;

	public:

		SEntityContainerProperty( void )
		: eType( E_ENTITY_CONTAINER_TYPE_LIST ), bDeep( FALSE ), pParent( NULL ), pSceneManager( NULL )
		{
		}

		SEntityContainerProperty( EEntityContainerType eDefaultType, RwBool bDefaultDeep, CEntity * pDefaultParent = NULL, CSceneManager * pDefaultSceneManager = NULL, SLocation3DInt * pDefaultMin = NULL, SLocation3DInt * pDefaultMax = NULL )
		: eType( eDefaultType ), bDeep( bDefaultDeep ), pParent( pDefaultParent ), pSceneManager( pDefaultSceneManager )
		{
			if( pDefaultMin != NULL )
			{
				SetMin( pDefaultMin );
			}

			if( pDefaultMax != NULL )
			{
				SetMax( pDefaultMax );
			}
		}

		inline void Assign( EEntityContainerType eNewType, RwBool bNewDeep, CEntity * pNewParent = NULL, CSceneManager * pNewSceneManager = NULL, SLocation3DInt * pNewMin = NULL, SLocation3DInt * pNewMax = NULL )
		{
			eType			= eNewType; 
			bDeep			= bNewDeep;
			pParent			= pNewParent;
			pSceneManager	= pNewSceneManager;

			SetMin( pNewMin );
			SetMax( pNewMax );
		}

		inline void Assign( SEntityContainerProperty * pNewProperty )
		{
			memcpy( this, pNewProperty, sizeof( SEntityContainerProperty ) );
		}

		inline EEntityContainerType GetType( void ) const { return eType; }
		inline RwBool IsDeep( void ) const { return bDeep; }

		inline CEntity * GetParent( void ) const { return pParent; }
		inline void SetParent( CEntity * pNewParent ) { pParent = pNewParent; }

		inline CSceneManager * GetSceneManager( void ) const { return pSceneManager; }
		inline void SetSceneManager( CSceneManager * pNewSceneManager ) { pSceneManager = pNewSceneManager; }

		inline RwBool IsValid( void ) const
		{
			if( ( eType > E_ENTITY_CONTAINER_TYPE_NONE )
				&& ( eType < E_ENTITY_CONTAINER_TYPE_COUNT ) )
			{
				return TRUE;
			}

			{
				return FALSE;
			}
		}

		inline const SLocation3DInt * GetMin( void ) const { return &Min; }
		inline const SLocation3DInt * GetMax( void ) const { return &Max; }
		inline const void SetMin( SLocation3DInt * pNewMin ) { if( pNewMin != NULL ) memcpy( &Min, pNewMin, sizeof( SLocation3DInt ) ); else ZeroMemory( &Min, sizeof( SLocation3DInt ) ); }
		inline const void SetMax( SLocation3DInt * pNewMax ) { if( pNewMax != NULL ) memcpy( &Max, pNewMax, sizeof( SLocation3DInt ) ); else ZeroMemory( &Min, sizeof( SLocation3DInt ) ); }

} SEntityContainerProperty;


typedef struct SEntityCommandProperty
{
	EEntityCommandType	eType;

	EEntityCommandType	GetType( void ) const { return eType; }

} SEntityCommandProperty;


typedef struct SEntityCreateProperty : public SEntityCommandProperty
{
	RwUInt32			iUniqueID;
	RwUInt32			iParentID;
	EEntityCategory		eCategory;
	RwUInt8				iRenderPriority;
	RwBool				bBase;
	CFixedString		Name;

	SEntityCreateProperty( void ) { eType = ENTITY_COMMAND_TYPE_CREATE; iUniqueID = 0; iParentID = 0; eCategory = E_ENTITY_CATEGORY_NORMAL; iRenderPriority = ENTITY_RENDER_PRIORITY_NONE; bBase = FALSE; }

	inline void SetUniqueID( RwUInt32 iNewUniqueID ) { iUniqueID = iNewUniqueID; }
	inline void SetParentID( RwUInt32 iNewParentID ) { iParentID = iNewParentID; }
	inline void SetCategory( EEntityCategory iNewCategory ) { eCategory = iNewCategory; }
	inline void SetName( RwChar * pNewName ) { Name.Assign( pNewName ); }
	inline void SetName( CFixedString NewName ) { Name.Assign( NewName ); }
	inline void SetBase( RwBool bNewBase ) { bBase = bNewBase; }
	inline void SetRenderPriority( RwUInt8 iNewRenderPriority ) { iRenderPriority = iNewRenderPriority; }

	inline RwUInt32 GetUniqueID( void ) const { return iUniqueID; }
	inline RwUInt32 GetParentID( void ) const { return iParentID; }
	inline EEntityCategory GetCategory( void ) const { return eCategory; }
	inline const CFixedString * GetName( void ) const { return &Name; }
	inline const RwChar * GetNameBuffer( void ) const { return Name.GetBuffer(); }
	inline RwBool IsBase( void ) const { return bBase; }
	inline RwUInt8 GetRenderPriority( void ) const { return iRenderPriority; }

	inline RwBool IsValid( void ) const { return TRUE; }

} SEntityCreateProperty;


typedef struct SEntityDestroyProperty : public SEntityCommandProperty
{
	RwUInt32			iUniqueID;

	RwUInt32			GetUniqueID( void ) const { return iUniqueID; }
	void				SetUniqueID( RwUInt32 iNewUniqueID ) { iUniqueID = iNewUniqueID; }

	SEntityDestroyProperty( void ) { eType = ENTITY_COMMAND_TYPE_DESTROY; }

} SEntityDestroyProperty;


typedef struct SEntityMoveProperty : public SEntityCommandProperty
{
	RwUInt32			iUniqueID;
	SLocation3D			Location;
	RwBool				bAdded;

	RwUInt32			GetUniqueID( void ) const { return iUniqueID; }
	void				SetUniqueID( RwUInt32 iNewUniqueID ) { iUniqueID = iNewUniqueID; }

	const SLocation3D *	GetLocation( void ) const { return &Location; }
	void				SetLocation( const SLocation3D * pNewLocation ) { Location.Set( *pNewLocation ); }

	RwBool				IsAdded( void ) const { return bAdded; }
	void				SetAdded( RwBool bNewAdded ) { bAdded = bNewAdded; }

	SEntityMoveProperty( void ) { eType = ENTITY_COMMAND_TYPE_MOVE; bAdded = FALSE; }

} SEntityMoveProperty;


typedef struct SEntityRotateProperty : public SEntityCommandProperty
{
	RwUInt32			iUniqueID;
	SRotation3D			Rotation;
	RwBool				bAdded;

	RwUInt32			GetUniqueID( void ) const { return iUniqueID; }
	void				SetUniqueID( RwUInt32 iNewUniqueID ) { iUniqueID = iNewUniqueID; }

	const SRotation3D *	GetRotation( void ) const { return &Rotation; }
	void				SetRotation( const SRotation3D * pNewRotation ) { Rotation.Set( *pNewRotation ); }

	RwBool				IsAdded( void ) const { return bAdded; }
	void				SetAdded( RwBool bNewAdded ) { bAdded = bNewAdded; }

	SEntityRotateProperty( void ) { eType = ENTITY_COMMAND_TYPE_ROTATE; bAdded = TRUE; }

} SEntityRotateProperty;


typedef struct SEntityScaleProperty : public SEntityCommandProperty
{
	RwUInt32			iUniqueID;
	SLocation3D			Scale;
	RwBool				bAdded;

	RwUInt32			GetUniqueID( void ) const { return iUniqueID; }
	void				SetUniqueID( RwUInt32 iNewUniqueID ) { iUniqueID = iNewUniqueID; }

	const SLocation3D *	GetScale( void ) const { return &Scale; }
	void				SetScale( const SLocation3D * pNewScale ) { Scale.Set( *pNewScale ); }

	RwBool				IsAdded( void ) const { return bAdded; }
	void				SetAdded( RwBool bNewAdded ) { bAdded = bNewAdded; }

	SEntityScaleProperty( void ) { eType = ENTITY_COMMAND_TYPE_SCALE; bAdded = FALSE; }

} SEntityScaleProperty;


typedef struct SItemInfo
{
	EItemType				eType;
	EEntityType				eEntityType;
	RwUInt32				iUniqueID;
	RwChar					Name[256];
	void *					pData;

	inline RwUInt32			GetUniqueID( void ) const { return iUniqueID; }
	inline EItemType		GetType( void ) const { return eType; }

	inline void				Reset( void ) { ZeroMemory( this, sizeof( SItemInfo ) ); }
	inline RwBool			IsValid( void ) const
	{
		if( eType <= E_ITEM_TYPE_NONE || eType >= E_ITEM_TYPE_COUNT )
		{
			return FALSE;
		}

		return TRUE;
	}

	inline void				Assign( SItemInfo * pItemInfo )
	{
		memcpy( this, pItemInfo, sizeof( SItemInfo ) );
	}

	inline RwBool			IsEqual( SItemInfo * pItemInfo )
	{
		if( pItemInfo == this )
		{
			return TRUE;
		}

		if( pItemInfo->GetType() != GetType() )
		{
			return FALSE;
		}

		if( pItemInfo->GetUniqueID() != GetUniqueID() )
		{
			return FALSE;
		}

		return TRUE;
	}

} SItemInfo;


typedef struct SEntitySearchCondition
{
	EEntityTypeFilter			eTypeFilter;
	EEntityCategoryFilter		eCategoryFilter;

	SEntitySearchCondition( void )
	{
		eTypeFilter		= E_ENTITY_TYPE_FILTER_ALL;
		eCategoryFilter	= E_ENTITY_CATEGORY_FILTER_ALL;
	}

	SEntitySearchCondition( EEntityTypeFilter eDefaultTypeFilter, EEntityCategoryFilter eDefaultCategoryFilter = E_ENTITY_CATEGORY_FILTER_ALL )
	{
		eTypeFilter		= eDefaultTypeFilter;
		eCategoryFilter	= eDefaultCategoryFilter;
	}

	EEntityTypeFilter			GetTypeFilter( void ) const { return eTypeFilter; }
	EEntityCategoryFilter		GetCategoryFilter( void ) const { return eCategoryFilter; }

	void						SetTypeFilter( EEntityTypeFilter eNewTypeFilter ) { eTypeFilter = eNewTypeFilter; }
	void						SetCategoryFilter( EEntityCategoryFilter eNewCategoryFilter ) { eCategoryFilter = eNewCategoryFilter; }

} SEntitySearchCondition;


typedef struct SImediateRenderInfo
{
	RwInt32					iVertexCount;
	RwIm3DVertex *			pVertices;

} SImediateRenderInfo;


typedef struct SEntityEventNode
{
	RwReal					fTime;
	EEntityEventApplyType	eApplyType;	
	void *					pData;

} SEntityEventNode;


typedef struct SRwInstanceEx
{
	CEntity *				pEntity;

	SRwInstanceEx( void ) : pEntity( NULL ) {}
	SRwInstanceEx( CEntity * pNewEntity ) : pEntity( pNewEntity ) {}

	CEntity *				GetEntity( void ) const { return pEntity; }
	void					SetEntity( CEntity * pNewEntity ) { pEntity = pNewEntity; }

} SRwInstanceEx;


typedef struct SVertexColorGenerationInfo
{
	CLightEntity *		pAmbientLight;
	CLightEntity *		pDirectionalLight;
	CLightEntity *		pShadowLight;

} SVertexColorGenerationInfo;


//-----------------------------------------------------------------------------
// Macro & Template Function Table
//-----------------------------------------------------------------------------

#ifndef Swap
template<class T> inline void Swap( T &rtVal1, T &rtVal2 )
{
	T	tTemp = rtVal1;
	rtVal1 = rtVal2;
	rtVal2 = tTemp;

	return;
}
#endif//Swap

/*
#ifndef Min
template<class T> inline T Min( T tVal1, T tVal2 )
{
	return (( tVal1 < tVal2 ) ? tVal1 : tVal2 );
}
#endif//Min


#ifndef Max
template<class T> inline T Max( T tVal1, T tVal2 )
{
	return (( tVal1 > tVal2 ) ? tVal1 : tVal2 );
}
#endif//Max
*/

HANDLE CreateFileSafe( LPCTSTR lptFileName, bool bForRead );


inline RwBool IsValidFilePath( const RwChar * pPathName )
{
	if( pPathName == NULL )
	{
		return FALSE;
	}

	if( rwstrlen( pPathName ) == 0 )
	{
		return FALSE;
	}

	RwStream *		pStream = RwStreamOpen( rwSTREAMFILENAME, rwSTREAMREAD, pPathName );
	if( pStream == NULL )
	{
		return TRUE;
	}

	RwStreamClose( pStream, NULL );

	return TRUE;
}


const RpWorldSector * GetRpWorldSectorByIndexFB( const RpSector * pRpSector, SLocation3DInt * pOffset, SLocation3DInt * pSize );
const RpWorldSector * GetRpWorldSectorIndexByPositionFB( const RpSector * pRpSector, const RwV3d * pPosition, SLocation3DInt * pOffset, SLocation3DInt * pSize );


HANDLE CreateFileSafe( LPCTSTR lptFileName, bool bForRead );


#endif //__BASE_TYPE_H__

//*****************************************************************************
//
// End of File : BaseType.h
//
//*****************************************************************************