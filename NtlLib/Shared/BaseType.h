//*****************************************************************************
// File       : BaseType.h
// Desc       : Base Type Header
// Author     : June C. Kim (cynis@hotmail.com)
// Update     : 2007. 1. 18
//*****************************************************************************

#ifndef ___BASE_TYPE_H___
#define ___BASE_TYPE_H___

//-----------------------------------------------------------------------------
// Pre-Compiler Option Table
//-----------------------------------------------------------------------------

#define BEGIN_NAMESPACE( NAME )					namespace NAME {
#define END_NAMESPACE( NAME )					} using namespace NAME;
//#define END_NAMESPACE( NAME )					}

#define USE_NAMESPACE( NAME )					using namespace NAME;
#define USE_NAMESPACE_ELEMENT( NAME, ELEMENT )	using NAME::ELEMENT;
#define USE_STD_ELEMENT( ELEMENT )				using std::ELEMENT;

#define down_cast								reinterpret_cast
#define up_cast									reinterpret_cast


//-----------------------------------------------------------------------------
// Name Space Table
//-----------------------------------------------------------------------------

BEGIN_NAMESPACE( bas )


//-----------------------------------------------------------------------------
// Type Forward Definition Table
//-----------------------------------------------------------------------------

typedef signed char				Int8;
typedef unsigned char			UInt8;
typedef signed short			Int16;
typedef unsigned short			UInt16;
typedef signed long				Int32;
typedef unsigned long			UInt32;

typedef float					Float32;
typedef double					Float64;

// I don't use Boolean2 instead of bool, because it may be confused with BOOL.
// In bool case, ( A == !B ) == ( A != B ).
// But in BOOL case, ( A == !B ) ==/!= ( A != B ).
// So, if you have to use BOOL,
// express ( A != FALSE ) instead of ( A == TRUE ),
// and ( A == FALSE ) instead of ( A != TRUE ).


//-----------------------------------------------------------------------------
// Header Table
//-----------------------------------------------------------------------------

#include "NtlSharedCommon.h"

#include <tchar.h>


//-----------------------------------------------------------------------------
// Constant Table
//-----------------------------------------------------------------------------

#ifndef ERROR_DATATYPE_MISMATCH
	#define ERROR_DATATYPE_MISMATCH	( 1629 )
#endif //ERROR_DATATYPE_MISMATCH

#ifndef ERROR_UNSUPPORTED_TYPE
	#define ERROR_UNSUPPORTED_TYPE	( 1630 )
#endif //ERROR_UNSUPPORTED_TYPE


#ifndef VK_ENTER
	#define VK_ENTER		VK_RETURN
#endif //VK_ENTER

#ifndef VK_ESC
	#define VK_ESC			VK_ESCAPE
#endif //VK_ESC

#ifndef VK_PAGEUP
	#define VK_PAGEUP		VK_PRIOR
#endif //VK_PGUP

#ifndef VK_PAGEDOWN
	#define VK_PAGEDOWN		VK_NEXT
#endif //VK_PAGEDOWN

#ifndef VK_CONSOLE
	#define VK_CONSOLE		VK_OEM_3
#endif //VK_CONSOLE

#ifndef VK_BACKSPACE
	#define VK_BACKSPACE	VK_BACK
#endif //VK_BACKSPACE


//-----------------------------------------------------------------------------
// Type Table
//-----------------------------------------------------------------------------

#ifndef SIZES
typedef struct SIZES
{
	short			sx;
	short			sy;
} SIZES;
#endif SIZES


#ifndef SVersionInfo

#define INT_TYPE_16			( 16 )
#define INT_TYPE_32			( 32 )
#define INT_TYPE_64			( 64 )

#define BLD_TYPE_DEBUG		( 1 )
#define BLD_TYPE_RELEASE	( 2 )
#define BLD_TYPE_TEST		( 8 )

#define CHR_TYPE_ANSI		( 1 )		// MBCS
#define CHR_TYPE_UTF_7		( 7 )
#define CHR_TYPE_UTF_8		( 8 )
#define CHR_TYPE_UCS_2		( 16 )		// UNICODE
#define CHR_TYPE_UCS_4		( 32 )

