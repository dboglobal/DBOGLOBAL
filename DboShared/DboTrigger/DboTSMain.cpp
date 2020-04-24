#include "precomp_trigger.h"
#include "DboTSMain.h"
#include "DboTSEntityFactory.h"
#include "DboTSCtrlFactory.h"
#include "DboTSUIFactory.h"
#include "DboTSQAgency.h"
#include "DboTSQRecv.h"
#include "NtlUnzip.h"
#include "zip.h"
#include "NtlCipher.h"


/** 
	Client main
*/


NTL_TS_IMPLEMENT_RTTI( CDboTSMain, CNtlTSMain )


CDboTSMain::CDboTSMain( void )
: m_pLog( 0 ),
  m_pEntityFactory( 0 ),
  m_pCtrlFactory( 0 ),
  m_pUIFactory( 0 )
{
}

CDboTSMain::~CDboTSMain( void )
{
}

bool CDboTSMain::Create( void )
{
	// log
	if ( !LoadLog() )
	{
		Delete();
		return false;
	}

	// factory
	if ( !LoadFactories() )
	{
		Delete();
		return false;
	}

	// loading scripts
	if ( !LoadScripts() )
	{
		Delete();
		return false;
	}

	// loading event mapper
	if ( !LoadEventMappers() )
	{
		Delete();
		return false;
	}

	// loading receiver
	if ( !LoadRecv() )
	{
		Delete();
		return false;
	}

	return true;
}

void CDboTSMain::Delete( void )
{
	UnloadRecv();
	UnloadEventMappers();
	UnloadAllScripts();
	UnloadFactories();
	UnloadLog();
}

CDboTSMain::hashdef_EVT_MAPPER_LIST& CDboTSMain::GetEventMapper( void )
{
	return m_defEvtMapper;
}

CNtlTSEvtMapper* CDboTSMain::FindEventMapper( const std::string& strMapper )
{
	hashdef_EVT_MAPPER_LIST::iterator it = m_defEvtMapper.find( strMapper );
	if ( it == m_defEvtMapper.end() ) return 0;
	return it->second;
}

void CDboTSMain::DeleteAgency( CNtlTSAgency*& pTSAgency )
{
	if ( pTSAgency && m_pUIFactory )
	{
		m_pUIFactory->DeleteObj( (CNtlTSUIObject*&)pTSAgency );
	}
}

bool CDboTSMain::LoadLog( void )
{
	UnloadLog();

	// 기본 로고를 생성한다
	m_pLog = new CNtlTSLog;
	CNtlTSLog::Register( m_pLog );

	return true;
}

void CDboTSMain::UnloadLog( void )
{
	// 기본 로고를 제거한다
	if ( m_pLog )
	{
		CNtlTSLog::Unregister();
		delete m_pLog;
		m_pLog = 0;
	}
}

bool CDboTSMain::LoadFactories( void )
{
	UnloadFactories();

	// Entity factory 생성
	m_pEntityFactory = new CDboTSEntityFactory;
	m_pEntityFactory->RegisterEntityType();

	// Control factory 생성
	m_pCtrlFactory = new CDboTSCtrlFactory;
	m_pCtrlFactory->RegisterCtrlType();

	// UI factory 생성
	m_pUIFactory = new CDboTSUIFactory;
	m_pUIFactory->RegisterUIType();

	return true;
}

void CDboTSMain::UnloadFactories( void )
{
	delete m_pUIFactory;
	m_pUIFactory = 0;

	delete m_pCtrlFactory;
	m_pCtrlFactory = 0;

	delete m_pEntityFactory;
	m_pEntityFactory = 0;

}

bool CDboTSMain::LoadScripts( void )
{
	return true;
}

void CDboTSMain::UnloadAllScripts( void )
{
}

bool CDboTSMain::LoadEventMappers( void )
{
	return true;
}

void CDboTSMain::UnloadEventMappers( void )
{
}

bool CDboTSMain::LoadRecv( void )
{

	return true;
}

void CDboTSMain::UnloadRecv( void )
{

}

