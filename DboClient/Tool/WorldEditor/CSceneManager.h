//*****************************************************************************
// File       : CSceneManager.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_SCENE_MANAGER_H__
#define __C_SCENE_MANAGER_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CFixedStringList;
struct SEntityCreateProperty;
class CEntityProperty;
class CEntity;
class CWorldEntity;
class CCameraEntity;
class CLightEntity;
class CEntityContainer;
class CEntityEvent;
class CEntityEventQueue;
class CRenderManager;
class CLightMapManager;
class CCollisionDetectionInfo;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define g_pSceneManager		CSceneManager::GetInstance()


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CSceneManager
// Desc       : 
// Author     : 
//=============================================================================

class CSceneManager
{
	protected:

		CWorldEntity *					m_pBaseWorld;
		CCameraEntity *					m_pActiveCamera;

		CRenderManager *				m_pRenderManager;
		CLightMapManager *				m_pLightMapManager;

		SRasterInfo						m_RasterInfo;

		CEntityContainer *				m_pWorldList;
		CEntityContainer *				m_pCameraList;

		CEntityContainer *				m_pTrashList;

		CEntityContainer *				m_pEntityTable;

		CEntityEventQueue *				m_pEventQueue;

		CFixedString					m_Name;

		RwUInt32						m_iFrameID;

		RwUInt32						m_iRenderCountWorld;
		RwUInt32						m_iRenderCountWorldSector;
		RwUInt32						m_iRenderCountAtomic;
		RwUInt32						m_iRenderCountLight;

	private:

										CSceneManager( void ) { Initialize(); }

										~CSceneManager( void ) { Destroy(); }

	private:

										CSceneManager( const CSceneManager & rManager );

		const CSceneManager &			operator=( const CSceneManager & rManager );

	public:

		RwBool							Create( SRasterInfo * pRasterInfo );
		void							Destroy( void );

		RwBool							UpdateFrame( RwReal fCurrentTime, RwReal fElaspedTime );
		RwBool							RenderFrame( HWND hWnd );
		RwBool							EndFrame( RwUInt32 iFlags = 0 );

		CRenderManager *				GetRenderManager( void ) const { return m_pRenderManager; }
		void							SetRenderManager( CRenderManager * pRenderManager ) { m_pRenderManager = pRenderManager; }

		void							SetRwFrame( const RwFrame * pRwFrame ) const {}
		RwFrame *						GetRwFrame( void ) const { return NULL; }

		CEntity *						CreateEntity( CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty );
		RwBool							DestroyEntity( CEntity ** ppEntityToDestroy );

		RwBool							MoveEntity( RwUInt32 iUniqueID, const SPosition3D * pPosition );

		CEntity *						GetEntity( RwUInt32 iUniqueID ) const;

		inline const CFixedString *		GetName( void ) const { return &m_Name; }
		inline const RwChar *			GetNameBuffer( void ) const { return m_Name.GetBuffer(); }

		inline void						SetName( RwChar * pName ) { m_Name.Assign( pName ); }
		inline void						SetName( const CFixedString &Name ) { m_Name.Assign( Name ); }

		CWorldEntity *					GetBaseWorld( void ) const { return m_pBaseWorld; }
		void							SetBaseWorld( CWorldEntity * pNewBaseWorld ) { m_pBaseWorld = pNewBaseWorld; }

		CCameraEntity *					GetActiveCamera( void ) const { return m_pActiveCamera; }
		void							SetActiveCamera( CCameraEntity * pNewActiveCamera ) { m_pActiveCamera = pNewActiveCamera; }

		const SRasterInfo *				GetRasterInfo( void ) const { return &m_RasterInfo; }
		void							SetRasterInfo( SRasterInfo * pRasterInfo ) { m_RasterInfo.Set( pRasterInfo ); }

		RwUInt32						SearchEntity( CEntityContainer * pResultContainer, const SEntitySearchCondition * pSearchCondition );

		CEntity *						SearchByRwInstance( void * pRwInstance ) const;
		RwUInt32						SearchByLocation( const SLocation3D * pLocation, CEntityContainer * pResultList, EEntityType eEntityType ) const;

		RwBool							DetectCollision( CCollisionDetectionInfo * pCollisionInfo ) const;
		RwBool							GetHeight( SLocation3D * pLocation, RwBool bUseHeightMap = TRUE ) const;

		RwBool							ExecuteCommand( RwChar * pCommandText, RwChar * pResultText = NULL );

		RwInt32							Export( CFixedStringList * pStringList, CFixedString * pName );

		virtual RwBool					ReportState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );

		RwBool							Convert2DTo3D( RwV2d * pLocation2D, SLocation3D * pLocation3D );
		RwBool							Convert2DTo3DForce( RwV2d * pLocation2D, SLocation3D * pLocation3D );

		RwBool							InsertEvent( CEntityEvent * pEvent );

		virtual RwUInt32				StartAnimation( RwReal fTime = 0.0f );
		virtual RwUInt32				StopAnimation( RwReal fTime = 0.0f );

		RwBool							CreateHeightMap( RwReal fPrecision, RwBool bLoadFromFile = FALSE );

		void							IncreaseRenderCountWorld( void ) { ++m_iRenderCountWorld; }
		void							IncreaseRenderCountWorldSector( void ) { ++m_iRenderCountWorldSector; }
		void							IncreaseRenderCountAtomic( void ) { ++m_iRenderCountAtomic; }
		void							IncreaseRenderCountLight( void ) { ++m_iRenderCountLight; }

		RwUInt32						GetRenderCountWorld( void ) const { return m_iRenderCountWorld; }
		RwUInt32						GetRenderCountWorldSector( void ) const { return m_iRenderCountWorldSector; }
		RwUInt32						GetRenderCountAtomic( void ) const { return m_iRenderCountAtomic; }
		RwUInt32						GetRenderCountLight( void ) const { return m_iRenderCountLight; }

		RwBool							CalculateVertexColor( SVertexColorGenerationInfo * pInfo );

	protected:

		void							Initialize( void );

		virtual RwBool					ReportSelfState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );
		virtual RwBool					ReportChildState( CLogMessageManager * pLogMessageManager, RwInt32 iTab = 0 );

		void							ResetRenderCount( void );

		RwBool							CreateLightMapManager( void );
		RwBool							ClearLightMapManager( void );
		void							DestroyLightMapManager( void );

	private:

		static CSceneManager *			m_pInstance;
		static CFixedString 			m_FilePath;
		static CFixedString 			m_FileExt;

	public:

		static RwBool					CreateInstance( void );
		static void						DestroyInstance( void );
		static CSceneManager *			GetInstance( void );

		static void						SetFilePath( CFixedString & rFilePath ) { m_FilePath.Assign( rFilePath ); }
		static const CFixedString *		GetFilePath( void ) { return &m_FilePath; }

		static void						SetFileExt( CFixedString & rFileExt ) { m_FileExt.Assign( rFileExt ); }
		static const CFixedString *		GetFileExt( void ) { return &m_FileExt; }

};


//=============================================================================
// End of Class : CSceneManager
//=============================================================================

#endif //__C_SCENE_MANAGER_H__

//*****************************************************************************
//
// End of File : CSceneManager.h
//
//*****************************************************************************