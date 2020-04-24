#pragma warning( disable : 4996 )

#include <Windows.h>
#include "NtlZip.h"

#include "zip.h"
#include "iowin32.h"


bool GetLastModified( const char* szPath, SYSTEMTIME& sysTime, bool bLocalTime )
{
	memset( &sysTime, 0, sizeof(SYSTEMTIME) );

	DWORD dwAttr = ::GetFileAttributes( szPath );

	if ( dwAttr == 0xFFFFFFFF ) return false;

	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile( (char*)szPath, &findFileData );

	if ( hFind == INVALID_HANDLE_VALUE ) return false;

	FindClose( hFind );

	FILETIME ft = findFileData.ftLastWriteTime;

	if ( bLocalTime ) FileTimeToLocalFileTime( &findFileData.ftLastWriteTime, &ft );
	FileTimeToSystemTime( &ft, &sysTime );

	return true;
}


bool CNtlZip::ZipFile( const char* szFilePath )
{
	char szDrive[ZIP_BUFFER_SIZE], szFolder[ZIP_BUFFER_SIZE], szName[ZIP_BUFFER_SIZE];
	_splitpath( szFilePath, szDrive, szFolder, szName, 0 );

	char szZipPath[ZIP_BUFFER_SIZE];
	_makepath( szZipPath, szDrive, szFolder, szName, "zip" );

	CNtlZip zip;
	if ( zip.OpenZip(szZipPath, false) ) return zip.AddFileToZip( szFilePath, false );

	return false;
}

bool CNtlZip::ZipFolder( const char* szFilePath, bool bIgnoreFilePath )
{
	char szDrive[ZIP_BUFFER_SIZE], szFolder[ZIP_BUFFER_SIZE], szName[ZIP_BUFFER_SIZE];
	_splitpath(szFilePath, szDrive, szFolder, szName, 0);

	char szZipPath[ZIP_BUFFER_SIZE];
	_makepath(szZipPath, szDrive, szFolder, 0, 0);

	char szZipFullPath[ZIP_BUFFER_SIZE];
	_makepath(szZipFullPath, szDrive, szFolder, szName, "zip");

	CNtlZip zip;

	if ( zip.OpenZip(szZipFullPath, false) ) return zip.AddFolderToZip(szZipPath, bIgnoreFilePath);

	return false;
}

CNtlZip::CNtlZip( const char* szFilePath, const char* szRootFolder, bool bAppend )
: m_hZipFile( 0 )
{
	CloseZip();

	if ( szFilePath ) OpenZip( szFilePath, szRootFolder, bAppend );
}

CNtlZip::~CNtlZip( void )
{
	CloseZip();
}

bool CNtlZip::OpenZip( const char* szFilePath, const char* szRootFolder, bool bAppend )
{
	CloseZip();

	if ( !szFilePath || !lstrlen(szFilePath) ) return false;

	char szFullPath[ZIP_BUFFER_SIZE];

	if ( !GetFullPathName( szFilePath, ZIP_BUFFER_SIZE, szFullPath, 0 ) ) return false;

	if ( bAppend && ::GetFileAttributes(szFullPath) == 0xffffffff ) bAppend = false;

	m_hZipFile = zipOpen( szFullPath, bAppend ? 1 : 0 );

	if ( m_hZipFile )
	{
		if ( !szRootFolder )
		{
			char szDrive[ZIP_BUFFER_SIZE], szFolder[ZIP_BUFFER_SIZE];
			_splitpath( szFullPath, szDrive, szFolder, 0, 0 );
			_makepath( m_szRootFolder, szDrive, szFolder, 0, 0 );
		}
		else if ( lstrlen(szRootFolder) )
		{
			_makepath( m_szRootFolder, 0, szRootFolder, "", 0 );
		}
	}

	return ( m_hZipFile != 0 );
}

bool CNtlZip::CloseZip( void )
{
	int nRet = m_hZipFile ? zipClose(m_hZipFile, 0) : ZIP_OK;

	m_hZipFile = 0;
	m_szRootFolder[0] = 0;
	memset( &m_sInfo, 0, sizeof(m_sInfo) );

	return (nRet == ZIP_OK);
}

void CNtlZip::GetFileInfo( sZ_FILEINFO& sInfo )
{
	sInfo = m_sInfo;
}

