//*****************************************************************************
// File       : CFixedStringW.h
// Desc       : 
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___C_FIXED_STRING_W_H___
#define ___C_FIXED_STRING_W_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

#define Get16BitsFromWchar( VAL )		(*((const UInt16 *)(VAL)))


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

class CFixedStringA;


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Type Definition Table
//-----------------------------------------------------------------------------


//=============================================================================
// Name       : CFixedStringW
// Desc       : 
// Author     : 
//=============================================================================

class CFixedStringW
{
	protected:

		wchar_t *						m_pchBuffer;
		size_t							m_nLength;
		UInt32							m_nKey;

#ifdef _DEBUG_STRING_
		bool							m_bMonitor;
#endif //_DEBUG_STRING_

	public:

		inline							CFixedStringW( void ) : m_pchBuffer( const_cast<wchar_t *>(reinterpret_cast<const wchar_t *>(&END_OF_STRING)) ), m_nLength( 0L ), m_nKey( 0L ) {}
		inline							CFixedStringW( const CFixedStringA & rcSourceString ) : m_pchBuffer( const_cast<wchar_t *>(reinterpret_cast<const wchar_t *>(&END_OF_STRING)) ), m_nLength( 0L ), m_nKey( 0L ) { Assign( rcSourceString ); }
		inline							CFixedStringW( const CFixedStringW & rcSourceString ) : m_pchBuffer( const_cast<wchar_t *>(reinterpret_cast<const wchar_t *>(&END_OF_STRING)) ), m_nLength( 0L ), m_nKey( 0L ) { Assign( rcSourceString ); }
		inline							CFixedStringW( const wchar_t * pchSourceString ) : m_pchBuffer( const_cast<wchar_t *>(reinterpret_cast<const wchar_t *>(&END_OF_STRING)) ), m_nLength( 0L ), m_nKey( 0L ) { Assign( pchSourceString ); }
		inline							CFixedStringW( const char * pchSourceString ) : m_pchBuffer( const_cast<wchar_t *>(reinterpret_cast<const wchar_t *>(&END_OF_STRING)) ), m_nLength( 0L ), m_nKey( 0L ) { Assign( pchSourceString ); }
//										CFixedStringW( wchar_t * pchFormat ... );

		inline 							~CFixedStringW( void ) { Assert( m_pchBuffer != NULL ); Clear(); }

	public:

		inline CFixedStringW &			operator =( const CFixedStringW & rcSourceString ) { Assign( rcSourceString ); return *this; }
		inline CFixedStringW &			operator =( const CFixedStringA & rcSourceString ) { Assign( rcSourceString ); return *this; }
		inline CFixedStringW &			operator =( const wchar_t * pchSourceString ) { Assign( pchSourceString ); return *this; }
		inline CFixedStringW &			operator =( const char * pchSourceString ) { Assign( pchSourceString ); return *this; }

		inline bool						operator ==( const CFixedStringW & rcCompareString ) const { return IsEqual( rcCompareString ); }
		inline bool						operator ==( const CFixedStringA & rcCompareString ) const { return IsEqual( rcCompareString ); }
		inline bool						operator ==( const wchar_t * pchCompareString ) const { return IsEqual( pchCompareString ); }

		inline bool						operator !=( const CFixedStringW & rcCompareString ) const { return !IsEqual( rcCompareString ); }
		inline bool						operator !=( const CFixedStringA & rcCompareString ) const { return !IsEqual( rcCompareString ); }
		inline bool						operator !=( const wchar_t * pchCompareString ) const { return !IsEqual( pchCompareString ); }

		inline 							operator LPCWSTR() const { return GetBuffer(); }
		inline							operator LPWSTR() const { return const_cast<LPWSTR>(GetBuffer()); }

		inline const wchar_t			operator[]( size_t nIndex ) const { return GetAt( nIndex ); }

	public:

		inline LPCWSTR					GetBuffer( void ) const { return m_pchBuffer; }
		inline size_t					GetLength( void ) const { return m_nLength; }
		inline size_t					GetBufferSize( void ) const { return m_nLength * sizeof( wchar_t ); }

		void							Clear( void );

		void							Assign( const CFixedStringW & rcSourceString );
		void							Assign( const CFixedStringA & rcSourceString );
		void							Assign( const wchar_t * pchCompareString );
		void							Assign( const char * pchCompareString );

		void							Append( const CFixedStringW & rcSourceString );
		void							Append( const CFixedStringA & rcSourceString );
		void							Append( const wchar_t * pchCompareString );
		void							Append( const char * pchCompareString );

		CFixedStringW *					Clone( void ) const;

		CFixedStringW *					CreateSubString( size_t nBegin, size_t nEnd ) const;
		void							CreateSubStringFrom( const CFixedStringW & rcSourceString, size_t nBegin, size_t nEnd );

		size_t							Format( wchar_t * pchFormat, ... );
		size_t							AppendFormat( wchar_t * pchFormat, ... );

		const wchar_t					GetAt( size_t nIndex ) const;
		inline const wchar_t			GetAtFast( size_t nIndex ) const { return m_pchBuffer[nIndex]; }

		inline const wchar_t			GetFirst( void ) const { return GetAtFast( 0 ); }
		inline const wchar_t			GetLast( void ) const { return GetAtFast( Max( m_nLength, size_t( 1 ) ) - 1 ); }

