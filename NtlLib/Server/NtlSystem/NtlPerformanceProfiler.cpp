//-- include ------------------------------------------------------------------------
#include "stdafx.h"
#include "NtlPerformanceProfiler.h"
#include <assert.h>
#include <fstream>
#include <winbase.h>
//-----------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
CProfileStatistic::CProfileStatistic( void )
: m_mapProfiles()
, m_dequeFunc()
, m_strFilename( PROFILE_OUTPUT_FILENAME )
, m_uCpuCycle( 2612191997 )
, m_mutex()
{
	CheckCpuCycle();

	SYSTEMTIME tempSystemTime;
	GetLocalTime(&tempSystemTime);
	sprintf_s(m_szFileNameTime, sizeof(m_szFileNameTime)
		, "%s(%04d-%02d-%02d %02d_%02d_%02d).txt"
		, m_strFilename.c_str()
		, tempSystemTime.wYear
		, tempSystemTime.wMonth
		, tempSystemTime.wDay
		, tempSystemTime.wHour
		, tempSystemTime.wMinute
		, tempSystemTime.wSecond );
}

//-----------------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
CProfileStatistic::~CProfileStatistic( void )
{
	Save();
	return;
}

//-----------------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CProfileStatistic::BeginProfile( unsigned __int64 startCycle )
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	FUNC func = { startCycle, 0 };
	m_dequeFunc.push_front( func );
	return;
}

//-----------------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CProfileStatistic::EndProfile( const char* pszToken
								   , unsigned __int64 endCycle
								   , const char* pszFile
								   , unsigned __int16 nLine )
{
	if( NULL == pszToken )
	{
		assert( 0 );
		return;
	}

	if( NULL == pszFile )
	{
		assert( 0 );
		return;
	}

	if( true == m_dequeFunc.empty() )
	{
		assert( 0 );
		return;
	}

	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	FUNC& func = m_dequeFunc.front();
	unsigned __int64 total_cycle = endCycle - func.m_uiStartCycle;
	unsigned __int64 pure_cycle = total_cycle - func.m_uiChildCycle;
	m_dequeFunc.pop_front();

	if( false == m_dequeFunc.empty() )
	{
		FUNC& parent_func = m_dequeFunc.front();
		parent_func.m_uiChildCycle += total_cycle;
	}

	PROFILES_ITER find_iter = m_mapProfiles.find( pszToken );
	if( m_mapProfiles.end() == find_iter )
	{
		if( false == m_mapProfiles.insert( PROFILES_VALUE( pszToken, PROFILEUNIT( pszFile, nLine, pure_cycle ) ) ).second )
		{
			assert( 0 );
			return;
		}
	}
	else
	{
		PROFILEUNIT& profile = find_iter->second;

		static const unsigned __int32 INVALID_INT32 = unsigned __int32(-1);
		static const unsigned __int64 INVALID_INT64 = unsigned __int64(-1);

		if( INVALID_INT32 == profile.m_uiCalledCount + 1 )
		{
			++profile.m_uiTurnCount_CalledCount;
			profile.m_uiCalledCount = 0;
		}

		++profile.m_uiCalledCount;

		if( pure_cycle > INVALID_INT64 - profile.m_uiTotalCycle )
		{
			++profile.m_uiTurnCount_TotalCycle;
			profile.m_uiTotalCycle = INVALID_INT64 - profile.m_uiTotalCycle;
		}
		else
		{
			profile.m_uiTotalCycle += pure_cycle;
		}

		if( pure_cycle > profile.m_uiMaxCycle )
		{
			profile.m_uiMaxCycle = pure_cycle;
		}

		if( pure_cycle < profile.m_uiMinCycle )
		{
			profile.m_uiMinCycle = pure_cycle;
		}
	}//end of if else
	return;
}