bool CNtlZip::AddFileToZip( const char* szFilePath, bool bIgnoreFilePath )
{
	if ( !m_hZipFile ) return false;

	if ( !bIgnoreFilePath && strstr(szFilePath, "..\\") == szFilePath ) return false;

	bool bFullPath = (strchr(szFilePath, ':') != 0);

	char szFullFilePath[ZIP_BUFFER_SIZE];
	lstrcpy( szFullFilePath, szFilePath );
	PrepareSourcePath(szFullFilePath);

	char szFileName[ZIP_BUFFER_SIZE] = "";

	if ( bIgnoreFilePath )
	{
		char szName[ZIP_BUFFER_SIZE], szExt[ZIP_BUFFER_SIZE];
		_splitpath( szFilePath, 0, 0, szName, szExt );
		_makepath( szFileName, 0, 0, szName, szExt );
	}
	else if ( bFullPath )
	{
		if ( 0 != _strnicmp(szFilePath, m_szRootFolder, lstrlen(m_szRootFolder)) ) return false;

		lstrcpy( szFileName, szFilePath + lstrlen(m_szRootFolder) );
	}
	else
	{
		if ( strstr(szFilePath, ".\\") == szFilePath )
		{
			lstrcpy(szFileName, szFilePath + 2);
		}
		else
		{
			lstrcpy(szFileName, szFilePath);
		}
	}

	zip_fileinfo zfi;
	zfi.internal_fa = 0;
	zfi.external_fa = ::GetFileAttributes(szFullFilePath);
	
	SYSTEMTIME st;
	GetLastModified(szFullFilePath, st, true);

	zfi.dosDate = 0;
	zfi.tmz_date.tm_year = st.wYear;
	zfi.tmz_date.tm_mon = st.wMonth - 1;
	zfi.tmz_date.tm_mday = st.wDay;
	zfi.tmz_date.tm_hour = st.wHour;
	zfi.tmz_date.tm_min = st.wMinute;
	zfi.tmz_date.tm_sec = st.wSecond;
	
	// load input file
	HANDLE hInputFile = ::CreateFile( szFullFilePath, 
									  GENERIC_READ,
									  0,
									  0,
									  OPEN_EXISTING,
									  FILE_ATTRIBUTE_READONLY,
									  0 );
	if ( hInputFile == INVALID_HANDLE_VALUE ) return false;

	int nRet = zipOpenNewFileInZip( m_hZipFile, 
									szFileName,
									&zfi, 
									0, 
									0,
									0,
									0, 
									0,
									Z_DEFLATED,
									Z_DEFAULT_COMPRESSION );
	if ( nRet == ZIP_OK )
	{
		m_sInfo.nFileCount++;

		char pBuffer[ZIP_BUFFER_SIZE];
		DWORD dwBytesRead = 0, dwFileSize = 0;

		while ( nRet == ZIP_OK && ::ReadFile( hInputFile, pBuffer, ZIP_BUFFER_SIZE, &dwBytesRead, 0 ) )
		{
			dwFileSize += dwBytesRead;

			if ( dwBytesRead )
			{
				nRet = zipWriteInFileInZip( m_hZipFile, pBuffer, dwBytesRead );
			}
			else
			{
				break;
			}
		}

		m_sInfo.dwUncompressedSize += dwFileSize;
	}

	zipCloseFileInZip( m_hZipFile );

	::CloseHandle( hInputFile );

	return (nRet == ZIP_OK);
}

bool CNtlZip::AddFileToZip( const char* szFilePath, const char* pWriteBuf, int nWriteSize, bool bIgnoreFilePath )
{
	if ( !m_hZipFile ) return false;

	if ( !bIgnoreFilePath && strstr(szFilePath, "..\\") == szFilePath ) return false;

	bool bFullPath = (strchr(szFilePath, ':') != 0);

	char szFileName[ZIP_BUFFER_SIZE] = "";

	if ( bIgnoreFilePath )
	{
		char szName[ZIP_BUFFER_SIZE], szExt[ZIP_BUFFER_SIZE];
		_splitpath( szFilePath, 0, 0, szName, szExt );
		_makepath( szFileName, 0, 0, szName, szExt );
	}
	else if ( bFullPath )
	{
		if ( 0 != _strnicmp( szFilePath, m_szRootFolder, lstrlen(m_szRootFolder) ) ) return false;

		lstrcpy( szFileName, szFilePath + lstrlen(m_szRootFolder) );
	}
	else
	{
		if ( strstr( szFilePath, ".\\" ) == szFilePath )
		{
			lstrcpy( szFileName, szFilePath + 2 );
		}
		else
		{
			lstrcpy( szFileName, szFilePath );
		}
	}

	zip_fileinfo zfi;
	zfi.internal_fa = 0;
	zfi.external_fa = FILE_ATTRIBUTE_NORMAL;

	SYSTEMTIME st;
	GetSystemTime( &st );

	zfi.dosDate = 0;
	zfi.tmz_date.tm_year = st.wYear;
	zfi.tmz_date.tm_mon = st.wMonth - 1;
	zfi.tmz_date.tm_mday = st.wDay;
	zfi.tmz_date.tm_hour = st.wHour;
	zfi.tmz_date.tm_min = st.wMinute;
	zfi.tmz_date.tm_sec = st.wSecond;

	int nRet = zipOpenNewFileInZip( m_hZipFile,
									szFileName,
									&zfi,
									0,
									0,
									0,
									0,
									0,
									Z_DEFLATED,
									Z_DEFAULT_COMPRESSION );
	if ( nRet == ZIP_OK )
	{
		m_sInfo.nFileCount++;

		if ( ZIP_OK != zipWriteInFileInZip( m_hZipFile, pWriteBuf, nWriteSize ) ) return false;

		m_sInfo.dwUncompressedSize += nWriteSize;
	}

	zipCloseFileInZip( m_hZipFile );

	return (nRet == ZIP_OK);
}

