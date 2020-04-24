//*****************************************************************************
// File       : CGlobalIndicator.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_GLOBAL_INDICATOR_H__
#define __C_GLOBAL_INDICATOR_H__

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


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define g_pGlobalIndicator					CGlobalIndicator::GetInstance()


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CGlobalIndicator
// Desc       : 
// Author     : 
//=============================================================================

class CGlobalIndicator
{
	private:

		RwReal							m_fCameraMoveSpeedByKeyboard;
		RwReal							m_fCameraRotateSpeedByKeyboard;
		RwReal							m_fCameraMoveSpeedByMouse;
		RwReal							m_fCameraRotateSpeedByMouse;

		RwChar							m_TextBuffer[16][256];

	protected:

	private:

										CGlobalIndicator( void );
										~CGlobalIndicator( void );

	private:

										CGlobalIndicator( const CGlobalIndicator & rcFactory );
		const CGlobalIndicator &		operator=( const CGlobalIndicator & rcFactory );

	public:

		void							Destroy( void );

		RwReal							GetCameraMoveSpeedByKeyboard( void ) const { return m_fCameraMoveSpeedByKeyboard; }
		void							SetCameraMoveSpeedByKeyboard( RwReal fCameraMoveSpeedByKeyboard ) { m_fCameraMoveSpeedByKeyboard = fCameraMoveSpeedByKeyboard; }

		RwReal							GetCameraRotateSpeedByKeyboard( void  ) const { return m_fCameraRotateSpeedByKeyboard; }
		void							SetCameraRotateSpeedByKeyboard( RwReal fCameraRotateSpeedByKeyboard ) { m_fCameraRotateSpeedByKeyboard = fCameraRotateSpeedByKeyboard; }

		RwReal							GetCameraMoveSpeedByMouse( void  ) const { return m_fCameraMoveSpeedByMouse; }
		void							SetCameraMoveSpeedByMouse( RwReal fCameraMoveSpeedByMouse ) { m_fCameraMoveSpeedByMouse = fCameraMoveSpeedByMouse; }

		RwReal							GetCameraRotateSpeedByMouse( void  ) const { return m_fCameraRotateSpeedByMouse; }
		void							SetCameraRotateSpeedByMouse( RwReal fCameraRotateSpeedByMouse ) { m_fCameraRotateSpeedByMouse = fCameraRotateSpeedByMouse; }

		const RwChar *					GetTextBuffer( RwUInt32 iIndex ) const { return reinterpret_cast<const RwChar *>(m_TextBuffer[iIndex]); }
		void							SetTextBuffer( RwUInt32 iIndex, RwChar * pText ) const { rwstrcpy( const_cast<RwChar *>(reinterpret_cast<const RwChar *>(m_TextBuffer[iIndex])), pText ); }

	protected:

		void							Init( void );

	private:

		static CGlobalIndicator *		m_pInstance;

	public:

		static RwBool					CreateInstance( void );
		static void						DestroyInstance( void );
		static CGlobalIndicator *		GetInstance( void ) { return m_pInstance; }

};


//=============================================================================
// End of Class : CGlobalIndicator
//=============================================================================

#endif //__C_GLOBAL_INDICATOR_H__

//*****************************************************************************
//
// End of File : CGlobalIndicator.h
//
//*****************************************************************************