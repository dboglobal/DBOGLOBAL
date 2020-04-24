// TSEvtMaker.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <string>
#include "TSEvtMaker.h"
#include "TableContainer.h"
#include "TSUtil.h"
#include "ExportEvtTSMng.h"
#include "NtlZip.h"
#include "NtlCipher.h"


int APIENTRY _tWinMain( HINSTANCE hInstance,
					    HINSTANCE hPrevInstance,
						LPTSTR    lpCmdLine,
						int       nCmdShow )
{
	std::string strTblPath;
	std::string strTSPath;
	std::string strOutputFolder;
	std::string strCopyFoler;

	std::string* arstrString[4] = { &strTblPath, &strTSPath, &strOutputFolder, &strCopyFoler };

	char seps[] = ";";
	char* token = strtok( lpCmdLine, seps );

	int nCnt = 0;
	while ( token )
	{
		(*arstrString[nCnt++]) = token;

		token = strtok( NULL, seps );
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Table
	//
	//////////////////////////////////////////////////////////////////////////

	CTableContainer* pTblContainer = CreateTableContainer( strTblPath.c_str(), CTable::LOADING_METHOD_BINARY );

	if ( NULL == pTblContainer )
	{
		CNtlTSLog::Log( "Loading tables is failed. %s", strTblPath.c_str() );
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	TS eventor <-> Table connection
	//
	//////////////////////////////////////////////////////////////////////////

	CExportEvtTSMng clTSMng;
	if ( !clTSMng.Create( strTSPath.c_str(), pTblContainer ) )
	{
		clTSMng.Delete();

		DeleteTableContainer( pTblContainer );

		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Save eventor
	//
	//////////////////////////////////////////////////////////////////////////

	CDboTSMain::hashdef_EVT_MAPPER_LIST& defList = clTSMng.GetEventMapper();
	CDboTSMain::hashdef_EVT_MAPPER_LIST::iterator it = defList.begin();

	for ( ; it != defList.end(); ++it )
	{
		std::string strName = it->first;
		CNtlTSEvtMapper* pEvtMapper = it->second;

		std::string strPath = strOutputFolder;
		strPath += strName;
		strPath += ".evt";

		if ( !pEvtMapper->Save( strPath ) )
		{
			clTSMng.Delete();

			DeleteTableContainer( pTblContainer );

			CNtlTSLog::Log( "Saving the eventor is failed. %s", strPath.c_str() );

			return -1;
		}
	}

	DeleteTableContainer( pTblContainer );

	//////////////////////////////////////////////////////////////////////////
	//
	//	Zip
	//
	//////////////////////////////////////////////////////////////////////////

	std::string strZipName = strOutputFolder + "temp.zip";

	{
		CNtlZip clZip;
		if ( !clZip.OpenZip( strZipName.c_str() ) )
		{
			CNtlTSLog::Log( "Zipping the eventor is failed. %s", strZipName.c_str() );

			return -1;
		}

		WIN32_FIND_DATA FindFileData;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		std::string strSearchSpec( strOutputFolder + "*" );

		hFind = FindFirstFile( strSearchSpec.c_str(), &FindFileData );
		if ( INVALID_HANDLE_VALUE == hFind )
		{
			CNtlTSLog::Log( "Can not find files. %s", strSearchSpec.c_str() );

			return -1;
		}

		do 
		{
			if ( FILE_ATTRIBUTE_DIRECTORY != (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
				 '.' != FindFileData.cFileName[0] )
			{
				std::string strFindFileName = FindFileData.cFileName;

				if ( std::string::npos != strFindFileName.find( ".evt" ) )
				{
					clZip.AddFileToZip( FindFileData.cFileName, true );
				}
			}
		}
		while ( FindNextFile( hFind, &FindFileData ) != 0 );

		FindClose( hFind );
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Encryption
	//
	//////////////////////////////////////////////////////////////////////////

	std::string strOutputName = strOutputFolder + "evt.e";

	{
		// 파일 로딩
		FILE* pFile;
		fopen_s( &pFile, strZipName.c_str(), "rb" );

		if ( NULL == pFile )
		{
			CNtlTSLog::Log( "Can not loading the file. %s", strZipName.c_str() );

			return -1;
		}

		fseek( pFile, 0, SEEK_END );
		int nReadSize = ftell( pFile );
		fseek( pFile, 0, SEEK_SET );
		char* pReadBuf = new char[nReadSize];
		fread( pReadBuf, 1, nReadSize, pFile );

		fclose( pFile );

		// 암호화
		std::string strKey = "dnfldbofmftkfkdgowntpdy";
		CNtlCipher Cipher;
		Cipher.SetKey( DES_CIPHER, strKey.c_str(), strKey.size() );
		char* pEncryptBuf = new char[nReadSize+256];
		ZeroMemory( pEncryptBuf, nReadSize+256 );
		int nEncSize = Cipher.Encrypt( pReadBuf, nReadSize, pEncryptBuf, nReadSize+256 );

		// 파일에 기록
		fopen_s( &pFile, strOutputName.c_str(), "wb" );

		if ( NULL == pFile )
		{
			CNtlTSLog::Log( "Can not saving the file. %s", strOutputName.c_str() );

			return -1;
		}

		fwrite( &nReadSize, sizeof(int), 1, pFile );
		fwrite( pEncryptBuf, 1, nEncSize, pFile );

		fclose( pFile );

		delete [] pReadBuf;
		delete [] pEncryptBuf;
	}

	//////////////////////////////////////////////////////////////////////////
	//
	//	Copy file
	//
	//////////////////////////////////////////////////////////////////////////

	std::string strDestFile = strCopyFoler + "evt.e";
	std::string strSrcFile = strOutputFolder + "evt.e";

	// 복사 시도
	bool bCopyFailed = true;
	int nCopyCnt = 10;
	while ( nCopyCnt-- > 0 )
	{
		if ( !::CopyFile( strSrcFile.c_str(), strDestFile.c_str(), FALSE ) )
		{
			// Read only 속성을 없애고 복사를 시도해도 복사가 실패한 경우
			CNtlTSLog::Log( "Can not copy the file[%u]. %s => %s", GetLastError(), strSrcFile.c_str(), strDestFile.c_str() );

			Sleep( 100 );

			bCopyFailed = true;
		}
		else
		{
			bCopyFailed = false;

			break;
		}
	}

	return bCopyFailed ? -1 : 0;
}
