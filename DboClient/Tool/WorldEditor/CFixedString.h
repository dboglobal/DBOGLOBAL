//*****************************************************************************
// File       : CFixedString.h
// Desc       : 
// Begin      : 
// Copyright  : 
// Author     : 
// Update     : 
//*****************************************************************************

#ifndef _C_FIXED_STRING_H_
#define _C_FIXED_STRING_H_

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

//#define _DEBUG_STRING

#define NULL_CHAR									_T( '\0' )
#define NULL_STRING									_T( "\0" )

#define LINE_CHAR									_T( '\n' )
#define LINE_STRING									_T( "\n" )

#define ANSI_INDEX_EXTEND_SET						( 128 )
#define ANSI_INDEX_ALPHABET_UPPERCASE_A				( 65 )
#define ANSI_INDEX_ALPHABET_UPPERCASE_Z				( 90 )
#define ANSI_INDEX_ALPHABET_LOWERCASE_A				( 97 )
#define ANSI_INDEX_ALPHABET_LOWERCASE_Z				( 122 )
#define ANSI_INDEX_OFFSET_UPPERCASE_TO_LOWERCASE	( 32 )

#define FIXED_STRING_SEARCH_FAIL					( UINT( -1 ) )


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------

//=============================================================================
// Name       : CFixedString
// Desc       : 
// Author     : 
//=============================================================================

class CFixedString
{
	protected:

		TCHAR *							m_ptBuffer;
		UINT							m_nLength;
		UINT							m_nKey;

#ifdef _DEBUG_STRING
		bool							m_bMonitor;
#endif //_DEBUG_STRING

	public:

										CFixedString( void );
										CFixedString( const TCHAR * lptSourceString );
										CFixedString( const CFixedString & rcSourceString );

										~CFixedString( void );

	public:

		CFixedString &					operator =( const CFixedString & rcSourceString ) { return Assign( rcSourceString ); }
		CFixedString &					operator =( const TCHAR * lptSourceString ) { return Assign( lptSourceString ); }

		bool							operator ==( const CFixedString & rcCompareString ) const { return IsEqual( rcCompareString ); }
		bool							operator ==( const TCHAR * lptCompareString ) const { return IsEqual( lptCompareString ); }

		bool							operator !=( const CFixedString & rcCompareString ) const { return !IsEqual( rcCompareString ); }
		bool							operator !=( const TCHAR * lptCompareString ) const { return !IsEqual( lptCompareString ); }

										operator LPCTSTR() const { return GetBuffer(); }

		const TCHAR						operator[]( UINT nIndex ) const { return GetAt( nIndex ); }

	public:

		LPCTSTR							GetBuffer( void ) const { return m_ptBuffer; }

		UINT							GetLength( void ) const { return m_nLength; }
		UINT							GetBufferSize( void ) const { return m_nLength * sizeof( TCHAR ); }

		CFixedString &					Assign( const CFixedString & rcSourceString );
		CFixedString &					Assign( const TCHAR * lptCompareString );
		CFixedString &					AssignInteger( long nValue, bool bHexa = false );
		CFixedString &					AssignFloat( float fValue, long nFraction = -1 );

		CFixedString &					Append( const CFixedString & rcSourceString );
		CFixedString &					Append( const TCHAR * lptCompareString );

		CFixedString *					Clone( void ) const;

		const TCHAR						GetAt( UINT nIndex ) const;
		const TCHAR						GetAtFast( UINT nIndex ) const { return m_ptBuffer[nIndex]; }

		void							SetAt( UINT nIndex, TCHAR tLetter );
		void							SetAtFast( UINT nIndex, TCHAR tLetter ) { m_ptBuffer[nIndex] = tLetter; }

		UINT							FindLetter( TCHAR tLetterToFind );
		UINT							FindLetter( TCHAR tLetterToFind, UINT nPositionToBegin, UINT nPositionToEnd );

		bool							IsEqual( const CFixedString & rcCompareString ) const;
		bool							IsEqual( const TCHAR * lptCompareString ) const;

		bool							IsEquivalent( const CFixedString & rcCompareString ) const;
		bool							IsEquivalent( const TCHAR * lptCompareString ) const;

		UINT							GetKey( void ) const { return m_nKey; }

		UINT							ConvertToLowercase( void );
		UINT							ConvertToUppercase( void );

		bool							IsValid( void ) const;

#ifdef _DEBUG_STRING
		void							SetMonitor( bool bMonitor ) { m_bMonitor = bMonitor; }
		bool							IsMonitored( void ) const { return m_bMonitor; }
#endif //_DEBUG_STRING

		bool							ReportState( void );

	protected:

		void							Clear( void );

		void							DestroyBuffer( void );

		void							CreateBuffer( const TCHAR * lptBuffer );
		void							CreateBuffer( const CFixedString & rString );

		void							ExpandBuffer( const TCHAR * lptBuffer );
		void							ExpandBuffer( const CFixedString & rString );

		void							GenerateKey( void );

	public:

		static const TCHAR				END_OF_STRING;
		static const UINT				SEARCH_FAIL;

};


//=============================================================================
// End of Class : CFixedString
//=============================================================================

inline bool IsLetterUppercase( TCHAR tLetter )
{
	return ( ( ( tLetter >= ANSI_INDEX_ALPHABET_UPPERCASE_A ) && ( tLetter >= ANSI_INDEX_ALPHABET_UPPERCASE_Z ) ) ? true : false );
}

inline TCHAR ConvertToUppercase( TCHAR tLetter )
{
	return ( IsLetterUppercase( tLetter ) ? tLetter : tLetter + ANSI_INDEX_OFFSET_UPPERCASE_TO_LOWERCASE );
}

inline TCHAR ConvertToLowercase( TCHAR tLetter )
{
	return ( IsLetterUppercase( tLetter ) ? tLetter - ANSI_INDEX_OFFSET_UPPERCASE_TO_LOWERCASE : tLetter );
}


#endif //_C_FIXED_STRING_H_

//*****************************************************************************
//
// End of File : CFixedString.h
//
//*****************************************************************************