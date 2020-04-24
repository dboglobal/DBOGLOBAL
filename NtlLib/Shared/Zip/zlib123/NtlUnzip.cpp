#pragma warning( disable : 4996 )

#include <Windows.h>
#include "NtlUnzip.h"

#include "unzip.h"
#include "iowin32.h"


bool CNtlUnzip::Unzip( const char* szFileName, const char* szFolder, bool bIgnoreFilePath )
{
	CNtlUnzip clUnzip;

	if ( !clUnzip.OpenZip( szFileName ) ) return false;

	return clUnzip.UnzipTo( szFolder, bIgnoreFilePath );
}


bool CNtlUnzip::CreateFolder( const char* szFolder )
{
	if ( !szFolder || !lstrlen(szFolder) ) return false;

	DWORD dwAttrib = GetFileAttributes( szFolder );

	if ( dwAttrib != 0xffffffff ) return ((dwAttrib & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);

	char* szPath = _strdup(szFolder);
	char* p = strrchr(szPath, '\\');

	if ( p ) 
	{
		*p = '\0';

		if ( !CreateFolder(szPath) )
		{
			free( szPath );
			return false;
		}
		free( szPath );

		if ( !::CreateDirectory( szFolder, 0 ) ) 
			return false;
	}

	return true;
}

bool CNtlUnzip::CreateFilePath( const char* szFilePath )
{
	char* szPath = _strdup(szFilePath);
	char* p = strrchr(szPath,'\\');

	bool bRes = false;

	if ( p )
	{
		*p = '\0';

		bRes = CreateFolder(szPath);
	}

	free( szPath );

	return bRes;
}

bool CNtlUnzip::SetFileModTime( const char* szFilePath, DWORD dwDosDate )
{
	HANDLE hFile = CreateFile( szFilePath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0 );

	if ( !hFile ) return false;

	FILETIME ftm, ftLocal, ftCreate, ftLastAcc, ftLastWrite;

	bool bRes = (GetFileTime(hFile, &ftCreate, &ftLastAcc, &ftLastWrite) == TRUE);
	if (bRes) bRes = (TRUE == DosDateTimeToFileTime((WORD)(dwDosDate >> 16), (WORD)dwDosDate, &ftLocal));
	if (bRes) bRes = (TRUE == LocalFileTimeToFileTime(&ftLocal, &ftm));
	if (bRes) bRes = (TRUE == SetFileTime(hFile, &ftm, &ftLastAcc, &ftm));

	CloseHandle( hFile );

	return bRes;
}


voidpf CNtlUnzip::Open_File_Func( voidpf opaque, const char* filename, int mode )
{
	if ( NULL == opaque ) return 0;
	if ( (mode & ZLIB_FILEFUNC_MODE_READWRITEFILTER) != ZLIB_FILEFUNC_MODE_READ ) return 0;
	if ( !(mode & ZLIB_FILEFUNC_MODE_EXISTING) ) return 0;

	return opaque;
}


uLong CNtlUnzip::Read_File_Func( voidpf opaque, voidpf stream, void* buf, uLong size )
{
	if ( NULL == stream ) return 0;
	if ( stream != opaque ) return 0;
	if ( NULL == buf ) return 0;
	if ( size <= 0 ) return 0;

	CNtlUnzip* pclUnZip = (CNtlUnzip*)opaque;
	if ( NULL == pclUnZip->m_pBuffer ) return 0;
	if ( pclUnZip->m_nBufSize <= 0 ) return 0;
	if ( pclUnZip->m_nBufSize <= pclUnZip->m_nCurPos ) return 0;

	int nReadSize = pclUnZip->m_nCurPos + (long)size > pclUnZip->m_nBufSize ?
		(long)size - (pclUnZip->m_nCurPos + (long)size - pclUnZip->m_nBufSize) :
		(long)size;

	memcpy( buf, &pclUnZip->m_pBuffer[pclUnZip->m_nCurPos], nReadSize );
	pclUnZip->m_nCurPos += nReadSize;

	return nReadSize;
}


uLong CNtlUnzip::Write_File_Func( voidpf opaque, voidpf stream, const void* buf, uLong size )
{
	// 메모리를 통한 Unzip에서는 Write를 지원하지 않습니다
	return 0;
}


long CNtlUnzip::Tell_File_Func( voidpf opaque, voidpf stream )
{
	if ( NULL == stream ) return -1;
	if ( stream != opaque ) return -1;

	CNtlUnzip* pclUnZip = (CNtlUnzip*)opaque;
	if ( NULL == pclUnZip->m_pBuffer ) return -1;
	if ( pclUnZip->m_nBufSize <= 0 ) return -1;

	return pclUnZip->m_nCurPos;
}


long CNtlUnzip::Seek_File_Func( voidpf opaque, voidpf stream, uLong offset, int origin )
{
	if ( NULL == stream ) return -1;
	if ( stream != opaque ) return -1;

	CNtlUnzip* pclUnZip = (CNtlUnzip*)opaque;
	if ( NULL == pclUnZip->m_pBuffer ) return -1;
	if ( pclUnZip->m_nBufSize <= 0 ) return -1;

	switch ( origin )
	{
	case ZLIB_FILEFUNC_SEEK_CUR:
		{
			pclUnZip->m_nCurPos += (long)offset;
		}
		break;
	case ZLIB_FILEFUNC_SEEK_END:
		{
			pclUnZip->m_nCurPos = pclUnZip->m_nBufSize + (long)offset;
		}
		break;
	case ZLIB_FILEFUNC_SEEK_SET:
		{
			pclUnZip->m_nCurPos = (long)offset;
		}
		break;
	default: return -1;
	}

	return 0;
}


int CNtlUnzip::Close_file_Func( voidpf opaque, voidpf stream )
{
	if ( NULL == stream ) return -1;
	if ( stream != opaque ) return -1;

	CNtlUnzip* pclUnZip = (CNtlUnzip*)opaque;

	pclUnZip->m_nCurPos = 0;

	return 0;
}


int CNtlUnzip::TestError_File_Func( voidpf opaque, voidpf stream )
{
	return 0;
}


CNtlUnzip::CNtlUnzip( const char* szFilePath )
: m_hZipFile( 0 ), m_pBuffer( NULL ), m_nBufSize( 0 ), m_nCurPos( 0 )
{
	m_ZLibFileFuncs.zopen_file		= Open_File_Func;
	m_ZLibFileFuncs.zread_file		= Read_File_Func;
	m_ZLibFileFuncs.zwrite_file		= Write_File_Func;
	m_ZLibFileFuncs.ztell_file		= Tell_File_Func;
	m_ZLibFileFuncs.zseek_file		= Seek_File_Func;
	m_ZLibFileFuncs.zclose_file		= Close_file_Func;
	m_ZLibFileFuncs.zerror_file		= TestError_File_Func;
	m_ZLibFileFuncs.opaque			= this;

	OpenZip( szFilePath );
}

CNtlUnzip::~CNtlUnzip( void )
{
	CloseZip();
}

bool CNtlUnzip::Unzip( bool bIgnoreFilePath )
{
	if ( !m_hZipFile ) return false;

	return UnzipTo( m_szOutputFolder, bIgnoreFilePath );
}

bool CNtlUnzip::UnzipTo( const char* szFolder, bool bIgnoreFilePath )
{
	if ( !m_hZipFile ) return false;

	if ( !szFolder || !CreateFolder(szFolder) ) return false;

	if ( GetFileCount() == 0 ) return false;

	if ( !GotoFirstFile() ) return false;

	do
	{
		if ( !UnzipFile( szFolder, bIgnoreFilePath ) ) return false;
	}
	while ( GotoNextFile() );

	return true;
}

bool CNtlUnzip::UnzipFile( const char* szFolder, bool bIgnoreFilePath )
{
	if ( !m_hZipFile ) return false;

	if ( !szFolder ) szFolder = m_szOutputFolder;

	if ( !CreateFolder(szFolder) ) return false;

	sUZ_FILEINFO sInfo;
	GetFileInfo( sInfo );

	if ( sInfo.bFolder )
	{
		char szFolderPath[UNZIP_BUFFER_SIZE];
		_makepath( szFolderPath, 0, m_szOutputFolder, sInfo.szFileName, 0 );
		return CreateFolder( szFolderPath );
	}

	char szFilePath[UNZIP_BUFFER_SIZE];

	if ( bIgnoreFilePath )
	{
		char szFName[UNZIP_BUFFER_SIZE], szExt[UNZIP_BUFFER_SIZE];

		_splitpath( sInfo.szFileName, 0, 0, szFName, szExt );
		_makepath( sInfo.szFileName, 0, 0, szFName, szExt );
	}

	_makepath( szFilePath, 0, szFolder, sInfo.szFileName, 0 );

	if ( !CreateFilePath(szFilePath) ) return false;

	HANDLE hOutputFile = ::CreateFile( szFilePath,
		GENERIC_WRITE,
		0,
		0,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		0 );

	if ( INVALID_HANDLE_VALUE == hOutputFile ) return false;

	if ( unzOpenCurrentFile(m_hZipFile) != UNZ_OK ) return false;

	int nRet = UNZ_OK;
	char pBuffer[UNZIP_BUFFER_SIZE];

	do
	{
		nRet = unzReadCurrentFile( m_hZipFile, pBuffer, UNZIP_BUFFER_SIZE );

		if ( nRet > 0 )
		{
			DWORD dwBytesWritten = 0;

			if ( !::WriteFile(hOutputFile, pBuffer, nRet, &dwBytesWritten, NULL) ||
				 dwBytesWritten != (DWORD)nRet )
			{
				nRet = UNZ_ERRNO;
				break;
			}
		}
	}
	while ( nRet > 0 );

	CloseHandle( hOutputFile );
	unzCloseCurrentFile( m_hZipFile );

	if ( nRet == UNZ_OK ) SetFileModTime(szFilePath, sInfo.dwDosDate);

	return (nRet == UNZ_OK);
}

bool CNtlUnzip::ReadFileData( char* pBuff, int nSize )
{
	if ( unzOpenCurrentFile( m_hZipFile ) != UNZ_OK ) return false;

	int nRet = unzReadCurrentFile( m_hZipFile, pBuff, nSize );

	unzCloseCurrentFile( m_hZipFile );

	return (nRet == nSize);
}

bool CNtlUnzip::UnzipFile( int nFile, const char* szFolder, bool bIgnoreFilePath )
{
	if ( !m_hZipFile ) return false;

	if ( !szFolder ) szFolder = m_szOutputFolder;

	if ( !GotoFile(nFile) ) return false;

	return UnzipFile( szFolder, bIgnoreFilePath );
}

bool CNtlUnzip::OpenZip( const char* szFilePath )
{
	CloseZip();

	if ( !szFilePath || !lstrlen( szFilePath ) ) return false;

	char szFullPath[UNZIP_BUFFER_SIZE];

	if ( !GetFullPathName( szFilePath, UNZIP_BUFFER_SIZE, szFullPath, 0 ) ) return false;

	m_hZipFile = unzOpen( szFullPath );

	if ( m_hZipFile )
	{
		char szDrive[UNZIP_BUFFER_SIZE], szFolder[UNZIP_BUFFER_SIZE], szFName[UNZIP_BUFFER_SIZE];
		_splitpath( szFullPath, szDrive, szFolder, szFName, 0 );
		_makepath( m_szOutputFolder, szDrive, szFolder, szFName, 0 );
	}

	return (m_hZipFile != 0);
}

bool CNtlUnzip::OpenZip( const char* szFilePath, const char* pBuffer, int nSize )
{
	CloseZip();

	if ( NULL == szFilePath || NULL == pBuffer || 0 >= nSize ) return false;

	if ( m_pBuffer ) delete [] m_pBuffer;
	m_pBuffer = new char[nSize];
	memcpy( m_pBuffer, pBuffer, nSize );

	m_nBufSize = nSize;

	char szFullPath[UNZIP_BUFFER_SIZE];

	if ( !GetFullPathName( szFilePath, UNZIP_BUFFER_SIZE, szFullPath, 0 ) ) return false;

	m_hZipFile = unzOpen2( szFullPath, &m_ZLibFileFuncs );

	if ( m_hZipFile )
	{
		char szDrive[UNZIP_BUFFER_SIZE], szFolder[UNZIP_BUFFER_SIZE], szFName[UNZIP_BUFFER_SIZE];
		_splitpath( szFullPath, szDrive, szFolder, szFName, 0 );
		_makepath( m_szOutputFolder, szDrive, szFolder, szFName, 0 );
	}

	return (m_hZipFile != 0);
}

bool CNtlUnzip::CloseZip( void )
{
	unzCloseCurrentFile( m_hZipFile );

	int nRet = unzClose( m_hZipFile );
	m_hZipFile = 0;
	m_szOutputFolder[0] = 0;

	if ( m_pBuffer ) { delete [] m_pBuffer; m_pBuffer = NULL; }
	m_nBufSize = 0;
	m_nCurPos = 0;

	return (nRet == UNZ_OK);
}

bool CNtlUnzip::SetOutputFolder( const char* szFolder )
{
	DWORD dwAttrib = GetFileAttributes( szFolder );

	if (dwAttrib != 0xffffffff && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY)) return false;

	lstrcpy( m_szOutputFolder, szFolder );

	return CreateFolder( szFolder );
}

