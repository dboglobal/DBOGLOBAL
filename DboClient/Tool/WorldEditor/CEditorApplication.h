//*****************************************************************************
// File       : CEditodAppilcation.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_EDITOR_APPLICATION_H__
#define __C_EDITOR_APPLICATION_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CSceneManager;
class CInput;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"
#include "CEntityCollision.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define g_pEngineApp					(CEditorApplication::GetInstance())


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef enum EEditMode
{
	E_EDIT_MODE_NONE					= 0,
	E_EDIT_MODE_OBJECT_SELECT			= 1,
	E_EDIT_MODE_OBJECT_MOVE_XZ			= 2,
	E_EDIT_MODE_OBJECT_MOVE_Y			= 3,
	E_EDIT_MODE_OBJECT_ROTATE_X			= 4,
	E_EDIT_MODE_OBJECT_ROTATE_Y			= 5,
	E_EDIT_MODE_OBJECT_ROTATE_Z			= 6,
	E_EDIT_MODE_OBJECT_SCALE_XZ			= 7,
	E_EDIT_MODE_OBJECT_SCALE_Y			= 8,
	E_EDIT_MODE_COUNT
} EEditMode;


typedef enum ERenderMode
{
	E_RENDER_MODE_NONE					= 0,
//	E_RENDER_MODE_POINT					= 1,
	E_RENDER_MODE_WIREFRAME				= 2,
	E_RENDER_MODE_SOLID					= 4,
	E_RENDER_MODE_WIREFRAME_AND_SOLID	= 6
};


typedef enum ECameraMovementMode
{
	E_CAMERA_MOVEMENT_MODE_FREE			= 0,
	E_CAMERA_MOVEMENT_MODE_FIRST		= 1,
	E_CAMERA_MOVEMENT_MODE_TOP			= 2,
	E_CAMERA_MOVEMENT_MODE_THIRD		= 3,
	E_CAMERA_MOVEMENT_MODE_COUNT
} ECameraMovementMode;


typedef enum EPropertySelectMode
{
	E_PROPERTY_SELECT_MODE_NONE					= 0,
	E_PROPERTY_SELECT_MODE_SELECT_ONCE			= 1,
	E_PROPERTY_SELECT_MODE_SELECT_REMAIN		= 2,
	E_PROPERTY_SELECT_MODE_SELECT_SAME_PROPERTY	= 3,
	E_PROPERTY_SELECT_MODE_COUNT
} EPropertySelectMode;


//=============================================================================
// Name       : CEditodAppilcation
// Desc       : 
// Author     : 
//=============================================================================

class CEditorApplication : public CNtlApplication
{
	protected:

		CFixedString					m_Name;

		RtCharset *						m_pCharset;

		CSceneManager *					m_pSceneManager;

		RwRGBA							m_BackgroundColor;
		RwRGBA							m_ForegroundColor;

		RwReal							m_fCurrentTime;
		RwReal							m_fElapsedTime;

		ECameraMovementMode				m_eCameraMovementMode;
		EPropertySelectMode				m_ePropertySelectMode;

		RwReal							m_fEntityMovementSpeed;
		RwReal							m_fEntityRotationSpeed;
		RwReal							m_fEntityScaleSpeed;

		CInput *						m_pInput;

		SItemInfo 						m_ItemSelected;
		SItemInfo 						m_ItemPreviousSelected;

		EEditMode						m_eEditMode;
		ERenderMode						m_eRenderMode;
		RwInt32							m_iSelectOrder;

		SLocation3D						m_PickingOffset;
		CEntityCollision				m_PickingInfo;

		RwV2d							m_DownPositionLeft;
		RwV2d							m_UpPositionLeft;
		RwV2d							m_OffsetLeft;

		RwV2d							m_DownPositionRight;
		RwV2d							m_UpPositionRight;
		RwV2d							m_OffsetRight;

		SLocation3D						m_CameraTopViewLocation;

		RwReal							m_fWheelOffset;

		RwReal							m_bAnimate;

	private:

										CEditorApplication( void );
										~CEditorApplication( void );

	private:

										CEditorApplication( const CEditorApplication & rcFactory );
		const CEditorApplication &		operator=( const CEditorApplication & rcFactory );

	public:

		virtual void					Destroy( void );

		const CFixedString *			GetName( void ) const { return &m_Name; }
		void							SetName( CFixedString * pName ) { m_Name.Assign( *pName ); }

		inline RwReal					GetCurrentAppTime( void ) const { return m_fCurrentTime; }
		inline RwReal					GetElapsedAppTime( void ) const { return m_fElapsedTime; }

		RwBool							OnIdle( void );
		RwBool							OnMouseWheel( RwInt32 iDelta );

		RwBool							SelectItem( EItemType eType, void * pData );
		RwBool							BackupSelectItem( void );
		void							UnselectItem( void );

		const SItemInfo *				GetSelectedItem( void ) const { return &m_ItemSelected; }
		void							UnselectPreviousItem( void );

		ERenderMode						GetRenderMode( void ) const { return m_eRenderMode; }

		inline EEditMode				GetEditMode( void ) const { return m_eEditMode; }
		inline void						SetEditMode( EEditMode eEditMode ) { m_eEditMode = eEditMode; }
		EEditMode						ToggleEditMode( void );

