#ifndef _NTL_ZIP_H_
#define _NTL_ZIP_H_


#ifdef __cplusplus
extern "C" {
#endif


#include "zconf.h"


#define ZIP_BUFFER_SIZE (2048)


struct sZ_FILEINFO
{
	int									nFileCount;
	int									nFolderCount;
	DWORD								dwUncompressedSize;
};


class ZEXPORT CNtlZip
{
// Static
public:
	static bool							ZipFile( const char* szFilePath );
	static bool							ZipFolder( const char* szFilePath, bool bIgnoreFilePath );

// Member variables
protected:
	void*								m_hZipFile;
	char								m_szRootFolder[ZIP_BUFFER_SIZE + 1];
	sZ_FILEINFO							m_sInfo;

// Constructions and Destructions
public:
	CNtlZip( const char* szFilePath = 0, const char* szRootFolder = 0, bool bAppend = false );
	virtual ~CNtlZip( void );

// Methods
public:
	bool								OpenZip( const char* szFilePath, const char* szRootFolder = 0, bool bAppend = false );
	bool								CloseZip( void );

	void								GetFileInfo( sZ_FILEINFO& sInfo );

	bool								AddFileToZip( const char* szFilePath, bool bIgnoreFilePath = false );
	bool								AddFileToZip( const char* szFilePath, const char* pWriteBuf, int nWriteSize, bool bIgnoreFilePath = false );
	bool								AddFileToZip( const char* szFilePath, const char* szRelFolderPath );
	bool								AddFileToZip( const char* szFilePath, const char* pWriteBuf, int nWriteSize, const char* szRelFolderPath );
	bool								AddFolderToZip( const char* szFolderPath, bool bIgnoreFilePath = false );

// Implementations
protected:
	void								PrepareSourcePath( char* szPath );
};


#ifdef __cplusplus
}
#endif


#endif