int CNtlUnzip::GetFileCount( void )
{
	if ( !m_hZipFile ) return 0;

	unz_global_info sInfo;

	if ( unzGetGlobalInfo( m_hZipFile, &sInfo ) == UNZ_OK )
	{
		return (int)sInfo.number_entry;
	}

	return 0;
}

bool CNtlUnzip::GetFileInfo( sUZ_FILEINFO& sInfo )
{
	if ( !m_hZipFile ) return false;

	unz_file_info uzfi;

	ZeroMemory( &sInfo, sizeof(sInfo) );
	ZeroMemory( &uzfi, sizeof(uzfi) );

	if ( UNZ_OK != unzGetCurrentFileInfo(m_hZipFile, &uzfi, sInfo.szFileName, UNZIP_BUFFER_SIZE, NULL, 0, sInfo.szComment, UNZIP_COMMENT_SIZE) ) return false;

	sInfo.dwVersion = uzfi.version;	
	sInfo.dwVersionNeeded = uzfi.version_needed;
	sInfo.dwFlags = uzfi.flag;	
	sInfo.dwCompressionMethod = uzfi.compression_method; 
	sInfo.dwDosDate = uzfi.dosDate;  
	sInfo.dwCRC = uzfi.crc;	 
	sInfo.dwCompressedSize = uzfi.compressed_size; 
	sInfo.dwUncompressedSize = uzfi.uncompressed_size;
	sInfo.dwInternalAttrib = uzfi.internal_fa; 
	sInfo.dwExternalAttrib = uzfi.external_fa; 

	int nLen = lstrlen( sInfo.szFileName );

	while ( nLen-- )
	{
		if ( sInfo.szFileName[nLen] == '/' )
			 sInfo.szFileName[nLen] = '\\';
	}

	sInfo.bFolder = ((sInfo.dwExternalAttrib & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);

	return true;
}

bool CNtlUnzip::GetFileInfo( int nFile, sUZ_FILEINFO& sInfo )
{
	if ( !m_hZipFile ) return false;

	if ( !GotoFile( nFile ) ) return false;

	return GetFileInfo( sInfo );
}

bool CNtlUnzip::GotoFirstFile( const char* szExt )
{
	if ( !m_hZipFile ) return false;

	if ( !szExt || !lstrlen(szExt) ) return ( unzGoToFirstFile(m_hZipFile) == UNZ_OK );

	if ( unzGoToFirstFile(m_hZipFile) == UNZ_OK )
	{
		sUZ_FILEINFO sInfo;

		if ( !GetFileInfo(sInfo) ) return false;

		char szFExt[UNZIP_BUFFER_SIZE];
		_splitpath( sInfo.szFileName, 0, 0, 0, szFExt );

		if ( szFExt )
		{
			if ( lstrcmpi(szExt, szFExt + 1) == 0 ) return true;
		}

		return GotoNextFile( szExt );
	}

	return false;
}

bool CNtlUnzip::GotoNextFile( const char* szExt )
{
	if ( !m_hZipFile ) return false;

	if ( !szExt || !lstrlen(szExt) ) return ( unzGoToNextFile(m_hZipFile) == UNZ_OK );

	sUZ_FILEINFO sInfo;

	while ( unzGoToNextFile(m_hZipFile) == UNZ_OK )
	{
		if ( !GetFileInfo(sInfo) ) return false;

		char szFExt[UNZIP_BUFFER_SIZE];
		_splitpath( sInfo.szFileName, 0, 0, 0, szFExt );

		if ( szFExt )
		{
			if ( lstrcmpi(szExt, szFExt + 1) == 0 ) return true;
		}
	}

	return false;
}

bool CNtlUnzip::GotoFile( const char* szFileName, bool bIgnoreFilePath )
{
	if ( !m_hZipFile ) return false;

	if ( unzLocateFile(m_hZipFile, szFileName, 2) == UNZ_OK ) return true;

	if ( bIgnoreFilePath )
	{ 
		if ( unzGoToFirstFile(m_hZipFile) != UNZ_OK ) return false;

		sUZ_FILEINFO sInfo;

		do
		{
			if ( !GetFileInfo( sInfo ) ) return false;

			char szFName[UNZIP_BUFFER_SIZE], szName[UNZIP_BUFFER_SIZE], szExt[UNZIP_BUFFER_SIZE];

			_splitpath( sInfo.szFileName, 0, 0, szName, szExt );
			_makepath( szFName, 0, 0, szName, szExt );

			if ( lstrcmpi(szFileName, szFName) == 0 ) return true;
		}
		while ( unzGoToNextFile(m_hZipFile) == UNZ_OK );
	}

	return false;
}


bool CNtlUnzip::GotoFile( int nFile )
{
	if ( !m_hZipFile ) return false;

	if ( nFile < 0 || nFile >= GetFileCount() ) return false;

	GotoFirstFile();

	while ( nFile-- )
	{
		if ( !GotoNextFile() ) return false;
	}

	return true;
}