bool CNtlZip::AddFileToZip( const char* szFilePath, const char* szRelFolderPath )
{
	if ( !m_hZipFile ) return false;

	if ( szRelFolderPath && strchr(szRelFolderPath, ':') ) return false;

	char szFullFilePath[ZIP_BUFFER_SIZE];
	lstrcpy( szFullFilePath, szFilePath );
	PrepareSourcePath( szFullFilePath );

	zip_fileinfo zfi;
	zfi.internal_fa = 0;
	zfi.external_fa = ::GetFileAttributes(szFullFilePath);
	
	SYSTEMTIME st;
	GetLastModified(szFullFilePath, st, true);

	zfi.dosDate = 0;
	zfi.tmz_date.tm_year = st.wYear;
	zfi.tmz_date.tm_mon = st.wMonth - 1;
	zfi.tmz_date.tm_mday = st.wDay;
	zfi.tmz_date.tm_hour = st.wHour;
	zfi.tmz_date.tm_min = st.wMinute;
	zfi.tmz_date.tm_sec = st.wSecond;

	// load input file
	HANDLE hInputFile = ::CreateFile( szFullFilePath,
									  GENERIC_READ,
									  0,
									  0,
									  OPEN_EXISTING,
									  FILE_ATTRIBUTE_READONLY,
									  0 );
	if ( hInputFile == INVALID_HANDLE_VALUE ) return false;

	char szName[ZIP_BUFFER_SIZE], szExt[ZIP_BUFFER_SIZE];
	_splitpath(szFilePath, 0, 0, szName, szExt);

	char szFileName[ZIP_BUFFER_SIZE];
	_makepath(szFileName, 0, szRelFolderPath, szName, szExt);

	int nRet = zipOpenNewFileInZip( m_hZipFile, 
									szFileName + (szFileName[0] == '\\' ? 1 : 0),
									&zfi, 
									0, 
									0,
									0,
									0, 
									0,
									Z_DEFLATED,
									Z_DEFAULT_COMPRESSION );

	if ( nRet == ZIP_OK )
	{
		m_sInfo.nFileCount++;

		char pBuffer[ZIP_BUFFER_SIZE];
		DWORD dwBytesRead = 0, dwFileSize = 0;

		while ( nRet == ZIP_OK && ::ReadFile( hInputFile, pBuffer, ZIP_BUFFER_SIZE, &dwBytesRead, 0 ) )
		{
			dwFileSize += dwBytesRead;

			if ( dwBytesRead )
			{
				nRet = zipWriteInFileInZip( m_hZipFile, pBuffer, dwBytesRead );
			}
			else
			{
				break;
			}
		}

		m_sInfo.dwUncompressedSize += dwFileSize;
	}

	zipCloseFileInZip( m_hZipFile );

	::CloseHandle( hInputFile );

	return (nRet == ZIP_OK);
}

