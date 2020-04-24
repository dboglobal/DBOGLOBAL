//*****************************************************************************
// File       : CEntity.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ENTITY_H__
#define __C_ENTITY_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

struct RwObject;

class CFixedStringList;
class CEntityProperty;
class CEntityResource;
class CEntityContainer;
class CCollisionDetectionInfo;
class CLogMessageManager;
class CRenderManager;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"

#include "CFixedString.h"

#include "CEntityProperty.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef list<RwUInt32>	 				TEntityUniqueIDList;
typedef TEntityUniqueIDList::iterator	TEntityUniqueIDListIterator;


#define ENTITY_FLAG_NONE				(0x00000000)
#define ENTITY_FLAG_VISIBLE				(0x10000000)
#define ENTITY_FLAG_MOVABLE				(0x20000000)
#define ENTITY_FLAG_GLOBAL				(0x00100000)
#define ENTITY_FLAG_USE_LIGHT_MAP		(0x01000000)
#define ENTITY_FLAG_TERRAIN				(0x00001000)
#define ENTITY_FLAG_SKY					(0x00002000)
#define ENTITY_FLAG_ALL					(0xFFFFFFFF)


#define ENTITY_MODIFIED_NONE			(0x00000000)
#define ENTITY_MODIFIED_POSITION		(0x10000000)
#define ENTITY_MODIFIED_SCALE			(0x20000000)
#define ENTITY_MODIFIED_MATRIX			(0xF0000000)
#define ENTITY_MODIFIED_ANIMATION		(0x02000000)
#define ENTITY_MODIFIED_FLAG			(0x00100000)
#define ENTITY_MODIFIED_STATE			(0x00200000)
#define ENTITY_MODIFIED_SELECT			(0x00400000)
#define ENTITY_MODIFIED_HIERARCHY		(0x01000000)
#define ENTITY_MODIFIED_VERTEX_COLOR	(0x00010000)
#define ENTITY_MODIFIED_ALL				(0xFFFFFFFF)


//=============================================================================
// Name       : CEntity
// Desc       : 
// Author     : 
//=============================================================================

class CEntity
{
	protected:

		RwUInt32						m_iUniqueID;

		CFixedString					m_sName;
		EEntityState					m_eState;
		EEntityCategory					m_eCategory;
		CEntityProperty *				m_pProperty;
		CEntityResource *				m_pResource;

		CEntity *						m_pParent;
		TEntityUniqueIDList *			m_pLinkedParent;
		CEntityContainer *				m_pChildren[E_ENTITY_CATEGORY_COUNT];

		RwUInt8							m_iRenderPriority;

		RwSphere						m_sBounding;

		RwBool							m_bSelected;
		RwBool							m_iModified;

		RwUInt32						m_iFrameID;
		RwUInt32						m_iFlag;

		RwInt32							m_iReferenceCount;

	public:

										CEntity( void );
										CEntity( RwInt32 iUniqueID ) : m_iUniqueID( iUniqueID ) {}
										CEntity( const CEntity & rEntity );

		virtual							~CEntity( void ) {}

		const CEntity &					operator =( const CEntity & rEntity );

	public:

		virtual RwBool					Create( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty );
		virtual RwBool					PostCreate( void );

		virtual void					Destroy( void );
		virtual void					PreDestroy( void );

		CEntityProperty *				GetProperty( void ) const { return m_pProperty; }

		inline RwUInt32					GetUniqueID( void ) const { return m_iUniqueID; }
		inline RwUInt32					GetParentID( void ) const { return ( ( m_pParent == NULL ) ? 0L : m_pParent->GetUniqueID() ); }

		inline EEntityType				GetType( void ) const { return (( m_pProperty != NULL ) ? m_pProperty->GetType() : E_ENTITY_TYPE_NONE); }
		inline EEntityCategory			GetCategory( void ) const { return m_eCategory; }

		inline RwInt32					GetReferenceCount( void ) const { return m_iReferenceCount; }
		inline void						IncreaseReferenceCount( void ) { ++m_iReferenceCount; }
		inline void						DecreaseReferenceCount( void ) { --m_iReferenceCount; }

		RwBool							LinkParent( RwUInt32 iParentID );
		RwBool							RedirectParent( RwBool bLinkCurrentParent = TRUE );
		RwBool							IsLinkedParent( RwUInt32 iParentID );
		void							ResetLinkedParent( void );

		inline const CFixedString *		GetName( void ) const { return &m_sName; }
		inline const RwChar *			GetNameBuffer( void ) const { return m_sName.GetBuffer(); }

