//*****************************************************************************
// File       : SWordNode.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_WORD_NODE_H__
#define __C_WORD_NODE_H__

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

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef enum EWordType
{
	E_WORD_TYPE_NONE	= 0,
	E_WORD_TYPE_STRING	= 1,
	E_WORD_TYPE_INTEGER	= 2,
	E_WORD_TYPE_FLOAT	= 3,
	E_WORD_TYPE_COUNT	= 4
} EWordType;


//=============================================================================
// Name       : SWordNode
// Desc       : 
// Author     : 
//=============================================================================

typedef struct SWordNode
{
	EWordType			eType;
	RwUInt32			iSize;
	void *				pBuffer;

	SWordNode( void ) { Initialize(); }
	~SWordNode( void ) { Destroy(); }

	void Initialize( void ) { eType = E_WORD_TYPE_NONE; iSize = 0; pBuffer = NULL; }

	void Create( RwChar * pData )
	{
		Destroy();

		eType	= E_WORD_TYPE_STRING;
		iSize	= static_cast<RwUInt32>(_tcslen( pData ));
		pBuffer	= reinterpret_cast<void *>(new char[iSize + sizeof( RwChar )]);
		ZeroMemory( pBuffer, iSize + sizeof( RwChar ) );
		memcpy( pBuffer, pData, iSize );
	}

	void Create( RwInt32 iData )
	{
		Destroy();
		
		eType	= E_WORD_TYPE_INTEGER;
		iSize	= sizeof( RwInt32 );
		pBuffer	= reinterpret_cast<void *>(new RwInt32);
		*(reinterpret_cast<RwInt32 *>(pBuffer))	= iData;
	}

	void Create( RwReal fData )
	{
		Destroy();
		
		eType	= E_WORD_TYPE_FLOAT;
		iSize	= sizeof( RwReal );
		pBuffer	= reinterpret_cast<void *>(new RwReal);
		*(reinterpret_cast<RwReal *>(pBuffer)) = fData;
	}

	EWordType GetType( void ) const { return eType; }
	void Destroy( void ) { if( pBuffer != NULL ) delete[] pBuffer; Initialize(); }

	RwBool IsString( void ) const { return ( eType == E_WORD_TYPE_STRING ); }
	RwBool IsInteger( void ) const { return ( eType == E_WORD_TYPE_INTEGER ); }
	RwBool IsFloat( void ) const { return ( eType == E_WORD_TYPE_FLOAT ); }

	const RwChar * GetString( void ) const { return reinterpret_cast<RwChar *>(pBuffer); }
	RwInt32 GetInteger( void ) const { return *(reinterpret_cast<RwInt32 *>(pBuffer)); }
	RwReal GetFloat( void ) const { return *(reinterpret_cast<RwReal *>(pBuffer)); }

} SWordNode;


//=============================================================================
// End of Class : SWordNode
//=============================================================================

#endif //__C_WORD_NODE_H__

//*****************************************************************************
//
// End of File : SWordNode.h
//
//*****************************************************************************