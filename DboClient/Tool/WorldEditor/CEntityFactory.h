//*****************************************************************************
// File       : CEntityFactory.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ENTITY_FACTORY_H__
#define __C_ENTITY_FACTORY_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CEntityProperty;
class CAtomicEntityProperty;
class CClumpEntityProperty;
class CLightEntityProperty;
class CCameraEntityProperty;
class CWorldEntityProperty;
class CWorldSectorEntityProperty;
class CFogEntityProperty;
class CSkyEntityProperty;
class CMeshEntityProperty;
class CAnimationEntityProperty;

class CEntity;
class CAtomicEntity;
class CClumpEntity;
class CLightEntity;
class CCameraEntity;
class CWorldEntity;
class CWorldSectorEntity;
class CFogEntity;
class CSkyEntity;
class CMeshEntity;
class CAnimationEntity;

struct SEntityContainerProperty;
class CEntityContainer;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#define g_pEntityFactory	CEntityFactory::GetInstance()

#define ENTITY_UNIQUE_ID_AUTO			ENTITY_UNIQUE_ID_NONE

const RwUInt32				ENTITY_UNIQUE_ID_NONE			= 0x00000000;
const RwUInt32				ENTITY_UNIQUE_ID_RESERVED		= 0x0FFFFFFF;
const RwUInt32				ENTITY_UNIQUE_ID_BASE_LIGHT		= 0x00000003;
const RwUInt32				ENTITY_UNIQUE_ID_BASE_CAMERA	= 0x00000004;
const RwUInt32				ENTITY_UNIQUE_ID_BASE_WORLD		= 0x00000005;
const RwUInt32				ENTITY_UNIQUE_ID_ACTIVE_LIGHT	= 0x00000030;
const RwUInt32				ENTITY_UNIQUE_ID_ACTIVE_CAMERA	= 0x00000040;
const RwUInt32				ENTITY_UNIQUE_ID_ACTIVE_WORLD	= 0x00000050;
const RwUInt32				ENTITY_UNIQUE_ID_PROPER_WORLD	= 0x00000500;


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CEntityFactory
// Desc       : 
// Author     : 
//=============================================================================

class CEntityFactory
{
	private:

		CEntityContainer *				m_pRegisteredEntityList;

		RwUInt32						m_iInstanceLatestUniqueID[E_ENTITY_CATEGORY_COUNT][E_ENTITY_TYPE_COUNT];
		RwUInt32						m_iInstanceCount[E_ENTITY_TYPE_COUNT];

	private:

										CEntityFactory( void ) { Init(); }
										~CEntityFactory( void ) { Destroy(); }

	private:

										CEntityFactory( const CEntityFactory & Factory );
		const CEntityFactory &			operator=( const CEntityFactory & Factory );

	public:

		CEntity *						CreateEntity( const CEntityProperty * pEntityProperty, SEntityCreateProperty * pCreateProperty );
		void							DestroyEntity( CEntity ** ppEntityToDestory );

		CEntity *						GetEntity( RwUInt32 iUniqueID ) const;

		CEntityContainer *				CreateEntityContainer( SEntityContainerProperty * pProperty );
		void							DestroyEntityContainer( CEntityContainer ** ppEntityContainerToDestory );

		RwUInt32						GenerateUniqueID( EEntityType eType, EEntityCategory eCategory = E_ENTITY_CATEGORY_NORMAL );

	protected:

		void							Init( void );
		RwBool							Create( void );
		void							Destroy( void );

		RwBool							RegisterEntity( CEntity * pEntityToRegister );
		RwBool							UnregisterEntity( CEntity * pEntityToUnregister );

	private:

		static CEntityFactory *			m_pInstance;

		static const RwUInt32			ENTITY_UNIQUE_ID_OFFSET_BY_TYPE[E_ENTITY_TYPE_COUNT];
		static const RwUInt32			ENTITY_UNIQUE_ID_OFFSET_BY_CATEGORY[E_ENTITY_CATEGORY_COUNT];

	public:

		static RwBool					CreateInstance( void );
		static void						DestroyInstance( void );
		static CEntityFactory *			GetInstance( void );

		static RwBool					IsValidUniqueID( RwUInt32 iUniqueID, EEntityType eType, EEntityCategory eCategory = E_ENTITY_CATEGORY_NORMAL );

};


//=============================================================================
// End of Class : CEntityFactory
//=============================================================================

#endif //__C_ENTITY_FACTORY_H__

//*****************************************************************************
//
// End of File : CEntityFactory.h
//
//*****************************************************************************