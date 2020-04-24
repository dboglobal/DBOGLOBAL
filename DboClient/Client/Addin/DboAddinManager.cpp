#include "precomp_dboclient.h"
#include "DboAddinManager.h"

#include "NtlDebug.h"
#include "NtlSLEvent.h"

// Devil
#include "il.h"

CDBOAddinManager* CDBOAddinManager::m_pInstance = NULL;

CDBOAddinManager::CDBOAddinManager() 
{
	LinkMsg(g_EventScreenShot);
}

CDBOAddinManager::~CDBOAddinManager() 
{
	UnLinkMsg(g_EventScreenShot);
}

void CDBOAddinManager::Init() 
{
    if(!m_pInstance)
    {
        m_pInstance = NTL_NEW CDBOAddinManager();
        ilInit();
    }    
}

void CDBOAddinManager::ShutDown() 
{
    if(m_pInstance)
    {
        ilShutDown();
        NTL_DELETE(m_pInstance);
    }
}

void CDBOAddinManager::HandleEvents( RWS::CMsg &pMsg ) 
{
    if(pMsg.Id == g_EventScreenShot)
    {
        // 스크린샷을 찍은후에 날라오는 이벤트
        OnEventScreenShot(pMsg);
    }
}

void CDBOAddinManager::OnEventScreenShot( RWS::CMsg& pMsg ) 
{
    // png로 저장된 파일을 jpg로 변환한다.
    SNtlEventScreenShot* pData = (SNtlEventScreenShot*)pMsg.pData;
    std::string strOrgfileName, strDestFileName;
    strOrgfileName = pData->strFileName;
    strOrgfileName.erase(strOrgfileName.end() - 4, strOrgfileName.end());
    strDestFileName = strOrgfileName;
    strOrgfileName += ".bmp";
    strDestFileName += ".jpg";

    ilLoadImage(strOrgfileName.c_str());
    ilSaveImage(strDestFileName.c_str());

    DeleteFile(strOrgfileName.c_str());
}