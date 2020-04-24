//*****************************************************************************
// File       : CEntityHashTableContainer.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ENTITY_HASH_TABLE_CONTAINER_H__
#define __C_ENTITY_HASH_TABLE_CONTAINER_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"

#include "CHashTable.h"

#include "CEntityContainer.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

class CEntity;


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CEntityHashTableContainer
// Desc       : 
// Author     : 
//=============================================================================

class CEntityHashTableContainer : public CEntityContainer
{
	protected:

		CHashTable *					m_pcHash;

	public:

										CEntityHashTableContainer( void ) { Initialize(); }

		virtual 						~CEntityHashTableContainer( void ) { Destroy(); }

	private:

										CEntityHashTableContainer( const CEntityHashTableContainer & rContainer );

		const CEntityHashTableContainer &	operator =( const CEntityHashTableContainer & rContainer );

	public:

		virtual RwBool					Create( SEntityContainerProperty * pProperty = NULL );
		virtual void					Destroy( void );

		virtual RwBool					UpdateFrame( void );
		virtual RwBool					RenderFrame( CEntity * pRenderer = NULL );

		virtual RwUInt32				Select( void );
		virtual RwUInt32				Unselect( void );

		virtual RwUInt32				StartAnimation( RwReal fTime = 0.0f );
		virtual RwUInt32				StopAnimation( RwReal fTime = 0.0f );

		virtual EEntityContainerType	GetType( void ) const { return E_ENTITY_CONTAINER_TYPE_HASH_TABLE; }

		virtual CEntity *				GetEntity( RwUInt32 iUniqueID );
		virtual CEntity *				GetAt( RwUInt32 iOrder );

		virtual RwBool					Insert( CEntity * pEntityToInsert );
		virtual RwBool					Remove( CEntity * pEntityToRemove );
		virtual void					RemoveAll( RwBool bDeleteAll = TRUE );
		virtual RwBool					Replace( CEntity * pEntityToRemove, CEntity * pEntityToInsert );

		virtual void					SetFlag( RwUInt32 iFlag, RwBool bApplyToChild = FALSE );
		virtual void					SetState( EEntityState eState, RwBool bApplyToChild = FALSE );

		virtual void					SetPosition( const SPosition3D * pPosition, RwBool bAdded = FALSE, RwBool bApplyToChild = FALSE );
		virtual void					SetScale( const SLocation3D * pScale, RwBool bAdded = FALSE, RwBool bApplyToChild = FALSE );

		virtual RwUInt32				SearchEntity( CEntityContainer * pResultContainer, const SEntitySearchCondition * pSearchCondition );

		virtual CEntity *				SearchByRwInstance( void * pRwInstance );
		virtual RwUInt32				SearchByLocation( const SLocation3D * pLocation, CEntityContainer * pResultHashTable, EEntityType eEntityType );

		virtual RwBool					DetectCollision( CCollisionDetectionInfo * pCollisionInfo );

		virtual RwInt32					Export( CFixedStringList * pStringList, CFixedString * pName );

	protected:

		void							Initialize( void );

		virtual RwBool					ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );
};


//=============================================================================
// End of Class : CEntityHashTableContainer
//=============================================================================

#endif //__C_ENTITY_HASH_TABLE_CONTAINER_H__

//*****************************************************************************
//
// End of File : CEntityHashTableContainer.h
//
//*****************************************************************************