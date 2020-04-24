#ifndef _NTL_PATHENGINE_LOG_H_
#define _NTL_PATHENGINE_LOG_H_

#include "..\PathEngine\i_pathengine.h"

class CNtlPathEngineLog : public iErrorHandler
{
	// Constructions and Destructions
public:
	CNtlPathEngineLog( void );

	// SingleTon
	static CNtlPathEngineLog*				GetInstance( void );

	// Error Handler
	eAction handle(const char* pchErrorType, const char* pchErrorString, const char* const* attributes = 0);

	// Log
	void	Log( const char* pFormat, ... );
};


#endif