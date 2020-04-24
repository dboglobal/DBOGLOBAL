//*****************************************************************************
// File       : CSerializer.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef __C_SERIALIZER_H__
#define __C_SERIALIZER_H__

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
#include "SWordNode.h"


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef vector<SWordNode *>	 						TWordNodeList;
typedef TWordNodeList::iterator						TWordNodeListIterator;

typedef enum EWordState
{
	E_WORD_STATE_NONE				= 0x0000,
	E_WORD_STATE_BEGIN_OF_WORD		= 0x0001,
	E_WORD_STATE_IN_WORD			= 0x0002,
	E_WORD_STATE_END_OF_WORD		= 0x0004,
	E_WORD_STATE_BEGIN_OF_STRING	= 0x0010,
	E_WORD_STATE_IN_STRING			= 0x0020,
	E_WORD_STATE_END_OF_STRING		= 0x0040,
	E_WORD_STATE_BEGIN_OF_VALUE		= 0x0100,
	E_WORD_STATE_IN_VALUE			= 0x0200,
	E_WORD_STATE_END_OF_VALUE		= 0x0400,
	E_WORD_STATE_BEGIN_OF_COMMENT	= 0x1000,
	E_WORD_STATE_IN_COMMENT			= 0x2000,
	E_WORD_STATE_END_OF_COMMENT		= 0x4000
} EWordState;

typedef struct SParseState
{
	EWordState		eWordState;
	RwBool			bFloat;

	RwChar *		pText;
	RwInt32			iTextSize;
	RwInt32			iParsePosition;

	SWordNode *		pParseWord;

	RwInt32			iBeginPosition;
	RwInt32			iEndPosition;
	RwInt32			iLinePosition;
	RwInt32			iWordPosition;

	RwBool			IsEndOfText( void ) const { return ( ( iParsePosition < iTextSize ) ?  FALSE : TRUE ); }
	RwChar			GetCurrentChar( void ) const { return GetChar( iParsePosition ); }
	RwChar			GetNextChar( void ) const { return GetChar( iParsePosition + 1 ); }
	RwChar			GetBeginChar( void ) const { return GetChar( iBeginPosition ); }
	RwChar			GetEndChar( void ) const { return GetChar( iEndPosition ); }
	void			SetState( EWordState NewState ) { eWordState = NewState; }
	EWordState		GetState( void ) const { return eWordState; }

	RwChar			GetChar( RwInt32 iIndex ) const { return ( ( pText != NULL && iIndex < iTextSize ) ? pText[iIndex] : NULL ); }
	void			SetBegin( void ) { iBeginPosition = iParsePosition; }
	void			SetEnd( void ) { iEndPosition = iParsePosition; }
	RwInt32			GetWordLength( void ) const { return (iEndPosition - iBeginPosition + 1); }

} SParseState;


//=============================================================================
// Name       : CSerializer
// Desc       : 
// Author     : 
//=============================================================================

class CSerializer
{
	protected:

		TWordNodeList					m_tList;
		RwInt32							m_iSize;

	public:

										CSerializer( void ) { Initialize(); }
										CSerializer( const CSerializer & rcSerializer );

										~CSerializer( void ) { Destroy(); }

		const CSerializer &				operator =( const CSerializer & rcSerializer );

	public:

		void							Destroy( void );

		RwInt32							GetSize( void ) const { return m_iSize; }
		const SWordNode *				GetWord( RwInt32 iIndex );

		RwInt32							LoadFromFile( RwChar * pFilePath );
		RwInt32							WriteToFile( RwChar * pFilePath );

		RwInt32							LoadFromString( RwChar * pString );

		const RwChar *					GetString( RwInt32 iIndex ) { return GetWord( iIndex )->GetString(); }
		RwInt32							GetInteger( RwInt32 iIndex ) { return GetWord( iIndex )->GetInteger(); }
		RwReal							GetFloat( RwInt32 iIndex ) { return GetWord( iIndex )->GetFloat(); }

		RwInt32							SearchString( RwChar * pString );

		const SWordNode *				SearchStringAndGetWord( RwChar * pString );
		const RwChar *					SearchStringAndGetString( RwChar * pString );
		RwInt32							SearchStringAndGetInteger( RwChar * pString );
		RwReal							SearchStringAndGetFloat( RwChar * pString );

		RwInt32							Report( RwChar * pFilePath );

	protected:

		void							Initialize( void );

		RwBool							AddWordNode( SWordNode * pNode );
		RwBool							ParseText( RwChar * pText, RwInt32 iTextSize );

		RwBool							DetermineState( SParseState * pParseState );
		RwBool							ProcessState( SParseState * pParseState );
};


//=============================================================================
// End of Class : CSerializer
//=============================================================================

#endif //__C_SERIALIZER_H__

//*****************************************************************************
//
// End of File : CSerializer.h
//
//*****************************************************************************