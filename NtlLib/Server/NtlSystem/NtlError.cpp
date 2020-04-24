//***********************************************************************************
//
//	File		:	NtlError.cpp
//
//	Begin		:	2005-11-30
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	Error 처리 관련
//
//***********************************************************************************

#include "stdafx.h"
#include "NtlError.h"

#ifdef NTL_DEFINE_ERROR
	#undef NTL_DEFINE_ERROR
	#define NTL_DEFINE_ERROR(x)		#x,
#endif

char * ntl_error_string[ MAX_NTL_ERROR - NTL_ERR_BEGIN ] = 
{
	"NTL_ERR_BEGIN",

	#include "NtlErrorCodes.h"
};

const char * GetNtlErrorString(int nErrorCode )
{
	if( nErrorCode >= MAX_NTL_ERROR )
		return "Ntl Error message not found";

	return ntl_error_string[ nErrorCode - NTL_ERR_BEGIN ];
}



const unsigned int	MAX_ERR_STR_BUFF = 256;


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void NtlGetErrorString( CNtlString & rStrError, int iErrorCode )
{
	if( iErrorCode >= NTL_ERR_BEGIN )
	{
		rStrError = GetNtlErrorString( iErrorCode );
	}
	else
	{
		char szBuf[MAX_ERR_STR_BUFF] = { 0x00, };

		FormatMessage(	FORMAT_MESSAGE_FROM_SYSTEM,
						0,
						iErrorCode,
						MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
						szBuf,
						MAX_ERR_STR_BUFF,
						NULL);

		rStrError = szBuf;
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
const char * NtlGetErrorMessage( int iErrorCode )
{
	if( iErrorCode >= NTL_ERR_BEGIN )
	{
		return const_cast<char*>( GetNtlErrorString( iErrorCode ) );
	}
	else
	{
		static char szBuf[MAX_ERR_STR_BUFF] = { 0x00, };

		FormatMessage(	FORMAT_MESSAGE_FROM_SYSTEM,
						0,
						iErrorCode,
						MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
						szBuf,
						MAX_ERR_STR_BUFF,
						NULL);

		return szBuf;
	}
}

