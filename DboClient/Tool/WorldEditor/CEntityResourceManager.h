//*****************************************************************************
// File       : CEntityResourceManager.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ENTITY_RESOURCE_MANAGER_H__
#define __C_ENTITY_RESOURCE_MANAGER_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CEntityProperty;
class CEntityResource;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define g_pResourceManager		CEntityResourceManager::GetInstance()


typedef struct SSectorCreateInfo
{
	RpWorld *		pWorld;
	RwFrame *		pParentFrame;
	RwV3d			SectorSize;

	SSectorCreateInfo( void ) { pWorld = NULL; pParentFrame = NULL; ZeroMemory( &SectorSize, sizeof( RwV3d ) ); }

	RpWorld *		GetWorld( void ) const { return pWorld; }
	RwFrame *		GetParentFrame( void ) const { return pParentFrame; }
	const RwV3d *	GetSectorSize( void ) const { return &SectorSize; }

	void			SetWorld( RpWorld * pNewWorld ) { pWorld = pNewWorld; }
	void			SetParentFrame( RwFrame * pNewParentFrame ) { pParentFrame = pNewParentFrame; }
	void			SetSectorSize( const RwV3d * pNewSectorSize ) { memcpy( &SectorSize, pNewSectorSize, sizeof( RwV3d ) ); }

} SSectorCreateInfo;

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CResourceManager
// Desc       : 
// Author     : 
//=============================================================================

class CEntityResourceManager
{
	private:

										CEntityResourceManager( void ) {}
										~CEntityResourceManager( void ) {}

	private:

										CEntityResourceManager( const CEntityResourceManager & ResourceManager );
		const CEntityResourceManager &	operator=( const CEntityResourceManager & ResourceManager );

	public:

		CEntityResource *				CreateResource( const CEntityProperty * pProperty );
		void							DestroyResource( CEntityResource ** ppResource );

	private:

		static CEntityResourceManager *	m_pInstance;

	public:

		static RwBool					CreateInstance( void );
		static void						DestroyInstance( void );
		static CEntityResourceManager *	GetInstance( void );
};


//=============================================================================
// End of Class : CEntityResourceManager
//=============================================================================

#endif //__C_ENTITY_RESOURCE_MANAGER_H__

//*****************************************************************************
//
// End of File : CEntityResourceManager.h
//
//****************************************************************************