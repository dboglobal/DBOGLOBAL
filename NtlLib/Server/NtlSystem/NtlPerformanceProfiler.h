#pragma once
//-- include ------------------------------------------------------------------------
#include <string>
#include <map>
#include <deque>
#include "NtlSingleton.h"
#include "NtlMutex.h"
//-----------------------------------------------------------------------------------

using namespace std;

#define PROFILE_OUTPUT_FILENAME		"Profile"

//-----------------------------------------------------------------------------------------
//
class CProfileStatistic
	: public CNtlSingleton<CProfileStatistic>
{
protected:
	struct PROFILEUNIT
	{
	public:
		unsigned __int64 m_uiTotalCycle;
		unsigned __int64 m_uiMinCycle;
		unsigned __int64 m_uiMaxCycle;
		unsigned __int32 m_uiCalledCount;
		unsigned __int32 m_uiTurnCount_TotalCycle;
		unsigned __int32 m_uiTurnCount_CalledCount;

	protected:
		string m_strFilename;
		unsigned __int32 m_uiLine;

	public:
		PROFILEUNIT()
			: m_uiTotalCycle( 0 )
			, m_uiMinCycle( 0 )
			, m_uiMaxCycle( 0 )
			, m_uiCalledCount( 0 )	
			, m_strFilename( )
			, m_uiLine( 0 )
			, m_uiTurnCount_TotalCycle( 0 )
			, m_uiTurnCount_CalledCount( 0 )
		{
		}

		PROFILEUNIT( const char * szFilename, unsigned __int16 line, unsigned __int64 cycle )
			: m_uiTotalCycle( cycle )
			, m_uiMinCycle( cycle )
			, m_uiMaxCycle( cycle )
			, m_uiCalledCount( 1 )
			, m_strFilename( szFilename )
			, m_uiLine( line )
			, m_uiTurnCount_TotalCycle( 0 )
			, m_uiTurnCount_CalledCount( 0 )
		{
		}

		~PROFILEUNIT()
		{
		}
	};

	struct FUNC
	{
		unsigned __int64 m_uiStartCycle;
		unsigned __int64 m_uiChildCycle;
	};

public:
	typedef map< string,PROFILEUNIT >		PROFILES_MAP;
	typedef PROFILES_MAP::iterator			PROFILES_ITER;
	typedef PROFILES_MAP::value_type		PROFILES_VALUE;

	typedef deque< FUNC >					FUNC_DEQUE;

public:
	CProfileStatistic( void );
	~CProfileStatistic( void );

	void BeginProfile( unsigned __int64 startCycle );
	void EndProfile( const char* strToken, unsigned __int64 endCycle, const char* strFile, unsigned __int16 nLine );
	void Save( void );
	void SetFileName( const char* pszFileName ) { m_strFilename = pszFileName; return; }
	void CheckCpuCycle( void );
	void ClearProfile( void );

private:
	unsigned __int64	m_uCpuCycle;
	PROFILES_MAP		m_mapProfiles;
	FUNC_DEQUE			m_dequeFunc;
	string				m_strFilename;
	char				m_szFileNameTime[256];
	CNtlMutex			m_mutex;
};

//-----------------------------------------------------------------------------------------
//
class CProfile
{

public:
	CProfile( const char* szToken, const char* szFile );
	CProfile( const char* szToken, const char* szFile, unsigned __int16 nLine );	
	virtual ~CProfile( void );

public:
	void Init( const char * szToken, const char * szFile, unsigned __int16 nLine );

public:
	void Create( unsigned __int16 nLine );
	void Destroy( void );

private:
	unsigned __int64	m_cycle;
	const char*			m_szToken;
	const char*			m_szFilename;
	unsigned __int16	m_nLine;
	CNtlMutex			m_mutex;
};

//-----------------------------------------------------------------------------------------
// Profile on the head of a desired function to the PROFILE ("your name")
// Write this only being put


//-----------------------------------------------------------------------------------------
//  [8/8/2009 zeroera] : CAUTION: Do you use will cause a decrease in performance when using the profiler, turning only when necessary.
//-----------------------------------------------------------------------------------------
//
// #define __USE_PERFORMANCE_PROFILER__
//
//-----------------------------------------------------------------------------------------

#ifdef __USE_PERFORMANCE_PROFILER__
	#define PROFILE(token) CProfile _tagPROFILE_CLASS(token, __FILE__, unsigned __int16(__LINE__) )
#else
	#define PROFILE(token) ((void)0)
#endif

//-----------------------------------------------------------------------------------
//
#ifdef __USE_PERFORMANCE_PROFILER__
	#define PROFILE_FLUSH CProfileStatistic::GetInstance()->Save()
#else
	#define PROFILE_FLUSH
#endif