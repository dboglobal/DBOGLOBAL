/****************************************************************************
*
* File			: DboCompileOpt.h
* Author		: HyungSuk, Jang
* Copyright	: (аж)NTL
* Date			: 2005. 9. 28	
* Abstract		: DBO compile option.
*****************************************************************************
* Desc         : 
*
*****************************************************************************/

#ifndef __DBO_COMPILE_OPT_H___
#define __DBO_COMPILE_OPT_H___

#define DATA_MODE_XML	0		// xml
#define DATA_MODE_BIN	1		// bin
#define DATA_MODE_ENC	2		// enc

//-----------------------------------------------------------------------------
// discord

#define USE_DISCORD

//-----------------------------------------------------------------------------
// server network sync mode	

#define NET_SERVER_SYNC

#define MUTEX_CHECK //allow multi client

#define LOAD_DATA_MODE		DATA_MODE_ENC

#define USE_PACK_DATA //Whether to use the pack

//#define COLLISION_VIEW

#define _UICONFIG

// Fluorite is only available
#ifdef NDEBUG

#ifndef NET_SERVER_SYNC
#define NET_SERVER_SYNC
#endif

#ifndef MUTEX_CHECK
#define MUTEX_CHECK
#endif

#ifndef USE_PACK_DATA
#define USE_PACK_DATA
#endif

#ifdef LOAD_DATA_MODE
#undef LOAD_DATA_MODE
#define LOAD_DATA_MODE		DATA_MODE_ENC
#endif

#endif	// NDEBUG



#endif