#pragma once

#include "..\..\Shared\NtlSharedCommon.h"
#include <stdlib.h>


// NULL define
#ifndef NULL
	#ifdef __cplusplus
		#define NULL    0
	#else
		#define NULL    ((void *)0)
	#endif
#endif

// memory safe delete
#if !defined(SAFE_DELETE)
	#define SAFE_DELETE(p)			if(p) { delete p; p = 0; }
#endif

#if !defined(SAFE_DELETE_ARRAY)
#define SAFE_DELETE_ARRAY(p)	if(p) { delete[] p; p = 0; }
#endif

#if !defined(SAFE_FREE)
#define SAFE_FREE(p)			if(p) { free(p); p = 0; }
#endif

#define NTL_SUCCESS				0
#define INVALID_HSESSION		0xFFFFFFFF
#define INVALID_HDATABASE		0xFFFFFFFF

typedef unsigned int			HSESSION;
typedef unsigned short			SESSIONTYPE;
typedef unsigned int			HDATABASE;