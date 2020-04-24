#include "StdAfx.h"
#include "NtlMD5CheckSum.h"
#include "md5.h"
#include "MD5ChekSumErrCode.h"


#pragma warning (disable : 4996)


CNtlMD5CheckSum::CNtlMD5CheckSum(void)
{
}

CNtlMD5CheckSum::~CNtlMD5CheckSum(void)
{ 
    for each(SChecksumInfo* pInfo in m_listCheksum)
    {
        delete pInfo;
    }

    m_listCheksum.clear();
}

int CNtlMD5CheckSum::md5_wrapper( char *filename, unsigned char *sum ) 
{
    int ret = md5_file( filename, sum );

    if( ret == 1 )
        return CHECKSUM_FAILED_TO_OPEN_FILE;        

    if( ret == 2 )
        return CHECKSUM_FAILED_TO_READ_FILE;

    return( ret );
}

int CNtlMD5CheckSum::md5_print(const char *filename, char* checksum )
{
    int i = 0;
    unsigned char sum[16] = {0,};

    if( md5_wrapper((char*)filename, sum ) != 0 )
        return( 1 );

    for( i = 0; i < 16; i++ )
    {
        sprintf(checksum + (i * 2), "%02x", sum[i]);
    }

    return( 0 );
}

int CNtlMD5CheckSum::md5_check( char *filename ) 
{
    int i;
    size_t n;
    FILE *f;
    int errCode = 0;    
    unsigned char sum[16] = {0,};
    char buf[33], line[2048];
    std::string strFileName;

    if(fopen_s(&f, filename, "rb") != 0)
    {
        return CHECKSUM_FAILED_TO_OPEN_MD5;
    }

	unsigned int uiCnt = 0;
	fscanf_s( f, "%u ", &uiCnt );

    memset( line, 0, sizeof( line ) );

    n = sizeof( line );

    while( fgets( line, sizeof(line) - 1, f ) != NULL )
    {
        n = strlen( line );

        if( n < 36 )
            continue;

        if( line[32] != ' ' || line[33] != ':'  || line[34] != ' ')
            continue;

        if( line[n - 1] == '\n' ) { n--; line[n] = '\0'; }
        if( line[n - 1] == '\r' ) { n--; line[n] = '\0'; }


        // 35번째 이후부터가 파일 이름이다. 이것을 가지고 체크섬을 생성한다.
        strFileName = line + 35;
        errCode = md5_wrapper((char*)strFileName.c_str(), sum );        
        if(errCode != CHECKSUM_SUCCESS)
        {
			if ( !OnEvent_ChecksumTest(strFileName.c_str(), errCode) )
			{
				fclose( f );

				return CHECKSUM_USER_BREAK;
			}

            continue;
        }
        

        for( i = 0; i < 16; i++ )
		{
            sprintf( buf + i * 2, "%02x", sum[i] );
		}

        if( memcmp( line, buf, 32 ) != 0 )
        {
			if ( !OnEvent_ChecksumTest(strFileName.c_str(), CHECKSUM_FAILED_WRONG_CHECKSUM) )
			{
				fclose( f );

				return CHECKSUM_USER_BREAK;
			}
        }
        else
        {
			// 체크섬 비교 성공
			if ( !OnEvent_ChecksumTest(strFileName.c_str(), CHECKSUM_SUCCESS) )
			{
				fclose( f );

				return CHECKSUM_USER_BREAK;
			}
        }

        n = sizeof( line );
    }

	fclose( f );

    return CHECKSUM_SUCCESS;
}

unsigned int CNtlMD5CheckSum::CheckSumBuildCount( char* szFolder )
{
	char strCurPath[1024];
	GetCurrentDirectoryA(1024, strCurPath);
	SetCurrentDirectoryA(szFolder);

	bool bForCounter = true;
	unsigned int uiCnt = 0;

	RecursiveChecksumBuild( bForCounter, uiCnt );

	SetCurrentDirectoryA(strCurPath);    

	return uiCnt;
}

