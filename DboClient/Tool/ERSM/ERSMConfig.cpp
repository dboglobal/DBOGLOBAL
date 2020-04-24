#include "StdAfx.h"
#include "ERSMConfig.h"
#include "ERSMDef.h"

ERSMConfig::ERSMConfig(void)
{
    m_nRunTime = 30;            ///< 디폴트 30분
    m_nSMTPRunTime = 3 * 60;    ///< 디폴트 3시간
}

ERSMConfig::~ERSMConfig(void)
{
}

bool ERSMConfig::ReadConfig( const std::string& strFileName ) 
{
    if(strFileName.empty())
        return false;

	Create();

	if(!Load(CCAST<char*>(strFileName.c_str())))
		return false;

	std::string strRoot				= "ERSM_CONFIG/";
	std::string strElemERSFolder	= strRoot + "ERS_FOLDER";
	std::string strElemERSMRuntime	= strRoot + "ERSM_RUNTIME";
	std::string steElemSMTPServer	= strRoot + "SMTP_SERVER";
	std::string strElemSMTPID		= strRoot + "SMTP_ID";
	std::string strElemSMTPPASS		= strRoot + "SMTP_PASSWORD";
	std::string strElemSMTPRunTime	= strRoot + "SMTP_RUNTIME";
	std::string	strElemSender		= strRoot + "SMTP_SENDER";

	char szFolder[ERSM_BUF_SIZE]		 = {0,};
	char szRunTime[ERSM_BUF_SIZE]		 = {0,};
	char szSMTPServer[ERSM_BUF_SIZE]	 = {0,};
	char szSMTPID[ERSM_BUF_SIZE]		 = {0,};
	char szSMTPPASS[ERSM_BUF_SIZE]		 = {0,};
	char szSMTPRuntime[ERSM_BUF_SIZE]	 = {0,};
	char szSMTPSender[ERSM_BUF_SIZE]	 = {0,};

	GetDataWithXPath(CCAST<char*>(strElemERSFolder.c_str()), szFolder, sizeof(szFolder));
	GetDataWithXPath(CCAST<char*>(strElemERSMRuntime.c_str()), szRunTime, sizeof(szRunTime));
	GetDataWithXPath(CCAST<char*>(strElemSMTPID.c_str()), szSMTPID, sizeof(szSMTPID));
	GetDataWithXPath(CCAST<char*>(strElemSMTPPASS.c_str()), szSMTPPASS, sizeof(szSMTPPASS));
	GetDataWithXPath(CCAST<char*>(strElemSMTPRunTime.c_str()), szSMTPRuntime, sizeof(szSMTPRuntime));
	GetDataWithXPath(CCAST<char*>(strElemSender.c_str()), szSMTPSender, sizeof(szSMTPSender));

	m_strERSFolder = szFolder;
	m_nRunTime = atoi(szRunTime);
	m_strSMTPServerIP = szSMTPServer;
	m_strSMTPID = szSMTPID;
	m_strSMTPPass = szSMTPPASS;
	m_nSMTPRunTime = atoi(szSMTPRuntime);
	m_strSMTPSender = szSMTPSender;

    return true;
}