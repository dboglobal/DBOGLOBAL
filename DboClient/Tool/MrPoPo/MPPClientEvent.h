#pragma once

#include "CNtlMrPoPoMsg.h"

/**
 * \ingroup MrPoPo
 * \brief 클라이언트로 보내는 이벤트를 관리하는 클래스
 *
 * \date 2008-08-01
 * \author agebreak
 */
class CMPPClientEvent
{
public:
    CMPPClientEvent(void);
    ~CMPPClientEvent(void);

    BOOL    CheckClientConnect();                           ///< 클라이언트와의 연결을 체크한다.
    void    SendEvent(int nMsgCode, DWORD param);    

protected:
    HWND     m_hDBOClient;                                   ///< 클라이언트 윈도우의 핸들
};

static CMPPClientEvent* GetMPPClientEvent()
{
    static CMPPClientEvent mppClientEvent;
    return &mppClientEvent;
}
    
static DWORD Float2DWORD(CString& str)
{
    float fVal = (float)_wtof(str);
    return *(DWORD*)&fVal;
}