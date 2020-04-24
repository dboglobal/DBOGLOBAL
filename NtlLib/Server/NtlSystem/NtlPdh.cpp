//***********************************************************************************
//
//	File		:	NtlPdh.cpp
//
//	Begin		:	2007-02-07
//
//	Copyright	:	¨Ï NTL-Inc Co., Ltd
//
//	Author		:	Hyun Woo, Koo   ( zeroera@ntl-inc.com )
//
//	Desc		:	
//
//***********************************************************************************


#include "stdAfx.h"
#include "NtlPdh.h"

#include "NtlError.h"

#include <stdio.h>
#include <tchar.h>

#pragma comment(lib, "Pdh")


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPdh::CNtlPdh()
{
	Init();
}	


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
CNtlPdh::~CNtlPdh(void)
{
	Stop();
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPdh::Init()
{
	m_hQuery = NULL;

	m_hLog = NULL;

	m_dwCounterCount = 0;

	for (DWORD dwIndex = 0 ; dwIndex < MAX_COUNTER_COUNT ; dwIndex++)
	{
		m_ahCounter[dwIndex] = NULL;
		m_astrCounterName[dwIndex].clear();
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlPdh::Prepare()
{
	Stop();


	int rc = PdhOpenQuery( NULL, 0, &m_hQuery );
	if ( NTL_SUCCESS != rc )
	{
		return rc;
	}	


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlPdh::RegisterCounter(const char * lpszCounter)
{
	if (MAX_COUNTER_COUNT <= m_dwCounterCount)
	{
		return NTL_FAIL;
	}


	int rc = PdhValidatePath( lpszCounter );
	if ( NTL_SUCCESS != rc )
	{
		return rc;
	}


	rc = PdhAddCounter( m_hQuery, lpszCounter, 0, &(m_ahCounter[m_dwCounterCount]) );
	if ( NTL_SUCCESS != rc )
	{
		return rc;
	}


	m_dwCounterCount++;


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
int CNtlPdh::Start(const char * lpszFileName)
{
	if ( NULL != lpszFileName )
	{
		DWORD dwLogType = PDH_LOG_TYPE_CSV;

		int rc = PdhOpenLog( lpszFileName, PDH_LOG_WRITE_ACCESS | PDH_LOG_CREATE_ALWAYS, &dwLogType, m_hQuery, 0, NULL, &m_hLog );
		if ( NTL_SUCCESS != rc )
		{
			return rc;
		}
	}


	return NTL_SUCCESS;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPdh::Stop()
{
	if( m_hLog )
	{
		PdhCloseLog( m_hLog, PDH_FLAGS_CLOSE_QUERY );
		m_hLog = NULL;
	}

	if( m_hQuery )
	{
		PdhCloseQuery( m_hQuery );
		m_hQuery = NULL;
	}

	m_dwCounterCount = 0;

	for (DWORD dwIndex = 0 ; dwIndex < MAX_COUNTER_COUNT ; dwIndex++)
	{
		if( m_ahCounter[dwIndex] )
		{
			PdhRemoveCounter( m_ahCounter[dwIndex] );
			m_ahCounter[dwIndex] = NULL;
			m_astrCounterName[dwIndex].clear();
		}
	}
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
LONG CNtlPdh::GetValue()
{
	int rc = PdhCollectQueryData( m_hQuery );
	if ( NTL_SUCCESS != rc )
	{
		return 0;
	}


	PDH_FMT_COUNTERVALUE value;
	rc = PdhGetFormattedCounterValue( m_ahCounter[0], PDH_FMT_LONG, NULL, &value );
	if (  NTL_SUCCESS != rc )
	{
		return 0;
	}

	return value.longValue;
}


//-----------------------------------------------------------------------------------
//		Purpose	:
//		Return	:
//-----------------------------------------------------------------------------------
void CNtlPdh::UpdateLog()
{
	if (NULL != m_hLog)
	{
		PdhUpdateLog(m_hLog, "Dummy Message");
	}
}


int CNtlPdh::UnitTest()
{
	CNtlPdh cpuHelper;
	CNtlPdh cpuHelper2;

	CNtlString strErrorMessage;

//	TCHAR buffer[256] = { 0x00, };

	//_stprintf_s(buffer, _countof(buffer), TEXT("\\Processor(0)\\% Processor Time") );
	//_stprintf_s(buffer, _countof(buffer), TEXT("\\Process\\% Processor Time") );
	//_stprintf_s(buffer, _countof(buffer), TEXT("\\Memory\\Available Bytes") );
	//_stprintf_s(buffer, _countof(buffer), TEXT("\\Process(PerformaceDataHelper)\\% Processor Time") );
	//_stprintf_s(buffer, _countof(buffer), TEXT("\\Processor(_Total)\\% Processor Time") );
	//_stprintf_s(buffer, _countof(buffer), TEXT("\\Process(EXPLORER)\\% Processor Time") );
	{
		int rc = cpuHelper.Prepare();
		if( NTL_SUCCESS != rc )
		{
			NtlGetErrorString(strErrorMessage, rc);
			printf( "Error %s\n", strErrorMessage.c_str() );
			return rc;
		}

		rc = cpuHelper.RegisterCounter("\\Processor(_Total)\\% Processor Time");
		if( NTL_SUCCESS != rc )
		{
			NtlGetErrorString(strErrorMessage, rc);
			printf( "Error %s\n", strErrorMessage.c_str() );
			return rc;
		}

		rc = cpuHelper.RegisterCounter("\\Processor(_Total)\\% User Time");
		if( NTL_SUCCESS != rc )
		{
			NtlGetErrorString(strErrorMessage, rc);
			printf( "Error %s\n", strErrorMessage.c_str() );
			return rc;
		}

		rc = cpuHelper.Start( "cpuHelper.csv" );
		if( NTL_SUCCESS != rc )
		{
			NtlGetErrorString(strErrorMessage, rc);
			printf( "Error %s\n", strErrorMessage.c_str() );
			return rc;
		}
	}

	{
		int rc = cpuHelper2.Prepare();
		if( NTL_SUCCESS != rc )
		{
			NtlGetErrorString(strErrorMessage, rc);
			printf( "Error %s\n", strErrorMessage.c_str() );
			return rc;
		}

		rc = cpuHelper2.RegisterCounter("\\Process(EXPLORER)\\% Processor Time");
		if( NTL_SUCCESS != rc )
		{
			NtlGetErrorString(strErrorMessage, rc);
			printf( "Error %s\n", strErrorMessage.c_str() );
			return rc;
		}

		rc = cpuHelper2.RegisterCounter("\\Process(EXPLORER)\\% User Time");
		if( NTL_SUCCESS != rc )
		{
			NtlGetErrorString(strErrorMessage, rc);
			printf( "Error %s\n", strErrorMessage.c_str() );
			return rc;
		}

		rc = cpuHelper2.Start( "cpuHelper2.csv" );
		if( NTL_SUCCESS != rc )
		{
			NtlGetErrorString(strErrorMessage, rc);
			printf( "Error %s\n", strErrorMessage.c_str() );
			return rc;
		}
	}

	for(int i = 0; i < 100; i ++ )
	{
		Sleep( 1000 );

		cpuHelper.UpdateLog();
		cpuHelper2.UpdateLog();

		printf( "cpu _Total  : %u\n", cpuHelper.GetValue() );
		
		printf( "cpu EXPLORER : %u\n", cpuHelper2.GetValue() );
	}

	return NTL_SUCCESS;
}


//====================================================================================
//
//====================================================================================
/*

// Finding Memory Bottlenecks 
Memory\ Page Faults\sec 
Memory\ Page Reads\sec 
Memory\ Page Writes\sec 
Memory\ Pages Input\sec 
Memory\ Pages Output\sec 
Memory\ Available Bytes 
Memory\ Pool Nonpaged Bytes

Process\ Page Faults\sec 
Process\ Working Set 
Process\ Private Bytes 
Process\ Page File Bytes 

// Finding Processor Bottlenecks 
Processor\ % Processor Time 
System\ % Total Processor Time 
System\ Processor Queue Length 

Process\ % Privileged Time 
Process\ % Processor Time 
Process\ % User Time 
Process\ Priority Base 
Thread\ % Privileged Time 
Thread\ % Processor Time 
Thread\ % User Time 
Thread\ Context Switches\sec 
Thread\ Priority Base 
Thread\ Priority Current 
Thread\ Thread State 

// Finding Disk Bottlenecks 
PhysicalDisk\ % Disk Time 
PhysicalDisk\ Avg. Disk Queue Length 
PhysicalDisk\ Current Disk Queue Length 
PhysicalDisk\ Avg. Disk Sec\Read 
PhysicalDisk\ Avg. Disk Sec\Write 
PhysicalDisk\ Disk Read Bytes\sec 
PhysicalDisk\ Disk Write Bytes\sec 
PhysicalDisk\ Avg. Disk Bytes\Read 
PhysicalDisk\ Avg. Disk Bytes\Write 
PhysicalDisk\ Disk Reads\sec 
PhysicalDisk\ Disk Writes\sec 

// Finding Cache Bottlenecks
Memory\ Cache Bytes 
Memory\ Cache Faults\sec 
Memory\ Page Faults\sec 
Memory\ Pages Input\sec 
Memory\ Pages Output\sec 
Cache\ Copy Reads\sec 
Cache\ Data Flushes\sec 
Cache\ Copy Read Hits % 
Cache\ Lazy Write Pages\sec 
Cache\ Lazy Write Flushes\sec 
Cache\ Read Aheads\sec 
PhysicalDisk\ Disk Reads\sec 
PhysicalDisk\ Disk Writes\sec 


// Finding Bottlenecks on Multiprocessor Computers
System\ % Total Processor Time 
System\ Processor Queue Length 
Processor\ % Processor Time 
Process\ % Processor Time 
Thread\ % Processor Time 


// Finding Network Bottlenecks
Network Interface\ Bytes Total\sec 
Network Interface\ Bytes Sent\sec 
Network Interface\ Bytes Received\sec 
Protocol_layer_object\ Segments Received\sec 
Protocol_layer_object\ Segments Sent\sec 
Protocol_layer_object\ Frames Sent\sec 
Protocol_layer_object\ Frames Received\sec

Server\ Bytes Total\sec 
Server\ Bytes Received\sec 
Server\ Bytes Sent\sec 
Network Segment\ % Network Utilization 

*/
//====================================================================================