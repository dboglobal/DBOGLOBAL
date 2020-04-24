//***********************************************************************************
//
//	File		:	NtlDebug.h
//
//	Begin		:	2005-12-06
//
//	Copyright	:	ⓒ NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	각종 디버그 관련 유틸리티
//
//***********************************************************************************

#pragma once


#include "NtlMiniDump.h"

#include <crtdbg.h>
#include <stdio.h>


#define __NTL_DEBUG_PRINT__


//-----------------------------------------------------------------------------------
// static variable
//-----------------------------------------------------------------------------------
extern const unsigned int PRINT_BUF_SIZE;
extern unsigned int s_dwCurFlag;
extern FILE * s_curStream;
//-----------------------------------------------------------------------------------



#ifdef __NTL_DEBUG_PRINT__

	void NtlSetPrintStream(FILE * fp);
	void NtlSetPrintFlag(unsigned int dwFlag);
	void NtlDebugPrint(unsigned int dwFlag, LPCTSTR lpszText, ...);

	#define NTL_PRINT			NtlDebugPrint

#else


	inline void NtlSetPrintStream(FILE * fp) {}
	inline void NtlSetPrintFlag(unsigned int dwFlag) {}
	inline void NtlDebugPrint(unsigned int dwFlag, LPCTSTR lpszText, ...) {}

	#define NTL_PRINT			1 ? (void) 0 : NtlDebugPrint


#endif


#if defined( _DEBUG ) && defined( _DEVEL )

	#define NTL_DBGREPORT(msg, ...)																	\
	{																								\
		if( 1 == _CrtDbgReport( _CRT_ASSERT, __FILE__, __LINE__, __FUNCTION__, msg, __VA_ARGS__ ) ) \
			_CrtDbgBreak();																			\
	}

	#define NTL_ASSERT(x)			_ASSERT(x)

#elif defined( _DEBUG )

	#define NTL_DBGREPORT(msg, ...)

	#define NTL_ASSERT(x)																			\
			(void) ( (!!(x)) ||																		\
					(CNtlMiniDump::Snapshot(), 0))

#else

	#define NTL_DBGREPORT(msg, ...)

	#define NTL_ASSERT(x)																			\
			(void) ( (!!(x)) ||																		\
					(CNtlMiniDump::Snapshot(), 0))

#endif


#define PRINT_SYSTEM		0x01
#define PRINT_ALL			0xFFFFFFFF