		inline void						SetName( const RwChar * pName ) { m_sName.Assign( pName ); }
		inline void						SetName( const CFixedString &Name ) { m_sName.Assign( Name ); }

		inline virtual void				SetModified( RwUInt32 iModified = ENTITY_MODIFIED_POSITION ) { m_iModified |= iModified; }
		inline virtual void				ResetModified( RwUInt32 iModified = ENTITY_MODIFIED_ALL ) { m_iModified &= ~iModified; }
		inline RwBool					IsModified( RwUInt32 iModified = ENTITY_MODIFIED_POSITION ) const { return ( ( m_iModified & iModified) ? TRUE : FALSE ); }
		inline RwUInt32					GetModified( void ) const { return m_iModified; }

		virtual void					SetState( EEntityState eState, RwBool bApplyToChild = FALSE );
		inline EEntityState				GetState( void ) const { return m_eState; }

		virtual void					SetFlag( RwUInt32 iFlag, RwBool bApplyToChild = FALSE );
		inline RwUInt32					GetFlag( void ) const { return m_iFlag; }

		inline RwBool					GetUseLightMap( void ) const { return ( ( m_iFlag & ENTITY_FLAG_USE_LIGHT_MAP ) ? TRUE : FALSE ); }
		inline void						SetUseLightMap( RwBool bHasLightMap ) { m_iFlag = ( ( bHasLightMap != FALSE ) ? ( m_iFlag | ENTITY_FLAG_USE_LIGHT_MAP ) : ( m_iFlag & ~ENTITY_FLAG_USE_LIGHT_MAP ) ); }

		inline RwBool					IsMovable( void ) const { return ( ( m_iFlag & ENTITY_FLAG_MOVABLE ) ? TRUE : FALSE ); }
		inline void						SetMovable( RwBool bMovable ) { m_iFlag = ( ( bMovable != FALSE ) ? ( m_iFlag | ENTITY_FLAG_MOVABLE ) : ( m_iFlag & ~ENTITY_FLAG_MOVABLE ) ); }

		inline RwBool					IsVisible( void ) const { return ( ( m_iFlag & ENTITY_FLAG_VISIBLE ) ? TRUE : FALSE ); }
		inline void						SetVisible( RwBool bVisible ) { m_iFlag = ( ( bVisible != FALSE ) ? ( m_iFlag | ENTITY_FLAG_VISIBLE ) : ( m_iFlag & ~ENTITY_FLAG_VISIBLE ) ); }

		inline RwBool					IsGlobal( void ) const { return ( ( m_iFlag & ENTITY_FLAG_GLOBAL ) ? TRUE : FALSE ); }
		inline void						SetGlobal( RwBool bGlobal ) { m_iFlag = ( ( bGlobal != FALSE ) ? ( m_iFlag | ENTITY_FLAG_GLOBAL ) : ( m_iFlag & ~ENTITY_FLAG_GLOBAL) ); }

		inline RwBool					IsTerrain( void ) const { return ( ( m_iFlag & ENTITY_FLAG_TERRAIN ) ? TRUE : FALSE ); }
		inline void						SetTerrain( RwBool bTerrain ) { m_iFlag = ( ( bTerrain != FALSE ) ? ( m_iFlag | ENTITY_FLAG_TERRAIN ) : ( m_iFlag & ~ENTITY_FLAG_TERRAIN) ); }

		inline RwBool					IsSky( void ) const { return ( ( m_iFlag & ENTITY_FLAG_SKY ) ? TRUE : FALSE ); }
		inline void						SetSky( RwBool bSky ) { m_iFlag = ( ( bSky != FALSE ) ? ( m_iFlag | ENTITY_FLAG_SKY ) : ( m_iFlag & ~ENTITY_FLAG_SKY) ); }

		inline void						SetFrameID( RwUInt32 iFrameID ) { m_iFrameID = iFrameID; }
		inline RwUInt32					GetFrameID( void ) const { return m_iFrameID; }

		inline RwBool					HasPosition( void ) const { return (( m_pProperty != NULL ) ? m_pProperty->HasPosition() : FALSE); }
		inline RwBool					HasLocation( void ) const { return (( m_pProperty != NULL ) ? m_pProperty->HasLocation() : FALSE); }
		inline RwBool					HasRotation( void ) const { return (( m_pProperty != NULL ) ? m_pProperty->HasRotation() : FALSE); }
		inline virtual RwBool			HasScale( void ) const { return (( m_pProperty != NULL ) ? m_pProperty->HasScale() : FALSE); }