typedef struct SVersionInfo
{
	UInt8			MAJOR;
	UInt8			MINOR;
	UInt16			BUILD;
	char			LABEL;
	char			INT_TYPE;
	char			BLD_TYPE;
	char			CHR_TYPE;

	inline SVersionInfo( UInt8 nNewMajor, UInt8 nNewMinor, UInt16 nNewBuild, char chNewLable )
	{
		MAJOR	= nNewMajor;
		MINOR	= nNewMinor;
		BUILD	= nNewBuild;
		LABEL	= chNewLable;

		INT_TYPE = sizeof( int ) << 3;

#ifdef _DEBUG
		BLD_TYPE = BLD_TYPE_DEBUG;
#else //_DEBUG
		BLD_TYPE = BLD_TYPE_RELEASE;
#endif //_DEBUG

#ifdef _UNICODE
		CHR_TYPE = CHR_TYPE_UCS_2;
#else //_UNICODE
		CHR_TYPE = CHR_TYPE_ANSI;
#endif //_UNICODE
	}
} SVersionInfo;
#endif //SVersionInfo


//-----------------------------------------------------------------------------
// Macro, Inline & Template Function Table
//-----------------------------------------------------------------------------

#ifndef LoHalf
	BYTE inline LoHalf( WORD wWord )
	{
		return (BYTE)( wWord & 0x0F );
	}
#endif //LoHalf


#ifndef HiHalf
	BYTE inline HiHalf( WORD wWord )
	{
		return (BYTE)( wWord >> 4 );
	}
#endif //HiHalf


#ifndef LoByte
	BYTE inline LoByte( WORD wWord )
	{
		return static_cast<BYTE>( wWord & 0x00FF );
	}
#endif //LoByte


#ifndef HiByte
	BYTE inline HiByte( WORD wWord )
	{
		return static_cast<BYTE>( wWord >> 8 );
	}
#endif //HiByte


#ifndef LoWord
	WORD inline LoWord( DWORD dwDblWord )
	{
		return static_cast<WORD>( dwDblWord & 0x0000FFFF );
	}
#endif //LoWord


#ifndef HiWord
	WORD inline HiWord( DWORD dwDblWord )
	{
		return static_cast<WORD>( dwDblWord >> 16 );
	}
#endif //HiWord


#ifndef MakeBool
template<class TCLASS> inline bool MakeBool( TCLASS tVal )
{
	return ( tVal == 0 ) ? false : true;
}
#endif //MakeBool

	
#ifndef MakeByte
	BYTE inline MakeByte( BYTE btLowHalf, BYTE btHighHalf )
	{
		return ( ( btHighHalf << 4 ) | ( btLowHalf & 0x0F ) );
	}
#endif //MakeByte


#ifndef MakeWord
	WORD inline MakeWord( BYTE btLowByte, BYTE btHighByte )
	{
		WORD		wWord = btHighByte;

		return ( ( wWord << 8 ) | btLowByte );
	}
#endif //MakeWord


#ifndef MakeDword
	DWORD inline MakeDword( WORD wLowWord, WORD wHighWord )
	{
		DWORD		dwDlbWord = wHighWord;

		return ( ( dwDlbWord << 16 ) | wLowWord );
	}

	DWORD inline MakeDword( BYTE wLowLowByte, BYTE wLowHighByte, BYTE wHighLowByte, BYTE wHighHighByte )
	{
		return MakeDword( MakeWord( wLowLowByte, wLowHighByte ), MakeWord( wHighLowByte, wHighHighByte ) );
	}
#endif// MakeDword


#ifndef ForceByte
template <class TCLASS>
BYTE inline ForceByte( TCLASS tVal )
{
	if( tVal < 0 )
	{
		return 0;
	}
	else if( tVal > 255 )
	{
		return 255;
	}
	else
	{
		return static_cast<BYTE>(tVal);
	}
}
#endif //ForceByte


#ifndef AfxMessageBox
	int inline AfxMessageBox( LPCTSTR pchMessage, LPTSTR pchTitle = _T( "AfxMessage\0" ), UINT nFlag = MB_OK )
	{
		return MessageBox( (HWND)NULL, pchMessage, pchTitle, nFlag );
	}

	int inline AfxMessageBoxA( LPCSTR pchMessage, LPSTR pchTitle = "AfxMessage\0", UINT nFlag = MB_OK )
	{
		return MessageBoxA( (HWND)NULL, pchMessage, pchTitle, nFlag );
	}

	int inline AfxMessageBoxW( LPCWSTR pchMessage, LPWSTR pchTitle = L"AfxMessage\0", UINT nFlag = MB_OK )
	{
		return MessageBoxW( (HWND)NULL, pchMessage, pchTitle, nFlag );
	}
#endif AfxMessageBox