bool CNtlZip::AddFileToZip( const char* szFilePath, const char* pWriteBuf, int nWriteSize, const char* szRelFolderPath )
{
	if ( !m_hZipFile ) return false;

	if ( szRelFolderPath && strchr(szRelFolderPath, ':') ) return false;

	zip_fileinfo zfi;
	zfi.internal_fa = 0;
	zfi.external_fa = FILE_ATTRIBUTE_NORMAL;

	SYSTEMTIME st;
	GetSystemTime( &st );

	zfi.dosDate = 0;
	zfi.tmz_date.tm_year = st.wYear;
	zfi.tmz_date.tm_mon = st.wMonth - 1;
	zfi.tmz_date.tm_mday = st.wDay;
	zfi.tmz_date.tm_hour = st.wHour;
	zfi.tmz_date.tm_min = st.wMinute;
	zfi.tmz_date.tm_sec = st.wSecond;

	char szName[ZIP_BUFFER_SIZE], szExt[ZIP_BUFFER_SIZE];
	_splitpath(szFilePath, 0, 0, szName, szExt);

	char szFileName[ZIP_BUFFER_SIZE];
	_makepath(szFileName, 0, szRelFolderPath, szName, szExt);

	int nRet = zipOpenNewFileInZip( m_hZipFile,
									szFileName + (szFileName[0] == '\\' ? 1 : 0),
									&zfi,
									0,
									0,
									0,
									0,
									0,
									Z_DEFLATED,
									Z_DEFAULT_COMPRESSION );
	if ( nRet == ZIP_OK )
	{
		m_sInfo.nFileCount++;

		if ( ZIP_OK != zipWriteInFileInZip( m_hZipFile, pWriteBuf, nWriteSize ) ) return false;

		m_sInfo.dwUncompressedSize += nWriteSize;
	}

	zipCloseFileInZip( m_hZipFile );

	return (nRet == ZIP_OK);
}

bool CNtlZip::AddFolderToZip( const char* szFolderPath, bool bIgnoreFilePath )
{
	if ( !m_hZipFile ) return false;

	m_sInfo.nFolderCount++;

	char szFullPath[ZIP_BUFFER_SIZE];
	lstrcpy(szFullPath, szFolderPath);
	PrepareSourcePath(szFullPath);

	zip_fileinfo zfi;
	zfi.internal_fa = 0;
	zfi.external_fa = ::GetFileAttributes(szFullPath);
	
	SYSTEMTIME st;
	GetLastModified(szFullPath, st, true);
	
	zfi.dosDate = 0;
	zfi.tmz_date.tm_year = st.wYear;
	zfi.tmz_date.tm_mon = st.wMonth - 1;
	zfi.tmz_date.tm_mday = st.wDay;
	zfi.tmz_date.tm_hour = st.wHour;
	zfi.tmz_date.tm_min = st.wMinute;
	zfi.tmz_date.tm_sec = st.wSecond;
	
	char szFolderName[ZIP_BUFFER_SIZE] = "";
	
	if ( bIgnoreFilePath )
	{
		_splitpath(szFullPath, 0, 0, szFolderName, 0);
	}
	else
	{
		if ( 0 != _strnicmp(szFullPath, m_szRootFolder, lstrlen(m_szRootFolder)) ) return false;

		lstrcpy(szFolderName, szFullPath + lstrlen(m_szRootFolder));
	}
	
	lstrcat(szFolderName, "\\");
	
	int nRet = zipOpenNewFileInZip( m_hZipFile,
									szFolderName,
									&zfi,
									0,
									0,
									0,
									0,
									0,
									Z_DEFLATED,
									Z_DEFAULT_COMPRESSION );
	
	zipCloseFileInZip( m_hZipFile );

	char szDrive[ZIP_BUFFER_SIZE], szFolder[ZIP_BUFFER_SIZE], szName[ZIP_BUFFER_SIZE];
	_splitpath(szFullPath, szDrive, szFolder, szName, 0);
	lstrcat(szFolder, szName);

	char szSearchSpec[ZIP_BUFFER_SIZE];
	_makepath(szSearchSpec, szDrive, szFolder, "*", "*");

	WIN32_FIND_DATA finfo;
	HANDLE hSearch = FindFirstFile(szSearchSpec, &finfo);

	if ( hSearch != INVALID_HANDLE_VALUE )
	{
		char szItem[ZIP_BUFFER_SIZE];

		do 
		{
			if ( finfo.cFileName[0] != '.' ) 
			{
				_makepath(szItem, szDrive, szFolder, finfo.cFileName, 0);
				
				if ( finfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					AddFolderToZip( szItem, bIgnoreFilePath );
				}
				else 
				{
					AddFileToZip( szItem, bIgnoreFilePath );
				}
			}
		} 
		while ( FindNextFile( hSearch, &finfo ) );

		FindClose( hSearch );
	}

	return true;
}

void CNtlZip::PrepareSourcePath( char* szPath )
{
	bool bFullPath = (strchr(szPath, ':') != 0);

	if ( !bFullPath )
	{
		char szTemp[ZIP_BUFFER_SIZE];
		lstrcpy( szTemp, szPath );
		_makepath( szPath, 0, m_szRootFolder, szTemp, 0 );
	}
}
