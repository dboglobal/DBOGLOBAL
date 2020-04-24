#include "Precomp_NtlPathEngine.h"
#include "NtlPathEngineLog.h"

// Platform
#include <stdio.h>
#include <time.h>
#include <stdarg.h>

// NtlPathEngine
#include "NtlPathEngineDef.h"

CNtlPathEngineLog::CNtlPathEngineLog( void )
{
	
}

CNtlPathEngineLog* CNtlPathEngineLog::GetInstance( void )
{
	static CNtlPathEngineLog instance;
	return &instance;
}

iErrorHandler::eAction CNtlPathEngineLog::handle( const char* pchErrorType, const char* pchErrorString, const char* const* attributes )
{	
	// Log
	Log( "%s\t: %s (%s)\n", pchErrorType, pchErrorString, attributes );

	// Error가 Handling 되어도 어플리케이션은 지속적으로 돌린다.
	return iErrorHandler::CONTINUE;
}

void CNtlPathEngineLog::Log( const char* pFormat, ... )
{
	FILE *fp = NULL;

#if defined(_MSC_VER) && (_MSC_VER >= 1400)
	if(fopen_s(&fp, NTLPE_LOG_FILENAME, "a+") != 0)
		return;
#else
	fp = fopen(NTLPE_LOG_FILENAME, "a+");
	if(fp == NULL)
		return;
#endif

	// Time
	char acBuffer[256];
	
	tm tmData;
	time_t the_time;

	time(&the_time);	
	localtime_s(&tmData, &the_time);

	strftime(acBuffer, 256, "[%H:%M:%S %p]\t", &tmData);
	fprintf_s( fp, acBuffer );

	va_list args;
	va_start(args, pFormat);
	vfprintf_s(fp, pFormat, args);
	va_end(args);

	fprintf_s(fp, "\n" );
	
	fclose(fp);
}