//***********************************************************************************
//
//	File		:	NtlError.h
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

#ifndef __NTLERROR_H__
#define __NTLERROR_H__

#include "NtlString.h"

#define NTL_DEFINE_ERROR(x)		x,

enum
{
	NTL_ERR_BEGIN = 100000,

	#include "NtlErrorCodes.h"

	MAX_NTL_ERROR
};

void NtlGetErrorString( CNtlString & rStrError,int iErrorCode );

const char * NtlGetErrorMessage( int iErrorCode );

#endif // __NTLERROR_H__