		ECameraMovementMode				GetCameraMovementMode( void ) const { return m_eCameraMovementMode; }
		void							SetCameraMovementMode( ECameraMovementMode eCameraMovementMode );

		EPropertySelectMode				GetPropertySelectMode( void ) const { return m_ePropertySelectMode; }
		void							SetPropertySelectMode( EPropertySelectMode ePropertySelectMode ) { m_ePropertySelectMode = ePropertySelectMode; }

		void							ToggleAnimate( void ) { m_bAnimate = !m_bAnimate; }
		RwBool							IsAnimate( void ) const { return m_bAnimate; }

		// Save & Load

		RwBool							OnFileNew( void );
		RwBool							OnFileLoad( RwChar * pName );
		RwBool							OnFileSave( RwChar * pName );

		RwBool							OnCreateHeightMap( RwBool bLoadFromFile = FALSE );

		virtual RwBool					CalculateVertexColor( SVertexColorGenerationInfo * pInfo );
		RwBool							SetStatusText( RwChar * pText );

	protected:

		virtual void					Init( void );

		virtual RwBool					AttachPlugin( void );

		RwBool							CreateSubSystem( RwUInt32 iWidth, RwUInt32 iHeight, RwBool bZBuffer );
		void							DestroySubSystem( void );

		RwBool							Update( void );
		RwBool							Resize( RwUInt32 iWidth, RwUInt32 iHeight, RwBool bZBuffer );

		// Pick

		RwBool							PickTerrain( void );
		RwBool							PickObject( void );

		// Input

		RwBool							CreateInput( void );
		void							DestroyInput( void );
		RwBool							UpdateInput( void );

		void							ProcessKeyState( void );
		void							ProcessMouseState( void );

		RwV2d *							GetMousePosition( void ) const;

		void							SetCurrentTime( void );
		void							DisplayInfo( void );

		RwBool							OnLButtonDown( void );
		RwBool							OnLButtonUp( void );
		RwBool							OnLButtonDrag( void );
		RwBool							OnLButtonMove( void );

		RwBool							OnLButtonDragModeMoveXZ( void );
		RwBool							OnLButtonDragModeMoveY( void );
		RwBool							OnLButtonDragModeRotateY( void );
		RwBool							OnLButtonDragModeRotateX( void );
		RwBool							OnLButtonDragModeRotateZ( void );
		RwBool							OnLButtonDragModeScaleXZ( void );
		RwBool							OnLButtonDragModeScaleY( void );

		RwBool							OnRButtonDown( void );
		RwBool							OnRButtonUp( void );
		RwBool							OnRButtonDrag( void );
		RwBool							OnRButtonMove( void );

		RwBool							UpdateCameraPosition( SLocation3D * pMovement, SRotation3D * pRotation );

		RwBool							UpdateSelectedItem( void ) const;

		// Command

		RwBool							ExecuteCreateEntityCommand( RwBool bFinal = TRUE );
		RwBool							ExecuteDestroyEntityCommand( RwBool bFinal = TRUE );

		RwBool							ExecuteMoveEntityCommand( RwUInt32 iUniqueID, const SLocation3D * pMovement, RwBool bBeAdded = FALSE, RwBool bFinal = FALSE );
		RwBool							ExecuteRotateEntityCommand( RwUInt32 iUniqueID, const SRotation3D * pRotation, RwBool bBeAdded = FALSE, RwBool bFinal = FALSE );
		RwBool							ExecuteScaleEntityCommand( RwUInt32 iUniqueID, const SLocation3D * pScale, RwBool bBeAdded = FALSE, RwBool bFinal = FALSE );

		RwBool							ExecuteMoveCameraEntityCommand( SLocation3D * pMovement, RwBool bFinal = FALSE );
		RwBool							ExecuteDirectionCameraEntityCommand( SRotation3D * pDirection,  RwBool bFinal = FALSE );
		RwBool							ExecuteRotateCameraEntityCommand( SRotation3D * pRotation,  RwBool bFinal = FALSE );

		ERenderMode						ToggleRenderMode( void );

		RwBool							OnFreeView( void );
		RwBool							OnTopView( void );

	private:

		static CEditorApplication *		m_pInstance;

		static CFixedString 			m_AppPath;
		static CFixedString 			m_FilePath;
		static CFixedString 			m_FileExt;

	protected:

		static RwBool					RwInstanceExPluginAttach( void );

	public:

		static RwBool					CreateInstance( void );
		static void						DestroyInstance( void );
		static CEditorApplication *		GetInstance( void ) { return m_pInstance; }

		static void						SetAppPath( CFixedString & rAppPath ) { m_AppPath.Assign( rAppPath ); }
		static const CFixedString *		GetAppPath( void ) { return &m_AppPath; }

		static void						SetFilePath( CFixedString & rFilePath ) { m_FilePath.Assign( rFilePath ); }
		static const CFixedString *		GetFilePath( void ) { return &m_FilePath; }

		static void						SetFileExt( CFixedString & rFileExt ) { m_FileExt.Assign( rFileExt ); }
		static const CFixedString *		GetFileExt( void ) { return &m_FileExt; }

};


//=============================================================================
// End of Class : CEditodAppilcation
//=============================================================================

#endif //__C_EDITOR_APPLICATION_H__

//*****************************************************************************
//
// End of File : CEditodAppilcation.h
//
//*****************************************************************************