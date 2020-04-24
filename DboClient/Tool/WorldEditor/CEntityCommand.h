//*****************************************************************************
// File       : CEntityCommand.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_ENTITY_COMMAND_H__
#define __C_ENTITY_COMMAND_H__

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CSerializer;
class CEntity;
class CClumpEntity;
class CLightEntity;
class CCameraEntity;
class CWorldEntity;
class CFixedStringList;
class CEntityProperty;


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "BaseType.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CEntityCommand
// Desc       : 
// Author     : 
//=============================================================================

class CEntityCommand
{
	protected:

		EEntityCommandType				m_eType;
		EEntityCommandCategory			m_eCategory;

		CEntityProperty *				m_pPriData;
		SEntityCommandProperty *		m_pExtData;

		void *							m_pResultData;

	private:

										CEntityCommand( void ) { Initialize(); }
										CEntityCommand( const CEntityCommand & rEntityCommand );

										~CEntityCommand( void ) { Destroy(); }

		const CEntityCommand &			operator =( const CEntityCommand & rEntityCommand );

	public:

		RwBool							Parse( RwChar * pCommandString, EEntityCommandCategory eCategory = ENTITY_COMMAND_CATEGORY_NONE );
		RwBool							Parse( CSerializer * pSerializer, RwInt32 & riPosition, EEntityCommandCategory eCategory = ENTITY_COMMAND_CATEGORY_NONE );

		void							Destroy( void );

		EEntityCommandType				GetType( void ) const { return m_eType; }
		EEntityCommandCategory			GetCategory( void ) const { return m_eCategory; }

		RwBool							Execute( RwChar * pResultString = NULL );

		void *							GetResultData( void ) const { return m_pResultData; }

	protected:

		void							Initialize( void );

		RwBool							ParseCreate( CSerializer * pSerializer, RwInt32 & riPosition );
		RwBool							ParseCreateAtomic( CSerializer * pSerializer, RwInt32 & riPosition );
		RwBool							ParseCreateClump( CSerializer * pSerializer, RwInt32 & riPosition );
		RwBool							ParseCreateLight( CSerializer * pSerializer, RwInt32 & riPosition );
		RwBool							ParseCreateCamera( CSerializer * pSerializer, RwInt32 & riPosition );
		RwBool							ParseCreateAmbientLight( CSerializer * pSerializer, RwInt32 & riPosition );
		RwBool							ParseCreateDirectionalLight( CSerializer * pSerializer, RwInt32 & riPosition );
		RwBool							ParseCreatePointLight( CSerializer * pSerializer, RwInt32 & riPosition );
		RwBool							ParseCreateSpotLight( CSerializer * pSerializer, RwInt32 & riPosition );
		RwBool							ParseCreateSoftSpotLight( CSerializer * pSerializer, RwInt32 & riPosition );
		RwBool							ParseCreateWorld( CSerializer * pSerializer, RwInt32 & riPosition );
		RwBool							ParseCreateEmptyWorld( CSerializer * pSerializer, RwInt32 & riPosition );
		RwBool							ParseCreateFog( CSerializer * pSerializer, RwInt32 & riPosition );
		RwBool							ParseCreateSky( CSerializer * pSerializer, RwInt32 & riPosition );
		RwBool							ParseCreateAnimation( CSerializer * pSerializer, RwInt32 & riPosition );

		RwBool							ParseDestroy( CSerializer * pSerializer, RwInt32 & riPosition );

		RwBool							ParseMove( CSerializer * pSerializer, RwInt32 & riPosition );
		RwBool							ParseRotate( CSerializer * pSerializer, RwInt32 & riPosition );
		RwBool							ParseScale( CSerializer * pSerializer, RwInt32 & riPosition );

		RwBool							ParseSector( CSerializer * pSerializer, RwInt32 & riPosition );

		RwBool							ExecuteCommandCreate( RwChar * pResultString = NULL );
		RwBool							ExecuteCommandDestroy( RwChar * pResultString = NULL );
		RwBool							ExecuteCommandMove( RwChar * pResultString = NULL );
		RwBool							ExecuteCommandRotate( RwChar * pResultString = NULL );
		RwBool							ExecuteCommandScale( RwChar * pResultString = NULL );

	public:

		static CEntityCommand *			CreateCommand( void );
		static CEntityCommand *			CreateCommand( RwChar * pCommandString, EEntityCommandCategory eCategory = ENTITY_COMMAND_CATEGORY_NONE );
		static CEntityCommand *			CreateCommand( CSerializer * pSerializer, RwInt32 & riPosition, EEntityCommandCategory eCategory = ENTITY_COMMAND_CATEGORY_NONE );
		static void						DestroyCommand( CEntityCommand ** ppEntityCommand );

		static RwBool					ParseFromFile( RwChar * pFilePath );

	protected:

		static RwBool					m_bInParsing;
		static RwBool					m_bInWaiting;
		static RwBool					m_bInExecuting;

};


//=============================================================================
// End of Class : CEntityCommand
//=============================================================================

#endif //__C_ENTITY_COMMAND_H__

//*****************************************************************************
//
// End of File : CEntityCommand.h
//
//*****************************************************************************