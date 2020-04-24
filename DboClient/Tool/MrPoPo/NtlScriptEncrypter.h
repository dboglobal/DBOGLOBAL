#pragma once

#include <string>

// config data
typedef struct _SConfigData
{
    CString     strAddr;		// account server ip address(해킹의 문제로 하드코딩을 고려한다?)
    DWORD       dwPort;				// account server ip port(해킹의 문제로 하드코딩을 고려한다?)
    CString     strLocalDsp;
    CString     strLocalSyncDsp;
    CString     strBugTrapServerIP;        
    DWORD       dwBugTrapServerPort;    
}SConfigData;

/**
 * \ingroup Util
 * \brief 클라이언트 스크립트를 암호화하기 위한 클래스
 *  암호화 툴에서도 사용하기 위해서 부득이하게 Util에 넣어둔다.
 * \date 2008-07-31
 * \author agebreak
 */
class CNtlScriptEncrypter
{
public:
    CNtlScriptEncrypter(void);
    ~CNtlScriptEncrypter(void);

    static BOOL LoadConfigOption(OUT SConfigData* pConfigData, char* szFileName);             
    static BOOL SaveConfigOption(SConfigData* pConfigData, char* szFileName, BOOL bEncrypt = FALSE); 

protected:
    static BOOL LoadConfigOptionXML(OUT SConfigData* pConfigData, char* szFileName);
    static BOOL LoadConfigOptionENC(OUT SConfigData* pConfigData, char* szFileName);
    static BOOL SaveConfigOptionXML(SConfigData* pConfigData, char* szFileName);
    static BOOL SaveConfigOptionENC(SConfigData* pConfigData, char* szFileName, char* szCryptPassword);
};
