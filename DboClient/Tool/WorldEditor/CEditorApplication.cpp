//*****************************************************************************
// File       : CEditorApplication.cpp
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

#include "CAtomicEntity.h"
#include "CClumpEntity.h"
#include "CLightEntity.h"
#include "CCameraEntity.h"
#include "CWorldEntity.h"
#include "CWorldSectorEntity.h"
#include "CEntityCommand.h"
#include "CGlobalIndicator.h"

#include "CEntityFactory.h"
#include "CSceneManager.h"
#include "CEntityResourceManager.h"
#include "CEntityCollision.h"
#include "CEntityCollisionList.h"
#include "Collision.h"

#include "MainFrm.h"
#include "CInstanceView.h"

#include "CEditorApplication.h"


//-----------------------------------------------------------------------------
// Initialize Table
//-----------------------------------------------------------------------------

CEditorApplication *	CEditorApplication::m_pInstance					= NULL;


//-----------------------------------------------------------------------------
// Name			: CreateInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::CreateInstance( void )
{
	if( m_pInstance == NULL )
	{
		m_pInstance = NTL_NEW CEditorApplication;

		if( m_pInstance != NULL )
		{
			return TRUE;
		}
	}

	CLogMessageManager::SetGlobalErrorFilePath( "GError.txt" );
	CLogMessageManager::SetGlobalLogFilePath( "GLog.txt" );

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name			: DestroyInstance
// Desc			: 
// Privilege	: static public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEditorApplication::DestroyInstance( void )
{
	if( m_pInstance != NULL )
	{
		NTL_DELETE( m_pInstance );
		m_pInstance = NULL;
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: CEditorApplication
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEditorApplication::CEditorApplication()
{
	CGlobalIndicator::CreateInstance();

	Init();

	return;
}


//-----------------------------------------------------------------------------
// Name			: ~CEditorApplication
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

CEditorApplication::~CEditorApplication()
{
	//Destroy();

	CGlobalIndicator::DestroyInstance();

	return;
}


//-----------------------------------------------------------------------------
// Name			: Init
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEditorApplication::Init( void )
{
	m_BackgroundColor.red   = 0;
	m_BackgroundColor.green = 0;
	m_BackgroundColor.blue  = 0;
	m_BackgroundColor.alpha = 255;

	m_ForegroundColor.red   = 255;
	m_ForegroundColor.green = 255;
	m_ForegroundColor.blue  = 255;
	m_ForegroundColor.alpha = 255;

	m_fCurrentTime			= 0.0f;
	m_fElapsedTime			= 0.0f;

	m_fEntityMovementSpeed	= ENTITY_MOVEMENT_SPEED;
	m_fEntityRotationSpeed	= ENTITY_ROTATION_SPEED;
	m_fEntityScaleSpeed		= ENTITY_SCALE_SPEED;

	m_eCameraMovementMode	= E_CAMERA_MOVEMENT_MODE_FREE;

	m_iSelectOrder			= 0;

	m_pSceneManager			= NULL;
	m_pInput				= NULL;

	m_ItemSelected.Reset();
	m_ItemPreviousSelected.Reset();

	m_eEditMode				= E_EDIT_MODE_OBJECT_MOVE_XZ;
	m_eRenderMode			= E_RENDER_MODE_SOLID;
	m_ePropertySelectMode	= E_PROPERTY_SELECT_MODE_SELECT_ONCE;

	m_bAnimate				= TRUE;

	return;
}


//-----------------------------------------------------------------------------
// Name			: Destroy
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEditorApplication::Destroy( void )
{
	BEGIN_FUNCTION( "CEditorApplication::Destroy" );

	DestroySubSystem();

	CNtlApplication::Destroy();

	END_FUNCTION_VOID();
}


//-----------------------------------------------------------------------------
// Name			: AttachPlugin
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::AttachPlugin( void )
{
	BEGIN_FUNCTION( "CEditorApplication::AttachPlugin" );

	// For RpWorld

	if( !RpWorldPluginAttach() )
	{
		END_FUNCTION(FALSE);
	}

	// For Collision Detection

	if( !RpCollisionPluginAttach() )
	{
		END_FUNCTION(FALSE);
	}

	// For Skin Animation

	if( !RtAnimInitialize() )
	{
		END_FUNCTION(FALSE);
	}

	if( !RpHAnimPluginAttach() )
	{
		END_FUNCTION(FALSE);
	}

	if( !RpSkinPluginAttach() )
	{
		END_FUNCTION(FALSE);
	}

	// For UV Animation

	if( !RpMatFXPluginAttach() )
	{
		END_FUNCTION(FALSE);
	}

	if( !RpUVAnimPluginAttach() )
	{
		END_FUNCTION(FALSE);
	}

	// For Static Light Map

	if( !RpRandomPluginAttach() )
	{
		END_FUNCTION(FALSE);
	}

	if( !RpLtMapPluginAttach() )
	{
		END_FUNCTION(FALSE);
	}

	if( !RpPVSPluginAttach() )
	{
		END_FUNCTION(FALSE);
	}

	// For Entity Information

	if( !RwInstanceExPluginAttach() )
	{
		END_FUNCTION(FALSE);
	}

	END_FUNCTION( TRUE );
}


//-----------------------------------------------------------------------------
// Name			: CreateSubSystem
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::CreateSubSystem( RwUInt32 iWidth, RwUInt32 iHeight, RwBool bZBuffer )
{
	CNtlApplication::CreateSubSystem( iWidth, iHeight, bZBuffer );

	API_PLInit();

	CNtlPLPropertyContainer * pSingleton = NTL_NEW CNtlPLPropertyContainer;
	pSingleton->Load( "" );

	if( CSceneManager::CreateInstance() == FALSE )
	{
		Assert( !"CWorldManager::CreateInstance() Failure" );

		return FALSE;
	}

	m_pSceneManager = CSceneManager::GetInstance();

	SRasterInfo		RasterInfo( iWidth, iHeight, bZBuffer );

	if( m_pSceneManager->Create( &RasterInfo ) == FALSE )
	{
		return FALSE;
	}

	if( OnFileNew() == FALSE )
	{
		return FALSE;
	}

	m_pCharset = RtCharsetCreate( &m_ForegroundColor, &m_BackgroundColor );
    if( m_pCharset == NULL )
	{
		return FALSE;
	}

	if( CreateInput() == FALSE )
	{
		return FALSE;
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: DestroySubSystem
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEditorApplication::DestroySubSystem( void )
{
	DestroyInput();

	CSceneManager::DestroyInstance();

	if( m_pCharset != NULL )
    {
		RtCharsetClose();
		RwRasterDestroy( m_pCharset );
		m_pCharset = NULL;
    }

	CNtlPLPropertyContainer::GetInstance()->Destroy(); 
	API_PLTerminate();

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetCurrentTime
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEditorApplication::SetCurrentTime( void )
{
	CNtlTimer::UpdateFrame( m_fFps );

	m_fCurrentTime = CNtlTimer::DXUtil_Timer( CNtlTimer::TIMER_GETAPPTIME );
	m_fElapsedTime = CNtlTimer::DXUtil_Timer( CNtlTimer::TIMER_GETELAPSEDTIME );

	return;
}


//-----------------------------------------------------------------------------
// Name			: OnIdle
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::OnIdle( void )
{
	Update();

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: Update
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::Update( void )
{
	BEGIN_FUNCTION( "CEditorApplication::Update" );

	// Update Time

	SetCurrentTime();

	// Update Input

	UpdateInput();

	// Update Frame

	m_pSceneManager->UpdateFrame( GetCurrentAppTime(), GetElapsedAppTime() );

	// Render Frame

	m_pSceneManager->RenderFrame( GetHWnd() );

	DisplayInfo();

	// Rasterize Scene

	m_pSceneManager->EndFrame();

	UpdateSelectedItem();

	END_FUNCTION( TRUE );
}


//-----------------------------------------------------------------------------
// Name			: Resize
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::Resize( RwUInt32 iWidth, RwUInt32 iHeight, RwBool bZBuffer )
{
	BEGIN_FUNCTION( "CEditorApplication::Resize" );

	CNtlApplication::Resize( iWidth, iHeight, bZBuffer );

	return( TRUE );
}


//-----------------------------------------------------------------------------
// Name			: DisplayInfo
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEditorApplication::DisplayInfo( void )
{
	BEGIN_FUNCTION( "CViewerApplication::DisplayInfo" );

	if( m_pSceneManager->GetActiveCamera() == NULL )
	{
		END_FUNCTION_VOID();
	}

	RwCamera * pCamera = reinterpret_cast<RwCamera *>(m_pSceneManager->GetActiveCamera()->GetRwInstance());

	if( RwCameraBeginUpdate( pCamera ) )
    {
		RwChar			szTextBuffer[256];

		rwsprintf( szTextBuffer, RWSTRING( "Fps      : %.2f" ), CEditorApplication::GetFps() );
		RsCharsetPrint( m_pCharset, szTextBuffer, 0, 1, rsPRINTPOSTOPLEFT );

		rwsprintf( szTextBuffer, RWSTRING( "Size     : %d x %d" ), GetScreenWidth(), GetScreenHeight() );
		RsCharsetPrint( m_pCharset, szTextBuffer, 0, 2, rsPRINTPOSTOPLEFT );

		rwsprintf( szTextBuffer, RWSTRING( "Camera   : %.1f %.1f %.1f, Dir : %.2f %.2f %.2f, Spd : %.1f %.1f" ),
					g_pSceneManager->GetActiveCamera()->GetPosition()->GetLocation()->GetX(),
					g_pSceneManager->GetActiveCamera()->GetPosition()->GetLocation()->GetY(),
					g_pSceneManager->GetActiveCamera()->GetPosition()->GetLocation()->GetZ(),
					g_pSceneManager->GetActiveCamera()->GetDirection()->GetX(),
					g_pSceneManager->GetActiveCamera()->GetDirection()->GetY(),
					g_pSceneManager->GetActiveCamera()->GetDirection()->GetZ(),
					g_pGlobalIndicator->GetCameraMoveSpeedByKeyboard(),
					g_pGlobalIndicator->GetCameraRotateSpeedByKeyboard() );
		RsCharsetPrint( m_pCharset, szTextBuffer, 0, 3, rsPRINTPOSTOPLEFT );

		rwsprintf( szTextBuffer, RWSTRING( "Render   : Wld %d, Sct %d, Atm %d, Lit %d" ),
					g_pSceneManager->GetRenderCountWorld(),
					g_pSceneManager->GetRenderCountWorldSector(),
					g_pSceneManager->GetRenderCountAtomic(), 
					g_pSceneManager->GetRenderCountLight() );
		RsCharsetPrint( m_pCharset, szTextBuffer, 0, 4, rsPRINTPOSTOPLEFT );

		RwCameraEndUpdate( RwCameraGetCurrentCamera() );
	}

	END_FUNCTION_VOID();
}


//-----------------------------------------------------------------------------
// Name			: DisplayInfo
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::UpdateSelectedItem( void ) const
{
	switch( m_ItemSelected.GetType() )
	{
		case E_ITEM_TYPE_ENTITY:
		{
			CEntity *	pEntity = g_pSceneManager->GetEntity( m_ItemSelected.GetUniqueID() );

			if( pEntity == NULL )
			{
				return FALSE;
			}

			return CInstanceView::GetInstance()->UpdateEntity( pEntity );
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: CalculateVertexColor
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::CalculateVertexColor( SVertexColorGenerationInfo * pInfo )
{
	if( g_pSceneManager == NULL )
	{
		return FALSE;
	}

	return g_pSceneManager->CalculateVertexColor( pInfo );
}


//-----------------------------------------------------------------------------
// Name			: PickTerrain
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::PickTerrain( void )
{
	CCollisionDetectionInfo		DetectionInfo;
	DetectionInfo.CreateByPickTerrain( GetMousePosition() );

	if( g_pSceneManager->DetectCollision( &DetectionInfo ) == FALSE )
	{
		return FALSE;
	}

	CEntityCollisionList *	pResultList = DetectionInfo.GetResultList();
	Assert( pResultList != NULL );

	CEntityCollision *		pEntityCollision;
	CEntity	*				pEntity;

	if( pResultList->IsEmpty() == FALSE )
	{
		if( m_iSelectOrder >= pResultList->GetCount() )
		{
			m_iSelectOrder = 0;
		}

		pEntityCollision = pResultList->GetCollisionEntityByOrder( m_iSelectOrder );
		Assert( pEntityCollision != NULL );

		pEntity = pEntityCollision->GetEntity();
		Assert( pEntity != NULL );
		pEntityCollision->SetEntity( pEntity->GetParent() );

		m_PickingInfo.Set( pEntityCollision );

		return TRUE;
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name			: PickObject
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::PickObject( void )
{
	CCollisionDetectionInfo		DetectionInfo;
	DetectionInfo.CreateByPickObject( GetMousePosition() );

	if( g_pSceneManager->DetectCollision( &DetectionInfo ) == FALSE )
	{
		return FALSE;
	}

	CEntityCollisionList *	pResultList = DetectionInfo.GetResultList();
	Assert( pResultList != NULL );

	CEntityCollision *		pEntityCollision;
	CEntity	*				pEntity;

	if( pResultList->IsEmpty() == FALSE )
	{
		if( m_iSelectOrder >= pResultList->GetCount() )
		{
			m_iSelectOrder = 0;
		}

		pEntityCollision = pResultList->GetCollisionEntityByOrder( m_iSelectOrder );
		Assert( pEntityCollision != NULL );

		pEntity = pEntityCollision->GetEntity();
		Assert( pEntity != NULL );
		pEntityCollision->SetEntity( pEntity->GetParent() );

		m_PickingInfo.Set( pEntityCollision );

		return TRUE;
	}

	return FALSE;
}


//-----------------------------------------------------------------------------
// Name			: SelectItem
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::SelectItem( EItemType eType, void * pData )
{
	if( m_ItemSelected.eType == eType )
	{
		switch( eType )
		{
			case E_ITEM_TYPE_PROPERTY:
			{
				CNtlPLProperty * pPlProperty = reinterpret_cast<CNtlPLProperty *>(pData);

				if( rwstrcmp( pPlProperty->GetName(), m_ItemSelected.Name ) == 0 )
				{
					return TRUE;
				}

				break;
			}

			case E_ITEM_TYPE_ENTITY:
			{
				CEntity * pEntity = reinterpret_cast<CEntity *>(pData);

				if( m_ItemSelected.iUniqueID == pEntity->GetUniqueID() )
				{
					return TRUE;
				}
			}
		}
	}

	switch( m_ItemSelected.eType )
	{
		case E_ITEM_TYPE_PROPERTY:
		{
			m_ItemPreviousSelected.Assign( &m_ItemSelected );
			m_ItemPreviousSelected.pData = m_ItemSelected.pData;

			break;
		}

		case E_ITEM_TYPE_ENTITY:
		{
			m_ItemPreviousSelected.Assign( &m_ItemSelected );
			m_ItemPreviousSelected.pData = m_ItemSelected.pData;

			break;
		}

		default:
		{
			m_ItemPreviousSelected.Reset();
		}
	}

	UnselectItem();

	m_ItemSelected.eType = eType;

	switch( eType )
	{
		case E_ITEM_TYPE_PROPERTY:
		{
			CNtlPLProperty * pPlProperty = reinterpret_cast<CNtlPLProperty *>(pData);
			m_ItemSelected.pData = pData;

			if( rwstrcmp( pPlProperty->GetClassID(), RWSTRING( "CNtlPLObjectProperty" ) ) == 0 )
			{
				m_ItemSelected.eEntityType = E_ENTITY_TYPE_CLUMP;

				CNtlPLObjectProperty * pPlObjectProperty = reinterpret_cast<CNtlPLObjectProperty *>(pPlProperty);

				rwstrcpy( m_ItemSelected.Name, pPlObjectProperty->GetName() );

				m_ItemSelected.iUniqueID = pPlObjectProperty->GetId();
			}
			else
			{
				m_ItemSelected.iUniqueID = 0L;
			}

			break;
		}

		case E_ITEM_TYPE_ENTITY:
		{
			CEntity * pEntity = reinterpret_cast<CEntity *>(pData);
			m_ItemSelected.pData = pData;

			m_ItemSelected.iUniqueID = pEntity->GetUniqueID();

			m_ItemSelected.eEntityType = pEntity->GetType();

			rwstrcpy( m_ItemSelected.Name, pEntity->GetNameBuffer() );

			pEntity->Select();

			CInstanceView::GetInstance()->SetEntity( pEntity );
		}
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: UnselectItem
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEditorApplication::UnselectItem( void )
{
	switch( m_ItemSelected.eType )
	{
		case E_ITEM_TYPE_ENTITY:
		{
			CEntity * pEntity = g_pSceneManager->GetEntity( m_ItemSelected.iUniqueID );

			if( pEntity != NULL )
			{
				pEntity->Unselect();
			}

			CInstanceView::GetInstance()->SetEntity( NULL );

			break;
		}
	}

	m_ItemSelected.Reset();
//	m_ItemPreviousSelected.Reset();

	return;
}


//-----------------------------------------------------------------------------
// Name			: UnselectPreviousItem
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEditorApplication::UnselectPreviousItem( void )
{
	m_ItemSelected.Reset();
	m_ItemPreviousSelected.Reset();

	return;
}


//-----------------------------------------------------------------------------
// Name			: BackupSelectItem
// Desc			: 
// Privilege	: public
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::BackupSelectItem( void )
{
	m_ItemSelected.Assign( &m_ItemPreviousSelected );

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: ToggleRenderMode
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

ERenderMode CEditorApplication::ToggleRenderMode( void )
{
	switch( m_eRenderMode )
	{
		case E_RENDER_MODE_WIREFRAME:
		{
			m_eRenderMode = E_RENDER_MODE_SOLID;

			break;
		}

		case E_RENDER_MODE_SOLID:
		{
			m_eRenderMode = E_RENDER_MODE_WIREFRAME_AND_SOLID;

			break;
		}

		case E_RENDER_MODE_WIREFRAME_AND_SOLID:
		{
			m_eRenderMode = E_RENDER_MODE_NONE;

			break;
		}

		default:
		{
			m_eRenderMode = E_RENDER_MODE_WIREFRAME;
		}
	}

	return m_eRenderMode;
}


//-----------------------------------------------------------------------------
// Name			: ToggleEditMode
// Desc			: 
// Privilege	: pubic
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

EEditMode CEditorApplication::ToggleEditMode( void )
{
	switch( GetEditMode() )
	{
		case E_EDIT_MODE_OBJECT_MOVE_XZ:
		{
			SetEditMode( E_EDIT_MODE_OBJECT_MOVE_Y );

			break;
		}

		case E_EDIT_MODE_OBJECT_MOVE_Y:
		{
			SetEditMode( E_EDIT_MODE_OBJECT_MOVE_XZ );

			break;
		}

		case E_EDIT_MODE_OBJECT_ROTATE_X:
		{
			SetEditMode( E_EDIT_MODE_OBJECT_ROTATE_Z );

			break;
		}

		case E_EDIT_MODE_OBJECT_ROTATE_Y:
		{
			SetEditMode( E_EDIT_MODE_OBJECT_ROTATE_X );

			break;
		}

		case E_EDIT_MODE_OBJECT_ROTATE_Z:
		{
			SetEditMode( E_EDIT_MODE_OBJECT_ROTATE_Y );

			break;
		}

		case E_EDIT_MODE_OBJECT_SCALE_XZ:
		{
			SetEditMode( E_EDIT_MODE_OBJECT_SCALE_XZ );

			break;
		}

		case E_EDIT_MODE_OBJECT_SCALE_Y:
		{
			SetEditMode( E_EDIT_MODE_OBJECT_SCALE_Y );

			break;
		}

		default:
		{
			Assert( !"Invalid Edit Mode" );

			SetEditMode( E_EDIT_MODE_OBJECT_MOVE_XZ );
		}
	}

	return GetEditMode();
}


//-----------------------------------------------------------------------------
// Name			: SetCameraMovementMode
// Desc			: 
// Privilege	: pubic
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

void CEditorApplication::SetCameraMovementMode( ECameraMovementMode eCameraMovementMode )
{
	m_eCameraMovementMode = eCameraMovementMode;

	switch( m_eCameraMovementMode )
	{
		case E_CAMERA_MOVEMENT_MODE_FREE:
		{
			OnFreeView();

			break;
		}

		case E_CAMERA_MOVEMENT_MODE_TOP:
		{
			OnTopView();

			break;
		}

		default:
		{
			Assert( !"Invalid Edit Mode" );
		}
	}

	return;
}


//-----------------------------------------------------------------------------
// Name			: SetStatusText
// Desc			: 
// Privilege	: protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::SetStatusText( RwChar * pText )
{
	static CFixedString		CurrText;

	if( CurrText.IsEqual( pText ) == FALSE )
	{
		CurrText.Assign( pText );

		CMainFrame * pFrame = reinterpret_cast<CMainFrame *>(CMainFrame::GetActiveWnd());

		pFrame->SetStatusText( pText );
	}

	return TRUE;
}


//-----------------------------------------------------------------------------
// Name			: RwInstanceExPluginAttach
// Desc			: 
// Privilege	: static protected
// Parameter	: 
// Return		: 
//-----------------------------------------------------------------------------

RwBool CEditorApplication::RwInstanceExPluginAttach( void )
{
    NTL_FUNCTION( "RwInstanceExPluginAttach" );

	if( CAtomicEntity::RwInstanceExPlugInAttach() == FALSE )
	{
		NTL_RETURN( FALSE );
	}

	if( CClumpEntity::RwInstanceExPlugInAttach() == FALSE )
	{
		NTL_RETURN( FALSE );
	}

	if( CWorldSectorEntity::RwInstanceExPlugInAttach() == FALSE )
	{
		NTL_RETURN( FALSE );
	}

	if( CWorldEntity::RwInstanceExPlugInAttach() == FALSE )
	{
		NTL_RETURN( FALSE );
	}

	if( CLightEntity::RwInstanceExPlugInAttach() == FALSE )
	{
		NTL_RETURN( FALSE );
	}

    NTL_RETURN( TRUE );
}


//*****************************************************************************
//
// End of File : CEditorApplication.cpp
//
//*****************************************************************************