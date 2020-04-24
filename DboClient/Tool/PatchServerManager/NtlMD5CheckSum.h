#pragma once

/// 체크섬 정보를 담을 구조체
struct SChecksumInfo
{
    char szFileName[MAX_PATH];
    char checksum[33];

    SChecksumInfo()
    {
        ZeroMemory(szFileName, MAX_PATH);
        ZeroMemory(checksum, 33);
    }
};

/**
 * \ingroup MD5SumTest
 * \brief 파일들을 MD5 Checksum을 이용하여, 무결성 검사를 진행하는 클래스
 *
 * \date 2009-02-23
 * \author agebreak
 */
class CNtlMD5CheckSum
{
public:
    CNtlMD5CheckSum(void);
    virtual ~CNtlMD5CheckSum(void);

	unsigned int CheckSumBuildCount(char* szFolder);					///< 빌드되는 체크섬 파일의 총 개수
    int CheckSumBuild(char* szChecksumFileName, char* szFolder);		///< 폴더의 파일들(하위 폴더 포함)로 부터 체크섬 파일을 생성한다.
	unsigned int CheckSumTestCount(char* szChecksumFileName);			///< 빌드되어있는 체크섬 파일의 총 개수
    int ChecksumTest(char* szChecksumFileName);							///< 생성된 체크섬 파일들을 비교한다.

protected:
    int md5_wrapper( char *filename, unsigned char *sum );
    int md5_print(const char *filename, char* checksum );
    int md5_check( char *filename );

    bool RecursiveChecksumBuild( bool bForCounter, unsigned int& uiCnt );		///< 하위 폴더들을 재귀루프로 돌면서 체크섬 정보를 만든다.
    int  WriteChecksumBuild(char* szFileName);									///< 만들어진 체크섬 정보를 파일에 기록한다.

	virtual bool IsSkipFileOnChecksumBuild(char* szFileName, bool bFolder) { return false; }

    virtual bool OnEvent_ChecksumBuild(const char* szFileName, int returnCode);		///< 체크섬 생성 결과를 받는 함수
    virtual bool OnEvent_ChecksumTest(const char* szFileName, int returnCode);		///< 체크섬 비교 결과를 받는 함수

protected:
    std::list<SChecksumInfo*>       m_listCheksum;                      ///< 체크섬 정보 리스트
    size_t                          m_nWorkingFolderLength;             ///< 작업 폴더 이름의 길이
};