//-----------------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CProfileStatistic::Save( void )
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	if( true == m_mapProfiles.empty() )
	{
		return;
	}

	string strLogFilePath = "PerformanceProfileLog";
	if (FALSE == ::CreateDirectory(strLogFilePath.c_str(), NULL))
	{
		DWORD dwLastError = ::GetLastError();
		if (ERROR_ALREADY_EXISTS != dwLastError)
		{
			strLogFilePath = ".";
		}
	}

	string strLogFile = strLogFilePath;
	strLogFile += "\\";
	strLogFile += m_szFileNameTime;

	ofstream file( strLogFile.c_str() );
	if( file.is_open() )
	{
		char szCpuCycle[64] = { 0, };
		sprintf_s( szCpuCycle, sizeof( szCpuCycle ), "%I64d", m_uCpuCycle );
		file << "Time Unit : ms, ";
		file << "CpuCycle : " << szCpuCycle << "\n\n";
		file << "\t\t\t\t MinCycle \t MaxCycle \t AvrCycle \t TotalCycle \t CallCount \t ToTalCycle_TurnCount \t CallCount_TurnCount \n";

		for( PROFILES_ITER iter = m_mapProfiles.begin()
			; m_mapProfiles.end() != iter
			; iter++ )
		{
			const string& strToken = iter->first;
			PROFILEUNIT& profile = iter->second;

			char szToken[64], szTotalCycle[64], szAverageCycle[64], szMinCycle[64], szMaxCycle[64];


			float fTimeUnit = 1000.0f;
			sprintf_s( szToken, sizeof( szToken ), "%-31s", strToken.c_str() );
			sprintf_s( szMinCycle, sizeof( szMinCycle ), "%-15.8f", float( profile.m_uiMinCycle ) / float( m_uCpuCycle ) * fTimeUnit );
			sprintf_s( szMaxCycle, sizeof( szMaxCycle ), "%-15.8f", float( profile.m_uiMaxCycle ) / float( m_uCpuCycle ) * fTimeUnit );
			sprintf_s( szTotalCycle, sizeof( szTotalCycle ), "%-15.8f", float( profile.m_uiTotalCycle ) / float( m_uCpuCycle ) * fTimeUnit );
			sprintf_s( szAverageCycle, sizeof( szAverageCycle ), "%-15.8f", float( profile.m_uiTotalCycle ) / float( profile.m_uiCalledCount ) / float( m_uCpuCycle ) * fTimeUnit );

			file << szToken;						
			file << "\t" << szMinCycle << "\t" << szMaxCycle;
			file << "\t" << szAverageCycle;
			file << "\t" << szTotalCycle;
			file << "\t" << profile.m_uiCalledCount;
			file << "\t" << profile.m_uiTurnCount_TotalCycle;
			file << "\t" << profile.m_uiTurnCount_CalledCount << endl;
		}
	}//end of if( file.is_open() )
	return;
}

//-----------------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CProfileStatistic::CheckCpuCycle( void )
{
	unsigned __int64 nTotal = 0;

	for( unsigned __int32 nT1 = 0
		; 10 > nT1
		; ++nT1 )
	{
		unsigned __int64 nStart;
		QueryPerformanceCounter( ( LARGE_INTEGER* )&nStart );

		Sleep( 1000 );

		unsigned __int64 nEnd;
		QueryPerformanceCounter( ( LARGE_INTEGER* )&nEnd );
		nTotal += nEnd - nStart;
	}//end of for

	nTotal = nTotal / 10;

	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	m_uCpuCycle = nTotal;

	return;
}

//-----------------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CProfileStatistic::ClearProfile()
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	m_dequeFunc.clear();
	m_mapProfiles.clear();
}

//-----------------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
CProfile::CProfile( const char * szToken, const char * szFile )
: m_mutex()
{
	Init( szToken, szFile, 0 );
	return;
}

//-----------------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
CProfile::CProfile( const char * szToken, const char * szFile, unsigned __int16 nLine )
: m_mutex()
{ 
	Init( szToken, szFile, nLine );
	return;
}

//-----------------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
CProfile::~CProfile( void )
{
	Destroy();

	return;
}

//-----------------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CProfile::Init( const char * szToken, const char * szFile, unsigned __int16 nLine )
{
	{
		CNtlAutoMutex mutex( &m_mutex );
		mutex.Lock();

		m_cycle = 0;
		m_szToken = szToken;
		m_szFilename = szFile;
		m_nLine = nLine;
	}

	Create( nLine );
	return;
}

//-----------------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CProfile::Create( unsigned __int16 nLine )
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();

	m_nLine = nLine;

	unsigned __int64 nStart;
	QueryPerformanceCounter( ( LARGE_INTEGER* )&nStart );

	m_cycle = nStart;
	CProfileStatistic::GetInstance()->BeginProfile( nStart );
	return;
}

//-----------------------------------------------------------------------------------
//  Purpose :
//  Return  :
//-----------------------------------------------------------------------------------
void CProfile::Destroy( void )
{
	CNtlAutoMutex mutex( &m_mutex );
	mutex.Lock();	

	unsigned __int64 nEnd;
	QueryPerformanceCounter( ( LARGE_INTEGER* )&nEnd );

	CProfileStatistic::GetInstance()->EndProfile( m_szToken, nEnd, m_szFilename, m_nLine );
	return;
}