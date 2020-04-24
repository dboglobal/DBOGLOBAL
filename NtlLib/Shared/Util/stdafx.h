// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers


// Turn-off this Warning for BoundsChecker

#pragma warning( disable:4651 )
#pragma warning( disable:4652 )
#pragma warning( disable:4653 )
#pragma warning( disable:4748 )
#pragma warning( disable:4100 )
#pragma warning( disable:4996 )


//#include <ws2tcpip.h>
#include <WinSock2.h>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include <wincrypt.h>

// TODO: reference additional headers your program requires here
