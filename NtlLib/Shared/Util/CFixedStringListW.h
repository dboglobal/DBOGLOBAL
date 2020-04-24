//*****************************************************************************
// File       : CFixedStringListW.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_FIXED_STRING_LIST_W_H___
#define ___C_FIXED_STRING_LIST_W_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "FixedStringBase.h"
#include "CFixedStringW.h"


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( str )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

class CFixedStringW;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

typedef list<CFixedStringW *>	 	TFixedStringListW;
typedef TFixedStringListW::iterator		TFixedStringListIteratorW;
typedef TFixedStringListW::const_iterator	TFixedStringListConstIteratorW;


//=============================================================================
// Name       : CFixedStringListW
// Desc       : 
// Author     : 
//=============================================================================

class CFixedStringListW
{
	protected:

		TFixedStringListW *				m_ptList;
		TFixedStringListIteratorW		m_tCurrentPosition;

		UInt32							m_nStringCount;

	public:

		inline							CFixedStringListW( void ) { Initialize(); }
		inline							CFixedStringListW( const CFixedStringW & rcBuffer, size_t nLetterCount = 0L ) { Initialize(); CreateFrom( rcBuffer, nLetterCount ); }
		inline							CFixedStringListW( const wchar_t * const pchBuffer, size_t nLetterCount = 0L ) { Initialize(); CreateFrom( pchBuffer, nLetterCount ); }

										~CFixedStringListW( void );

	private:

										CFixedStringListW( const CFixedStringListW & rcFixedStringList );

		const CFixedStringListW &		operator=( const CFixedStringListW & rcFixedStringList );

	public:

		inline UInt32					GetCount( void ) const { return m_nStringCount; }
		inline bool						IsEmpty( void ) const { return !m_nStringCount; }

		void							Push( const CFixedStringW * const pcString );
		void							PushFast( CFixedStringW * const pcString );

		void							PushAt( const CFixedStringW * const pcString, UInt32 nPosition );
		void							PushAtFast( CFixedStringW * const pcString, UInt32 nPosition );

		CFixedStringW *					Pop( bool bDestroy = false );
		CFixedStringW *					PopAt( UInt32 nPosition, bool bDestroy = false );
		void							PopAll( bool bDestroy = false );

		const CFixedStringW * const		Get( void ) const;
		CFixedStringW *					GetAt( UInt32 nPosition ) const;

		inline UInt32					FindEqual( const CFixedStringW & rcString ) const { return FindEqual( rcString, 0, m_nStringCount - 1 ); }
		inline UInt32					FindEqual( const CFixedStringW & rcString, UInt32 nBeginIndex ) const { return FindEqual( rcString, nBeginIndex, m_nStringCount - 1 ); }
		UInt32							FindEqual( const CFixedStringW & rcString, UInt32 nBeginIndex, UInt32 nEndIndex ) const;

		inline UInt32					FindEquivalent( const CFixedStringW & rcString ) const { return FindEquivalent( rcString, 0, m_nStringCount - 1 ); }
		inline UInt32					FindEquivalent( const CFixedStringW & rcString, UInt32 nBeginIndex ) const { return FindEquivalent( rcString, nBeginIndex, m_nStringCount - 1 ); }
		UInt32							FindEquivalent( const CFixedStringW & rcString, UInt32 nBeginIndex, UInt32 nEndIndex ) const;

		bool							MoveFront( void );
		bool							MoveBack( void );
		bool							MovePrev( void );
		bool							MoveNext( void );
		bool							MoveAt( UInt32 nPosition );

		size_t							GetBufferSize( void ) const;

		size_t							CreateFrom( const CFixedStringW & rcBuffer, size_t nLetterCount = 0 );
		size_t							CreateFrom( const wchar_t * const pchBuffer, size_t nLetterCount = 0 );
		size_t							CreateFromFile( const CFixedStringW & rcFileName, size_t nLetterCount = 0 );
		size_t							CreateFromFile( const wchar_t * const pchFileName, size_t nLetterCount = 0 );
		size_t							CreateFromFile( HANDLE hFile, size_t nLetterCount = 0 );

		size_t							Serialize( CFixedStringW& rcBuffer, wchar_t chSeparator = L' ' ) const;
		size_t							Serialize( wchar_t * const pchBuffer, size_t nLetterCount, wchar_t chSeparator = L' ' ) const;
		size_t							Serialize( HANDLE hFile, wchar_t chSeparator = L' ', bool bBOM = true ) const;

	protected:

		void							Initialize( void );

		TFixedStringListIteratorW		GetPrevIterator( TFixedStringListIteratorW tSearchPosition ) const;

		EFixedStringListParseState		DetermineCurrentState( EFixedStringListParseState & reCurrentState, wchar_t * const pchPosition );
		bool							EndPreviousState( EFixedStringListParseState ePreviousState, wchar_t * const pchBuffer, size_t nLength );
		EFixedStringListParseState		BeginNextState( EFixedStringListParseState eCurrentState, size_t & rnBeginPosition, size_t nPosition );

		size_t							ParseAndCreate( wchar_t * const pchBuffer, size_t nBufferSize );

	public:

		static const SVersionInfo		VERSION;

		static const UInt32				POSITION_FRONT;
		static const UInt32				POSITION_BACK;

		static CFixedStringW			SEPARATORS;
		static CFixedStringW			PRE_SEPARATORS;
		static CFixedStringW			POST_SEPARATORS;
		static CFixedStringW			TEXT_INDICATORS;
		static CFixedStringW			COMMENT_INDICATOR;
};

//=============================================================================
// End of Class : CFixedStringListW
//=============================================================================

END_NAMESPACE( str )

#endif //___C_FIXED_STRING_LIST_W_H___

//*****************************************************************************
//
// End of File : CFixedStringListW.h
//
//*****************************************************************************