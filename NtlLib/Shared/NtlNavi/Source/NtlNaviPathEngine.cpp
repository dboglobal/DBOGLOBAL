#include "precomp_navi.h"
#include "NtlNaviPathEngine.h"
#include "NtlNaviLog.h"


class CPEErrorHandler : public iErrorHandler
{
public:
	virtual eAction handle(const char* type, const char* description, const char *const* attributes)
	{
		std::string displayString(description);
		if(attributes && *attributes)
		{
			displayString += "\nAttributes:\n";
			do
			{
				displayString += "attribute ";
				displayString += *attributes;
				displayString += "=";
				attributes++;
				displayString += *attributes;
				displayString += "\n";
				attributes++;
			}
			while(*attributes);
		}

		char title[1000];
		sprintf_s(title, 1000, "Error, type: %s", type);

		CNtlNaviLog::GetInstance()->Log( title );
		CNtlNaviLog::GetInstance()->Log( displayString.c_str() );
		
		return iErrorHandler::CONTINUE;
	}
};

static CPEErrorHandler g_clErrorHandler;


CPathDataOutStream::CPathDataOutStream( const char* pFileName )
{
	fopen_s( &m_pFile, pFileName, "wb" );
}

CPathDataOutStream::~CPathDataOutStream( void )
{
	if ( m_pFile )
	{
		fclose( m_pFile );
		m_pFile = NULL;
	}
}

void CPathDataOutStream::put( const char* data, tUnsigned32 dataSize )
{
	if ( m_pFile )
	{
		fwrite( data, dataSize, 1, m_pFile );
	}
}



CNtlNaviPathEngine* CNtlNaviPathEngine::GetInstance( void )
{
	static CNtlNaviPathEngine g_clPathEngine;
	return &g_clPathEngine;
}

CNtlNaviPathEngine::CNtlNaviPathEngine( void )
{
	m_hInstance = NULL;
	m_pPathEngine = NULL;
}

CNtlNaviPathEngine::~CNtlNaviPathEngine( void )
{
	Delete();
}

iPathEngine* CNtlNaviPathEngine::GetPathEngine( void )
{
	return m_pPathEngine;
}

bool CNtlNaviPathEngine::Create( const char* pPathDllName )
{
	if ( NULL == pPathDllName )
	{
		return true;
	}

	m_hInstance = LoadLibrary( pPathDllName );

	if ( NULL == m_hInstance )
	{
		return false;
	}

	FARPROC procAddr;
	procAddr = GetProcAddress( m_hInstance, (LPCSTR)1 );

	if ( NULL == procAddr )
	{
		m_hInstance = NULL;
		return false;
	}

	tGetInterfaceFunction getInterfaceFunction = (tGetInterfaceFunction) procAddr;

	m_pPathEngine = getInterfaceFunction( this );

	if ( NULL == m_pPathEngine )
	{
		m_hInstance = NULL;
		return false;
	}

	m_pPathEngine->setErrorHandler( &g_clErrorHandler );

	return true;
}

void CNtlNaviPathEngine::Delete( void )
{
	if ( m_pPathEngine )
	{
		m_pPathEngine->deleteAllObjects();
		FreeLibrary( m_hInstance );

		m_hInstance = NULL;
		m_pPathEngine = NULL;
	}
}

iErrorHandler::eAction CNtlNaviPathEngine::handle( const char* type, const char* description, const char *const* attributes )
{
	std::string displayString(description);
	if(attributes && *attributes)
	{
		displayString += "\nAttributes:\n";
		do
		{
			displayString += "attribute ";
			displayString += *attributes;
			displayString += "=";
			attributes++;
			displayString += *attributes;
			displayString += "\n";
			attributes++;
		}
		while(*attributes);
	}

	char title[1000];
	sprintf_s(title, 1000, "Error, type: %s", type);

	CNtlNaviLog::GetInstance()->Log( title );
	CNtlNaviLog::GetInstance()->Log( displayString.c_str() );
	return CONTINUE;
}
