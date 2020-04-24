#pragma once
#include "NtlXMLDoc.h"
NCDeclareSGT(ERSMConfig);

/**
 * \ingroup ERSManager
 * \brief ERSMan의 Config 파일(XML)로부터 설정값을 읽어오는 클래스
 * \date 2007-01-26
 * \author agebreak
 */
class ERSMConfig : public CNtlXMLDoc
{
public:
    ERSMConfig(void);
    virtual ~ERSMConfig(void);

    bool ReadConfig(const std::string& strFileName);            ///< Config 파일로부터 정보를 읽어 온다.

    // Get Methods
    std::string     GetERSFolder() {return m_strERSFolder;}
    int             GetRunTime() {return m_nRunTime;}
    std::string     GetSMTPServerIP() {return m_strSMTPServerIP;}
    std::string     GetSMTPServerID() {return m_strSMTPID;}
    std::string     GetSMTPPass() {return m_strSMTPPass;}
    int             GetSMTPRunTime() {return m_nSMTPRunTime;}

protected:
    std::string     m_strERSFolder;                             ///< 에러 리포트 폴더
    int             m_nRunTime;                                 ///< 정리 작업 시간 텀 (분)
    std::string     m_strSMTPServerIP;                          ///< SMTP 서버 IP
    std::string     m_strSMTPID;                                ///< SMTP 서버 ID
    std::string     m_strSMTPPass;                              ///< SMTP 서버 패스워드
    int             m_nSMTPRunTime;                             ///< 메일을 발송할 시간 텀 (분)	
	std::string		m_strSMTPSender;							///< 메일 발송자 이름
};
