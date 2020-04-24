/*****************************************************************************
*
* File			: NtlPatchServerConfig.h
* Author		: agebreak
* Copyright	: (주)NTL
* Date			: 2007. 10. 4.
* Abstract		: 
*****************************************************************************
* Desc         : Patch Server에 존재하는 PatchServerConfig.XML파일의 내용을 읽어서 분석하는 클래스
*
*****************************************************************************/
#pragma once

#include <string>

/// 패치서버에 존재하는 PatchServerConfig.XML 파일의 내용을 읽어서 분석하는 파일
class CNtlPatchServerConfig
{
public:    
    static CNtlPatchServerConfig* GetInstance();
    static void                   DeleteInstance();

    BOOL LoadServerConfigFile(const char* pszXMLFileName);              ///< XML 파일을 로딩한다.

    BOOL            GetPatchEnable() {return m_bPatchEnable;}           ///< 패치 가능 유무를 반환한다.
    std::string&    GetPatchErrMsg()    {return m_strErrorMsg;}         ///< 에러메시지를 반환한다.
    std::string&    GetFullVerServerIP() {return m_strFullVerServerIP;} ///< 풀버전이 있는 서버의 IP를 반환한다.
    std::string&    GetFullVerServerFolder() {return m_strFullVerServerFolder;} ///< 풀버전이 있는 서버의 폴더경로를 반환한다.

protected:
    CNtlPatchServerConfig();
    ~CNtlPatchServerConfig();

protected:
    static CNtlPatchServerConfig* m_pInstance;
    
    BOOL            m_bPatchEnable;
    std::string     m_strFullVerServerIP;
    std::string     m_strFullVerServerFolder;
    std::string     m_strErrorMsg;
};