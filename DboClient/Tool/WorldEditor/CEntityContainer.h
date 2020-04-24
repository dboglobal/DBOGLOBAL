//*****************************************************************************
// File       : CEntityContainer.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ENTITY_CONTAINER_H__
#define __C_ENTITY_CONTAINER_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

// #define _COMPILE_OPTION_CHECK_ENTITY_DUPLICATION_
// #define _COMPILE_OPTION_CHECK_CONTAINER_POINTER_


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

struct SEntityContainerProperty;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

class CEntity;
class CFixedStringList;
class CCameraEntity;
class CEntityContainer;
class CCollisionDetectionInfo;
class CnEntityCollisionList;
class CSceneManager;


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CEntityContainer
// Desc       : 
// Author     : 
//=============================================================================

class CEntityContainer
{
	protected:

		RwUInt32						m_iCount;

		SEntityContainerProperty		m_Property;

	public:

										CEntityContainer( void ) {}
	virtual 							~CEntityContainer( void ) {}

	private:

										CEntityContainer( const CEntityContainer & rContainer );

		const CEntityContainer			operator=( const CEntityContainer & rContainer );

	public:

		virtual RwBool					Create( SEntityContainerProperty * pProperty = NULL ) = 0;
		virtual void					Destroy( void ) = 0;

		virtual RwBool					UpdateFrame( void ) { return FALSE; }
		virtual RwBool					RenderFrame( CEntity * pRenderer = NULL ) { return FALSE; }

		virtual RwBool					RenderPrimitive( void ) { return FALSE; }

		virtual RwUInt32				Select( void ) = 0;
		virtual RwUInt32				Unselect( void ) = 0;

		virtual RwUInt32				StartAnimation( RwReal fTime = 0.0f ) = 0;
		virtual RwUInt32				StopAnimation( RwReal fTime = 0.0f ) = 0;

		virtual EEntityContainerType	GetType( void ) const { return E_ENTITY_CONTAINER_TYPE_NONE; }

		RwBool							IsDeep( void ) const { return m_Property.IsDeep(); }

		RwBool							IsEmpty( void ) const { return (!m_iCount); }
		RwUInt32						GetCount( void ) const { return m_iCount; }

		CEntity *						GetParent( void ) const { return m_Property.GetParent(); }
		CSceneManager *					GetSceneManager( void ) const { return m_Property.GetSceneManager(); }

		virtual CEntity *				GetEntity( RwUInt32 iUniqueID ) = 0;
		virtual CEntity *				GetAt( RwUInt32 iOrder ) = 0;

		virtual RwBool					Insert( CEntity * pEntityToInsert ) = 0;
		virtual RwBool					Remove( CEntity * pEntityToRemove ) = 0;
		virtual void					RemoveAll( RwBool bDeleteAll = TRUE ) = 0;
		virtual RwBool					Replace( CEntity * pEntityToRemove, CEntity * pEntityToInsert ) = 0;

		virtual void					SetFlag( RwUInt32 iFlag, RwBool bApplyToChild = FALSE ) = 0;
		virtual void					SetState( EEntityState eState, RwBool bApplyToChild = FALSE ) = 0;

		virtual void					SetPosition( const SPosition3D * pPosition, RwBool bAdded = FALSE, RwBool bChildren = FALSE ) = 0;
		virtual void					SetScale( const SLocation3D * pScale, RwBool bAdded = FALSE, RwBool bChildren = FALSE ) = 0;

		virtual RwUInt32				SearchEntity( CEntityContainer * pResultContainer, const SEntitySearchCondition * pSearchCondition ) = 0;

		virtual CEntity *				SearchByRwInstance( void * pRwInstance ) = 0;
		virtual RwUInt32				SearchByLocation( const SLocation3D * pLocation, CEntityContainer * pResultList, EEntityType eEntityType ) = 0;

		virtual RwBool					DetectCollision( CCollisionDetectionInfo * pCollisionInfo ) = 0;

		virtual RwInt32					Export( CFixedStringList * pStringList, CFixedString * pName ) = 0;

		virtual RwBool					ReportState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );

		virtual RwBool					CalculateVertexColor( SVertexColorGenerationInfo * pInfo ) { Assert( !"Not Supported" ); return FALSE; }

	protected:

		virtual void					Initialize( void );

		const SEntityContainerProperty *	GetProperty( void ) const { return &m_Property; }

		virtual RwBool					DestroyEntity( CEntity * pEntityToDestroy );

		virtual RwBool					ReportSelfState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );
		virtual RwBool					ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 ) = 0;

};


//=============================================================================
// End of Class : CEntityContainer
//=============================================================================

#endif //__C_ENTITY_CONTAINER_H__

//*****************************************************************************
//
// End of File : CEntityContainer.h
//
//*****************************************************************************