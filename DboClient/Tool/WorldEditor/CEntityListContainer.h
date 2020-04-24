//*****************************************************************************
// File       : CEntityListContainer.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ENTITY_LIST_CONTAINER_H__
#define __C_ENTITY_LIST_CONTAINER_H__

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

#include "CEntityContainer.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

class CEntity;


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef list<CEntity *>	 							TEntityList;
typedef TEntityList::iterator						TEntityListIterator;


//=============================================================================
// Name       : CEntityListContainer
// Desc       : 
// Author     : 
//=============================================================================

class CEntityListContainer : public CEntityContainer
{
	protected:

		TEntityList *					m_ptList;

	public:

										CEntityListContainer( void ) { Initialize(); }

		virtual 						~CEntityListContainer( void ) { Destroy(); }

	private:

										CEntityListContainer( const CEntityListContainer & rContainer );

		const CEntityListContainer &	operator =( const CEntityListContainer & rContainer );

	public:

		virtual RwBool					Create( SEntityContainerProperty * pProperty = NULL );
		virtual void					Destroy( void );

		virtual RwBool					UpdateFrame( void );
		virtual RwBool					RenderFrame( CEntity * pRenderer = NULL );

		virtual RwBool					RenderPrimitive( void );

		virtual RwUInt32				Select( void );
		virtual RwUInt32				Unselect( void );

		virtual RwUInt32				StartAnimation( RwReal fTime = 0.0f );
		virtual RwUInt32				StopAnimation( RwReal fTime = 0.0f );

		virtual EEntityContainerType	GetType( void ) const { return E_ENTITY_CONTAINER_TYPE_LIST; }

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
		virtual RwUInt32				SearchByLocation( const SLocation3D * pLocation, CEntityContainer * pResultList, EEntityType eEntityType );

		virtual RwBool					DetectCollision( CCollisionDetectionInfo * pCollisionInfo );

		virtual RwInt32					Export( CFixedStringList * pStringList, CFixedString * pName );

		virtual RwBool					CalculateVertexColor( SVertexColorGenerationInfo * pInfo );

	protected:

		void							Initialize( void );

		virtual RwBool					ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );
};


//=============================================================================
// End of Class : CEntityListContainer
//=============================================================================

#endif //__C_ENTITY_LIST_CONTAINER_H__

//*****************************************************************************
//
// End of File : CEntityListContainer.h
//
//*****************************************************************************