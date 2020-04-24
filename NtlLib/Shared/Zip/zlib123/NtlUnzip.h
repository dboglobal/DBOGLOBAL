#ifndef _NTL_UNZIP_H_
#define _NTL_UNZIP_H_


#ifdef __cplusplus
extern "C" {
#endif


#include "zconf.h"
#include "ioapi.h"


#define UNZIP_BUFFER_SIZE (2048)
#define UNZIP_COMMENT_SIZE (2048)


struct sUZ_FILEINFO
{
	char								szFileName[UNZIP_BUFFER_SIZE + 1];
	char								szComment[UNZIP_COMMENT_SIZE + 1];

	DWORD								dwVersion;  
	DWORD								dwVersionNeeded;
	DWORD								dwFlags;	 
	DWORD								dwCompressionMethod; 
	DWORD								dwDosDate;	
	DWORD								dwCRC;   
	DWORD								dwCompressedSize; 
	DWORD								dwUncompressedSize;
	DWORD								dwInternalAttrib; 
	DWORD								dwExternalAttrib; 
	bool								bFolder;
};


class ZEXPORT CNtlUnzip  
{
// Static methods
public:
	static bool							Unzip( const char* szFileName, const char* szFolder = 0, bool bIgnoreFilePath = false );

protected:
	static bool							CreateFolder( const char* szFolder );
	static bool							CreateFilePath( const char* szFilePath );
	static bool							SetFileModTime( const char* szFilePath, DWORD dwDosDate );

	static voidpf ZCALLBACK				Open_File_Func( voidpf opaque, const char* filename, int mode );
	static uLong ZCALLBACK				Read_File_Func( voidpf opaque, voidpf stream, void* buf, uLong size );
	static uLong ZCALLBACK				Write_File_Func( voidpf opaque, voidpf stream, const void* buf, uLong size );
	static long ZCALLBACK				Tell_File_Func( voidpf opaque, voidpf stream );
	static long ZCALLBACK				Seek_File_Func( voidpf opaque, voidpf stream, uLong offset, int origin );
	static int ZCALLBACK				Close_file_Func( voidpf opaque, voidpf stream );
	static int ZCALLBACK				TestError_File_Func( voidpf opaque, voidpf stream );

// Member variables
protected:
	void*								m_hZipFile;
	char								m_szOutputFolder[UNZIP_BUFFER_SIZE + 1];

	zlib_filefunc_def					m_ZLibFileFuncs;

	char*								m_pBuffer;
	int									m_nBufSize;
	int									m_nCurPos;

// Constructions and Destructions
public:
	CNtlUnzip( const char* szFilePath = 0 );
	virtual ~CNtlUnzip( void );

// Methods
public:
	bool								Unzip( bool bIgnoreFilePath = false );
	bool								UnzipTo( const char* szFolder, bool bIgnoreFilePath = false );

	bool								UnzipFile( int nFile, const char* szFolder = 0, bool bIgnoreFilePath = false );
	bool								UnzipFile( const char* szFolder = 0, bool bIgnoreFilePath = false );

	bool								OpenZip( const char* szFilePath );
	bool								OpenZip( const char* szFilePath, const char* pBuffer, int nSize );
	bool								CloseZip( void );

	bool								SetOutputFolder( const char* szFolder );
	
	int									GetFileCount( void );

	bool								GetFileInfo( sUZ_FILEINFO& sInfo );
	bool								GetFileInfo( int nFile, sUZ_FILEINFO& sInfo );

	bool								ReadFileData( char* pBuff, int nSize );
	
	bool								GotoFile( const char* szFileName, bool bIgnoreFilePath = true );
	bool								GotoFile( int nFile );

	bool								GotoFirstFile( const char* szExt = 0 );
	bool								GotoNextFile( const char* szExt = 0 );
};


#ifdef __cplusplus
}
#endif


#endif
