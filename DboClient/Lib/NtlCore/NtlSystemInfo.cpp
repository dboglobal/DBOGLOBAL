#include "precomp_ntlcore.h"
#include "NtlSystemInfo.h"

#include <lm.h>
#include "NtlDebug.h"


#pragma comment(lib, "Netapi32.lib")

const BYTE* GetSystemMacAddress()
{
	WKSTA_TRANSPORT_INFO_0*	pwkti;                  // Allocate data structure for Netbios
	DWORD					dwEntriesRead;
	DWORD					dwTotalEntries;
	BYTE					*pbBuffer;
	static unsigned char	acMACAddress[6];

	// Get MAC address via NetBios's enumerate function
	NET_API_STATUS dwStatus = NetWkstaTransportEnum(NULL,					// [in]  server name
													0,						// [in]  data structure to return
													&pbBuffer,				// [out] pointer to buffer
													MAX_PREFERRED_LENGTH,	// [in]  maximum length
													&dwEntriesRead,			// [out] counter of elements actually enumerated
													&dwTotalEntries,		// [out] total number of elements that could be enumerated
													NULL);					// [in/out] resume handle

	if( NERR_Success != dwStatus )
	{
		DBO_FAIL("Fail read MAC address");
		return NULL;
	}

	pwkti = (WKSTA_TRANSPORT_INFO_0 *)pbBuffer;		// type cast the buffer

	// first address is 00000000, skip it
	for( DWORD dwLoop = 1 ; dwLoop < dwEntriesRead; dwLoop++ )            
	{
		// enumerate MACs and print
		swscanf((wchar_t *)pwkti[dwLoop].wkti0_transport_address, L"%2hx%2hx%2hx%2hx%2hx%2hx", 
				&acMACAddress[0], &acMACAddress[1], &acMACAddress[2],
				&acMACAddress[3], &acMACAddress[4], &acMACAddress[5]);
	}

	// Release pbBuffer allocated by above function
	dwStatus = NetApiBufferFree(pbBuffer);

	if( NERR_Success != dwStatus )
		DBO_FAIL("Fail free buffer NetAPI");

	return acMACAddress;
}