#ifndef IsEqualString
	BOOL inline IsEqualString( LPTSTR pchString1, LPTSTR pchString2 )
	{
		return !( _tcscmp( pchString1, pchString2 ) );
	}
#endif IsEqualString


#ifndef Swap
template<class TCLASS> inline void Swap( TCLASS & rtVal1, TCLASS & rtVal2 )
{
	TCLASS	tTemp = rtVal1;
	rtVal1 = rtVal2;
	rtVal2 = tTemp;
}
#endif //Swap


#ifndef Min
template<class TCLASS> inline TCLASS Min( TCLASS tVal1, TCLASS tVal2 )
{
	return (( tVal1 < tVal2 ) ? tVal1 : tVal2 );
}

template<class TCLASS> inline TCLASS Min( TCLASS tVal1, TCLASS tVal2, TCLASS tVal3 )
{
	return (( tVal1 < tVal2 ) ?
				(( tVal1 < tVal3 ) ?
					tVal1 :					// 1 < 2 ? 3
					tVal3 ) :				// 3 < 1 < 2
				(( tVal2 < tVal3 ) ?
					tVal2 :					// 2 < 3 ? 1
					tVal3 ));				// 3 < 2 < 1
}
#endif //Min


#ifndef Max
template<class TCLASS> inline TCLASS Max( TCLASS tVal1, TCLASS tVal2 )
{
	return ( ( tVal1 > tVal2 ) ? tVal1 : tVal2 );
}

template<class TCLASS> inline TCLASS Max( TCLASS tVal1, TCLASS tVal2, TCLASS tVal3 )
{
	return (( tVal1 > tVal2 ) ?
				(( tVal1 > tVal3 ) ?
					tVal1 :					// 1 > 2 ? 3
					tVal3 ) :				// 3 > 1 > 2 
				(( tVal2 > tVal3 ) ?
					tVal2 :					// 2 > 3 ? 1
					tVal3 ));				// 3 > 2 > 1
}
#endif //Max


#ifndef Med
template<class TCLASS> inline TCLASS Med( TCLASS tVal1, TCLASS tVal2, TCLASS tVal3 )
{
	return ( ( tVal2 > tVal3 ) ?
				(( tVal2 > tVal1 ) ?
					(( tVal3 > tVal1 ) ?
						tVal3 :				// 2 > 3 > 1
						tVal1 ) :			// 2 > 1 > 3
					tVal2 ) :				// 1 > 2 > 3
				(( tVal1 > tVal2 ) ?
					(( tVal3 > tVal1 ) ?
						tVal1 :				// 3 > 1 > 2
						tVal3 ) :			// 1 > 3 > 2
					tVal2 ) );				// 3 > 2 > 1
}
#endif //Med


#ifndef GetElapsedTickCount
inline DWORD GetElapsedTickCount( void )
{
	static DWORD	dwUpdatedCount = GetTickCount();
	DWORD			dwCurrentCount = GetTickCount();
	DWORD			dwElapsedTickCount = dwCurrentCount - dwUpdatedCount;
	dwUpdatedCount	= dwCurrentCount;

	return dwElapsedTickCount;
}
#endif //GetElapsedTickCount


#define FILE_SHARE_DEFAULT		( 0L )
#define CREATE_DEFAULT			( 0L )
#define FILE_ATTRIBUTE_DEFAULT	( 0L )

