//*****************************************************************************
// File       : CFixedStringA.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_FIXED_STRING_A_H___
#define ___C_FIXED_STRING_A_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

#define Get16BitsFromChar( VAL )		(*((const UInt16 *)(VAL)))


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "FixedStringBase.h"


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


//=============================================================================
// Name       : CFixedStringA
// Desc       : 
// Author     : 
//=============================================================================

class CFixedStringA
{
	protected:

		char *							m_pchBuffer;
		size_t							m_nLength;
		UInt32							m_nKey;

#ifdef _DEBUG_STRING_
		bool							m_bMonitor;
#endif //_DEBUG_STRING_

	public:

		inline							CFixedStringA( void ) : m_pchBuffer( const_cast<char *>(reinterpret_cast<const char *>(&END_OF_STRING)) ), m_nLength( 0L ), m_nKey( 0L ) {}
		inline							CFixedStringA( const CFixedStringA & rcSourceString ) : m_pchBuffer( const_cast<char *>(reinterpret_cast<const char *>(&END_OF_STRING)) ), m_nLength( 0L ), m_nKey( 0L ) { Assign( rcSourceString ); }
		inline							CFixedStringA( const CFixedStringW & rcSourceString ) :  m_pchBuffer( const_cast<char *>(reinterpret_cast<const char *>(&END_OF_STRING)) ), m_nLength( 0L ), m_nKey( 0L ) { Assign( rcSourceString ); }
		inline							CFixedStringA( const char * pchSourceString ) : m_pchBuffer( const_cast<char *>(reinterpret_cast<const char *>(&END_OF_STRING)) ), m_nLength( 0L ), m_nKey( 0L ) { Assign( pchSourceString ); }
		inline							CFixedStringA( const wchar_t * pchSourceString ) : m_pchBuffer( const_cast<char *>(reinterpret_cast<const char *>(&END_OF_STRING)) ), m_nLength( 0L ), m_nKey( 0L ) { Assign( pchSourceString ); }
//										CFixedStringA( char * pchFormat, ... );

		inline 							~CFixedStringA( void ) { DestroyBuffer(); }

	public:

		inline CFixedStringA &			operator =( const CFixedStringA & rcSourceString ) { Assign( rcSourceString ); return *this; }
		inline CFixedStringA &			operator =( const CFixedStringW & rcSourceString ) { Assign( rcSourceString ); return *this; }
		inline CFixedStringA &			operator =( const char * pchSourceString ) { Assign( pchSourceString ); return *this; }
		inline CFixedStringA &			operator =( const wchar_t * pchSourceString ) { Assign( pchSourceString ); return *this; }

		inline bool						operator ==( const CFixedStringA & rcCompareString ) const { return IsEqual( rcCompareString ); }
		inline bool						operator ==( const CFixedStringW & rcCompareString ) const { return IsEqual( rcCompareString ); }
		inline bool						operator ==( const char * pchCompareString ) const { return IsEqual( pchCompareString ); }

		inline bool						operator !=( const CFixedStringA & rcCompareString ) const { return !IsEqual( rcCompareString ); }
		inline bool						operator !=( const CFixedStringW & rcCompareString ) const { return !IsEqual( rcCompareString ); }
		inline bool						operator !=( const char * pchCompareString ) const { return !IsEqual( pchCompareString ); }

		inline							operator LPCSTR() const { return GetBuffer(); }
		inline							operator LPSTR() const { return const_cast<LPSTR>(GetBuffer()); }

		inline const char				operator[]( size_t nIndex ) const { return GetAt( nIndex ); }

	public:

		inline LPCSTR					GetBuffer( void ) const { return m_pchBuffer; }
		inline size_t					GetLength( void ) const { return m_nLength; }
		inline size_t					GetBufferSize( void ) const { return GetLength() * sizeof( char ); }

		void							Clear( void );

		void							Assign( const CFixedStringA & rcSourceString );
		void							Assign( const CFixedStringW & rcSourceString );
		void							Assign( const char * pchCompareString );
		void							Assign( const wchar_t * pchCompareString );

		void							Append( const CFixedStringA & rcSourceString );
		void							Append( const CFixedStringW & rcSourceString );
		void							Append( const char * pchCompareString );
		void							Append( const wchar_t * pchCompareString );

		CFixedStringA *					Clone( void ) const;

		CFixedStringA *					CreateSubString( size_t nBegin, size_t nEnd ) const;
		void							CreateSubStringFrom( const CFixedStringA & rcSourceString, size_t nBegin, size_t nEnd );

		size_t							Format( char * pchFormat, ... );
		size_t							AppendFormat( char * pchFormat, ... );

		const char						GetAt( size_t nIndex ) const;
		inline const char				GetAtFast( size_t nIndex ) const { return m_pchBuffer[nIndex]; }

		inline const char				GetFirst( void ) const { return GetAtFast( 0 ); }
		inline const char				GetLast( void ) const { return GetAtFast( Max( m_nLength, size_t( 1 ) ) - 1 ); }

		void							SetAt( size_t nIndex, char chLetter );
		inline void						SetAtFast( size_t nIndex, char chLetter ) { m_pchBuffer[nIndex] = chLetter; }