bool CDboTSMain::LoadTSPath( std::string strPath, mapdef_TLIST& defTList )
{
	if ( '\\' != strPath[strPath.size()-1] ) strPath += "\\";

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	std::string strSearchSpec( strPath + "*.t" );

	hFind = FindFirstFile( strSearchSpec.c_str(), &FindFileData );
	if ( INVALID_HANDLE_VALUE == hFind )
	{
		printf( "Can not find the scripts to load. Info[%s]. [%s] \n", strPath.c_str(), TS_CODE_TRACE() );
		return false;
	}

	do 
	{
		if ( FILE_ATTRIBUTE_DIRECTORY != (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			 '.' != FindFileData.cFileName[0] )
		{
			LoadTriggerObject( strPath, FindFileData.cFileName, defTList );
		}
	}
	while ( FindNextFile( hFind, &FindFileData ) != 0 );

	FindClose( hFind );

	return true;
}

bool CDboTSMain::LoadTSZip( std::string strFile, mapdef_TLIST& defTList )
{
	char szFPath[_MAX_DIR];
	_splitpath_s( strFile.c_str(), 0, 0, szFPath, _MAX_DIR, 0, 0, 0, 0 );
	std::string strPath = szFPath;
	if ( '\\' != strPath[strPath.size()-1] ) strPath += "\\";

	CNtlUnzip clZip;
	clZip.OpenZip( strFile.c_str() );

	if ( !clZip.GotoFirstFile() ) return false;

	unsigned int MAX_BUFFER_SIZE = 102400;
	char* pBuf = new char[MAX_BUFFER_SIZE];

	sUZ_FILEINFO sInfo;
	do 
	{
		clZip.GetFileInfo( sInfo );

		if ( sInfo.bFolder ) continue;

		int nNameLength = (int)strlen( sInfo.szFileName );
		if ( nNameLength <= 2 ) continue;
		if ( '.' != sInfo.szFileName[nNameLength-2] ||
			 't' != sInfo.szFileName[nNameLength-1] )
			 continue;

		if ( MAX_BUFFER_SIZE < sInfo.dwUncompressedSize )
		{
			MAX_BUFFER_SIZE = sInfo.dwUncompressedSize + 1;
			delete [] pBuf;
			pBuf = new char[MAX_BUFFER_SIZE];
		}

		if ( !clZip.ReadFileData( pBuf, sInfo.dwUncompressedSize ) )
		{
			CNtlTSLog::Log( "Load TS compressed file. Info[%s]. [%s]", sInfo.szFileName, TS_CODE_TRACE() );
		}
		else
		{
			LoadTriggerObject( strPath, sInfo.szFileName, pBuf, sInfo.dwUncompressedSize, defTList );
		}
	}
	while ( clZip.GotoNextFile() );

	delete [] pBuf;

	return true;
}

bool CDboTSMain::LoadTSCryptoData( std::string strFile, mapdef_TLIST& defTList )
{
	char szFPath[_MAX_DIR];
	_splitpath_s( strFile.c_str(), 0, 0, szFPath, _MAX_DIR, 0, 0, 0, 0 );
	std::string strPath = szFPath;
	if ( '\\' != strPath[strPath.size()-1] ) strPath += "\\";

	// 파일 로딩
	FILE* pFile;
	fopen_s( &pFile, strFile.c_str(), "rb" );
	if(NULL == pFile)
	{
		return false;
	}

	fseek( pFile, 0, SEEK_END );
	int nReadSize = ftell( pFile ) - 4;
	fseek( pFile, 0, SEEK_SET );
	char* pReadBuf = new char[nReadSize];
	int nOriginSize;
	fread( &nOriginSize, sizeof(int), 1, pFile );
	fread( pReadBuf, 1, nReadSize, pFile );
	fclose( pFile );

	// 암호화를 푼다
	std::string strKey = "dnfldbofmftkfkdgowntpdy"; // OLD tddkaghghkqlalfqjsghdlqslek
	CNtlCipher Cipher;
	Cipher.SetKey( DES_CIPHER, strKey.c_str(), (int)strKey.size() );
	char* pDecryptBuf = new char[nReadSize+512];
	ZeroMemory( pDecryptBuf, nReadSize+512 );
	Cipher.Decrypt( pReadBuf, nReadSize, pDecryptBuf, nReadSize+512 );

	CNtlUnzip clZip;
	clZip.OpenZip( strFile.c_str(), pDecryptBuf, nOriginSize );

	if ( !clZip.GotoFirstFile() )
	{
		delete [] pReadBuf;
		delete [] pDecryptBuf;

		return false;
	}

	unsigned int MAX_BUFFER_SIZE = 1024000;
	char* pBuf = new char[MAX_BUFFER_SIZE];

	sUZ_FILEINFO sInfo;
	do 
	{
		clZip.GetFileInfo( sInfo );

		if ( sInfo.bFolder ) continue;

		int nNameLength = (int)strlen( sInfo.szFileName );
		if ( nNameLength <= 2 ) continue;
		if ( '.' != sInfo.szFileName[nNameLength-2] ||
			't' != sInfo.szFileName[nNameLength-1] )
			continue;

		if ( MAX_BUFFER_SIZE < sInfo.dwUncompressedSize )
		{
			MAX_BUFFER_SIZE = sInfo.dwUncompressedSize + 1;
			delete [] pBuf;
			pBuf = new char[MAX_BUFFER_SIZE];
		}

		if ( !clZip.ReadFileData( pBuf, sInfo.dwUncompressedSize ) )
		{
			CNtlTSLog::Log( "Load TS compressed file. Info[%s]. [%s]", sInfo.szFileName, TS_CODE_TRACE() );
		}
		else
		{
			LoadTriggerObject( strPath, sInfo.szFileName, pBuf, sInfo.dwUncompressedSize, defTList );
		}
	}
	while ( clZip.GotoNextFile() );

	delete [] pBuf;

	delete [] pReadBuf;
	delete [] pDecryptBuf;

	return true;
}

bool CDboTSMain::LoadTSCryptoData_UnZip( std::string strFile, CNtlUnzip* pclUnzip )
{
	char szFPath[_MAX_DIR];
	_splitpath_s( strFile.c_str(), 0, 0, szFPath, _MAX_DIR, 0, 0, 0, 0 );
	std::string strPath = szFPath;
	if ( '\\' != strPath[strPath.size()-1] ) strPath += "\\";

	// 파일 로딩
	FILE* pFile;
	fopen_s( &pFile, strFile.c_str(), "rb" );
	if(NULL == pFile)
	{
		return false;
	}

	fseek( pFile, 0, SEEK_END );
	int nReadSize = ftell( pFile ) - 4;
	fseek( pFile, 0, SEEK_SET );
	char* pReadBuf = new char[nReadSize];
	int nOriginSize;
	fread( &nOriginSize, sizeof(int), 1, pFile );
	fread( pReadBuf, 1, nReadSize, pFile );
	fclose( pFile );

	// 암호화를 푼다
	std::string strKey = "dnfldbofmftkfkdgowntpdy"; // OLD tddkaghghkqlalfqjsghdlqslek
	CNtlCipher Cipher;
	Cipher.SetKey( DES_CIPHER, strKey.c_str(), (int)strKey.size() );
	char* pDecryptBuf = new char[nReadSize+256];
	ZeroMemory( pDecryptBuf, nReadSize+256 );
	Cipher.Decrypt( pReadBuf, nReadSize, pDecryptBuf, nReadSize+256 );

	if ( !pclUnzip->OpenZip( strFile.c_str(), pDecryptBuf, nOriginSize ) )
	{
		delete [] pReadBuf;
		delete [] pDecryptBuf;

		return false;
    }

    delete [] pReadBuf;
    delete [] pDecryptBuf;

	return true;
}

bool CDboTSMain::LoadTriggerObject( const std::string& strPath, const char* pFileName, mapdef_TLIST& defTList )
{
	std::string strFullName = strPath + pFileName;

	CNtlTSScrStream clStream( m_pEntityFactory, strFullName );
	if ( !clStream.Load() )
	{
		printf( "Loading the trigger script is failed. Info[%s]. [%s]", strFullName.c_str(), TS_CODE_TRACE() );
		return false;
	}

	std::vector<CNtlTSScrObject*>& vecObjList = clStream.GetObjList();
	std::vector<CNtlTSScrObject*>::iterator itObjList = vecObjList.begin();
	for ( ; itObjList != vecObjList.end(); ++itObjList )
	{
		if ( (*itObjList)->IsDerivedClass( "CNtlTSTrigger" ) )
		{
			CNtlTSTrigger* pTriObj = (CNtlTSTrigger*) *itObjList;
			defTList[pTriObj->GetID()] = pTriObj;
		}
		else
		{
			printf( "The root class type must have to be the CNtlTSTrigger class. Info[%s]. [%s]", (*itObjList)->GetClassName(), TS_CODE_TRACE() );
			return false;
		}
	}

	return true;
}
bool CDboTSMain::LoadTriggerObject( const std::string& strPath, const char* pFileName, char* pBuff, int nSize, mapdef_TLIST& defTList )
{
	std::string strFullName = strPath + pFileName;

	if ( 0 == pBuff || 0 == nSize )
	{
		CNtlTSLog::Log( "0 == pBuff || 0 == nSize. Info[%s]. [%s]", strFullName.c_str(), TS_CODE_TRACE() );
		return false;
	}

	CNtlTSScrStream clStream( m_pEntityFactory, strFullName, pBuff, nSize );
	if ( !clStream.Load() )
	{
		CNtlTSLog::Log( "Loading the trigger script is failed. Info[%s]. [%s]", strFullName.c_str(), TS_CODE_TRACE() );
		return false;
	}

	std::vector<CNtlTSScrObject*>& vecObjList = clStream.GetObjList();
	std::vector<CNtlTSScrObject*>::iterator itObjList = vecObjList.begin();
	for ( ; itObjList != vecObjList.end(); ++itObjList )
	{
		if ( (*itObjList)->IsDerivedClass( "CNtlTSTrigger" ) )
		{
			CNtlTSTrigger* pTriObj = (CNtlTSTrigger*) *itObjList;
			defTList[pTriObj->GetID()] = pTriObj;
		}
		else
		{
			CNtlTSLog::Log( "The root class type must have to be the CNtlTSTrigger class. Info[%s]. [%s]", (*itObjList)->GetClassName(), TS_CODE_TRACE() );
			return false;
		}
	}

	return true;
}


bool CDboTSMain::LoadTriggerObjectFromUnZip( NTL_TS_T_ID tID, CNtlUnzip* pclUnzip, mapdef_TLIST& defTList )
{
    char szFileName[128] = {0,};
	sprintf_s( szFileName, "%d.t", tID );

	if ( !pclUnzip->GotoFirstFile() )
	{
		return false;
	}

	if ( !pclUnzip->GotoFile( szFileName ) )
	{
		return false;
	}

	static unsigned int MAX_BUFFER_SIZE = 102400;

	char* pBuf = new char[MAX_BUFFER_SIZE];

	sUZ_FILEINFO sInfo;
	if ( !pclUnzip->GetFileInfo( sInfo ) )
	{
		delete [] pBuf;

		return false;
	}

	int nNameLength = (int)strlen( sInfo.szFileName );

	if ( nNameLength <= 2 ) return false;

	if ( '.' != sInfo.szFileName[nNameLength-2] ||
		 't' != sInfo.szFileName[nNameLength-1] )
	{
		delete [] pBuf;

		return false;
	}

	if ( MAX_BUFFER_SIZE <= sInfo.dwUncompressedSize )
	{
		MAX_BUFFER_SIZE = sInfo.dwUncompressedSize + 1;
		delete [] pBuf;
		pBuf = new char[MAX_BUFFER_SIZE];
	}

	mapdef_TLIST mapTList;

	if ( !pclUnzip->ReadFileData( pBuf, sInfo.dwUncompressedSize ) )
	{
		CNtlTSLog::Log( "Load TS compressed file. Info[%s]. [%s]", sInfo.szFileName, TS_CODE_TRACE() );

		delete [] pBuf;

		return false;
	}
	else
	{
		LoadTriggerObject( "", sInfo.szFileName, pBuf, sInfo.dwUncompressedSize, defTList );
	}

	delete [] pBuf;

	return true;
}