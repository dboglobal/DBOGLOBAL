#pragma once
#include "ceventhandler.h"

/// Addin 라이브러리들을 관리하는 클래스
/// 현재는 스크린샷을 위한 Devil을 관리한다.
class CDBOAddinManager : public RWS::CEventHandler
{
public:
    CDBOAddinManager();
    ~CDBOAddinManager();

    static void Init();
    static void ShutDown();

protected:
    virtual void			HandleEvents(RWS::CMsg &pMsg); 

    void    OnEventScreenShot(RWS::CMsg& pMsg);                 ///< 스크린샷 찍은후에 파일 변경 처리

protected:
    static CDBOAddinManager* m_pInstance;

};