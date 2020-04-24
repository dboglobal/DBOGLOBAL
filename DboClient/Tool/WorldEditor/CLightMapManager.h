//*****************************************************************************
// File       : CLightMapManager.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_LIGHT_MAP_MANAGER_H__
#define __C_LIGHT_MAP_MANAGER_H__

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

#define g_pLightMapManager				CLightMapManager::GetInstance()


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CLightMapManager
// Desc       : 
// Author     : 
//=============================================================================

class CLightMapManager
{
	private:

		RtLtMapAreaLightGroup *			m_pAreaLightGroup;
		RtLtMapLightingSession *		m_pLightingSession;

	protected:

	private:

										CLightMapManager( void );
										~CLightMapManager( void );

	private:

										CLightMapManager( const CLightMapManager & rcFactory );
		const CLightMapManager &		operator=( const CLightMapManager & rcFactory );

	public:

		RwBool							Create( CEntity * pWorld );
		void							Clear( void );
		void							Destroy( void );

		RwBool							RenderFrame( CEntity * pRenderer = NULL );

	protected:

		void							Init( void );

	private:

		static CLightMapManager *		m_pInstance;

	public:

		static CLightMapManager *		CreateInstance( void );
		static void						DestroyInstance( void );
		static CLightMapManager *		GetInstance( void ) { return m_pInstance; }

};


//=============================================================================
// End of Class : CLightMapManager
//=============================================================================

static RpClump * ClumpPreLightClearAll128CB(RpClump *clump, void *data);
static RpAtomic * AtomicPreLightClearAll128CB(RpAtomic *atomic, void *data);


#endif //__C_LIGHT_MAP_MANAGER_H__

//*****************************************************************************
//
// End of File : CLightMapManager.h
//
//*****************************************************************************