		void							SetAt( size_t nIndex, wchar_t chLetter );
		inline void						SetAtFast( size_t nIndex, wchar_t chLetter ) { m_pchBuffer[nIndex] = chLetter; }

		size_t							FindLetter( wchar_t chLetterToFind ) const;
		size_t							FindLetter( wchar_t chLetterToFind, size_t nPositionToBegin, size_t nPositionToEnd ) const;

		bool							IsEqual( const CFixedStringW & rcCompareString ) const;
		bool							IsEqual( const CFixedStringA & rcCompareString ) const;
		bool							IsEqual( const wchar_t * pchCompareString ) const;

		bool							IsEqual( const CFixedStringW & rcCompareString, size_t nLength ) const;
		bool							IsEqual( const CFixedStringA & rcCompareString, size_t nLength ) const;
		bool							IsEqual( const wchar_t * pchCompareString, size_t nLength ) const;

		bool							IsEquivalent( const CFixedStringW & rcCompareString ) const;
		bool							IsEquivalent( const CFixedStringA & rcCompareString ) const;
		bool							IsEquivalent( const wchar_t * pchCompareString ) const;

		bool							IsEquivalent( const CFixedStringW & rcCompareString, size_t nLength ) const;
		bool							IsEquivalent( const CFixedStringA & rcCompareString, size_t nLength ) const;
		bool							IsEquivalent( const wchar_t * pchCompareString, size_t nLength ) const;

		inline UInt32					GetKey( void ) const { return m_nKey; }

		size_t							ConvertToLowercase( void );
		size_t							ConvertToUppercase( void );

		Int32							ConvertToInt32( void ) const;
		Float32							ConvertToFloat32( void ) const;

		bool							IsValid( void ) const;

		UInt32							GenerateKey( wchar_t * pchBuffer, size_t nLength );

#ifdef _DEBUG_STRING_
		void							SetMonitor( bool bMonitor ) { m_bMonitor = bMonitor; }
		bool							IsMonitored( void ) const { return m_bMonitor; }
#endif //_DEBUG_STRING_

		bool							ReportState( void );

	protected:

		void							DestroyBuffer( void );
		void							ResetBuffer( void );

		void							CreateBuffer( const wchar_t * pchBuffer, size_t nLength );
		void							CreateBuffer( const char * pchBuffer, size_t nLength );

		void							ExpandBuffer( const wchar_t * pchBuffer, size_t nLength );
		void							ExpandBuffer( const char * pchBuffer, size_t nLength );

		Int32							ConvertBinToInt32( bool bPositive, size_t nPosition ) const;
		Int32							ConvertOctToInt32( bool bPositive, size_t nPosition ) const;
		Int32							ConvertHexToInt32( bool bPositive, size_t nPosition ) const;

		inline void						GenerateKey( void ) { m_nKey = GenerateKey( m_pchBuffer, m_nLength ); }

	public:

		static const SVersionInfo		VERSION;

		static const wchar_t			END_OF_STRING;
		static const UInt32				SEARCH_FAIL;

		static const CFixedStringW		NULLSTRING;
		static CFixedStringW			SEPARATORS;

		static UINT						CONVERSION_CODE_PAGE;
		static DWORD					CONVERSION_FLAGS;

		static UInt32					m_nLastError;

	public:

		static bool						IsSeparator( wchar_t chChar );

		inline static void				SetLastError( UInt32 nErrorCode ) { m_nLastError = nErrorCode; }
		inline static UInt32			GetLastError( void ) { return m_nLastError; }
};

//=============================================================================
// End of Class : CFixedStringW
//=============================================================================

inline bool IsLetterUppercase( wchar_t tLetter )
{
	return ( ( ( tLetter >= ANSI_INDEX_ALPHABET_UPPERCASE_A ) && ( tLetter >= ANSI_INDEX_ALPHABET_UPPERCASE_Z ) ) ? true : false );
}

inline wchar_t ConvertToUppercase( wchar_t tLetter )
{
	return ( ( ( tLetter >= ANSI_INDEX_ALPHABET_UPPERCASE_A ) && ( tLetter >= ANSI_INDEX_ALPHABET_UPPERCASE_Z ) ) ? tLetter : tLetter + ANSI_INDEX_OFFSET_UPPERCASE_TO_LOWERCASE );
}

inline wchar_t ConvertToLowercase( wchar_t tLetter )
{
	return ( ( ( tLetter >= ANSI_INDEX_ALPHABET_UPPERCASE_A ) && ( tLetter >= ANSI_INDEX_ALPHABET_UPPERCASE_Z ) ) ? tLetter - ANSI_INDEX_OFFSET_UPPERCASE_TO_LOWERCASE : tLetter );
}

inline HANDLE CreateFileSafe( CFixedStringW & rcFileName,
								DWORD dwDesiredAccess,
								DWORD dwShareMode							= FILE_SHARE_DEFAULT,
								LPSECURITY_ATTRIBUTES lpSecurityAttributes	= NULL,
								DWORD dwCreationDisposition					= CREATE_DEFAULT, 
								DWORD dwFlagsAndAttributes					= FILE_ATTRIBUTE_DEFAULT, 
								HANDLE hTemplateFile						= NULL )
{
	return CreateFileSafeW( rcFileName.GetBuffer(), dwDesiredAccess, dwShareMode, lpSecurityAttributes,
							dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );
}


END_NAMESPACE( str )

#endif //___C_FIXED_STRING_W_H___

//*****************************************************************************
//
// End of File : CFixedStringW.h
//
//*****************************************************************************