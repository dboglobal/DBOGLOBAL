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
	USES_CONVERSION;

    if(strFileName.empty())
        return false;

	Create();

	if(!Load(RCAST(char*)strFileName.c_str())
		return false;

	std::string strRoot				= "ESM_CONFIG/";
	std::string strElemERSFolder	= strRoot + "ERS_FOLDER";
	std::string strElemERSMRuntime	= strRoot + "ERSM_RUNTIME";
	std::string strElemSMTPID		= strRoot + "SMTP_ID";

    return true;
}