int CNtlMD5CheckSum::CheckSumBuild( char* szChecksumFileName, char* szFolder ) 
{
	char szCheckSumFolder[1024] = { 0, };
	GetFullPathNameA( szFolder, 1024, szCheckSumFolder, 0 );

    char szCurPath[1024];
    GetCurrentDirectoryA( 1024, szCurPath );

	SetCurrentDirectoryA( szCheckSumFolder );
    m_nWorkingFolderLength = strlen( szCheckSumFolder ) - 1;

    // 체크섬 정보를 만들어서 리스트에 저장한다.
	bool bForCounter = false;
	unsigned int uiCnt = 0;

	if ( !RecursiveChecksumBuild( bForCounter, uiCnt ) )
	{
		SetCurrentDirectoryA( szCurPath );    

		return CHECKSUM_USER_BREAK;
	}

    SetCurrentDirectoryA( szCurPath );    

    // 파일에 출력한다.
    return WriteChecksumBuild( szChecksumFileName );
}

bool CNtlMD5CheckSum::RecursiveChecksumBuild( bool bForCounter, unsigned int& uiCnt ) 
{
    // 하위 폴더를 모두 돌면서 파일들의 체크섬을 만든다.
    char strCurPath[1024];
    GetCurrentDirectoryA( 1024, strCurPath );

    std::string strFullFileName;
    int retCode = CHECKSUM_SUCCESS;

    WIN32_FIND_DATAA FindFileData;
    HANDLE hFile = FindFirstFileA("*.*", &FindFileData);

	if ( hFile == INVALID_HANDLE_VALUE )
	{
		return true;
	}

    do 
    {
        strFullFileName		= strCurPath;
        strFullFileName		+= "\\";
        strFullFileName		+= FindFileData.cFileName;        

        if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            //  디렉토리이면
            if(FindFileData.cFileName[0] == '.')
            {
                //  Dot 디렉토리는 스킵한다.
            } 
            else
            {
				if ( !IsSkipFileOnChecksumBuild( FindFileData.cFileName, true ) )
				{
					SetCurrentDirectoryA(strFullFileName.c_str());

					if ( !RecursiveChecksumBuild( bForCounter, uiCnt ) )
					{
						FindClose( hFile );
						return false;
					}
				}
            }
        } 
        else
        {
			if ( !IsSkipFileOnChecksumBuild( FindFileData.cFileName, false ) )
			{
				if ( !bForCounter )
				{
					// 파일의 체크섬을 리스트에 저장한다.
					SChecksumInfo* pInfo = new SChecksumInfo();

					retCode = md5_print(strFullFileName.c_str(), pInfo->checksum);
		            
					// 상대 경로만 저장한다.
					strFullFileName.replace(0, m_nWorkingFolderLength + 1, "");

					if(retCode == CHECKSUM_SUCCESS)
					{
						sprintf_s(pInfo->szFileName, "%s", strFullFileName.c_str());
						m_listCheksum.push_back(pInfo);                
					}
					else
					{
						delete pInfo;
					}

					if ( !OnEvent_ChecksumBuild(strFullFileName.c_str(), retCode) )
					{
						FindClose( hFile );
						return false;
					}
				}

				uiCnt++;
			}
        }
    } while(FindNextFileA(hFile, &FindFileData));

    FindClose(hFile);

	return true;
}

int CNtlMD5CheckSum::WriteChecksumBuild( char* szFileName ) 
{
    FILE* fp = NULL;
    if(fopen_s(&fp, szFileName, "w") != 0)
    {
        return CHECKSUM_NOT_CREATE_CHEKSUMFILE;
    }

	fprintf_s(fp, "%d\n", (unsigned int)m_listCheksum.size());

    for each(SChecksumInfo* pInfo in m_listCheksum)
    {
        fprintf_s(fp, "%s : %s\n", pInfo->checksum, pInfo->szFileName);
    }

    fclose(fp);

    return CHECKSUM_SUCCESS;
}

unsigned int CNtlMD5CheckSum::CheckSumTestCount(char* szChecksumFileName)
{
	FILE* fp = NULL;

	if(fopen_s(&fp, szChecksumFileName, "rb") != 0)
	{
		return 0;
	}

	unsigned int uiCnt = 0;
	fscanf_s( fp, "%u", &uiCnt );

	fclose( fp );

	return uiCnt;
}

int CNtlMD5CheckSum::ChecksumTest( char* szChecksumFileName ) 
{
    return md5_check(szChecksumFileName);
}

bool CNtlMD5CheckSum::OnEvent_ChecksumBuild(const char* szFileName, int returnCode)
{
    printf_s("[CheckSum Build] %s : ErrCode(%d)\n", szFileName, returnCode);
	return true;
}

bool CNtlMD5CheckSum::OnEvent_ChecksumTest(const char* szFileName, int returnCode)
{
    printf_s("[Checksum Check] %s : ErrCode(%d)\n", szFileName, returnCode);
	return true;
}
