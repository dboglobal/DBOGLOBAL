//*****************************************************************************
// File       : CEntityCubeContainer.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ENTITY_CUBE_CONTAINER_H__
#define __C_ENTITY_CUBE_CONTAINER_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

struct RpWorldSector;
class CEntity;
class CFixedStringList;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"

#include "CWorldSectorEntity.h"
#include "CEntityContainer.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CEntityCubeContainer
// Desc       : 
// Author     : 
//=============================================================================

class CEntityCubeContainer : public CEntityContainer
{
	protected:

		SLocation3DInt					m_Size;

		RwInt32							m_iSizeXZ;
		RwInt32							m_iSizeXYZ;

		CEntity ****					m_ppppMap;

	public:

										CEntityCubeContainer( void ) { Initialize(); }

	virtual 							~CEntityCubeContainer( void ) { Destroy(); }

	private:

										CEntityCubeContainer( const CEntityCubeContainer & rMap );

		const CEntityCubeContainer &	operator=( const CEntityCubeContainer & rMap );

	public:

		virtual RwBool					Create( SEntityContainerProperty * pProperty = NULL );
		virtual void					Destroy( void );

		RwBool							CreateWorldSector( void );

		virtual RwBool					UpdateFrame( void );
		virtual RwBool					RenderFrame( CEntity * pRenderer = NULL );

		virtual RwUInt32				Select( void );
		virtual RwUInt32				Unselect( void );

		virtual RwUInt32				StartAnimation( RwReal fTime = 0.0f );
		virtual RwUInt32				StopAnimation( RwReal fTime = 0.0f );

		virtual EEntityContainerType	GetType( void ) const { return E_ENTITY_CONTAINER_TYPE_CUBE; }

		virtual RwBool					Insert( CEntity * pChild );
		virtual RwBool					Remove( CEntity * pChild );
		virtual void					RemoveAll( RwBool bDeleteAll = TRUE );
		virtual RwBool					Replace( CEntity * pEntityToRemove, CEntity * pEntityToInsert ) { return FALSE;}

		virtual void					SetFlag( RwUInt32 iFlag, RwBool bApplyToChild = FALSE );
		virtual void					SetState( EEntityState eState, RwBool bApplyToChild = FALSE );

		virtual void					SetPosition( const SPosition3D * pPosition, RwBool bAdded = FALSE, RwBool bApplyToChild = FALSE );
		virtual void					SetScale( const SLocation3D * pScale, RwBool bAdded = FALSE, RwBool bApplyToChild = FALSE );

		inline RwInt32					GetSizeXY( void ) const { return m_iSizeXZ; }
		inline RwInt32					GetSizeXYZ( void ) const { return m_iSizeXYZ; }

		inline const SLocation3DInt *	GetMin( void ) const { return m_Property.GetMin(); }
		inline const SLocation3DInt *	GetMax( void ) const { return m_Property.GetMax(); }
		inline const SLocation3DInt *	GetSize( void ) const { return &m_Size; }

		inline void						GetMin( SLocation3DInt * pMin ) const { pMin->Set( GetMin() ); }
		inline void						GetMax( SLocation3DInt * pMax ) const { pMax->Set( GetMax() ); }
		inline void						GetSize( SLocation3DInt * pSize ) const { pSize->Set( &m_Size ); }

		RwBool							IsEmpty( void ) const { return (!m_iSizeXYZ); }
		RwUInt32						GetCount( void ) const { return m_iSizeXYZ; }

		CEntity *						GetWorldSector( RwInt32 iIndexX, RwInt32 iIndexY, RwInt32 iIndexZ ) const;
		CEntity *						GetWorldSector( RwInt32 iIndexX, RwInt32 iIndexZ ) const;		
		CEntity *						GetWorldSector( SLocation3DInt * pLocation ) const;

		virtual CEntity *				SearchByRwInstance( void * pRwInstance );
		virtual RwUInt32				SearchByLocation( const SLocation3D * pLocation, CEntityContainer * pResultList, EEntityType eEntityType );

		virtual RwBool					DetectCollision( CCollisionDetectionInfo * pCollisionInfo );

		RwInt32							Export( CFixedStringList * pStringList, CFixedString * pName );

		virtual CEntity *				GetEntity( RwUInt32 iUniqueID );
		virtual CEntity *				GetAt( RwUInt32 iOrder );

		virtual RwUInt32				SearchEntity( CEntityContainer * pResultContainer, const SEntitySearchCondition * pSearchCondition );

		virtual RwBool					CalculateVertexColor( SVertexColorGenerationInfo * pInfo );

	protected:

		virtual void					Initialize( void );
		
		virtual RwBool					ReportSelfState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );
		virtual RwBool					ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );
};


//=============================================================================
// End of Class : CEntityCubeContainer
//=============================================================================

#endif //__C_ENTITY_CUBE_CONTAINER_H__

//*****************************************************************************
//
// End of File : CEntityCubeContainer.h
//
//*****************************************************************************