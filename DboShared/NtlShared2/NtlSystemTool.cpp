#include "StdAfx.h"
#include "NtlSystemTool.h"
#include "NtlString.h"
#include <sstream>

//-- SGpro --------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
BYTE GetIP( int nIP, BYTE byIndex )
{
	BYTE byOut;

	int nTemp = 0x000000FF;
	nIP = nIP >> ( 8 /*sizeof( BYTE )*/ * byIndex );
	byOut = (BYTE) ( nTemp & nIP );
	return byOut;
}

//-- SGpro --------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
std::string GetIP( int nIP )
{
	CNtlString strIP;

	strIP.Format( "%d.%d.%d.%d"
		, GetIP( nIP, 0 )
		, GetIP( nIP, 1 )
		, GetIP( nIP, 2 )
		, GetIP( nIP, 3 ) );

	std::string strResult( strIP.c_str() );

	return strResult;
}

int GetIP( std::string strIP )
{
	int nIP[4], nJustInCase(0);
	while (std::string::npos!=strIP.find("."))
	{
		strIP[strIP.find(".")] = ' ';
		if ( ++nJustInCase > 5 ) return -1; // ERROR
	}
	std::istringstream ss(strIP.c_str());

	ss >> nIP[0];
	ss >> nIP[1];
	ss >> nIP[2];
	ss >> nIP[3];

	uGM_TOOL_IP_ADDRESS uIPAddress;
	uIPAddress.achIP[0] = (BYTE)nIP[0];
	uIPAddress.achIP[1] = (BYTE)nIP[1];
	uIPAddress.achIP[2] = (BYTE)nIP[2];
	uIPAddress.achIP[3] = (BYTE)nIP[3];

	return uIPAddress.nIP;
}