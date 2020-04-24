//*****************************************************************************
// File       : CFixedStringListA.h
// Desc       :
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_FIXED_STRING_LIST_A_H___
#define ___C_FIXED_STRING_LIST_A_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "FixedStringBase.h"
#include "CFixedStringA.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( str )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CFixedStringA;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef list<CFixedStringA *>	 	TFixedStringListA;
typedef TFixedStringListA::iterator		TFixedStringListIteratorA;
typedef TFixedStringListA::const_iterator	TFixedStringListConstIteratorA;


//=============================================================================
// Name       : CFixedStringListA
// Desc       : 
// Author     : 
//=============================================================================

class CFixedStringListA
{
	protected:

		TFixedStringListA *				m_ptList;
		TFixedStringListIteratorA		m_tCurrentPosition;

		UInt32							m_nStringCount;

	public:

		inline							CFixedStringListA( void ) { Initialize(); }
		inline							CFixedStringListA( const CFixedStringA & rcBuffer, size_t nLetterCount = 0L ) { Initialize(); CreateFrom( rcBuffer, nLetterCount ); }
		inline							CFixedStringListA( const char * const pchBuffer, size_t nLetterCount = 0L ) { Initialize(); CreateFrom( pchBuffer, nLetterCount ); }

										~CFixedStringListA( void );

	private:

		inline							CFixedStringListA( const CFixedStringListA & rcFixedStringList );

		const CFixedStringListA &		operator=( const CFixedStringListA & rcFixedStringList );

	public:

		inline UInt32					GetCount( void ) const { return m_nStringCount; }
		inline bool						IsEmpty( void ) const { return !m_nStringCount; }

		void							Push( const CFixedStringA * const pcString );
		void							PushFast( CFixedStringA * const pcString );

		void							PushAt( const CFixedStringA * const pcString, UInt32 nPosition );
		void							PushAtFast( CFixedStringA * const pcString, UInt32 nPosition );

		CFixedStringA *					Pop( bool bDestroy = false );
		CFixedStringA *					PopAt( UInt32 nPosition, bool bDestroy = false );
		void							PopAll( bool bDestroy = false );

		const CFixedStringA * const		Get( void ) const;
		CFixedStringA *					GetAt( UInt32 nPosition ) const;

		inline UInt32					FindEqual( const CFixedStringA & rcString ) const { return FindEqual( rcString, 0, m_nStringCount - 1 ); }
		inline UInt32					FindEqual( const CFixedStringA & rcString, UInt32 nBeginIndex ) const { return FindEqual( rcString, nBeginIndex, m_nStringCount - 1 ); }
		UInt32							FindEqual( const CFixedStringA & rcString, UInt32 nBeginIndex, UInt32 nEndIndex ) const;

		inline UInt32					FindEquivalent( const CFixedStringA & rcString ) const { return FindEquivalent( rcString, 0, m_nStringCount - 1 ); }
		inline UInt32					FindEquivalent( const CFixedStringA & rcString, UInt32 nBeginIndex ) const { return FindEquivalent( rcString, nBeginIndex, m_nStringCount - 1 ); }
		UInt32							FindEquivalent( const CFixedStringA & rcString, UInt32 nBeginIndex, UInt32 nEndIndex ) const;

		bool							MoveFront( void );
		bool							MoveBack( void );
		bool							MovePrev( void );
		bool							MoveNext( void );
		bool							MoveAt( UInt32 nPosition );

		size_t							GetBufferSize( void ) const;

		size_t							CreateFrom( const CFixedStringA & rcBuffer, size_t nLetterCount = 0 );
		size_t							CreateFrom( const char * const pchBuffer, size_t nLetterCount = 0 );
		size_t							CreateFromFile( const CFixedStringA & rcFileName, size_t nLetterCount = 0 );
		size_t							CreateFromFile( const char * const pchFileName, size_t nLetterCount = 0 );
		size_t							CreateFromFile( HANDLE hFile, size_t nLetterCount = 0 );

		size_t							Serialize( CFixedStringA & rcBuffer, char chSeparator = ' ' ) const;
		size_t							Serialize( char * const pchBuffer, size_t nLetterCount, char chSeparator = ' ' ) const;
		size_t							Serialize( HANDLE hFile, char chSeparator = ' ', bool bBOM = true ) const;

	protected:

		void							Initialize( void );

		TFixedStringListIteratorA		GetPrevIterator( TFixedStringListIteratorA tSearchPosition ) const;

		EFixedStringListParseState		DetermineCurrentState( EFixedStringListParseState & reCurrentState, char * pchPosition );
		bool							EndPreviousState( EFixedStringListParseState ePreviousState, char * const pchBuffer, size_t nLength );
		EFixedStringListParseState		BeginNextState( EFixedStringListParseState eCurrentState, size_t & rnBeginPosition, size_t nPosition );

		size_t							ParseAndCreate( char * const pchBuffer, size_t nBufferSize );

	public:

		static const SVersionInfo		VERSION;

		static const UInt32				POSITION_FRONT;
		static const UInt32				POSITION_BACK;

		static CFixedStringA			SEPARATORS;
		static CFixedStringA			PRE_SEPARATORS;
		static CFixedStringA			POST_SEPARATORS;
		static CFixedStringA			TEXT_INDICATORS;
		static CFixedStringA			COMMENT_INDICATOR;
};

//=============================================================================
// End of Class : CFixedStringListA
//=============================================================================

END_NAMESPACE( str )

#endif //___C_FIXED_STRING_LIST_A_H___

//*****************************************************************************
//
// End of File : CFixedStringListA.h
//
//*****************************************************************************