inline HANDLE CreateFileSafeW( LPCWSTR pchFileName,
								DWORD dwDesiredAccess,
								DWORD dwShareMode							= FILE_SHARE_DEFAULT,
								LPSECURITY_ATTRIBUTES pSecurityAttributes	= NULL, 
								DWORD dwCreationDisposition					= CREATE_DEFAULT, 
								DWORD dwFlagsAndAttributes					= FILE_ATTRIBUTE_DEFAULT, 
								HANDLE hTemplateFile						= NULL )
{
	if( dwShareMode == FILE_SHARE_DEFAULT )
	{
		if( dwDesiredAccess & GENERIC_WRITE )
		{
			dwShareMode = FILE_SHARE_WRITE;
		}

		if( dwDesiredAccess & GENERIC_READ )
		{
			dwShareMode |= FILE_SHARE_READ;
		}
	}

	if( dwCreationDisposition == CREATE_DEFAULT )
	{
		if( dwDesiredAccess & GENERIC_WRITE )
		{
			dwCreationDisposition = CREATE_ALWAYS;
		}
		else 
		{
			dwCreationDisposition = OPEN_EXISTING;
		}
	}

	if( dwFlagsAndAttributes == FILE_ATTRIBUTE_DEFAULT )
	{
		dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	}

	OSVERSIONINFO	sOSVersionInfo;

	sOSVersionInfo.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
	GetVersionEx( &sOSVersionInfo );

	switch( sOSVersionInfo.dwPlatformId )
	{
		case VER_PLATFORM_WIN32s:
		case VER_PLATFORM_WIN32_WINDOWS:
		{
			int		nLength = static_cast<int>(wcslen( pchFileName ));
			char *	pchFileNameAnsi = new char[(nLength << 1) + 1];

			if( WideCharToMultiByte( CP_ACP, WC_SEPCHARS,
									pchFileName, -1,
									pchFileNameAnsi, nLength + 1,
									NULL, NULL ) == 0 )
			{
				delete[] pchFileNameAnsi;

				return INVALID_HANDLE_VALUE;
			}

			HANDLE	hFile = CreateFileA( pchFileNameAnsi, dwDesiredAccess, dwShareMode,
										pSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );

			delete[] pchFileNameAnsi;

			return hFile;
		}

		case VER_PLATFORM_WIN32_NT:
		{
			return CreateFileW( pchFileName, dwDesiredAccess, dwShareMode,
								pSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );
		}

		default:
		{
			SetLastError( ERROR_OLD_WIN_VERSION );

			return INVALID_HANDLE_VALUE;
		}
	}
}


inline HANDLE CreateFileSafeA( LPCSTR pchFileName,
								DWORD dwDesiredAccess,
								DWORD dwShareMode							= FILE_SHARE_DEFAULT,
								LPSECURITY_ATTRIBUTES pSecurityAttributes	= NULL,
								DWORD dwCreationDisposition					= CREATE_DEFAULT,
								DWORD dwFlagsAndAttributes					= FILE_ATTRIBUTE_DEFAULT,
								HANDLE hTemplateFile						= NULL )
{
	if( dwShareMode == FILE_SHARE_DEFAULT )
	{
		if( dwDesiredAccess & GENERIC_WRITE )
		{
			dwShareMode = FILE_SHARE_WRITE;
		}

		if( dwDesiredAccess & GENERIC_READ )
		{
			dwShareMode |= FILE_SHARE_READ;
		}
	}

	if( dwCreationDisposition == CREATE_DEFAULT )
	{
		if( dwDesiredAccess & GENERIC_WRITE )
		{
			dwCreationDisposition = CREATE_ALWAYS;
		}
		else 
		{
			dwCreationDisposition = OPEN_EXISTING;
		}
	}

	if( dwFlagsAndAttributes == FILE_ATTRIBUTE_DEFAULT )
	{
		dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
	}

	HANDLE	hFile = CreateFileA( pchFileName, dwDesiredAccess, dwShareMode,
								pSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );

	return hFile;
}


#ifndef CreateFileSafe
#ifdef _UNICODE
inline HANDLE CreateFileSafe( LPCTSTR lpcFileName,
								DWORD dwDesiredAccess,
								DWORD dwShareMode							= FILE_SHARE_DEFAULT,
								LPSECURITY_ATTRIBUTES lpSecurityAttributes	= NULL,
								DWORD dwCreationDisposition					= CREATE_DEFAULT, 
								DWORD dwFlagsAndAttributes					= FILE_ATTRIBUTE_DEFAULT, 
								HANDLE hTemplateFile						= NULL )
{
	return CreateFileSafeW( lpcFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
							dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );
}
#else //_UNICODE
inline HANDLE CreateFileSafe( LPCTSTR lpcFileName,
								DWORD dwDesiredAccess,
								DWORD dwShareMode							= FILE_SHARE_DEFAULT,
								LPSECURITY_ATTRIBUTES lpSecurityAttributes	= NULL,
								DWORD dwCreationDisposition					= CREATE_DEFAULT, 
								DWORD dwFlagsAndAttributes					= FILE_ATTRIBUTE_DEFAULT, 
								HANDLE hTemplateFile						= NULL )
{
	return CreateFileSafeA( lpcFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes,
							dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile );
}
#endif //_UNICODE
#endif //CreateFileSafe


inline UInt32 Power10( UInt32 nPower )
{
	UInt32	nValue = 1;

	while( nPower != 0 )
	{
		nValue = (nValue << 3) + (nValue << 1);

		--nPower;
	}

	return nValue;
}

END_NAMESPACE( bas )

#endif //___BASE_TYPE_H___

//*****************************************************************************
//
// End of File : BaseType.h
//
//*****************************************************************************