		size_t							FindLetter( char chLetterToFind ) const;
		size_t							FindLetter( char chLetterToFind, size_t nPositionToBegin, size_t nPositionToEnd ) const;

		bool							IsEqual( const CFixedStringA & rcCompareString ) const;
		bool							IsEqual( const CFixedStringW & rcCompareString ) const;
		bool							IsEqual( const char * pchCompareString ) const;

		bool							IsEqual( const CFixedStringA & rcCompareString, size_t nLength ) const;
		bool							IsEqual( const CFixedStringW & rcCompareString, size_t nLength ) const;
		bool							IsEqual( const char * pchCompareString, size_t nLength ) const;

		bool							IsEquivalent( const CFixedStringA & rcCompareString ) const;
		bool							IsEquivalent( const CFixedStringW & rcCompareString ) const;
		bool							IsEquivalent( const char * pchCompareString ) const;

		bool							IsEquivalent( const CFixedStringA & rcCompareString, size_t nLength ) const;
		bool							IsEquivalent( const CFixedStringW & rcCompareString, size_t nLength ) const;
		bool							IsEquivalent( const char * pchCompareString, size_t nLength ) const;

		inline UInt32					GetKey( void ) const { return m_nKey; }

		size_t							ConvertToLowercase( void );
		size_t							ConvertToUppercase( void );

		Int32							ConvertToInt32( void ) const;
		Float32							ConvertToFloat32( void ) const;

		bool							IsValid( void ) const;

		UInt32							GenerateKey( char * pchBuffer, size_t nLength ) const;

#ifdef _DEBUG_STRING_
		void							SetMonitor( bool bMonitor ) { m_bMonitor = bMonitor; }
		bool							IsMonitored( void ) const { return m_bMonitor; }
#endif //_DEBUG_STRING_

		bool							ReportState( void );

	protected:

		void							DestroyBuffer( void );
		void							ResetBuffer( void );

		void							CreateBuffer( const char * pchBuffer, size_t nLength );
		void							CreateBuffer( const wchar_t * pchBuffer, size_t nLength );

		void							ExpandBuffer( const char * pchBuffer, size_t nLength );
		void							ExpandBuffer( const wchar_t * pchBuffer, size_t nLength );

		Int32							ConvertBinToInt32( bool bPositive, size_t nPosition ) const;
		Int32							ConvertOctToInt32( bool bPositive, size_t nPosition ) const;
		Int32							ConvertHexToInt32( bool bPositive, size_t nPosition ) const;

		inline void						GenerateKey( void ) { m_nKey = GenerateKey( m_pchBuffer, m_nLength ); }

	public:

		static const SVersionInfo		VERSION;

		static const char				END_OF_STRING;
		static const UInt32				SEARCH_FAIL;

		static const CFixedStringA		NULLSTRING;
		static CFixedStringA			SEPARATORS;

		static UINT						CONVERSION_CODE_PAGE;
		static DWORD					CONVERSION_FLAGS;

		static UInt32					m_nLastError;

	public:

		static bool						IsSeparator( char tChar );

		inline static void				SetLastError( UInt32 nErrorCode ) { m_nLastError = nErrorCode; }
		inline static UInt32			GetLastError( void ) { return m_nLastError; }
};

//=============================================================================
// End of Class : CFixedStringA
//=============================================================================

inline bool IsLetterUppercase( char chLetter )
{
	return ( ( ( chLetter >= ANSI_INDEX_ALPHABET_UPPERCASE_A ) && ( chLetter >= ANSI_INDEX_ALPHABET_UPPERCASE_Z ) ) ? true : false );
}

inline char ConvertToUppercase( char chLetter )
{
	return ( ( ( chLetter >= ANSI_INDEX_ALPHABET_UPPERCASE_A ) && ( chLetter >= ANSI_INDEX_ALPHABET_UPPERCASE_Z ) ) ? chLetter : chLetter + ANSI_INDEX_OFFSET_UPPERCASE_TO_LOWERCASE );
}

inline char ConvertToLowercase( char chLetter )
{
	return ( ( ( chLetter >= ANSI_INDEX_ALPHABET_UPPERCASE_A ) && ( chLetter >= ANSI_INDEX_ALPHABET_UPPERCASE_Z ) ) ? chLetter - ANSI_INDEX_OFFSET_UPPERCASE_TO_LOWERCASE : chLetter );
}

inline HANDLE CreateFileSafe( CFixedStringA & rcFileName,
								DWORD dwDesiredAccess,
								DWORD dwShareMode							= FILE_SHARE_DEFAULT,
								LPSECURITY_ATTRIBUTES lpSecurityAttributes	= NULL,
								DWORD dwCreationDisposition					= CREATE_DEFAULT, 
								DWORD dwFlagsAndAttributes					= FILE_ATTRIBUTE_DEFAULT, 
								HANDLE hTemplateFile						= NULL )
{
	return CreateFileSafeA( rcFileName.GetBuffer(), dwDesiredAccess, dwShareMode, lpSecurityAttributes,
							dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );
}

END_NAMESPACE( str )

#endif //___C_FIXED_STRING_A_H___

//*****************************************************************************
//
// End of File : CFixedStringA.h
//
//*****************************************************************************