		inline virtual const SPosition3D *	GetPosition( void ) const { return (( m_pProperty != NULL ) ? m_pProperty->GetPosition() : &(SPosition3D::POSITION_DEFAULT)); }
		virtual void					SetPosition( const SPosition3D * pPosition, RwBool bAdded = FALSE, RwBool bChildren = FALSE );

		inline virtual const SLocation3D *	GetScale( void ) const { return (( m_pProperty != NULL ) ? m_pProperty->GetScale() : &(SLocation3D::SCALE_DEFAULT)); }
		virtual void					SetScale( const SLocation3D * pScale, RwBool bAdded = FALSE, RwBool bChildren = FALSE );

		virtual CEntity *				SearchByRwInstance( void * pRwInstance ) const;
		virtual RwUInt32				SearchByLocation( const SLocation3D * pLocation, CEntityContainer * pResultList, EEntityType eEntityType ) const;

		void *							GetRwInstance( void ) const { return const_cast<void *>(GetConstRwInstance()); }
		const void *					GetConstRwInstance( void ) const;

		virtual void					SetRwFrame( const RwFrame * pRwFrame ) const {}
		virtual RwFrame *				GetRwFrame( void ) const { return NULL; }

		virtual RwBool					UpdateFrame( void );
		virtual RwBool					RenderFrame( CEntity * pRenderer = NULL );
		virtual RwBool					RenderPrimitive( void ) { return TRUE; }

		virtual RwUInt32				Select( void );
		virtual RwUInt32				Unselect( void );
		RwBool							IsSelected( void ) const { return m_bSelected; }

		virtual RwUInt32				StartAnimation( RwReal fTime = 0.0f );
		virtual RwUInt32				StopAnimation( RwReal fTime = 0.0f );

		RwBool							IsRoot( void ) const { return ( ( GetParent() == NULL ) ? TRUE : FALSE ); }
		const CEntity *					GetRoot( void ) const { return ( ( GetParent() == NULL ) ? this : GetParent()->GetRoot() ); }

		void							SetParent( CEntity * pParent );
		void							SetParentForce( CEntity * pParent ) { m_pParent = pParent; }

		CEntity *						GetParent( void ) const { return m_pParent; }

		virtual RwBool					Attach( CEntity * pChild ) { return FALSE; }
		virtual RwBool					Detach( CEntity * pChild ) { return FALSE; }

		const CEntity *					GetParentWorld( void ) const;

		RwUInt32						SearchEntity( CEntityContainer * pResultContainer, const SEntitySearchCondition * pSearchCondition );

		virtual RwBool					DetectCollision( CCollisionDetectionInfo * pCollisionInfo ) const;

		virtual RwBool					IsValid( void ) const;

		virtual RwBool					IsPointInBounding( const SLocation3D * pLocation ) const { return FALSE; }

		const RwSphere *				GetBounding( void ) const { return &m_sBounding; }

		virtual RwInt32					Export( CFixedStringList * pStringList, CFixedString * pName );


		virtual RwBool					ReportState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );

		const CFixedString *			GetTypeName( void ) const;
		const CFixedString *			GetCategoryName( void ) const;

		RwBool							Invalidate( void ) const;

		virtual RwBool					CalculateVertexColor( SVertexColorGenerationInfo * pInfo ) { return FALSE; }

	protected:

		virtual void					Initialize( void );

		virtual void					SetDefaultName( void );

		RwBool							DetermineVisiblilty( void ) const { return TRUE; }

		virtual RwBool					SetParameter( void );

		virtual void					InitializeChildren( void );
		virtual void					DestroyChildren( void );

		virtual RwBool					InsertChild( CEntity * pChild );
		virtual RwBool					RemoveChild( CEntity * pChild );

		virtual void					CalculateBounding( void ) {}

		RwBool							ExportHeader( CFixedStringList * pStringList, CFixedString * pName ) const;
		virtual RwInt32					ExportChild( CFixedStringList * pStringList, CFixedString * pName );

		virtual RwBool					ReportSelfState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );
		virtual RwBool					ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );

		RwBool							InsertRenderManager( void );

		virtual RwBool					SetPlugInData( void ) { return FALSE; }
		virtual RwBool					ResetPlugInData( void ) { return FALSE; }

	protected:

		static const CFixedString		ENTITY_CATEGORY_NAME[E_ENTITY_CATEGORY_COUNT];

	public:

		static const CFixedString *		GetEntityCategoryName( EEntityCategory eCategory );

};


//=============================================================================
// End of Class : CEntity
//=============================================================================

#endif //__C_ENTITY_H__

//*****************************************************************************
//
// End of File : CEntity.h
//
//*****************************************************************************